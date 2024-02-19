#ifndef M1_LV2_STORE_H
#define M1_LV2_STORE_H

#include "m1_lv0_item.h"
#include <set>

Q_DECLARE_LOGGING_CATEGORY(g_cat_wrapper)
Q_DECLARE_LOGGING_CATEGORY(g_table_dumping)

namespace M1Store{
    class ItemWrapperFullEdge;

    class ItemWrapper{
        friend class ItemWrapperFullVertex;
        friend class ItemWrapperSimpleVertex;
        friend class ItemWrapperFullEdge;
        friend class ItemWrapperSimpleEdge;
    private:
        Item_lv0* m_item;
        bool m_must_delete = false;

        static ItemWrapper* instantiateFromMMap(Item_lv0* p);

        virtual void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        virtual void createTypeEdges();
        virtual void defaultConnections(){}
    public:
        // ------------------------------- Static --------------------------------------------------------
        static ItemWrapperFullEdge* getNewEdge(const FlagField p_flags, const ItemType& p_type);
        static ItemWrapper* getNew(const FlagField p_flags, const ItemType& p_type);
        static ItemWrapper* getNew(const FlagField p_flags, const ItemType& p_type, const char* p_label);
        static ItemWrapper* getNew(
            const FlagField p_flags, const ItemType p_type, const char* p_label,
            const FlagField p_flags_special, const char* p_mnemonic);
        static ItemWrapper* getExisting(const char* p_mnemonic);
        static ItemWrapper* getExisting(const ItemID p_ID);

        static void recurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left);

        // ------------------------------- Instances --------------------------------------------------------
        ItemWrapper(){}
        ItemWrapper(const ItemID p_ID, const FlagField p_flags, const ItemType p_type){
            m_item = new Item_lv0(p_ID, p_flags, p_type);
            m_must_delete = true;
        }
        ItemWrapper(Item_lv0* p_item);
        virtual ~ItemWrapper();

        virtual bool isFullVertex(){return (m_item->m_flags & ITEM_NATURE_MASK) == FULL_VERTEX;}
        virtual bool isSimpleVertex(){return (m_item->m_flags & ITEM_NATURE_MASK) == SIMPLE_VERTEX;}
        virtual bool isFullEdge(){return (m_item->m_flags & ITEM_NATURE_MASK) == FULL_EDGE;}
        virtual bool isSimpleEdge(){return (m_item->m_flags & ITEM_NATURE_MASK) == SIMPLE_EDGE;}

        virtual QString dbgString();
        QString dbgTypeShort();
        virtual QString dbgShort(int p_depth = 0);
        virtual QString dbgHalf();

        void setText(const QString& s){m_item->setText(s);}
        ItemID itemID(){return m_item->id();}

        void setType(const SpecialItem* p_type_si);
        void setType(const SpecialItemID p_type_id);
        void setType(const char* p_mnemonic);

        virtual void linkTo(ItemWrapper* p_target, const SpecialItemID p_type);
        virtual void linkTo(ItemWrapper* p_target, const char* p_mnemonic);
        virtual void linkTo(ItemID p_target_id, const SpecialItemID p_type);
        virtual void linkTo(ItemID p_target_id, const char* p_mnemonic);

        virtual ItemWrapper* getFirstEdge();
        virtual ItemWrapper* getFirstEdgeSpecial();
        virtual ItemWrapper* getNext();
        virtual void setNext(ItemID p_edge_id){m_item->setNext(p_edge_id);}
        virtual void setPrevious(ItemID p_edge_id){m_item->setPrevious(p_edge_id);}

        virtual ItemID getTarget(){return m_item->target();}
    };

    class ItemWrapperFullVertex : public virtual ItemWrapper{
    private:
        void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        void createTypeEdges();
        void defaultConnections();
    public:
        ItemWrapperFullVertex(Item_lv0* p_item);
        ~ItemWrapperFullVertex();

        virtual bool isFullVertex(){return true;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return false;}
        virtual bool isSimpleEdge(){return false;}

        void setFirstEdge(ItemID p_edge){m_item->setFirstEdge(p_edge);}
        void setFirstEdgeSpecial(ItemID p_edge){m_item->setFirstEdgeSpecial(p_edge);}

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();

        virtual ItemWrapper* getFirstEdge();
        virtual ItemWrapper* getFirstEdgeSpecial();
        ItemWrapper* getNext();
        void setNext(ItemID p_edge_id);
        void setPrevious(ItemID p_edge_id);
        ItemID getTarget();
    };

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

        ItemWrapper* getFirstEdge();
        ItemWrapper* getFirstEdgeSpecial();
        ItemWrapper* getNext();
        void setNext(ItemID p_edge_id);
        void setPrevious(ItemID p_edge_id);
        ItemID getTarget();
    };

    class ItemWrapperFullEdge : public virtual ItemWrapper{
    private:
        void addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type);
        void createTypeEdges();
        void defaultConnections();
    public:
        ItemWrapperFullEdge(Item_lv0* p_item);
        ~ItemWrapperFullEdge();

        virtual bool isFullVertex(){return false;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return true;}
        virtual bool isSimpleEdge(){return false;}

        void setTarget(ItemID p_target_id){m_item->setTarget(p_target_id);}
        void setOrigin(ItemID p_origin_id){m_item->setOrigin(p_origin_id);}
        void setReciprocal(ItemID p_reciprocal){m_item->setReciprocal(p_reciprocal);}

        ItemWrapper* getFirstEdge();
        ItemWrapper* getFirstEdgeSpecial();
        ItemWrapper* getNext();
        void setNext(ItemID p_edge_id){ItemWrapper::setNext(p_edge_id);}
        void setPrevious(ItemID p_edge_id){ItemWrapper::setPrevious(p_edge_id);}
        ItemID getTarget();

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();
    };

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

        void setOrigin(ItemID p_origin_id){m_item->setOrigin(p_origin_id);}
        ItemWrapper* getFirstEdge();
        ItemWrapper* getFirstEdgeSpecial();
        ItemWrapper* getNext();
        void setNext(ItemID p_edge_id){ ItemWrapper::setNext(p_edge_id);}
        void setPrevious(ItemID p_edge_id){ItemWrapper::setPrevious(p_edge_id);}
        ItemID getTarget();

        QString dbgString();
        QString dbgShort(int p_depth = 0);
        QString dbgHalf();
    };
}

#endif // M1_LV2_STORE_H
