#ifndef M1_LV0_ITEM_H
#define M1_LV0_ITEM_H

#include <QDateTime>
#include <QLoggingCategory>

#include "m1_constants.h"

Q_DECLARE_LOGGING_CATEGORY(g_cat_store)

namespace M1Store{

    // ----------------------------------------- ItemType ----------------------------------------------------
    /**
     * @brief Union struct for Item types. Either 4 16-bit short types or an regular ItemID
     */
    class ItemType{
        friend class Item_lv0;
    private:
        union {
            // self explanatory
            ItemID m_type_item;
            SpecialItemID m_type_short[4];
        } t;

        /**
         * @brief initSpecials initialize as 4 void special types
         */
        void initSpecials(){
            t.m_type_short[0] = G_VOID_TYPE_ID;
            t.m_type_short[1] = G_VOID_TYPE_ID;
            t.m_type_short[2] = G_VOID_TYPE_ID;
            t.m_type_short[3] = G_VOID_TYPE_ID;
        }
        /**
         * @brief initItem initialize as 1 Item type
         */
        void initItem(){t.m_type_item = G_VOID_ID;}
    public:
        /**
         * @brief ItemType Constructor 0 (Empty constructor) --> init as 4 void special types
         */
        ItemType(){initSpecials();}

        /**
         * @brief ItemType Constructor 1 (copy constructor)
         * @param p_type ref to another ItemType instance
         */
        ItemType(const ItemType& p_type_item_id){ t.m_type_item = p_type_item_id.t.m_type_item;}

        /**
         * @brief Constructor 2 - set all 4 short types at once
         * @param p_0 4 types
         * @param p_1 4 types
         * @param p_2 4 types
         * @param p_3 4 types
         */
        ItemType(SpecialItemID p_0, SpecialItemID p_1, SpecialItemID p_2, SpecialItemID p_3) {
            t.m_type_short[0] = p_0;
            t.m_type_short[1] = p_1;
            t.m_type_short[2] = p_2;
            t.m_type_short[3] = p_3;
        }

        /**
         * @brief Constructor 3 - set the ItemID type
         * @param p
         */
        ItemType(ItemID p_item_id) { t.m_type_item = p_item_id; }

        /**
         * @brief operator = overloading
         * @param p_type ref to another ItemType instance
         */
        void operator=(const ItemType& p_type_class){ t.m_type_item = p_type_class.t.m_type_item;}

        /**
         * @brief Setter 1 - 1 ItemID type
         * @param p_id
         */
        void setItemIDType(ItemID p_item_id) { t.m_type_item = p_item_id;}

        /**
         * @brief getter 1 - as an ItemID
         * @return
         */
        ItemID getItemIDType() { return t.m_type_item; }

        void setSpecialType(unsigned int p_index, SpecialItemID p_si_id);
        SpecialItemID getSpecialType(unsigned int p_index);
        QString dbgString() const;
    }__attribute__((__packed__));

    // ----------------------------------------- Item ----------------------------------------------------

    /**
     * @brief The Item class - vertices and edges - 128 bytes long
     */
    class Item_lv0{
        friend class Storage;
        friend class ItemWrapper;
        friend class ItemWrapperFullVertex;
        friend class ItemWrapperSimpleVertex;
        friend class ItemWrapperFullEdge;
        friend class ItemWrapperSimpleEdge;
    private:
        ItemID m_id;        ///< Item ID
        FlagField m_flags;  ///< (1) Primary flag field
        ItemType m_type;    ///< (2)Item type

        // union for the 4 cases
        union{
            union{
                struct{
                    StringID m_search_string_id;                ///< ID for the sarch string
                    char m_text[SIMPLE_VERTEX_TEXT_LEN];        ///< Local string space
                }s; ///< simple vertex
                struct{
                    FlagField m_flags_extra;                    ///< (3) Extra flag field
                    Date m_creation_date;                       ///< (4) Creation date
                    Date m_lastmod_date;                        ///< (5) Last Modification date
                    ItemCounter m_incoming_edges;               ///< (6) Number of incoming edges
                    ItemCounter m_visible_edges;                ///< (7) Number of visible edges
                    ItemID m_first_edge;                        ///< (8) ID of first edge
                    ItemID m_first_edge_special;                ///< (9) ID of first special edge
                    StringID m_string_id;                       ///< (10) ID of string, if any
                    StringID m_search_string_id;                ///< (11) ID for the sarch string
                    char m_text[FULL_VERTEX_TEXT_LEN];          ///< (12) Local string space
                }f; ///< full vertex
            }v; ///< vertices
            union{
                struct{
                    StringID m_search_string_id;                ///< ID for the sarch string
                    ItemID m_v_origin;                          ///< origin
                    ItemID m_e_previous;                        ///< previou edge
                    ItemID m_e_next;                            ///< next edge
                    char m_text[SIMPLE_EDGE_TEXT_LEN];          ///< Local string space
                }s; ///< simple edge
                struct{
                    FlagField m_flags_extra;                    ///< Extra flag field
                    ItemID m_v_origin;                          ///< origin
                    ItemID m_v_target;                          ///< target
                    ItemID m_e_previous;                        ///< previous edge
                    ItemID m_e_next;                            ///< next edge
                    ItemID m_e_reciprocal;                      ///< ID of reciprocal edge, if any
                    Date m_creation_date;                       ///< Creation date
                    Date m_lastmod_date;                        ///< Last Modification date
                    ItemCounter m_incoming_edges;               ///< Number of incoming edges
                    ItemID m_first_edge;                        ///< ID of first edge
                    ItemID m_first_edge_special;                ///< ID of first special edge
                    char m_text[FULL_EDGE_TEXT_LEN];            ///< Local string space
                }f; ///< full edge
            }e; ///< edges
        }p; ///< rest of the payload

        // number of bits == 1 in a flag field (for set/unset flags)
        unsigned short count_1_bits(const FlagField p_flag);
        // all methods documented in m1_store.cpp
    public:
        // Does nothing. The class is not supposed to be constructed but cast from an mmap() area pointer
        Item_lv0(){}
        // For testing purposes only
        Item_lv0(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);

        // to be used in real situations when instantiating from mmap() area pointer
        void initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);
        void initializeMembers();

        ItemID id();

        void setFlags(const FlagField p_flags, bool p_force_init = false);
        void setFlag(const FlagField p_flag, bool p_force_init = false);
        void unSetFlag(const FlagField p_flag, bool p_force_init = false);
        FlagField flags();

        void setType(const unsigned int p_index, const SpecialItemID p_type);
        SpecialItemID getType(const unsigned int p_index);
        ItemID getType();
        void setType(const ItemID p_type);
        bool isOfType(const ItemID p_type);
        bool isOfType(const SpecialItemID p_type);

        void setFlagsExtra(const FlagField p_flags);
        void setFlagExtra(const FlagField p_flag);
        void unSetFlagExtra(const FlagField p_flag);
        FlagField flagsExtra();

        void setOrigin(const ItemID p_origin);
        ItemID origin();
        void setTarget(const ItemID p_target);
        ItemID target();
        void setPrevious(const ItemID p_previous);
        ItemID previous();
        void setNext(const ItemID p_next);
        ItemID next();
        void setReciprocal(const ItemID p_reciprocal);
        ItemID reciprocal();
        void setFirstEdge(const ItemID p_first_edge);
        ItemID firstEdge();
        void setFirstEdgeSpecial(const ItemID p_first_edge_special);
        ItemID firstEdgeSpecial();

        void setCreationDate(const QDateTime& p_date);
        QDateTime creationDate();
        void setLastmodDate(const QDateTime& p_date);
        QDateTime lastmodDate();

        void setIncomingEdges(ItemCounter p_incoming_edges);
        ItemCounter incomingEdges();
        void addIncomingEdges(ItemCounter p_add); // can be negative

        void setVisibleEdges(ItemCounter p_visible_edges);
        ItemCounter visibleEdges();
        void addVisibleEdges(ItemCounter p_add); // can be negative

        void setText(const QString& p_text);
        char* text();
    };

    // ----------------------------------------- SpecialItem ----------------------------------------------------

    /**
     * @brief The Special Item class - Quick access for types, anchor nodes, etc - 32 bytes long
     */
    class SpecialItem{
    private:
        FlagField m_flags;      ///< [8] flags
        ItemID m_item_id;       ///< [8] ItemID of corresponding item
        SpecialItemID m_id;     ///< [2] 16 bit special item id
        char m_mnemonic[5];     ///< [5] menmonic
        /// [2] 16 bit special item id of the reciprocal type (for edge types only)
        SpecialItemID m_id_reciprocal;
        char m_extra[7];        ///< [7] extra data storage field (len = 7 to make total 32)
    public:
        SpecialItem(){}
        ~SpecialItem(){}

        void setAttr(
            const ItemID p_item_id, const SpecialItemID p_id, const SpecialItemID p_reciprocal,
            const FlagField p_flags, const char* p_mnemonic);

        void setReciprocal(SpecialItemID p){m_id_reciprocal = p;}

        SpecialItemID getSpecialId() const {return m_id;}
        SpecialItemID reciprocalId() const {return m_id_reciprocal;}
        ItemID itemId() const {return m_item_id;}
        FlagField flags() const {return m_flags;}
        QString mnemonic() const;
        const char* mnemonic_raw() const {return m_mnemonic;}

        QString dbgString();
    };

} // end namespace M1Store

#endif // M1_LV0_ITEM_H
