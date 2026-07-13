#include <QXmlStreamReader>
#include <QElapsedTimer>
#include <QRegularExpression>

// #include <iostream>

#include "m1B_tei_interface.h"
#include "m1A_env.h"
#include "m1A_constants.h"
#include "m1B_graph_init.h"
// #include "m1B_lv2_item.h"
// #include "m1B_lv2_iterators.h"

// g_cat_tei
Q_LOGGING_CATEGORY(g_cat_tei, "tei_interface")

void M1Store::TEIInterface::init(){}
void M1Store::TEIInterface::loadTei1(const QString& p_file_path){
    if(QFile::exists(p_file_path)){
        loadTeiInternal1(p_file_path);
    }
    else qCDebug(g_cat_tei) << p_file_path << " does not exist";
}
void M1Store::TEIInterface::loadTeiGeneric(const QString& p_file_path){
    if(QFile::exists(p_file_path)){
        loadTeiInternal(p_file_path);
    }
    else qCDebug(g_cat_tei) << p_file_path << " does not exist";
}

/**
 * @brief M1Store::TEIInterface::loadTeiInternal1
 * @param p_full_load
 */
void M1Store::TEIInterface::loadTeiInternal1(const QString& p_file_path, bool p_validate_only){
    // QXmlStreamReader l_xml;
    QFile l_fin_tei(p_file_path);
    qCDebug(g_cat_tei) << "Exists: " << l_fin_tei.exists();

    // std::cout << "msg";
    // std::cout << "msg" << QString("Loading File %1").arg(p_file_path).toUtf8().constData();
    printf("%s\n", QString("Loading File %1").arg(p_file_path).toUtf8().constData());

    // the xml stream parse
    if (!l_fin_tei.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for reading:" << l_fin_tei.errorString();
        std::_Exit(0);
    }
    QXmlStreamReader l_xml_reader(&l_fin_tei);
    QString l_title;
    QString l_sub_title;
    QString l_alt_title;
    QString l_author_main;
    cm_cur_chapter_number = 0;
    cm_cur_sloka_number = 0;
    cm_indent = "";
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
        qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

        // text title(s)
        if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "title"){
            QXmlStreamAttributes l_att_1 = l_xml_reader.attributes();
            QString l_type = l_att_1.value("type").toString();

            QXmlStreamReader::TokenType l_tt_title = l_xml_reader.readNext();
            if(l_tt_title != QXmlStreamReader::Characters) throw M1Env::M1Exception("<title> not followed by Characters", 1000);
            QString l_title_text = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<title type=\"%1\"> [%2]").arg(l_type).arg(l_title_text);

            if(l_type == "main") {
                l_title = l_title_text;
                l_found_title  = true;
            }
            else if(l_type == "sub") l_sub_title = l_title_text;
            else if(l_type == "alt") l_alt_title = l_title_text;

            // qCDebug(g_cat_tei).noquote() << l_title << l_sub_title << l_alt_title;
        }
        // text author(s): 1 main + any number of secondaries
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
            l_found_author = true;
            QXmlStreamReader::TokenType l_tt_author = l_xml_reader.readNext();
            if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author> not followed by Characters", 1001);
            l_author_main = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<author> main : %1").arg(l_author_main);

            cm_indent += QString(" ").repeated(l_indent_count);

            while (!l_xml_reader.atEnd()) {
                QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                QStringView l_tok_name = l_xml_reader.name();
                qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "persName"){
                    QXmlStreamReader::TokenType l_tt_name = l_xml_reader.readNext();
                    if(l_tt_name != QXmlStreamReader::Characters) throw M1Env::M1Exception("<persName> not followed by Characters", 1001);
                    QString l_author_name = l_xml_reader.text().toString().trimmed();

                    QXmlStreamReader::TokenType l_tt_role_1 = l_xml_reader.readNext();
                    if(l_tt_role_1 != QXmlStreamReader::StartElement || l_xml_reader.name() != "roleName") throw M1Env::M1Exception("<roleName> not found", 1001);
                    QXmlStreamReader::TokenType l_tt_role_2 = l_xml_reader.readNext();
                    if(l_tt_role_2 != QXmlStreamReader::Characters) throw M1Env::M1Exception("<roleName> not followed by Characters", 1001);
                    QString l_author_role = l_xml_reader.text().toString().trimmed();

                    cm_author_list.append(AuthorTEI(l_author_name, l_author_role));
                    qCDebug(g_cat_tei).noquote() << cm_indent << QString("<persName> 2ndary author : %1/%2").arg(l_author_name).arg(l_author_role);
                }
                else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                    qCDebug(g_cat_tei).noquote() << cm_indent << QString("<author/> %1 (%2) [%3] author: %4")
                    .arg(l_title)
                        .arg(l_alt_title)
                        .arg(l_sub_title)
                        .arg(l_author_main);
                    break; // from <author> inner while
                }
            }
        }
        // end of TEI header
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "teiHeader"){
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<teiHeader/> %1 (%2) [%3] author: %4")
            .arg(l_title)
                .arg(l_alt_title)
                .arg(l_sub_title)
                .arg(l_author_main);

            // + m_author_list as built above
            // with lexicon --> p_with_lexicon = true
            create_text(l_title, l_alt_title, l_sub_title, l_author_main, true);
        }
        // lexicon or chapter start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div1"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            if(l_type == "chapter"){
                // chapter start
                cm_cur_chapter_number = l_xml_reader.attributes().value("n").toInt();
                l_found_at_least_one_chapter = true;
                l_found_at_least_one_sloka = false;
                qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div1 type=%1> Chapter %2 start").arg(l_type).arg(cm_cur_chapter_number);

                M1Store::TEIInterface::cm_cur_chapter_vertex = M1Store::Item_lv2::getNew(
                    // vertex flags
                    M1Env::FULL_VERTEX,
                    // label
                    QString("Chapter %1").arg(cm_cur_chapter_number));
                cm_cur_chapter_vertex->setType(M1Env::TEXT_CHAPTER_SIID); //
                cm_cur_chapter_vertex->setFieldEdge(QString("%1").arg(cm_cur_chapter_number), M1Env::TEXT_CHAP_NUMBER_SIID);
                cm_text_root->linkTo(cm_cur_chapter_vertex, M1Store::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
            }
            else if(l_type == "lexicon"){
                // lexicon start
                qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div1 type=%1> Lexicon start").arg(l_type).arg(cm_cur_chapter_number);
                cm_indent += QString(" ").repeated(l_indent_count);

                bool l_found_one_entry = false;

                QString l_pos_text;
                QString l_lemma_text;
                QString l_url_dict_list;

                QList<FormLexicon> l_form_list;

                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    // start of entry
                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "entryFree"){
                        qCDebug(g_cat_tei).noquote() << cm_indent << QString("<entryFree> start of one lexicon lemma entry").arg(l_type).arg(cm_cur_chapter_number);
                        cm_indent += QString(" ").repeated(l_indent_count);
                        l_pos_text = "";
                        l_lemma_text = "";
                        l_url_dict_list = "";
                        l_form_list.clear();
                    }
                    // end of lexicon entry (</entryFree>)
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "entryFree"){
                        if(l_pos_text.length() == 0) throw M1Env::M1Exception("no POS", 1002);
                        if(l_lemma_text.length() == 0) throw M1Env::M1Exception("no lemma text", 1003);
                        // if(l_url_dict_list.length() == 0) throw M1Env::M1Exception("no dictionary reference", 1004);
                        if(l_form_list.length() == 0) throw M1Env::M1Exception("no inflected forms", 1005);

                        cm_indent.chop(l_indent_count);
                        qCDebug(g_cat_tei).noquote() << cm_indent << "<entryFree/> End of lexicon entry";
                        qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("lemma: %1 POS: %2 Dictionary ref: %3").arg(l_lemma_text).arg(l_pos_text).arg(l_url_dict_list);
                        for(const FormLexicon& f: l_form_list)
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("    form: %1 Grammar: %2").arg(f.form()).arg(f.grammar());
                        l_found_one_entry = true;

                        create_Lexicon_Entry(l_lemma_text, l_pos_text, l_url_dict_list, l_form_list);
                    }
                    // POS
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "pos"){
                        QXmlStreamReader::TokenType l_tt_sk = l_xml_reader.readNext();
                        if(l_tt_sk != QXmlStreamReader::Characters) throw M1Env::M1Exception("<pos> not followed by Characters", 1006);
                        l_pos_text = l_xml_reader.text().toString().trimmed();

                        qCDebug(g_cat_tei).noquote() << cm_indent << QString("<pos> value: %1").arg(l_pos_text);
                    }
                    // form: lemma or inflected
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "form"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        // provisional reading of attribute corresp (only present if type=lemma)
                        QString l_url_dict_list_tmp = l_xml_reader.attributes().value("corresp").toString();

                        // common element: <orth>
                        l_tt = l_xml_reader.readNext();
                        if(!(l_tt == QXmlStreamReader::StartElement && l_xml_reader.name() == "orth"))
                            throw M1Env::M1Exception("<form> not followed by <orth>", 1007);

                        QXmlStreamReader::TokenType l_tt_or = l_xml_reader.readNext();
                        if(l_tt_or != QXmlStreamReader::Characters) throw M1Env::M1Exception("<orth> not followed by Characters", 1008);
                        QString l_orth_text = l_xml_reader.text().toString().trimmed();

                        l_tt_or = l_xml_reader.readNext();
                        if(!(l_tt_or == QXmlStreamReader::EndElement && l_xml_reader.name() == "orth"))
                            throw M1Env::M1Exception("<orth> not closed", 1009);

                        if(l_type == "lemma"){
                            // lemma
                            l_lemma_text = l_orth_text;
                            l_url_dict_list = l_url_dict_list_tmp;

                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<form type=%1 url=%2> orth: %3").arg(l_type).arg(l_url_dict_list).arg(l_orth_text);
                        }
                        else if(l_type == "inflected"){
                            // inflected
                            QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                            if(!(l_tt == QXmlStreamReader::StartElement && l_xml_reader.name() == "gram"))
                                throw M1Env::M1Exception("inflected <form> does not contain <gram>", 1010);

                            QXmlStreamReader::TokenType l_tt_gr = l_xml_reader.readNext();
                            if(l_tt_gr != QXmlStreamReader::Characters) throw M1Env::M1Exception("<gram> not followed by Characters", 1011);
                            QString l_gram_text = l_xml_reader.text().toString().trimmed();

                            l_form_list.append(FormLexicon(l_orth_text, l_gram_text));

                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<form type=%1> gram: %2 orth: %3").arg(l_type).arg(l_gram_text).arg(l_orth_text);
                        }
                    }
                    // end of lexicon section (</div1>)
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div1"){
                        cm_indent.chop(l_indent_count);
                        cm_indent.chop(l_indent_count);
                        qCDebug(g_cat_tei).noquote() << cm_indent << "<div1/> End of lexicon";
                        if(! l_found_one_entry) throw M1Env::M1Exception("Lexicon has 0 entries", 1012);
                        break;
                    }
                }
            }
        }
        // sloka start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div2"){
            l_found_sk = false;
            l_found_trnsl = false;
            l_found_wfw = false;
            l_found_translations = false;
            l_found_commentaries = false;

            QString l_type = l_xml_reader.attributes().value("type").toString();
            cm_cur_sloka_number = l_xml_reader.attributes().value("n").toInt();
            l_found_at_least_one_sloka = true;
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div2 type=%1> Sloka %2.%3 start").arg(l_type).arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
            // std::cout << QString("Loading Sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number).toUtf8().constData();
            printf("%s\n", QString("Loading Sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number).toUtf8().constData());

            cm_cur_sloka_vertex = M1Store::Item_lv2::getNew(
                // vertex flags
                M1Env::FULL_VERTEX,
                // label
                QString("Sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number)); // TEXT_SLOKA_NUMBER_SIID
            cm_cur_sloka_vertex->setType(M1Env::SLOKA_SIID);
            cm_cur_sloka_vertex->setFieldEdge(QString("%1").arg(cm_cur_sloka_number), M1Env::TEXT_SLOKA_NUMBER_SIID);
            cm_cur_chapter_vertex->linkTo(cm_cur_sloka_vertex, M1Store::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        }
        // sloka sk text
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "l"){
            l_found_sk = true;
            QXmlStreamReader::TokenType l_tt_sk = l_xml_reader.readNext();
            if(l_tt_sk != QXmlStreamReader::Characters) throw M1Env::M1Exception("<l> not followed by Characters", 1013);
            QString l_sloka_text1 = l_xml_reader.text().toString().trimmed();
            QXmlStreamReader::TokenType l_tt_caesura = l_xml_reader.readNext();
            if(l_tt_caesura != QXmlStreamReader::StartElement && l_xml_reader.name() != "caesura") throw M1Env::M1Exception("No caesura in Sloka", 1014);
            l_xml_reader.readNext(); // to pass the automatic EndElement after <caesura/>
            QXmlStreamReader::TokenType l_tt_sk2 = l_xml_reader.readNext();
            if(l_tt_sk2 != QXmlStreamReader::Characters) qFatal() << QString("No Characters after caesura: %1").arg(l_tt_sk2);
            QString l_sloka_text2 = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("Sloka %1.%2: %3")
                                                             .arg(cm_cur_chapter_number)
                                                             .arg(cm_cur_sloka_number)
                                                             .arg(QString("%1 । %2").arg(l_sloka_text1).arg(l_sloka_text2));

            M1Store::Item_lv2* l_sloka_line = M1Store::Item_lv2::getNew(
                // vertex flags
                M1Env::FULL_VERTEX,
                // label
                QString("%1 । %2").arg(l_sloka_text1).arg(l_sloka_text2));
            l_sloka_line->setType(M1Env::TEXT_SLOKA_LINE_SIID);
            cm_cur_sloka_vertex->linkTo(l_sloka_line, M1Store::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        }
        // sloka transliteration
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "seg"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            if(l_type == "transliteration"){
                l_found_trnsl = true;
                QString l_standard = l_xml_reader.attributes().value("standard").toString();
                QXmlStreamReader::TokenType l_tt_trl = l_xml_reader.readNext();
                if(l_tt_trl != QXmlStreamReader::Characters) throw M1Env::M1Exception("<seg type=transliteration> not followed by Characters", 1015);
                QString l_trl_text = l_xml_reader.text().toString().trimmed();
                qCDebug(g_cat_tei).noquote() << cm_indent << QString("<seg type=\"%1\" standard=\"%2\"> %3").arg(l_type).arg(l_standard).arg(l_trl_text);

                M1Store::Item_lv2* l_sloka_transl = M1Store::Item_lv2::getNew(
                    // vertex flags
                    M1Env::FULL_VERTEX,
                    // label
                    l_trl_text);
                l_sloka_transl->setType(M1Env::TEXT_SLOKA_TRANSLIT_SIID);
                cm_cur_sloka_vertex->linkTo(l_sloka_transl, M1Store::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
            }
        }
        // div3 start (wfw, translation or commentaries block)
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div3"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            QString l_sub_type = l_xml_reader.attributes().value("subtype").toString();

            // word-for-word block
            if(l_type == "wfw-block" && l_sub_type == "empty"){
                // skip wfw block (while marking it as being there) if indicated as empty
                l_found_wfw = true;
            }
            else if(l_type == "wfw-block"){
                l_found_wfw = true;

                QString l_sk_segment;
                QString l_transliteration;
                QString l_translation;

                bool l_is_first = true;

                QList<Form_WfW> l_form_list;

                bool l_found_one_wfw = false;
                bool l_found_wfw_translation = false;
                bool l_found_wfw_transliteration = false;
                bool l_found_wfw_morphology = false;

                qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div3> start of wfw block for sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
                int l_cur_word = 1;
                cm_indent += QString(" ").repeated(l_indent_count);

                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    // wfw-unit <seg>
                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "seg"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "wfw-unit"){
                            l_found_one_wfw = true;
                            l_found_wfw_translation = false;
                            l_found_wfw_transliteration = false;
                            l_found_wfw_morphology = false;

                            QXmlStreamReader::TokenType l_tt_wfw = l_xml_reader.readNext();
                            if(l_tt_wfw != QXmlStreamReader::Characters) throw M1Env::M1Exception("<seg type=transliteration> not followed by Characters", 1016);
                            QString l_wfw_form = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("Word Word %1.%2.%3 <seg type=\"%4\"> %5")
                                                                             .arg(cm_cur_chapter_number)
                                                                             .arg(cm_cur_sloka_number)
                                                                             .arg(l_cur_word)
                                                                             .arg(l_type)
                                                                             .arg(l_wfw_form);
                            l_sk_segment = l_wfw_form;
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
                                qCDebug(g_cat_tei).noquote() << cm_indent << QString("<interp type=\"%1\" source=\"%2\" standard=\"%3\"> %4").arg(l_type).arg(l_source).arg(l_standard).arg(l_trl_text);
                                l_transliteration = l_trl_text;
                            }
                        }
                        // wfw translation
                        else if(l_type == "translation"){
                            l_found_wfw_translation = true;
                            QString l_source = l_xml_reader.attributes().value("source").toString();
                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<interp type=translation> not followed by Characters", 1017);
                            QString l_trl_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<interp type=\"%1\" source=\"%2\"> %3").arg(l_type).arg(l_source).arg(l_trl_text);
                            l_translation = l_trl_text;
                        }
                        // one wfw morphology line inside interpGrp
                        else if(l_type == "morphology"){
                            QString l_lemma = l_xml_reader.attributes().value("lemma").toString();
                            QString l_pos = l_xml_reader.attributes().value("pos").toString();
                            QString l_ref = l_xml_reader.attributes().value("lemmaRef").toString();
                            QString l_grv = l_xml_reader.attributes().value("msd").toString();

                            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
                            if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<interp type=morphology> not followed by Characters", 1018);

                            QString l_form_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<interp type=\"%1\" lemma=\"%2\" pos=\"%3\" msg=\"%4\" lemmaRef=\"%5\"> %6")
                                                                             .arg(l_type).arg(l_lemma).arg(l_pos).arg(l_grv).arg(l_ref).arg(l_form_text);

                            l_form_list.append(Form_WfW(l_form_text, l_lemma, l_pos, l_ref, l_grv));
                        }
                    }
                    // start wfw morphology (<interpGrp>)
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "interpGrp"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "morphology"){
                            l_found_wfw_morphology = true;
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<interpGrp> start of INRIA morphology block for sloka %1.%2 word %3")
                                                                             .arg(cm_cur_chapter_number).arg(cm_cur_sloka_number).arg(l_cur_word);
                            cm_indent += QString(" ").repeated(l_indent_count);
                        }
                    }
                    // end of wfw morphology (</interpGrp>)
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "interpGrp"){
                        l_cur_word += 1;
                        cm_indent.chop(l_indent_count);
                        qCDebug(g_cat_tei).noquote() << cm_indent << "<interpGrp/> End of INRIA morphology block";
                    }
                    // end of wfw unit </seg>
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "seg"){
                        if(! l_found_wfw_translation) throw M1Env::M1Exception(QString("Word %1.%2.%3 has no wfw translation")
                                                         .arg(cm_cur_chapter_number).arg(cm_cur_sloka_number).arg(l_cur_word), 0);
                        if(! l_found_wfw_transliteration) throw M1Env::M1Exception(QString("Word %1.%2.%3 has no wfw transliteration")
                                                         .arg(cm_cur_chapter_number).arg(cm_cur_sloka_number).arg(l_cur_word), 0);
                        if(! l_found_wfw_morphology) throw M1Env::M1Exception(QString("Word %1.%2.%3 has no wfw morphology group")
                                                         .arg(cm_cur_chapter_number).arg(cm_cur_sloka_number).arg(l_cur_word), 0);

                        qCDebug(g_cat_tei).noquote() << cm_indent << "<seg/> End of wfw group";
                        qCDebug(g_cat_tei).noquote() << cm_indent << QString("WfW segment: %1 Translit: %2 Translat: %3")
                                                                                   .arg(l_sk_segment)
                                                                                   .arg(l_transliteration)
                                                                                   .arg(l_translation);
                        for(const Form_WfW& f: l_form_list)
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("    %1").arg(f.dbgOneLiner());

                        create_wfw_unit(l_sk_segment, l_transliteration, l_translation, l_form_list, l_is_first);
                        l_is_first = false;

                        l_form_list.clear();
                    }
                    // end of wfw block <div3>
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        if(! l_found_one_wfw) throw M1Env::M1Exception(QString("SLoka %1.%2 has has no wfw units")
                                                         .arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);

                        cm_indent.chop(l_indent_count);
                        qCDebug(g_cat_tei).noquote() << cm_indent << "End of wfw block";
                        break; // from the inner wfw while
                    }
                    l_cur_word += 1;
                } // end of wfw while
            }
            // translations block
            else if(l_type == "translations-block"){
                l_found_translations = true;
                qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div3> start of translation block for sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
                cm_indent += QString(" ").repeated(l_indent_count);

                QString l_source_txt;
                QString l_language;
                AuthorTEI l_main_author;

                bool l_is_first = true;

                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div4"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        l_language = l_xml_reader.attributes().value("xml:lang").toString();
                        if(l_type == "translation"){
                            l_source_txt = l_xml_reader.attributes().value("source").toString();
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div4 type=\"%1\">").arg(l_type);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
                        l_main_author = readPersName(l_xml_reader);
                        if(l_main_author.isEmpty()) throw M1Env::M1Exception("No main author", 1001);

                        qCDebug(g_cat_tei).noquote() << cm_indent << QString("<persName> translation author : %1/%2").arg(l_main_author.name()).arg(l_main_author.role());
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                        QString l_translation_text = skipUntil(l_indent_count, cm_indent, l_xml_reader, "div4");
                        qCDebug(g_cat_tei).noquote() << cm_indent << QString("translation (%1/%2) [%3]:")
                                                                         .arg(l_source_txt).arg(l_main_author.name()).arg(l_translation_text.length()) << l_translation_text;

                        addTranslationBhashya(l_translation_text, l_language, l_source_txt, l_main_author, AuthorTEI(), true, l_is_first);
                        l_is_first = false;
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        cm_indent.chop(l_indent_count);
                        qCDebug(g_cat_tei).noquote() << cm_indent << "End of translation block";
                        break; // from the inner translation while
                    }
                } // end of translation while
            }
            // commentaries block
            else if(l_type == "commentaries-block"){
                l_found_commentaries = true;
                qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div3> start of commentaries block for sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
                cm_indent += QString(" ").repeated(l_indent_count);

                QString l_source_txt;
                QString l_language;
                AuthorTEI l_main_author;
                AuthorTEI l_secondary_author;

                bool l_is_first = true;

                while (!l_xml_reader.atEnd()) {
                    if(! cm_no_read){ // because readPersName() may read a token ahead
                        cm_tt = l_xml_reader.readNext();
                        Q_ASSERT_X(!l_xml_reader.hasError(),
                                   "TEI read, Sloka comment section",
                                   QString("l_xml_reader.error(): %1").arg(l_xml_reader.error()).toUtf8().constData());
                    }
                    cm_no_read = false;
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(cm_tt == QXmlStreamReader::StartElement && l_tok_name == "div4"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        l_language = l_xml_reader.attributes().value("xml:lang").toString();
                        if(l_type == "commentary"){
                            l_source_txt = l_xml_reader.attributes().value("source").toString();
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div4 type=\"%1\">").arg(l_type);
                        }
                    }
                    else if(cm_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
                        l_main_author = readPersName(l_xml_reader);
                        l_secondary_author = readPersName(l_xml_reader);
                        if(l_main_author.isEmpty()) throw M1Env::M1Exception("No main author", 1001);
                    }
                    else if(cm_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                        qCDebug(g_cat_tei).noquote() << "Main Author:" << l_main_author.name() << l_main_author.role()
                                                               << "Main Author:" << l_secondary_author.name() << l_secondary_author.role()
                                                               << l_language << l_xml_reader.name();
                        QString l_commentary_text = skipUntil(l_indent_count, cm_indent, l_xml_reader, "div4");
                        if(l_xml_reader.hasError())
                            qCDebug(g_cat_tei).noquote() << "XML parsing ERROR" << l_xml_reader.error() << l_xml_reader.errorString();
                        qCDebug(g_cat_tei).noquote() << cm_indent << QString("commentary (%1/%2) [%3]:")
                                                                         .arg(l_source_txt).arg(l_main_author.name()).arg(l_commentary_text.length()) << l_commentary_text;

                        addTranslationBhashya(l_commentary_text, l_language, l_source_txt, l_main_author, l_secondary_author, false, l_is_first);
                        l_is_first = false;
                    }
                    else if(cm_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        cm_indent.chop(l_indent_count);
                        qCDebug(g_cat_tei).noquote() << cm_indent << "End of commentaries block";
                        break; // from the inner commentaries while
                    }
                } // end of commentaries while
            } // end of <div3> (wfw or commentaries or translations blocks)
        }
        // chapter end
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div1"){
            if(! l_found_at_least_one_sloka) throw M1Env::M1Exception(QString("Chapter %1 has 0 Sloka").arg(cm_cur_chapter_number), 0);
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("End of chapter %1").arg(cm_cur_chapter_number);
        }
        // Sloka end
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div2"){
            if(! l_found_sk) throw M1Env::M1Exception(QString("Sloka %1.%2 has no sanskrit text").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_trnsl) throw M1Env::M1Exception(QString("Sloka %1.%2 has no transliteration text").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_wfw) throw M1Env::M1Exception(QString("Sloka %1.%2 has no wfw section").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_translations) throw M1Env::M1Exception(QString("Sloka %1.%2 has no translations section").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_commentaries) throw M1Env::M1Exception(QString("Sloka %1.%2 has no commentaries section").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);

            qCDebug(g_cat_tei).noquote() << cm_indent << QString("End of Sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);

            // if(cm_cur_chapter_number == 1 && cm_cur_sloka_number == 10) break;
        }
    }

    // end of text
    if(! l_found_title) throw M1Env::M1Exception("No title", 1021);
    if(! l_found_author) throw M1Env::M1Exception("No author", 1022);
    if(! l_found_at_least_one_chapter) throw M1Env::M1Exception("No chapters", 1023);
}

void M1Store::TEIInterface::loadTeiInternal(const QString& p_file_path, bool p_validate_only){
    qCDebug(g_cat_tei).noquote() << "loadTeiGeneric() loading file:" << p_file_path;
    QFile l_fin_tei(p_file_path);
    qCDebug(g_cat_tei) << "Exists: " << l_fin_tei.exists();

    // the xml stream parse
    if (!l_fin_tei.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for reading:" << l_fin_tei.errorString();
        std::_Exit(0);
    }
    QXmlStreamReader l_xml_reader(&l_fin_tei);
    cm_indent = "";

    int l_indent_count = 4;
    int l_heading_number = 1;
    int l_hi_number = 1;
    cm_cur_paragraph_number = 1;

    QString l_hi_rend;
    QString l_hi_type;
    QString l_note_type;
    QString l_note_place;
    QString l_note_n;

    QString l_title;
    QString l_sub_title;
    QString l_alt_title;
    QString l_author_main;
    // cm_cur_chapter_number = 0;
    // cm_cur_sloka_number = 0;
    cm_indent = "";

    bool l_found_title = false;
    bool l_found_author = false;
    bool l_in_text = false;
    bool l_in_figure_table = false;
    // bool l_found_at_least_one_chapter = false;
    // bool l_found_at_least_one_sloka = false;
    // bool l_found_sk = false;
    // bool l_found_trnsl = false;
    // bool l_found_wfw = false;
    // bool l_found_translations = false;
    // bool l_found_commentaries = false;

    while (!l_xml_reader.atEnd()) {
        QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
        QStringView l_tok_name = l_xml_reader.name();
        qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << ((l_tt != QXmlStreamReader::Characters) ?
                                                                                    format_tt_token(l_xml_reader) :
                                                                                    M1Store::StorageStatic::maxLengthChop(l_xml_reader.text().toString().trimmed(), 60));
        // text title(s)
        if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "title"){
            QString l_type = l_xml_reader.attributes().value("type").toString();

            QXmlStreamReader::TokenType l_tt_title = l_xml_reader.readNext();
            if(l_tt_title != QXmlStreamReader::Characters) throw M1Env::M1Exception("<title> not followed by Characters", 1000);
            QString l_title_text = l_xml_reader.text().toString().trimmed();
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<title type=\"%1\"> [%2]").arg(l_type).arg(l_title_text);

            if(l_type == "main" || l_type == QString("") ){
                l_title = l_title_text;
                l_found_title  = true;
            }
            else if(l_type == "sub") l_sub_title = l_title_text;
            else if(l_type == "alt") l_alt_title = l_title_text;

            // qCDebug(g_cat_tei).noquote() << l_title << l_sub_title << l_alt_title;
        }
        // text author(s) list (main = first)
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
            l_found_author = true;

            // QXmlStreamReader::TokenType l_tt_author = l_xml_reader.readNext();
            // if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author> not followed by Characters", 1001);
            // l_author_main = l_xml_reader.text().toString().trimmed();
            // qCDebug(g_cat_tei).noquote() << cm_indent << QString("<author> main : %1").arg(l_author_main);

            cm_indent += QString(" ").repeated(l_indent_count);

            while (!l_xml_reader.atEnd()) {
                QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                QStringView l_tok_name = l_xml_reader.name();
                qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "persName"){
                    QXmlStreamReader::TokenType l_tt_name = l_xml_reader.readNext();
                    if(l_tt_name != QXmlStreamReader::Characters) throw M1Env::M1Exception("<persName> not followed by Characters", 1001);
                    QString l_author_name = l_xml_reader.text().toString().trimmed();

                    if(l_author_main.length() == 0) l_author_main = l_author_name;

                    QXmlStreamReader::TokenType l_tt_role_1 = l_xml_reader.readNext();
                    if(l_tt_role_1 != QXmlStreamReader::StartElement || l_xml_reader.name() != "roleName") throw M1Env::M1Exception("<roleName> not found", 1001);
                    QXmlStreamReader::TokenType l_tt_role_2 = l_xml_reader.readNext();
                    if(l_tt_role_2 != QXmlStreamReader::Characters) throw M1Env::M1Exception("<roleName> not followed by Characters", 1001);
                    QString l_author_role = l_xml_reader.text().toString().trimmed();

                    cm_author_list.append(AuthorTEI(l_author_name, l_author_role));
                    qCDebug(g_cat_tei).noquote() << cm_indent << QString("<persName> author : %1 Role : %2").arg(l_author_name).arg(l_author_role);
                }
                else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                    qCDebug(g_cat_tei).noquote() << cm_indent << QString("<author/> %1 (%2) [%3] author: %4")
                    .arg(l_title)
                        .arg(l_alt_title)
                        .arg(l_sub_title)
                        .arg(l_author_main);
                    break; // from <author> inner while
                }
            }
        }
        // end of TEI header
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "teiHeader"){
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<teiHeader/> %1 (%2) [%3] author: %4")
                .arg(l_title)
                .arg(l_alt_title)
                .arg(l_sub_title)
                .arg(l_author_main);

            // + m_author_list as built above
            create_text(l_title, l_alt_title, l_sub_title, l_author_main);
            l_in_text = true;
        }
        // if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "title"){
        //     QString l_type = l_xml_reader.attributes().value("type").toString();
        //     QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
        //     if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<name> not followed by Characters", 1018);
        //     QString l_title = l_xml_reader.text().toString().trimmed();
        //     qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) << QString("<name type=%1>%2").arg(l_type).arg(l_title);
        // } figure
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "figure") l_in_figure_table = true;
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "figure") l_in_figure_table = false;
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "table") l_in_figure_table = true;
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "table") l_in_figure_table = false;
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            QString l_div_n = l_xml_reader.attributes().value("n").toString();

            cm_tei_elements_stack.push(TEIStackLevel(TEIEntityType::Div, create_div(l_type, l_div_n)));

            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div type=%1 n=%2> start").arg(l_type).arg(l_div_n);
            cm_indent += QString(" ").repeated(l_indent_count);
        }
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div"){
            cm_tei_elements_stack.pop();

            cm_indent.chop(l_indent_count);
        }
        else if(!l_in_figure_table && l_tt == QXmlStreamReader::StartElement && l_tok_name == "head"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            cm_tei_elements_stack.push(TEIStackLevel(TEIEntityType::ChunkHeading, create_chunk(QString("Heading %1").arg(l_heading_number++), "head", l_type)));
            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<head type=%1> start").arg(l_type);
            cm_indent += QString(" ").repeated(l_indent_count);
        }
        else if(!l_in_figure_table && l_tt == QXmlStreamReader::EndElement && l_tok_name == "head"){
            cm_tei_elements_stack.pop();

            cm_indent.chop(l_indent_count);
        }
        else if(!l_in_figure_table && l_in_text && l_tt == QXmlStreamReader::StartElement && l_tok_name == "p"){
            cm_tei_elements_stack.push(TEIStackLevel(TEIEntityType::Paragraph, create_paragraph()));

            qCDebug(g_cat_tei).noquote() << cm_indent << "<p> start";
            cm_indent += QString(" ").repeated(l_indent_count);
        }
        else if(!l_in_figure_table && l_in_text && l_tt == QXmlStreamReader::EndElement && l_tok_name == "p"){
            cm_tei_elements_stack.pop();
            cm_indent.chop(l_indent_count);
            qCDebug(g_cat_tei).noquote() << cm_indent << "End of <p> block";
        }
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "name"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            QString l_name_n = l_xml_reader.attributes().value("n").toString();
            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
            if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<name> not followed by Characters", 1018);
            QString l_name = l_xml_reader.text().toString().trimmed();

            create_chunk(l_name, "name", l_type, l_name_n);
            qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) << QString("<name type=%1 n=%2>%3").arg(l_type).arg(l_name_n).arg(l_name);
        }
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "term"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            QXmlStreamReader::TokenType l_tt_interp = l_xml_reader.readNext();
            if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<term> not followed by Characters", 1018);
            QString l_term = l_xml_reader.text().toString().trimmed();
            create_chunk(l_term, "term", l_type);
            qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) << QString("<term type=%1>%2").arg(l_type).arg(l_term);
        }
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "note"){
            cm_indent += QString(" ").repeated(l_indent_count);
            l_note_type = l_xml_reader.attributes().value("type").toString();
            l_note_place = l_xml_reader.attributes().value("place").toString();
            l_note_n = l_xml_reader.attributes().value("n").toString();
            cm_tei_elements_stack.push(TEIStackLevel(TEIEntityType::ChunkNote, create_chunk(QString("Note %1").arg(l_note_n), "note", l_note_type, l_note_n, l_note_place)));
        }
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "note"){
            cm_tei_elements_stack.pop();
            cm_indent.chop(l_indent_count);
            qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) << QString("<note type=%1 place=%2 n=%3> ... </note>")
                                                                                                     .arg(l_note_type).arg(l_note_place).arg(l_note_n);
            l_note_type = "";
            l_note_place = "";
            l_note_n = "";
        }
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "hi"){
            l_hi_type = l_xml_reader.attributes().value("type").toString();
            l_hi_rend = l_xml_reader.attributes().value("rend").toString();
            cm_tei_elements_stack.push(TEIStackLevel(TEIEntityType::ChunkHi, create_chunk(QString("Hi %1").arg(l_hi_number++), "hi", l_hi_type, "", "", l_hi_rend)));
            cm_indent += QString(" ").repeated(l_indent_count);
        }
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "hi"){
            qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) << QString("<hi type=%1 rend=%2> ... </hi>").arg(l_hi_type).arg(l_hi_rend);
            l_hi_type = "";
            l_hi_rend = "";
            cm_tei_elements_stack.pop();
            cm_indent.chop(l_indent_count);
        }
        else if(l_in_text && l_tt == QXmlStreamReader::Characters){
            QString l_text = l_xml_reader.text().toString().trimmed();
            if(l_text.length() > 0){
                create_chunk(l_text, "Characters");
                qCDebug(g_cat_tei).noquote() << cm_indent << "Characters: " << M1Store::StorageStatic::maxLengthChop(l_text, 50);
            }
        }
    }

    //std::_Exit(0);
}

// if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<interp type=morphology> not followed by Characters", 1018);

// QString l_form_text = l_xml_reader.text().toString().trimmed();

// void M1Store::TEIInterface::loadTei2(const QString& p_file_path){
//     qCDebug(g_cat_tei) << "Attempting to load" << p_file_path;
//     if(QFile::exists(p_file_path)){
//         loadTeiInternal2(p_file_path);
//     }
//     else qCDebug(g_cat_tei) << p_file_path << " does not exist";
// }
// void M1Store::TEIInterface::loadTei3(const QString& p_file_path){
//     qCDebug(g_cat_tei) << "Attempting to load" << p_file_path;
//     if(QFile::exists(p_file_path)){
//         loadTeiInternal3(p_file_path);
//     }
//     else qCDebug(g_cat_tei) << p_file_path << " does not exist";
// }

/**
 * @brief M1Store::TEIInterface::loadTeiInternal2
 * @param p_file_path
 * @param p_validate_only
 */
// void M1Store::TEIInterface::loadTeiInternal3(const QString& p_file_path, bool p_validate_only){
//     // QXmlStreamReader l_xml;
//     QFile l_fin_tei(p_file_path);
//     qCDebug(g_cat_tei) << "Exists: " << l_fin_tei.exists();

//     // std::cout << "msg";
//     // std::cout << "msg" << QString("Loading File %1").arg(p_file_path).toUtf8().constData();
//     printf("%s\n", QString("Loading File %1").arg(p_file_path).toUtf8().constData());

//     // the xml stream parse
//     if (!l_fin_tei.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qWarning() << "Failed to open file for reading:" << l_fin_tei.errorString();
//         std::_Exit(0);
//     }

//     int l_indent_count = 4;
//     QXmlStreamReader l_xml_reader(&l_fin_tei);

//     QString l_cur_section;
//     QString l_cur_stephanus;
//     QString l_cur_para;
//     QString l_cur_q;
//     QString l_trailing_text;

//     while (!l_xml_reader.atEnd()) {
//         QXmlStreamReader::TokenType l_tok_type = l_xml_reader.readNext();
//         QStringView l_tok_name = l_xml_reader.name();
//         if(l_tok_type != QXmlStreamReader::Characters) qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tok_type << l_tok_name <<
//                 "H:" << l_cur_section << l_cur_stephanus << l_cur_para << l_cur_q;

//         if(l_tok_type == QXmlStreamReader::Characters) l_trailing_text = l_xml_reader.text().toString().trimmed();
//     }
// }
// /**
//  * @brief M1Store::TEIInterface::loadTeiInternal2
//  * @param p_file_path
//  * @param p_validate_only
//  */
// void M1Store::TEIInterface::loadTeiInternal2(const QString& p_file_path, bool p_validate_only){
//     // QXmlStreamReader l_xml;
//     QFile l_fin_tei(p_file_path);
//     qCDebug(g_cat_tei) << "Exists: " << l_fin_tei.exists();

//     // std::cout << "msg";
//     // std::cout << "msg" << QString("Loading File %1").arg(p_file_path).toUtf8().constData();
//     printf("%s\n", QString("Loading File %1").arg(p_file_path).toUtf8().constData());

//     // the xml stream parse
//     if (!l_fin_tei.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qWarning() << "Failed to open file for reading:" << l_fin_tei.errorString();
//         std::_Exit(0);
//     }

//     int l_indent_count = 4;
//     QXmlStreamReader l_xml_reader(&l_fin_tei);
//     QString l_title;
//     QString l_sub_title;
//     QString l_alt_title;
//     QString l_cur_section;
//     QString l_cur_stephanus;
//     QString l_cur_para;
//     QString l_cur_q;
//     QString l_trailing_text;
//     typedef std::array<QString, 3> StringTriplet;
//     QList<StringTriplet> l_cur_word_list;

//     bool l_found_title = false;
//     bool l_in_titleStmt = false;
//     bool l_in_q_spoken = false;

//     while (!l_xml_reader.atEnd()) {
//         QXmlStreamReader::TokenType l_tok_type = l_xml_reader.readNext();
//         QStringView l_tok_name = l_xml_reader.name();
//         if(l_tok_type != QXmlStreamReader::Characters) qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tok_type << l_tok_name <<
//                 "H:" << l_cur_section << l_cur_stephanus << l_cur_para << l_cur_q;

//         if(l_tok_type == QXmlStreamReader::Characters) l_trailing_text = l_xml_reader.text().toString().trimmed();
//         if(l_tok_type == QXmlStreamReader::StartElement && l_tok_name == "titleStmt") l_in_titleStmt = true;
//         if(l_tok_type == QXmlStreamReader::EndElement && l_tok_name == "titleStmt") l_in_titleStmt = false;
//         if(l_tok_type == QXmlStreamReader::StartElement && l_tok_name == "title" && l_in_titleStmt){
//             QString l_type = l_xml_reader.attributes().value("type").toString();

//             QXmlStreamReader::TokenType l_tt_title = l_xml_reader.readNext();
//             if(l_tt_title != QXmlStreamReader::Characters) throw M1Env::M1Exception("<title> not followed by Characters", 1000);
//             QString l_title_text = l_xml_reader.text().toString().trimmed();
//             qCDebug(g_cat_tei).noquote() << cm_indent << QString("<title type=\"%1\"> [%2]").arg(l_type).arg(l_title_text);

//             if(l_type == "main" || l_type == "") {
//                 l_title = l_title_text;
//                 l_found_title  = true;
//             }
//             else if(l_type == "sub") l_sub_title = l_title_text;
//             else if(l_type == "alt") l_alt_title = l_title_text;

//             // qCDebug(g_cat_tei).noquote() << l_title << l_sub_title << l_alt_title;
//         }
//         if(l_tok_type == QXmlStreamReader::StartElement && l_tok_name == "div"){
//             // <div type="textpart" subtype="book" n="5">
//             // <div type="textpart" subtype="section" resp="perseus" n="449">
//             cm_indent += QString(" ").repeated(l_indent_count);
//             QString l_type = l_xml_reader.attributes().value("type").toString();
//             if(l_type == "textpart"){
//                 QString l_subtype = l_xml_reader.attributes().value("subtype").toString();
//                 if(l_subtype == "section") l_cur_section = l_xml_reader.attributes().value("n").toString();
//             }
//         }
//         if(l_tok_type == QXmlStreamReader::EndElement && l_tok_name == "div"){
//             for (const auto &l_triplet : l_cur_word_list)
//                 qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) <<
//                     QString(">>A> %1 %2%3").arg(l_triplet[0]).arg(l_triplet[1]).arg(l_triplet[2].length() > 0 ? " --> " + l_triplet[2] : "");
//             l_cur_word_list.clear();
//             cm_indent.chop(l_indent_count);
//         }
//         if(l_tok_type == QXmlStreamReader::StartElement && l_tok_name == "milestone"){
//             // cm_indent += QString(" ").repeated(l_indent_count);
//             // <milestone unit="page" resp="Stephanus" n="449" xml:id="eng_ms_page_449" />
//             // <milestone unit="section" resp="Stephanus" n="449a" xml:id="eng_ms_section_449a" />
//             // <milestone ed="P" unit="para" xml:id="eng_ms_para_0" />
//             QString l_unit = l_xml_reader.attributes().value("unit").toString();
//             if(l_trailing_text.length() > 0 && (l_unit == "section" || l_unit == "para"))
//                 qCDebug(g_cat_tei).noquote() << cm_indent << QString("previous milestone trailing: %1").arg(l_trailing_text);

//             for (const auto &l_triplet : l_cur_word_list)
//                 qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) <<
//                     QString(">>B> %1 %2%3").arg(l_triplet[0]).arg(l_triplet[1]).arg(l_triplet[2].length() > 0 ? " --> " + l_triplet[2] : "");
//             l_cur_word_list.clear();

//             qCDebug(g_cat_tei).noquote() << cm_indent << QString("<milestone unit=\"%1\">").arg(l_unit);
//             if(l_unit == "section") l_cur_stephanus = l_xml_reader.attributes().value("xml:id").toString().replace("eng_ms_section_", "S");
//             if(l_unit == "para") l_cur_para = l_xml_reader.attributes().value("xml:id").toString().replace("eng_ms_para_", "ep_");
//         }
//         if(l_tok_type == QXmlStreamReader::StartElement && l_tok_name == "q"){
//             // <q type="spoken"
//             QString l_type = l_xml_reader.attributes().value("type").toString();

//             if(l_type == "spoken"){
//                 if(l_trailing_text.length() > 0)
//                     qCDebug(g_cat_tei).noquote() << cm_indent << QString("trailing text before <q>: %1").arg(l_trailing_text);

//                 for (const auto &l_triplet : l_cur_word_list)
//                     qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) <<
//                         QString(">>C> %1 %2%3").arg(l_triplet[0]).arg(l_triplet[1]).arg(l_triplet[2].length() > 0 ? " --> " + l_triplet[2] : "");
//                 l_cur_word_list.clear();

//                 l_cur_q = l_xml_reader.attributes().value("xml:id").toString();
//                 cm_indent += QString(" ").repeated(l_indent_count);
//                 l_in_q_spoken = true;
//             }
//         }
//         if(l_tok_type == QXmlStreamReader::EndElement && l_tok_name == "q" && l_in_q_spoken){
//             qCDebug(g_cat_tei).noquote() << cm_indent << QString("<q> trailing: %1").arg(l_trailing_text);

//             for (const auto &l_triplet : l_cur_word_list)
//                 qCDebug(g_cat_tei).noquote() << cm_indent + QString(" ").repeated(l_indent_count) <<
//                     QString(">>D> %1 %2%3").arg(l_triplet[0]).arg(l_triplet[1]).arg(l_triplet[2].length() > 0 ? " --> " + l_triplet[2] : "");
//             l_cur_word_list.clear();

//             cm_indent.chop(l_indent_count);
//             l_in_q_spoken = false;
//         }
//         if(l_tok_type == QXmlStreamReader::StartElement && l_tok_name == "w"){
//             // <w xml:id="eng_450_50" corresp="#eng_ms_page_450 #eng_ms_section_450a #eng_ms_para_0">Thrasymachus,<link target="#grc_450_41 #eng_450_50" /></w>
//             QString l_w_id = l_xml_reader.attributes().value("xml:id").toString();

//             QXmlStreamReader::TokenType l_tok_type = l_xml_reader.readNext();
//             if(l_tok_type != QXmlStreamReader::Characters) throw M1Env::M1Exception("<w> not followed by Characters", 1000);
//             QString l_w_content = l_xml_reader.text().toString().trimmed();

//             QString l_link_id;
//             l_tok_type = l_xml_reader.readNext();
//             QStringView l_tok_name = l_xml_reader.name();
//             if(l_tok_type == QXmlStreamReader::StartElement && l_tok_name == "link"){
//                 // <link target="#grc_450_39 #eng_450_49" />
//                 QString l_target = l_xml_reader.attributes().value("target").toString();
//                 if(l_w_id.contains("eng_")) l_link_id = l_target.replace(g_re_remove_eng, "").trimmed().replace("#", "");
//                 else l_link_id = l_target.replace(g_re_remove_grc, "").trimmed().replace("#", "");
//             }
//             l_cur_word_list.append({l_w_id, l_w_content, l_link_id});

//             qCDebug(g_cat_tei).noquote() << cm_indent << QString("<w id=\"%1\"> %2 --> %3").arg(l_w_id).arg(l_w_content).arg(l_link_id);
//         }
//     }
// }
