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
#include "m1B_lv2_item.h"

// LMDB helper macros taken from the examples in the distrib. (require declaration "int l_rc;" wherever used)
// cannot be factored back into m1B_store.h for some reason
#define E(expr) CHECK((l_rc = (expr)) == MDB_SUCCESS, #expr)
#define CHECK(test, msg) if( !(test) ) \
{qCCritical(g_cat_store) << QString("[LMDB errno/msg: %1/%2] - %3").arg(l_rc).arg(mdb_strerror(l_rc), msg); qFatal("Aborting/LMDB");}

/**
 * \defgroup StoreDebug M1Store::Storage debug code
 * @ingroup STORE
 */
/**@{*/

/**
 * @brief M1Store::Storage::dbgUtilDump
 */
void M1Store::StorageStatic::dbgUtilDump(){
    // LMDB return code (for use with return code test macros)
    int l_rc;
    MDB_val l_key, l_data;
    MDB_cursor* l_curs;
    MDB_txn* l_txn;

    qCDebug(g_cat_silence) << QString("================ Util dump ==========");
    // initiate transaction
    E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

    E(mdb_cursor_open(l_txn, cm_dbi_util, &l_curs));

    l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_FIRST);
    while(l_rc == MDB_SUCCESS){
        E(mdb_cursor_get(l_curs, &l_key, &l_data, MDB_GET_CURRENT));
        qint32 l_key_value = *((qint32 *)(l_key.mv_data));
        if(l_key_value < 0x10000) // icon paths
            qCDebug(g_cat_silence) <<
                QString("Key: %1 Value: %2 [%3]")
                    .arg(l_key_value, 8, 16, QChar('0'))
                    .arg(QString::fromUtf8((char *)(l_data.mv_data)))
                    .arg(l_data.mv_size);
        else{ // counters
            unsigned long long l_value = l_data.mv_size == 4 ? *((unsigned int *)(l_data.mv_data)) :
                                             (l_data.mv_size == 2 ? *((SpecialItemID *)(l_data.mv_data)) : *((ItemID *)(l_data.mv_data)));
            qCDebug(g_cat_silence) <<
                QString("Key: %1 Value: %2 [%3]")
                    .arg(l_key_value, 8, 16, QChar('0'))
                    .arg(l_value)
                    .arg(l_data.mv_size);
        }

        l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_NEXT);
    }
    E(mdb_txn_commit(l_txn));
}

/**
 * @brief M1Store::Storage::dbgSpecialDump
 */
void M1Store::StorageStatic::dbgSpecialDump(){
    qCDebug(g_cat_silence) << QString("========= Special Items dump ========");

    for(SpecialItemID i = 0; i < cm_next_special; i++)
        qCDebug(g_cat_silence) << getSpecialItemPointer(i)->dbgString();
}

/**
 * @brief M1Store::Storage::dbgStringDump
 */
void M1Store::StorageStatic::dbgStringDump(){
    // LMDB return code (for use with return code test macros)
    int l_rc;
    MDB_val l_key, l_data;
    MDB_cursor* l_curs;
    MDB_txn* l_txn;

    qCDebug(g_cat_silence) << QString("================ String dump ========");

    for(StringID i=0; i<cm_next_string; i++){
        l_key.mv_size = sizeof(ItemID);
        l_key.mv_data = &i;

        // initiate transaction
        E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

        // retrieve string and put char* pointer into l_data.mv_data
        l_rc = mdb_get(l_txn, cm_dbi_string, &l_key, &l_data);

        if(l_rc == MDB_NOTFOUND)
            l_key.mv_data = 0; // do nothing
        else{
            QString s((char *)(l_data.mv_data));
            qCDebug(g_cat_silence) << QString("0x%1 --> %2%3 [%4]")
                                          .arg(QString("%1").arg(i, 16, 16, QChar('0')).toUpper())
                                          .arg(s.left(100), s.length() > 100 ? "..." : "")
                                          .arg(l_data.mv_size).toUtf8().constData();
            // the .toUtf8().constData() is to avoid qDebug() escaping the backslashes as per
            //https://stackoverflow.com/questions/40366555/c-qt-qstring-replace-double-backslash-with-one
        }

        // commit transaction
        E(mdb_txn_commit(l_txn));
    }
}

/**
 * @brief M1Store::Storage::dbgVerticesDump
 */
void M1Store::StorageStatic::dbgVerticesDump(){
    qCDebug(g_cat_silence) << QString("========= Vertices dump =============");

    QFile l_file("vertices_dump.txt");
    if (!l_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream l_out(&l_file);


    for(ItemID i = 0; i < cm_next_item; i++){
        Item_lv2* l_item = Item_lv2::getExisting(i);
        if(l_item->isFullVertex())
            l_out << QString("%1\n").arg(l_item->dbgString().toUtf8().constData());
    }
}

/**
 * @brief M1Store::Storage::dbgItemsDump
 */
void M1Store::StorageStatic::dbgItemsDump(){
    qCDebug(g_cat_silence) << QString("========= Items dump ================");

    QFile l_file("items_dump.txt");
    if (!l_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream l_out(&l_file);

    for(ItemID i = 0; i < cm_next_item; i++){
        Item_lv2* l_item = Item_lv2::getExisting(i);
        l_out << QString("0x%1 ").arg(i, 16, 16, QChar('0')) + l_item->dbgShort() + "\n";
    }
}

/**
 * @brief M1Store::Storage::dbgDump
 */
void M1Store::StorageStatic::dbgDump(){
    // various table dumps, executed only in debug build

    // dump util table
    dbgUtilDump();

    // dump string table
    dbgStringDump();

    // dump Constants
    M1Env::GraphInit::dbg_dump_pseudo_constants();

    // dump special items table
    dbgSpecialDump();

    // dump items --------------------------------------------------------------
    dbgItemsDump();

    // dump vertices --------------------------------------------------------------
    dbgVerticesDump();

    // tree dump --------------------------------------------------------------
    // Item_lv2::dbgRecurGraphStart(getSpecialItemPointer("ROOT_")->itemId());

    /*
    // LMDB return code (for use with return code test macros)
    int l_rc;
    MDB_val l_key, l_data;
    MDB_cursor* l_curs;

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

    qCDebug(g_cat_silence) << QString("================ Util dump ==========");

    E(mdb_cursor_open(l_txn, cm_dbi_util, &l_curs));

    l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_FIRST);
    while(l_rc == MDB_SUCCESS){
        E(mdb_cursor_get(l_curs, &l_key, &l_data, MDB_GET_CURRENT));
        qint32 l_key_value = *((qint32 *)(l_key.mv_data));
        if(l_key_value < 0x10000) // icon paths
            qCDebug(g_cat_silence) <<
                QString("Key: %1 Value: %2 [%3]")
                    .arg(l_key_value, 8, 16, QChar('0'))
                    .arg(QString::fromUtf8((char *)(l_data.mv_data)))
                    .arg(l_data.mv_size);
        else{ // counters
            unsigned long long l_value = l_data.mv_size == 4 ? *((unsigned int *)(l_data.mv_data)) :
                                             (l_data.mv_size == 2 ? *((SpecialItemID *)(l_data.mv_data)) : *((ItemID *)(l_data.mv_data)));
            qCDebug(g_cat_silence) <<
                QString("Key: %1 Value: %2 [%3]")
                    .arg(l_key_value, 8, 16, QChar('0'))
                    .arg(l_value)
                    .arg(l_data.mv_size);
        }

        l_rc = mdb_cursor_get(l_curs, NULL, NULL, MDB_NEXT);
    }
    E(mdb_txn_commit(l_txn));
    */

    /*
    qCDebug(g_cat_silence) << QString("================ String dump ========");

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
            qCDebug(g_cat_silence) << QString("0x%1 --> %2%3 [%4]")
                                          .arg(QString("%1").arg(i, 16, 16, QChar('0')).toUpper())
                                          .arg(s.left(100), s.length() > 100 ? "..." : "")
                                          .arg(l_data.mv_size).toUtf8().constData();
            // the .toUtf8().constData() is to avoid qDebug() escaping the backslashes as per
            //https://stackoverflow.com/questions/40366555/c-qt-qstring-replace-double-backslash-with-one
        }

        // commit transaction
        E(mdb_txn_commit(l_txn));
    }
    */
    /*
    qCDebug(g_cat_silence) << QString("========= Special Items dump ========");
    for(SpecialItemID i = 0; i < cm_next_special; i++)
        qCDebug(g_cat_silence) << getSpecialItemPointer(i)->dbgString();
    */

    // dump items --------------------------------------------------------------
    /*
    qCDebug(g_cat_silence) << QString("========= Items dump ================");

    for(ItemID i = 0; i < cm_next_item; i++){
        Item_lv2* l_item = Item_lv2::getExisting(i);
        qCDebug(g_cat_silence) << QString("0x%1 ").arg(i, 16, 16, QChar('0')) + l_item->dbgShort();
    }
    */
    /*
    // dump vertices --------------------------------------------------------------
    qCDebug(g_cat_silence) << QString("========= Vertices dump =============");

    for(ItemID i = 0; i < cm_next_item; i++){
        Item_lv2* l_item = Item_lv2::getExisting(i);
        if(l_item->isFullVertex())
            qCDebug(g_cat_silence) << l_item->dbgString().toUtf8().constData();
    }

    // tree dump --------------------------------------------------------------
    qCDebug(g_cat_silence) << QString("========= Tree Dump =================");
    Item_lv2::dbgRecurGraphStart(getSpecialItemPointer("ROOT_")->itemId());
    */

    // restore categories --> all allowed
    M1Env::EnvStatic::setSilentMode(false);
}
/**@}*/ //end of StoreDebug
