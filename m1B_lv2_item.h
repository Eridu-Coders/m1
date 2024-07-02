#ifndef M1_LV2_STORE_H
#define M1_LV2_STORE_H

#include <set>

#include "m1B_lv0_item.h"

namespace M1Store{
    /**
     * @brief The ItemWrapper class handles the basic interactions between items, i.e. mainly their connections by means of edges.
     */
class Item_lv2 : public Item_lv0 {
    private:
        static void dbgRecurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left);

        void addFullEdge(Item_lv2* p_new_edge, const SpecialItemID p_edge_type);
        void createTypeEdges();
        void defaultConnections();
    public:
        // ------------------------------- Static --------------------------------------------------------
        static Item_lv2* getNew(const FlagField p_flags, const ItemType& p_type);
        static Item_lv2* getNew(const FlagField p_flags, const ItemType& p_type, const char* p_label);
        static Item_lv2* getNew(
            const FlagField p_flags, const ItemType p_type, const char* p_label,
            const FlagField p_flags_special, const char* p_mnemonic);
        static Item_lv2* getExisting(const char* p_mnemonic);
        static Item_lv2* getExisting(const ItemID p_ID);

        static void dbgRecurGraphStart(const ItemID p_item_id);

        // ------------------------------- Instances --------------------------------------------------------
        /// zero constructor - never used
        Item_lv2(){}
        /**
         * @brief full constructor - used only for testing purposes.
         *
         * Normally, instantiateFromMMap(), which calls ItemWrapper(Item_lv0*) and its category-dependent descendants, is used instead
         *
         * @param p_item_id the ItemID
         * @param p_flags flags
         * @param p_type ItemType instance
         */
        Item_lv2(const ItemID p_item_id, const FlagField p_flags, const ItemType p_type) : Item_lv0(p_item_id, p_flags, p_type){}
        ~Item_lv2();

        bool isFullVertex(){return (flags() & ITEM_NATURE_MASK) == FULL_VERTEX;}
        bool isSimpleVertex(){return (flags() & ITEM_NATURE_MASK) == SIMPLE_VERTEX;}
        bool isFullEdge(){return (flags() & ITEM_NATURE_MASK) == FULL_EDGE;}
        bool isSimpleEdge(){return (flags() & ITEM_NATURE_MASK) == SIMPLE_EDGE;}

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();

        /// text setter - calls the underlying member from m_item
        void setText(const QString& s){Item_lv0::setText(s);}
        /// ItemID getter - calls the underlying member from m_item
        /*
        ItemID itemID(){return item_id();}
        */

        Item_lv2* getFirstEdgeWrapper();
        Item_lv2* getFirstEdgeSpecialWrapper();
        Item_lv2* getNextWrapper();
        Item_lv2* getPreviousWrapper();

        bool setType(const SpecialItem* p_type_si);
        bool setType(const SpecialItemID p_type_si_id);
        bool setType(const char* p_mnemonic);

        bool isOfType(const SpecialItem* p_type_si);
        bool isOfType(const ItemID p_type_item_id) const;
        bool isOfType(const SpecialItemID p_type_si_id) const;
        bool isOfType(const char* p_mnemonic) const;

        void linkTo(Item_lv2* p_target, const SpecialItemID p_type);
        void linkTo(Item_lv2* p_target, const char* p_mnemonic);
        void linkTo(ItemID p_target_id, const SpecialItemID p_type);
        void linkTo(ItemID p_target_id, const char* p_mnemonic);
    };

}

    QDebug operator<<(QDebug d, M1Store::Item_lv2 p);
    QDebug operator<<(QDebug d, M1Store::Item_lv2* p);

#endif // M1_LV2_STORE_H
