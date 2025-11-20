#include <QtGlobal>
#include <QDebug>

#include <string.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include <gtest/gtest.h>

#include "m1A_env.h"
#include "m1B_store.h"

Q_LOGGING_CATEGORY(g_cat_lv0_members, "lv0.members_access")

// ---------------------------------------------------------------------------------------------------------
// ----------------------------- M1Store::Item -------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------

//**************************************************************************************************************************
/** \defgroup ConstLv0 Constructors and member initialization
 *  \ingroup LV0
 *
 *  Constructors are not used except for tests. Normally Item_lv0 instances are initialized from within the mmap() area.
 *  Hence the importance of initializeMembers()
 *  @{
 */

/**
 * @brief Contructor with ID, flags and type - not used except for tests
 * @param p_item_id The item ID
 * @param p_flags flags
 * @param p_type ItemType instance
 */
M1Store::Item_lv0::Item_lv0(const ItemID p_item_id, const FlagField p_flags, const ItemType& p_type){
    initializeMembers(p_item_id, p_flags, p_type);
}

/**
 * @brief Item initialization with all values
 * @param p_ID The item ID
 * @param p_flags flags
 * @param p_type type
 */
void M1Store::Item_lv0::initializeMembers(const M1Store::ItemID p_item_id, const M1Store::FlagField p_flags, const M1Store::ItemType& p_type){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Item initialization p_id: 0x%1, p_flags: 0b%2, p_type: %3")
                      .arg(p_item_id, 16, 16, QLatin1Char('0'))   // %1
                      .arg(p_flags, 64, 2, QLatin1Char('0'))      // %2
                      .arg(p_type.dbgStringHr()))                 // %3

    m_item_id = p_item_id;
    m_flags = p_flags;
    m_type = p_type;
    initializeMembers();

    M1_FUNC_EXIT
}

/**
 * @brief Initialization of branch-specific members members
 */
void M1Store::Item_lv0::initializeMembers(){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Initializing members ..."))

    if((m_flags & ITEM_NATURE_MASK) == FULL_EDGE){
        // full edge
        qCDebug(g_cat_lv0_members) << QString("Full Edge");
        p.e.f.m_flags_extra = 0;
        p.e.f.m_creation_date = QDateTime::currentDateTime().currentMSecsSinceEpoch();
        p.e.f.m_lastmod_date = QDateTime::currentDateTime().currentMSecsSinceEpoch();
        p.e.f.m_v_origin = G_VOID_ITEM_ID;
        p.e.f.m_v_target = G_VOID_ITEM_ID;
        p.e.f.m_e_previous = G_VOID_ITEM_ID;
        p.e.f.m_e_next = G_VOID_ITEM_ID;
        p.e.f.m_e_reciprocal = G_VOID_ITEM_ID;
        p.e.f.m_first_edge = G_VOID_ITEM_ID;
        p.e.f.m_first_edge_special = G_VOID_ITEM_ID;
        p.e.f.m_incoming_edges = 0;
        p.e.f.m_text[0] = 0; // ""
    }
    else if((m_flags & ITEM_NATURE_MASK) == SIMPLE_EDGE){
        // simple edge
        qCDebug(g_cat_lv0_members) << QString("Simple Edge");
        p.e.s.m_e_next = G_VOID_ITEM_ID;
        p.e.s.m_e_previous = G_VOID_ITEM_ID;
        p.e.s.m_v_origin = G_VOID_ITEM_ID;
        p.e.s.m_string_id = G_VOID_ITEM_ID;
        p.e.s.m_text[0] = 0; // ""
    }
    else if((m_flags & ITEM_NATURE_MASK) == FULL_VERTEX){
        // full vertex
        qCDebug(g_cat_lv0_members) << QString("Full Vertex");
        p.v.f.m_creation_date = QDateTime::currentDateTime().currentMSecsSinceEpoch();
        p.v.f.m_lastmod_date = QDateTime::currentDateTime().currentMSecsSinceEpoch();
        p.v.f.m_first_edge = G_VOID_ITEM_ID;
        p.v.f.m_auto_edge = G_VOID_ITEM_ID;
        p.v.f.m_first_edge_special = G_VOID_ITEM_ID;
        p.v.f.m_flags_extra = 0;
        p.v.f.m_incoming_edges = 0;
        p.v.f.m_search_string_id = G_VOID_ITEM_ID;
        p.v.f.m_string_id = G_VOID_ITEM_ID;
        p.v.f.m_text[0] = 0; // ""
    }
    else {
        // simple vertex (only case left)
        qCDebug(g_cat_lv0_members) << QString("Simple Vertex");
        p.v.s.m_search_string_id = G_VOID_ITEM_ID;
        p.v.s.m_text[0] = 0; // ""
    }

    M1_FUNC_EXIT
}
/** @} end group ConstLv0 */
//**************************************************************************************************************************

/**
 * @brief get the ID of this Item
 * @return the ID
 */
M1Store::ItemID M1Store::Item_lv0::item_id() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Get Item ID: 0x%1").arg(m_item_id, 16, 16, QLatin1Char('0')))

    M1_FUNC_EXIT
    return m_item_id;
}

// common flag field access
// initialize all members if the vertex/edge or simple/full flags are changed
/**
 * @brief set the main flag field
 *
 * member initialization is performed if p_force_init is true or if one of the flags ITEM_IS_VERTEX or ITEM_IS_SIMPLE changed, i.e.
 * if the vertex/edge or simple/full flags changed. In this case, m_id is not changed, but m_type is reset
 * as well as all other flags from m_flags
 *
 * @param p_flags the new value (by default, false)
 * @param p_force_init true -> force initialization of other fields
 */
void M1Store::Item_lv0::setFlags_lv0(const M1Store::FlagField p_flags, const bool p_force_init){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Setting flags to %1 (0b%2)").arg(p_flags).arg(p_flags, 64, 2, QLatin1Char('0')))

    M1Store::FlagField l_old_flags = m_flags;
    m_flags = p_flags;
    if( (((l_old_flags & ITEM_NATURE_MASK) ^ (m_flags & ITEM_NATURE_MASK)) > 0) || p_force_init ){
        // erase all other flags
        m_flags = m_flags & ITEM_NATURE_MASK;
        // initalize as 4 void special types (TYPE_IS_ITEM_ID has just been set to 0)
        m_type.initSpecials();
        // initialize other members
        initializeMembers();
    }

    M1_FUNC_EXIT
}
/**
 * @brief set a particular flag
 * @param p_flag the flag
 * @param p_force_init true -> force initialization of other fields
 */
void M1Store::Item_lv0::setFlag(const M1Store::FlagField p_flag, const bool p_force_init){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Setting flag %1 (0b%2)").arg(p_flag).arg(p_flag, 64, 2, QLatin1Char('0')))
    Q_ASSERT_X(M1Store::Item_lv0::count_1_bits(p_flag) == 1,
               "M1Store::Item_lv0::setFlag(M1Store::FlagField, bool)",
               "p_flag must have exactly one bit set");

    M1Store::FlagField l_old_flags = m_flags;
    m_flags = m_flags | p_flag;
    if( (((l_old_flags & ITEM_NATURE_MASK) ^ (m_flags & ITEM_NATURE_MASK)) > 0) || p_force_init ){
        // erase all other flags
        m_flags = m_flags & ITEM_NATURE_MASK;
        // initalize as 4 void special types (TYPE_IS_ITEM_ID has just been set to 0)
        m_type.initSpecials();
        // initialize other members
        initializeMembers();
    }

    M1_FUNC_EXIT
}
/**
 * @brief unset a particular flag
 * @param p_flag the flag
 * @param p_force_init true -> force initialization of other fields
 */
void M1Store::Item_lv0::unSetFlag(const M1Store::FlagField p_flag, const bool p_force_init){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Unsetting flag %1 (0b%2)").arg(p_flag).arg(p_flag, 64, 2, QLatin1Char('0')))
    Q_ASSERT_X(M1Store::Item_lv0::count_1_bits(p_flag) == 1,
               "M1Store::Item_lv0::unSetFlag(M1Store::FlagField, bool)",
               "p_flag must have exactly one bit set");

    M1Store::FlagField l_old_flags = m_flags;
    m_flags = m_flags & (~p_flag);
    if( (((l_old_flags & ITEM_NATURE_MASK) ^ (m_flags & ITEM_NATURE_MASK)) > 0) || p_force_init ){
        // erase all other flags
        m_flags = m_flags & ITEM_NATURE_MASK;
        // initalize as 4 void special types (TYPE_IS_ITEM_ID has just been set to 0)
        m_type.initSpecials();
        // initialize other members
        initializeMembers();
    }

    M1_FUNC_EXIT
}
// number of bits == 1 in a flag field (for set/unset flags)
unsigned short M1Store::Item_lv0::count_1_bits(const M1Store::FlagField p_flag){
    unsigned int l_ret = 0;
    M1Store::FlagField n = p_flag;
    while (n) {
        l_ret += n & 1;
        n >>= 1;
    }

    return l_ret;
}
/**
 * @brief get main flag field value
 * @return this value
 */
M1Store::FlagField M1Store::Item_lv0::flags() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("get main flag field %1 (0b%2)").arg(m_flags).arg(m_flags, 64, 2, QLatin1Char('0')))

    M1_FUNC_EXIT
    return m_flags;
}

/**
 * @brief M1Store::Item_lv0::storeSpecialItemID
 * @param p_si_id
 */
void M1Store::Item_lv0::storeSpecialItemID(const SpecialItemID p_si_id){
    FlagField l_id_as_flags = p_si_id;
    l_id_as_flags = l_id_as_flags << 48;
    this->setFlags_lv0( (flags() & 0x0000FFFFFFFFFFFF) | l_id_as_flags);
}
/**
 * @brief M1Store::Item_lv0::specialItemId
 * @return
 */
M1Env::SpecialItemID M1Store::Item_lv0::specialItemId(){
    return (flags() & 0xFFFF000000000000) >> 48;
}

//**************************************************************************************************************************
/** \defgroup IOT0 Type Setting and Testing (only from m_type)
 *  \ingroup LV0
 *
 *  Tests whether an item is of a type contained in the m_type field (4x SpecialItemID)
 *  @{
 */

/**
 * @brief Basic type-setting method - sets the entire m_type member
 * @param p_type the ItemType to set
 */
void M1Store::Item_lv0::setType_member_lv0(const ItemType& p_type){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Set m_type to").arg(p_type.dbgString()))
    m_type = p_type;
    M1_FUNC_EXIT
}

/**
 * @brief set one of the 4 special types
 * @param p_index index of the one to set (0 to 3)
 * @param p_type the new value
 */
void M1Store::Item_lv0::setType_member_si_id(const unsigned int p_index, const M1Store::SpecialItemID p_type){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Set special type %1 to %2").arg(p_index).arg(p_type))
    Q_ASSERT(p_index < 4);

    m_type.setSpecialType(p_index, p_type);

    M1_FUNC_EXIT
}

/**
 * @brief get one of the 4 special types
 * @param p_index index of the one to get (0 to 3)
 * @return the value of this special type
 */
M1Store::SpecialItemID M1Store::Item_lv0::getType_si_id(const unsigned int p_index) const{
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("get special type %1 --> %2").arg(p_index).arg(m_type.getSpecialType(p_index)))
    Q_ASSERT(p_index < 4);

    M1Store::SpecialItemID l_ret = m_type.getSpecialType(p_index);
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief as a single ItemID type value
 * @return The single ItemID type value
 */
M1Store::ItemID M1Store::Item_lv0::getType_item_id() const{
    M1Store::ItemID l_ret = m_type.getItemIDType();
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("get single ID type 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret))

    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief set type as an ItemID
 * @param p_type the new type value
 */
void M1Store::Item_lv0::setType_member_item_id(const M1Store::ItemID p_type_item_id){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Set Item ID type to 0x%1").arg(p_type_item_id, 16, 16, QLatin1Char('0')))
    m_type = p_type_item_id;

    M1_FUNC_EXIT
}

/**
 * @brief test if item is of a given ItemID type (using only the information in m_type)
 * @param p_type the value to test (an ItemID)
 * @return true/false
 */
bool M1Store::Item_lv0::isOfType_member(const M1Store::ItemID p_type_item_id) const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Checking whether is of type (ItemID) 0x%1 ...").arg(p_type_item_id, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & TYPE_IS_ITEM_ID) > 0,
               "M1Store::Item_lv0::isOfType(M1Store::ItemID)",
               "requesting an ItemID type test while TYPE_IS_ITEM_ID is not set");

    bool l_ret = m_type.getItemIDType() == p_type_item_id;
    qCDebug(g_cat_lv0_members) << QString("--> %1").arg(l_ret);


    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief test if item is of a given special type (using only the information in m_type)
 * @param p_type the value to test (a SpecialItemID)
 * @return true/false
 */
bool M1Store::Item_lv0::isOfType_member(const M1Store::SpecialItemID p_type) const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Checking whether is of type (SpecialItemID) %1 ...").arg(p_type))
    Q_ASSERT_X((m_flags & TYPE_IS_ITEM_ID) == 0,
               "M1Store::Item_lv0::isOfType(M1Store::SpecialItemID)",
               "requesting a SpecialItemID type test while the proper flags are not set");

    bool l_ret = false;
    for(int i = 0; i<4; i++) l_ret = l_ret || (m_type.getSpecialType(i) == p_type);
    qCDebug(g_cat_lv0_members) << QString("--> %1").arg(l_ret);


    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief test if item is of a given special type mnemonic (using only the information in m_type)
 * @param p_type the value to test (a char*)
 * @return true/false
 */
bool M1Store::Item_lv0::isOfType_member(const char* p_mnemonic) const{
    SpecialItem* pi = Storage::getSpecialItemPointer(p_mnemonic);
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Checking whether is of type (mnemonic) %1 ...").arg(p_mnemonic))

    bool l_ret = M1Store::Item_lv0::isOfType_member(pi->specialId());
    M1_FUNC_EXIT
        return l_ret;
}
/** @} end group IOT0*/
//**************************************************************************************************************************

// ---------------------------------------------------------------------------------------------------------
// extra flag field access (not for simple items)
/**
 * @brief set the extra flags field (not for simple items)
 * @param p_flags the new value
 */
void M1Store::Item_lv0::setFlagsExtra_lv0(const M1Store::FlagField p_flags){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Setting extra flag field to 0x%1 (0b%2)").arg(p_flags,0,16).arg(p_flags, 64, 2, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0,
               "M1Store::Item::setFlagsExtra()",
               "simple items do not have an extra flag field");

    if(m_flags & ITEM_IS_VERTEX)
        p.v.f.m_flags_extra = p_flags;
    else
        p.e.f.m_flags_extra = p_flags;

    M1_FUNC_EXIT
}
/**
 * @brief set a particular flag (in the extra field)
 * @param p_flag the flag
 * @param p_force_init
 */
void M1Store::Item_lv0::setFlagExtra(const M1Store::FlagField p_flag){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Setting flag %1 (0b%2)").arg(p_flag).arg(p_flag, 64, 2, QLatin1Char('0')))
    Q_ASSERT_X(M1Store::Item_lv0::count_1_bits(p_flag) == 1,
               "M1Store::Item_lv0::setFlagExtra(M1Store::FlagField)",
               "p_flag must have exactly one bit set");
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0,
               "M1Store::Item::setFlagExtra()",
               "simple items do not have an extra flag field");
    if(m_flags & ITEM_IS_VERTEX)
        p.v.f.m_flags_extra = p.v.f.m_flags_extra | p_flag;
    else
        p.e.f.m_flags_extra = p.e.f.m_flags_extra | p_flag;

    M1_FUNC_EXIT
}
/**
 * @brief unset a particular flag (in the extra field)
 * @param p_flag the flag
 * @param p_force_init
 */
void M1Store::Item_lv0::unSetFlagExtra(const M1Store::FlagField p_flag){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Setting flag %1 (0b%2)").arg(p_flag).arg(p_flag, 64, 2, QLatin1Char('0')))
    Q_ASSERT_X(M1Store::Item_lv0::count_1_bits(p_flag) == 1,
               "M1Store::Item_lv0::unSetFlagExtra(M1Store::FlagField)",
               "p_flag must have exactly one bit set");
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0,
               "M1Store::Item::unSetFlagExtra()",
               "simple items do not have an extra flag field");
    if(m_flags & ITEM_IS_VERTEX)
        p.v.f.m_flags_extra = p.v.f.m_flags_extra & (~p_flag);
    else
        p.e.f.m_flags_extra = p.e.f.m_flags_extra & (~p_flag);

    M1_FUNC_EXIT
}
/**
 * @brief get the extra flags field (if there is one)
 * @return the flags value
 */
M1Store::FlagField M1Store::Item_lv0::flagsExtra() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting extra flag field ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0,
               "M1Store::Item::flagsExtra()",
               "simple items do not have an extra flag field");

    FlagField l_ret = 0;
    if(m_flags & ITEM_IS_VERTEX)
        l_ret = p.v.f.m_flags_extra;
    else
        l_ret = p.e.f.m_flags_extra;

    qCDebug(g_cat_lv0_members) << QString("--> %1 (0b%2)").arg(l_ret).arg(l_ret, 64, 2, QLatin1Char('0'));

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// edge origin access (only for edges)
/**
 * @brief set origin (edges only)
 * @param p_origin ItemID of origin
 */
void M1Store::Item_lv0::setOrigin_lv0(const M1Store::ItemID p_origin){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting m_v_origin to 0x%1").arg(p_origin, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_IS_VERTEX) == 0,
               "Item::setOrigin()",
               "accessing vOrigin on a vertex");

    if(m_flags & ITEM_IS_SIMPLE) p.e.s.m_v_origin = p_origin;
    else p.e.f.m_v_origin = p_origin;

    M1_FUNC_EXIT
}
/**
 * @brief get origin (edges only)
 * @return ItemID of origin
 */
M1Store::ItemID M1Store::Item_lv0::origin_item_id() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting m_v_origin ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_VERTEX) == 0, "Item::origin()",
               "accessing vOrigin on a vertex");

    ItemID l_ret;
    if(m_flags & ITEM_IS_SIMPLE) l_ret = p.e.s.m_v_origin;
    else l_ret =  p.e.f.m_v_origin;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// edge target access (only for full edges)
/**
 * @brief set target ItemID (only for full edges)
 * @param p_target the new value
 */
void M1Store::Item_lv0::setTarget_lv0(const M1Store::ItemID p_target){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting m_v_target to 0x%1").arg(p_target, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_NATURE_MASK) == 0,
               "Item::setTarget(M1Store::ItemID)",
               "accessing the target of an item that is not a full edge");

    p.e.f.m_v_target = p_target;
    // increase target's incoming edges counter
    M1Store::Item_lv0* l_target_pointer = M1Store::Storage::getItemPointer_lv0(p_target);
    if((l_target_pointer->flags() & ITEM_NATURE_MASK) == M1Env::FULL_EDGE || (l_target_pointer->flags() & ITEM_NATURE_MASK) == M1Env::FULL_VERTEX)
        l_target_pointer->setIncomingEdges_lv0(l_target_pointer->incomingEdges() + 1);

    M1_FUNC_EXIT
}
/**
 * @brief get target ItemID (only for full edges)
 * @return the value
 */
M1Store::ItemID M1Store::Item_lv0::target_item_id() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting m_v_target ..."))
    Q_ASSERT_X((m_flags & ITEM_NATURE_MASK) == 0,
               "Item::target()",
               "accessing the target of an item that is not a full edge");

    ItemID l_ret = p.e.f.m_v_target;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// previous edge access (for edges)
/**
 * @brief set previous edge ItemID (only for edges)
 * @param p_previous the new value
 */
void M1Store::Item_lv0::setPrevious_lv0(const M1Store::ItemID p_previous){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting previous edge to 0x%1").arg(p_previous, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_IS_VERTEX) == 0,
               "Item::setPrevious()",
               "accessing the previous edge on a vertex");

    if(m_flags & ITEM_IS_SIMPLE) p.e.s.m_e_previous = p_previous;
    else p.e.f.m_e_previous = p_previous;

    M1_FUNC_EXIT
}
/**
 * @brief get previous edge ItemID (only for edges)
 * @return the value
 */
M1Store::ItemID M1Store::Item_lv0::previous_item_id() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting previous edge ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_VERTEX) == 0,
               "Item::previous()",
               "accessing the previous edge on a vertex");

    ItemID l_ret;
    if(m_flags & ITEM_IS_SIMPLE) l_ret = p.e.s.m_e_previous;
    else l_ret = p.e.f.m_e_previous;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// next edge (for edges)
/**
 * @brief set next edge ItemID (edges only)
 * @param p_next the new value
 */
void M1Store::Item_lv0::setNext_lv0(const M1Store::ItemID p_next){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting next edge to 0x%1").arg(p_next, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_IS_VERTEX) == 0, "Item::setNext()", "accessing the next edge on a vertex");

    if(m_flags & ITEM_IS_SIMPLE) p.e.s.m_e_next = p_next;
    else p.e.f.m_e_next = p_next;

    M1_FUNC_EXIT
}
/**
 * @brief get next edge ItemID (edges only)
 * @return the value
 */
M1Store::ItemID M1Store::Item_lv0::next_item_id() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting next edge ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_VERTEX) == 0, "Item::next()", "accessing the next edge on a vertex");

    ItemID l_ret;
    if(m_flags & ITEM_IS_SIMPLE) l_ret = p.e.s.m_e_next;
    else l_ret = p.e.f.m_e_next;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// reciprocal (full edge only)
/**
 * @brief set ItemID of reciprocal edge (full edges only)
 * @param p_reciprocal the ItemID
 */
void M1Store::Item_lv0::setReciprocal_lv0(const M1Store::ItemID p_reciprocal_item_id){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting reciprocal edge to 0x%1").arg(p_reciprocal_item_id, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_NATURE_MASK) == 0,
               "Item::setReciprocal()",
               "accessing the reciprocal of an item that is not a full edge");

    p.e.f.m_e_reciprocal = p_reciprocal_item_id;

    M1_FUNC_EXIT
}
/**
 * @brief get ItemID of reciprocal edge (full edges only)
 * @return the ItemID value
 */
M1Store::ItemID M1Store::Item_lv0::reciprocal_item_id() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting reciprocal edge ..."))
    Q_ASSERT_X((m_flags & ITEM_NATURE_MASK) == 0,
               "Item::reciprocal()",
               "accessing the reciprocal of an item that is not a full edge");

    ItemID l_ret = p.e.f.m_e_reciprocal;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// first edge (not for simple items)
/**
 * @brief set first edge (not for simple items)
 * @param p_first_edge the ItemID value
 */
void M1Store::Item_lv0::setFirstEdge_lv0(const M1Store::ItemID p_first_edge_item_id){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting first edge to 0x%1").arg(p_first_edge_item_id, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::setFirstEdge()", "accessing the first edge on a non-full item");

    if(m_flags & ITEM_IS_VERTEX) p.v.f.m_first_edge = p_first_edge_item_id;
    else p.e.f.m_first_edge = p_first_edge_item_id;

    M1_FUNC_EXIT
}
/**
 * @brief get first edge (not for simple items)
 * @return the ItemID value
 */
M1Store::ItemID M1Store::Item_lv0::firstEdge_item_id() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting first edge ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::FirstEdge()", "accessing the first edge on a non-full item");

    ItemID l_ret;
    if(m_flags & ITEM_IS_VERTEX) l_ret = p.v.f.m_first_edge;
    else l_ret = p.e.f.m_first_edge;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// auto edge (not for simple items)
/**
 * @brief set auto edge (not for simple items)
 * @param p_auto_edge the ItemID value
 */
void M1Store::Item_lv0::setAutoEdge_lv0(const ItemID p_auto_edge){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting auto edge to 0x%1").arg(p_auto_edge, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_NATURE_MASK) == FULL_VERTEX, "Item::setAutoEdge()", "accessing the auto edge on a non-full vertex");

    p.v.f.m_auto_edge = p_auto_edge;

    M1_FUNC_EXIT
}
/**
 * @brief get auto edge (not for simple items)
 * @return the ItemID value
 */
M1Store::ItemID M1Store::Item_lv0::autoEdge_item_id() const{
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting auto edge ..."))
    Q_ASSERT_X((m_flags & ITEM_NATURE_MASK) == FULL_VERTEX, "Item::autoEdge_item_id()", "accessing the auto edge on a non-full vertex");

    ItemID l_ret;
    l_ret = p.v.f.m_auto_edge;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// first special edge (not for simples)
/**
 * @brief set first special edge (only for full items)
 * @param p_first_edge_special the ItemID value
 */
void M1Store::Item_lv0::setFirstEdgeSpecial_lv0(const M1Store::ItemID p_first_edge_special_item_id){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting first special edge to 0x%1").arg(p_first_edge_special_item_id, 16, 16, QLatin1Char('0')))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::setFirstEdgeSpecial()", "accessing the special first edge on a non-full item");

    if(m_flags & ITEM_IS_VERTEX) p.v.f.m_first_edge_special = p_first_edge_special_item_id;
    else p.e.f.m_first_edge_special = p_first_edge_special_item_id;

    M1_FUNC_EXIT
}
/**
 * @brief get first special edge (only for full items)
 * @return the ItemID value
 */
M1Store::ItemID M1Store::Item_lv0::firstEdgeSpecial_item_id() const {
    M1_FUNC_ENTRY(g_cat_store, QString("Getting first special edge ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::firstEdgeSpecial()", "accessing the special first edge on a non-full item");

    ItemID l_ret;
    if(m_flags & ITEM_IS_VERTEX) l_ret = p.v.f.m_first_edge_special;
    else l_ret = p.e.f.m_first_edge_special;
    qCDebug(g_cat_lv0_members) << QString("--> 0x%1 %2").arg(l_ret, 16, 16, QLatin1Char('0')).arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// creation date (not for simples)
/**
 * @brief set creation date (for full items)
 * @param p_date the date value as a QDateTime
 */
void M1Store::Item_lv0::setCreationDate_lv0(const QDateTime& p_date){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting creation date to %1").arg(p_date.toString("dd/MM/yyyy hh:mm:ss")))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::setCreationDate()", "accessing the creation date on a non-full item");

    if(m_flags & ITEM_IS_VERTEX) p.v.f.m_creation_date = p_date.currentMSecsSinceEpoch();
    else p.e.f.m_creation_date = p_date.currentMSecsSinceEpoch();

    M1_FUNC_EXIT
}
/**
 * @brief get creation date (for full items)
 * @return the date value as a QDateTime
 */
QDateTime M1Store::Item_lv0::creationDate() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting creation date ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::creationDate()", "accessing the creation date on a non-full item");

    QDateTime l_ret;
    if(m_flags & ITEM_IS_VERTEX) l_ret = QDateTime::fromMSecsSinceEpoch(p.v.f.m_creation_date);
    else l_ret = QDateTime::fromMSecsSinceEpoch(p.e.f.m_creation_date);
    qCDebug(g_cat_lv0_members) << QString("--> %1").arg(l_ret.toString("dd/MM/yyyy hh:mm:ss"));

    M1_FUNC_EXIT
        return l_ret;
}

// ---------------------------------------------------------------------------------------------------------
// last modification date (not for simples)
/**
 * @brief set Last modification date (for full items)
 * @param p_date the date value as a QDateTime
 */
void M1Store::Item_lv0::setLastmodDate_lv0(const QDateTime& p_date){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting last mod date to %1").arg(p_date.toString("dd/MM/yyyy hh:mm:ss")))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::setLastmodDate()", "accessing the lastmod date on a non-full item");

    if(m_flags & ITEM_IS_VERTEX) p.v.f.m_lastmod_date = p_date.currentMSecsSinceEpoch();
    else p.e.f.m_lastmod_date = p_date.currentMSecsSinceEpoch();

    M1_FUNC_EXIT
}
/**
 * @brief get Last modification date (for full items)
 * @return the date value as a QDateTime
 */
QDateTime M1Store::Item_lv0::lastmodDate() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting lastmod date ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item_lv0::lastmodDate()", "accessing the lastmod date on a non-full item");

    QDateTime l_ret;
    if(m_flags & ITEM_IS_VERTEX) l_ret = QDateTime::fromMSecsSinceEpoch(p.v.f.m_lastmod_date);
    else l_ret = QDateTime::fromMSecsSinceEpoch(p.e.f.m_lastmod_date);
    qCDebug(g_cat_lv0_members) << QString("--> %1").arg(l_ret.toString("dd/MM/yyyy hh:mm:ss"));

    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief getting the string ID (for Debug purposes)
 * @return the string ID
 */
M1Env::StringID M1Store::Item_lv0::string_id() const{
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting string id ..."))
    Q_ASSERT_X((m_flags & ITEM_NATURE_MASK) == FULL_VERTEX, "Item_lv0::string_id()", "accessing the string id on a non-full vertex");
    StringID l_ret = p.v.f.m_string_id;
    M1_FUNC_EXIT
        return l_ret;
}
// ---------------------------------------------------------------------------------------------------------
// incoming edges count (not for simples)
/**
 * @brief set the incoming edge count (for full items only)
 * @param p_incoming_edges the count value
 */
void M1Store::Item_lv0::setIncomingEdges_lv0(M1Store::ItemCounter p_incoming_edges){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting incoming edges count to %1").arg(p_incoming_edges))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::setIncomingEdges()", "accessing the incoming edges counter on a non-full item");

    if(m_flags & ITEM_IS_VERTEX) p.v.f.m_incoming_edges = p_incoming_edges;
    else p.e.f.m_incoming_edges = p_incoming_edges;

    M1_FUNC_EXIT
}
/**
 * @brief get the incoming edge count (for full items only)
 * @return the count value
 */
M1Store::ItemCounter M1Store::Item_lv0::incomingEdges() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("Getting incoming edges count ..."))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::incomingEdges()", "accessing the incoming edges counter on a non-full item");

    ItemCounter l_ret;
    if(m_flags & ITEM_IS_VERTEX) l_ret = p.v.f.m_incoming_edges;
    else l_ret = p.e.f.m_incoming_edges;
    qCDebug(g_cat_lv0_members) << QString("--> %1").arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}
/**
 * @brief increment incoming edge counter (only for full items)
 * @param p_add the increment
 */
void M1Store::Item_lv0::addIncomingEdges(M1Store::ItemCounter p_add){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("incrementing incoming edges by %1").arg(p_add))
    Q_ASSERT_X((m_flags & ITEM_IS_SIMPLE) == 0, "Item::addIncomingEdges()", "accessing the incoming edges counter on a non-full item");

    if(m_flags & ITEM_IS_VERTEX) p.v.f.m_incoming_edges += p_add;
    else p.e.f.m_incoming_edges += p_add;

    M1_FUNC_EXIT
}


//**************************************************************************************************************************
/** \defgroup TextLv0 Text setting and getting
 *  \ingroup LV0
 *
 *  Handles storing text in the local string member (m_text) or in the string table if too long for that
 *  @{
 */

/**
 * @brief set the text value of the item (valid for all Item natures)
 * @param p_text the text (char *). Must be below max length for edges and simple vertex (-1 to accomodate the \0 terminator)
 */
void M1Store::Item_lv0::setText_lv0(const QString& p_text){
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("setting text to [%1] (Utf8 len = %2)").arg(p_text).arg(p_text.toUtf8().length()))
    Q_ASSERT_X(((m_flags & ITEM_NATURE_MASK) == FULL_EDGE) ? (p_text.toUtf8().length() <= FULL_EDGE_TEXT_LEN-1) : true,
               "Item::setText()", "full edge --> length must be < FULL_EDGE_TEXT_LEN");
    // Q_ASSERT_X(((m_flags & ITEM_NATURE_MASK) == SIMPLE_EDGE) ? (p_text.toUtf8().length() <= SIMPLE_EDGE_TEXT_LEN-1) : true,
    //            "Item::setText()", "simple edge --> length must be < SIMPLE_EDGE_TEXT_LEN");
    Q_ASSERT_X(((m_flags & ITEM_NATURE_MASK) == SIMPLE_VERTEX) ? (p_text.toUtf8().length() <= SIMPLE_VERTEX_TEXT_LEN-1) : true,
               "Item::setText()", "simple vertex --> length must be < SIMPLE_VERTEX_TEXT_LEN");
    // for full vertices, the length can be arbitrary (goes into string table)

    if((m_flags & ITEM_NATURE_MASK) == FULL_VERTEX){
        // full vertex =============================
        if( p.v.f.m_string_id != G_VOID_ITEM_ID){
            // in all cases free string id if there is one
            M1Store::Storage::freeString(p.v.f.m_string_id);
            p.v.f.m_string_id = G_VOID_ITEM_ID;
        }

        if(p_text.length() == 0){ // 0 length
            // unset ITEM_HAS_LOCAL_STRING
            m_flags = m_flags & (~ITEM_HAS_LOCAL_STRING);
            // truncate local string
            p.v.f.m_text[0] = 0;
        }
        else
            if(p_text.toUtf8().length() <= FULL_VERTEX_TEXT_LEN-1){ // 0 < length < FULL_VERTEX_TEXT_LEN-1
                qCDebug(g_cat_lv0_members) << QString("storing into local string");
                strncpy(p.v.f.m_text, p_text.toUtf8().data(), FULL_VERTEX_TEXT_LEN); // strncpy() padds with \0
                p.v.f.m_text[m1_min(FULL_VERTEX_TEXT_LEN-1, p_text.toUtf8().length())] = 0; // to make sure
                // set ITEM_HAS_LOCAL_STRING
                m_flags = m_flags | ITEM_HAS_LOCAL_STRING;
            }
            else{ // length over FULL_VERTEX_TEXT_LEN-1
                qCDebug(g_cat_lv0_members) << QString("storing into LMDB string DB");
                // unset ITEM_HAS_LOCAL_STRING
                m_flags = m_flags & (~ITEM_HAS_LOCAL_STRING);
                // stores the string in the LMDB string db
                p.v.f.m_string_id = M1Store::Storage::storeString(p_text);
            }
    }
    else
        if((m_flags & ITEM_NATURE_MASK) == SIMPLE_EDGE){
            // simple edge =============================
            if( p.e.s.m_string_id != G_VOID_ITEM_ID){
                // in all cases free string id if there is one
                M1Store::Storage::freeString(p.e.s.m_string_id);
                p.e.s.m_string_id = G_VOID_ITEM_ID;
            }

            if(p_text.toUtf8().length() <= SIMPLE_EDGE_TEXT_LEN-1){ // 0 < length < SIMPLE_EDGE_TEXT_LEN-1
                qCDebug(g_cat_lv0_members) << QString("storing into local string");
                strncpy(p.e.s.m_text, p_text.toUtf8().data(), SIMPLE_EDGE_TEXT_LEN); // strncpy() padds with \0
                p.e.s.m_text[m1_min(SIMPLE_EDGE_TEXT_LEN-1, p_text.toUtf8().length())] = 0; // to make sure
                // set ITEM_HAS_LOCAL_STRING
                m_flags = m_flags | ITEM_HAS_LOCAL_STRING;
            }
            else{ // length over FULL_VERTEX_TEXT_LEN-1
                qCDebug(g_cat_lv0_members) << QString("storing into LMDB string DB");
                // unset ITEM_HAS_LOCAL_STRING
                m_flags = m_flags & (~ITEM_HAS_LOCAL_STRING);
                // stores the string in the LMDB string db
                p.e.s.m_string_id = M1Store::Storage::storeString(p_text);
            }
        }
        else{
            // not full vertex nor simple edge ===========
            if((m_flags & ITEM_NATURE_MASK) == FULL_EDGE){
                // full edge
                strncpy(p.e.f.m_text, p_text.toUtf8().data(), FULL_EDGE_TEXT_LEN); // strncpy() padds with \0
                p.e.f.m_text[m1_min(FULL_EDGE_TEXT_LEN-1, p_text.toUtf8().length())] = 0; // to make sure
            }
            else{ // if((m_flags & ITEM_NATURE_MASK) == SIMPLE_VERTEX){
                // simple vertex (only choice left)
                strncpy(p.v.s.m_text, p_text.toUtf8().data(), SIMPLE_VERTEX_TEXT_LEN);  // strncpy() padds with \0
                p.v.s.m_text[m1_min(SIMPLE_VERTEX_TEXT_LEN-1, p_text.toUtf8().length())] = 0; // to make sure
            }

            if(p_text.length() > 0) // set ITEM_HAS_LOCAL_STRING if string length > 0. Unset it otherwise
                m_flags = m_flags | ITEM_HAS_LOCAL_STRING;
            else
                m_flags = m_flags & (~ITEM_HAS_LOCAL_STRING);
        }

    qCDebug(g_cat_lv0_members) << QString("String Stored: %1").arg(text());
    M1_FUNC_EXIT
}
/**
 * @brief get item text
 * @return text, as a char*
 *
 * Both full vertices and simple edges can have m_string_id. Otheres have only their local m_text
 */
char* M1Store::Item_lv0::text() const {
    M1_FUNC_ENTRY(g_cat_lv0_members, QString("getting text ..."))
    // no condition. If no text available, return ""

    static char l_empty_string[1] = "";

    char* l_ret = nullptr;
    if((m_flags & ITEM_NATURE_MASK) == FULL_EDGE)
        // full edge
        l_ret = (char *)p.e.f.m_text;
    else if((m_flags & ITEM_NATURE_MASK) == SIMPLE_VERTEX)
        // simple vertex
        l_ret = (char *)p.v.s.m_text;
    else if((m_flags & ITEM_NATURE_MASK) == SIMPLE_EDGE)
        // simple edge
        if((m_flags & ITEM_HAS_LOCAL_STRING) > 0) l_ret = (char *)p.e.s.m_text;
        else
            if(p.e.s.m_string_id == G_VOID_ITEM_ID) l_ret = l_empty_string;
            else l_ret = M1Store::Storage::retrieveString(p.e.s.m_string_id);
    else {
        // only possibility here: full vertex
        if((m_flags & ITEM_HAS_LOCAL_STRING) > 0) l_ret = (char *)p.v.f.m_text;
        else
            if(p.v.f.m_string_id == G_VOID_ITEM_ID) l_ret = l_empty_string;
            else l_ret = M1Store::Storage::retrieveString(p.v.f.m_string_id);
    }
    qCDebug(g_cat_lv0_members) << QString("--> [%1]").arg(l_ret);

    M1_FUNC_EXIT
        return l_ret;
}
/** @} end group TextLv0 */
//**************************************************************************************************************************

