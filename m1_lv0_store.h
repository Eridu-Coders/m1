#ifndef M1_STORE_H
#define M1_STORE_H

#include <lmdb.h>

#include <QDateTime>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(g_cat_store)

namespace M1Store{

    typedef unsigned long long FlagField;   ///< unsigned 64-bit wide flag field
    typedef unsigned long long ItemID;      ///< unsigned 64-bit wide ID number for both vertices and edges
    typedef unsigned long long StringID;    ///< unsigned 64-bit wide ID number for vertex strings
    typedef unsigned long long Date;        ///< unsigned 64-bit wide date (seconds since 1-1-1970)
    typedef long long ItemCounter;          ///< signed 64-bit wide counter (incoming edges, etc) -1 = unset
    typedef unsigned short SpecialItemID;   ///< unsigned 16-bit wide ID number for core type numbers, etc

    const ItemID G_VOID_ID = 0xffffffffffffffff; ///< -1 = NULL value
    const SpecialItemID G_VOID_TYPE_ID = 0xffff; ///< -1 = NULL value

    // ----------------------------------------- ItemType ----------------------------------------------------
    /**
     * @brief Union struct for Item types. Either 4 16-bit short types or an regular ItemID
     */
    class ItemType{
        friend class Item;
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
         * @brief ItemType Constructor 0 (copy constructor)
         * @param p_type ref to another ItemType instance
         */
        ItemType(const ItemType& p_type){ t.m_type_item = p_type.t.m_type_item;}
        /**
         * @brief operator = overloading
         * @param p_type ref to another ItemType instance
         */
        void operator=(const ItemType& p_type){ t.m_type_item = p_type.t.m_type_item;}
        /**
         * @brief Constructor 1 - set all 4 short types at once
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
         * @brief Constructor 2 - set the ItemID type
         * @param p
         */
        ItemType(ItemID p) { t.m_type_item = p; }
        /**
         * @brief Empty constructor --> init as 4 void special types
         */
        ItemType(){initSpecials();}
        /**
         * @brief Setter 1 - 1 ItemID type
         * @param p_id
         */
        void setItemIDType(ItemID p_id) { t.m_type_item = p_id;}
        /**
         * @brief Setter 2 - one of the 4 short type
         * @param p_index the number of the type to set (0 to 3)
         * @param p_id the value
         */
        void setSpecialType(unsigned int p_index, SpecialItemID p_id);

        /**
         * @brief getter 1 - as an ItemID
         * @return
         */
        ItemID getItemIDType() { return t.m_type_item; }
        /**
         * @brief getter 2 : One of the 4 short types
         * @param p_index the number of the type to get (0 to 3)
         * @return
         */
        SpecialItemID getSpecialType(unsigned int p_index);

        /**
         * @brief get a debug representation of this type
         * @return the debug string
         */
        QString dbgString();
    }__attribute__((__packed__));

    const unsigned long long ITEM_IS_VERTEX         = 0x0000000000000001; ///< 0 --> item is an edge, 1 --> item is a vertex
    const unsigned long long ITEM_IS_SIMPLE         = 0x0000000000000002; ///< 1 --> item is either a simple edge or simple vertex
    const unsigned long long ITEM_HAS_LOCAL_STRING  = 0x0000000000000004; ///< 1 --> the local m_text (if any) is used to store the string
    const unsigned long long TYPE_IS_ITEM_ID        = 0x0000000000000008; ///< 1 --> the type field is a single ItemID, 0 --> 4 SpecialItemIDs

    // composite flag values for item nature testing
    const unsigned long long FULL_VERTEX = ITEM_IS_VERTEX;
    const unsigned long long SIMPLE_VERTEX = ITEM_IS_VERTEX | ITEM_IS_SIMPLE;
    const unsigned long long FULL_EDGE = 0x0;
    const unsigned long long SIMPLE_EDGE = ITEM_IS_SIMPLE;
    const unsigned long long ITEM_NATURE_MASK = 0x0000000000000003;     ///< all bits to determine Item nature
    const unsigned long long ITEM_BASE0_MASK = 0x0000000000000007;      ///< same as ITEM_NATURE_MASK + ITEM_HAS_LOCAL_STRING
    const unsigned long long ITEM_BASE1_MASK = 0x000000000000000f;      ///< same as ITEM_BASE0_MASK + TYPE_IS_ITEM_ID

    const int SIMPLE_EDGE_TEXT_LEN = 72;    ///< text length for simple edges
    const int SIMPLE_VERTEX_TEXT_LEN = 96;  ///< text length for simple vertices
    const int FULL_VERTEX_TEXT_LEN = 32;    ///< text length for full vertices
    const int FULL_EDGE_TEXT_LEN = 16;      ///< text length for full edges

    const int ITEM_NEXT_KEY = 0xABCD;
    const int ITEM_NEXT_STRING = 0xDCBA;

    // ----------------------------------------- Item ----------------------------------------------------

    /**
     * @brief The Item class - vertices and edges - 128 bytes long
     */
    class Item{
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

        // all methods documented in m1_store.cpp
    public:
        // Does nothing. The class is not supposed to be constructed but cast from an mmap() area pointer
        Item(){}
        // For testing purposes only
        Item(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);

        // to be used in real situations when instantiating from mmap() area pointer
        void initializeMembers(const ItemID p_ID, const FlagField p_flags, const ItemType p_type);
        void initializeMembers();

        ItemID id();

        void setFlags(const FlagField p_flags, bool p_force_init = false);
        FlagField flags();

        void setType(const unsigned int p_index, const SpecialItemID p_type);
        SpecialItemID getType(const unsigned int p_index);
        ItemID getType();
        void setType(const ItemID p_type);
        bool isOfType(const ItemID p_type);
        bool isOfType(const SpecialItemID p_type);

        void setFlagsExtra(const FlagField p_flags);
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

    // ----------------------------------------- Storage ----------------------------------------------------
    class Storage{
    private:
        static const char* STORE_DATA_PATH;     ///< Path to the data store
        static const char* STORE_LMDB_DIR;      ///< Subdir for LMDB store (strings + utilities)
        static const char* LMDB_UTIL_DB;        ///< name of the table for util data
        static const char* LMDB_STRING_DB;      ///< string table

        static MDB_env* cm_lmdb_env;            ///< LMDB env
        static MDB_dbi cm_dbi_util;             ///< util table handle
        static MDB_dbi cm_dbi_string;           ///< string table handle

        static ItemID cm_next_item;             ///< next ItemID counter (stored in util table)
        static ItemID cm_next_string;           ///< next StringID counter (stored in util table)
    public:
        static void init();
        static void close();
        static void loadCounters(MDB_txn* p_txn);
        static void saveCounters(MDB_txn* p_txn);
        static ItemID storeString(QString p_string);
        static char* retrieveString(ItemID p_string_id);
        static void freeString(ItemID p_string_id);

        static StringID dbg_get_string_id(Item& p_item) {return p_item.p.v.f.m_string_id;}
    };

} // end namespace M1Store

#endif // M1_STORE_H
