#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_iterators.h"

#include <QUrl>

/**
 * @file m1C_interp_tei.cpp
 * @brief TEI-related subclasses of Interp (TEI_Container and its descendants)
 */

/** --------------------------------------------------------------- TEI_Container ---------------------------------
 * @brief M1MidPlane::TEI_Container::initialize
 */
M1MidPlane::TEI_Container::TEI_Container(M1Store::Item_lv2* p_myself) : M1MidPlane::Interp::Interp(p_myself){}

void M1MidPlane::TEI_Container::initialize(){
    if(!m_initialized){
        for(M1Store::Item_lv2_iterator l_it = m_myself->getIteratorAuto(); !l_it.beyondEnd(); l_it.next())
            if(!l_it.at()->isFullEdge()) continue;
            else if(l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_CHUNK_SIID) && l_it.at()->getTarget_lv2()->getField(M1Env::TEI_TAG_NAME_SIID) == "head")
                m_heading = getInterp(l_it.at()->getTarget_lv2());
            else if((l_it.at()->isOfType(M1Env::OWNS_SIID) && (l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_CHUNK_SIID) ||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_PARA_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_DIV_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_TABLE_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_ROW_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_CELL_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_FIGURE_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_QUOTE_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_LIST_SIID)||
                                                                l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_ITEM_SIID)) ||
                      (l_it.at()->isOfType(M1Env::TEXT_HIGHLIGHT_QUOTE_SIID) && l_it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_SIID))))
                m_descendant_list.push_back(getInterp(l_it.at()->getTarget_lv2()));
        m_initialized = true;
    }
}

QString M1MidPlane::TEI_Container::baseText(){
    initialize();
    if(m_descendant_list.empty()){
        if(m_heading != nullptr)
            return QString("<p style=\"text-align: center;\">%1</p>").arg(m_heading->baseText());
        else
            return m_myself->text();
    }

    // chunks are joined using their own PCTLF (leading space) and PCTRT (trailing punctuation,
    // glued to this chunk from what follows it in the source) fields instead of a plain space,
    // so the original TEI spacing/punctuation is reproduced -- see TEIInterface::create_chunk()
    // and the Characters-run handling in loadTeiInternal()
    QString l_ret;
    bool l_first = true;
    for(const auto& l_cur_chunk: m_descendant_list){
        QString l_base_text = l_cur_chunk->baseText();
        // trimmed: a chunk's own rendered HTML (e.g. the indented <span> block built for
        // "note") may carry incidental leading/trailing whitespace that is not part of its
        // content; only PCTLF/PCTRT are allowed to introduce separators between chunks
        l_base_text = QString("%1").arg(l_base_text.replace(g_re_no_base, "")).trimmed();

        if(!l_first) l_ret += l_cur_chunk->myself()->getField(M1Env::PCTLF_SIID);
        l_ret += l_base_text;
        l_ret += l_cur_chunk->myself()->getField(M1Env::PCTRT_SIID);
        l_first = false;
    }

    return l_ret;
}

/** --------------------------------------------------------------- TEI_DivInterp ---------------------------------
 * @brief M1MidPlane::TEI_DivInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_DivInterp* M1MidPlane::TEI_DivInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_DivInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_DivInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_DIV_SIID)){
        l_ret = new TEI_DivInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "<div>" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_DivInterp::TEI_DivInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}

QString M1MidPlane::TEI_DivInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("<div> %1: %2")
        .arg(m_myself->text())
        .arg(M1Store::StorageStatic::maxLengthChop(TEI_Container::get_heading().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_DivInterp::baseText(){
    return QString("<div><p style=\"font-weight: bold\">%1</p>\n")
               .arg(TEI_Container::get_heading()) +
           TEI_Container::baseText() + "</div>\n";
}

QString M1MidPlane::TEI_DivInterp::getHtmlVirtual(){
    return this->baseText();
}

/** --------------------------------------------------------------- TEI_ParagraphInterp ---------------------------------
 * @brief M1MidPlane::TEI_ParagraphInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_ParagraphInterp* M1MidPlane::TEI_ParagraphInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_ParagraphInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_ParagraphInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_PARA_SIID)){
        l_ret = new TEI_ParagraphInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "<p>" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_ParagraphInterp::TEI_ParagraphInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}

QString M1MidPlane::TEI_ParagraphInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("%1: %2")
        .arg(m_myself->text())
        .arg(M1Store::StorageStatic::maxLengthChop(TEI_Container::baseText().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_ParagraphInterp::baseText(){
    return "<p>" + TEI_Container::baseText() + "</p>\n";
}

QString M1MidPlane::TEI_ParagraphInterp::getHtmlVirtual(){
    return this->baseText();
}

/** --------------------------------------------------------------- TEI_ChunkInterp ---------------------------------
 * @brief M1MidPlane::TEI_ChunkInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_ChunkInterp* M1MidPlane::TEI_ChunkInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_ChunkInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_ChunkInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_CHUNK_SIID)){
        l_ret = new TEI_ChunkInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "chunk" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_ChunkInterp::TEI_ChunkInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){
    m_chunk_tag = m_myself->getField(M1Store::TEI_TAG_NAME_SIID);
}

QString M1MidPlane::TEI_ChunkInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    QString l_base_text = M1Store::StorageStatic::maxLengthChop(baseText().replace(g_re_html_tags, "").replace(g_re_space, " "), 50);
    QString l_ret;

    if(m_chunk_tag == "name") l_ret = QString("<name> %1").arg(l_base_text);
    else if(m_chunk_tag == "head") l_ret = QString("<head> %1").arg(l_base_text);
    else if(m_chunk_tag == "hi") l_ret = QString("<hi> %1").arg(l_base_text);
    else if(m_chunk_tag == "term") l_ret = QString("<term> %1").arg(l_base_text);
    else if(m_chunk_tag == "title") l_ret = QString("<title> %1").arg(l_base_text);
    else if(m_chunk_tag == "q") l_ret = QString("<q> %1").arg(l_base_text);
    else if(m_chunk_tag == "cit" || m_chunk_tag == "bibl" || m_chunk_tag == "author" || m_chunk_tag == "publisher" ||
             m_chunk_tag == "date" || m_chunk_tag == "pubPlace" || m_chunk_tag == "citedRange" ||
             m_chunk_tag == "biblScope" || m_chunk_tag == "ref")
        l_ret = QString("<%1> %2").arg(m_chunk_tag).arg(l_base_text);
    else if(m_chunk_tag == "note") l_ret = QString("<note> %1").arg(l_base_text);
    else if(m_chunk_tag == "lg") l_ret = QString("<lg> %1").arg(l_base_text);
    else if(m_chunk_tag == "l") l_ret = QString("<l> %1").arg(l_base_text);
    else l_ret = l_base_text;

    return l_ret.replace(g_re_space, " ");
}
QString M1MidPlane::TEI_ChunkInterp::getHtmlVirtual(){
    return baseText();
}

QString M1MidPlane::TEI_ChunkInterp::baseText(){
    if(m_chunk_tag == "Characters") return m_myself->text();
    // <name>/<term>/<title> are containers: their text is in their sub-chunks (their own label is
    // "[tag] joined text"). TEI_Container::baseText() falls back on the label when there are no
    // sub-chunks, which keeps texts imported before they became containers displaying correctly
    if(m_chunk_tag == "name" ) return QString("<span style=\"background-color: #fdd;\">%1</span>").arg(TEI_Container::baseText());
    if(m_chunk_tag == "term" ) return QString("<span style=\"background-color: #ddf;\">%1</span>").arg(TEI_Container::baseText());
    if(m_chunk_tag == "title") return QString("<i>%1</i>").arg(TEI_Container::baseText());
    // <q>: the quotation marks are not in the source text --> added here
    if(m_chunk_tag == "q"){
        QString l_quoted = QString("“%1”").arg(TEI_Container::baseText());
        return m_myself->getField(M1Env::TEI_REND_ATTR_SIID) == "italic" ? QString("<i>%1</i>").arg(l_quoted) : l_quoted;
    }
    // <cit>: block made of the <quote> (rendered as an indented paragraph) followed by its <bibl>
    if(m_chunk_tag == "cit") return QString("<div style=\"margin-bottom: 1em;\">\n%1</div>\n").arg(TEI_Container::baseText());
    // <ref target="...">: link. <bibl> and the other bibliographic components (<author>, <date>, <publisher>,
    // <pubPlace>, <citedRange>, <biblScope>) are rendered as plain text by the final else below
    // (TEI_Container::baseText() returns the label of a child-less chunk)
    if(m_chunk_tag == "ref"){
        QString l_target = m_myself->getField(M1Env::TEI_TARGET_ATTR_SIID);
        return l_target.length() > 0 ? QString("<a href=\"%1\">%2</a>").arg(l_target.toHtmlEscaped()).arg(m_myself->text()) : m_myself->text();
    }
    if(m_chunk_tag == "hi") return QString("<%1>%3</%1>").arg(m_myself->getField(M1Env::TEI_REND_ATTR_SIID) == "bold" ? "b" : "i").arg(TEI_Container::baseText());
    if(m_chunk_tag == "note"){
        QString l_note_n = m_myself->getField(M1Store::TEI_N_ATTR_SIID);
        return \
            QString("    <span class=\"footnote-container\">\n") + \
            QString("        <button class=\"footnote-btn\" aria-expanded=\"false\" aria-controls=\"fn%1-content\">[%1]</button>\n").arg(l_note_n) + \
            QString("        <span class=\"footnote-popup\" id=\"fn%1-content\" role=\"tooltip\">\n%2\n").arg(l_note_n).arg(TEI_Container::baseText()) + \
            QString("        </span>\n") + \
            QString("    </span>\n");
    }
    if(m_chunk_tag == "l") return TEI_Container::baseText() + "<br/>\n";
    if(m_chunk_tag == "lg") return QString("<div style=\"padding-left: 2em; font-style: italic; margin-bottom: 1em;\">\n%1</div>\n").arg(TEI_Container::baseText());
    else return TEI_Container::baseText();
}

/** --------------------------------------------------------------- TEI_TableInterp ---------------------------------
 * @brief M1MidPlane::TEI_TableInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_TableInterp* M1MidPlane::TEI_TableInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_TableInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_TableInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_TABLE_SIID)){
        l_ret = new TEI_TableInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "chunk" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_TableInterp::TEI_TableInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}
QString M1MidPlane::TEI_TableInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("%1: %2")
        .arg(m_myself->text())
        .arg(M1Store::StorageStatic::maxLengthChop(TEI_Container::get_heading().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_TableInterp::baseText(){
    return "<table style=\"border-collapse: collapse;\">\n" + TEI_Container::baseText() + "</table>\n";
}
QString M1MidPlane::TEI_TableInterp::getHtmlVirtual(){
    return this->baseText();
}

/** --------------------------------------------------------------- TEI_RowInterp ---------------------------------
 * @brief M1MidPlane::TEI_RowInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_RowInterp* M1MidPlane::TEI_RowInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_RowInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_RowInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_ROW_SIID)){
        l_ret = new TEI_RowInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "chunk" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_RowInterp::TEI_RowInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}
QString M1MidPlane::TEI_RowInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("%1: %2")
        .arg(m_myself->text())
        .arg(M1Store::StorageStatic::maxLengthChop(this->baseText().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_RowInterp::getHtmlVirtual(){
    qCDebug(g_cat_tmp_spotlight).noquote() << "TEI_RowInterp getHtmlVirtual()" << this->baseText();
    return QString("<table style=\"border-collapse: collapse;\">%1</table>").arg(this->baseText());
}
QString M1MidPlane::TEI_RowInterp::baseText(){
    qCDebug(g_cat_tmp_spotlight).noquote() << "TEI_RowInterp baseText()" << TEI_Container::baseText();
    return "<tr>\n" + TEI_Container::baseText() + "</tr>\n";
}

/** --------------------------------------------------------------- TEI_CellInterp ---------------------------------
 * @brief M1MidPlane::TEI_CellInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_CellInterp* M1MidPlane::TEI_CellInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_CellInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_CellInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_CELL_SIID)){
        l_ret = new TEI_CellInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "chunk" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_CellInterp::TEI_CellInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}
QString M1MidPlane::TEI_CellInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("%1: %2")
        .arg(m_myself->text())
        .arg(M1Store::StorageStatic::maxLengthChop(baseText().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_CellInterp::getHtmlVirtual(){
    return QString("<table><tr>%1</tr></table>").arg(this->baseText());
}
QString M1MidPlane::TEI_CellInterp::baseText(){
    return "<td style=\"border: 1px solid black; padding: .3em\">\n" + TEI_Container::baseText() + "</td>\n";
}

/** --------------------------------------------------------------- TEI_FigureInterp ---------------------------------
 * @brief M1MidPlane::TEI_FigureInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_FigureInterp* M1MidPlane::TEI_FigureInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_FigureInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_FigureInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_FIGURE_SIID)){
        l_ret = new TEI_FigureInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "TEI_FigureInterp" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_FigureInterp::TEI_FigureInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}
QString M1MidPlane::TEI_FigureInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("<figure> %1")
        .arg(M1Store::StorageStatic::maxLengthChop(TEI_Container::get_heading().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_FigureInterp::getHtmlVirtual(){return this->baseText();}
QString M1MidPlane::TEI_FigureInterp::baseText(){
    return QString("<p style=\"padding-left:2em;\">\n<img src=\"%1\" style=\"display: block; margin: 0 auto; max-width: 90%; height: auto;\"/>\n%2</p>\n")
        .arg(QUrl::fromPercentEncoding(m_myself->getField(M1Env::TEI_GRAPHIC_SIID).toUtf8()))
        .arg(TEI_Container::baseText());
}

/** --------------------------------------------------------------- TEI_QuoteInterp ---------------------------------
 * @brief M1MidPlane::TEI_QuoteInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_QuoteInterp* M1MidPlane::TEI_QuoteInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_QuoteInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_QuoteInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_QUOTE_SIID)){
        l_ret = new TEI_QuoteInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "TEI_QuoteInterp" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_QuoteInterp::TEI_QuoteInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}
QString M1MidPlane::TEI_QuoteInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("<quote> %1")
        .arg(M1Store::StorageStatic::maxLengthChop(TEI_Container::baseText().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_QuoteInterp::getHtmlVirtual(){
    return this->baseText();
}
QString M1MidPlane::TEI_QuoteInterp::baseText(){
    return "<p style=\"padding-left: 2em; font-style: italic;\">" + TEI_Container::baseText() + "</p>\n";
}

/** --------------------------------------------------------------- TEI_ListInterp ---------------------------------
 * @brief M1MidPlane::TEI_ListInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_ListInterp* M1MidPlane::TEI_ListInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_ListInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_ListInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_LIST_SIID)){
        l_ret = new TEI_ListInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "TEI_ListInterp" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_ListInterp::TEI_ListInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}

QString M1MidPlane::TEI_ListInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("<list type=%1> %2")
        .arg(m_myself->getField(M1Env::TEI_LIST_TYPE_SIID))
        .arg(M1Store::StorageStatic::maxLengthChop(TEI_Container::baseText().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_ListInterp::getHtmlVirtual(){
    return this->baseText();
}
QString M1MidPlane::TEI_ListInterp::baseText(){
    return "<ul>\n" + TEI_Container::baseText() + "</ul>\n";
}

/** --------------------------------------------------------------- TEI_ItemInterp ---------------------------------
 * @brief M1MidPlane::TEI_ItemInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::TEI_ItemInterp* M1MidPlane::TEI_ItemInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be a TEI_ItemInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::TEI_ItemInterp* l_ret = nullptr;
    if(p_myself->isFullVertex() && p_myself->isOfType(M1Env::TEXT_ITEM_SIID)){
        l_ret = new TEI_ItemInterp(p_myself);
        qCDebug(g_cat_tmp_spotlight).noquote() << "TEI_ListInterp" << p_myself->dbgShort();
    }
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::TEI_ItemInterp::TEI_ItemInterp(M1Store::Item_lv2* p_myself) : M1MidPlane::TEI_Container::TEI_Container(p_myself){}
QString M1MidPlane::TEI_ItemInterp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return QString("<item> %1")
        .arg(M1Store::StorageStatic::maxLengthChop(TEI_Container::baseText().replace(g_re_html_tags, "").replace(g_re_space, " "), 50));
}
QString M1MidPlane::TEI_ItemInterp::getHtmlVirtual(){
    return this->baseText();
}
QString M1MidPlane::TEI_ItemInterp::baseText(){
    return "<li>\n" + TEI_Container::baseText() + "</li>\n";
}
