#include "m1A_env.h"
#include "m1B_lv2_iterators.h"
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
 * @return Item_lv2* of the first edge
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
 * @return Item_lv2* of the auto edge
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
 * @return Item_lv2* of the first special edge
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
 * @return Item_lv2* of the next edge
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
 * @return Item_lv2* of the previous edge
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
 * @return Item_lv2* of the reciprocal edge edge
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
 * @return Item_lv2* of the edge target
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
 * @return Item_lv2* of the edge origin
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
 * @brief Return the SpecialItemID to be used for Icon choice
 * @return the SpecialItemID
 */
M1Env::SpecialItemID M1Store::Item_lv2::getIconSITypeID() const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("get type ID in member for Icon display"))
    short l_ret = -1;
    for(int i=0; i<4; i++){
        SpecialItemID l_si_id = this->getType_si_id(i);
        if(l_si_id != M1Env::G_VOID_SI_ID &&
            (l_ret == -1 || (M1Store::StorageStatic::getSpecialItemPointer(l_si_id)->flags() & M1Env::SI_IS_ICON_TYPE))) l_ret = l_si_id;
    }
    qCDebug(g_cat_lv2_members) << QString("l_ret [%1] %2").arg(l_ret).arg(M1Store::StorageStatic::getSpecialItemPointer(static_cast<M1Env::SpecialItemID>(l_ret))->mnemonic());
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief Internal mechanics of the field setting process
 * @param p_content QString context to assign to the field
 * @param p_force_new true --> force the creation of a new field of this type even if one already exists, otherwise overwrite exixting field contents
 * @param p_edge true --> create a simple edge field, otherwise a full edge + simple vertex field
 * @param p_field_type_si type of the field to create (M1Store::SpecialItem*)
 * @return the newly created edge (full edge if simple vertex, simple edge otherwise)
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
/// create/add to simple vertex field
M1Store::Item_lv2* M1Store::Item_lv2::setFieldVertex(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, false, false, M1Store::StorageStatic::getSpecialItemPointer(p_field_type_siid));
}
/// create simple vertex field (force new creation)
M1Store::Item_lv2* M1Store::Item_lv2::setFieldVertexForceNew(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, true, false, M1Store::StorageStatic::getSpecialItemPointer(p_field_type_siid));
}
/// create/add to simple edge field
M1Store::Item_lv2* M1Store::Item_lv2::setFieldEdge(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, false, true, M1Store::StorageStatic::getSpecialItemPointer(p_field_type_siid));
}
/// create simple edge field (force new creation)
M1Store::Item_lv2* M1Store::Item_lv2::setFieldEdgeForceNew(const QString& p_content, const M1Store::SpecialItemID p_field_type_siid){
    return setFieldInternal(p_content, true, true, M1Store::StorageStatic::getSpecialItemPointer(p_field_type_siid));
}

/**
 * @brief Get the edge leading to a field
 * @param p_field_type_si edge type
 * @param p_field_type2_si optional second edge type (nullptr by default)
 * @return the simple edge if applicable or a full edge leading to a simple vertex otherwise
 */
M1Store::Item_lv2* M1Store::Item_lv2::getFieldEdge(
    const SpecialItem* p_field_type_si,
    const SpecialItem* p_field_type2_si) const{

    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Getting first field edge of type [%1]%2")
                      .arg(M1Store::StorageStatic::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic())
                      .arg(p_field_type2_si == nullptr ? "" : QString(" and [%1]").arg(
                                                                  M1Store::StorageStatic::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic())))

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
 * \todo delete this and replace with getFieldEdge()
 * @param p_field_type_si
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2::findFieldEdge(const SpecialItem* p_field_type_si) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding field edge of type [%1] if any")
                      .arg(M1Store::StorageStatic::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic()))
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
 * @brief Get field contents (only works on simple edge fields atm)
 *
 * \todo make this work for simple vertex fields as well
 *
 * @param p_field_type_si type of the field
 * @param p_field_type2_si optional second type of the field
 * @param p_all true --> concatenate all field contents of the given type(s)
 * @return the contents
 */
QString M1Store::Item_lv2::getField(const SpecialItem* p_field_type_si, const SpecialItem* p_field_type2_si, const bool p_all) const {
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Getting %3 of type [%1]%2")
                      .arg(M1Store::StorageStatic::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic())
                      .arg(p_field_type2_si == nullptr ? "" : QString(" and [%1]").arg(
                                                                  M1Store::StorageStatic::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic()))
                      .arg(p_all ? "concatenated values of fields" : "value of field"))

    QString l_ret;
    for(Item_lv2_iterator l_it = this->getIteratorTop(); !l_it.beyondEnd(); l_it.next()){
        qCDebug(g_cat_tmp_debug) << "it.at():" << l_it.at()->dbgShort();
        qCDebug(g_cat_tmp_debug) << "it.at()->isOfType(p_field_type_si):" << (l_it.at()->isOfType(p_field_type_si) ? "true" : "false");
        if(l_it.at()->isOfType(p_field_type_si) && (p_field_type2_si == nullptr || l_it.at()->isOfType(p_field_type2_si))){
            QString l_payload;

            if(l_it.at()->isFullEdge()) l_payload = l_it.at()->getTarget_lv2()->text();
            else l_payload = l_it.at()->text();

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
    }
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief Find an edge
 * @param p_type_edge edge type
 * @param p_type_target target type
 * @return the first edge meeting the criteria (or nullptr)
 */
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_generic(const SpecialItemID p_type_edge, const SpecialItemID p_type_target, bool p_special) const{
    M1_FUNC_ENTRY(g_cat_lv2_members,
                  QString("Finding edge of type [%1] and with target of type [%2]")
                      .arg(p_type_edge == M1Env::G_VOID_SI_ID ? "None" : M1Store::StorageStatic::getSpecialItemPointer(p_type_edge)->mnemonic() )
                      .arg(p_type_target == M1Env::G_VOID_SI_ID ? "None" : M1Store::StorageStatic::getSpecialItemPointer(p_type_target)->mnemonic()))

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
 * @brief find and edge based only on its type
 * @param p_type the edge type
 * @return the first edge meeting the criteria (or nullptr)
 */
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_edge_type(const M1Env::SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding edge of type [%1]").arg(M1Store::StorageStatic::getSpecialItemPointer(p_type)->mnemonic()))
    M1Store::Item_lv2* l_ret = this->find_edge_generic(p_type, M1Env::G_VOID_SI_ID);
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief find an edge based only on its target type type
 * @param p_type the type
 * @return the first edge meeting the criteria (or nullptr)
 */
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_target_type(const M1Env::SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding edge of any type with target of type [%1]").arg(M1Store::StorageStatic::getSpecialItemPointer(p_type)->mnemonic()))
    M1Store::Item_lv2* l_ret = this->find_edge_generic(M1Env::G_VOID_SI_ID, p_type);
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief find an edge of a given type with a certain target text
 * @param p_type the edge type
 * @param p_target_text the target text
 * @return the first edge meeting the criteria (or nullptr)
 */
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_target_string(const M1Env::SpecialItemID p_type_edge, const QString& p_target_text, bool p_special) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding edge of type [%1] with target string: [%2]")
                                         .arg(M1Store::StorageStatic::getSpecialItemPointer(p_type_edge)->mnemonic()).arg(p_target_text))
    M1Store::Item_lv2* l_ret = nullptr;
    for(Item_lv2_iterator l_it = p_special ? this->getIteratorSpecial(p_type_edge) : this->getIteratorTop(p_type_edge); !l_it.beyondEnd(); l_it.next()){
        qCDebug(g_cat_lv2_members) << "Examining: " << l_it.at()->getTarget_lv2()->text();
        if(l_it.at()->getTarget_lv2()->text() == p_target_text){
            l_ret = l_it.at();
            qCDebug(g_cat_lv2_members) << "Found: " << l_it.at()->getTarget_lv2()->text();
            break;
        }
    }
    M1_FUNC_EXIT
    return l_ret;

}



