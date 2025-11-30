#ifndef M1_LV0_ITEM_H
#define M1_LV0_ITEM_H

#include <QDateTime>
#include <QLoggingCategory>

#include "m1A_constants.h"

namespace M1Store{

using namespace M1Env;
    // ----------------------------------------- ItemType ----------------------------------------------------
    /**
     * @brief Union struct for Item types. Either 4 16-bit short types or an regular (64-bit) ItemID
     *
     * \ingroup LV0
     */
    class ItemType{
        friend class Item_lv0;
    private:
        union {
            ItemID m_type_item;             ///< Single Item case
            SpecialItemID m_type_short[4];  ///< 4 Special Items case
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
         *
         * @param p_type ref to another ItemType instance
         */
        ItemType(const ItemType& p_type_item_id){ t.m_type_item = p_type_item_id.t.m_type_item;}

        /**
         * @brief Constructor 2 - set the ItemID type
         * @param p_item_id
         */
        ItemType(const ItemID p_item_id) { t.m_type_item = p_item_id; }

        ItemType(const SpecialItemID p_0, const SpecialItemID p_1, const SpecialItemID p_2, const SpecialItemID p_3);

        /**
         * @brief Constructor 4 - single SpecialItemID value - others set to G_VOID_SI_ID
         * @param p_0 SpecialItemID to be assigned to thes slot
         */
        ItemType(const SpecialItemID p_0) : ItemType(p_0, G_VOID_SI_ID, G_VOID_SI_ID, G_VOID_SI_ID){}

        /**
         * @brief operator = overloading
         *
         * @param p_type ref to another ItemType instance
         */
        void operator=(const ItemType& p_type_class){ t.m_type_item = p_type_class.t.m_type_item;}

        /**
         * @brief Setter 1 - 1 ItemID type
         * @param p_id The ItemID to be stored
         */
        void setItemIDType(const ItemID p_item_id) { t.m_type_item = p_item_id;}

        /**
         * @brief getter 1 - as an ItemID
         * @return The ItemID stored in this instance
         */
        ItemID getItemIDType() const {return t.m_type_item; }

        void setSpecialType(const unsigned int p_index, const SpecialItemID p_si_id);
        SpecialItemID getSpecialType(const unsigned int p_index) const;

        QString dbgString() const;
        QString dbgStringHr(const bool p_is_item_id = false) const;
    };

    // ----------------------------------------- Item ----------------------------------------------------
    /**
     * @brief The Item class - vertices and edges - 128 bytes long
     * \ingroup LV0
     *
     * Instances of this class live in the items mmap() area
     */
    class Item_lv0{
        friend class StorageStatic;
        friend class Item_lv2;
    private:
        /** \defgroup ShareMb Shared Members - present in all 4 kinds of Items
         * \ingroup LV0
         *
         * @{
         */
        ItemID m_item_id;   ///< (0) Item ID
        FlagField m_flags;  ///< (1) Primary flag field
        ItemType m_type;    ///< (2) Item type
        /**@}*/ //end of ShareMb

        /** \defgroup SpecMb Branch-specific Members - present in only one kind of member (= branch of the 4-way union)
         * \ingroup LV0
         *
         * @{
         */
        union{
            union{
                struct{
                    StringID m_search_string_id;                ///< (3) ID for the search string, if any (G_VOID_ITEM_ID otherwise)
                    char m_text[SIMPLE_VERTEX_TEXT_LEN];        ///< (4) Local string space
                }s; ///< Simple Vertex branch of the 4-way union
                struct{
                    FlagField m_flags_extra;                    ///< (3) Extra flag field
                    ItemCounter m_incoming_edges;               ///< (4) Number of incoming edges
                    Date m_creation_date;                       ///< (5) Creation date
                    Date m_lastmod_date;                        ///< (6) Last Modification date
                    ItemID m_first_edge;                        ///< (7) ID of first edge, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_first_edge_special;                ///< (8) ID of first special edge, if any (G_VOID_ITEM_ID otherwise)
                    char m_text[FULL_VERTEX_TEXT_LEN];          ///< (9) Local string space

                    ItemID m_auto_edge;                         ///< (10) ID of AUTO edge, if any (G_VOID_ITEM_ID otherwise)
                    StringID m_string_id;                       ///< (11) ID of string, if any (G_VOID_ITEM_ID otherwise)
                    StringID m_search_string_id;                ///< (12) ID for the search string, if any (G_VOID_ITEM_ID otherwise)
                }f; ///< Full Vertex branch of the 4-way union
            }v; ///< Simple/Full vertices branches
            union{
                struct{
                    StringID m_search_string_id;                ///< (3) ID for the sarch string
                    ItemID m_v_origin;                          ///< (4) origin, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_e_previous;                        ///< (5) previous edge, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_e_next;                            ///< (6) next edge, if any (G_VOID_ITEM_ID otherwise)
                    StringID m_string_id;                       ///< (7) ID of string, if any (G_VOID_ITEM_ID otherwise)
                    char m_text[SIMPLE_EDGE_TEXT_LEN];          ///< (8) Local string space
                }s; ///< Simple Edge branch of the 4-way union
                struct{
                    FlagField m_flags_extra;                    ///< (3) Extra flag field
                    ItemCounter m_incoming_edges;               ///< (4) Number of incoming edges
                    Date m_creation_date;                       ///< (5) Creation date
                    Date m_lastmod_date;                        ///< (6) Last Modification date
                    ItemID m_first_edge;                        ///< (7) ID of first edge, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_first_edge_special;                ///< (8) ID of first special edge, if any (G_VOID_ITEM_ID otherwise)
                    char m_text[FULL_EDGE_TEXT_LEN];            ///< (9) Local string space

                    ItemID m_v_origin;                          ///< (10) origin, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_v_target;                          ///< (11) target, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_e_previous;                        ///< (12) previous edge, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_e_next;                            ///< (13) next edge, if any (G_VOID_ITEM_ID otherwise)
                    ItemID m_e_reciprocal;                      ///< (14) ID of reciprocal edge, if any (G_VOID_ITEM_ID otherwise)
                }f; ///< Full Edge branch of the 4-way union
            }e; ///< Simple/Full Edges branches
        }p; ///< Rest of the payload (branch-specific)
        /**@}*/ //end of SpecMb
    public:
        // number of bits == 1 in a flag field (for set/unset flags)
        static unsigned short count_1_bits(const FlagField p_flag);

        /**
         * @brief Unused, except in tests
         *
         * Does nothing. The class is not supposed to be constructed but cast from an mmap() area pointer
         */
        Item_lv0(){}
        Item_lv0(const ItemID p_item_id,
                 const FlagField p_flags,
                 const ItemType& p_type);

        // ------------------------ Setters ---------------------------------------------------------
        // to be used in real situations when instantiating from an mmap() area pointer
        void initializeMembers_lv0(const ItemID p_item_id,
                               const FlagField p_flags,
                               const ItemType& p_type);
        void initializeMembers_lv0();

        void setType_member_lv0(const ItemType& p_type);
        void setFlags_lv0(const FlagField p_flags, const bool p_force_init = false);
        void setFlagsExtra_lv0(const FlagField p_flags);
        void setOrigin_lv0(const ItemID p_origin);
        void setTarget_lv0(const ItemID p_target);
        void setPrevious_lv0(const ItemID p_previous);
        void setNext_lv0(const ItemID p_next);
        void setReciprocal_lv0(const ItemID p_reciprocal);
        void setFirstEdge_lv0(const ItemID p_first_edge);
        void setAutoEdge_lv0(const ItemID p_auto_edge);
        void setFirstEdgeSpecial_lv0(const ItemID p_first_edge_special);
        void setCreationDate_lv0(const QDateTime& p_date);
        void setLastmodDate_lv0(const QDateTime& p_date);
        void setIncomingEdges_lv0(const ItemCounter p_incoming_edges);
        void setText_lv0(const QString& p_text);

        // void setType_member_si_id(const unsigned int p_index, const SpecialItemID p_type);
        // void setType_member_item_id(const ItemID p_type_item_id);
        // void setFlag(const FlagField p_flag, const bool p_force_init = false);
        // void unSetFlag(const FlagField p_flag, const bool p_force_init = false);
        // void setFlagExtra(const FlagField p_flag);
        // void unSetFlagExtra(const FlagField p_flag);
        // void storeSpecialItemID(const SpecialItemID p_si_id);
        // void addIncomingEdges(const ItemCounter p_add); // can be negative

        // ------------------------ Getters ---------------------------------------------------------
        ItemType& getType();
        SpecialItemID specialItemId();
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
     *
     * Instances of this class live in the special items mmap() area
     * \ingroup LV0
     *
     * @todo Remove m_si_id from SpecialItem and m_item_id from Item_lv0
     * @todo bring m_mnemonic to 6 chars, to add \0 at the end
     * @todo Add a string ID to SpecialItem, as a label
     */

    class SpecialItem{
    private:
        FlagField m_flags;                  ///< [8] flags
        ItemID m_item_id;                   ///< [8] ItemID of corresponding item (= G_VOID_ID if none)
        SpecialItemID m_si_id;              ///< [2] 16 bit special item id
        SpecialItemID m_si_id_reciprocal;   ///< [2] 16 bit special item id of the reciprocal type (for edge types only, SI_HAS_RECIPROCAL must be set)
        // StringID_16 m_icon_path;            ///< [2] path to icon (stored in the general string table)
        // StringID m_icon_path;               ///< [4] path to icon (stored in the general string table)
        char m_mnemonic[5];                 ///< [5] menmonic
        char m_extra[7];                    ///< [5] extra data storage field (len = 3 to make total 32)
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
        SpecialItemID reciprocalSpecialId() const {return m_si_id_reciprocal;}
                                                                        ///< reciprocal SI ID getter
        ItemID itemId() const {return m_item_id;}                       ///< ItemID getter
        FlagField flags() const {return m_flags;}                       ///< flags getter
        const char* mnemonic_raw() const {return m_mnemonic;}           ///< raw mnemonic pointer getter (!! NOT a \0 terminated string)

        QString mnemonic() const;
        QString dbgString() const;
    };

} // end namespace M1Store

/**
 * \defgroup OperatorOverload_lv0 << Operator overloading for debug purposes (Item_lv0)
 * @ingroup DebugLog
 * @{
 */
    QDebug operator<<(QDebug d, M1Store::ItemType p);
    QDebug operator<<(QDebug d, M1Store::ItemType* p);

    QDebug operator<<(QDebug d, M1Store::Item_lv0 p);
    QDebug operator<<(QDebug d, M1Store::Item_lv0* p);

    QDebug operator<<(QDebug d, M1Store::SpecialItem p);
    QDebug operator<<(QDebug d, M1Store::SpecialItem* p);
/**@}*/ //end of OperatorOverload_lv0
#endif // M1_LV0_ITEM_H
