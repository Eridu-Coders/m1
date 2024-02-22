#ifndef M1_LV2_STORE_H
#define M1_LV2_STORE_H

#include <set>

#include "m1_lv0_item.h"

namespace M1Store{
    // pre-declaration required for addFullEdge() in ItemWrapper
    class ItemWrapperFullEdge;

    /**
     * @brief The ItemWrapper class handles the basic interactions between items, i.e. mainly their connections by means of edges.
     */
    class ItemWrapper{
        friend class ItemWrapperFullVertex;
        friend class ItemWrapperSimpleVertex;
        friend class ItemWrapperFullEdge;
        friend class ItemWrapperSimpleEdge;
    private:
        /// single member in release build
        Item_lv0* m_item;
#if CMAKE_BUILD_TYPE == Debug
        /// used for test purposes only
        bool m_must_delete = false;
#endif
        static ItemWrapper* mbd_instantiateFromMMap(Item_lv0* p);
        static void dbgRecurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left);

        virtual void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        virtual void createTypeEdges();
        virtual void defaultConnections(){}
    public:
        // ------------------------------- Static --------------------------------------------------------
        static ItemWrapper* mbd_getNew(const FlagField p_flags, const ItemType& p_type);
        static ItemWrapper* mbd_getNew(const FlagField p_flags, const ItemType& p_type, const char* p_label);
        static ItemWrapper* mbd_getNew(
            const FlagField p_flags, const ItemType p_type, const char* p_label,
            const FlagField p_flags_special, const char* p_mnemonic);
        static ItemWrapper* mbd_getExisting(const char* p_mnemonic);
        static ItemWrapper* mbd_getExisting(const ItemID p_ID);

        static void dbgRecurGraphStart(const ItemID p_item_id);

        // ------------------------------- Instances --------------------------------------------------------
        /// zero constructor - never used
        ItemWrapper(){}
        /**
         * @brief full constructor - used only for testing purposes.
         *
         * Normally, instantiateFromMMap(), which calls ItemWrapper(Item_lv0*) and its category-dependent descendants, is used instead
         *
         * @param p_item_id the ItemID
         * @param p_flags flags
         * @param p_type ItemType instance
         */
        ItemWrapper(const ItemID p_item_id, const FlagField p_flags, const ItemType p_type){
            // instanciates an Item_lv0 on the heap, i.e. NOT in the mmap() area
            m_item = new Item_lv0(p_item_id, p_flags, p_type);
#if CMAKE_BUILD_TYPE == Debug
            m_must_delete = true;
#endif
        }
        ItemWrapper(Item_lv0* p_item);
        virtual ~ItemWrapper();

        /// normally never called (only the descendents are called)
        virtual bool isFullVertex(){return (m_item->m_flags & ITEM_NATURE_MASK) == FULL_VERTEX;}
        /// normally never called (only the descendents are called)
        virtual bool isSimpleVertex(){return (m_item->m_flags & ITEM_NATURE_MASK) == SIMPLE_VERTEX;}
        /// normally never called (only the descendents are called)
        virtual bool isFullEdge(){return (m_item->m_flags & ITEM_NATURE_MASK) == FULL_EDGE;}
        /// normally never called (only the descendents are called)
        virtual bool isSimpleEdge(){return (m_item->m_flags & ITEM_NATURE_MASK) == SIMPLE_EDGE;}

        virtual QString dbgString();
        QString dbgTypeShort();
        virtual QString dbgShort(int p_depth = 0);
        virtual QString dbgHalf();

        /// text setter - calls the underlying member from m_item
        void setText(const QString& s){m_item->setText(s);}
        /// ItemID getter - calls the underlying member from m_item
        ItemID itemID(){return m_item->item_id();}

        virtual void setNext(ItemID p_edge_id);
        virtual void setPrevious(ItemID p_edge_id);
        virtual ItemID getTargetID();
        virtual ItemID getFirstEdgeID();
        virtual ItemWrapper* mbd_getFirstEdgeWrapper();
        virtual ItemWrapper* mbd_getFirstEdgeSpecialWrapper();
        virtual ItemWrapper* mbd_getNextWrapper();

        bool setType(const SpecialItem* p_type_si);
        bool setType(const SpecialItemID p_type_id);
        bool setType(const char* p_mnemonic);

        virtual void linkTo(ItemWrapper* p_target, const SpecialItemID p_type);
        virtual void linkTo(ItemWrapper* p_target, const char* p_mnemonic);
        virtual void linkTo(ItemID p_target_id, const SpecialItemID p_type);
        virtual void linkTo(ItemID p_target_id, const char* p_mnemonic);
    };

    /**
     * @brief derived class for full vertices only
     */
    class ItemWrapperFullVertex : public virtual ItemWrapper{
    private:
        void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        void createTypeEdges();
        void defaultConnections();
    public:
        ItemWrapperFullVertex(Item_lv0* p_item);
        ~ItemWrapperFullVertex(){}

        virtual bool isFullVertex(){return true;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return false;}
        virtual bool isSimpleEdge(){return false;}

        void setFirstEdge(ItemID p_edge){m_item->setFirstEdge(p_edge);}
        void setFirstEdgeSpecial(ItemID p_edge){m_item->setFirstEdgeSpecial(p_edge);}

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();

        ItemID getFirstEdgeID();
        virtual ItemWrapper* mbd_getFirstEdgeWrapper();
        virtual ItemWrapper* mbd_getFirstEdgeSpecialWrapper();
        ItemWrapper* mbd_getNextWrapper();
        void setNext(ItemID p_edge_id);
        void setPrevious(ItemID p_edge_id);
        ItemID getTargetID();
    };

    /**
     * @brief derived class for simple vertices only
     */
    class ItemWrapperSimpleVertex : public virtual ItemWrapper{
    private:
        void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        void createTypeEdges();
        void defaultConnections();
    public:
        ItemWrapperSimpleVertex(Item_lv0* p_item);
        ~ItemWrapperSimpleVertex(){}

        virtual bool isFullVertex(){return false;}
        virtual bool isSimpleVertex(){return true;}
        virtual bool isFullEdge(){return false;}
        virtual bool isSimpleEdge(){return false;}

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();

        ItemID getFirstEdgeID();
        ItemWrapper* mbd_getFirstEdgeWrapper();
        ItemWrapper* mbd_getFirstEdgeSpecialWrapper();
        ItemWrapper* mbd_getNextWrapper();
        void setNext(ItemID p_edge_id);
        void setPrevious(ItemID p_edge_id);
        ItemID getTargetID();
    };

    /**
     * @brief derived class for full edges only
     */
    class ItemWrapperFullEdge : public virtual ItemWrapper{
    private:
        void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        void createTypeEdges();
        void defaultConnections();
    public:
        ItemWrapperFullEdge(Item_lv0* p_item);
        ~ItemWrapperFullEdge(){}

        virtual bool isFullVertex(){return false;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return true;}
        virtual bool isSimpleEdge(){return false;}

        void setTarget(ItemID p_target_id){m_item->setTarget(p_target_id);}
        void setOrigin(ItemID p_origin_id){m_item->setOrigin(p_origin_id);}
        void setReciprocal(ItemID p_reciprocal){m_item->setReciprocal(p_reciprocal);}

        ItemID getFirstEdgeID();
        ItemWrapper* mbd_getFirstEdgeWrapper();
        ItemWrapper* mbd_getFirstEdgeSpecialWrapper();
        ItemWrapper* mbd_getNextWrapper();
        virtual void setNext(ItemID p_edge_id);
        virtual void setPrevious(ItemID p_edge_id);
        ItemID getTargetID();

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();
    };

    /**
     * @brief derived class for simple edges only
     */
    class ItemWrapperSimpleEdge : public virtual ItemWrapper{
    private:
        void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        void createTypeEdges();
        void defaultConnections();
    public:
        ItemWrapperSimpleEdge(Item_lv0* p_item);
        ~ItemWrapperSimpleEdge(){}

        virtual bool isFullVertex(){return false;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return false;}
        virtual bool isSimpleEdge(){return true;}

        ItemID getFirstEdgeID();
        void setOrigin(ItemID p_origin_id){m_item->setOrigin(p_origin_id);}
        ItemWrapper* mbd_getFirstEdgeWrapper();
        ItemWrapper* mbd_getFirstEdgeSpecialWrapper();
        ItemWrapper* mbd_getNextWrapper();
        ItemID getTargetID();
        virtual void setNext(ItemID p_edge_id);
        virtual void setPrevious(ItemID p_edge_id);

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();
    };
}

#endif // M1_LV2_STORE_H
