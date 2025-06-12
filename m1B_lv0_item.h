#ifndef M1_LV0_ITEM_H
#define M1_LV0_ITEM_H

#include <QDateTime>
#include <QLoggingCategory>

#include "m1A_constants.h"

namespace M1Store{

using namespace M1Env;
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
            t.m_type_short[0] = G_VOID_SI_ID;
            t.m_type_short[1] = G_VOID_SI_ID;
            t.m_type_short[2] = G_VOID_SI_ID;
            t.m_type_short[3] = G_VOID_SI_ID;
        }
        /**
         * @brief initItem initialize as 1 Item type
         */
        void initItem(){t.m_type_item = G_VOID_ITEM_ID;}
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
         * @brief Constructor 3 - set the ItemID type
         * @param p
         */
        ItemType(const ItemID p_item_id) { t.m_type_item = p_item_id; }

        /**
         * @brief operator = overloading
         * @param p_type ref to another ItemType instance
         */
        void operator=(const ItemType& p_type_class){ t.m_type_item = p_type_class.t.m_type_item;}

        /**
         * @brief Setter 1 - 1 ItemID type
         * @param p_id
         */
        void setItemIDType(const ItemID p_item_id) { t.m_type_item = p_item_id;}

        /**
         * @brief getter 1 - as an ItemID
         * @return
         */
        ItemID getItemIDType() const {return t.m_type_item; }

        ItemType(const SpecialItemID p_0, const SpecialItemID p_1, const SpecialItemID p_2, const SpecialItemID p_3);
        ItemType(const SpecialItemID p_0) : ItemType(p_0, G_VOID_SI_ID, G_VOID_SI_ID, G_VOID_SI_ID){}

        void setSpecialType(const unsigned int p_index, const SpecialItemID p_si_id);
        SpecialItemID getSpecialType(const unsigned int p_index) const;
        QString dbgString() const;
        QString dbgStringHr(const bool p_is_item_id = false) const;
    }__attribute__((__packed__));

    // ----------------------------------------- Item ----------------------------------------------------
    /**
     * @brief The Item class - vertices and edges - 128 bytes long
     */
    class Item_lv0{
        friend class Storage;
        friend class Item_lv2;
    private:
        ItemID m_item_id;   ///< (0) Item ID
        FlagField m_flags;  ///< (1) Primary flag field
        ItemType m_type;    ///< (2) Item type

        // union for the 4 cases
        union{
            union{
                struct{
                    StringID m_search_string_id;                ///< ID for the sarch string
                    char m_text[SIMPLE_VERTEX_TEXT_LEN];        ///< Local string space
                }s; ///< simple vertex
                struct{
                    FlagField m_flags_extra;                    ///< (3) Extra flag field
                    ItemCounter m_incoming_edges;               ///< (4) Number of incoming edges
                    Date m_creation_date;                       ///< (5) Creation date
                    Date m_lastmod_date;                        ///< (6) Last Modification date
                    ItemID m_first_edge;                        ///< (7) ID of first edge
                    ItemID m_first_edge_special;                ///< (8) ID of first special edge
                    char m_text[FULL_VERTEX_TEXT_LEN];          ///< (9) Local string space

                    ItemID m_auto_edge;                         ///< (10) ID of AUTO edge
                    StringID m_string_id;                       ///< (11) ID of string, if any
                    StringID m_search_string_id;                ///< (12) ID for the sarch string
                }f; ///< full vertex
            }v; ///< vertices
            union{
                struct{
                    StringID m_search_string_id;                ///< ID for the sarch string
                    ItemID m_v_origin;                          ///< origin
                    ItemID m_e_previous;                        ///< previous edge
                    ItemID m_e_next;                            ///< next edge
                    StringID m_string_id;                       ///< ID of string, if any
                    char m_text[SIMPLE_EDGE_TEXT_LEN];          ///< Local string space
                }s; ///< simple edge
                struct{
                    FlagField m_flags_extra;                    ///< (3) Extra flag field
                    ItemCounter m_incoming_edges;               ///< (4) Number of incoming edges
                    Date m_creation_date;                       ///< (5) Creation date
                    Date m_lastmod_date;                        ///< (6) Last Modification date
                    ItemID m_first_edge;                        ///< (7) ID of first edge
                    ItemID m_first_edge_special;                ///< (8) ID of first special edge
                    char m_text[FULL_EDGE_TEXT_LEN];            ///< (9) Local string space

                    ItemID m_v_origin;                          ///< origin
                    ItemID m_v_target;                          ///< target
                    ItemID m_e_previous;                        ///< previous edge
                    ItemID m_e_next;                            ///< next edge
                    ItemID m_e_reciprocal;                      ///< ID of reciprocal edge, if any
                }f; ///< full edge
            }e; ///< edges
        }p; ///< rest of the payload

        // number of bits == 1 in a flag field (for set/unset flags)
        static unsigned short count_1_bits(const FlagField p_flag);
        // all methods documented in m1_store.cpp
    public:
        // Does nothing. The class is not supposed to be constructed but cast from an mmap() area pointer
        Item_lv0(){}
        // For testing purposes only
        Item_lv0(const ItemID p_item_id,
                 const FlagField p_flags,
                 const ItemType& p_type);

        // ------------------------ Setters ---------------------------------------------------------
        // to be used in real situations when instantiating from mmap() area pointer
        void initializeMembers(const ItemID p_item_id,
                               const FlagField p_flags,
                               const ItemType& p_type);
        void initializeMembers();

        void setFlags(const FlagField p_flags, const bool p_force_init = false);
        void setFlag(const FlagField p_flag, const bool p_force_init = false);
        void unSetFlag(const FlagField p_flag, const bool p_force_init = false);

        void setType_member_si_id(const unsigned int p_index, const SpecialItemID p_type);
        void setType_member_item_id(const ItemID p_type_item_id);

        void setFlagsExtra(const FlagField p_flags);
        void setFlagExtra(const FlagField p_flag);
        void unSetFlagExtra(const FlagField p_flag);

        void setOrigin(const ItemID p_origin);
        void setTarget(const ItemID p_target);
        void setPrevious(const ItemID p_previous);
        void setNext(const ItemID p_next);
        void setReciprocal(const ItemID p_reciprocal);
        void setFirstEdge(const ItemID p_first_edge);
        void setAutoEdge(const ItemID p_auto_edge);
        void setFirstEdgeSpecial(const ItemID p_first_edge_special);
        void setCreationDate(const QDateTime& p_date);
        void setLastmodDate(const QDateTime& p_date);

        void setIncomingEdges(const ItemCounter p_incoming_edges);
        void addIncomingEdges(const ItemCounter p_add); // can be negative

        void setText(const QString& p_text);

        // ------------------------ Getters ---------------------------------------------------------
        ItemID item_id() const;
        FlagField flags() const;
        SpecialItemID getType_si_id(const unsigned int p_index) const;
        ItemID getType_item_id() const;
        FlagField flagsExtra() const;
        ItemID origin_item_id() const;
        ItemID target_item_id() const;
        ItemID previous_item_id() const;
        ItemID next_item_id() const;
        ItemID reciprocal_item_id() const;
        ItemID firstEdge_item_id() const;
        ItemID autoEdge_item_id() const;
        ItemID firstEdgeSpecial_item_id() const;
        QDateTime creationDate() const;
        QDateTime lastmodDate() const;
        StringID string_id() const;
        ItemCounter incomingEdges() const;
        char* text() const;

        bool isOfType_member(const ItemID p_type) const;
        bool isOfType_member(const SpecialItemID p_type) const;
        bool isOfType_member(const char* p_type) const;

        QString dbgString() const;
        QString dbgTypeShort() const;
        QString dbgShort() const;
        QString getTypeDbg_string(){return m_type.dbgString();}
    };

    // ----------------------------------------- SpecialItem ----------------------------------------------------

    /**
     * @brief The Special Item class - Quick access for types, anchor nodes, etc - 32 bytes long.
     * Instances of this class live in the special items mmap() area
     */
    class SpecialItem{
    private:
        FlagField m_flags;                  ///< [8] flags
        ItemID m_item_id;                   ///< [8] ItemID of corresponding item (= G_VOID_ID if none)
        SpecialItemID m_si_id;              ///< [2] 16 bit special item id
        char m_mnemonic[5];                 ///< [5] menmonic
        SpecialItemID m_si_id_reciprocal;   ///< [2] 16 bit special item id of the reciprocal type (for edge types only, SI_HAS_RECIPROCAL must be set)
        StringID m_icon_path;               ///< [4] path to icon (stored in the general string table
        char m_extra[3];                    ///< [3] extra data storage field (len = 3 to make total 32)
    public:
        static SpecialItem* cm_dummy;
        /**
         * @brief SpecialItem default constructor.
         * Never called normally bc class resides in mmap() area
         */
        SpecialItem(){}
        /**
         * @brief SpecialItem default destructor.
         * Never called normally bc class resides in mmap() area
         */
        ~SpecialItem(){}
        SpecialItem(const char* p_mnemo);

        void setAttr(const ItemID p_item_id, const SpecialItemID p_si_id,
                     const FlagField p_flags, const char* p_mnemonic);

        /// reciprocal ID setter
        void setReciprocal(const SpecialItemID p_si_id){m_si_id_reciprocal = p_si_id;}

        SpecialItemID specialId() const {return m_si_id;}               ///< special ID getter
        SpecialItemID reciprocalSpecialId() const {return m_si_id_reciprocal;} ///< reciprocal SI ID getter
        ItemID itemId() const {return m_item_id;}                       ///< ItemID getter
        FlagField flags() const {return m_flags;}                       ///< flags getter
        const char* mnemonic_raw() const {return m_mnemonic;}           ///< raw mnemonic pointer getter (!! NOT a \0 terminated string)

        QString mnemonic() const;
        QString dbgString() const;
    };

} // end namespace M1Store

    QDebug operator<<(QDebug d, M1Store::ItemType p);
    QDebug operator<<(QDebug d, M1Store::ItemType* p);

    QDebug operator<<(QDebug d, M1Store::Item_lv0 p);
    QDebug operator<<(QDebug d, M1Store::Item_lv0* p);

    QDebug operator<<(QDebug d, M1Store::SpecialItem p);
    QDebug operator<<(QDebug d, M1Store::SpecialItem* p);
#endif // M1_LV0_ITEM_H
