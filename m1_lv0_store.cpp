#include <QtGlobal>
#include <QDebug>

#include <stdio.h>
#include <string.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include "m1_lv0_store.h"

Q_LOGGING_CATEGORY(g_cat_storage, "store.storage")

// LMDB helper macros taken from the examples in the distrib.
#define E(expr) CHECK((l_rc = (expr)) == MDB_SUCCESS, #expr)
#define CHECK(test, msg) if( !(test) ) \
    {qCCritical(g_cat_store) << QString("[LMDB errno/msg: %1/%2] - %3").arg(l_rc).arg(mdb_strerror(l_rc), msg); qFatal("Aborting");}


// class-level constants holding the names of various things
// root dire for data storage
const char* M1Store::Storage::STORE_DATA_PATH = "/home/fi11222/disk-share/Dev/m1/Storage";
const char* M1Store::Storage::STORE_LMDB_DIR = "lmdb";      // subdir for LMDB data
const char* M1Store::Storage::LMDB_UTIL_DB = "util_db";     // name of the utilities table
const char* M1Store::Storage::LMDB_STRING_DB = "string_db"; // name of the strings table

// class members holding the LMDB session data
MDB_env* M1Store::Storage::cm_lmdb_env = NULL;  // LMDB environment
MDB_dbi M1Store::Storage::cm_dbi_util;          // Utilities table handle
MDB_dbi M1Store::Storage::cm_dbi_string;        // string table handle

// class members holding counters for next IDs for items and string table (current value saved in utilities table between sessions)
M1Store::ItemID M1Store::Storage::cm_next_item{0};
M1Store::ItemID M1Store::Storage::cm_next_string{0};
// simplified namespace to access boost filesystem library functions
namespace fs = boost::filesystem;

// store a new string into the string table and return the corresponding newly created string ID
/**
 * @brief M1Store::Storage::storeString store a new string into the string table and return the corresponding newly created string ID
 * @param p_string the string (QString)
 * @return the StringID of the stored string
 */
M1Store::ItemID M1Store::Storage::storeString(QString p_string){
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::storeString()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros E and CHECK)
    int l_rc;

    MDB_val l_key, l_data;

    // attribute next String ID from the counter
    ItemID l_ret_id = cm_next_string++;

    // designate it as the LMDB key
    l_key.mv_size = sizeof(ItemID);
    l_key.mv_data = &l_ret_id;

    // get UTF8 version of string to store as a char* sequence
    QByteArray l_utf8_bytes = p_string.toUtf8();
    l_data.mv_size = l_utf8_bytes.length() + 1; // +1 so that the null termination character also stored
    l_data.mv_data = l_utf8_bytes.data();

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(cm_lmdb_env, NULL, 0, &l_txn));

    // storing next string
    E(mdb_put(l_txn, cm_dbi_string, &l_key, &l_data, 0));
    qCDebug(g_cat_store) << QString("Stored string: 0x%1 --> [%2] Utf8 len = %3")
                                .arg(l_ret_id, 8, 16, QChar('0')).arg(p_string).arg(l_utf8_bytes.length());

    // commit transaction
    E(mdb_txn_commit(l_txn));

    return l_ret_id;
}

/**
 * @brief M1Store::Storage::retrieveString retrieve a string from LMDB storage
 * @param p_string_id the StringID of the requested string
 * @return the string, as a (char *)
 */
char* M1Store::Storage::retrieveString(ItemID p_string_id){
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::retrieveString()", "LMDB env must be initialized");

    // don't even try if p_string_id == G_VOID_ID
    static char l_empty_string[1] = "";
    if(p_string_id == G_VOID_ID) return l_empty_string;

    // LMDB return code (for use with return code test macros)
    int l_rc;

    // return value buffer
    static char *l_buf = NULL;

    MDB_val l_key, l_data;

    l_key.mv_size = sizeof(ItemID);
    l_key.mv_data = &p_string_id;

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(M1Store::Storage::cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

    // retrieve string and put char* pointer into l_data.mv_data
    l_rc = mdb_get(l_txn, cm_dbi_string, &l_key, &l_data);
    qCDebug(g_cat_storage) << ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(p_string_id, 8, 16, QChar('0')):
                                 QString("retrieved: 0x%1 --> %2 [len = %3]").arg(p_string_id, 8, 16, QChar('0')).arg((char *)(l_data.mv_data)).arg(l_data.mv_size));

    // store string in buffer
    if(l_buf != NULL) free(l_buf); // free previous buffer if not NULL
    l_buf = (char *)malloc(l_data.mv_size + 1); // allocate new buffer of the appropriate size
    strncpy(l_buf, (char *)(l_data.mv_data), l_data.mv_size);
    l_buf[l_data.mv_size] = 0;

    // commit transaction
    E(mdb_txn_commit(l_txn));

    //return (char *)(l_data.mv_data);
    return l_buf;
}

/**
 * @brief M1Store::Storage::freeString Free a string from the LMDB store
 * @param p_string_id the StringID of the string
 */
void M1Store::Storage::freeString(ItemID p_string_id){
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::freeString()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros)
    int l_rc;

    MDB_val l_key;
    l_key.mv_size = sizeof(ItemID);
    l_key.mv_data = &p_string_id;

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(M1Store::Storage::cm_lmdb_env, NULL, 0, &l_txn));

    // delete the key from the store
    E(mdb_del(l_txn, cm_dbi_string, &l_key, NULL));
    qCDebug(g_cat_storage) << QString("Deleting string with ID 0x%1").arg(p_string_id, 8, 16, QChar('0'));

    // commit transaction
    E(mdb_txn_commit(l_txn));
}

/**
 * @brief M1Store::Storage::init initialize LMDB environment (util + string)
 */
void M1Store::Storage::init(){
    // LMDB return code (for use with return code test macros)
    int l_rc;

    // create data directory if not exists
    if ( ! fs::is_directory(STORE_DATA_PATH) ) {
        qCDebug(g_cat_storage) << QString("Data directory [%1] does not exist - creating it").arg(STORE_DATA_PATH);
        fs::create_directory(STORE_DATA_PATH);
    }
    // create LMDB storage path if not exists
    fs::path l_env_dir = fs::path(STORE_DATA_PATH) / fs::path(STORE_LMDB_DIR);
    if (!fs::is_directory(l_env_dir)) {
        qCDebug(g_cat_storage) << QString("LMDB directory [%1] does not exist - creating it\n").arg(l_env_dir.c_str());
        fs::create_directory(l_env_dir);
    }

    qCDebug(g_cat_storage) << QString("Opening LMDB environment in [%1]").arg(l_env_dir.c_str());
    // open LMDB environment
    E(mdb_env_create(&cm_lmdb_env));
    E(mdb_env_set_maxreaders(cm_lmdb_env, 1));
    E(mdb_env_set_maxdbs(cm_lmdb_env, 2));

    // 16 Gb Maximum Db size
    E(mdb_env_set_mapsize(cm_lmdb_env, 16L * 1024 * 1024 * 1024));

    E(mdb_env_open(cm_lmdb_env, l_env_dir.c_str(), 0, 0664));

    MDB_txn* l_txn;
    E(mdb_txn_begin(cm_lmdb_env, NULL, 0, &l_txn));

    bool l_not_found;

    // test the existence of string db and creates it if absent
    qCDebug(g_cat_storage) << QString("Testing existence of table [%1] ... ").arg(LMDB_STRING_DB);
    l_not_found = (mdb_dbi_open(l_txn, LMDB_STRING_DB, 0, &cm_dbi_string) == MDB_NOTFOUND);
    if(l_not_found){
        E(mdb_dbi_open(l_txn, LMDB_STRING_DB, MDB_CREATE , &cm_dbi_string));
    }
    qCDebug(g_cat_storage) << QString(l_not_found ? "does not exist --> creating it" : "exists");

    // test the existence of util db and creates it if absent
    qCDebug(g_cat_storage) << QString("Testing existence of table [%1] ... ").arg(LMDB_UTIL_DB);
    l_not_found = (mdb_dbi_open(l_txn, LMDB_UTIL_DB, 0, &cm_dbi_util) == MDB_NOTFOUND);
    qCDebug(g_cat_storage) << QString(l_not_found ? "does not exist --> creating it" : "exists");

    if(l_not_found){
        // create util table if not found and store 0 counters
        E(mdb_dbi_open(l_txn, LMDB_UTIL_DB, MDB_CREATE , &cm_dbi_util));
        saveCounters(l_txn);
    }
    else loadCounters(l_txn);

    // commit transaction
    E(mdb_txn_commit(l_txn));
 }

/**
 * @brief M1Store::Storage::close shut down LMDB environment
 */
void M1Store::Storage::close(){
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::close()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros)
    int l_rc;
    qCDebug(g_cat_storage) << QString("Closing Storage environment");

    saveCounters(NULL);

    MDB_stat l_mst;
    E(mdb_env_stat(cm_lmdb_env, &l_mst));
    qCDebug(g_cat_storage) << QString("--------------------- Env stats ----------------------------");
    qCDebug(g_cat_storage) << QString("B+tree depth:   %1").arg(l_mst.ms_depth);
    qCDebug(g_cat_storage) << QString("Branch pages:   %1").arg(l_mst.ms_branch_pages);
    qCDebug(g_cat_storage) << QString("Leaf pages:     %1").arg(l_mst.ms_leaf_pages);
    qCDebug(g_cat_storage) << QString("Overflow pages: %1").arg(l_mst.ms_overflow_pages);
    qCDebug(g_cat_storage) << QString("Total entries:  %1").arg(l_mst.ms_entries);

    // stats for Util and Strings
    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));
    // E(mdb_dbi_open(l_txn, LMDB_STRING_DB, 0 , &cm_dbi_string));

    E(mdb_stat(l_txn, cm_dbi_util, &l_mst));
    qCDebug(g_cat_storage) << QString("--------------------- Util stats ---------------------------");
    qCDebug(g_cat_storage) << QString("B+tree depth:   %1").arg(l_mst.ms_depth);
    qCDebug(g_cat_storage) << QString("Branch pages:   %1").arg(l_mst.ms_branch_pages);
    qCDebug(g_cat_storage) << QString("Leaf pages:     %1").arg(l_mst.ms_leaf_pages);
    qCDebug(g_cat_storage) << QString("Overflow pages: %1").arg(l_mst.ms_overflow_pages);
    qCDebug(g_cat_storage) << QString("Total entries:  %1").arg(l_mst.ms_entries);
    E(mdb_stat(l_txn, cm_dbi_string, &l_mst));
    qCDebug(g_cat_storage) << QString("--------------------- String stats -------------------------");
    qCDebug(g_cat_storage) << QString("B+tree depth:   %1").arg(l_mst.ms_depth);
    qCDebug(g_cat_storage) << QString("Branch pages:   %1").arg(l_mst.ms_branch_pages);
    qCDebug(g_cat_storage) << QString("Leaf pages:     %1").arg(l_mst.ms_leaf_pages);
    qCDebug(g_cat_storage) << QString("Overflow pages: %1").arg(l_mst.ms_overflow_pages);
    qCDebug(g_cat_storage) << QString("Total entries:  %1").arg(l_mst.ms_entries);

    // commit transaction
    E(mdb_txn_commit(l_txn));

    MDB_val l_key, l_data;
    MDB_cursor* l_curs;

#if CMAKE_BUILD_TYPE == Debug
    // table dumps only in Debug mode
    // dump util table
    qCDebug(g_cat_store) << QString("================ Util dump ==========");
    E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

    E(mdb_cursor_open(l_txn, cm_dbi_util, &l_curs));

    l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_FIRST);
    while(l_rc == MDB_SUCCESS){
        E(mdb_cursor_get(l_curs, &l_key, &l_data, MDB_GET_CURRENT));
        qCDebug(g_cat_storage) << QString("Key: %1 Value: %2")
                                    .arg(*((qint32 *)(l_key.mv_data)), 8, 16, QChar('0'))
                                    .arg(*((ItemID *)(l_data.mv_data)));

        l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_NEXT);
    }
    E(mdb_txn_commit(l_txn));

    // dump string table
    qCDebug(g_cat_store) << QString("================ String dump ========");

    for(StringID i=0; i<cm_next_string; i++){
        l_key.mv_size = sizeof(ItemID);
        l_key.mv_data = &i;

        // initiate transaction
        E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

        // retrieve string and put char* pointer into l_data.mv_data
        l_rc = mdb_get(l_txn, cm_dbi_string, &l_key, &l_data);

        if(l_rc == MDB_NOTFOUND)
            //qCDebug(g_cat_store) << QString("0x%1 --> MDB_NOTFOUND")
            //                            .arg(QString("%1").arg(i, 16, 16, QChar('0')).toUpper());
            l_key.mv_data = 0; // do nothing
        else{
            QString s((char *)(l_data.mv_data));
            qCDebug(g_cat_storage) << QString("0x%1 --> %2%3 [%4]")
                                          .arg(QString("%1").arg(i, 16, 16, QChar('0')).toUpper())
                                          .arg(s.left(100), s.length() > 100 ? "..." : "")
                                          .arg(l_data.mv_size).toUtf8().constData();
            // the .toUtf8().constData() is to avoid qDeboug() escaping the backslashes as per https://stackoverflow.com/questions/40366555/c-qt-qstring-replace-double-backslash-with-one
        }

        // commit transaction
        E(mdb_txn_commit(l_txn));
    }
    /*
    E(mdb_cursor_open(l_txn, cm_dbi_string, &l_curs));

    l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_FIRST);
    while(l_rc == MDB_SUCCESS){
        E(mdb_cursor_get(l_curs, &l_key, &l_data, MDB_GET_CURRENT));
        qCDebug(g_cat_storage) << QString("Key: %1 Value: %2 [%3]")
                                    .arg(*((ItemID *)(l_key.mv_data)), 16, 16, QChar('0'))
                                    .arg(((char *)(l_data.mv_data))).arg(l_data.mv_size);

        l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_NEXT);
    }
    E(mdb_txn_commit(l_txn));
    */
#endif

    // close everything
    mdb_dbi_close(cm_lmdb_env, cm_dbi_util);
    mdb_dbi_close(cm_lmdb_env, cm_dbi_string);
    mdb_env_close(cm_lmdb_env);
    qCDebug(g_cat_store) << QString("Storage environment closed");
}

/**
 * @brief M1Store::Storage::loadCounters load counters from LMDB util table
 * @param p_txn (required) LMDB transaction
 */
void M1Store::Storage::loadCounters(MDB_txn *p_txn){
    qCDebug(g_cat_storage) << QString("Loading counters");
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::loadCounters()", "LMDB env must be initialized");
    Q_ASSERT_X( p_txn != NULL, "Storage::loadCounters()", "must be passed an active transaction");
    // LMDB return code (for use with return code test macros)
    int l_rc;

    MDB_val l_key, l_data;

    qint32 l_key_val;

    l_key.mv_size = sizeof(qint32);
    l_key.mv_data = &l_key_val;

    // loading next Item ID value
    l_key_val = ITEM_NEXT_KEY;
    l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
    if(l_rc == MDB_NOTFOUND)
        cm_next_item = 0;
    else
        cm_next_item = *((ItemID *)(l_data.mv_data));
    qCDebug(g_cat_storage) << ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')):
                                 QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_next_item).arg(l_data.mv_size));

    // loading next String ID value
    l_key_val = ITEM_NEXT_STRING;
    l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
    if(l_rc == MDB_NOTFOUND)
        cm_next_string = 0;
    else
        cm_next_string = *((ItemID *)(l_data.mv_data));
    qCDebug(g_cat_storage) << ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')) :
                                 QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_next_string).arg(l_data.mv_size));
}

/**
 * @brief M1Store::Storage::saveCounters store counters into LMDB util table
 * @param p_txn (optional) LMDB transaction
 */
void M1Store::Storage::saveCounters(MDB_txn *p_txn){
    qCDebug(g_cat_storage) << QString("Saving counters");
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::saveCounters()", "LMDB env must be initialized");
    // LMDB return code (for use with return code test macros)
    int l_rc;

    MDB_val l_key, l_data;

    qint32 l_key_val;

    l_key.mv_size = sizeof(qint32);
    l_key.mv_data = &l_key_val;

    l_data.mv_size = sizeof(ItemID);

    // initiate transaction locally if none provided
    MDB_txn* l_txn;
    bool l_local_txn = false;
    if( p_txn == NULL ){
        E(mdb_txn_begin(cm_lmdb_env, NULL, 0, &l_txn));
        l_local_txn = true;
    }
    else l_txn = p_txn;

    // storing next Item ID value
    l_key_val = ITEM_NEXT_KEY;
    l_data.mv_data = &(cm_next_item);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_storage) << QString("ITEM_NEXT_KEY: 0x%2 --> %1").arg(cm_next_item).arg(l_key_val, 8, 16, QChar('0'));

    // storing next String ID value
    l_key_val = ITEM_NEXT_STRING;
    l_data.mv_data = &(cm_next_string);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_storage) << QString("ITEM_NEXT_STRING: 0x%2 --> %1").arg(cm_next_string).arg(l_key_val, 8, 16, QChar('0'));

    // if the transaction was locally initiated, commit it
    if( l_local_txn ) mdb_txn_commit(l_txn);
}
