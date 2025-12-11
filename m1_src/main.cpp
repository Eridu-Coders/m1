#include <QApplication>
#include <QRegularExpression>
#include <QXmlStreamReader>

#include "m1D_main_window.h"
#include "m1A_env.h"
#include "m1B_store.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_item.h"
#include "m1C_interp.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

Q_LOGGING_CATEGORY(g_cat_main, "m1_main")

// cloc count
// cloc --by-file-by-lang --exclude-content="M1Env::GraphInit::init_base()" --include-lang=C++,"C/C++ Header",Python --fullpath --not-match-d='^build.*|.*stringcase.*' *

QRegularExpression g_re_punc(R"(^(\W*)(\w+)(\W*))");
QRegularExpression g_re_space(R"(\s+)");

void loadEnoch();
int loadGita();
int loadTei();

int main(int argc, char *argv[])
{
    // printf("Xa");
    po::options_description l_desc("Allowed options");
    // printf("Xb");
    l_desc.add_options()
        ("help,h", "produce help message")
        ("load-gita,g", "Load Bhagavad Gita test data")
        ("load-plato,p", "Load The Republic of Plato test data")
        ("load-tei,t", "Loads a TEI file")
        ("reset,r", "Reset (empty) storage")
    ;

    // printf("X");
    po::variables_map l_program_options_vm;
    // printf("Y");
    po::store(po::parse_command_line(argc, argv, l_desc), l_program_options_vm);
    // printf("Z");
    po::notify(l_program_options_vm);
    // printf("T");

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
                                        "interp.dev=true\n"
                                        // "interp.drag=true\n"
                                        "tree_display=false\n"
                                        "passages_panel=false\n"
                                        "main_window=false\n"
                                        "qt.*.debug=false");

    M1MidPlane::Interp::init();

    // loadEnoch();
    // if(l_program_options_vm.count("load-gita")) loadGita();
    if(l_program_options_vm.count("load-plato")) M1Store::GraphInit::init_plato();
    if(l_program_options_vm.count("load-tei")) loadTei();

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

QDataStream& operator<<(QDataStream &l_out, const QXmlStreamAttribute &a){
    QString l_rep = QString("%1: %2").arg(a.qualifiedName()).arg(a.value());
    qCDebug(g_cat_main).noquote() << l_rep;
    l_out << l_rep.toUtf8().constData();
    return l_out;
}

int loadTei(){
    QXmlStreamReader l_xml;
    QFile l_fin_tei("../gitaDnl/bg_final_ex.xml");
    qCDebug(g_cat_main) << "Exists: " << l_fin_tei.exists();

    // the xml stream parse
    if (!l_fin_tei.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for reading:" << l_fin_tei.errorString();
        std::_Exit(0);
    }
    // QTextStream l_in_tei(&l_fin_tei);
    // QString l_txt(l_in_gita.readAll());
    QXmlStreamReader l_xml_reader(&l_fin_tei);
    QString l_title;
    QString l_sub_title;
    QString l_alt_title;
    int l_cur_chapter_number = 0;
    int l_cur_sloka_number = 0;
    QString l_indent = "";
    int l_indent_count = 4;

    while (!l_xml_reader.atEnd()) {
        QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
        QStringView l_tok_name = l_xml_reader.name();
        qCDebug(g_cat_main).noquote() << "Token Type: " << l_tt << l_xml_reader.name();

        // title(s)
        if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "title"){
            QXmlStreamAttributes l_att_1 = l_xml_reader.attributes();
            QString l_type = l_att_1.value("type").toString();

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

            QXmlStreamReader::TokenType l_tt_title = l_xml_reader.readNext();
            if(l_tt_title != QXmlStreamReader::Characters) qFatal() << "<title> not followed by Characters";
            QString l_title_text = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_main).noquote() << QString("<title type=\"%1\"> [%2]").arg(l_type).arg(l_title_text);

            if(l_type == "main") l_title = l_title_text;
            else if(l_type == "sub") l_sub_title = l_title_text;
            else if(l_type == "alt") l_alt_title = l_title_text;

            // qCDebug(g_cat_main).noquote() << l_title << l_sub_title << l_alt_title;
        }
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "teiHeader"){
            qCDebug(g_cat_main).noquote() << QString("<teiHeader/> %1 (%2) [%3]")
                                                 .arg(l_title)
                                                 .arg(l_alt_title)
                                                 .arg(l_sub_title);
        }
        // chapter start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div1"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            l_cur_chapter_number = l_xml_reader.attributes().value("n").toInt();
            qCDebug(g_cat_main).noquote() << QString("<div1 type=%1> Chapter [%2]").arg(l_type).arg(l_cur_chapter_number);
        }
        // sloka start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div2"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            l_cur_sloka_number = l_xml_reader.attributes().value("n").toInt();
            qCDebug(g_cat_main).noquote() << QString("<div1 type=%1> Sloka [%2]").arg(l_type).arg(l_cur_sloka_number);
        }
        // sloka sk text
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "l"){
            QXmlStreamReader::TokenType l_tt_sk = l_xml_reader.readNext();
            if(l_tt_sk != QXmlStreamReader::Characters) qFatal() << "<l> not followed by Characters";
            QString l_sloka_text1 = l_xml_reader.text().toString().trimmed();
            QXmlStreamReader::TokenType l_tt_caesura = l_xml_reader.readNext();
            if(l_tt_caesura != QXmlStreamReader::StartElement && l_xml_reader.name() != "caesura") qFatal() << "No caesura in Sloka";
            l_xml_reader.readNext(); // to pass the automatic EndElement after <caesura/>
            QXmlStreamReader::TokenType l_tt_sk2 = l_xml_reader.readNext();
            if(l_tt_sk2 != QXmlStreamReader::Characters) qFatal() << QString("No Characters after caesura: %1").arg(l_tt_sk2);
            QString l_sloka_text2 = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_main).noquote() << QString("Sloka %1.%2: %3")
                                                 .arg(l_cur_chapter_number)
                                                 .arg(l_cur_sloka_number)
                                                 .arg(QString("%1 । %2").arg(l_sloka_text1).arg(l_sloka_text2));
        }
        // sloka transliteration
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "seg"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            if(l_type == "transliteration"){
                QString l_standard = l_xml_reader.attributes().value("standard").toString();
                QXmlStreamReader::TokenType l_tt_trl = l_xml_reader.readNext();
                if(l_tt_trl != QXmlStreamReader::Characters) qFatal() << "<seg type=transliteration> not followed by Characters";
                QString l_trl_text = l_xml_reader.text().toString().trimmed();
                qCDebug(g_cat_main).noquote() << QString("<seg type=\"%1\" standard=\"%2\"> %3").arg(l_type).arg(l_standard).arg(l_trl_text);
            }
        }
        // wfw start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div3"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            if(l_type == "wfw-block"){
                qCDebug(g_cat_main).noquote() << QString("<div3> start of wfw block for sloka %1.%2").arg(l_cur_chapter_number).arg(l_cur_sloka_number);
                int l_cur_word = 1;
                l_indent += QString(" ").repeated(l_indent_count);
                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "seg"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "wfw-unit"){
                            QXmlStreamReader::TokenType l_tt_wfw = l_xml_reader.readNext();
                            if(l_tt_wfw != QXmlStreamReader::Characters) qFatal() << "<seg type=transliteration> not followed by Characters";
                            QString l_wfw_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<seg type=\"%1\" %2").arg(l_type).arg(l_wfw_text);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "interp"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "transliteration"){
                            QString l_source = l_xml_reader.attributes().value("source").toString();
                            QString l_standard = l_xml_reader.attributes().value("standard").toString();
                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp == QXmlStreamReader::Characters){
                                QString l_trl_text = l_xml_reader.text().toString().trimmed();
                                qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" source=\"%2\" standard=\"%3\"> %4").arg(l_type).arg(l_source).arg(l_standard).arg(l_trl_text);
                            }
                        }
                        else if(l_type == "translation"){
                            QString l_source = l_xml_reader.attributes().value("source").toString();
                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp != QXmlStreamReader::Characters) qFatal() << "<interp type=translation> not followed by Characters";
                            QString l_trl_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" source=\"%2\"> %3").arg(l_type).arg(l_source).arg(l_trl_text);
                        }
                        else if(l_type == "morphology"){
                            QString l_lemma = l_xml_reader.attributes().value("lemma").toString();
                            QString l_pos = l_xml_reader.attributes().value("pos").toString();
                            QString l_ref = l_xml_reader.attributes().value("lemmaRef").toString();

                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp != QXmlStreamReader::Characters) qFatal() << "<interp type=morphology> not followed by Characters";

                            QString l_form_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" lemma=\"%2\" pos=\"%3\" lemmaRef=\"%4\"> %5")
                                                                             .arg(l_type).arg(l_lemma).arg(l_pos).arg(l_ref).arg(l_form_text);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "interpGrp"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "morphology"){
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interpGrp> start of INRIA morphology block for sloka %1.%2 word %3")
                                                                 .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word);
                            l_indent += QString(" ").repeated(l_indent_count);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "interpGrp"){
                        l_cur_word += 1;
                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << QString("<interpGrp/> End of INRIA morphology block");
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << QString("End of wfw block");
                        break; // from the inner wfw while
                    }
                }
            }
        }
    }

    std::_Exit(0);
}

