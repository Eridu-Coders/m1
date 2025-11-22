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
     * \ingroup LV1
     */
    class Item_lv1 : public Item_lv0 {
    private:
    public:
        /// Does nothing. The class is not supposed to be constructed but cast from an mmap() area pointer
        Item_lv1(){}
        /// For testing purposes only
        Item_lv1(const ItemID p_item_id,
                 const FlagField p_flags,
                 const ItemType p_type);

        /**
         * \defgroup ArchivLog Achive Log data storage for replay in case of crash
         * \ingroup LV1
         * @{
         */
        void initializeMembers_lv1(const ItemID p_item_id,
                               const FlagField p_flags,
                               const ItemType p_type);
        void initializeMembers_lv1();

        void setType_member_lv1(const ItemType& p_type);
        void setFlags_lv1(const FlagField p_flags, const bool p_force_init = false);
        void setFlagsExtra_lv1(const FlagField p_flags);
        void setOrigin_lv1(const ItemID p_origin);
        void setTarget_lv1(const ItemID p_target);
        void setPrevious_lv1(const ItemID p_previous);
        void setNext_lv1(const ItemID p_next);
        void setReciprocal_lv1(const ItemID p_reciprocal);
        void setFirstEdge_lv1(const ItemID p_first_edge);
        void setAutoEdge_lv1(const ItemID p_auto_edge);
        void setFirstEdgeSpecial_lv1(const ItemID p_first_edge_special);
        void setCreationDate_lv1(const QDateTime& p_date);
        void setLastmodDate_lv1(const QDateTime& p_date);
        void setIncomingEdges_lv1(const ItemCounter p_incoming_edges);
        void setText_lv1(const QString& p_text);
        /**@}*/ //end of ArchivLog

        /**
         * \defgroup Higher_lv1 Higher level function within Item_lv1
         * @ingroup LV1
         * @{
         */
        void setType_member_si_id(const unsigned int p_index, const SpecialItemID p_type);
        void setType_member_item_id(const ItemID p_type_item_id);
        void setFlag(const FlagField p_flag, const bool p_force_init = false);
        void unSetFlag(const FlagField p_flag, const bool p_force_init = false);
        void setFlagExtra(const FlagField p_flag);
        void unSetFlagExtra(const FlagField p_flag);
        void storeSpecialItemID(const SpecialItemID p_si_id);
        void addIncomingEdges(const ItemCounter p_add); // can be negative
        /**@}*/ //end of Higher_lv1
    };
} // end namespace M1Store

/**
 * \defgroup OperatorOverload_lv1 << Operator overloading for debug purposes (Item_lv1)
 * @ingroup DebugLog
 * @{
 */
    QDebug operator<<(QDebug d, M1Store::Item_lv1 p);
    QDebug operator<<(QDebug d, M1Store::Item_lv1* p);
/**@}*/ //end of OperatorOverload_lv1

#endif // M1_LV1_ITEM_H
