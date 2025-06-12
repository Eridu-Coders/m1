#ifndef M1_LV1_ITEM_H
#define M1_LV1_ITEM_H

#include <QDateTime>
#include <QLoggingCategory>

#include "m1A_constants.h"
#include "m1B_lv0_item.h"

namespace M1Store{

using namespace M1Env;
    /**
     * @brief The Item lv1 class - trapping and logging updates for undo and crash recovery
     */
    class Item_lv1 : public Item_lv0 {
    private:
    public:
        // Does nothing. The class is not supposed to be constructed but cast from an mmap() area pointer
        Item_lv1(){}
        // For testing purposes only
        Item_lv1(const ItemID p_item_id,
                 const FlagField p_flags,
                 const ItemType p_type);

        // ------------------------ Setters ---------------------------------------------------------
        // to be used in real situations when instantiating from mmap() area pointer
        void initializeMembers(const ItemID p_item_id,
                               const FlagField p_flags,
                               const ItemType p_type);
        void initializeMembers();

        void setFlags(const FlagField p_flags, const bool p_force_init = false);
        void setFlag(const FlagField p_flag, const bool p_force_init = false);
        void unSetFlag(const FlagField p_flag, const bool p_force_init = false);

        void setType_member_si_id(const unsigned int p_index, const SpecialItemID p_type);
        void setType_member_item_id(const ItemID p_type_item_id);

        void setFlagsExtra(const FlagField p_flags);
        void setFlagExtra(const FlagField p_flag);
        void unSetFlagExtra(const FlagField p_flag);

        void setOrigin(const ItemID p_origin);
        void setTarget(const ItemID p_target);
        void setPrevious(const ItemID p_previous);
        void setNext(const ItemID p_next);
        void setReciprocal(const ItemID p_reciprocal);
        void setFirstEdge(const ItemID p_first_edge);
        void setAutoEdge(const ItemID p_auto_edge);
        void setFirstEdgeSpecial(const ItemID p_first_edge_special);
        void setCreationDate(const QDateTime& p_date);
        void setLastmodDate(const QDateTime& p_date);

        void setIncomingEdges(const ItemCounter p_incoming_edges);
        void addIncomingEdges(const ItemCounter p_add); // can be negative

        void setText(const QString& p_text);
    };
} // end namespace M1Store

#endif // M1_LV1_ITEM_H
