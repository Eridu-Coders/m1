#ifndef M1B_LV2_ITERATORS_H
#define M1B_LV2_ITERATORS_H

#include "m1B_lv2_item.h"

namespace M1Store{

/**
 * \defgroup ITER Iterators
 * \ingroup LV2
 *
 * Iterators provide access to sequences of eges, generally belonging to a given item, but not necessarily. All that is required is that the edges be
 * connected through their previous/next pointes and form a ring.
 *
 * Iterators in m1 are used through a wrapper class (M1Store::Item_lv2_iterator) which guarantees automatic deletion when the iterator variable goes
 * out of scope (see the description of the above-mentioned class for more details).
 *
 * The actual iterators are instances of M1Store::Item_lv2_iterator_base and its derived classes, which are protected (and deleted) by the wrapper class.
 *
 * For a typical example of use, see ck_html_fragment() in m1C_interp.cpp
 * @{
 */

/**
 * @brief Base class --> scans through ALL edges (no filter)
 *
 * Scans only full edges. simple edges are ignored, but not full edges leading to simple vertices
 *
 * Almost all work is done within this base class. The only significant virtual member is validEdge(), which returns true/false according
 * to the criteria of the particular subclass. In the base class, it returns true for all full edges.
 *
 * The constructors are private because they should only be called from M1Store::Item_lv2::getIteratorGeneric() (Item_lv2 is a friend class)
 *
 * **CAUTION**: because of a C++ quirk (virtual methods should not be called from the constructor), constructors are not able to perform the
 * full list of tasks necessary for complete instance initialization. As a result, a separate init() method is provided, which must be called after the constructor
 * completes. This is normally done in M1Store::Item_lv2::getIteratorGeneric(), and **SHOULD NOT** be done elsewhere.
 */
class Item_lv2_iterator_base{
    friend class Item_lv2;
    friend class Item_lv2_iterator_edge_type;
    friend class Item_lv2_iterator_et_type;
private:
    Item_lv2* m_current_edge = nullptr;
    ItemID m_first_edge_item_id = G_VOID_ITEM_ID;
    bool m_on_first = true;

    Item_lv2_iterator_base();
    Item_lv2_iterator_base(const Item_lv2_iterator_base& p_copiand);
    Item_lv2_iterator_base(Item_lv2* p_start_edge);
    void next_private(bool p_stay_first);
protected:
    virtual bool validEdge();

    virtual bool isBase(){return true;}
    virtual bool isEdgeType(){return false;}
    virtual bool isETType(){return false;}
public:
    bool isNull();

    void init();
    void next();
    bool beyondEnd() const;
    Item_lv2* at() const;

    virtual QString dbgShort();
};

/**
 * @brief Edge type filter iterator --> scans only thoses edges that match the filter
 *
 * Here, validEdge() returns true only for full edges of type m_edge_type
 */
class Item_lv2_iterator_edge_type : public Item_lv2_iterator_base{
    friend class Item_lv2;
    friend class Item_lv2_iterator_et_type;
private:
    SpecialItemID m_edge_type = G_VOID_SI_ID;

    Item_lv2_iterator_edge_type();
    Item_lv2_iterator_edge_type(const Item_lv2_iterator_edge_type& p_copiand);
    Item_lv2_iterator_edge_type(Item_lv2* p_start_edge, const SpecialItemID p_edge_type);
protected:
    virtual bool validEdge() override;

    virtual bool isBase() override {return false;}
    virtual bool isEdgeType() override {return true;}
    virtual bool isETType() override {return false;}
public:
    virtual QString dbgShort() override;
};

/**
 * @brief Edge and target type filter iterator --> scans only thoses edges that match the filter
 *
 * Here, validEdge() returns true only for full edges of type m_edge_type whose targets are of type m_target_type
 */
class Item_lv2_iterator_et_type : public Item_lv2_iterator_edge_type{
    friend class Item_lv2;
private:
    SpecialItemID m_target_type = G_VOID_SI_ID;

    Item_lv2_iterator_et_type();
    Item_lv2_iterator_et_type(const Item_lv2_iterator_et_type& p_copiand);
    Item_lv2_iterator_et_type(Item_lv2* p_start_edge, const SpecialItemID p_edge_type, const SpecialItemID p_target_type);
protected:
    virtual bool validEdge() override;

    virtual bool isBase() override {return false;}
    virtual bool isEdgeType() override {return false;}
    virtual bool isETType() override {return true;}
public:
    virtual QString dbgShort() override;
};

/**
 * @brief Wrapper class for Item_lv2_iterator_base and derived classes (automatic out of scope deletion)
 *
 * This wrapper is needed to make sure that the iterator is deleted when it becomes out of scope (i.e. generally when the loop that created it finishes).
 * If a bare pointer to the Item_lv2_iterator_base class was used, it would need an explicit delete instruction when it is no longer needed.
 *
 * Most methods just pass on the call to the embedded Item_lv2_iterator_base in m_it
 *
 * The use of a std::shared_ptr guarantees that the last wrapper owning that particulae iterator (there may be several, as a result of assignments and returns)
 * goes out of scope and is thus deleted.
 */
class Item_lv2_iterator{
    friend class Item_lv2;
private:
    std::shared_ptr<Item_lv2_iterator_base> m_it;

    Item_lv2_iterator(){}
    Item_lv2_iterator(const Item_lv2_iterator& p_copiand);
    Item_lv2_iterator(std::shared_ptr<Item_lv2_iterator_base> p_it);
public:
    ~Item_lv2_iterator();
    Item_lv2_iterator& operator=(const Item_lv2_iterator& p_assignand);

    bool isNull() const {return m_it->isNull();}
    void next(){m_it->next();}
    bool beyondEnd() const {return m_it->beyondEnd();}
    Item_lv2* at() const {return m_it->at();}
};
/**@}*/ //end of ITER

} // end namespace M1Store
#endif // M1B_LV2_ITERATORS_H
