#include "m1A_env.h"
#include "m1B_lv2_item.h"
#include "m1B_store.h"

// ---------------------------------- Debug data -------------------------------------------
/** \defgroup DebugLv2 Level 2 Debug messages
 *  \ingroup DebugLog
 * @{
 */

/**
 * @brief Launching the graph recursive dump
 * @param p_item_id starting point (generally ROOT)
 */
void M1Store::Item_lv2::dbgRecurGraphStart(const ItemID p_item_id){
    qCDebug(g_cat_silence) << QString("========= Tree Dump =================");

    QFile l_file("tree_dump.txt");
    if (!l_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream l_out(&l_file);

    std::set<ItemID> l_already_expanded;
    dbgRecurGraph(p_item_id, l_already_expanded, "", l_out);
}

/**
 * @brief Graph recursive dump
 * @param p_item_id starting point (and starting points of subtrees)
 * @param p_already_expanded set of already visited items (not to be done again)
 * @param p_left padding string to print to the left with presence and absence of | where appropriate
 */
void M1Store::Item_lv2::dbgRecurGraph(const ItemID p_item_id, std::set<ItemID>& p_already_expanded, const QString& p_left, QTextStream& p_out){
    // if p_item_id is G_VOID_ITEM_ID (you never know) or p_item_id has already been visited --> do nothing and return
    if(p_already_expanded.find(p_item_id) == p_already_expanded.end() and p_item_id != G_VOID_ITEM_ID){
        // add current item to the already visited set
        p_already_expanded.insert(p_item_id);

        Item_lv2* l_current = getExisting(p_item_id);
        // print an item debug one-liner
        p_out << p_left + l_current->dbgShort() + "\n";

        if(l_current->isFullEdge() || l_current->isFullVertex()){
            // go through special edges of current item (if any) ...
            if(Item_lv2* l_edge = l_current->getFirstEdgeSpecial_lv2()){
                // NB getFirstEdgeSpecial() will return nullptr if there are no special edges
                ItemID l_stop_id = l_edge->item_id(); // this id is the first AND last edge bc edges are arranged in a doubly linked ring
                // loop through all special edges
                do{
                    Item_lv2* l_next_edge = l_edge->get_next_lv2();
                    // debug one liner of current edge ("S" = "special")
                    p_out << p_left + "+--S" + (l_edge->isFullEdge() ? l_edge->dbgHalf() : l_edge->dbgShort(1)) + "\n";
                    // recursive call to recurGraph on the target of the current edge  (may not be a full edge bc of fields)
                    if(l_edge->isFullEdge())
                        dbgRecurGraph(l_edge->target_item_id(), p_already_expanded,
                                      // the padding will be "|  " unless it is the last recusive call for this item ie if the current edge is the last
                                      // special one and there are no ordinary edges (rare case but you never know)
                                      p_left + (((l_next_edge->item_id() == l_stop_id) && (l_current->firstEdge_item_id() == G_VOID_ITEM_ID)) ? "   " : "|  "), p_out);
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
                    p_out << p_left + "+--O" + (l_edge->isFullEdge() ? l_edge->dbgHalf() : l_edge->dbgShort(1)) + "\n";
                    Item_lv2* l_next_edge = l_edge->get_next_lv2();
                    // recursive call to recurGraph on the target of the current edge (may not be a full edge bc of fields)
                    if(l_edge->isFullEdge())
                        dbgRecurGraph(l_edge->target_item_id(), p_already_expanded,
                                      // the padding will be "|  " unless it is the last recusive call for this item ie if
                                      // the current edge is the last ordinary one
                                      p_left + ((l_next_edge->item_id() == l_stop_id) ? QString("   ") : QString("|  ")), p_out);

                    // release the current edge ItemWrapper and replace it with the ne current edge obtained previously (l_next_edge)
                    l_edge = l_next_edge;
                }while(l_edge->item_id() != l_stop_id);
                // finally, release the last edge ItemWrapper
            }
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

    // string to contain the representation of edges coming off of this item
    QString l_edges;
    if((flags() & ITEM_NATURE_MASK) == M1Env::FULL_VERTEX || (flags() & ITEM_NATURE_MASK) == M1Env::FULL_EDGE){
        // list of edges construction : same logic as recurGraph() above, minus the recursiveness
        // special edges
        if(Item_lv2* l_current_edge = getExisting(firstEdgeSpecial_item_id())){
            l_edges += "\n===== Special edges =====";
            ItemID l_stop_id = l_current_edge->item_id();
            do {
                l_edges += "\n" + l_current_edge->dbgShort();
                Item_lv2* l_next_edge = l_current_edge->get_next_lv2();
                l_current_edge = l_next_edge;
            } while (l_current_edge->item_id() != l_stop_id);
        }
        // ordinary edges
        if(Item_lv2* l_current_edge = getExisting(firstEdge_item_id())){
            l_edges += "\n===== Ordinary edges =====";
            ItemID l_stop_id = l_current_edge->item_id();
            do {
                l_edges += "\n" + l_current_edge->dbgShort();
                Item_lv2* l_next_edge = l_current_edge->get_next_lv2();
                l_current_edge = l_next_edge;
            } while (l_current_edge->item_id() != l_stop_id);
        }
    }

    QString l_text = this->text();
    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:
        // add the rest to the returned string
        l_ret +=
               QString("--------------------------- FULL VERTEX -------------------------------------------------\n") +
               QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9")
                   .arg(QString("m_flags_extra        : 0b%1").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                   .arg(QString("m_creation_date      : %1").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                   .arg(QString("m_lastmod_date       : %1").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                   .arg(QString("m_incoming_edges     : %1").arg(incomingEdges()))
                   .arg(QString("m_first_edge         : 0x%1 / %2").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                   .arg(QString("m_auto_edge          : 0x%1 / %2").arg(autoEdge_item_id(), 16, 16, QLatin1Char('0')).arg(autoEdge_item_id()))
                   .arg(QString("m_first_edge_special : 0x%1 / %2").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                   .arg(QString("m_string_id          : 0x%1 / %2").arg(string_id(), 16, 16, QLatin1Char('0')).arg(string_id()))
                   .arg(QString("text                 : [%1] %2")
                            .arg(l_text.length()).arg(M1Store::Storage::maxLengthChop(l_text, 100))) + l_edges;
        break;
    case SIMPLE_VERTEX:
        l_ret +=
               QString("------------------------- SIMPLE VERTEX -------------------------------------------------\n") +
               QString("%1")
                    .arg(QString("text                 : [%1] %2")
                        .arg(l_text.length()).arg(M1Store::Storage::maxLengthChop(l_text, 100)));
        break;
    case FULL_EDGE:
        l_ret +=
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
                   .arg(QString("text                 : %1").arg(text())) + l_edges;
        break;
    case SIMPLE_EDGE:
        l_ret +=
               QString("-------------------------- SIMPLE EDGE -------------------------------------------------\n") +
               QString("%1\n%2\n%3\n%4")
                   .arg(QString("m_v_origin           : 0x%1 %2").arg(origin_item_id(), 16, 16, QLatin1Char('0')).arg(origin_item_id()))
                   .arg(QString("m_e_previous         : 0x%1 %2").arg(previous_item_id(), 16, 16, QLatin1Char('0')).arg(previous_item_id()))
                   .arg(QString("m_e_next             : 0x%1 %2").arg(next_item_id(), 16, 16, QLatin1Char('0')).arg(next_item_id()))
                   .arg(QString("text                 : [%1] 2").arg(l_text.length()).arg(M1Store::Storage::maxLengthChop(l_text, 100)));
        break;
    }

    return l_ret + "\n===== END =====";
}

QString M1Store::Item_lv2::dbgStringHtml(){
    QString l_ret = QString("<p style=\"margin: 0;font-weight: bold;\">------------------------ LV2 ------------------------</p>\n") +
                    QString("<table>\n%1%2%3</table>\n")
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
                Item_lv2* l_next_edge = l_current_edge->get_next_lv2();
                l_current_edge = l_next_edge;
            } while (l_current_edge->item_id() != l_stop_id);
        }
        if(!l_edge_list.empty())
            l_edges = "<p style=\"margin: 0;font-weight: bold;\">Special edges:</p>\n" + l_edge_list.join("");
        l_edge_list.clear();
        // ordinary edges
        if(Item_lv2* l_current_edge = getExisting(firstEdge_item_id())){
            //l_edges += "\nOrdinary edges:";
            ItemID l_stop_id = l_current_edge->item_id();
            do {
                l_edge_list.append(QString("<p style=\"margin: 0;\">%1</p>\n").arg(l_current_edge->dbgShort()));
                // l_edges += "\n" + l_current_edge->dbgShort();
                Item_lv2* l_next_edge = l_current_edge->get_next_lv2();
                l_current_edge = l_next_edge;
            } while (l_current_edge->item_id() != l_stop_id);
        }
        if(!l_edge_list.empty())
            l_edges += "<p style=\"margin: 0;font-weight: bold;\">Ordinary edges:</p>\n" + l_edge_list.join("");
    }

    QString l_text(text());
    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:
        // add the rest to the returned string
        l_ret +=
               QString("<p style=\"margin: 0;font-weight: bold;\">-------------------- FULL VERTEX --------------------</p>\n") +
               QString("<table>\n%1%2%3%4%5%6%7%8<table>\n%9")
                   .arg(QString("<tr><td>m_flags_extra</td><td>:</td><td>0b%1</td></tr>\n").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                   .arg(QString("<tr><td>m_creation_date</td><td>:</td><td>%1</td></tr>\n").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                   .arg(QString("<tr><td>m_lastmod_date</td><td>:</td><td>%1</td></tr>\n").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                   .arg(QString("<tr><td>m_incoming_edges</td><td>:</td><td>%1</td></tr>\n").arg(incomingEdges()))
                   .arg(QString("<tr><td>m_first_edge</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                   .arg(QString("<tr><td>m_auto_edge</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(autoEdge_item_id(), 16, 16, QLatin1Char('0')).arg(autoEdge_item_id()))
                   .arg(QString("<tr><td>m_first_edge_special</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                   .arg(QString("<tr><td>m_string_id</td><td>:</td><td>0x%1 / %2</td></tr>\n").arg(string_id(), 16, 16, QLatin1Char('0')).arg(string_id()))
                   .arg(QString("<p style=\"margin: 0;\">Text : [%1] %2</p>\n")
                            .arg(l_text.length())
                            .arg(M1Store::Storage::maxLengthChop(l_text, 100))) + l_edges;
        break;
    case SIMPLE_VERTEX:
        l_ret +=
               QString("<p style=\"margin: 0;font-weight: bold;\">------------------- SIMPLE VERTEX -------------------</p>\n") +
               QString("<p style=\"margin: 0;\">Text : [%1] %2</p>")
                          .arg(l_text.length())
                          .arg(M1Store::Storage::maxLengthChop(l_text, 100));
        break;
    case FULL_EDGE:
        l_ret +=
               QString("<p style=\"margin: 0;font-weight: bold;\">--------------------- FULL EDGE ---------------------</p>\n") +
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
        break;
    case SIMPLE_EDGE:
        l_ret +=
               QString("<p style=\"margin: 0;font-weight: bold;\">-------------------- SIMPLE EDGE --------------------</p>\n") +
               QString("<table>%1\n%2\n%3\n</table>%4")
                   .arg(QString("<tr><td>m_v_origin</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(origin_item_id(), 16, 16, QLatin1Char('0')).arg(origin_item_id()))
                   .arg(QString("<tr><td>m_e_previous</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(previous_item_id(), 16, 16, QLatin1Char('0')).arg(previous_item_id()))
                   .arg(QString("<tr><td>m_e_next</td><td>:</td><td>0x%1 %2</td></tr>\n").arg(next_item_id(), 16, 16, QLatin1Char('0')).arg(next_item_id()))
                   .arg(QString("<p style=\"margin: 0;\">Text: [%1] %2</p>").arg(l_text.length()).arg(M1Store::Storage::maxLengthChop(l_text, 100)));
        break;
    }

    return l_ret;
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
/** @}*/ // end \defgroup DebugLv2
