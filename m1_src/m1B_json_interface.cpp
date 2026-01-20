#include "m1B_json_interface.h"
#include "m1B_tei_interface.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

// g_cat_json
Q_LOGGING_CATEGORY(g_cat_json, "tei_interface")

M1Store::Item_lv2* M1Store::JsonInterface::cm_text_root = nullptr;
M1Store::Item_lv2* M1Store::JsonInterface::cm_lexicon_root = nullptr;
int M1Store::JsonInterface::cm_cur_chapter_number = 0;
QString M1Store::JsonInterface::cm_indent;
QMap<QString, M1Store::Item_lv2*> M1Store::JsonInterface::m_form_map;

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
    for(QJsonObject::const_iterator l_it = l_lemmas_object.constBegin(); l_it != l_lemmas_object.constEnd(); ++l_it){
        QJsonObject l_this_lemma = l_it.value().toObject();
        QString l_ent_type = l_this_lemma.find("EntType").value().toString();
        QString l_pos = l_this_lemma.find("Pos").value().toString();
        QString l_text = l_this_lemma.find("Text").value().toString();
        qCDebug(g_cat_tmp_spotlight).noquote() << QString("Key: %1 --> %2-%3%4")
                                                      .arg(l_it.key(), 20)
                                                      .arg(l_pos).arg(l_text)
                                                      .arg(l_ent_type.length() > 0 ? " (" + l_ent_type + ")": "");

        M1Store::TEIInterface::create_Lemma(l_text, l_pos, "", &cm_lexicon_root);
    }
}

// namespace M1Store
