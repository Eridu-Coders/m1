#include <QApplication>
#include <QRegularExpression>
#include <QXmlStreamReader>

#include "m1D_main_window.h"
#include "m1A_env.h"
#include "m1B_tei_interface.h"
#include "m1B_json_interface.h"
#include "m1B_store.h"
#include "m1B_graph_init.h"
#include "m1C_interp.h"

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;

Q_LOGGING_CATEGORY(g_cat_main, "m1_main")
Q_LOGGING_CATEGORY(g_cat_tmp_spotlight, "tmp.spotlight")
Q_LOGGING_CATEGORY(g_cat_tmp_debug, "tmp_debug")

// cloc count
// cloc --by-file-by-lang --exclude-content="M1Env::GraphInit::init_base()" --include-lang=C++,"C/C++ Header",Python --fullpath --not-match-d='^build.*|.*stringcase.*' *

/*
void loadEnoch();
int loadGita();
bool loadTei1(bool p_validate=false);
*/

int main(int argc, char *argv[])
{
    // printf("Xa\n");
    po::options_description l_desc("Allowed options");
    // printf("Xb\n");
    // std::vector<std::string> l_fuck;
    l_desc.add_options()
        ("help,h", "produce help message")
        //("load-json,j", "Load a json file (Old Plato Republic style)")
        ("load-tei-1,b", "Load a TEI file - Bhagavad Gita Style")
        // ("load-tei-2,q", "Load a TEI file - Plato Perseus Style - with EMBEDDED <w> tags")
        // ("load-tei-3,p", "Load a TEI file - Plato Perseus Style - with SEPARATE <w> tags")
        ("reset,r", "Reset (empty) storage")
        // ("qmljsdebugger", po::value<std::vector<std::string>>(&l_fuck), "Fuck")
        ;
    // printf("X\n");
    po::variables_map l_program_options_vm;
    // printf("Xc\n");
    try{
        // printf("Y\n");
        po::store(po::parse_command_line(argc, argv, l_desc), l_program_options_vm);
        // printf("Z\n");
        po::notify(l_program_options_vm);
        // printf("T\n");

        if(l_program_options_vm.count("reset")) std::cout << "reset option detected" << std::endl;
        if(l_program_options_vm.count("load-tei-1")) std::cout << "load-tei-1 option detected" << std::endl;
        //if(l_program_options_vm.count("load-tei-2")) std::cout << "load-tei-2 option detected" << std::endl;
        //if(l_program_options_vm.count("load-tei-2")) std::cout << "load-tei-3 option detected" << std::endl;
        //if(l_program_options_vm.count("load-json")) std::cout << "load-json option detected" << std::endl;
        // if(l_program_options_vm.count("qmljsdebugger")) std::cout << l_fuck.at(0) << std::endl;
        // printf("U\n");
    } catch ( po::error& e){
        std::cout << e.what();
        return 1;
    }

    M1Env::EnvStatic::init();
    M1Env::EnvStatic::setNormalFilter("*.debug=true\n"
                                        "store.*=false\n"
                                        "lv0.*=false\n"
                                        "lv1.*=false\n"
                                        "lv2.*=false\n"
                                        // "lv2.g_cat_lv2_constructors=true\n"
                                        "qt.*.debug=false");

    M1_FUNC_ENTRY(g_cat_main, QString("App starts"))

    if(l_program_options_vm.count("help")) {
        std::cout << l_desc << "\n";
        return 1;
    }

    // screen logging excluded categories
    M1Store::EnvStatic::addExcludeCatergoryForScreen("lv0.item_type");
    M1Store::EnvStatic::addExcludeCatergoryForScreen("lv0.special_item");
    M1Store::EnvStatic::addExcludeCatergoryForScreen("lv0.members_access");
    M1Store::EnvStatic::addExcludeCatergoryForScreen("lv1.members_access");
    M1Store::EnvStatic::addExcludeCatergoryForScreen("lv2.constructors");
    M1Store::EnvStatic::addExcludeCatergoryForScreen("lv2.members_access");
    // M1Store::EnvStatic::addExcludeCatergoryForScreen("lv2.iterators");
    M1Store::EnvStatic::addExcludeCatergoryForScreen("lv2.test");
    M1Store::EnvStatic::addExcludeCatergoryForScreen("store.storage");

    if(l_program_options_vm.count("reset")) M1Store::StorageStatic::storeSetUp(true);
    else M1Store::StorageStatic::storeSetUp(false);

    M1Env::EnvStatic::setNormalFilter("*.debug=true\n"
                                        "store.*=false\n"
                                        "lv0.*=false\n"
                                        "lv1.*=false\n"
                                        "lv2.*=false\n"
                                        // "lv2.type_iterators=true\n"
                                        "interp.*=false\n"
                                        // "interp.drag=true\n"
                                        "tree_display=false\n"
                                        "passages_panel=false\n"
                                        "main_window=false\n"
                                        "tei_interface=false\n"
                                        "tree_row=false\n"
                                        "qt.*.debug=false\n");
    M1Env::EnvStatic::setTmpDebug(false);

    M1MidPlane::Interp::init();
    M1UI::TreeRow::init();

    M1Store::TEIInterface::init();
    M1Store::JsonInterface::init();

    // if(l_program_options_vm.count("load-json")){
    //     try{
    //         std::cout << "Loading JSON file ..." << std::endl;
    //         M1Store::JsonInterface::loadJson("../republic.json");
    //         qCDebug(g_cat_main).noquote() << "End of JSON loading";
    //         // std::_Exit(0);
    //     }
    //     catch(const M1Env::M1Exception& e){
    //         qCDebug(g_cat_main).noquote() << "JSON load error" << e.code() << e.message();
    //         return 1;
    //     }
    // }
    if(l_program_options_vm.count("load-tei-1")){
        try{
            std::cout << "Loading TEI file (Gita Style) ..." << std::endl;
            M1Store::TEIInterface::loadTei1("../gitaDnl/bg_final_ex.xml");
            qCDebug(g_cat_main).noquote() << "End of TEI XML (1) loading";
            // std::_Exit(0);
        }
        catch(const M1Env::M1Exception& e){
            qCDebug(g_cat_main).noquote() << "TEI (1) load error" << e.code() << e.message();
            return 1;
        }
    }
    // if(l_program_options_vm.count("load-tei-2")){
    //     try{
    //         std::cout << "Loading TEI file (Plato style) ..." << std::endl;
    //         M1Store::TEIInterface::loadTei2("../../Greek_Translation_alignment_claude/PR05_emb.xml");
    //         qCDebug(g_cat_main).noquote() << "End of TEI (2) XML loading";
    //         // std::_Exit(0);
    //     }
    //     catch(const M1Env::M1Exception& e){
    //         qCDebug(g_cat_main).noquote() << "TEI load (2) error" << e.code() << e.message();
    //         return 1;
    //     }
    // }
    // if(l_program_options_vm.count("load-tei-3")){
    //     try{
    //         std::cout << "Loading TEI file (Plato style with embedded <w>) ..." << std::endl;
    //         M1Store::TEIInterface::loadTei3("../../Greek_Translation_alignment_claude/PR05_emb.xml");
    //         qCDebug(g_cat_main).noquote() << "End of TEI (3) XML loading";
    //         // std::_Exit(0);
    //     }
    //     catch(const M1Env::M1Exception& e){
    //         qCDebug(g_cat_main).noquote() << "TEI load (3) error" << e.code() << e.message();
    //         return 1;
    //     }
    // }

    // qCDebug(g_cat_main).noquote() << "Testing g_re_final_p: " << QString("भोगिनः। विष्णोश्चलति तद्भोगोऽत्यतीव हरितोषणम् इति पाद्मे।</p>").replace(g_re_final_p, "");

    // throw M1Env::M1Exception("", 0);
    QApplication a(argc, argv);

    M1UI::MainWindow w;
    w.show();
    int l_ret = a.exec();
    w.purge();

    M1Store::StorageStatic::storeShutDown();
    qCDebug(g_cat_tmp_spotlight) << "Final Debug Message";
    M1_FUNC_EXIT
    M1Env::EnvStatic::close();
    M1Env::EnvStatic::setDebug_extra();

    printf("Hurrah! No Core Dump ... Returning %d\n", l_ret);
    return l_ret;
}


