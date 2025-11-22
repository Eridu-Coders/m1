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

namespace M1Store{
    // ----------------------------------------- Storage ----------------------------------------------------
    /**
     * @brief The Storage class encapsulates all the storage-related features of the app
     *
     * It is a purely static class grouping a number of storage-related features for convenience
     *
     */
    class Storage{
        friend class Item_lv0;
        friend class SpecialItem;
    private:
        static fs::path cm_lmdb_dir;                ///< path to storage directory
        static fs::path cm_mmap_filepath_specials;  ///< path to specials mmap() file
        static fs::path cm_mmap_filepath_items;     ///< path to items mmap() file

        static MDB_env* cm_lmdb_env;                ///< LMDB env
        static MDB_dbi cm_dbi_util;                 ///< util table handle
        static MDB_dbi cm_dbi_string;               ///< string table handle

        static ItemID cm_next_item;                 ///< next ItemID counter (stored in util table)
        static StringID cm_next_string;             ///< next StringID counter (stored in util table)
        static SpecialItemID cm_next_special;       ///< next StringID counter (stored in util table)
        static unsigned int cm_current_version;     ///< current version of the graph

        /// mnemonic --> special id and ItemID --> special id associative arrays
        static std::map<QString, SpecialItem*> cm_mnemonic_to_special;
        static std::map<ItemID, SpecialItem*> cm_item_id_to_special;
        static char *cm_special_mmap_base;                  ///< base address of the mmapped space for special items (type = char* to make pointer arithmetic possible)
        static char *cm_item_mmap_base;                     ///< base address of the mmapped space for items (type = char* to make pointer arithmetic possible)
        static unsigned long cm_item_next_upper_excluded;   ///< Excluded upper bound for cm_next_item within this mmap() size

        /// special item ID --> icon vector
        static QVector<QString> cm_icon_path;
        static QVector<QIcon*> cm_type_icon;

        static bool lowSpaceMargin();
        static void openItemsMmap();
        static void closeItemsMmap();
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
        static void version_1_to_2();
        static void version_2_to_3();

        static void dbgUtilDump();
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

        /// with corresponding Item
        static SpecialItem* getNewSpecialWithItem(
            const ItemID p_item_id,
            const FlagField p_flags,
            const char* p_mnemonic,
            const char* p_icon_path
            // SpecialItemID* p_siid_var
            );
        /// without corresponding item
        static SpecialItem* getNewSpecialNoItem(
            const FlagField p_flags,
            const char* p_mnemonic,
            const char* p_icon_path
            // SpecialItemID* p_siid_var
            );
        /// without corresponding item and reciprocal type
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

        /// get the list of special vertices that have the flag SI_IS_SELECTABLE set (for the edge type selection on the left pannel)
        static QList<SpecialItem*>& getSelectableEdgeTypes();
        static QList<SpecialItem*>& getSelectableVertexTypes();

        /// for testing purposes only
        static StringID dbg_get_string_id(Item_lv0& p_item) {return p_item.p.v.f.m_string_id;}

        static QIcon* getQIcon(SpecialItemID p_si_id);

        static QString maxLength(const QString& s, int p_maxlength){return s.length() > p_maxlength ? s.left(p_maxlength) + " ..." : s;}
        static void dbgDump();
    };

} // end namespace M1Store

#endif // M1_STORE_H
