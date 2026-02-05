#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_iterators.h"
#include "m1D_passages_panel.h"

#include <QPainter>
#include <QResizeEvent>
#include <QRegularExpression>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDrag>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>

/** --------------------------------------------------------------- FieldInterp ---------------------------------
 * @brief M1MidPlane::FieldInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::FieldInterp* M1MidPlane::FieldInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a FieldInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::FieldInterp* l_ret = nullptr;
    if(p_myself->isSimpleEdge() || p_myself->isSimpleVertex())
        l_ret = new FieldInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief M1MidPlane::FieldInterp::FieldInterp
 * @param p_myself
 */
M1MidPlane::FieldInterp::FieldInterp(M1Store::Item_lv2* p_myself) : Interp(p_myself){}

QString M1MidPlane::FieldInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("[%1] %2").arg(m_myself->dbgTypeShort()).arg(m_myself->text());
}

QIcon* M1MidPlane::FieldInterp::edgeIcon(const M1Store::Item_lv2* p_edge){
    static QIcon ls_field_icon(M1Env::CROOKED_ICON_PATH);
    return &ls_field_icon;
}
QIcon* M1MidPlane::FieldInterp::vertexIcon(const M1Store::Item_lv2* p_edge){
    static QIcon ls_field_icon(M1Env::FIELD_ICON_PATH);
    return &ls_field_icon;
}

/** --------------------------------------------------------------- TextInterp ---------------------------------
 * @brief M1MidPlane::TextInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TextInterp* M1MidPlane::TextInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TextInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TextInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_SIID))
        l_ret = new TextInterp(p_myself);
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1MidPlane::TextInterp::TextInterp
 * @param p_myself
 */
M1MidPlane::TextInterp::TextInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself) {}

/**
 * @brief M1MidPlane::TextInterp::inTreeDisplayText
 * @return
 */
QString M1MidPlane::TextInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    M1Store::Item_lv2* l_author_edge = m_myself->find_edge_edge_type(M1Env::TEXT_WRITTEN_BY_SIID);
    return QString("%1%2").arg(l_author_edge != nullptr ? QString("[%1] ").arg(l_author_edge->getTarget_lv2()->text()) : "").arg(m_myself->text());
}

/**
 * @brief M1MidPlane::TextInterp::getHtmlVirtual
 * @param p_edge
 * @return
 */
QString M1MidPlane::TextInterp::getHtmlVirtual(){
    initialize();
    QString l_html;
    // title
    l_html += QString("<h2>%1</h2>\n").arg(m_myself->text());

    // author
    M1Store::Item_lv2* l_author_edge = m_myself->find_edge_edge_type(M1Env::TEXT_WRITTEN_BY_SIID);
    if(l_author_edge != nullptr) l_html += QString("<h3>by: %1</h2>\n").arg(l_author_edge->getTarget_lv2()->text());

    // alternate and subtitle
    QString l_alt_title = m_myself->getField(M1Env::TEXT_ALT_TITLE_SIID);
    QString l_sub_title = m_myself->getField(M1Env::TEXT_SUB_TITLE_SIID);
    if(l_alt_title.length() > 0) l_html += QString("<h2>Otherwise known as: %1</h2>\n").arg(l_alt_title);
    if(l_sub_title.length() > 0) l_html += QString("<h3>%1</h3>\n").arg(l_sub_title);

    QStringList l_para_strings;
    for(const auto& l_interp: m_para_list)
        l_para_strings.append(l_interp->baseText());

    return l_html + "<p>" + l_para_strings.join(" ") + "</p>";
}

void M1MidPlane::TextInterp::initialize(){
    if(!m_initialized){
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(); !l_it.beyondEnd(); l_it.next())
            if(!l_it.at()->isFullEdge()) continue;
            else if((l_it.at()->isOfType(M1Env::OWNS_SIID) && l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_PARA_SIID)))
                m_para_list.push_back(getInterp(l_it.at()->getTarget_lv2()));
        m_initialized = true;
    }
}
/** --------------------------------------------------------------- RoleInterp ---------------------------------
 * @brief M1MidPlane::RoleInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::RoleInterp* M1MidPlane::RoleInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a RoleInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::RoleInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::ROLE_FLDR_SIID))
        l_ret = new RoleInterp(p_myself);
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1MidPlane::RoleInterp::RoleInterp
 * @param p_myself
 */
M1MidPlane::RoleInterp::RoleInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    M1Store::Item_lv2* l_author_edge = p_myself->find_edge_generic(M1Env::BLNGS_SIID, M1Env::PERS_TYPE_SIID);
    if(l_author_edge == nullptr)
        l_author_edge = p_myself->find_edge_generic(M1Env::BLNGS_SIID, M1Env::ORG_TYPE_SIID);
    m_author = M1MidPlane::Interp::getInterp(l_author_edge->getTarget_lv2());
}

/**
 * @brief M1MidPlane::RoleInterp::getlUtimateTarget
 * @param p_edge
 * @return
 */
/*
M1Store::Item_lv2* M1MidPlane::RoleInterp::getlUtimateTarget(const M1Store::Item_lv2* p_edge){
    if(p_edge->isOfType(M1Env::TEXT_WRITTEN_BY_SIID) || p_edge->isOfType(M1Env::DATA_SOURCE_FROM_SIID)){
        M1Store::Item_lv2* l_ultimate_target = m_myself->find_edge_generic(M1Store::BLNGS_SIID, M1Store::PERS_TYPE_SIID);
        if(l_ultimate_target == nullptr) l_ultimate_target = m_myself->find_edge_generic(M1Store::BLNGS_SIID, M1Env::ORG_TYPE_SIID);
        return l_ultimate_target->getTarget_lv2();
    }
    else return nullptr;
}*/

/**
 * @brief M1MidPlane::RoleInterp::vertexIcon
 * @param p_edge
 * @return
 */
QIcon* M1MidPlane::RoleInterp::vertexIcon(const M1Store::Item_lv2* p_edge){
    return m_author->vertexIcon(p_edge);
}

/**
 * @brief M1MidPlane::RoleInterp::inTreeDisplayText
 * @param p_edge
 * @return
 */
QString M1MidPlane::RoleInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return m_author->inTreeDisplayText(p_edge) + QString(" (%1)").arg(m_myself->text());
}
// QString M1MidPlane::RoleInterp::getHtmlVirtual(const M1Store::Item_lv2* p_edge){}

/** --------------------------------------------------------------- WfWUnit ---------------------------------
 * @brief M1MidPlane::WfWUnit::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::WfWUnit* M1MidPlane::WfWUnit::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a WfWUnit? %1").arg(p_myself->dbgShort()))
    M1MidPlane::WfWUnit* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_WFW_UNIT_SIID))
        l_ret = new WfWUnit(p_myself);
    M1_FUNC_EXIT
        return l_ret;
}

/**
 * @brief M1MidPlane::WfWUnit::WfWUnit
 * @param p_myself
 */
M1MidPlane::WfWUnit::WfWUnit(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    //m_forms_list
    for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorTop(M1Env::OWNS_SIID, M1Env::OCCUR_SIID); !l_it.beyondEnd(); l_it.next())
        m_forms_list.push_back(M1MidPlane::Interp::getInterp(l_it.at()->getTarget_lv2()->getTarget_lv2()));
}

/**
 * @brief M1MidPlane::WfWUnit::inTreeDisplayText
 * @param p_edge
 * @return
 */
QString M1MidPlane::WfWUnit::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    QString l_translit = m_myself->getField(M1Env::TEXT_WFW_TRANSLIT_SIID);
    QString l_translat = m_myself->getField(M1Env::TEXT_WFW_TRANSLAT_SIID);

    return QString("%1 (%2) : %3").arg(m_myself->text()).arg(l_translit).arg(l_translat);
}

QString M1MidPlane::WfWUnit::getHtmlVirtual(){
    QStringList l_translate_list = m_myself->getField(M1Env::TEXT_WFW_TRANSLAT_SIID).split('|');
    QString l_row_span = l_translate_list.length() >= 2 ? QString(" rowspan=\"%1\"").arg(l_translate_list.length()) : "";
    QString l_ret = QString("<table><tr><td%4 class=\"wfw-top\" style=\"color: maroon;\">%1</td><td%4 class=\"wfw-top\">%2</td><td class=\"wfw-top\">%3</td></tr>\n")
                        .arg(m_myself->text())
                        .arg(m_myself->getField(M1Env::TEXT_WFW_TRANSLIT_SIID))
                        .arg(l_translate_list[0])
                        .arg(l_row_span);

    for(int i=1; i<l_translate_list.length(); i++)
        l_ret += QString("<tr><td class=\"wfw\">%1</td></tr>\n").arg(l_translate_list[i]);
    l_ret += "</table>\n";

    QStringList l_form_html_list;
    for(const std::shared_ptr<Interp>& l_form : m_forms_list)
        l_form_html_list.append(l_form->getHtmlVirtual());

    l_ret += QString("<div class=\"grammar\">\n<p><span style=\"color: maroon;\">%1</span> (%2)</p>\n%3</div>\n")
                 .arg(m_myself->text())
                 .arg(m_myself->getField(M1Env::TEXT_WFW_TRANSLIT_SIID))
                 .arg(l_form_html_list.join("\n"));

    return l_ret;
}

/** --------------------------------------------------------------- UrlInterp ---------------------------------
 * @brief M1MidPlane::UrlInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::UrlInterp* M1MidPlane::UrlInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a WfWUnit? %1").arg(p_myself->dbgShort()))
    M1MidPlane::UrlInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_URL_LINK_SIID))
        l_ret = new UrlInterp(p_myself);
    M1_FUNC_EXIT
        return l_ret;
}

M1MidPlane::UrlInterp::UrlInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

QString M1MidPlane::UrlInterp::getHtmlVirtual(){
    return m_myself->text();
}

/** --------------------------------------------------------------- OccurInterp ---------------------------------
 * @brief M1MidPlane::OccurInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::OccurInterp* M1MidPlane::OccurInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an OccurInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::OccurInterp* l_ret = nullptr;
    if(p_myself->isFullEdge() && p_myself->isOfType(M1Env::OCCUR_SIID))
        l_ret = new OccurInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

// class Toto{
// public:
//     int a;
//     Toto(){a=354;}

//     QString the_value(){return QString("a: %1").arg(a);}
// };

// class Tuto : public Toto{};

M1MidPlane::OccurInterp::OccurInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    // Toto l_toto;
    // Tuto l_tuto;

    m_target = Interp::getInterp(p_myself->getTarget_lv2());
    m_is_capitalized = p_myself->getField(M1Env::CAPTL_SIID) == "true";
    // M1Env::EnvStatic::setTmpDebug(true);
    m_capitalized_fld = p_myself->getField(M1Env::CAPTL_SIID);
    // M1Env::EnvStatic::setTmpDebug(false);
    m_punct_left = p_myself->getField(M1Env::PCTLF_SIID);
    m_punct_right = p_myself->getField(M1Env::PCTRT_SIID);
}
QString M1MidPlane::OccurInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    QString l_text = m_target->inTreeDisplayText(m_myself);
    if(m_is_capitalized) l_text = l_text[0].toUpper() + l_text.right(l_text.length() - 1);
    return QString("Occurrence of: %1").arg(l_text);
}

M1Store::Item_lv2* M1UI::TreeRow::where_to_go(){
    return m_target->where_to_go(m_edge);
}
M1Store::Item_lv2* M1MidPlane::Interp::where_to_go(const M1Store::Item_lv2* p_edge){
    return m_myself;
}
M1Store::Item_lv2* M1MidPlane::OccurInterp::where_to_go(const M1Store::Item_lv2* p_edge){
    return m_target->where_to_go(m_myself);
}

QString M1MidPlane::OccurInterp::getHtmlVirtual(){
    /*
    QString l_members_html = QString("<p class=\"technical\">m_punct_left: %1</p>\n").arg(m_punct_left) +
                             QString("<p class=\"technical\">m_myself->getField(M1Env::PCTLF_SIID): %1</p>\n").arg(m_myself->getField(M1Env::PCTLF_SIID)) +
                             QString("<p class=\"technical\">m_myself->getTarget_lv2()->getField(M1Env::PCTLF_SIID): %1</p>\n").arg(m_myself->getTarget_lv2()->getField(M1Env::PCTLF_SIID)) +
                             QString("<p class=\"technical\">m_punct_right: %1</p>\n").arg(m_punct_right) +
                             QString("<p class=\"technical\">m_myself->text(): %1</p>\n").arg(m_myself->text());
    return QString("%1<p>Occurrence of:</p>%2\n").arg(l_members_html).arg(m_target->getHtmlVirtual());
    */
    return QString("<p>Occurrence of:</p>%1\n").arg(m_target->getHtmlVirtual());
}
QString M1MidPlane::OccurInterp::baseText(){
    QString l_text = m_target->baseText();
    if(m_is_capitalized) l_text = l_text[0].toUpper() + l_text.right(l_text.length() - 1);
    // return m_punct_left + QString("ID: %1 IC: %2 %3 OI: ").arg(m_myself->text()).arg(m_is_capitalized ? "True" : "False").arg(m_capitalized_fld) + l_text + m_punct_right;
    return m_punct_left + l_text + m_punct_right;
}
QString M1MidPlane::OccurInterp::baseTextPlus(){
    QString l_section = m_myself->getField(M1Env::STEPHANUS_SIID);
    if(l_section.length() > 0)
        return QString("<span style=\"color: maroon;\">%1</span> %2").arg(l_section).arg(baseText());
    else
        return baseText();
}
/** --------------------------------------------------------------- LemmaInterp ---------------------------------
 * @brief M1MidPlane::LemmaInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::LemmaInterp* M1MidPlane::LemmaInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an FormInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::LemmaInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::LEMMA_SIID))
        l_ret = new LemmaInterp(p_myself);
    M1_FUNC_EXIT
        return l_ret;
}

M1MidPlane::LemmaInterp::LemmaInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    M1Store::Item_lv2* l_pos_edge = p_myself->find_edge_generic(M1Env::ISA_SIID, M1Env::NLPOS_SIID, true);
    if(l_pos_edge != nullptr)
        m_pos = l_pos_edge->getTarget_lv2()->text();
    else
        m_pos = m_myself->getField(M1Env::TEXT_WFW_POS_SIID);
}

QString M1MidPlane::LemmaInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("%1 [%2]").arg(m_myself->text()).arg(m_pos);
}

/** --------------------------------------------------------------- FormInterp ---------------------------------
 * @brief M1MidPlane::FormInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::FormInterp* M1MidPlane::FormInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an FormInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::FormInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_WFW_FORM_SIID))
        l_ret = new FormInterp(p_myself);
    M1_FUNC_EXIT
        return l_ret;
}

M1MidPlane::FormInterp::FormInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    M1Store::Item_lv2* l_lemma_edge = p_myself->find_edge_generic(M1Env::BLNGS_SIID, M1Env::LEMMA_SIID);
    if(l_lemma_edge != nullptr)
        m_lemma = Interp::getInterp(l_lemma_edge->getTarget_lv2());
}

QString M1MidPlane::FormInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("%1 ← %2").arg(m_myself->text()).arg(m_lemma != nullptr ? m_lemma->inTreeDisplayText(p_edge) : "<No Lemma>");
}

QString M1MidPlane::FormInterp::getHtmlVirtual(){
    // GRAMMAR_ATTR_VAL_SIID
    QStringList l_grammar_values;
    for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorSpecial(M1Env::ISA_SIID, M1Env::GRAMMAR_ATTR_VAL_SIID); !l_it.beyondEnd(); l_it.next())
        l_grammar_values.append(l_it.at()->getTarget_lv2()->text());

    return QString("<p class=\"grammar\">%1: %2</p>").arg(this->inTreeDisplayText(nullptr)).arg(l_grammar_values.join(", "));
}

QString M1MidPlane::FormInterp::baseText(){
    return m_myself->text();
}
/** --------------------------------------------------------------- TranslationBhashya ---------------------------------
 * @brief M1MidPlane::TranslationBhashya::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TranslationBhashya* M1MidPlane::TranslationBhashya::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an TranslationBhashya? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TranslationBhashya* l_ret = nullptr;
    if(p_myself->isFullVertex() && (p_myself->isOfType(M1Env::TEXT_SLOKA_BHASHYA_SIID) || p_myself->isOfType(M1Env::TEXT_SLOKA_TRANSLATION_SIID)))
        l_ret = new TranslationBhashya(p_myself);
    M1_FUNC_EXIT
        return l_ret;
}

M1MidPlane::TranslationBhashya::TranslationBhashya(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    m_language = m_myself->getField(M1Env::TEXT_LANGUAGE_SIID);

    int l_count = 0;
    for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorTop(M1Env::TEXT_WRITTEN_BY_SIID, M1Env::ROLE_FLDR_SIID); !l_it.beyondEnd(); l_it.next()){
        if(l_count == 0) m_author_1 = M1MidPlane::Interp::getInterp(l_it.at()->getTarget_lv2());
        else m_author_2 = M1MidPlane::Interp::getInterp(l_it.at()->getTarget_lv2());
        l_count += 1;
    }
}

QString M1MidPlane::TranslationBhashya::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    QString l_text_cleanup = QString("%1%2 [%3] %4")
    .arg(m_author_1->inTreeDisplayText(p_edge))
        .arg(m_author_2 != nullptr ? QString(" + %1").arg(m_author_2->inTreeDisplayText(p_edge)) : "")
        .arg(m_language)
        .arg(m_myself->text()).replace(g_re_tags, "").replace(g_re_space, " ").trimmed();
    if(l_text_cleanup.length() > 200)
        return l_text_cleanup.slice(0, 200) + " ...";
    else
        return l_text_cleanup;
}

QString M1MidPlane::TranslationBhashya::getHtmlVirtual(){
    QString l_text = m_myself->text();
    QString l_author = QString("%1%2")
                           .arg(m_author_1->inTreeDisplayText(nullptr))
                           .arg(m_author_2 != nullptr ? QString(" + %1").arg(m_author_2->inTreeDisplayText(nullptr)) : "");

    if(g_re_initial_p.match(l_text).hasMatch())
        l_text = l_text.replace(g_re_final_p, "").replace(g_re_initial_p, "");

    return QString("<p>%1 <span style=\"font-style: italic; font-size: smaller; color: maroon;\">%2 Lang.: %3</span></p>\n")
        .arg(l_text).arg(l_author).arg(m_language);
}

/** --------------------------------------------------------------- SlokaInterp ---------------------------------
 * @brief M1MidPlane::SlokaInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::SlokaInterp* M1MidPlane::SlokaInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an TranslationBhashya? %1").arg(p_myself->dbgShort()))
    M1MidPlane::SlokaInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::SLOKA_SIID))
        l_ret = new SlokaInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief M1MidPlane::SlokaInterp::SlokaInterp
 * @param p_myself
 */
M1MidPlane::SlokaInterp::SlokaInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    // Get chapter and Sloka numbers
    m_sloka_num = m_myself->getField(M1Env::TEXT_SLOKA_NUMBER_SIID).toInt();
    M1Store::Item_lv2* l_chapter_edge = m_myself->find_edge_generic(M1Env::BLNGS_SIID, M1Env::TEXT_CHAPTER_SIID);
    if(l_chapter_edge != nullptr)
        m_chapter_num = l_chapter_edge->getTarget_lv2()->getField(M1Env::TEXT_CHAP_NUMBER_SIID).toInt();
    else
        m_chapter_num = -1;

    // get Sanskrit text
    M1Store::Item_lv2* l_sk_edge = m_myself->find_edge_generic(M1Env::OWNS_SIID, M1Env::TEXT_SLOKA_LINE_SIID);
    if(l_sk_edge != nullptr)
        m_sk = l_sk_edge->getTarget_lv2()->text();

    // get IAST translation text
    M1Store::Item_lv2* l_transl_edge = m_myself->find_edge_generic(M1Env::OWNS_SIID, M1Env::TEXT_SLOKA_TRANSLIT_SIID);
    if(l_transl_edge != nullptr)
        m_iast = l_transl_edge->getTarget_lv2()->text();

    // just to be sure
    m_initialized = false;
}

/**
 * @brief M1MidPlane::SlokaInterp::initialize
 *
 * Here the initialization only deals with vectors (WfW, Translations and Bhashyas)
 *
 * Single-value attributes are initialized in the constructor
 */
void M1MidPlane::SlokaInterp::initialize(){
    if(! m_initialized){
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorTop(M1Env::OWNS_SIID, M1Env::TEXT_SLOKA_TRANSLATION_SIID); !l_it.beyondEnd(); l_it.next())
            m_translations_list.push_back(getInterp(l_it.at()->getTarget_lv2()));

        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorTop(M1Env::OWNS_SIID, M1Env::TEXT_SLOKA_BHASHYA_SIID); !l_it.beyondEnd(); l_it.next())
            m_bhashya_list.push_back(getInterp(l_it.at()->getTarget_lv2())); // m_wfw_list

        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorTop(M1Env::OWNS_SIID, M1Env::TEXT_WFW_UNIT_SIID); !l_it.beyondEnd(); l_it.next())
            m_wfw_list.push_back(getInterp(l_it.at()->getTarget_lv2()));
    }
    m_initialized = true;
}

/**
 * @brief M1MidPlane::SlokaInterp::inTreeDisplayText
 * @param p_edge
 *
 * Here, initialization is not necessary because only single-value attributes are used
 *
 * @return
 */
QString M1MidPlane::SlokaInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("[%1.%2] %3 (%4)").arg(m_chapter_num).arg(m_sloka_num).arg(m_sk).arg(m_iast);
}

/**
 * @brief M1MidPlane::SlokaInterp::getHtmlVirtual
 *
 * Here, initialization is necessary because all attributes are used, including the vectors
 *
 * @return
 */
QString M1MidPlane::SlokaInterp::getHtmlVirtual(){
    initialize();

    QStringList l_wfw_html_list;
    QStringList l_grammar_html_list;
    for(const std::shared_ptr<Interp>& w: m_wfw_list){
        QString l_html_raw = w->getHtmlVirtual();

        qCDebug(g_cat_interp_base).noquote() << "l_html_raw:\n" << l_html_raw;
        QRegularExpressionMatch l_match = g_re_capture_gammar.match(l_html_raw, QRegularExpression::DotMatchesEverythingOption);
        if(l_match.hasMatch()){
            qCDebug(g_cat_interp_base).noquote() << "l_match.captured(0/1)" << l_match.captured(0) << l_match.captured(1);
            l_grammar_html_list.append(l_match.captured(1));
        }
        else
            qCDebug(g_cat_interp_base).noquote() << "Not Matched";

        QString l_wfw_html = l_html_raw.replace(g_re_capture_gammar, "").replace(g_re_initial_table, "").replace(g_re_final_table, "");
        l_wfw_html_list.append(l_wfw_html);
    }

    QStringList l_translation_html_list;
    for(const std::shared_ptr<Interp>& t: m_translations_list) l_translation_html_list.append(t->getHtmlVirtual());

    QStringList l_bhashya_html_list;
    for(const std::shared_ptr<Interp>& b: m_bhashya_list) l_bhashya_html_list.append(b->getHtmlVirtual());

    return (QString("<h5>Sloka %1.%2</h5>\n<p>%3</p>\n<p>%4 (IAST)</p>\n<h4>Word-for-Word Analysis</h4><table>%5</table>") +
            QString("\n<h4>Translations</h4><div>%6</div>\n<h4>Bhashyas</h4><div>%7<h4>Grammar</h4><div>%8</div>\n"))
        .arg(m_chapter_num)                         // 1
        .arg(m_sloka_num)                           // 2
        .arg(m_sk)                                  // 3
        .arg(m_iast)                                // 4
        .arg(l_wfw_html_list.join("\n"))            // 5
        .arg(l_translation_html_list.join("\n"))    // 6
        .arg(l_bhashya_html_list.join("\n"))        // 7
        .arg(l_grammar_html_list.join("\n"));       // 8
}

/** --------------------------------------------------------------- SentenceInterp ---------------------------------
 * @brief M1MidPlane::SentenceInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::SentenceInterp* M1MidPlane::SentenceInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an TranslationBhashya? %1").arg(p_myself->dbgShort()))
    M1MidPlane::SentenceInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_SENTENCE_SIID))
        l_ret = new SentenceInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

//     int m_book_num;
// int m_sentence_num;

// QString m_text;
// QString m_word_begin;
// QString m_word_end;

M1MidPlane::SentenceInterp::SentenceInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    M1Store::Item_lv2* l_begin_edge = m_myself->find_edge_generic(M1Env::TW_CHUNK_2_OCC_BEGIN_SIID, M1Env::OCCUR_SIID);
    if(l_begin_edge)
        m_word_begin_occ = getInterp(l_begin_edge->getTarget_lv2());

    M1Store::Item_lv2* l_end_edge = m_myself->find_edge_generic(M1Env::TW_CHUNK_2_OCC_END_SIID, M1Env::OCCUR_SIID);
    if(l_end_edge)
        m_word_end_occ = getInterp(l_end_edge->getTarget_lv2());

    if(g_re_sent_num.match(m_myself->text()).hasMatch())
        m_sentence_num = g_re_sent_num.match(m_myself->text()).captured(0).toUInt();
}

/**
 * @brief M1MidPlane::SentenceInterp::initialize
 */
void M1MidPlane::SentenceInterp::initialize(){
    if(m_word_begin_occ != nullptr && !m_initialized){
        m_occ_list.push_back(m_word_begin_occ);
        M1Store::Item_lv2* l_cur_occ_edge = m_word_begin_occ->myself();
        qCDebug(g_cat_tmp_spotlight).noquote() << "Strart" << l_cur_occ_edge->dbgShort();
        do{
            l_cur_occ_edge = l_cur_occ_edge->get_next_lv2();
            m_occ_list.push_back(getInterp(l_cur_occ_edge));
            qCDebug(g_cat_tmp_spotlight).noquote() << "Cur" << l_cur_occ_edge->dbgShort();
            qCDebug(g_cat_tmp_spotlight).noquote() << "End" << m_word_end_occ->myself()->dbgShort();

        }while(l_cur_occ_edge != m_word_end_occ->myself());
        // m_occ_list.push_back(m_word_end_occ);
        m_initialized = true;
    }
}

QString M1MidPlane::SentenceInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("S%1: %2 ... %3").arg(m_sentence_num).arg(m_word_begin_occ->baseText()).arg(m_word_end_occ->baseText());
}
QString M1MidPlane::SentenceInterp::getHtmlVirtual(){
    // return QString(QString("m_word_begin: %1").arg(m_word_begin_occ->baseText()));
    initialize();

    QStringList l_word_list;
    for(const auto& l_occ_interp : m_occ_list) l_word_list.append(l_occ_interp->baseTextPlus());
    return QString("<p>%1</p>\n").arg(l_word_list.join(" "));
}

/** --------------------------------------------------------------- SectionInterp ---------------------------------
 * @brief M1MidPlane::SectionInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::SectionInterp* M1MidPlane::SectionInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an TranslationBhashya? %1").arg(p_myself->dbgShort()))
    M1MidPlane::SectionInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::STEPHANUS_SIID))
        l_ret = new SectionInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::SectionInterp::SectionInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}
QString M1MidPlane::SectionInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return m_myself->text();
}

void M1MidPlane::SectionInterp::initialize(){
    qCDebug(g_cat_tmp_spotlight).noquote() << "initialize()" << m_myself->dbgShort();
    if(!m_initialized){
        // perform same task for all versions
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(M1Env::OWNS_SIID, M1Env::OCCUR_SIID); !l_it.beyondEnd(); l_it.next() ){
            // qCDebug(g_cat_tmp_spotlight).noquote() << "l_it" << l_it.at()->dbgShort();
            M1Store::Item_lv2* l_current_edge = l_it.at()->getTarget_lv2();

            qCDebug(g_cat_tmp_spotlight) << "START l_current_edge:" << l_current_edge->dbgShort();
            // position l_current_edge on previous sentence start
            // M1Store::Item_lv2* l_start_sentence_occ = nullptr;
            while(true){
                qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
                qCDebug(g_cat_interp_base) << QString("   previous") << l_current_edge->get_previous_lv2()->dbgShort();
                qCDebug(g_cat_interp_base) << QString("   next    ") << l_current_edge->get_next_lv2()->dbgShort();
                if(l_current_edge->getField(M1Env::STPOS_SIID) == "SS" || l_current_edge->getField(M1Env::STPOS_SIID) == "SX"){
                    // l_start_sentence_occ = l_current_edge;
                    break;
                }
                if(l_current_edge->previous_item_id() == M1Store::G_VOID_ITEM_ID) break;
                l_current_edge = l_current_edge->get_previous_lv2();
            }
            // version ?
            M1Store::Item_lv2* l_edge_2_sentence = l_current_edge->find_edge_generic(M1Env::TW_OCC_2_CHUNK_BEGIN_SIID, M1Env::TEXT_SENTENCE_SIID);
            M1Store::Item_lv2* l_edge_2_book = l_edge_2_sentence->getTarget_lv2()->find_edge_generic(M1Env::BLNGS_SIID, M1Env::TEXT_BOOK_SIID);
            M1Store::Item_lv2* l_edge_2_version = l_edge_2_book->getTarget_lv2()->find_edge_generic(M1Env::BLNGS_SIID, M1Env::TXTVR_SIID);
            QString l_version = l_edge_2_version->getTarget_lv2()->text();

            qCDebug(g_cat_tmp_spotlight) << QString("[%1] l_current_edge:").arg(l_version) << l_current_edge->dbgShort();

            // find end of section
            int l_sections_encountered = 0;
            while(l_sections_encountered < 2){ // 2 sections must be encountered (this one plus the next)
                if(l_current_edge->getField(M1Env::STEPHANUS_SIID).length() > 0) l_sections_encountered += 1;
                if(l_sections_encountered < 2){ // section end excluded
                    std::shared_ptr<Interp> l_occ_interp = getInterp(l_current_edge);
                    m_occ_map[l_version].push_back(l_occ_interp);
                    qCDebug(g_cat_tmp_spotlight) << "Add occ" << m_occ_map[l_version].size() << l_occ_interp->baseText();
                    l_current_edge = l_current_edge->get_next_lv2();
                }
            }

            // find end of sentence
            while(true){
                qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
                qCDebug(g_cat_interp_base) << QString("   previous") << l_current_edge->get_previous_lv2()->dbgShort();
                qCDebug(g_cat_interp_base) << QString("   next    ") << l_current_edge->get_next_lv2()->dbgShort();
                if(l_current_edge->getField(M1Env::STPOS_SIID) == "SS" || l_current_edge->getField(M1Env::STPOS_SIID) == "SX"){
                    // l_start_sentence_occ = l_current_edge;
                    break;
                }
                std::shared_ptr<Interp> l_occ_interp = getInterp(l_current_edge);
                m_occ_map[l_version].push_back(l_occ_interp);
                if(l_current_edge->next_item_id() == M1Store::G_VOID_ITEM_ID) break;
                l_current_edge = l_current_edge->get_next_lv2();
            }
        }

        /*
        // find previous sentence end
        M1Store::Item_lv2* l_current_edge = m_myself->find_edge_generic(M1Env::OWNS_SIID, M1Env::OCCUR_SIID)->getTarget_lv2();
        qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
        M1Store::Item_lv2* l_section;
        while(true){
            qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
            qCDebug(g_cat_interp_base) << QString("   previous") << l_current_edge->get_previous_lv2()->dbgShort();
            qCDebug(g_cat_interp_base) << QString("   next    ") << l_current_edge->get_next_lv2()->dbgShort();
            l_section = l_current_edge->find_edge_generic(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID);
            if(l_section != nullptr){
                l_section = l_section->getTarget_lv2();
                break;
            }
            if(l_current_edge->previous_item_id() == M1Store::G_VOID_ITEM_ID) break;
            l_current_edge = l_current_edge->get_previous_lv2();
            // l_current_edge = l_current_edge->getNext_lv2();
        }
        */
        m_initialized = true;
    }
}

QString M1MidPlane::SectionInterp::getHtmlVirtual(){
    initialize();

    QMap<QString, QStringList> l_strings_map;

    for(const auto& l_item : m_occ_map){
        QString l_version = l_item.first;
        std::vector<std::shared_ptr<Interp>> l_interp_list = l_item.second;

        for(const auto& l_occ_interp : l_interp_list)
            l_strings_map[l_version].append(l_occ_interp->baseTextPlus());
    }

    QString l_html;
    for(const QString& l_key : l_strings_map.keys())
        l_html += QString("<p><b>%1</b>: %2</p>\n").arg(l_key).arg(l_strings_map[l_key].join(" "));

    return l_html;
}

QWidget *M1MidPlane::SectionInterp::get_edit_widget(){
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("SectionInterp get_edit_widget()") << m_myself->dbgShort();
    initialize();
    /*
    // find previous Stephanus Number
    M1Store::Item_lv2* l_current_edge = m_myself->find_edge_generic(M1Env::OWNS_SIID, M1Env::OCCUR_SIID)->getTarget_lv2();
    qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
    M1Store::Item_lv2* l_section;
    while(true){
        qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
        qCDebug(g_cat_interp_base) << QString("   previous") << l_current_edge->get_previous_lv2()->dbgShort();
        qCDebug(g_cat_interp_base) << QString("   next    ") << l_current_edge->get_next_lv2()->dbgShort();
        l_section = l_current_edge->find_edge_generic(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID);
        if(l_section != nullptr){
            l_section = l_section->getTarget_lv2();
            break;
        }
        if(l_current_edge->previous_item_id() == M1Store::G_VOID_ITEM_ID) break;
        l_current_edge = l_current_edge->get_previous_lv2();
        // l_current_edge = l_current_edge->getNext_lv2();
    }

    qCDebug(g_cat_interp_base) << QString("Previous section") << l_section->dbgShort();
    M1Store::Item_lv2* l_greek_start = nullptr;
    M1Store::Item_lv2* l_jowett_start = nullptr;
    M1Store::Item_lv2* l_shorey_start = nullptr;
    for(M1Store::Item_lv2_iterator it = l_section->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID)){
            qCDebug(g_cat_interp_base) << QString("section occur") << it.at()->getTarget_lv2()->dbgShort();
            if(it.at()->getTarget_lv2()->isOfType("RVGRK")) l_greek_start = it.at()->getTarget_lv2();
            if(it.at()->getTarget_lv2()->isOfType("RVJWT")) l_jowett_start = it.at()->getTarget_lv2();
            if(it.at()->getTarget_lv2()->isOfType("RVSHR")) l_shorey_start = it.at()->getTarget_lv2();
        }
    qCDebug(g_cat_interp_base) << QString("greek  start") << l_greek_start->dbgShort();
    qCDebug(g_cat_interp_base) << QString("jowett start") << l_jowett_start->dbgShort();
    qCDebug(g_cat_interp_base) << QString("shorey start") << l_shorey_start->dbgShort();
    */

    // Set up UI
    QWidget* l_panel_widget = new QWidget();
    QVBoxLayout* l_panel_layout = new QVBoxLayout();
    l_panel_widget->setLayout(l_panel_layout);

    QWidget* l_button_bar = new QWidget(l_panel_widget);
    l_panel_layout->addWidget(l_button_bar);
    QHBoxLayout* l_bar_layout = new QHBoxLayout();
    l_button_bar->setLayout(l_bar_layout);

    // QPushButton* l_btn0 = new QPushButton("< 10", l_button_bar);
    // QPushButton* l_btn1 = new QPushButton("< 1", l_button_bar);
    // QPushButton* l_btn2 = new QPushButton("> 1", l_button_bar);
    // QPushButton* l_btn3 = new QPushButton("> 10", l_button_bar);
    QPushButton* l_btn4 = new QPushButton("Highlight", l_button_bar);
    l_btn4->setEnabled(false);

    QComboBox* l_cat_combo = new QComboBox(l_button_bar);
    // fill categories combo box
    M1Store::Item_lv2* l_republic = m_occ_map["Greek"][0]->myself()->getOrigin_lv2();
    qCDebug(g_cat_tmp_spotlight) << QString("l_republic") << l_republic->dbgShort();
    M1Store::Item_lv2* l_cat_folder = l_republic->find_edge_generic(M1Env::OWNS_SIID, M1Env::TEXT_HIGHLIGHT_CAT_FLDR_SIID)->getTarget_lv2();
    qCDebug(g_cat_tmp_spotlight) << QString("l_cat_folder") << l_cat_folder->dbgShort();

    // get list of categories
    QList<M1Store::Item_lv2*> l_cat_list;
    for(M1Store::Item_lv2_iterator it = l_cat_folder->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_CAT_SIID)){
            M1Store::Item_lv2* l_cat = it.at()->getTarget_lv2();
            l_cat_list.append(l_cat);
            QString l_color = l_cat->getField(M1Env::HLCLR_SIID);
            qCDebug(g_cat_interp_base) << QString("Cat: ") << it.at()->dbgShort();
            qCDebug(g_cat_interp_base) << QString("l_color: ") << l_color;
            QPixmap l_color_pixmap(16, 16);
            l_color_pixmap.fill(QColor(l_color));
            l_cat_combo->addItem(QIcon(l_color_pixmap), l_cat->text());
        }

    // l_bar_layout->addWidget(l_btn0);
    // l_bar_layout->addWidget(l_btn1);
    // l_bar_layout->addWidget(l_btn2);
    // l_bar_layout->addWidget(l_btn3);
    l_bar_layout->addWidget(l_btn4);
    l_bar_layout->addWidget(l_cat_combo);
    l_bar_layout->addStretch(1);

    M1UI::Scene* l_scene = new M1UI::Scene();
    l_scene->setBackgroundBrush(Qt::white);

    M1UI::PassagesPanel* l_passages_panel = new M1UI::PassagesPanel(
        l_republic->find_edge_generic(M1Env::OWNS_SIID, M1Env::TEXT_HIGHLIGHT_FLDR_SIID)->getTarget_lv2(),
        l_cat_list
        );
    l_scene->addItem(l_passages_panel);

    l_passages_panel->add_passage_editor(new M1UI::PassageEditor("Shorey", m_occ_map["Shorey"], "A", l_passages_panel));
    l_passages_panel->add_passage_editor(new M1UI::PassageEditor("Jowett", m_occ_map["Jowett"], "B", l_passages_panel));
    l_passages_panel->add_passage_editor(new M1UI::PassageEditor("Greek", m_occ_map["Greek"], "C", l_passages_panel));

    // QObject::connect(l_btn0, &QPushButton::clicked,
    //                  l_passages_panel, &M1UI::PassagesPanel::move_backwards_ten);
    // QObject::connect(l_btn1, &QPushButton::clicked,
    //                  l_passages_panel, &M1UI::PassagesPanel::move_backwards_one);
    // QObject::connect(l_btn2, &QPushButton::clicked,
    //                  l_passages_panel, &M1UI::PassagesPanel::move_forward_one);
    // QObject::connect(l_btn3, &QPushButton::clicked,
    //                  l_passages_panel, &M1UI::PassagesPanel::move_forward_ten);
    QObject::connect(l_btn4, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::highlight);

    QObject::connect(l_cat_combo, &QComboBox::activated,
                     l_passages_panel, &M1UI::PassagesPanel::cat_select);

    // activate_highlight_button
    QObject::connect(l_passages_panel, &M1UI::PassagesPanel::activate_highlight_button,
                     l_btn4, &QPushButton::setEnabled);

    M1UI::View* l_view = new M1UI::View(l_scene);
    l_view->set_panel(l_passages_panel);
    l_panel_layout->addWidget(l_view);

    return l_panel_widget;
}

/** --------------------------------------------------------------- ChunkInterp ---------------------------------
 * @brief M1MidPlane::ChunkInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::ChunkInterp* M1MidPlane::ChunkInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an TranslationBhashya? %1").arg(p_myself->dbgShort()))
    M1MidPlane::ChunkInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_HIGHLIGHT_CHUNK_SIID))
        l_ret = new ChunkInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::ChunkInterp::ChunkInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

void M1MidPlane::ChunkInterp::initialize(){
    if(! m_initialized){
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(M1Env::OWNS_SIID, M1Env::OCCUR_SIID); !l_it.beyondEnd(); l_it.next() )
            m_occ_list.push_back(getInterp(l_it.at()->getTarget_lv2()));
        m_initialized = true;
    }
}
QString M1MidPlane::ChunkInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    initialize();
    QStringList l_word_list;
    for(const auto& l_occ_interp : m_occ_list) l_word_list.append(l_occ_interp->baseText());
    return QString("Passage in [%1]: %2").arg(m_myself->text()).arg(M1Store::StorageStatic::maxLengthChop(l_word_list.join(" "), 100));
}
QString M1MidPlane::ChunkInterp::getHtmlVirtual(){
    initialize();
    QStringList l_word_list;
    for(const auto& l_occ_interp : m_occ_list) l_word_list.append(l_occ_interp->baseTextPlus());
    return QString("%1").arg(l_word_list.join(" "));
}

/** --------------------------------------------------------------- HighlightInterp ---------------------------------
 * @brief M1MidPlane::HighlightInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::HighlightInterp* M1MidPlane::HighlightInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an HighlightInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::HighlightInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_HIGHLIGHT_SIID))
        l_ret = new HighlightInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::HighlightInterp::HighlightInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    M1Store::Item_lv2* l_cat_edge = m_myself->find_edge_generic(M1Env::ISA_SIID, M1Env::TEXT_HIGHLIGHT_CAT_SIID, true);
    if(l_cat_edge != nullptr)
        m_category = getInterp(l_cat_edge->getTarget_lv2());
}
QString M1MidPlane::HighlightInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return m_myself->text();
}
void M1MidPlane::HighlightInterp::initialize(){
    if(! m_initialized){
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(M1Env::OWNS_SIID, M1Env::TEXT_HIGHLIGHT_CHUNK_SIID); !l_it.beyondEnd(); l_it.next() )
            m_chunk_list.push_back(getInterp(l_it.at()->getTarget_lv2()));
        m_initialized = true;
    }
}
QString M1MidPlane::HighlightInterp::getHtmlVirtual(){
    initialize();
    QStringList l_word_list;
    for(const auto& l_chunk_interp : m_chunk_list)
        l_word_list.append(QString("<p><b>%1</b>: %2</p>\n")
                               .arg(l_chunk_interp->myself()->text())
                               .arg(l_chunk_interp->getHtmlVirtual()));
    return QString("<p>%1 highlight:</p>\n%2").arg(m_category->getHtmlVirtual()).arg(l_word_list.join("\n"));
}
QString M1MidPlane::HighlightInterp::baseText(){
    initialize();
    QStringList l_word_list;
    for(const auto& l_chunk_interp : m_chunk_list)
        l_word_list.append(QString("[%1]%2")
                               .arg(l_chunk_interp->myself()->text())
                               .arg(l_chunk_interp->getHtmlVirtual()));
    return QString("%1").arg(l_word_list.join("✹"));
}
/** --------------------------------------------------------------- HighlightCategory ---------------------------------
 * @brief M1MidPlane::HighlightCategory::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::HighlightCategory* M1MidPlane::HighlightCategory::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an HighlightCategory? %1").arg(p_myself->dbgShort()))
    M1MidPlane::HighlightCategory* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_HIGHLIGHT_CAT_SIID))
        l_ret = new HighlightCategory(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::HighlightCategory::HighlightCategory(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){
    if(M1Store::Item_lv2* l_color_edge = m_myself->getFieldEdge(M1Store::HLCLR_SIID))
        m_color = l_color_edge->getTarget_lv2()->text();
}

QString M1MidPlane::HighlightCategory::getHtmlVirtual(){
    return QString("<span style=\"background-color: %1\">%2</span>").arg(m_color).arg(m_myself->text());
}

/** --------------------------------------------------------------- ParagraphInterp ---------------------------------
 * @brief M1MidPlane::ParagraphInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::ParagraphInterp* M1MidPlane::ParagraphInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a ParagraphInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::ParagraphInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_PARA_SIID))
        l_ret = new ParagraphInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::ParagraphInterp::ParagraphInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}
QString M1MidPlane::ParagraphInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return m_myself->text();
}

void M1MidPlane::ParagraphInterp::initialize(){
    if(!m_initialized){
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(); !l_it.beyondEnd(); l_it.next())
            if(!l_it.at()->isFullEdge()) continue;
            else if((l_it.at()->isOfType(M1Env::OWNS_SIID) && l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_CHUNK_SIID)) ||
                     (l_it.at()->isOfType(M1Env::TEXT_HIGHLIGHT_QUOTE_SIID) && l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_SIID)))
                m_chunk_list.push_back(getInterp(l_it.at()->getTarget_lv2()));
        m_initialized = true;
    }
}

QString M1MidPlane::ParagraphInterp::getHtmlVirtual(){
    return this->baseText();
}

QString M1MidPlane::ParagraphInterp::baseText(){
    initialize();
    QStringList l_chunk_strings;
    for(const auto& l_interp: m_chunk_list){
        QString l_base_text = l_interp->baseText();
        if(l_base_text.contains("✹")){
            QStringList l_ver_chunks;
            QString l_ground_text;
            for(const QString& l_version_text : l_base_text.split("✹")){
                QString l_bt_bare = l_version_text;
                l_bt_bare.replace(g_re_html_tags, "");
                QString l_version;
                QString l_text;
                if(g_re_extract_vt.match(l_bt_bare).hasMatch()){
                    l_version = g_re_extract_vt.match(l_bt_bare).captured(1);
                    l_text = g_re_extract_vt.match(l_bt_bare).captured(2);
                }
                if(l_version == "Greek")
                    l_ground_text = QString("<span style=\"font-style: italic;\">%1</span>").arg(l_text);
                else
                    l_ver_chunks.append(QString("[%1] %2").arg(l_version).arg(l_text));
            }
            l_chunk_strings.append(
                QString("<span class=\"tooltip\" style=\"font-style: italic; font-family: 'Noto Serif', 'Times New Roman', serif;\">%1").arg(l_ground_text) +
                QString("<span class=\"tooltiptext\" style=\"font-style: italic; font-family: 'Noto Sans', 'Arial', sans-serif;\">%1</span>").arg(l_ver_chunks.join(" / ")) + "</span>");
        }
        else
            l_chunk_strings.append(QString("%1").arg(l_interp->baseText().replace(g_re_no_base, "")));
    }

    return "<p>" + l_chunk_strings.join(" ") + "</p>";
}

/** --------------------------------------------------------------- DustbinInterp ---------------------------------
 * @brief M1MidPlane::DustbinInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::DustbinInterp* M1MidPlane::DustbinInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a DustbinInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::DustbinInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->specialItemId() == M1Env::DUSTBIN_SIID)
        l_ret = new DustbinInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief M1MidPlane::DustbinInterp::cm_dustbin_icon
 * @return
 */
QIcon M1MidPlane::DustbinInterp::cm_dustbin_icon(M1Env::DUSTBIN_ICON_PATH);

/**
 * @brief M1MidPlane::DustbinInterp::DustbinInterp
 * @param p_myself
 */
M1MidPlane::DustbinInterp::DustbinInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

/**
 * @brief M1MidPlane::DustbinInterp::inTreeDisplayText
 * @param p_edge
 * @return
 */
QString M1MidPlane::DustbinInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    int l_in_can_count = 0;
    for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(); !l_it.beyondEnd(); l_it.next())
        if(l_it.at()->isOfType(M1Env::AUTO_SIID) || l_it.at()->isOfType(M1Env::BLNGS_SIID))
            continue;
        else
            l_in_can_count += 1;
    return QString("[%1] Dustbin").arg(l_in_can_count);
}
