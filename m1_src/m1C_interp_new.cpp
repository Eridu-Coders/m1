#include "m1C_interp_new.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"
#include "m1B_lv2_iterators.h"
#include "m1D_tree_display.h"
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

// g_cat_interp_dev
Q_LOGGING_CATEGORY(g_cat_interp_dev, "interp.dev")
Q_LOGGING_CATEGORY(g_cat_interp_base, "interp.base")
// g_cat_interp_drag
Q_LOGGING_CATEGORY(g_cat_interp_drag, "interp.drag")

QIcon M1UI::TreeRow::cm_open;
QIcon M1UI::TreeRow::cm_closed;

void M1UI::TreeRow::init(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Init TreeRow class members"))

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
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TreeRow Constructor from: %1").arg(p_edge->dbgShort()))

    Q_ASSERT_X(p_edge->isFullEdge() || p_edge->isSimpleEdge(),
               "M1MidPlane::TreeRow::TreeRow()",
               "A TreeRow can obnly be created out of an edge");

    if(p_edge->isFullEdge()) m_target = M1MidPlane::Interp::getInterp(p_edge->getTarget_lv2()); // nearly all cases, including simple vertex field
    else m_target = M1MidPlane::Interp::getInterp(p_edge); // simple edge field case only

    m_edge = p_edge;

    p_tree->tree_vb_layout()->addWidget(this);
    qCDebug(g_cat_interp_base) << "TreeRow added" << p_tree->tree_vb_layout()->count() << p_tree->widget()->children().count() << p_edge->dbgShort();

    m_td_parent = p_tree;

    qCDebug(g_cat_interp_base) << "baseSize()  :" << this->baseSize();
    qCDebug(g_cat_interp_base) << "rect()      :" << this->rect();
    qCDebug(g_cat_interp_base) << "font height : " << this->fontMetrics().height();
    qCDebug(g_cat_interp_base) << "font family : " << this->fontInfo().family();

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
 * @brief M1MidPlane::Interp::getInterp
 * @param p_myself
 * @return
 */
shared_ptr<M1MidPlane::Interp> M1MidPlane::Interp::getInterp(M1Store::Item_lv2* p_myself){
    shared_ptr<M1MidPlane::Interp> l_ret = cm_interp_cache[p_myself->item_id()];

    if(l_ret == nullptr){
        Interp* l_interp_raw = nullptr;
        while(l_interp_raw == nullptr){
            if((l_interp_raw = AutoInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else if((l_interp_raw = FieldInterp::getOneIfMatch(p_myself)) != nullptr) break;
            else l_interp_raw = new Interp(p_myself);
        }
        l_ret = shared_ptr<M1MidPlane::Interp>(l_interp_raw);
        cm_interp_cache[p_myself->item_id()] = l_ret;
    }
    return l_ret;
}

/**
 * @brief M1MidPlane::Interp::Interp
 * @param p_myself (Item_lv2*) can be any category of item, including simple vertices and eges in the case of fields
 */
M1MidPlane::Interp::Interp(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Base Interp Constructor from: %1").arg(p_myself->dbgShort()))

    m_myself = p_myself;
    cm_interp_cache[p_myself->item_id()] = shared_ptr<Interp>(this);

    M1_FUNC_EXIT
}

/**
 * @brief M1MidPlane::AutoInterp::getOneIfMatch
 * @param p_myself
 * @return
 */
M1MidPlane::AutoInterp* M1MidPlane::AutoInterp::getOneIfMatch(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Will this be an AutoInterp? %1").arg(p_myself->dbgShort()))
    M1MidPlane::AutoInterp* l_ret = nullptr;
    if(p_myself->isFullEdge() && p_myself->isOfType(M1Env::AUTO_SIID))
        l_ret = new AutoInterp(p_myself);
    M1_FUNC_EXIT
    return l_ret;
}

/**
 * @brief M1MidPlane::AutoInterp::AutoInterp
 * @param p_myself
 */
M1MidPlane::AutoInterp::AutoInterp(M1Store::Item_lv2* p_myself) : Interp(p_myself){}

/**
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
