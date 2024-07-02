#include "m1B_nav.h"

M1Store::Nav::Nav(SpecialItemID p_id_special){
    m_origin = Item_lv2::getExisting(p_id_special);
    m_current_edge = m_origin->getFirstEdgeWrapper();

    m_is_at_end = false;

    M1_FUNC_EXIT
}

void M1Store::Nav::next(){
    m_is_at_end = false;
    m_current_edge = m_current_edge->getNextWrapper();
    m_is_at_end = m_current_edge->item_id() == m_origin->firstEdge_item_id();

    M1_FUNC_EXIT
}

void M1Store::Nav::previous(){
    m_is_at_end = false;
    m_current_edge = m_current_edge->getPreviousWrapper();

    M1_FUNC_EXIT
}

// void M1Store::Nav::next_until(SpecialItemID p_type_id_special){}
// void M1Store::Nav::previous_until(SpecialItemID p_type_id_special){}

bool M1Store::Nav::origin_is_edge(){ return m_origin->isFullEdge();}
bool M1Store::Nav::origin_is_of_type(SpecialItemID p_type_id_special){

    M1_FUNC_EXIT
    return false;
}
bool M1Store::Nav::edge_is_of_type(SpecialItemID p_type_id_special){

    M1_FUNC_EXIT
    return false;
}
bool M1Store::Nav::is_at_end(){return m_is_at_end;}

QString M1Store::Nav::dbgString(){return QString();}
QString M1Store::Nav::dbgShort(int p_depth){return QString();}
QString M1Store::Nav::dbgHalf(){return QString();}

QDebug operator<<(QDebug d, M1Store::Nav p){return d << p.dbgShort().toUtf8().constData();}
QDebug operator<<(QDebug d, M1Store::Nav* p){return d << p->dbgShort().toUtf8().constData();}


