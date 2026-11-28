#ifndef M1B_ARCHIVE_H
#define M1B_ARCHIVE_H

#include <string>

#include <QString>
#include <QByteArray>
#include <QVector>
#include <QHash>

#include "m1A_constants.h"

namespace M1Store{

using namespace M1Env;

/**
 * @brief Archive log - crash recovery (and, later, undo) for the store
 * \ingroup LV1
 *
 * Principle:
 * - at session start (end of StorageStatic::storeSetUp()) a snapshot of all storage files is taken
 *   (archive/snapshot/), the log (archive/log.m1l) is emptied and a dirty marker (archive/dirty) is created
 * - all store updates happen within transactions (Archive::begin()/commit(), or a M1Store::Transaction guard object).
 *   Nested transactions are merged into the outermost one
 * - within a transaction, the Item_lv1 setters (and the string table / special items / util setters) call the touch*() / log*()
 *   hooks, which record the before-image of each modified item on first touch. At commit, the after-images are taken and the
 *   whole transaction is appended to the log as a single frame, followed by fdatasync()
 * - at clean shutdown, the log and the dirty marker are deleted
 * - at startup, if the dirty marker is present (= crash): the snapshot is restored, then all complete transactions
 *   in the log are replayed (after-images, string table operations, counters). An incomplete last transaction is dropped
 *
 * Writes outside of any transaction while logging --> Q_ASSERT in debug builds; in release builds they are wrapped in an
 * implicit transaction, committed at the next event loop iteration (or at the next begin() / shutdown)
 *
 * During CLI imports, logging is suspended (suspend()) and a new snapshot is taken at the end (resume())
 *
 * Log format (native byte order) - a sequence of frames:
 * - begin marker:  'M1BG' u32 | len u32 | seq u64 | label (u32 len + UTF-8)  (written, without sync, at the first write of
 *   a transaction, only to be able to name an incomplete transaction in the recovery report)
 * - transaction:   'M1TX' u32 | len u32 | body | crc32(body) u32 | 'M1CM' u32
 *   body = format version u32 | seq u64 | timestamp ms i64 | label (u32 len + UTF-8) | records ... | END u8
 *   records: COUNTERS (always first), ITEM (id, new flag, before[128], after[128]), SPECIAL (id, before[64], after[64], icon path),
 *   STR_PUT / STR_DEL (id, UTF-8 bytes incl. \0 - for DEL, the deleted value), UTIL_U32 (key, before, after)
 */
class Archive{
public:
    enum class Mode{
        Off,        ///< no logging, no checks (store set-up, version updates, shutdown)
        Logging,    ///< normal operation: writes must happen within transactions and are logged
        Suspended   ///< bulk operations (CLI imports): no logging, a new snapshot is taken at resume()
    };
private:
    static Mode cm_mode;
    static int cm_depth;                        ///< transaction nesting depth
    static bool cm_implicit;                    ///< the current transaction is an implicit one (write outside transaction)
    static bool cm_begin_marker_written;        ///< begin marker of the current transaction already in the log
    static QString cm_label;                    ///< label of the current (outermost) transaction
    static quint64 cm_seq;                      ///< sequence number of the last transaction
    static ItemID cm_next_item_at_begin;        ///< cm_next_item at transaction start (--> items >= are new)

    static QVector<ItemID> cm_item_order;                       ///< items touched in the current transaction (touch order)
    static QHash<ItemID, QByteArray> cm_item_before;            ///< their before-images
    static QVector<SpecialItemID> cm_special_order;             ///< special items touched in the current transaction
    static QHash<SpecialItemID, QByteArray> cm_special_before;  ///< their before-images
    static QByteArray cm_ops;                                   ///< string table / util records, in chronological order

    static int cm_log_fd;                       ///< file descriptor of the open log (-1 if none)
    static bool cm_recovery_pending;            ///< snapshot restored, log to be replayed
    static QString cm_report;                   ///< recovery report (empty if no recovery took place)

    static bool mustLog(const char* p_what);
    static void beginImplicit(const char* p_what);
    static void writeBeginMarker();
    static void writeTransaction();
    static void clearTransaction();
    static void writeFrame(const QByteArray& p_frame, bool p_sync);
    static void takeSnapshot();
    static void openLog(bool p_truncate);
    static void closeLog();
    static void createDirtyMarker();
    static bool applyTransaction(const char* p_body, qsizetype p_len);
public:
    // ------------------------------- session life cycle -------------------------------
    static void wipe();
    static bool crashDetected();
    static void restoreSnapshot();
    static void replayLog();
    static void startSession();
    static void endSession();
    static void suspend();
    static void resume();
    static Mode mode(){return cm_mode;}
    /// recovery report (empty if the previous session ended normally)
    static const QString& recoveryReport(){return cm_report;}

    // ------------------------------- transactions -------------------------------
    static void begin(const QString& p_label);
    static void commit();
    static bool inTransaction(){return cm_depth > 0;}

    // ------------------------------- hooks (called by Item_lv1 / StorageStatic) -------------------------------
    static void touchItem(const ItemID p_item_id);
    static void touchSpecial(const SpecialItemID p_si_id);
    static void logStringPut(const StringID p_string_id, const QByteArray& p_bytes);
    static void logStringDel(const StringID p_string_id, const QByteArray& p_bytes);
    static void logUtilU32(const unsigned int p_key, const quint32 p_before, const quint32 p_after);

    // ------------------------------- helpers -------------------------------
    static void sparseCopy(const std::string& p_src, const std::string& p_dst);
    static void removeMmapFiles(const std::string& p_dir_name);

    // ------------------------------- developer self-test (m1B_archive_selftest.cpp) -------------------------------
    static int selfTest(const QString& p_phase);
};

/**
 * @brief RAII transaction guard: begin() in the constructor, commit() in the destructor
 * \ingroup LV1
 *
 * Usage: { M1Store::Transaction l_tx("Save fields"); ... store updates ... }
 */
class Transaction{
public:
    explicit Transaction(const QString& p_label){Archive::begin(p_label);}
    ~Transaction(){Archive::commit();}
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
};

} // namespace M1Store

#endif // M1B_ARCHIVE_H
