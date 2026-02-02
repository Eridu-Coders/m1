#include "m1A_constants.h"
#include "m1A_env.h"
#include "m1B_lv2_iterators.h"
#include "m1D_passages_panel.h"
#include "m1C_interp.h"
#include "m1B_graph_init.h"

#include <QResizeEvent>
#include <QRegularExpression>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDrag>

// g_cat_passages_panel
Q_LOGGING_CATEGORY(g_cat_passages_panel, "passages_panel")

// BasePassageItem ---------------------------------------------------------------------------------------------------------------------------------------
QFont M1UI::WordItem::BasePassageItem::cm_base_font = QFont("Noto Mono", 12);

/**
 * @brief M1UI::BasePassageItem::BasePassageItem
 *
 * \todo English passage lengths proportional to Greek length (calculate ratio of distance between 2 Stephanus markers)
 *
 * @param p_id
 * @param p_parent
 */
M1UI::BasePassageItem::BasePassageItem(const int p_id, PassageEditor *p_parent): QGraphicsSimpleTextItem(p_parent){
    m_id = p_id;
    m_editor = p_parent;
    m_height = QFontInfo(cm_base_font).pixelSize();
    QGraphicsSimpleTextItem::setAcceptDrops(true);
}
QSizeF M1UI::BasePassageItem::sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint) const {
    QSizeF l_ret = QSizeF(QGraphicsSimpleTextItem::boundingRect().size().width(), m_height);
    qCDebug(g_cat_passages_panel) << QString("BasePassageItem::sizeHint [") << p_which << p_constraint << QString("] %1 -->").arg(this->text()) << l_ret;
    return l_ret;
}
QRectF M1UI::BasePassageItem::boundingRect() const{
    //qCDebug(g_cat_passages_panel) << QString("BasePassageItem boundingRect()") << m_id << l_br.topLeft() << l_br.bottomRight();
    QRectF l_ret = QRectF(QGraphicsSimpleTextItem::boundingRect().topLeft(),
                          QSizeF(QGraphicsSimpleTextItem::boundingRect().size().width(), m_height));
    return l_ret;
}
void M1UI::BasePassageItem::paint(QPainter *p_painter,
                           const QStyleOptionGraphicsItem *p_option,
                           QWidget *p_widget)
{
    //if(m_end_line) p_painter->setPen(Qt::red);
    //if(m_start_line) p_painter->setPen(Qt::green);
    //p_painter->drawRect(boundingRect());
    if(m_color.length() >0){
        QRectF l_br = boundingRect() + QMargins(4, 2, 2, 4);
        if(!(m_start_selection || m_start_line)) l_br += QMargins(m_editor->spacing()-4, 0, 0, 0);

        p_painter->fillRect(l_br, QBrush(QColor(m_color)));
    }

    QGraphicsSimpleTextItem::paint(p_painter, p_option, p_widget);

    if(m_in_selection){
        p_painter->setPen(QPen(Qt::red, 3, Qt::DashLine));

        QRectF l_br = boundingRect() + QMargins(4, 2, 2, 4);

        if(m_start_selection || m_start_line) p_painter->drawLine(QLineF(l_br.topLeft(), l_br.bottomLeft()));
        else l_br += QMargins(m_editor->spacing(), 0, 0, 0);

        // p_painter->drawRect(l_br);
        p_painter->drawLine(QLineF(l_br.topLeft(), l_br.topRight()));
        p_painter->drawLine(QLineF(l_br.bottomLeft(), l_br.bottomRight()));

        if(m_end_selection || m_end_line) p_painter->drawLine(QLineF(l_br.topRight(), l_br.bottomRight()));
        /*
        p_painter->setPen(QPen(Qt::green, 3, Qt::DashLine));
        if(m_end_selection) p_painter->drawLine(QLineF(l_br.topRight(), l_br.bottomRight()));
        p_painter->setPen(QPen(Qt::blue, 3, Qt::DashLine));
        if(m_end_line) p_painter->drawLine(QLineF(l_br.topRight(), l_br.bottomRight()));
        */
        qCDebug(g_cat_passages_panel) << QString("WordItem paint selection") << this->text() << l_br.topLeft() << l_br.bottomRight() << l_br;
    }
}
void M1UI::BasePassageItem::mousePressEvent(QGraphicsSceneMouseEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("mouse_press") << this->text() << p_event;
    m_editor->unselect_all();
    QString l_payload = QString("%1").arg(m_id);
    qCDebug(g_cat_passages_panel) << QString("Drag CREATION ") << this->text() << l_payload;
    QMimeData *l_data = new QMimeData;
    l_data->setText(l_payload);
    QDrag *l_drag = new QDrag(p_event->widget());
    l_drag->setMimeData(l_data);
    l_drag->exec();
}
void M1UI::BasePassageItem::dragEnterEvent(QGraphicsSceneDragDropEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("Drag ENTER p_event") << m_id << this->text() << p_event->mimeData()->text() << p_event;
    p_event->setAccepted(true);
}
void M1UI::BasePassageItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("Drag LEAVE p_event") << m_id << this->text() << p_event->mimeData()->text() << p_event;
}
void M1UI::BasePassageItem::dragMoveEvent(QGraphicsSceneDragDropEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("Drag MOVE p_event") << m_id << this->text() << p_event->mimeData()->text() << p_event;
    int l_from = m_id;
    int l_to = p_event->mimeData()->text().toInt();
    if(l_from > l_to){
        int l_tmp = l_from;
        l_from = l_to;
        l_to = l_tmp;
    }
    m_editor->select_from_to(l_from, l_to);
}
void M1UI::BasePassageItem::dropEvent(QGraphicsSceneDragDropEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("DROP p_event") << m_id << this->text() << p_event->mimeData()->text() << p_event;
}

QString M1UI::BasePassageItem::highlight(M1Store::Item_lv2* p_chunk, M1Store::Item_lv2* p_category, M1Store::Item_lv2* p_color){
    qCDebug(g_cat_passages_panel) << QString("highlight") << m_id << this->text() << p_category->text() << p_color->text();

    m_color = p_color->text();
    this->update(this->boundingRect());

    return this->text();
}
QString M1UI::WordItem::highlight(M1Store::Item_lv2* p_chunk, M1Store::Item_lv2* p_category, M1Store::Item_lv2* p_color){
    QString l_ret = BasePassageItem::highlight(p_chunk, p_category, p_color);

    p_chunk->linkTo(this->m_occ->myself(), M1Store::OWNS_SIID, M1Store::InsertionPoint::at_bottom, M1Store::InsertionPoint::at_top);
    this->m_occ->myself()->linkTo(p_color, M1Store::HLCLR_SIID);

    return l_ret;
}

M1UI::WordItem::WordItem(const int p_id, std::shared_ptr<M1MidPlane::Interp> p_occ, PassageEditor *p_parent): BasePassageItem(p_id, p_parent){
    m_occ = p_occ;

    if(M1Store::Item_lv2* l_color_edge = m_occ->myself()->getFieldEdge(M1Store::HLCLR_SIID))
        m_color = l_color_edge->getTarget_lv2()->text();

    QString l_text = m_occ->baseText();
    this->setFont(cm_base_font);
    this->setText(l_text);

    qCDebug(g_cat_passages_panel) << QString("Find fields START") << l_text;
    M1Store::Item_lv2* l_pos_edge = nullptr;
    M1Store::Item_lv2* l_tag_edge = nullptr;
    QString l_grammar_attr;
    for(M1Store::Item_lv2_iterator it = m_occ->myself()->getIteratorSpecial(); !it.beyondEnd(); it.next()){
        if(it.at()->isOfType(M1Env::ISA_SIID) && it.at()->getTarget_lv2()->isOfType(M1Env::NLPOS_SIID)) l_pos_edge = it.at();
        else
        if(it.at()->isOfType(M1Env::ISA_SIID) && it.at()->getTarget_lv2()->isOfType(M1Env::NLTAG_SIID)) l_tag_edge = it.at();

        for (const M1Env::SpecialItemID& l_ssid_class : M1Env::GraphInit::cm_gram_attr_list) {
           if(it.at()->isOfType(M1Env::ISA_SIID) && it.at()->getTarget_lv2()->isOfType(l_ssid_class)){
                M1Store::Item_lv2* l_class_edge = it.at()->getTarget_lv2()->find_edge_generic(M1Env::ISA_SIID, M1Env::GRAMMAR_ATTR_SIID, true);
                QString l_grammar_class = l_class_edge != nullptr ? QString("%1").arg(l_class_edge->getTarget_lv2()->text()) : "";
                l_grammar_attr += QString("<p style=\"white-space:pre;margin:0;padding:0;\"><b>%1</b>: %2</p>").arg(l_grammar_class).arg(it.at()->getTarget_lv2()->text());
            }
        }
    }

    QString l_pos_txt = l_pos_edge != nullptr ? QString("<p style=\"white-space:pre;margin:0;padding:0;\"><b>POS</b>: %1</p>").arg(l_pos_edge->getTarget_lv2()->text()) : "";
    QString l_tag_txt = l_tag_edge != nullptr ? QString("<p style=\"white-space:pre;margin:0;padding:0;\"><b>TAG</b>: %1</p>").arg(l_tag_edge->getTarget_lv2()->text()) : "";

    M1Store::Item_lv2* l_lemma_edge = m_occ->myself()->getTarget_lv2()->find_edge_generic(M1Env::BLNGS_SIID, M1Env::LEMMA_SIID);
    QString l_lemma_txt = l_lemma_edge != nullptr ? QString("<p style=\"white-space:pre;margin:0;padding:0;\"><b>Lemma</b>: %1</p>").arg(l_lemma_edge->getTarget_lv2()->text()) : "";

    qCDebug(g_cat_passages_panel) << QString("Find fields END") << l_text;

    // qCDebug(g_cat_passages_panel) << QString("POS/TAG") << id() << l_pos_txt + l_tag_txt << M1Env::NLPOS_SIID << M1Env::NLTAG_SIID;

    this->setToolTip(QString("<p style=\"white-space:pre;margin:0;padding:0;\">Id: %1 %2</p>").arg(id()).arg(l_text) + l_pos_txt + l_tag_txt + l_lemma_txt + l_grammar_attr);
}

QFont M1UI::StephanusItem::StephanusItem::cm_font_stephanus = QFont("Noto Mono", 8);
M1UI::StephanusItem::StephanusItem(const int p_id, const QString& p_stephanus_number, PassageEditor *p_parent): BasePassageItem(p_id, p_parent){
    m_stephanus_number = p_stephanus_number;
    this->setBrush(Qt::darkRed);
    this->setFont(cm_font_stephanus);
    this->setText(m_stephanus_number);
    this->setToolTip(QString("Id: %1").arg(id()));
}

// PassageEditor ---------------------------------------------------------------------------------------------------------------------------------------
void M1UI::PassageEditor::move_forward(int p_steps){
    qCDebug(g_cat_passages_panel) << QString("move_forward()") << m_id << p_steps;
    /*
    if(m_current_start->next_item_id() != M1Env::G_VOID_ITEM_ID){
        for (M1UI::BasePassageItem *l_item : std::as_const(m_item_list)) delete l_item;
        m_item_list.clear();
        while(p_steps > 0 && m_current_start->next_item_id() != M1Env::G_VOID_ITEM_ID){
            m_current_start = m_current_start->get_next_lv2();
            p_steps--;
        }
        populate();
    }
    */
}
void M1UI::PassageEditor::move_backwards(int p_steps){
    qCDebug(g_cat_passages_panel) << QString("move_backwards()") << m_id << p_steps;
    /*
    if(m_current_start->previous_item_id() != M1Env::G_VOID_ITEM_ID){
        for (M1UI::BasePassageItem *l_item : std::as_const(m_item_list)) delete l_item;
        m_item_list.clear();
        while(p_steps > 0 && m_current_start->previous_item_id() != M1Env::G_VOID_ITEM_ID){
            m_current_start = m_current_start->get_previous_lv2();
            p_steps--;
        }
        populate();
    }
    */
}
QString M1UI::PassageEditor::bake_highlight(M1Store::Item_lv2* p_highlight_vertex, M1Store::Item_lv2* p_category, M1Store::Item_lv2* p_color){
    qCDebug(g_cat_tmp_spotlight).noquote() << QString("PassageEditor Id:") << m_id << m_version_vertex->text() << p_category->dbgShort();

    M1Store::Item_lv2* l_highlight_chunk = p_highlight_vertex->create_descendant(
        M1Store::OWNS_SIID,
        QString(m_version_vertex->text()) + " Chunk",
        M1Store::TEXT_HIGHLIGHT_CHUNK_SIID);
    l_highlight_chunk->setType(m_version_vertex->specialItemId());

    QStringList l_word_list;
    if(m_from_sel >= 0)
        for(int i = m_from_sel; i <= m_to_sel; i++) l_word_list.append(m_item_list.at(i)->highlight(l_highlight_chunk, p_category, p_color));
    this->unselect_all();
    /*
    qCDebug(g_cat_passages_panel).noquote() << QString("bake_highlight") << m_id << p_category->text() << p_color->text() << m_current_start->dbgShort();
    qCDebug(g_cat_passages_panel).noquote() << QString("version") << l_version->text() << m_from_sel << m_to_sel;


    QStringList l_word_list;
    if(m_from_sel >= 0)
        for(int i = m_from_sel; i <= m_to_sel; i++) l_word_list.append(m_item_list.at(i)->highlight(l_highlight_chunk, p_category, p_color));
    this->unselect_all();

    return M1Store::StorageStatic::maxLengthChop(l_word_list.join(" "), 36);
    */
    return M1Store::StorageStatic::maxLengthChop(l_word_list.join(" "), 36);
}
void M1UI::PassageEditor::populate(){
    qCDebug(g_cat_passages_panel) << QString("populate()") << m_id << m_occur_list.size();

    int l_id = 0;
    for(const std::shared_ptr<M1MidPlane::Interp> l_occ_interp: m_occur_list){
        QString l_text_plus = l_occ_interp->baseTextPlus();
        if(g_re_stephanus.match(l_text_plus).hasMatch()){
            QString l_steph_code = g_re_stephanus.match(l_text_plus).captured(1);
            M1UI::StephanusItem* l_steph_number_item = new M1UI::StephanusItem(l_id++, l_steph_code, this);
            m_item_list.append(l_steph_number_item);
            qCDebug(g_cat_passages_panel) << QString("StephanusItem added") << m_id;
        }
        M1UI::WordItem* l_item = new M1UI::WordItem(l_id++, l_occ_interp, this);
        m_item_list.append(l_item);
        qCDebug(g_cat_passages_panel) << QString("WordItem added") << m_id;
    }
}

QRectF M1UI::PassageEditor::do_layout(const QRectF& p_rect){
    qCDebug(g_cat_passages_panel) << QString("do_layout p_rect:") << m_id << p_rect;
    int l_init_x = m_margin_pe;
    int l_init_y = m_margin_pe;
    int l_x = l_init_x;
    int l_y = l_init_y;

    int max_x = 0;
    int max_y = 0;
    for(BasePassageItem *l_item : std::as_const(m_item_list)){
        QSizeF l_size_hint = l_item->sizeHint(Qt::SizeHint::MinimumSize);
        qCDebug(g_cat_passages_panel) << QString("SizeHint:") << l_item->text() << l_size_hint;

        l_item->set_end_line(false);
        int l_x_right = l_x + l_size_hint.width();
        if( l_x_right > p_rect.width() - m_margin_pe){
            m_item_list.at(l_item->id() - 1)->set_end_line(true);
            l_x = l_init_x;
            l_y += l_size_hint.height() + m_spacing;
        }

        if(l_x_right > max_x) max_x = l_x_right;
        if(l_y + l_size_hint.height() > max_y) max_y = l_y + l_size_hint.height();

        l_item->QGraphicsSimpleTextItem::setPos(l_x, l_y);
        l_item->set_start_line(l_x == l_init_x);
        qCDebug(g_cat_passages_panel) << QString("item rec: (%1, %2) (%3, %4)")
                                         .arg(l_x).arg(l_y).arg(l_x + l_size_hint.width()-1).arg(l_y + l_size_hint.height()-1);

        l_x += l_size_hint.width() + m_spacing;
    }

    QRectF l_outer_geometry = QRectF(p_rect.topLeft(), QSizeF(p_rect.size().width(), max_y - l_init_y + 2*m_margin_pe));
    qCDebug(g_cat_passages_panel) << QString("do_layout m_outer_geometry:") << m_id << l_outer_geometry;
    m_editor_size = l_outer_geometry.size();
    return l_outer_geometry;
}

M1UI::PassageEditor::PassageEditor(const QString& p_version_name, std::vector<std::shared_ptr<M1MidPlane::Interp>>& p_occur_list,
                                   const QString& p_id,
                                   QGraphicsItem *p_parent): QGraphicsObject(p_parent), m_occur_list(p_occur_list){

    qCDebug(g_cat_tmp_spotlight) << QString("PassageEditor") << p_id << p_occur_list.size();
    m_id = p_id;
    // m_current_start = p_occur_start;
    m_panel = static_cast<PassagesPanel*>(p_parent);
    M1Store::Item_lv2* l_edge_2_sentence = p_occur_list[0]->myself()->find_edge_generic(M1Env::TW_OCC_2_CHUNK_BEGIN_SIID, M1Env::TEXT_SENTENCE_SIID);
    M1Store::Item_lv2* l_edge_2_book = l_edge_2_sentence->getTarget_lv2()->find_edge_generic(M1Env::BLNGS_SIID, M1Env::TEXT_BOOK_SIID);
    M1Store::Item_lv2* l_edge_2_version = l_edge_2_book->getTarget_lv2()->find_edge_generic(M1Env::BLNGS_SIID, M1Env::TXTVR_SIID);
    m_version_vertex = l_edge_2_version->getTarget_lv2();
    populate();

    qCDebug(g_cat_passages_panel) << QString("Count:") << m_id << m_item_list.count();
}

QRectF M1UI::PassageEditor::boundingRect() const{
    QRectF l_br(QPoint(0, 0), m_editor_size);
    //qCDebug(g_cat_passages_panel) << QString("PassageEditor boundingRect()") << m_id << l_br.topLeft() << l_br.bottomRight();
    return l_br;
}

void M1UI::PassageEditor::mousePressEvent(QGraphicsSceneMouseEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("mouse press") << m_id << p_event;
    if(p_event->button() == Qt::LeftButton) unselect_all();
}

void M1UI::PassageEditor::unselect_all(){
    for(int i=0; i<m_item_list.count(); i++)
        m_item_list.at(i)->reset_selection_flags();
    m_from_sel = -1;
    m_to_sel = -1;
    m_panel->selection_changed();
    this->update(boundingRect());
}
void M1UI::PassageEditor::paint(QPainter *p_painter,
                   const QStyleOptionGraphicsItem *p_option,
                   QWidget *p_widget){

    QRectF l_outline(QPoint(0, 0), m_editor_size);
    // p_painter->setPen(Qt::blue);
    // p_painter->drawRect(l_outline);
    qCDebug(g_cat_passages_panel) << QString("PassageEditor l_outline") << m_id << l_outline.topLeft() << l_outline.bottomRight();
}
QVariant M1UI::PassageEditor::itemChange(GraphicsItemChange p_change, const QVariant &p_value){
    qCDebug(g_cat_passages_panel) << QString("PassageEditor itemChange:") << m_id << p_change << p_value;
    QVariant l_ret = QGraphicsItem::itemChange(p_change, p_value);
    qCDebug(g_cat_passages_panel) << QString("PassageEditor itemChange Done") << m_id;
    return l_ret;
}
bool M1UI::PassageEditor::sceneEvent(QEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("PassageEditor sceneEvent:") << m_id << p_event;
    return QGraphicsItem::sceneEvent(p_event);
}
QRectF M1UI::PassageEditor::get_outer_rect(const QPointF& p_top_left, int p_editor_width){
    qCDebug(g_cat_passages_panel) << QString("PassageEditor get_outer_rect:") << p_editor_width;
    prepareGeometryChange();
    QRectF l_outer_geometry = do_layout(QRectF(p_top_left, QSize(p_editor_width, 0)));
    setPos(p_top_left);
    qCDebug(g_cat_passages_panel) << QString("PassageEditor get_outer_rect:") << m_id << l_outer_geometry;
    return l_outer_geometry;
}
void M1UI::PassageEditor::select_from_to(const int p_from, const int p_to){
    qCDebug(g_cat_passages_panel) << QString("PassageEditor select_from_to p_from, p_to:") << m_id << p_from << p_to;
    if(!(m_from_sel == p_from && m_to_sel == p_to)){
        this->unselect_all();
        for(int i=p_from; i<=p_to; i++){
            m_item_list.at(i)->reset_selection_flags();
            m_item_list.at(i)->set_selection();
            if(i==p_from) m_item_list.at(i)->set_start_selection();
            if(i==p_to) m_item_list.at(i)->set_end_selection();
        }
        this->update(boundingRect());
        m_from_sel = p_from;
        m_to_sel = p_to;

        m_panel->selection_changed();
    }
}

// PassagesPanel ---------------------------------------------------------------------------------------------------------------------------------------
void M1UI::PassagesPanel::selection_changed(){
    qCDebug(g_cat_passages_panel) << QString("selection_changed()");
    bool l_all_selections = true;
    for(int i = 0; i < m_pe_list.count(); i++){
        qCDebug(g_cat_passages_panel) << QString("PassageEditor:") << m_pe_list.at(i)->id() << m_pe_list.at(i)->has_selection();
        l_all_selections = l_all_selections && m_pe_list.at(i)->has_selection();
    }
    qCDebug(g_cat_passages_panel) << QString("All Selected?: ") << l_all_selections;
    emit activate_highlight_button(l_all_selections);
}

QRectF M1UI::PassagesPanel::boundingRect() const {
    QRectF l_br(QPoint(0, 0), m_size_panel);
    //qCDebug(g_cat_passages_panel) << QString("PassagesPanel boundingRect()") << l_br.topLeft() << l_br.bottomRight();
    return l_br;
}
void M1UI::PassagesPanel::paint(QPainter *p_painter,
                   const QStyleOptionGraphicsItem *p_option,
                   QWidget *p_widget) {

    QRectF l_outline(QPoint(0, 0), m_size_panel);
    // p_painter->setPen(Qt::GlobalColor::darkYellow);
    // p_painter->drawRect(l_outline);
    qCDebug(g_cat_passages_panel) << QString("PassagesPanel l_outline") << l_outline.topLeft() << l_outline.bottomRight();
}
void M1UI::PassagesPanel::calculate_positions(){
    QRectF l_inner_rect(QPointF(m_margin_pp, m_margin_pp), QSizeF(m_view_width - 2*m_margin_pp, 0));
    for(int i = 0; i < m_pe_list.count(); i++){
        PassageEditor* l_pe = m_pe_list.at(i);
        QRectF l_rect = l_pe->get_outer_rect(l_inner_rect.bottomLeft(), l_inner_rect.width());
        qCDebug(g_cat_passages_panel) << QString("calculate_positions pe:") << l_pe->id() << "--> outer rect:" << l_rect;
        l_inner_rect = l_inner_rect.united(l_rect);
        l_inner_rect += QMarginsF(0, 0, 0, m_margin_pp);
    }
    qCDebug(g_cat_passages_panel) << QString("PassagesPanel calculate_positions() l_inner_rect:") << l_inner_rect;
    m_size_panel = (l_inner_rect + QMarginsF(m_margin_pp, m_margin_pp, m_margin_pp, 0)).size();
}
QSizeF M1UI::PassagesPanel::set_panel_width(const QPointF& p_top_left, int p_panel_width){
    qCDebug(g_cat_passages_panel) << QString("PassagesPanel calculate_positions() set_view_width:") << p_panel_width;
    m_view_width = p_panel_width;

    prepareGeometryChange();
    calculate_positions();
    setPos(p_top_left);
    qCDebug(g_cat_passages_panel) << QString("PassagesPanel set_panel_width m_size_panel:") << m_size_panel;
    return m_size_panel;
}
// slots
void M1UI::PassagesPanel::move_forward_one(){
    qCDebug(g_cat_passages_panel) << QString("move_forward_one()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_forward(1);
    prepareGeometryChange();
    calculate_positions();
}
void M1UI::PassagesPanel::move_backwards_one(){
    qCDebug(g_cat_passages_panel) << QString("move_backwards_one()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_backwards(1);
    prepareGeometryChange();
    calculate_positions();
}
void M1UI::PassagesPanel::move_forward_ten(){
    qCDebug(g_cat_passages_panel) << QString("move_forward_ten()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_forward(10);
    prepareGeometryChange();
    calculate_positions();
}
void M1UI::PassagesPanel::move_backwards_ten(){
    qCDebug(g_cat_passages_panel) << QString("move_backwards_ten()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_backwards(10);
    prepareGeometryChange();
    calculate_positions();
}
void M1UI::PassagesPanel::cat_select(int p_index){
    qCDebug(g_cat_passages_panel) << QString("cat_select()") << p_index << m_cat_list[p_index]->text();
    m_current_cat = p_index;
}
// highlight
void M1UI::PassagesPanel::highlight(){
    qCDebug(g_cat_tmp_spotlight()) << QString("highlight()") << m_cat_list[m_current_cat]->text() << m_highlight_folder->dbgShort();

    M1Store::Item_lv2* l_highlight_vertex = m_highlight_folder->create_descendant(M1Env::OWNS_SIID, "", M1Env::TEXT_HIGHLIGHT_SIID);

    M1Store::Item_lv2* l_category = m_cat_list[m_current_cat];
    qCDebug(g_cat_tmp_spotlight) << QString("l_category") << l_category->specialItemId() << l_category->dbgShort();
    l_highlight_vertex->setType(l_category->specialItemId());
    M1Store::Item_lv2* l_color = l_category->getFieldEdge(M1Store::HLCLR_SIID)->getTarget_lv2();

    QStringList l_ret_list;
    for(int i = 0; i < m_pe_list.count(); i++) l_ret_list.append(m_pe_list.at(i)->bake_highlight(l_highlight_vertex, l_category, l_color));

    l_highlight_vertex->setText_lv1(l_ret_list.join("/"));
}

// Scene & View ---------------------------------------------------------------------------------------------------------------------------------------
bool M1UI::Scene::event(QEvent *p_event){
    if(p_event->type() != QEvent::GraphicsSceneDragMove)
        qCDebug(g_cat_passages_panel) << QString("Scene event:") << p_event;
    return QGraphicsScene::event(p_event);
}

M1UI::View::View(QGraphicsScene *p_scene):QGraphicsView(p_scene){
    this->setAcceptDrops(true);
}
bool M1UI::View::event(QEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("View event:") << p_event;
    return QGraphicsView::event(p_event);
}
void M1UI::View::paintEvent(QPaintEvent *p_event){
    QRectF l_scene_rect = scene()->sceneRect();
    l_scene_rect = QRectF(l_scene_rect.topLeft(), QSize(l_scene_rect.size().width()-1, l_scene_rect.size().height()-1));
    qCDebug(g_cat_passages_panel) << QString("View paintEvent l_scene_rect") << l_scene_rect.topLeft() << l_scene_rect.bottomRight();

    QGraphicsView::paintEvent(p_event);
}
void M1UI::View::resizeEvent(QResizeEvent *p_event){
    qCDebug(g_cat_passages_panel) << QString("View resizeEvent p_event:") << p_event;

    qCDebug(g_cat_passages_panel) << QString("View resizeEvent START") << "######################################### before set_panel_width";
    QSizeF l_panel_size = m_outer_panel->set_panel_width(QPointF(m_margin_view, m_margin_view), p_event->size().width() - 2*m_margin_view);
    qCDebug(g_cat_passages_panel) << QString("View resizeEvent") << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% after set_panel_width";

    int l_panel_height = l_panel_size.height() + 2*m_margin_view;
    QRectF l_scene_rect = QRectF(QPoint(0, 0), QSizeF(p_event->size().width(), l_panel_height > p_event->size().height() ? l_panel_height : p_event->size().height()));
    scene()->setSceneRect(l_scene_rect);
    qCDebug(g_cat_passages_panel) << QString("View resizeEvent l_scene_rect:") << l_scene_rect;

    QGraphicsView::resizeEvent(p_event);
    qCDebug(g_cat_passages_panel) << QString("View resizeEvent m_outer_panel->rect():") << p_event << m_outer_panel->boundingRect() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ END";
}
