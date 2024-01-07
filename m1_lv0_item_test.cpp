#include <QtGlobal>
#include <QDebug>

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <random>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>

#include "m1_lv0_item.h"

Q_LOGGING_CATEGORY(g_cat_store_test, "Testing member access")

// ----------------------------- M1Store::ItemType -------------------------------------------------------------

TEST(ItemTypeTest, IndexOutOfBounds){
    M1Store::ItemType l_type;

    EXPECT_DEATH(l_type.setSpecialType(4, 47), "");
    EXPECT_DEATH(l_type.setSpecialType(5, 47), "");
    EXPECT_DEATH(l_type.setSpecialType(-1, 47), "");

    EXPECT_DEATH(l_type.getSpecialType(4), "");
    EXPECT_DEATH(l_type.getSpecialType(-1), "");
}

TEST(ItemTypeTest, ValueStore){
    M1Store::ItemType l_type;

    l_type.setSpecialType(0, 0x10ab);
    l_type.setSpecialType(1, 48);
    l_type.setSpecialType(2, 59);
    l_type.setSpecialType(3, 0xf001);
    EXPECT_EQ(l_type.getSpecialType(0), 0x10ab);
    EXPECT_EQ(l_type.getSpecialType(1), 48);
    EXPECT_EQ(l_type.getSpecialType(2), 59);
    EXPECT_EQ(l_type.getSpecialType(3), 0xf001);
}

// ----------------------------- M1Store::Item -------------------------------------------------------------

// triggers asserts that protect against unlawful combinations of item config and member access
TEST(ItemTest, AppropriateConfig){
    qCDebug(g_cat_store_test) << QString("Testing the asserts guaranteeing proper access to members");

    // start as a full edge with 4 short types (all flags = 0)
    M1Store::Item_lv0 l_item(0, 0, M1Store::ItemType());

    qCDebug(g_cat_store_test) << QString("isOfType(SpecialItemID) call / TYPE_IS_ITEM_ID inconsistency");
    EXPECT_DEATH(l_item.isOfType((M1Store::ItemID) 43), "");
    l_item.setFlags(M1Store::TYPE_IS_ITEM_ID);
    EXPECT_DEATH(l_item.isOfType((M1Store::SpecialItemID) 44), "");
    EXPECT_DEATH(l_item.setFlag(0x3, false), "");
    EXPECT_DEATH(l_item.setFlag(0xb0, false), "");
    EXPECT_DEATH(l_item.setFlag(0xf000, false), "");
    EXPECT_DEATH(l_item.unSetFlag(0x3, false), "");
    EXPECT_DEATH(l_item.unSetFlag(0xb0, false), "");
    EXPECT_DEATH(l_item.unSetFlag(0xf000, false), "");
    EXPECT_DEATH(l_item.setFlagExtra(0x3), "");
    EXPECT_DEATH(l_item.setFlagExtra(0xb0), "");
    EXPECT_DEATH(l_item.setFlagExtra(0xf000), "");
    EXPECT_DEATH(l_item.unSetFlagExtra(0x3), "");
    EXPECT_DEATH(l_item.unSetFlagExtra(0xb0), "");
    EXPECT_DEATH(l_item.unSetFlagExtra(0xf000), "");

    // becomes simple (edge)
    qCDebug(g_cat_store_test) << QString("setFlagsExtra() call / ITEM_IS_SIMPLE inconsistency");
    l_item.setFlags(M1Store::ITEM_IS_SIMPLE);
    EXPECT_DEATH(l_item.setFlagsExtra(3218), "");
    M1Store::FlagField v = 0;
    EXPECT_DEATH( v = l_item.flagsExtra(), "");
    EXPECT_DEATH(l_item.setFlagExtra(0x1), "");
    EXPECT_DEATH(l_item.unSetFlagExtra(0x1), "");

    // becomes full vertex
    l_item.setFlags(M1Store::FULL_VERTEX);
    M1Store::ItemID w = 0;
    qCDebug(g_cat_store_test) << QString("setOrigin/target() call / FULL_VERTEX,SIMPLE_VERTEX inconsistency");
    EXPECT_DEATH(l_item.setOrigin(2315), "");
    EXPECT_DEATH(w = l_item.origin(), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setOrigin(2316), "");
    EXPECT_DEATH(w = l_item.origin(), "");

    l_item.setFlags(M1Store::FULL_VERTEX);
    EXPECT_DEATH(l_item.setTarget(2315), "");
    EXPECT_DEATH(w = l_item.target(), "");
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setTarget(2316), "");
    EXPECT_DEATH(w = l_item.target(), "");
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setTarget(2317), "");
    EXPECT_DEATH(w = l_item.target(), "");

    // becomes full vertex
    qCDebug(g_cat_store_test) << QString("setPrevious() call / vertices inconsistency");
    l_item.setFlags(M1Store::FULL_VERTEX);
    EXPECT_DEATH(l_item.setPrevious(2325), "");
    EXPECT_DEATH(w = l_item.previous(), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setPrevious(2326), "");
    EXPECT_DEATH(w = l_item.previous(), "");

    qCDebug(g_cat_store_test) << QString("setReciprocal() call / anything other than full edge inconsistency");
    // becomes full vertex
    l_item.setFlags(M1Store::FULL_VERTEX);
    EXPECT_DEATH(l_item.setReciprocal(2415), "");
    EXPECT_DEATH(w = l_item.reciprocal(), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setReciprocal(2416), "");
    EXPECT_DEATH(w = l_item.reciprocal(), "");
    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setReciprocal(2417), "");
    EXPECT_DEATH(w = l_item.reciprocal(), "");

    qCDebug(g_cat_store_test) << QString("setFirstEdge() call / simples inconsistency");
    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setFirstEdge(2327), "");
    EXPECT_DEATH(w = l_item.firstEdge(), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setFirstEdge(2328), "");
    EXPECT_DEATH(w = l_item.firstEdge(), "");

    qCDebug(g_cat_store_test) << QString("setFirstEdgeSpecial() call / simples inconsistency");
    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setFirstEdgeSpecial(2329), "");
    EXPECT_DEATH(w = l_item.firstEdgeSpecial(), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setFirstEdgeSpecial(2330), "");
    EXPECT_DEATH(w = l_item.firstEdgeSpecial(), "");

    QDateTime t;
    qCDebug(g_cat_store_test) << QString("setCreationDate()/setLastmodDate() call / simples inconsistency");
    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setCreationDate(QDateTime::currentDateTime()), "");
    EXPECT_DEATH(t = l_item.creationDate(), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setCreationDate(QDateTime::currentDateTime()), "");
    EXPECT_DEATH(t = l_item.creationDate(), "");

    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setLastmodDate(QDateTime::currentDateTime()), "");
    EXPECT_DEATH(t = l_item.lastmodDate(), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setLastmodDate(QDateTime::currentDateTime()), "");
    EXPECT_DEATH(t = l_item.lastmodDate(), "");

    qCDebug(g_cat_store_test) << QString("setIncomingEdges() call / simples inconsistency");
    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setIncomingEdges(45), "");
    EXPECT_DEATH(w = l_item.incomingEdges(), "");
    EXPECT_DEATH(l_item.addIncomingEdges(21), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setIncomingEdges(46), "");
    EXPECT_DEATH(w = l_item.incomingEdges(), "");
    EXPECT_DEATH(l_item.addIncomingEdges(31), "");

    qCDebug(g_cat_store_test) << QString("setVisibleEdges() call / non full vertex inconsistency");
    l_item.setFlags(M1Store::FULL_EDGE);
    EXPECT_DEATH(l_item.setVisibleEdges(55), "");
    EXPECT_DEATH(w = l_item.visibleEdges(), "");
    EXPECT_DEATH(l_item.addVisibleEdges(5), "");
    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_DEATH(l_item.setVisibleEdges(45), "");
    EXPECT_DEATH(w = l_item.visibleEdges(), "");
    EXPECT_DEATH(l_item.addVisibleEdges(8), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_DEATH(l_item.setVisibleEdges(46), "");
    EXPECT_DEATH(w = l_item.visibleEdges(), "");
    EXPECT_DEATH(l_item.addVisibleEdges(9), "");

    qCDebug(g_cat_store_test) << QString("text length violations + default empty string");
    l_item.setFlags(M1Store::FULL_EDGE);
    EXPECT_EQ(QString(l_item.text()), QString(""));
    EXPECT_DEATH(l_item.setText(QString("").fill('x', M1Store::FULL_EDGE_TEXT_LEN)), "");
    // becomes simple edge
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_EQ(QString(l_item.text()), QString(""));
    EXPECT_DEATH(l_item.setText(QString("").fill('x', M1Store::SIMPLE_EDGE_TEXT_LEN)), "");
    // becomes simple vertex
    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_EQ(QString(l_item.text()), QString(""));
    EXPECT_DEATH(l_item.setText(QString("").fill('x', M1Store::SIMPLE_VERTEX_TEXT_LEN)), "");
    // becomes full vertex
    l_item.setFlags(M1Store::FULL_VERTEX);
    EXPECT_EQ(QString(l_item.text()), QString(""));
}
/*
// set/get of members in different Item configs
TEST(ItemTest, ValuesRoundTrip){
    M1Store::Item l_item(0, 0, M1Store::ItemType());

    l_item.setFlags(M1Store::FULL_VERTEX);
    l_item.setFlagsExtra(0xab01);
    EXPECT_EQ(l_item.flagsExtra(), 0xab01);
    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setFlagsExtra(0x90b2);
    EXPECT_EQ(l_item.flagsExtra(), 0x90b2);

    l_item.setOrigin(213);
    EXPECT_EQ(l_item.origin(), 213);
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    l_item.setOrigin(313);
    EXPECT_EQ(l_item.origin(), 313);

    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setTarget(813);
    EXPECT_EQ(l_item.target(), 813);

    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setPrevious(814);
    EXPECT_EQ(l_item.previous(), 814);
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    l_item.setPrevious(816);
    EXPECT_EQ(l_item.previous(), 816);

    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setNext(914);
    EXPECT_EQ(l_item.next(), 914);
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    l_item.setNext(916);
    EXPECT_EQ(l_item.next(), 916);

    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setTarget(823);
    EXPECT_EQ(l_item.target(), 823);

    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setFirstEdge(924);
    EXPECT_EQ(l_item.firstEdge(), 924);
    l_item.setFlags(M1Store::FULL_VERTEX);
    l_item.setFirstEdge(926);
    EXPECT_EQ(l_item.firstEdge(), 926);

    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setFirstEdgeSpecial(934);
    EXPECT_EQ(l_item.firstEdgeSpecial(), 934);
    l_item.setFlags(M1Store::FULL_VERTEX);
    l_item.setFirstEdgeSpecial(926);
    EXPECT_EQ(l_item.firstEdgeSpecial(), 926);

    l_item.setFlags(M1Store::FULL_EDGE);
    QDateTime d = QDateTime::currentDateTime();
    l_item.setCreationDate(d);
    // a round trip between QDateTime and ms since epoch can result in a difference of 1 ms
    EXPECT_NEAR(l_item.creationDate().currentMSecsSinceEpoch(), d.currentMSecsSinceEpoch(), 1);
    d = d.addSecs(10);
    l_item.setFlags(M1Store::FULL_VERTEX);
    l_item.setCreationDate(d);
    // a round trip between QDateTime and ms since epoch can result in a difference of 1 ms
    EXPECT_NEAR(l_item.creationDate().currentMSecsSinceEpoch(), d.currentMSecsSinceEpoch(), 1);

    l_item.setFlags(M1Store::FULL_EDGE);
    d = d.addSecs(12);
    l_item.setLastmodDate(d);
    // a round trip between QDateTime and ms since epoch can result in a difference of 1 ms
    EXPECT_NEAR(l_item.lastmodDate().currentMSecsSinceEpoch(), d.currentMSecsSinceEpoch(), 1);
    d = d.addSecs(15);
    l_item.setFlags(M1Store::FULL_VERTEX);
    l_item.setLastmodDate(d);
    // a round trip between QDateTime and ms since epoch can result in a difference of 1 ms
    EXPECT_NEAR(l_item.lastmodDate().currentMSecsSinceEpoch(), d.currentMSecsSinceEpoch(), 1);

    l_item.setFlags(M1Store::FULL_EDGE);
    l_item.setIncomingEdges(87);
    EXPECT_EQ(l_item.incomingEdges(), 87);
    l_item.addIncomingEdges(3);
    EXPECT_EQ(l_item.incomingEdges(), 90);
    l_item.setFlags(M1Store::FULL_VERTEX);
    l_item.setIncomingEdges(88);
    EXPECT_EQ(l_item.incomingEdges(), 88);
    l_item.addIncomingEdges(3);
    EXPECT_EQ(l_item.incomingEdges(), 91);

    l_item.setFlags(M1Store::FULL_VERTEX);
    l_item.setVisibleEdges(145);
    EXPECT_EQ(l_item.visibleEdges(), 145);
    l_item.addVisibleEdges(5);
    EXPECT_EQ(l_item.visibleEdges(), 150);

    QString s0("");
    l_item.setFlags(M1Store::SIMPLE_EDGE);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
    QString s1("toto dggsgsdghrwthhs sfghdsfsg sdsfggsdfg sdtewrwwtwrwjdd efhhjetyje");
    l_item.setText(s1);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, M1Store::ITEM_HAS_LOCAL_STRING);
    EXPECT_EQ(QString(l_item.text()), s1);
    l_item.setText(s0);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);

    l_item.setFlags(M1Store::SIMPLE_VERTEX);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
    QString s2("tutu JLK sfddsgfk dlfgkdfo dlfgkodpofit ddifigudffnd dfiiugoddeeoritu dfigu");
    l_item.setText(s2);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, M1Store::ITEM_HAS_LOCAL_STRING);
    EXPECT_EQ(QString(l_item.text()), s2);
    l_item.setText(s0);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);

    l_item.setFlags(M1Store::FULL_VERTEX);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
    EXPECT_EQ(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    QString s3;
    s3.fill('x', M1Store::FULL_VERTEX_TEXT_LEN -1);
    l_item.setText(s3);
    EXPECT_EQ(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, M1Store::ITEM_HAS_LOCAL_STRING);
    EXPECT_EQ(QString(l_item.text()), s3);
    s3 = "x" + s3;
    l_item.setText(s3);
    EXPECT_NE(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
    EXPECT_EQ(QString(l_item.text()), s3);
    s3 = "titi " + s3;
    l_item.setText(s3);
    EXPECT_NE(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
    EXPECT_EQ(QString(l_item.text()), s3);
    QString s4;
    s4.fill('z', M1Store::FULL_VERTEX_TEXT_LEN -1);
    l_item.setText(s4);
    EXPECT_EQ(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, M1Store::ITEM_HAS_LOCAL_STRING);
    EXPECT_EQ(QString(l_item.text()), s4);
    s4 = "z" + s4;
    l_item.setText(s4);
    EXPECT_NE(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
    EXPECT_EQ(QString(l_item.text()), s4);
    s4 = "tata " + s4;
    l_item.setText(s4);
    EXPECT_NE(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
    EXPECT_EQ(QString(l_item.text()), s4);
    l_item.setText(s0);
    EXPECT_EQ(M1Store::Storage::dbg_get_string_id(l_item), M1Store::G_VOID_ID);
    EXPECT_EQ(l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING, 0);
}
*/

// set/get of members in different Item configs
TEST(ItemTest, ValuesRoundTripFullVertex){
    qCDebug(g_cat_store_test) << QString("Putting random values in and retrieving them in a FULL_VERTEX");
    for(int t=0; t<20; t++){
        qCDebug(g_cat_store_test) << QString("Item: %1").arg(t);

        std::random_device l_rd; // obtain a random number from hardware
        std::mt19937 l_gen(l_rd()); // seed the generator
        std::uniform_int_distribution<int> l_distr_member(1, 18); // define the range for the choice of member
        std::uniform_int_distribution<M1Store::ItemID> l_distr_id(0, 0xffffffffffffffff -1); // define the range for ItemIDs
        std::uniform_int_distribution<M1Store::FlagField> l_distr_flags(0, 0xffffffffffffffff); // define the range for flags

        std::uniform_int_distribution<int> l_distr_year(0, 4000); // define the range for Date years
        std::uniform_int_distribution<int> l_distr_month(1, 12); // define the range for Date months
        std::uniform_int_distribution<int> l_distr_secs(0, 31 * 24 * 3600); // define the range for Date seconds increment (max one month)

        std::uniform_int_distribution<M1Store::ItemCounter> l_distr_counter(0, 0x7fffffffffffffff); // define the range for counters

        std::uniform_int_distribution<int> l_distr_short_text(0, M1Store::FULL_VERTEX_TEXT_LEN -1); // define the range for short text values
        std::uniform_int_distribution<int> l_distr_long_text(M1Store::FULL_VERTEX_TEXT_LEN, 400); // define the range for long text values

        std::uniform_int_distribution<char> l_distr_char(0, 0x10 * 3); // define the range for random char

        std::uniform_int_distribution<int> l_distr_slot(0, 3); // define the range for random special type slot
        std::uniform_int_distribution<unsigned short> l_distr_special(0, 0xffff); // define the range for random special type

        // bits 0, 1 and 2 are off limits
        std::uniform_int_distribution<int> l_distr_bit(0, 60); // define the range for random flag set/unset
        // not in this case
        std::uniform_int_distribution<int> l_distr_bit_extra(0, 61); // define the range for random flag set/unset

        M1Store::ItemID l_id = l_distr_id(l_gen);
        M1Store::FlagField l_flags = (l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::FULL_VERTEX;
        M1Store::ItemType l_type(l_distr_id(l_gen));

        M1Store::Item_lv0 l_item(l_id, l_flags, l_type);

        M1Store::FlagField l_flags_extra = 0;
        QDateTime l_date_creation = QDateTime::currentDateTime();
        QDateTime l_date_lastmod = QDateTime::currentDateTime();

        M1Store::ItemCounter l_incoming_edges = 0;
        M1Store::ItemCounter l_visible_edges = 0;

        M1Store::ItemID l_first_edge = M1Store::G_VOID_ID;
        M1Store::ItemID l_first_edge_special = M1Store::G_VOID_ID;

        M1Store::StringID l_string_id = M1Store::G_VOID_ID;
        M1Store::StringID l_search_string_id = M1Store::G_VOID_ID;

        QString l_text = QString();

        for(int i = 0; i<30; i++){
            int m = l_distr_member(l_gen);
            qCDebug(g_cat_store_test) << QString("Round: %1 --> %2 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++").arg(i).arg(m);
            /*
            void initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);

            ItemID m_id;
            FlagField m_flags;                               (1)
            ItemType m_type;                                 (2)  (12)
            FlagField m_flags_extra;                    ///< (3)  Extra flag field
            Date m_creation_date;                       ///< (4)  Creation date
            Date m_lastmod_date;                        ///< (5)  Last Modification date
            ItemCounter m_incoming_edges;               ///< (6)  (13) Number of incoming edges
            ItemCounter m_visible_edges;                ///< (7)  (14) Number of visible edges
            ItemID m_first_edge;                        ///< (8)  ID of first edge
            ItemID m_first_edge_special;                ///< (9)  ID of first special edge
            StringID m_string_id;                       ///<      ID of string, if any
            StringID m_search_string_id;                ///<      ID for the sarch string
            char m_text[FULL_VERTEX_TEXT_LEN];          ///< (10) (11) Local string space
            */
            char l_char;
            int l_slot;
            int l_increment;
            M1Store::FlagField l_bit;
            M1Store::SpecialItemID l_special;
            switch(m){
            case 1:
                // m_flags
                l_flags = ((l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::ITEM_IS_VERTEX) | (l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING);
                l_item.setFlags(l_flags);
                break;
            case 2:
                // m_type
                l_type = M1Store::ItemType(l_distr_id(l_gen));
                l_item.setType(l_type.getItemIDType());
                break;
            case 3:
                // m_flags_extra
                l_flags_extra = l_distr_flags(l_gen);
                l_item.setFlagsExtra(l_flags_extra);
                break;
            case 4:
                // m_creation_date
                l_date_creation = QDateTime(QDate(l_distr_year(l_gen), l_distr_month(l_gen), 1), QTime());
                l_date_creation = l_date_creation.addSecs(l_distr_secs(l_gen));
                l_item.setCreationDate(l_date_creation);
                break;
            case 5:
                // m_lastmod_date
                l_date_lastmod = QDateTime(QDate(l_distr_year(l_gen), l_distr_month(l_gen), 1), QTime());
                l_date_lastmod = l_date_lastmod.addSecs(l_distr_secs(l_gen));
                l_item.setLastmodDate(l_date_lastmod);
                break;
            case 6:
                // m_incoming_edges
                l_incoming_edges = l_distr_counter(l_gen);
                l_item.setIncomingEdges(l_incoming_edges);
                break;
            case 7:
                // l_visible_edges
                l_visible_edges = l_distr_counter(l_gen);
                l_item.setVisibleEdges(l_visible_edges);
                break;
            case 8:
                // m_first_edge
                l_first_edge = l_distr_id(l_gen);
                l_item.setFirstEdge(l_first_edge);
                break;
            case 9:
                // m_first_edge_special
                l_first_edge_special = l_distr_id(l_gen);
                l_item.setFirstEdgeSpecial(l_first_edge_special);
                break;
            case 10:
                // text below FULL_VERTEX_TEXT_LEN
                l_char = '0' + l_distr_char(l_gen);
                l_text.fill(l_char, l_distr_short_text(l_gen));
                if(l_text.length() > 0)
                    l_flags = l_flags | M1Store::ITEM_HAS_LOCAL_STRING;
                else
                    l_flags = l_flags & ~M1Store::ITEM_HAS_LOCAL_STRING;
                l_item.setText(l_text);
                break;
            case 11:
                // text above FULL_VERTEX_TEXT_LEN
                l_flags = l_flags & ~M1Store::ITEM_HAS_LOCAL_STRING;
                l_char = '0' + l_distr_char(l_gen);
                l_text.fill(l_char, l_distr_long_text(l_gen));
                l_item.setText(l_text);
                break;
            case 12:
                // single special ID type
                l_slot = l_distr_slot(l_gen);
                l_special = l_distr_special(l_gen);
                l_type.setSpecialType(l_slot, l_special);
                l_item.setType(l_slot, l_special);
                break;
            case 13:
                // add to incoming edges
                l_increment = l_distr_year(l_gen);
                l_incoming_edges += l_increment;
                l_item.addIncomingEdges(l_increment);
                break;
            case 14:
                // add to visible edges
                l_increment = l_distr_year(l_gen);
                l_visible_edges += l_increment;
                l_item.addVisibleEdges(l_increment);
                break;
            case 15:
                // set flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.setFlag(l_bit);
                l_flags = l_flags | l_bit;
                break;
            case 16:
                // unset flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.unSetFlag(l_bit);
                l_flags = l_flags & (~l_bit);
                break;
            case 17:
                // set flag extra
                l_bit = 0x8000000000000000 >> l_distr_bit_extra(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.setFlagExtra(l_bit);
                l_flags_extra = l_flags_extra | l_bit;
                break;
            case 18:
                // unset flag extra
                l_bit = 0x8000000000000000 >> l_distr_bit_extra(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.unSetFlagExtra(l_bit);
                l_flags_extra = l_flags_extra & (~l_bit);
                break;
            }
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing equalities").arg(i);
            EXPECT_EQ(l_item.id(), l_id);
            EXPECT_EQ(l_item.flags(), l_flags);
            EXPECT_EQ(l_item.getType(), l_type.getItemIDType());
            EXPECT_EQ(l_item.flagsExtra(), l_flags_extra);
            EXPECT_NEAR(l_item.creationDate().currentMSecsSinceEpoch(), l_date_creation.currentMSecsSinceEpoch(), 100);
            EXPECT_NEAR(l_item.lastmodDate().currentMSecsSinceEpoch(), l_date_lastmod.currentMSecsSinceEpoch(), 100);
            EXPECT_EQ(l_item.incomingEdges(), l_incoming_edges);
            EXPECT_EQ(l_item.visibleEdges(), l_visible_edges);
            EXPECT_EQ(l_item.firstEdge(), l_first_edge);
            EXPECT_EQ(l_item.firstEdgeSpecial(), l_first_edge_special);
            EXPECT_EQ(QString(l_item.text()), l_text);
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing Done ----------------------------------------------------").arg(i);
        }
        //qCDebug(g_cat_store_test) << QString("Random: %1").arg(l_distr(l_gen));
    }
}

TEST(ItemTest, ValuesRoundTripFullEdge){
    qCDebug(g_cat_store_test) << QString("Putting random values in and retrieving them in a FULL_EDGE");
    for(int t=0; t<20; t++){
        qCDebug(g_cat_store_test) << QString("Item: %1").arg(t);

        std::random_device l_rd; // obtain a random number from hardware
        std::mt19937 l_gen(l_rd()); // seed the generator
        std::uniform_int_distribution<int> l_distr_member(1, 20); // define the range for the choice of member
        std::uniform_int_distribution<M1Store::ItemID> l_distr_id(0, 0xffffffffffffffff -1); // define the range for ItemIDs
        std::uniform_int_distribution<M1Store::FlagField> l_distr_flags(0, 0xffffffffffffffff); // define the range for flags

        std::uniform_int_distribution<int> l_distr_year(0, 4000); // define the range for Date years
        std::uniform_int_distribution<int> l_distr_month(1, 12); // define the range for Date months
        std::uniform_int_distribution<int> l_distr_secs(0, 31 * 24 * 3600); // define the range for Date seconds increment (max one month)

        std::uniform_int_distribution<M1Store::ItemCounter> l_distr_counter(0, 0x7fffffffffffffff); // define the range for counters

        std::uniform_int_distribution<int> l_distr_short_text(0, M1Store::FULL_EDGE_TEXT_LEN -1); // define the range for short text values

        std::uniform_int_distribution<char> l_distr_char(0, 0x10 * 3); // define the range for random char

        std::uniform_int_distribution<int> l_distr_slot(0, 3); // define the range for random special type slot
        std::uniform_int_distribution<unsigned short> l_distr_special(0, 0xffff); // define the range for random special type

        // bits 0, 1 and 2 are off limits
        std::uniform_int_distribution<int> l_distr_bit(0, 60); // define the range for random flag set/unset
        // not in this case
        std::uniform_int_distribution<int> l_distr_bit_extra(0, 61); // define the range for random flag set/unset

        M1Store::ItemID l_id = l_distr_id(l_gen);
        M1Store::FlagField l_flags = (l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::FULL_EDGE;
        M1Store::ItemType l_type(l_distr_id(l_gen));

        M1Store::Item_lv0 l_item(l_id, l_flags, l_type);

        M1Store::FlagField l_flags_extra = 0;
        QDateTime l_date_creation = QDateTime::currentDateTime();
        QDateTime l_date_lastmod = QDateTime::currentDateTime();

        M1Store::ItemCounter l_incoming_edges = 0;

        M1Store::ItemID l_v_origin = M1Store::G_VOID_ID;
        M1Store::ItemID l_v_target = M1Store::G_VOID_ID;
        M1Store::ItemID l_e_previous = M1Store::G_VOID_ID;
        M1Store::ItemID l_e_next = M1Store::G_VOID_ID;
        M1Store::ItemID l_e_reciprocal = M1Store::G_VOID_ID;

        M1Store::ItemID l_first_edge = M1Store::G_VOID_ID;
        M1Store::ItemID l_first_edge_special = M1Store::G_VOID_ID;

        QString l_text = QString();

        for(int i = 0; i<30; i++){
            int m = l_distr_member(l_gen);
            qCDebug(g_cat_store_test) << QString("Round: %1 --> %2 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++").arg(i).arg(m);
            /*
            void initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);

            ItemID m_id;
            FlagField m_flags;                               (1)
            ItemType m_type;                                 (2)  (15)
            FlagField m_flags_extra;                    ///< (3)  Extra flag field
            ItemID m_v_origin;                          ///< (4)  origin
            ItemID m_v_target;                          ///< (5)  target
            ItemID m_e_previous;                        ///< (6)  previous edge
            ItemID m_e_next;                            ///< (7)  next edge
            ItemID m_e_reciprocal;                      ///< (8)  ID of reciprocal edge, if any
            Date m_creation_date;                       ///< (9)  Creation date
            Date m_lastmod_date;                        ///< (10) Last Modification date
            ItemCounter m_incoming_edges;               ///< (11) (16) Number of incoming edges
            ItemID m_first_edge;                        ///< (12) ID of first edge
            ItemID m_first_edge_special;                ///< (13) ID of first special edge
            char m_text[FULL_EDGE_TEXT_LEN];            ///< (14) Local string space
            */
            char l_char;
            int l_slot;
            int l_increment;
            M1Store::FlagField l_bit;
            M1Store::SpecialItemID l_special;
            switch(m){
            case 1:
                // m_flags
                l_flags = ((l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::FULL_EDGE) | (l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING);
                l_item.setFlags(l_flags);
                break;
            case 2:
                // m_type
                l_type = M1Store::ItemType(l_distr_id(l_gen));
                l_item.setType(l_type.getItemIDType());
                break;
            case 3:
                // m_flags_extra
                l_flags_extra = l_distr_flags(l_gen);
                l_item.setFlagsExtra(l_flags_extra);
                break;
            case 4:
                // m_v_origin
                l_v_origin = l_distr_id(l_gen);
                l_item.setOrigin(l_v_origin);
                break;
            case 5:
                // m_v_target
                l_v_target = l_distr_id(l_gen);
                l_item.setTarget(l_v_target);
                break;
            case 6:
                // m_e_previous
                l_e_previous = l_distr_id(l_gen);
                l_item.setPrevious(l_e_previous);
                break;
            case 7:
                // m_e_next
                l_e_next = l_distr_id(l_gen);
                l_item.setNext(l_e_next);
                break;
            case 8:
                // m_e_reciprocal
                l_e_reciprocal = l_distr_id(l_gen);
                l_item.setReciprocal(l_e_reciprocal);
                break;
            case 9:
                // m_creation_date
                l_date_creation = QDateTime(QDate(l_distr_year(l_gen), l_distr_month(l_gen), 1), QTime());
                l_date_creation = l_date_creation.addSecs(l_distr_secs(l_gen));
                l_item.setCreationDate(l_date_creation);
                break;
            case 10:
                // m_lastmod_date
                l_date_lastmod = QDateTime(QDate(l_distr_year(l_gen), l_distr_month(l_gen), 1), QTime());
                l_date_lastmod = l_date_lastmod.addSecs(l_distr_secs(l_gen));
                l_item.setLastmodDate(l_date_lastmod);
                break;
            case 11:
                // m_incoming_edges
                l_incoming_edges = l_distr_counter(l_gen);
                l_item.setIncomingEdges(l_incoming_edges);
                break;
            case 12:
                // m_first_edge
                l_first_edge = l_distr_id(l_gen);
                l_item.setFirstEdge(l_first_edge);
                break;
            case 13:
                // m_first_edge_special
                l_first_edge_special = l_distr_id(l_gen);
                l_item.setFirstEdgeSpecial(l_first_edge_special);
                break;
            case 14:
                // text below FULL_EDGE_TEXT_LEN
                l_char = '0' + l_distr_char(l_gen);
                l_text.fill(l_char, l_distr_short_text(l_gen));
                if(l_text.length() > 0)
                    l_flags = l_flags | M1Store::ITEM_HAS_LOCAL_STRING;
                else
                    l_flags = l_flags & ~M1Store::ITEM_HAS_LOCAL_STRING;
                l_item.setText(l_text);
                break;
            case 15:
                // single special ID type
                l_slot = l_distr_slot(l_gen);
                l_special = l_distr_special(l_gen);
                l_type.setSpecialType(l_slot, l_special);
                l_item.setType(l_slot, l_special);
                break;
            case 16:
                // add to incoming edges
                l_increment = l_distr_year(l_gen);
                l_incoming_edges += l_increment;
                l_item.addIncomingEdges(l_increment);
                break;
            case 17:
                // set flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.setFlag(l_bit);
                l_flags = l_flags | l_bit;
                break;
            case 18:
                // unset flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.unSetFlag(l_bit);
                l_flags = l_flags & (~l_bit);
                break;
            case 19:
                // set flag extra
                l_bit = 0x8000000000000000 >> l_distr_bit_extra(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.setFlagExtra(l_bit);
                l_flags_extra = l_flags_extra | l_bit;
                break;
            case 20:
                // unset flag extra
                l_bit = 0x8000000000000000 >> l_distr_bit_extra(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.unSetFlagExtra(l_bit);
                l_flags_extra = l_flags_extra & (~l_bit);
                break;
            }
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing equalities").arg(i);
            EXPECT_EQ(l_item.id(), l_id);
            EXPECT_EQ(l_item.flags(), l_flags);
            EXPECT_EQ(l_item.getType(), l_type.getItemIDType());
            EXPECT_EQ(l_item.flagsExtra(), l_flags_extra);
            EXPECT_NEAR(l_item.creationDate().currentMSecsSinceEpoch(), l_date_creation.currentMSecsSinceEpoch(), 100);
            EXPECT_NEAR(l_item.lastmodDate().currentMSecsSinceEpoch(), l_date_lastmod.currentMSecsSinceEpoch(), 100);
            EXPECT_EQ(l_item.incomingEdges(), l_incoming_edges);
            EXPECT_EQ(l_item.firstEdge(), l_first_edge);
            EXPECT_EQ(l_item.firstEdgeSpecial(), l_first_edge_special);
            EXPECT_EQ(l_item.origin(), l_v_origin);
            EXPECT_EQ(l_item.target(), l_v_target);
            EXPECT_EQ(l_item.previous(), l_e_previous);
            EXPECT_EQ(l_item.next(), l_e_next);
            EXPECT_EQ(l_item.reciprocal(), l_e_reciprocal);
            EXPECT_EQ(QString(l_item.text()), l_text);
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing Done ----------------------------------------------------").arg(i);
        }
        //qCDebug(g_cat_store_test) << QString("Random: %1").arg(l_distr(l_gen));
    }
}

TEST(ItemTest, ValuesRoundTripSimpleEdge){
    qCDebug(g_cat_store_test) << QString("Putting random values in and retrieving them in a FULL_EDGE");
    for(int t=0; t<20; t++){
        qCDebug(g_cat_store_test) << QString("Item: %1").arg(t);

        std::random_device l_rd; // obtain a random number from hardware
        std::mt19937 l_gen(l_rd()); // seed the generator
        std::uniform_int_distribution<int> l_distr_member(1, 9); // define the range for the choice of member
        std::uniform_int_distribution<M1Store::ItemID> l_distr_id(0, 0xffffffffffffffff -1); // define the range for ItemIDs
        std::uniform_int_distribution<M1Store::FlagField> l_distr_flags(0, 0xffffffffffffffff); // define the range for flags

        std::uniform_int_distribution<int> l_distr_short_text(0, M1Store::SIMPLE_EDGE_TEXT_LEN -1); // define the range for short text values

        std::uniform_int_distribution<char> l_distr_char(0, 0x10 * 3); // define the range for random char

        std::uniform_int_distribution<int> l_distr_slot(0, 3); // define the range for random special type slot
        std::uniform_int_distribution<unsigned short> l_distr_special(0, 0xffff); // define the range for random special type

        // bits 0, 1 and 2 are off limits
        std::uniform_int_distribution<int> l_distr_bit(0, 60); // define the range for random flag set/unset

        M1Store::ItemID l_id = l_distr_id(l_gen);
        M1Store::FlagField l_flags = (l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::SIMPLE_EDGE;
        M1Store::ItemType l_type(l_distr_id(l_gen));

        M1Store::Item_lv0 l_item(l_id, l_flags, l_type);

        M1Store::ItemID l_v_origin = M1Store::G_VOID_ID;
        M1Store::ItemID l_e_previous = M1Store::G_VOID_ID;
        M1Store::ItemID l_e_next = M1Store::G_VOID_ID;

        QString l_text = QString();

        for(int i = 0; i<30; i++){
            int m = l_distr_member(l_gen);
            qCDebug(g_cat_store_test) << QString("Round: %1 --> %2 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++").arg(i).arg(m);
            /*
            void initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);

            ItemID m_id;
            FlagField m_flags;                               (1)
            ItemType m_type;                                 (2)  (7)
            StringID m_search_string_id;                ///<      ID for the sarch string
            ItemID m_v_origin;                          ///< (3)  origin
            ItemID m_e_previous;                        ///< (4)  previou edge
            ItemID m_e_next;                            ///< (5)  next edge
            char m_text[SIMPLE_EDGE_TEXT_LEN];          ///< (6)  Local string space
            */
            char l_char;
            int l_slot;
            int l_increment;
            M1Store::FlagField l_bit;
            M1Store::SpecialItemID l_special;
            switch(m){
            case 1:
                // m_flags
                l_flags = ((l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::SIMPLE_EDGE) | (l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING);
                l_item.setFlags(l_flags);
                break;
            case 2:
                // m_type
                l_type = M1Store::ItemType(l_distr_id(l_gen));
                l_item.setType(l_type.getItemIDType());
                break;
            case 3:
                // m_v_origin
                l_v_origin = l_distr_id(l_gen);
                l_item.setOrigin(l_v_origin);
                break;
            case 4:
                // m_e_previous
                l_e_previous = l_distr_id(l_gen);
                l_item.setPrevious(l_e_previous);
                break;
            case 5:
                // m_e_next
                l_e_next = l_distr_id(l_gen);
                l_item.setNext(l_e_next);
                break;
            case 6:
                // text below FULL_EDGE_TEXT_LEN
                l_char = '0' + l_distr_char(l_gen);
                l_text.fill(l_char, l_distr_short_text(l_gen));
                if(l_text.length() > 0)
                    l_flags = l_flags | M1Store::ITEM_HAS_LOCAL_STRING;
                else
                    l_flags = l_flags & ~M1Store::ITEM_HAS_LOCAL_STRING;
                l_item.setText(l_text);
                break;
            case 7:
                // single special ID type
                l_slot = l_distr_slot(l_gen);
                l_special = l_distr_special(l_gen);
                l_type.setSpecialType(l_slot, l_special);
                l_item.setType(l_slot, l_special);
                break;
            case 8:
                // set flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.setFlag(l_bit);
                l_flags = l_flags | l_bit;
                break;
            case 9:
                // unset flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.unSetFlag(l_bit);
                l_flags = l_flags & (~l_bit);
                break;
            }
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing equalities").arg(i);
            EXPECT_EQ(l_item.id(), l_id);
            EXPECT_EQ(l_item.flags(), l_flags);
            EXPECT_EQ(l_item.getType(), l_type.getItemIDType());
            EXPECT_EQ(l_item.origin(), l_v_origin);
            EXPECT_EQ(l_item.previous(), l_e_previous);
            EXPECT_EQ(l_item.next(), l_e_next);
            EXPECT_EQ(QString(l_item.text()), l_text);
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing Done ----------------------------------------------------").arg(i);
        }
        //qCDebug(g_cat_store_test) << QString("Random: %1").arg(l_distr(l_gen));
    }
}

TEST(ItemTest, ValuesRoundTripSimpleVertex){
    qCDebug(g_cat_store_test) << QString("Putting random values in and retrieving them in a FULL_EDGE");
    for(int t=0; t<20; t++){
        qCDebug(g_cat_store_test) << QString("Item: %1").arg(t);

        std::random_device l_rd; // obtain a random number from hardware
        std::mt19937 l_gen(l_rd()); // seed the generator
        std::uniform_int_distribution<int> l_distr_member(1, 6); // define the range for the choice of member
        std::uniform_int_distribution<M1Store::ItemID> l_distr_id(0, 0xffffffffffffffff -1); // define the range for ItemIDs
        std::uniform_int_distribution<M1Store::FlagField> l_distr_flags(0, 0xffffffffffffffff); // define the range for flags

        std::uniform_int_distribution<int> l_distr_short_text(0, M1Store::SIMPLE_VERTEX_TEXT_LEN -1); // define the range for short text values

        std::uniform_int_distribution<char> l_distr_char(0, 0x10 * 3); // define the range for random char

        std::uniform_int_distribution<int> l_distr_slot(0, 3); // define the range for random special type slot
        std::uniform_int_distribution<unsigned short> l_distr_special(0, 0xffff); // define the range for random special type

        // bits 0, 1 and 2 are off limits
        std::uniform_int_distribution<int> l_distr_bit(0, 60); // define the range for random flag set/unset

        M1Store::ItemID l_id = l_distr_id(l_gen);
        M1Store::FlagField l_flags = (l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::SIMPLE_VERTEX;
        M1Store::ItemType l_type(l_distr_id(l_gen));

        M1Store::Item_lv0 l_item(l_id, l_flags, l_type);

        QString l_text = QString();

        for(int i = 0; i<30; i++){
            int m = l_distr_member(l_gen);
            qCDebug(g_cat_store_test) << QString("Round: %1 --> %2 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++").arg(i).arg(m);
            /*
            void initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);

            ItemID m_id;
            FlagField m_flags;                               (1)
            ItemType m_type;                                 (2)  (4)
            StringID m_search_string_id;                ///<      ID for the sarch string
            char m_text[SIMPLE_VERTEX_TEXT_LEN];        ///< (3)  Local string space
            */
            char l_char;
            int l_slot;
            M1Store::FlagField l_bit;
            M1Store::SpecialItemID l_special;
            switch(m){
            case 1:
                // m_flags
                l_flags = ((l_distr_flags(l_gen) & ~M1Store::ITEM_BASE0_MASK) | M1Store::SIMPLE_VERTEX) | (l_item.flags() & M1Store::ITEM_HAS_LOCAL_STRING);
                l_item.setFlags(l_flags);
                break;
            case 2:
                // m_type
                l_type = M1Store::ItemType(l_distr_id(l_gen));
                l_item.setType(l_type.getItemIDType());
                break;
            case 3:
                // text below FULL_EDGE_TEXT_LEN
                l_char = '0' + l_distr_char(l_gen);
                l_text.fill(l_char, l_distr_short_text(l_gen));
                if(l_text.length() > 0)
                    l_flags = l_flags | M1Store::ITEM_HAS_LOCAL_STRING;
                else
                    l_flags = l_flags & ~M1Store::ITEM_HAS_LOCAL_STRING;
                l_item.setText(l_text);
                break;
            case 4:
                // single special ID type
                l_slot = l_distr_slot(l_gen);
                l_special = l_distr_special(l_gen);
                l_type.setSpecialType(l_slot, l_special);
                l_item.setType(l_slot, l_special);
                break;
            case 5:
                // set flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.setFlag(l_bit);
                l_flags = l_flags | l_bit;
                break;
            case 6:
                // unset flag
                l_bit = 0x8000000000000000 >> l_distr_bit(l_gen);
                qCDebug(g_cat_store_test) << QString("l_bit %1 (0b%2)").arg(l_bit).arg(l_bit, 64, 2, QLatin1Char('0'));
                l_item.unSetFlag(l_bit);
                l_flags = l_flags & (~l_bit);
                break;
            }
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing equalities").arg(i);
            EXPECT_EQ(l_item.id(), l_id);
            EXPECT_EQ(l_item.flags(), l_flags);
            EXPECT_EQ(l_item.getType(), l_type.getItemIDType());
            EXPECT_EQ(QString(l_item.text()), l_text);
            qCDebug(g_cat_store_test) << QString("Round: %1 Testing Done ----------------------------------------------------").arg(i);
        }
        //qCDebug(g_cat_store_test) << QString("Random: %1").arg(l_distr(l_gen));
    }
}


