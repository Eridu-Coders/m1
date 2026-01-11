#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"
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
Q_LOGGING_CATEGORY(g_cat_tree_row, "tree_row")
Q_LOGGING_CATEGORY(g_cat_interp_base, "interp.base")
// g_cat_interp_drag
Q_LOGGING_CATEGORY(g_cat_interp_drag, "interp.drag")

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

/**
 * @brief M1UI::TreeRow::dbgOneLiner
 * @return
 */
QString M1UI::TreeRow::dbgOneLiner(){
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
 * @brief M1UI::TreeRow::~TreeRow
 */
M1UI::TreeRow::~TreeRow(){
    qCDebug(g_cat_tmp_spotlight()) << QString("Use Count: %1").arg(m_target.use_count()) << "Deleting:" << this->dbgOneLiner();
    // sever link btw TreeRow and Interp (m_target) before destruction
    if(m_target != nullptr) m_target->setParent(nullptr);
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

    QMenu l_context_menu(this);
    QAction *l_new_descendant_action = l_context_menu.addAction("Create New Descendant");
    connect(l_new_descendant_action, &QAction::triggered,
            this,                    &M1UI::TreeRow::create_descendant);

    QAction *l_dbg_Interp_list = l_context_menu.addAction("List Interp Cache");
    connect(l_dbg_Interp_list, &QAction::triggered,
            this,              &M1UI::TreeRow::dbg_interp_cache);

    l_context_menu.exec(p_event->globalPos());
}

void M1UI::TreeRow::dbg_interp_cache(){
    qCDebug(g_cat_tmp_spotlight()) << "dbg_interp_cache";
    emit emitHtml(M1MidPlane::Interp::dbgMapContents(true));
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
void M1UI::TreeRow::create_descendant(){
    M1Store::SpecialItem* l_new_edge_type = m_td_parent->newEdgeType();
    M1Store::SpecialItem* l_new_vertex_type = m_td_parent->newVertexType();
    qCDebug(g_cat_tree_row) << QString("Create New Descendant") <<
        "Edge Type:" << l_new_edge_type->mnemonic() <<
        "Vertex Type:" << l_new_vertex_type->mnemonic() <<
        this->dbgOneLiner();

    m_target->createDescendant(l_new_edge_type, l_new_vertex_type);
    m_td_parent->gotoVertex(nullptr, this);
}

/** --------------------------------------------------------------- Interp Root Class ---------------------------------
 * @brief M1MidPlane::Interp::getInterp
 * @param p_myself
 * @return
 */
std::shared_ptr<M1MidPlane::Interp> M1MidPlane::Interp::getInterp(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Getting appropriate Interp for: %1").arg(p_myself->dbgShort()))

    std::shared_ptr<M1MidPlane::Interp> l_ret = nullptr;
    // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());
    // qCDebug(g_cat_interp_base).noquote() << M1MidPlane::Interp::dbgMapContents();
    // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());

    int l_use_count_ret = 0;
    int l_use_count_map = 0;
    auto l_it = cm_interp_map.find(p_myself->item_id());
    if(l_it != cm_interp_map.end()){
        l_use_count_map = l_it->second.use_count();
        l_ret = l_it->second;
        l_use_count_ret = l_ret.use_count();
        // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());
        // qCDebug(g_cat_interp_base) << "after cache lookup : Exists";
    }
    else{
        // qCDebug(g_cat_interp_base) << "after cache lookup : Not found -> creating it";
        // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());
        Interp* l_interp_raw = nullptr;
        while(l_interp_raw == nullptr){
            // qCDebug(g_cat_interp_base) << QString("A l_interp_raw: %1").arg(l_interp_raw == nullptr ? "null" : "instanciated");

            //if((l_interp_raw = AutoInterp::getOneIfMatch(p_myself)) != nullptr) break;
            // else if((l_interp_raw = FieldInterp::getOneIfMatch(p_myself)) != nullptr) break; SlokaInterp
            if((l_interp_raw = FieldInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = TextInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = RoleInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = WfWUnit::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = UrlInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = OccurInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = FormInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = LemmaInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = TranslationBhashya::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = SlokaInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else l_interp_raw = new Interp(p_myself);
        }
        // qCDebug(g_cat_interp_base) << QString("B l_interp_raw: %1").arg(l_interp_raw == nullptr ? "null" : "instanciated");
        // qCDebug(g_cat_interp_base) << QString("B l_interp_raw class name: %1").arg(l_interp_raw->className());
        // qCDebug(g_cat_interp_base) << QString("l_interp_raw: %1").arg(l_interp_raw->dbgOneLiner());

        // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());
        l_ret = std::shared_ptr<M1MidPlane::Interp>(l_interp_raw);
        l_use_count_ret = l_ret.use_count();
        // qCDebug(g_cat_interp_base) << QString("l_ret one-liner: %1").arg(l_ret->dbgOneLiner());
        // qCDebug(g_cat_interp_base) << "C";
        // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());
        cm_interp_map[p_myself->item_id()] = l_ret;
        // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());
        // qCDebug(g_cat_interp_base) << "D";
    }
    // qCDebug(g_cat_interp_base) << QString("l_ret %1").arg(l_ret.use_count());
    // qCDebug(g_cat_interp_base) << QString("Returning: %1").arg(l_ret->dbgOneLiner());

    // qCDebug(g_cat_interp_base).noquote() << M1MidPlane::Interp::dbgMapContents();

    qCDebug(g_cat_tmp_spotlight()).noquote() << "Found:" << (l_it == cm_interp_map.end() ? "false" : "true") <<
        QString("Use Count: %1/%2").arg(l_use_count_map).arg(l_use_count_ret) << l_ret->dbgOneLiner();

    M1_FUNC_EXIT
    return l_ret;
}
M1MidPlane::Interp::~Interp(){
    // qCDebug(g_cat_interp_base).noquote() << "Interp deletion" << this->dbgOneLiner();
    qCDebug(g_cat_interp_base).noquote() << "Interp deletion:" << m_dbgOneLinerCache;
}

std::map<M1Env::ItemID, std::shared_ptr<M1MidPlane::Interp>> M1MidPlane::Interp::cm_interp_map;

QString M1MidPlane::Interp::dbgMapContents(bool p_html){
    qCDebug(g_cat_tmp_spotlight()) << "dbgMapContents";

    QStringList l_ret;

    QString l_row_template;
    if(p_html){
        l_row_template = "<tr class=\"technical\"><td>%1:</td><td>(use count: %2)</td><td>%3</td></tr>";
        l_ret.append(QString("<p class=\"technical\">cm_interp_map [%1] contents:</p>\n<table>").arg(cm_interp_map.size()));
    }
    else{
        l_row_template = "%1: (use count: %2) %3";
        l_ret.append(QString("cm_interp_map [%1] contents:").arg(cm_interp_map.size()));
    }

    for(const auto& l_pair : cm_interp_map)
        l_ret.append(QString(l_row_template)
                         .arg(l_pair.first, 6)
                         .arg(l_pair.second != nullptr ? QString("%1").arg(l_pair.second.use_count()) : "NA")
                         .arg(l_pair.second != nullptr ? l_pair.second->dbgOneLiner() : "WARNING: nullptr"));

    if(p_html){
        l_ret.append("</table>");
        return cm_html_template.arg(l_ret.join("\n"));
    }
    else return l_ret.join("\n");
}

QString M1MidPlane::Interp::cm_html_template =
    QString("<html><Head>\n") +
        "<style>\n" +
        "body {font-size: 12pt; font-family: \"Latin Modern Math\", Times New Roman, Serif}\n" + // sepabove
        "h1 {font-size: 20pt; text-align: center;}\n" +
        "h2 {font-size: 16pt; font-weight: normal; text-align: center;}\n" +
        "h3 {font-size: 12pt; font-weight: normal; text-align: center;}\n" +
        "h4 {font-size: 14pt; font-weight: normal; border-top: 1px solid black; font-style: italic}\n" +
        "h5 {font-size: 16pt; font-weight: normal;}\n" +
        "td.wfw-top {padding-top: 1em; padding-right: 1em;}\n" +
        "td.wfw {padding-right: 1em;}\n" + // grammar
        "p.grammar {margin: 0; margin-left: 2em;}\n" +
        "p.sepabove {border-top: 1px solid black; margin-top: 2em; padding-top: 1em;}\n" +
        ".technical {font-size: 12pt; font-weight: normal; font-family: \"Noto Mono\", FreeMono, Monospace}\n}\n" +
        "table.wb {border: 1px solid black;border-collapse: collapse;}\n"
        "td.wb, th.wb {border: 1px solid black; border-collapse: collapse; padding: 0.5em 0.5em 0.5em 0.5em}\n"
        "/* Tooltip container */\n" +
        ".tooltip {\n" +
        "  position: relative;\n" +
        "  display: inline-block;\n" +
        "  border-bottom: 1px dotted black; /* If you want dots under the hoverable text */\n" +
        "}\n" +
        "\n" +
        "/* Tooltip text */\n" +
        ".tooltip .tooltiptext {\n" +
        "  visibility: hidden;\n" +
        "  width: 800px;\n" +
        "  background-color: #555;\n" +
        "  color: #fff;\n" +
        "  text-align: center;\n" +
        "  padding: 5px 0;\n" +
        "  border-radius: 6px;\n" +
        "\n" +
        "  /* Position the tooltip text */\n" +
        "  position: absolute;\n" +
        "  z-index: 1;\n" +
        "  bottom: -200%;\n" +
        "  left: 50%;\n" +
        "  margin-left: -60px;\n" +
        "\n" +
        "  /* Fade in tooltip */\n" +
        "  opacity: 0;\n" +
        "  transition: opacity 0.3s;\n" +
        "}\n" +
        "\n" +
        "/* Tooltip arrow */\n" +
        ".tooltip .tooltiptext::after {\n" +
        "  content: "";\n" +
        "  position: absolute;\n" +
        "  top: 100%;\n" +
        "  left: 50%;\n" +
        "  margin-left: -5px;\n" +
        "  border-width: 5px;\n" +
        "  border-style: solid;\n" +
        "  border-color: #555 transparent transparent transparent;\n" +
        "}\n" +
        "\n" +
        "/* Show the tooltip text when you mouse over the tooltip container */\n" +
        ".tooltip:hover .tooltiptext {\n" +
        "  visibility: visible;\n" +
        "  opacity: 1;\n" +
        "}\n" +
        "</style>\n" +
        "</Head><body><div style=\"margin: 1em;\">\n%1</div></body></html>";

/**
 * @brief M1MidPlane::Interp::Interp
 * @param p_myself (Item_lv2*) can be any category of item, including simple vertices and eges in the case of fields
 */
M1MidPlane::Interp::Interp(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Base Interp Constructor from: %1").arg(p_myself->dbgShort()))

    m_myself = p_myself;
    // M1MidPlane::Interp::cm_interp_map[p_myself->item_id()] = std::shared_ptr<Interp>(this);
    // qCDebug(g_cat_interp_base) << QString("l_interp_raw: %1").arg(this->dbgOneLiner());

    M1_FUNC_EXIT
}

/**
 * @brief M1MidPlane::Interp::inTreeDisplayText
 * @return
 */
QString M1MidPlane::Interp::inTreeDisplayText(const M1Store::Item_lv2* p_edge){
    return m_myself->text();
}

/**
 * @brief M1MidPlane::Interp::edgeIcon
 * @param p_edge
 * @return
 */
QIcon* M1MidPlane::Interp::edgeIcon(const M1Store::Item_lv2* p_edge){
    return M1Store::StorageStatic::getQIcon(p_edge->getIconSITypeID());
}

/**
 * @brief M1MidPlane::Interp::vertexIcon
 * @return
 */
QIcon* M1MidPlane::Interp::vertexIcon(const M1Store::Item_lv2* p_edge){
    return M1Store::StorageStatic::getQIcon(m_myself->getIconSITypeID());
}

/**
 * @brief M1MidPlane::Interp::base_edge_html_fragment
 * @param p_edge
 * @return
 */
QString M1MidPlane::Interp::base_edge_html_fragment(const M1Store::Item_lv2* p_edge){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("base_edge_html_fragment: %1").arg(this->dbgOneLiner()))

    QStringList l_self_list;
    l_self_list.append(QString("<tr><td>ID</td><td>:</td><td>%1</td></tr>\n").arg(p_edge->item_id()));
    l_self_list.append(QString("<tr><td>Nature</td><td>:</td><td>%1</td></tr>\n").arg(
        p_edge->isFullEdge() ? "Full Edge" : (p_edge->isSimpleEdge() ? "Simple Edge" : "ERROROLOLO")
        ));
    l_self_list.append(QString("<tr><td>Type</td><td>:</td><td>%1</td></tr>\n").arg(p_edge->dbgTypeShort()));
    QString l_self_html = QString("<table class=\"technical\">%1</table>\n").arg(l_self_list.join("\n"));
    QString l_ret = QString("<p class=\"technical\" style=\"font-weight: bold;\">Edge:</p>\n%1").arg(l_self_html);
    qCDebug(g_cat_interp_base) << "l_self_html:" << l_ret.length();

    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief M1MidPlane::Interp::base_html_fragment
 * @return
 */
QString M1MidPlane::Interp::base_html_fragment(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("base_html_fragment: %1").arg(this->dbgOneLiner()))

    QString l_html = m_myself->isSimpleEdge() ?
        QString("<p class=\"technical\"><span style=\"font-weight: bold;\">Field Payload</span>: %1</p>\n").arg(m_myself->text()) :
        QString("<p class=\"technical\" style=\"font-weight: bold;\">Target:</p>\n<div class=\"technical\">%1</div>\n").arg(m_myself->dbgStringHtml());

    qCDebug(g_cat_interp_base) << "l_html:" << l_html.length();
    M1_FUNC_EXIT
    return QString("<div\">%1</div>\n").arg(l_html);
}

/**
 * @brief M1MidPlane::Interp::getHtmlVirtual
 * @param p_edge
 * @return
 */
QString M1MidPlane::Interp::getHtmlVirtual(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Interp getHtmlVirtual(): %1").arg(this->dbgOneLiner()))
    M1_FUNC_EXIT
    return "";
}

/**
 * @brief M1MidPlane::Interp::getHtml
 * @param p_edge
 * @return
 */
QString M1MidPlane::Interp::getHtml(const M1Store::Item_lv2* p_edge){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Interp getHtml(): (Edge ID: %2) %1").arg(this->dbgOneLiner()).arg(p_edge->item_id()))

    QString l_own_html = getHtmlVirtual();
    QString l_ret_html =
        l_own_html.left(4) == "http" ? l_own_html : cm_html_template.arg(l_own_html +
            QString("<p class=\"technical sepabove\"><span style=\"font-weight: bold;\">Class</span>: %1</p>\n").arg(className()) +
            base_edge_html_fragment(p_edge) +
            base_html_fragment()
        );

    qCDebug(g_cat_interp_base) << "l_ret_html:" << l_ret_html.length();

    M1_FUNC_EXIT
    return l_ret_html;
}

/**
 * @brief M1MidPlane::Interp::invalidateCache
 */
void M1MidPlane::Interp::invalidateCache(){
    // m_edge_cache_iid = M1Store::G_VOID_ITEM_ID;
}

/**
 * @brief M1MidPlane::Interp::invalidateAllCaches
 */
void M1MidPlane::Interp::invalidateAllCaches(){
    for(const auto& l_interp_pair : cm_interp_map)
        l_interp_pair.second->invalidateCache();
}

/**
 * @brief M1MidPlane::Interp::get_edit_widget
 *
 * \todo create a cache for edit_widgets
 *
 * @return
 */
QWidget *M1MidPlane::Interp::get_edit_widget(){
    qCDebug(g_cat_interp_base) << QString("text: %1").arg(m_myself->dbgShort());

    QWidget* l_panel_widget = new QWidget();
    QVBoxLayout* l_panel_layout = new QVBoxLayout();
    l_panel_widget->setLayout(l_panel_layout);

    QWidget* l_button_bar = new QWidget(l_panel_widget);
    l_panel_layout->addWidget(l_button_bar);
    QHBoxLayout* l_bar_layout = new QHBoxLayout();
    l_button_bar->setLayout(l_bar_layout);

    QPushButton* l_btn0 = new QPushButton("Save", l_button_bar);
    l_bar_layout->addWidget(l_btn0);
    l_bar_layout->addStretch(1);
    QObject::connect(l_btn0, &QPushButton::clicked,
                     this, &M1MidPlane::Interp::save_text_edit);

    m_text_edit = new QTextEdit(l_panel_widget);
    l_panel_layout->addWidget(m_text_edit);

    m_text_edit->setPlainText(m_myself->text());

    return l_panel_widget;
}

/**
 * @brief M1MidPlane::Interp::save_text_edit
 */
void M1MidPlane::Interp::save_text_edit(){
    qCDebug(g_cat_interp_base) << "Saving text edit field: " << m_text_edit->toPlainText();
    if(m_myself->isSimpleEdge())
        m_myself->setText_lv1(m_text_edit->toPlainText());
    else
        m_myself->getTarget_lv2()->setText_lv1(m_text_edit->toPlainText());

    m_tree_row->performPostUpdate();
}

/**
 * @brief M1MidPlane::Interp::createDescendant
 * @param p_new_edge_type
 * @param p_new_vertex_type
 */
void M1MidPlane::Interp::createDescendant(M1Store::SpecialItem* p_new_edge_type, M1Store::SpecialItem* p_new_vertex_type){
    m_myself->create_descendant(
        p_new_edge_type->specialId(),
        "New",
        p_new_vertex_type->specialId()
        );
}

/** --------------------------------------------------------------- AutoInterp ---------------------------------
 * @brief M1MidPlane::AutoInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
/*
M1MidPlane::AutoInterp* M1MidPlane::AutoInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an AutoInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::AutoInterp* l_ret = nullptr;
    if(p_myself->isFullEdge() && p_myself->isOfType(M1Env::AUTO_SIID))
        l_ret = new AutoInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::AutoInterp::AutoInterp(M1Store::Item_lv2* p_myself) : Interp(p_myself){}
*/

