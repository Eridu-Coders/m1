#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_iterators.h"
#include "m1D_passages_panel.h"

#include <QPainter>
#include <QLocale>
#include <QDate>
#include <tuple>
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
#include <QUrl>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QMap>
#include "m1B_archive.h"

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
    QString l_author = authorText();
    return QString("%1%2").arg(l_author.length() > 0 ? QString("[%1] ").arg(l_author) : "").arg(m_myself->text());
}

/**
 * @brief Author name(s) of the text (", " separated)
 *
 * From the text's own Written by edges if any, otherwise from those of the work (WORKS) the text belongs to.
 * Names: see PersonInterp::shortName()
 *
 * @return the author name(s) ("" if none found)
 */
QString M1MidPlane::TextInterp::authorText() const{
    QString l_names = authorsOf(m_myself);
    if(l_names.isEmpty()){
        M1Store::Item_lv2* l_work_edge = m_myself->find_edge_generic(M1Env::BLNGS_SIID, M1Env::WORKS_SIID);
        if(l_work_edge != nullptr) l_names = authorsOf(l_work_edge->getTarget_lv2());
    }
    return l_names;
}

/**
 * @brief Names of the targets of the (live) Written by edges of a text or work (", " separated, see PersonInterp::shortName())
 *
 * @param p_item text or work node
 * @return the author name(s) ("" if none)
 */
QString M1MidPlane::TextInterp::authorsOf(const M1Store::Item_lv2* p_item){
    QStringList l_names;
    for(M1Store::Item_lv2_iterator l_it = p_item->getIteratorTop(); !l_it.beyondEnd(); l_it.next())
        if(l_it.at()->isFullEdge() && !l_it.at()->isDead() && l_it.at()->isOfType(M1Env::TEXT_WRITTEN_BY_SIID)){
            QString l_name = PersonInterp::shortName(l_it.at()->getTarget_lv2());
            if(l_name.length() > 0) l_names.append(l_name);
        }
    return l_names.join(", ");
}

/** --------------------------------------------------------------- WorkInterp ---------------------------------
 * @brief WorkInterp for WORKS nodes
 */
M1MidPlane::WorkInterp* M1MidPlane::WorkInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a WorkInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::WorkInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::WORKS_SIID))
        l_ret = new WorkInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::WorkInterp::WorkInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

/**
 * @brief "[Author(s)] Title", like TextInterp::inTreeDisplayText()
 */
QString M1MidPlane::WorkInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    QString l_author = TextInterp::authorsOf(m_myself);
    return QString("%1%2").arg(l_author.length() > 0 ? QString("[%1] ").arg(l_author) : "").arg(m_myself->text());
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
    QString l_author = authorText();
    if(l_author.length() > 0) l_html += QString("<h3>by: %1</h3>\n").arg(l_author.toHtmlEscaped());

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
            // deleted (EDGE_DEAD) edges are ignored
            if(!l_it.at()->isFullEdge() || l_it.at()->isDead()) continue;
            else if((l_it.at()->isOfType(M1Env::OWNS_SIID) && (l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_PARA_SIID) ||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_DIV_SIID))))
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

/** --------------------------------------------------------------- StephanusSectionInterp ---------------------------------
 * @brief M1MidPlane::StephanusSectionInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::StephanusSectionInterp* M1MidPlane::StephanusSectionInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an TranslationBhashya? %1").arg(p_myself->dbgShort()))
    M1MidPlane::StephanusSectionInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::STEPHANUS_SIID))
        l_ret = new StephanusSectionInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::StephanusSectionInterp::StephanusSectionInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}
QString M1MidPlane::StephanusSectionInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return m_myself->text();
}

void M1MidPlane::StephanusSectionInterp::initialize(){
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

QString M1MidPlane::StephanusSectionInterp::getHtmlVirtual(){
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

QWidget *M1MidPlane::StephanusSectionInterp::get_edit_widget(){
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

/** --------------------------------------------------------------- HighlightChunkInterp ---------------------------------
 * @brief M1MidPlane::HighlightChunkInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::HighlightChunkInterp* M1MidPlane::HighlightChunkInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an TranslationBhashya? %1").arg(p_myself->dbgShort()))
    M1MidPlane::HighlightChunkInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_HIGHLIGHT_CHUNK_SIID))
        l_ret = new HighlightChunkInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::HighlightChunkInterp::HighlightChunkInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

void M1MidPlane::HighlightChunkInterp::initialize(){
    if(! m_initialized){
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(M1Env::OWNS_SIID, M1Env::OCCUR_SIID); !l_it.beyondEnd(); l_it.next() )
            m_occ_list.push_back(getInterp(l_it.at()->getTarget_lv2()));
        m_initialized = true;
    }
}
QString M1MidPlane::HighlightChunkInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    initialize();
    QStringList l_word_list;
    for(const auto& l_occ_interp : m_occ_list) l_word_list.append(l_occ_interp->baseText());
    return QString("Passage in [%1]: %2").arg(m_myself->text()).arg(M1Store::StorageStatic::maxLengthChop(l_word_list.join(" "), 100));
}
QString M1MidPlane::HighlightChunkInterp::getHtmlVirtual(){
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
        if(l_it.at()->isDead() || l_it.at()->isOfType(M1Env::AUTO_SIID) || l_it.at()->isOfType(M1Env::BLNGS_SIID))
            continue;
        else
            l_in_can_count += 1;
    return QString("[%1] Dustbin").arg(l_in_can_count);
}

/** --------------------------------------------------------------- ContactIdInterp ---------------------------------
 * @brief list of phone country codes, one entry per distinct dialing code (loaded by loadCountryCodes())
 */
QList<M1MidPlane::PhoneCountryCode> M1MidPlane::ContactIdInterp::cm_country_codes;

/**
 * @brief Load the phone country codes list from STORE_DATA_PATH/COUNTRY_CODES_FILE (called from Interp::init() at startup)
 *
 * Countries sharing the same dialing code (+1, +7, +44, ...) are grouped into a single entry so that the
 * value stored in PHNCC fields (the dialing code) always maps back to exactly one combo box entry.
 */
void M1MidPlane::ContactIdInterp::loadCountryCodes(){
    cm_country_codes.clear();

    QString l_path = QString("%1/%2").arg(M1Store::STORE_DATA_PATH).arg(M1Store::COUNTRY_CODES_FILE);
    QFile l_file(l_path);
    if(!l_file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Cannot open phone country codes file:" << l_path;
        return;
    }
    QJsonParseError l_error;
    QJsonDocument l_doc = QJsonDocument::fromJson(l_file.readAll(), &l_error);
    if(l_doc.isNull()){
        qWarning() << "Cannot parse phone country codes file:" << l_path << l_error.errorString();
        return;
    }

    // dialing code --> list of country names
    QMap<QString, QStringList> l_code_2_names;
    for(const QJsonValue& l_val : l_doc.object().value("countries").toArray()){
        QJsonObject l_country = l_val.toObject();
        QString l_code = l_country.value("code").toString();
        if(l_code.length() > 0) l_code_2_names[l_code].append(l_country.value("name").toString());
    }
    for(auto l_it = l_code_2_names.cbegin(); l_it != l_code_2_names.cend(); ++l_it){
        QStringList l_names = l_it.value();
        l_names.sort(Qt::CaseInsensitive);
        cm_country_codes.append({l_it.key(), l_names.join(", ")});
    }
    // sorted by country name(s) for display in combo boxes
    std::sort(cm_country_codes.begin(), cm_country_codes.end(),
              [](const PhoneCountryCode& a, const PhoneCountryCode& b){return a.m_label.compare(b.m_label, Qt::CaseInsensitive) < 0;});

    qCDebug(g_cat_interp_base) << "Phone country codes loaded:" << cm_country_codes.size();
}

/**
 * @brief Contact ID subtypes (SIID + display label), in display order
 *
 * Built on first call because the SIID pseudo-constants are only set after store set-up
 */
const QList<std::pair<M1Env::SpecialItemID, QString>>& M1MidPlane::ContactIdInterp::subtypes(){
    static QList<std::pair<M1Env::SpecialItemID, QString>> ls_subtypes{
        {M1Env::CTC_EMAIL_SIID,     "E-mail"},
        {M1Env::CTC_PHONE_SIID,     "Phone Number"},
        {M1Env::CTC_DISCORD_SIID,   "Discord ID"},
        {M1Env::CTC_FACEBOOK_SIID,  "Facebook URL"},
        {M1Env::CTC_SNAPCHAT_SIID,  "SnapChat URL"},
        {M1Env::CTC_INSTAGRAM_SIID, "Instagram URL"},
        {M1Env::CTC_WECHAT_SIID,    "WeChat URL"},
        {M1Env::CTC_LINKEDIN_SIID,  "LinkedIn URL"},
        {M1Env::CTC_X_SIID,         "X URL"},
        {M1Env::CTC_YOUTUBE_SIID,   "YouTube URL"},
        {M1Env::CTC_SUBSTACK_SIID,  "Substack URL"},
    };
    return ls_subtypes;
}

bool M1MidPlane::ContactIdInterp::isUrlSubtype(const M1Env::SpecialItemID p_subtype){
    return p_subtype != M1Env::CTC_EMAIL_SIID && p_subtype != M1Env::CTC_PHONE_SIID && p_subtype != M1Env::CTC_DISCORD_SIID;
}

M1Env::SpecialItemID M1MidPlane::ContactIdInterp::subtypeOf(const M1Store::Item_lv2* p_contact){
    for(const auto& l_st : subtypes())
        if(p_contact->isOfType(l_st.first)) return l_st.first;
    return M1Env::G_VOID_SI_ID;
}

QString M1MidPlane::ContactIdInterp::subtypeLabel(const M1Env::SpecialItemID p_subtype){
    for(const auto& l_st : subtypes())
        if(l_st.first == p_subtype) return l_st.second;
    return "Contact ID";
}

M1MidPlane::ContactIdInterp* M1MidPlane::ContactIdInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a ContactIdInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::ContactIdInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::CONTACT_ID_SIID))
        l_ret = new ContactIdInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::ContactIdInterp::ContactIdInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

/**
 * @brief label of the node, preceded by the country code and followed by the messaging app letters for phone numbers
 */
QString M1MidPlane::ContactIdInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    if(!m_myself->isOfType(M1Env::CTC_PHONE_SIID)) return m_myself->text();

    QStringList l_parts;
    QString l_cc = m_myself->getField(M1Env::PHONE_COUNTRY_CODE_SIID);
    if(l_cc.length() > 0) l_parts.append(l_cc);
    l_parts.append(m_myself->text());
    QString l_links = m_myself->getField(M1Env::PHONE_MSG_LINKS_SIID);
    if(l_links.length() > 0) l_parts.append(l_links);
    return l_parts.join(" ");
}

QIcon* M1MidPlane::ContactIdInterp::vertexIcon(const M1Store::Item_lv2* p_edge){
    static QIcon ls_contact_icon(M1Env::CONTACT_ID_ICON_PATH);
    return &ls_contact_icon;
}

/**
 * @brief URL contact IDs --> the URL itself (displayed in the browser by Interp::getHtml(), like UrlInterp)
 */
QString M1MidPlane::ContactIdInterp::getHtmlVirtual(){
    if(m_myself->isOfType(M1Env::TEXT_URL_LINK_SIID)) return m_myself->text();
    return QString("<p><b>%1</b>: %2</p>\n")
        .arg(subtypeLabel(subtypeOf(m_myself)))
        .arg(inTreeDisplayText(nullptr).toHtmlEscaped());
}

/** --------------------------------------------------------------- PersonInterp ---------------------------------
 * @brief helper: set a simple edge field or remove it if the new value is empty
 */
static void setOrClearField(M1Store::Item_lv2* p_item, const M1Env::SpecialItemID p_field_type, const QString& p_value){
    M1Store::Item_lv2* l_field_edge = p_item->getFieldEdge(p_field_type);
    if(p_value.length() == 0){
        if(l_field_edge != nullptr) l_field_edge->unlinkEdge();
    }
    else if(l_field_edge == nullptr || l_field_edge->text() != p_value)
        p_item->setFieldEdge(p_value, p_field_type);
}

/// the 4 messaging apps a phone number can be linked to (letter stored in MSGLK, checkbox label)
static const QList<std::pair<QChar, QString>> g_msg_apps{
    {'W', "WhatsApp"}, {'T', "Telegram"}, {'S', "Signal"}, {'V', "Viber"}
};

M1MidPlane::PersonInterp* M1MidPlane::PersonInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a PersonInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::PersonInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::PERS_TYPE_SIID))
        l_ret = new PersonInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::PersonInterp::PersonInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

bool M1MidPlane::PersonInterp::cm_full_mode = false;

/**
 * @brief Honorific First Regnal Patronymic "Nickname" Last Generational Toponymic [Aliases], Title (birth–death)
 *
 * Only non-empty fields are included. Middle names are not displayed (only in the edit form). Dates: "(b–d)", "(b. b)" or "(d. d)"
 */
QString M1MidPlane::PersonInterp::namesText() const{
    QStringList l_parts;
    for(const M1Env::SpecialItemID l_field : {M1Env::PERS_HONORIFIC_SIID, M1Env::PERS_FIRST_NAME_SIID,
                                             M1Env::PERS_REGNAL_SIID, M1Env::PERS_PATRONYMIC_SIID, M1Env::PERS_NICKNAME_SIID,
                                             M1Env::PERS_LAST_NAME_SIID, M1Env::PERS_GENERATIONAL_SIID, M1Env::PERS_TOPONYMIC_SIID,
                                             M1Env::PERS_ALIAS_SIID}){
        QString l_val = m_myself->getField(l_field);
        if(l_val.length() > 0)
            l_parts.append(l_field == M1Env::PERS_NICKNAME_SIID ? QString("\"%1\"").arg(l_val) :
                           l_field == M1Env::PERS_ALIAS_SIID ? QString("[%1]").arg(l_val) : l_val);
    }
    QString l_ret = l_parts.join(" ");

    QString l_title = m_myself->getField(M1Env::PERS_TITLE_SIID);
    if(l_title.length() > 0) l_ret = l_ret.length() > 0 ? QString("%1, %2").arg(l_ret).arg(l_title) : l_title;

    QString l_birth = localizedDates(m_myself->getField(M1Env::PERS_BIRTH_DATE_SIID));
    QString l_death = localizedDates(m_myself->getField(M1Env::PERS_DEATH_DATE_SIID));
    QString l_dates;
    if(l_birth.length() > 0 && l_death.length() > 0) l_dates = QString("%1–%2").arg(l_birth).arg(l_death);
    else if(l_birth.length() > 0) l_dates = QString("b. %1").arg(l_birth);
    else if(l_death.length() > 0) l_dates = QString("d. %1").arg(l_death);
    if(l_dates.length() > 0) l_ret = l_ret.length() > 0 ? QString("%1 (%2)").arg(l_ret).arg(l_dates) : QString("(%1)").arg(l_dates);

    return l_ret;
}

/**
 * @brief First, middle and last names of a Person node (the "short" form fields); its label if they are all empty
 *
 * Used where a Person is referred to by name only (e.g. text authors, see TextInterp::authorText())
 *
 * @param p_person the Person node
 * @return the name
 */
QString M1MidPlane::PersonInterp::shortName(const M1Store::Item_lv2* p_person){
    QStringList l_parts;
    for(const M1Env::SpecialItemID l_field : {M1Env::PERS_FIRST_NAME_SIID, M1Env::PERS_MIDDLE_NAME_SIID, M1Env::PERS_LAST_NAME_SIID}){
        QString l_val = p_person->getField(l_field);
        if(l_val.length() > 0) l_parts.append(l_val);
    }
    return l_parts.isEmpty() ? QString(p_person->text()) : l_parts.join(" ");
}

/**
 * @brief Rewrite the ISO dates (Y-MM-DD or Y-MM, as produced by the TEI importer from @when) found in a date field text
 * according to the current locale
 *
 * - Y-MM-DD: locale short date format, with the year written in full and unpadded ("565-11-14" --> "14/11/565" in fr_FR,
 *   "11/14/565" in en_US), since the locale short format may have a 2-digit year
 * - Y-MM: locale month name + year ("1209-02" --> "février 1209" / "February 1209")
 *
 * Anything else ("c. 1810 BCE", "23/24 CE", invalid dates, ...) is left as is.
 *
 * @param p_text date field text
 * @return the text with its ISO dates localized
 */
QString M1MidPlane::PersonInterp::localizedDates(const QString& p_text){
    static const QRegularExpression ls_re_iso(R"((?<![\d-])(\d{1,4})-(\d{1,2})(?:-(\d{1,2}))?(?![\d-]))");
    static const QRegularExpression ls_re_year_token("y+");

    QLocale l_locale;
    QString l_ret;
    int l_last = 0;
    QRegularExpressionMatchIterator l_it = ls_re_iso.globalMatch(p_text);
    while(l_it.hasNext()){
        QRegularExpressionMatch l_m = l_it.next();
        int l_year = l_m.captured(1).toInt();
        int l_month = l_m.captured(2).toInt();
        QString l_localized;
        if(l_m.captured(3).length() > 0){
            QDate l_date(l_year, l_month, l_m.captured(3).toInt());
            if(l_date.isValid() && l_year > 0){
                // year as a quoted literal: full and unpadded whatever the locale format says
                QString l_format = l_locale.dateFormat(QLocale::ShortFormat);
                l_format.replace(ls_re_year_token, QString("'%1'").arg(l_year));
                l_localized = l_locale.toString(l_date, l_format);
            }
        }
        else if(l_month >= 1 && l_month <= 12 && l_year > 0)
            l_localized = QString("%1 %2").arg(l_locale.standaloneMonthName(l_month, QLocale::LongFormat)).arg(l_year);

        if(l_localized.length() > 0){
            l_ret += p_text.mid(l_last, l_m.capturedStart() - l_last) + l_localized;
            l_last = l_m.capturedEnd();
        }
    }
    return l_ret + p_text.mid(l_last);
}

/**
 * @brief names (see namesText()) followed by the node label, if any, after an em dash
 */
QString M1MidPlane::PersonInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    QString l_names = namesText();
    QString l_label = m_myself->text();
    if(l_names.length() == 0) return l_label;
    if(l_label.length() == 0) return l_names;
    return QString("%1 — %2").arg(l_names).arg(l_label);
}

QString M1MidPlane::PersonInterp::getHtmlVirtual(){
    QStringList l_contacts;
    for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorTop(); !l_it.beyondEnd(); l_it.next())
        if(l_it.at()->isFullEdge() && !l_it.at()->isDead() && l_it.at()->isOfType(M1Env::OWNS_SIID) && l_it.at()->getTarget_lv2()->isOfType(M1Env::CONTACT_ID_SIID)){
            std::shared_ptr<Interp> l_contact_interp = getInterp(l_it.at()->getTarget_lv2());
            M1Store::Item_lv2* l_contact = l_it.at()->getTarget_lv2();
            QString l_text = l_contact_interp->inTreeDisplayText(l_it.at()).toHtmlEscaped();
            if(l_contact->isOfType(M1Env::TEXT_URL_LINK_SIID))
                l_text = QString("<a href=\"%1\">%2</a>").arg(QString(l_contact->text()).toHtmlEscaped()).arg(l_text);
            l_contacts.append(QString("<tr><td>%1</td><td>:</td><td>%2</td></tr>")
                                  .arg(ContactIdInterp::subtypeLabel(ContactIdInterp::subtypeOf(l_contact)))
                                  .arg(l_text));
        }
    QString l_html = QString("<h4>%1</h4>\n").arg(inTreeDisplayText(nullptr).toHtmlEscaped());
    if(l_contacts.length() > 0) l_html += QString("<table>\n%1\n</table>\n").arg(l_contacts.join("\n"));
    return l_html;
}

/**
 * @brief Edit form: label + name fields, list of contact IDs (edit/delete) and contact ID creation
 */
QWidget *M1MidPlane::PersonInterp::get_edit_widget(){
    qCDebug(g_cat_interp_base).noquote() << QString("PersonInterp get_edit_widget()") << m_myself->dbgShort();

    QWidget* l_panel_widget = new QWidget();
    m_form = l_panel_widget;
    QVBoxLayout* l_panel_layout = new QVBoxLayout();
    l_panel_widget->setLayout(l_panel_layout);

    // button bar
    QWidget* l_button_bar = new QWidget(l_panel_widget);
    QHBoxLayout* l_bar_layout = new QHBoxLayout();
    l_bar_layout->setContentsMargins(0, 0, 0, 0);
    l_button_bar->setLayout(l_bar_layout);
    QPushButton* l_save_btn = new QPushButton("Save", l_button_bar);
    l_bar_layout->addWidget(l_save_btn);
    // short/full toggle (checked = full)
    m_mode_btn = new QPushButton(l_button_bar);
    m_mode_btn->setCheckable(true);
    l_bar_layout->addWidget(m_mode_btn);
    l_bar_layout->addStretch(1);
    l_panel_layout->addWidget(l_button_bar);
    QObject::connect(l_save_btn, &QPushButton::clicked, this, &M1MidPlane::PersonInterp::save_person);

    // names
    QGroupBox* l_names_box = new QGroupBox("Names", l_panel_widget);
    m_names_layout = new QFormLayout();
    l_names_box->setLayout(m_names_layout);
    m_full_only_edits.clear();
    m_label_edit = new QLineEdit(m_myself->text(), l_names_box);
    m_names_layout->addRow("Label", m_label_edit);
    m_full_only_edits.append(m_label_edit);
    m_name_edits.clear();
    // third member: displayed in short mode as well
    for(const auto& l_field : QList<std::tuple<M1Env::SpecialItemID, QString, bool>>{
                                {M1Env::PERS_HONORIFIC_SIID, "Honorific", false},
                                {M1Env::PERS_FIRST_NAME_SIID, "First name / praenomen", true},
                                {M1Env::PERS_MIDDLE_NAME_SIID, "Middle name(s)", true},
                                {M1Env::PERS_REGNAL_SIID, "Regnal number", false},
                                {M1Env::PERS_PATRONYMIC_SIID, "Patronymic", false},
                                {M1Env::PERS_NICKNAME_SIID, "Nickname / cognomen / epithet", false},
                                {M1Env::PERS_LAST_NAME_SIID, "Last name / nomen", true},
                                {M1Env::PERS_GENERATIONAL_SIID, "Generational suffix", false},
                                {M1Env::PERS_TOPONYMIC_SIID, "Toponymic byname", false},
                                {M1Env::PERS_ALIAS_SIID, "Alias(es)", false},
                                {M1Env::PERS_TITLE_SIID, "Title", false},
                                {M1Env::PERS_BIRTH_DATE_SIID, "Birth date", false},
                                {M1Env::PERS_DEATH_DATE_SIID, "Death date", false},
                                // NMTMK: TEI key (@xml:id) used to match <name>/<author> references to this person
                                {M1Env::TEI_NAME_TERM_KEY_SIID, "Key", false}}){
        QLineEdit* l_edit = new QLineEdit(m_myself->getField(std::get<0>(l_field)), l_names_box);
        m_names_layout->addRow(std::get<1>(l_field), l_edit);
        m_name_edits.append({std::get<0>(l_field), l_edit});
        if(! std::get<2>(l_field)) m_full_only_edits.append(l_edit);
    }
    l_panel_layout->addWidget(l_names_box);
    QObject::connect(m_mode_btn, &QPushButton::toggled, this, &M1MidPlane::PersonInterp::toggle_mode);
    m_mode_btn->setChecked(cm_full_mode);
    toggle_mode(cm_full_mode);

    // contact IDs
    QGroupBox* l_contacts_box = new QGroupBox("Contact IDs", l_panel_widget);
    QVBoxLayout* l_contacts_box_layout = new QVBoxLayout();
    l_contacts_box->setLayout(l_contacts_box_layout);

    QWidget* l_contacts_widget = new QWidget(l_contacts_box);
    m_contacts_layout = new QVBoxLayout();
    m_contacts_layout->setContentsMargins(0, 0, 0, 0);
    l_contacts_widget->setLayout(m_contacts_layout);
    l_contacts_box_layout->addWidget(l_contacts_widget);

    // new contact ID bar
    QWidget* l_add_bar = new QWidget(l_contacts_box);
    QHBoxLayout* l_add_layout = new QHBoxLayout();
    l_add_layout->setContentsMargins(0, 0, 0, 0);
    l_add_bar->setLayout(l_add_layout);
    m_new_contact_type = new QComboBox(l_add_bar);
    for(const auto& l_st : ContactIdInterp::subtypes())
        m_new_contact_type->addItem(l_st.second, QVariant::fromValue(static_cast<unsigned int>(l_st.first)));
    QPushButton* l_add_btn = new QPushButton("Add Contact ID", l_add_bar);
    l_add_layout->addWidget(m_new_contact_type);
    l_add_layout->addWidget(l_add_btn);
    l_add_layout->addStretch(1);
    l_contacts_box_layout->addWidget(l_add_bar);
    QObject::connect(l_add_btn, &QPushButton::clicked, this, &M1MidPlane::PersonInterp::add_contact);

    l_panel_layout->addWidget(l_contacts_box);
    l_panel_layout->addStretch(1);

    populateContacts();

    // the form may be bigger than the edit tab
    QScrollArea* l_scroll = new QScrollArea();
    l_scroll->setWidgetResizable(true);
    l_scroll->setWidget(l_panel_widget);
    return l_scroll;
}

/**
 * @brief (re)build the contact ID rows of the edit form from the graph
 */
void M1MidPlane::PersonInterp::populateContacts(){
    if(m_form.isNull() || m_contacts_layout == nullptr) return;

    // remove existing rows (deleteLater bc this may be called from a slot triggered by one of their buttons)
    while(QLayoutItem* l_item = m_contacts_layout->takeAt(0)){
        if(l_item->widget() != nullptr) l_item->widget()->deleteLater();
        delete l_item;
    }
    m_contact_rows.clear();

    for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorTop(); !l_it.beyondEnd(); l_it.next()){
        M1Store::Item_lv2* l_edge = l_it.at();
        if(!(l_edge->isFullEdge() && !l_edge->isDead() && l_edge->isOfType(M1Env::OWNS_SIID) && l_edge->getTarget_lv2()->isOfType(M1Env::CONTACT_ID_SIID))) continue;

        ContactRow l_row;
        l_row.m_edge = l_edge;
        l_row.m_contact = l_edge->getTarget_lv2();
        M1Env::SpecialItemID l_subtype = ContactIdInterp::subtypeOf(l_row.m_contact);

        QWidget* l_row_widget = new QWidget();
        QHBoxLayout* l_row_layout = new QHBoxLayout();
        l_row_layout->setContentsMargins(0, 0, 0, 0);
        l_row_widget->setLayout(l_row_layout);

        QLabel* l_type_label = new QLabel(ContactIdInterp::subtypeLabel(l_subtype), l_row_widget);
        l_type_label->setMinimumWidth(100);
        l_row_layout->addWidget(l_type_label);

        if(l_subtype == M1Env::CTC_PHONE_SIID){
            l_row.m_country = new QComboBox(l_row_widget);
            l_row.m_country->addItem("(no country code)", QString());
            QString l_cc = l_row.m_contact->getField(M1Env::PHONE_COUNTRY_CODE_SIID);
            int l_cc_index = 0;
            for(const PhoneCountryCode& l_pcc : ContactIdInterp::countryCodes()){
                l_row.m_country->addItem(QString("%1 (%2)").arg(l_pcc.m_label).arg(l_pcc.m_code), l_pcc.m_code);
                if(l_pcc.m_code == l_cc) l_cc_index = l_row.m_country->count() - 1;
            }
            // value not in the list (e.g. list changed) --> keep it anyway
            if(l_cc.length() > 0 && l_cc_index == 0){
                l_row.m_country->addItem(l_cc, l_cc);
                l_cc_index = l_row.m_country->count() - 1;
            }
            l_row.m_country->setCurrentIndex(l_cc_index);
            l_row.m_country->setMaximumWidth(220);
            l_row_layout->addWidget(l_row.m_country);
        }

        l_row.m_value = new QLineEdit(l_row.m_contact->text(), l_row_widget);
        l_row_layout->addWidget(l_row.m_value, 1);

        if(l_subtype == M1Env::CTC_PHONE_SIID){
            QString l_links = l_row.m_contact->getField(M1Env::PHONE_MSG_LINKS_SIID);
            for(const auto& l_app : g_msg_apps){
                QCheckBox* l_cb = new QCheckBox(l_app.second, l_row_widget);
                l_cb->setChecked(l_links.contains(l_app.first));
                l_row_layout->addWidget(l_cb);
                l_row.m_msg_links.append(l_cb);
            }
        }

        QPushButton* l_del_btn = new QPushButton("Delete", l_row_widget);
        l_row_layout->addWidget(l_del_btn);
        QObject::connect(l_del_btn, &QPushButton::clicked, this, [this, l_edge](){delete_contact(l_edge);});

        m_contacts_layout->addWidget(l_row_widget);
        m_contact_rows.push_back(l_row);
    }
}

/**
 * @brief write the contents of the contact ID rows into the graph
 */
void M1MidPlane::PersonInterp::saveContactRows(){
    for(const ContactRow& l_row : m_contact_rows){
        QString l_value = l_row.m_value->text().trimmed();
        if(l_value != l_row.m_contact->text()) l_row.m_contact->setText_lv1(l_value);

        if(l_row.m_country != nullptr)
            setOrClearField(l_row.m_contact, M1Env::PHONE_COUNTRY_CODE_SIID, l_row.m_country->currentData().toString());

        if(l_row.m_msg_links.size() > 0){
            QString l_links;
            for(int i = 0; i < l_row.m_msg_links.size(); i++)
                if(l_row.m_msg_links[i]->isChecked()) l_links += g_msg_apps[i].first;
            setOrClearField(l_row.m_contact, M1Env::PHONE_MSG_LINKS_SIID, l_links);
        }
    }
}

/**
 * @brief full tree rebuild (structural change: contact ID added or removed)
 */
void M1MidPlane::PersonInterp::refreshTree(){
    if(m_tree_row != nullptr) emit m_tree_row->gotoVertex(nullptr, m_tree_row);
}

/**
 * @brief Save button: label, name fields and contact IDs
 */
void M1MidPlane::PersonInterp::save_person(){
    M1Store::Transaction l_archive_tx("Save person");
    qCDebug(g_cat_interp_base).noquote() << "Saving Person" << m_myself->dbgShort();
    if(m_form.isNull()) return;

    QString l_label = m_label_edit->text().trimmed();
    if(l_label != m_myself->text()) m_myself->setText_lv1(l_label);
    for(const auto& l_name : m_name_edits)
        setOrClearField(m_myself, l_name.first, l_name.second->text().trimmed());

    saveContactRows();

    if(m_tree_row != nullptr) m_tree_row->performPostUpdate();
}

/**
 * @brief Short/Full toggle: in short mode only first, middle and last names are displayed (and thus editable)
 *
 * Hidden fields keep their values and are saved unchanged
 */
void M1MidPlane::PersonInterp::toggle_mode(bool p_full){
    if(m_form.isNull()) return;
    cm_full_mode = p_full;
    m_mode_btn->setText(p_full ? "Full" : "Short");
    m_mode_btn->setToolTip(p_full ? "Full mode: all fields. Click for short mode (first, middle and last names only)"
                                  : "Short mode: first, middle and last names only. Click for full mode (all fields)");
    for(QWidget* l_edit : m_full_only_edits)
        m_names_layout->setRowVisible(l_edit, p_full);
}

/**
 * @brief Add Contact ID button: creates a new (empty) contact ID of the selected subtype, owned by this person
 *
 * Pending edits in the form are saved first, since the form is rebuilt.
 */
void M1MidPlane::PersonInterp::add_contact(){
    M1Store::Transaction l_archive_tx("Add contact ID");
    if(m_form.isNull()) return;
    M1Env::SpecialItemID l_subtype = static_cast<M1Env::SpecialItemID>(m_new_contact_type->currentData().toUInt());
    qCDebug(g_cat_interp_base).noquote() << "Adding contact ID" << ContactIdInterp::subtypeLabel(l_subtype) << "to" << m_myself->dbgShort();

    saveContactRows();

    // OWNS edge (+ BLNGS reciprocal) from this person to the new contact ID, at the bottom of the edge list
    M1Store::Item_lv2* l_contact = m_myself->create_descendant(M1Env::OWNS_SIID, "", M1Env::CONTACT_ID_SIID, M1Store::InsertionPoint::at_bottom);
    l_contact->setType(l_subtype);
    if(ContactIdInterp::isUrlSubtype(l_subtype)) l_contact->setType(M1Env::TEXT_URL_LINK_SIID);

    populateContacts();
    if(m_contact_rows.size() > 0) m_contact_rows.back().m_value->setFocus();
    refreshTree();
}

/**
 * @brief Delete button of a contact ID row: the OWNS edge (and its BLNGS reciprocal) between this person and the contact ID is marked dead (EDGE_DEAD)
 * @param p_edge the OWNS edge from this person to the contact ID
 */
void M1MidPlane::PersonInterp::delete_contact(M1Store::Item_lv2* p_edge){
    M1Store::Transaction l_archive_tx("Delete contact ID");
    if(m_form.isNull()) return;
    M1Store::Item_lv2* l_contact = p_edge->getTarget_lv2();

    if(QMessageBox::question(m_form, "Delete Contact ID",
                              QString("Delete %1 \"%2\"?")
                                  .arg(ContactIdInterp::subtypeLabel(ContactIdInterp::subtypeOf(l_contact)))
                                  .arg(l_contact->text())) != QMessageBox::Yes)
        return;

    qCDebug(g_cat_interp_base).noquote() << "Deleting contact ID" << l_contact->dbgShort() << "from" << m_myself->dbgShort();
    saveContactRows();

    p_edge->deleteEdge();

    populateContacts();
    refreshTree();
}
