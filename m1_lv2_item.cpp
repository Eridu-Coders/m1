#include "m1_lv1_store.h"

void M1Store::ItemWrapper::initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type){
    m_item->initializeMembers(p_ID, p_flags, p_type);
}

/**
 * @brief M1Store::ItemWrapper::instantiateFromMMap convert in the case of an existing Item
 * @param p the pointer from the mmap() area
 * @return an ItemWrapper pointer
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::instantiateFromMMap(void* p){
    M1Store::Item* l_item = reinterpret_cast<M1Store::Item*>(p);
    return M1Store::ItemWrapper::instantiateFromMMap(l_item);
}
/**
 * @brief M1Store::ItemWrapper::instantiateFromMMap convert in the case of a new Item
 * @param p the pointer from the mmap() area
 * @param p_ID the New ID
 * @param p_flags the flags
 * @param p_type the tyep
 * @return an ItemWrapper pointer
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::instantiateFromMMap(void* p, const ItemID p_ID, const FlagField p_flags, const ItemType p_type){
    M1Store::Item* l_item = reinterpret_cast<M1Store::Item*>(p);

    l_item->initializeMembers(p_ID, p_flags, p_type);

    return M1Store::ItemWrapper::instantiateFromMMap(l_item);
}
/**
 * @brief M1Store::ItemWrapper::instantiateFromMMap private static function instantiating the proper wrapper based on the Item info
 * @param p the item
 * @return a new wrapper for p
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::instantiateFromMMap(M1Store::Item* p){
    M1Store::ItemWrapper* l_ret;

    switch(p->m_flags & ITEM_NATURE_MASK){
    case FULL_VERTEX:
        l_ret = new M1Store::ItemWrapperFullVertex(p);
        break;
    case SIMPLE_VERTEX:
        l_ret = new M1Store::ItemWrapperSimpleVertex(p);
        break;
    case FULL_EDGE:
        l_ret = new M1Store::ItemWrapperFullEdge(p);
        break;
    case SIMPLE_EDGE:
        l_ret = new M1Store::ItemWrapperSimpleEdge(p);
        break;
    }

    return l_ret;
}

M1Store::ItemWrapper::~ItemWrapper(){
    qDebug() << "~ItemWrapper()";

    if(m_must_delete){
        qDebug() << "deleting m_item";
        delete m_item;
    }
}
M1Store::ItemWrapperFullVertex::~ItemWrapperFullVertex(){
    qDebug() << "~ItemWrapperFullVertex()";
}

QString M1Store::ItemWrapper::dbgString(){
    return QString("\n%1\n%2\n%3\n")
        .arg(QString("m_id                 : 0x%1").arg(m_item->m_id, 16, 16, QLatin1Char('0')))
        .arg(QString("m_flags              : 0b%1").arg(m_item->m_flags, 64, 2, QLatin1Char('0')))
        .arg(QString("m_type               : %1").arg(m_item->m_type.dbgString()));
}
QString M1Store::ItemWrapperFullVertex::dbgString(){
    QString l_text(m_item->text());
    return M1Store::ItemWrapper::dbgString() +
           QString("--------------------------- FULL VERTEX -------------------------------------------------\n") +
           QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8")
        .arg(QString("m_flags_extra        : 0b%1").arg(m_item->flagsExtra(), 64, 2, QLatin1Char('0')))
        .arg(QString("m_creation_date      : %1").arg(m_item->creationDate().toString("dd/MM/yyyy hh:mm:ss")))
        .arg(QString("m_lastmod_date       : %1").arg(m_item->lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
        .arg(QString("m_incoming_edges     : %1").arg(m_item->incomingEdges()))
        .arg(QString("m_visible_edges      : %1").arg(m_item->visibleEdges()))
        .arg(QString("m_first_edge         : 0x%1 %2").arg(m_item->firstEdge(), 16, 16, QLatin1Char('0')).arg(m_item->firstEdge()))
        .arg(QString("m_first_edge_special : 0x%1 %2").arg(m_item->firstEdgeSpecial(), 16, 16, QLatin1Char('0')).arg(m_item->firstEdgeSpecial()))
        .arg(QString("text                 : [%1] %2%3")
            .arg(l_text.length())
            .arg(l_text.left(100))
            .arg(l_text.length() > 100 ? "..." : ""));
}
QString M1Store::ItemWrapperSimpleVertex::dbgString(){
    return M1Store::ItemWrapper::dbgString() +
           QString("--------------------------- SIMPLE VERTEX -----------------------------------------------\n") +
           QString("%1")
        .arg(QString("text                 : %1").arg(m_item->text()));
}
QString M1Store::ItemWrapperFullEdge::dbgString(){
    return M1Store::ItemWrapper::dbgString() +
           QString("--------------------------- FULL EDGE ---------------------------------------------------\n") +
           QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9\n%10\n%11\n%12")
        .arg(QString("m_flags_extra        : 0b%1").arg(m_item->flagsExtra(), 64, 2, QLatin1Char('0')))
        .arg(QString("m_creation_date      : %1").arg(m_item->creationDate().toString("dd/MM/yyyy hh:mm:ss")))
        .arg(QString("m_lastmod_date       : %1").arg(m_item->lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
        .arg(QString("m_incoming_edges     : %1").arg(m_item->incomingEdges()))
        .arg(QString("m_first_edge         : 0x%1 %2").arg(m_item->firstEdge(), 16, 16, QLatin1Char('0')).arg(m_item->firstEdge()))
        .arg(QString("m_first_edge_special : 0x%1 %2").arg(m_item->firstEdgeSpecial(), 16, 16, QLatin1Char('0')).arg(m_item->firstEdgeSpecial()))
        .arg(QString("m_v_origin           : 0x%1 %2").arg(m_item->origin(), 16, 16, QLatin1Char('0')).arg(m_item->origin()))
        .arg(QString("m_v_target           : 0x%1 %2").arg(m_item->target(), 16, 16, QLatin1Char('0')).arg(m_item->target()))
        .arg(QString("m_e_previous         : 0x%1 %2").arg(m_item->previous(), 16, 16, QLatin1Char('0')).arg(m_item->previous()))
        .arg(QString("m_e_next             : 0x%1 %2").arg(m_item->next(), 16, 16, QLatin1Char('0')).arg(m_item->next()))
        .arg(QString("m_e_reciprocal       : 0x%1 %2").arg(m_item->reciprocal(), 16, 16, QLatin1Char('0')).arg(m_item->reciprocal()))
        .arg(QString("text                 : %1").arg(m_item->text()));
}
QString M1Store::ItemWrapperSimpleEdge::dbgString(){
    return M1Store::ItemWrapper::dbgString() +
           QString("--------------------------- SIMPLE EDGE -------------------------------------------------\n") +
           QString("%1\n%2\n%3\n%4")
        .arg(QString("m_v_origin           : 0x%1 %2").arg(m_item->origin(), 16, 16, QLatin1Char('0')).arg(m_item->origin()))
        .arg(QString("m_e_previous         : 0x%1 %2").arg(m_item->previous(), 16, 16, QLatin1Char('0')).arg(m_item->previous()))
        .arg(QString("m_e_next             : 0x%1 %2").arg(m_item->next(), 16, 16, QLatin1Char('0')).arg(m_item->next()))
        .arg(QString("text                 : %1").arg(m_item->text()));
}

