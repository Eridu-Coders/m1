#include "m1A_env.h"
#include "m1B_lv1_item.h"

Q_LOGGING_CATEGORY(g_cat_lv1_members, "lv1.members_access")

void M1Store::Item_lv1::initializeMembers(const ItemID p_item_id,
                                          const FlagField p_flags,
                                          const ItemType p_type){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::initializeMembers(p_item_id, p_flags, p_type);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::initializeMembers(){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::initializeMembers();
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setFlags(const FlagField p_flags, const bool p_force_init){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFlags(p_flags, p_force_init);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setFlag(const FlagField p_flag, const bool p_force_init){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFlag(p_flag, p_force_init);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::unSetFlag(const FlagField p_flag, const bool p_force_init){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::unSetFlag(p_flag, p_force_init);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setType_member_si_id(const unsigned int p_index, const SpecialItemID p_type){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setType_member_si_id(p_index, p_type);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setType_member_item_id(const ItemID p_type_item_id){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setType_member_item_id(p_type_item_id);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setFlagsExtra(const FlagField p_flags){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFlagsExtra(p_flags);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setFlagExtra(const FlagField p_flag){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFlagExtra(p_flag);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::unSetFlagExtra(const FlagField p_flag){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::unSetFlagExtra(p_flag);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setOrigin(const ItemID p_origin){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setOrigin(p_origin);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setTarget(const ItemID p_target){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setTarget(p_target);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setPrevious(const ItemID p_previous){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setPrevious(p_previous);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setNext(const ItemID p_next){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setNext(p_next);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setReciprocal(const ItemID p_reciprocal){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setReciprocal(p_reciprocal);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setFirstEdge(const ItemID p_first_edge){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFirstEdge(p_first_edge);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setAutoEdge(const ItemID p_first_edge){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setAutoEdge(p_first_edge);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setFirstEdgeSpecial(const ItemID p_first_edge_special){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setFirstEdgeSpecial(p_first_edge_special);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setCreationDate(const QDateTime& p_date){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setCreationDate(p_date);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::setLastmodDate(const QDateTime& p_date){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setLastmodDate(p_date);
    M1_FUNC_EXIT
}

void M1Store::Item_lv1::setIncomingEdges(const ItemCounter p_incoming_edges){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setIncomingEdges(p_incoming_edges);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::addIncomingEdges(const ItemCounter p_add){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::addIncomingEdges(p_add);
    M1_FUNC_EXIT
} // can be negative

void M1Store::Item_lv1::setVisibleEdges(const ItemCounter p_visible_edges){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setVisibleEdges(p_visible_edges);
    M1_FUNC_EXIT
}
void M1Store::Item_lv1::addVisibleEdges(const ItemCounter p_add){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::addVisibleEdges(p_add);
    M1_FUNC_EXIT
} // can be negative

void M1Store::Item_lv1::setText(const QString& p_text){
    M1_FUNC_ENTRY(g_cat_lv1_members, "Entry")
    M1Store::Item_lv0::setText(p_text);
    M1_FUNC_EXIT
}
