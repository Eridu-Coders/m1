#include "m1A_env.h"
#include "m1B_lv2_item.h"
#include "m1B_store.h"
#include "m1B_graph_init.h"

Q_LOGGING_CATEGORY(g_cat_lv2_members, "lv2.constructors")

// ---------------------------------- member access -------------------------------------------

/*******************************************************************************/
/** \defgroup LV2retLV2p Item_lv2 methods returning newly instanciated Item_lv2*
 *  \ingroup LV2
 *  Most of these methods can return a nullptr (if the underlying ItemID is G_VOID_ITEM_ID).
 *  As a result, they have to be called within if() constructs to properly treat this case (see e.g. recurGraph())
 *
 *  Will trigger an abort if called on the wrong nature of Item
 *  @{
 */

/**
 * @brief (full items only) First Edge Item_lv2* or nullptr if none
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::getFirstEdge_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the first edge lv2 item pointer"))
    // no Q_ASSERT here bc the job is done by lv0
    M1Store::Item_lv2* l_ret = getExisting(firstEdge_item_id());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief (full vertices only) Auto Edge Item_lv2* or nullptr if none
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::getAutoEdge_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the auto edge lv2 item pointer"))
    // no Q_ASSERT here bc the job is done by lv0
    M1Store::Item_lv2* l_ret = getExisting(autoEdge_item_id());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief (full items only) First Special Edge Item_lv2* or nullptr if none
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::getFirstEdgeSpecial_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the first special edge lv2 item pointer"))
    // no Q_ASSERT here bc the job is done by lv0
    M1Store::Item_lv2* l_ret = getExisting(firstEdgeSpecial_item_id());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief (only for edges, full and simple) Item_lv2* for next edge. Does NOT ever return a nullptr
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::get_next_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the next edge lv2 item pointer"))
    Q_ASSERT_X(next_item_id() != G_VOID_ITEM_ID,
               "Item_lv2::getNext_lv2()", "next_item_id() == G_VOID_ITEM_ID");
    // no Q_ASSERTs abt whether this is an edge here bc the job is done by lv0

    M1Store::Item_lv2* l_ret = getExisting(next_item_id());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief (only for edges, full and simple) Item_lv2* for previous edge. Does NOT ever return a nullptr
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::get_previous_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the reciprocal edge lv2 item pointer"))
    Q_ASSERT_X(previous_item_id() != G_VOID_ITEM_ID,
               "Item_lv2::getPrevious_lv2()", "previous_item_id() == G_VOID_ITEM_ID");
    // no Q_ASSERTs abt whether this is an edge here bc the job is done by lv0

    M1Store::Item_lv2* l_ret = getExisting(previous_item_id());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief (only for full edges) Item_lv2* for reciprocal edge, if any, or a nullptr if no reciprocal
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::getReciprocalEdge_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the reciprocal edge lv2 item pointer"))
    Q_ASSERT_X(this->reciprocal_item_id() != G_VOID_ITEM_ID,
               "Item_lv2::getReciprocalEdge_lv2()", "reciprocal_item_id() == G_VOID_ITEM_ID");
    // no more Q_ASSERTs here bc the job is done by lv0

    M1Store::Item_lv2* l_ret = getExisting(reciprocal_item_id());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief (only for full edges) Item_lv2* for target.
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::getTarget_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the target lv2 item pointer of a full edge"))
    Q_ASSERT_X(this->target_item_id() != G_VOID_ITEM_ID,
               "Item_lv2::getTarget_lv2()", "target_item_id() == G_VOID_ITEM_ID");
    // no more Q_ASSERTs here bc the job is done by lv0

    M1Store::Item_lv2* l_ret = getExisting(this->target_item_id());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief (only for full edges) Item_lv2* for origin.
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::getOrigin_lv2() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Get the origin lv2 item pointer of a full edge"))
    Q_ASSERT_X(this->origin_item_id() != G_VOID_ITEM_ID,
               "Item_lv2::getTarget_lv2()", "target_item_id() == G_VOID_ITEM_ID");
    // no more Q_ASSERTs here bc the job is done by lv0

    M1Store::Item_lv2* l_ret = getExisting(this->origin_item_id());
    M1_FUNC_EXIT
    return l_ret;
}
/** @}*/
// end of \defgroup LV2retLV2p

// ---------------------------------- other methods (critical ones) -------------------------------------------

/***********************************************************************************************************/
/** \defgroup IOT2 Type Setting Testing (Both in Item_lv0::m_type and with ISA edges)
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
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Set type to: %1").arg(M1Store::Storage::getSpecialItemPointer(p_type_si_id)->mnemonic()))

    bool l_ret = setType(M1Store::Storage::getSpecialItemPointer(p_type_si_id));
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

    bool l_ret = setType(M1Store::Storage::getSpecialItemPointer(p_mnemonic));
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

/*
bool M1Store::Item_lv2::isOfType(const ItemID p_type_item_id) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Is of type (ItemID): %1 ?").arg(p_type_item_id))

    M1_FUNC_EXIT
    return false;
}*/

/**
 * @brief type test from Special Item ID
 * @param p_type_si_id SpecialItemID to be tested
 * @return boolean
 */
bool M1Store::Item_lv2::isOfType(const SpecialItemID p_type_si_id) const{
    M1_FUNC_ENTRY(g_cat_lv2_members,
                  QString("Is of type (SpecialItemID): %1 ?").arg(M1Store::Storage::getSpecialItemPointer(p_type_si_id)->mnemonic()))

    // type test within own m_type (lv0)
    bool l_ret = this->isOfType_member(p_type_si_id);
    // if type not found and there are special edges (and this is not a simple edge/vertex either)
    if( !l_ret && !this->isSimpleEdge() && !this->isSimpleVertex() && this->firstEdgeSpecial_item_id() != G_VOID_ITEM_ID){
        ItemID l_expected_target_id = Storage::getSpecialItemPointer(p_type_si_id)->itemId();
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

    bool l_ret = this->isOfType(M1Store::Storage::getSpecialItemPointer(p_mnemonic)->specialId());
    M1_FUNC_EXIT
    return l_ret;
}
/** @}*/
// end of \defgroup IOT2

/**
 * @brief M1Store::Item_lv2::setField
 * @param p_content
 * @param p_force_new
 * @param p_field_type_si
 * @param p_field_extra_type_si
 * @return
 */
// bool setField(const QString& p_content, const SpecialItem* p_field_type_si, const SpecialItem* p_field_extra_type_si = nullptr);
/*
bool M1Store::Item_lv2::setField(const QString& p_content,
                                 const bool p_force_new,
                                 const SpecialItem* p_field_type_si,
                                 const SpecialItem* p_field_extra_type_si){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Setting field of type [%1] to {%2}%3")
                                         .arg(p_field_type_si->mnemonic())
                                         .arg(p_content)
                                         .arg(p_field_extra_type_si == nullptr ? "" : QString(" Extra type: %1").arg(p_field_extra_type_si->mnemonic())))

    M1Store::Item_lv2* l_field_edge = nullptr;
    if(! p_force_new) l_field_edge = this->findFieldEdge(p_field_type_si);

    if(p_force_new || l_field_edge == nullptr){
        l_field_edge = getNew(M1Env::SIMPLE_EDGE, M1Store::ItemType(p_field_type_si->specialId()));
        l_field_edge->setOrigin_lv1(this->item_id());
        // below AUTO edge
        this->installFullEdge(l_field_edge, G_VOID_SI_ID, nullptr, false);
    }

    // does not REPLACE the previously set type (p_field_type_si) but may add a new one
    if(p_field_extra_type_si != nullptr && !l_field_edge->isOfType(p_field_extra_type_si))
        l_field_edge->setType(p_field_extra_type_si);

    // will always work regardless length of p_content bc simple edges accept arbitrary length strings
    l_field_edge->setText(p_content);

    M1_FUNC_EXIT
    return true;
}
bool M1Store::Item_lv2::setField(const QString& p_content, const bool p_force_new, const M1Env::SpecialItemID p_field_type_siid){
    return setField(p_content, p_force_new, M1Store::Storage::getSpecialItemPointer(p_field_type_siid));
}
*/

/**
 * @brief M1Store::Item_lv2::getIconSITypeID
 * @return
 */
M1Env::SpecialItemID M1Store::Item_lv2::getIconSITypeID(){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("get type ID in member for Icon display"))
    short l_ret = -1;
    for(int i=0; i<4; i++){
        SpecialItemID l_si_id = this->getType_si_id(i);
        if(l_si_id != M1Env::G_VOID_SI_ID &&
            (l_ret == -1 || (M1Store::Storage::getSpecialItemPointer(l_si_id)->flags() & M1Env::SI_IS_ICON_TYPE))) l_ret = l_si_id;
    }
    qCDebug(g_cat_lv2_members) << QString("l_ret [%1] %2").arg(l_ret).arg(M1Store::Storage::getSpecialItemPointer(static_cast<M1Env::SpecialItemID>(l_ret))->mnemonic());
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1Store::Item_lv2::setFieldInternal
 * @param p_content
 * @param p_force_new
 * @param p_edge
 * @param p_field_type_si
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::setFieldInternal(const QString& p_content,
                                                       const bool p_force_new,
                                                       const bool p_edge,
                                                       const M1Store::SpecialItem* p_field_type_si){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Setting field of type [%1] to {%2}%3%4")
                      .arg(p_field_type_si->mnemonic())
                      .arg(p_content)
                      .arg(p_force_new ? " Force New" : "")
                      .arg(p_edge ? " Edge Field" : " Special Vertex Field"))
    M1Store::Item_lv2* l_ret;

    M1Store::Item_lv2* l_field_edge = nullptr;
    if(! p_force_new) l_field_edge = this->findFieldEdge(p_field_type_si);

    if(p_force_new || l_field_edge == nullptr){
        // new creation
        if(p_edge){
            // Simple Edge field
            l_field_edge = getNew(M1Env::SIMPLE_EDGE, M1Store::ItemType(p_field_type_si->specialId()));
            l_field_edge->setOrigin_lv1(this->item_id());
            // below AUTO edge
            this->installFullEdge(l_field_edge, G_VOID_SI_ID, nullptr, false);
        }
        else{
            // Simple Vertex field
            M1Store::Item_lv2* l_field_vertex = getNew(M1Env::SIMPLE_VERTEX, M1Store::ItemType(p_field_type_si->specialId()));
            // also below AUTO edge
            l_field_edge = this->linkTo(l_field_vertex, p_field_type_si->specialId());
        }
    }

    // set content value
    if(p_edge)
        // will always work regardless of p_content's length bc simple edges accept arbitrary length strings
        l_field_edge->setText_lv1(p_content);
    else {
        // content must fit in a Simple Vertex text area
        Q_ASSERT_X(p_content.length() < M1Env::SIMPLE_VERTEX_TEXT_LEN, "Item_lv2::linkTo()", "content too big for a simple vertex");
        l_field_edge->getTarget_lv2()->setText_lv1(p_content);
    }

    M1_FUNC_EXIT
        return l_field_edge;
}
M1Store::Item_lv2* M1Store::Item_lv2::setFieldVertex(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, false, false, M1Store::Storage::getSpecialItemPointer(p_field_type_siid));
}
M1Store::Item_lv2* M1Store::Item_lv2::setFieldVertexForce(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, true, false, M1Store::Storage::getSpecialItemPointer(p_field_type_siid));
}
M1Store::Item_lv2* M1Store::Item_lv2::setFieldEdge(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, false, true, M1Store::Storage::getSpecialItemPointer(p_field_type_siid));
}
M1Store::Item_lv2* M1Store::Item_lv2::setFieldEdgeForce(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, true, true, M1Store::Storage::getSpecialItemPointer(p_field_type_siid));
}

/**
 * @brief M1Store::Item_lv2::getFieldEdge
 * @param p_field_type_si
 * @param p_field_type2_si
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::getFieldEdge(
    const SpecialItem* p_field_type_si,
    const SpecialItem* p_field_type2_si) const{

    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Getting first field edge of type [%1]%2")
                      .arg(M1Store::Storage::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic())
                      .arg(p_field_type2_si == nullptr ? "" : QString(" and [%1]").arg(
                                                                  M1Store::Storage::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic())))

    M1Store::Item_lv2* l_ret = nullptr;
    for(Item_lv2_iterator it = this->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isOfType(p_field_type_si) && (p_field_type2_si == nullptr || it.at()->isOfType(p_field_type2_si))){
            l_ret = it.at();
            break;
        }

    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief [Private]
 * @param p_field_type_si
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::findFieldEdge(const SpecialItem* p_field_type_si) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding field edge of type [%1] if any")
                      .arg(M1Store::Storage::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic()))
    M1Store::Item_lv2* l_ret = nullptr;
    for(Item_lv2_iterator it = this->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isOfType(p_field_type_si)){
            l_ret = it.at();
            break;
        }

    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1Store::Item_lv2::getField
 * @param p_field_type_si
 * @return
 */
QString M1Store::Item_lv2::getField(const SpecialItem* p_field_type_si, const SpecialItem* p_field_type2_si, const bool p_all) const {
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Getting %3 of type [%1]%2")
                      .arg(M1Store::Storage::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic())
                      .arg(p_field_type2_si == nullptr ? "" : QString(" and [%1]").arg(
                                                                  M1Store::Storage::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic()))
                      .arg(p_all ? "concatenated values of fields" : "value of field"))

    QString l_ret;
    for(Item_lv2_iterator it = this->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isOfType(p_field_type_si) && (p_field_type2_si == nullptr || it.at()->isOfType(p_field_type2_si))){
            QString l_payload;

            if(it.at()->isFullEdge()) l_payload = it.at()->getTarget_lv2()->text();
            else l_payload = it.at()->text();

            if(p_all)
                if(l_ret.length() == 0)
                    l_ret = l_payload;
                else
                    l_ret += QString("/%1").arg(l_payload);
            else{
                l_ret = l_payload;
                break;
            }
        }
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1Store::Item_lv2::find_edge
 * @param p_type_edge
 * @param p_type_target
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::find_edge(const SpecialItemID p_type_edge, const SpecialItemID p_type_target, bool p_special) const{
    M1_FUNC_ENTRY(g_cat_lv2_members,
                  QString("Finding edge of type [%1] and with target of type [%2]")
                      .arg(p_type_edge == M1Env::G_VOID_SI_ID ? "None" : M1Store::Storage::getSpecialItemPointer(p_type_edge)->mnemonic() )
                      .arg(p_type_target == M1Env::G_VOID_SI_ID ? "None" : M1Store::Storage::getSpecialItemPointer(p_type_target)->mnemonic()))

    M1Store::Item_lv2* l_ret = nullptr;
    for(Item_lv2_iterator it = p_special ? this->getIteratorSpecial() : this->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge()){
            qCDebug(g_cat_lv2_members) << "it.at():" << it.at()->dbgShort();
            bool l_success_condition_edge = p_type_edge == M1Env::G_VOID_SI_ID || it.at()->isOfType(p_type_edge);
            bool l_success_condition_target = p_type_target == M1Env::G_VOID_SI_ID || it.at()->getTarget_lv2()->isOfType(p_type_target);
            if(l_success_condition_edge && l_success_condition_target){
                l_ret = it.at();
                qCDebug(g_cat_lv2_members) << "Found" << it.at()->getTarget_lv2()->text();
                break;
            }
        }
    }
    M1_FUNC_EXIT
    return l_ret;
}
/**
 * @brief M1Store::Item_lv2::find_edge_edge
 * @param p_type
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_edge(const M1Env::SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding edge of type [%1]").arg(M1Store::Storage::getSpecialItemPointer(p_type)->mnemonic()))
    M1Store::Item_lv2* l_ret = this->find_edge(p_type, M1Env::G_VOID_SI_ID);
    M1_FUNC_EXIT
    return l_ret;
}
/**
 * @brief M1Store::Item_lv2::find_edge_target
 * @param p_type
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_target(const M1Env::SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding edge of any type with target of type [%1]").arg(M1Store::Storage::getSpecialItemPointer(p_type)->mnemonic()))
    M1Store::Item_lv2* l_ret = this->find_edge(M1Env::G_VOID_SI_ID, p_type);
    M1_FUNC_EXIT
    return l_ret;
}


