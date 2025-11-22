#include "m1A_env.h"
#include "m1B_lv1_item.h"

Q_LOGGING_CATEGORY(g_cat_lv1_members, "lv1.members_access")

/**
 * \defgroup ArchivLog Storing of Achive Log data for replay in case of crash
 * \ingroup LV1
 * @{
 */
void M1Store::Item_lv1::initializeMembers_lv1(const ItemID p_item_id,
                                          const FlagField p_flags,
                                          const ItemType p_type){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::initializeMembers_lv0(p_item_id, p_flags, p_type);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::initializeMembers_lv1(){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::initializeMembers_lv0();
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setType_member_lv1(const ItemType& p_type){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setType_member_lv0(p_type);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setFlags_lv1(const FlagField p_flags, const bool p_force_init){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFlags_lv0(p_flags, p_force_init);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setFlagsExtra_lv1(const FlagField p_flags){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFlagsExtra_lv0(p_flags);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setOrigin_lv1(const ItemID p_origin){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setOrigin_lv0(p_origin);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setTarget_lv1(const ItemID p_target){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setTarget_lv0(p_target);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setPrevious_lv1(const ItemID p_previous){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setPrevious_lv0(p_previous);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setNext_lv1(const ItemID p_next){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setNext_lv0(p_next);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setReciprocal_lv1(const ItemID p_reciprocal){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setReciprocal_lv0(p_reciprocal);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setFirstEdge_lv1(const ItemID p_first_edge){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFirstEdge_lv0(p_first_edge);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setAutoEdge_lv1(const ItemID p_first_edge){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setAutoEdge_lv0(p_first_edge);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setFirstEdgeSpecial_lv1(const ItemID p_first_edge_special){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFirstEdgeSpecial_lv0(p_first_edge_special);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setCreationDate_lv1(const QDateTime& p_date){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setCreationDate_lv0(p_date);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setLastmodDate_lv1(const QDateTime& p_date){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setLastmodDate_lv0(p_date);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setIncomingEdges_lv1(const ItemCounter p_incoming_edges){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setIncomingEdges_lv0(p_incoming_edges);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setText_lv1(const QString& p_text){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setText_lv0(p_text);
    M1_FUNC_EXIT
}
/**@}*/ //end of ArchivLog
