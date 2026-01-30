#ifndef M1_CONSTANTS_H
#define M1_CONSTANTS_H

#include <QLoggingCategory>

// classic min/max macros
#define m1_min(a, b) (((a) < (b)) ? (a) : (b))
#define m1_max(a, b) (((a) > (b)) ? (a) : (b))

/**
 * ********************************************************************************************************************************
 * \defgroup GlobDec Global Declarations
 */
/**@{*/

namespace M1Env{
    /// file name for the storage of log messages by M1Env
    static const char* LOG_FILE_NAME = "m1_log.csv";

/**
 * \defgroup BaseTypes Integer types (Item ID, Special Item ID, etc)
 * @ingroup GlobDec
 */
/**@{*/
    typedef unsigned long long FlagField;   ///< unsigned 64-bit wide flag field
    typedef unsigned long long ItemID;      ///< unsigned 64-bit wide ID number for both vertices and edges
    typedef unsigned long long StringID;    ///< unsigned 64-bit wide ID number for vertex strings
    typedef unsigned long long Date;        ///< unsigned 64-bit wide date (seconds since 1-1-1970)
    typedef long long ItemCounter;          ///< signed 64-bit wide counter (incoming edges, etc) -1 = unset
    typedef unsigned short SpecialItemID;   ///< unsigned 16-bit wide ID number for core type numbers, etc
    // typedef unsigned short StringID_16;     ///< 16-bit string ID for use inside M1Store::SpecialItem
/**@}*/ //end of BaseTypes

/**
 * \defgroup NullVal Null Values for Item types
 * @ingroup GlobDec
 */
/**@{*/
    // Null values for ItemID (64 bits) and SpecialItemID (16 bits) respectively
    const ItemID G_VOID_ITEM_ID = 0xffffffffffffffff;   ///< -1 = NULL value for ItemID
    // const ItemID G_NONEX_ITEM_ID = 0xfffffffffffffffe;  ///< Value given to
    const SpecialItemID G_VOID_SI_ID = 0xffff;          ///< -1 = NULL value for SpecialItemID
    const SpecialItemID G_NONEX_SI_ID = 0xfffe;         ///< Value given to SpecialItemID pseudo-constants before they are initialized with their proper value
/**@}*/ //end of NullVal

/**
 * \defgroup ItemFlags Item Flags (for M1Store::Item_lv0)
 * @ingroup GlobDec
 * @{
 */
    // flag bits for items (m_flags in M1Store::Item_lv0)
    //                                                  XXXX____XXXX____
    const unsigned long long ITEM_IS_VERTEX         = 0x0000000000000001; ///< 0 --> item is an edge, 1 --> item is a vertex
    const unsigned long long ITEM_IS_SIMPLE         = 0x0000000000000002; ///< 1 --> item is either a simple edge or simple vertex
    const unsigned long long ITEM_HAS_LOCAL_STRING  = 0x0000000000000004; ///< 1 --> the local m_text (if any) is used to store the string
    const unsigned long long TYPE_IS_ITEM_ID        = 0x0000000000000008; ///< 1 --> the type field is a single ItemID, 0 --> 4 SpecialItemIDs
    const unsigned long long IS_SPECIAL             = 0x0000000000000010; ///< 1 --> this item is a special vertex
    const unsigned long long IS_AUTO                = 0x0000000000000020; ///< 1 --> this is an AUTO_ edge
    const unsigned long long EDGE_IS_OPEN           = 0x0000000000000080; ///< 1 --> this edge is open (recursive descent)
    const unsigned long long EDGE_SEPABOVE          = 0x0000000000000100; ///< 1 --> this edge has a horizontal separator above it

    const unsigned long long SPECIAL_ITEM_ID_MASK   = 0xFFFF000000000000; ///< Mask of the area used to store the SpecialItemID of a vertex, when applicable (IS_SPECIAL is set)

    // bit masks for testing the values below           XXXX____XXXX____
    const unsigned long long ITEM_NATURE_MASK       = 0x0000000000000003; ///< all bits to determine Item nature
    const unsigned long long ITEM_BASE0_MASK        = 0x0000000000000007; ///< same as ITEM_NATURE_MASK | ITEM_HAS_LOCAL_STRING
    const unsigned long long ITEM_BASE1_MASK        = 0x000000000000000f; ///< same as ITEM_BASE0_MASK | TYPE_IS_ITEM_ID
    // composite flag values for item nature testing
    const unsigned long long FULL_VERTEX = ITEM_IS_VERTEX;                      ///< (0x1) flag & ITEM_NATURE_MASK == FULL_VERTEX --> it is a full vertex ?
    const unsigned long long SIMPLE_VERTEX = ITEM_IS_VERTEX | ITEM_IS_SIMPLE;   ///< (0x3) flag & ITEM_NATURE_MASK == SIMPLE_VERTEX --> it is a simple vertex ?
    const unsigned long long FULL_EDGE = 0x0;                                   ///< (0x0) flag & ITEM_NATURE_MASK == FULL_EDGE --> it is a full edge ?
    const unsigned long long SIMPLE_EDGE = ITEM_IS_SIMPLE;                      ///< (0x2) flag & ITEM_NATURE_MASK == SIMPLE_EDGE --> it is a simple vertex ?
/**@}*/ //end of ItemFlags

/**
 * \defgroup TextLen Length of string member for 4 types of items
 * @ingroup GlobDec
 * length of text field (m_text) within each of the 4 types of items (simple/full x Edge/Vertex)
 * @{
 */
    const unsigned int FULL_VERTEX_TEXT_LEN   = 32;  ///< text length for full vertices
    const unsigned int SIMPLE_VERTEX_TEXT_LEN = 96;  ///< text length for simple vertices
    const unsigned int FULL_EDGE_TEXT_LEN     = 16;  ///< text length for full edges
    const unsigned int SIMPLE_EDGE_TEXT_LEN   = 64;  ///< text length for simple edges
/**@}*/ //end of TextLen

/**
 * \defgroup FlagsSpec Special Item Flags
 * @ingroup GlobDec
 * flag bits for special items (m_flags in M1Store::SpecialItem)
 * @{
 */
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
/**@}*/ //end of FlagsSpec

/**
 * \defgroup StoreConst M1Store constants
 * @ingroup GlobDec
 * M1Store class-level constants holding the names of various things
 * @{
 */
    static const char* STORE_DATA_PATH              = "../Storage";        ///< root dir for data storage
    static const char* STORE_LMDB_DIR               = "lmdb";              ///< subdir for LMDB data
    static const char* LMDB_UTIL_DB                 = "util_db";           ///< name of the utilities table
    static const char* LMDB_STRING_DB               = "string_db";         ///< name of the strings table
    static const char* LMDB_ITEM_MMAP_FILE_TEMPLATE = "local_store%1.m1";  ///< name template for mmap() item segment files
    static const char* LMDB_SPECIAL_MMAP_FILE       = "specials.m1";       ///< name of mmap() file for special items
/**@}*/ //end of StoreConst

/**
 * \defgroup IconPaths Icon Paths
 * @ingroup GlobDec
 * @{
 */
    static const char* UNKOWN_ICON_PATH = "../m1_src/Icons/Unknown.svg";                        ///<
    static const char* AUTO_ICON_PATH = "../m1_src/Icons/360-rotate.svg";                       ///<
    static const char* FOLDER_ICON_PATH = "../m1_src/Icons/Folders.svg";                        ///<
    static const char* ISA_ICON_PATH = "../m1_src/Icons/Isa.svg";                               ///<
    static const char* ITO_ICON_PATH = "../m1_src/Icons/Ito.svg";                               ///<
    static const char* OWNS_ICON_PATH = "../m1_src/Icons/OWNS.svg";                             ///<
    static const char* BLNGS_ICON_PATH = "../m1_src/Icons/BLNGS.svg";                           ///<
    static const char* TEXT_ICON_PATH = "../m1_src/Icons/Text.svg";                             ///<
    static const char* OCCURRENCE_ICON_PATH = "../m1_src/Icons/Occurrence.svg";                 ///<
    static const char* WFORM_ICON_PATH = "../m1_src/Icons/Word.svg";                            ///<
    static const char* LEMMA_ICON_PATH = "../m1_src/Icons/Lemma.svg";                           ///<
    static const char* ENTITY_ICON_PATH = "../m1_src/Icons/Entity.svg";                         ///<
    static const char* POS_ICON_PATH = "../m1_src/Icons/Nlp-Pos.svg";                           ///<
    static const char* TAG_ICON_PATH = "../m1_src/Icons/Nlp-Tag.svg";                           ///<
    static const char* OPEN_ICON_PATH = "../m1_src/Icons/Open.svg";                             ///<
    static const char* CLOSED_ICON_PATH = "../m1_src/Icons/Closed.svg";                         ///<
    static const char* CROOKED_ICON_PATH = "../m1_src/Icons/CrookedArrow.svg";                  ///<
    static const char* FIELD_ICON_PATH = "../m1_src/Icons/Field.svg";                           ///<

    static const char* TEXT_CHUNK_ICON_PATH = "../m1_src/Icons/Section.svg";                    ///<
    static const char* SLOKA_ICON_PATH = "../m1_src/Icons/Sloka.svg";                           ///<
    static const char* TW_SECTION_2_OCC_BEGIN_ICON_PATH = "../m1_src/Icons/Sec-Begin.svg";      ///<
    static const char* TW_SECTION_2_OCC_END_ICON_PATH = "../m1_src/Icons/Sec-End.svg";          ///<
    static const char* TEXT_WFW_UNIT_ICON_PATH = "../m1_src/Icons/TranslUnit.svg";              ///<
    static const char* TEXT_SLOKA_BHASHYA_ICON_PATH = "../m1_src/Icons/Bhashya.svg";            ///<
    static const char* TEXT_SLOKA_TRANSLATION_ICON_PATH = "../m1_src/Icons/Translation.svg";    ///<
    static const char* TEXT_SLOKA_TRANSLIT_ICON_PATH = "../m1_src/Icons/SlkTranslit.svg";       ///<
    static const char* TEXT_SLOKA_LINE_ICON_PATH = "../m1_src/Icons/SlkLn.svg";                 ///<
    static const char* TYPE_NODE_ICON_PATH = "../m1_src/Icons/Type.svg";                        ///<
    static const char* PERSON_ICON_PATH = "../m1_src/Icons/Person.svg";                         ///<
    static const char* ORGANIZATION_ICON_PATH = "../m1_src/Icons/Organization.svg";             ///<
    static const char* ROLE_FOLDER_ICON_PATH = "../m1_src/Icons/RoleFolder.svg";                ///<
    static const char* TEXT_URL_LINK_ICON_PATH = "../m1_src/Icons/Url.svg";                     ///<
    static const char* TEXT_WROTE_ICON_PATH = "../m1_src/Icons/Wrote.svg";                      ///<
    static const char* TEXT_WRITTEN_BY_ICON_PATH = "../m1_src/Icons/WrittenBy.svg";             ///<
    static const char* DATA_SOURCE_ICON_PATH = "../m1_src/Icons/DataSource.svg";                ///<
    static const char* BOOK_ICON_PATH = "../m1_src/Icons/Book.svg";                             ///<
    static const char* CHAPTER_ICON_PATH = "../m1_src/Icons/Chapter.svg";                       ///<
    static const char* SENTENCE_ICON_PATH = "../m1_src/Icons/Sentence.svg";                     ///<
    static const char* HIGHLIGHTER_ICON_PATH = "../m1_src/Icons/Highlighter.svg";               ///<
    static const char* HL_CHUNK_ICON_PATH = "../m1_src/Icons/Highlighter2.svg";                 ///<
    static const char* HL_QUOTE_ICON_PATH = "../m1_src/Icons/Quote.svg";                        ///<
    static const char* SLOKA_FOLDER_ICON_PATH = "../m1_src/Icons/SlokaFolder.svg";              ///<
    static const char* TEXT_WFW_FORM_ICON_PATH = "../m1_src/Icons/word-form.svg";               ///<
/**@}*/ //end of IconPaths

    static const bool SKIP_HEAVY_CHECKING = true; ///< true --> skip time-consuming Q_ASSERT in setType() and linkTo()
} // end of M1Env namespace
/**@}*/ //end of GlobDec


/**
 * ***************************************************************************************************************************
 * \defgroup DebugLog Debug-related messages logging
 * @{
*/

/**
 * \defgroup QtLogCat Qt logging categories pre-declarations
 * @ingroup DebugLog
 * @{
 */

Q_DECLARE_LOGGING_CATEGORY(g_cat_store)                 ///< for Storage class (static functions)
Q_DECLARE_LOGGING_CATEGORY(g_cat_store_mmap)            ///< for Storage class (mmap() handling)
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_item_type)         ///< for ItemType
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_special_item)      ///< for SpecialItem
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_members)           ///< for lv0_Item
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv1_members)           ///< for lv1_Item
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_members)           ///< for lv2_Item class members
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_constructors)      ///< for lv2_Item class constructors and instanciation from mmap()
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_iterators)         ///< for lv2_Item-derived iterators g_cat_lv2_type_iterators
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_type_iterators)    ///< for edge type iterators
Q_DECLARE_LOGGING_CATEGORY(g_cat_silence)               ///< for debug strings generation (silencing all other debug categories)
Q_DECLARE_LOGGING_CATEGORY(g_main_test)                 ///< messages from main fumction in a test build
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv0_test)              ///< for tests of the lv0_Item class
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv1_test)              ///< for tests of the lv1_Item class
Q_DECLARE_LOGGING_CATEGORY(g_cat_lv2_test)              ///< for tests of the lv2_Item class
Q_DECLARE_LOGGING_CATEGORY(g_cat_tree_display)          ///< TreeDisplay widget class
Q_DECLARE_LOGGING_CATEGORY(g_cat_passages_panel)        ///< Passages panel UI class
Q_DECLARE_LOGGING_CATEGORY(g_cat_tree_row)              ///< TreeRow class
Q_DECLARE_LOGGING_CATEGORY(g_cat_interp_base)           ///< Base class of the Interp hierarchy
Q_DECLARE_LOGGING_CATEGORY(g_cat_interp_drag)           ///< Drag/Drop in the Interp hierarchy
Q_DECLARE_LOGGING_CATEGORY(g_cat_main_window)           ///< MainWindow widget class
Q_DECLARE_LOGGING_CATEGORY(g_cat_main)                  ///< messages from main of complete (non-test) build (including XML loaders)
Q_DECLARE_LOGGING_CATEGORY(g_cat_tei)                   ///< messages from TEI interface
Q_DECLARE_LOGGING_CATEGORY(g_cat_json)                  ///< messages from JSON interface
Q_DECLARE_LOGGING_CATEGORY(g_cat_tmp_spotlight)         ///< messages from feature under temporary scrutiny
Q_DECLARE_LOGGING_CATEGORY(g_cat_tmp_debug)             ///< messages to be triggered temporarily
/**@}*/ //end of QtLogCat

/**@}*/ //end of DebugLog

/** \defgroup STORE Physical storage layer - class M1Store::StorageStatic
 *
 *  Handles LMDB and mmap() access
 */

/** \defgroup LV0 Level 0 Item - Class M1Store::Item_lv0 and others
 *
 *  Base item instance level. Data as stored in the mmap() areas.
 */

/** \defgroup LV1 Level 1 Item - Class M1Store::Item_lv1
 *
 *  Data Store integrity maintenance level. This level takes care of the archive-logs which are replayed in case of a crash. Some higher level methodes
 *  are alson taken care of at that level. There are no private members in M1Store::Item_lv1. The data it handles is still the same as M1Store::Item_lv0
 */

/** \defgroup LV2 Level 2 Item - Class M1Store::Item_lv2 + Iterastors
 *
 *  Graph manipulation API provided to the rest of the system. There are no private members in M1Store::Item_lv2.
 *  The data it handles is still the same as in M1Store::Item_lv0 and M1Store::Item_lv1 (the 128 byte long union which lives in the mmap() area).
 *
 *  This clas cooperates with M1Store::Storage for the creation of new Items.
 */

/** \mainpage The M1 Project
 * \image html ShankhaChakra.png
 * # Introduction
 *
 * This project, which is still unnamed, aims to create a software suite to help the Vaishnava scholar write _Bhashyas_
 * (commentaries) on ancient texts. Specifically, the goal is to preserve the links inherent in the quotations within
 * each _Bhashya_ while retaining the ease of use of WYSIWIG tools like Microsoft Word and other wordprocessors.
 *
 * At its core is a graph database (NOSQL) which is a natural fit for the graph of quotations inherent in all _Bhashyas_
 * taken as a whole. The interface gives access to the texts both in a graph-structured way (left-hand panel on the
 * screenshot below) and in a WYSIWIG fashion (right-hand panel).
 *
 * \image html m1_Screenshot_01.png
 *
 * # Installation
 *
 * # Naming
 *
 * For the moment, the software goes under the temporary moniker "m1". In due course, a suitable name should be chosen.
 *
 * # Installation
 *
 * ## Prerequisites
 *
 * Linux is strongly recommended as a development platform for m1; Specifically [Ubuntu](https://en.wikipedia.org/wiki/Ubuntu)
 * or its derivatives ([mint](https://en.wikipedia.org/wiki/Linux_Mint), etc.)
 *
 * ### Lmdb
 *
 * [Lmdb](https://en.wikipedia.org/wiki/Lightning_Memory-Mapped_Database) is a widely used key/value dabase library with
 * C++ bindings. On Ubuntu, and Ubuntu-derived, Linux distributions, Lmdb installation is done with:
 * ```
 * sudo apt install liblmdb-dev
 * ```
 * m1 uses Lmdb to store ancillary data and strings. Its main storage space uses the same memory-mapped OS
 * feature as Lmdb, but not Lmdb itself.
 *
 * ### Boost
 *
 * [Boost](https://en.wikipedia.org/wiki/Boost_(C%2B%2B_libraries)) is a widely used general-purpose set of C++
 * libraries. On Ubuntu, and Ubuntu-derived, Linux distributions, Boost installation is done with:
 * ```
 * sudo apt install libboost-all-dev
 * ```
 *
 * ### Qt
 *
 * [Qt](https://en.wikipedia.org/wiki/Qt_(software)) is a widely used GUI library for C++. In fact it is more
 * than just a GUI toolkit and includes an entire development environment complete with several IDE interfaces
 * and even extensions to the C++ language itself (the so-called "[signals and slots](https://en.wikipedia.org/wiki/Signals_and_slots)").
 *
 * Qt has both a paid and an open-source version. The latter can be downloaded from [the company's website](https://www.qt.io/download-open-source).
 *
 * **CAUTION**: Qt's regional mirrors are not always reliable (especially in India), and one of the better ones, from the US or
 * Europe, should sometimes be manually chosen (as a CMD line option passed to the installer).
 * See [this page](https://download.qt.io/static/mirrorlist/mirmon/) or [this one](https://download.qt.io/static/mirrorlist/) for a list of Qt mirrors.
 * ## Source Download
 *
 * As usual with any GitHub repo:
 * ```
 * git clone https://github.com/Eridu-Coders/m1.git
 * ```
 *
 * ## m1 Compilation
 *
 * Load the project (the `CMakeLists.txt` file) into the _Qt Creator_ IDE and launch the compilation from there,
 * in Debug (many runtime messages) or Release mode (faster). In Release mode, the option `-DQT_NO_DEBUG_OUTPUT`
 * in `CMAKE_CXX_FLAGS` should be added to the options list in advanced mode ("Projects" tab of the IDE), to disable the runtime messages.
 *
 * \image html m1_Screenshot_02.png
 *
 * Also, make sure that the build directories are at the same level as `m1_src`, so that the executables can access the
 * icon `svg` files within that directory.
 *
 * Launch the application with the command (in the Debug or Release build directory):
 * ```
 * ./m1 -rpg
 * ```
 *
 * This will load the sample data from the _Baghad Gita_ and Plato's _Republic_. Afterwards, the application can be re-launched without
 * the load process (and therefore without erasing your data) with:
 * ```
 * ./m1
 * ```
 *
 * \image html ShankhaChakra.png
 */
#endif // M1_CONSTANTS_H
