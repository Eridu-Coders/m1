#include "m1_lv2_item.h"
#include "m1_store.h"

Q_LOGGING_CATEGORY(g_cat_lv2_members, "lv2.members_access")
Q_LOGGING_CATEGORY(g_cat_lv2_constructors, "lv2.constructors")

// ---------------------------------- Constructors and instantiation from mmap() data -------------------------------------------
M1Store::ItemWrapper::ItemWrapper(Item_lv0* p_item){m_item = p_item;}
M1Store::ItemWrapperFullVertex::ItemWrapperFullVertex(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->id());
}
M1Store::ItemWrapperSimpleVertex::ItemWrapperSimpleVertex(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->id());
}
M1Store::ItemWrapperFullEdge::ItemWrapperFullEdge(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->id());
}
M1Store::ItemWrapperSimpleEdge::ItemWrapperSimpleEdge(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->id());
}

/**
 * @brief M1Store::ItemWrapper::instantiateFromMMap convert in the case of an existing Item
 * @param p the pointer from the mmap() area
 * @return an ItemWrapper pointer
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::instantiateFromMMap(Item_lv0* p){
    qCDebug(g_cat_lv2_constructors) << QString("instantiateFromMMap from lv0 ID: %1").arg(p->id());
    M1Store::ItemWrapper* l_ret;

    switch(p->flags() & ITEM_NATURE_MASK){
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
/**
 * @brief M1Store::ItemWrapper::instantiateFromMMap convert in the case of a new Item
 * @param p the pointer from the mmap() area
 * @param p_ID the New ID
 * @param p_flags the flags
 * @param p_type the tyep
 * @return an ItemWrapper pointer
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::getNew(const FlagField p_flags, const ItemType& p_type){
    qCDebug(g_cat_lv2_constructors) << QString("new Item p_flags: 0b%1 p_type: %2")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString());               // %2

    M1Store::Item_lv0* l_item = M1Store::Storage::getNewItemSlotPointer(p_flags, p_type);
    M1Store::ItemWrapper* l_wrapper = M1Store::ItemWrapper::instantiateFromMMap(l_item);
    // build default connexions
    l_wrapper->defaultConnections();
    qCDebug(g_cat_lv2_constructors) << "end getNew(FlagField, ItemType)";
    return l_wrapper;
}

M1Store::ItemWrapper* M1Store::ItemWrapper::getNew(const FlagField p_flags, const ItemType& p_type , const char* p_label){
    qCDebug(g_cat_lv0_members) << QString("new Item p_flags: 0b%1 p_type: %2 p_label: %3")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString())                // %2
                                .arg(p_label);                          // %3

    M1Store::ItemWrapper* l_wrapper = getNew(p_flags, p_type);
    qCDebug(g_cat_lv2_constructors) << QString("Setting text to: %1").arg(p_label);
    l_wrapper->setText(p_label);
    qCDebug(g_cat_lv2_constructors) << "end getNew(FlagField, ItemType, char*)";
    return l_wrapper;
}

M1Store::ItemWrapper* M1Store::ItemWrapper::getNew(
    const FlagField p_flags, const ItemType p_type , const char* p_label,
    const FlagField p_flags_special, const char* p_mnemonic){

    qCDebug(g_cat_lv2_constructors) << QString("new Item p_flags: 0b%1 p_type: %2 p_label: %3 p_flags_special %4 p_menonic %5")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString())                // %2
                                .arg(p_label)                           // %3
                                .arg(p_flags_special, 64, 2, QLatin1Char('0'))  // %4
                                .arg(p_mnemonic);                       // %5

    M1Store::ItemWrapper* l_wrapper = getNew(p_flags, p_type, p_label);
    qCDebug(g_cat_lv2_constructors) << QString("Creating new special: %1").arg(p_mnemonic);
    M1Store::Storage::newSpecial(l_wrapper->itemID(), p_flags_special, p_mnemonic);
    return l_wrapper;
}

M1Store::ItemWrapper* M1Store::ItemWrapper::getExisting(const ItemID p_ID){
    qCDebug(g_cat_lv2_constructors) << QString("getExisting from ItemID: %1").arg(p_ID);
    if(p_ID == G_VOID_ID) return nullptr;
    return M1Store::ItemWrapper::instantiateFromMMap(
        M1Store::Storage::getItemSlotPointer(p_ID));
}
M1Store::ItemWrapper* M1Store::ItemWrapper::getExisting(const char* p_mnemonic){
    qCDebug(g_cat_lv2_constructors) << QString("getExisting from mnemonic: %1").arg(p_mnemonic);
    return getExisting(M1Store::Storage::getSpecial(p_mnemonic)->itemId());
}

M1Store::ItemWrapper::~ItemWrapper(){
    // qCDebug(g_cat_lv2_constructors) << QString("~ItemWrapper(): %1").arg(this->dbgShort());
    qCDebug(g_cat_lv2_constructors) << QString("~ItemWrapper()");

    if(m_must_delete){
        qDebug() << "deleting m_item";
        delete m_item;
    }
}
M1Store::ItemWrapperFullVertex::~ItemWrapperFullVertex(){
    // qCDebug(g_cat_lv2_constructors) << QString("~ItemWrapperFullVertex() : %1").arg(dbgShort());
}
M1Store::ItemWrapperFullEdge::~ItemWrapperFullEdge(){
    // qCDebug(g_cat_lv2_constructors) << QString("~ItemWrapperFullEdge() : %1").arg(dbgShort());
}

// ---------------------------------- Debug data -------------------------------------------
void M1Store::ItemWrapper::recurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left){
    if(p_already_expanded.find(p_item_id) == p_already_expanded.end()){
        p_already_expanded.insert(p_item_id);

        ItemWrapper* l_current = getExisting(p_item_id);
        qDebug(g_cat_silence) << p_left + l_current->dbgShort();

        if(ItemWrapper* l_edge = l_current->getFirstEdgeSpecial()){
            ItemID l_stop_id = l_edge->itemID();
            do{
                ItemWrapper* l_next_edge = l_edge->getNext();
                qDebug(g_cat_silence) << p_left + "+--S" + l_edge->dbgHalf();
                recurGraph(l_edge->getTarget(), p_already_expanded, p_left + "|  ");
                delete l_edge;
                l_edge = l_next_edge;
            }while(l_edge->itemID() != l_stop_id);
            delete l_edge;
        }
        if(ItemWrapper* l_edge = l_current->getFirstEdge()){
            ItemID l_stop_id = l_edge->itemID();
            do{
                qDebug(g_cat_silence) << p_left + "+--O" + l_edge->dbgHalf();
                ItemWrapper* l_next_edge = l_edge->getNext();
                recurGraph(l_edge->getTarget(), p_already_expanded,
                           p_left + ((l_next_edge->itemID() == l_stop_id) ? QString("   ") : QString("|  ")));
                delete l_edge;
                l_edge = l_next_edge;
            }while(l_edge->itemID() != l_stop_id);
            delete l_edge;
        }
        delete l_current;
    }
}

QString M1Store::ItemWrapper::dbgString(){
    QString l_types(" [");
    for(int i = 0; i < 4; i++)
        if(m_item->getType(i) != G_VOID_TYPE_ID)
            l_types += Storage::getSpecialSlotPointer(m_item->getType(i))->mnemonic();
    if(l_types.length() == 2) l_types = "";
    else l_types += "]";
    return QString("\n-----------------------------------------------------------------------------------------\n%1\n%2\n%3\n")
        .arg(QString("m_id                 : 0x%1").arg(m_item->m_id, 16, 16, QLatin1Char('0')))
        .arg(QString("m_flags              : 0b%1").arg(m_item->m_flags, 64, 2, QLatin1Char('0')))
        .arg(QString("m_type               : %1").arg(m_item->m_type.dbgString() + l_types));
}
QString M1Store::ItemWrapperFullVertex::dbgString(){
    QString l_text(m_item->text());
    QString l_edges;
    if(m_item->firstEdgeSpecial() != G_VOID_ID){
        ItemWrapper* l_current_edge = getExisting(m_item->firstEdgeSpecial());
        l_edges += "\nSpecial edges:";
        do {
            l_edges += "\n" + l_current_edge->dbgShort();
            ItemWrapper* l_next_edge = l_current_edge->getNext();
            delete l_current_edge;
            l_current_edge = l_next_edge;
            qDebug() << "l_edges: " << l_edges;
        } while (l_current_edge->itemID() != m_item->firstEdgeSpecial());
        delete l_current_edge;
    }
    if(m_item->firstEdge() != G_VOID_ID){
        ItemWrapper* l_current_edge = getExisting(m_item->firstEdge());
        l_edges += "\nOrdinary edges:";
        do {
            l_edges += "\n" + l_current_edge->dbgShort();
            ItemWrapper* l_next_edge = l_current_edge->getNext();
            delete l_current_edge;
            l_current_edge = l_next_edge;
            qDebug() << "l_edges: " << l_edges;
        } while (l_current_edge->itemID() != m_item->firstEdge());
        delete l_current_edge;
    }

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
            .arg(l_text.length() > 100 ? "..." : "")) + l_edges;
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

QString M1Store::ItemWrapper::dbgTypeShort(){
    if(m_item->getType(0) == G_VOID_TYPE_ID)
        return "_____";
    else
        return Storage::getSpecialSlotPointer(m_item->getType(0))->mnemonic();
}
QString M1Store::ItemWrapper::dbgShort(int p_depth){
    return QString();
}
QString M1Store::ItemWrapperFullVertex::dbgShort(int p_depth){
    return QString("ITEM [%1] %2%3")
        .arg(dbgTypeShort())
        .arg(m_item->text())
        .arg((m_item->flags() & IS_SPECIAL) ? QString(" (%1)").arg(M1Store::Storage::getSpecial(m_item->id())->mnemonic()) : "");
}
QString M1Store::ItemWrapperSimpleVertex::dbgShort(int p_depth){
    return QString("VFLD [%1]").arg(m_item->text());
}
QString M1Store::ItemWrapperFullEdge::dbgShort(int p_depth){
    if(p_depth > 0) return("...");

    M1Store::ItemWrapper* l_origin = ItemWrapper::getExisting(m_item->origin());
    M1Store::ItemWrapper* l_target = ItemWrapper::getExisting(m_item->target());

    QString l_ret = QString("EDGE [%1] {%2}-->{%3}").arg(dbgTypeShort()).arg(l_origin->dbgShort()).arg(l_target->dbgShort());

    delete l_origin;
    delete l_target;
    return l_ret;
}
QString M1Store::ItemWrapperSimpleEdge::dbgShort(int p_depth){
    return QString("EFLD [%1]").arg(m_item->text());
}

QString M1Store::ItemWrapper::dbgHalf(){return "";}
QString M1Store::ItemWrapperFullVertex::dbgHalf(){qFatal("Aborting / Cannot get dbgHalf on a full vertex"); return "";}
QString M1Store::ItemWrapperSimpleVertex::dbgHalf(){qFatal("Aborting / Cannot get dbgHalf on a full vertex"); return "";}
QString M1Store::ItemWrapperFullEdge::dbgHalf(){
    M1Store::ItemWrapper* l_target = ItemWrapper::getExisting(m_item->target());

    QString l_ret = QString("-[%1]-->{%2}").arg(dbgTypeShort()).arg(l_target->dbgShort());

    delete l_target;
    return l_ret;
}
QString M1Store::ItemWrapperSimpleEdge::dbgHalf(){return dbgShort();}

// ---------------------------------- other methods -------------------------------------------

M1Store::ItemWrapper* M1Store::ItemWrapper::getFirstEdge(){return getExisting(m_item->firstEdge());}
M1Store::ItemWrapper* M1Store::ItemWrapper::getFirstEdgeSpecial(){return getExisting(m_item->firstEdgeSpecial());}
M1Store::ItemWrapper* M1Store::ItemWrapperFullVertex::getFirstEdge(){return ItemWrapper::getFirstEdge();}
M1Store::ItemWrapper* M1Store::ItemWrapperFullVertex::getFirstEdgeSpecial(){return ItemWrapper::getFirstEdgeSpecial();}
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleVertex::getFirstEdge(){qFatal("Aborting / Cannot get first edge of a simple vertex"); return nullptr;}
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleVertex::getFirstEdgeSpecial(){qFatal("Aborting / Cannot get first special edge of a simple vertex"); return nullptr;}
M1Store::ItemWrapper* M1Store::ItemWrapperFullEdge::getFirstEdge(){return ItemWrapper::getFirstEdge();}
M1Store::ItemWrapper* M1Store::ItemWrapperFullEdge::getFirstEdgeSpecial(){return ItemWrapper::getFirstEdgeSpecial();}
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleEdge::getFirstEdge(){qFatal("Aborting / Cannot get first edge of a simple edge"); return nullptr;}
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleEdge::getFirstEdgeSpecial(){qFatal("Aborting / Cannot get first special edge of a simple edge"); return nullptr;}

M1Store::ItemID M1Store::ItemWrapperFullVertex::getTarget(){qFatal("Aborting / Cannot get target of a full vertex"); return G_VOID_ID;}
M1Store::ItemID M1Store::ItemWrapperSimpleVertex::getTarget(){qFatal("Aborting / Cannot get target of a simple vertex"); return G_VOID_ID;}
M1Store::ItemID M1Store::ItemWrapperFullEdge::getTarget(){return ItemWrapper::getTarget();}
M1Store::ItemID M1Store::ItemWrapperSimpleEdge::getTarget(){qFatal("Aborting / Cannot get target of a simple edge"); return G_VOID_ID;}

void M1Store::ItemWrapperFullVertex::defaultConnections(){
    qCDebug(g_cat_lv2_members) << "Default connections for full vertex";
    M1Store::ItemWrapperFullEdge* l_auto_edge =
        dynamic_cast<M1Store::ItemWrapperFullEdge*>(
        getNew(FULL_EDGE | IS_AUTO,
               M1Store::ItemType(M1Store::Storage::getSpecialID("AUTO_"),
                                 G_VOID_TYPE_ID,
                                 G_VOID_TYPE_ID,
                                 G_VOID_TYPE_ID))
    );
    l_auto_edge->setTarget(m_item->id());
    l_auto_edge->setOrigin(m_item->id());

    l_auto_edge->setPrevious(l_auto_edge->itemID());
    l_auto_edge->setNext(l_auto_edge->itemID());

    this->setFirstEdge(l_auto_edge->itemID());

    createTypeEdges();
    qCDebug(g_cat_lv2_members) << QString("Default connections for full vertex end: %1").arg(m_item->id());
    delete l_auto_edge;
}
void M1Store::ItemWrapperSimpleVertex::defaultConnections(){
    qCDebug(g_cat_lv2_members) << "Default connections for simple vertex";
}
void M1Store::ItemWrapperFullEdge::defaultConnections(){
    qCDebug(g_cat_lv2_members) << "Default connections for full edge";
    createTypeEdges();
    qCDebug(g_cat_lv2_members) << "Default connections for full edge end";
}
void M1Store::ItemWrapperSimpleEdge::defaultConnections(){
    qCDebug(g_cat_lv2_members) << "Default connections for simple edge";
}

void M1Store::ItemWrapper::setType(const SpecialItem* p_type_si){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(p_type_si->mnemonic());

    int l_free_slot = m_item->getType(0) == G_VOID_TYPE_ID ? 0 :
        m_item->getType(1) == G_VOID_TYPE_ID ? 1 :
        m_item->getType(2) == G_VOID_TYPE_ID ? 2 :
        m_item->getType(3) == G_VOID_TYPE_ID ? 3 : -1;

    if(l_free_slot >= 0)
        m_item->setType(l_free_slot, p_type_si->getSpecialId());

    if(l_free_slot < 0 || p_type_si->flags() & SI_REQUIRES_EDGE)
        linkTo(p_type_si->itemId(), "_ISA_");
}
void M1Store::ItemWrapper::setType(const SpecialItemID p_type_id){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(M1Store::Storage::getSpecialSlotPointer(p_type_id)->mnemonic());
    setType(M1Store::Storage::getSpecialSlotPointer(p_type_id));
}
void M1Store::ItemWrapper::setType(const char* p_mnemonic){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(p_mnemonic);
    setType(M1Store::Storage::getSpecial(p_mnemonic));
}

void M1Store::ItemWrapper::linkTo(ItemWrapper* p_target, const SpecialItemID p_type_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]")
                    .arg(p_target->dbgShort())
                    .arg(M1Store::Storage::getSpecialSlotPointer(p_type_edge)->mnemonic());

    M1Store::ItemWrapperFullEdge* l_new_edge =
        dynamic_cast<M1Store::ItemWrapperFullEdge*>(getNew(FULL_EDGE, M1Store::ItemType(p_type_edge, G_VOID_TYPE_ID, G_VOID_TYPE_ID, G_VOID_TYPE_ID)));
    l_new_edge->setTarget(p_target->itemID());
    l_new_edge->setOrigin(m_item->id());

    if(M1Store::Storage::getSpecialSlotPointer(p_type_edge)->flags() & SI_HAS_RECIPROCAL){
        SpecialItemID l_reciprocal_id = M1Store::Storage::getSpecialSlotPointer(p_type_edge)->reciprocalId();
        M1Store::ItemWrapperFullEdge* l_new_edge_reciprocal =
            dynamic_cast<M1Store::ItemWrapperFullEdge*>(
            getNew(FULL_EDGE, M1Store::ItemType(l_reciprocal_id, 0, 0, 0))
        );

        l_new_edge_reciprocal->setOrigin(p_target->itemID());
        l_new_edge_reciprocal->setTarget(m_item->id());

        l_new_edge->setReciprocal(l_new_edge_reciprocal->itemID());
        l_new_edge_reciprocal->setReciprocal(l_new_edge->itemID());

        p_target->addFullEdge(l_new_edge_reciprocal, l_reciprocal_id);

        delete l_new_edge_reciprocal;
    }
    this->addFullEdge(l_new_edge, p_type_edge);
    delete l_new_edge;
}
void M1Store::ItemWrapper::linkTo(ItemWrapper* p_target, const char* p_mnemonic_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]").arg(p_target->dbgShort()).arg(p_mnemonic_edge);

    linkTo(p_target, M1Store::Storage::getSpecialSlotPointer(p_mnemonic_edge)->getSpecialId());
}
void M1Store::ItemWrapper::linkTo(ItemID p_target_id, const SpecialItemID p_type){
    M1Store::ItemWrapper* l_target = getExisting(p_target_id);
    linkTo(l_target, p_type);
    delete l_target;
}
void M1Store::ItemWrapper::linkTo(ItemID p_target_id, const char* p_mnemonic){
    M1Store::ItemWrapper* l_target = getExisting(p_target_id);
    linkTo(l_target, p_mnemonic);
    delete l_target;
}

void M1Store::ItemWrapper::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    ItemID new_edge_id = p_new_edge->itemID();
    bool edge_is_special = M1Store::Storage::getSpecialSlotPointer(p_edge_type)->flags() & SI_IS_SPECIAL_EDGE;

    ItemID first_edge_id = edge_is_special ? m_item->firstEdgeSpecial() : m_item->firstEdge();

    if(first_edge_id == G_VOID_ID){
        p_new_edge->setPrevious(new_edge_id);
        p_new_edge->setNext(new_edge_id);

        if(edge_is_special)
            // add to the special edge ring
            m_item->setFirstEdgeSpecial(p_new_edge->itemID());
        else
            // add to the visible edge ring
            m_item->setFirstEdge(p_new_edge->itemID());
    }
    else{
        M1Store::ItemWrapper* l_previous_edge = getExisting(first_edge_id);
        M1Store::ItemWrapper* l_next_edge = l_previous_edge->getNext();

        p_new_edge->setPrevious(first_edge_id);
        p_new_edge->setNext(l_next_edge->itemID());

        l_previous_edge->setNext(new_edge_id);
        l_next_edge->setPrevious(new_edge_id);

        delete l_previous_edge;
        delete l_next_edge;
    }
}
void M1Store::ItemWrapperFullVertex::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    ItemWrapper::addFullEdge(p_new_edge, p_edge_type);
}
void M1Store::ItemWrapperSimpleVertex::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    qFatal("Aborting / Cannot add an edge to a simple vertex");
}
void M1Store::ItemWrapperFullEdge::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    ItemWrapper::addFullEdge(p_new_edge, p_edge_type);
}
void M1Store::ItemWrapperSimpleEdge::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    qFatal("Aborting / Cannot add an edge to a simple vertex");
}

void M1Store::ItemWrapper::createTypeEdges(){
    qCDebug(g_cat_lv2_members) << "Creating type edges if required";
    if((m_item->flags() & TYPE_IS_ITEM_ID) == 0) // not an ItemID type therefore 4 special item ids
        for(int l_slot = 0; l_slot < 4; l_slot++){
            SpecialItem* l_type_si = M1Store::Storage::getSpecialSlotPointer(m_item->getType(l_slot));
            if(l_type_si->flags() & SI_REQUIRES_EDGE) // this type must be materialized with an edge (not just a special type id)
                linkTo(l_type_si->itemId(), "_ISA_");
        }
    qCDebug(g_cat_lv2_members) << "Creating type edges end";
}
void M1Store::ItemWrapperFullVertex::createTypeEdges(){
    ItemWrapper::createTypeEdges();
}
void M1Store::ItemWrapperSimpleVertex::createTypeEdges(){
    qFatal("Aborting / Cannot add a type edge to a simple vertex");
}
void M1Store::ItemWrapperFullEdge::createTypeEdges(){
    ItemWrapper::createTypeEdges();
}
void M1Store::ItemWrapperSimpleEdge::createTypeEdges(){
    qFatal("Aborting / Cannot add a type edge to a simple edge");
}

M1Store::ItemWrapper* M1Store::ItemWrapper::getNext(){
    return getExisting(m_item->next());
}
M1Store::ItemWrapper* M1Store::ItemWrapperFullVertex::getNext(){
    qFatal() << "Cannot get next edge on a vertex (full)";
    return nullptr;
}
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleVertex::getNext(){
    qFatal() << "Cannot get next edge on a vertex (simple)";
    return nullptr;
}
M1Store::ItemWrapper* M1Store::ItemWrapperFullEdge::getNext(){
    return ItemWrapper::getNext();
}
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleEdge::getNext(){
    return ItemWrapper::getNext();
}

void M1Store::ItemWrapperFullVertex::setNext(ItemID p_edge_id){
    qFatal() << "Cannot set next edge on a vertex (full)";
}
void M1Store::ItemWrapperSimpleVertex::setNext(ItemID p_edge_id){
    qFatal() << "Cannot set next edge on a vertex (simple)";
}
void M1Store::ItemWrapperFullVertex::setPrevious(ItemID p_edge_id){
    qFatal() << "Cannot set next edge on a vertex (full)";
}
void M1Store::ItemWrapperSimpleVertex::setPrevious(ItemID p_edge_id){
    qFatal() << "Cannot set next edge on a vertex (simple)";
}


