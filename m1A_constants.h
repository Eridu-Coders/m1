#ifndef M1_CONSTANTS_H
#define M1_CONSTANTS_H

#include <QLoggingCategory>

// classic min/max macros
#define m1_min(a, b) (((a) < (b)) ? (a) : (b))
#define m1_max(a, b) (((a) > (b)) ? (a) : (b))

namespace M1Env{
    /// file name for the storage of log messages by M1Env
    static const char* LOG_FILE_NAME = "m1_log.csv";

    typedef unsigned long long FlagField;   ///< unsigned 64-bit wide flag field
    typedef unsigned long long ItemID;      ///< unsigned 64-bit wide ID number for both vertices and edges
    typedef unsigned long long StringID;    ///< unsigned 64-bit wide ID number for vertex strings
    typedef unsigned long long Date;        ///< unsigned 64-bit wide date (seconds since 1-1-1970)
    typedef long long ItemCounter;          ///< signed 64-bit wide counter (incoming edges, etc) -1 = unset
    typedef unsigned short SpecialItemID;   ///< unsigned 16-bit wide ID number for core type numbers, etc

    // Null values for ItemID (64 bits) and SpecialItemID (16 bits) respectively
    const ItemID G_VOID_ITEM_ID = 0xffffffffffffffff;   ///< -1 = NULL value for ItemID
    const ItemID G_NONEX_ITEM_ID = 0xfffffffffffffffe;  ///<
    const SpecialItemID G_VOID_SI_ID = 0xffff;          ///< -1 = NULL value for SpecialItemID
    const SpecialItemID G_NONEX_SI_ID = 0xfffe;         ///<

    // flag bits for items                              OOOO____OOOO____
    const unsigned long long ITEM_IS_VERTEX         = 0x0000000000000001; ///< 0 --> item is an edge, 1 --> item is a vertex
    const unsigned long long ITEM_IS_SIMPLE         = 0x0000000000000002; ///< 1 --> item is either a simple edge or simple vertex
    const unsigned long long ITEM_HAS_LOCAL_STRING  = 0x0000000000000004; ///< 1 --> the local m_text (if any) is used to store the string
    const unsigned long long TYPE_IS_ITEM_ID        = 0x0000000000000008; ///< 1 --> the type field is a single ItemID, 0 --> 4 SpecialItemIDs
    const unsigned long long IS_SPECIAL             = 0x0000000000000010; ///< 1 --> this item is a special vertex
    const unsigned long long IS_AUTO                = 0x0000000000000020; ///< 1 --> this is an AUTO_ edge
    const unsigned long long EDGE_IS_OPEN           = 0x0000000000000080; ///< 1 --> this edge is open (recursive descent)

    const unsigned long long SPECIAL_ITEM_ID_MASK   = 0xFFFF000000000080; ///<

    // bit masks for testing the values below           OOOO____OOOO____
    const unsigned long long ITEM_NATURE_MASK       = 0x0000000000000003; ///< all bits to determine Item nature
    const unsigned long long ITEM_BASE0_MASK        = 0x0000000000000007; ///< same as ITEM_NATURE_MASK | ITEM_HAS_LOCAL_STRING
    const unsigned long long ITEM_BASE1_MASK        = 0x000000000000000f; ///< same as ITEM_BASE0_MASK | TYPE_IS_ITEM_ID
    // composite flag values for item nature testing
    const unsigned long long FULL_VERTEX = ITEM_IS_VERTEX;                      ///< (0x1) flag & ITEM_NATURE_MASK == FULL_VERTEX --> it is a full vertex
    const unsigned long long SIMPLE_VERTEX = ITEM_IS_VERTEX | ITEM_IS_SIMPLE;   ///< (0x3) flag & ITEM_NATURE_MASK == SIMPLE_VERTEX --> it is a simple vertex
    const unsigned long long FULL_EDGE = 0x0;                                   ///< (0x0) flag & ITEM_NATURE_MASK == FULL_EDGE --> it is a full edge
    const unsigned long long SIMPLE_EDGE = ITEM_IS_SIMPLE;                      ///< (0x2) flag & ITEM_NATURE_MASK == SIMPLE_EDGE --> it is a simple vertex

    // length of text area in each of the 4 types of items
    const unsigned int FULL_VERTEX_TEXT_LEN   = 32;  ///< text length for full vertices
    const unsigned int SIMPLE_VERTEX_TEXT_LEN = 96;  ///< text length for simple vertices
    const unsigned int FULL_EDGE_TEXT_LEN     = 16;  ///< text length for full edges
    const unsigned int SIMPLE_EDGE_TEXT_LEN   = 64;  ///< text length for simple edges

    // keys for value storage in the utilities table
    // values up to 0x00010000 are reserved for icon paths
    const unsigned int ITEM_NEXT_KEY        = 0x00010000; ///< LMDB key for next ItemID
    const unsigned int ITEM_NEXT_STRING     = 0x00010001; ///< LMDB key for next string ID
    const unsigned int ITEM_NEXT_SPECIAL    = 0x00010002; ///< LMDB key for next special ID
    const unsigned int CURRENT_VERSION      = 0x00010003; ///< LMDB key for the current version of the graph
    const unsigned int CURRENT_UPPER_COUNT  = 0x00010004; ///< LMDB key for the current upper count of cm_next_item

    /// number of Items to add to the mmap() area on each increment. Results in file size incrementsw of 1GB
    const unsigned int ITEMS_MMAP_INCREMENT_COUNT = 8388608;    // = 2^23 (*128 --> 1GB)
    const float ITEMS_SPACE_MARGIN_FACTOR = .1;                 // space left inferior to this margin --> space increase at startup

    const unsigned short M1_WARNING_LOW_FREE_SPACE = 0x0001;    // warning that free items are above the margin above

    // flag bits for special items
    const unsigned long long SI_IS_TYPE         = 0x0000000000000001; ///< is a type
    const unsigned long long SI_REQUIRES_EDGE   = 0x0000000000000002; ///< type requiring an edge representation
    const unsigned long long SI_IS_ETF          = 0x0000000000000004; ///< exclusive type family (only one type in this group at a time)
    const unsigned long long SI_HAS_NO_ITEM     = 0x0000000000000008; ///< No corresponding Iem to this special item
    const unsigned long long SI_HAS_RECIPROCAL  = 0x0000000000000010; ///< This type's reciprocal type (for edge types only)
    const unsigned long long SI_IS_SPECIAL_EDGE = 0x0000000000000020; ///< When this type is given to an edge it must be added to the special edge list
    const unsigned long long SI_INSERT_AT_TOP   = 0x0000000000000040; ///< insert at top of the ordinary edges ring (for edges that are not special)
    const unsigned long long SI_IS_ICON_TYPE    = 0x0000000000000080; ///< type for icon choice
    const unsigned long long SI_IS_SELECTABLE   = 0x0000000000000100; ///< type appears in the vertex/edge selection combos on left panel
    const unsigned long long SI_EDGE_TYPE       = 0x0000000000000200; ///< type appears in the vertex/edge selection combos on left panel

    // M1Store constants
    // class-level constants holding the names of various things
    /// root dir for data storage
    static const char* STORE_DATA_PATH           = "../Storage";
    static const char* STORE_LMDB_DIR            = "lmdb";           // subdir for LMDB data
    static const char* LMDB_UTIL_DB              = "util_db";        // name of the utilities table
    static const char* LMDB_STRING_DB            = "string_db";      // name of the strings table
    static const char* LMDB_ITEM_MMAP_FILE       = "local_store.m1"; // name of mmap() file for items
    static const char* LMDB_SPECIAL_MMAP_FILE    = "specials.m1";    // name of mmap() file for special items

    static const char* UNKOWN_ICON_PATH = "../Icons/Unknown.svg";
    static const char* AUTO_ICON_PATH = "../Icons/360-rotate.svg";                      // *
    static const char* FOLDER_ICON_PATH = "../Icons/Folders.svg";                       // *
    static const char* ISA_ICON_PATH = "../Icons/Isa.svg";                              // *
    static const char* ITO_ICON_PATH = "../Icons/Ito.svg";                              // *
    static const char* OWNS_ICON_PATH = "../Icons/OWNS.svg";                            // *
    static const char* BLNGS_ICON_PATH = "../Icons/BLNGS.svg";                          // *
    static const char* TEXT_ICON_PATH = "../Icons/Text.svg";                            // *
    static const char* OCCURRENCE_ICON_PATH = "../Icons/Occurrence.svg";                // *
    static const char* WFORM_ICON_PATH = "../Icons/Word.svg";                           // *
    static const char* LEMMA_ICON_PATH = "../Icons/Lemma.svg";                          // *
    static const char* ENTITY_ICON_PATH = "../Icons/Entity.svg";                        // *
    static const char* POS_ICON_PATH = "../Icons/Nlp-Pos.svg";                          // *
    static const char* TAG_ICON_PATH = "../Icons/Nlp-Tag.svg";                          // *

    static const char* TEXT_CHUNK_ICON_PATH = "../Icons/Section.svg";                   // *
    static const char* SLOKA_ICON_PATH = "../Icons/Sloka.svg";                          // *
    static const char* TW_SECTION_2_OCC_BEGIN_ICON_PATH = "../Icons/Sec-Begin.svg";     // *
    static const char* TW_SECTION_2_OCC_END_ICON_PATH = "../Icons/Sec-End.svg";         // *
    static const char* TEXT_WFW_UNIT_ICON_PATH = "../Icons/TranslUnit.svg";             // *
    static const char* TEXT_SLOKA_BHASHYA_ICON_PATH = "../Icons/Bhashya.svg";           // *
    static const char* TEXT_SLOKA_TRANSLATION_ICON_PATH = "../Icons/Translation.svg";   // *
    static const char* TEXT_SLOKA_LINE_ICON_PATH = "../Icons/SlkLn.svg";                // *
    static const char* TYPE_NODE_ICON_PATH = "../Icons/Type.svg";                       // *
    static const char* PERSON_ICON_PATH = "../Icons/Person.svg";                        // *
    static const char* ORGANIZATION_ICON_PATH = "../Icons/Organization.svg";            // *
    static const char* TEXT_URL_LINK_ICON_PATH = "../Icons/Url.svg";                    // *
    static const char* TEXT_WROTE_ICON_PATH = "../Icons/Wrote.svg";                     // *
    static const char* TEXT_WRITTEN_BY_ICON_PATH = "../Icons/WrittenBy.svg";            // *
    static const char* BOOK_ICON_PATH = "../Icons/Book.svg";                            // *
    static const char* SENTENCE_ICON_PATH = "../Icons/Sentence.svg";                    // *
    static const char* HIGHLIGHTER_ICON_PATH = "../Icons/Highlighter.svg";              // *
    static const char* HL_CHUNK_ICON_PATH = "../Icons/Highlighter2.svg";                // *
    static const char* HL_QUOTE_ICON_PATH = "../Icons/Quote.svg";                       // *
    static const char* SLOKA_FOLDER_ICON_PATH = "../Icons/SlokaFolder.svg";             // *

    static const bool SKIP_HEAVY_CHECKING = true; ///< true --> skip time-consuming Q_ASSERT in setType() and linkTo()
} // end of M1Env namespace

// debug categories pre-declarations
Q_DECLARE_LOGGING_CATEGORY(g_cat_store)                 ///< for Storage class (static functions)
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_item_type)         ///< for ItemType
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_special_item)      ///< for SpecialItem
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_members)           ///< for lv0_Item
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv1_members)           ///< for lv1_Item
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_members)           ///< for lv2_Item class members
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_constructors)      ///< for lv2_Item class constructors and instanciation from mmap()
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_iterators)         ///< for lv2_Item-derived iterators
Q_DECLARE_LOGGING_CATEGORY(g_cat_silence)               ///< for debug strings generation (silencing all other debug categories)
Q_DECLARE_LOGGING_CATEGORY(g_main_test)                 ///< messages from main fumction in a test build
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_test)              ///< for tests of the lv0_Item class
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv1_test)              ///< for tests of the lv1_Item class
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_test)              ///< for tests of the lv2_Item class
Q_DECLARE_LOGGING_CATEGORY(g_cat_tree_display)          ///< TreeDisplay widget class
Q_DECLARE_LOGGING_CATEGORY(g_cat_passages_panel)        ///< Passages panel UI class
Q_DECLARE_LOGGING_CATEGORY(g_cat_interp_base)           ///< Base class of the Interp hierarchy
Q_DECLARE_LOGGING_CATEGORY(g_cat_interp_drag)           ///< Drag/Drop in the Interp hierarchy
Q_DECLARE_LOGGING_CATEGORY(g_cat_main_window)           ///< MainWindow widget class
Q_DECLARE_LOGGING_CATEGORY(g_cat_main)                  ///< messages from main of complete (non-test) build (including XML loaders)

#endif // M1_CONSTANTS_H
