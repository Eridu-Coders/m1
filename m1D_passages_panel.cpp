#include "m1A_constants.h"
#include "m1B_graph_init.h"
#include "m1C_interp.h"
#include "m1D_passages_panel.h"

#include <QResizeEvent>

QSizeF M1UI::WordItem::sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint) const {
    QSizeF l_ret = QGraphicsSimpleTextItem::boundingRect().size();
    qCDebug(g_cat_td_signals) << QString("WordItem::sizeHint [") << p_which << p_constraint << QString("] %1 -->").arg(this->text()) << l_ret;
    return l_ret;
}
void M1UI::WordItem::setGeometry(const QRectF &p_rect){
    QGraphicsSimpleTextItem::setPos(p_rect.topLeft());
    qCDebug(g_cat_td_signals) << QString("WordItem::setGeometry") << p_rect << this->text();
}
void M1UI::WordItem::paint(QPainter *p_painter,
                   const QStyleOptionGraphicsItem *p_option,
                   QWidget *p_widget){
    QGraphicsSimpleTextItem::paint(p_painter, p_option, p_widget);
    QRectF l_br = boundingRect();
    // p_painter->drawRect(l_br);
    qCDebug(g_cat_td_signals) << QString("WordItem paint") << this->text() << l_br.topLeft() << l_br.bottomRight() << l_br;
}

void M1UI::StephanusItem::paint(QPainter *p_painter,
                   const QStyleOptionGraphicsItem *p_option,
                   QWidget *p_widget){

    WordItem::paint(p_painter, p_option, p_widget);
    p_painter->setPen(Qt::red);
    p_painter->drawRect(boundingRect());
}

void M1UI::PassageEditor::move_forward(int p_steps){
    qCDebug(g_cat_td_signals) << QString("move_forward()") << m_id << p_steps;
    if(m_current_start->next_item_id() != M1Env::G_VOID_ITEM_ID){
        for (M1UI::WordItem *l_item : std::as_const(m_item_list)) delete l_item;
        m_item_list.clear();
        while(p_steps > 0 && m_current_start->next_item_id() != M1Env::G_VOID_ITEM_ID){
            m_current_start = m_current_start->get_next_lv2();
            p_steps--;
        }
        populate();
    }
}
void M1UI::PassageEditor::move_backwards(int p_steps){
    qCDebug(g_cat_td_signals) << QString("move_backwards()") << m_id << p_steps;
    if(m_current_start->previous_item_id() != M1Env::G_VOID_ITEM_ID){
        for (M1UI::WordItem *l_item : std::as_const(m_item_list)) delete l_item;
        m_item_list.clear();
        while(p_steps > 0 && m_current_start->previous_item_id() != M1Env::G_VOID_ITEM_ID){
            m_current_start = m_current_start->get_previous_lv2();
            p_steps--;
        }
        populate();
    }
}
void M1UI::PassageEditor::populate(){
    qCDebug(g_cat_td_signals) << QString("populate()") << m_id << m_current_start->text();

    if(m_current_start != nullptr){
        M1Store::Item_lv2* l_cur_occur = m_current_start;
        for(int l_count_words = 0; l_count_words < 100 && l_cur_occur->next_item_id() != M1Env::G_VOID_ITEM_ID; l_count_words++){
            qCDebug(g_cat_td_signals) << QString("Current edge") << m_id << l_cur_occur->dbgShort();
            if(l_cur_occur->isFullEdge() && l_cur_occur->isOfType(M1Env::OCCUR_SIID)){
                QString l_text = M1MidPlane::SentenceInterp::occur_to_text(l_cur_occur);
                qCDebug(g_cat_td_signals) << QString("Adding word") << m_id << l_text;
                if(M1Store::Item_lv2* l_section = l_cur_occur->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID); l_section != nullptr){
                    // StephanusItem
                    M1UI::StephanusItem* l_steph_number_item = new M1UI::StephanusItem(l_section->getTarget_lv2()->text(), this);
                    l_steph_number_item->setFont(cm_font);
                    m_item_list.append(l_steph_number_item);
                }
                M1UI::WordItem* l_item = new M1UI::WordItem(l_text, this);
                l_item->setFont(cm_font);
                m_item_list.append(l_item);
            }

            l_cur_occur = l_cur_occur->get_next_lv2();
        }
    }
}
QRectF M1UI::PassageEditor::do_layout(const QRectF& p_rect){
    qCDebug(g_cat_td_signals) << QString("do_layout p_rect:") << m_id << p_rect;
    int l_init_x = m_margin_pe;
    int l_init_y = m_margin_pe;
    int l_x = l_init_x;
    int l_y = l_init_y;

    int max_x = 0;
    int max_y = 0;
    for(WordItem *l_item : std::as_const(m_item_list)){
        QSizeF l_size_hint = l_item->sizeHint(Qt::SizeHint::MinimumSize);
        qCDebug(g_cat_td_signals) << QString("SizeHint:") << l_item->text() << l_size_hint;

        int l_x_right = l_x + l_size_hint.width();
        if( l_x_right > p_rect.width() - m_margin_pe){
            l_x = l_init_x;
            l_y += l_size_hint.height() + m_spacing;
        }

        if(l_x_right > max_x) max_x = l_x_right;
        if(l_y + l_size_hint.height() > max_y) max_y = l_y + l_size_hint.height();

        l_item->setPos(l_x, l_y);
        qCDebug(g_cat_td_signals) << QString("item rec: (%1, %2) (%3, %4)")
                                         .arg(l_x).arg(l_y).arg(l_x + l_size_hint.width()-1).arg(l_y + l_size_hint.height()-1);

        l_x += l_size_hint.width() + m_spacing;
    }

    QRectF l_outer_geometry = QRectF(p_rect.topLeft(), QSizeF(p_rect.size().width(), max_y - l_init_y + 2*m_margin_pe));
    qCDebug(g_cat_td_signals) << QString("do_layout m_outer_geometry:") << m_id << l_outer_geometry;
    m_editor_size = l_outer_geometry.size();
    return l_outer_geometry;
}
/*
PassageEditor(M1Store::Item_lv2* p_sentence, const QString& p_id, QGraphicsWidget *p_parent): QGraphicsWidget(p_parent){
    qCDebug(g_cat_td_signals) << QString("PassageEditor") << p_id << p_sentence->text();
    m_editor_layout = new FlowLayout(p_id, this);
    this->setLayout(m_editor_layout);
    m_id = p_id;
    static_cast<QGraphicsLinearLayout*>(p_parent->layout())->addItem(this);

    //M1Store::Item_lv2* l_cur_occur = p_occur_start;
    for(M1Store::Item_lv2_iterator it = p_sentence->getIteratorTop(); !it.beyondEnd(); it.next()) {
        qCDebug(g_cat_td_signals) << QString("Current edge") << m_id << it.at()->dbgShort();
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID)){
            QString l_text = toText(it.at()->getTarget_lv2());
            qCDebug(g_cat_td_signals) << QString("Adding word") << m_id << l_text;
            if(M1Store::Item_lv2* l_section = it.at()->getTarget_lv2()->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID); l_section != nullptr){
                // StephanusItem
                M1UI::StephanusItem* l_steph_number_item = new M1UI::StephanusItem(l_section->getTarget_lv2()->text(), this);
                l_steph_number_item->setFont(cm_font);
                m_editor_layout->addItem(l_steph_number_item);
            }
            M1UI::WordItem* l_item = new M1UI::WordItem(l_text, this);
            l_item->setFont(cm_font);
            m_editor_layout->addItem(l_item);
        }
    }

    qCDebug(g_cat_td_signals) << QString("Count:") << m_id << m_editor_layout->count();
}
*/
// PassageEditor(M1Store::Item_lv2* p_sentence, const QString& p_id, QGraphicsWidget *p_parent): QGraphicsWidget(p_parent){
M1UI::PassageEditor::PassageEditor(M1Store::Item_lv2* p_occur_start, const QString& p_id, QGraphicsItem *p_parent): QGraphicsObject(p_parent){
    qCDebug(g_cat_td_signals) << QString("PassageEditor") << p_id << p_occur_start->text();
    m_id = p_id;
    m_current_start = p_occur_start;

    populate();

    qCDebug(g_cat_td_signals) << QString("Count:") << m_id << m_item_list.count();
}
QRectF M1UI::PassageEditor::boundingRect() const{
    QRectF l_br(QPoint(0, 0), m_editor_size);
    qCDebug(g_cat_td_signals) << QString("PassageEditor boundingRect()") << m_id << l_br.topLeft() << l_br.bottomRight();
    return l_br;
}
void M1UI::PassageEditor::paint(QPainter *p_painter,
                   const QStyleOptionGraphicsItem *p_option,
                   QWidget *p_widget){

    QRectF l_outline(QPoint(0, 0), m_editor_size);
    p_painter->setPen(Qt::blue);
    p_painter->drawRect(l_outline);
    qCDebug(g_cat_td_signals) << QString("PassageEditor l_outline") << m_id << l_outline.topLeft() << l_outline.bottomRight();
}
QVariant M1UI::PassageEditor::itemChange(GraphicsItemChange p_change, const QVariant &p_value){
    qCDebug(g_cat_td_signals) << QString("PassageEditor itemChange:") << m_id << p_change << p_value;
    return QGraphicsItem::itemChange(p_change, p_value);
}
bool M1UI::PassageEditor::sceneEvent(QEvent *p_event){
    qCDebug(g_cat_td_signals) << QString("PassageEditor sceneEvent:") << m_id << p_event;
    return QGraphicsItem::sceneEvent(p_event);
}
QRectF M1UI::PassageEditor::get_outer_rect(const QPointF& p_top_left, int p_editor_width){
    qCDebug(g_cat_td_signals) << QString("PassageEditor get_outer_rect:") << p_editor_width;
    prepareGeometryChange();
    QRectF l_outer_geometry = do_layout(QRectF(p_top_left, QSize(p_editor_width, 0)));
    setPos(p_top_left);
    qCDebug(g_cat_td_signals) << QString("PassageEditor get_outer_rect:") << m_id << l_outer_geometry;
    return l_outer_geometry;
}

QFont M1UI::PassageEditor::PassageEditor::cm_font = QFont("Noto Mono", 12);

QRectF M1UI::PassagesPanel::boundingRect() const {
    QRectF l_br(QPoint(0, 0), m_size_panel);
    qCDebug(g_cat_td_signals) << QString("PassagesPanel boundingRect()") << l_br.topLeft() << l_br.bottomRight();
    return l_br;
}
void M1UI::PassagesPanel::paint(QPainter *p_painter,
                   const QStyleOptionGraphicsItem *p_option,
                   QWidget *p_widget) {

    QRectF l_outline(QPoint(0, 0), m_size_panel);
    p_painter->setPen(Qt::GlobalColor::darkYellow);
    p_painter->drawRect(l_outline);
    qCDebug(g_cat_td_signals) << QString("PassagesPanel l_outline") << l_outline.topLeft() << l_outline.bottomRight();
}
void M1UI::PassagesPanel::calculate_positions(){
    QRectF l_inner_rect(QPointF(m_margin_pp, m_margin_pp), QSizeF(m_view_width - 2*m_margin_pp, 0));
    for(int i = 0; i < m_pe_list.count(); i++){
        PassageEditor* l_pe = m_pe_list.at(i);
        QRectF l_rect = l_pe->get_outer_rect(l_inner_rect.bottomLeft(), l_inner_rect.width());
        qCDebug(g_cat_td_signals) << QString("calculate_positions pe:") << l_pe->id() << "--> outer rect:" << l_rect;
        l_inner_rect = l_inner_rect.united(l_rect);
        l_inner_rect += QMarginsF(0, 0, 0, m_margin_pp);
    }
    qCDebug(g_cat_td_signals) << QString("PassagesPanel calculate_positions() l_inner_rect:") << l_inner_rect;
    m_size_panel = (l_inner_rect + QMarginsF(m_margin_pp, m_margin_pp, m_margin_pp, 0)).size();
}
QSizeF M1UI::PassagesPanel::set_panel_width(const QPointF& p_top_left, int p_panel_width){
    qCDebug(g_cat_td_signals) << QString("PassagesPanel calculate_positions() set_view_width:") << p_panel_width;
    m_view_width = p_panel_width;
    /*
    QRectF l_inner_rect(QPointF(m_margin_pp, m_margin_pp), QSizeF(m_view_width - 2*m_margin_pp, 0));
    for(int i = 0; i < m_pe_list.count(); i++){
        PassageEditor* l_pe = m_pe_list.at(i);
        QRectF l_rect = l_pe->get_outer_rect(l_inner_rect.bottomLeft(), l_inner_rect.width());
        qCDebug(g_cat_td_signals) << QString("PassagesPanel pe:") << l_pe->id() << "--> outer rect:" << l_rect;
        l_inner_rect = l_inner_rect.united(l_rect);
        l_inner_rect += QMarginsF(0, 0, 0, m_margin_pp);
    }
    qCDebug(g_cat_td_signals) << QString("PassagesPanel get_outer_rect l_inner_geometry:") << l_inner_rect;
    prepareGeometryChange();
    m_size_panel = (l_inner_rect + QMarginsF(m_margin_pp, m_margin_pp, m_margin_pp, 0)).size();
    */
    prepareGeometryChange();
    calculate_positions();
    setPos(p_top_left);
    qCDebug(g_cat_td_signals) << QString("PassagesPanel set_panel_width m_size_panel:") << m_size_panel;
    return m_size_panel;
}
// slots
void M1UI::PassagesPanel::move_forward_one(){
    qCDebug(g_cat_td_signals) << QString("move_forward_one()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_forward(1);
    prepareGeometryChange();
    calculate_positions();
}
void M1UI::PassagesPanel::move_backwards_one(){
    qCDebug(g_cat_td_signals) << QString("move_backwards_one()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_backwards(1);
    prepareGeometryChange();
    calculate_positions();
}
void M1UI::PassagesPanel::move_forward_ten(){
    qCDebug(g_cat_td_signals) << QString("move_forward_ten()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_forward(10);
    prepareGeometryChange();
    calculate_positions();
}
void M1UI::PassagesPanel::move_backwards_ten(){
    qCDebug(g_cat_td_signals) << QString("move_backwards_ten()");
    for(int i = 0; i < m_pe_list.count(); i++) m_pe_list.at(i)->move_backwards(10);
    prepareGeometryChange();
    calculate_positions();
}

bool M1UI::Scene::event(QEvent *p_event){
    qCDebug(g_cat_td_signals) << QString("Scene event:") << p_event;
    return QGraphicsScene::event(p_event);
}

bool M1UI::View::event(QEvent *p_event){
    qCDebug(g_cat_td_signals) << QString("View event:") << p_event;
    return QGraphicsView::event(p_event);
}
void M1UI::View::paintEvent(QPaintEvent *p_event){
    QRectF l_scene_rect = scene()->sceneRect();
    l_scene_rect = QRectF(l_scene_rect.topLeft(), QSize(l_scene_rect.size().width()-1, l_scene_rect.size().height()-1));
    qCDebug(g_cat_td_signals) << QString("View paintEvent l_scene_rect") << l_scene_rect.topLeft() << l_scene_rect.bottomRight();

    QGraphicsView::paintEvent(p_event);
}
void M1UI::View::resizeEvent(QResizeEvent *p_event){
    qCDebug(g_cat_td_signals) << QString("View resizeEvent p_event:") << p_event;

    qCDebug(g_cat_td_signals) << QString("View resizeEvent START") << "######################################### before set_panel_width";
    QSizeF l_panel_size = m_outer_panel->set_panel_width(QPointF(m_margin_view, m_margin_view), p_event->size().width() - 2*m_margin_view);
    qCDebug(g_cat_td_signals) << QString("View resizeEvent") << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% after set_panel_width";

    int l_panel_height = l_panel_size.height() + 2*m_margin_view;
    QRectF l_scene_rect = QRectF(QPoint(0, 0), QSizeF(p_event->size().width(), l_panel_height > p_event->size().height() ? l_panel_height : p_event->size().height()));
    scene()->setSceneRect(l_scene_rect);
    qCDebug(g_cat_td_signals) << QString("View resizeEvent l_scene_rect:") << l_scene_rect;

    QGraphicsView::resizeEvent(p_event);
    qCDebug(g_cat_td_signals) << QString("View resizeEvent m_outer_panel->rect():") << p_event << m_outer_panel->boundingRect() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ END";
}
