#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1D_tree_display.h"
// #include "m1D_passages_panel.h"

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

// g_cat_interp_dev g_cat_tree_row
// Q_LOGGING_CATEGORY(g_cat_interp_dev, "interp.dev") q_re_space

QIcon M1UI::TreeRow::cm_open;
QIcon M1UI::TreeRow::cm_closed;
std::vector<M1UI::TreeRow*> M1UI::TreeRow::cm_row_list;

/**
 * @brief M1UI::TreeRow::init
 */
void M1UI::TreeRow::init(){
    M1_FUNC_ENTRY(g_cat_tree_row, QString("Init TreeRow class members"))

    cm_open = QIcon(M1Env::OPEN_ICON_PATH);
    Q_ASSERT_X( !cm_open.isNull(), "TreeRow::init()", "Open link Icon failed to load");
    cm_closed = QIcon(M1Env::CLOSED_ICON_PATH);
    Q_ASSERT_X( !cm_closed.isNull(), "TreeRow::init()", "Closed link Icon failed to load");

    M1_FUNC_EXIT
}

/**
 * @brief M1UI::TreeRow::TreeRow
 * @param p_edge
 * @param p_tree
 * @param p_depth
 */
M1UI::TreeRow::TreeRow(M1Store::Item_lv2* p_edge, M1UI::TreeDisplay* p_tree, int p_depth) : QWidget(p_tree->scroll_area_widget()){
    M1_FUNC_ENTRY(g_cat_tree_row, QString("TreeRow Constructor from: %1").arg(p_edge->dbgShort()))

    Q_ASSERT_X(p_edge->isFullEdge() || p_edge->isSimpleEdge(),
               "M1MidPlane::TreeRow::TreeRow()",
               "A TreeRow can obnly be created out of an edge");

    if(p_edge->isFullEdge()){
        // nearly all cases, including simple vertex field
        // if(p_edge->isOfType(M1Env::AUTO_SIID)) m_target = std::shared_ptr<M1MidPlane::Interp>(new M1MidPlane::AutoInterp(p_edge->getTarget_lv2()));
        // else m_target = M1MidPlane::Interp::getInterp(p_edge->getTarget_lv2());
        m_target = M1MidPlane::Interp::getInterp(p_edge->getTarget_lv2());
    }
    else m_target = M1MidPlane::Interp::getInterp(p_edge); // simple edge field case only

    m_target->setParent(this);
    cm_row_list.push_back(this);

    m_edge = p_edge;

    p_tree->tree_vb_layout()->addWidget(this);
    qCDebug(g_cat_tree_row) << QString("TreeRow added. Layout count: %1 Children count %2")
                                   .arg(p_tree->tree_vb_layout()->count())
                                   .arg(p_tree->widget()->children().count());

    m_td_parent = p_tree;

    qCDebug(g_cat_tree_row) << "baseSize()  :" << this->baseSize();
    qCDebug(g_cat_tree_row) << "rect()      :" << this->rect();
    qCDebug(g_cat_tree_row) << "font height : " << this->fontMetrics().height();
    qCDebug(g_cat_tree_row) << "font family : " << this->fontInfo().family();

    m_depth = p_depth;

    m_target_height = (this->fontMetrics().height() * 1300) / 1000;
    m_target_baseline = (m_target_height * 800) / 1000;
    m_target_padding = (m_target_height - this->fontMetrics().height())/2;
    m_icon_size = this->fontMetrics().height();
    m_oc_x = m_target_padding + (p_depth + 1)*m_target_height;

    this->setMinimumHeight(m_target_height);
    this->setMaximumHeight(m_target_height);
    this->setFocusPolicy(Qt::StrongFocus);
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(Qt::darkRed));
    this->setPalette(p);
    this->setBackgroundRole(QPalette::Window);

    // qCDebug(g_cat_interp_drag) << "Accepting Drops" << m_myself->dbgShort();
    // this->setMouseTracking(true);
    this->setAcceptDrops(true);

    this->setAttribute(Qt::WA_AcceptDrops, false);
    this->setAttribute(Qt::WA_AcceptDrops, true);

    // QTimer m_hold_timer initialized automatically during instance construction because it is a full member (not just a pointer)
    // timer signal after HOLD_DELAY ms
    connect(&m_hold_timer, &QTimer::timeout, this, &M1UI::TreeRow::handleMouseHold);

    M1_FUNC_EXIT
}

void M1UI::TreeRow::copyFrom(const M1UI::TreeRow& p_to_copy){
    m_depth = p_to_copy.m_depth;
    m_target_height = p_to_copy.m_target_height;
    m_target_baseline = p_to_copy.m_target_baseline;
    m_target_padding = p_to_copy.m_target_padding;
    m_icon_size = p_to_copy.m_icon_size;
    m_oc_x = p_to_copy.m_oc_x;

    m_td_parent = p_to_copy.m_td_parent;

    m_drag_top = p_to_copy.m_drag_top;
    m_drag_bottom = p_to_copy.m_drag_bottom;
    m_block_emit = p_to_copy.m_block_emit;

    // m_hold_timer ;

    m_edge = p_to_copy.m_edge;
    m_html_cache = p_to_copy.m_html_cache;
    m_target = p_to_copy.m_target;
}

M1UI::TreeRow::TreeRow(const M1UI::TreeRow& p_to_copy){
    qCDebug(g_cat_tree_row()) << QString("Copy Constructor - Use Count: %1").arg(p_to_copy.m_target.use_count()) << this->dbgOneLiner();
    copyFrom(p_to_copy);
}

M1UI::TreeRow& M1UI::TreeRow::operator=(M1UI::TreeRow p_to_copy){
    qCDebug(g_cat_tree_row()) << QString("Assignment - No Ref - Use Count: %1").arg(p_to_copy.m_target.use_count()) << this->dbgOneLiner();
    copyFrom(p_to_copy);
    return *this;
}
M1UI::TreeRow& M1UI::TreeRow::operator=(const M1UI::TreeRow& p_to_copy){
    qCDebug(g_cat_tree_row()) << QString("Assignment - With Ref - Use Count: %1").arg(p_to_copy.m_target.use_count()) << this->dbgOneLiner();
    copyFrom(p_to_copy);
    return *this;
}
M1UI::TreeRow::TreeRow(){
    qCDebug(g_cat_tree_row()) << "Empty TreeRow Constructed";
}

/**
 * @brief M1UI::TreeRow::~TreeRow
 */
M1UI::TreeRow::~TreeRow(){
    if(!m_freeze_debug){ // to avoid issuing debug messages after debug system has been shut down
        if(m_target == nullptr)
            qCDebug(g_cat_tree_row()) << "Warning: m_target NULL";
        else
            qCDebug(g_cat_tree_row()) << QString("Deleting - Use Count: %1").arg(m_target.use_count()) << this->dbgOneLiner();
    }
}

/**
 * @brief M1UI::TreeRow::deParentTarget
 */
void M1UI::TreeRow::deParentTarget(){
    if(m_target != nullptr){
        m_target->setParent(nullptr);
        m_target = nullptr;
    }
    else qCDebug(g_cat_tmp_spotlight()) << "Warning: m_target NULL";
}

/**
 * @brief M1UI::TreeRow::dbgOneLiner
 * @return
 */
QString M1UI::TreeRow::dbgOneLiner() const{
    Q_ASSERT_X(m_edge != nullptr, "M1UI::TreeRow::dbgOneLiner()", "m_edge is null");
    Q_ASSERT_X(m_target != nullptr, "M1UI::TreeRow::dbgOneLiner()", "m_target is null");

    return QString("%1 %2").arg(m_edge->dbgHalfLeft()).arg(m_target->dbgOneLiner());
}

/**
 * @brief M1MidPlane::Interp::dbgOneLiner
 * @return
 */
QString M1MidPlane::Interp::dbgOneLinerVirtual(){
    Q_ASSERT_X(m_myself != nullptr, "M1MidPlane::Interp::dbgOneLiner()", "m_myself is null");

    return QString("[%1] %2").arg(this->className()).arg(m_myself->dbgShort().replace(g_re_tags, ""));
}

/**
 * @brief M1UI::TreeRow::blockFocusEvents
 */
void M1UI::TreeRow::blockFocusEvents(){
    FocusEventsBlocker *l_focus_blocker = new FocusEventsBlocker(this);
    this->installEventFilter(l_focus_blocker);
}

/**
 * @brief M1UI::TreeRow::paintOpenClose
 * @param p
 */
void M1UI::TreeRow::paintOpenClose(QPainter& p){
    if(m_edge->flags() & M1Env::EDGE_IS_OPEN)
        M1UI::TreeRow::cm_open.paint(&p, m_oc_x, m_target_padding, m_icon_size, m_icon_size);
    else
        M1UI::TreeRow::cm_closed.paint(&p, m_oc_x, m_target_padding, m_icon_size, m_icon_size);
}

/**
 * @brief M1UI::TreeRow::emitSignals
 */
void M1UI::TreeRow::emitSignals(){
    M1_FUNC_ENTRY(g_cat_tree_row, QString("emit signals(): %1").arg(this->dbgOneLiner()))
    if(m_html_cache.length() == 0) m_html_cache = m_target->getHtml(m_edge);
    emit emitHtml(m_html_cache);
    emit emitEdit(m_target->get_edit_widget());
    M1_FUNC_EXIT
}

/**
 * @brief M1UI::TreeRow::performPostUpdate
 *
 * Called after text edit completion (from Interp::save_text_edit()) - lighter update than full gotoVertex (done after createDescendant or Drag/Drop)
 */
void M1UI::TreeRow::performPostUpdate(){
    M1MidPlane::Interp::invalidateAllCaches();
    M1UI::TreeRow::invalidateAllCaches();
    m_td_parent->repaint();
    this->emitSignals();
    QWidget::setFocus(Qt::OtherFocusReason);
}

void M1UI::TreeRow::invalidateCache(){
    m_html_cache = "";
}

void M1UI::TreeRow::invalidateAllCaches(){
    for(const auto& l_row : cm_row_list)
        l_row->invalidateCache();
}
/**
 * @brief M1UI::TreeRow::paintEvent
 * @param p_event
 */
void M1UI::TreeRow::paintEvent(QPaintEvent* p_event){
    qCDebug(g_cat_tree_row) << this->dbgOneLiner() << " painting: " << p_event->rect();

    QPainter p(this);
    // edge type icon
    m_target->edgeIcon(m_edge)->paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    // target type icon
    m_target->vertexIcon(m_edge)->paint(&p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);

    // AUTO edge
    if(m_edge->isOfType(M1Env::AUTO_SIID)){
        p.setPen(Qt::darkGray);
        p.drawLine(QPoint(0,0), QPoint(this->width(),0));
        p.drawLine(QPoint(0, m_target_height-1), QPoint(this->width(), m_target_height-1));
    }
    else
        // open/close icon if requested
        if(m_target->displayOpenClose()) paintOpenClose(p);

    // text
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_baseline), m_target->inTreeDisplayText(m_edge));

    // drag markers
    p.setPen(QPen(QBrush(Qt::darkRed), 2.0));
    if(m_drag_top){
        p.drawLine(1, 1, this->width()-1, 1);
        if(!m_drag_bottom)
            for(int x = this->width()/4; x < this->width()-4; x += this->width()/4)
                p.drawPolygon(QList<QPoint>() << QPoint(x, 2) << QPoint(x-4, 10) << QPoint(x+4, 10));
        else{
            p.setBrush(QBrush(QColor(255, 0, 0, 63)));
            p.drawRect(this->rect());
        }
    }
    if(m_drag_bottom){
        p.drawLine(0, this->height()-2, this->width()-1, this->height()-2);
        if(!m_drag_top)
            for(int x = this->width()/4; x < this->width()-4; x += this->width()/4)
                p.drawPolygon(QList<QPoint>() <<
                              QPoint(x, this->height()-2) <<
                              QPoint(x-4, this->height()-10) <<
                              QPoint(x+4, this->height()-10));
    }

    // Separator above
    p.setPen(Qt::white);
    if(m_edge->flags() & M1Env::EDGE_SEPABOVE)
        p.drawLine(QPoint(m_oc_x + m_target_height * 2, 0), QPoint(this->width()-1, 0));

    // qCDebug(g_cat_interp_drag) << "Accepting Drops" << this->acceptDrops() << m_myself->getTarget_lv2()->text();
}

/**
 * @brief M1UI::TreeRow::resizeEvent
 * @param p_event
 */
void M1UI::TreeRow::resizeEvent(QResizeEvent *p_event){
    qCDebug(g_cat_tree_row) << this->dbgOneLiner() << " resized: " << p_event->size();
}

/**
 * @brief M1UI::TreeRow::mouseDoubleClickEvent
 * @param p_event
 */
void M1UI::TreeRow::mouseDoubleClickEvent(QMouseEvent *p_event){
    qCDebug(g_cat_tree_row) << this->dbgOneLiner() << " double click: " << p_event->pos();
    emit gotoVertex(this->where_to_go(), this);
}

/**
 * @brief M1UI::TreeRow::focusInEvent
 * @param p_event
 */
void M1UI::TreeRow::focusInEvent(QFocusEvent *p_event){
    qCDebug(g_cat_tree_row) << "focus in: " << p_event->reason() << this->dbgOneLiner();
    qCDebug(g_cat_tree_row) << " Background color: " << this->palette().color(QPalette::Window);

    this->setAutoFillBackground(true);
    this->repaint();
}

/**
 * @brief M1UI::TreeRow::focusOutEvent
 * @param p_event
 */
void M1UI::TreeRow::focusOutEvent(QFocusEvent *p_event){
    qCDebug(g_cat_tree_row) << "focus out: " << p_event->reason() << this->dbgOneLiner();
    this->setAutoFillBackground(false);
    this->repaint();
}

/**
 * @brief M1UI::TreeRow::mousePressEvent
 * @param p_event
 */
void M1UI::TreeRow::mousePressEvent(QMouseEvent *p_event){
    qCDebug(g_cat_interp_drag) << this->dbgOneLiner() << " mouse press: " << p_event->pos();

    if(m_oc_x < p_event->pos().x() && p_event->pos().x() < m_oc_x + m_target_height){
        qCDebug(g_cat_interp_drag) << m_edge->dbgShort() << " OC clicked";
        if(m_edge->flags() & M1Env::EDGE_IS_OPEN)
            m_edge->unSetFlag(M1Env::EDGE_IS_OPEN);
        else
            m_edge->setFlag(M1Env::EDGE_IS_OPEN);
        qCDebug(g_cat_interp_drag) << QString("GotoVertex after O/C BEFORE") << this->dbgOneLiner();
        emit gotoVertex(nullptr, this);
    }
    else {
        // initiate Drag/Drop
        m_hold_timer.start(HOLD_DELAY);
    }
}

/**
 * @brief M1UI::TreeRow::mouseReleaseEvent
 * @param p_event
 */
void M1UI::TreeRow::mouseReleaseEvent(QMouseEvent *p_event){
    M1_FUNC_ENTRY(g_cat_interp_drag, QString("mouse release(): %1").arg(this->dbgOneLiner()))
    m_hold_timer.stop();
    this->emitSignals();
    M1_FUNC_EXIT
}
/*
void M1MidPlane::Interp::mouseMoveEvent(QMouseEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("INTERP mouse move event") << this->acceptDrops() << m_myself->getTarget_lv2()->text() << p_event->position();
}*/

/**
 * @brief M1UI::TreeRow::dragEnterEvent
 * @param p_event
 */
void M1UI::TreeRow::dragEnterEvent(QDragEnterEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("Drag ENTER p_event") << p_event->mimeData()->text() << p_event->mimeData()->formats() << this->dbgOneLiner();
    p_event->setAccepted(true);
}

/**
 * @brief M1UI::TreeRow::dragMoveEvent
 * @param p_event
 */
void M1UI::TreeRow::dragMoveEvent(QDragMoveEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("Drag MOVE p_event") << p_event->mimeData()->text() << p_event->mimeData()->formats()
    << this->dbgOneLiner() << p_event->position().toPoint() << this->rect();

    m_drag_top = false;
    m_drag_bottom = false;
    if(p_event->position().toPoint().y() < 7 ) m_drag_top = true;
    else if(p_event->position().toPoint().y() > this->height() - 7 ) m_drag_bottom = true;
    else {
        m_drag_top = true;
        m_drag_bottom = true;
    }
    this->repaint();
    p_event->setAccepted(true);
}

/**
 * @brief M1UI::TreeRow::dragLeaveEvent
 * @param p_event
 */
void M1UI::TreeRow::dragLeaveEvent(QDragLeaveEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("Drag LEAVE p_event") << this->dbgOneLiner();
    m_drag_top = false;
    m_drag_bottom = false;

    this->repaint();
}

/**
 * @brief M1UI::TreeRow::dropEvent
 * @param p_event
 */
void M1UI::TreeRow::dropEvent(QDropEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("DROP p_event") << p_event->mimeData()->text() << p_event->mimeData()->formats() << this->dbgOneLiner();
    M1Env::ItemID l_payload_item_id = QString(p_event->mimeData()->text()).toLongLong();
    M1Store::Item_lv2* l_item_to = M1Store::Item_lv2::getExisting(l_payload_item_id);

    if(m_drag_top && m_drag_bottom){
        M1Store::Item_lv2* l_item_from = this->where_to_go();
        qCDebug(g_cat_interp_drag) << QString("Creating edge of type") << m_td_parent->newEdgeType()->mnemonic() <<
            " From " << l_item_from->dbgShort() << " to " << l_item_to->dbgShort();
        l_item_from->linkTo(l_item_to, m_td_parent->newEdgeType()->specialId());
    }
    else{
        M1Store::Item_lv2* l_item_from = this->where_to_go();
        qCDebug(g_cat_interp_drag) << QString("Creating edge of type") << m_td_parent->newEdgeType()->mnemonic() <<
            " From " << l_item_from->dbgShort() << " to " << l_item_to->dbgShort();
        if(m_drag_bottom)
            l_item_from->linkTo(l_item_to, m_td_parent->newEdgeType()->specialId(), m_edge, false);
        else
            l_item_from->linkTo(l_item_to, m_td_parent->newEdgeType()->specialId(), m_edge->get_previous_lv2(), false);
    }

    m_drag_top = false;
    m_drag_bottom = false;

    // m_proxy->repaint();
    emit gotoVertex(nullptr, this);
}

/**
 * @brief M1UI::TreeRow::contextMenuEvent
 * @param p_event
 */
void M1UI::TreeRow::contextMenuEvent(QContextMenuEvent *p_event) {
    qCDebug(g_cat_tree_row) << QString("Context menu request") << this->dbgOneLiner();
    m_hold_timer.stop();

    m_td_parent->setTargetForMenuActions(this->m_target);
    QMenu l_context_menu(m_td_parent);
    QMenu* l_sub = l_context_menu.addMenu("Create New Descendant");

    QAction *l_nd_auto = l_sub->addAction("Below Auto");
    QAction *l_nd_bottom = l_sub->addAction("At Bottom");

    connect(l_nd_auto,   &QAction::triggered,
            m_td_parent, &M1UI::TreeDisplay::create_descendant_auto);
    connect(l_nd_bottom, &QAction::triggered,
            m_td_parent, &M1UI::TreeDisplay::create_descendant_bottom);

    QAction *l_dbg_Interp_list = l_context_menu.addAction("List Interp Cache");
    connect(l_dbg_Interp_list, &QAction::triggered,
            m_td_parent,       &M1UI::TreeDisplay::dbg_interp_cache);

    QAction *l_dbg_garbage_collect = l_context_menu.addAction("Garbage Collection");
    connect(l_dbg_garbage_collect, &QAction::triggered,
            m_td_parent,           &M1UI::TreeDisplay::garbageCollect);

    l_context_menu.exec(p_event->globalPos());
}

/**
 * @brief M1UI::TreeRow::initiateDrag
 */
void M1UI::TreeRow::initiateDrag(){
    Drag *l_drag = new Drag(this, m_td_parent);

    QString l_payload = QString("%1").arg(this->where_to_go()->item_id());
    QMimeData *l_data = new QMimeData;
    l_data->setText(l_payload);
    qCDebug(g_cat_tree_row) << QString("Drag INITIATION ") << this->acceptDrops() << l_payload << l_data->formats();

    l_drag->setMimeData(l_data);
    this->setAcceptDrops(false);
    m_td_parent->setBeingDragged(this);
    l_drag->exec();
}

/**
 * @brief M1UI::Drag::~Drag
 */
M1UI::Drag::~Drag(){
    qCDebug(g_cat_interp_drag) << "DRAG end: " << this->mimeData()->text();
    m_td_parent->restoreAcceptDrop();
}

/**
 * @brief M1UI::TreeRow::restore_acept_drops
 */
void M1UI::TreeRow::restore_acept_drops(){
    qCDebug(g_cat_interp_drag) << "Accept drops again " << this->dbgOneLiner();
    this->setAcceptDrops(true);
}

/**
 * @brief M1UI::TreeDisplay::restoreAcceptDrop
 */
void M1UI::TreeDisplay::restoreAcceptDrop(){
    if(m_being_dragged != nullptr) m_being_dragged->restore_acept_drops();
    m_being_dragged = nullptr;
}

/**
 * @brief M1UI::TreeRow::handleMouseHold
 */
void M1UI::TreeRow::handleMouseHold(){
    qCDebug(g_cat_interp_drag) << QString("after %1 ms --> initiate DRAG").arg(HOLD_DELAY) << this->dbgOneLiner();
    m_hold_timer.stop();
    // initiate Drag/Drop
    this->initiateDrag();
}

/**
 * @brief M1UI::TreeRow::create_descendant
 */
/*
void M1UI::TreeRow::create_descendant(){
    M1Store::SpecialItem* l_new_edge_type = m_td_parent->newEdgeType();
    M1Store::SpecialItem* l_new_vertex_type = m_td_parent->newVertexType();
    qCDebug(g_cat_tree_row) << QString("Create New Descendant") <<
        "Edge Type:" << l_new_edge_type->mnemonic() <<
        "Vertex Type:" << l_new_vertex_type->mnemonic() <<
        this->dbgOneLiner();

    m_target->createDescendant(l_new_edge_type, l_new_vertex_type);
    m_td_parent->gotoVertex(nullptr, this);
}*/