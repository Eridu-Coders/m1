#include "m1A_env.h"
#include "m1B_lv2_iterators.h"

Q_LOGGING_CATEGORY(g_cat_lv2_iterators, "lv2.iterators")
Q_LOGGING_CATEGORY(g_cat_lv2_type_iterators, "lv2.type_iterators")

/******************************* Item_lv2_iterator ***********************************/
/**
 * @brief Main constructor
 * @param p_it shared pointer to an Item_lv2_iterator_base that the class is a wrapper for
 */
M1Store::Item_lv2_iterator::Item_lv2_iterator(std::shared_ptr<Item_lv2_iterator_base> p_it){
    qCDebug(g_cat_lv2_iterators) << "Item_lv2_iterator main constructor";
    m_it = p_it;
}

/**
 * @brief Copy constructor
 * @param p_copiand the Item_lv2_iterator to be copied
 */
M1Store::Item_lv2_iterator::Item_lv2_iterator(const Item_lv2_iterator& p_copiand){
    qCDebug(g_cat_lv2_iterators) << "Item_lv2_iterator Copy constructor";
    m_it = p_copiand.m_it;
}

/**
 * @brief Assignment operator overloading
 * @param p_assignand the Item_lv2_iterator to be assigned to this
 * @return this
 */
M1Store::Item_lv2_iterator& M1Store::Item_lv2_iterator::operator=(const M1Store::Item_lv2_iterator& p_assignand){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("assignment operator - p_assignand = %1").arg(p_assignand.m_it->dbgShort()));
    this->m_it = p_assignand.m_it;
    qCDebug(g_cat_lv2_iterators) << QString("Asignee: %1").arg(this->m_it->dbgShort());
    M1_FUNC_EXIT
    return *this;
}

/**
 * @brief Destructor
 *
 * There is no need to delete m_it explicitly. It will be deleted when the last Item_lv2_iterator owning it goes out of scope,
 * as per the specifications of std::shared_ptr
 */
M1Store::Item_lv2_iterator::~Item_lv2_iterator(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("About to delete - this = %1").arg(this->m_it->dbgShort()));
    qCDebug(g_cat_lv2_iterators) << QString("m_it use counter: %1").arg(m_it.use_count());
    M1_FUNC_EXIT
}

/******************************* Item_lv2_iterator_base ***********************************/
/**
 * @brief Empty constuctor (never used normally)
 */
M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, "Creation of a NULL iterator");
    m_current_edge = nullptr;
    m_first_edge_item_id = M1Store::G_VOID_ITEM_ID;
    m_on_first = true;
    M1_FUNC_EXIT
}

/**
 * @brief Copy constructor
 * @param p_copiand the instance to copy
 */
M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base(const Item_lv2_iterator_base& p_copiand){
    qCDebug(g_cat_lv2_iterators) << "Item_lv2_iterator_base Copy constructor";
    m_current_edge = p_copiand.m_current_edge;
    m_first_edge_item_id = p_copiand.m_first_edge_item_id;
    m_on_first = p_copiand.m_on_first;
}

/**
 * @brief Main constuctor
 * @param p_start_edge the edge to start at
 *
 * for an explanation of m_first_edge_item_id and m_on_first, see M1Store::Item_lv2_iterator_base::beyondEnd()
 */
M1Store::Item_lv2_iterator_base::Item_lv2_iterator_base(Item_lv2* p_start_edge){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Simple lv2 iterator creation: %1").arg(p_start_edge->dbgShort()))
    Q_ASSERT_X(p_start_edge != nullptr, "Item_lv2_iterator_base()", "p_start_edge is NULL");

    m_current_edge = p_start_edge;
    m_first_edge_item_id = p_start_edge->item_id();
    m_on_first = true;

    M1_FUNC_EXIT
}
/**
 * @brief Base class version
 * @return true if m_current_edge is a full edge (and of course not nullptr)
 */
bool M1Store::Item_lv2_iterator_base::validEdge(){
    return (m_current_edge != nullptr);
    // return (m_current_edge != nullptr) && m_current_edge->isFullEdge();
}

/**
 * @brief The method made necessary by the impossibility to call virtual methods from the constructor
 *
 * Makes sure that m_current_edge is positionned on a valid edge (according to the criteria of the subclass, as determied by validEdge()). The
 * true valus passed as a parameter to next_private() indicates that if a valid edge is found, m_on_first should still be set to true, even
 * if several next() were required to reach it
 */
void M1Store::Item_lv2_iterator_base::init(){
    if(!validEdge()) next_private(true);
    if(!isBase()) qCDebug(g_cat_lv2_type_iterators) << QString("Constructor BBBBB m_current_edge: %1").arg(m_current_edge != nullptr ? m_current_edge->dbgShort() : "nullptr");
}

/**
 * @brief [public] Wrapper for next_private() to ensure the parameter is set to false when the method is called from the outside of the class
 */
void M1Store::Item_lv2_iterator_base::next(){
    M1Store::Item_lv2_iterator_base::next_private(false);
}
/**
 * @brief [private] Does the job of moving to the next item according to the criteria expressed in validEdge()
 * @param p_stay_first see M1Store::Item_lv2_iterator_base::init() for an explanantion of this parameter
 */
void M1Store::Item_lv2_iterator_base::next_private(bool p_stay_first){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("move to next edge"))

    if(m_current_edge != nullptr && !beyondEnd())
        // move forward by one item
        do {
            m_on_first = false;
            m_current_edge = m_current_edge->get_next_lv2();
            if(!isBase()) qCDebug(g_cat_lv2_type_iterators) << QString("NEXT p_stay_first: %1 - m_current_edge: %2")
                                                         .arg(p_stay_first)
                                                         .arg(m_current_edge->dbgShort());
            // this continues until a valid edge is found or the end is reached
        } while(!validEdge() && !beyondEnd());
    else
        // nothing to do
        if(!isBase()) qCDebug(g_cat_lv2_type_iterators) << QString("NONEXT p_stay_first: %1 - m_current_edge: %2")
                                                 .arg(p_stay_first)
                                                 .arg(m_current_edge != nullptr ? m_current_edge->dbgShort() : "nullptr");

    // see M1Store::Item_lv2_iterator_base::init() for an explanantion of this
    // this happens only if the end has not been reached
    if(!beyondEnd()) m_on_first = p_stay_first;

    M1_FUNC_EXIT
}

/**
 * @brief Detects the current position is beyond the end of the ring (end of loop condition)
 *
 * NB: this will return true if the ring does not contain any valid edge because next_private(), as called from init(), will have set m_on_first to false. For
 * more details, see the comments of the last line of next_private()
 *
 * @return true if it is not the first item (next has been called at least once) and m_current_edge is back to the first element (m_first_edge_item_id)
 */
bool M1Store::Item_lv2_iterator_base::beyondEnd() const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("m_current_edge: %1").arg(m_current_edge ? m_current_edge->dbgShort() : "nullptr"))

    bool l_ret = true;
    if(m_current_edge != nullptr ){
        l_ret = !m_on_first && (m_current_edge->item_id() == m_first_edge_item_id);
        qCDebug(g_cat_lv2_type_iterators) << QString("m_on_first: %1 / m_current_edge->item_id() == m_first_edge_item_id: %2 --> %3")
                                                 .arg(m_on_first)
                                                 .arg(m_current_edge->item_id() == m_first_edge_item_id)
                                                 .arg(l_ret);
    }
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief Current edge access
 * @return m_current_edge or nullptr, if beyond end
 */
M1Store::Item_lv2* M1Store::Item_lv2_iterator_base::at() const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("current ege: %1").arg(m_current_edge ? m_current_edge->dbgShort() : "nullptr"))
    M1Store::Item_lv2* l_ret = (beyondEnd() ? nullptr : m_current_edge);
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief Debug string
 * @return the string
 */
QString M1Store::Item_lv2_iterator_base::dbgShort(){
    return QString("Beyond End: %1 - Current position: %2")
        .arg(beyondEnd() ? "Yes" : "No")
        .arg(m_current_edge != nullptr ? m_current_edge->dbgShort() : "nullptr");
}

/**
 * @brief detects empty iterators (should not happen)
 * @return true of ther is no current edge
 */
bool M1Store::Item_lv2_iterator_base::isNull(){
    return m_current_edge == nullptr;
}

/******************************* Item_lv2_iterator_edge_type ***********************************/
/**
 * @brief Empty constructor (not used)
 */
M1Store::Item_lv2_iterator_edge_type::Item_lv2_iterator_edge_type() : Item_lv2_iterator_base(){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Zero constructor"))
    m_edge_type = M1Env::G_VOID_SI_ID;
    M1_FUNC_EXIT
}

/**
 * @brief Copy constructor
 * @param p_copiand the iterator to copy
 */

M1Store::Item_lv2_iterator_edge_type::Item_lv2_iterator_edge_type(const Item_lv2_iterator_edge_type& p_copiand) : Item_lv2_iterator_base(p_copiand){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Copy constructor"))
    m_edge_type = p_copiand.m_edge_type;
    M1_FUNC_EXIT
}

/**
 * @brief Main constructor
 * @param p_start_edge the edge to start at
 * @param p_type the criterion for valid edges
 */
M1Store::Item_lv2_iterator_edge_type::Item_lv2_iterator_edge_type(Item_lv2* p_start_edge,
                                                                  const SpecialItemID p_type) : Item_lv2_iterator_base(p_start_edge){
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("Constructor %1").arg(p_start_edge->dbgShort()))
    Q_ASSERT_X(p_type != M1Env::G_VOID_SI_ID, "Item_lv2_iterator_edge_type()", "p_type == G_VOID_SI_ID");

    m_edge_type = p_type;
    qCDebug(g_cat_lv2_type_iterators) << QString("Constructor AAAAA m_current_edge->isOfType(p_type): %1 - p_type: %2 - m_current_edge: %3")
                                             .arg(m_current_edge->isOfType(p_type) ? "Yes" : "No")
                                             .arg(M1Store::StorageStatic::getSpecialItemPointer(p_type)->mnemonic())
                                             .arg(m_current_edge->dbgShort());

    M1_FUNC_EXIT
}
/**
 * @brief Valid edges criterion
 * @return true --> full edge of type m_edge_type
 */
bool M1Store::Item_lv2_iterator_edge_type::validEdge(){
    return M1Store::Item_lv2_iterator_base::validEdge() && m_current_edge->isOfType(m_edge_type);
}

/**
 * @brief Debug string
 * @return the string
 */
QString M1Store::Item_lv2_iterator_edge_type::dbgShort(){
    return QString("Beyond End?: %1 - Edge type [%2] - current position: %3")
        .arg(beyondEnd() ? "Yes" : "No")
        .arg(M1Store::StorageStatic::getSpecialItemPointer(m_edge_type)->mnemonic())
        .arg(m_current_edge != nullptr ? m_current_edge->dbgShort() : "nullptr");
}

/******************************* Item_lv2_iterator_edge_type ***********************************/

/**
 * @brief Empty constructor (not used)
 */
M1Store::Item_lv2_iterator_et_type::Item_lv2_iterator_et_type() : Item_lv2_iterator_edge_type(){
    m_target_type = M1Env::G_VOID_SI_ID;
}

/**
 * @brief Copy constructor
 * @param p_copiand the iterator to copy
 */
M1Store::Item_lv2_iterator_et_type::Item_lv2_iterator_et_type(const Item_lv2_iterator_et_type& p_copiand) : Item_lv2_iterator_edge_type(p_copiand){
    m_target_type = p_copiand.m_target_type;
}

/**
 * @brief Main constructor
 * @param p_start_edge the edge to start at
 * @param p_type the first criterion for valid edges
 * @param p_target_type the second criterion for valid edges
 */
M1Store::Item_lv2_iterator_et_type::Item_lv2_iterator_et_type(Item_lv2* p_start_edge,
                                                              const SpecialItemID p_edge_type,
                                                              const SpecialItemID p_target_type) : Item_lv2_iterator_edge_type(p_start_edge, p_edge_type) {
    qCDebug(g_cat_lv2_type_iterators) << QString("Constructor aaAaa m_current_edge->getTarget_lv2()->isOfType(p_target_type): %1")
                                                                     .arg(m_current_edge->getTarget_lv2()->isOfType(p_target_type) ? "Yes" : "No");;
    m_target_type = p_target_type;
}

/**
 * @brief Valid edge detection
 * @return true --> full edge of type m_edge_type, with a target of type m_target_type
 */
bool M1Store::Item_lv2_iterator_et_type::validEdge(){
    qCDebug(g_cat_lv2_type_iterators) << this->dbgShort();
    return Item_lv2_iterator_edge_type::validEdge() &&
           m_current_edge->getTarget_lv2()->isOfType(m_target_type);
}

/**
 * @brief Debug string
 * @return the string
 */
QString M1Store::Item_lv2_iterator_et_type::dbgShort(){
    return QString("Beyond End?: %1 - Edge/Target types [%2/%3] - current position: %4")
        .arg(beyondEnd() ? "Yes" : "No")
        .arg(M1Store::StorageStatic::getSpecialItemPointer(m_edge_type)->mnemonic())
        .arg(M1Store::StorageStatic::getSpecialItemPointer(m_target_type)->mnemonic())
        .arg(m_current_edge != nullptr ? m_current_edge->dbgShort() : "nullptr");
}

/******************************* iterator getters ***********************************/
/**
 * @brief The private function that actually does the job for getIteratorTop(), getIteratorAuto() and getIteratorSpecial()
 * \ingroup ITER
 * @param p_start_edge the edge to start at
 * @param p_edge_type the first criterion (or G_VOID_SI_ID)
 * @param p_edge_type the second criterion (or G_VOID_SI_ID)
 * @return the appropriate iterator (inside a wrapper), based on the criteria set
 */
std::shared_ptr<M1Store::Item_lv2_iterator_base> M1Store::Item_lv2::getIteratorGeneric(Item_lv2* p_start_edge,
                                                                                       const SpecialItemID p_edge_type,
                                                                                       const SpecialItemID p_target_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator"))

    std::shared_ptr<Item_lv2_iterator_base> l_ret;
    // determines what subclass to use based on what criteria are set
    if(p_edge_type == M1Env::G_VOID_SI_ID)
        l_ret = std::shared_ptr<Item_lv2_iterator_base>(new Item_lv2_iterator_base(p_start_edge));
    else{
        if(p_target_type == M1Env::G_VOID_SI_ID)
            l_ret = std::shared_ptr<Item_lv2_iterator_base>(new Item_lv2_iterator_edge_type(p_start_edge, p_edge_type));
        else
            l_ret = std::shared_ptr<Item_lv2_iterator_base>(new Item_lv2_iterator_et_type(p_start_edge, p_edge_type, p_target_type));
    }

    // only place at which this method is (and should be) called
    l_ret->init();

    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief Get an iterator positioned at the top of the ordinary edges ring
 * \ingroup ITER
 * @param p_edge_type the first criterion (or G_VOID_SI_ID)
 * @param p_edge_type the second criterion (or G_VOID_SI_ID)
 * @return the appropriate iterator (inside a wrapper), based on the criteria set
 */
M1Store::Item_lv2_iterator M1Store::Item_lv2::getIteratorTop(const SpecialItemID p_edge_type,
                                                             const SpecialItemID p_target_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator from top"));
    M1_FUNC_EXIT
    return Item_lv2_iterator(getIteratorGeneric(this->getFirstEdge_lv2(), p_edge_type, p_target_type));
}

/**
 * @brief Get an iterator positioned at the AUTO edge (in the ordinary edges ring)
 * \ingroup ITER
 * @param p_edge_type the first criterion (or G_VOID_SI_ID)
 * @param p_edge_type the second criterion (or G_VOID_SI_ID)
 * @return the appropriate iterator (inside a wrapper), based on the criteria set
 */
M1Store::Item_lv2_iterator M1Store::Item_lv2::getIteratorAuto(const SpecialItemID p_edge_type,
                                                              const SpecialItemID p_target_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator from auto edge"));
    M1_FUNC_EXIT
    return Item_lv2_iterator(getIteratorGeneric(this->getAutoEdge_lv2(), p_edge_type, p_target_type));
}

/**
 * @brief Get an iterator positioned at the top of the special edges edge ring
 * \ingroup ITER
 * @param p_edge_type the first criterion (or G_VOID_SI_ID)
 * @param p_edge_type the second criterion (or G_VOID_SI_ID)
 * @return the appropriate iterator (inside a wrapper), based on the criteria set
 */
M1Store::Item_lv2_iterator M1Store::Item_lv2::getIteratorSpecial(const SpecialItemID p_edge_type,
                                                                 const SpecialItemID p_target_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_iterators, QString("new iterator from top of special edges"));
    M1_FUNC_EXIT
    return Item_lv2_iterator(getIteratorGeneric(this->getFirstEdgeSpecial_lv2(), p_edge_type, p_target_type));
}
/** @}*/
// end of \defgroup ITER
