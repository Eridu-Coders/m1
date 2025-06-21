#include <QtGlobal>
#include <QDebug>
#include <QIcon>

#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"

Q_LOGGING_CATEGORY(g_cat_store, "store.storage")
Q_LOGGING_CATEGORY(g_cat_silence, "dump")

// LMDB helper macros taken from the examples in the distrib.
#define E(expr) CHECK((l_rc = (expr)) == MDB_SUCCESS, #expr)
#define CHECK(test, msg) if( !(test) ) \
    {qCCritical(g_cat_store) << QString("[LMDB errno/msg: %1/%2] - %3").arg(l_rc).arg(mdb_strerror(l_rc), msg); qFatal("Aborting/LMDB");}

#define EK(test) if(test) \
    {qCDebug(g_cat_store) << QString("kernel [errno]-msg: [%1]-%2").arg(errno).arg(strerror(errno)); qFatal("Aborting/kernel");}

fs::path M1Store::Storage::cm_lmdb_dir;                 // path to storage directory
fs::path M1Store::Storage::cm_mmap_filepath_specials;  // path to specials mmap() file
fs::path M1Store::Storage::cm_mmap_filepath_items;     // path to items mmap() file

// class members holding the LMDB session data
MDB_env* M1Store::Storage::cm_lmdb_env = NULL;  // LMDB environment
MDB_dbi M1Store::Storage::cm_dbi_util;          // Utilities table handle
MDB_dbi M1Store::Storage::cm_dbi_string;        // string table handle

// class members holding counters for next IDs for items and string table (current value saved in utilities table between sessions)
M1Store::ItemID M1Store::Storage::cm_next_item{0};
M1Store::ItemID M1Store::Storage::cm_next_string{0};
M1Store::SpecialItemID M1Store::Storage::cm_next_special{0};
unsigned long M1Store::Storage::cm_item_next_upper_excluded{M1Env::ITEMS_MMAP_INCREMENT_COUNT};

// current version of graph structure
unsigned int M1Store::Storage::cm_current_version{0};

// base address of the mmapped space for special items(char* to make pointer arithmetic possible)
char* M1Store::Storage::cm_special_mmap_base;
// base address of mmap segment (as char* to make pointer arithmetic possible in getItemSlotPointer())
char* M1Store::Storage::cm_item_mmap_base;

// Mnemonic --> SpecialItem* associative array
std::map<QString, M1Store::SpecialItem*> M1Store::Storage::cm_mnemonic_to_special;
std::map<M1Env::ItemID, M1Store::SpecialItem*> M1Store::Storage::cm_item_id_to_special;

// special item constants initialized in setConstants()
// M1Env::SpecialItemID M1Env::TEXT_SIID = G_NONEX_SI_ID;

/*
M1Env::SpecialItemID M1Env::TEXT_WORD_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SECTION_SIID = G_NONEX_SI_ID;

M1Env::SpecialItemID M1Env::TW_WORD_OCC_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TW_REV_WORD_OCC_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TW_SECTION_2_OCC_BEGIN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TW_SECTION_2_OCC_END_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TW_REV_SECTION_2_OCC_BEGIN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TW_REV_SECTION_2_OCC_END_SIID = G_NONEX_SI_ID;

M1Env::SpecialItemID M1Env::TEXT_SLOKA_BHASHYA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_TRANSLATION_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_LINE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WORD_TRANSLIT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WORD_DICT_REF_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_LEMMA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WORD_DREF_INRIA_SIID = G_NONEX_SI_ID;

M1Env::SpecialItemID M1Env::TEXT_WFW_UNIT_SIID = G_NONEX_SI_ID; // subtype of TEXT_SECTION_SIID
M1Env::SpecialItemID M1Env::TEXT_WFW_TRANSL_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_URL_LINK_SIID = G_NONEX_SI_ID;

M1Env::SpecialItemID M1Env::TEXT_WROTE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WRITTEN_BY_SIID = G_NONEX_SI_ID;*/

/**
 * @brief initialize LMDB environment (util + string) and mmap() areas for items and specials.
 *
 * Also does version updates and sets constants values depending on up to date version
 */
void M1Store::Storage::storeSetUp(bool p_reset){
    M1_FUNC_ENTRY(g_cat_store, QString("Store set-up %1").arg(p_reset ? " (with reset)" : ""))
    qCDebug(g_cat_store) << QString("cm_item_next_upper_excluded = %1").arg(cm_item_next_upper_excluded);

    // LMDB return code (for use with return code test macros)
    int l_rc;

    // ------------------------------------ Directory creation -------------------------------------------------------------------------
    // create data directory if not exists
    if( !fs::is_directory(M1Store::STORE_DATA_PATH) ) {
        qCDebug(g_cat_store) << QString("Data directory [%1] does not exist - creating it").arg(M1Store::STORE_DATA_PATH);
        fs::create_directory(M1Store::STORE_DATA_PATH);
    }
    cm_lmdb_dir = fs::path(M1Store::STORE_DATA_PATH) / fs::path(M1Store::STORE_LMDB_DIR);
    // path to special items mmap() file
    cm_mmap_filepath_specials = fs::path(M1Store::STORE_DATA_PATH) / fs::path(M1Store::LMDB_SPECIAL_MMAP_FILE);
    // path to item mmap() file
    cm_mmap_filepath_items = fs::path(M1Store::STORE_DATA_PATH) / fs::path(M1Store::LMDB_ITEM_MMAP_FILE);

    // ------------------------------------ Store reset -------------------------------------------------------------------------
    // if the reset flag has been passed to the function --> reset all storage elements
    if(p_reset){
        qCDebug(g_cat_store) << QString("Resetting storage (p_reset = true)");
        // erase LMDB files
        fs::remove_all(cm_lmdb_dir);

        if(fs::exists(cm_mmap_filepath_specials)) fs::remove(cm_mmap_filepath_specials);
        if(fs::exists(cm_mmap_filepath_items)) fs::remove(cm_mmap_filepath_items);

        cm_item_next_upper_excluded = ITEMS_MMAP_INCREMENT_COUNT;
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
    E(mdb_env_set_mapsize(cm_lmdb_env, 16L * 1024 * 1024 * 1024));

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
    qCDebug(g_cat_store) << "init mmap() storage ...";

    // items
    qCDebug(g_cat_store) << QString("cm_item_next_upper_excluded = %1").arg(cm_item_next_upper_excluded);
    openItemsMmap();

    // specials
    unsigned long l_special_mmap_size = 65536 * sizeof(M1Store::SpecialItem);
    int l_fd = open(cm_mmap_filepath_specials.c_str(), O_RDWR); // open storage in R/W mode
    if (l_fd == -1) {
        qCDebug(g_cat_store) << QString("mmap() file [%1] does not exist. Creating it").arg(cm_mmap_filepath_specials.c_str());
        // failure to open --> assumes file does not exist --> creating it
        EK( (l_fd = open(cm_mmap_filepath_specials.c_str(), O_RDWR | O_CREAT, 0664)) < 0)
        // fill the file to the mmap() length (as per
        // https://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/ third post, by Hko)
        EK( lseek(l_fd, l_special_mmap_size-1, SEEK_SET) < 0 )
        EK( write(l_fd, "", 1) != 1 )
    }

    // set up the memory map for special items
    cm_special_mmap_base = (char*)mmap(
        NULL,                               // placed anywhere in RAM by kernel
        l_special_mmap_size,                  // size
        PROT_READ | PROT_WRITE,             // R/W allowed
        MAP_SHARED,                         // not private --> written to disk
        l_fd,                               // file descriptor
        0                                   // no offset within the file. Map starts at file origin
        );
    // file descriptor can be closed at this point as per
    // https://pubs.opengroup.org/onlinepubs/007904875/functions/mmap.html
    // 'The mmap() function shall add an extra reference ...'
    close(l_fd);

    if(cm_special_mmap_base == MAP_FAILED){
        // mmap() error
        qDebug() << QString("mmap() error (special items): [%1]-%2").arg(errno).arg(strerror(errno));
        qFatal("Aborting / mmap() special items");
    }
    qCDebug(g_cat_store) << "init mmap() storage complete";

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
    M1Env::M1EnvStatic::setSilentMode(true);

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

    qCDebug(g_cat_silence) << QString("ROOT_SIID                  = %1").arg(M1Store::ROOT_SIID);
    qCDebug(g_cat_silence) << QString("HOME_SIID                  = %1").arg(M1Store::HOME_SIID);
    qCDebug(g_cat_silence) << QString("AUTO_SIID                  = %1").arg(M1Store::AUTO_SIID);
    qCDebug(g_cat_silence) << QString("FOLDER_SIID                = %1").arg(M1Store::FOLDER_SIID);
    qCDebug(g_cat_silence) << QString("ISA_SIID                   = %1").arg(M1Store::ISA_SIID);
    qCDebug(g_cat_silence) << QString("ITO_SIID                   = %1").arg(M1Store::ITO_SIID);
    qCDebug(g_cat_silence) << QString("OWNS_SIID                  = %1").arg(M1Store::OWNS_SIID);
    qCDebug(g_cat_silence) << QString("BLNGS_SIID                 = %1").arg(M1Store::BLNGS_SIID);

    qCDebug(g_cat_silence) << QString("TEXT_SIID                  = %1").arg(M1Store::TEXT_SIID);
    qCDebug(g_cat_silence) << QString("T_WORD_SIID                = %1").arg(M1Store::FORM_SIID);
    qCDebug(g_cat_silence) << QString("TEXT_SECTION_SIID          = %1").arg(M1Store::TXTCK_SIID);

    qCDebug(g_cat_silence) << QString("TW_WORD_OCC                = %1").arg(M1Store::OCCUR_SIID);
    qCDebug(g_cat_silence) << QString("TW_SECTION_2_OCC_BEGIN     = %1").arg(M1Store::TW_SECTION_2_OCC_BEGIN_SIID);
    qCDebug(g_cat_silence) << QString("TW_SECTION_2_OCC_END       = %1").arg(M1Store::TW_SECTION_2_OCC_END_SIID);
    qCDebug(g_cat_silence) << QString("TW_REV_SECTION_2_OCC_BEGIN = %1").arg(M1Store::TW_REV_SECTION_2_OCC_BEGIN_SIID);
    qCDebug(g_cat_silence) << QString("TW_REV_SECTION_2_OCC_END   = %1").arg(M1Store::TW_REV_SECTION_2_OCC_END_SIID);

    // re-allow logging
    M1Env::M1EnvStatic::setSilentMode(false);

    // icons
    loadIcons();

    qCDebug(g_cat_store) << "storage environment set-up complete";

    M1_FUNC_EXIT
}

bool M1Store::Storage::lowSpaceMargin(){ return (cm_item_next_upper_excluded - cm_next_item) < (ITEMS_SPACE_MARGIN_FACTOR*ITEMS_MMAP_INCREMENT_COUNT); }
void M1Store::Storage::openItemsMmap(){
    M1_FUNC_ENTRY(g_cat_store, QString("Opening Items mmap() area"))

    // increase size if necessary
    if(lowSpaceMargin()) cm_item_next_upper_excluded += ITEMS_MMAP_INCREMENT_COUNT;

    // size of mmap() item area
    qCDebug(g_cat_store) << QString("cm_item_next_upper_excluded = %1").arg(cm_item_next_upper_excluded);
    qCDebug(g_cat_store) << QString("sizeof(M1Store::Item_lv0) = %1").arg(sizeof(M1Store::Item_lv0));
    unsigned long long l_item_mmap_size = cm_item_next_upper_excluded * sizeof(M1Store::Item_lv0);

    qCDebug(g_cat_store) << QString("opening item mmap() file [%1] size = %2").arg(cm_mmap_filepath_items.c_str()).arg(l_item_mmap_size);
    int l_fd = open(cm_mmap_filepath_items.c_str(), O_RDWR); // open storage file in R/W mode
    if (l_fd == -1) {
        qDebug() << QString("mmap() file [%1] does not exist. Creating it").arg(cm_mmap_filepath_items.c_str());
        // failure to open --> assumes file does not exist --> creating it
        EK( (l_fd = open(cm_mmap_filepath_items.c_str(), O_RDWR | O_CREAT, 0664)) < 0)
    }

    // fill the file to the mmap() length (as per
    // https://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/ third post, by Hko)
    EK( lseek(l_fd, l_item_mmap_size-1, SEEK_SET) < 0 )
    EK( write(l_fd, "", 1) != 1 )
    // the above is now placed here instead of within the if block above in case of enlargement requirement

    // set up the memory map for items
    cm_item_mmap_base = (char*)mmap(
        NULL,                               // placed anywhere in RAM by kernel
        l_item_mmap_size,                   // size
        PROT_READ | PROT_WRITE,             // R/W allowed
        MAP_SHARED,                         // not private --> written to disk
        l_fd,                               // file descriptor
        0                                   // no offset within the file. Map starts at file origin
    );
    // file descriptor can be closed at this point as per
    // https://pubs.opengroup.org/onlinepubs/007904875/functions/mmap.html
    // 'The mmap() function shall add an extra reference ...'
    close(l_fd);

    if(cm_item_mmap_base == MAP_FAILED){
        // mmap() error
        qCDebug(g_cat_store) << QString("mmap() error (items): [%1]-%2").arg(errno).arg(strerror(errno));
        qFatal("Aborting / mmap() items");
    }
    M1_FUNC_EXIT
}
void M1Store::Storage::closeItemsMmap(){
    M1_FUNC_ENTRY(g_cat_store, QString("Closing Items mmap() area"))
    unsigned long long l_item_mmap_size = cm_item_next_upper_excluded * sizeof(M1Store::Item_lv0);
    // just to be sure
    msync(cm_item_mmap_base, l_item_mmap_size, MS_SYNC);
    // actual mmap closing
    qCDebug(g_cat_store) << QString("items munmap: %1").arg(munmap(cm_item_mmap_base, l_item_mmap_size));
    M1_FUNC_EXIT
}

/**
 * @brief force mmap() synchronization for both items and special items.
 * To be called after each transaction
 */
void M1Store::Storage::mmapSync(){
    M1_FUNC_ENTRY(g_cat_store, QString("Syncing mmap"))
    unsigned long long l_item_mmap_size = cm_item_next_upper_excluded * sizeof(M1Store::Item_lv0);
    unsigned long l_special_mmap_size = 65536 * sizeof(M1Store::SpecialItem);

    msync(cm_item_mmap_base, l_item_mmap_size, MS_SYNC);
    msync(cm_special_mmap_base, l_special_mmap_size, MS_SYNC);
    M1_FUNC_EXIT
}

/**
 * @brief ItemID --> Item_lv0 pointer in mmap() area
 * @param p_item_id the ItemID
 * @return the pointer
 */
M1Store::Item_lv0* M1Store::Storage::getItemPointer_lv0(ItemID p_item_id){
    M1_FUNC_ENTRY(g_cat_store, QString("getItemPointer_lv0 from ItemID: 0x%1").arg(p_item_id, 16, 16, QChar('0')))
    Q_ASSERT_X(p_item_id < cm_next_item, "M1Store::Storage::getItemPointer_lv0()", "p_item_id out of bounds");

    // shift left by 7 bits = mult by 128
    M1_FUNC_EXIT
    return (Item_lv0*)(cm_item_mmap_base + (p_item_id << 7));
}

/**
 * @brief SpecialItemID --> SpecialItem pointer in mmap() area
 * @param p_si_id the special item id
 * @return the pointer
 */
M1Store::SpecialItem* M1Store::Storage::getSpecialItemPointer(const SpecialItemID p_si_id){
    M1_FUNC_ENTRY(g_cat_store, QString("getSpecialItemPointer from SpecialItemID: 0x%1").arg(p_si_id, 4, 16, QChar('0')))
    // shift left by 5 bits = mult by 32

    M1_FUNC_EXIT
    return (SpecialItem*)(cm_special_mmap_base + (p_si_id << 5));
}

/**
 * @brief allocate new Item_lv0 in the mmap() area and give it a new ItemID.
 *
 * Also initialize members. This function "replaces" the Item_lv0() constructor which is never used except for testing
 *
 * @param p_flags flag value for initialization
 * @param p_type ItemType value for initialization
 * @return pointer to the new Item_lv0
 */
M1Store::Item_lv0* M1Store::Storage::getNewItemPointer_lv0(const FlagField p_flags, const ItemType& p_type){
    M1_FUNC_ENTRY(g_cat_store, QString("initialize new item: flags 0b%1 type %2").arg(p_flags, 64, 2, QChar('0')).arg(p_type.dbgString()))
    // get new item ID and increment the top ItemID counter
    ItemID l_id = cm_next_item++;

    // throw warning if low free items
    if(lowSpaceMargin()) throw (new M1Env::M1Exception(
            QString("Low free space. Only %1 free items left").arg(cm_item_next_upper_excluded-cm_next_item),
            M1_WARNING_LOW_FREE_SPACE)
    );

    // get the pointer inside the mmap() area
    Item_lv0* l_ret = getItemPointer_lv0(l_id);
    // initialize members (depending on category determined from flags), including the new ItemID
    l_ret->initializeMembers(l_id, p_flags, p_type);

    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief Mnemonic --> SpecialItem pointer in mmap() area
 *
 * find the SpecialItemID corresponding to the mnemonic using cm_mnemonic_to_special then call getSpecialSlotPointer(SpecialItemID)
 *
 * @param p_mnemonic the mnemonic
 * @return the pointer
 */
M1Store::SpecialItem* M1Store::Storage::getSpecialItemPointer(const char* p_mnemonic){
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
bool M1Store::Storage::menmonic_exists(const char* p_mnemonic){
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
// TODO: Keep cm_dummy?
M1Store::SpecialItem* M1Store::Storage::getSpecialItemPointer(const ItemID p_item_id){
    M1_FUNC_ENTRY(g_cat_store, QString("getSpecial from ItemID: 0x%1").arg(p_item_id, 16, 16, QChar('0')))
    // SpecialItem* l_ret = nullptr;
    SpecialItem* l_ret = M1Store::SpecialItem::cm_dummy;

    if(auto l_search = cm_item_id_to_special.find(p_item_id); l_search != cm_item_id_to_special.end())
        l_ret = l_search->second;

    /*
    // goes through the whole table to find the corresponding SpecialItem, if it exists
    qCDebug(g_cat_store) << QString("cm_next_special: %1").arg(cm_next_special);
    for(SpecialItemID i = 0; i < cm_next_special; i++){
        SpecialItem* l_special_item = getSpecialItemPointer(i);
        qCDebug(g_cat_store) << QString("Special [0x%1] %3 --> 0x%2")
                                    .arg(i, 4, 16, QChar('0'))
                                    .arg(l_special_item->itemId(), 16, 16, QChar('0'))
                                    .arg(l_special_item->mnemonic());
        if(l_special_item->itemId() == p_item_id){
            M1_FUNC_EXIT
            return l_special_item;
        }
    }
    */

    // not found --> fatal error
    // qFatal("Aborting / searching in the special items table for an item that is not there");

    M1_FUNC_EXIT
    // return M1Store::SpecialItem::cm_dummy;
    return l_ret;
}

/**
 * @brief initialize a new special item in the mmap() area and allocates a new SpecialItemID
 * @param p_item_id the correspoinding item, if any, or G_VOID_ITEM_ID
 * @param p_flags flags for the new special item
 * @param p_mnemonic mnemonic for th new special item
 * @return pointer to the new special item
 */
M1Store::SpecialItem* M1Store::Storage::getNewSpecialWithItem(
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
M1Store::SpecialItem* M1Store::Storage::getNewSpecialNoItem(
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
void M1Store::Storage::getNewSpecialWithReciprocal(
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
    // M1Store::SpecialItem* l_s1 = getNewSpecialNoItem(p_flags | SI_HAS_RECIPROCAL, p_mnemonic_1, p_icon_path_1, p_siid_var_1);
    // M1Store::SpecialItem* l_s2 = getNewSpecialNoItem(p_flags_2 | SI_HAS_RECIPROCAL, p_mnemonic_2, p_icon_path_2, p_siid_var_2);
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
M1Store::SpecialItemID M1Store::Storage::getSpecialID(const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_store, QString("Mnemonic: %1").arg(p_mnemonic))
    Q_ASSERT_X(cm_mnemonic_to_special.find(p_mnemonic) != cm_mnemonic_to_special.end(),
               "Storage::getSpecialID()", (QString("Missing mnemonic: %1").arg(p_mnemonic)).toUtf8());

    M1_FUNC_EXIT
    return cm_mnemonic_to_special.at(p_mnemonic)->specialId();
}

/**
 * @brief Version update for 0 to 1
 *
 * Creates the basic structure of the graph root and associated nodes (Home, me, inboxes, ...) + basic types (ISA, ITO, etc.)
 */
void M1Store::Storage::version_0_to_1(){
    M1_FUNC_ENTRY(g_cat_store, QString("Creating item-less basic types"))
    M1Env::GraphInit::init_base();

    M1Store::Storage::mmapSync();
    M1Store::Storage::incrementCurrentVersion();
    M1_FUNC_EXIT
}

/**
 * @brief version update 1 to 2
 *
 * Creates the root nodes for texts
 */
/*
void M1Store::Storage::version_1_to_2(){
    M1_FUNC_ENTRY(g_cat_store, QString("Creating text-related types"))

    // Word Occurence edge
    getNewSpecialNoItem(SI_IS_TYPE, "WDOCC", M1Env::TEXT_WORD_ICON_PATH, &M1Env::TW_WORD_OCC_SIID);

    // "Reverse" word to occurence edges
    getNewSpecialNoItem(SI_IS_TYPE, "RWDOC", M1Env::TEXT_WORD_ICON_PATH, &M1Env::TW_REV_WORD_OCC_SIID);

    // Hierarchical section to occurence - begin and end - with their reciprocal
    getNewSpecialWithReciprocal(SI_IS_TYPE,
                                "HS2OB", M1Env::TW_SECTION_2_OCC_BEGIN_ICON_PATH, &M1Env::TW_SECTION_2_OCC_BEGIN_SIID,
                                "OC2HB", M1Env::TW_SECTION_2_OCC_BEGIN_ICON_PATH, &M1Env::TW_REV_SECTION_2_OCC_BEGIN_SIID,
                                SI_IS_TYPE);
    getNewSpecialWithReciprocal(SI_IS_TYPE,
                                "HS2OE", M1Env::TW_SECTION_2_OCC_END_ICON_PATH, &M1Env::TW_SECTION_2_OCC_END_SIID,
                                "OC2HE", M1Env::TW_SECTION_2_OCC_END_ICON_PATH, &M1Env::TW_REV_SECTION_2_OCC_END_SIID,
                                SI_IS_TYPE);

    M1Store::Item_lv2* l_type_root = M1Store::Item_lv2::getExisting("TYPE_");
    M1Store::Item_lv2* l_text_type = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "Texts root and type",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "TEXT_",                    // special item mnemonic
        M1Env::TEXT_ICON_PATH,      // icon (none in this case)
        &M1Env::TEXT_SIID           // special ID pseudo constant
    );
    l_text_type->linkTo(l_type_root, "BLNGS", nullptr, true);
    
    M1Store::Item_lv2* l_word = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "Text words (type)",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "TWORD",                    // special item mnemonic
        M1Env::TEXT_WORD_ICON_PATH, // icon (none in this case)
        &M1Env::TEXT_WORD_SIID      // special ID pseudo constant
    );
    l_word->linkTo(l_type_root, "BLNGS", nullptr, true);

    M1Store::Item_lv2* l_lemma = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "Text lemmas (type)",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "TXLEM",                    // special item mnemonic
        M1Env::TEXT_LEMMA_ICON_PATH,// icon (none in this case)
        &M1Env::TEXT_LEMMA_SIID     // special ID pseudo constant
    );
    l_lemma->linkTo(l_type_root, "BLNGS", nullptr, true);

    M1Store::Item_lv2* l_text_section_type = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "Text section type",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "TXSEC",                        // special item mnemonic
        M1Env::TEXT_SECTION_ICON_PATH,  // icon (none in this case)
        &M1Env::TEXT_SECTION_SIID       // special ID pseudo constant
    );
    l_text_section_type->linkTo(l_type_root, "BLNGS", nullptr, true);

    M1Store::Storage::mmapSync();
    M1Store::Storage::incrementCurrentVersion();

    M1_FUNC_EXIT
}

void M1Store::Storage::version_2_to_3(){
    M1_FUNC_ENTRY(g_cat_store, QString("Creating Indic text types"))

    // Word transliteration field simple edge type
    getNewSpecialNoItem(SI_IS_TYPE, "TRSLT", nullptr, &M1Env::TEXT_WORD_TRANSLIT_SIID);
    // word dictionary reference field simple edge type
    getNewSpecialNoItem(SI_IS_TYPE, "DCTRF", nullptr, &M1Env::TEXT_WORD_DICT_REF_SIID);

    // Word-for-Word translation field simple edge type
    getNewSpecialNoItem(SI_IS_TYPE, "WFTRN", nullptr, &M1Env::TEXT_WFW_TRANSL_SIID);
    // URL link field simple edge type
    getNewSpecialNoItem(SI_IS_TYPE, "URLNK", M1Env::TEXT_URL_LINK_ICON_PATH, &M1Env::TEXT_URL_LINK_SIID);

    getNewSpecialWithReciprocal(SI_IS_TYPE,
                                "WROTE", M1Env::TEXT_WROTE_ICON_PATH, &M1Env::TEXT_WROTE_SIID,
                                "WRTBY", M1Env::TEXT_WRITTEN_BY_ICON_PATH, &M1Env::TEXT_WRITTEN_BY_SIID,
                                SI_IS_TYPE | M1Env::SI_INSERT_AT_TOP);

    M1Store::Item_lv2* l_type_root = M1Store::Item_lv2::getExisting("TYPE_");
    M1Store::Item_lv2* l_sloka_bhashya_type = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "Sloka Bhashya",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "SLBHS",                                // special item mnemonic
        M1Env::TEXT_SLOKA_BHASHYA_ICON_PATH,    // icon (none in this case)
        &M1Env::TEXT_SLOKA_BHASHYA_SIID         // special ID pseudo constant
    );
    l_sloka_bhashya_type->linkTo(l_type_root, "BLNGS", nullptr, true);
    M1Store::Item_lv2* l_sloka_translation_type = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "Sloka Translation",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "SLTRN",                                // special item mnemonic
        M1Env::TEXT_SLOKA_TRANSLATION_ICON_PATH,// icon (none in this case)
        &M1Env::TEXT_SLOKA_TRANSLATION_SIID     // special ID pseudo constant
    );
    l_sloka_translation_type->linkTo(l_type_root, "BLNGS", nullptr, true);
    M1Store::Item_lv2* l_sloka_skt_line = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "Sloka Line",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "SLKLN",                            // special item mnemonic
        M1Env::TEXT_SLOKA_LINE_ICON_PATH,   // icon (none in this case)
        &M1Env::TEXT_SLOKA_LINE_SIID        // special ID pseudo constant
    );
    l_sloka_skt_line->linkTo(l_type_root, "BLNGS", nullptr, true);
    M1Store::Item_lv2* l_inria_dict_ref = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "INRIA Dictionary Reference",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "INRIA",                            // special item mnemonic
        nullptr,                            // icon (none in this case)
        &M1Env::TEXT_WORD_DREF_INRIA_SIID   // special ID pseudo constant
    );
    l_inria_dict_ref->linkTo(l_type_root, "BLNGS", nullptr, true);
    M1Store::Item_lv2* l_transl_unit = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX | M1Store::IS_SPECIAL,
        // type
        M1Store::ItemType(getSpecialItemPointer("TYPE_")->specialId()),
        // label
        "WfW Translation Unit (type)",
        // special item flags
        M1Store::SI_IS_TYPE | M1Store::SI_REQUIRES_EDGE,
        // special item mnemonic
        "TRLUN",                            // special item mnemonic
        M1Env::TEXT_WFW_UNIT_ICON_PATH,     // icon (none in this case)
        &M1Env::TEXT_WFW_UNIT_SIID          // special ID pseudo constant
    );
    l_transl_unit->linkTo(l_type_root, "BLNGS", nullptr, true);

    M1Store::Storage::mmapSync();
    M1Store::Storage::incrementCurrentVersion();

    M1_FUNC_EXIT
}
*/

QVector<QIcon*> M1Store::Storage::cm_type_icon;
QVector<QString> M1Store::Storage::cm_icon_path;

void M1Store::Storage::loadIcons(){
    M1_FUNC_ENTRY(g_cat_store, "setting up Special Item QIcon list")

    for(QString& l_icon_path: cm_icon_path){
        QIcon* l_icon = new QIcon(l_icon_path);
        Q_ASSERT_X( !l_icon->isNull(),
                   "Storage::loadIcons()",
                   QString("Icon [%1] failed to load").arg(l_icon_path).toUtf8());
        cm_type_icon.append(l_icon);
    }

    M1_FUNC_EXIT
}
void M1Store::Storage::appendDefaultIcon(){cm_type_icon.append(new QIcon("../Icons/Folder.svg"));}
QIcon* M1Store::Storage::getQIcon(SpecialItemID p_si_id){
    M1_FUNC_ENTRY(g_cat_store, QString("Get Icon for [%1]").arg(getSpecialItemPointer(p_si_id)->mnemonic()))

    // return "Unknown" Icon if no valid p_si_id was provided
    if(p_si_id == M1Env::G_VOID_SI_ID) return new QIcon("../Icons/Unknown.svg");

    // otherwise, return appropriate QIcon pointer
    QIcon* l_ret = cm_type_icon[p_si_id];
    M1_FUNC_EXIT
    return l_ret;
}

/*
0x0000 AUTO_ 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x0001 OWNS_ 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000011001 --RECIPROCAL--> BLNGS
0x0002 BLNGS 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000001011001 --RECIPROCAL--> OWNS_
0x0003 _ISA_ 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000111001 --RECIPROCAL--> _ITO_
0x0004 _ITO_ 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000111001 --RECIPROCAL--> _ISA_
0x0005 ROOT_ 0x0000000000000000 0b0000000000000000000000000000000000000000000000000000000000000000 --ITEM--> VRTX [_____] Global graph root (ROOT_)
0x0006 HOME_ 0x0000000000000002 0b0000000000000000000000000000000000000000000000000000000000000000 --ITEM--> VRTX [_____] Home (HOME_)
0x0007 ME___ 0x0000000000000004 0b0000000000000000000000000000000000000000000000000000000000000000 --ITEM--> VRTX [PERSN] Me (ME___)
0x0008 TYPE_ 0x0000000000000006 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Root of all types (TYPE_)
0x0009 PERSN 0x0000000000000008 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Person (type) (PERSN)
0x000a EMAIL 0x000000000000000c 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [_____] Email Inbox (EMAIL)
0x000b WHTSP 0x000000000000000e 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [_____] Whatsapp Inbox (WHTSP)
0x000c DSCRD 0x0000000000000010 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [_____] Discord Inbox (DSCRD)
0x000d WDOCC 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x000e RWDOC 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x000f HS2OB 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000011001 --RECIPROCAL--> OC2HB
0x0010 OC2HB 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000011001 --RECIPROCAL--> HS2OB
0x0011 HS2OE 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000011001 --RECIPROCAL--> OC2HE
0x0012 OC2HE 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000011001 --RECIPROCAL--> HS2OE
0x0013 TEXT_ 0x0000000000000032 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Texts root and type (TEXT_)
0x0014 TWORD 0x0000000000000038 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Text words (type) (TWORD)
0x0015 TXLEM 0x000000000000003e 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Text lemmas (type) (TXLEM)
0x0016 TXSEC 0x0000000000000044 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Text section type (TXSEC)
0x0017 TRSLT 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x0018 DCTRF 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x0019 WFTRN 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x001a PRABH 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x001b SIVAN 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x001c GAMBI 0xffffffffffffffff 0b0000000000000000000000000000000000000000000000000000000000001001
0x001d SLBHS 0x000000000000004a 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Sloka Bhashya (SLBHS)
0x001e SLTRN 0x0000000000000050 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Sloka Translation (SLTRN)
0x001f SLKLN 0x0000000000000056 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] Sloka Line (SLKLN)
0x0020 INRIA 0x000000000000005c 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] INRIA Dictionary Reference (INRIA)
0x0021 TRLUN 0x0000000000000062 0b0000000000000000000000000000000000000000000000000000000000000011 --ITEM--> VRTX [TYPE_] WfW Translation Unit (type) (TRLUN)
*/

// TODO: Ascending edges should not be represented when opening edges

/**
 * @brief Shuts down LMDB environment and mmap() areas for items and specials
 */
void M1Store::Storage::storeShutDown(){
    M1_FUNC_ENTRY(g_cat_store, "Store shut down")
    Q_ASSERT_X( cm_lmdb_env != nullptr, "Storage::close()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros)
    int l_rc;
    qCDebug(g_cat_store) << QString("Closing Storage environment");

    saveUtilities(nullptr);

    // set debug categories to just allow g_cat_silence (= "dump")
    M1Env::M1EnvStatic::setSilentMode(true);

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
    // E(mdb_dbi_open(l_txn, LMDB_STRING_DB, 0 , &cm_dbi_string));

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

    // close everything
    mdb_dbi_close(cm_lmdb_env, cm_dbi_util);
    mdb_dbi_close(cm_lmdb_env, cm_dbi_string);
    mdb_env_close(cm_lmdb_env);

    // just to be sure
    M1Store::Storage::mmapSync();

    // unmap areas
    closeItemsMmap();
    unsigned long l_special_mmap_size = 0x10000 * sizeof(M1Store::SpecialItem);
    qCDebug(g_cat_store) << QString("special items munmap: %1").arg(munmap(cm_special_mmap_base, l_special_mmap_size));

    // qCDebug(g_cat_store) << QString("sizeof(M1Store::Item_lv0) : %1").arg(sizeof(M1Store::Item_lv0));
    qCDebug(g_cat_store) << QString("Storage environment closed");

    M1_FUNC_EXIT
}

void M1Store::Storage::dbgUtilDump(){
    // LMDB return code (for use with return code test macros)
    int l_rc;
    MDB_val l_key, l_data;
    MDB_cursor* l_curs;

    // initiate transaction
    MDB_txn* l_txn;

    // dump util table
    qCDebug(g_cat_silence) << QString("================ Util dump ==========");
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

void M1Store::Storage::dbgSpecialDump(){
    qCDebug(g_cat_silence) << QString("========= Special Items dump ========");

    for(SpecialItemID i = 0; i < cm_next_special; i++)
        qCDebug(g_cat_silence) << getSpecialItemPointer(i)->dbgString();
}

void M1Store::Storage::dbgDump(){
    // various table dumps, executed only in debug build

    // LMDB return code (for use with return code test macros)
    int l_rc;
    MDB_val l_key, l_data;
    MDB_cursor* l_curs;

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

    // dump util table
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

    // dump string table
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

    // dump Constants
    qCDebug(g_cat_silence) << QString("================ Constants dump ========");
    qCDebug(g_cat_silence) << QString("ROOT_SIID  = 0x%1").arg(M1Store::ROOT_SIID, 4, 16, QChar('0'));
    qCDebug(g_cat_silence) << QString("HOME_SIID  = 0x%1").arg(M1Store::HOME_SIID, 4, 16, QChar('0'));
    qCDebug(g_cat_silence) << QString("ISA_SIID   = 0x%1").arg(M1Store::ISA_SIID, 4, 16, QChar('0'));
    qCDebug(g_cat_silence) << QString("ITO_SIID   = 0x%1").arg(M1Store::ITO_SIID, 4, 16, QChar('0'));
    qCDebug(g_cat_silence) << QString("OWNS_SIID  = 0x%1").arg(M1Store::OWNS_SIID, 4, 16, QChar('0'));
    qCDebug(g_cat_silence) << QString("BLNGS_SIID = 0x%1").arg(M1Store::BLNGS_SIID, 4, 16, QChar('0'));
    qCDebug(g_cat_silence) << QString("TEXT_SIID  = 0x%1").arg(M1Store::TEXT_SIID, 4, 16, QChar('0'));
    qCDebug(g_cat_silence) << QString("FORM_SIID  = 0x%1").arg(M1Store::FORM_SIID, 4, 16, QChar('0'));

    // dump special items table
    qCDebug(g_cat_silence) << QString("========= Special Items dump ========");

    for(SpecialItemID i = 0; i < cm_next_special; i++)
        qCDebug(g_cat_silence) << getSpecialItemPointer(i)->dbgString();

    // dump items --------------------------------------------------------------
    qCDebug(g_cat_silence) << QString("========= Items dump ================");

    for(ItemID i = 0; i < cm_next_item; i++){
        Item_lv2* l_item = Item_lv2::getExisting(i);
        qCDebug(g_cat_silence) << QString("0x%1 ").arg(i, 16, 16, QChar('0')) + l_item->dbgShort();
    }

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

    // restore categories --> all allowed
    M1Env::M1EnvStatic::setSilentMode(false);
}

/**
 * @brief load counters from LMDB util table
 * @param p_txn (required) LMDB transaction
 */
void M1Store::Storage::loadUtilites(MDB_txn *p_txn){
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

    // loading curremt upper bound for cm_next_item
    l_key_val = CURRENT_UPPER_COUNT;
    l_rc = mdb_get(p_txn, cm_dbi_util, &l_key, &l_data);
    if(l_rc == MDB_NOTFOUND)
        cm_item_next_upper_excluded = 0;
    else
        cm_item_next_upper_excluded = *((ItemID *)(l_data.mv_data));
    qCDebug(g_cat_store) << QString("CURRENT_UPPER_COUNT: ") +
        ((l_rc == MDB_NOTFOUND) ? QString("%1 --> KEY NOT FOUND").arg(l_key_val, 8, 16, QChar('0')) :
        QString("retrieved: 0x%1 --> %2 [%3]").arg(l_key_val, 8, 16, QChar('0')).arg(cm_item_next_upper_excluded).arg(l_data.mv_size));

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
void M1Store::Storage::saveUtilities(MDB_txn *p_txn){
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

    // storing curremt upper bound for cm_next_item
    l_key_val = CURRENT_UPPER_COUNT;
    l_data.mv_data = &(cm_item_next_upper_excluded);
    E(mdb_put(l_txn, cm_dbi_util, &l_key, &l_data, 0));
    qCDebug(g_cat_store) << QString("CURRENT_UPPER_COUNT: 0x%1 --> %2").arg(l_key_val, 8, 16, QChar('0')).arg(cm_item_next_upper_excluded);

    // --------------------------------------------------- Save icon paths --------------------------------------------
    for(M1Store::SpecialItemID l_siid = 0; l_siid < cm_next_special; l_siid++){
        l_key_val = l_siid;
        l_key.mv_size = sizeof(qint32);
        l_key.mv_data = &l_key_val;

        // get UTF8 version of string to store as a char* sequence
        QByteArray l_utf8_bytes = cm_icon_path[l_siid].toUtf8();
        l_data.mv_size = l_utf8_bytes.length() + 1; // +1 so that the null termination character also stored
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
M1Store::StringID M1Store::Storage::storeString(QString p_string){
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
char* M1Store::Storage::retrieveString(StringID p_string_id){
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
    E(mdb_txn_begin(M1Store::Storage::cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));

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
void M1Store::Storage::freeString(StringID p_string_id){
    M1_FUNC_ENTRY(g_cat_store, QString("Freeing String with id: %1").arg(p_string_id))
    Q_ASSERT_X( cm_lmdb_env != NULL, "Storage::freeString()", "LMDB env must be initialized");

    // LMDB return code (for use with return code test macros)
    int l_rc;

    MDB_val l_key;
    l_key.mv_size = sizeof(StringID);
    l_key.mv_data = &p_string_id;

    // initiate transaction
    MDB_txn* l_txn;
    E(mdb_txn_begin(M1Store::Storage::cm_lmdb_env, NULL, 0, &l_txn));

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
