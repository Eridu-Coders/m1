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
    std::map<QString, M1Store::Item_lv2*> l_form_map;
    for(QJsonObject::const_iterator l_it = l_forms_object.constBegin(); l_it != l_forms_object.constEnd(); ++l_it){
        QJsonObject l_this_form = l_it.value().toObject();
        QJsonArray l_lemma_key_array = l_this_form.find("LemmaKey").value().toArray();
        QString l_tag = l_this_form.find("Tag").value().toString();
        QString l_text = l_this_form.find("Text").value().toString();

        qCDebug(g_cat_tmp_spotlight).noquote() << QString("Key: %1 --> %2-%3 (%4)")
                                                      .arg(l_it.key(), 20)
                                                      .arg(l_tag).arg(l_text)
                                                      .arg(l_lemma_key_array.count());

        l_form_map[l_it.key()] = M1Store::JsonInterface::create_form(l_text, l_tag, l_lemma_key_array);
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
