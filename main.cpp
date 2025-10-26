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

QRegularExpression g_re_punc(R"(^(\W*)(\w+)(\W*))");
QRegularExpression g_re_space(R"(\s+)");

void loadEnoch();
int loadGita();

int main(int argc, char *argv[])
{
    po::options_description l_desc("Allowed options");
    l_desc.add_options()
        ("help,h", "produce help message")
        ("load-gita,g", "Load Bhagavad Gita test data")
        ("load-plato,p", "Load The Republic of Plato")
        ("reset,r", "Reset (empty) storage")
    ;

    po::variables_map l_program_options_vm;
    po::store(po::parse_command_line(argc, argv, l_desc), l_program_options_vm);
    po::notify(l_program_options_vm);

    M1Env::M1EnvStatic::init();
    M1Env::M1EnvStatic::setNormalFilter("*.debug=true\n"
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
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.item_type");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.special_item");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv1.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.constructors");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.iterators");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.test");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("store.storage");

    if(l_program_options_vm.count("reset")) M1Store::Storage::storeSetUp(true);
    else M1Store::Storage::storeSetUp(false);

    M1Env::M1EnvStatic::setNormalFilter("*.debug=true\n"
                                        "store.*=false\n"
                                        "lv0.*=false\n"
                                        "lv1.*=false\n"
                                        "lv2.*=false\n"
                                        "interp.*=false\n"
                                        "tree_display=false\n"
                                        "passages_panel=false\n"
                                        "qt.*.debug=false");

    M1MidPlane::Interp::init();

    // loadEnoch();
    if(l_program_options_vm.count("load-gita")) loadGita();
    if(l_program_options_vm.count("load-plato")) M1Store::GraphInit::init_plato();

    QApplication a(argc, argv);
    M1UI::MainWindow w;
    w.show();
    int l_ret = a.exec();

    M1Store::Storage::storeShutDown();
    qCDebug(g_cat_main) << "App ends";
    M1_FUNC_EXIT
    M1Env::M1EnvStatic::close();

    printf("Hurrah! No Core Dump ... Returning %d\n", l_ret);
    return l_ret;
}

int loadGita(){
    M1_FUNC_ENTRY(g_cat_main, QString("Loading Gita passage"))

    // perform the load only if not already done
    if(M1Store::Storage::menmonic_exists("GITAL")){
        M1_FUNC_EXIT
        return 0;
    }
    else
        M1Store::Storage::getNewSpecialNoItem(M1Store::SI_IS_TYPE, "GITAL", nullptr);

    // lexicon root
    // M1Store::Item_lv2* l_words_root = M1Store::Item_lv2::getExisting(M1Env::FORM_SIID);
    // texts root
    // M1Store::Item_lv2* l_text_root = M1Store::Item_lv2::getExisting(M1Env::TEXT_SIID);
    // person roots
    M1Store::Item_lv2* l_prabhupada_person = M1Store::Item_lv2::getExisting("PPRAB");
    M1Store::Item_lv2* l_sivananda_person = M1Store::Item_lv2::getExisting("PSIVA");
    M1Store::Item_lv2* l_gambirananda_person = M1Store::Item_lv2::getExisting("PGAMB");

    // Text and its sections vertex
    M1Store::Item_lv2* l_gita_text = nullptr;
    M1Store::Item_lv2* l_gita_sections = nullptr;

    // variables for sloka creation
    M1Store::Item_lv2* l_last_edge = nullptr;           // last edge of the Gita text node under construction
    M1Store::Item_lv2* l_current_section = nullptr;     // current section under construction (belonging to l_gita_sections)
    M1Store::Item_lv2* l_sections_last_edge = nullptr;  // last section of of the edge folder (l_gita_sections)
    M1Store::Item_lv2* l_cursec_last_edge = nullptr;    // last section of the current section
    M1Store::Item_lv2* l_cur_lex_item = nullptr;        // current lexical item under construction within a w tag
    M1Store::Item_lv2* l_section_begin_edge = nullptr;  // The edge linking the current section to its first word

    bool l_store_word_request = false;  // true --> next characher token must be stored in l_store_word
    bool l_fresh_sloka = false;         // true --> sloka just created (s tag) so next word encountered must be maked as its begining

    // the frequency map to determine which key combinations are encountered
    QMap<QString, int> l_freq_map;

    // text of the last encountered character token
    QString l_last_char;
    // type of the last interp and span tags encountered (bhashya, translation, wfw, etc.)
    QString l_last_interp, l_last_span;
    // the char token of the current w tag
    QString l_store_word;
    // transliteration text for the current w tag
    QString l_transliteration_text;
    // Dictionary reference for the current w tag
    QString l_dict_ref_name;
    QString l_dict_ref_string;
    // lemma and xml:id for the current w tag
    QString l_lemma;
    QString l_xml_id;
    // from/to for span-wfw-tu tags
    QString l_wfw_from, l_wfw_to;
    // source and translation for interp-wfw-tr, interp-translation and interp-bhashya tags
    QString l_source;
    // QString l_source_txt;
    QString l_wfw_prabhupada;
    QString l_wfw_sivananda;
    // url label
    QString l_url_label;
    // dictionary of all created lexical items encounterd so far (to avoid creating duplicates)
    QMap<QString, M1Store::Item_lv2*> l_word_2_lex;
    // dictionary of all created occurence edges encounterd so far
    QMap<QString, M1Store::Item_lv2*> l_id_2_occ;

    // XMl input file
    QFile l_fin_gita("../BG12-TEI+.xml");
    qCDebug(g_cat_main) << "Exists: " << l_fin_gita.exists();

    // the xml stream parse
    l_fin_gita.open(QIODeviceBase::ReadOnly);
    QTextStream l_in_gita(&l_fin_gita);
    QString l_txt(l_in_gita.readAll());
    QXmlStreamReader l_xml_reader(l_txt);

    // parse XML file
    while (!l_xml_reader.atEnd()) {
        l_xml_reader.readNext();

        // vector of attributes (for debug display)
        QVector<QXmlStreamAttribute> l_att_vec = l_xml_reader.attributes();
        // string representation of attributes
        QStringList l_att_string;
        for(int i=0; i<l_att_vec.size(); i++)
            l_att_string.append(QString("%1: %2").arg(l_att_vec.at(i).name()).arg(l_att_vec.at(i).value()));
        QString l_att = l_att_string.join(", ");

        // current token type (StartElement, Characters, etc)
        QString l_token = l_xml_reader.tokenString();
        // current token name (interp, w, s, l, etc.)
        QString l_name = l_xml_reader.name().toString();
        // text of current token (non-empty only if Characters)
        QString l_txt = l_xml_reader.text().toString().trimmed();
        // exclude empy characters tokens
        if( !(l_token == "Characters" && l_txt.length() == 0) ){
            // key construction
            QString l_key = l_token; // By default: token type
            if(l_token == "StartElement")
                // StartElement name + type (if any)
                l_key = l_xml_reader.attributes().hasAttribute("type") \
                                    ? l_name + "-" + l_xml_reader.attributes().value("type").toString() : l_name;
            else if(l_token == "EndElement")
                // EndElement: End + name
                l_key = "End-" + l_name;
            // frequency count of key configs
            l_freq_map[l_key] = l_freq_map.contains(l_key) ? l_freq_map[l_key] + 1 : 1;

            // debug print
            qCDebug(g_cat_main) << QString("[%5] %1%2%3%4")
                                       .arg(l_xml_reader.lineNumber())
                                       .arg(l_token.length() > 0 ? " " + l_token : "")
                                       .arg(l_name.length() > 0 ? " " + l_name : "")
                                       .arg(l_att.length() > 0 ? QString(" [%1]").arg(l_att) : "")
                                       .arg(l_key, 30).toUtf8().constData()
                                << l_txt.toUtf8().constData();

            // store type of current interp tag
            if(l_token == "StartElement" && l_name == "interp")
                l_last_interp = l_xml_reader.attributes().value("type").toString();
            if(l_token == "StartElement" && l_name == "span")
                l_last_span = l_xml_reader.attributes().value("type").toString();
            // no else here bc the case above must be processed below (interp-wfw-tr)
            // store character tokens (+ into l_store_word if required)
            if(l_key == "Characters"){
                l_last_char = l_txt;
                if(l_store_word_request){
                    l_store_word = l_txt;
                    l_store_word_request = false;
                }
            }
            // creation of the text root node
            else if(l_key == "End-title"){
                QString l_title = l_last_char;
                qCDebug(g_cat_main) << QString("Found Title [%1] - Creating text root").arg(l_title);
                // Text and its sections vertex
                l_gita_text = M1Store::Item_lv2::getNew(
                    M1Store::FULL_VERTEX,                      // category & attributes (flags)
                    //M1Store::ItemType(M1Env::TEXT_SIID), // type
                    l_title.toUtf8().constData()               // label
                );
                l_gita_text->setType(M1Env::TEXT_SIID);
                // l_gita_text->linkTo(l_text_root, "BLNGS", nullptr, true);

                l_gita_sections = M1Store::Item_lv2::getNew(
                    M1Store::FULL_VERTEX,                            // category & attributes (flags)
                    //M1Store::ItemType(M1Store::FOLDER_SIID),   // type folder
                    "Gītā Sloka"                                     // label
                );
                l_gita_sections->setType(M1Store::FOLDER_SIID);
                l_last_edge = l_gita_text->linkTo(l_gita_sections, "OWNS_", nullptr, false);
            }
            // creation of each sloka node in the sections folder
            else if(l_key == "End-ref"){
                QString l_sloka_ref = l_last_char;
                qCDebug(g_cat_main) << QString("Found Sloka Ref [%1] - Creating sloka node").arg(l_sloka_ref);
                l_current_section = M1Store::Item_lv2::getNew(
                    M1Store::FULL_VERTEX,                               // category & attributes (flags)
                    // M1Store::ItemType(M1Env::TEXT_SECTION_SIID),  // type
                    l_sloka_ref.toUtf8().constData()                    // label
                );
                l_current_section->setType(M1Env::TEXT_CHUNK_SIID);
                l_sections_last_edge = l_gita_sections->linkTo(l_current_section, "OWNS_", l_sections_last_edge, false);
                l_cursec_last_edge = nullptr;
                l_fresh_sloka = true;
            }
            // Sloka lines in Devanagari
            else if(l_key == "End-l"){
                QString l_sloka_line = l_last_char;
                qCDebug(g_cat_main) << QString("Found Sloka line [%1] - Adding it to the current sloka").arg(l_sloka_line);
                M1Store::Item_lv2* l_new_sloka_line = M1Store::Item_lv2::getNew(
                    M1Store::FULL_VERTEX,                               // category & attributes (flags)
                    // M1Store::ItemType(M1Env::TEXT_SLOKA_LINE_SIID),     // type
                    l_sloka_line.toUtf8().constData()                   // label
                );
                l_new_sloka_line->setType(M1Env::TEXT_SLOKA_LINE_SIID);
                l_cursec_last_edge = l_current_section->linkTo(l_new_sloka_line, "OWNS_", l_cursec_last_edge, false);
            }
            // start of word --> request storage of next character token
            else if(l_key == "w"){
                l_store_word_request = true;
                l_lemma = l_xml_reader.attributes().hasAttribute("lemma") \
                                      ? l_xml_reader.attributes().value("lemma").toString() : "<no lemma>";
                l_xml_id = l_xml_reader.attributes().hasAttribute("xml:id") \
                               ? l_xml_reader.attributes().value("xml:id").toString() : "<no id>";
                qCDebug(g_cat_main) << QString("Found lemma [%1] id: [%2] - Will Add it to the current word")
                                           .arg(l_lemma).arg(l_xml_id);

            }
            // lexical item creation
            else if(l_key == "End-w"){
                // capture punctuation, if any
                QRegularExpressionMatch l_match = g_re_punc.match(l_store_word);
                QString l_punc_left, l_punc_right;
                QString l_bare_word = l_store_word;
                if(l_match.hasMatch()){
                    l_punc_left = l_match.captured(1).replace(g_re_space, "");
                    l_bare_word = l_match.captured(2);
                    l_punc_right = l_match.captured(3).replace(g_re_space, "");
                }
                qCDebug(g_cat_main) << QString("Found word [%1-%2-%3] - Adding it to the text and current sloka")
                                           .arg(l_punc_left).arg(l_bare_word).arg(l_punc_right);

                // find node of current word, if it exist or create it
                if(l_word_2_lex.contains(l_bare_word)) l_cur_lex_item = l_word_2_lex[l_bare_word];
                else {
                    // word creation (inflected form)
                    l_cur_lex_item = M1Store::Item_lv2::getNew(
                        M1Store::FULL_VERTEX,                           // category & attributes (flags)
                        // M1Store::ItemType(M1Env::TEXT_WORD_SIID), // type
                        l_bare_word.toUtf8().constData()                // label
                    );
                    l_cur_lex_item->setType(M1Env::FORM_SIID);
                    l_word_2_lex[l_bare_word] = l_cur_lex_item;
                    // l_cur_lex_item->linkTo(l_words_root, "BLNGS", nullptr, true);

                    // lemma creation
                    M1Store::Item_lv2* l_lemma_node = M1Store::Item_lv2::getNew(
                        M1Store::FULL_VERTEX,                           // category & attributes (flags)
                        // M1Store::ItemType(M1Env::TEXT_LEMMA_SIID),   // type
                        l_lemma.toUtf8().constData()                    // label
                    );
                    l_lemma_node->setType(M1Env::LEMMA_SIID);
                    l_cur_lex_item->linkTo(l_lemma_node, "BLNGS", nullptr, true);
                }
                // create occurrence edge
                l_last_edge = l_gita_text->linkTo(l_cur_lex_item, "OWNS_", l_last_edge, false);
                // store occurence edge for translation unit reference
                l_id_2_occ[l_xml_id] = l_last_edge;
                // store transliteration text
                l_cur_lex_item->setFieldEdge(l_transliteration_text, M1Env::TEXT_WORD_TRANSLIT_SIID);
                // store dict-ref, if any
                if(l_dict_ref_name.length() > 0 && l_dict_ref_name == "INRIA"){
                    M1Store::Item_lv2* l_field_edge = l_cur_lex_item->setFieldEdge(
                        QString("%1/%2").arg(l_dict_ref_name).arg(l_dict_ref_string),
                        M1Env::TEXT_WORD_DICT_REF_SIID);
                    l_field_edge->setType(M1Env::TEXT_WORD_DREF_INRIA_SIID);
                }
                // M1Store::Storage::getSpecialItemPointer(M1Env::TEXT_WORD_DREF_INRIA_SIID));

                if(l_fresh_sloka){
                    // sloka just created --> link to first word
                    qCDebug(g_cat_main) << QString("Start of Sloka : first word [%1]").arg(l_last_edge->dbgShort(2));
                    l_section_begin_edge = l_current_section->linkTo(l_last_edge, M1Env::TW_SECTION_2_OCC_BEGIN_SIID, nullptr, false);
                    l_fresh_sloka = false;
                }
            }
            // end of sloka --> link to last word
            else if(l_key == "End-s"){
                qCDebug(g_cat_main) << QString("End of Sloka : last word [%1]").arg(l_last_edge->dbgShort(2));
                l_current_section->linkTo(l_last_edge, M1Env::TW_SECTION_2_OCC_END_SIID, l_section_begin_edge, false);
                for(QMap<QString, M1Store::Item_lv2*>::Iterator l_it = l_id_2_occ.begin(); l_it != l_id_2_occ.end(); l_it++)
                    qCDebug(g_cat_main) << QString("Occurence [%1] --> [%2]").arg(l_it.key(), 12).arg(l_it.value()->dbgShort(2));
                l_id_2_occ.clear();
            }
            // end of transliteration tag --> store in current lexical element
            else if(l_key == "End-interp" && l_last_interp == "transliteration"){
                l_transliteration_text = l_last_char;
                qCDebug(g_cat_main) << QString("Found transliteration [%1] - Will Add it to the current word")
                                           .arg(l_transliteration_text.left(20));
            }
            // Dictionary reference start
            else if(l_key == "dict-ref"){
                l_dict_ref_name = l_xml_reader.attributes().hasAttribute("dict-name") \
                                              ? l_xml_reader.attributes().value("dict-name").toString() : "<no name>";
                qCDebug(g_cat_main) << QString("Found Dict Ref name [%1] - Will Add it to the current word")
                                           .arg(l_dict_ref_name.left(20));
            }
            // Dictionary reference end
            else if(l_key == "End-dict-ref"){
                l_dict_ref_string = l_last_char;
                qCDebug(g_cat_main) << QString("Found Dict Ref string [%1] - Will Add it to the current word")
                                           .arg(l_dict_ref_string.left(20));
            }
            // Start wfw span
            else if(l_key == "span-wfw-tu"){
                l_wfw_from = l_xml_reader.attributes().hasAttribute("from") \
                                      ? l_xml_reader.attributes().value("from").toString() : "<no from>";
                l_wfw_to = l_xml_reader.attributes().hasAttribute("to") \
                             ? l_xml_reader.attributes().value("to").toString() : "<no to>";
                qCDebug(g_cat_main) << QString("Found span-wfw-tu [%1 --> %2] - Will Add it to the current section")
                                           .arg(l_wfw_from).arg(l_wfw_to);
            }
            // source for various interps
            else if(l_key == "interp-wfw-tr" || l_key == "interp-translation"|| l_key == "interp-bhashya"){
                l_source = l_xml_reader.attributes().hasAttribute("source") \
                               ? l_xml_reader.attributes().value("source").toString() : "<no source>";
                qCDebug(g_cat_main) << QString("Found source [%1] - Will Add it to the current section")
                                           .arg(l_source);
            }
            // end of various interp
            else if(l_key == "End-interp" && (l_last_interp == "wfw-tr" || l_last_interp == "translation" || l_last_interp == "bhashya")){
                QString l_source_txt = l_last_char;
                qCDebug(g_cat_main) << QString("Found source text [%1] - Will Add it to the current section or WfW unit")
                                           .arg(l_source_txt);
                if(l_last_interp == "wfw-tr"){
                    if(l_source == "prabhupada")
                        l_wfw_prabhupada = l_source_txt;
                    else if(l_source == "sivananda")
                        l_wfw_sivananda = l_source_txt;
                }
                else{
                    M1Env::SpecialItemID l_source_type = l_source == "prabhupada" ? M1Env::TEXT_WFW_PRABUPADA_SIID :
                                                             (l_source == "sivananda" ? M1Env::TEXT_WFW_SIVANANDA_SIID :
                                                                  M1Env::TEXT_WFW_GAMBIRANANDA_SIID);
                    // sloka translation nodes
                    if(l_last_interp == "translation"){
                        QString l_translation_text = l_source_txt;
                        qCDebug(g_cat_main) << QString("Found translation [%1] - Adding it to the current sloka").arg(l_translation_text.left(20));
                        M1Store::Item_lv2* l_new_transl = M1Store::Item_lv2::getNew(
                            M1Store::FULL_VERTEX,                                  // category & attributes (flags)
                            // M1Store::ItemType(M1Env::TEXT_SLOKA_TRANSLATION_SIID), // type
                            l_translation_text.toUtf8().constData()                // label
                        );
                        l_new_transl->setType(M1Env::TEXT_SLOKA_TRANSLATION_SIID);
                        // Add source type
                        l_new_transl->setType(l_source_type);
                        l_cursec_last_edge = l_current_section->linkTo(l_new_transl, "OWNS_", l_cursec_last_edge, false);
                        // attribution
                        if(l_source == "prabhupada")
                            l_prabhupada_person->linkTo(l_new_transl, M1Store::TEXT_WROTE_SIID, nullptr, false);
                        else if(l_source == "sivananda")
                            l_sivananda_person->linkTo(l_new_transl, M1Store::TEXT_WROTE_SIID, nullptr, false);
                        else
                            l_gambirananda_person->linkTo(l_new_transl, M1Store::TEXT_WROTE_SIID, nullptr, false);
                    }
                    // sloka bhashya nodes
                    else if(l_last_interp == "bhashya"){
                        QString l_bhashya_text = l_source_txt;
                        qCDebug(g_cat_main) << QString("Found bhashya [%1] - Adding it to the current sloka").arg(l_bhashya_text.left(20));
                        M1Store::Item_lv2* l_new_bhashya = M1Store::Item_lv2::getNew(
                            M1Store::FULL_VERTEX,                               // category & attributes (flags)
                            // M1Store::ItemType(M1Env::TEXT_SLOKA_BHASHYA_SIID),  // type
                            l_bhashya_text.toUtf8().constData()                 // label
                        );
                        l_new_bhashya->setType(M1Env::TEXT_SLOKA_BHASHYA_SIID);
                        // Add source type
                        l_new_bhashya->setType(l_source_type);
                        l_cursec_last_edge = l_current_section->linkTo(l_new_bhashya, "OWNS_", l_cursec_last_edge, false);
                        // attribution
                        if(l_source == "prabhupada")
                            l_prabhupada_person->linkTo(l_new_bhashya, M1Store::TEXT_WROTE_SIID, nullptr, false);
                        else if(l_source == "sivananda")
                            l_sivananda_person->linkTo(l_new_bhashya, M1Store::TEXT_WROTE_SIID, nullptr, false);
                        else
                            l_gambirananda_person->linkTo(l_new_bhashya, M1Store::TEXT_WROTE_SIID, nullptr, false);
                    }
                }
            }
            // end of wfw span
            else if(l_key == "End-span" && l_last_span == "wfw-tu"){
                qCDebug(g_cat_main) << QString("End of WfW translation span [%1 --> %2 (%3 - %4)] - building it")
                                           .arg(l_wfw_from).arg(l_wfw_to).arg(l_wfw_prabhupada).arg(l_wfw_sivananda);
                M1Store::Item_lv2* l_from = l_id_2_occ[l_wfw_from];
                M1Store::Item_lv2* l_to = l_id_2_occ[l_wfw_to];

                // translation unit label
                QString l_label = l_from == l_to ? l_from->getTarget_lv2()->text() :
                                      QString("%1 - %2") \
                                          .arg(l_from->getTarget_lv2()->text())
                                          .arg(l_to->getTarget_lv2()->text());
                // create translation unit
                // TEXT_SECTION_SIID TEXT_WFW_UNIT_SIID
                M1Store::Item_lv2* l_unit = M1Store::Item_lv2::getNew(
                    M1Store::FULL_VERTEX,                              // category & attributes (flags)
                    // M1Store::ItemType(M1Env::TEXT_SECTION_SIID), // type
                    l_label.toUtf8().constData()                       // label
                );
                l_unit->setType(M1Env::TEXT_CHUNK_SIID);
                // additional type TEXT_WFW_UNIT_SIID
                l_unit->setType(M1Env::TEXT_WFW_UNIT_SIID);
                // connect to begin/end occurence edges
                l_unit->linkTo(l_to, M1Env::TW_SECTION_2_OCC_END_SIID, nullptr, false);
                l_unit->linkTo(l_from, M1Env::TW_SECTION_2_OCC_BEGIN_SIID, nullptr, false);
                // connect to current sloka
                l_cursec_last_edge = l_current_section->linkTo(l_unit, "OWNS_", l_cursec_last_edge, false);
                // add Prabhupada translation, if any
                if(l_wfw_prabhupada.length() > 0){
                    M1Store::Item_lv2* l_edge_field = l_unit->setFieldEdge(
                        l_wfw_prabhupada,
                        M1Env::TEXT_WFW_TRANSL_SIID);
                    l_edge_field->setType(M1Env::TEXT_WFW_PRABUPADA_SIID);
                    // M1Store::Storage::getSpecialItemPointer(M1Env::TEXT_WFW_PRABUPADA_SIID));
                    qCDebug(g_cat_main) << QString("Setting Prabhupada WfW: %1").arg(l_wfw_prabhupada);
                }
                // add Sivanande translation, if any
                if(l_wfw_sivananda.length() > 0){
                    M1Store::Item_lv2* l_edge_field = l_unit->setFieldEdgeForce(
                        l_wfw_sivananda,
                        M1Env::TEXT_WFW_TRANSL_SIID);
                    l_edge_field->setType(M1Env::TEXT_WFW_SIVANANDA_SIID);
                    // M1Store::Storage::getSpecialItemPointer(M1Env::TEXT_WFW_SIVANANDA_SIID));
                    qCDebug(g_cat_main) << QString("Setting Sivanande WfW: %1").arg(l_wfw_sivananda);
                }
                l_wfw_prabhupada = "";
                l_wfw_sivananda = "";
            }
            // start of an URL tag
            else if(l_key == "urlLink"){
                l_url_label = l_xml_reader.attributes().hasAttribute("label") \
                                    ? l_xml_reader.attributes().value("label").toString() : "<no label>";
                qCDebug(g_cat_main) << QString("Found urlLink label = [%1]").arg(l_url_label);
            }
            // End of an URL tag
            else if(l_key == "End-urlLink"){
                QString l_url = l_last_char;
                qCDebug(g_cat_main) << QString("End urlLink label = [%1] / url = [%2]").arg(l_url_label).arg(l_url);
                M1Store::Item_lv2* l_new_url = M1Store::Item_lv2::getNew(
                    M1Store::FULL_VERTEX,                          // category & attributes (flags)
                    // M1Store::ItemType(M1Env::TEXT_URL_LINK_SIID),  // type
                    (l_url_label).toUtf8().constData()             // label
                );
                l_new_url->setType(M1Env::TEXT_URL_LINK_SIID);

                l_current_section->linkTo(l_new_url, M1Env::OWNS_SIID, nullptr, false);

                bool l_res = l_new_url->setFieldEdge(l_url, M1Env::TEXT_URL_LINK_SIID);
                qCDebug(g_cat_main) << QString("Url field created: %1").arg(l_res);
            }
        }
    }
    // Display XML parsing errors, if any
    if (l_xml_reader.hasError()) {
        qCDebug(g_cat_main) << "XML errors!" << l_xml_reader.errorString();
    }
    // display key-config frequency list
    for(QMap<QString, int>::ConstIterator l_it = l_freq_map.begin(); l_it != l_freq_map.constEnd(); l_it++)
        qCDebug(g_cat_main) << QString("%1 --> %2").arg(l_it.key(), 25).arg(l_it.value());
    M1_FUNC_EXIT
    return 1;
}

void loadEnoch(){
    M1_FUNC_ENTRY(g_cat_main, QString("Loading Enoch passage"))

    M1Env::M1EnvStatic::setSilentMode(true);

    //--------------------------------- words -----------------------------------------------------------
    // lexicon building
    M1Store::Item_lv2* l_words_root = M1Store::Item_lv2::getExisting(M1Env::FORM_SIID);
    QFile l_fin_words("../Enoch-words.txt");
    l_fin_words.open(QFile::ReadOnly | QFile::Text);
    QTextStream l_in_words(&l_fin_words);
    QString l_txt_words(l_in_words.readAll());
    QStringList l_lex_entries = l_txt_words.split("\n");
    std::map<QString, M1Store::Item_lv2*> l_lex_2_word;
    for ( const auto& l_lex : l_lex_entries ){
        qCDebug(g_cat_silence) << "Lexical element: " + l_lex;
        M1Store::Item_lv2* l_lex_item = M1Store::Item_lv2::getNew(
            M1Store::FULL_VERTEX,                           // category & attributes (flags)
            // M1Store::ItemType(M1Env::TEXT_WORD_SIID), // type
            l_lex.toUtf8().constData()                      // label
            );
        l_lex_item->setType(M1Env::FORM_SIID);
        l_lex_item->linkTo(l_words_root, "BLNGS", nullptr, true);
        l_lex_2_word[l_lex] = l_lex_item;
    }

    // Text and its sections vertex
    M1Store::Item_lv2* l_enoch_text = M1Store::Item_lv2::getNew(
        M1Store::FULL_VERTEX,                      // category & attributes (flags)
        // M1Store::ItemType(M1Env::TEXT_SIID), // type
        "Book of Enoch, version B"                 // label
        );
    l_enoch_text->setType(M1Env::TEXT_SIID);
    M1Store::Item_lv2* l_text_root = M1Store::Item_lv2::getExisting(M1Env::TEXT_SIID);
    l_enoch_text->linkTo(l_text_root, "BLNGS", nullptr, true);

    M1Store::Item_lv2* l_enoch_sections = M1Store::Item_lv2::getNew(
        M1Store::FULL_VERTEX,  // category & attributes (flags)
        //M1Store::ItemType(),   // type (none)
        "Sentences of EB"      // label
        );
    l_enoch_sections->linkTo(l_enoch_text, "BLNGS", nullptr, true);

    // Occurence edges and their connexions
    QFile l_fin("../Enoch B.txt");
    l_fin.open(QFile::ReadOnly | QFile::Text);
    QTextStream l_in(&l_fin);
    QString l_txt(l_in.readAll());
    QStringList l_words = l_txt.split(" ");
    M1Store::Item_lv2* l_last_edge = nullptr;
    M1Store::Item_lv2* l_current_section = nullptr;
    M1Store::Item_lv2* l_sections_last_edge = nullptr;
    M1Store::Item_lv2* l_cursec_last_edge = nullptr;

    int l_section_count = 1;
    for ( const auto& l_word : l_words ){
        // Extract punctuation from word, if any
        QRegularExpressionMatch l_match = g_re_punc.match(l_word);
        QString l_punc_left, l_punc_right;
        QString l_bare_word = l_word;
        if(l_match.hasMatch()){
            l_punc_left = l_match.captured(1).replace(g_re_space, "");
            l_bare_word = l_match.captured(2);
            l_punc_right = l_match.captured(3).replace(g_re_space, "");
        }

        // Create the word occurence
        M1Store::Item_lv2* l_the_lex_word = l_lex_2_word[l_bare_word];
        l_last_edge = l_enoch_text->linkTo(l_the_lex_word, M1Env::OCCUR_SIID, l_last_edge, false);
        l_the_lex_word->linkTo(l_last_edge, M1Env::OCCUR_SIID);

        // Create a new section (sentence) if appropriate
        if(l_current_section == nullptr){
            l_current_section = M1Store::Item_lv2::getNew(
                M1Store::FULL_VERTEX,                                                 // category & attributes (flags)
                //M1Store::ItemType(M1Env::TEXT_SECTION_SIID),                    // type
                QString("EB Sentence %1").arg(l_section_count++).toUtf8().constData() // label
                );
            l_current_section->setType(M1Env::TEXT_CHUNK_SIID);
            l_sections_last_edge = l_enoch_sections->linkTo(l_current_section, "OWNS_", l_sections_last_edge, false);
            l_cursec_last_edge = l_current_section->linkTo(l_last_edge, M1Store::TW_SECTION_2_OCC_BEGIN_SIID);
        }
        else
            // Terminates the current section (sentence) if appropriate
            if(l_punc_right.contains(".")){
                l_current_section->linkTo(l_last_edge, M1Store::TW_REV_SECTION_2_OCC_END_SIID, l_cursec_last_edge, false);
                l_current_section = nullptr;
                l_cursec_last_edge = nullptr;
            }

        qCDebug(g_cat_silence) << QString("[%1] %2 [%3]")
                                      .arg(l_punc_left)
                                      .arg(l_bare_word)
                                      .arg(l_punc_right).toUtf8().constData();
    }

    M1Env::M1EnvStatic::setSilentMode(false);
    M1_FUNC_EXIT
}
