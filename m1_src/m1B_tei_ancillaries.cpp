#include <QXmlStreamReader>
#include <QElapsedTimer>
#include <QRegularExpression>

// #include <iostream>

#include "m1B_tei_interface.h"
#include "m1A_env.h"
#include "m1A_constants.h"
#include "m1B_graph_init.h"

/**
 * @brief M1Store::TEIInterface::cm_text_root
 */
M1Store::Item_lv2* M1Store::TEIInterface::cm_text_root;
M1Store::Item_lv2* M1Store::TEIInterface::cm_lexicon_root;
M1Store::Item_lv2* M1Store::TEIInterface::cm_cur_chapter_vertex;
M1Store::Item_lv2* M1Store::TEIInterface::cm_cur_sloka_vertex;
std::map<QString, M1Store::Item_lv2*> M1Store::TEIInterface::cm_cache_tei_ent;

int M1Store::TEIInterface::cm_cur_chapter_number;
int M1Store::TEIInterface::cm_cur_sloka_number;
QString M1Store::TEIInterface::cm_indent;

QList<M1Store::AuthorTEI> M1Store::TEIInterface::cm_author_list;
QMap<QString, M1Store::Item_lv2*> M1Store::TEIInterface::cm_form_map;

QXmlStreamReader::TokenType M1Store::TEIInterface::cm_tt;
bool M1Store::TEIInterface::cm_no_read = false;

std::stack<M1Store::TEIStackLevel> M1Store::TEIInterface::cm_tei_elements_stack;
int M1Store::TEIInterface::cm_cur_paragraph_number;

M1Store::Item_lv2* M1Store::TEIInterface::create_div(
    const QString& p_type,
    const QString& p_div_n,
    const QString& p_xml_id){

    M1Store::Item_lv2* l_new_div = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("type=[%1] n=%2").arg(p_type).arg(p_div_n)
    );
    l_new_div->setType(M1Env::TEXT_DIV_SIID);

    if(cm_tei_elements_stack.empty())
        cm_text_root->linkTo(l_new_div, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    else
        cm_tei_elements_stack.top().item()->linkTo(l_new_div, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    if(p_type.length() > 0)l_new_div->setFieldEdge(p_type, M1Env::TEI_TYPE_ATTR_SIID);
    if(p_div_n.length() > 0)l_new_div->setFieldEdge(p_div_n, M1Env::TEI_N_ATTR_SIID);
    if(p_xml_id.length() > 0) l_new_div->setFieldEdge(p_xml_id, M1Env::TEI_XMLID_ATTR_SIID);

    return l_new_div;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_paragraph(){
    M1Store::Item_lv2* l_new_paragraph = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("Paragraph %1").arg(cm_cur_paragraph_number++)
        );
    l_new_paragraph->setType(M1Env::TEXT_PARA_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_paragraph, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    return l_new_paragraph;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_chunk(
    const QString& p_text,
    const QString& p_tag,
    const QString& p_type,
    const QString& p_attr_n,
    const QString& p_note_place,
    const QString& p_hi_rend,
    const QString& p_xml_id
){
    M1Store::Item_lv2* l_new_chunk = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_text
        );
    l_new_chunk->setType(M1Env::TEXT_CHUNK_SIID);

    l_new_chunk->setFieldEdge(p_tag, M1Env::TEI_TAG_NAME_SIID);
    if(p_type.length() > 0) l_new_chunk->setFieldEdge(p_type, M1Env::TEI_TYPE_ATTR_SIID);
    if(p_attr_n.length() > 0) l_new_chunk->setFieldEdge(p_attr_n, M1Env::TEI_N_ATTR_SIID);
    if(p_xml_id.length() > 0) l_new_chunk->setFieldEdge(p_xml_id, M1Env::TEI_XMLID_ATTR_SIID);
    if(p_hi_rend.length() > 0) l_new_chunk->setFieldEdge(p_hi_rend, M1Env::TEI_REND_ATTR_SIID);
    if(p_note_place.length() > 0) l_new_chunk->setFieldEdge(p_note_place, M1Env::TEI_PLACE_ATTR_SIID);

    cm_tei_elements_stack.top().item()->linkTo(l_new_chunk, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    return l_new_chunk;
}

/**
 * @brief M1Store::TEIInterface::create_text
 *
 * Create the root node of a TEI text
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
    const bool p_with_lexicon,
    M1Store::Item_lv2** p_text_root,
    M1Store::Item_lv2** p_lexicon_root
){
    M1Store::Item_lv2** l_text_root = p_text_root;
    M1Store::Item_lv2** l_lexicon_root = p_lexicon_root;

    if(l_text_root == nullptr) l_text_root = &cm_text_root;
    if(l_lexicon_root == nullptr) l_lexicon_root = &cm_lexicon_root;

    // M1Store::Item_lv2** l_text_root = &cm_text_root;
    // M1Store::Item_lv2** l_lexicon_root = &cm_lexicon_root;

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
    if(p_with_lexicon){
        (*l_lexicon_root) = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            "Lexicon");
        (*l_lexicon_root)->setType(M1Env::FOLDER_SIID);
        (*l_text_root)->linkTo((*l_lexicon_root), M1Env::OWNS_SIID);
    }
}

/**
 * @brief M1Store::TEIInterface::addTranslationBhashya
 *
 * Add a translation or bhashya to the current sloka (cm_cur_sloka_vertex)
 *
 * Called by loadTeiInternal1()
 *
 * @param p_translation_text
 * @param p_language
 * @param p_source
 * @param p_main_author
 * @param p_secondary_author
 * @param p_is_translation
 * @param p_sep_above
 */
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
 * @param p_name Person or Org name
 * @param p_role "" --> No role
 * @param p_is_person type of the entity to be created or retrieved (PERS_TYPE_SIID or ORG_TYPE_SIID)
 * @param p_return ReturnEntityOrRole::Entity --> the entity itself (person or org); otherwise the role folder (or nullptr if none found)
 * @return see above. In all cases, an Item_lv2 pointer
 */
M1Store::Item_lv2* M1Store::TEIInterface::getPersonOrg(const QString& p_name, const QString& p_role, OrgOrPerson p_is_person, ReturnEntityOrRole p_return){
    M1Store::SpecialItemID l_entity_type_siid = p_is_person == OrgOrPerson::Person ? M1Env::PERS_TYPE_SIID : M1Env::ORG_TYPE_SIID;

    M1Store::Item_lv2* l_entity_existing = cm_cache_tei_ent[p_name];
    M1Store::Item_lv2* l_role_folder = nullptr;

    if(l_entity_existing == nullptr){
        l_entity_existing = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            p_name);
        l_entity_existing->setType(l_entity_type_siid);
        cm_cache_tei_ent[p_name] = l_entity_existing;
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

/**
 * @brief M1Store::TEIInterface::create_Lemma
 * @param p_lemma_text
 * @param p_pos_text
 * @param p_url_dict_list
 * @param p_lexicon_root
 * @return
 */
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

    if(p_url_dict_list.length() > 0){
        M1Store::Item_lv2* l_new_url = M1Store::Item_lv2::getNew(
            // vertex flags
            M1Env::FULL_VERTEX,
            // label
            p_url_dict_list);
        l_new_url->setType(M1Env::TEXT_URL_LINK_SIID);
        l_new_lemma->linkTo(l_new_url, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    }
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
        qCDebug(g_cat_tei).noquote() << QString("l_key: [%1]").arg(l_key);
        cm_form_map[l_key] = l_new_form;

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
            qCDebug(g_cat_tei).noquote() << QString("l_key: [%1] found: %2").arg(l_key).arg(cm_form_map.constFind(l_key) == cm_form_map.cend() ? "No" : "yes");
            M1Store::Item_lv2* l_form = cm_form_map[QString("%1-%2").arg(f.form()).arg(l_lemma)];
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

    cm_tt = p_xml_reader.readNext();
    if(cm_tt == QXmlStreamReader::StartElement && p_xml_reader.name() == "persName"){
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
    else cm_no_read = true;

    return l_ret;
}

QString M1Store::TEIInterface::format_tt_token(const QXmlStreamReader& p_xml_reader){
    QString l_tok_name = p_xml_reader.name().toString();
    return ((p_xml_reader.tokenType() != QXmlStreamReader::Characters) ?
                ((p_xml_reader.tokenType() == QXmlStreamReader::StartElement) ? QString("<%1>").arg(l_tok_name) :
                     ((p_xml_reader.tokenType() == QXmlStreamReader::EndElement) ? QString("</%1>").arg(l_tok_name) : l_tok_name)) :
                M1Store::StorageStatic::maxLengthChop(p_xml_reader.text().toString().trimmed(), 100));
}

