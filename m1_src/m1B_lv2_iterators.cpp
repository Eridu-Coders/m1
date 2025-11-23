#include "m1A_env.h"
#include "m1B_lv2_item.h"
#include "m1B_graph_init.h"

Q_LOGGING_CATEGORY(g_cat_lv2_iterators, "lv2.iterators")

/***************************************/
/**
 * \defgroup ITER Iterators
 * \ingroup LV2
 * @{
 */

/******************************* Item_lv2_iterator ***********************************/
/**
 * @brief M1Store::Item_lv2_iterator::Item_lv2_iterator
 * @param p_copiand
 */
M1Store::Item_lv2_iterator::Item_lv2_iterator(const Item_lv2_iterator& p_copiand){
    qCDebug(g_cat_lv2_iterators) << "Item_lv2_iterator Copy constructor";
    m_it = p_copiand.m_it;
}
/**
 * @brief M1Store::Item_lv2_iterator::operator =
 * @param p_assignand
 * @return
 */
M1Store::Item_lv2_iterator& M1Store::Item_lv2_iterator::operator=(const M1Store::Item_lv2_iterator& p_assignand){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("assignment operator - p_assignand = %1").arg(p_assignand.m_it->dbgShort()));
    this->m_it = p_assignand.m_it;
    qCDebug(g_cat_lv2_iterators) << QString("Asignee: %1").arg(this->m_it->dbgShort());
    M1_FUNC_EXIT
        return *this;
}
/**
 * @brief M1Store::Item_lv2_iterator::~Item_lv2_iterator
 */
M1Store::Item_lv2_iterator::~Item_lv2_iterator(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("About to delete - this = %1").arg(this->m_it->dbgShort()));
    // no need to delete m_it. Will be deleted when the last Item_lv2_iterator owning it goes out of scope
    qCDebug(g_cat_lv2_iterators) << QString("m_it use counter: %1").arg(m_it.use_count());
    M1_FUNC_EXIT
}

/**
 * @brief M1Store::Item_lv2_iterator::beyondEnd
 * @return
 */
bool M1Store::Item_lv2_iterator::beyondEnd() const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("calling m_it->beyondEnd() [%1]").arg(m_it->dbgShort()));
    bool l_ret = m_it->beyondEnd();
    M1_FUNC_EXIT
        return l_ret;
}
/******************************* Item_lv2_iterator_base ***********************************/
/**
 * @brief M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base
 */
M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, "Creation of a NULL iterator");
    M1_FUNC_EXIT
}
/**
 * @brief M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base
 * @param p_copiand
 */
M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base(const Item_lv2_iterator_base& p_copiand){
    qCDebug(g_cat_lv2_iterators) << "Item_lv2_iterator_base Copy constructor";
    m_current_edge = p_copiand.m_current_edge;
    m_first_edge_item_id = p_copiand.m_first_edge_item_id;
    m_not_first = p_copiand.m_not_first;
}
/**
 * @brief M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base
 * @param p_start_edge
 */
M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base(Item_lv2* p_start_edge){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Simple lv2 iterator creation"))
    if(p_start_edge != nullptr){
        m_current_edge = p_start_edge;
        m_first_edge_item_id = p_start_edge->item_id();
    }
    M1_FUNC_EXIT
}
/**
 * @brief M1Store::Item_lv2_iterator_base::dbgShort
 * @return
 */
QString M1Store::Item_lv2_iterator_base::dbgShort(){
    return QString("Item_lv2_iterator_base - current position: ") + (m_current_edge != nullptr ? m_current_edge->dbgShort() : "nullptr");
}
/**
 * @brief M1Store::Item_lv2_iterator_base::isNull
 * @return
 */
bool M1Store::Item_lv2_iterator_base::isNull(){
    return m_current_edge == nullptr;
}
/**
 * @brief M1Store::Item_lv2_iterator_base::next
 */
void M1Store::Item_lv2_iterator_base::next(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("move to next edge"))
    if(m_current_edge != nullptr )
        m_current_edge = m_current_edge->get_next_lv2();
    m_not_first = true;
    M1_FUNC_EXIT
}
/**
 * @brief M1Store::Item_lv2_iterator_base::beyondEnd
 * @return
 */
bool M1Store::Item_lv2_iterator_base::beyondEnd() const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("end test"))
    bool l_ret = true;
    if(m_current_edge != nullptr )
        l_ret = m_not_first && m_current_edge->item_id() == m_first_edge_item_id;
    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief M1Store::Item_lv2_iterator_base::at
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2_iterator_base::at() const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("current value"))
    M1_FUNC_EXIT
        return m_current_edge;
}
/******************************* Item_lv2_iterator_edge_type ***********************************/
/**
 * @brief M1Store::Item_lv2_iterator_type::Item_lv2_iterator_type
 */
M1Store::Item_lv2_iterator_edge_type::Item_lv2_iterator_edge_type(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Zero constructor"))
    M1_FUNC_EXIT
}
/**
 * @brief M1Store::Item_lv2_iterator_type::Item_lv2_iterator_type
 * @param p_copiand
 */
M1Store::Item_lv2_iterator_edge_type::Item_lv2_iterator_edge_type(const Item_lv2_iterator_edge_type& p_copiand){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Copy constructor"))
    M1_FUNC_EXIT
}
/**
 * @brief M1Store::Item_lv2_iterator_type::Item_lv2_iterator_type
 * @param p_start_edge
 * @param p_type
 */
M1Store::Item_lv2_iterator_edge_type::Item_lv2_iterator_edge_type(Item_lv2* p_start_edge,
                                                                  const SpecialItemID p_type){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Constructor"))
    M1_FUNC_EXIT
}
/**
 * @brief M1Store::Item_lv2_iterator_type::next
 */
void M1Store::Item_lv2_iterator_edge_type::next(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString(""))
    M1_FUNC_EXIT
}
/**
 * @brief M1Store::Item_lv2_iterator_type::beyondEnd
 * @return
 */
bool M1Store::Item_lv2_iterator_edge_type::beyondEnd() const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString(""))
    bool l_ret = false;
    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief M1Store::Item_lv2_iterator_type::at
 * @return
 */
M1Store::Item_lv2* M1Store::Item_lv2_iterator_edge_type::at() const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString(""))
    M1Store::Item_lv2* l_ret = nullptr;
    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief M1Store::Item_lv2_iterator_type::dbgShort
 * @return
 */
QString M1Store::Item_lv2_iterator_edge_type::dbgShort(){
    return QString("");
}

/******************************* static iterator getters ***********************************/
/**
 * @brief M1Store::Item_lv2::getIteratorGeneric
 * @param p_start_edge
 * @param p_type
 * @return
 */
std::shared_ptr<M1Store::Item_lv2_iterator_base> M1Store::Item_lv2::getIteratorGeneric(Item_lv2* p_start_edge,
                                                                                       const SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator"))
    std::shared_ptr<Item_lv2_iterator_base> l_ret;
    if(p_type == M1Env::G_VOID_SI_ID){
        l_ret = std::shared_ptr<Item_lv2_iterator_base>(new Item_lv2_iterator_base(p_start_edge));
    }
    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief M1Store::Item_lv2::getIteratorTop
 * @param p_type
 * @return
 */
M1Store::Item_lv2_iterator M1Store::Item_lv2::getIteratorTop(const SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator from top"));
    M1_FUNC_EXIT
        return Item_lv2_iterator(getIteratorGeneric(this->getFirstEdge_lv2(), p_type));
}
/**
 * @brief M1Store::Item_lv2::getIteratorAuto
 * @param p_type
 * @return
 */
M1Store::Item_lv2_iterator M1Store::Item_lv2::getIteratorAuto(const SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator from auto edge"));
    M1_FUNC_EXIT
        return Item_lv2_iterator(getIteratorGeneric(this->getAutoEdge_lv2(), p_type));
}
/**
 * @brief M1Store::Item_lv2::getIteratorSpecial
 * @param p_type
 * @return
 */
M1Store::Item_lv2_iterator M1Store::Item_lv2::getIteratorSpecial(const SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator from top of special edges"));
    M1_FUNC_EXIT
        return Item_lv2_iterator(getIteratorGeneric(this->getFirstEdgeSpecial_lv2(), p_type));
}
/** @}*/
// end of \defgroup ITER
