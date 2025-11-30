#ifndef M1_STORE_H
#define M1_STORE_H

#include <lmdb.h>

#include <map>

#include <QDateTime>
#include <QLoggingCategory>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

// simplified namespace to access boost filesystem library functions
namespace fs = boost::filesystem;

#include "m1B_lv0_item.h"
#include "m1B_lv1_item.h"

/**
 * \defgroup UtilKeys Keys for stored in the utilities LMDB tables
 * @ingroup GlobDec
 * @{
 */
// keys for value storage in the utilities table
// values up to 0x00010000 are reserved for icon paths
const unsigned int ITEM_NEXT_KEY            = 0x00010000; ///< LMDB key for next ItemID
const unsigned int ITEM_NEXT_MMAP_SEGMENT   = 0x00010001; ///< LMDB key for next mmap() Item storage segment number
const unsigned int ITEM_NEXT_STRING         = 0x00010002; ///< LMDB key for next string ID
const unsigned int ITEM_NEXT_SPECIAL        = 0x00010003; ///< LMDB key for next special ID
const unsigned int CURRENT_VERSION          = 0x00010004; ///< LMDB key for the current version of the graph
//const unsigned int CURRENT_UPPER_COUNT  = 0x00010004; ///< LMDB key for the current upper count of cm_next_item
/**@}*/ //end of UtilKeys


namespace M1Store{
    // ----------------------------------------- Storage ----------------------------------------------------
    /**
     * @brief The Storage class encapsulates all the storage-related features of the system
     * \ingroup STORE
     *
     * It is a purely static class grouping a number of storage-related features for convenience
     * \todo Keep cm_dummy?
     */
    class StorageStatic{
        friend class Item_lv0;
        friend class SpecialItem;
    private:
        static fs::path cm_lmdb_dir;                ///< path to storage directory
        static fs::path cm_mmap_filepath_specials;  ///< path to specials mmap() file

        static MDB_env* cm_lmdb_env;                ///< LMDB env
        static MDB_dbi cm_dbi_util;                 ///< util table handle
        static MDB_dbi cm_dbi_string;               ///< string table handle

        static ItemID cm_next_item;                 ///< next ItemID counter (stored in util table)
        static StringID cm_next_string;             ///< next StringID counter (stored in util table)
        static SpecialItemID cm_next_special;       ///< next StringID counter (stored in util table)
        static unsigned int cm_current_version;     ///< current version of the graph

        /// mnemonic --> special id and ItemID --> special id associative arrays
        static std::map<QString, SpecialItem*> cm_mnemonic_to_special;
        /// \todo remove cm_item_id_to_special: no longer needed because of M1Store::Item_lv1::storeSpecialItemID()
        static std::map<ItemID, SpecialItem*> cm_item_id_to_special;

        /**
        * \defgroup MmapSpace Management of free space in the mmap() areas (and LMDB)
        * @ingroup GlobDec
        * @{
        */
        static const unsigned short SPECIAL_SIZE_P2 = 5;                    ///< 32 = 2^4 = size of a special item
        static const unsigned short ITEM_SIZE_P2 = 7;                       ///< 128 = 2^7 = size of an item
        static const unsigned short ITEMS_MMAP_SEGMENT_SIZE_P2 = 30;        ///< 1Gb = 2^30 = 0x4000, 0000 = 0b0100 0000 0000 0000, 0000 0000 0000 0000
        static const unsigned short SPECIAL_ITEMS_COUNT_P2 = 16;            ///< 65 536 = 2^16 = max number of special items

        /// 1Gb / 128 (2^7) = 2^23 = 2^(30 - 7) = number of item per segment = 0x80, 0000 = 0b1000 0000, 0000 0000 0000 0000 = 8 388 608
        static const unsigned short ITEMS_MMAP_INCREMENT_COUNT_P2 = ITEMS_MMAP_SEGMENT_SIZE_P2 - ITEM_SIZE_P2;
        /// 0x80 0000 - 1 = 0x5f ffff = mask to extract the segment displacement from an Item ID
        static const unsigned long ITEMS_MMAP_SEG_DISPLACEMENT_MASK = (1 << ITEMS_MMAP_INCREMENT_COUNT_P2) - 1;
        static const unsigned int ITEMS_SEGMENT_COUNT = 0x1000;             ///< 4096 --> total items space = 4 Tb = 34 359 738 368 items

        static const unsigned long long LMDB_MAX_SIZE = 0x400000000;        ///< 0x4 0000 0000 = 16Gb = max size of the LMDB database (incl strings)
        /**@}*/ //end of MmapSpace

        static unsigned long long cm_item_mmap_seg_size;        ///< Items mmap() segment size
        static char *cm_special_mmap_base;                      ///< base address of the mmapped space for special items (type = char* to make pointer arithmetic possible)
        static char *cm_item_mmap_base[ITEMS_SEGMENT_COUNT];    ///< base address of the mmapped space for items (type = char* to make pointer arithmetic possible)
        static ItemID cm_item_next_upper_excluded;              ///< Excluded upper bound for cm_next_item within the current segment
        static ItemID cm_items_per_mmap_segment;                ///< Item count per segment
        static unsigned int cm_item_mmap_next_seg_id;           ///< current number of segments

        /// special item ID --> icon vector
        static QVector<QString> cm_icon_path;
        static QVector<QIcon*> cm_type_icon;

        static void initMmapConstants();
        static void clearMmapFiles();
        static void openItemsMmap();
        static void openSpecialsMmap();
        static void openSegment(unsigned int p_seg_id);
        static void closeMmapAreas();
        /// increment the current version counter
        static void incrementCurrentVersion(){cm_current_version += 1;}

        static void loadUtilites(MDB_txn* p_txn);
        static void saveUtilities(MDB_txn* p_txn);

        static StringID storeString(QString p_string);
        static char* retrieveString(StringID p_string_id);
        static void freeString(StringID p_string_id);

        static void setConstants();
        static void loadIcons();
        static void version_0_to_1();
        // static void version_1_to_2();
        // static void version_2_to_3();

        static void dbgUtilDump();
        static void dbgStringDump();
        static void dbgVerticesDump();
        static void dbgItemsDump();
        static void dbgSpecialDump();
    public:
        static void storeSetUp(bool p_reset = false);
        static void mmapSync();
        static void storeShutDown();

        static Item_lv1* getItemPointer_lv1(const ItemID p_item_id);
        static Item_lv1* getNewItemPointer_lv1(const FlagField p_flags, const ItemType& p_type);

        static SpecialItem* getSpecialItemPointer(const SpecialItemID p_si_id);
        static SpecialItem* getSpecialItemPointer(const ItemID p_item_id);
        static SpecialItem* getSpecialItemPointer(const char* p_mnemonic);
        static bool menmonic_exists(const char* p_mnemonic);
        static SpecialItemID getSpecialID(const char* p_mnemonic);

        static void appendDefaultIcon();

        static SpecialItem* getNewSpecialWithItem(
            const ItemID p_item_id,
            const FlagField p_flags,
            const char* p_mnemonic,
            const char* p_icon_path
            // SpecialItemID* p_siid_var
            );
        static SpecialItem* getNewSpecialNoItem(
            const FlagField p_flags,
            const char* p_mnemonic,
            const char* p_icon_path
            // SpecialItemID* p_siid_var
            );
        static void getNewSpecialWithReciprocal(const FlagField p_flags,
                                                const char* p_mnemonic_1,
                                                const char* p_icon_path_1,
                                                // SpecialItemID* p_siid_var_1,
                                                const char* p_mnemonic_2,
                                                const char* p_icon_path_2,
                                                // SpecialItemID* p_siid_var_2,
                                                const FlagField p_flags_2);

        /// get the current version number
        static unsigned int currentVersion(){return cm_current_version;}

        static QList<SpecialItem*>& getSelectableEdgeTypes();
        static QList<SpecialItem*>& getSelectableVertexTypes();

        static StringID dbg_get_string_id(Item_lv0& p_item) {return p_item.p.v.f.m_string_id;}

        static QIcon* getQIcon(SpecialItemID p_si_id);

        static QString maxLengthChop(const QString& s, int p_maxlength){return s.length() > p_maxlength ? s.left(p_maxlength) + " ..." : s;}
        static void dbgDump();
    };

} // end namespace M1Store

#endif // M1_STORE_H
