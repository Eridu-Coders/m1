#include "m1_lv2_item.h"
#include "m1_store.h"

Q_LOGGING_CATEGORY(g_cat_lv2_members, "lv2.members_access")
Q_LOGGING_CATEGORY(g_cat_lv2_constructors, "lv2.constructors")

// ---------------------------------- Constructors and instantiation from mmap() data -------------------------------------------
/** \defgroup IWInst ItemWrapper Instantiation and destruction
 *  @{
 */

/**
 * @brief get an ItemWrapper for a new item, with a new ItemID, instantiated from mmap()
 * @param p_flags the flags of the new item
 * @param p_type the type of the new item
 * @return an ItemWrapper pointer for the just created item
 */
M1Store::Item_lv2* M1Store::Item_lv2::getNew(const FlagField p_flags, const ItemType& p_type){
    qCDebug(g_cat_lv2_constructors) << QString("new Item p_flags: 0b%1 p_type: %2")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString());               // %2

    M1Store::Item_lv2* l_wrapper = static_cast<Item_lv2*>(M1Store::Storage::getNewtemSlotPointer(p_flags, p_type));
    // build default connexions
    l_wrapper->defaultConnections();
    qCDebug(g_cat_lv2_constructors) << "end getNewFlagField, ItemType)";
    return l_wrapper;
}

/**
 * @brief get an ItemWrapper for a new item, with a new ItemID, instantiated from mmap()
 * @param p_flags the flags of the new item
 * @param p_type the type of the new item
 * @param p_label the label (item text) of the new item
 * @return an ItemWrapper pointer for the just created item
 */
M1Store::Item_lv2* M1Store::Item_lv2::getNew(const FlagField p_flags, const ItemType& p_type , const char* p_label){
    qCDebug(g_cat_lv2_constructors) << QString("new Item p_flags: 0b%1 p_type: %2 p_label: %3")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString())                // %2
                                .arg(p_label);                          // %3
    
    M1Store::Item_lv2* l_wrapper = getNew(p_flags, p_type);
    qCDebug(g_cat_lv2_constructors) << QString("Setting text to: %1").arg(p_label);
    l_wrapper->setText(p_label);
    qCDebug(g_cat_lv2_constructors) << "end getNewFlagField, ItemType, char*)";
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
M1Store::Item_lv2* M1Store::Item_lv2::getNew(
    const FlagField p_flags, const ItemType p_type , const char* p_label,
    const FlagField p_flags_special, const char* p_mnemonic){

    qCDebug(g_cat_lv2_constructors) << QString("new Item p_flags: 0b%1 p_type: %2 p_label: %3 p_flags_special %4 p_menonic %5")
                                .arg(p_flags, 64, 2, QLatin1Char('0'))  // %1
                                .arg(p_type.dbgString())                // %2
                                .arg(p_label)                           // %3
                                .arg(p_flags_special, 64, 2, QLatin1Char('0'))  // %4
                                .arg(p_mnemonic);                       // %5
    
    M1Store::Item_lv2* l_wrapper = getNew(p_flags, p_type, p_label);
    qCDebug(g_cat_lv2_constructors) << QString("Creating new special: %1").arg(p_mnemonic);
    M1Store::Storage::newSpecial(l_wrapper->itemID(), p_flags_special, p_mnemonic);
    return l_wrapper;
}

/**
 * @brief get an ItemWrapper* for an existing item
 * @param p_item_id the ItemID to build the ItemWrapper for
 * @return the corresponding ItemWrapper* or nullptr if p_item_id is G_VOID_ITEM_ID
 */
M1Store::Item_lv2* M1Store::Item_lv2::getExisting(const ItemID p_item_id){
    qCDebug(g_cat_lv2_constructors) << QString("getExisting from ItemID: %1").arg(p_item_id);
    if(p_item_id == G_VOID_ITEM_ID) return nullptr;
    return static_cast<Item_lv2*>(M1Store::Storage::getItemSlotPointer(p_item_id));
}

/**
 * @brief get an ItemWrapper* for an existing item
 * @param p_mnemonic the mnemonic to build the ItemWrapper for
 * @return the corresponding ItemWrapper*
 */
M1Store::Item_lv2* M1Store::Item_lv2::getExisting(const char* p_mnemonic){
    qCDebug(g_cat_lv2_constructors) << QString("getExisting from mnemonic: %1").arg(p_mnemonic);
    return getExisting(M1Store::Storage::getSpecial(p_mnemonic)->itemId());
}

/**
 * @brief destructor
 */
M1Store::Item_lv2::~Item_lv2(){
    qCDebug(g_cat_lv2_constructors) << QString("~ItemWrapper()");
}
/** @}*/

// ---------------------------------- Debug data -------------------------------------------
/**
 * @brief Launching the graph recursive dump
 * @param p_item_id starting point (generally ROOT)
 */
void M1Store::Item_lv2::dbgRecurGraphStart(const ItemID p_item_id){
    std::set<ItemID> l_already_expanded;
    dbgRecurGraph(p_item_id, l_already_expanded, "");
}

/**
 * @brief Graph recursive dump
 * @param p_item_id starting point (and starting points of subtrees)
 * @param p_already_expanded set of already visited items (not to be done again)
 * @param p_left padding string to print to the left with presence and absence of | where appropriate
 */
void M1Store::Item_lv2::dbgRecurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left){
    // if p_item_id is G_VOID_ITEM_ID (you never know) or p_item_id has already been visited --> do nothing and return
    if(p_already_expanded.find(p_item_id) == p_already_expanded.end() and p_item_id != G_VOID_ITEM_ID){
        // add current item to the already visited set
        p_already_expanded.insert(p_item_id);

        Item_lv2* l_current = getExisting(p_item_id);
        // print an item debug one-liner
        qDebug(g_cat_silence) << p_left + l_current->dbgShort();

        // go through special edges of current item (if any) ...
        if(Item_lv2* l_edge = l_current->getFirstEdgeSpecialWrapper()){
            // NB getFirstEdgeSpecial() will return nullptr if there are no special edges
            ItemID l_stop_id = l_edge->itemID(); // this id is the first AND last edge bc edges are arranged in a doubly linked ring
            // loop through all special edges
            do{
                Item_lv2* l_next_edge = l_edge->getNextWrapper();
                // debug one liner of current edge ("S" = "special")
                qDebug(g_cat_silence) << p_left + "+--S" + l_edge->dbgHalf();
                // recursive call to recurGraph on the target of the current edge
                dbgRecurGraph(l_edge->target_item_id(), p_already_expanded,
                           // the padding will be "|  " unless it is the last recusive call for this item ie if the current edge is the last
                           // special one and there are no ordinary edges (rare case but you never know)
                              p_left + (((l_next_edge->itemID() == l_stop_id) && (l_current->firstEdge_item_id() == G_VOID_ITEM_ID)) ? "   " : "|  "));
                l_edge = l_next_edge;
            }while(l_edge->itemID() != l_stop_id);
        }
        // ... and edges (if any)
        if(Item_lv2* l_edge = l_current->getFirstEdgeWrapper()){
            // NB getFirstEdgeSpecial() will return nullptr if there are no edges
            ItemID l_stop_id = l_edge->itemID();  // this id is the first AND last edge bc edges are arranged in a doubly linked ring
            // loop through all ordinary edges
            do{
                // debug one liner of current edge ("O" = "ordinary")
                qDebug(g_cat_silence) << p_left + "+--O" + l_edge->dbgHalf();
                Item_lv2* l_next_edge = l_edge->getNextWrapper();
                // recursive call to recurGraph on the target of the current edge
                dbgRecurGraph(l_edge->target_item_id(), p_already_expanded,
                           // the padding will be "|  " unless it is the last recusive call for this item ie if
                           // the current edge is the last ordinary one
                           p_left + ((l_next_edge->itemID() == l_stop_id) ? QString("   ") : QString("|  ")));

                // release the current edge ItemWrapper and replace it with the ne current edge obtained previously (l_next_edge)
                l_edge = l_next_edge;
            }while(l_edge->itemID() != l_stop_id);
            // finally, release the last edge ItemWrapper
        }
    }
}

/**
 * @brief base debug dump for the base ItemWrapper class (not supposed to be called directly but from subclasses)
 *
 * This is a multi-line debug representation that will appear at the top of that of all 4 derived classes
 *
 * @return the debug string
 */
QString M1Store::Item_lv2::dbgString(){
    QString l_types(" [");
    for(int i = 0; i < 4; i++)
        if(getType_si_id(i) != G_VOID_SI_ID)
            l_types += Storage::getSpecialSlotPointer(getType_si_id(i))->mnemonic();

    if(l_types.length() == 2) l_types = "";
    else l_types += "]";

    return QString("\n-----------------------------------------------------------------------------------------\n%1\n%2\n%3\n")
        .arg(QString("m_id                 : 0x%1").arg(item_id(), 16, 16, QLatin1Char('0')))
        .arg(QString("m_flags              : 0b%1").arg(flags(), 64, 2, QLatin1Char('0')))
        .arg(QString("m_type               : %1").arg(getTypeDbg_string() + l_types));

    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:{
            // string to contain the representation of edges coming off of this item
            QString l_edges;
            // list of edges construction : same logic as recurGraph() above, minus the recursiveness
            // special edges
            if(Item_lv2* l_current_edge = getExisting(firstEdgeSpecial_item_id())){
                l_edges += "\nSpecial edges:";
                ItemID l_stop_id = l_current_edge->itemID();
                do {
                    l_edges += "\n" + l_current_edge->dbgShort();
                Item_lv2* l_next_edge = l_current_edge->getNextWrapper();
                    l_current_edge = l_next_edge;
                } while (l_current_edge->itemID() != l_stop_id);
            }
            // ordinary edges
            if(Item_lv2* l_current_edge = getExisting(firstEdge_item_id())){
                l_edges += "\nOrdinary edges:";
                ItemID l_stop_id = l_current_edge->itemID();
                do {
                    l_edges += "\n" + l_current_edge->dbgShort();
                    Item_lv2* l_next_edge = l_current_edge->getNextWrapper();
                    l_current_edge = l_next_edge;
                } while (l_current_edge->itemID() != l_stop_id);
            }

            // add the rest to the returned string
            QString l_text(text());
            return M1Store::Item_lv2::dbgString() +
                   QString("--------------------------- FULL VERTEX -------------------------------------------------\n") +
                   QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8")
                       .arg(QString("m_flags_extra        : 0b%1").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                       .arg(QString("m_creation_date      : %1").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("m_lastmod_date       : %1").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("m_incoming_edges     : %1").arg(incomingEdges()))
                       .arg(QString("m_visible_edges      : %1").arg(visibleEdges()))
                       .arg(QString("m_first_edge         : 0x%1 %2").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                       .arg(QString("m_first_edge_special : 0x%1 %2").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                       .arg(QString("text                 : [%1] %2%3")
                                .arg(l_text.length())
                                .arg(l_text.left(100))
                                .arg(l_text.length() > 100 ? "..." : "")) + l_edges;
        }
        break;
    case SIMPLE_VERTEX:{
            return M1Store::Item_lv2::dbgString() +
                   QString("--------------------------- SIMPLE VERTEX -----------------------------------------------\n") +
                   QString("%1")
                       .arg(QString("text                 : %1").arg(text()));
        }
        break;
    case FULL_EDGE:{
            return M1Store::Item_lv2::dbgString() +
                   QString("--------------------------- FULL EDGE ---------------------------------------------------\n") +
                   QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9\n%10\n%11\n%12")
                       .arg(QString("m_flags_extra        : 0b%1").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                       .arg(QString("m_creation_date      : %1").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("m_lastmod_date       : %1").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("m_incoming_edges     : %1").arg(incomingEdges()))
                       .arg(QString("m_first_edge         : 0x%1 %2").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                       .arg(QString("m_first_edge_special : 0x%1 %2").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                       .arg(QString("m_v_origin           : 0x%1 %2").arg(origin_item_id(), 16, 16, QLatin1Char('0')).arg(origin_item_id()))
                       .arg(QString("m_v_target           : 0x%1 %2").arg(target_item_id(), 16, 16, QLatin1Char('0')).arg(target_item_id()))
                       .arg(QString("m_e_previous         : 0x%1 %2").arg(previous_item_id(), 16, 16, QLatin1Char('0')).arg(previous_item_id()))
                       .arg(QString("m_e_next             : 0x%1 %2").arg(next_item_id(), 16, 16, QLatin1Char('0')).arg(next_item_id()))
                       .arg(QString("m_e_reciprocal       : 0x%1 %2").arg(reciprocal_item_id(), 16, 16, QLatin1Char('0')).arg(reciprocal_item_id()))
                       .arg(QString("text                 : %1").arg(text()));
        }
        break;
    case SIMPLE_EDGE:{
            return M1Store::Item_lv2::dbgString() +
                   QString("--------------------------- SIMPLE EDGE -------------------------------------------------\n") +
                   QString("%1\n%2\n%3\n%4")
                        .arg(QString("m_v_origin           : 0x%1 %2").arg(origin_item_id(), 16, 16, QLatin1Char('0')).arg(origin_item_id()))
                        .arg(QString("m_e_previous         : 0x%1 %2").arg(previous_item_id(), 16, 16, QLatin1Char('0')).arg(previous_item_id()))
                        .arg(QString("m_e_next             : 0x%1 %2").arg(next_item_id(), 16, 16, QLatin1Char('0')).arg(next_item_id()))
                        .arg(QString("text                 : %1").arg(text()));
        }
        break;
    }
        return "";
}

/**
 * @brief mnemonic of first special item type or _____ if none
 * @return the mnemonic
 */
QString M1Store::Item_lv2::dbgTypeShort(){
    if(getType_si_id(0) == G_VOID_SI_ID)
        return "_____";
    else
        return Storage::getSpecialSlotPointer(getType_si_id(0))->mnemonic();
}

/**
 * @brief One liner debug representation - not supposed to be called on base class
 * @param p_depth is there only for ItemWrapperFullEdge::dbgShort(int p_depth)
 * @return the one-liner
 */
QString M1Store::Item_lv2::dbgShort(int p_depth){
    switch(flags() & ITEM_NATURE_MASK){
        case FULL_VERTEX:
        return QString("ITEM [%1] %2%3")
            .arg(dbgTypeShort())
            .arg(text())
            .arg((flags() & IS_SPECIAL) ? QString(" (%1)").arg(M1Store::Storage::getSpecial(item_id())->mnemonic()) : "");
        case SIMPLE_VERTEX:
        return QString("VFLD [%1]").arg(text());
        case FULL_EDGE:{
        Q_ASSERT_X(origin_item_id() != G_VOID_ITEM_ID && target_item_id() != G_VOID_ITEM_ID,
                       "ItemWrapperFullEdge::dbgShort())", "origin or target = G_VOID_ITEM_ID");

            // to break cycles
            if(p_depth > 4) return("...");

            M1Store::Item_lv2* l_origin = Item_lv2::getExisting(origin_item_id());
            M1Store::Item_lv2* l_target = Item_lv2::getExisting(target_item_id());

            return QString("EDGE [%1] {%2}-->{%3}")
                .arg(dbgTypeShort())
                .arg(l_origin->dbgShort(p_depth + 1))
                .arg(l_target->dbgShort(p_depth + 1));
        }
        case SIMPLE_EDGE:
            return QString("EFLD [%1]").arg(text());
        default:
        return "";
    }
}

/// only for full edges
QString M1Store::Item_lv2::dbgHalf(){
    Q_ASSERT_X((flags() & ITEM_NATURE_MASK) == FULL_EDGE && target_item_id() != G_VOID_ITEM_ID,
               "ItemWrapperFullEdge::dbgHalf()", "target = G_VOID_ITEM_ID");

    M1Store::Item_lv2* l_target = Item_lv2::getExisting(target_item_id());

    return QString("-[%1]-->{%2}").arg(dbgTypeShort()).arg(l_target->dbgShort());
}

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
M1Store::Item_lv2* M1Store::Item_lv2::getFirstEdgeWrapper(){
    return getExisting(firstEdge_item_id());
}
/// First Special Edge wrapper or nullptr if none
M1Store::Item_lv2* M1Store::Item_lv2::getFirstEdgeSpecialWrapper(){
    return getExisting(firstEdgeSpecial_item_id());
}

/// Wapper for next edge (only for edges) does NOT ever return a nullptr
M1Store::Item_lv2* M1Store::Item_lv2::getNextWrapper(){
    Q_ASSERT_X(next_item_id() != G_VOID_ITEM_ID,
               "ItemWrapper::getNext()", "next_item_id() == G_VOID_ITEM_ID");

    return getExisting(next_item_id());
}
/** @}*/

// ---------------------------------- other methods (critical ones) -------------------------------------------

/**
 * @brief called after the creation of a new full vertex. Creates mew edges based on category and type (AUTO, ISA, ...)
 */
void M1Store::Item_lv2::defaultConnections(){
    qCDebug(g_cat_lv2_members) << QString("Default connections for full vertex [%1]").arg(item_id());
    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:{
            // creation of the AUTO edge
        M1Store::Item_lv2* l_auto_edge = getNew(FULL_EDGE | IS_AUTO,
                       M1Store::ItemType(M1Store::Storage::getSpecialID("AUTO_"),
                                         G_VOID_SI_ID,
                                         G_VOID_SI_ID,
                                         G_VOID_SI_ID));
            // loops onto this vertex
            l_auto_edge->setTarget(item_id());
            l_auto_edge->setOrigin(item_id());

            // it is the first edge so it links to itself
            l_auto_edge->setPrevious(l_auto_edge->itemID());
            l_auto_edge->setNext(l_auto_edge->itemID());

            // attach this adge as the first ordinary one
            this->setFirstEdge(l_auto_edge->itemID());

            // create type edges if required
            createTypeEdges();
            qCDebug(g_cat_lv2_members) << QString("Default connections for full vertex end: %1").arg(item_id());
        }
        break;
    case FULL_EDGE:
        createTypeEdges();
        break;
    // nothing to do for simple edges/vertices
    }
}

/**
 * @brief creation if ISA edges if required by m_item SpecialItemID types
 */
void M1Store::Item_lv2::createTypeEdges(){
    Q_ASSERT_X((flags() & ITEM_NATURE_MASK) == FULL_EDGE || (flags() & ITEM_NATURE_MASK) == FULL_VERTEX,
               "ItemWrapperFullEdge::createTypeEdges()", "not full edge or full vertex");
    qCDebug(g_cat_lv2_members) << QString("Creating type edges if required for [%1]").arg(item_id());

    // nothing to do if this item has an ItemID type
    if((flags() & TYPE_IS_ITEM_ID) == 0)
        // not an ItemID type therefore 4 special item ids - loop through them all
        for(int l_slot = 0; l_slot < 4; l_slot++){
            // special item id from this slot
            SpecialItemID l_si = getType_si_id(l_slot);
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

/**
 * @brief Type attribution with a special item type
 * @param p_type_si a SpecialItem pointer for the type to be attributed
 * @return true/false depending on success/failure
 */
bool M1Store::Item_lv2::setType(const SpecialItem* p_type_si){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(p_type_si->mnemonic());

    // index of first free slot (or -1 if none)
    int l_free_slot = getType_si_id(0) == G_VOID_SI_ID ? 0 :
                          (getType_si_id(1) == G_VOID_SI_ID ? 1 :
                            (getType_si_id(2) == G_VOID_SI_ID ? 2 : getType_si_id(3) == G_VOID_SI_ID ? 3 : -1));

    // store the special item id in m_type if possible
    if(l_free_slot >= 0)
        setType_si_id(l_free_slot, p_type_si->specialId());

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
bool M1Store::Item_lv2::setType(const SpecialItemID p_type_si_id){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(M1Store::Storage::getSpecialSlotPointer(p_type_si_id)->mnemonic());
    return setType(M1Store::Storage::getSpecialSlotPointer(p_type_si_id));
}
/**
 * @brief Type attribution with a special item type
 * @param p_mnemonic the mnemonic of the type to be attributed
 * @return true/false depending on success/failure
 */
bool M1Store::Item_lv2::setType(const char* p_mnemonic){
    qCDebug(g_cat_lv2_members) << QString("Set type to: %1").arg(p_mnemonic);
    return setType(M1Store::Storage::getSpecial(p_mnemonic));
}

/**
 * @brief link an item to another
 * @param p_target the target (as an ItemWrapper)
 * @param p_type_edge the edge type (as a SpecialItemID)
 */
void M1Store::Item_lv2::linkTo(Item_lv2* p_target, const SpecialItemID p_type_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]")
                    .arg(p_target->dbgShort())
                    .arg(M1Store::Storage::getSpecialSlotPointer(p_type_edge)->mnemonic());
    // cannot link simple items to anything
    Q_ASSERT_X(isFullVertex() || isFullEdge(),
               "ItemWrapper::linkTo()", "cannot link simple items or edges to anything");

    // the new edge from this item to the target
    M1Store::Item_lv2* l_new_edge = getNew(FULL_EDGE, M1Store::ItemType(p_type_edge, G_VOID_SI_ID, G_VOID_SI_ID, G_VOID_SI_ID));
    l_new_edge->setTarget(p_target->itemID());
    l_new_edge->setOrigin(item_id());

    // creation of a reciprocal edge if necessary
    if(M1Store::Storage::getSpecialSlotPointer(p_type_edge)->flags() & SI_HAS_RECIPROCAL){
        // the reciprocal edge
        SpecialItemID l_reciprocal_id = M1Store::Storage::getSpecialSlotPointer(p_type_edge)->reciprocalId();
        M1Store::Item_lv2* l_new_edge_reciprocal = getNew(FULL_EDGE, M1Store::ItemType(l_reciprocal_id, 0, 0, 0));
        l_new_edge_reciprocal->setOrigin(p_target->itemID());
        l_new_edge_reciprocal->setTarget(item_id());

        // mutual hook-up of the 2 edges
        l_new_edge->setReciprocal(l_new_edge_reciprocal->itemID());
        l_new_edge_reciprocal->setReciprocal(l_new_edge->itemID());

        // insert the reciprocal edge into one of the edge rings (special or ordinary) of the target item
        p_target->addFullEdge(l_new_edge_reciprocal, l_reciprocal_id);
    }
    // insert the direct edge into one of the edge rings (special or ordinary) of this item
    this->addFullEdge(l_new_edge, p_type_edge);
}
/**
 * @brief link an item to another
 * @param p_target the target (as an ItemWrapper)
 * @param p_mnemonic_edge the edge type (as a mnemonic)
 */
void M1Store::Item_lv2::linkTo(Item_lv2* p_target, const char* p_mnemonic_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]").arg(p_target->dbgShort()).arg(p_mnemonic_edge);

    linkTo(p_target, Storage::getSpecial(p_mnemonic_edge)->specialId());
}
/**
 * @brief link an item to another
 * @param p_target_id the target (as an ItemID)
 * @param p_type the edge type (as a SpecialItemID)
 */
void M1Store::Item_lv2::linkTo(ItemID p_target_id, const SpecialItemID p_type){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]").arg(p_target_id).arg(p_type);
    Q_ASSERT_X(p_target_id != G_VOID_ITEM_ID,
               "ItemWrapper::linkTo())", "p_target_id = G_VOID_ITEM_ID");

    M1Store::Item_lv2* l_target = getExisting(p_target_id);
    linkTo(l_target, p_type);
}
/**
 * @brief link an item to another
 * @param p_target_id the target (as an ItemID)
 * @param p_mnemonic_edge the edge type (as a mnemonic)
 */
void M1Store::Item_lv2::linkTo(ItemID p_target_id, const char* p_mnemonic_edge){
    qCDebug(g_cat_lv2_members) << QString("link to: %1 [%2]").arg(p_target_id).arg(p_mnemonic_edge);
    Q_ASSERT_X(p_target_id != G_VOID_ITEM_ID,
               "ItemWrapper::linkTo())", "p_target_id = G_VOID_ITEM_ID");

    M1Store::Item_lv2* l_target = getExisting(p_target_id);
    linkTo(l_target, p_mnemonic_edge);
}

/**
 * @brief insert a newly created edge into one of the 2 edge rings (special or ordinary) of this item
 * @param p_new_edge the newly created edge (as a ItemWrapperFullEdge)
 * @param p_edge_type the edge type, as SpecialItemID
 */
void M1Store::Item_lv2::addFullEdge(Item_lv2* p_new_edge, const SpecialItemID p_edge_type){
    // cannot link simple items to anything
    Q_ASSERT_X(isFullVertex() || isFullEdge(),
               "ItemWrapper::addFullEdge()", "cannot cannot add an edge to a simple edge or vertex");

    ItemID new_edge_id = p_new_edge->itemID();
    // determine in which ring the edge is to be inserted
    bool edge_is_special = M1Store::Storage::getSpecialSlotPointer(p_edge_type)->flags() & SI_IS_SPECIAL_EDGE;

    ItemID first_edge_id = edge_is_special ? firstEdgeSpecial_item_id() : firstEdge_item_id();

    if(first_edge_id == G_VOID_ITEM_ID){ // case in which this item does not have any edge yet in the appropriate ring
        // link the new edge to itself
        p_new_edge->setPrevious(new_edge_id);
        p_new_edge->setNext(new_edge_id);

        // th edge becomes the new first edge of the appropriate ring
        if(edge_is_special)
            // add to the special edge ring
            setFirstEdgeSpecial(p_new_edge->itemID());
        else
            // add to the visible edge ring
            setFirstEdge(p_new_edge->itemID());
    }
    else{
        // insert the edge BELOW the current first edge
        M1Store::Item_lv2* l_previous_edge = getExisting(first_edge_id);
        M1Store::Item_lv2* l_next_edge = l_previous_edge->getNextWrapper();

        // appropriate next/previous hookups
        p_new_edge->setPrevious(first_edge_id);
        p_new_edge->setNext(l_next_edge->itemID());

        l_previous_edge->setNext(new_edge_id);
        l_next_edge->setPrevious(new_edge_id);
    }
}
