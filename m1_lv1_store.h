#include "m1_lv0_store.h"

#ifndef M1_LV1_STORE_H
#define M1_LV1_STORE_H

namespace M1Store{

    class ItemWrapper{
        friend class ItemWrapperFullVertex;
        friend class ItemWrapperSimpleVertex;
        friend class ItemWrapperFullEdge;
        friend class ItemWrapperSimpleEdge;
    private:
        Item* m_item;
        bool m_must_delete = false;

        static ItemWrapper* instantiateFromMMap(Item* p);
    public:
        ItemWrapper(){}
        ItemWrapper(const ItemID p_ID, const FlagField p_flags, const ItemType p_type){
            m_item = new Item(p_ID, p_flags, p_type);
            m_must_delete = true;
        }
        ItemWrapper(Item* p_item){m_item = p_item;}
        virtual ~ItemWrapper();

        virtual bool isFullVertex(){return (m_item->m_flags & ITEM_NATURE_MASK) == FULL_VERTEX;}
        virtual bool isSimpleVertex(){return (m_item->m_flags & ITEM_NATURE_MASK) == SIMPLE_VERTEX;}
        virtual bool isFullEdge(){return (m_item->m_flags & ITEM_NATURE_MASK) == FULL_EDGE;}
        virtual bool isSimpleEdge(){return (m_item->m_flags & ITEM_NATURE_MASK) == SIMPLE_EDGE;}

        virtual void initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);

        virtual QString dbgString();

        void setText(const QString& s){m_item->setText(s);}

        static ItemWrapper* instantiateFromMMap(void* p);
        static ItemWrapper* instantiateFromMMap(void* p, const ItemID p_ID, const FlagField p_flags, const ItemType p_type);
    };

    class ItemWrapperFullVertex : public virtual ItemWrapper{
    public:
        ItemWrapperFullVertex(Item* p_item) : ItemWrapper(p_item){}
        ~ItemWrapperFullVertex();

        virtual bool isFullVertex(){return true;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return false;}
        virtual bool isSimpleEdge(){return false;}

        virtual QString dbgString();
    };

    class ItemWrapperSimpleVertex : public virtual ItemWrapper{
    public:
        ItemWrapperSimpleVertex(Item* p_item) : ItemWrapper(p_item){}
        ~ItemWrapperSimpleVertex(){}

        virtual bool isFullVertex(){return false;}
        virtual bool isSimpleVertex(){return true;}
        virtual bool isFullEdge(){return false;}
        virtual bool isSimpleEdge(){return false;}

        virtual QString dbgString();
    };

    class ItemWrapperFullEdge : public virtual ItemWrapper{
    public:
        ItemWrapperFullEdge(Item* p_item) : ItemWrapper(p_item){}
        ~ItemWrapperFullEdge(){}

        virtual bool isFullVertex(){return false;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return true;}
        virtual bool isSimpleEdge(){return false;}

        virtual QString dbgString();
    };

    class ItemWrapperSimpleEdge : public virtual ItemWrapper{
    public:
        ItemWrapperSimpleEdge(Item* p_item) : ItemWrapper(p_item){}
        ~ItemWrapperSimpleEdge(){}

        virtual bool isFullVertex(){return false;}
        virtual bool isSimpleVertex(){return false;}
        virtual bool isFullEdge(){return false;}
        virtual bool isSimpleEdge(){return true;}

        virtual QString dbgString();
    };
}

#endif // M1_LV1_STORE_H
