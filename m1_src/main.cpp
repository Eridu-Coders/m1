#include <QApplication>
#include <QRegularExpression>
#include <QXmlStreamReader>

#include "m1D_main_window.h"
#include "m1A_env.h"
#include "m1B_tei_interface.h"
#include "m1B_store.h"
#include "m1B_graph_init.h"
#include "m1C_interp_new.h"

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;

Q_LOGGING_CATEGORY(g_cat_main, "m1_main")
Q_LOGGING_CATEGORY(g_cat_tmp_spotlight, "tmp")

// cloc count
// cloc --by-file-by-lang --exclude-content="M1Env::GraphInit::init_base()" --include-lang=C++,"C/C++ Header",Python --fullpath --not-match-d='^build.*|.*stringcase.*' *

QRegularExpression g_re_punc(R"(^(\W*)(\w+)(\W*))");
QRegularExpression g_re_space(R"(\s+)");

/*
void loadEnoch();
int loadGita();
bool loadTei(bool p_validate=false);
*/

int main(int argc, char *argv[])
{
    // printf("Xa\n");
    po::options_description l_desc("Allowed options");
    // printf("Xb\n");
    // std::vector<std::string> l_fuck;
    l_desc.add_options()
        ("help,h", "produce help message")
        ("load-gita,g", "Load Bhagavad Gita test data")
        ("load-plato,p", "Load The Republic of Plato test data")
        ("load-tei,t", "Loads a TEI file")
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

        if(l_program_options_vm.count("reset")) std::cout << "reset option detected" << std::endl;;
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
                                        // "interp.*=false\n"
                                        // "interp.drag=true\n"
                                        "tree_display=false\n"
                                        "passages_panel=false\n"
                                        "main_window=false\n"
                                        "qt.*.debug=false");

    M1MidPlane::Interp::init();
    M1UI::TreeRow::init();

    M1Store::TEIInterface::init();
    // loadEnoch();
    // if(l_program_options_vm.count("load-gita")) loadGita();
    if(l_program_options_vm.count("load-plato")) M1Store::GraphInit::init_plato();
    if(l_program_options_vm.count("load-tei")){
        try{
            M1Store::TEIInterface::loadTei("../gitaDnl/bg_final_ex.xml");
            qCDebug(g_cat_main).noquote() << "End of TEI XML loading";
            // std::_Exit(0);
        }
        catch(const M1Env::M1Exception& e){
            qCDebug(g_cat_main).noquote() << "TEI load error" << e.code() << e.message();
            return 1;
        }
    }

    // throw M1Env::M1Exception("", 0);
    QApplication a(argc, argv);
    M1UI::MainWindow w;
    w.show();
    int l_ret = a.exec();

    M1Store::StorageStatic::storeShutDown();
    qCDebug(g_cat_main) << "App ends";
    M1_FUNC_EXIT
    M1Env::EnvStatic::close();

    printf("Hurrah! No Core Dump ... Returning %d\n", l_ret);
    return l_ret;
}

/**
 * @brief skipUntil
 * @param p_indent_count
 * @param p_indent
 * @param p_xml_reader
 * @param p_elem_close
 * @return
 */
/*
QString skipUntil(int p_indent_count, QString& p_indent, QXmlStreamReader& p_xml_reader, const QString& p_elem_close){
    QString l_ret;
    qCDebug(g_cat_main).noquote() << p_indent << QString("Skip until <%1>").arg(p_elem_close);
    p_indent += QString(" ").repeated(p_indent_count);
    while (!p_xml_reader.atEnd()) {
        QXmlStreamReader::TokenType l_tt = p_xml_reader.readNext();
        QStringView l_tok_name = p_xml_reader.name();
        qCDebug(g_cat_main).noquote() << p_indent << "Token Type: " << l_tt << p_xml_reader.name();

        if(l_tt == QXmlStreamReader::EndElement && l_tok_name == p_elem_close){
            p_indent.chop(p_indent_count);
            qCDebug(g_cat_main).noquote() << p_indent << QString("End of skip until <%1>").arg(p_elem_close);
            break; // from the inner commentaries while
        }
        else {
            if(l_tt == QXmlStreamReader::StartElement &&  l_tok_name != "br") l_ret += QString("<%1>").arg(l_tok_name);
            else if(l_tt == QXmlStreamReader::EndElement) l_ret += QString("<%1/>").arg(l_tok_name);
            else if(l_tt == QXmlStreamReader::Characters){
                QString l_characters = p_xml_reader.text().toString().trimmed();
                l_ret += l_characters;
            }
        }
    }
    return l_ret;
}
*/
/**
 * @brief loadTei
 * @param p_validate
 * @return
 */
/*
bool loadTei(bool p_validate){
    QXmlStreamReader l_xml;
    QFile l_fin_tei("../gitaDnl/bg_final_ex.xml");
    qCDebug(g_cat_main) << "Exists: " << l_fin_tei.exists();

    // the xml stream parse
    if (!l_fin_tei.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for reading:" << l_fin_tei.errorString();
        std::_Exit(0);
    }
    QXmlStreamReader l_xml_reader(&l_fin_tei);
    QString l_title;
    QString l_sub_title;
    QString l_alt_title;
    QString l_author_text;
    int l_cur_chapter_number = 0;
    int l_cur_sloka_number = 0;
    QString l_indent = "";
    int l_indent_count = 4;

    bool l_found_title = false;
    bool l_found_author = false;
    bool l_found_at_least_one_chapter = false;
    bool l_found_at_least_one_sloka = false;
    bool l_found_sk = false;
    bool l_found_trnsl = false;
    bool l_found_wfw = false;
    bool l_found_translations = false;
    bool l_found_commentaries = false;

    while (!l_xml_reader.atEnd()) {
        QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
        QStringView l_tok_name = l_xml_reader.name();
        qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

        // text title(s)
        if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "title"){
            QXmlStreamAttributes l_att_1 = l_xml_reader.attributes();
            QString l_type = l_att_1.value("type").toString();

            QXmlStreamReader::TokenType l_tt_title = l_xml_reader.readNext();
            if(l_tt_title != QXmlStreamReader::Characters) throw M1Env::M1Exception("<title> not followed by Characters", 0);
            QString l_title_text = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_main).noquote() << l_indent << QString("<title type=\"%1\"> [%2]").arg(l_type).arg(l_title_text);

            if(l_type == "main") {
                l_title = l_title_text;
                l_found_title  = true;
            }
            else if(l_type == "sub") l_sub_title = l_title_text;
            else if(l_type == "alt") l_alt_title = l_title_text;

            // qCDebug(g_cat_main).noquote() << l_title << l_sub_title << l_alt_title;
        }
        // text author
        if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
            l_found_author = true;
            QXmlStreamReader::TokenType l_tt_author = l_xml_reader.readNext();
            if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author> not followed by Characters", 0);
            l_author_text = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_main).noquote() << l_indent << QString("<author> %1").arg(l_author_text);
        }
        // end of TEI header
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "teiHeader"){
            qCDebug(g_cat_main).noquote() << l_indent << QString("<teiHeader/> %1 (%2) [%3] author: %4")
                                                             .arg(l_title)
                                                             .arg(l_alt_title)
                                                             .arg(l_sub_title)
                                                             .arg(l_author_text);
        }
        // chapter start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div1"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            l_cur_chapter_number = l_xml_reader.attributes().value("n").toInt();
            l_found_at_least_one_chapter = true;
            l_found_at_least_one_sloka = false;
            qCDebug(g_cat_main).noquote() << l_indent << QString("<div1 type=%1> Chapter %2 start").arg(l_type).arg(l_cur_chapter_number);
        }
        // sloka start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div2"){
            l_found_sk = false;
            l_found_trnsl = false;
            l_found_wfw = false;
            l_found_translations = false;
            l_found_commentaries = false;

            QString l_type = l_xml_reader.attributes().value("type").toString();
            l_cur_sloka_number = l_xml_reader.attributes().value("n").toInt();
            l_found_at_least_one_sloka = true;
            qCDebug(g_cat_main).noquote() << l_indent << QString("<div2 type=%1> Sloka %2.%3 start").arg(l_type).arg(l_cur_chapter_number).arg(l_cur_sloka_number);
        }
        // sloka sk text
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "l"){
            l_found_sk = true;
            QXmlStreamReader::TokenType l_tt_sk = l_xml_reader.readNext();
            if(l_tt_sk != QXmlStreamReader::Characters) throw M1Env::M1Exception("<l> not followed by Characters", 0);
            QString l_sloka_text1 = l_xml_reader.text().toString().trimmed();
            QXmlStreamReader::TokenType l_tt_caesura = l_xml_reader.readNext();
            if(l_tt_caesura != QXmlStreamReader::StartElement && l_xml_reader.name() != "caesura") throw M1Env::M1Exception("No caesura in Sloka", 0);
            l_xml_reader.readNext(); // to pass the automatic EndElement after <caesura/>
            QXmlStreamReader::TokenType l_tt_sk2 = l_xml_reader.readNext();
            if(l_tt_sk2 != QXmlStreamReader::Characters) qFatal() << QString("No Characters after caesura: %1").arg(l_tt_sk2);
            QString l_sloka_text2 = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_main).noquote() << l_indent << QString("Sloka %1.%2: %3")
                                                 .arg(l_cur_chapter_number)
                                                 .arg(l_cur_sloka_number)
                                                 .arg(QString("%1 । %2").arg(l_sloka_text1).arg(l_sloka_text2));
        }
        // sloka transliteration
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "seg"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            if(l_type == "transliteration"){
                l_found_trnsl = true;
                QString l_standard = l_xml_reader.attributes().value("standard").toString();
                QXmlStreamReader::TokenType l_tt_trl = l_xml_reader.readNext();
                if(l_tt_trl != QXmlStreamReader::Characters) throw M1Env::M1Exception("<seg type=transliteration> not followed by Characters", 0);
                QString l_trl_text = l_xml_reader.text().toString().trimmed();
                qCDebug(g_cat_main).noquote() << l_indent << QString("<seg type=\"%1\" standard=\"%2\"> %3").arg(l_type).arg(l_standard).arg(l_trl_text);
            }
        }
        // div3 start (wfw, translation or commentaries block)
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div3"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            // word-for-word block
            if(l_type == "wfw-block"){
                l_found_wfw = true;

                bool l_found_one_wfw = false;
                bool l_found_wfw_translation = false;
                bool l_found_wfw_transliteration = false;
                bool l_found_wfw_morphology = false;

                qCDebug(g_cat_main).noquote() << l_indent << QString("<div3> start of wfw block for sloka %1.%2").arg(l_cur_chapter_number).arg(l_cur_sloka_number);
                int l_cur_word = 1;
                l_indent += QString(" ").repeated(l_indent_count);
                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    // wfw-unit <seg>
                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "seg"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "wfw-unit"){
                            l_found_one_wfw = true;
                            l_found_wfw_translation = false;
                            l_found_wfw_transliteration = false;
                            l_found_wfw_morphology = false;

                            QXmlStreamReader::TokenType l_tt_wfw = l_xml_reader.readNext();
                            if(l_tt_wfw != QXmlStreamReader::Characters) throw M1Env::M1Exception("<seg type=transliteration> not followed by Characters", 0);
                            QString l_wfw_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("Word Word %1.%2.%3 <seg type=\"%4\"> %5")
                                                                             .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word)
                                                                             .arg(l_type).arg(l_wfw_text);
                        }
                    }
                    // <interp> --> wfw transliteration/wfw translation/one wfw morphology line inside interpGrp
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "interp"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        // wfw transliteration
                        if(l_type == "transliteration"){
                            l_found_wfw_transliteration = true;
                            QString l_source = l_xml_reader.attributes().value("source").toString();
                            QString l_standard = l_xml_reader.attributes().value("standard").toString();
                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp == QXmlStreamReader::Characters){
                                QString l_trl_text = l_xml_reader.text().toString().trimmed();
                                qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" source=\"%2\" standard=\"%3\"> %4").arg(l_type).arg(l_source).arg(l_standard).arg(l_trl_text);
                            }
                        }
                        // wfw translation
                        else if(l_type == "translation"){
                            l_found_wfw_translation = true;
                            QString l_source = l_xml_reader.attributes().value("source").toString();
                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<interp type=translation> not followed by Characters", 0);
                            QString l_trl_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" source=\"%2\"> %3").arg(l_type).arg(l_source).arg(l_trl_text);
                        }
                        // one wfw morphology line inside interpGrp
                        else if(l_type == "morphology"){
                            QString l_lemma = l_xml_reader.attributes().value("lemma").toString();
                            QString l_pos = l_xml_reader.attributes().value("pos").toString();
                            QString l_ref = l_xml_reader.attributes().value("lemmaRef").toString();

                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<interp type=morphology> not followed by Characters", 0);

                            QString l_form_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" lemma=\"%2\" pos=\"%3\" lemmaRef=\"%4\"> %5")
                                                                             .arg(l_type).arg(l_lemma).arg(l_pos).arg(l_ref).arg(l_form_text);
                        }
                    }
                    // wfw morphology (<interpGrp>)
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "interpGrp"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "morphology"){
                            l_found_wfw_morphology = true;
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interpGrp> start of INRIA morphology block for sloka %1.%2 word %3")
                                                                 .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word);
                            l_indent += QString(" ").repeated(l_indent_count);
                        }
                    }
                    // end of wfw morphology (</interpGrp>)
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "interpGrp"){
                        l_cur_word += 1;
                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << "<interpGrp/> End of INRIA morphology block";
                    }
                    // end of wfw unit </seg>
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "seg"){
                        if(! l_found_wfw_translation) throw M1Env::M1Exception(QString("Word %1.%2.%3 has no wfw translation")
                                                         .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word), 0);
                        if(! l_found_wfw_transliteration) throw M1Env::M1Exception(QString("Word %1.%2.%3 has no wfw transliteration")
                                                         .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word), 0);
                        if(! l_found_wfw_morphology) throw M1Env::M1Exception(QString("Word %1.%2.%3 has no wfw morphology group")
                                                         .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word), 0);

                        qCDebug(g_cat_main).noquote() << l_indent << "<seg/> End of wfw group";
                    }
                    // end of wfw block <div3>
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        if(! l_found_one_wfw) throw M1Env::M1Exception(QString("SLoka %1.%2.%3 has has no wfw units")
                                                         .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word), 0);

                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << "End of wfw block";
                        break; // from the inner wfw while
                    }
                    l_cur_word += 1;
                } // end of wfw while
            }
            // translations block
            else if(l_type == "translations-block"){
                l_found_translations = true;
                qCDebug(g_cat_main).noquote() << l_indent << QString("<div3> start of translation block for sloka %1.%2").arg(l_cur_chapter_number).arg(l_cur_sloka_number);
                l_indent += QString(" ").repeated(l_indent_count);
                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div4"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "translation"){
                            QString l_source = l_xml_reader.attributes().value("source").toString();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<div4 type=\"%1\">").arg(l_type);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
                        QXmlStreamReader::TokenType l_tt_author = l_xml_reader.readNext();
                        if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author type=translation> not followed by Characters", 0);
                        QString l_author_text = l_xml_reader.text().toString().trimmed();
                        qCDebug(g_cat_main).noquote() << l_indent << QString("<author> %1").arg(l_author_text);
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                        QString l_translation_text = skipUntil(l_indent_count, l_indent, l_xml_reader, "div4");
                        qCDebug(g_cat_main).noquote() << l_indent << "translation:" << l_translation_text;
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << "End of translation block";
                        break; // from the inner translation while
                    }
                } // end of translation while
            }
            // commentaries block
            else if(l_type == "commentaries-block"){
                l_found_commentaries = true;
                qCDebug(g_cat_main).noquote() << l_indent << QString("<div3> start of commentaries block for sloka %1.%2").arg(l_cur_chapter_number).arg(l_cur_sloka_number);
                l_indent += QString(" ").repeated(l_indent_count);
                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div4"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "commentary"){
                            QString l_source = l_xml_reader.attributes().value("source").toString();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<div4 type=\"%1\">").arg(l_type);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
                        QXmlStreamReader::TokenType l_tt_author = l_xml_reader.readNext();
                        if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author type=commentary> not followed by Characters", 0);
                        QString l_author_text = l_xml_reader.text().toString().trimmed();
                        qCDebug(g_cat_main).noquote() << l_indent << QString("<author> %1").arg(l_author_text);
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                        QString l_commentary_text = skipUntil(l_indent_count, l_indent, l_xml_reader, "div4");
                        qCDebug(g_cat_main).noquote() << l_indent << "commentary:" << l_commentary_text;
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << "End of commentaries block";
                        break; // from the inner commentaries while
                    }
                } // end of commentaries while
            }
            // end of <div3> (wfw or commentaries or translations blocks)
        }
        // chapter end
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div1"){
            if(! l_found_at_least_one_sloka) throw M1Env::M1Exception(QString("Chapter %1 has 0 Sloka").arg(l_cur_chapter_number), 0);
            qCDebug(g_cat_main).noquote() << l_indent << QString("End of chapter %1").arg(l_cur_chapter_number);
        }
        // Sloka end
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div2"){
            if(! l_found_sk) throw M1Env::M1Exception(QString("Sloka %1.%2 has no sanskrit text").arg(l_cur_chapter_number).arg(l_cur_sloka_number), 0);
            if(! l_found_trnsl) throw M1Env::M1Exception(QString("Sloka %1.%2 has no transliteration text").arg(l_cur_chapter_number).arg(l_cur_sloka_number), 0);
            if(! l_found_wfw) throw M1Env::M1Exception(QString("Sloka %1.%2 has no wfw section").arg(l_cur_chapter_number).arg(l_cur_sloka_number), 0);
            if(! l_found_translations) throw M1Env::M1Exception(QString("Sloka %1.%2 has no translations section").arg(l_cur_chapter_number).arg(l_cur_sloka_number), 0);
            if(! l_found_commentaries) throw M1Env::M1Exception(QString("Sloka %1.%2 has no commentaries section").arg(l_cur_chapter_number).arg(l_cur_sloka_number), 0);

            qCDebug(g_cat_main).noquote() << l_indent << QString("End of Sloka %1.%2").arg(l_cur_chapter_number).arg(l_cur_sloka_number);
        }
    }

    // end of text
    if(! l_found_title) throw M1Env::M1Exception("No title", 0);
    if(! l_found_author) throw M1Env::M1Exception("No author", 0);
    if(! l_found_at_least_one_chapter) throw M1Env::M1Exception("No chapters", 0);

    std::_Exit(0);
}
*/
//qCDebug(g_cat_main).noquote() << "A";
// QXmlStreamAttributes& l_att_2 = l_att_1;
// QByteArray l_att_array(10000, '\0');
//qCDebug(g_cat_main).noquote() << "B";
// QDataStream l_att_stream(&l_att_array, QIODeviceBase::ReadWrite);
// QDataStream& l_att_str_ref = l_att_str;
//qCDebug(g_cat_main).noquote() << "C";
// l_att_stream << dynamic_cast<QList<QXmlStreamAttribute>&>(l_att_1);
// l_att_stream.device()->reset();
/*qCDebug(g_cat_main).noquote() << "D" <<
                l_att_array.size() <<
                QString("[%1]").arg(l_att_array.at(0)) <<
                l_att_stream.status() <<
                l_att_stream.atEnd() <<
                (l_att_stream.device() == nullptr);*/
// QString s = QStringDecoder("utf8").decode(l_att_stream.device()->readAll());
// QString s = QStringDecoder("utf8").decode(l_att_array.data());
// qCDebug(g_cat_main).noquote() << "E";
// qCDebug(g_cat_main).noquote() << s.trimmed();
// qCDebug(g_cat_main).noquote() << l_att_1.value("type");

