#include <QtGlobal>
#include <QDebug>

#include <string.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>

#include "m1A_env.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"

Q_LOGGING_CATEGORY(g_cat_lv0_item_type, "lv0.item_type")
Q_LOGGING_CATEGORY(g_cat_lv0_special_item, "lv0.special_item")

// --------------------------------------------------------------------------------------------------------
// ----------------------------- M1Store::ItemType --------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

/**
 * @brief Constructor 3 - set all 4 short types at once
 * @param p_0 4 SpecialItemID to be assigned to thes slot
 * @param p_1 4 SpecialItemID to be assigned to thes slot
 * @param p_2 4 SpecialItemID to be assigned to thes slot
 * @param p_3 4 SpecialItemID to be assigned to thes slot
 */
M1Store::ItemType::ItemType(const SpecialItemID p_0, const SpecialItemID p_1, const SpecialItemID p_2, const SpecialItemID p_3) {
    t.m_type_short[0] = p_0;
    t.m_type_short[1] = p_1;
    t.m_type_short[2] = p_2;
    t.m_type_short[3] = p_3;

    qCDebug(g_cat_lv0_item_type) << QString("Constructed: %1").arg(dbgString());
}

/**
 * @brief Setter 2 - one of the 4 short type
 * @param p_index the number of the type to set (0 to 3)
 * @param p_id the value
 */
void M1Store::ItemType::setSpecialType(const unsigned int p_index, const SpecialItemID p_si_id) {
    M1_FUNC_ENTRY(g_cat_lv0_item_type, QString("setting special type %1 to %2").arg(p_index).arg(p_si_id))
    Q_ASSERT(p_index < 4);

    t.m_type_short[p_index] = p_si_id;
    M1_FUNC_EXIT
}

/**
 * @brief getter 2 : One of the 4 short types
 * @param p_index the number of the type to get (0 to 3)
 * @return
 */
M1Store::SpecialItemID M1Store::ItemType::getSpecialType(const unsigned int p_index) const {
    M1_FUNC_ENTRY(g_cat_lv0_item_type, QString("getting special type %1 ...").arg(p_index))
    Q_ASSERT(p_index < 4);
    qCDebug(g_cat_lv0_members) << QString("--> %1").arg(t.m_type_short[p_index]);

    M1_FUNC_EXIT
    return t.m_type_short[p_index];
}

// ---------------------------------------------------------------------------------------------------------
// ----------------------------- M1Store::SpecialItem ------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

/**
 * @brief Used only for debug purposes
 * @param p_mnemo the menmonic
 */
M1Store::SpecialItem::SpecialItem(const char* p_mnemo){
    // make sure the memonic member is initialized to 0 (normally not necessary
    memset(m_mnemonic, 0, 5);
    strncpy(m_mnemonic, p_mnemo, 5);
}
M1Store::SpecialItem* M1Store::SpecialItem::cm_dummy = new M1Store::SpecialItem("dummy");

/**
 * @brief mnemonic getter (as a QString)
 * @return the QString containing the mnemonic value
 */
QString M1Store::SpecialItem::mnemonic() const {
    // char buffer initialized to 0 to turn the 5 char mnemonic into a \0 terminated C string
    char l_buf[6] = {0};
    strncpy(l_buf, m_mnemonic, 5);
    qCDebug(g_cat_lv0_special_item) << QString("getting mnemonic: %1").arg(l_buf);

    return QString(l_buf);
}

/**
 * @brief initial setting of attributes - called immediately after allocation within mmap() area
 * @param p_item_id the corresponding ItemID (or G_VOID_ITEM_ID if item-less)
 * @param p_si_id special item ID
 * @param p_flags flags
 * @param p_mnemonic menmonic (the parameter is a \0 terminated C string but not the corresponding member)
 */
void M1Store::SpecialItem::setAttr(const ItemID p_item_id, const SpecialItemID p_si_id,
                                   const FlagField p_flags, const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_lv0_special_item, QString("Setting attributes: p_item_id 0x%1 p_si_id 0x%2 p_flags 0b%3 p_mnemonic %4")
                                      .arg(p_item_id, 16, 16, QLatin1Char('0'))
                                      .arg(p_si_id, 4, 16, QLatin1Char('0'))
                                      .arg(p_flags, 64, 2, QLatin1Char('0'))
                                      .arg(p_mnemonic))
    Q_ASSERT_X(strlen(p_mnemonic) == 5,
               "M1Store::SpecialItem::setAttr(ItemID, SpecialItemID, FlagField, char*)",
               "p_mnemonic must be exactly 5 char long");

    m_flags = p_flags;
    m_si_id = p_si_id;
    m_si_id_reciprocal = G_VOID_SI_ID;
    m_item_id = p_item_id;

    // make sure the memonic member is initialized to 0 (normally not necessary
    memset(m_mnemonic, 0, 5);
    strncpy(m_mnemonic, p_mnemonic, 5);

    M1_FUNC_EXIT
}

// ---------------------------------------------------------------------------------------------------------
// ----------------------------- Debug ---------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

/** \defgroup DebugLv0 Level 0 Debug messages
 *  @ingroup DebugLog
 * @{
 */

/**
 * @brief low-level debug representation
 * @return the debug string
 */
QString M1Store::ItemType::dbgString() const{
    return QString("4s[0x%1 0x%2 0x%3 0x%4]-id[0x%5 %6]")
        .arg(t.m_type_short[0], 4, 16, QLatin1Char('0'))
        .arg(t.m_type_short[1], 4, 16, QLatin1Char('0'))
        .arg(t.m_type_short[2], 4, 16, QLatin1Char('0'))
        .arg(t.m_type_short[3], 4, 16, QLatin1Char('0'))
        .arg(t.m_type_item, 16, 16, QLatin1Char('0'))
        .arg(t.m_type_item);
}

/**
 * @brief Human readable debug string
 * @return
 */
QString M1Store::ItemType::dbgStringHr(bool p_is_item_id) const{
    if(p_is_item_id)
        return QString("<II %1>").arg(Item_lv2::getExisting(t.m_type_item)->text());
    else
        return QString("<4SI %1/%2/%3/%4>")
            .arg(t.m_type_short[0] == M1Env::G_VOID_SI_ID ? "" : StorageStatic::getSpecialItemPointer(t.m_type_short[0])->mnemonic())
            .arg(t.m_type_short[1] == M1Env::G_VOID_SI_ID ? "" : StorageStatic::getSpecialItemPointer(t.m_type_short[1])->mnemonic())
            .arg(t.m_type_short[2] == M1Env::G_VOID_SI_ID ? "" : StorageStatic::getSpecialItemPointer(t.m_type_short[2])->mnemonic())
            .arg(t.m_type_short[3] == M1Env::G_VOID_SI_ID ? "" : StorageStatic::getSpecialItemPointer(t.m_type_short[3])->mnemonic())
            .replace("<4SI >", "<4SI None>");
}

/**
 * @brief Debug string of the form <si id> <mnemonic> <item id> <flags (in binary)> (+ the mnemonic of the reciprocal, if any)
 * @return the debug string
 */
QString M1Store::SpecialItem::dbgString() const {
    QString l_item_dbg = QString(" %1").arg(M1Store::StorageStatic::cm_icon_path[m_si_id], 25);
    if(M1Store::Item_lv2* l_item = M1Store::Item_lv2::getExisting(m_item_id)){
        l_item_dbg += " --ITEM--> " + l_item->dbgShort();
    }

    return QString("0x%1 %2 0x%3 0b%4%5%6")
        .arg(m_si_id, 4, 16, QLatin1Char('0'))
        .arg(mnemonic())
        .arg(m_item_id, 16, 16, QLatin1Char('0'))
        .arg(m_flags, 64, 2, QLatin1Char('0'))
        .arg(l_item_dbg)
        .arg(m_flags & SI_HAS_RECIPROCAL ?
                 QString(" --RECIPROCAL--> ") + M1Store::StorageStatic::getSpecialItemPointer(m_si_id_reciprocal)->mnemonic():
                 "");
}

/**
 * @brief debug dump
 *
 * This is a multi-line debug representation
 *
 * @return the debug string
 */
QString M1Store::Item_lv0::dbgString() const{
    QStringList l_list;
    for(int i = 0; i < 4; i++)
        if(getType_si_id(i) != G_VOID_SI_ID)
            l_list.append(StorageStatic::getSpecialItemPointer(getType_si_id(i))->mnemonic());

    QString l_types = QString("[") + l_list.join(", ") + "]";

    QString l_ret = QString("\n---------------------------------- LV0 -------------------------------------------------------\n%1\n%2\n%3\n")
                        .arg(QString("m_id                 : 0x%1").arg(item_id(), 16, 16, QLatin1Char('0')))
                        .arg(QString("m_flags              : 0b%1").arg(flags(), 64, 2, QLatin1Char('0')))
                        .arg(QString("m_type               : %1").arg(l_types));

    // add the rest to the returned string
    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:{
        QString l_text(text());
        return l_ret +
               QString("--------------------------- FULL VERTEX -------------------------------------------------\n") +
               QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8")
                   .arg(QString("m_flags_extra        : 0b%1").arg(flagsExtra(), 64, 2, QLatin1Char('0')))
                   .arg(QString("m_creation_date      : %1").arg(creationDate().toString("dd/MM/yyyy hh:mm:ss")))
                   .arg(QString("m_lastmod_date       : %1").arg(lastmodDate().toString("dd/MM/yyyy hh:mm:ss")))
                   .arg(QString("m_incoming_edges     : %1").arg(incomingEdges()))
                   .arg(QString("m_first_edge         : 0x%1 %2").arg(firstEdge_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdge_item_id()))
                   .arg(QString("m_first_edge_special : 0x%1 %2").arg(firstEdgeSpecial_item_id(), 16, 16, QLatin1Char('0')).arg(firstEdgeSpecial_item_id()))
                   .arg(QString("text                 : [%1] %2%3")
                            .arg(l_text.length())
                            .arg(l_text.left(100))
                            .arg(l_text.length() > 100 ? "..." : ""));
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

    // to prevent compiler complaints
    return "";
}

/**
 * @brief mnemonic of first special item type or _____ if none
 * @return the mnemonic
 */
QString M1Store::Item_lv0::dbgTypeShort() const{
    if(getType_si_id(0) == G_VOID_SI_ID)
        return "_____";
    else{
        QStringList m_type_list;
        for(int i = 0; i<4; i++)
            if(getType_si_id(i) != G_VOID_SI_ID)
                m_type_list.append(StorageStatic::getSpecialItemPointer(getType_si_id(i))->mnemonic());

        return m_type_list.join("/");
    }
}

/**
 * @brief One-liner debug representation
 * @return short debug string
 */
QString M1Store::Item_lv0::dbgShort() const{
    switch(flags() & ITEM_NATURE_MASK){
    case FULL_VERTEX:
        return QString("VRTX [%1] %2%3")
            .arg(dbgTypeShort())
            .arg(text())
            .arg((flags() & IS_SPECIAL) ? QString(" (%1)").arg(M1Store::StorageStatic::getSpecialItemPointer(item_id())->mnemonic()) : "");
    case SIMPLE_VERTEX:
        return QString("VFLD [%1]").arg(text());
    case FULL_EDGE:{
        return QString("EDGE [%1] {%2}-->{%3}")
        .arg(dbgTypeShort())
            .arg(origin_item_id())
            .arg(target_item_id());
    }
    case SIMPLE_EDGE:
        return QString("EFLD [%1]").arg(text());
    default:
        return "";
    }
}

// QDebug serialization : calls dbgString() for ItemType and SpecialItem, but dbgShort for Item_lv0
QDebug operator<<(QDebug d, M1Store::ItemType p){return d << p.dbgString().toUtf8().constData();}
QDebug operator<<(QDebug d, M1Store::ItemType* p){return d << p->dbgString().toUtf8().constData();}

QDebug operator<<(QDebug d, M1Store::Item_lv0 p){return d << p.dbgShort().toUtf8().constData();}
QDebug operator<<(QDebug d, M1Store::Item_lv0* p){return d << p->dbgShort().toUtf8().constData();}

QDebug operator<<(QDebug d, M1Store::SpecialItem p){return d << p.dbgString().toUtf8().constData();}
QDebug operator<<(QDebug d, M1Store::SpecialItem* p){return d << p->dbgString().toUtf8().constData();}

/**@}*/ //end of DebugLv0
