#include <QtGlobal>
#include <QDebug>
#include <QIcon>

#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cmath>

#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"

Q_LOGGING_CATEGORY(g_cat_store_mmap, "store.mmap")

// helper macro for mmap() operations
#define EK(test) if(test) \
{qCDebug(g_cat_store) << QString("kernel [errno]-msg: [%1]-%2").arg(errno).arg(strerror(errno)); qFatal("Aborting/kernel");}

/**
 * \defgroup StoreMmap mmap() handling code
 * @ingroup STORE
 */
/**@{*/

M1Store::ItemID M1Store::StorageStatic::cm_item_next_upper_excluded{0};
M1Store::ItemID M1Store::StorageStatic::cm_items_per_mmap_segment{0};
unsigned int M1Store::StorageStatic::cm_item_mmap_next_seg_id{0};
unsigned long long M1Store::StorageStatic::cm_item_mmap_seg_size{0};

// base address of the mmapped space for special items(char* to make pointer arithmetic possible)
char* M1Store::StorageStatic::cm_special_mmap_base;
// base address of mmap segments (as char* to make pointer arithmetic possible in getItemPointer_lv1())
char* M1Store::StorageStatic::cm_item_mmap_base[M1Store::StorageStatic::ITEMS_SEGMENT_COUNT];

/**
 * @brief M1Store::Storage::initMmapConstants
 */
void M1Store::StorageStatic::initMmapConstants(){
    // path to special items mmap() file
    cm_mmap_filepath_specials = fs::path(M1Store::STORE_DATA_PATH) / fs::path(M1Store::LMDB_SPECIAL_MMAP_FILE);

    // mmap() constants
    cm_item_mmap_seg_size = 1 << ITEMS_MMAP_SEGMENT_SIZE_P2;
    cm_items_per_mmap_segment = 1 << ITEMS_MMAP_INCREMENT_COUNT_P2;
    cm_item_mmap_next_seg_id = m1_max(1, (unsigned int)ceil((double)cm_next_item / (double)cm_items_per_mmap_segment));
    cm_item_next_upper_excluded = cm_items_per_mmap_segment * cm_item_mmap_next_seg_id;
    qCDebug(g_cat_store_mmap) << QString("Items per segment: %1. Segment count: %2 --> next upper excluded: %3")
                                     .arg(cm_items_per_mmap_segment)
                                     .arg(cm_item_mmap_next_seg_id)
                                     .arg(cm_item_next_upper_excluded);
}

/**
 * @brief M1Store::Storage::clearMmapFiles
 */
void M1Store::StorageStatic::clearMmapFiles(){
    if(fs::exists(cm_mmap_filepath_specials)) fs::remove(cm_mmap_filepath_specials);
    // if(fs::exists(cm_mmap_filepath_items)) fs::remove(cm_mmap_filepath_items);
    for(unsigned int l_seg_id = 0; l_seg_id < cm_item_mmap_next_seg_id; l_seg_id++){
        QString l_filename = QString(M1Store::LMDB_ITEM_MMAP_FILE_TEMPLATE).arg(l_seg_id, 4, 16, QChar('0'));
        qCDebug(g_cat_store_mmap) << QString("Deleting [%1]").arg(l_filename);
        fs::path l_mmap_items_segment_filepath = fs::path(M1Store::STORE_DATA_PATH) / fs::path(l_filename.toStdString());
        if(fs::exists(l_mmap_items_segment_filepath)) fs::remove(l_mmap_items_segment_filepath);
    }
}

/**
 * @brief M1Store::Storage::openSpecialsMmap
 */
void M1Store::StorageStatic::openSpecialsMmap(){
    unsigned long l_special_mmap_size = (1 << SPECIAL_ITEMS_COUNT_P2) * sizeof(M1Store::SpecialItem);
    int l_fd = open(cm_mmap_filepath_specials.c_str(), O_RDWR); // open storage in R/W mode
    if (l_fd == -1) {
        qCDebug(g_cat_store_mmap) << QString("mmap() file [%1] does not exist. Creating it").arg(cm_mmap_filepath_specials.c_str());
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
        l_special_mmap_size,                // size
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
        qCDebug(g_cat_store_mmap) << QString("mmap() error (special items): [%1]-%2").arg(errno).arg(strerror(errno));
        qFatal("Aborting / mmap() special items");
    }
    qCDebug(g_cat_store) << "init mmap() storage complete";
}

/**
 * @brief M1Store::Storage::openItemsMmap
 */
void M1Store::StorageStatic::openItemsMmap(){
    M1_FUNC_ENTRY(g_cat_store_mmap, QString("Opening Items mmap() area"))

    // size of mmap() item area
    qCDebug(g_cat_store_mmap) << QString("sizeof(M1Store::Item_lv0) = %1").arg(sizeof(M1Store::Item_lv0));

    for(unsigned int l_seg_id = 0; l_seg_id < cm_item_mmap_next_seg_id; l_seg_id++)
        openSegment(l_seg_id);

    M1_FUNC_EXIT
}

/**
 * @brief M1Store::Storage::openSegment
 * @param p_seg_id
 */
void M1Store::StorageStatic::openSegment(unsigned int p_seg_id){
    M1_FUNC_ENTRY(g_cat_store_mmap, QString("Opening Items mmap() segment: %1").arg(p_seg_id))

    QString l_filename = QString(M1Store::LMDB_ITEM_MMAP_FILE_TEMPLATE).arg(p_seg_id, 4, 16, QChar('0'));
    fs::path l_mmap_items_segment_filepath = fs::path(M1Store::STORE_DATA_PATH) / fs::path(l_filename.toStdString());
    qCDebug(g_cat_store_mmap) << QString("opening item segment [%1] mmap() file [%2] size = %3")
                                     .arg(p_seg_id)
                                     .arg(l_mmap_items_segment_filepath.c_str())
                                     .arg(cm_item_mmap_seg_size);
    int l_fd = open(l_mmap_items_segment_filepath.c_str(), O_RDWR); // open storage file in R/W mode
    if (l_fd == -1) {
        qDebug() << QString("mmap() segment file [%1] does not exist. Creating it").arg(l_mmap_items_segment_filepath.c_str());
        // failure to open --> assumes file does not exist --> creating it
        EK( (l_fd = open(l_mmap_items_segment_filepath.c_str(), O_RDWR | O_CREAT, 0664)) < 0)
    }

    // fill the file to the mmap() length (as per
    // https://www.linuxquestions.org/questions/programming-9/mmap-tutorial-c-c-511265/ third post, by Hko)
    EK( lseek(l_fd, cm_item_mmap_seg_size-1, SEEK_SET) < 0 )
    EK( write(l_fd, "", 1) != 1 )

    // set up the memory map for items
    cm_item_mmap_base[p_seg_id] = (char*)mmap(
        NULL,                               // placed anywhere in RAM by kernel
        cm_item_mmap_seg_size,              // size
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
        qCDebug(g_cat_store_mmap) << QString("mmap() error (items): [%1]-%2").arg(errno).arg(strerror(errno));
        qFatal("Aborting / mmap() items");
    }
    M1_FUNC_EXIT
}

/**
 * @brief M1Store::Storage::closeMmapAreas
 */
void M1Store::StorageStatic::closeMmapAreas(){
    M1_FUNC_ENTRY(g_cat_store_mmap, QString("Closing Items mmap() area"))
    for(unsigned int l_seg_id = 0; l_seg_id < cm_item_mmap_next_seg_id; l_seg_id++){
        // just to be sure
        msync(cm_item_mmap_base[l_seg_id], cm_item_mmap_seg_size, MS_SYNC);
        // actual mmap closing
        qCDebug(g_cat_store_mmap) << QString("items segment [%1] munmap: %2").arg(l_seg_id).arg(munmap(cm_item_mmap_base, cm_item_mmap_seg_size));
    }

    unsigned long l_special_mmap_size = (1 << SPECIAL_ITEMS_COUNT_P2) * sizeof(M1Store::SpecialItem);
    qCDebug(g_cat_store) << QString("special items munmap: %1").arg(munmap(cm_special_mmap_base, l_special_mmap_size));
    M1_FUNC_EXIT
}

/**
 * @brief force mmap() synchronization for both items and special items.
 * To be called after each transaction
 */
void M1Store::StorageStatic::mmapSync(){
    M1_FUNC_ENTRY(g_cat_store, QString("Syncing mmap"))

    for(unsigned int l_seg_id = 0; l_seg_id < cm_item_mmap_next_seg_id; l_seg_id++)
        msync(cm_item_mmap_base[l_seg_id], cm_item_mmap_seg_size, MS_SYNC);

    unsigned long l_special_mmap_size = (1 << SPECIAL_ITEMS_COUNT_P2) * sizeof(M1Store::SpecialItem);
    msync(cm_special_mmap_base, l_special_mmap_size, MS_SYNC);
    M1_FUNC_EXIT
}

/**
 * @brief ItemID --> Item_lv0 pointer in mmap() area
 *
 * \ingroup StoreAccess
 * @param p_item_id the ItemID
 * @return the pointer
 */
M1Store::Item_lv1* M1Store::StorageStatic::getItemPointer_lv1(ItemID p_item_id){
    M1_FUNC_ENTRY(g_cat_store, QString("getItemPointer_lv0 from ItemID: 0x%1").arg(p_item_id, 16, 16, QChar('0')))
    Q_ASSERT_X(p_item_id < cm_next_item, "M1Store::Storage::getItemPointer_lv0()", "p_item_id out of bounds");

    // shift right by 23 = divide by the size of a segment in Items = 1Gb / 128 = 2^(30 - 7)
    unsigned int l_segment_number = p_item_id >> ITEMS_MMAP_INCREMENT_COUNT_P2;
    ItemID l_item_id_displacement = p_item_id & ITEMS_MMAP_SEG_DISPLACEMENT_MASK;
    M1_FUNC_EXIT
        // shift left by 7 bits = mult by 128 (Item size)
        return (Item_lv1*)(cm_item_mmap_base[l_segment_number] + (l_item_id_displacement << ITEM_SIZE_P2));
}

/**
 * @brief SpecialItemID --> SpecialItem pointer in mmap() area
 *
 * \ingroup StoreAccess
 * @param p_si_id the special item id
 * @return the pointer
 */
M1Store::SpecialItem* M1Store::StorageStatic::getSpecialItemPointer(const SpecialItemID p_si_id){
    M1_FUNC_ENTRY(g_cat_store, QString("getSpecialItemPointer from SpecialItemID: 0x%1").arg(p_si_id, 4, 16, QChar('0')))
    // shift left by 5 bits = mult by 32

    M1_FUNC_EXIT
        return (SpecialItem*)(cm_special_mmap_base + (p_si_id << 5));
}

/**
 * @brief allocate new Item_lv0 (and _lv1) in the mmap() area and give it a new ItemID.
 *
 * Also initialize members. This function "replaces" the Item_lv0/_lv1() constructors which are never used except for testing
 *
 * \ingroup StoreAccess
 * @param p_flags flag value for initialization
 * @param p_type ItemType value for initialization
 * @return pointer to the new Item_lv1
 */
M1Store::Item_lv1* M1Store::StorageStatic::getNewItemPointer_lv1(const FlagField p_flags, const ItemType& p_type){
    M1_FUNC_ENTRY(g_cat_store, QString("initialize new item: flags 0b%1 type %2").arg(p_flags, 64, 2, QChar('0')).arg(p_type.dbgString()))
    // get new item ID and increment the top ItemID counter
    if(cm_next_item == cm_item_next_upper_excluded){
        // create new segment if required
        openSegment(cm_item_mmap_next_seg_id);
        cm_item_mmap_next_seg_id += 1;
        cm_item_next_upper_excluded += cm_items_per_mmap_segment;
    }
    ItemID l_new_id = cm_next_item++;

    // get the pointer inside the mmap() area
    Item_lv1* l_ret = getItemPointer_lv1(l_new_id);
    // initialize members (depending on category determined from flags), including the new ItemID
    l_ret->initializeMembers_lv1(l_new_id, p_flags, p_type);

    M1_FUNC_EXIT
        return l_ret;
}
/**@}*/ //end of StoreMmap
