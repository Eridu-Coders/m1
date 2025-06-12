#ifndef M1_LV2_STORE_H
#define M1_LV2_STORE_H

#include <set>

#include "m1B_lv1_item.h"
#include "m1B_store.h"

namespace M1Store{
    /**
     * @brief The Item_lv2 class handles the basic interactions between items, i.e. mainly their connections by means of edges.
     */
class Item_lv2_iterator_base;
class Item_lv2_iterator;

class Item_lv2 : public Item_lv1 {
    private:
        static void dbgRecurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left);

        void installFullEdge(Item_lv2* p_new_edge, const SpecialItemID p_edge_type, Item_lv2* p_edge_above, const bool p_at_top = false);
        static void insertEdgeBelow(Item_lv2* p_new_edge, Item_lv2* p_edge_above);
        bool edgeBelongs(Item_lv2* p_edge, bool p_edge_is_special);
        void createTypeEdges();
        void defaultEdges();
        void loopNextPrevious();
        Item_lv2* findFieldEdge(const SpecialItem* p_field_type_si) const;
        std::shared_ptr<Item_lv2_iterator_base> getIteratorGeneric(Item_lv2* p_start_edge, const SpecialItemID p_type) const;
    public:
        // ------------------------------- Static --------------------------------------------------------
        static Item_lv2* getNew(const FlagField p_flags,
                                const ItemType& p_type);
        static Item_lv2* getNew(const FlagField p_flags,
                                const char* p_label);
        static Item_lv2* getNew(const FlagField p_flags,
                                const char* p_label,
                                const FlagField p_flags_special,
                                const char* p_mnemonic,
                                const char* p_icon_path);

        static Item_lv2* getExisting(const char* p_mnemonic);
        static Item_lv2* getExisting(const ItemID p_item_id);
        static Item_lv2* getExisting(const SpecialItemID p_si_id);

        // prevents other types from being implicitly converted
        template <class T>
        static  Item_lv2* getExisting(T) = delete; // C++11

        static void dbgRecurGraphStart(const ItemID p_item_id);

        // ------------------------------- Instances --------------------------------------------------------
        /// zero constructor - never used
        Item_lv2(){}
        /**
         * @brief full constructor - used only for testing purposes.
         *
         * Normally, getExisting() and getNew(), are used instead. They call Storage::getItemPointer_lv0()
         * and Storage::getNewItemPointer_lv0() respectively and do a static cast to Item_lv2*
         *
         * @param p_item_id the ItemID
         * @param p_flags flags
         * @param p_type ItemType instance
         */
        Item_lv2(const ItemID p_item_id, const FlagField p_flags, const ItemType p_type) : Item_lv1(p_item_id, p_flags, p_type){}
        ~Item_lv2();

        bool isFullVertex() const {return (flags() & ITEM_NATURE_MASK) == FULL_VERTEX;}
        bool isSimpleVertex() const {return (flags() & ITEM_NATURE_MASK) == SIMPLE_VERTEX;}
        bool isFullEdge() const {return (flags() & ITEM_NATURE_MASK) == FULL_EDGE;}
        bool isSimpleEdge() const {return (flags() & ITEM_NATURE_MASK) == SIMPLE_EDGE;}

        Item_lv2* getFirstEdge_lv2() const;
        Item_lv2* getAutoEdge_lv2() const;
        Item_lv2* getFirstEdgeSpecial_lv2() const;
        Item_lv2* getReciprocalEdge_lv2() const;
        Item_lv2* getTarget_lv2() const;
        Item_lv2* getOrigin_lv2() const;

        Item_lv2* getNext_lv2() const;
        Item_lv2* getNext_lv2(const SpecialItemID p_type) const;
        Item_lv2* getNext_lv2(const char* p_mnemonic) const;

        // prevents other types from being implicitly converted
        template <class T>
        Item_lv2* getNext_lv2(T) = delete; // C++11

        Item_lv2* getPrevious_lv2() const;
        Item_lv2* getPrevious_lv2(const SpecialItemID p_type) const;
        Item_lv2* getPrevious_lv2(const char* p_mnemonic) const;

        // prevents other types from being implicitly converted
        template <class T>
        Item_lv2* getPrevious_lv2(T) = delete; // C++11

        bool setType(const SpecialItem* p_type_si);
        bool setType(const SpecialItemID p_type_si_id);
        bool setType(const char* p_mnemonic);

        bool isOfType(const SpecialItem* p_type_si) const;
        bool isOfType(const SpecialItemID p_type_si_id) const;
        bool isOfType(const char* p_mnemonic) const;
        // bool isOfType(const ItemID p_type_item_id) const;
        SpecialItemID getMaxTypeMember();

        bool setField(const QString& p_content,
                      const bool p_force_new,
                      const SpecialItem* p_field_type_si,
                      const SpecialItem* p_field_extra_type_si = nullptr);
        QString getField(const SpecialItem* p_field_type_si, const SpecialItem* p_field_type2_si=nullptr, const bool p_all=false) const;
        QString getField(const SpecialItemID p_field_type_si_id, const bool p_all=false) const{
            return this->getField(M1Store::Storage::getSpecialItemPointer(p_field_type_si_id), nullptr, p_all);
        }
        QString getField(const SpecialItemID p_field_type_si_id, const SpecialItemID p_field_type2_si_id, const bool p_all=false) const{
            return this->getField(
                M1Store::Storage::getSpecialItemPointer(p_field_type_si_id),
                M1Store::Storage::getSpecialItemPointer(p_field_type2_si_id), p_all);
        }

        Item_lv2* linkTo(Item_lv2* p_target, const SpecialItemID p_type, Item_lv2* p_edge_above = nullptr, const bool p_at_top = false);
        Item_lv2* linkTo(Item_lv2* p_target, const char* p_mnemonic, Item_lv2* p_edge_above = nullptr, const bool p_at_top = false);

        Item_lv2* linkTo(ItemID p_target_id, const SpecialItemID p_type, Item_lv2* p_edge_above = nullptr, const bool p_at_top = false);
        Item_lv2* linkTo(ItemID p_target_id, const char* p_mnemonic, Item_lv2* p_edge_above = nullptr, const bool p_at_top = false);

        // prevents other types from being implicitly converted
        template <class T>
        Item_lv2* linkTo(T, const SpecialItemID p_type, Item_lv2* p_edge_above = nullptr, const bool p_at_top = false) = delete; // C++11
        template <class U>
        Item_lv2* linkTo(U, const char* p_mnemonic, Item_lv2* p_edge_above = nullptr, const bool p_at_top = false) = delete; // C++11
        template <class V>
        Item_lv2* linkTo(ItemID p_target_id, const V, Item_lv2* p_edge_above = nullptr, const bool p_at_top = false) = delete; // C++11

        Item_lv2* find_edge(const SpecialItemID p_type_edge, const SpecialItemID p_type_target) const;
        // Item_lv2* find_edge(const char* p_mnemonic) const;
        Item_lv2* find_edge_edge(const SpecialItemID p_type) const;
        Item_lv2* find_edge_target(const SpecialItemID p_type) const;

        // prevents other types from being implicitly converted
        template <class T>
        Item_lv2* find_edge(T) const = delete; // C++11

        Item_lv2_iterator getIteratorTop(const SpecialItemID p_type = G_VOID_SI_ID) const;
        Item_lv2_iterator getIteratorAuto(const SpecialItemID p_type = G_VOID_SI_ID) const;
        Item_lv2_iterator getIteratorSpecial(const SpecialItemID p_type = G_VOID_SI_ID) const;

        // prevents other types from being implicitly converted
        template <class T>
        Item_lv2_iterator getIteratorTop(const T) const = delete; // C++11
        template <class T>
        Item_lv2_iterator getIteratorAuto(const T) const = delete; // C++11
        template <class T>
        Item_lv2_iterator getIteratorSpecial(const T) const = delete; // C++11

        QString dbgString();
        QString dbgStringHtml();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();
};

/**
 * @brief Base class --> scans through ALL edges (no filter)
 */
class Item_lv2_iterator_base{
        friend class Item_lv2;
        friend class Item_lv2_iterator_edge_type;
    private:
        Item_lv2* m_current_edge = nullptr;
        ItemID m_first_edge_item_id = G_VOID_ITEM_ID;
        bool m_not_first = false;

        Item_lv2_iterator_base();
        Item_lv2_iterator_base(const Item_lv2_iterator_base& p_copiand);
        Item_lv2_iterator_base(Item_lv2* p_start_edge);
    public:
        bool isNull();

        virtual void next();
        virtual bool beyondEnd() const;
        virtual Item_lv2* at() const;

        virtual QString dbgShort();
};

/**
 * @brief Edge type filter iterator --> scans only thoses edges that match the filter
 */
class Item_lv2_iterator_edge_type : public Item_lv2_iterator_base{
        friend class Item_lv2;
    private:
        std::vector<Item_lv2*>::iterator m_selection;
        int m_curren_position;

        Item_lv2_iterator_edge_type();
        Item_lv2_iterator_edge_type(const Item_lv2_iterator_edge_type& p_copiand);
        Item_lv2_iterator_edge_type(Item_lv2* p_start_edge, const SpecialItemID p_type);
    public:
        void next();
        bool beyondEnd() const;
        Item_lv2* at() const;

        QString dbgShort();
};

/**
 * @brief Wrapper class for Item_lv2_iterator_base and derived classes
 *
 * This wrapper is needed because I want to be able to instantiate iterators as local variables (not pointers), so that
 * they are automatically deleted (freed) when leaving the block. If Item_lv2_iterator_base classes were directly instantiated
 * as pointers, using new, a separate delete instruction would be necessary to delete them at the end of the block.
 */
class Item_lv2_iterator{
        friend class Item_lv2;
    private:
        std::shared_ptr<Item_lv2_iterator_base> m_it;

        Item_lv2_iterator(){}
        Item_lv2_iterator(const Item_lv2_iterator& p_copiand);
        Item_lv2_iterator(std::shared_ptr<Item_lv2_iterator_base> p_it){m_it = p_it;}
    public:
        ~Item_lv2_iterator();
        Item_lv2_iterator& operator=(const Item_lv2_iterator& p_assignand);

        bool isNull() const {return m_it->isNull();}
        void next(){m_it->next();}
        bool beyondEnd() const;
        Item_lv2* at() const {return m_it->at();}
};

} // end namespace M1Store

    QDebug operator<<(QDebug d, M1Store::Item_lv2 p);
    QDebug operator<<(QDebug d, M1Store::Item_lv2* p);

#endif // M1_LV2_STORE_H
