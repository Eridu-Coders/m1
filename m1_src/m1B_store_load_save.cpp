#include <QtGlobal>
#include <QDebug>
#include <QIcon>

#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cmath>

#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"

// LMDB helper macros taken from the examples in the distrib. (require declaration "int l_rc;" wherever used)
// cannot be factored back into m1B_store.h for some reason
#define E(expr) CHECK((l_rc = (expr)) == MDB_SUCCESS, #expr)
#define CHECK(test, msg) if( !(test) ) \
{qCCritical(g_cat_store) << QString("[LMDB errno/msg: %1/%2] - %3").arg(l_rc).arg(mdb_strerror(l_rc), msg); qFatal("Aborting/LMDB");}

/**
 * \defgroup StoreLoadSave Storing and saving from/into LMDB tables
 * @ingroup STORE
 */
/**@{*/

/**
 * @brief load counters from LMDB util table
 * @param p_txn (required) LMDB transaction
 */
void M1Store::StorageStatic::loadUtilites(MDB_txn *p_txn){
    M1_FUNC_ENTRY(g_cat_store, QString("Loading counters"))
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::loadCounters()", "LMDB env must be initialized");
    Q_ASSERT_X( p_txn != NULL, "Storage::loadCounters()", "must be passed an active transaction");

    // --------------------------------------------------- Load counters --------------------------------------------
    int l_rc; // LMDB return code (for use with return code test macros)
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
    qCDebug(g_cat_store) << QString("ITEM_NEXT_KEY: ") +
                                ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')):
                                     QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_next_item).arg(l_data.mv_size));

    // loading next String ID value
    l_key_val = ITEM_NEXT_STRING;
    l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
    if(l_rc == MDB_NOTFOUND)
        cm_next_string = 0;
    else
        cm_next_string = *((ItemID *)(l_data.mv_data));
    qCDebug(g_cat_store) << QString("ITEM_NEXT_STRING: ") +
                                ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')) :
                                     QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_next_string).arg(l_data.mv_size));

    // loading current graph version
    l_key_val = CURRENT_VERSION;
    l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
    if(l_rc == MDB_NOTFOUND)
        cm_current_version = 0;
    else
        cm_current_version = *((unsigned int *)(l_data.mv_data));
    qCDebug(g_cat_store) << QString("CURRENT_VERSION: ") +
                                ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')) :
                                     QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_current_version).arg(l_data.mv_size));

    // loading next special item
    l_key_val = ITEM_NEXT_SPECIAL;
    l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
    if(l_rc == MDB_NOTFOUND)
        cm_next_special = 0;
    else
        cm_next_special = *((SpecialItemID *)(l_data.mv_data));
    qCDebug(g_cat_store) << QString("ITEM_NEXT_SPECIAL: ") +
                                ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')) :
                                     QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_next_special).arg(l_data.mv_size));

    // loading next segment number
    l_key_val = ITEM_NEXT_MMAP_SEGMENT;
    l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
    if(l_rc == MDB_NOTFOUND)
        cm_item_mmap_next_seg_id = 0;
    else
        cm_item_mmap_next_seg_id = *((ItemID *)(l_data.mv_data));
    qCDebug(g_cat_store) << QString("ITEM_NEXT_MMAP_SEGMENT: ") +
                                ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')) :
                                     QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_item_mmap_next_seg_id).arg(l_data.mv_size));

    // --------------------------------------------------- load icon paths --------------------------------------------
    for(M1Store::SpecialItemID l_siid = 0; l_siid < cm_next_special; l_siid++){
        l_key_val = l_siid;
        l_key.mv_size = sizeof(qint32);
        l_key.mv_data = &l_key_val;

        // retrieve string and put char* pointer into l_data.mv_data
        l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
        qCDebug(g_cat_store) << ((l_rc == MDB_NOTFOUND) ?
                                     QString("%1 --> KEY NOT FOUND").arg(l_siid, 8, 16, QChar('0')):
                                     QString("retrieved: 0x%1 --> %2 [len = %3]")
                                         .arg(l_siid, 8, 16, QChar('0'))
                                         .arg((char *)(l_data.mv_data))
                                         .arg(l_data.mv_size)
                                 );

        cm_icon_path.append(QString::fromUtf8((char *)(l_data.mv_data)));
    }

    M1_FUNC_EXIT
}

/**
 * @brief store counters into LMDB util table
 * @param p_txn (optional) LMDB transaction
 */
void M1Store::StorageStatic::saveUtilities(MDB_txn *p_txn){
    M1_FUNC_ENTRY(g_cat_store, QString("Saving counters & icon paths"))
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::saveCounters()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros)
    int l_rc;
    MDB_val l_key, l_data;
    qint32 l_key_val;

    // --------------------------------------------------- Save counters --------------------------------------------
    l_key.mv_size = sizeof(qint32);
    l_key.mv_data = &l_key_val;

    // initiate transaction locally if none provided
    MDB_txn* l_txn;
    bool l_local_txn = false;
    if( p_txn == nullptr ){
        E(mdb_txn_begin(cm_lmdb_env, NULL, 0, &l_txn));
        l_local_txn = true;
        qCDebug(g_cat_store) << "Transaction started locally";
    }
    else l_txn = p_txn;

    l_data.mv_size = sizeof(ItemID);

    // storing next Item ID value
    l_key_val = ITEM_NEXT_KEY;
    l_data.mv_data = &(cm_next_item);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_store) << QString("ITEM_NEXT_KEY: 0x%2 --> %1").arg(cm_next_item).arg(l_key_val, 8, 16, QChar('0'));

    // storing next String ID value
    l_key_val = ITEM_NEXT_STRING;
    l_data.mv_data = &(cm_next_string);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_store) << QString("ITEM_NEXT_STRING: 0x%2 --> %1").arg(cm_next_string).arg(l_key_val, 8, 16, QChar('0'));

    l_data.mv_size = sizeof(unsigned int);

    // storing graph current version
    l_key_val = CURRENT_VERSION;
    l_data.mv_data = &(cm_current_version);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_store) << QString("CURRENT_VERSION: 0x%2 --> %1").arg(cm_current_version).arg(l_key_val, 8, 16, QChar('0'));

    l_data.mv_size = sizeof(SpecialItemID);

    // storing next Special Item id
    l_key_val = ITEM_NEXT_SPECIAL;
    l_data.mv_data = &(cm_next_special);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_store) << QString("ITEM_NEXT_SPECIAL: 0x%2 --> %1").arg(cm_next_special).arg(l_key_val, 8, 16, QChar('0'));

    l_data.mv_size = sizeof(ItemID);

    // storing next segment number
    l_key_val = ITEM_NEXT_MMAP_SEGMENT;
    l_data.mv_data = &(cm_item_mmap_next_seg_id);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_store) << QString("ITEM_NEXT_MMAP_SEGMENT: 0x%1 --> %2").arg(l_key_val, 8, 16, QChar('0')).arg(cm_item_mmap_next_seg_id);

    // --------------------------------------------------- Save icon paths --------------------------------------------
    for(M1Store::SpecialItemID l_siid = 0; l_siid < cm_next_special; l_siid++){
        l_key_val = l_siid;
        l_key.mv_size = sizeof(qint32);
        l_key.mv_data = &l_key_val;

        // get UTF8 version of string to store as a char* sequence
        QByteArray l_utf8_bytes = cm_icon_path[l_siid].toUtf8();
        l_data.mv_size = l_utf8_bytes.length() + 1; // +1 so that the null termination character is also stored
        l_data.mv_data = l_utf8_bytes.data();

        // storing next string
        E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
        qCDebug(g_cat_store) << QString("Stored icon path: 0x%1/0x%2 --> [%3/%4/%5] Utf8 len (+1) = %6")
                                    .arg(l_siid, 8, 16, QChar('0'))
                                    .arg(*((qint32*)l_key.mv_data), 8, 16, QChar('0'))
                                    .arg(cm_icon_path[l_siid])
                                    .arg(l_utf8_bytes.data())
                                    .arg(((char*)l_data.mv_data))
                                    .arg(l_data.mv_size);
    }

    // if the transaction was locally initiated, commit it
    if( l_local_txn ){
        mdb_txn_commit(l_txn);
        qCDebug(g_cat_store) << "Local transaction committed";

        // dbgUtilDump();
    }
    M1_FUNC_EXIT
}

/**
 * @brief store a new string into the string table and return the corresponding newly created string ID
 * @param p_string the string (QString)
 * @return the StringID of the stored string
 */
M1Store::StringID M1Store::StorageStatic::storeString(QString p_string){
    M1_FUNC_ENTRY(g_cat_store, QString("Storing String: %1").arg(p_string))
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::storeString()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros E and CHECK)
    int l_rc;

    MDB_val l_key, l_data;

    // attribute next String ID from the counter
    StringID l_ret_id = cm_next_string++;

    // designate it as the LMDB key
    l_key.mv_size = sizeof(StringID);
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
    qCDebug(g_cat_lv0_members) << QString("Stored string: 0x%1 --> [%2] Utf8 len = %3")
                                      .arg(l_ret_id, 8, 16, QChar('0')).arg(p_string).arg(l_utf8_bytes.length());

    // commit transaction
    E(mdb_txn_commit(l_txn));

    M1_FUNC_EXIT
        return l_ret_id;
}

/**
 * @brief retrieve a string from LMDB storage
 * @param p_string_id the StringID of the requested string
 * @return the string, as a (char *)
 */
char* M1Store::StorageStatic::retrieveString(StringID p_string_id){
    M1_FUNC_ENTRY(g_cat_store, QString("Getting String for id: %1").arg(p_string_id))
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::retrieveString()", "LMDB env must be initialized");

    // don't even try if p_string_id == G_VOID_ID
    static char l_empty_string[1] = "";
    if(p_string_id == G_VOID_ITEM_ID){
        M1_FUNC_EXIT
            return l_empty_string;
    }

    // LMDB return code (for use with return code test macros)
    int l_rc;

    // return value buffer
    static char *l_buf = NULL;

    MDB_val l_key, l_data;

    l_key.mv_size = sizeof(StringID);
    l_key.mv_data = &p_string_id;

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(M1Store::StorageStatic::cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

    // retrieve string and put char* pointer into l_data.mv_data
    l_rc = mdb_get(l_txn, cm_dbi_string, &l_key, &l_data);
    qCDebug(g_cat_store) << ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(p_string_id, 8, 16, QChar('0')):
                                 QString("retrieved: 0x%1 --> %2 [len = %3]").arg(p_string_id, 8, 16, QChar('0')).arg((char *)(l_data.mv_data)).arg(l_data.mv_size));

    // store string in buffer
    if(l_buf != NULL) free(l_buf); // free previous buffer if not NULL
    l_buf = (char *)malloc(l_data.mv_size + 1); // allocate new buffer of the appropriate size
    strncpy(l_buf, (char *)(l_data.mv_data), l_data.mv_size);
    l_buf[l_data.mv_size] = 0;

    // commit transaction
    E(mdb_txn_commit(l_txn));

    //return (char *)(l_data.mv_data);

    M1_FUNC_EXIT
        return l_buf;
}

/**
 * @brief Free a string from the LMDB store
 * @param p_string_id the StringID of the string
 */
void M1Store::StorageStatic::freeString(StringID p_string_id){
    M1_FUNC_ENTRY(g_cat_store, QString("Freeing String with id: %1").arg(p_string_id))
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::freeString()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros)
    int l_rc;

    MDB_val l_key;
    l_key.mv_size = sizeof(StringID);
    l_key.mv_data = &p_string_id;

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(M1Store::StorageStatic::cm_lmdb_env, NULL, 0, &l_txn));

    // delete the key from the store
    l_rc = mdb_del(l_txn, cm_dbi_string, &l_key, NULL);
    if( l_rc == MDB_NOTFOUND )
        qCDebug(g_cat_store) << QString("String with ID 0x%1 does not exist (acceptable)").arg(p_string_id, 8, 16, QChar('0'));
    else
        if( l_rc == MDB_SUCCESS )
            qCDebug(g_cat_store) << QString("String with ID 0x%1 deleted").arg(p_string_id, 8, 16, QChar('0'));
        else{
            qCCritical(g_cat_store) << QString("[LMDB errno/msg: %1/%2] - %3").arg(l_rc).arg(mdb_strerror(l_rc),
                                                                                             "mdb_del(l_txn, cm_dbi_string, &l_key, NULL)");
            qFatal("Aborting/LMDB");
        }

    // commit transaction
    E(mdb_txn_commit(l_txn));

    M1_FUNC_EXIT
}
/**@}*/ //end of StoreLoadSave
