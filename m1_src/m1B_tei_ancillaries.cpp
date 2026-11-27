#include <QXmlStreamReader>
#include <QElapsedTimer>
#include <QRegularExpression>

// #include <iostream>

#include "m1B_tei_interface.h"
#include "m1A_env.h"
#include "m1A_constants.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_iterators.h"

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
M1Store::Item_lv2* M1Store::TEIInterface::cm_last_created = nullptr;
QMap<QString, M1Store::Item_lv2*> M1Store::TEIInterface::cm_person_by_key;
bool M1Store::TEIInterface::cm_person_cache_loaded = false;
QMap<QString, M1Store::PersonTEI> M1Store::TEIInterface::cm_list_person;
QSet<QString> M1Store::TEIInterface::cm_person_done;
int M1Store::TEIInterface::cm_cur_paragraph_number;

M1Store::Item_lv2* M1Store::TEIInterface::create_list(const QString& p_list_type){
    M1Store::Item_lv2* l_new_list = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("<list type=%1>").arg(p_list_type)
        );
    l_new_list->setType(M1Env::TEXT_LIST_SIID);
    if(p_list_type.length() > 0) l_new_list->setFieldEdge(QString("%1").arg(p_list_type), M1Env::TEI_LIST_TYPE_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_list, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    cm_last_created = l_new_list;
    return l_new_list;
}
M1Store::Item_lv2* M1Store::TEIInterface::create_item(){
    M1Store::Item_lv2* l_new_item = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("<item>")
        );
    l_new_item->setType(M1Env::TEXT_ITEM_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_item, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    cm_last_created = l_new_item;
    return l_new_item;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_figure(const QString& p_picture_path){
    M1Store::Item_lv2* l_new_row = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("<figure>")
        );
    l_new_row->setType(M1Env::TEXT_FIGURE_SIID);
    if(p_picture_path.length() > 0) l_new_row->setFieldEdge(QString("%1").arg(p_picture_path), M1Env::TEI_GRAPHIC_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_row, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    cm_last_created = l_new_row;
    return l_new_row;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_table_row(const QString &p_role){
    M1Store::Item_lv2* l_new_row = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("<row role=%1>").arg(p_role)
        );
    l_new_row->setType(M1Env::TEXT_ROW_SIID);
    if(p_role.length() > 0) l_new_row->setFieldEdge(QString("%1").arg(p_role), M1Env::TEI_ROW_ROLE_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_row, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    cm_last_created = l_new_row;
    return l_new_row;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_table_cell(){
    M1Store::Item_lv2* l_new_cell = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("<cell>")
        );
    l_new_cell->setType(M1Env::TEXT_CELL_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_cell, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    cm_last_created = l_new_cell;
    return l_new_cell;
}
M1Store::Item_lv2* M1Store::TEIInterface::create_table(
    const int p_rows,
    const int p_columns,
    const QString& p_xml_id){

    M1Store::Item_lv2* l_new_table = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        QString("<table> %1 rows by %2 columns").arg(p_rows).arg(p_columns)
        );
    l_new_table->setType(M1Env::TEXT_TABLE_SIID);
    l_new_table->setFieldEdge(QString("%1").arg(p_rows), M1Env::TEI_TABLE_ROWS_SIID);
    l_new_table->setFieldEdge(QString("%1").arg(p_columns), M1Env::TEI_TABLE_COLS_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_table, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    cm_last_created = l_new_table;
    return l_new_table;
}

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

    cm_last_created = l_new_div;
    return l_new_div;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_paragraph_quote(const bool p_is_quote){
    M1Store::Item_lv2* l_new_paragraph = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        p_is_quote ? "<quote>" : QString("Paragraph %1").arg(cm_cur_paragraph_number++)
    );
    if(p_is_quote)
        l_new_paragraph->setType(M1Env::TEXT_QUOTE_SIID);
    else
        l_new_paragraph->setType(M1Env::TEXT_PARA_SIID);
    cm_tei_elements_stack.top().item()->linkTo(l_new_paragraph, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);

    cm_last_created = l_new_paragraph;
    return l_new_paragraph;
}

M1Store::Item_lv2* M1Store::TEIInterface::create_chunk(
    const QString& p_text,
    const QString& p_tag,
    const QString& p_type,
    const QString& p_attr_n,
    const QString& p_note_place,
    const QString& p_hi_rend,
    const QString& p_xml_id,
    const QString& p_xml_lang
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
    if(p_xml_lang.length() > 0) l_new_chunk->setFieldEdge(p_xml_lang, M1Env::TEI_LANG_ATTR_SIID);

    // leading space: only when the source has whitespace between the previous sibling and this
    // chunk (tracked by TEIStackLevel::spacePending(), see the Characters branch of loadTeiInternal()),
    // so that e.g. "(<date>2020</date>)" or "AT&amp;T" are reproduced without spurious spaces. Never
    // for the container's first child (nothing to separate from) nor for <note>/<l>/<lg>, which are
    // always rendered as self-contained blocks (footnote popup, <br/>, indented <div>)
    bool l_is_first_child = cm_tei_elements_stack.top().lastChild() == nullptr;
    if(!l_is_first_child && cm_tei_elements_stack.top().spacePending() && p_tag != "note" && p_tag != "l" && p_tag != "lg")
        l_new_chunk->setFieldEdge(" ", M1Env::PCTLF_SIID);
    // from now on, we are inside (or just after) this chunk: no whitespace seen yet
    cm_tei_elements_stack.top().setSpacePending(false);

    cm_tei_elements_stack.top().item()->linkTo(l_new_chunk, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
    cm_tei_elements_stack.top().setLastChild(l_new_chunk);

    cm_last_created = l_new_chunk;
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
    (*l_text_root)->setType(M1Env::TEXT_SIID);
    if(p_alt_title.length() > 0) (*l_text_root)->setFieldEdge(p_alt_title, M1Env::TEXT_ALT_TITLE_SIID);
    if(p_sub_title.length() > 0) (*l_text_root)->setFieldEdge(p_sub_title, M1Env::TEXT_SUB_TITLE_SIID);

    // Author (main author role is just 'Author'). None if the header has no author (e.g. chapters of a work, whose author
    // is that of the work, see linkTextToWork())
    if(p_author_text.trimmed().length() > 0){
        M1Store::Item_lv2* l_author = getPersonOrg(p_author_text, "Author", OrgOrPerson::Person, ReturnEntityOrRole::Entity);
        (*l_text_root)->linkTo(l_author, M1Env::TEXT_WRITTEN_BY_SIID);
    }

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
    QString l_ret;

    if(p_xml_reader.tokenType() == QXmlStreamReader::StartElement){
        QXmlStreamAttributes attributes = p_xml_reader.attributes();
        QString l_attr_string;
        for (const QXmlStreamAttribute &l_attr : attributes) {
            // Reconstruct the string: name="value"
            l_attr_string += l_attr.qualifiedName().toString() +
                          "=\"" +
                          l_attr.value().toString() +
                          "\" ";
        }
        l_ret = QString("<%1>%2").arg(l_tok_name).arg(l_attr_string.length() > 0 ? " " + l_attr_string : "");
    }
    else
        l_ret = ((p_xml_reader.tokenType() == QXmlStreamReader::Characters) ?
                    "CHAR: " + M1Store::StorageStatic::maxLengthChop(p_xml_reader.text().toString().trimmed(), 50) :
                    ((p_xml_reader.tokenType() == QXmlStreamReader::EndElement) ?
                        QString("</%1>").arg(l_tok_name) :
                        "Bare Token: " + l_tok_name));

    return l_ret + QString(" [%1]").arg(p_xml_reader.tokenType());
}


/**
 * @brief Read the <listPerson> of the TEI header (reader positioned on its start tag) into cm_list_person
 *
 * Only <person> elements having an @xml:id and at least one <persName> are kept. Within a <person>, only <persName>,
 * <birth> and <death> are used (<note>, etc. are ignored). Nested lists are skipped. On return, the reader is
 * positioned on the </listPerson> end tag.
 *
 * @param p_reader the XML reader
 */
void M1Store::TEIInterface::readListPerson(QXmlStreamReader& p_reader){
    while(p_reader.readNextStartElement()){
        if(p_reader.name() != QString("person")){
            p_reader.skipCurrentElement();
            continue;
        }
        QString l_key = p_reader.attributes().value("xml:id").toString().trimmed();
        PersonTEI l_person;
        while(p_reader.readNextStartElement()){
            if(p_reader.name() == QString("persName")) l_person.m_names.append(readListPersName(p_reader));
            else if(p_reader.name() == QString("birth")) l_person.m_birth = readPersonDate(p_reader);
            else if(p_reader.name() == QString("death")) l_person.m_death = readPersonDate(p_reader);
            else p_reader.skipCurrentElement();
        }
        if(l_key.length() > 0 && l_person.m_names.size() > 0){
            if(cm_list_person.contains(l_key))
                qCDebug(g_cat_tei).noquote() << QString("<listPerson> duplicate <person xml:id=\"%1\"> (last one kept)").arg(l_key);
            cm_list_person[l_key] = l_person;
        }
        else
            qCDebug(g_cat_tei).noquote() << QString("<listPerson> <person> without @xml:id or <persName> ignored [%1]").arg(l_key);
    }
    qCDebug(g_cat_tei).noquote() << QString("<listPerson> %1 person(s)").arg(cm_list_person.size());
}

/**
 * @brief Read a <persName> (reader positioned on its start tag) and map its parts to Person fields
 *
 * - <roleName type="honorific"> --> PERHN, other <roleName> --> PERTL (", " separated)
 * - <forename type="first"> or first untyped <forename> --> PERFN, other <forename> --> PERMN
 * - <genName type="generational"> --> PERGN, other <genName> (regnal) --> PERRN
 * - <surname type="patronymic">, <addName type="patronymic"> --> PERPN
 * - <surname> --> PERLN
 * - <addName type="toponymic">, <placeName> --> PERTB
 * - <addName type="alias"> --> PERAL (", " separated)
 * - other <addName> (cognomen, epithet, nickname, sobriquet, ...) --> PERNN (", " separated, document order)
 * - <nameLink> is prefixed to the following part ("de" + "Sade" --> "de Sade", "d'" + "Arc" --> "d'Arc")
 *
 * The text of a part includes that of its descendants (<addName><nameLink>of</nameLink> <placeName>Babylon</placeName></addName>
 * --> "of Babylon"). On return, the reader is positioned on the </persName> end tag.
 *
 * @param p_reader the XML reader
 * @return the name fields, in the canonical field order (see PersonInterp::namesText())
 */
M1Store::PersNameTEI M1Store::TEIInterface::readListPersName(QXmlStreamReader& p_reader){
    PersNameTEI l_name;
    l_name.m_type = p_reader.attributes().value("type").toString().trimmed();
    l_name.m_lang = p_reader.attributes().value("xml:lang").toString().trimmed();

    QMap<M1Env::SpecialItemID, QStringList> l_parts;
    QString l_pending_link;
    while(p_reader.readNextStartElement()){
        QString l_tag = p_reader.name().toString();
        QString l_type = p_reader.attributes().value("type").toString().trimmed();
        QString l_text = p_reader.readElementText(QXmlStreamReader::IncludeChildElements).simplified();
        if(l_text.length() == 0) continue;

        if(l_tag == "nameLink"){
            l_pending_link = l_pending_link.length() > 0 ? l_pending_link + " " + l_text : l_text;
            continue;
        }
        if(l_pending_link.length() > 0){
            l_text = (l_pending_link.endsWith('\'') || l_pending_link.endsWith(QChar(0x2019))) ? l_pending_link + l_text : l_pending_link + " " + l_text;
            l_pending_link.clear();
        }

        M1Env::SpecialItemID l_field;
        if(l_tag == "roleName") l_field = (l_type == "honorific") ? M1Env::PERS_HONORIFIC_SIID : M1Env::PERS_TITLE_SIID;
        else if(l_tag == "forename")
            l_field = (l_type == "first" || (l_type != "middle" && !l_parts.contains(M1Env::PERS_FIRST_NAME_SIID))) ?
                          M1Env::PERS_FIRST_NAME_SIID : M1Env::PERS_MIDDLE_NAME_SIID;
        else if(l_tag == "genName") l_field = (l_type == "generational") ? M1Env::PERS_GENERATIONAL_SIID : M1Env::PERS_REGNAL_SIID;
        else if(l_tag == "surname") l_field = (l_type == "patronymic") ? M1Env::PERS_PATRONYMIC_SIID : M1Env::PERS_LAST_NAME_SIID;
        else if(l_tag == "placeName") l_field = M1Env::PERS_TOPONYMIC_SIID;
        else if(l_tag == "addName"){
            if(l_type == "patronymic") l_field = M1Env::PERS_PATRONYMIC_SIID;
            else if(l_type == "toponymic") l_field = M1Env::PERS_TOPONYMIC_SIID;
            else if(l_type == "alias") l_field = M1Env::PERS_ALIAS_SIID;
            else l_field = M1Env::PERS_NICKNAME_SIID;
        }
        else{
            qCDebug(g_cat_tei).noquote() << QString("<persName> part <%1> ignored [%2]").arg(l_tag).arg(l_text);
            continue;
        }
        l_parts[l_field].append(l_text);
    }
    // trailing <nameLink> with nothing after it
    if(l_pending_link.length() > 0) l_parts[M1Env::PERS_LAST_NAME_SIID].append(l_pending_link);

    for(const M1Env::SpecialItemID l_field : persNameFields())
        if(l_parts.contains(l_field)){
            bool l_comma = (l_field == M1Env::PERS_TITLE_SIID || l_field == M1Env::PERS_NICKNAME_SIID || l_field == M1Env::PERS_ALIAS_SIID);
            l_name.m_fields.append({l_field, l_parts[l_field].join(l_comma ? ", " : " ")});
        }
    return l_name;
}

/**
 * @brief Read a <birth> or <death> element (reader positioned on its start tag)
 *
 * The element text is used if there is one ("c. 1810 BCE"), otherwise @when, or @notBefore/@notAfter ("1820 BCE/1800 BCE"),
 * or @from/@to. ISO years lose their leading zeros ("0565-11-14" --> "565-11-14") and negative years become BCE
 * ("-0685" --> "685 BCE").
 *
 * @param p_reader the XML reader
 * @return the date text ("" if none)
 */
QString M1Store::TEIInterface::readPersonDate(QXmlStreamReader& p_reader){
    auto l_fmt = [](const QString& p_iso) -> QString {
        static const QRegularExpression ls_re(R"(^(-?)0*(\d+)(.*)$)");
        QRegularExpressionMatch l_m = ls_re.match(p_iso.trimmed());
        if(!l_m.hasMatch()) return p_iso.trimmed();
        if(l_m.captured(1) == "-") return QString("%1%2 BCE").arg(l_m.captured(2)).arg(l_m.captured(3));
        return l_m.captured(2) + l_m.captured(3);
    };
    QXmlStreamAttributes l_attr = p_reader.attributes();
    QString l_text = p_reader.readElementText(QXmlStreamReader::IncludeChildElements).simplified();
    if(l_text.length() > 0) return l_text;
    if(l_attr.hasAttribute("when")) return l_fmt(l_attr.value("when").toString());
    for(const auto& l_range : {std::pair<QString, QString>{"notBefore", "notAfter"}, std::pair<QString, QString>{"from", "to"}}){
        QString l_lo = l_attr.value(l_range.first).toString();
        QString l_hi = l_attr.value(l_range.second).toString();
        if(l_lo.length() > 0 && l_hi.length() > 0) return QString("%1/%2").arg(l_fmt(l_lo)).arg(l_fmt(l_hi));
        if(l_lo.length() > 0) return QString("after %1").arg(l_fmt(l_lo));
        if(l_hi.length() > 0) return QString("before %1").arg(l_fmt(l_hi));
    }
    return "";
}

/**
 * @brief Set the name fields of a Person node from a <persName>
 */
void M1Store::TEIInterface::setPersNameFields(M1Store::Item_lv2* p_person, const PersNameTEI& p_name){
    for(const auto& l_field : p_name.m_fields)
        p_person->setFieldEdge(l_field.second, l_field.first);
}

/**
 * @brief Fill cm_person_by_key from the existing Person nodes having an NMTMK key (once per loadTeiInternal() call)
 *
 * Existing Person nodes (from previous imports) are found through the ITO edges of the Person type node
 */
void M1Store::TEIInterface::loadPersonCache(){
    if(cm_person_cache_loaded) return;
    cm_person_by_key.clear();
    M1Store::Item_lv2* l_person_type = M1Store::Item_lv2::getExisting(M1Env::PERS_TYPE_SIID);
    for(M1Store::Item_lv2_iterator l_it = l_person_type->getIteratorTop(); !l_it.beyondEnd(); l_it.next())
        if(l_it.at()->isFullEdge() && l_it.at()->isOfType(M1Env::ITO_SIID)){
            M1Store::Item_lv2* l_person = l_it.at()->getTarget_lv2();
            QString l_key = l_person->getField(M1Env::TEI_NAME_TERM_KEY_SIID);
            if(l_key.length() > 0) cm_person_by_key[l_key] = l_person;
        }
    cm_person_cache_loaded = true;
    qCDebug(g_cat_tei).noquote() << QString("Person cache loaded: %1 keyed Person node(s)").arg(cm_person_by_key.size());
}

/**
 * @brief Person node for a work author (<monogr>/<author xml:id="...">), created if necessary
 *
 * - key in <listPerson> --> getPersonFromKey()
 * - otherwise, existing Person with that NMTMK key
 * - otherwise, new Person: last word of the author text --> PERLN, the rest --> PERFN, key --> NMTMK
 *
 * @param p_key @xml:id (or @ref without '#') of the <author>; if empty, no Person is returned
 * @param p_text text of the <author> ("Nicolas Reimen")
 * @return the Person node (nullptr if no key)
 */
M1Store::Item_lv2* M1Store::TEIInterface::getPersonForAuthor(const QString& p_key, const QString& p_text){
    if(p_key.length() == 0) return nullptr;
    if(cm_list_person.contains(p_key)) return getPersonFromKey(p_key);

    loadPersonCache();
    M1Store::Item_lv2* l_person = cm_person_by_key.value(p_key, nullptr);
    if(l_person == nullptr){
        l_person = M1Store::Item_lv2::getNew(M1Env::FULL_VERTEX, "");
        l_person->setType(M1Env::PERS_TYPE_SIID);
        QStringList l_words = p_text.simplified().split(' ', Qt::SkipEmptyParts);
        if(l_words.size() > 0) l_person->setFieldEdge(l_words.takeLast(), M1Env::PERS_LAST_NAME_SIID);
        if(l_words.size() > 0) l_person->setFieldEdge(l_words.join(" "), M1Env::PERS_FIRST_NAME_SIID);
        l_person->setFieldEdge(p_key, M1Env::TEI_NAME_TERM_KEY_SIID);
        cm_person_by_key[p_key] = l_person;
        qCDebug(g_cat_tei).noquote() << QString("New Person (work author) [%1] %2").arg(p_key).arg(p_text);
    }
    return l_person;
}

/**
 * @brief Read the header <monogr> (reader positioned on its start tag): the work the text belongs to
 *
 * On return, the reader is positioned on the </monogr> end tag.
 *
 * @param p_reader the XML reader
 * @param p_title [out] work title (<title> text)
 * @param p_key [out] @xml:id of the <title> ("" if none)
 * @param p_authors [out] (key, text) of each <author> (key = @xml:id or @ref without '#')
 */
void M1Store::TEIInterface::readMonogr(QXmlStreamReader& p_reader, QString& p_title, QString& p_key, QList<std::pair<QString, QString>>& p_authors){
    while(p_reader.readNextStartElement()){
        if(p_reader.name() == QString("title") && p_title.length() == 0){
            p_key = p_reader.attributes().value("xml:id").toString().trimmed();
            p_title = p_reader.readElementText(QXmlStreamReader::IncludeChildElements).simplified();
        }
        else if(p_reader.name() == QString("author")){
            QString l_key = p_reader.attributes().value("xml:id").toString().trimmed();
            if(l_key.length() == 0){
                l_key = p_reader.attributes().value("ref").toString().trimmed();
                if(l_key.startsWith('#')) l_key = l_key.mid(1);
            }
            p_authors.append({l_key, p_reader.readElementText(QXmlStreamReader::IncludeChildElements).simplified()});
        }
        else p_reader.skipCurrentElement();
    }
    qCDebug(g_cat_tei).noquote() << QString("<monogr> [%1] %2 (%3 author(s))").arg(p_key).arg(p_title).arg(p_authors.size());
}

/**
 * @brief Connect the text root to the work it belongs to (from the header <monogr>): text --Belongs to--> work
 *
 * The work (type WORKS, label = title, TEXID = @xml:id of the <title>) is looked up among the existing works (by TEXID
 * if there is a key, by label otherwise). If it does not exist, it is created and linked to its author(s)
 * (work --Written by--> Person, see getPersonForAuthor()); an existing work is left as is.
 *
 * @param p_text the text root node
 * @param p_title work title
 * @param p_key @xml:id of the <monogr>/<title>
 * @param p_authors (key, text) of the <monogr>/<author> elements
 */
void M1Store::TEIInterface::linkTextToWork(M1Store::Item_lv2* p_text, const QString& p_title, const QString& p_key,
                                           const QList<std::pair<QString, QString>>& p_authors){
    if(p_title.length() == 0 || p_text == nullptr) return;

    M1Store::Item_lv2* l_work = nullptr;
    M1Store::Item_lv2* l_works_type = M1Store::Item_lv2::getExisting(M1Env::WORKS_SIID);
    for(M1Store::Item_lv2_iterator l_it = l_works_type->getIteratorTop(); !l_it.beyondEnd() && l_work == nullptr; l_it.next())
        if(l_it.at()->isFullEdge() && l_it.at()->isOfType(M1Env::ITO_SIID)){
            M1Store::Item_lv2* l_candidate = l_it.at()->getTarget_lv2();
            if(p_key.length() > 0 ? l_candidate->getField(M1Env::TEI_XMLID_ATTR_SIID) == p_key : QString(l_candidate->text()) == p_title)
                l_work = l_candidate;
        }

    if(l_work == nullptr){
        l_work = M1Store::Item_lv2::getNew(M1Env::FULL_VERTEX, p_title.toUtf8().constData());
        l_work->setType(M1Env::WORKS_SIID);
        if(p_key.length() > 0) l_work->setFieldEdge(p_key, M1Env::TEI_XMLID_ATTR_SIID);
        for(const auto& l_author : p_authors){
            M1Store::Item_lv2* l_person = getPersonForAuthor(l_author.first, l_author.second);
            if(l_person != nullptr)
                l_work->linkTo(l_person, M1Env::TEXT_WRITTEN_BY_SIID, InsertionPoint::at_bottom, InsertionPoint::at_bottom);
        }
        qCDebug(g_cat_tei).noquote() << QString("New work [%1] %2").arg(p_key).arg(p_title);
    }

    // chapters listed below the work in import order
    p_text->linkTo(l_work, M1Env::BLNGS_SIID, InsertionPoint::at_top, InsertionPoint::at_bottom);
}

/**
 * @brief Person name fields, in canonical (display) order
 */
const QList<M1Env::SpecialItemID>& M1Store::TEIInterface::persNameFields(){
    static const QList<M1Env::SpecialItemID> ls_fields{
        M1Env::PERS_HONORIFIC_SIID, M1Env::PERS_FIRST_NAME_SIID, M1Env::PERS_MIDDLE_NAME_SIID, M1Env::PERS_REGNAL_SIID,
        M1Env::PERS_PATRONYMIC_SIID, M1Env::PERS_NICKNAME_SIID, M1Env::PERS_LAST_NAME_SIID, M1Env::PERS_GENERATIONAL_SIID,
        M1Env::PERS_TOPONYMIC_SIID, M1Env::PERS_ALIAS_SIID, M1Env::PERS_TITLE_SIID};
    return ls_fields;
}

/**
 * @brief Label of an alternate-name Person node: @type (birth --> "birth name", ...) + @xml:lang in brackets ("birth name [la]")
 */
QString M1Store::TEIInterface::altPersNameLabel(const PersNameTEI& p_name){
    static const QMap<QString, QString> ls_type_labels{{"birth", "birth name"}, {"regnal", "regnal name"}, {"full", "full name"}};
    QString l_label = ls_type_labels.value(p_name.m_type, p_name.m_type);
    if(p_name.m_lang.length() > 0) l_label = l_label.length() > 0 ? QString("%1 [%2]").arg(l_label).arg(p_name.m_lang) : QString("[%1]").arg(p_name.m_lang);
    return l_label;
}

/**
 * @brief Identity of an alternate name (label + name fields), used to avoid adding the same alternate name twice
 */
QString M1Store::TEIInterface::persNameSignature(const QString& p_label, const QList<std::pair<M1Env::SpecialItemID, QString>>& p_fields){
    QStringList l_parts{p_label};
    for(const auto& l_field : p_fields) l_parts.append(QString("%1=%2").arg(l_field.first).arg(l_field.second));
    return l_parts.join("|");
}

/**
 * @brief Create an alternate-name Person node and link it to the main one (main --EQV_1--> alternate, EQV_2 reciprocal)
 */
void M1Store::TEIInterface::addAltPersName(M1Store::Item_lv2* p_person, const PersNameTEI& p_name){
    M1Store::Item_lv2* l_alt = M1Store::Item_lv2::getNew(M1Env::FULL_VERTEX, altPersNameLabel(p_name).toUtf8().constData());
    l_alt->setType(M1Env::PERS_TYPE_SIID);
    setPersNameFields(l_alt, p_name);
    p_person->linkTo(l_alt, M1Env::EQUIVALENT_1_SIID, InsertionPoint::at_bottom, InsertionPoint::at_bottom);
}

/**
 * @brief Complete an existing Person node with <listPerson> data (e.g. the same <person> in a later chapter)
 *
 * Nothing is overwritten: only empty fields (main name, birth/death dates) are filled and alternate names not yet
 * present (same label and same fields) are added.
 *
 * @param p_person the existing Person node
 * @param p_data <listPerson> data for its key
 */
void M1Store::TEIInterface::mergePerson(M1Store::Item_lv2* p_person, const PersonTEI& p_data){
    int l_added = 0;
    for(const auto& l_field : p_data.m_names.first().m_fields)
        if(p_person->getField(l_field.first).length() == 0){
            p_person->setFieldEdge(l_field.second, l_field.first);
            l_added++;
        }
    for(const auto& l_date : {std::pair<M1Env::SpecialItemID, QString>{M1Env::PERS_BIRTH_DATE_SIID, p_data.m_birth},
                              std::pair<M1Env::SpecialItemID, QString>{M1Env::PERS_DEATH_DATE_SIID, p_data.m_death}})
        if(l_date.second.length() > 0 && p_person->getField(l_date.first).length() == 0){
            p_person->setFieldEdge(l_date.second, l_date.first);
            l_added++;
        }

    // existing alternate names
    QSet<QString> l_existing;
    for(M1Store::Item_lv2_iterator l_it = p_person->getIteratorTop(); !l_it.beyondEnd(); l_it.next())
        if(l_it.at()->isFullEdge() && !l_it.at()->isDead() && l_it.at()->isOfType(M1Env::EQUIVALENT_1_SIID)){
            M1Store::Item_lv2* l_alt = l_it.at()->getTarget_lv2();
            QList<std::pair<M1Env::SpecialItemID, QString>> l_fields;
            for(const M1Env::SpecialItemID l_field : persNameFields()){
                QString l_val = l_alt->getField(l_field);
                if(l_val.length() > 0) l_fields.append({l_field, l_val});
            }
            l_existing.insert(persNameSignature(QString(l_alt->text()), l_fields));
        }
    for(int i = 1; i < p_data.m_names.size(); i++){
        const PersNameTEI& l_name = p_data.m_names.at(i);
        QString l_sig = persNameSignature(altPersNameLabel(l_name), l_name.m_fields);
        if(!l_existing.contains(l_sig)){
            addAltPersName(p_person, l_name);
            l_existing.insert(l_sig);
            l_added++;
        }
    }
    if(l_added > 0)
        qCDebug(g_cat_tei).noquote() << QString("Person [%1] completed: %2 field(s)/alternate name(s) added")
                                            .arg(p_person->getField(M1Env::TEI_NAME_TERM_KEY_SIID)).arg(l_added);
}

/**
 * @brief Get the Person node having this key in its NMTMK field, or create it from the <listPerson> data if there is none
 *
 * Only keys present in the <listPerson> of the current file are considered (nullptr otherwise).
 *
 * Existing Person nodes (from previous imports, see loadPersonCache()) are completed with the <listPerson> data of the
 * current file, without overwriting anything (see mergePerson()), once per import.
 *
 * A new Person node gets the fields of the first <persName>, the birth/death dates and the NMTMK key. Each additional
 * <persName> becomes a separate Person node (no NMTMK key, label = its @type/@xml:lang, e.g. "birth name [la]"),
 * linked to the main one by an EQV_1 edge (EQV_2 reciprocal).
 *
 * @param p_key @xml:id of a <person> in <listPerson>
 * @return the Person node or nullptr if the key is not in <listPerson>
 */
M1Store::Item_lv2* M1Store::TEIInterface::getPersonFromKey(const QString& p_key){
    if(!cm_list_person.contains(p_key)) return nullptr;

    loadPersonCache();
    const PersonTEI& l_data = cm_list_person[p_key];
    M1Store::Item_lv2* l_person = cm_person_by_key.value(p_key, nullptr);
    if(l_person == nullptr){
        // no label: the display text is made from the name fields (see PersonInterp::inTreeDisplayText())
        l_person = M1Store::Item_lv2::getNew(M1Env::FULL_VERTEX, "");
        l_person->setType(M1Env::PERS_TYPE_SIID);
        setPersNameFields(l_person, l_data.m_names.first());
        if(l_data.m_birth.length() > 0) l_person->setFieldEdge(l_data.m_birth, M1Env::PERS_BIRTH_DATE_SIID);
        if(l_data.m_death.length() > 0) l_person->setFieldEdge(l_data.m_death, M1Env::PERS_DEATH_DATE_SIID);
        l_person->setFieldEdge(p_key, M1Env::TEI_NAME_TERM_KEY_SIID);
        cm_person_by_key[p_key] = l_person;

        for(int i = 1; i < l_data.m_names.size(); i++) addAltPersName(l_person, l_data.m_names.at(i));
        qCDebug(g_cat_tei).noquote() << QString("New Person [%1] (%2 alternate name(s))").arg(p_key).arg(l_data.m_names.size() - 1);
    }
    else if(!cm_person_done.contains(p_key))
        mergePerson(l_person, l_data);
    cm_person_done.insert(p_key);
    return l_person;
}

/**
 * @brief Link a <name> or <author> chunk to the corresponding Person node (Person --Owns--> chunk, chunk --Belongs to--> Person)
 *
 * Nothing is done if the key is not the @xml:id of a <person> in the header <listPerson>
 *
 * @param p_chunk the <name> or <author> chunk
 * @param p_key its NMTMK key
 */
void M1Store::TEIInterface::linkChunkToPerson(M1Store::Item_lv2* p_chunk, const QString& p_key){
    M1Store::Item_lv2* l_person = getPersonFromKey(p_key);
    if(l_person != nullptr)
        // occurrences in document order below the Person node
        l_person->linkTo(p_chunk, M1Env::OWNS_SIID, InsertionPoint::at_bottom, InsertionPoint::at_top);
}
