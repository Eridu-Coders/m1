#include "m1_lv2_item.h"
#include "m1_store.h"

Q_LOGGING_CATEGORY(g_cat_lv2_members, "lv2.members_access")
Q_LOGGING_CATEGORY(g_cat_lv2_constructors, "lv2.constructors")

// ---------------------------------- Constructors and instantiation from mmap() data -------------------------------------------
/** \defgroup IWInst ItemWrapper Instantiation and destruction
 *
 *  ItemWrappers (in their 4 subclass avatars) are instantiated through the ItemWrapper::instantiateFromMMap() static method.
 *  They are obtained from outside the class through either mbd_getNew() (new item creation) or mbd_getExisting
 *  (access to an existing one). The "mbd_" prefix stands for "must be deleted"
 *  @{
 */

/**
 * @brief constructor from an existing Item_lv0* (normally coming from the mmap() item area)
 * @param p_item the pointer to the Item_lv0, to be stored as m_item
 */
M1Store::ItemWrapper::ItemWrapper(Item_lv0* p_item){m_item = p_item;}
/**
 * @brief constructor from an existing Item_lv0* (normally coming from the mmap() item area).
 * Does nothing apart from calling the parent constructor
 * @param p_item the pointer to the Item_lv0, to be stored as m_item
 */
M1Store::ItemWrapperFullVertex::ItemWrapperFullVertex(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->item_id());
}
/**
 * @brief constructor from an existing Item_lv0* (normally coming from the mmap() item area).
 * Does nothing apart from calling the parent constructor
 * @param p_item the pointer to the Item_lv0, to be stored as m_item
 */
M1Store::ItemWrapperSimpleVertex::ItemWrapperSimpleVertex(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->item_id());
}
/**
 * @brief constructor from an existing Item_lv0* (normally coming from the mmap() item area).
 * Does nothing apart from calling the parent constructor
 * @param p_item the pointer to the Item_lv0, to be stored as m_item
 */
M1Store::ItemWrapperFullEdge::ItemWrapperFullEdge(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->item_id());
}
/**
 * @brief constructor from an existing Item_lv0* (normally coming from the mmap() item area).
 * Does nothing apart from calling the parent constructor
 * @param p_item the pointer to the Item_lv0, to be stored as m_item
 */
M1Store::ItemWrapperSimpleEdge::ItemWrapperSimpleEdge(Item_lv0* p_item) : ItemWrapper(p_item){
    qCDebug(g_cat_lv2_constructors) << QString("p_item: %1").arg(p_item->item_id());
}
/**
 * @brief get a new ItemWrapper of the appropriate subtype from an existing Item_lv0* (normally from the mmap() area)
 * @param p the pointer from the mmap() area
 * @return an ItemWrapper pointer or the correct subtype depending on p's category
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_instantiateFromMMap(Item_lv0* p){
    qCDebug(g_cat_lv2_constructors) << QString("instantiateFromMMap from lv0 ID: %1").arg(p->item_id());
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
 * @brief get an ItemWrapper for a new item, with a new ItemID, instantiated from mmap()
 * @param p_flags the flags of the new item
 * @param p_type the type of the new item
 * @return an ItemWrapper pointer for the just created item
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getNew(const FlagField p_flags, const ItemType& p_type){
    qCDebug(g_cat_lv2_constructors) << QString("new Item p_flags: 0b%1 p_type: %2")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString());               // %2

    M1Store::Item_lv0* l_item = M1Store::Storage::getNewItemSlotPointer(p_flags, p_type);
    M1Store::ItemWrapper* l_wrapper = M1Store::ItemWrapper::mbd_instantiateFromMMap(l_item);
    // build default connexions
    l_wrapper->defaultConnections();
    qCDebug(g_cat_lv2_constructors) << "end getNew(FlagField, ItemType)";
    return l_wrapper;
}

/**
 * @brief get an ItemWrapper for a new item, with a new ItemID, instantiated from mmap()
 * @param p_flags the flags of the new item
 * @param p_type the type of the new item
 * @param p_label the label (item text) of the new item
 * @return an ItemWrapper pointer for the just created item
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getNew(const FlagField p_flags, const ItemType& p_type , const char* p_label){
    qCDebug(g_cat_lv0_members) << QString("new Item p_flags: 0b%1 p_type: %2 p_label: %3")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString())                // %2
                                .arg(p_label);                          // %3

    M1Store::ItemWrapper* l_wrapper = mbd_getNew(p_flags, p_type);
    qCDebug(g_cat_lv2_constructors) << QString("Setting text to: %1").arg(p_label);
    l_wrapper->setText(p_label);
    qCDebug(g_cat_lv2_constructors) << "end getNew(FlagField, ItemType, char*)";
    return l_wrapper;
}

/**
 * @brief get an ItemWrapper* for a new item, with a new ItemID, instantiated from mmap() + create an associated special item
 * @param p_flags the flags of the new item
 * @param p_type the type of the new item
 * @param p_label the label (item text) of the new item
 * @param p_flags_special the flags of the newly created special item
 * @param p_mnemonic the mnemonic of the newly created special item
 * @return an ItemWrapper pointer for the just created item
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getNew(
    const FlagField p_flags, const ItemType p_type , const char* p_label,
    const FlagField p_flags_special, const char* p_mnemonic){

    qCDebug(g_cat_lv2_constructors) << QString("new Item p_flags: 0b%1 p_type: %2 p_label: %3 p_flags_special %4 p_menonic %5")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString())                // %2
                                .arg(p_label)                           // %3
                                .arg(p_flags_special, 64, 2, QLatin1Char('0'))  // %4
                                .arg(p_mnemonic);                       // %5

    M1Store::ItemWrapper* l_wrapper = mbd_getNew(p_flags, p_type, p_label);
    qCDebug(g_cat_lv2_constructors) << QString("Creating new special: %1").arg(p_mnemonic);
    M1Store::Storage::newSpecial(l_wrapper->itemID(), p_flags_special, p_mnemonic);
    return l_wrapper;
}

/**
 * @brief get an ItemWrapper* for an existing item
 * @param p_item_id the ItemID to build the ItemWrapper for
 * @return the corresponding ItemWrapper* or nullptr if p_item_id is G_VOID_ITEM_ID
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getExisting(const ItemID p_item_id){
    qCDebug(g_cat_lv2_constructors) << QString("getExisting from ItemID: %1").arg(p_item_id);
    if(p_item_id == G_VOID_ITEM_ID) return nullptr;
    return M1Store::ItemWrapper::mbd_instantiateFromMMap(
        M1Store::Storage::getItemSlotPointer(p_item_id));
}

/**
 * @brief get an ItemWrapper* for an existing item
 * @param p_mnemonic the mnemonic to build the ItemWrapper for
 * @return the corresponding ItemWrapper*
 */
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getExisting(const char* p_mnemonic){
    qCDebug(g_cat_lv2_constructors) << QString("getExisting from mnemonic: %1").arg(p_mnemonic);
    return mbd_getExisting(M1Store::Storage::getSpecial(p_mnemonic)->itemId());
}

/**
 * @brief destructor
 */
M1Store::ItemWrapper::~ItemWrapper(){
    qCDebug(g_cat_lv2_constructors) << QString("~ItemWrapper()");

#if CMAKE_BUILD_TYPE == Debug
    if(m_must_delete){
        qCDebug(g_cat_lv2_constructors) << "deleting m_item";
        delete m_item;
    }
#endif
}
/** @}*/

// ---------------------------------- Debug data -------------------------------------------
/**
 * @brief Launching the graph recursive dump
 * @param p_item_id starting point (generally ROOT)
 */
void M1Store::ItemWrapper::dbgRecurGraphStart(const ItemID p_item_id){
    std::set<ItemID> l_already_expanded;
    dbgRecurGraph(p_item_id, l_already_expanded, "");
}

/**
 * @brief Graph recursive dump
 * @param p_item_id starting point (and starting points of subtrees)
 * @param p_already_expanded set of already visited items (not to be done again)
 * @param p_left padding string to print to the left with presence and absence of | where appropriate
 */
void M1Store::ItemWrapper::dbgRecurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left){
    // if p_item_id is G_VOID_ITEM_ID (you never know) or p_item_id has already been visited --> do nothing and return
    if(p_already_expanded.find(p_item_id) == p_already_expanded.end() and p_item_id != G_VOID_ITEM_ID){
        // add current item to the already visited set
        p_already_expanded.insert(p_item_id);

        ItemWrapper* l_current = mbd_getExisting(p_item_id);
        // print an item debug one-liner
        qDebug(g_cat_silence) << p_left + l_current->dbgShort();

        // go through special edges of current item (if any) ...
        if(ItemWrapper* l_edge = l_current->mbd_getFirstEdgeSpecialWrapper()){
            // NB getFirstEdgeSpecial() will return nullptr if there are no special edges
            ItemID l_stop_id = l_edge->itemID(); // this id is the first AND last edge bc edges are arranged in a doubly linked ring
            // loop through all special edges
            do{
                ItemWrapper* l_next_edge = l_edge->mbd_getNextWrapper();
                // debug one liner of current edge ("S" = "special")
                qDebug(g_cat_silence) << p_left + "+--S" + l_edge->dbgHalf();
                // recursive call to recurGraph on the target of the current edge
                dbgRecurGraph(l_edge->getTargetID(), p_already_expanded,
                           // the padding will be "|  " unless it is the last recusive call for this item ie if the current edge is the last
                           // special one and there are no ordinary edges (rare case but you never know)
                           p_left + (((l_next_edge->itemID() == l_stop_id) && (l_current->getFirstEdgeID() == G_VOID_ITEM_ID)) ? "   " : "|  "));

                // release the current edge ItemWrapper and replace it with the ne current edge obtained previously (l_next_edge)
                delete l_edge;
                l_edge = l_next_edge;
            }while(l_edge->itemID() != l_stop_id);
            // finally, release the last edge ItemWrapper
            delete l_edge;
        }
        // ... and edges (if any)
        if(ItemWrapper* l_edge = l_current->mbd_getFirstEdgeWrapper()){
            // NB getFirstEdgeSpecial() will return nullptr if there are no edges
            ItemID l_stop_id = l_edge->itemID();  // this id is the first AND last edge bc edges are arranged in a doubly linked ring
            // loop through all ordinary edges
            do{
                // debug one liner of current edge ("O" = "ordinary")
                qDebug(g_cat_silence) << p_left + "+--O" + l_edge->dbgHalf();
                ItemWrapper* l_next_edge = l_edge->mbd_getNextWrapper();
                // recursive call to recurGraph on the target of the current edge
                dbgRecurGraph(l_edge->getTargetID(), p_already_expanded,
                           // the padding will be "|  " unless it is the last recusive call for this item ie if
                           // the current edge is the last ordinary one
                           p_left + ((l_next_edge->itemID() == l_stop_id) ? QString("   ") : QString("|  ")));

                // release the current edge ItemWrapper and replace it with the ne current edge obtained previously (l_next_edge)
                delete l_edge;
                l_edge = l_next_edge;
            }while(l_edge->itemID() != l_stop_id);
            // finally, release the last edge ItemWrapper
            delete l_edge;
        }
        delete l_current;
    }
}

/**
 * @brief base debug dump for the base ItemWrapper class (not supposed to be called directly but from subclasses)
 *
 * This is a multi-line debug representation that will appear at the top of that of all 4 derived classes
 *
 * @return the debug string
 */
QString M1Store::ItemWrapper::dbgString(){
    QString l_types(" [");
    for(int i = 0; i < 4; i++)
        if(m_item->getType(i) != G_VOID_SI_ID)
            l_types += Storage::getSpecialSlotPointer(m_item->getType(i))->mnemonic();
    if(l_types.length() == 2) l_types = "";
    else l_types += "]";
    return QString("\n-----------------------------------------------------------------------------------------\n%1\n%2\n%3\n")
        .arg(QString("m_id                 : 0x%1").arg(m_item->m_item_id, 16, 16, QLatin1Char('0')))
        .arg(QString("m_flags              : 0b%1").arg(m_item->m_flags, 64, 2, QLatin1Char('0')))
        .arg(QString("m_type               : %1").arg(m_item->m_type.dbgString() + l_types));
}
/**
 * @brief multi-line debug dump for full vertices
 * @return the debug string
 */
QString M1Store::ItemWrapperFullVertex::dbgString(){
    // string to contain the representation of edges coming off of this item
    QString l_edges;
    // list of edges construction : same logic as recurGraph() above, minus the recursiveness
    // special edges
    if(ItemWrapper* l_current_edge = mbd_getExisting(m_item->firstEdgeSpecial())){
        l_edges += "\nSpecial edges:";
        ItemID l_stop_id = l_current_edge->itemID();
        do {
            l_edges += "\n" + l_current_edge->dbgShort();
            ItemWrapper* l_next_edge = l_current_edge->mbd_getNextWrapper();
            delete l_current_edge;
            l_current_edge = l_next_edge;
        } while (l_current_edge->itemID() != l_stop_id);
        delete l_current_edge;
    }
    // ordinary edges
    if(ItemWrapper* l_current_edge = mbd_getExisting(m_item->firstEdge())){
        l_edges += "\nOrdinary edges:";
        ItemID l_stop_id = l_current_edge->itemID();
        do {
            l_edges += "\n" + l_current_edge->dbgShort();
            ItemWrapper* l_next_edge = l_current_edge->mbd_getNextWrapper();
            delete l_current_edge;
            l_current_edge = l_next_edge;
        } while (l_current_edge->itemID() != l_stop_id);
        delete l_current_edge;
    }

    // add the rest to the returned string
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
            .arg(l_text.length() > 100 ? "..." : "")) + l_edges;
}

/**
 * @brief multi-line debug dump for simple vertices
 * @return the debug string
 */
QString M1Store::ItemWrapperSimpleVertex::dbgString(){
    return M1Store::ItemWrapper::dbgString() +
           QString("--------------------------- SIMPLE VERTEX -----------------------------------------------\n") +
           QString("%1")
        .arg(QString("text                 : %1").arg(m_item->text()));
}

/**
 * @brief multi-line debug dump for full edges
 * @return the debug string
 */
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

/**
 * @brief multi-line debug dump for simple edges
 * @return the debug string
 */
QString M1Store::ItemWrapperSimpleEdge::dbgString(){
    return M1Store::ItemWrapper::dbgString() +
           QString("--------------------------- SIMPLE EDGE -------------------------------------------------\n") +
           QString("%1\n%2\n%3\n%4")
        .arg(QString("m_v_origin           : 0x%1 %2").arg(m_item->origin(), 16, 16, QLatin1Char('0')).arg(m_item->origin()))
        .arg(QString("m_e_previous         : 0x%1 %2").arg(m_item->previous(), 16, 16, QLatin1Char('0')).arg(m_item->previous()))
        .arg(QString("m_e_next             : 0x%1 %2").arg(m_item->next(), 16, 16, QLatin1Char('0')).arg(m_item->next()))
        .arg(QString("text                 : %1").arg(m_item->text()));
}

/**
 * @brief mnemonic of first special item type or _____ if none
 * @return the mnemonic
 */
QString M1Store::ItemWrapper::dbgTypeShort(){
    if(m_item->getType(0) == G_VOID_SI_ID)
        return "_____";
    else
        return Storage::getSpecialSlotPointer(m_item->getType(0))->mnemonic();
}

/**
 * @brief One liner debug representation - not supposed to be called on base class
 * @param p_depth is there only for ItemWrapperFullEdge::dbgShort(int p_depth)
 * @return the one-liner
 */
QString M1Store::ItemWrapper::dbgShort(int p_depth){
    return QString();
}

/**
 * @brief One liner debug representation for full vertices
 * @param p_depth is there only for ItemWrapperFullEdge::dbgShort(int p_depth)
 * @return the one-liner
 */
QString M1Store::ItemWrapperFullVertex::dbgShort(int p_depth){
    return QString("ITEM [%1] %2%3")
        .arg(dbgTypeShort())
        .arg(m_item->text())
        .arg((m_item->flags() & IS_SPECIAL) ? QString(" (%1)").arg(M1Store::Storage::getSpecial(m_item->item_id())->mnemonic()) : "");
}

/**
 * @brief One liner debug representation for simple vertices
 * @param p_depth is there only for ItemWrapperFullEdge::dbgShort(int p_depth)
 * @return the one-liner
 */
QString M1Store::ItemWrapperSimpleVertex::dbgShort(int p_depth){
    return QString("VFLD [%1]").arg(m_item->text());
}

/**
 * @brief One liner debug representation for full edges
 *
 * This method calls itself because edges may link to other edges and other edges, and so on. only 4 levels of this are allowed, through p_depth
 *
 * @param p_depth to avoid looping if there are cycles among edges
 * @return the one-liner
 */
QString M1Store::ItemWrapperFullEdge::dbgShort(int p_depth){
    Q_ASSERT_X(m_item->origin() != G_VOID_ITEM_ID && m_item->target() != G_VOID_ITEM_ID,
               "ItemWrapperFullEdge::dbgShort())", "origin or target = G_VOID_ITEM_ID");

    // to break cycles
    if(p_depth > 4) return("...");

    M1Store::ItemWrapper* l_origin = ItemWrapper::mbd_getExisting(m_item->origin());
    M1Store::ItemWrapper* l_target = ItemWrapper::mbd_getExisting(m_item->target());

    QString l_ret = QString("EDGE [%1] {%2}-->{%3}").arg(dbgTypeShort()).arg(l_origin->dbgShort(p_depth + 1)).arg(l_target->dbgShort(p_depth + 1));

    delete l_origin;
    delete l_target;
    return l_ret;
}

/**
 * @brief One liner debug representation for full edges
 * @param p_depth is there only for ItemWrapperFullEdge::dbgShort(int p_depth)
 * @return the one-liner
 */
QString M1Store::ItemWrapperSimpleEdge::dbgShort(int p_depth){
    return QString("EFLD [%1]").arg(m_item->text());
}

/// only for full edges
QString M1Store::ItemWrapper::dbgHalf(){return "";}
/// only for full edges
QString M1Store::ItemWrapperFullVertex::dbgHalf(){qFatal("Aborting / Cannot get dbgHalf on a full vertex"); return "";}
/// only for full edges
QString M1Store::ItemWrapperSimpleVertex::dbgHalf(){qFatal("Aborting / Cannot get dbgHalf on a full vertex"); return "";}
/**
 * @brief a debug one-liner which is "half" of dbgShort(), i.e. only the target part
 * @return the one-liner
 */
QString M1Store::ItemWrapperFullEdge::dbgHalf(){
    Q_ASSERT_X(m_item->target() != G_VOID_ITEM_ID,
               "ItemWrapperFullEdge::dbgHalf())", "target = G_VOID_ITEM_ID");

    M1Store::ItemWrapper* l_target = ItemWrapper::mbd_getExisting(m_item->target());

    QString l_ret = QString("-[%1]-->{%2}").arg(dbgTypeShort()).arg(l_target->dbgShort());

    delete l_target;
    return l_ret;
}
/// only for full edges
QString M1Store::ItemWrapperSimpleEdge::dbgHalf(){return dbgShort();}

// ---------------------------------- member access -------------------------------------------

/** \defgroup IWretIW ItemWrapper methods returning new ItemWrappers
 *
 *  Most of these methods can return a nullptr (if the underlying ItemID is G_VOID_ITEM_ID).
 *  As a result, they have to be called within if() constructs to properly treat this case (see e.g. recurGraph())
 *
 *  Will trigger an abort if called on the wrong subtype
 *  @{
 */
/// First Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getFirstEdgeWrapper(){return mbd_getExisting(m_item->firstEdge());}
/// First Special Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getFirstEdgeSpecialWrapper(){return mbd_getExisting(m_item->firstEdgeSpecial());}
/// First Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperFullVertex::mbd_getFirstEdgeWrapper(){return ItemWrapper::mbd_getFirstEdgeWrapper();}
/// First Special Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperFullVertex::mbd_getFirstEdgeSpecialWrapper(){return ItemWrapper::mbd_getFirstEdgeSpecialWrapper();}
/// First Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleVertex::mbd_getFirstEdgeWrapper(){qFatal("Aborting / Cannot get first edge of a simple vertex"); return nullptr;}
/// First Special Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleVertex::mbd_getFirstEdgeSpecialWrapper(){qFatal("Aborting / Cannot get first special edge of a simple vertex"); return nullptr;}
/// First Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperFullEdge::mbd_getFirstEdgeWrapper(){return ItemWrapper::mbd_getFirstEdgeWrapper();}
/// First Special Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperFullEdge::mbd_getFirstEdgeSpecialWrapper(){return ItemWrapper::mbd_getFirstEdgeSpecialWrapper();}
/// First Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleEdge::mbd_getFirstEdgeWrapper(){qFatal("Aborting / Cannot get first edge of a simple edge"); return nullptr;}
/// First Special Edge wrapper or nullptr if none
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleEdge::mbd_getFirstEdgeSpecialWrapper(){qFatal("Aborting / Cannot get first special edge of a simple edge"); return nullptr;}

/// Wapper for next edge (only for edges) does NOT ever return a nullptr
M1Store::ItemWrapper* M1Store::ItemWrapper::mbd_getNextWrapper(){
    Q_ASSERT_X(m_item->next() != G_VOID_ITEM_ID,
               "ItemWrapper::getNext()", "m_item->next() == G_VOID_ITEM_ID");

    return mbd_getExisting(m_item->next());
}
/// Wapper for next edge (only for edges) does NOT ever return a nullptr
M1Store::ItemWrapper* M1Store::ItemWrapperFullVertex::mbd_getNextWrapper(){ qFatal() << "Cannot get next edge on a vertex (full)"; return nullptr;}
/// Wapper for next edge (only for edges) does NOT ever return a nullptr
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleVertex::mbd_getNextWrapper(){qFatal() << "Cannot get next edge on a vertex (simple)"; return nullptr;}
/// Wapper for next edge (only for edges) does NOT ever return a nullptr
M1Store::ItemWrapper* M1Store::ItemWrapperFullEdge::mbd_getNextWrapper(){return ItemWrapper::mbd_getNextWrapper();}
/// Wapper for next edge (only for edges) does NOT ever return a nullptr
M1Store::ItemWrapper* M1Store::ItemWrapperSimpleEdge::mbd_getNextWrapper(){return ItemWrapper::mbd_getNextWrapper();}
/** @}*/

/** \defgroup IWsimp ItemWrapper simple methods
 *
 * Nothing special - does what it says in the name - will trigger an abort if called on the wrong subtype
 *  @{
 */
/// get target id (only for full edges)
M1Store::ItemID M1Store::ItemWrapper::getTargetID(){return m_item->target();}
/// get target id (only for full edges)
M1Store::ItemID M1Store::ItemWrapperFullVertex::getTargetID(){qFatal("Aborting / Cannot get target of a full vertex"); return G_VOID_ITEM_ID;}
/// get target id (only for full edges)
M1Store::ItemID M1Store::ItemWrapperSimpleVertex::getTargetID(){qFatal("Aborting / Cannot get target of a simple vertex"); return G_VOID_ITEM_ID;}
/// get target id (only for full edges)
M1Store::ItemID M1Store::ItemWrapperFullEdge::getTargetID(){return m_item->target();}
/// get target id (only for full edges)
M1Store::ItemID M1Store::ItemWrapperSimpleEdge::getTargetID(){qFatal("Aborting / Cannot get target of a simple edge"); return G_VOID_ITEM_ID;}

/// set next id (only for edges)
void M1Store::ItemWrapper::setNext(ItemID p_edge_id){m_item->setNext(p_edge_id);}
/// set next id (only for edges)
void M1Store::ItemWrapperFullVertex::setNext(ItemID p_edge_id){qFatal() << "Cannot set next edge on a vertex (full)";}
/// set next id (only for edges)
void M1Store::ItemWrapperSimpleVertex::setNext(ItemID p_edge_id){qFatal() << "Cannot set next edge on a vertex (simple)";}
/// set next id (only for edges)
void M1Store::ItemWrapperFullEdge::setNext(ItemID p_edge_id){ItemWrapper::setNext(p_edge_id);}
/// set next id (only for edges)
void M1Store::ItemWrapperSimpleEdge::setNext(ItemID p_edge_id){ ItemWrapper::setNext(p_edge_id);}

/// set previous id (only for edges)
void M1Store::ItemWrapper::setPrevious(ItemID p_edge_id){m_item->setPrevious(p_edge_id);}
/// set previous id (only for edges)
void M1Store::ItemWrapperFullVertex::setPrevious(ItemID p_edge_id){qFatal() << "Cannot set next edge on a vertex (full)";}
/// set previous id (only for edges)
void M1Store::ItemWrapperSimpleVertex::setPrevious(ItemID p_edge_id){qFatal() << "Cannot set next edge on a vertex (simple)";}
/// set previous id (only for edges)
void M1Store::ItemWrapperFullEdge::setPrevious(ItemID p_edge_id){ItemWrapper::setPrevious(p_edge_id);}
/// set previous id (only for edges)
void M1Store::ItemWrapperSimpleEdge::setPrevious(ItemID p_edge_id){ItemWrapper::setPrevious(p_edge_id);}

/// get first edge id (only for full items)
M1Store::ItemID M1Store::ItemWrapper::getFirstEdgeID(){return m_item->firstEdge();}
/// get first edge id (only for full items)
M1Store::ItemID M1Store::ItemWrapperFullVertex::getFirstEdgeID(){return ItemWrapper::getFirstEdgeID();}
/// get first edge id (only for full items)
M1Store::ItemID M1Store::ItemWrapperSimpleVertex::getFirstEdgeID(){qFatal() << "Cannot get first edge ID on a simple vertex";return G_VOID_ITEM_ID;}
/// get first edge id (only for full items)
M1Store::ItemID M1Store::ItemWrapperFullEdge::getFirstEdgeID(){return ItemWrapper::getFirstEdgeID();}
/// get first edge id (only for full items)
M1Store::ItemID M1Store::ItemWrapperSimpleEdge::getFirstEdgeID(){qFatal() << "Cannot get first edge ID on a simple edge";return G_VOID_ITEM_ID;}
/** @}*/

// ---------------------------------- other methods (critical ones) -------------------------------------------

/**
 * @brief called after the creation of a new full vertex. Creates mew edges based on category and type (AUTO, ISA, ...)
 */
void M1Store::ItemWrapperFullVertex::defaultConnections(){
    qCDebug(g_cat_lv2_members) << QString("Default connections for full vertex [%1]").arg(m_item->item_id());
    // creation of the AUTO edge
    M1Store::ItemWrapperFullEdge* l_auto_edge =
        // dynamic cast possible because has to be full edge according to category flags
        dynamic_cast<M1Store::ItemWrapperFullEdge*>(
        // does not use special idem id constants because may not be set at this time
        mbd_getNew(FULL_EDGE | IS_AUTO,
               M1Store::ItemType(M1Store::Storage::getSpecialID("AUTO_"),
                                 G_VOID_SI_ID,
                                 G_VOID_SI_ID,
                                 G_VOID_SI_ID))
    );
    // loops onto this vertex
    l_auto_edge->setTarget(m_item->item_id());
    l_auto_edge->setOrigin(m_item->item_id());

    // it is the first edge so it links to itself
    l_auto_edge->setPrevious(l_auto_edge->itemID());
    l_auto_edge->setNext(l_auto_edge->itemID());

    // attach this adge as the first ordinary one
    this->setFirstEdge(l_auto_edge->itemID());

    // create type edges if required
    createTypeEdges();
    qCDebug(g_cat_lv2_members) << QString("Default connections for full vertex end: %1").arg(m_item->item_id());
    delete l_auto_edge;
}
/// no default connexions for simple vertices
void M1Store::ItemWrapperSimpleVertex::defaultConnections(){
    qCDebug(g_cat_lv2_members) << "Default connections for simple vertex";
}
/// only creation of type edges if required by edge type
void M1Store::ItemWrapperFullEdge::defaultConnections(){
    qCDebug(g_cat_lv2_members) << "Default connections for full edge";
    createTypeEdges();
    qCDebug(g_cat_lv2_members) << "Default connections for full edge end";
}
/// no default connexions for simple vertices
void M1Store::ItemWrapperSimpleEdge::defaultConnections(){
    qCDebug(g_cat_lv2_members) << "Default connections for simple edge";
}

/**
 * @brief creation if ISA edges if required by m_item SpecialItemID types
 */
void M1Store::ItemWrapper::createTypeEdges(){
    qCDebug(g_cat_lv2_members) << QString("Creating type edges if required for [%1]").arg(m_item->item_id());
    // nothing to do if this item has an ItemID type
    if((m_item->flags() & TYPE_IS_ITEM_ID) == 0)
        // not an ItemID type therefore 4 special item ids - loop through them all
        for(int l_slot = 0; l_slot < 4; l_slot++){
            // special item id from this slot
            SpecialItemID l_si = m_item->getType(l_slot);
            if(l_si != G_VOID_SI_ID){ // G_VOID_SI_ID --> nothing to do
                SpecialItem* l_type_si = M1Store::Storage::getSpecialSlotPointer(l_si);
                qCDebug(g_cat_lv2_members) << QString("Create edge to [%1] ? flags = 0b%2")
                                                  .arg(l_type_si->mnemonic()).arg(l_type_si->flags(), 64, 2, QChar('0'));
                // create edge if SI_REQUIRES_EDGE flag is set in the special item record
                if(l_type_si->flags() & SI_REQUIRES_EDGE) // this type must be materialized with an edge (not just a special type id)
                    linkTo(l_type_si->itemId(), "_ISA_");
            }
        }
    qCDebug(g_cat_lv2_members) << "Creating type edges end";
}
/// do it for full vertices
void M1Store::ItemWrapperFullVertex::createTypeEdges(){
    ItemWrapper::createTypeEdges();
}
/// illegal for simples
void M1Store::ItemWrapperSimpleVertex::createTypeEdges(){
    qFatal("Aborting / Cannot add a type edge to a simple vertex");
}
/// do it for full edges
void M1Store::ItemWrapperFullEdge::createTypeEdges(){
    ItemWrapper::createTypeEdges();
}
/// illegal for simples
void M1Store::ItemWrapperSimpleEdge::createTypeEdges(){
    qFatal("Aborting / Cannot add a type edge to a simple edge");
}

/**
 * @brief Type attribution with a special item type
 * @param p_type_si a SpecialItem pointer for the type to be attributed
 * @return true/false depending on success/failure
 */
bool M1Store::ItemWrapper::setType(const SpecialItem* p_type_si){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(p_type_si->mnemonic());

    // index of first free slot (or -1 if none)
    int l_free_slot = m_item->getType(0) == G_VOID_SI_ID ? 0 :
                          (m_item->getType(1) == G_VOID_SI_ID ? 1 :
                            (m_item->getType(2) == G_VOID_SI_ID ? 2 : m_item->getType(3) == G_VOID_SI_ID ? 3 : -1));

    // store the special item id in m_type if possible
    if(l_free_slot >= 0)
        m_item->setType(l_free_slot, p_type_si->specialId());

    // cannot go any further for simple edges and simple verices (may return false if no slot was available)
    if(isSimpleEdge() || isSimpleVertex()) return l_free_slot >= 0;

    // full items only from this point onwards
    // create an ISA edge if require (no slot available or SI_REQUIRES_EDGE is set)
    if((l_free_slot < 0) || (p_type_si->flags() & SI_REQUIRES_EDGE))
        linkTo(p_type_si->itemId(), "_ISA_");

    // will always succeed for full edges and vertices (always possible to materialyse type with an ISA edge)
    return true;
}
/**
 * @brief Type attribution with a special item type
 * @param p_type_id the SpecialItemID for the type to be attributed
 * @return true/false depending on success/failure
 */
bool M1Store::ItemWrapper::setType(const SpecialItemID p_type_id){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(M1Store::Storage::getSpecialSlotPointer(p_type_id)->mnemonic());
    return setType(M1Store::Storage::getSpecialSlotPointer(p_type_id));
}
/**
 * @brief Type attribution with a special item type
 * @param p_mnemonic the mnemonic of the type to be attributed
 * @return true/false depending on success/failure
 */
bool M1Store::ItemWrapper::setType(const char* p_mnemonic){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(p_mnemonic);
    return setType(M1Store::Storage::getSpecial(p_mnemonic));
}

/**
 * @brief link an item to another
 * @param p_target the target (as an ItemWrapper)
 * @param p_type_edge the edge type (as a SpecialItemID)
 */
void M1Store::ItemWrapper::linkTo(ItemWrapper* p_target, const SpecialItemID p_type_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]")
                    .arg(p_target->dbgShort())
                    .arg(M1Store::Storage::getSpecialSlotPointer(p_type_edge)->mnemonic());
    // cannot link simple items to anything
    Q_ASSERT_X(isFullVertex() || isFullEdge(),
               "ItemWrapper::linkTo()", "cannot link simple items to anything");


    // the new edge from this item to the target
    M1Store::ItemWrapperFullEdge* l_new_edge =
        dynamic_cast<M1Store::ItemWrapperFullEdge*>(mbd_getNew(FULL_EDGE, M1Store::ItemType(p_type_edge, G_VOID_SI_ID, G_VOID_SI_ID, G_VOID_SI_ID)));
    l_new_edge->setTarget(p_target->itemID());
    l_new_edge->setOrigin(m_item->item_id());

    // creation of a reciprocal edge if necessary
    if(M1Store::Storage::getSpecialSlotPointer(p_type_edge)->flags() & SI_HAS_RECIPROCAL){
        // the reciprocal edge
        SpecialItemID l_reciprocal_id = M1Store::Storage::getSpecialSlotPointer(p_type_edge)->reciprocalId();
        M1Store::ItemWrapperFullEdge* l_new_edge_reciprocal =
            dynamic_cast<M1Store::ItemWrapperFullEdge*>(
            mbd_getNew(FULL_EDGE, M1Store::ItemType(l_reciprocal_id, 0, 0, 0))
        );
        l_new_edge_reciprocal->setOrigin(p_target->itemID());
        l_new_edge_reciprocal->setTarget(m_item->item_id());

        // mutual hook-up of the 2 edges
        l_new_edge->setReciprocal(l_new_edge_reciprocal->itemID());
        l_new_edge_reciprocal->setReciprocal(l_new_edge->itemID());

        // insert the reciprocal edge into one of the edge rings (special or ordinary) of the target item
        p_target->addFullEdge(l_new_edge_reciprocal, l_reciprocal_id);

        delete l_new_edge_reciprocal;
    }
    // insert the direct edge into one of the edge rings (special or ordinary) of this item
    this->addFullEdge(l_new_edge, p_type_edge);
    delete l_new_edge;
}
/**
 * @brief link an item to another
 * @param p_target the target (as an ItemWrapper)
 * @param p_mnemonic_edge the edge type (as a mnemonic)
 */
void M1Store::ItemWrapper::linkTo(ItemWrapper* p_target, const char* p_mnemonic_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]").arg(p_target->dbgShort()).arg(p_mnemonic_edge);

    linkTo(p_target, Storage::getSpecial(p_mnemonic_edge)->specialId());
}
/**
 * @brief link an item to another
 * @param p_target_id the target (as an ItemID)
 * @param p_type the edge type (as a SpecialItemID)
 */
void M1Store::ItemWrapper::linkTo(ItemID p_target_id, const SpecialItemID p_type){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]").arg(p_target_id).arg(p_type);
    Q_ASSERT_X(p_target_id != G_VOID_ITEM_ID,
               "ItemWrapper::linkTo())", "p_target_id = G_VOID_ITEM_ID");

    M1Store::ItemWrapper* l_target = mbd_getExisting(p_target_id);
    linkTo(l_target, p_type);
    delete l_target;
}
/**
 * @brief link an item to another
 * @param p_target_id the target (as an ItemID)
 * @param p_mnemonic_edge the edge type (as a mnemonic)
 */
void M1Store::ItemWrapper::linkTo(ItemID p_target_id, const char* p_mnemonic_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]").arg(p_target_id).arg(p_mnemonic_edge);
    Q_ASSERT_X(p_target_id != G_VOID_ITEM_ID,
               "ItemWrapper::linkTo())", "p_target_id = G_VOID_ITEM_ID");

    M1Store::ItemWrapper* l_target = mbd_getExisting(p_target_id);
    linkTo(l_target, p_mnemonic_edge);
    delete l_target;
}

/**
 * @brief insert a newly created edge into one of the 2 edge rings (special or ordinary) of this item
 * @param p_new_edge the newly created edge (as a ItemWrapperFullEdge)
 * @param p_edge_type the edge type, as SpecialItemID
 */
void M1Store::ItemWrapper::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    ItemID new_edge_id = p_new_edge->itemID();
    // determine in which ring the edge is to be inserted
    bool edge_is_special = M1Store::Storage::getSpecialSlotPointer(p_edge_type)->flags() & SI_IS_SPECIAL_EDGE;

    ItemID first_edge_id = edge_is_special ? m_item->firstEdgeSpecial() : m_item->firstEdge();

    if(first_edge_id == G_VOID_ITEM_ID){ // case in which this item does not have any edge yet in the appropriate ring
        // link the new edge to itself
        p_new_edge->setPrevious(new_edge_id);
        p_new_edge->setNext(new_edge_id);

        // th edge becomes the new first edge of the appropriate ring
        if(edge_is_special)
            // add to the special edge ring
            m_item->setFirstEdgeSpecial(p_new_edge->itemID());
        else
            // add to the visible edge ring
            m_item->setFirstEdge(p_new_edge->itemID());
    }
    else{
        // insert the edge BELOW the current first edge
        M1Store::ItemWrapper* l_previous_edge = mbd_getExisting(first_edge_id);
        M1Store::ItemWrapper* l_next_edge = l_previous_edge->mbd_getNextWrapper();

        // appropriate next/previous hookups
        p_new_edge->setPrevious(first_edge_id);
        p_new_edge->setNext(l_next_edge->itemID());

        l_previous_edge->setNext(new_edge_id);
        l_next_edge->setPrevious(new_edge_id);

        delete l_previous_edge;
        delete l_next_edge;
    }
}
/// insert a newly created edge into one of the 2 edge rings (special or ordinary) of this full vertex
void M1Store::ItemWrapperFullVertex::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    ItemWrapper::addFullEdge(p_new_edge, p_edge_type);
}
/// cannot be done on a simple vertex
void M1Store::ItemWrapperSimpleVertex::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    qFatal("Aborting / Cannot add an edge to a simple vertex");
}
/// insert a newly created edge into one of the 2 edge rings (special or ordinary) of this full edge
void M1Store::ItemWrapperFullEdge::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    ItemWrapper::addFullEdge(p_new_edge, p_edge_type);
}
/// cannot be done on a simple edge
void M1Store::ItemWrapperSimpleEdge::addFullEdge(ItemWrapperFullEdge* p_new_edge, const SpecialItemID p_edge_type){
    qFatal("Aborting / Cannot add an edge to a simple vertex");
}

