#ifndef M1_STORE_H
#define M1_STORE_H

#include <lmdb.h>

#include <QDateTime>
#include <QLoggingCategory>

#include "m1_lv0_item.h"

Q_DECLARE_LOGGING_CATEGORY(g_cat_store)

namespace M1Store{

    // ----------------------------------------- Storage ----------------------------------------------------
    class Storage{
    private:
        static const char* STORE_DATA_PATH;     ///< Path to the data store
        static const char* STORE_LMDB_DIR;      ///< Subdir for LMDB store (strings + utilities)
        static const char* LMDB_UTIL_DB;        ///< name of the table for util data
        static const char* LMDB_STRING_DB;      ///< string table

        static MDB_env* cm_lmdb_env;            ///< LMDB env
        static MDB_dbi cm_dbi_util;             ///< util table handle
        static MDB_dbi cm_dbi_string;           ///< string table handle

        static ItemID cm_next_item;             ///< next ItemID counter (stored in util table)
        static ItemID cm_next_string;           ///< next StringID counter (stored in util table)
    public:
        static void init();
        static void close();
        static void loadCounters(MDB_txn* p_txn);
        static void saveCounters(MDB_txn* p_txn);
        static ItemID storeString(QString p_string);
        static char* retrieveString(ItemID p_string_id);
        static void freeString(ItemID p_string_id);

        static StringID dbg_get_string_id(Item_lv0& p_item) {return p_item.p.v.f.m_string_id;}
    };

} // end namespace M1Store

#endif // M1_STORE_H
