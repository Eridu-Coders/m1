#ifndef M1_CONSTANTS_H
#define M1_CONSTANTS_H

#include <QLoggingCategory>

// classic min macro
#define m1_min(a, b) (((a) < (b)) ? (a) : (b))

Q_DECLARE_LOGGING_CATEGORY(g_cat_store)
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_members)
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_test)
Q_DECLARE_LOGGING_CATEGORY(g_cat_wrapper)
Q_DECLARE_LOGGING_CATEGORY(g_cat_silence)
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_members)
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_constructors)

namespace M1Store{

    /// file name for the storage of log messages by M1Env
    static const char* LOG_FILE_NAME = "m1_log.csv";

    typedef unsigned long long FlagField;   ///< unsigned 64-bit wide flag field
    typedef unsigned long long ItemID;      ///< unsigned 64-bit wide ID number for both vertices and edges
    typedef unsigned long long StringID;    ///< unsigned 64-bit wide ID number for vertex strings
    typedef unsigned long long Date;        ///< unsigned 64-bit wide date (seconds since 1-1-1970)
    typedef long long ItemCounter;          ///< signed 64-bit wide counter (incoming edges, etc) -1 = unset
    typedef unsigned short SpecialItemID;   ///< unsigned 16-bit wide ID number for core type numbers, etc

    // Null values for ItemID (64 bits) and SpecialItemID (16 bits) respectively
    const ItemID G_VOID_ID = 0xffffffffffffffff; ///< -1 = NULL value
    const SpecialItemID G_VOID_TYPE_ID = 0xffff; ///< -1 = NULL value

    // flag bits for items
    const unsigned long long ITEM_IS_VERTEX         = 0x0000000000000001; ///< 0 --> item is an edge, 1 --> item is a vertex
    const unsigned long long ITEM_IS_SIMPLE         = 0x0000000000000002; ///< 1 --> item is either a simple edge or simple vertex
    const unsigned long long ITEM_HAS_LOCAL_STRING  = 0x0000000000000004; ///< 1 --> the local m_text (if any) is used to store the string
    const unsigned long long TYPE_IS_ITEM_ID        = 0x0000000000000008; ///< 1 --> the type field is a single ItemID, 0 --> 4 SpecialItemIDs
    const unsigned long long IS_SPECIAL             = 0x0000000000000010; ///< 1 --> this item is a special item
    const unsigned long long IS_AUTO                = 0x0000000000000020; ///< 1 --> this is an AUTO_ edge

    // composite flag values for item nature testing
    const unsigned long long FULL_VERTEX = ITEM_IS_VERTEX;                      // 0x1
    const unsigned long long SIMPLE_VERTEX = ITEM_IS_VERTEX | ITEM_IS_SIMPLE;   // 0x3
    const unsigned long long FULL_EDGE = 0x0;                                   // 0x0
    const unsigned long long SIMPLE_EDGE = ITEM_IS_SIMPLE;                      // 0x2
    const unsigned long long ITEM_NATURE_MASK = 0x0000000000000003;     ///< all bits to determine Item nature
    const unsigned long long ITEM_BASE0_MASK = 0x0000000000000007;      ///< same as ITEM_NATURE_MASK + ITEM_HAS_LOCAL_STRING
    const unsigned long long ITEM_BASE1_MASK = 0x000000000000000f;      ///< same as ITEM_BASE0_MASK + TYPE_IS_ITEM_ID

    // length of text area in each of the 4 types of items
    const unsigned int SIMPLE_EDGE_TEXT_LEN = 72;    ///< text length for simple edges
    const unsigned int SIMPLE_VERTEX_TEXT_LEN = 96;  ///< text length for simple vertices
    const unsigned int FULL_VERTEX_TEXT_LEN = 32;    ///< text length for full vertices
    const unsigned int FULL_EDGE_TEXT_LEN = 16;      ///< text length for full edges

    // keys for value storage in the utilities table
    const unsigned int ITEM_NEXT_KEY = 0xABCD;       ///< LMDB key for next ItemID
    const unsigned int ITEM_NEXT_STRING = 0xDCBA;    ///< LMDB key for next string ID
    const unsigned int ITEM_NEXT_SPECIAL = 0xDCBB;   ///< LMDB key for next special ID
    const unsigned int CURRENT_VERSION = 0xFFAA;     ///< current version of the graph

    // flag bits for special items
    const unsigned long long SI_IS_TYPE         = 0x0000000000000001; ///< is a type
    const unsigned long long SI_REQUIRES_EDGE   = 0x0000000000000002; ///< type requiring an edge representation
    const unsigned long long SI_IS_ETF          = 0x0000000000000004; ///< exclusive type family (only one type in this group at a time)
    const unsigned long long SI_HAS_NO_ITEM     = 0x0000000000000008; ///< No corresponding Iem to this special item
    const unsigned long long SI_HAS_RECIPROCAL  = 0x0000000000000010; ///< This type's reciprocal type (for edge types only)
    const unsigned long long SI_IS_SPECIAL_EDGE = 0x0000000000000020; ///< When this type is given to an edge it must be added to the special edge list

    // M1Store constants
    // class-level constants holding the names of various things
    // root dire for data storage
    static const char* STORE_DATA_PATH           = "/home/fi11222/disk-share/Dev/m1/Storage";
    static const char* STORE_LMDB_DIR            = "lmdb";           // subdir for LMDB data
    static const char* LMDB_UTIL_DB              = "util_db";        // name of the utilities table
    static const char* LMDB_STRING_DB            = "string_db";      // name of the strings table
    static const char* LMDB_ITEM_MMAP_FILE       = "local_store.m1"; // name of mmap() file for items
    static const char* LMDB_SPECIAL_MMAP_FILE    = "specials.m1";    // name of mmap() file for special items

} // end of M1Store namespace
#endif // M1_CONSTANTS_H
