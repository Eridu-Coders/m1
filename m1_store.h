#ifndef M1_STORE_H
#define M1_STORE_H

#include <lmdb.h>

#include <map>

#include <QDateTime>
#include <QLoggingCategory>

#include "m1_lv0_item.h"

namespace M1Store{

    // ----------------------------------------- Storage ----------------------------------------------------
    /**
     * @brief The Storage class encapsulates all the storage-related features of the app
     *
     * It is a purely static class grouping a number of storage-related features for convenience
     */
    class Storage{
    private:
        static MDB_env* cm_lmdb_env;                ///< LMDB env
        static MDB_dbi cm_dbi_util;                 ///< util table handle
        static MDB_dbi cm_dbi_string;               ///< string table handle

        static ItemID cm_next_item;                 ///< next ItemID counter (stored in util table)
        static ItemID cm_next_string;               ///< next StringID counter (stored in util table)
        static SpecialItemID cm_next_special;       ///< next StringID counter (stored in util table)
        static unsigned int cm_current_version;     ///< current version of the graph

        /// mnemonic --> special id associative array
        static std::map<QString, SpecialItem*> cm_mnemonic_to_special;
        static char *cm_special_mmap_base;          ///< base address of the mmapped space for special items(char* to make pointer arithmetic possible)
        static char *cm_item_mmap_base;             ///< base address of the mmapped space for items (char* to make pointer arithmetic possible)
        static unsigned long cm_item_map_length;    ///< length of mmap() area for items
        static unsigned long cm_special_length;     ///< length of mmap() area for special items
    public:
        static void storeSetUp();
        static void mmapSync();
        static void storeShutDown();

        static Item_lv0* getItemSlotPointer(const ItemID p_item_id);

        static Item_lv0* getNewItemSlotPointer(const FlagField p_flags, const ItemType p_type);

        static SpecialItem* getSpecialSlotPointer(const SpecialItemID p_si_id);

        static SpecialItem* getSpecial(const ItemID p_item_id);
        static SpecialItem* getSpecial(const char* p_mnemonic);
        static SpecialItemID getSpecialID(const char* p_mnemonic);

        /// with corresponding Item
        static SpecialItem* newSpecial(const ItemID p_item_id, const FlagField p_flags, const char* p_mnemonic);
        /// without corresponding item
        static SpecialItem* newSpecial(const FlagField p_flags, const char* p_mnemonic);
        /// without corresponding item and reciprocal type
        static void newSpecial(const FlagField p_flags, const char* p_mnemonic_1, const char* p_mnemonic_2);

        /// increment the current version counter
        static void incrementCurrentVersion(){cm_current_version += 1;}
        /// get the current version number
        static unsigned int currentVersion(){return cm_current_version;}

        static void loadCounters(MDB_txn* p_txn);
        static void saveCounters(MDB_txn* p_txn);

        static ItemID storeString(QString p_string);
        static char* retrieveString(ItemID p_string_id);
        static void freeString(ItemID p_string_id);

        /// for testing purposes only
        static StringID dbg_get_string_id(Item_lv0& p_item) {return p_item.p.v.f.m_string_id;}

        static void setConstants();
        static void version_0_to_1();
        static void version_1_to_2();
    };

} // end namespace M1Store

#endif // M1_STORE_H
