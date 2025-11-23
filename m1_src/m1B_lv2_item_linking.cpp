#include "m1A_env.h"
#include "m1B_lv2_item.h"
#include "m1B_store.h"
#include "m1B_graph_init.h"

/******************************************/
/** \defgroup LITO linking items with edges
 *  \ingroup LV2
 *
 * The new edge is positioned below p_edge_above if this parameter is provided or at the top if p_at_top is set.
 * In case of conflict between these 2 parameters, p_edge_above is given preference.
 *
 * If p_edge_above is not provided (= nullptr), the new edge is positioned below
 * the auto edge (if there is one and the origin is a full vertex)
 * or at the top of the apropriate ring otherwise.
 *
 * If p_edge_above is provided, and both the new edge and p_edge_above have a reciprocal edge, then the reciprocal of the new edge
 * will be positionned below the reciprocal of p_edge_above, unless p_at_top is set, in which case, it will be positioned at the
 * top of the appropriate ring of the target element.
 * @{
 */

/**
 * @brief link an item to another
 *
 * If p_edge_above = nullptr, the edge is positioned below the auto edge (if there is one)
 * or at the top of the apropriate ring otherwise.
 *
 * @param p_target the target (as an ItemWrapper)
 * @param p_type_edge the edge type (as a SpecialItemID)
 * @param p_edge_above edge below which the new edge is to be inserted.
 * @param p_at_top if true, inser tnew edge  at top of relevant ring
 * @return the new edge (Item_lv2*)
 */
M1Store::Item_lv2* M1Store::Item_lv2::linkTo(Item_lv2* p_target, const SpecialItemID p_type_edge, Item_lv2* p_edge_above, bool p_at_top){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("{%3} link to: {%1} [%2]")
                      .arg(p_target->dbgShort())
                      .arg(M1Store::Storage::getSpecialItemPointer(p_type_edge)->mnemonic())
                      .arg(this->dbgShort()))
    // cannot link simple items to anything
    Q_ASSERT_X(isFullVertex() || isFullEdge(), "Item_lv2::linkTo()", "cannot link simple vertices or edges to anything");

    // the new edge from this item to the target
    M1Store::Item_lv2* l_new_edge =
        getNew(FULL_EDGE, M1Store::ItemType(p_type_edge));
    l_new_edge->setTarget_lv1(p_target->item_id());
    l_new_edge->setOrigin_lv1(this->item_id());

    // creation of a reciprocal edge if necessary
    if(M1Store::Storage::getSpecialItemPointer(p_type_edge)->flags() & SI_HAS_RECIPROCAL){
        // the reciprocal edge
        SpecialItemID l_reciprocal_type_si_id = M1Store::Storage::getSpecialItemPointer(p_type_edge)->reciprocalSpecialId();
        M1Store::Item_lv2* l_new_edge_reciprocal =
            getNew(FULL_EDGE, M1Store::ItemType(l_reciprocal_type_si_id));
        l_new_edge_reciprocal->setOrigin_lv1(p_target->item_id());
        l_new_edge_reciprocal->setTarget_lv1(this->item_id());

        // mutual reciprocal hook-up of the 2 edges
        l_new_edge->setReciprocal_lv1(l_new_edge_reciprocal->item_id());
        l_new_edge_reciprocal->setReciprocal_lv1(l_new_edge->item_id());

        qCDebug(g_cat_lv2_members) << QString("Reciprocal edge created: %1 --> %2")
                                          .arg(l_new_edge->dbgTypeShort())
                                          .arg(l_new_edge_reciprocal->dbgTypeShort());

        // it is IMPOSSIBLE to determine a "reciprocal edge above" from p_edge_above
        // only edge positioning available is from the special item flag SI_INSERT_AT_TOP of the reciprocal type
        // if it is not set. the insertion is below the autp edge by default
        p_target->installFullEdge(l_new_edge_reciprocal,
                                  l_reciprocal_type_si_id,
                                  nullptr, // no edge above
                                  M1Store::Storage::getSpecialItemPointer(l_reciprocal_type_si_id)->flags() & M1Env::SI_INSERT_AT_TOP);
    }
    // insert the direct edge into one of the edge rings (special or ordinary) of this item
    this->installFullEdge(l_new_edge, p_type_edge, p_edge_above, p_at_top);

    qCDebug(g_cat_lv2_members) << QString("Returning new edge: %1").arg(l_new_edge->dbgShort());
    M1_FUNC_EXIT
        return l_new_edge;
}

/**
 * @brief insert a newly created edge into one of the 2 edge rings (special or ordinary) of this item
 *
 * If p_edge_above = nullptr, the edge is positioned below the auto edge (if there is one)
 * unless p_at_top is set or if there is no auto edge (like in the special edge ring)
 *
 * @param p_new_edge the newly created edge (as an Item_lv2*)
 * @param p_edge_type the edge type, as SpecialItemID
 * @param p_edge_above the existing edge below which the new one is to be inserted (or nullptr)
 * @param p_at_top if true, insert new edge at top of relevant ring, regardless of the presence of an auto edge
 */
// TODO: implement visible ISA edges
void M1Store::Item_lv2::installFullEdge(Item_lv2* p_new_edge, const SpecialItemID p_edge_type, Item_lv2* p_edge_above, bool p_at_top){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("{%3} adding full edge [%1] at top? %2")
                      .arg(p_new_edge->dbgShort())
                      .arg(p_at_top)
                      .arg(this->dbgShort()))

    // cannot link simple items to anything
    Q_ASSERT_X(this->isFullVertex() || this->isFullEdge(),
               "Item_lv2::installFullEdge()", "Cannot add an edge to a simple edge or vertex");

    // determine in which ring the edge is to be inserted
    bool l_edge_is_special =
        p_edge_type == G_VOID_SI_ID ? false : M1Store::Storage::getSpecialItemPointer(p_edge_type)->flags() & SI_IS_SPECIAL_EDGE;

    ItemID l_first_edge_id = l_edge_is_special ? this->firstEdgeSpecial_item_id() : this->firstEdge_item_id();

    // indicates whether the new edge is inserted at the top of the ring,
    // and must therefore take the place of the first edge (of the relevant ring)
    bool l_at_top = false;

    // id of the new edge
    ItemID l_new_edge_id = p_new_edge->item_id();
    if(p_edge_above == nullptr){
        qCDebug(g_cat_lv2_members) << QString("No edge above provided");
        // no edge above provided
        if(l_first_edge_id == G_VOID_ITEM_ID){ // case in which this item does not have any edge yet (in the relevant ring)
            qCDebug(g_cat_lv2_members) << QString("inserting in empty edge ring");
            l_at_top = true;

            // link the new edge to itself
            // no longer necessary bc done by default in createNew()
            // p_new_edge->setPrevious(l_new_edge_id);
            // p_new_edge->setNext(l_new_edge_id);
        }
        else{
            // determine DEFAULT edge above (needed bc p_edge_above is null here) based on the following situations ..
            M1Store::Item_lv2* l_edge_above;
            if(p_new_edge->getOrigin_lv2()->isFullVertex() && !l_edge_is_special  && !p_at_top){
                // .. BELOW the AUTO edge if the origin is a full vertex (and the edge type does not require a special edge)
                l_edge_above = p_new_edge->getOrigin_lv2()->getAutoEdge_lv2();
                l_at_top = false; // just to be sure
            } else {
                // .. ABOVE the current first edge (of the appropriate ring) otherwise (including p_at_top set)
                l_edge_above = this->getExisting(l_first_edge_id)->get_previous_lv2();
                // this would in fact insert the new edge at the BOTTOM because "above the first edge" is also the last (bc it is a ring)
                // however, the flag below will make sure that the new edge becomes the first edge and so it will indeed be at top
                l_at_top = true;
            }

            qCDebug(g_cat_lv2_members) << QString("l_at_top before insetion: %1").arg(l_at_top);
            insertEdgeBelow(p_new_edge, l_edge_above);
        }
    } else { // p_edge_above is not null (and p_at_top is therefore irrelevant)
        qCDebug(g_cat_lv2_members) << QString("[%1] Edge above was provided").arg(this->dbgShort());
        Q_ASSERT_X(l_first_edge_id != G_VOID_ITEM_ID,
                   "Item_lv2::installFullEdge()", "Cannot install an edge into an empty ring if edge above provided");
        Q_ASSERT_X(M1Env::SKIP_HEAVY_CHECKING || this->edgeBelongs(p_edge_above, l_edge_is_special),
                   "Item_lv2::installFullEdge()",
                   "the given edge above does not belong to this ring");

        l_at_top = false; // just to be sure
        // WRONG!! will be inserted at the top of the ring only if consistent with the edge above given
        // l_at_top = p_at_top && (p_edge_above->next_item_id() == l_first_edge_id);
        insertEdgeBelow(p_new_edge, p_edge_above);
    }

    if(l_at_top){
        qCDebug(g_cat_lv2_members) << QString("Set the new edge as first edge");
        // the edge becomes the new first edge of the appropriate ring
        if(l_edge_is_special)
            // add to the special edge ring
            this->setFirstEdgeSpecial_lv1(l_new_edge_id);
        else
            // add to the ordinary edge ring
            this->setFirstEdge_lv1(l_new_edge_id);
    }

    qCDebug(g_cat_lv2_constructors) << QString("%1 Installed").arg(p_new_edge->dbgShort());
    M1_FUNC_EXIT
}

/**
 * @brief [Static] Does the actual insertion work
 * @param p_new_edge
 * @param p_edge_above
 */
void M1Store::Item_lv2::insertEdgeBelow(Item_lv2* p_new_edge, Item_lv2* p_edge_above){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("inserting in (non empty) edge ring"))
    Q_ASSERT_X(p_edge_above != nullptr, "Item_lv2::insertEdgeBelow()", "p_edge_above is nullptr");
    M1Store::Item_lv2* l_next_edge = p_edge_above->get_next_lv2();

    // appropriate next/previous hookups
    p_new_edge->setPrevious_lv1(p_edge_above->item_id());
    p_new_edge->setNext_lv1(l_next_edge->item_id());

    p_edge_above->setNext_lv1(p_new_edge->item_id());
    l_next_edge->setPrevious_lv1(p_new_edge->item_id());

    M1_FUNC_EXIT
}

/**
 * @brief link an item to another
 * @param p_target the target (as an Item_lv2+)
 * @param p_mnemonic_edge the edge type (as a mnemonic)
 * @param p_edge_above the existing edge below which the new one is to be inserted (or nullptr)
 * @param p_at_top if true, inser tnew edge  at top of relevant ring
 * @return the new edge (Item_lv2*)
 */
M1Store::Item_lv2* M1Store::Item_lv2::linkTo(Item_lv2* p_target, const char* p_mnemonic_edge, Item_lv2* p_edge_above, bool p_at_top){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("link to: %1 [%2]")
                      .arg(p_target->dbgShort())
                      .arg(p_mnemonic_edge))

    M1Store::Item_lv2* l_ret = linkTo(p_target, Storage::getSpecialItemPointer(p_mnemonic_edge)->specialId(), p_edge_above, p_at_top);

    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief link an item to another
 * @param p_target_id the target (as an ItemID)
 * @param p_type the edge type (as a SpecialItemID)
 * @param p_edge_above the existing edge below which the new one is to be inserted (or nullptr)
 * @param p_at_top if true, inser tnew edge  at top of relevant ring
 * @return the new edge (Item_lv2*)
 */
M1Store::Item_lv2* M1Store::Item_lv2::linkTo(ItemID p_target_id, const SpecialItemID p_type, Item_lv2* p_edge_above, bool p_at_top){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("link to: %1 [%2]").arg(p_target_id).arg(p_type))
    Q_ASSERT_X(p_target_id != G_VOID_ITEM_ID,
               "Item_lv2::linkTo())", "p_target_id = G_VOID_ITEM_ID");

    M1Store::Item_lv2* l_target = getExisting(p_target_id);
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]")
                                      .arg(l_target->dbgShort())
                                      .arg(Storage::getSpecialItemPointer(p_type)->mnemonic());
    M1Store::Item_lv2* l_ret = linkTo(l_target, p_type, p_edge_above, p_at_top);

    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief link an item to another
 * @param p_target_id the target (as an ItemID)
 * @param p_mnemonic_edge the edge type (as a mnemonic)
 * @param p_edge_above the existing edge below which the new one is to be inserted (or nullptr)
 * @param p_at_top if true, inser tnew edge  at top of relevant ring
 * @return the new edge (Item_lv2*)
 */
M1Store::Item_lv2*  M1Store::Item_lv2::linkTo(ItemID p_target_id, const char* p_mnemonic_edge, Item_lv2* p_edge_above, bool p_at_top){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("link to: %1 [%2]").arg(p_target_id).arg(p_mnemonic_edge))
    Q_ASSERT_X(p_target_id != G_VOID_ITEM_ID,
               "Item_lv2::linkTo())", "p_target_id = G_VOID_ITEM_ID");

    M1Store::Item_lv2* l_target = getExisting(p_target_id);
    M1Store::Item_lv2* l_ret = linkTo(l_target, p_mnemonic_edge, p_edge_above, p_at_top);

    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief Checking whether an edge belongs to one of the rings of this item
 * @param p_edge edge to test
 * @param p_edge_is_special true --> edge is to be inserted in the special edges ring
 * @return true --> it does
 */
bool M1Store::Item_lv2::edgeBelongs(Item_lv2* p_edge, bool p_edge_is_special){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Checking whether this edge [%1] belongs to one of the rings of this item [%2]")
                      .arg(p_edge->dbgShort()).arg(this->dbgShort()))
    bool l_ret = false;

    bool l_has_appropriate_ring = p_edge_is_special ?
                                      (this->firstEdgeSpecial_item_id() != G_VOID_ITEM_ID) :
                                      (this->firstEdge_item_id() != G_VOID_ITEM_ID);
    if( l_has_appropriate_ring ){
        for(Item_lv2_iterator it = (p_edge_is_special ? this->getIteratorSpecial() : this->getIteratorTop()); !it.beyondEnd(); it.next()){
            qCDebug(g_cat_lv2_members) << QString("Current Edge: %1").arg(it.at()->dbgHalf());
            if(it.at()->item_id() == p_edge->item_id()){
                l_ret = true;
                break;
            }
        }
    }

    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief Create a new vertex linked to this item
 *
 * @param p_edge_type
 * @param p_label
 * @param p_vertex_type
 * @param p_edge_above
 * @param p_at_top
 * @return the newly created vertex
 */
M1Store::Item_lv2* M1Store::Item_lv2::create_descendant(
    const SpecialItemID p_edge_type,
    const QString& p_label,
    const SpecialItemID p_vertex_type,
    Item_lv2* p_edge_above, const bool p_at_top){

    M1_FUNC_ENTRY(g_cat_lv2_members,
                  QString("new descendant: %1 --{%2}--> [%3] %4")
                      .arg(this->dbgShort())
                      .arg(M1Store::Storage::getSpecialItemPointer(p_edge_type)->mnemonic())
                      .arg(M1Store::Storage::getSpecialItemPointer(p_vertex_type)->mnemonic())
                      .arg(p_label)
                  )
    // cannot give descendants to anything but full vertices or edge
    Q_ASSERT_X(isFullVertex() || isFullEdge(), "Item_lv2::create_descendant()", "cannot give descendants to simple vertices or edges");

    // the new vertex
    M1Store::Item_lv2* l_new_vertex =
        getNew(M1Store::FULL_VERTEX, M1Store::ItemType(p_vertex_type));
    l_new_vertex->setText_lv1(p_label);

    // link this to it
    // unless it is an ITO from this vertex down to the new one and this is the type vertex corresponding to p_vertex_type
    if( !((p_edge_type == M1Env::ITO_SIID) && (this->specialItemId() == p_vertex_type)) )
        this->linkTo(l_new_vertex, p_edge_type);

    M1_FUNC_EXIT
        return l_new_vertex;
}

/** @}*/
// end of \defgroup LITO
