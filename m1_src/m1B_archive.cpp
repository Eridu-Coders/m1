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
    const quint32 MAGIC_BEGIN  = 0x4742314D;    ///< 'M1BG' (little endian)
    const quint32 MAGIC_TX     = 0x5854314D;    ///< 'M1TX'
    const quint32 MAGIC_COMMIT = 0x4D43314D;    ///< 'M1CM'
    const quint32 FORMAT_VERSION = 1;

    // record types
    const quint8 REC_END      = 0xFF;
    const quint8 REC_COUNTERS = 1;
    const quint8 REC_ITEM     = 2;
    const quint8 REC_SPECIAL  = 3;
    const quint8 REC_STR_PUT  = 4;
    const quint8 REC_STR_DEL  = 5;
    const quint8 REC_UTIL_U32 = 6;

    const qsizetype ITEM_BYTES = sizeof(M1Store::Item_lv0);
    const qsizetype SPECIAL_BYTES = sizeof(M1Store::SpecialItem);

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

    // ------------------------------- serialization helpers -------------------------------
    template<typename T> void put(QByteArray& p_buf, const T p_val){p_buf.append(reinterpret_cast<const char*>(&p_val), sizeof(T));}
    void putBytes(QByteArray& p_buf, const QByteArray& p_data){put<quint32>(p_buf, p_data.size()); p_buf.append(p_data);}

    /// bounds-checked reader over a byte buffer
    struct Reader{
        const char* m_data;
        qsizetype m_len;
        qsizetype m_pos = 0;
        bool m_ok = true;
        Reader(const char* p_data, qsizetype p_len) : m_data(p_data), m_len(p_len){}
        template<typename T> T get(){
            T l_val{};
            if(!m_ok || m_pos + (qsizetype)sizeof(T) > m_len){m_ok = false; return l_val;}
            memcpy(&l_val, m_data + m_pos, sizeof(T));
            m_pos += sizeof(T);
            return l_val;
        }
        const char* raw(qsizetype p_n){
            if(!m_ok || p_n < 0 || m_pos + p_n > m_len){m_ok = false; return nullptr;}
            const char* l_ret = m_data + m_pos;
            m_pos += p_n;
            return l_ret;
        }
        QByteArray bytes(){
            quint32 l_n = get<quint32>();
            const char* l_p = raw(l_n);
            return l_p == nullptr ? QByteArray() : QByteArray(l_p, l_n);
        }
    };

    /// standard CRC-32 (IEEE 802.3, reflected, poly 0xEDB88320)
    quint32 crc32(const char* p_data, qsizetype p_len){
        static quint32 l_table[256];
        static bool l_init = false;
        if(!l_init){
            for(quint32 i = 0; i < 256; i++){
                quint32 c = i;
                for(int k = 0; k < 8; k++) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
                l_table[i] = c;
            }
            l_init = true;
        }
        quint32 l_crc = 0xFFFFFFFFu;
        for(qsizetype i = 0; i < p_len; i++)
            l_crc = l_table[(l_crc ^ static_cast<quint8>(p_data[i])) & 0xFF] ^ (l_crc >> 8);
        return l_crc ^ 0xFFFFFFFFu;
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
    Reader l_r(p_body, p_len);
    if(l_r.get<quint32>() != FORMAT_VERSION) return false;
    cm_seq = l_r.get<quint64>();
    l_r.get<qint64>();
    QString l_label = QString::fromUtf8(l_r.bytes());
    qCDebug(g_cat_archive) << "replaying transaction" << cm_seq << l_label;

    MDB_txn* l_txn;
    E(mdb_txn_begin(StorageStatic::cm_lmdb_env, NULL, 0, &l_txn));

    bool l_end = false;
    while(l_r.m_ok && !l_end){
        const quint8 l_type = l_r.get<quint8>();
        switch(l_type){
        case REC_END:
            l_end = true;
            break;
        case REC_COUNTERS:{
            ItemID l_next_item = l_r.get<quint64>();
            StringID l_next_string = l_r.get<quint64>();
            SpecialItemID l_next_special = l_r.get<quint16>();
            quint32 l_next_seg = l_r.get<quint32>();
            if(!l_r.m_ok) break;
            // open the segments created during the transaction
            while(StorageStatic::cm_item_mmap_next_seg_id < l_next_seg){
                StorageStatic::openSegment(StorageStatic::cm_item_mmap_next_seg_id);
                StorageStatic::cm_item_mmap_next_seg_id += 1;
                StorageStatic::cm_item_next_upper_excluded += StorageStatic::cm_items_per_mmap_segment;
            }
            StorageStatic::cm_next_item = l_next_item;
            StorageStatic::cm_next_string = l_next_string;
            StorageStatic::cm_next_special = l_next_special;
            break;
        }
        case REC_ITEM:{
            ItemID l_id = l_r.get<quint64>();
            l_r.get<quint8>();          // new flag (for undo)
            l_r.raw(ITEM_BYTES);        // before-image (for undo)
            const char* l_after = l_r.raw(ITEM_BYTES);
            if(!l_r.m_ok || l_id >= StorageStatic::cm_next_item){l_r.m_ok = false; break;}
            memcpy(static_cast<void*>(StorageStatic::getItemPointer_lv1(l_id)), l_after, ITEM_BYTES);
            break;
        }
        case REC_SPECIAL:{
            SpecialItemID l_si_id = l_r.get<quint16>();
            l_r.raw(SPECIAL_BYTES);     // before-image (for undo)
            const char* l_after = l_r.raw(SPECIAL_BYTES);
            QString l_icon_path = QString::fromUtf8(l_r.bytes());
            if(!l_r.m_ok) break;
            memcpy(static_cast<void*>(StorageStatic::getSpecialItemPointer(l_si_id)), l_after, SPECIAL_BYTES);
            if(StorageStatic::cm_icon_path.size() <= l_si_id) StorageStatic::cm_icon_path.resize(l_si_id + 1);
            StorageStatic::cm_icon_path[l_si_id] = l_icon_path;
            break;
        }
        case REC_STR_PUT:
        case REC_STR_DEL:{
            StringID l_id = l_r.get<quint64>();
            QByteArray l_bytes = l_r.bytes();
            if(!l_r.m_ok) break;
            MDB_val l_key{sizeof(StringID), &l_id};
            if(l_type == REC_STR_PUT){
                MDB_val l_val{(size_t)l_bytes.size(), l_bytes.data()};
                E(mdb_put(l_txn, StorageStatic::cm_dbi_string, &l_key, &l_val, 0));
            }
            else{
                l_rc = mdb_del(l_txn, StorageStatic::cm_dbi_string, &l_key, NULL);
                CHECK(l_rc == MDB_SUCCESS || l_rc == MDB_NOTFOUND, "mdb_del() (replay)");
            }
            break;
        }
        case REC_UTIL_U32:{
            quint32 l_key = l_r.get<quint32>();
            l_r.get<quint32>();
            quint32 l_after = l_r.get<quint32>();
            if(!l_r.m_ok) break;
            if(l_key == TEI_EDITOR_FONT_SIZE) StorageStatic::cm_tei_editor_font_size = l_after;
            break;
        }
        default:
            l_r.m_ok = false;
        }
    }

    E(mdb_txn_commit(l_txn));
    return l_r.m_ok && l_end;
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
    cm_mode = Mode::Logging;
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
    writeFrame(l_frame, true);

    qCDebug(g_cat_archive) << "transaction" << cm_seq << cm_label << "committed:" << cm_item_order.size() << "item(s),"
                           << cm_special_order.size() << "special(s)," << l_frame.size() << "bytes";
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
