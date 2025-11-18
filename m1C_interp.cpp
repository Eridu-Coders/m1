#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"
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

QIcon M1MidPlane::Interp::cm_open;
QIcon M1MidPlane::Interp::cm_closed;

const int HOLD_DELAY = 800;

void M1UI::InterpProxyWidget::paintEvent(QPaintEvent* p_event){m_main_instance->paintEvent(p_event);}
void M1UI::InterpProxyWidget::resizeEvent(QResizeEvent *p_event){m_main_instance->resizeEvent(p_event);}
void M1UI::InterpProxyWidget::mouseDoubleClickEvent(QMouseEvent *p_event){m_main_instance->mouseDoubleClickEvent(p_event);}
void M1UI::InterpProxyWidget::mousePressEvent(QMouseEvent *p_event){m_main_instance->mousePressEvent(p_event);}
void M1UI::InterpProxyWidget::mouseReleaseEvent(QMouseEvent *p_event){m_main_instance->mouseReleaseEvent(p_event);}
void M1UI::InterpProxyWidget::focusOutEvent(QFocusEvent *p_event){m_main_instance->focusOutEvent(p_event);}
void M1UI::InterpProxyWidget::focusInEvent(QFocusEvent *p_event){m_main_instance->focusInEvent(p_event);}
void M1UI::InterpProxyWidget::contextMenuEvent(QContextMenuEvent *p_event){m_main_instance->contextMenuEvent(p_event);}

void M1UI::InterpProxyWidget::dragEnterEvent(QDragEnterEvent *p_event){m_main_instance->dragEnterEvent(p_event);}
void M1UI::InterpProxyWidget::dragMoveEvent(QDragMoveEvent *p_event){m_main_instance->dragMoveEvent(p_event);}
void M1UI::InterpProxyWidget::dragLeaveEvent(QDragLeaveEvent *p_event){m_main_instance->dragLeaveEvent(p_event);}
void M1UI::InterpProxyWidget::dropEvent(QDropEvent *p_event){m_main_instance->dropEvent(p_event);}

// ------------------------------------ Interp Base Class -----------------------------------------------------
QString g_html_template = QString(QString("<html><Head>\n") +
                          "<style>\n" +
                          "table.wb {border: 1px solid black;border-collapse: collapse;}\n"
                          "td.wb, th.wb {border: 1px solid black;border-collapse: collapse; padding: 0.5em 0.5em 0.5em 0.5em}\n"
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
                          "</Head><body><div style=\"margin: 1em;\">\n%1</div></body></html>");

QList<M1Store::Item_lv2*> M1MidPlane::Interp::cm_gratt;

void M1MidPlane::Interp::init(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Init Interp class members"))
    cm_open = QIcon(M1Env::OPEN_ICON_PATH);
    Q_ASSERT_X( !cm_open.isNull(), "Interp::init()", "Open link Icon failed to load");
    cm_closed = QIcon(M1Env::CLOSED_ICON_PATH);
    Q_ASSERT_X( !cm_closed.isNull(), "Interp::init()", "Closed link Icon failed to load");

    M1Store::Item_lv2* l_gratt = M1Store::Item_lv2::getExisting(M1Store::GRAMMAR_ATTR_SIID);
    qCDebug(g_cat_interp_dev) << "l_gratt" << l_gratt->text();
    for(M1Store::Item_lv2_iterator it = l_gratt->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->isOfType(M1Env::ITO_SIID) ){
            qCDebug(g_cat_interp_dev) << it.at()->dbgShort();
            cm_gratt.append(it.at()->getTarget_lv2());
        }

    M1_FUNC_EXIT
}

M1MidPlane::Interp* M1MidPlane::Interp::getInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Getting appropriate Interp from: %1").arg(p_myself->dbgShort()))
    Interp* l_ret = nullptr;
    if(AutoInterp::wantIt(p_myself))
        l_ret = new AutoInterp(p_myself, p_vb, p_parent, p_depth);
    else if(TranslUnit::wantIt(p_myself))
        l_ret = new TranslUnit(p_myself, p_vb, p_parent, p_depth);
    else if(FieldInterp::wantIt(p_myself))
        l_ret = new FieldInterp(p_myself, p_vb, p_parent, p_depth);
    else if(SectionBeginEnd::wantIt(p_myself))
        l_ret = new SectionBeginEnd(p_myself, p_vb, p_parent, p_depth);
    else if(ChunkInterp::wantIt(p_myself))
        l_ret = new ChunkInterp(p_myself, p_vb, p_parent, p_depth);
    else if(UrlInterp::wantIt(p_myself))
        l_ret = new UrlInterp(p_myself, p_vb, p_parent, p_depth);
    else if(BhashyaTranslation::wantIt(p_myself))
        l_ret = new BhashyaTranslation(p_myself, p_vb, p_parent, p_depth);
    else if(TextInterp::wantIt(p_myself))
        l_ret = new TextInterp(p_myself, p_vb, p_parent, p_depth);
    else if(TextOccurrence::wantIt(p_myself))
        l_ret = new TextOccurrence(p_myself, p_vb, p_parent, p_depth);
    else if(SentenceInterp::wantIt(p_myself))
        l_ret = new SentenceInterp(p_myself, p_vb, p_parent, p_depth);
    else if(BookInterp::wantIt(p_myself))
        l_ret = new BookInterp(p_myself, p_vb, p_parent, p_depth);
    else if(HighlightChunkInterp::wantIt(p_myself))
        l_ret = new HighlightChunkInterp(p_myself, p_vb, p_parent, p_depth);
    else if(HighlightQuotationInterp::wantIt(p_myself))
        l_ret = new HighlightQuotationInterp(p_myself, p_vb, p_parent, p_depth);
    else if(HighlightInterp::wantIt(p_myself))
        l_ret = new HighlightInterp(p_myself, p_vb, p_parent, p_depth);
    else if(SlokaInterp::wantIt(p_myself))
        l_ret = new SlokaInterp(p_myself, p_vb, p_parent, p_depth);
    else if(TextLemma::wantIt(p_myself))
        l_ret = new TextLemma(p_myself, p_vb, p_parent, p_depth);
    else if(TextWForm::wantIt(p_myself))
        l_ret = new TextWForm(p_myself, p_vb, p_parent, p_depth);
    else{ // HighlightQuotationInterp SlokaInterp TextLemma TextWForm
        qCDebug(g_cat_interp_base) << "default Interp()";
        l_ret = new Interp(p_myself, p_vb, p_parent, p_depth);
    }
    M1_FUNC_EXIT
    return l_ret;
}

// ------------------------------------ Interp Base class -----------------------------------------------------
M1MidPlane::Interp::Interp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_tree, int p_depth) : QObject(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Base Interp Constructor from: %1").arg(p_myself->dbgShort()))

    m_proxy = new M1UI::InterpProxyWidget(this, p_tree->widget());
    p_vb->addWidget(m_proxy);
    qCDebug(g_cat_interp_base) << "proxy added" << p_vb->count() << p_tree->widget()->children().count() << p_myself->dbgShort();

    m_td_parent = p_tree;

    qCDebug(g_cat_interp_base) << "baseSize()  :" << m_proxy->baseSize();
    qCDebug(g_cat_interp_base) << "rect()      :" << m_proxy->rect();
    qCDebug(g_cat_interp_base) << "font height : " << m_proxy->fontMetrics().height();
    qCDebug(g_cat_interp_base) << "font family : " << m_proxy->fontInfo().family();
    m_depth = p_depth;
    m_myself = p_myself;
    m_target_height = (m_proxy->fontMetrics().height() * 1300) / 1000;
    m_target_beseline = (m_target_height * 800) / 1000;
    m_target_padding = (m_target_height - m_proxy->fontMetrics().height())/2;
    m_icon_size = m_proxy->fontMetrics().height();
    m_oc_x = m_target_padding + (p_depth + 1)*m_target_height;

    m_proxy->setMinimumHeight(m_target_height);
    m_proxy->setMaximumHeight(m_target_height);
    m_proxy->setFocusPolicy(Qt::StrongFocus);
    QPalette p = m_proxy->palette();
    p.setColor(QPalette::Window, QColor(Qt::darkRed));
    m_proxy->setPalette(p);
    m_proxy->setBackgroundRole(QPalette::Window);

    // qCDebug(g_cat_interp_drag) << "Accepting Drops" << m_myself->dbgShort();
    // this->setMouseTracking(true);
    m_proxy->setAcceptDrops(true);

    m_proxy->setAttribute(Qt::WA_AcceptDrops, false);
    m_proxy->setAttribute(Qt::WA_AcceptDrops, true);

    // timer signal after HOLD_DELAY ms
    connect(&m_hold_timer, &QTimer::timeout, this, &M1MidPlane::Interp::handleMouseHold);

    M1_FUNC_EXIT
}

// proxy invalidation
/*
M1UI::InterpProxyWidget::~InterpProxyWidget(){
    qCDebug(g_cat_interp_drag) << "proxy deletion for: " << m_main_instance->dbgString();
    // m_main_instance->invalidateProxy();
}
*/
void M1MidPlane::Interp::invalidateProxy(){
    qCDebug(g_cat_interp_drag) << "Invalidation of proxy in: " << this->dbgString();
    m_proxy = nullptr;
}
void M1MidPlane::Interp::deleteProxy(){
    if(m_proxy != nullptr){
        qCDebug(g_cat_interp_drag) << "Proxy delete" << dbgString();
        m_proxy->parentWidget()->layout()->removeWidget(m_proxy);
        m_proxy->setParent(nullptr);
        m_proxy->deleteLater();
        // delete m_proxy;
        m_proxy = nullptr;
    }
}
M1MidPlane::Interp::~Interp(){
    qCDebug(g_cat_interp_base) << Interp::dbgString() << "deleted";
}
void M1MidPlane::Interp::blockFocusEvents(){
    MyEventFilter *l_filter = new MyEventFilter(m_proxy);
    m_proxy->installEventFilter(l_filter);
}

void M1MidPlane::Interp::paintOC(QPainter& p){
    if(m_myself->flags() & M1Env::EDGE_IS_OPEN)
        M1MidPlane::Interp::cm_open.paint(&p, m_oc_x, m_target_padding, m_icon_size, m_icon_size);
    else
        M1MidPlane::Interp::cm_closed.paint(&p, m_oc_x, m_target_padding, m_icon_size, m_icon_size);
}

QString M1MidPlane::Interp::displayText(){
    return m_myself->getTarget_lv2()->text();
}

QIcon* M1MidPlane::Interp::edgeIcon(){
    return M1Store::Storage::getQIcon(m_myself->getIconSITypeID());
}
QIcon* M1MidPlane::Interp::vertexIcon(){
    return M1Store::Storage::getQIcon(m_myself->getTarget_lv2()->getIconSITypeID());
}

void M1MidPlane::Interp::paintEvent(QPaintEvent* p_event){
    qCDebug(g_cat_interp_base) << m_myself->dbgShort() << " painting: " << p_event->rect();
    if(m_proxy != nullptr){
        QPainter p(m_proxy);
        // edge type icon
        this->edgeIcon()->paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
        // open/close icon
        if(diplayOpenClose()) paintOC(p);
        // target type icon
        this->vertexIcon()->paint(&p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);

        // text
        p.setPen(Qt::white);
        p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), this->displayText());

        // drag markers
        p.setPen(QPen(QBrush(Qt::darkRed), 2.0));
        if(m_drag_top){
            p.drawLine(1, 1, m_proxy->width()-1, 1);
            if(!m_drag_bottom)
                for(int x = m_proxy->width()/4; x < m_proxy->width()-4; x += m_proxy->width()/4)
                    p.drawPolygon(QList<QPoint>() << QPoint(x, 2) << QPoint(x-4, 10) << QPoint(x+4, 10));
            else{
                p.setBrush(QBrush(QColor(255, 0, 0, 63)));
                p.drawRect(m_proxy->rect());
            }
        }
        if(m_drag_bottom){
            p.drawLine(0, m_proxy->height()-2, m_proxy->width()-1, m_proxy->height()-2);
            if(!m_drag_top)
                for(int x = m_proxy->width()/4; x < m_proxy->width()-4; x += m_proxy->width()/4)
                    p.drawPolygon(QList<QPoint>() << QPoint(x, m_proxy->height()-2) << QPoint(x-4, m_proxy->height()-10) << QPoint(x+4, m_proxy->height()-10));
        }
    }
    // qCDebug(g_cat_interp_drag) << "Accepting Drops" << this->acceptDrops() << m_myself->getTarget_lv2()->text();
}
void M1MidPlane::Interp::resizeEvent(QResizeEvent *p_event){
    qCDebug(g_cat_interp_base) << m_myself->dbgShort() << " resized: " << p_event->size();
}

void M1MidPlane::Interp::mouseDoubleClickEvent(QMouseEvent *p_event){
    qCDebug(g_cat_interp_base) << m_myself->dbgShort() << " double click: " << p_event->pos();
    emit gotoVertex(m_myself->getTarget_lv2(), this);
}

QString base_html_fragment(M1Store::Item_lv2* p_myself, const QString& p_class_name=QString()){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("base_html_fragment: %1").arg(p_myself->getTarget_lv2()->text()))
    QStringList l_edge_list;
    l_edge_list.append(QString("<tr><td>ID</td><td>:</td><td>%1</td></tr>\n").arg(p_myself->item_id()));
    l_edge_list.append(QString("<tr><td>Type</td><td>:</td><td>%1</td></tr>\n").arg(p_myself->dbgTypeShort()));
    QString l_edge_html = QString("<table>%1</table>\n").arg(l_edge_list.join("\n"));
    QString l_html;
    if((p_myself->flags() & M1Env::ITEM_NATURE_MASK) == M1Env::FULL_EDGE){
        QString l_target_html = p_myself->getTarget_lv2()->dbgStringHtml();
        qCDebug(g_cat_tree_display) << l_target_html;
        l_html =
            QString("<p style=\"font-size: larger;\">%2</p>\n<p style=\"font-weight: bold;\">Edge:</p>\n%1\n")
                     .arg(l_edge_html)
                     .arg(p_class_name.length() > 0 ? p_class_name : "Interp Base") +
                 QString("<p style=\"font-weight: bold;\">Target:</p>\n<p>%1</p>\n").arg(l_target_html);
    }else{
        // Simple Edge
        QString l_payload_html = p_myself->text();
        qCDebug(g_cat_tree_display) << l_payload_html;
        l_html = QString("<p style=\"font-weight: bold;\">Edge:</p>\n%1").arg(l_edge_html) +
            QString("<p style=\"font-weight: bold;\">Payload: %1</p>\n").arg(l_payload_html);
    }
    M1_FUNC_EXIT
    return QString("<div style=\"font-family: 'Noto mono', Courier New, monospace;\">%1</div>\n").arg(l_html);
}

QString M1MidPlane::Interp::getHtml(){
    return g_html_template.arg(base_html_fragment(m_myself));
    // return QString("<html>\n<Head></Head>\n<body>\n%1</body>\n</html>\n").arg(base_html_fragment(m_myself));
}

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
    m_text_edit->setPlainText(m_myself->getTarget_lv2()->text());

    return l_panel_widget;
}
void M1MidPlane::Interp::save_text_edit(){
    qCDebug(g_cat_interp_base) << "Saving text edit field: " << m_text_edit->toPlainText();
    m_myself->getTarget_lv2()->setText(m_text_edit->toPlainText());
    m_td_parent->repaint();
    this->emitSignals();
    this->m_proxy->setFocus(Qt::OtherFocusReason);
}

void M1MidPlane::Interp::focusInEvent(QFocusEvent *p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Interp::focusInEvent()"))
    qCDebug(g_cat_interp_drag) << "focus in: " << p_event->reason() << this->dbgString();
    if(m_proxy != nullptr){
        qCDebug(g_cat_interp_base) << " Background color: " << m_proxy->palette().color(QPalette::Window) << this->dbgString();

        m_proxy->setAutoFillBackground(true);
        m_proxy->repaint();
    }
    M1_FUNC_EXIT
}
void M1MidPlane::Interp::focusOutEvent(QFocusEvent *p_event){
    qCDebug(g_cat_interp_base) << "focus out: " << p_event->reason() << this->dbgString();
    m_proxy->setAutoFillBackground(false);
    m_proxy->repaint();
}

void M1MidPlane::Interp::initiateDrag(){
    Drag *l_drag = new Drag(this, m_td_parent);

    QString l_payload = QString("%1").arg(m_myself->getTarget_lv2()->item_id());
    QMimeData *l_data = new QMimeData;
    l_data->setText(l_payload);
    qCDebug(g_cat_interp_drag) << QString("Drag INITIATION ") << m_proxy->acceptDrops() << l_payload << l_data->formats();

    l_drag->setMimeData(l_data);
    m_proxy->setAcceptDrops(false);
    m_td_parent->setBeingDragged(this);
    l_drag->exec();
}
M1MidPlane::Drag::~Drag(){
    qCDebug(g_cat_interp_drag) << "DRAG end: " << this->mimeData()->text();
    m_td_parent->restoreAcceptDrop();
}
void M1MidPlane::Interp::restore_acept_drops(){
    qCDebug(g_cat_interp_drag) << "Accept drops again " << this->dbgString();
    if(m_proxy != nullptr) m_proxy->setAcceptDrops(true);
}
void M1UI::TreeDisplay::restoreAcceptDrop(){
    if(m_being_dragged != nullptr) m_being_dragged->restore_acept_drops();
    m_being_dragged = nullptr;
}

void M1MidPlane::Interp::emitSignals(){
    emit emitHtml(getHtml());
    emit emitEdit(get_edit_widget());
}

void M1MidPlane::Interp::mousePressEvent(QMouseEvent *p_event){
    qCDebug(g_cat_interp_drag) << m_myself->dbgShort() << " mouse press: " << p_event->pos();
    if(m_oc_x < p_event->pos().x() && p_event->pos().x() < m_oc_x + m_target_height){
        qCDebug(g_cat_interp_base) << m_myself->dbgShort() << " OC clicked";
        if(m_myself->flags() & M1Env::EDGE_IS_OPEN)
            m_myself->unSetFlag(M1Env::EDGE_IS_OPEN);
        else
            m_myself->setFlag(M1Env::EDGE_IS_OPEN);
        qCDebug(g_cat_interp_drag) << QString("GotoVertex after O/C BEFORE") << this->dbgString();
        emit gotoVertex(nullptr, this);
        qCDebug(g_cat_interp_drag) << QString("GotoVertex after O/C AFTER") << this->dbgString();
    }
    else {
        // initiate Drag/Drop
        m_hold_timer.start(HOLD_DELAY);
    }
}
void M1MidPlane::Interp::handleMouseHold(){
    qCDebug(g_cat_interp_drag) << QString("after %1 ms --> initiate DRAG").arg(HOLD_DELAY) << dbgString();
    m_hold_timer.stop();
    // initiate Drag/Drop
    this->initiateDrag();
}

void M1MidPlane::Interp::mouseReleaseEvent(QMouseEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("mouse release") << this->dbgString();
    m_hold_timer.stop();
    this->emitSignals();
}
/*
void M1MidPlane::Interp::mouseMoveEvent(QMouseEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("INTERP mouse move event") << this->acceptDrops() << m_myself->getTarget_lv2()->text() << p_event->position();
}*/

void M1MidPlane::Interp::dragEnterEvent(QDragEnterEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("Drag ENTER p_event") << p_event->mimeData()->text() << p_event->mimeData()->formats() << m_myself->getTarget_lv2()->text();
    p_event->setAccepted(true);
}
void M1MidPlane::Interp::dragMoveEvent(QDragMoveEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("Drag MOVE p_event") << p_event->mimeData()->text() << p_event->mimeData()->formats()
                               << m_myself->getTarget_lv2()->text() << p_event->position().toPoint() << m_proxy->rect();

    m_drag_top = false;
    m_drag_bottom = false;
    if(p_event->position().toPoint().y() < 7 ) m_drag_top = true;
    else if(p_event->position().toPoint().y() > m_proxy->height() - 7 ) m_drag_bottom = true;
    else {
        m_drag_top = true;
        m_drag_bottom = true;
    }
    m_proxy->repaint();
    p_event->setAccepted(true);
}
void M1MidPlane::Interp::dragLeaveEvent(QDragLeaveEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("Drag LEAVE p_event") << m_myself->getTarget_lv2()->text();
    m_drag_top = false;
    m_drag_bottom = false;

    m_proxy->repaint();
}
void M1MidPlane::Interp::dropEvent(QDropEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("DROP p_event") << p_event->mimeData()->text() << p_event->mimeData()->formats() << m_myself->text();
    M1Env::ItemID l_payload_item_id = QString(p_event->mimeData()->text()).toLongLong();
    M1Store::Item_lv2* l_item_to = M1Store::Item_lv2::getExisting(l_payload_item_id);

    if(m_drag_top && m_drag_bottom){
        M1Store::Item_lv2* l_item_from = m_myself->getTarget_lv2();
        qCDebug(g_cat_interp_drag) << QString("Creating edge of type") << m_td_parent->newEdgeType()->mnemonic() <<
            " From " << l_item_from->dbgShort() << " to " << l_item_to->dbgShort();
        l_item_from->linkTo(l_item_to, m_td_parent->newEdgeType()->specialId());
    }
    else{
        M1Store::Item_lv2* l_item_from = m_myself->getOrigin_lv2();
        qCDebug(g_cat_interp_drag) << QString("Creating edge of type") << m_td_parent->newEdgeType()->mnemonic() <<
            " From " << l_item_from->dbgShort() << " to " << l_item_to->dbgShort();
        if(m_drag_bottom)
            l_item_from->linkTo(l_item_to, m_td_parent->newEdgeType()->specialId(), m_myself, false);
        else
            l_item_from->linkTo(l_item_to, m_td_parent->newEdgeType()->specialId(), m_myself->get_previous_lv2(), false);
    }

    m_drag_top = false;
    m_drag_bottom = false;

    // m_proxy->repaint();
    emit gotoVertex(nullptr, this);
}

void M1MidPlane::Interp::contextMenuEvent(QContextMenuEvent *p_event) {
    qCDebug(g_cat_interp_base) << QString("Context menu request") << m_myself->text();
    m_hold_timer.stop();

    QMenu l_context_menu(m_proxy);
    QAction *l_new_descendant_action = l_context_menu.addAction("Create New Descendant");
    connect(l_new_descendant_action, &QAction::triggered,
            this,                    &M1MidPlane::Interp::create_descendant);

    l_context_menu.exec(p_event->globalPos());
}

void M1MidPlane::Interp::create_descendant(){
    M1Store::SpecialItem* l_new_edge_type = m_td_parent->newEdgeType();
    M1Store::SpecialItem* l_new_vertex_type = m_td_parent->newVertexType();
    qCDebug(g_cat_interp_base) << QString("Create New Descendant") << m_myself->text() <<
        "Edge Type:" << l_new_edge_type->mnemonic() <<
        "Vertex Type:" << l_new_vertex_type->mnemonic();

    m_myself->getTarget_lv2()->create_descendant(
        l_new_edge_type->specialId(),
        "New",
        l_new_vertex_type->specialId()
    );
    m_td_parent->gotoVertex(nullptr, this);
    // emit gotoVertex(nullptr);
}
// ------------------------------------ AutoInterp -----------------------------------------------------
bool M1MidPlane::AutoInterp::wantIt(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Getting appropriate Interp from: %1").arg(p_myself->dbgShort()))
    bool l_is_auto = p_myself->isOfType(M1Env::AUTO_SIID);
    qCDebug(g_cat_interp_base) << QString("p_myself->isOfType(M1Env::AUTO_SIID): %1").arg(l_is_auto ? "True" : "False");
    qCDebug(g_cat_interp_base) << QString("p_myself->isFullEdge()              : %1").arg(p_myself->isFullEdge() ? "True" : "False");
    M1_FUNC_EXIT
    return p_myself->isFullEdge() && p_myself->isOfType(M1Env::AUTO_SIID);
}
M1MidPlane::AutoInterp::AutoInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("AutoInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
void M1MidPlane::AutoInterp::paintEvent(QPaintEvent* p_event){
    M1MidPlane::Interp::paintEvent(p_event);
    QPainter p(m_proxy);
    p.setPen(Qt::darkGray);
    p.drawLine(QPoint(0,0), QPoint(m_proxy->width(),0));
    p.drawLine(QPoint(0, m_target_height-1), QPoint(m_proxy->width(), m_target_height-1));
}

QString M1MidPlane::AutoInterp::getHtml(){
    return g_html_template.arg(base_html_fragment(m_myself, "AutoInterp"));
}
// ------------------------------------ FieldInterp -----------------------------------------------------
bool M1MidPlane::FieldInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isSimpleEdge() || p_myself->getTarget_lv2()->isSimpleVertex();
}
M1MidPlane::FieldInterp::FieldInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("FieldInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::FieldInterp::displayText(){
    if(m_myself->isSimpleEdge())
        return QString("e[%1] ").arg(m_myself->dbgTypeShort()) + m_myself->text();
    else
        return QString("v[%1] ").arg(m_myself->getTarget_lv2()->dbgTypeShort()) + m_myself->getTarget_lv2()->text();
}

QIcon* M1MidPlane::FieldInterp::edgeIcon(){
    static QIcon ls_field_icon(M1Env::CROOKED_ICON_PATH);
    return &ls_field_icon;
}
QIcon* M1MidPlane::FieldInterp::vertexIcon(){
    static QIcon ls_field_icon(M1Env::FIELD_ICON_PATH);
    return &ls_field_icon;
}

void M1MidPlane::FieldInterp::paintEvent(QPaintEvent* p_event){
    QPainter p(m_proxy);
    // QString l_text = QString("[%1] ").arg(m_myself->dbgTypeShort()) + m_myself->text();
    this->edgeIcon()->paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    // M1MidPlane::Interp::cm_closed.paint(&p, m_target_padding + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    this->vertexIcon()->paint(&p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), this->displayText());
}
QString M1MidPlane::FieldInterp::getHtml(){
    return g_html_template.arg(base_html_fragment(m_myself, "FieldInterp"));
}

// ------------------------------------ TranslUnit -----------------------------------------------------
QString g_transl_template = "<tr><td style=\"font-family: 'Noto Sans', Arial, sans-serif; text-align: center;\">%1 (%2)</td></tr>\n";

QString tu_html_fragment(M1Store::Item_lv2* p_tu){
    qCDebug(g_cat_tree_display) << "p_tu         : " << p_tu->dbgShort(2);
    
    M1Store::Item_lv2* l_begin_word = p_tu->find_edge_edge(M1Store::TW_SECTION_2_OCC_BEGIN_SIID);
    M1Store::Item_lv2* l_end_word = p_tu->find_edge_edge(M1Store::TW_SECTION_2_OCC_END_SIID);

    if(l_begin_word != nullptr) l_begin_word = l_begin_word->getTarget_lv2();
    if(l_end_word != nullptr) l_end_word = l_end_word->getTarget_lv2();

    if(l_begin_word != nullptr) qCDebug(g_cat_tree_display) << "l_begin_word : " << l_begin_word->dbgShort(2);
    if(l_end_word != nullptr) qCDebug(g_cat_tree_display) << "l_end_word   : " << l_end_word->dbgShort(2);

    QString l_ground_text = "xx";
    QString l_translit = "yy";
    if(l_begin_word != nullptr){
        if(l_begin_word == l_end_word){
            l_ground_text = l_begin_word->getTarget_lv2()->text();
            l_translit = l_begin_word->getTarget_lv2()->getField(M1Store::TEXT_WORD_TRANSLIT_SIID);
        }
        else{
            l_ground_text = "";
            l_translit = "";
            for(M1Store::Item_lv2* l_occ = l_begin_word; true; l_occ = l_occ->get_next_lv2()){
                l_ground_text += QString("%1 ").arg(l_occ->getTarget_lv2()->text());
                l_translit += QString("%1 ").arg(l_occ->getTarget_lv2()->getField(M1Store::TEXT_WORD_TRANSLIT_SIID));
                if(l_occ == l_end_word) break;
            }
        }
    }
    l_ground_text = l_ground_text.trimmed();
    l_translit = l_translit.trimmed();

    QString l_translation_p = p_tu->getField(M1Store::TEXT_WFW_TRANSL_SIID, M1Store::TEXT_WFW_PRABUPADA_SIID);
    QString l_translation_s = p_tu->getField(M1Store::TEXT_WFW_TRANSL_SIID, M1Store::TEXT_WFW_SIVANANDA_SIID);
    return "<table style=\"display: inline-block; margin: 0 .5em 0 0;\">\n" +
        QString("<tr><td style=\"font-family: 'Noto Serif', 'Times New Roman', serif; text-align: center; color: maroon;\">%1</td></tr>\n").arg(l_ground_text) +
        QString("<tr><td style=\"font-style: italic; text-align: center;\">%1</td></tr>\n").arg(l_translit) +
       (l_translation_p.length() > 0 ? QString(g_transl_template).arg(l_translation_p).arg("P") : "") +
       (l_translation_s.length() > 0 ? QString(g_transl_template).arg(l_translation_s).arg("S") : "") +
        "</table>\n";
}

bool M1MidPlane::TranslUnit::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_WFW_UNIT_SIID);
}

M1MidPlane::TranslUnit::TranslUnit(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TranslUnit Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::TranslUnit::displayText(){
    return QString("%1 [%2]")
        .arg(m_myself->getTarget_lv2()->text())
        .arg(m_myself->getTarget_lv2()->getField(M1Store::TEXT_WFW_TRANSL_SIID, true));
}
QString M1MidPlane::TranslUnit::getHtml(){
    return g_html_template.arg(tu_html_fragment(m_myself->getTarget_lv2()) +
                               "<hr/>\n" +
                               base_html_fragment(m_myself, "TranslUnit"));
}

// ------------------------------------ SectionBeginEnd -----------------------------------------------------
bool M1MidPlane::SectionBeginEnd::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() &&
           (p_myself->isOfType(M1Env::TW_SECTION_2_OCC_BEGIN_SIID) || p_myself->isOfType(M1Env::TW_SECTION_2_OCC_END_SIID));
}
M1MidPlane::SectionBeginEnd::SectionBeginEnd(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SectionBeginEnd Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::SectionBeginEnd::displayText(){
    return QString("%1 [%2]")
        .arg(m_myself->getTarget_lv2()->getTarget_lv2()->text())
        .arg(m_myself->getTarget_lv2()->getTarget_lv2()->getField(M1Store::TEXT_WORD_TRANSLIT_SIID));
}
QString M1MidPlane::SectionBeginEnd::getHtml(){
    return g_html_template.arg(base_html_fragment(m_myself, "SectionBeginEnd"));
}

//------------------------------------ UrlInterp -----------------------------------------------------
bool M1MidPlane::UrlInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_URL_LINK_SIID);
}
QString M1MidPlane::UrlInterp::getHtml(){
    return m_url;
}
M1MidPlane::UrlInterp::UrlInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("UrlInterp Constructor from: %1").arg(p_myself->dbgShort()))

    m_label = m_myself->getTarget_lv2()->text();
    m_url = m_myself->getTarget_lv2()->getField(M1Env::TEXT_URL_LINK_SIID);
    M1_FUNC_EXIT
}
/*
QString M1MidPlane::UrlInterp::getHtml(){
    return g_html_template.arg("<p style=\"font-size: larger;\">UrlInterp</p>\n" + base_html_fragment(m_myself));
}
*/
//------------------------------------ BhashyaTranslation -----------------------------------------------------
QString bt_html_fragment(M1Store::Item_lv2* p_bt){
    M1Store::Item_lv2* l_author_edge = p_bt->find_edge_edge(M1Store::TEXT_WRITTEN_BY_SIID);
    QString l_author = l_author_edge->getTarget_lv2()->text();
    QString l_text = p_bt->text();
    QString l_type = p_bt->isOfType(M1Env::TEXT_SLOKA_BHASHYA_SIID) ? "Bhashya" : "Translation";
    return QString("<div style=\"margin-bottom: 1em;\"><span style=\"font-weight: bold;\">%1</span>: ").arg(l_type) +
           QString("%1 <span style=\"font-style: italic; font-size: smaller; color: maroon;\">%2</span></div>").arg(l_text).arg(l_author);
}

bool M1MidPlane::BhashyaTranslation::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() &&
           (p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_BHASHYA_SIID) ||
                                      p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_TRANSLATION_SIID));
}
QString M1MidPlane::BhashyaTranslation::getHtml(){
    return g_html_template.arg(bt_html_fragment(m_myself->getTarget_lv2()) +
                               "<hr/>\n" +
                               base_html_fragment(m_myself, "BhashyaTranslation"));
}
QString M1MidPlane::BhashyaTranslation::displayText(){
    QString l_text = m_myself->getTarget_lv2()->text();
    if(l_text.length() > 100) l_text = l_text.left(100) + "...";
    return l_text;
}
M1MidPlane::BhashyaTranslation::BhashyaTranslation(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("UrlInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

//------------------------------------ ChunkInterp -----------------------------------------------------
QString ck_html_fragment(M1Store::Item_lv2* p_si){
    QStringList l_raw;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge() && !it.at()->isOfType(M1Env::BLNGS_SIID) )
            l_raw.append(it.at()->getTarget_lv2()->text());
    }

    QString l_html = QString("<span style=\"font-family: 'Noto Serif', 'Times New Roman', serif;\">") + l_raw.join(" ") + "</span>";

    return l_html;
}
bool M1MidPlane::ChunkInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_CHUNK_SIID);
}
QString M1MidPlane::ChunkInterp::getHtml(){
    QString l_html = "<p>" + ck_html_fragment(m_myself->getTarget_lv2()) + "</p>\n";
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "ChunkInterp");
    return g_html_template.arg(l_html);
}
M1MidPlane::ChunkInterp::ChunkInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("ChunkInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

//------------------------------------ SlokaInterp -----------------------------------------------------
QString sk_html_fragment(M1Store::Item_lv2* p_si){
    QString l_html_wfw;
    QString l_html_lines;
    QString l_html_translations;
    QString l_html_bhashyas;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_WFW_UNIT_SIID))
            l_html_wfw += tu_html_fragment(it.at()->getTarget_lv2());
        else if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_LINE_SIID))
            l_html_lines += QString("<p style=\"margin: 0; padding: 0;\">%1</p>\n").arg(it.at()->getTarget_lv2()->text());
        else if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_TRANSLATION_SIID))
            l_html_translations += bt_html_fragment(it.at()->getTarget_lv2());
        else if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_BHASHYA_SIID))
            l_html_bhashyas += bt_html_fragment(it.at()->getTarget_lv2());
    }
    qCDebug(g_cat_interp_base) << QString("sk_html_fragment") << p_si->dbgShort() << l_html_wfw;
    QString l_html = QString("<p style=\"font-weight: bold;\">%1</p>\n") .arg(p_si->text()) +
                 QString("<div style=\"margin-bottom: 1em;\">%1</div>\n<div style=\"margin-bottom: 1em; background-color: SeaShell;\">%2</div>\n")
                     .arg(l_html_lines)
                     .arg(l_html_wfw)
                 + l_html_translations + l_html_bhashyas;

    return l_html;
}
bool M1MidPlane::SlokaInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::SLOKA_SIID);
}

M1MidPlane::SlokaInterp::SlokaInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SlokaInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

QString M1MidPlane::SlokaInterp::getHtml(){
    QString l_html = sk_html_fragment(m_myself->getTarget_lv2());
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "SlokaInterp");
    return g_html_template.arg(l_html);
}
//------------------------------------ SentenceInterp -----------------------------------------------------
QString M1MidPlane::SentenceInterp::occur_to_text(const M1Store::Item_lv2* p_occur_edge){
    QString l_text(p_occur_edge->getTarget_lv2()->text());
    if(p_occur_edge->getField(M1Store::CAPTL_SIID) == "true")
        l_text.front() = l_text.front().toUpper();
    return p_occur_edge->getField(M1Store::PCTLF_SIID) + l_text + p_occur_edge->getField(M1Store::PCTRT_SIID);
}
QString st_html_fragment(M1Store::Item_lv2* p_si){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("p_si: %1").arg(p_si->text()))
    QStringList l_word_list;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID)){
            QString l_text = M1MidPlane::SentenceInterp::occur_to_text(it.at()->getTarget_lv2());
            qCDebug(g_cat_interp_base) << "l_text" << l_text << it.at()->getTarget_lv2()->dbgShort();
            if(M1Store::Item_lv2* l_section = it.at()->getTarget_lv2()->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID); l_section != nullptr){
                l_word_list.append(QString("<span style=\"font-size: smaller; color: maroon; font-weight: bold;\">%1</span> %2")
                                       .arg(l_section->getTarget_lv2()->text())
                                       .arg(l_text));
            }
            else{
                l_word_list.append(l_text);
            }
        }
    }
    M1_FUNC_EXIT
    return QString("<p>%1</p>").arg(l_word_list.join(" "));
}
bool M1MidPlane::SentenceInterp::wantIt(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Is a sentence? %1")
                                         .arg(p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SENTENCE_SIID) ? "true" : "false"))
    M1_FUNC_EXIT
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SENTENCE_SIID);
}
M1MidPlane::SentenceInterp::SentenceInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : ChunkInterp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SentenceInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::SentenceInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Sent: %1").arg(m_myself->text()))
    QString l_html = st_html_fragment(m_myself->getTarget_lv2());
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "SentenceInterp");
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QWidget *M1MidPlane::SentenceInterp::get_edit_widget(){
    // find previous Stephanus Number
    M1Store::Item_lv2* l_current_edge = m_myself->getTarget_lv2()->find_edge(M1Env::OWNS_SIID, M1Env::OCCUR_SIID)->getTarget_lv2();
    qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
    M1Store::Item_lv2* l_section;
    while(true){
        qCDebug(g_cat_interp_base) << QString("l_current_edge") << l_current_edge->dbgShort();
        qCDebug(g_cat_interp_base) << QString("   previous") << l_current_edge->get_previous_lv2()->dbgShort();
        qCDebug(g_cat_interp_base) << QString("   next    ") << l_current_edge->get_next_lv2()->dbgShort();
        l_section = l_current_edge->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID);
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

    // Set up UI
    QWidget* l_panel_widget = new QWidget();
    QVBoxLayout* l_panel_layout = new QVBoxLayout();
    l_panel_widget->setLayout(l_panel_layout);

    QWidget* l_button_bar = new QWidget(l_panel_widget);
    l_panel_layout->addWidget(l_button_bar);
    QHBoxLayout* l_bar_layout = new QHBoxLayout();
    l_button_bar->setLayout(l_bar_layout);

    QPushButton* l_btn0 = new QPushButton("< 10", l_button_bar);
    QPushButton* l_btn1 = new QPushButton("< 1", l_button_bar);
    QPushButton* l_btn2 = new QPushButton("> 1", l_button_bar);
    QPushButton* l_btn3 = new QPushButton("> 10", l_button_bar);
    QPushButton* l_btn4 = new QPushButton("Highlight", l_button_bar);
    l_btn4->setEnabled(false);

    QComboBox* l_cat_combo = new QComboBox(l_button_bar);
    // fill categories combo box
    M1Store::Item_lv2* l_republic = l_greek_start->getOrigin_lv2();
    M1Store::Item_lv2* l_cat_folder = l_republic->find_edge(M1Env::OWNS_SIID, M1Env::TEXT_HIGHLIGHT_CAT_FLDR_SIID)->getTarget_lv2();
    qCDebug(g_cat_interp_base) << QString("l_cat_folder") << l_cat_folder->dbgShort();

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

    l_bar_layout->addWidget(l_btn0);
    l_bar_layout->addWidget(l_btn1);
    l_bar_layout->addWidget(l_btn2);
    l_bar_layout->addWidget(l_btn3);
    l_bar_layout->addWidget(l_btn4);
    l_bar_layout->addWidget(l_cat_combo);
    l_bar_layout->addStretch(1);

    M1UI::Scene* l_scene = new M1UI::Scene();
    l_scene->setBackgroundBrush(Qt::white);

    M1UI::PassagesPanel* l_passages_panel = new M1UI::PassagesPanel(
        l_republic->find_edge(M1Env::OWNS_SIID, M1Env::TEXT_HIGHLIGHT_FLDR_SIID)->getTarget_lv2(),
        l_cat_list
    );
    l_scene->addItem(l_passages_panel);

    l_passages_panel->add_passage_editor(new M1UI::PassageEditor(l_jowett_start, "A", l_passages_panel));
    l_passages_panel->add_passage_editor(new M1UI::PassageEditor(l_shorey_start, "B", l_passages_panel));
    l_passages_panel->add_passage_editor(new M1UI::PassageEditor(l_greek_start, "C", l_passages_panel));

    QObject::connect(l_btn0, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_backwards_ten);
    QObject::connect(l_btn1, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_backwards_one);
    QObject::connect(l_btn2, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_forward_one);
    QObject::connect(l_btn3, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_forward_ten);
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

//------------------------------------ BookInterp -----------------------------------------------------
bool M1MidPlane::BookInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_BOOK_SIID);
}
M1MidPlane::BookInterp::BookInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : ChunkInterp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("BookInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString bk_html_fragment(M1Store::Item_lv2* p_si){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("p_si: %1").arg(p_si->text()))
    QStringList l_word_list;
    QString l_cur_version;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SENTENCE_SIID)){
            M1Store::Item_lv2* l_version = it.at()->getTarget_lv2()->find_edge(M1Env::ISA_SIID, M1Env::TXTVR_SIID, true);
            QString l_version_text = l_version->getTarget_lv2()->text();
            if(l_version_text != l_cur_version){
                l_cur_version = l_version_text;
                l_word_list.append(QString("<p style=\"border-top: 1px solid black; font-size: 60%;\">%1<p>").arg(l_version_text));
            }
            l_word_list.append(st_html_fragment(it.at()->getTarget_lv2()));
        }
    }
    M1_FUNC_EXIT
    return QString("<h3>%1</h3>").arg(p_si->text()) + l_word_list.join("\n");
}
QString M1MidPlane::BookInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Bk: %1").arg(m_myself->getTarget_lv2()->text()))
    QString l_html = bk_html_fragment(m_myself->getTarget_lv2());
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "BookInterp");
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);
}
//------------------------------------ TextInterp -----------------------------------------------------
QString hc_html_fragment_words(M1Store::Item_lv2* p_si){
    QStringList l_word_list;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID))
            l_word_list.append(M1MidPlane::SentenceInterp::occur_to_text(it.at()->getTarget_lv2()));

    std::reverse(l_word_list.begin(), l_word_list.end());
    return l_word_list.join(" ");
}

QString hq_html_fragment(M1Store::Item_lv2* p_si){
    QStringList l_chunk_list;
    bool l_is_first = true;
    QString m_ground_text;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_CHUNK_SIID)){
            if(l_is_first){
                l_is_first = false;
                m_ground_text = hc_html_fragment_words(it.at()->getTarget_lv2());
            }
            else
                l_chunk_list.append(hc_html_fragment_words(it.at()->getTarget_lv2()));
        }

    return QString("<div class=\"tooltip\" style=\"font-style: italic; font-family: 'Noto Serif', 'Times New Roman', serif;\">%1").arg(m_ground_text) +
           QString("<span class=\"tooltiptext\" style=\"font-style: italic; font-family: 'Noto Sans', 'Arial', sans-serif;\">%1</span>").arg(l_chunk_list.join("/")) +
           "</div>";
    // return QString("<span style=\"font-style: italic; font-family: 'Noto Serif', 'Times New Roman', serif;\">%1</span>").arg(l_chunk_list.join("/"));
}

bool M1MidPlane::TextInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SIID);
}
QString M1MidPlane::TextInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextInterp::getHtml()"))
    QString l_html = QString("<h1>Title: %1</h1>\n").arg(m_myself->getTarget_lv2()->text());

    QStringList l_frags;
    M1Store::Item_lv2* l_folder = m_myself->getTarget_lv2()->find_edge_target(M1Env::TEXT_SLOKA_FLDR_SIID);
    qCDebug(g_cat_interp_base) << QString("l_folder") << ((l_folder == nullptr) ? "" : l_folder->dbgShort());
    if(l_folder != nullptr){
        l_folder = l_folder->getTarget_lv2();
        for(M1Store::Item_lv2_iterator it = l_folder->getIteratorTop(); !it.beyondEnd(); it.next()){
            if(it.at()->getTarget_lv2()->isOfType(M1Store::SLOKA_SIID))
                l_frags.append(sk_html_fragment(it.at()->getTarget_lv2()));
        }
    }
    if(l_frags.count() == 0)
        for(M1Store::Item_lv2_iterator it = m_myself->getTarget_lv2()->getIteratorTop(); !it.beyondEnd(); it.next()){
            if(it.at()->getTarget_lv2()->isOfType(M1Store::TEXT_CHUNK_SIID))
                l_frags.append(ck_html_fragment(it.at()->getTarget_lv2()));
            else if(it.at()->isOfType(M1Store::TEXT_HIGHLIGHT_QUOTE_SIID))
                l_frags.append(hq_html_fragment(it.at()->getTarget_lv2()));
        }

    l_html += l_frags.join(" ");
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "TextInterp");
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);
}

M1MidPlane::TextInterp::TextInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

//------------------------------------ TextOccurrence -----------------------------------------------------
QStringList textOccData(M1Store::Item_lv2* p_occ){
    QStringList l_ret;
    M1Store::Item_lv2* l_snt = p_occ->find_edge(M1Store::BLNGS_SIID, M1Store::TEXT_SENTENCE_SIID);
    M1Store::Item_lv2* l_book = l_snt->getTarget_lv2()->find_edge(M1Store::BLNGS_SIID, M1Store::TEXT_BOOK_SIID);

    // sentence name
    l_ret.append(l_snt == nullptr ? "" : l_snt->getTarget_lv2()->text());

    // book name
    l_ret.append(l_book == nullptr ? "" : l_book->getTarget_lv2()->text());

    // section
    M1Store::Item_lv2* l_sec_search_occ = p_occ;
    M1Store::Item_lv2* l_sec_edge = nullptr;
    while(true){
        // qCDebug(g_cat_interp_dev).noquote() << QString("l_sec_search_occ") << l_sec_search_occ->dbgShort();
        l_sec_edge = l_sec_search_occ->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID);
        if(l_sec_edge != nullptr){
            qCDebug(g_cat_interp_dev).noquote() << QString("l_sec_edge") << l_sec_edge->dbgShort();
            l_ret.append(l_sec_edge->getTarget_lv2()->text());
            break;
        }
        if(l_sec_search_occ->previous_item_id() == M1Store::G_VOID_ITEM_ID) break;
        l_sec_search_occ = l_sec_search_occ->get_previous_lv2();
    }
    if(l_ret.length() < 3) l_ret.append("{No Stephanus Section Found}");

    // neighborhood
    M1Store::Item_lv2* l_nig_search_occ = p_occ;
    for(int i=0; i<5; i++)
        if(l_nig_search_occ->previous_item_id() == M1Store::G_VOID_ITEM_ID) break;
        else l_nig_search_occ = l_nig_search_occ->get_previous_lv2();
    QStringList l_words;
    for(int i=0; i<11; i++)
        if(l_nig_search_occ->next_item_id() == M1Store::G_VOID_ITEM_ID) break;
        else{
            if(l_nig_search_occ == p_occ) l_words.append(QString("<b>%1</b>").arg(M1MidPlane::SentenceInterp::occur_to_text(l_nig_search_occ)));
            else l_words.append(M1MidPlane::SentenceInterp::occur_to_text(l_nig_search_occ));
            l_nig_search_occ = l_nig_search_occ->get_next_lv2();
        }
    l_ret.append(l_words.join(" "));

    return l_ret;
}

QString lemma_html_fragment(M1Store::Item_lv2* p_lemma){
    QString l_html;
    if(p_lemma == nullptr)
        return("<p>No Lemma</p>");

    // qCDebug(g_cat_interp_base).noquote() << QString("l_lemma->text()") << l_lemma->text();
    M1Store::Item_lv2* l_pos_lemma = p_lemma->find_edge(M1Env::ISA_SIID, M1Env::NLPOS_SIID, true);
    l_html += QString("<p>Lemma: <b>%1</b> (%2)</p>\n")
                  .arg(p_lemma->text())
                  .arg(l_pos_lemma == nullptr ? "<No POS tag>" : l_pos_lemma->getTarget_lv2()->text());

    l_html += "<p>Other occurrences of this Lemma:</p>\n";
    l_html += "<table class=\"wb\" style=\"border: 1px solid black; border-collapse: collapse; font-size:smaller;\">\n";
    qCDebug(g_cat_interp_dev).noquote() << QString("l_html") << l_html;
    for(M1Store::Item_lv2_iterator l_it_1 = p_lemma->getIteratorTop(); !l_it_1.beyondEnd(); l_it_1.next())
        if(l_it_1.at()->getTarget_lv2()->isOfType(M1Store::WFORM_SIID)){
            M1Store::Item_lv2* l_form = l_it_1.at()->getTarget_lv2();
            qCDebug(g_cat_interp_dev) << QString("l_form") << l_form->dbgShort();
            for(M1Store::Item_lv2_iterator l_it_2 = l_form->getIteratorTop(); !l_it_2.beyondEnd(); l_it_2.next())
                if(l_it_2.at()->getTarget_lv2()->isOfType(M1Store::OCCUR_SIID)){
                    M1Store::Item_lv2* l_occ = l_it_2.at()->getTarget_lv2();
                    QStringList l_occ_data = textOccData(l_occ);
                    l_html += QString("<tr><td class=\"wb\">%1</td><td class=\"wb\">%2</td><td class=\"wb\">%3</td><td class=\"wb\" style=\"color: maroon;\">%4</td><td class=\"wb\">%5</td></tr>\n")
                                  .arg(l_form->text())
                                  .arg(l_occ_data[1])
                                  .arg(l_occ_data[0])
                                  .arg(l_occ_data[2])
                                  .arg(l_occ_data[3]);
                    qCDebug(g_cat_interp_dev) << QString("l_occ") << l_occ->dbgShort();
                }
        }

    l_html += "</table>\n";
    return l_html;
}

bool M1MidPlane::TextOccurrence::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID);
}
M1MidPlane::TextOccurrence::TextOccurrence(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextOccurrence Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::TextOccurrence::displayText(){
    return M1MidPlane::SentenceInterp::occur_to_text(m_myself->getTarget_lv2());
}
QString M1MidPlane::TextOccurrence::getHtml(){
    M1Store::Item_lv2* l_form = m_myself->getTarget_lv2()->getTarget_lv2();
    QStringList l_occ_data = textOccData(m_myself->getTarget_lv2());
    QString l_html = QString("<p>Occurrence of “<b>%1</b>” in [%2] of %3 (Stephanus Section: <span style=\"color: maroon;\">%4</span>) of <span style=\"font-style: italic;\">%5</span></p>\n")
                         .arg(l_form->text()).arg(l_occ_data[0]).arg(l_occ_data[1]).arg(l_occ_data[2])
                         .arg(m_myself->getTarget_lv2()->getOrigin_lv2()->text());

    l_html += "<table class=\"wb\" style=\"border: 1px solid black; border-collapse: collapse; font-size:smaller;\">\n";
    l_html += "<tr><th class=\"wb\">Attribute</th><th class=\"wb\">Value</th><tr/>\n";
    QStringList l_other_attr;
    for(M1Store::Item_lv2_iterator it = m_myself->getTarget_lv2()->getIteratorSpecial(); !it.beyondEnd(); it.next())
        if(it.at()->isOfType(M1Store::ISA_SIID)){
            M1Store::Item_lv2* l_type = it.at()->getTarget_lv2();
            qCDebug(g_cat_interp_dev) << QString("ISA edge") << l_type->text() << " / " << it.at()->dbgShort();

            if(l_type->isOfType(M1Store::NLPOS_SIID))
                l_html += QString("<tr class=\"wb\"><td class=\"wb\" style=\"background-color: #cccccc; font-weight: bold;\">Coarse Grained POS</td>") +
                          QString("<td class=\"wb\">%1</td></tr>\n").arg(l_type->text());

            if(l_type->isOfType(M1Store::NLTAG_SIID))
                l_html += QString("<tr class=\"wb\"><td class=\"wb\" style=\"background-color: #cccccc; font-weight: bold;\">Fine Grained POS</td>") +
                          QString("<td class=\"wb\">%1</td></tr>\n").arg(l_type->text());

            for(M1Store::Item_lv2_iterator it2 = l_type->getIteratorSpecial(); !it2.beyondEnd(); it2.next())
                if(it2.at()->isOfType(M1Store::ISA_SIID) && it2.at()->getTarget_lv2()->isOfType(M1Store::GRAMMAR_ATTR_SIID)){
                    M1Store::Item_lv2* l_gratt = it2.at()->getTarget_lv2();
                    qCDebug(g_cat_interp_dev) << QString("   gratt") << l_gratt->dbgShort();
                    l_other_attr.append(QString("<tr class=\"wb\"><td class=\"wb\" style=\"background-color: #cccccc; font-weight: bold;\">%1</td><td class=\"wb\">%2</td></tr>").arg(l_gratt->text()).arg(l_type->text()));
                    break;
                }
        }
    l_html += l_other_attr.join("\n") + "</table>\n";

    l_html += QString("<p>Neighborhood: %1</p>\n").arg(l_occ_data[3]);

    M1Store::Item_lv2* l_lemma = l_form->find_edge(M1Env::BLNGS_SIID, M1Env::LEMMA_SIID);
    if(l_lemma == nullptr)
        l_html += "<p>No Lemma</p>";
    else{
        l_lemma = l_lemma->getTarget_lv2();
        l_html += lemma_html_fragment(l_lemma);
    }
    /*
    if(l_lemma == nullptr)
        l_html += "<p>No Lemma</p>";
    else{
        l_lemma = l_lemma->getTarget_lv2();
        // qCDebug(g_cat_interp_base).noquote() << QString("l_lemma->text()") << l_lemma->text();
        M1Store::Item_lv2* l_pos_lemma = l_lemma->find_edge(M1Env::ISA_SIID, M1Env::NLPOS_SIID, true);
        l_html += QString("<p>Lemma: <b>%1</b> (%2)</p>\n")
                      .arg(l_lemma->text())
                      .arg(l_pos_lemma == nullptr ? "<No POS tag>" : l_pos_lemma->getTarget_lv2()->text());
    }

    l_html += "<p>Other occurrences of this Lemma:</p>\n";
    l_html += "<table class=\"wb\" style=\"border: 1px solid black; border-collapse: collapse; font-size:smaller;\">\n";
    qCDebug(g_cat_interp_dev).noquote() << QString("l_html") << l_html;
    for(M1Store::Item_lv2_iterator l_it_1 = l_lemma->getIteratorTop(); !l_it_1.beyondEnd(); l_it_1.next())
        if(l_it_1.at()->getTarget_lv2()->isOfType(M1Store::WFORM_SIID)){
            M1Store::Item_lv2* l_form = l_it_1.at()->getTarget_lv2();
            qCDebug(g_cat_interp_dev) << QString("l_form") << l_form->dbgShort();
            for(M1Store::Item_lv2_iterator l_it_2 = l_form->getIteratorTop(); !l_it_2.beyondEnd(); l_it_2.next())
                if(l_it_2.at()->getTarget_lv2()->isOfType(M1Store::OCCUR_SIID)){
                    M1Store::Item_lv2* l_occ = l_it_2.at()->getTarget_lv2();
                    QStringList l_occ_data = textOccData(l_occ);
                    l_html += QString("<tr><td class=\"wb\">%1</td><td class=\"wb\">%2</td><td class=\"wb\">%3</td><td class=\"wb\" style=\"color: maroon;\">%4</td><td class=\"wb\">%5</td></tr>\n")
                                  .arg(l_form->text())
                                  .arg(l_occ_data[1])
                                  .arg(l_occ_data[0])
                                  .arg(l_occ_data[2])
                                  .arg(l_occ_data[3]);
                    qCDebug(g_cat_interp_dev) << QString("l_occ") << l_occ->dbgShort();
                }
        }

    l_html += "</table>\n";
    */
    return g_html_template.arg(l_html + "<hr/>\n" + base_html_fragment(m_myself, "TextOccurrence"));
}

//------------------------------------ TextLemma -----------------------------------------------------
bool M1MidPlane::TextLemma::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::LEMMA_SIID);
}

QString M1MidPlane::TextLemma::getHtml(){
    return g_html_template.arg(lemma_html_fragment(m_myself->getTarget_lv2()) + "<hr/>\n" + base_html_fragment(m_myself, "TextLemma"));
}
M1MidPlane::TextLemma::TextLemma(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextLemma Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

//------------------------------------ TextLemma -----------------------------------------------------
bool M1MidPlane::TextWForm::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::WFORM_SIID);
}

QString M1MidPlane::TextWForm::getHtml(){
    M1Store::Item_lv2* l_pos_wform = m_myself->getTarget_lv2()->find_edge(M1Env::ISA_SIID, M1Env::NLPOS_SIID, true);
    QString l_html = QString("<p>Word Form: <b>%1</b>%2</p>\n")
                         .arg(m_myself->getTarget_lv2()->text())
                         .arg(l_pos_wform == nullptr ? "" : QString(" (%1)").arg(l_pos_wform->getTarget_lv2()->text()));

    M1Store::Item_lv2* l_lemma_edge = m_myself->getTarget_lv2()->find_edge(M1Env::BLNGS_SIID, M1Env::LEMMA_SIID);
    if(l_lemma_edge == nullptr)
        l_html += "<p>No Lemma</p>\n";
    else
        l_html += lemma_html_fragment(l_lemma_edge->getTarget_lv2());

    return g_html_template.arg(l_html + "<hr/>\n" + base_html_fragment(m_myself, "TextWForm"));
}
M1MidPlane::TextWForm::TextWForm(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextWForm Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
//------------------------------------ HighlightChunkInterp -----------------------------------------------------
bool M1MidPlane::HighlightChunkInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_CHUNK_SIID);
}

M1MidPlane::HighlightChunkInterp::HighlightChunkInterp(M1Store::Item_lv2* p_myself,
                                                       QVBoxLayout* p_vb,
                                                       M1UI::TreeDisplay* p_parent,
                                                       int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("HighlightChunkInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::HighlightChunkInterp::displayText(){
    QString l_ret(m_myself->getTarget_lv2()->text());
    l_ret += " : ";
    QStringList l_word_list;
    for(M1Store::Item_lv2_iterator it = m_myself->getTarget_lv2()->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->getTarget_lv2()->isOfType(M1Store::OCCUR_SIID))
            l_word_list.append(M1MidPlane::SentenceInterp::occur_to_text(it.at()->getTarget_lv2()));

    std::reverse(l_word_list.begin(), l_word_list.end());
    l_ret += l_word_list.join(" ");
    return l_ret.trimmed();
}

QString hc_html_fragment(M1Store::Item_lv2* p_si){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("p_si: %1").arg(p_si->text()))
    QString l_version_txt("No version");
    M1Store::Item_lv2* l_version = p_si->find_edge(M1Env::ISA_SIID, M1Env::TXTVR_SIID, true);
    if(l_version != nullptr) l_version_txt = l_version->getTarget_lv2()->text();

    M1_FUNC_EXIT
        return QString("<p>%1 (%2)</p>").arg(hc_html_fragment_words(p_si)).arg(l_version_txt);
}
QString M1MidPlane::HighlightChunkInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Hilight chunk: %1").arg(m_myself->text()))
    QString l_html = QString("<p>%1</p>").arg(m_myself->getTarget_lv2()->text()) + hc_html_fragment(m_myself->getTarget_lv2());
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "HighlightChunkInterp");
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);
}

//------------------------------------ HighlightInterp -----------------------------------------------------
bool M1MidPlane::HighlightInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_SIID);
}

M1MidPlane::HighlightInterp::HighlightInterp(M1Store::Item_lv2* p_myself,
                                             QVBoxLayout* p_vb,
                                             M1UI::TreeDisplay* p_parent,
                                             int p_depth) : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("HighlightInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::HighlightInterp::displayText(){
    return m_myself->getTarget_lv2()->text();
}
QString M1MidPlane::HighlightInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Hilight: %1").arg(m_myself->text()))
    QStringList l_chunk_list;
    for(M1Store::Item_lv2_iterator it = m_myself->getTarget_lv2()->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_CHUNK_SIID))
            l_chunk_list.append(hc_html_fragment(it.at()->getTarget_lv2()));

    M1Store::Item_lv2* l_category_edge = m_myself->getTarget_lv2()->find_edge(M1Env::ISA_SIID, M1Env::TEXT_HIGHLIGHT_CAT_SIID, true);
    QString l_cat_html;
    if(l_category_edge != nullptr){
        M1Store::Item_lv2* l_color_edge = l_category_edge->getTarget_lv2()->find_edge(M1Env::HLCLR_SIID, M1Env::HLCLR_SIID);
        if(l_color_edge != nullptr)
            l_cat_html = QString("Highlight (<span style=\"background-color:%1;\">%2</span>)")
                             .arg(l_color_edge->getTarget_lv2()->text())
                             .arg(l_category_edge->getTarget_lv2()->text());
    }

    QString l_html = QString("<div style=\"font-family: 'Noto Serif', 'Times New Roman', serif;\"><p>%1</p>").arg(l_cat_html) +
                     "<p>" + l_chunk_list.join("</p>\n<p>") + "</p></div>\n";
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "HighlightInterp");
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);;
}

//------------------------------------ HighlightQuotationInterp -----------------------------------------------------
bool M1MidPlane::HighlightQuotationInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() &&
           p_myself->isOfType(M1Env::TEXT_HIGHLIGHT_QUOTE_SIID) &&
           p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_SIID);
}

M1MidPlane::HighlightQuotationInterp::HighlightQuotationInterp(M1Store::Item_lv2* p_myself,
                                                               QVBoxLayout* p_vb,
                                                               M1UI::TreeDisplay* p_parent,
                                                               int p_depth)  : Interp(p_myself, p_vb, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("HighlightQuotationInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::HighlightQuotationInterp::displayText(){
    return m_myself->getTarget_lv2()->text();
}
QString M1MidPlane::HighlightQuotationInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("QuotationInterp: %1").arg(m_myself->text()))
    QString l_html = QString("<p>%1</p>").arg(hq_html_fragment(m_myself->getTarget_lv2()));
    l_html += "<hr/>\n" + base_html_fragment(m_myself, "HighlightQuotationInterp");
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);;

}
