#include "m1A_env.h"
#include "m1B_lv2_item.h"
#include "m1B_store.h"
#include "m1B_graph_init.h"

/***********************************************************************************************************/
/** \defgroup IOT2 Type Setting and Testing (Both in Item_lv0::m_type and with ISA edges)
 *  \ingroup LV2
 *
 *  Tests type values stored in Item_lv0::m_type AND ALSO the types embodied by ISA/ITO edges
 *
 *  @{
 */

/**
 * @brief Type assignment with a special item pointer
 *
 * 1) put the type into m_type (ItemType) if one of the 4 slots is available, regardless of whether the type has the SI_REQUIRES_EDGE attribut
 * 2) create an ISA/ITO edge if no slot was available in OR (inclusive or) SI_REQUIRES_EDGE is set
 *
 * @param p_type_si a SpecialItem pointer for the type to be assigned
 * @return true/false depending on success/failure
 */
bool M1Store::Item_lv2::setType(const SpecialItem* p_type_si){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Set type to: %1").arg(p_type_si->mnemonic()))
    Q_ASSERT_X(M1Env::SKIP_HEAVY_CHECKING || (! this->isOfType(p_type_si)), "Item_lv2::setType()", "Already of above type");

    bool l_ret = true;
    // index of first free slot (or -1 if none)
    int l_free_slot = \
                              getType_si_id(0) == G_VOID_SI_ID ? 0 :
                          (getType_si_id(1) == G_VOID_SI_ID ? 1 :
                               (getType_si_id(2) == G_VOID_SI_ID ? 2 :
                                    getType_si_id(3) == G_VOID_SI_ID ? 3 : -1));

    // store the special item id in m_type if possible
    if(l_free_slot >= 0)
        setType_member_si_id(l_free_slot, p_type_si->specialId());

    // cannot go any further for simple edges and simple verices (may return false if no slot was available)
    if(isSimpleEdge() || isSimpleVertex()){
        l_ret = (l_free_slot >= 0);
    }
    else {
        // full items only from this point onwards
        // create an ISA edge if required (no slot available or SI_REQUIRES_EDGE is set)
        if((l_free_slot < 0) || (p_type_si->flags() & SI_REQUIRES_EDGE))
            linkTo(p_type_si->itemId(), "_ISA_");
        // will always succeed for full edges and vertices (always possible to materialyse type with an ISA edge) so l_ret is always true
    }
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief Type assignment with a special item ID
 * @param p_type_id the SpecialItemID for the type to be assigned
 * @return true/false depending on success/failure
 */
bool M1Store::Item_lv2::setType(const SpecialItemID p_type_si_id){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Set type to: %1").arg(M1Store::StorageStatic::getSpecialItemPointer(p_type_si_id)->mnemonic()))

    bool l_ret = setType(M1Store::StorageStatic::getSpecialItemPointer(p_type_si_id));
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief Type assignment with a mnemonic
 * @param p_mnemonic the mnemonic of the type to be assigned
 * @return true/false depending on success/failure
 */
bool M1Store::Item_lv2::setType(const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Set type to: %1").arg(p_mnemonic))

    bool l_ret = setType(M1Store::StorageStatic::getSpecialItemPointer(p_mnemonic));
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1Store::Item_lv2::setType
 * @param p_type_lv2
 * @return
 */
bool M1Store::Item_lv2::setType(const Item_lv2* p_type_lv2){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Set type to: %1").arg(p_type_lv2->text()))

    bool l_ret;
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1Store::Item_lv2::isOfType
 * @param p_type_item_id
 * @return
 */
bool M1Store::Item_lv2::isOfType(const Item_lv2* p_type_lv2) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Is of type: %1?").arg(p_type_lv2->text()))

    bool l_ret;
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief type test from Special Item ID
 * @param p_type_si_id SpecialItemID to be tested
 * @return boolean
 */
bool M1Store::Item_lv2::isOfType(const SpecialItemID p_type_si_id) const{
    M1_FUNC_ENTRY(g_cat_lv2_members,
                  QString("Is of type (SpecialItemID): %1 ?").arg(M1Store::StorageStatic::getSpecialItemPointer(p_type_si_id)->mnemonic()))

    // type test within own m_type (lv0)
    bool l_ret = this->isOfType_member(p_type_si_id);
    // if type not found and there are special edges (and this is not a simple edge/vertex either)
    if( !l_ret && !this->isSimpleEdge() && !this->isSimpleVertex() && this->firstEdgeSpecial_item_id() != G_VOID_ITEM_ID){
        ItemID l_expected_target_id = StorageStatic::getSpecialItemPointer(p_type_si_id)->itemId();
        // if the type corresponds to an ItemID (otherwise, there cannot be ISA edges))
        if(l_expected_target_id != G_VOID_ITEM_ID){
            bool l_not_first = false;
            // loop through special edges ...
            for(Item_lv2* l_current_edge = this->getFirstEdgeSpecial_lv2();
                 !(l_not_first && (l_current_edge->item_id() == this->firstEdgeSpecial_item_id()));
                 l_current_edge = l_current_edge->get_next_lv2()){

                // ... until an edge of the from ---ISA---> (expected ItemID) is found
                if(l_current_edge->isOfType_member("_ISA_") && l_current_edge->target_item_id() == l_expected_target_id){
                    l_ret = true;
                    break;
                }
                l_not_first = true;
            }
        }
    }

    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief type test from Special Item pointer
 * @param p_type_si SpecialItem* to be tested
 * @return boolean
 */
bool M1Store::Item_lv2::isOfType(const SpecialItem* p_type_si) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Is of type (SpecialItem*): %1 ?").arg(p_type_si->mnemonic()))

    bool l_ret = this->isOfType(p_type_si->specialId());
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief type test from mnemonic
 * @param p_mnemonic string to be tested
 * @return boolean
 */
bool M1Store::Item_lv2::isOfType(const char* p_mnemonic) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Is of type (char*): %1 ?").arg(p_mnemonic))

    bool l_ret = this->isOfType(M1Store::StorageStatic::getSpecialItemPointer(p_mnemonic)->specialId());
    M1_FUNC_EXIT
        return l_ret;
}
/** @}*/
// end of \defgroup IOT2
