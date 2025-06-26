#include "m1A_env.h"
#include "m1B_lv2_item.h"
#include "m1B_store.h"

Q_LOGGING_CATEGORY(g_cat_lv2_members, "lv2.members_access")
Q_LOGGING_CATEGORY(g_cat_lv2_constructors, "lv2.constructors")
Q_LOGGING_CATEGORY(g_cat_lv2_iterators, "lv2.iterators")

// ---------------------------------- Constructors and instantiation from mmap() data -------------------------------------------

/**********************************************************/
/** \defgroup I2Inst Item_lv2 Instantiation and destruction
 *  @{
 */

/**
 * @brief [Static] get an Item_lv2* for a new item, with a new ItemID (stored in the mmap() area). Without text parameter.
 *
 * Static Method. Since the memory in already present in the mmap() area, a call to the constructor of the class is unnecessary.
 * Only a static_cast fromn a pointer to that area is performed.
 *
 * @param p_flags the flags of the new item
 * @param p_type (ItemType) the type of the new item
 * @return an Item_lv2 pointer for the newly created item
 */
M1Store::Item_lv2* M1Store::Item_lv2::getNew(const FlagField p_flags, const ItemType& p_type){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("A) new Item p_flags: 0b%1 p_type: %2")
                                              .arg(p_flags, 64, 2, QLatin1Char('0'))               // %1
                                              .arg(p_type.dbgStringHr(p_flags & TYPE_IS_ITEM_ID))) // %2

    M1Store::Item_lv2* l_item_lv2 = static_cast<Item_lv2*>(M1Store::Storage::getNewItemPointer_lv0(p_flags, p_type));
    // build default edges
    l_item_lv2->defaultEdges();

    qCDebug(g_cat_lv2_constructors) << QString("New: %1").arg(l_item_lv2->dbgShort());
    M1_FUNC_EXIT
    return l_item_lv2;
}

/**
 * @brief [Static] Called after the creation of a new item (getNew). In the case of full vertices or edges, creates mew edges based on nature and type (AUTO, ISA, ...)
 *
 * Static Method.
 *
 * Loops next/previous of edges onto itself to guarantee that these values are never G_VOID_ITEM_ID
 */
void M1Store::Item_lv2::defaultEdges(){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("Default connections for full vertex [%1]").arg(item_id()))
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
        // !! no longer needed bc done by default below !!
        // l_auto_edge->setPrevious(l_auto_edge->item_id());
        // l_auto_edge->setNext(l_auto_edge->item_id());

        // attach this edge as the first ordinary one
        this->setFirstEdge(l_auto_edge->item_id());
        this->setAutoEdge(l_auto_edge->item_id());

        qCDebug(g_cat_lv2_constructors) << QString("created edge: %1").arg(l_auto_edge->dbgShort());
        // create type edges if required
        createTypeEdges();
    }
    break;
    case FULL_EDGE:
        // loop edge next/previous onto itself, to guarantee that next and previous are never G_VOID_ITEM_ID
        loopNextPrevious();

        // create type edges if required
        createTypeEdges();
        break;
    case SIMPLE_EDGE:
        // loop edge next/previous onto itself, to guarantee that next and previous are never G_VOID_ITEM_ID
        loopNextPrevious();
        break;

        // nothing to do for simple vertices
    }

    M1_FUNC_EXIT
}

void M1Store::Item_lv2::loopNextPrevious(){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("Loop back previous/next onto itself ID=[%1]").arg(item_id()))
    Q_ASSERT_X((flags() & ITEM_NATURE_MASK) == FULL_EDGE || (flags() & ITEM_NATURE_MASK) == SIMPLE_EDGE,
               "Item_lv2::loopNextPrevious()", "not edge (full or simple)");
    this->setPrevious(this->item_id());
    this->setNext(this->item_id());
    M1_FUNC_EXIT
}
/**
 * @brief [Static] Creation of ISA edges if required by m_type SpecialItemID types
 */
void M1Store::Item_lv2::createTypeEdges(){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("Creating type edges if required for [%1]").arg(item_id()))
    Q_ASSERT_X((flags() & ITEM_NATURE_MASK) == FULL_EDGE || (flags() & ITEM_NATURE_MASK) == FULL_VERTEX,
               "Item_lv2::createTypeEdges()", "not full edge or full vertex");

    // nothing to do if this item has an ItemID type
    if((flags() & TYPE_IS_ITEM_ID) == 0)
        // not an ItemID type therefore 4 special item ids - loop through them all
        for(int l_slot = 0; l_slot < 4; l_slot++){
            // special item id from this slot
            SpecialItemID l_si_id = getType_si_id(l_slot);
            if(l_si_id != G_VOID_SI_ID){ // G_VOID_SI_ID --> nothing to do
                SpecialItem* l_type_si = M1Store::Storage::getSpecialItemPointer(l_si_id);
                qCDebug(g_cat_lv2_constructors) << QString("flags = 0b%2 - Create edge to [%1]? %3 ...")
                                                       .arg(l_type_si->mnemonic())
                                                       .arg(l_type_si->flags(), 64, 2, QChar('0'))
                                                       .arg((l_type_si->flags() & SI_REQUIRES_EDGE) ? "Yes" : "No");
                // create edge if SI_REQUIRES_EDGE flag is set in the special item record
                if(l_type_si->flags() & SI_REQUIRES_EDGE){ // this type must be materialized with an edge (not just a special type id)
                    qCDebug(g_cat_lv2_constructors) << QString("Creating it");
                    linkTo(l_type_si->itemId(), "_ISA_");
                }
            }
        }
    M1_FUNC_EXIT
}

/**
 * @brief [Static] get an Item_lv2* for a new item, with a new ItemID (stored in the mmap() area). With text parameter.
 *
 * Static Method. Since the memory in already present in the mmap() area, a call to the constructor of the class is unnecessary.
 * Only a static_cast fromn a pointer to that area is performed.
 *
 * @param p_flags the flags of the new item
 * @param p_type the type of the new item
 * @param p_label the label (item text) of the new item
 * @return an Item_lv2 pointer for the just created item
 */
M1Store::Item_lv2* M1Store::Item_lv2::getNew(const FlagField p_flags, const char* p_label){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("B) new Item p_flags: 0b%1 p_type: %2 p_label: %3")
                                              .arg(p_flags, 64, 2, QLatin1Char('0'))              // %1
                                              .arg(p_label))                                      // %3
    
    M1Store::Item_lv2* l_item_lv2 = getNew(p_flags, M1Store::ItemType());
    qCDebug(g_cat_lv2_constructors) << QString("Setting text to: %1").arg(p_label);
    l_item_lv2->setText(p_label);

    qCDebug(g_cat_lv2_constructors) << QString("New: %1").arg(l_item_lv2->dbgShort());
    M1_FUNC_EXIT
    return l_item_lv2;
}

/**
 * @brief [Static] get an Item_lv2* for a new item, with a new ItemID (stored in the mmap() area) + associated special item. With text parameter.
 *
 * Static Method. Since the memory in already present in the mmap() area, a call to the constructor of the class is unnecessary.
 * Only a static_cast fromn a pointer to that area is performed.
 *
 * @param p_flags the flags of the new item
 * @param p_label the label (item text) of the new item
 * @param p_flags_special the flags of the newly created special item
 * @param p_mnemonic the mnemonic of the newly created special item
 * @param p_icon_path the path to the icon associated with the vertex
 * @return an Item_lv2 pointer for the newly created item
 */
M1Store::Item_lv2* M1Store::Item_lv2::getNew(
    const FlagField p_flags,
    const char* p_label,
    const FlagField p_flags_special,
    const char* p_mnemonic,
    const char* p_icon_path){

    M1_FUNC_ENTRY(g_cat_lv2_constructors,
                    QString("C) p_mnemonic %1 new Item p_flags: 0b%2 p_type: %3 p_label: %4 p_flags_special 0b%5 p_icon_path %1")
                      .arg(p_mnemonic)                                        // %1
                      .arg(p_flags, 64, 2, QLatin1Char('0'))                  // %2
                      .arg(p_label)                                           // %4
                      .arg(p_flags_special, 64, 2, QLatin1Char('0'))
                      .arg(p_icon_path)                                       // %6
                  )
    
    M1Store::Item_lv2* l_item_lv2 = getNew(p_flags, p_label);
    qCDebug(g_cat_lv2_constructors) << QString("Creating new special: %1").arg(p_mnemonic);
    M1Store::Storage::getNewSpecialWithItem(l_item_lv2->item_id(), p_flags_special, p_mnemonic, p_icon_path);

    qCDebug(g_cat_lv2_constructors) << QString("New: %1").arg(l_item_lv2->dbgShort());
    M1_FUNC_EXIT
    return l_item_lv2;
}

/**
 * @brief [Static] Get an Item_lv2* for an existing item
 * @param p_item_id the ItemID to build the ItemWrapper for
 * @return the corresponding ItemWrapper* or nullptr if p_item_id is G_VOID_ITEM_ID
 */
M1Store::Item_lv2* M1Store::Item_lv2::getExisting(const ItemID p_item_id){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("A) getExisting from ItemID: %1").arg(p_item_id))
    Item_lv2* l_ret = nullptr;
    // p_item_id can be G_VOID_ITEM_ID
    if(p_item_id != G_VOID_ITEM_ID){
        l_ret = static_cast<Item_lv2*>(M1Store::Storage::getItemPointer_lv0(p_item_id));
        qCDebug(g_cat_lv2_constructors) << QString("Existing: %1").arg(l_ret->dbgShort());
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1Store::Item_lv2* M1Store::Item_lv2::getExisting(const SpecialItemID p_si_id){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("B) getExisting from ItemID: %1").arg(p_si_id));
    Item_lv2* l_ret = getExisting(M1Store::Storage::getSpecialItemPointer(p_si_id)->itemId());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief [Static] Get an Item_lv2* for an existing item
 * @param p_mnemonic the mnemonic to build the ItemWrapper for
 * @return the corresponding ItemWrapper*
 */
M1Store::Item_lv2* M1Store::Item_lv2::getExisting(const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("C) getExisting from mnemonic: %1").arg(p_mnemonic))

    Item_lv2* l_ret = getExisting(M1Store::Storage::getSpecialItemPointer(p_mnemonic)->itemId());
    qCDebug(g_cat_lv2_constructors) << QString("Existing: %1").arg(l_ret->dbgShort());
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief destructor - normally not used as the item segments in the mmap() area do not need to be freed
 */
M1Store::Item_lv2::~Item_lv2(){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("~Item_lv2()"));

    M1_FUNC_EXIT
}
/** @}*/
// end \defgroup I2Inst

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
        if(Item_lv2* l_edge = l_current->getFirstEdgeSpecial_lv2()){
            // NB getFirstEdgeSpecial() will return nullptr if there are no special edges
            ItemID l_stop_id = l_edge->item_id(); // this id is the first AND last edge bc edges are arranged in a doubly linked ring
            // loop through all special edges
            do{
                Item_lv2* l_next_edge = l_edge->getNext_lv2();
                // debug one liner of current edge ("S" = "special")
                qDebug(g_cat_silence) << p_left + "+--S" + (l_edge->isFullEdge() ? l_edge->dbgHalf() : l_edge->dbgShort(1));
                // recursive call to recurGraph on the target of the current edge  (may not be a full edge bc of fields)
                if(l_edge->isFullEdge())
                    dbgRecurGraph(l_edge->target_item_id(), p_already_expanded,
                               // the padding will be "|  " unless it is the last recusive call for this item ie if the current edge is the last
                               // special one and there are no ordinary edges (rare case but you never know)
                                  p_left + (((l_next_edge->item_id() == l_stop_id) && (l_current->firstEdge_item_id() == G_VOID_ITEM_ID)) ? "   " : "|  "));
                l_edge = l_next_edge;
            }while(l_edge->item_id() != l_stop_id);
        }
        // ... and edges (if any)
        if(Item_lv2* l_edge = l_current->getFirstEdge_lv2()){
            // NB getFirstEdgeSpecial() will return nullptr if there are no edges
            ItemID l_stop_id = l_edge->item_id();  // this id is the first AND last edge bc edges are arranged in a doubly linked ring
            // loop through all ordinary edges
            do{
                // debug one liner of current edge ("O" = "ordinary")
                qDebug(g_cat_silence) << p_left + "+--O" + (l_edge->isFullEdge() ? l_edge->dbgHalf() : l_edge->dbgShort(1));
                Item_lv2* l_next_edge = l_edge->getNext_lv2();
                // recursive call to recurGraph on the target of the current edge (may not be a full edge bc of fields)
                if(l_edge->isFullEdge())
                    dbgRecurGraph(l_edge->target_item_id(), p_already_expanded,
                           // the padding will be "|  " unless it is the last recusive call for this item ie if
                           // the current edge is the last ordinary one
                           p_left + ((l_next_edge->item_id() == l_stop_id) ? QString("   ") : QString("|  ")));

                // release the current edge ItemWrapper and replace it with the ne current edge obtained previously (l_next_edge)
                l_edge = l_next_edge;
            }while(l_edge->item_id() != l_stop_id);
            // finally, release the last edge ItemWrapper
        }
    }
}

/**
 * @brief debug dump
 *
 * This is a multi-line debug representation
 *
 * @return the debug string
 */
QString M1Store::Item_lv2::dbgString(){
    QString l_ret = QString("\n---------------------------------- LV2 -------------------------------------------------------\n%1\n%2\n%3\n")
        .arg(QString("m_id                 : 0x%1 %2").arg(item_id(), 16, 16, QLatin1Char('0')).arg(item_id()))
        .arg(QString("m_flags              : 0b%1").arg(flags(), 64, 2, QLatin1Char('0')))
        .arg(QString("m_type               : %1").arg(m_type.dbgStringHr(flags() & TYPE_IS_ITEM_ID)));

    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:{
            // string to contain the representation of edges coming off of this item
            QString l_edges;
            // list of edges construction : same logic as recurGraph() above, minus the recursiveness
            // special edges
            if(Item_lv2* l_current_edge = getExisting(firstEdgeSpecial_item_id())){
                l_edges += "\nSpecial edges:";
                ItemID l_stop_id = l_current_edge->item_id();
                do {
                    l_edges += "\n" + l_current_edge->dbgShort();
                Item_lv2* l_next_edge = l_current_edge->getNext_lv2();
                    l_current_edge = l_next_edge;
                } while (l_current_edge->item_id() != l_stop_id);
            }
            // ordinary edges
            if(Item_lv2* l_current_edge = getExisting(firstEdge_item_id())){
                l_edges += "\nOrdinary edges:";
                ItemID l_stop_id = l_current_edge->item_id();
                do {
                    l_edges += "\n" + l_current_edge->dbgShort();
                    Item_lv2* l_next_edge = l_current_edge->getNext_lv2();
                    l_current_edge = l_next_edge;
                } while (l_current_edge->item_id() != l_stop_id);
            }

            // add the rest to the returned string
            QString l_text(text());

            return l_ret +
                        QString("--------------------------- FULL VERTEX -------------------------------------------------\n") +
                        QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9\n%10")
                           .arg(QString("m_flags_extra        : 0b%1").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                           .arg(QString("m_creation_date      : %1").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                           .arg(QString("m_lastmod_date       : %1").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                           .arg(QString("m_incoming_edges     : %1").arg(incomingEdges()))
                           .arg(QString("m_first_edge         : 0x%1 / %2").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                           .arg(QString("m_auto_edge          : 0x%1 / %2").arg(autoEdge_item_id(), 16, 16, QLatin1Char('0')).arg(autoEdge_item_id()))
                           .arg(QString("m_first_edge_special : 0x%1 / %2").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                           .arg(QString("m_string_id          : 0x%1 / %2").arg(string_id(), 16, 16, QLatin1Char('0')).arg(string_id()))
                           .arg(QString("text                 : [%1] %2%3")
                                    .arg(l_text.length())
                                    .arg(l_text.left(100))
                                    .arg(l_text.length() > 100 ? "..." : "")) + l_edges;
        }
        break;
    case SIMPLE_VERTEX:{
            return l_ret +
                        QString("--------------------------- SIMPLE VERTEX -----------------------------------------------\n") +
                        QString("%1")
                           .arg(QString("text                 : %1").arg(text()));
        }
        break;
    case FULL_EDGE:{
            return l_ret +
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
            return l_ret +
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

QString M1Store::Item_lv2::dbgStringHtml(){
    QString l_ret = QString("<p style=\"margin: 0;\">------------------------ LV2 ------------------------</p>\n") +
                    QString("<table>\n%1\n%2\n%3\n</table>\n")
                        .arg(QString("<tr><td>m_id</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(item_id(), 16, 16, QLatin1Char('0')).arg(item_id()))
                        .arg(QString("<tr><td>m_flags</td><td>:</td><td>0b%1</td></tr>\n").arg(flags(), 64, 2, QLatin1Char('0')))
                        .arg(QString("<tr><td>m_type</td><td>:</td><td>%1</td></tr>\n").arg(m_type.dbgStringHr(flags() & TYPE_IS_ITEM_ID)));

    // string to contain the representation of edges coming off of this item (only for full items)
    QString l_edges;
    if((flags() & ITEM_NATURE_MASK) == FULL_VERTEX || (flags() & ITEM_NATURE_MASK) == FULL_EDGE){
        QStringList l_edge_list;
        // list of edges construction : same logic as recurGraph() above, minus the recursiveness
        // special edges
        if(Item_lv2* l_current_edge = getExisting(firstEdgeSpecial_item_id())){
            // l_edges += "\nSpecial edges:";
            ItemID l_stop_id = l_current_edge->item_id();
            do {
                l_edge_list.append(QString("<p style=\"margin: 0;\">%1</p>\n").arg(l_current_edge->dbgShort()));
                // l_edges += "\n" + l_current_edge->dbgShort();
                Item_lv2* l_next_edge = l_current_edge->getNext_lv2();
                l_current_edge = l_next_edge;
            } while (l_current_edge->item_id() != l_stop_id);
        }
        if(!l_edge_list.empty())
            l_edges = "<p style=\"margin: 0;\">Special edges:</p>\n" + l_edge_list.join("");
        l_edge_list.clear();
        // ordinary edges
        if(Item_lv2* l_current_edge = getExisting(firstEdge_item_id())){
            //l_edges += "\nOrdinary edges:";
            ItemID l_stop_id = l_current_edge->item_id();
            do {
                l_edge_list.append(QString("<p style=\"margin: 0;\">%1</p>\n").arg(l_current_edge->dbgShort()));
                // l_edges += "\n" + l_current_edge->dbgShort();
                Item_lv2* l_next_edge = l_current_edge->getNext_lv2();
                l_current_edge = l_next_edge;
            } while (l_current_edge->item_id() != l_stop_id);
        }
        if(!l_edge_list.empty())
            l_edges += "<p style=\"margin: 0;\">Ordinary edges:</p>\n" + l_edge_list.join("");
    }

    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:{
            // add the rest to the returned string
            QString l_text(text());

            return l_ret +
                   QString("<p style=\"margin: 0;\">----------------------- FULL VERTEX -----------------------</p>\n") +
                   QString("<table>%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9<table>\n")
                       .arg(QString("<tr><td>m_flags_extra</td><td>:</td><td>0b%1</td></tr>\n").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                       .arg(QString("<tr><td>m_creation_date</td><td>:</td><td>%1</td></tr>\n").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("<tr><td>m_lastmod_date</td><td>:</td><td>%1</td></tr>\n").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("<tr><td>m_incoming_edges</td><td>:</td><td>%1</td></tr>\n").arg(incomingEdges()))
                       .arg(QString("<tr><td>m_first_edge</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                       .arg(QString("<tr><td>m_auto_edge</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(autoEdge_item_id(), 16, 16, QLatin1Char('0')).arg(autoEdge_item_id()))
                       .arg(QString("<tr><td>m_first_edge_special</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                       .arg(QString("<tr><td>m_string_id</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(string_id(), 16, 16, QLatin1Char('0')).arg(string_id()))
                       .arg(QString("<p style=\"margin: 0;\">text : [%1] %2%3</p>\n")
                                .arg(l_text.length())
                                .arg(l_text.left(100))
                                .arg(l_text.length() > 100 ? "..." : "")) + l_edges;
    }
    break;
    case SIMPLE_VERTEX:{
            return l_ret +
                   QString("<p style=\"margin: 0;\">----------------------- SIMPLE VERTEX -----------------------</p>\n") +
                   QString("<p style=\"margin: 0;\">%1</p>")
                       .arg(QString("text : %1").arg(text()));
    }
    break;
    case FULL_EDGE:{
            return l_ret +
                   QString("<p style=\"margin: 0;\">----------------------- FULL EDGE -----------------------</p>\n") +
                   QString("<table>%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9\n%10\n%11<table>\n%12\n%13\n")
                       .arg(QString("<tr><td>m_flags_extra</td><td>:</td><td>0b%1</td></tr>\n").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                       .arg(QString("<tr><td>m_creation_date</td><td>:</td><td>%1</td></tr>\n").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("<tr><td>m_lastmod_date</td><td>:</td><td>%1</td></tr>\n").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                       .arg(QString("<tr><td>m_incoming_edges</td><td>:</td><td>%1</td></tr>\n").arg(incomingEdges()))
                       .arg(QString("<tr><td>m_first_edge</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                       .arg(QString("<tr><td>m_first_edge_special</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                       .arg(QString("<tr><td>m_v_origin</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(origin_item_id(), 16, 16, QLatin1Char('0')).arg(origin_item_id()))
                       .arg(QString("<tr><td>m_v_target</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(target_item_id(), 16, 16, QLatin1Char('0')).arg(target_item_id()))
                       .arg(QString("<tr><td>m_e_previous</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(previous_item_id(), 16, 16, QLatin1Char('0')).arg(previous_item_id()))
                       .arg(QString("<tr><td>m_e_next</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(next_item_id(), 16, 16, QLatin1Char('0')).arg(next_item_id()))
                       .arg(QString("<tr><td>m_e_reciprocal</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(reciprocal_item_id(), 16, 16, QLatin1Char('0')).arg(reciprocal_item_id()))
                       .arg(QString("<p style=\"margin: 0;\">Text: %1</p>\n").arg(text()))
                       .arg(QString("<p style=\"margin: 0;\">Dbg : %1</p>\n").arg(this->dbgShort())) + l_edges;
    }
    break;
    case SIMPLE_EDGE:{
            return l_ret +
                   QString("<p style=\"margin: 0;\">----------------------- SIMPLE EDGE -----------------------</p>\n") +
                   QString("<table>%1\n%2\n%3\n</table>%4")
                       .arg(QString("<tr><td>m_v_origin</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(origin_item_id(), 16, 16, QLatin1Char('0')).arg(origin_item_id()))
                       .arg(QString("<tr><td>m_e_previous</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(previous_item_id(), 16, 16, QLatin1Char('0')).arg(previous_item_id()))
                       .arg(QString("<tr><td>m_e_next</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(next_item_id(), 16, 16, QLatin1Char('0')).arg(next_item_id()))
                       .arg(QString("<p style=\"margin: 0;\">%1</p>").arg(text()));
    }
    break;
    }

    return "";
}

QDebug operator<<(QDebug d, M1Store::Item_lv2 p){return d << p.dbgShort().toUtf8().constData();}
QDebug operator<<(QDebug d, M1Store::Item_lv2* p){return d << p->dbgShort().toUtf8().constData();}

/**
 * @brief One liner debug representation
 * @param p_depth is used in the case of full edges which may need recursive descent
 * @return the one-liner
 */
QString M1Store::Item_lv2::dbgShort(int p_depth){
    QString l_text = text();
    l_text = QString("%1%2").arg(l_text.left(100)).arg(l_text.length() > 100 ? "..." : "");

    switch(flags() & ITEM_NATURE_MASK){
        case FULL_VERTEX:
            return QString("VRTX [%1] %2%3")
                .arg(dbgTypeShort())
                .arg(l_text)
                .arg((flags() & IS_SPECIAL) ? QString(" (%1)").arg(M1Store::Storage::getSpecialItemPointer(item_id())->mnemonic()) : "");
        case FULL_EDGE:{
            // must accept to represent incomplete edges
            // Q_ASSERT_X(origin_item_id() != G_VOID_ITEM_ID && target_item_id() != G_VOID_ITEM_ID,
            //           "Item_lv2::dbgShort())", "origin or target = G_VOID_ITEM_ID");

            // to break cycles
            if(p_depth > 4) return("...");

            M1Store::Item_lv2* l_origin = Item_lv2::getExisting(origin_item_id());
            M1Store::Item_lv2* l_target = Item_lv2::getExisting(target_item_id());
            QString l_origin_dbg = l_origin == nullptr ? "<no origin>" : l_origin->dbgShort(p_depth + 1);
            QString l_target_dbg = l_target == nullptr ? "<no target>" : l_target->dbgShort(p_depth + 1);

            return QString("EDGE {%2}--[%1]-->{%3}")
                        .arg(dbgTypeShort())
                        .arg(l_origin_dbg)
                        .arg(l_target_dbg);
        }
        case SIMPLE_VERTEX:
            return QString("-[%1]--> VFLD {%2}").arg(dbgTypeShort()).arg(l_text);
        case SIMPLE_EDGE:
            return QString("-[%1]--> EFLD {%2}").arg(dbgTypeShort()).arg(l_text);
        default:
            return "";
    }
}

// only for full edges
QString M1Store::Item_lv2::dbgHalf(){
    Q_ASSERT_X((flags() & ITEM_NATURE_MASK) == FULL_EDGE && target_item_id() != G_VOID_ITEM_ID,
               "ItemWrapperFullEdge::dbgHalf()", "target = G_VOID_ITEM_ID");

    M1Store::Item_lv2* l_target = Item_lv2::getExisting(target_item_id());

    return QString("-[%1]-->{%2}").arg(dbgTypeShort()).arg(l_target->dbgShort());
}

// ---------------------------------- member access -------------------------------------------

/*******************************************************************************/
/** \defgroup LV2retLV2p Item_lv2 methods returning newly instanciated Item_lv2*
 *
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
M1Store::Item_lv2* M1Store::Item_lv2::getNext_lv2() const{
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
M1Store::Item_lv2* M1Store::Item_lv2::getPrevious_lv2() const{
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

/***********************************************************************************************************/
/** \defgroup IOT2 Tests type values stored in Item_lv0::m_type AND ALSO the types embodied by ISA/ITO edges
 *  @{
 */
/**
 * @brief M1Store::Item_lv2::isOfType
 * @param p_type_item_id
 * @return
 */
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
                 l_current_edge = l_current_edge->getNext_lv2()){

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

M1Env::SpecialItemID M1Store::Item_lv2::getIconTypeMember(){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("get type ID in member for Icon display"))
    short l_ret = -1;
    for(int i=0; i<4; i++){
        SpecialItemID l_si_id = this->getType_si_id(i);
        if(l_si_id != M1Env::G_VOID_SI_ID &&
            (l_ret == -1 || (M1Store::Storage::getSpecialItemPointer(l_si_id)->flags() & M1Env::SI_IS_ICON_TYPE))) l_ret = l_si_id;
        // qCDebug(g_cat_lv2_members) << QString("l_ret %1 getType_si_id(i) %2").arg(l_ret).arg(this->getType_si_id(i));
    }
    M1_FUNC_EXIT
    return l_ret;
}
/******************************************/
/** \defgroup LITO linking items with edges
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
    l_new_edge->setTarget(p_target->item_id());
    l_new_edge->setOrigin(this->item_id());

    // creation of a reciprocal edge if necessary
    if(M1Store::Storage::getSpecialItemPointer(p_type_edge)->flags() & SI_HAS_RECIPROCAL){
        // the reciprocal edge
        SpecialItemID l_reciprocal_type_si_id = M1Store::Storage::getSpecialItemPointer(p_type_edge)->reciprocalSpecialId();
        M1Store::Item_lv2* l_new_edge_reciprocal =
            getNew(FULL_EDGE, M1Store::ItemType(l_reciprocal_type_si_id));
        l_new_edge_reciprocal->setOrigin(p_target->item_id());
        l_new_edge_reciprocal->setTarget(this->item_id());

        // mutual reciprocal hook-up of the 2 edges
        l_new_edge->setReciprocal(l_new_edge_reciprocal->item_id());
        l_new_edge_reciprocal->setReciprocal(l_new_edge->item_id());

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
 * @brief M1Store::Item_lv2::setField
 * @param p_content
 * @param p_force_new
 * @param p_field_type_si
 * @param p_field_extra_type_si
 * @return
 */
// bool setField(const QString& p_content, const SpecialItem* p_field_type_si, const SpecialItem* p_field_extra_type_si = nullptr);

bool M1Store::Item_lv2::setField(const QString& p_content, const bool p_force_new,
                                 const SpecialItem* p_field_type_si, const SpecialItem* p_field_extra_type_si){
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Setting field of type [%1] to {%2}%3")
                                         .arg(p_field_type_si->mnemonic())
                                         .arg(p_content)
                                         .arg(p_field_extra_type_si == nullptr ? "" : QString(" Extra type: %1").arg(p_field_extra_type_si->mnemonic())))

    M1Store::Item_lv2* l_field_edge = nullptr;
    if(! p_force_new) l_field_edge = this->findFieldEdge(p_field_type_si);

    if(p_force_new || l_field_edge == nullptr){
        l_field_edge = getNew(M1Env::SIMPLE_EDGE, M1Store::ItemType(p_field_type_si->specialId()));
        l_field_edge->setOrigin(this->item_id());
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

/**
 * @brief M1Store::Item_lv2::getField
 * @param p_field_type_si
 * @return
 */
QString M1Store::Item_lv2::getField(const SpecialItem* p_field_type_si, const SpecialItem* p_field_type2_si, const bool p_all) const {
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Getting value of field of type [%1]")
        .arg(M1Store::Storage::getSpecialItemPointer(p_field_type_si->specialId())->mnemonic()))

    QString l_ret;
    for(Item_lv2_iterator it = this->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isOfType(p_field_type_si) &&(p_field_type2_si == nullptr || it.at()->isOfType(p_field_type2_si))){
            if(p_all)
                if(l_ret.length() == 0)
                    l_ret = it.at()->text();
                else
                    l_ret += QString("/%1").arg(it.at()->text());
            else{
                l_ret = it.at()->text();
                break;
            }
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
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding field of type [%1] if any")
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
                                         .arg(p_at_top).arg(this->dbgShort()))

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
                l_edge_above = this->getExisting(l_first_edge_id)->getPrevious_lv2();
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
            this->setFirstEdgeSpecial(l_new_edge_id);
        else
            // add to the ordinary edge ring
            this->setFirstEdge(l_new_edge_id);
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
    M1Store::Item_lv2* l_next_edge = p_edge_above->getNext_lv2();

    // appropriate next/previous hookups
    p_new_edge->setPrevious(p_edge_above->item_id());
    p_new_edge->setNext(l_next_edge->item_id());

    p_edge_above->setNext(p_new_edge->item_id());
    l_next_edge->setPrevious(p_new_edge->item_id());

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

/** @}*/
// end of \defgroup LITO

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
            qCDebug(g_cat_interp_base) << "it.at():\n" << it.at()->dbgShort();
            bool l_success_condition_edge = p_type_edge == M1Env::G_VOID_SI_ID || it.at()->isOfType(p_type_edge);
            bool l_success_condition_target = p_type_target == M1Env::G_VOID_SI_ID || it.at()->getTarget_lv2()->isOfType(p_type_target);
            if(l_success_condition_edge && l_success_condition_target){
                l_ret = it.at();
                break;
            }
        }
    }
    M1_FUNC_EXIT
    return l_ret;
}
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_edge(const M1Env::SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding edge of type [%1]").arg(M1Store::Storage::getSpecialItemPointer(p_type)->mnemonic()))
    M1Store::Item_lv2* l_ret = this->find_edge(p_type, M1Env::G_VOID_SI_ID);
    M1_FUNC_EXIT
    return l_ret;
}
M1Store::Item_lv2* M1Store::Item_lv2::find_edge_target(const M1Env::SpecialItemID p_type) const{
    M1_FUNC_ENTRY(g_cat_lv2_members, QString("Finding edge of any type with target of type [%1]").arg(M1Store::Storage::getSpecialItemPointer(p_type)->mnemonic()))
    M1Store::Item_lv2* l_ret = this->find_edge(M1Env::G_VOID_SI_ID, p_type);
    M1_FUNC_EXIT
    return l_ret;
}

/***************************************/
/**
 * \defgroup ITER Iterators
 *
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
        m_current_edge = m_current_edge->getNext_lv2();
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



