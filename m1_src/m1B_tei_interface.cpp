#include <QXmlStreamReader>

#include "m1B_tei_interface.h"
#include "m1A_env.h"
#include "m1A_constants.h"
#include "m1B_graph_init.h"

void M1Store::TEIInterface::init(){}
void M1Store::TEIInterface::loadTei(const QString& p_file_path){
    if(QFile::exists(p_file_path)){
        loadTeiInternal(p_file_path);
        // loadTeiInternal(p_file_path, true);
    }
}

/**
 * @brief M1Store::TEIInterface::cm_text_root
 */
M1Store::Item_lv2* M1Store::TEIInterface::cm_text_root;
/**
 * @brief M1Store::TEIInterface::create_text
 * @param p_title
 * @param p_alt_title
 * @param p_sub_title
 * @param p_author_text
 */
void M1Store::TEIInterface::create_text(const QString& p_title, const QString& p_alt_title, const QString& p_sub_title, const QString& p_author_text){
    cm_text_root = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_title);
    cm_text_root->setType("TEXT_");
    cm_text_root->setFieldEdge(p_alt_title, M1Env::TEXT_ALT_TITLE_SIID);
    cm_text_root->setFieldEdge(p_sub_title, M1Env::TEXT_SUB_TITLE_SIID);
}

/**
 * @brief M1Store::TEIInterface::skipUntil
 * @param p_indent_count
 * @param p_indent
 * @param p_xml_reader
 * @param p_elem_close
 * @return
 */
QString M1Store::TEIInterface::skipUntil(int p_indent_count, QString& p_indent, QXmlStreamReader& p_xml_reader, const QString& p_elem_close){
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

/**
 * @brief M1Store::TEIInterface::loadTeiInternal
 * @param p_full_load
 */
void M1Store::TEIInterface::loadTeiInternal(const QString& p_file_path, bool p_validate_only){
    QXmlStreamReader l_xml;
    QFile l_fin_tei(p_file_path);
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
            if(l_tt_title != QXmlStreamReader::Characters) throw M1Env::M1Exception("<title> not followed by Characters", 1000);
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
            if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author> not followed by Characters", 1001);
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

            create_text(l_title, l_alt_title, l_sub_title, l_author_text);
        }
        // lexicon or chapter start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div1"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            if(l_type == "chapter"){
                // chapter start
                l_cur_chapter_number = l_xml_reader.attributes().value("n").toInt();
                l_found_at_least_one_chapter = true;
                l_found_at_least_one_sloka = false;
                qCDebug(g_cat_main).noquote() << l_indent << QString("<div1 type=%1> Chapter %2 start").arg(l_type).arg(l_cur_chapter_number);
            }
            else if(l_type == "lexicon"){
                // lexicon start
                qCDebug(g_cat_main).noquote() << l_indent << QString("<div1 type=%1> Lexicon start").arg(l_type).arg(l_cur_chapter_number);
                l_indent += QString(" ").repeated(l_indent_count);

                bool l_found_one_entry = false;

                QString l_pos_text;
                QString l_lemma_text;
                QString l_url_dict_list;

                class Form{
                private:
                    QString m_orth;
                    QString m_grammar;
                public:
                    Form(const QString& p_orth, const QString& p_grammar){m_orth = p_orth; m_grammar = p_grammar;}

                    const QString& form(){return m_orth;}
                    const QString& grammar(){return m_grammar;}
                };

                QList<Form> l_form_list;

                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    // start of entry
                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "entryFree"){
                        qCDebug(g_cat_main).noquote() << l_indent << QString("<entryFree> start of one lexicon lemma entry").arg(l_type).arg(l_cur_chapter_number);
                        l_indent += QString(" ").repeated(l_indent_count);
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

                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << "<entryFree/> End of lexicon entry";
                        qCDebug(g_cat_main).noquote() << l_indent << QString("lemma: %1 POS: %2 Dictionary ref: %3").arg(l_lemma_text).arg(l_pos_text).arg(l_url_dict_list);
                        for(Form f: l_form_list)
                            qCDebug(g_cat_main).noquote() << l_indent << QString("    form: %1 Grammar: %2").arg(f.form()).arg(f.grammar());
                        l_found_one_entry = true;
                    }
                    // POS
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "pos"){
                        QXmlStreamReader::TokenType l_tt_sk = l_xml_reader.readNext();
                        if(l_tt_sk != QXmlStreamReader::Characters) throw M1Env::M1Exception("<pos> not followed by Characters", 1006);
                        l_pos_text = l_xml_reader.text().toString().trimmed();

                        qCDebug(g_cat_main).noquote() << l_indent << QString("<pos> value: %1").arg(l_pos_text);
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

                            qCDebug(g_cat_main).noquote() << l_indent << QString("<form type=%1 url=%2> orth: %3").arg(l_type).arg(l_url_dict_list).arg(l_orth_text);
                        }
                        else if(l_type == "inflected"){
                            // inflected
                            QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                            if(!(l_tt == QXmlStreamReader::StartElement && l_xml_reader.name() == "gram"))
                                throw M1Env::M1Exception("inflected <form> does not contain <gram>", 1010);

                            QXmlStreamReader::TokenType l_tt_gr = l_xml_reader.readNext();
                            if(l_tt_gr != QXmlStreamReader::Characters) throw M1Env::M1Exception("<gram> not followed by Characters", 1011);
                            QString l_gram_text = l_xml_reader.text().toString().trimmed();

                            l_form_list.append(Form(l_orth_text, l_gram_text));

                            qCDebug(g_cat_main).noquote() << l_indent << QString("<form type=%1> gram: %2 orth: %3").arg(l_type).arg(l_gram_text).arg(l_orth_text);
                        }
                    }
                    // end of lexicon (</div1>)
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div1"){
                        l_indent.chop(l_indent_count);
                        l_indent.chop(l_indent_count);
                        qCDebug(g_cat_main).noquote() << l_indent << "<div1/> End of lexicon";
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
            l_cur_sloka_number = l_xml_reader.attributes().value("n").toInt();
            l_found_at_least_one_sloka = true;
            qCDebug(g_cat_main).noquote() << l_indent << QString("<div2 type=%1> Sloka %2.%3 start").arg(l_type).arg(l_cur_chapter_number).arg(l_cur_sloka_number);
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
                if(l_tt_trl != QXmlStreamReader::Characters) throw M1Env::M1Exception("<seg type=transliteration> not followed by Characters", 1015);
                QString l_trl_text = l_xml_reader.text().toString().trimmed();
                qCDebug(g_cat_main).noquote() << l_indent << QString("<seg type=\"%1\" standard=\"%2\"> %3").arg(l_type).arg(l_standard).arg(l_trl_text);
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
                            if(l_tt_wfw != QXmlStreamReader::Characters) throw M1Env::M1Exception("<seg type=transliteration> not followed by Characters", 1016);
                            QString l_wfw_form = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("Word Word %1.%2.%3 <seg type=\"%4\"> %5")
                                                                             .arg(l_cur_chapter_number).arg(l_cur_sloka_number).arg(l_cur_word)
                                                                             .arg(l_type).arg(l_wfw_form);
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
                            if(l_tt_interp != QXmlStreamReader::Characters) throw M1Env::M1Exception("<interp type=translation> not followed by Characters", 1017);
                            QString l_trl_text = l_xml_reader.text().toString().trimmed();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" source=\"%2\"> %3").arg(l_type).arg(l_source).arg(l_trl_text);
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
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<interp type=\"%1\" lemma=\"%2\" pos=\"%3\" msg=\"%4\" lemmaRef=\"%5\"> %6")
                                                                             .arg(l_type).arg(l_lemma).arg(l_pos).arg(l_grv).arg(l_ref).arg(l_form_text);
                        }
                    }
                    // start wfw morphology (<interpGrp>)
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
                        if(! l_found_one_wfw) throw M1Env::M1Exception(QString("SLoka %1.%2 has has no wfw units")
                                                         .arg(l_cur_chapter_number).arg(l_cur_sloka_number), 0);

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
                QString l_source;
                QString l_author_text;
                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div4"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "translation"){
                            l_source = l_xml_reader.attributes().value("source").toString();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<div4 type=\"%1\">").arg(l_type);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
                        QXmlStreamReader::TokenType l_tt_author = l_xml_reader.readNext();
                        if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author type=translation> not followed by Characters", 1019);
                        l_author_text = l_xml_reader.text().toString().trimmed();
                        qCDebug(g_cat_main).noquote() << l_indent << QString("<author> %1").arg(l_author_text);
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                        QString l_translation_text = skipUntil(l_indent_count, l_indent, l_xml_reader, "div4");
                        qCDebug(g_cat_main).noquote() << l_indent << QString("translation (%1/%2) [%3]:")
                                                                         .arg(l_source).arg(l_author_text).arg(l_translation_text.length()) << l_translation_text;
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
                QString l_source;
                QString l_author_text;
                while (!l_xml_reader.atEnd()) {
                    QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_main).noquote() << l_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div4"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        if(l_type == "commentary"){
                            l_source = l_xml_reader.attributes().value("source").toString();
                            qCDebug(g_cat_main).noquote() << l_indent << QString("<div4 type=\"%1\">").arg(l_type);
                        }
                    }
                    else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
                        QXmlStreamReader::TokenType l_tt_author = l_xml_reader.readNext();
                        if(l_tt_author != QXmlStreamReader::Characters) throw M1Env::M1Exception("<author type=commentary> not followed by Characters", 1020);
                        l_author_text = l_xml_reader.text().toString().trimmed();
                        qCDebug(g_cat_main).noquote() << l_indent << QString("<author> %1").arg(l_author_text);
                    }
                    else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                        QString l_commentary_text = skipUntil(l_indent_count, l_indent, l_xml_reader, "div4");
                        if(l_xml_reader.hasError())
                            qCDebug(g_cat_main).noquote() << "XML parsing ERROR" << l_xml_reader.error() << l_xml_reader.errorString();
                        qCDebug(g_cat_main).noquote() << l_indent << QString("commentary (%1/%2) [%3]:")
                                                                         .arg(l_source).arg(l_author_text).arg(l_commentary_text.length()) << l_commentary_text;
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
    if(! l_found_title) throw M1Env::M1Exception("No title", 1021);
    if(! l_found_author) throw M1Env::M1Exception("No author", 1022);
    if(! l_found_at_least_one_chapter) throw M1Env::M1Exception("No chapters", 1023);
}
