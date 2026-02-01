#include "m1B_json_interface.h"
#include "m1B_tei_interface.h"
#include "m1A_env.h"
#include "m1A_constants.h"
#include "m1B_graph_init.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// g_cat_json QJsonArray
Q_LOGGING_CATEGORY(g_cat_json, "tei_interface")

M1Store::Item_lv2* M1Store::JsonInterface::cm_text_root = nullptr;
M1Store::Item_lv2* M1Store::JsonInterface::cm_lexicon_root = nullptr;
M1Store::Item_lv2* M1Store::JsonInterface::cm_cur_book = nullptr;
M1Store::Item_lv2* M1Store::JsonInterface::cm_cur_sentence = nullptr;
M1Store::Item_lv2* M1Store::JsonInterface::cm_section_folder = nullptr;
M1Store::Item_lv2* M1Store::JsonInterface::cm_hilights_categories_fldr = nullptr;
M1Store::Item_lv2* M1Store::JsonInterface::cm_notes_folder = nullptr;

std::map<QString, M1Store::Item_lv2*> M1Store::JsonInterface::cm_lemma_map;
std::map<QString, M1Store::Item_lv2*> M1Store::JsonInterface::cm_form_map;
std::map<QString, M1Store::Item_lv2*> M1Store::JsonInterface::cm_section_map;
QStringList M1Store::JsonInterface::cm_form_gr_done;
int M1Store::JsonInterface::cm_cur_sentence_number = 1;
int M1Store::JsonInterface::cm_cur_book_number = 0;

QString M1Store::JsonInterface::cm_indent;

// QMap<QString, M1Store::Item_lv2*> M1Store::JsonInterface::m_form_map;

void M1Store::JsonInterface::init(){
    qCDebug(g_cat_tmp_spotlight).noquote() << "M1Store::JsonInterface::init";
}

void M1Store::JsonInterface::loadJson(const QString& p_file_path){
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("Loading JSON file: [%1]").arg(p_file_path);

    QFile l_file(p_file_path);
    if (!l_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCDebug(g_cat_tmp_spotlight).noquote() << QString("Cannot Load JSON file: [%1]").arg(p_file_path);
        std::_Exit(0);
    }
    QString jsonString = QTextStream(&l_file).readAll();
    l_file.close();

    QByteArray l_jsonData = jsonString.toUtf8();
    QJsonDocument l_document = QJsonDocument::fromJson(l_jsonData);
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("l_document.isObject(): %1").arg(l_document.isObject() ? "True" : "False");

    QJsonObject l_document_object = l_document.object();
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("l_document_object.count(): %1").arg(l_document_object.count());
    for(QJsonObject::const_iterator l_it = l_document_object.constBegin(); l_it != l_document_object.constEnd(); ++l_it)
        qCDebug(g_cat_tmp_spotlight).noquote() << QString("Key: %1").arg(l_it.key());

    QString l_title = l_document_object.find("Title").value().toString();
    QString l_author = l_document_object.find("Author").value().toString();
    QString l_ground_text_version = l_document_object.find("GroundTextVersion").value().toString();
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("Author: %1 Title: %2").arg(l_author).arg(l_title);

    M1Store::TEIInterface::create_text(l_title, "", "", l_author, &cm_text_root, &cm_lexicon_root);

    QJsonObject l_lemmas_object = l_document_object.find("Lemmas").value().toObject();
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("l_lemmas_object.count(): %1").arg(l_lemmas_object.count());
    // std::map<QString, M1Store::Item_lv2*> l_lemma_map;
    for(QJsonObject::const_iterator l_it = l_lemmas_object.constBegin(); l_it != l_lemmas_object.constEnd(); ++l_it){
        QJsonObject l_this_lemma = l_it.value().toObject();
        QString l_ent_type = l_this_lemma.find("EntType").value().toString();
        QString l_pos = l_this_lemma.find("Pos").value().toString();
        QString l_text = l_this_lemma.find("Text").value().toString();
        qCDebug(g_cat_tmp_spotlight).noquote() << QString("Key: %1 --> %2-%3%4")
                                                      .arg(l_it.key(), 20)
                                                      .arg(l_pos).arg(l_text)
                                                      .arg(l_ent_type.length() > 0 ? " (" + l_ent_type + ")": "");

        cm_lemma_map[l_it.key()] = M1Store::TEIInterface::create_Lemma(l_text, l_pos, "", &cm_lexicon_root);
    }

    QJsonObject l_forms_object = l_document_object.find("Forms").value().toObject();
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("l_forms_object.count(): %1").arg(l_forms_object.count());
    for(QJsonObject::const_iterator l_it = l_forms_object.constBegin(); l_it != l_forms_object.constEnd(); ++l_it){
        QJsonObject l_this_form = l_it.value().toObject();
        QJsonArray l_lemma_key_array = l_this_form.find("LemmaKey").value().toArray();
        QString l_tag = l_this_form.find("Tag").value().toString();
        QString l_text = l_this_form.find("Text").value().toString();

        qCDebug(g_cat_tmp_spotlight).noquote() << QString("Key: %1 --> %2-%3 (%4)")
                                                      .arg(l_it.key(), 20)
                                                      .arg(l_tag).arg(l_text)
                                                      .arg(l_lemma_key_array.count());

        cm_form_map[l_it.key()] = M1Store::JsonInterface::create_form(l_text, l_tag, l_lemma_key_array);

    }


    // Notes
    cm_notes_folder = cm_text_root->create_descendant(M1Env::OWNS_SIID, "Notes", M1Env::FOLDER_SIID);

    // Section folder
    cm_section_folder = cm_text_root->create_descendant(M1Env::OWNS_SIID, "Stephanus Sections", M1Env::FOLDER_SIID);

    // TEXT_HIGHLIGHT_CAT_FLDR_SIID
    // cm_hilights_categories_fldr = cm_text_root->create_descendant(M1Env::OWNS_SIID, "Highlight Categories", M1Env::TEXT_HIGHLIGHT_CAT_FLDR_SIID);
    // creation of Republic highlight categories node
    cm_hilights_categories_fldr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Highlight categories");
    cm_hilights_categories_fldr->setType(M1Env::FOLDER_SIID);
    cm_hilights_categories_fldr->setType(M1Env::TEXT_HIGHLIGHT_CAT_FLDR_SIID);
    cm_text_root->linkTo(cm_hilights_categories_fldr, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    // TEXT_HIGHLIGHT_FLDR_SIID
    // cm_text_root->create_descendant(M1Env::OWNS_SIID, "Highlights", M1Env::TEXT_HIGHLIGHT_FLDR_SIID);
    M1Store::Item_lv2* l_hilights_categories = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Highlights");
    l_hilights_categories->setType(M1Env::FOLDER_SIID);
    l_hilights_categories->setType(M1Env::TEXT_HIGHLIGHT_FLDR_SIID);
    cm_text_root->linkTo(l_hilights_categories, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    // creation of example highlights categories
    qCDebug(g_cat_silence) << QString("Creating example Republic highlight categories");
    M1Store::Item_lv2* l_republic_highlight_cat_1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Category 1",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RCTG1",
        // icon path
        nullptr);
    l_republic_highlight_cat_1->setType(M1Env::TYPE_NODE_SIID);
    l_republic_highlight_cat_1->setType(M1Env::TEXT_HIGHLIGHT_CAT_SIID);
    l_republic_highlight_cat_1->setFieldVertex("#f78a38", M1Env::HLCLR_SIID);
    cm_hilights_categories_fldr->linkTo(l_republic_highlight_cat_1, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    M1Store::Item_lv2* l_republic_highlight_cat_2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Category 2",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RCTG2",
        // icon path
        nullptr);
    l_republic_highlight_cat_2->setType(M1Env::TYPE_NODE_SIID);
    l_republic_highlight_cat_2->setType(M1Env::TEXT_HIGHLIGHT_CAT_SIID);
    l_republic_highlight_cat_2->setFieldVertex("#3883f7", M1Env::HLCLR_SIID);
    cm_hilights_categories_fldr->linkTo(l_republic_highlight_cat_2, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    // Versions
    M1Store::Item_lv2* l_version_folder = cm_text_root->create_descendant(M1Env::OWNS_SIID, "Versions", M1Env::FOLDER_SIID);
    QJsonObject l_versions_object = l_document_object.find("Versions").value().toObject();
    for(QJsonObject::const_iterator l_it_ver = l_versions_object.constBegin(); l_it_ver != l_versions_object.constEnd(); ++l_it_ver){
        QString l_version_name = l_it_ver.key();
        bool l_is_ground_text = l_version_name == l_ground_text_version;
        QJsonObject l_this_version_object = l_it_ver.value().toObject();
        QString l_version_type = l_is_ground_text ? "G" : "T";
        qCDebug(g_cat_tmp_spotlight).noquote() << QString("Version: %1 --> %2 %3")
                                                      .arg(l_version_name)
                                                      .arg(l_this_version_object.count())
                                                      .arg(l_version_type);

        M1Store::Item_lv2* l_current_version = l_version_folder->create_descendant(M1Env::OWNS_SIID, l_version_name, M1Env::TXTVR_SIID);
        l_current_version->setFieldEdge(l_version_type, M1Env::TEXT_VER_TYPE_SIID);

        cm_cur_book_number = 0;
        cm_cur_sentence_number = 1;
        // occurrences
        for(QJsonObject::const_iterator l_it_occ = l_this_version_object.constBegin(); l_it_occ != l_this_version_object.constEnd(); ++l_it_occ){
            QJsonObject l_this_occ_object = l_it_occ.value().toObject();

            QString l_occ_id = l_it_occ.key();
            // QString l_text = l_this_occ_object.find("Text").value().toString();
            QString l_tag = l_this_occ_object.find("Tag").value().toString();
            QString l_pos = l_this_occ_object.find("Pos").value().toString();
            // qCDebug(g_cat_tmp_spotlight).noquote() << QString("ID: %1 --> %2 %3").arg(l_occ_id).arg(l_tag, 5).arg(l_text);

            /*
            "Z-GRK-054611": {
                "BookNumber": -1,
                "BookTitle": "",
                "EntityKey": "",
                "FormKey": "φαμεν‣VERB",
                "Grammar": {
                    "Mood": "Ind",
                    "Number": "Sing",
                    "Person": "3",
                    "Tense": "Pres",
                    "VerbForm": "Fin",
                    "Voice": "Act"
                },
                "MarkupAfter": "",
                "MarkupBefore": "",
                "NewSection": "",
                "NoteKey": "",
                "Pos": "VERB",
                "PunctLeft": "",
                "PunctRight": "",
                "SentencePos": "",
                "Tag": "VERB",
                "Text": "φαμεν"
            */
            QString l_sentence_position = l_this_occ_object.find("SentencePos").value().toString();
            QString l_form_key = l_this_occ_object.find("FormKey").value().toString();
            QString l_mkp_before = l_this_occ_object.find("MarkupBefore").value().toString();
            QString l_mkp_after = l_this_occ_object.find("MarkupAfter").value().toString();
            QString l_punct_left = l_this_occ_object.find("PunctLeft").value().toString();
            QString l_punct_right = l_this_occ_object.find("PunctRight").value().toString();
            QString l_note_key = l_this_occ_object.find("NoteKey").value().toString();
            QString l_occ_text = l_this_occ_object.find("Text").value().toString();
            QString l_cur_stephanus = l_this_occ_object.find("NewSection").value().toString();

            if(int l_book_number = l_this_occ_object.find("BookNumber").value().toInt(); l_book_number != -1){
                QString l_book_title = l_this_occ_object.find("BookTitle").value().toString();
                if(l_book_title.length() == 0) l_book_title = QString("Book %1").arg(l_book_number);
                qCDebug(g_cat_tmp_spotlight).noquote() << QString("Stephanus Section: %1 [%2] BookTitle:").arg(l_cur_stephanus).arg(l_book_number) << l_book_title;

                if(cm_cur_book_number != l_book_number){
                    cm_cur_book_number = l_book_number;
                    qCDebug(g_cat_tmp_spotlight).noquote() << "New Book: " << cm_cur_book_number;
                    cm_cur_book = M1Store::Item_lv2::getNew(
                        // vertex flags
                        M1Env::FULL_VERTEX,
                        // label
                        QString("Book %1").arg(cm_cur_book_number));
                    cm_cur_book->setType(M1Env::TEXT_BOOK_SIID);
                    l_current_version->linkTo(cm_cur_book, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
                }
            }

            QJsonObject l_this_grammar_object = l_this_occ_object.find("Grammar").value().toObject();
            QStringList l_gram_list;
            if(l_this_grammar_object.count() > 0)
                for(QJsonObject::const_iterator l_it_gram = l_this_grammar_object.constBegin(); l_it_gram != l_this_grammar_object.constEnd(); ++l_it_gram){
                    QString l_gram_key = l_it_gram.key();
                    QString l_gram_val = l_it_gram.value().toString();
                    // qCDebug(g_cat_tmp_spotlight).noquote() << QString("    GR %1: %2").arg(l_gram_key, 10).arg(l_gram_val);
                    l_gram_list.append(QString("%1: %2").arg(l_gram_key).arg(l_gram_val));
                }

            QJsonArray l_this_grammar_pentacodes_array = l_this_occ_object.find("PentaGrammar").value().toArray();
            // qCDebug(g_cat_tmp_spotlight).noquote() << QString("    l_this_grammar_pentacodes_array: %1").arg(l_this_grammar_pentacodes_array.count());
            QStringList l_gram_penta_list;
            if(l_this_grammar_pentacodes_array.count() > 0)
                for(const auto& l_value : l_this_grammar_pentacodes_array){
                    // qCDebug(g_cat_tmp_spotlight).noquote() << QString("    GR %1").arg(l_value.toString());
                    l_gram_penta_list.append(l_value.toString());
                }

            qCDebug(g_cat_tmp_spotlight).noquote() << QString("ID: %1 --> %2 %3 %4 [%5 %6] %7%8%9%10%11%12")
                                                          .arg(l_occ_id) // 1
                                                          .arg(l_pos, 5) // 2
                                                          .arg(l_tag, 5) // 3
                                                          .arg(l_sentence_position, 2) // 4
                                                          .arg(l_punct_left, 2) // 5
                                                          .arg(l_punct_right, 2) // 6
                                                          .arg(l_mkp_before.length() > 0 ? QString(" %1").arg(l_mkp_before) : "") // 7
                                                          .arg(l_mkp_after.length() > 0 ? QString(" %1").arg(l_mkp_after) : "") // 8
                                                          .arg(l_occ_text + "-" + l_form_key) // 9
                                                          .arg(l_gram_penta_list.length() > 0 ? " (" + l_gram_penta_list.join(" / ") + ")" : "") // 10
                                                          .arg(l_gram_list.length() > 0 ? " {" + l_gram_list.join(" / ") + "}" : "") // 11
                                                          .arg(l_note_key.length() > 0 ? " " + l_note_key : ""); // 12

            add_word(l_occ_text, l_occ_id, l_cur_stephanus, l_pos, l_tag, l_sentence_position,
                     l_punct_left, l_punct_right, l_mkp_before, l_mkp_after, l_form_key, l_gram_penta_list, l_note_key);

        }
    }
}

void M1Store::JsonInterface::add_word(const QString& p_occ_text,
              const QString& p_occ_id,
              const QString& p_new_stephanus_section,
              const QString& p_pos,
              const QString& p_tag,
              const QString& p_sentence_position,
              const QString& p_punct_left,
              const QString& p_punct_right,
              const QString& p_mkp_before,
              const QString& p_mkp_after,
              const QString& p_form_key,
              const QStringList& p_gram_penta_list,
              const QString& p_note_key){

    M1Store::Item_lv2* l_form = cm_form_map[p_form_key];

    M1Store::Item_lv2* l_new_occ = cm_text_root->linkTo(l_form, M1Env::OCCUR_SIID, InsertionPoint::at_bottom_force_special, InsertionPoint::at_top);
    l_new_occ->setText_lv1(p_occ_id);

    if(g_re_cap_initial.match(p_occ_text).hasMatch()){
        qCDebug(g_cat_tmp_spotlight).noquote() << "Capitalized";
        l_new_occ->setFieldEdge("true", M1Env::CAPTL_SIID);
    }
    if(p_sentence_position.length() > 0) l_new_occ->setFieldEdge(p_sentence_position, M1Env::STPOS_SIID);
    if(p_punct_left.length() > 0) l_new_occ->setFieldEdge(p_punct_left, M1Env::PCTLF_SIID);
    if(p_punct_right.length() > 0) l_new_occ->setFieldEdge(p_punct_right, M1Env::PCTRT_SIID);
    if(p_mkp_before.length() > 0) l_new_occ->setFieldEdge(p_mkp_before, M1Env::MKPLF_SIID);
    if(p_mkp_after.length() > 0) l_new_occ->setFieldEdge(p_mkp_after, M1Env::MKPRT_SIID);
    if(p_new_stephanus_section.length() > 0){
        l_new_occ->setFieldEdge(p_new_stephanus_section, M1Env::STEPHANUS_SIID);

        M1Store::Item_lv2* l_new_section;
        auto l_it = cm_section_map.find(p_new_stephanus_section);
        if(l_it != cm_section_map.end()) l_new_section = l_it->second;
        else{
            l_new_section = M1Store::Item_lv2::getNew(
                // vertex flags
                M1Env::FULL_VERTEX,
                // label
                p_new_stephanus_section);
            l_new_section->setType(M1Env::STEPHANUS_SIID);
            cm_section_map[p_new_stephanus_section] = l_new_section;
            cm_section_folder->linkTo(l_new_section, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        }

        cm_cur_book->linkTo(l_new_section, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        l_new_section->linkTo(l_new_occ, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    }

    if(!cm_form_gr_done.contains(p_form_key)){
        for(const auto& l_pentacode : p_gram_penta_list)
            l_form->setType(l_pentacode.toUtf8().constData());
        cm_form_gr_done.append(p_form_key);
    }

    // sentence position
    if(p_sentence_position == "SS" || p_sentence_position == "SX"){
        cm_cur_sentence = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            QString("Sentence %1").arg(cm_cur_sentence_number));
        cm_cur_sentence_number += 1;
        cm_cur_sentence->setType(M1Env::TEXT_SENTENCE_SIID);
        cm_cur_sentence->linkTo(l_new_occ, M1Env::TW_CHUNK_2_OCC_BEGIN_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        cm_cur_book->linkTo(cm_cur_sentence, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    }

    if(p_sentence_position == "SE" || p_sentence_position == "SX"){
        cm_cur_sentence->linkTo(l_new_occ, M1Env::TW_CHUNK_2_OCC_END_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        qCDebug(g_cat_tmp_spotlight).noquote() << "End Sentence";
    }
}

M1Store::Item_lv2* M1Store::JsonInterface::create_form(const QString& p_form_text, const QString& p_tag_text, const QJsonArray& p_lemma_keys){
    M1Store::Item_lv2* l_new_form = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_form_text);
    l_new_form->setType(M1Env::TEXT_WFW_FORM_SIID);

    // (*l_lexicon_root)->linkTo(l_new_lemma, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    l_new_form->setFieldEdge(p_tag_text, M1Env::TEXT_WFW_POS_SIID);

    if(p_tag_text.length() == 5)
        l_new_form->setType(M1Store::StorageStatic::getSpecialItemPointer(p_tag_text.toUtf8().constData()));

    for(QJsonArray::const_iterator l_it = p_lemma_keys.constBegin(); l_it != p_lemma_keys.constEnd(); ++l_it){
        QString l_lemma_key = l_it->toString();
        cm_lemma_map[l_lemma_key]->linkTo(l_new_form, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    }

    return l_new_form;
}

// namespace M1Store
