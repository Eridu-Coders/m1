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

Q_LOGGING_CATEGORY(g_cat_store, "store.storage")
Q_LOGGING_CATEGORY(g_cat_silence, "dump")

fs::path M1Store::StorageStatic::cm_lmdb_dir;                // path to storage directory
fs::path M1Store::StorageStatic::cm_mmap_filepath_specials;  // path to specials mmap() file
// fs::path M1Store::Storage::cm_mmap_filepath_items;     // path to items mmap() file

// class members holding the LMDB session data
MDB_env* M1Store::StorageStatic::cm_lmdb_env = NULL;  // LMDB environment
MDB_dbi M1Store::StorageStatic::cm_dbi_util;          // Utilities table handle
MDB_dbi M1Store::StorageStatic::cm_dbi_string;        // string table handle

/// Icon paths
QVector<QString> M1Store::StorageStatic::cm_icon_path;
/// loaded Icons
QVector<QIcon*> M1Store::StorageStatic::cm_type_icon;

// class members holding counters for next IDs for items and string table (current value saved in utilities table between sessions)
M1Store::ItemID M1Store::StorageStatic::cm_next_item{0};
M1Store::ItemID M1Store::StorageStatic::cm_next_string{0};
M1Store::SpecialItemID M1Store::StorageStatic::cm_next_special{0};
// current version of graph structure
unsigned int M1Store::StorageStatic::cm_current_version{0};

// Mnemonic --> SpecialItem* associative array
std::map<QString, M1Store::SpecialItem*> M1Store::StorageStatic::cm_mnemonic_to_special;
std::map<M1Env::ItemID, M1Store::SpecialItem*> M1Store::StorageStatic::cm_item_id_to_special;

/**
 * \defgroup StoreOpenClose Sets-up and closes the storage environment
 * @ingroup STORE
 */
/**@{*/

/**
 * @brief initialize LMDB environment (util + string) and mmap() areas for items and specials.
 *
 * Also does version updates and sets constants values depending on up to date version
 */
void M1Store::StorageStatic::storeSetUp(bool p_reset){
    M1_FUNC_ENTRY(g_cat_store, QString("Store set-up %1").arg(p_reset ? " (with reset)" : ""))

    // LMDB return code (for use with return code test macros)
    int l_rc;
    initMmapConstants();

    // ------------------------------------ Directory creation -------------------------------------------------------------------------
    // create data directory if not exists
    if( !fs::is_directory(M1Store::STORE_DATA_PATH) ) {
        qCDebug(g_cat_store) << QString("Data directory [%1] does not exist - creating it").arg(M1Store::STORE_DATA_PATH);
        fs::create_directory(M1Store::STORE_DATA_PATH);
    }
    cm_lmdb_dir = fs::path(M1Store::STORE_DATA_PATH) / fs::path(M1Store::STORE_LMDB_DIR);
    // path to item mmap() file
    // cm_mmap_filepath_items = fs::path(M1Store::STORE_DATA_PATH) / fs::path(M1Store::LMDB_ITEM_MMAP_FILE);

    // ------------------------------------ Store reset -------------------------------------------------------------------------
    // if the reset flag has been passed to the function --> reset all storage elements
    if(p_reset){
        qCDebug(g_cat_store_mmap) << QString("Resetting storage (p_reset = true)");
        // erase LMDB files
        fs::remove_all(cm_lmdb_dir);

        clearMmapFiles();
    }

    // create LMDB storage path if not exists (placed here bc of the remove_all() above, in case of reset)
    if( !fs::is_directory(cm_lmdb_dir)) {
        qCDebug(g_cat_store) << QString("LMDB directory [%1] does not exist - creating it\n").arg(cm_lmdb_dir.c_str());
        fs::create_directory(cm_lmdb_dir);
    }

    // ------------------------------------ LMDB set-up -------------------------------------------------------------------------
    qCDebug(g_cat_store) << QString("Opening LMDB environment in [%1]").arg(cm_lmdb_dir.c_str());
    // open LMDB environment
    E(mdb_env_create(&cm_lmdb_env));
    E(mdb_env_set_maxreaders(cm_lmdb_env, 1));
    E(mdb_env_set_maxdbs(cm_lmdb_env, 2));

    // 16 Gb Maximum Db size
    E(mdb_env_set_mapsize(cm_lmdb_env, LMDB_MAX_SIZE));

    // open LMDB env and assign its directory (l_env_dir)
    E(mdb_env_open(cm_lmdb_env, cm_lmdb_dir.c_str(), 0, 0664));

    MDB_txn* l_txn;
    E(mdb_txn_begin(cm_lmdb_env, NULL, 0, &l_txn));

    // test the existence of string db and creates it if absent
    qCDebug(g_cat_store) << QString("Testing existence of table [%1] ... ").arg(M1Store::LMDB_STRING_DB);
    bool l_not_found = (mdb_dbi_open(l_txn, M1Store::LMDB_STRING_DB, 0, &cm_dbi_string) == MDB_NOTFOUND);
    if(l_not_found){
        E(mdb_dbi_open(l_txn, M1Store::LMDB_STRING_DB, MDB_CREATE , &cm_dbi_string));
    }
    qCDebug(g_cat_store) << QString(l_not_found ? "does not exist --> creating it" : "exists");

    // test the existence of util db and creates it if absent
    qCDebug(g_cat_store) << QString("Testing existence of table [%1] ... ").arg(M1Store::LMDB_UTIL_DB);
    l_not_found = (mdb_dbi_open(l_txn, M1Store::LMDB_UTIL_DB, 0, &cm_dbi_util) == MDB_NOTFOUND);
    qCDebug(g_cat_store) << QString(l_not_found ? "does not exist --> creating it" : "exists");

    if(l_not_found){
        // create util table if not found and store 0 counters
        E(mdb_dbi_open(l_txn, M1Store::LMDB_UTIL_DB, MDB_CREATE , &cm_dbi_util));
        saveUtilities(l_txn);
    }
    else loadUtilites(l_txn);

    // commit transaction
    E(mdb_txn_commit(l_txn));

    // ------------------------------------ mmap() set-up -------------------------------------------------------------------------
    // mmap() init
    qCDebug(g_cat_store_mmap) << QString("mmap() init ... ");

    // items
    openItemsMmap();

    // specials
    openSpecialsMmap();

    // ------------------------------------ rest of init -------------------------------------------------------------------------
    // version updates
    qCDebug(g_cat_silence) << "version updates";
    if(cm_current_version == 0)
        version_0_to_1();
    /*
    if(cm_current_version == 1)
        version_1_to_2();
    if(cm_current_version == 2)
        version_2_to_3();
    */

    // suspend logging for the task below
    M1Env::EnvStatic::setSilentMode(true);

    // pre-load special items associative arrays (menmonic --> SpecialItem* and ItemID --> SpecialItem*)
    for(SpecialItemID i = 0; i<cm_next_special; i++){
        SpecialItem* l_slot = getSpecialItemPointer(i);
        qCDebug(g_cat_silence) << QString("Special: [%2] %1 ---> 0x%3 / 0b%4")
                                      .arg(l_slot->mnemonic())
                                      .arg(i, 4, 16, QChar('0'))
                                      .arg(l_slot->itemId(), 16, 16, QChar('0'))
                                      .arg(l_slot->flags(), 64, 2, QChar('0'));
        cm_mnemonic_to_special[l_slot->mnemonic()] = l_slot;
        if(l_slot->itemId() != M1Env::G_VOID_ITEM_ID) cm_item_id_to_special[l_slot->itemId()] = l_slot;
    }
    dbgSpecialDump();

    // special items ID constants
    qCDebug(g_cat_store) << "set SpecialItemID constants";
    M1Env::GraphInit::set_pseudo_constants();
    M1Env::GraphInit::dbg_dump_pseudo_constants();

    // re-allow logging
    M1Env::EnvStatic::setSilentMode(false);

    // icons
    loadIcons();

    qCDebug(g_cat_store) << "storage environment set-up complete";

    M1_FUNC_EXIT
}

/**
 * @brief Shuts down LMDB environment and mmap() areas for items and specials
 */
void M1Store::StorageStatic::storeShutDown(){
    M1_FUNC_ENTRY(g_cat_store, "Store shut down")
    Q_ASSERT_X( cm_lmdb_env != nullptr, "Storage::close()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros)
    int l_rc;
    qCDebug(g_cat_store) << QString("Closing Storage environment");

    saveUtilities(nullptr);

    // set debug categories to just allow g_cat_silence (= "dump")
    M1Env::EnvStatic::setSilentMode(true);

    MDB_stat l_mst;
    E(mdb_env_stat(cm_lmdb_env, &l_mst));
    qCDebug(g_cat_silence) << QString("--------------------- Env stats ----------------------------");
    qCDebug(g_cat_silence) << QString("B+tree depth:   %1").arg(l_mst.ms_depth);
    qCDebug(g_cat_silence) << QString("Branch pages:   %1").arg(l_mst.ms_branch_pages);
    qCDebug(g_cat_silence) << QString("Leaf pages:     %1").arg(l_mst.ms_leaf_pages);
    qCDebug(g_cat_silence) << QString("Overflow pages: %1").arg(l_mst.ms_overflow_pages);
    qCDebug(g_cat_silence) << QString("Total entries:  %1").arg(l_mst.ms_entries);

    // stats for Util and Strings
    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

    E(mdb_stat(l_txn, cm_dbi_util, &l_mst));
    qCDebug(g_cat_silence) << QString("--------------------- Util stats ---------------------------");
    qCDebug(g_cat_silence) << QString("B+tree depth:   %1").arg(l_mst.ms_depth);
    qCDebug(g_cat_silence) << QString("Branch pages:   %1").arg(l_mst.ms_branch_pages);
    qCDebug(g_cat_silence) << QString("Leaf pages:     %1").arg(l_mst.ms_leaf_pages);
    qCDebug(g_cat_silence) << QString("Overflow pages: %1").arg(l_mst.ms_overflow_pages);
    qCDebug(g_cat_silence) << QString("Total entries:  %1").arg(l_mst.ms_entries);
    E(mdb_stat(l_txn, cm_dbi_string, &l_mst));
    qCDebug(g_cat_silence) << QString("--------------------- String stats -------------------------");
    qCDebug(g_cat_silence) << QString("B+tree depth:   %1").arg(l_mst.ms_depth);
    qCDebug(g_cat_silence) << QString("Branch pages:   %1").arg(l_mst.ms_branch_pages);
    qCDebug(g_cat_silence) << QString("Leaf pages:     %1").arg(l_mst.ms_leaf_pages);
    qCDebug(g_cat_silence) << QString("Overflow pages: %1").arg(l_mst.ms_overflow_pages);
    qCDebug(g_cat_silence) << QString("Total entries:  %1").arg(l_mst.ms_entries);

    // commit transaction
    E(mdb_txn_commit(l_txn));

#if CMAKE_BUILD_TYPE == Debug
    dbgDump();
#endif

    // close LMDB env
    mdb_dbi_close(cm_lmdb_env, cm_dbi_util);
    mdb_dbi_close(cm_lmdb_env, cm_dbi_string);
    mdb_env_close(cm_lmdb_env);

    // just to be sure
    M1Store::StorageStatic::mmapSync();

    // unmap areas
    closeMmapAreas();

    qCDebug(g_cat_store) << QString("Storage environment closed");
    M1_FUNC_EXIT
}

/**
 * @brief Version update for 0 to 1
 *
 * Creates the basic structure of the graph root and associated nodes (Home, me, inboxes, ...) + basic types (ISA, ITO, etc.)
 */
void M1Store::StorageStatic::version_0_to_1(){
    M1_FUNC_ENTRY(g_cat_store, QString("Creating item-less basic types"))
    M1Env::GraphInit::init_base();

    M1Store::StorageStatic::mmapSync();
    M1Store::StorageStatic::incrementCurrentVersion();
    M1_FUNC_EXIT
}

/**
 * @brief M1Store::Storage::loadIcons
 */
void M1Store::StorageStatic::loadIcons(){
    M1_FUNC_ENTRY(g_cat_store, "setting up Special Item QIcon list")

    for(QString& l_icon_path: cm_icon_path){
        QIcon* l_icon = new QIcon(l_icon_path);
        Q_ASSERT_X( !l_icon->isNull(), "Storage::loadIcons()",
                   QString("Icon [%1] failed to load").arg(l_icon_path).toUtf8());
        cm_type_icon.append(l_icon);
    }

    M1_FUNC_EXIT
}

/**
 * @brief M1Store::Storage::appendDefaultIcon
 */
void M1Store::StorageStatic::appendDefaultIcon(){cm_type_icon.append(new QIcon(M1Env::FOLDER_ICON_PATH));}

/**@}*/ //end of StoreOpenClose

/**
 * \defgroup StoreAccess Access to Storage Data Items
 * \ingroup STORE
 */
/**@{*/

/**
 * @brief Mnemonic --> SpecialItem pointer in mmap() area
 *
 * find the SpecialItemID corresponding to the mnemonic using cm_mnemonic_to_special then call getSpecialSlotPointer(SpecialItemID)
 *
 * @param p_mnemonic the mnemonic
 * @return the pointer
 */
M1Store::SpecialItem* M1Store::StorageStatic::getSpecialItemPointer(const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_store, QString("getSpecialItemPointer from mnemonic: %1").arg(p_mnemonic))
    Q_ASSERT_X(menmonic_exists(p_mnemonic), "Storage::getSpecialItemPointer()", (QString("Missing mnemonic: %1").arg(p_mnemonic)).toUtf8());

    M1Store::SpecialItem* l_ret = cm_mnemonic_to_special.at(p_mnemonic);

    M1_FUNC_EXIT
    return l_ret;
}
/**
 * @brief M1Store::Storage::menmonic_exists
 * @param p_mnemonic
 * @return
 */
bool M1Store::StorageStatic::menmonic_exists(const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_store, QString("Check existence of mnemonic: %1").arg(p_mnemonic))
    bool l_ret = cm_mnemonic_to_special.find(p_mnemonic) != cm_mnemonic_to_special.end();
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief ItemID --> SpecialItem pointer corresponding to it (if any)
 *
 * triggers an error if not found
 *
 * @param p_item_id the ItemID
 * @return the pointer
 */
M1Store::SpecialItem* M1Store::StorageStatic::getSpecialItemPointer(const ItemID p_item_id){
    M1_FUNC_ENTRY(g_cat_store, QString("getSpecial from ItemID: 0x%1").arg(p_item_id, 16, 16, QChar('0')))
    SpecialItem* l_ret = M1Store::SpecialItem::cm_dummy;

    if(auto l_search = cm_item_id_to_special.find(p_item_id); l_search != cm_item_id_to_special.end())
        l_ret = l_search->second;

    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief initialize a new special item in the mmap() area and allocates a new SpecialItemID
 * @param p_item_id the correspoinding item, if any, or G_VOID_ITEM_ID
 * @param p_flags flags for the new special item
 * @param p_mnemonic mnemonic for th new special item
 * @return pointer to the new special item
 */
M1Store::SpecialItem* M1Store::StorageStatic::getNewSpecialWithItem(
    const ItemID p_item_id,
    const FlagField p_flags,
    const char* p_mnemonic,
    const char* p_icon_path
    // SpecialItemID* p_siid_var
    ){
    M1_FUNC_ENTRY(g_cat_store, QString("Creating special item [%1]%2 Icon: %3")
                    .arg(p_mnemonic)
                    .arg((p_item_id == G_VOID_ITEM_ID) ? " (item-less)" : " (associated to supplied Item)")
                    .arg((p_icon_path == nullptr) ? "<none>" : p_icon_path))

    // gets the pointer
    M1Store::SpecialItem* l_ret = getSpecialItemPointer(cm_next_special);
    // sets the attributes, including the new SpecialItemID
    l_ret->setAttr(p_item_id, cm_next_special, p_flags, p_mnemonic);
    // increments top special item ID
    cm_next_special += 1;
    // update the mnemonic --> SpecialItem* map
    cm_mnemonic_to_special[p_mnemonic] = l_ret;
    cm_item_id_to_special[p_item_id] = l_ret;

    // add icon to cm_type_icon, which is a SpecialItemID --> QIcon array
    if(p_icon_path != nullptr)
        cm_icon_path.append(p_icon_path);
        // cm_type_icon.append(QIcon(p_icon_path));
    else
        cm_icon_path.append(M1Env::UNKOWN_ICON_PATH);
        // cm_type_icon.append(QIcon(M1Env::UNKOWN_ICON_PATH));

    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief initialize an item-less new special item in the mmap() area and allocates a new SpecialItemID
 * @param p_flags flags for the new special item
 * @param p_mnemonic mnemonic for th new special item
 * @return pointer to the new special item
 */
M1Store::SpecialItem* M1Store::StorageStatic::getNewSpecialNoItem(
    const FlagField p_flags,
    const char* p_mnemonic,
    const char* p_icon_path
    // SpecialItemID* p_siid_var
    ){
    // calls newSpecial() with G_VOID_ITEM_ID is ItemID
    // return getNewSpecialWithItem(G_VOID_ITEM_ID, p_flags | SI_HAS_NO_ITEM, p_mnemonic, p_icon_path, p_siid_var);
    return getNewSpecialWithItem(G_VOID_ITEM_ID, p_flags | SI_HAS_NO_ITEM, p_mnemonic, p_icon_path);
}

/**
 * @brief Creates a pair of reciprocal item-less type special items
 *
 * Does not return anything, contrary to the other newSpecial() variants
 *
 * @param p_flags flags for both
 * @param p_mnemonic_1 first mnemonic
 * @param p_mnemonic_2 second mnemonic
 */
void M1Store::StorageStatic::getNewSpecialWithReciprocal(
    const FlagField p_flags,
    const char* p_mnemonic_1,
    const char* p_icon_path_1,
    // SpecialItemID* p_siid_var_1,
    const char* p_mnemonic_2,
    const char* p_icon_path_2,
    // SpecialItemID* p_siid_var_2,
    const FlagField p_flags_2){
    M1_FUNC_ENTRY(g_cat_store, QString("Creating reciprocal item-less special items [%1] and [%2]")
                                .arg(p_mnemonic_1)
                                .arg(p_mnemonic_2))

    // forces the flag SI_HAS_RECIPROCAL for both
    M1Store::SpecialItem* l_s1 = getNewSpecialNoItem(p_flags | SI_HAS_RECIPROCAL, p_mnemonic_1, p_icon_path_1);
    M1Store::SpecialItem* l_s2 = getNewSpecialNoItem(p_flags_2 | SI_HAS_RECIPROCAL, p_mnemonic_2, p_icon_path_2);

    // iondicate that both are reciprocal of each other
    l_s1->setReciprocal(l_s2->specialId());
    l_s2->setReciprocal(l_s1->specialId());

    M1_FUNC_EXIT
}

/**
 * @brief mnemonic to SpecialItemID
 * @param p_mnemonic the mnemonic
 * @return SpecialItemID for this mnemonic
 */
M1Store::SpecialItemID M1Store::StorageStatic::getSpecialID(const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_store, QString("Mnemonic: %1").arg(p_mnemonic))
    Q_ASSERT_X(cm_mnemonic_to_special.find(p_mnemonic) != cm_mnemonic_to_special.end(),
               "Storage::getSpecialID()", (QString("Missing mnemonic: %1").arg(p_mnemonic)).toUtf8());

    M1_FUNC_EXIT
    return cm_mnemonic_to_special.at(p_mnemonic)->specialId();
}

/**
 * @brief M1Store::Storage::getQIcon
 * @param p_si_id
 * @return
 */
QIcon* M1Store::StorageStatic::getQIcon(SpecialItemID p_si_id){
    M1_FUNC_ENTRY(g_cat_store, QString("Get Icon for [%1]").arg(getSpecialItemPointer(p_si_id)->mnemonic()))

    static QIcon* l_unknown_icon = new QIcon(M1Env::UNKOWN_ICON_PATH);

    QIcon* l_ret = nullptr;
    if(p_si_id == M1Env::G_VOID_SI_ID)
        // return "Unknown" Icon if no valid p_si_id was provided
        l_ret = l_unknown_icon;
    else
        // otherwise, return appropriate QIcon pointer
        l_ret = cm_type_icon[p_si_id];

    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief M1Store::Storage::getSelectableEdgeTypes
 * @return
 */
QList<M1Store::SpecialItem*>& M1Store::StorageStatic::getSelectableEdgeTypes(){
    M1_FUNC_ENTRY(g_cat_store, QString("Get list of selectable edge types"))
    static QList<M1Store::SpecialItem*> l_ret_edges;

    l_ret_edges.clear();
    for(M1Env::SpecialItemID l_special_id = 0; l_special_id < cm_next_special; l_special_id++){
        M1Store::SpecialItem* l_special_item = getSpecialItemPointer(l_special_id);
        if((l_special_item->flags() & M1Env::SI_IS_SELECTABLE) && (l_special_item->flags() & M1Env::SI_EDGE_TYPE)) l_ret_edges.append(l_special_item);
    }

    M1_FUNC_EXIT
    return l_ret_edges;
}
/**
 * @brief M1Store::Storage::getSelectableVertexTypes
 * @return
 */
QList<M1Store::SpecialItem*>& M1Store::StorageStatic::getSelectableVertexTypes(){
    M1_FUNC_ENTRY(g_cat_store, QString("Get list of selectable vertex types"))
    static QList<M1Store::SpecialItem*> l_ret_vertices;

    l_ret_vertices.clear();
    for(M1Env::SpecialItemID l_special_id = 0; l_special_id < cm_next_special; l_special_id++){
        M1Store::SpecialItem* l_special_item = getSpecialItemPointer(l_special_id);
        qCDebug(g_cat_store) << l_special_item->mnemonic()
                 << (l_special_item->flags() & M1Env::SI_IS_SELECTABLE)
                 << (l_special_item->flags() & M1Env::SI_EDGE_TYPE)
                 << ((l_special_item->flags() & M1Env::SI_IS_SELECTABLE) && !(l_special_item->flags() & M1Env::SI_EDGE_TYPE));
        if((l_special_item->flags() & M1Env::SI_IS_SELECTABLE) && !(l_special_item->flags() & M1Env::SI_EDGE_TYPE)) l_ret_vertices.append(l_special_item);
    }

    M1_FUNC_EXIT
    return l_ret_vertices;
}
/**@}*/ //end of StoreAccess
