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
M1Store::Item_lv2* M1Store::TEIInterface::cm_lexicon_root;
M1Store::Item_lv2* M1Store::TEIInterface::cm_cur_chapter_vertex;
M1Store::Item_lv2* M1Store::TEIInterface::cm_cur_sloka_vertex;

int M1Store::TEIInterface::cm_cur_chapter_number;
int M1Store::TEIInterface::cm_cur_sloka_number;
QString M1Store::TEIInterface::cm_indent;

QList<M1Store::AuthorTEI> M1Store::TEIInterface::m_author_list;
QMap<QString, M1Store::Item_lv2*> M1Store::TEIInterface::m_form_map;

QXmlStreamReader::TokenType M1Store::TEIInterface::m_tt;
bool M1Store::TEIInterface::m_no_read = false;


/**
 * @brief M1Store::TEIInterface::create_text
 *
 * The secondary authors are not created at this stage because they will automatically be when adding the translations and Bhashyas
 *
 * @param p_title
 * @param p_alt_title
 * @param p_sub_title
 * @param p_author_text
 */
void M1Store::TEIInterface::create_text(
    const QString& p_title,
    const QString& p_alt_title,
    const QString& p_sub_title,
    const QString& p_author_text,
    M1Store::Item_lv2** p_text_root,
    M1Store::Item_lv2** p_lexicon_root)
{
    M1Store::Item_lv2** l_text_root = p_text_root;
    M1Store::Item_lv2** l_lexicon_root = p_lexicon_root;

    if(l_text_root == nullptr) l_text_root = &cm_text_root;
    if(l_lexicon_root == nullptr) l_lexicon_root = &cm_lexicon_root;

    // title(s)
    (*l_text_root) = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_title);
    (*l_text_root)->setType("TEXT_");
    if(p_alt_title.length() > 0) (*l_text_root)->setFieldEdge(p_alt_title, M1Env::TEXT_ALT_TITLE_SIID);
    if(p_sub_title.length() > 0) (*l_text_root)->setFieldEdge(p_sub_title, M1Env::TEXT_SUB_TITLE_SIID);

    // Author (main author role is just 'Author')
    M1Store::Item_lv2* l_author = getPersonOrg(p_author_text, "Author", OrgOrPerson::Person, ReturnEntityOrRole::Entity);
    (*l_text_root)->linkTo(l_author, M1Env::TEXT_WRITTEN_BY_SIID);

    // Lexicon Root
    (*l_lexicon_root) = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Lexicon");
    (*l_lexicon_root)->setType(M1Env::FOLDER_SIID);
    (*l_text_root)->linkTo((*l_lexicon_root), M1Env::OWNS_SIID);
}

void M1Store::TEIInterface::addTranslationBhashya(
    const QString& p_translation_text,
    const QString& p_language,
    const QString& p_source,
    const AuthorTEI& p_main_author,
    const AuthorTEI& p_secondary_author,
    bool p_is_translation,
    bool p_sep_above)
{
    QString l_text = p_translation_text;
    l_text = l_text.replace(g_re_space, QString(" ")).trimmed();
    if(l_text.length() > 0){
        // Text node creation
        M1Store::Item_lv2* l_new_text = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            l_text);
        if(p_is_translation) l_new_text->setType(M1Env::TEXT_SLOKA_TRANSLATION_SIID);
        else l_new_text->setType(M1Env::TEXT_SLOKA_BHASHYA_SIID);
        l_new_text->setFieldEdge(p_language, M1Store::TEXT_LANGUAGE_SIID);

        M1Store::Item_lv2* l_new_edge = cm_cur_sloka_vertex->linkTo(l_new_text, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        if(p_sep_above) l_new_edge->setFlag(M1Env::EDGE_SEPABOVE);

        // Source
        if(p_source.length() > 0){ // role for an org = data source
            M1Store::Item_lv2* l_source_folder = getPersonOrg(p_source, "Data Source", OrgOrPerson::Organization, ReturnEntityOrRole::RoleFolder);
            l_new_text->linkTo(l_source_folder, M1Env::DATA_SOURCE_FROM_SIID, InsertionPoint::at_bottom, InsertionPoint::at_bottom);
        }

        // Author
        M1Store::Item_lv2* l_author_folder = getPersonOrg(p_main_author.name(), p_main_author.role(), OrgOrPerson::Person, ReturnEntityOrRole::RoleFolder);
        l_new_text->linkTo(l_author_folder, M1Env::TEXT_WRITTEN_BY_SIID, InsertionPoint::at_bottom, InsertionPoint::at_bottom);

        // Secondary Author
        if(! p_secondary_author.isEmpty()){
            M1Store::Item_lv2* l_sec_author_folder = getPersonOrg(p_secondary_author.name(), p_secondary_author.role(), OrgOrPerson::Person, ReturnEntityOrRole::RoleFolder);
            l_new_text->linkTo(l_sec_author_folder, M1Env::TEXT_WRITTEN_BY_SIID, InsertionPoint::at_bottom, InsertionPoint::at_bottom);
        }
    }
}

/**
 * @brief M1Store::TEIInterface::getPersonOrg
 *
 * returns the entity itself (Person or Org) or the appropriate role folder depending on p_role and p_return
 *
 * @param p_name
 * @param p_role
 * @param p_is_person type of the entity to be created or retrieved (PERS_TYPE_SIID or ORG_TYPE_SIID)
 * @param p_return
 * @return
 */
M1Store::Item_lv2* M1Store::TEIInterface::getPersonOrg(const QString& p_name, const QString& p_role, OrgOrPerson p_is_person, ReturnEntityOrRole p_return){
    static std::map<QString, M1Store::Item_lv2*> s_cache_tei_ent;

    M1Store::SpecialItemID l_entity_type_siid = p_is_person == OrgOrPerson::Person ? M1Env::PERS_TYPE_SIID : M1Env::ORG_TYPE_SIID;

    M1Store::Item_lv2* l_entity_existing = s_cache_tei_ent[p_name];
    M1Store::Item_lv2* l_role_folder = nullptr;

    if(l_entity_existing == nullptr){
        l_entity_existing = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            p_name);
        l_entity_existing->setType(l_entity_type_siid);
        s_cache_tei_ent[p_name] = l_entity_existing;
    }
    // find role folder or create it, if appropriate
    l_role_folder = l_entity_existing->find_edge_target_string(M1Env::OWNS_SIID, p_role);

    if(l_role_folder == nullptr){
        l_role_folder = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            p_role);
        l_role_folder->setType(M1Env::ROLE_FLDR_SIID);
        l_entity_existing->linkTo(l_role_folder, M1Store::OWNS_SIID);
    }
    else
        l_role_folder = l_role_folder->getTarget_lv2();

    if(p_return == ReturnEntityOrRole::Entity)
        return l_entity_existing;
    else
        return l_role_folder;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_Lemma(
    const QString& p_lemma_text,
    const QString& p_pos_text,
    const QString& p_url_dict_list,
    M1Store::Item_lv2** p_lexicon_root)
{
    M1Store::Item_lv2** l_lexicon_root = p_lexicon_root;
    if(l_lexicon_root == nullptr) l_lexicon_root = &cm_lexicon_root;

    M1Store::Item_lv2* l_new_lemma = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_lemma_text);
    l_new_lemma->setType(M1Env::LEMMA_SIID);
    (*l_lexicon_root)->linkTo(l_new_lemma, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    l_new_lemma->setFieldEdge(p_pos_text, M1Env::TEXT_WFW_POS_SIID);

    if(p_pos_text.length() == 5)
        l_new_lemma->setType(M1Store::StorageStatic::getSpecialItemPointer(p_pos_text.toUtf8().constData()));

    M1Store::Item_lv2* l_new_url = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_url_dict_list);
    l_new_url->setType(M1Env::TEXT_URL_LINK_SIID);
    l_new_lemma->linkTo(l_new_url, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    return l_new_lemma;
}


void M1Store::TEIInterface::create_Lexicon_Entry(const QString& p_lemma_text, const QString& p_pos_text, const QString& p_url_dict_list, QList<FormLexicon>& p_form_list){
    M1Store::Item_lv2* l_new_lemma = create_Lemma(p_lemma_text, p_pos_text, p_url_dict_list);
    /*
    M1Store::Item_lv2* l_new_lemma = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_lemma_text);
    l_new_lemma->setType(M1Env::LEMMA_SIID);
    cm_lexicon_root->linkTo(l_new_lemma, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    l_new_lemma->setFieldEdge(p_pos_text, M1Env::TEXT_WFW_POS_SIID);

    if(p_pos_text.length() == 5)
        l_new_lemma->setType(M1Store::StorageStatic::getSpecialItemPointer(p_pos_text.toUtf8().constData()));

    M1Store::Item_lv2* l_new_url = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_url_dict_list);
    l_new_url->setType(M1Env::TEXT_URL_LINK_SIID);
    l_new_lemma->linkTo(l_new_url, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    */

    // forms
    for(const auto &f: p_form_list){
        M1Store::Item_lv2* l_new_form = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            f.form());
        l_new_form->setType(M1Env::TEXT_WFW_FORM_SIID);
        l_new_lemma->linkTo(l_new_form, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        QString l_key = QString("%1-%2").arg(f.form()).arg(p_lemma_text);
        qCDebug(g_cat_tmp_spotlight).noquote() << QString("l_key: [%1]").arg(l_key);
        m_form_map[l_key] = l_new_form;

        for(const QString& l_gram_alternative : f.grammar().split("|")){
            for(const QString& l_gram_code_1 : l_gram_alternative.split("-"))
                if(l_gram_code_1 == "GNFEM/GNEUT/GMASC" || l_gram_code_1 == "**no analysis**")
                    l_new_form->setFieldEdge(l_gram_code_1, M1Env::TEXT_LEXICON_GRAMMAR_SIID);
                else
                    for(const QString& l_gram_code_2 : l_gram_code_1.split("/"))
                        l_new_form->setType(M1Store::StorageStatic::getSpecialItemPointer(l_gram_code_2.toUtf8().constData()));
        }
    }
}

/**
 * @brief M1Store::TEIInterface::create_wfw_unit
 *
 * \todo properly handle InsertionPoint::none in M1Store::Item_lv2::linkTo()
 *
 * @param p_sk_segment
 * @param p_transliteration
 * @param p_translation
 * @param p_form_list
 */
void M1Store::TEIInterface::create_wfw_unit(
    const QString& p_sk_segment,
    const QString& p_transliteration,
    const QString& p_translation,
    QList<Form_WfW>& p_form_list,
    bool p_sep_above)
{
    M1Store::Item_lv2* l_new_wfw_unit = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_sk_segment);
    l_new_wfw_unit->setType(M1Env::TEXT_WFW_UNIT_SIID);

    M1Store::Item_lv2* l_new_edge = cm_cur_sloka_vertex->linkTo(l_new_wfw_unit, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    if(p_sep_above) l_new_edge->setFlag(M1Env::EDGE_SEPABOVE);

    l_new_wfw_unit->setFieldEdge(p_transliteration, M1Env::TEXT_WFW_TRANSLIT_SIID);
    l_new_wfw_unit->setFieldEdge(p_translation, M1Env::TEXT_WFW_TRANSLAT_SIID);

    for(const Form_WfW& f: std::as_const(p_form_list)){
        for(const auto& l_lemma : f.lemma().split("॥")){
            QString l_key = QString("%1-%2").arg(f.form()).arg(f.lemma());
            qCDebug(g_cat_tmp_spotlight).noquote() << QString("l_key: [%1] found: %2").arg(l_key).arg(m_form_map.constFind(l_key) == m_form_map.cend() ? "No" : "yes");
            M1Store::Item_lv2* l_form = m_form_map[QString("%1-%2").arg(f.form()).arg(l_lemma)];
            M1Store::Item_lv2* l_occur_edge = cm_text_root->linkTo(l_form, M1Env::OCCUR_SIID, InsertionPoint::special_override, InsertionPoint::none);
            l_new_wfw_unit->linkTo(l_occur_edge, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
            l_form->linkTo(l_new_wfw_unit, M1Env::OCCURS_IN_SIID, InsertionPoint::at_bottom, InsertionPoint::none);
        }
        /*
        M1Store::Item_lv2* l_new_wfw_form = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            f.form());
        l_new_wfw_form->setType(M1Env::TEXT_WFW_FORM_SIID);
        l_new_wfw_unit->linkTo(l_new_wfw_form, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
        l_new_wfw_form->setFieldEdge(f.pos(), M1Env::TEXT_WFW_POS_SIID);
        l_new_wfw_form->setFieldEdge(f.grammar(), M1Env::TEXT_WFW_GRM_SIID);
        */
    }
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
    qCDebug(g_cat_tei).noquote() << p_indent << QString("Skip until <%1>").arg(p_elem_close);
    p_indent += QString(" ").repeated(p_indent_count);
    while (!p_xml_reader.atEnd()) {
        QXmlStreamReader::TokenType l_tt = p_xml_reader.readNext();
        if(p_xml_reader.hasError()){
            qCDebug(g_cat_tmp_spotlight).noquote() << "TEI read, skipUntil" << QString("p_xml_reader.error(): %1 %2")
                                                                                   .arg(p_xml_reader.error())
                                                                                   .arg(p_xml_reader.errorString());
            qCDebug(g_cat_tmp_spotlight).noquote() << "l_ret                         :" << l_ret;
            qCDebug(g_cat_tmp_spotlight).noquote() << "cm_cur_chapter_number         :" << cm_cur_chapter_number;
            qCDebug(g_cat_tmp_spotlight).noquote() << "cm_cur_sloka_number           :" << cm_cur_sloka_number;
            qCDebug(g_cat_tmp_spotlight).noquote() << "l_tt                          :" << l_tt;
            qCDebug(g_cat_tmp_spotlight).noquote() << "p_xml_reader.name()           :" << p_xml_reader.name();
            qCDebug(g_cat_tmp_spotlight).noquote() << "p_xml_reader.lineNumber()     :" << p_xml_reader.lineNumber();
            qCDebug(g_cat_tmp_spotlight).noquote() << "p_xml_reader.columnNumber()   :" << p_xml_reader.columnNumber();
            qCDebug(g_cat_tmp_spotlight).noquote() << "p_xml_reader.characterOffset():" << p_xml_reader.characterOffset();
            qCDebug(g_cat_tmp_spotlight).noquote() << "isCDATA                        " << (p_xml_reader.isCDATA() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isCharacters                   " << (p_xml_reader.isCharacters() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isComment                      " << (p_xml_reader.isComment() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isEndDocument                  " << (p_xml_reader.isDTD() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isEndDocument                  " << (p_xml_reader.isEndDocument() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isEndElement                   " << (p_xml_reader.isEndElement() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isEntityReference              " << (p_xml_reader.isEntityReference() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isProcessingInstruction        " << (p_xml_reader.isProcessingInstruction() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isStandaloneDocument           " << (p_xml_reader.isStandaloneDocument() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isStartDocument                " << (p_xml_reader.isStartDocument() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isStartElement                 " << (p_xml_reader.isStartElement() ? "true" : "false");
            qCDebug(g_cat_tmp_spotlight).noquote() << "isWhitespace                   " << (p_xml_reader.isWhitespace() ? "true" : "false");
            std::_Exit(0);
        }

        QStringView l_tok_name = p_xml_reader.name();
        qCDebug(g_cat_tei).noquote() << p_indent << "Token Type: " << l_tt << p_xml_reader.name();

        if(l_tt == QXmlStreamReader::EndElement && l_tok_name == p_elem_close){
            p_indent.chop(p_indent_count);
            qCDebug(g_cat_tei).noquote() << p_indent << QString("End of skip until <%1>: %2").arg(p_elem_close).arg(l_ret);
            break; // from while (!p_xml_reader.atEnd())
        }
        else {
            if(l_tt == QXmlStreamReader::StartElement &&  l_tok_name != "br") l_ret += QString("<%1>").arg(l_tok_name);
            else if(l_tt == QXmlStreamReader::EndElement) l_ret += QString("</%1>").arg(l_tok_name);
            else if(l_tt == QXmlStreamReader::Characters){
                QString l_characters = p_xml_reader.text().toString().trimmed();
                l_ret += l_characters;
            }
        }
    }
    return l_ret;
}

M1Store::AuthorTEI M1Store::TEIInterface::readPersName(QXmlStreamReader& p_xml_reader){
    M1Store::AuthorTEI l_ret;

    m_tt = p_xml_reader.readNext();
    if(m_tt == QXmlStreamReader::StartElement && p_xml_reader.name() == "persName"){
        QXmlStreamReader::TokenType l_tt_name = p_xml_reader.readNext();
        if(l_tt_name != QXmlStreamReader::Characters) throw M1Env::M1Exception("<persName> not followed by Characters", 1001);
        QString l_author_name = p_xml_reader.text().toString().trimmed();

        QXmlStreamReader::TokenType l_tt_role_1 = p_xml_reader.readNext();
        if(l_tt_role_1 != QXmlStreamReader::StartElement || p_xml_reader.name() != "roleName") throw M1Env::M1Exception("<roleName> not found", 1001);
        QXmlStreamReader::TokenType l_tt_role_2 = p_xml_reader.readNext();
        if(l_tt_role_2 != QXmlStreamReader::Characters) throw M1Env::M1Exception("<roleName> not followed by Characters", 1001);
        QString l_author_role = p_xml_reader.text().toString().trimmed();

        l_ret.set(l_author_name, l_author_role);

        QXmlStreamReader::TokenType l_tt_role_3 = p_xml_reader.readNext();
        if(l_tt_role_3 != QXmlStreamReader::EndElement || p_xml_reader.name() != "roleName") throw M1Env::M1Exception("</roleName> not found", 1001);
        QXmlStreamReader::TokenType l_tt_pers = p_xml_reader.readNext();
        if(l_tt_pers != QXmlStreamReader::EndElement || p_xml_reader.name() != "persName") throw M1Env::M1Exception("</persName> not found", 1001);
    }
    else m_no_read = true;

    return l_ret;
}
/**
 * @brief M1Store::TEIInterface::loadTeiInternal
 * @param p_full_load
 */
void M1Store::TEIInterface::loadTeiInternal(const QString& p_file_path, bool p_validate_only){
    QXmlStreamReader l_xml;
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
            qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<author> main : %1").arg(l_author_main);

            cm_indent += QString(" ").repeated(l_indent_count);

            while (!l_xml_reader.atEnd()) {
                QXmlStreamReader::TokenType l_tt = l_xml_reader.readNext();
                QStringView l_tok_name = l_xml_reader.name();
                qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "persName"){
                    QXmlStreamReader::TokenType l_tt_name = l_xml_reader.readNext();
                    if(l_tt_name != QXmlStreamReader::Characters) throw M1Env::M1Exception("<persName> not followed by Characters", 1001);
                    QString l_author_name = l_xml_reader.text().toString().trimmed();

                    QXmlStreamReader::TokenType l_tt_role_1 = l_xml_reader.readNext();
                    if(l_tt_role_1 != QXmlStreamReader::StartElement || l_xml_reader.name() != "roleName") throw M1Env::M1Exception("<roleName> not found", 1001);
                    QXmlStreamReader::TokenType l_tt_role_2 = l_xml_reader.readNext();
                    if(l_tt_role_2 != QXmlStreamReader::Characters) throw M1Env::M1Exception("<roleName> not followed by Characters", 1001);
                    QString l_author_role = l_xml_reader.text().toString().trimmed();

                    m_author_list.append(AuthorTEI(l_author_name, l_author_role));
                    qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<persName> 2ndary author : %1/%2").arg(l_author_name).arg(l_author_role);
                }
                else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
                    qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<author/> %1 (%2) [%3] author: %4")
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
            qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<teiHeader/> %1 (%2) [%3] author: %4")
            .arg(l_title)
                .arg(l_alt_title)
                .arg(l_sub_title)
                .arg(l_author_main);

            // + m_author_list as built above
            create_text(l_title, l_alt_title, l_sub_title, l_author_main);
        }
        // lexicon or chapter start
        else if(l_tt == QXmlStreamReader::StartElement && l_tok_name == "div1"){
            QString l_type = l_xml_reader.attributes().value("type").toString();
            if(l_type == "chapter"){
                // chapter start
                cm_cur_chapter_number = l_xml_reader.attributes().value("n").toInt();
                l_found_at_least_one_chapter = true;
                l_found_at_least_one_sloka = false;
                qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<div1 type=%1> Chapter %2 start").arg(l_type).arg(cm_cur_chapter_number);

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
                            qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("    form: %1 Grammar: %2").arg(f.form()).arg(f.grammar());
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
            qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<div2 type=%1> Sloka %2.%3 start").arg(l_type).arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
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

                qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<div3> start of wfw block for sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
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
                                                                             .arg(cm_cur_chapter_number).arg(cm_cur_sloka_number).arg(l_cur_word)
                                                                             .arg(l_type).arg(l_wfw_form);
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
                        qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << "End of wfw block";
                        break; // from the inner wfw while
                    }
                    l_cur_word += 1;
                } // end of wfw while
            }
            // translations block
            else if(l_type == "translations-block"){
                l_found_translations = true;
                qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<div3> start of translation block for sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
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
                qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("<div3> start of commentaries block for sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);
                cm_indent += QString(" ").repeated(l_indent_count);

                QString l_source_txt;
                QString l_language;
                AuthorTEI l_main_author;
                AuthorTEI l_secondary_author;

                bool l_is_first = true;

                while (!l_xml_reader.atEnd()) {
                    if(! m_no_read){ // because readPersName() may read a token ahead
                        m_tt = l_xml_reader.readNext();
                        Q_ASSERT_X(!l_xml_reader.hasError(),
                                   "TEI read, Sloka comment section",
                                   QString("l_xml_reader.error(): %1").arg(l_xml_reader.error()).toUtf8().constData());
                    }
                    m_no_read = false;
                    QStringView l_tok_name = l_xml_reader.name();
                    qCDebug(g_cat_tei).noquote() << cm_indent << "Token Type: " << l_tt << l_xml_reader.name();

                    if(m_tt == QXmlStreamReader::StartElement && l_tok_name == "div4"){
                        QString l_type = l_xml_reader.attributes().value("type").toString();
                        l_language = l_xml_reader.attributes().value("xml:lang").toString();
                        if(l_type == "commentary"){
                            l_source_txt = l_xml_reader.attributes().value("source").toString();
                            qCDebug(g_cat_tei).noquote() << cm_indent << QString("<div4 type=\"%1\">").arg(l_type);
                        }
                    }
                    else if(m_tt == QXmlStreamReader::StartElement && l_tok_name == "author"){
                        l_main_author = readPersName(l_xml_reader);
                        l_secondary_author = readPersName(l_xml_reader);
                        if(l_main_author.isEmpty()) throw M1Env::M1Exception("No main author", 1001);
                    }
                    else if(m_tt == QXmlStreamReader::EndElement && l_tok_name == "author"){
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
                    else if(m_tt == QXmlStreamReader::EndElement && l_tok_name == "div3"){
                        cm_indent.chop(l_indent_count);
                        qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << "End of commentaries block";
                        break; // from the inner commentaries while
                    }
                } // end of commentaries while
            } // end of <div3> (wfw or commentaries or translations blocks)
        }
        // chapter end
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div1"){
            if(! l_found_at_least_one_sloka) throw M1Env::M1Exception(QString("Chapter %1 has 0 Sloka").arg(cm_cur_chapter_number), 0);
            qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("End of chapter %1").arg(cm_cur_chapter_number);
        }
        // Sloka end
        else if(l_tt == QXmlStreamReader::EndElement && l_tok_name == "div2"){
            if(! l_found_sk) throw M1Env::M1Exception(QString("Sloka %1.%2 has no sanskrit text").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_trnsl) throw M1Env::M1Exception(QString("Sloka %1.%2 has no transliteration text").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_wfw) throw M1Env::M1Exception(QString("Sloka %1.%2 has no wfw section").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_translations) throw M1Env::M1Exception(QString("Sloka %1.%2 has no translations section").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);
            if(! l_found_commentaries) throw M1Env::M1Exception(QString("Sloka %1.%2 has no commentaries section").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number), 0);

            qCDebug(g_cat_tmp_spotlight).noquote() << cm_indent << QString("End of Sloka %1.%2").arg(cm_cur_chapter_number).arg(cm_cur_sloka_number);

            // if(cm_cur_chapter_number == 1 && cm_cur_sloka_number == 10) break;
        }
    }

    // end of text
    if(! l_found_title) throw M1Env::M1Exception("No title", 1021);
    if(! l_found_author) throw M1Env::M1Exception("No author", 1022);
    if(! l_found_at_least_one_chapter) throw M1Env::M1Exception("No chapters", 1023);
}
