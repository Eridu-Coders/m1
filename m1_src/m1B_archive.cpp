#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QDateTime>
#include <QRegularExpression>
#include <QLoggingCategory>

#include "m1A_env.h"
#include "m1B_store.h"
#include "m1B_archive.h"
#include "m1B_archive_internal.h"

using namespace M1Store::ArchiveInternal;

Q_LOGGING_CATEGORY(g_cat_archive, "store.archive")

// LMDB helper macro (same as in the other store files; requires "int l_rc;")
#define E(expr) CHECK((l_rc = (expr)) == MDB_SUCCESS, #expr)
#define CHECK(test, msg) if( !(test) ) \
{qCCritical(g_cat_archive) << QString("[LMDB errno/msg: %1/%2] - %3").arg(l_rc).arg(mdb_strerror(l_rc), msg); qFatal("Aborting/LMDB");}

/**
 * \defgroup ArchiveImpl Archive log implementation details
 * \ingroup LV1
 * @{
 */
namespace {
    // ------------------------------- paths -------------------------------
    fs::path storeDir(){return fs::path(M1Store::STORE_DATA_PATH);}
    fs::path archiveDir(){return storeDir() / M1Store::ARCHIVE_DIR;}
    fs::path logPath(){return archiveDir() / M1Store::ARCHIVE_LOG_FILE;}
    fs::path dirtyPath(){return archiveDir() / M1Store::ARCHIVE_DIRTY_FILE;}
    fs::path snapshotDir(){return archiveDir() / M1Store::ARCHIVE_SNAPSHOT_DIR;}
    fs::path snapshotTmpDir(){return archiveDir() / (std::string(M1Store::ARCHIVE_SNAPSHOT_DIR) + ".tmp");}
    fs::path snapshotOldDir(){return archiveDir() / (std::string(M1Store::ARCHIVE_SNAPSHOT_DIR) + ".old");}

    /// true for the mmap() storage files: item segments (local_storeXXXX.m1) and specials.m1
    bool isMmapStoreFile(const std::string& p_name){
        // template "local_store%1.m1" --> ^local_store[0-9a-f]{4}\.m1$ (escape each side of %1 separately)
        static const QStringList l_parts = QString(M1Store::LMDB_ITEM_MMAP_FILE_TEMPLATE).split("%1");
        static const QRegularExpression l_re_segment(
            "^" + QRegularExpression::escape(l_parts.value(0)) + "[0-9a-f]{4}" + QRegularExpression::escape(l_parts.value(1)) + "$");
        return p_name == M1Store::LMDB_SPECIAL_MMAP_FILE || l_re_segment.match(QString::fromStdString(p_name)).hasMatch();
    }

    /// fsync a file or directory (errors are logged, not fatal)
    void fsyncPath(const fs::path& p_path, bool p_dir = false){
        int l_fd = open(p_path.c_str(), p_dir ? (O_RDONLY | O_DIRECTORY) : O_RDONLY);
        if(l_fd < 0){
            qCWarning(g_cat_archive) << "fsync open failed" << p_path.c_str() << strerror(errno);
            return;
        }
        if(fsync(l_fd) != 0) qCWarning(g_cat_archive) << "fsync failed" << p_path.c_str() << strerror(errno);
        close(l_fd);
    }

}
/**@}*/ //end of ArchiveImpl

M1Store::Archive::Mode M1Store::Archive::cm_mode = M1Store::Archive::Mode::Off;
int M1Store::Archive::cm_depth = 0;
bool M1Store::Archive::cm_implicit = false;
bool M1Store::Archive::cm_begin_marker_written = false;
QString M1Store::Archive::cm_label;
quint64 M1Store::Archive::cm_seq = 0;
M1Env::ItemID M1Store::Archive::cm_next_item_at_begin = 0;
QVector<M1Env::ItemID> M1Store::Archive::cm_item_order;
QHash<M1Env::ItemID, QByteArray> M1Store::Archive::cm_item_before;
QVector<M1Env::SpecialItemID> M1Store::Archive::cm_special_order;
QHash<M1Env::SpecialItemID, QByteArray> M1Store::Archive::cm_special_before;
QByteArray M1Store::Archive::cm_ops;
int M1Store::Archive::cm_log_fd = -1;
bool M1Store::Archive::cm_recovery_pending = false;
QString M1Store::Archive::cm_report;
M1Env::StringID M1Store::Archive::cm_next_string_at_begin = 0;
M1Env::SpecialItemID M1Store::Archive::cm_next_special_at_begin = 0;
QVector<M1Store::Archive::TxPos> M1Store::Archive::cm_tx_index;
qint64 M1Store::Archive::cm_log_size = 0;

/// signals the UI (Rollback panel) when the set of committed transactions changes
M1Store::ArchiveNotifier* M1Store::Archive::notifier(){
    static ArchiveNotifier* l_notifier = new ArchiveNotifier();
    return l_notifier;
}

// =====================================================================================================================
// helpers
// =====================================================================================================================

/**
 * @brief sparse-aware file copy: only the data extents of p_src are copied (SEEK_DATA / SEEK_HOLE), holes are preserved
 * and the destination gets the same length. The destination is fsync()ed. Any failure is fatal.
 *
 * Needed because the storage files are mostly holes (1 GiB item segments, 16 GiB LMDB map) - a plain copy would
 * materialize them on disk
 *
 * @param p_src source file path
 * @param p_dst destination file path (overwritten)
 */
void M1Store::Archive::sparseCopy(const std::string& p_src, const std::string& p_dst){
    qCDebug(g_cat_archive) << "sparse copy" << p_src.c_str() << "-->" << p_dst.c_str();
    int l_in = open(p_src.c_str(), O_RDONLY);
    if(l_in < 0) qFatal("Archive::sparseCopy() cannot open %s: %s", p_src.c_str(), strerror(errno));
    int l_out = open(p_dst.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if(l_out < 0) qFatal("Archive::sparseCopy() cannot create %s: %s", p_dst.c_str(), strerror(errno));

    struct stat l_st;
    if(fstat(l_in, &l_st) != 0) qFatal("Archive::sparseCopy() fstat failed on %s: %s", p_src.c_str(), strerror(errno));
    const off_t l_size = l_st.st_size;

    static QByteArray l_buf(1 << 20, 0);
    off_t l_pos = 0;
    while(l_pos < l_size){
        off_t l_data = lseek(l_in, l_pos, SEEK_DATA);
        if(l_data < 0){
            if(errno == ENXIO) break; // only a hole is left
            qFatal("Archive::sparseCopy() SEEK_DATA failed on %s: %s", p_src.c_str(), strerror(errno));
        }
        off_t l_hole = lseek(l_in, l_data, SEEK_HOLE);
        if(l_hole < 0) qFatal("Archive::sparseCopy() SEEK_HOLE failed on %s: %s", p_src.c_str(), strerror(errno));
        for(off_t l_off = l_data; l_off < l_hole;){
            ssize_t l_n = pread(l_in, l_buf.data(), std::min<off_t>(l_buf.size(), l_hole - l_off), l_off);
            if(l_n <= 0) qFatal("Archive::sparseCopy() read failed on %s: %s", p_src.c_str(), strerror(errno));
            for(ssize_t l_done = 0; l_done < l_n;){
                ssize_t l_w = pwrite(l_out, l_buf.data() + l_done, l_n - l_done, l_off + l_done);
                if(l_w <= 0) qFatal("Archive::sparseCopy() write failed on %s: %s", p_dst.c_str(), strerror(errno));
                l_done += l_w;
            }
            l_off += l_n;
        }
        l_pos = l_hole;
    }
    if(ftruncate(l_out, l_size) != 0) qFatal("Archive::sparseCopy() ftruncate failed on %s: %s", p_dst.c_str(), strerror(errno));
    if(fsync(l_out) != 0) qFatal("Archive::sparseCopy() fsync failed on %s: %s", p_dst.c_str(), strerror(errno));
    close(l_out);
    close(l_in);
}

/**
 * @brief remove all mmap() storage files (item segments + specials) from a directory
 *
 * Done by name pattern rather than by segment count, so that no stale segment can survive
 * @param p_dir the directory
 */
void M1Store::Archive::removeMmapFiles(const std::string& p_dir_name){
    const fs::path p_dir(p_dir_name);
    if(!fs::is_directory(p_dir)) return;
    std::vector<fs::path> l_to_remove;
    for(const fs::directory_entry& l_entry : fs::directory_iterator(p_dir))
        if(fs::is_regular_file(l_entry.path()) && isMmapStoreFile(l_entry.path().filename().string()))
            l_to_remove.push_back(l_entry.path());
    for(const fs::path& l_path : l_to_remove){
        qCDebug(g_cat_archive) << "removing" << l_path.c_str();
        fs::remove(l_path);
    }
}

// =====================================================================================================================
// session life cycle
// =====================================================================================================================

/**
 * @brief --reset: delete the whole archive (snapshot, log, dirty marker)
 */
void M1Store::Archive::wipe(){
    qCDebug(g_cat_archive) << "wiping archive" << archiveDir().c_str();
    fs::remove_all(archiveDir());
}

/**
 * @brief true if the previous session did not end normally (dirty marker still present)
 */
bool M1Store::Archive::crashDetected(){
    return fs::exists(dirtyPath());
}

/**
 * @brief crash recovery, step 1 (before LMDB and the mmap() areas are opened): replace the storage files by the snapshot
 */
void M1Store::Archive::restoreSnapshot(){
    qCWarning(g_cat_archive) << "Crash detected (dirty marker present) --> restoring the session snapshot";

    // interrupted snapshot rotation (see takeSnapshot()) --> the previous snapshot is still valid
    if(!fs::exists(snapshotDir()) && fs::exists(snapshotOldDir())) fs::rename(snapshotOldDir(), snapshotDir());
    if(!fs::exists(snapshotDir()) || !fs::exists(snapshotDir() / STORE_LMDB_DIR / "data.mdb")){
        cm_report = "A crash was detected but no snapshot was found: the storage files were left as they are "
                    "(changes made just before the crash may be incomplete).";
        qCCritical(g_cat_archive).noquote() << cm_report;
        cm_recovery_pending = false;
        return;
    }

    // LMDB
    const fs::path l_lmdb_dir = storeDir() / STORE_LMDB_DIR;
    fs::remove_all(l_lmdb_dir);
    fs::create_directories(l_lmdb_dir);
    sparseCopy((snapshotDir() / STORE_LMDB_DIR / "data.mdb").string(), (l_lmdb_dir / "data.mdb").string());

    // mmap() files (segments created after the snapshot are removed as well)
    removeMmapFiles(storeDir().string());
    for(const fs::directory_entry& l_entry : fs::directory_iterator(snapshotDir()))
        if(fs::is_regular_file(l_entry.path()) && isMmapStoreFile(l_entry.path().filename().string()))
            sparseCopy(l_entry.path().string(), (storeDir() / l_entry.path().filename()).string());
    fsyncPath(storeDir(), true);
    fsyncPath(l_lmdb_dir, true);

    cm_recovery_pending = true;
}

/**
 * @brief crash recovery, step 2 (LMDB and mmap() areas open, before anything else is done with them):
 * replay all complete transactions of the log. Builds the recovery report.
 */
void M1Store::Archive::replayLog(){
    if(!cm_recovery_pending) return;
    cm_recovery_pending = false;

    QByteArray l_data;
    QFile l_file(QString::fromStdString(logPath().string()));
    if(l_file.exists()){
        if(!l_file.open(QIODevice::ReadOnly)) qFatal("Archive::replayLog() cannot open the log: %s", logPath().c_str());
        l_data = l_file.readAll();
        l_file.close();
    }

    int l_applied = 0;
    bool l_pending = false;         // a begin marker without its transaction frame
    quint64 l_pending_seq = 0;
    QString l_pending_label;
    bool l_garbage = false;

    qsizetype l_pos = 0;
    const qsizetype l_size = l_data.size();
    const char* l_base = l_data.constData();
    while(l_pos + 8 <= l_size){
        quint32 l_magic, l_len;
        memcpy(&l_magic, l_base + l_pos, 4);
        memcpy(&l_len, l_base + l_pos + 4, 4);

        if(l_magic == MAGIC_BEGIN){
            if(l_pos + 8 + (qsizetype)l_len > l_size) break;
            Reader l_r(l_base + l_pos + 8, l_len);
            l_pending_seq = l_r.get<quint64>();
            l_pending_label = QString::fromUtf8(l_r.bytes());
            l_pending = l_r.m_ok;
            l_pos += 8 + l_len;
        }
        else if(l_magic == MAGIC_TX){
            // incomplete (torn) frame --> the transaction was never committed
            if(l_pos + 16 + (qsizetype)l_len > l_size) break;
            const char* l_body = l_base + l_pos + 8;
            quint32 l_crc, l_trailer;
            memcpy(&l_crc, l_body + l_len, 4);
            memcpy(&l_trailer, l_body + l_len + 4, 4);
            if(l_trailer != MAGIC_COMMIT || l_crc != crc32(l_body, l_len)){l_garbage = true; break;}

            Reader l_r(l_body, l_len);
            l_r.get<quint32>();
            quint64 l_seq = l_r.get<quint64>();
            if(!applyTransaction(l_body, l_len)){l_garbage = true; break;}
            l_applied++;
            if(l_pending && l_seq == l_pending_seq) l_pending = false;
            l_pos += 16 + l_len;
        }
        else{l_garbage = true; break;}
    }

    // make the replayed state durable (counters into the util table, LMDB and mmap() areas to disk)
    int l_rc;
    StorageStatic::saveUtilities(nullptr);
    E(mdb_env_sync(StorageStatic::cm_lmdb_env, 1));
    StorageStatic::mmapSync();

    cm_report = QString("The previous session did not end normally. The storage was restored to its state at the start of that "
                        "session and %1 completed transaction(s) were replayed.").arg(l_applied);
    if(l_pending)
        cm_report += QString("\nThe transaction in progress at the time of the crash (\"%1\") was incomplete and has been discarded.")
                         .arg(l_pending_label);
    if(l_garbage)
        cm_report += "\nThe end of the archive log was unreadable and has been ignored.";
    qCWarning(g_cat_archive).noquote() << cm_report;
}

/**
 * @brief apply one (CRC-checked) transaction body to the store
 * @return false if the body could not be parsed
 */
bool M1Store::Archive::applyTransaction(const char* p_body, qsizetype p_len){
    int l_rc;
    ParsedTx l_tx;
    if(!parseTransaction(p_body, p_len, l_tx)) return false;
    cm_seq = l_tx.m_seq;
    qCDebug(g_cat_archive) << "replaying transaction" << cm_seq << l_tx.m_label;

    // counters first: open the segments created during the transaction
    while(StorageStatic::cm_item_mmap_next_seg_id < l_tx.m_next_seg){
        StorageStatic::openSegment(StorageStatic::cm_item_mmap_next_seg_id);
        StorageStatic::cm_item_mmap_next_seg_id += 1;
        StorageStatic::cm_item_next_upper_excluded += StorageStatic::cm_items_per_mmap_segment;
    }
    StorageStatic::cm_next_item = l_tx.m_next_item;
    StorageStatic::cm_next_string = l_tx.m_next_string;
    StorageStatic::cm_next_special = l_tx.m_next_special;

    for(const ParsedItem& l_item : std::as_const(l_tx.m_items)){
        if(l_item.m_id >= StorageStatic::cm_next_item) return false;
        memcpy(static_cast<void*>(StorageStatic::getItemPointer_lv1(l_item.m_id)), l_item.m_after, ITEM_BYTES);
    }
    for(const ParsedSpecial& l_special : std::as_const(l_tx.m_specials)){
        memcpy(static_cast<void*>(StorageStatic::getSpecialItemPointer(l_special.m_id)), l_special.m_after, SPECIAL_BYTES);
        if(StorageStatic::cm_icon_path.size() <= l_special.m_id) StorageStatic::cm_icon_path.resize(l_special.m_id + 1);
        StorageStatic::cm_icon_path[l_special.m_id] = l_special.m_icon_path;
    }

    MDB_txn* l_txn;
    E(mdb_txn_begin(StorageStatic::cm_lmdb_env, NULL, 0, &l_txn));
    for(const ParsedOp& l_op : std::as_const(l_tx.m_ops)){
        if(l_op.m_type == REC_STR_PUT || l_op.m_type == REC_STR_DEL){
            StringID l_id = l_op.m_id;
            MDB_val l_key{sizeof(StringID), &l_id};
            if(l_op.m_type == REC_STR_PUT){
                QByteArray l_bytes = l_op.m_bytes;
                MDB_val l_val{(size_t)l_bytes.size(), l_bytes.data()};
                E(mdb_put(l_txn, StorageStatic::cm_dbi_string, &l_key, &l_val, 0));
            }
            else{
                l_rc = mdb_del(l_txn, StorageStatic::cm_dbi_string, &l_key, NULL);
                CHECK(l_rc == MDB_SUCCESS || l_rc == MDB_NOTFOUND, "mdb_del() (replay)");
            }
        }
        else if(l_op.m_type == REC_UTIL_U32 && l_op.m_key == TEI_EDITOR_FONT_SIZE)
            StorageStatic::cm_tei_editor_font_size = l_op.m_after;
    }
    E(mdb_txn_commit(l_txn));
    return true;
}

/**
 * @brief session start (end of StorageStatic::storeSetUp(), or end of an import): make the current state durable,
 * take a snapshot, empty the log, create the dirty marker and start logging
 */
void M1Store::Archive::startSession(){
    qCDebug(g_cat_archive) << "session start";
    int l_rc;
    StorageStatic::saveUtilities(nullptr);
    E(mdb_env_sync(StorageStatic::cm_lmdb_env, 1));
    StorageStatic::mmapSync();

    fs::create_directories(archiveDir());
    takeSnapshot();
    openLog(true);
    createDirtyMarker();

    cm_seq = 0;
    cm_tx_index.clear();
    cm_mode = Mode::Logging;
    emit notifier()->logChanged();
}

/**
 * @brief clean shutdown (called by StorageStatic::storeShutDown() after saveUtilities()): make everything durable,
 * then delete the log and the dirty marker
 */
void M1Store::Archive::endSession(){
    qCDebug(g_cat_archive) << "session end";
    if(cm_implicit){cm_implicit = false; commit();}
    if(cm_depth > 0){
        qCCritical(g_cat_archive) << "shutdown with an open transaction" << cm_label << "--> committed";
        cm_depth = 1;
        commit();
    }
    cm_mode = Mode::Off;

    int l_rc;
    E(mdb_env_sync(StorageStatic::cm_lmdb_env, 1));
    StorageStatic::mmapSync();

    closeLog();
    cm_tx_index.clear();
    fs::remove(logPath());
    fs::remove(dirtyPath());
    fsyncPath(archiveDir(), true);
}

/**
 * @brief suspend logging (bulk CLI imports). A crash before resume() --> recovery to the state before the import
 */
void M1Store::Archive::suspend(){
    qCDebug(g_cat_archive) << "logging suspended";
    if(cm_implicit){cm_implicit = false; commit();}
    Q_ASSERT_X(cm_depth == 0, "M1Store::Archive::suspend()", "cannot suspend logging within a transaction");
    cm_mode = Mode::Suspended;
}

/**
 * @brief resume logging after suspend(): new snapshot (which includes the imported data) and empty log
 */
void M1Store::Archive::resume(){
    qCDebug(g_cat_archive) << "logging resumed";
    startSession();
}

/**
 * @brief take a snapshot of all storage files into archive/snapshot/ (atomically replacing the previous one)
 *
 * LMDB is copied with mdb_env_copy2(MDB_CP_COMPACT) (consistent, compact), the mmap() files with sparseCopy()
 */
void M1Store::Archive::takeSnapshot(){
    int l_rc;
    const fs::path l_tmp = snapshotTmpDir();
    fs::remove_all(l_tmp);
    fs::create_directories(l_tmp / STORE_LMDB_DIR);

    E(mdb_env_copy2(StorageStatic::cm_lmdb_env, (l_tmp / STORE_LMDB_DIR).c_str(), MDB_CP_COMPACT));
    fsyncPath(l_tmp / STORE_LMDB_DIR / "data.mdb");

    for(const fs::directory_entry& l_entry : fs::directory_iterator(storeDir()))
        if(fs::is_regular_file(l_entry.path()) && isMmapStoreFile(l_entry.path().filename().string()))
            sparseCopy(l_entry.path().string(), (l_tmp / l_entry.path().filename()).string());
    fsyncPath(l_tmp / STORE_LMDB_DIR, true);
    fsyncPath(l_tmp, true);

    // rotation: snapshot --> snapshot.old, snapshot.tmp --> snapshot, delete snapshot.old
    // (if interrupted between the 2 renames, restoreSnapshot() falls back on snapshot.old)
    fs::remove_all(snapshotOldDir());
    if(fs::exists(snapshotDir())) fs::rename(snapshotDir(), snapshotOldDir());
    fs::rename(l_tmp, snapshotDir());
    fsyncPath(archiveDir(), true);
    fs::remove_all(snapshotOldDir());
    qCDebug(g_cat_archive) << "snapshot taken";
}

void M1Store::Archive::openLog(bool p_truncate){
    closeLog();
    cm_log_fd = open(logPath().c_str(), O_WRONLY | O_CREAT | O_APPEND | (p_truncate ? O_TRUNC : 0), 0664);
    if(cm_log_fd < 0) qFatal("Archive::openLog() cannot open %s: %s", logPath().c_str(), strerror(errno));
    struct stat l_st;
    cm_log_size = (fstat(cm_log_fd, &l_st) == 0) ? l_st.st_size : 0;
    if(p_truncate && fsync(cm_log_fd) != 0) qCWarning(g_cat_archive) << "log fsync failed" << strerror(errno);
    fsyncPath(archiveDir(), true);
}

void M1Store::Archive::closeLog(){
    if(cm_log_fd >= 0) close(cm_log_fd);
    cm_log_fd = -1;
}

void M1Store::Archive::createDirtyMarker(){
    int l_fd = open(dirtyPath().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if(l_fd < 0) qFatal("Archive::createDirtyMarker() cannot create %s: %s", dirtyPath().c_str(), strerror(errno));
    QByteArray l_stamp = QDateTime::currentDateTime().toString(Qt::ISODate).toUtf8() + "\n";
    if(write(l_fd, l_stamp.constData(), l_stamp.size()) < 0) qCWarning(g_cat_archive) << "dirty marker write failed";
    fsync(l_fd);
    close(l_fd);
    fsyncPath(archiveDir(), true);
}

// =====================================================================================================================
// transactions
// =====================================================================================================================

/**
 * @brief begin a transaction (nested calls are merged into the outermost transaction)
 * @param p_label description of the transaction (recovery report, future undo menu)
 */
void M1Store::Archive::begin(const QString& p_label){
    // a pending implicit transaction is closed before any explicit one
    if(cm_implicit){cm_implicit = false; commit();}
    if(cm_depth++ == 0){
        clearTransaction();
        cm_label = p_label;
        cm_next_item_at_begin = StorageStatic::cm_next_item;
        cm_next_string_at_begin = StorageStatic::cm_next_string;
        cm_next_special_at_begin = StorageStatic::cm_next_special;
    }
}

/**
 * @brief end of a transaction. At the outermost level, the transaction is written to the log and synced
 */
void M1Store::Archive::commit(){
    if(cm_depth <= 0){
        qCCritical(g_cat_archive) << "commit() without begin()";
        Q_ASSERT_X(false, "M1Store::Archive::commit()", "commit() without begin()");
        return;
    }
    if(--cm_depth > 0) return;
    if(cm_mode == Mode::Logging) writeTransaction();
    clearTransaction();
}

void M1Store::Archive::clearTransaction(){
    cm_item_order.clear();
    cm_item_before.clear();
    cm_special_order.clear();
    cm_special_before.clear();
    cm_ops.clear();
    cm_label.clear();
    cm_begin_marker_written = false;
}

/**
 * @brief check before recording a write: logging active? within a transaction?
 * @param p_what description of the write (for the error message / implicit transaction label)
 * @return true if the write must be recorded
 */
bool M1Store::Archive::mustLog(const char* p_what){
    if(cm_mode != Mode::Logging) return false;
    if(cm_depth == 0){
        qCCritical(g_cat_archive) << "store write outside of a transaction:" << p_what;
        Q_ASSERT_X(false, "M1Store::Archive", "store write outside of a transaction - wrap the action in a M1Store::Transaction");
        beginImplicit(p_what);
    }
    if(!cm_begin_marker_written){
        writeBeginMarker();
        cm_begin_marker_written = true;
    }
    return true;
}

/**
 * @brief (release builds) open an implicit transaction for a write made outside of any transaction. It is committed at the
 * next event loop iteration, at the next begin() or at shutdown, whichever comes first
 */
void M1Store::Archive::beginImplicit(const char* p_what){
    begin(QString("(implicit) %1").arg(p_what));
    cm_implicit = true;
    if(QCoreApplication::instance() != nullptr)
        QTimer::singleShot(0, QCoreApplication::instance(), [](){
            if(cm_implicit){cm_implicit = false; commit();}
        });
}

/// append a frame to the log (fdatasync if p_sync). A write failure is fatal (crash protection would be lost)
void M1Store::Archive::writeFrame(const QByteArray& p_frame, bool p_sync){
    if(cm_log_fd < 0) qFatal("Archive::writeFrame() log not open");
    for(qsizetype l_done = 0; l_done < p_frame.size();){
        ssize_t l_n = write(cm_log_fd, p_frame.constData() + l_done, p_frame.size() - l_done);
        if(l_n < 0){
            if(errno == EINTR) continue;
            qFatal("Archive::writeFrame() write failed: %s", strerror(errno));
        }
        l_done += l_n;
    }
    cm_log_size += p_frame.size();
    if(p_sync && fdatasync(cm_log_fd) != 0) qFatal("Archive::writeFrame() fdatasync failed: %s", strerror(errno));
}

/// begin marker frame (no sync: it only serves to name an incomplete transaction in the recovery report)
void M1Store::Archive::writeBeginMarker(){
    QByteArray l_body;
    put<quint64>(l_body, cm_seq + 1);
    putBytes(l_body, cm_label.toUtf8());
    QByteArray l_frame;
    put<quint32>(l_frame, MAGIC_BEGIN);
    put<quint32>(l_frame, l_body.size());
    l_frame.append(l_body);
    writeFrame(l_frame, false);
}

/**
 * @brief serialize the current transaction (before-images recorded at touch time, after-images taken now) and append it
 * to the log, then fdatasync()
 */
void M1Store::Archive::writeTransaction(){
    if(cm_item_order.isEmpty() && cm_special_order.isEmpty() && cm_ops.isEmpty()) return;

    QByteArray l_body;
    put<quint32>(l_body, FORMAT_VERSION);
    put<quint64>(l_body, ++cm_seq);
    put<qint64>(l_body, QDateTime::currentMSecsSinceEpoch());
    putBytes(l_body, cm_label.toUtf8());

    // counters first (replay opens the new segments before applying the item images)
    put<quint8>(l_body, REC_COUNTERS);
    put<quint64>(l_body, StorageStatic::cm_next_item);
    put<quint64>(l_body, StorageStatic::cm_next_string);
    put<quint16>(l_body, StorageStatic::cm_next_special);
    put<quint32>(l_body, StorageStatic::cm_item_mmap_next_seg_id);
    // (format 2) values at transaction start, for undo
    put<quint64>(l_body, cm_next_item_at_begin);
    put<quint64>(l_body, cm_next_string_at_begin);
    put<quint16>(l_body, cm_next_special_at_begin);

    for(const ItemID l_id : std::as_const(cm_item_order)){
        put<quint8>(l_body, REC_ITEM);
        put<quint64>(l_body, l_id);
        put<quint8>(l_body, l_id >= cm_next_item_at_begin ? 1 : 0);
        l_body.append(cm_item_before.value(l_id));
        l_body.append(reinterpret_cast<const char*>(StorageStatic::getItemPointer_lv1(l_id)), ITEM_BYTES);
    }
    for(const SpecialItemID l_si_id : std::as_const(cm_special_order)){
        put<quint8>(l_body, REC_SPECIAL);
        put<quint16>(l_body, l_si_id);
        l_body.append(cm_special_before.value(l_si_id));
        l_body.append(reinterpret_cast<const char*>(StorageStatic::getSpecialItemPointer(l_si_id)), SPECIAL_BYTES);
        putBytes(l_body, l_si_id < StorageStatic::cm_icon_path.size() ? StorageStatic::cm_icon_path[l_si_id].toUtf8() : QByteArray());
    }
    l_body.append(cm_ops);
    put<quint8>(l_body, REC_END);

    QByteArray l_frame;
    l_frame.reserve(l_body.size() + 16);
    put<quint32>(l_frame, MAGIC_TX);
    put<quint32>(l_frame, l_body.size());
    l_frame.append(l_body);
    put<quint32>(l_frame, crc32(l_body.constData(), l_body.size()));
    put<quint32>(l_frame, MAGIC_COMMIT);

    // index entry: a rollback of this transaction truncates the log at the end of the previous one
    // (which also drops the begin marker frame written for this one)
    TxPos l_pos;
    l_pos.m_seq = cm_seq;
    l_pos.m_truncate_at = cm_tx_index.isEmpty() ? 0 : cm_tx_index.last().m_end;
    l_pos.m_frame_offset = cm_log_size;
    l_pos.m_body_len = l_body.size();
    writeFrame(l_frame, true);
    l_pos.m_end = cm_log_size;
    cm_tx_index.append(l_pos);

    qCDebug(g_cat_archive) << "transaction" << cm_seq << cm_label << "committed:" << cm_item_order.size() << "item(s),"
                           << cm_special_order.size() << "special(s)," << l_frame.size() << "bytes";
    emit notifier()->logChanged();
}

// =====================================================================================================================
// hooks
// =====================================================================================================================

/**
 * @brief record the before-image of an item about to be modified (first touch in the transaction only)
 * @param p_item_id the item (must already be allocated, i.e. < cm_next_item)
 */
void M1Store::Archive::touchItem(const ItemID p_item_id){
    if(!mustLog("item update")) return;
    if(cm_item_before.contains(p_item_id)) return;
    cm_item_before.insert(p_item_id, QByteArray(reinterpret_cast<const char*>(StorageStatic::getItemPointer_lv1(p_item_id)), ITEM_BYTES));
    cm_item_order.append(p_item_id);
}

/**
 * @brief record the before-image of a special item about to be modified (first touch in the transaction only)
 */
void M1Store::Archive::touchSpecial(const SpecialItemID p_si_id){
    if(!mustLog("special item update")) return;
    if(cm_special_before.contains(p_si_id)) return;
    cm_special_before.insert(p_si_id, QByteArray(reinterpret_cast<const char*>(StorageStatic::getSpecialItemPointer(p_si_id)), SPECIAL_BYTES));
    cm_special_order.append(p_si_id);
}

/// a string is stored in the string table (p_bytes: UTF-8 incl. \0, as stored)
void M1Store::Archive::logStringPut(const StringID p_string_id, const QByteArray& p_bytes){
    if(!mustLog("string store")) return;
    put<quint8>(cm_ops, REC_STR_PUT);
    put<quint64>(cm_ops, p_string_id);
    putBytes(cm_ops, p_bytes);
}

/// a string is deleted from the string table (p_bytes: its value, for undo)
void M1Store::Archive::logStringDel(const StringID p_string_id, const QByteArray& p_bytes){
    if(!mustLog("string free")) return;
    put<quint8>(cm_ops, REC_STR_DEL);
    put<quint64>(cm_ops, p_string_id);
    putBytes(cm_ops, p_bytes);
}

/// a 32-bit value of the util table is changed
void M1Store::Archive::logUtilU32(const unsigned int p_key, const quint32 p_before, const quint32 p_after){
    if(!mustLog("util value")) return;
    put<quint8>(cm_ops, REC_UTIL_U32);
    put<quint32>(cm_ops, p_key);
    put<quint32>(cm_ops, p_before);
    put<quint32>(cm_ops, p_after);
}

// =====================================================================================================================
// rollback (undo)
// =====================================================================================================================

/// true if the item exists in the store (allocated, i.e. not beyond the counter - e.g. after a rollback)
bool M1Store::Archive::itemAlive(const ItemID p_item_id){
    return p_item_id != G_VOID_ITEM_ID && p_item_id < StorageStatic::cm_next_item;
}

/// true if p_vertex (a pointer held by the UI) still designates an existing full vertex (it may have been rolled back)
bool M1Store::Archive::vertexAlive(const Item_lv0* p_vertex){
    if(p_vertex == nullptr) return false;
    const ItemID l_id = p_vertex->item_id();
    return itemAlive(l_id) &&
           static_cast<const void*>(StorageStatic::getItemPointer_lv1(l_id)) == static_cast<const void*>(p_vertex) &&
           (p_vertex->flags() & ITEM_NATURE_MASK) == FULL_VERTEX;
}

/// true if a rollback is possible now (logging active, no explicit transaction open)
bool M1Store::Archive::canRollback(){
    return cm_mode == Mode::Logging && (cm_depth == 0 || cm_implicit);
}

/**
 * @brief read the body of a committed transaction back from the log (CRC-checked)
 * @return the body, empty if it could not be read
 */
QByteArray M1Store::Archive::readBody(const TxPos& p_pos){
    QByteArray l_frame(p_pos.m_body_len + 16, 0);
    int l_fd = open(logPath().c_str(), O_RDONLY);
    if(l_fd < 0){
        qCCritical(g_cat_archive) << "cannot open the log for reading" << strerror(errno);
        return QByteArray();
    }
    qsizetype l_done = 0;
    while(l_done < l_frame.size()){
        ssize_t l_n = pread(l_fd, l_frame.data() + l_done, l_frame.size() - l_done, p_pos.m_frame_offset + l_done);
        if(l_n < 0 && errno == EINTR) continue;
        if(l_n <= 0) break;
        l_done += l_n;
    }
    close(l_fd);
    if(l_done < l_frame.size()) return QByteArray();

    quint32 l_magic, l_len, l_crc, l_trailer;
    memcpy(&l_magic, l_frame.constData(), 4);
    memcpy(&l_len, l_frame.constData() + 4, 4);
    if(l_magic != MAGIC_TX || l_len != p_pos.m_body_len) return QByteArray();
    const char* l_body = l_frame.constData() + 8;
    memcpy(&l_crc, l_body + l_len, 4);
    memcpy(&l_trailer, l_body + l_len + 4, 4);
    if(l_trailer != MAGIC_COMMIT || l_crc != crc32(l_body, l_len)) return QByteArray();
    return QByteArray(l_body, l_len);
}

/// number of transactions committed (and still in the log) in the current session
int M1Store::Archive::transactionCount(){return cm_tx_index.size();}

/// sequence number of the transaction at p_index (0 = oldest)
quint64 M1Store::Archive::transactionSeq(int p_index){
    return (p_index >= 0 && p_index < cm_tx_index.size()) ? cm_tx_index[p_index].m_seq : 0;
}

/**
 * @brief undo the p_count most recent transactions of the current session
 *
 * Newest first, each transaction is reversed: string table operations in reverse order (STR_PUT --> delete,
 * STR_DEL --> put the old value back), util values set back, item and special images replaced by their before-images,
 * counters set back to their values at transaction start. Then the log is truncated at the end of the last remaining
 * transaction (fdatasync).
 *
 * Crash safety: the rollback writes go to the mmap() areas / LMDB (not synced), the truncation comes last. A crash
 * before the truncation --> recovery = snapshot + whole log = state before the rollback; after --> snapshot + truncated
 * log = state after the rollback
 *
 * @param p_count number of transactions to undo (most recent ones)
 * @return empty string if OK, otherwise an error message (nothing has been changed in that case)
 */
QString M1Store::Archive::rollback(int p_count){
    if(cm_implicit){cm_implicit = false; commit();}
    if(cm_mode != Mode::Logging) return "The archive log is not active (rollback impossible).";
    if(cm_depth != 0) return "A transaction is in progress (rollback impossible).";
    if(p_count <= 0) return QString();
    if(p_count > cm_tx_index.size()) return "Not that many transactions in the log.";

    // read and parse everything first: nothing is changed if any transaction cannot be read
    const int l_first = cm_tx_index.size() - p_count;
    QVector<QByteArray> l_bodies;
    QVector<ParsedTx> l_txs;
    for(int i = cm_tx_index.size() - 1; i >= l_first; i--){
        QByteArray l_body = readBody(cm_tx_index[i]);
        ParsedTx l_tx;
        if(l_body.isEmpty() || !parseTransaction(l_body.constData(), l_body.size(), l_tx))
            return QString("Transaction #%1 cannot be read back from the log (rollback cancelled).").arg(cm_tx_index[i].m_seq);
        if(!l_tx.m_has_before_counters)
            return QString("Transaction #%1 was written in an old log format (rollback cancelled).").arg(cm_tx_index[i].m_seq);
        l_bodies.append(l_body);    // keeps the buffers alive (ParsedTx points into them)
        l_txs.append(l_tx);
    }

    int l_rc;
    bool l_specials_touched = false;
    MDB_txn* l_txn;
    E(mdb_txn_begin(StorageStatic::cm_lmdb_env, NULL, 0, &l_txn));
    for(const ParsedTx& l_tx : std::as_const(l_txs)){
        qCDebug(g_cat_archive) << "rolling back transaction" << l_tx.m_seq << l_tx.m_label;
        // string table / util operations, in reverse order
        for(qsizetype i = l_tx.m_ops.size() - 1; i >= 0; i--){
            const ParsedOp& l_op = l_tx.m_ops[i];
            if(l_op.m_type == REC_STR_PUT || l_op.m_type == REC_STR_DEL){
                StringID l_id = l_op.m_id;
                MDB_val l_key{sizeof(StringID), &l_id};
                if(l_op.m_type == REC_STR_PUT){
                    l_rc = mdb_del(l_txn, StorageStatic::cm_dbi_string, &l_key, NULL);
                    CHECK(l_rc == MDB_SUCCESS || l_rc == MDB_NOTFOUND, "mdb_del() (rollback)");
                }
                else{
                    QByteArray l_bytes = l_op.m_bytes;
                    MDB_val l_val{(size_t)l_bytes.size(), l_bytes.data()};
                    E(mdb_put(l_txn, StorageStatic::cm_dbi_string, &l_key, &l_val, 0));
                }
            }
            else if(l_op.m_type == REC_UTIL_U32 && l_op.m_key == TEI_EDITOR_FONT_SIZE)
                StorageStatic::cm_tei_editor_font_size = l_op.m_before;
        }
        // item and special before-images (counters still at their "after" values --> all items addressable)
        for(const ParsedItem& l_item : std::as_const(l_tx.m_items))
            memcpy(static_cast<void*>(StorageStatic::getItemPointer_lv1(l_item.m_id)), l_item.m_before, ITEM_BYTES);
        for(const ParsedSpecial& l_special : std::as_const(l_tx.m_specials)){
            memcpy(static_cast<void*>(StorageStatic::getSpecialItemPointer(l_special.m_id)), l_special.m_before, SPECIAL_BYTES);
            l_specials_touched = true;
        }
        // counters (segments created by the transaction stay open: they are simply empty)
        StorageStatic::cm_next_item = l_tx.m_next_item_before;
        StorageStatic::cm_next_string = l_tx.m_next_string_before;
        StorageStatic::cm_next_special = l_tx.m_next_special_before;
    }
    E(mdb_txn_commit(l_txn));

    // special items: rebuild the lookup maps, drop the icons of the removed ones
    if(l_specials_touched){
        StorageStatic::cm_mnemonic_to_special.clear();
        StorageStatic::cm_item_id_to_special.clear();
        for(SpecialItemID i = 0; i < StorageStatic::cm_next_special; i++){
            SpecialItem* l_slot = StorageStatic::getSpecialItemPointer(i);
            StorageStatic::cm_mnemonic_to_special[l_slot->mnemonic()] = l_slot;
            if(l_slot->itemId() != G_VOID_ITEM_ID) StorageStatic::cm_item_id_to_special[l_slot->itemId()] = l_slot;
        }
        if(StorageStatic::cm_icon_path.size() > StorageStatic::cm_next_special)
            StorageStatic::cm_icon_path.resize(StorageStatic::cm_next_special);
        while(StorageStatic::cm_type_icon.size() > StorageStatic::cm_next_special){
            delete StorageStatic::cm_type_icon.last();
            StorageStatic::cm_type_icon.removeLast();
        }
    }

    // truncate the log (last step, see above). cm_seq is NOT decreased: sequence numbers stay unique within the session
    const qint64 l_truncate_at = cm_tx_index[l_first].m_truncate_at;
    if(ftruncate(cm_log_fd, l_truncate_at) != 0) qFatal("Archive::rollback() ftruncate failed: %s", strerror(errno));
    if(fdatasync(cm_log_fd) != 0) qFatal("Archive::rollback() fdatasync failed: %s", strerror(errno));
    cm_log_size = l_truncate_at;
    cm_tx_index.resize(l_first);

    qCDebug(g_cat_archive) << p_count << "transaction(s) rolled back, log truncated at" << l_truncate_at;
    emit notifier()->logChanged();
    return QString();
}
