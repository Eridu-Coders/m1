#include "m1A_env.h"
#include "m1B_lv2_item.h"
#include "m1B_store.h"
#include "m1B_graph_init.h"

Q_LOGGING_CATEGORY(g_cat_lv2_constructors, "lv2.constructors")

/**********************************************************/
/** \defgroup I2Inst Item_lv2 Instantiation and destruction
 *  \ingroup LV2
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

    M1Store::Item_lv2* l_item_lv2 = static_cast<Item_lv2*>(M1Store::StorageStatic::getNewItemPointer_lv1(p_flags, p_type));
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
                                                M1Store::ItemType(M1Store::StorageStatic::getSpecialID("AUTO_"),
                                                                  G_VOID_SI_ID,
                                                                  G_VOID_SI_ID,
                                                                  G_VOID_SI_ID));
        // loops onto this vertex
        l_auto_edge->setTarget_lv1(item_id());
        l_auto_edge->setOrigin_lv1(item_id());

        // it is the first edge so it links to itself
        // !! no longer needed bc done by default below !!
        // l_auto_edge->setPrevious(l_auto_edge->item_id());
        // l_auto_edge->setNext(l_auto_edge->item_id());

        // attach this edge as the first ordinary one
        this->setFirstEdge_lv1(l_auto_edge->item_id());
        this->setAutoEdge_lv1(l_auto_edge->item_id());

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

/**
 * @brief close previous and next onto the item (edge) itself
 *
 * This is what begins the ring list structure of an item's edges
 */
void M1Store::Item_lv2::loopNextPrevious(){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("Loop back previous/next onto itself ID=[%1]").arg(item_id()))
    Q_ASSERT_X((flags() & ITEM_NATURE_MASK) == FULL_EDGE || (flags() & ITEM_NATURE_MASK) == SIMPLE_EDGE,
               "Item_lv2::loopNextPrevious()", "not edge (full or simple)");
    this->setPrevious_lv1(this->item_id());
    this->setNext_lv1(this->item_id());
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
                SpecialItem* l_type_si = M1Store::StorageStatic::getSpecialItemPointer(l_si_id);
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
    l_item_lv2->setText_lv1(p_label);

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
    M1Store::SpecialItem* l_special = M1Store::StorageStatic::getNewSpecialWithItem(l_item_lv2->item_id(), p_flags_special, p_mnemonic, p_icon_path);
    l_item_lv2->storeSpecialItemID(l_special->specialId());

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
        l_ret = static_cast<Item_lv2*>(M1Store::StorageStatic::getItemPointer_lv1(p_item_id));
        qCDebug(g_cat_lv2_constructors) << QString("Existing: %1").arg(l_ret->dbgShort());
    }
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief [Static] get an existing Item_lv2 from a special Item Id
 * @param p_si_id SpecialItemID
 * @return the Item_lv2* pointer corresponding to p_si_id
 */
M1Store::Item_lv2* M1Store::Item_lv2::getExisting(const SpecialItemID p_si_id){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("B) getExisting from ItemID: %1").arg(p_si_id));
    Item_lv2* l_ret = getExisting(M1Store::StorageStatic::getSpecialItemPointer(p_si_id)->itemId());
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief [Static] Get an Item_lv2* for an existing Item_lv2
 * @param p_mnemonic the mnemonic to return the Item_lv2* for
 * @return the corresponding Item_lv2*
 */
M1Store::Item_lv2* M1Store::Item_lv2::getExisting(const char* p_mnemonic){
    M1_FUNC_ENTRY(g_cat_lv2_constructors, QString("C) getExisting from mnemonic: %1").arg(p_mnemonic))

    Item_lv2* l_ret = getExisting(M1Store::StorageStatic::getSpecialItemPointer(p_mnemonic)->itemId());
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
/** @}*/ // end \defgroup I2Inst
