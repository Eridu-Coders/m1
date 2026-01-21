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
int M1Store::JsonInterface::cm_cur_chapter_number = 0;
QString M1Store::JsonInterface::cm_indent;
std::map<QString, M1Store::Item_lv2*> M1Store::JsonInterface::cm_lemma_map;
std::map<QString, M1Store::Item_lv2*> M1Store::JsonInterface::cm_form_map;
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

        int l_cur_book = -1;
        QString l_cur_stephanus;
        // occurrences
        for(QJsonObject::const_iterator l_it_occ = l_this_version_object.constBegin(); l_it_occ != l_this_version_object.constEnd(); ++l_it_occ){
            QJsonObject l_this_occ_object = l_it_occ.value().toObject();

            QString l_occ_id = l_it_occ.key();
            // QString l_text = l_this_occ_object.find("Text").value().toString();
            QString l_tag = l_this_occ_object.find("Tag").value().toString();
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
            QString l_sentence_pos = l_this_occ_object.find("SentencePos").value().toString();
            QString l_form_key = l_this_occ_object.find("FormKey").value().toString();
            QString l_mkp_before = l_this_occ_object.find("MarkupBefore").value().toString();
            QString l_mkp_after = l_this_occ_object.find("MarkupAfter").value().toString();
            QString l_punct_left = l_this_occ_object.find("PunctLeft").value().toString();
            QString l_punct_right = l_this_occ_object.find("PunctRight").value().toString();

            qCDebug(g_cat_tmp_spotlight).noquote() << QString("ID: %1 --> %2 %3 [%4 %5] %6%7%8")
                                                          .arg(l_occ_id)
                                                          .arg(l_tag, 5)
                                                          .arg(l_sentence_pos, 2)
                                                          .arg(l_punct_left, 2)
                                                          .arg(l_punct_right, 2)
                                                          .arg(l_mkp_before.length() > 0 ? QString(" %1").arg(l_mkp_before) : "")
                                                          .arg(l_mkp_after.length() > 0 ? QString(" %1").arg(l_mkp_after) : "")
                                                          .arg(l_form_key);

            if(int l_book_number = l_this_occ_object.find("BookNumber").value().toInt(); l_book_number != -1){
                QString l_book_title = l_this_occ_object.find("BookTitle").value().toString();
                l_cur_stephanus = l_this_occ_object.find("NewSection").value().toString();
                if(l_book_title.length() == 0) l_book_title = QString("Book %1").arg(l_book_number);
                qCDebug(g_cat_tmp_spotlight).noquote() << QString("Stephanus Section: %1 [%2] BookTitle:").arg(l_cur_stephanus).arg(l_book_number) << l_book_title;

                if(l_cur_book != l_book_number){
                    l_cur_book = l_book_number;
                    qCDebug(g_cat_tmp_spotlight).noquote() << "New Book";
                }
            }

            QJsonObject l_this_grammar_object = l_this_occ_object.find("Grammar").value().toObject();
            if(l_this_grammar_object.count() > 0)
                for(QJsonObject::const_iterator l_it_gram = l_this_grammar_object.constBegin(); l_it_gram != l_this_grammar_object.constEnd(); ++l_it_gram){
                    QString l_gram_key = l_it_gram.key();
                    QString l_gram_val = l_it_gram.value().toString();
                    qCDebug(g_cat_tmp_spotlight).noquote() << QString("    GR %1: %2").arg(l_gram_key, 10).arg(l_gram_val);
                }
        }
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
