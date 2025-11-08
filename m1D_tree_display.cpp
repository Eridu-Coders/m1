#include <QMouseEvent>
#include <QPainter>
#include <QBoxLayout>

#include "m1D_tree_display.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_item.h"

Q_LOGGING_CATEGORY(g_cat_tree_display, "tree_display")

M1UI::TreeDisplay::TreeDisplay(QWidget *p_parent, MainWindow *p_main_window) : QScrollArea{p_parent}{
    M1_FUNC_ENTRY(g_cat_interp_drag, QString("TreeDisplay started yay"));

    m_main_window = p_main_window;
    // setMouseTracking(true);
    // addTestWidgets();
    QFont f("Noto Mono", 12);
    this->setFont(f);
    this->setMinimumWidth(450);

    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(QColor(30, 30, 30)));
    this->setPalette(p);
    this->setBackgroundRole(QPalette::Window);
    this->setAutoFillBackground(true);

    m_scroll_area_widget = new QWidget(this);
    m_vb_layout = new QVBoxLayout();
    m_scroll_area_widget->setLayout(m_vb_layout);
    this->setWidget(m_scroll_area_widget);
    m_vb_layout->setSpacing(0);
    m_vb_layout->setContentsMargins(0, 0, 0, 0);

    addInterp(M1Store::Item_lv2::getExisting(M1Env::HOME_SIID));

    m_new_edge_type = M1Store::Storage::getSelectableEdgeTypes()[0];
    m_new_vertex_type = M1Store::Storage::getSelectableVertexTypes()[0];
    qCDebug(g_cat_interp_drag) << "Default Edge type : " << m_new_edge_type->mnemonic();

    // this->setAcceptDrops(true);
    // this->setMouseTracking(true);

    M1_FUNC_EXIT
}

/*
void M1UI::TreeDisplay::mouseMoveEvent(QMouseEvent *p_event){
    qCDebug(g_cat_interp_drag) << QString("TD mouse move event") << p_event->position();
}*/

void M1UI::TreeDisplay::htmlReceive(const QString& p_html){
    // M1_FUNC_ENTRY(g_cat_interp_drag, QString("html %1").arg(p_html))
    M1_FUNC_ENTRY(g_cat_interp_drag, QString("html received"))
    emit emitHtml(p_html);
    M1_FUNC_EXIT
}

void M1UI::TreeDisplay::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_tree_display, QString("paintEvent (%1, %2 --> %3, %4)")
                                      .arg(p_event->rect().topLeft().x())
                                      .arg(p_event->rect().topLeft().y())
                                      .arg(p_event->rect().bottomRight().x())
                                      .arg(p_event->rect().bottomRight().y()))

    /*
    QPainter p(this);
    p.drawLine(
        p_event->rect().topLeft().x(),
        p_event->rect().topLeft().y(),
        p_event->rect().bottomRight().x(),
        p_event->rect().bottomRight().y());
    */

    //variousTests();
    M1_FUNC_EXIT
}

M1MidPlane::Interp* M1UI::TreeDisplay::addInterpRecur(
    M1Store::Item_lv2* p_root,
    int p_depth,
    QVBoxLayout* p_vb,
    QVector<M1Store::ItemID>& p_edges_alrady_traversed)
{
    M1_FUNC_ENTRY(g_cat_tree_display, QString("current p_root: %1").arg(p_root->dbgShort()));
    M1MidPlane::Interp* l_auto_edge = nullptr;
    for(M1Store::Item_lv2_iterator it = p_root->getIteratorTop(); !it.beyondEnd(); it.next()){
        M1MidPlane::Interp* l_ti = nullptr;
        // instantiate the line for this edge
        if(it.at()->isOfType(M1Env::AUTO_SIID)){
            qCDebug(g_cat_tree_display) << "current edge (auto) : " << it.at()->dbgShort() << " Auto? " << it.at()->isOfType(M1Env::AUTO_SIID);
            if(p_depth == 0){
                l_ti = M1MidPlane::Interp::getInterp(it.at(), p_vb, this, p_depth);
                l_auto_edge = l_ti;
            }
            else continue;
        }
        else if(p_edges_alrady_traversed.contains(it.at()->item_id()))
            continue;
        else {
            qCDebug(g_cat_tree_display) << "current edge (regular) : " << it.at()->dbgShort();
            l_ti = M1MidPlane::Interp::getInterp(it.at(), p_vb, this, p_depth);
        }
        m_interp_list.append(l_ti);

        QObject::connect(l_ti, &M1MidPlane::Interp::gotoVertex,
                         this, &M1UI::TreeDisplay::gotoVertex);
        QObject::connect(l_ti, &M1MidPlane::Interp::emitHtml,
                         this, &M1UI::TreeDisplay::htmlReceive);
        QObject::connect(l_ti, &M1MidPlane::Interp::emitEdit,
                         m_main_window, &M1UI::MainWindow::editReceive);
        // p_vb->addWidget(l_ti);

        // recur if this edge is open
        if(it.at()->flags() & M1Env::EDGE_IS_OPEN && !p_edges_alrady_traversed.contains(it.at()->item_id())){
            qCDebug(g_cat_tree_display) << "current edge (open) : " << it.at()->dbgShort();
            p_edges_alrady_traversed.append(it.at()->item_id());
            if(it.at()->isOfType(M1Env::TW_SECTION_2_OCC_BEGIN_SIID) || it.at()->isOfType(M1Env::TW_SECTION_2_OCC_END_SIID))
                addInterpRecur(it.at()->getTarget_lv2()->getTarget_lv2(), p_depth+1, p_vb, p_edges_alrady_traversed);
            else{
                p_edges_alrady_traversed.append(it.at()->getReciprocalEdge_lv2()->item_id());
                addInterpRecur(it.at()->getTarget_lv2(), p_depth+1, p_vb, p_edges_alrady_traversed);
            }
        }
    }
    M1_FUNC_EXIT
    return l_auto_edge;
}

void M1UI::TreeDisplay::addInterp(M1Store::Item_lv2* p_root){
    M1_FUNC_ENTRY(g_cat_tree_display, QString("p_root: %1").arg(p_root->dbgShort()));
    m_root = p_root;

    QVector<M1Store::ItemID> l_alrady_traversed;
    qCDebug(g_cat_interp_drag) << "before recur" << m_vb_layout->count() << this->widget()->children().count() << m_interp_list.count();
    M1MidPlane::Interp* l_auto_edge = addInterpRecur(p_root, 0, m_vb_layout, l_alrady_traversed);
    qCDebug(g_cat_interp_drag) << "after recur" << m_vb_layout->count() << this->widget()->children().count() << m_interp_list.count();

    m_vb_layout->addStretch();
    qCDebug(g_cat_interp_drag) << "after stretch" << m_vb_layout->count() << this->widget()->children().count() << m_interp_list.count();
    // std::cout << "Class name: " << typeid(m_vb_layout->).name() << std::endl;
    // l_vb->addStretch();

    this->setWidgetResizable(true);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->show();

    if(l_auto_edge != nullptr) l_auto_edge->setFocus(Qt::OtherFocusReason);

    qCDebug(g_cat_tree_display) << "Font family : " << this->fontInfo().family();
    qCDebug(g_cat_tree_display) << "Font style  : " << this->fontInfo().style();
    qCDebug(g_cat_tree_display) << "Font height : " << this->fontMetrics().height();
    M1_FUNC_EXIT
}

void M1UI::TreeDisplay::gotoVertex(M1Store::Item_lv2* p_new_vertex, M1MidPlane::Interp* p_sender){
    M1_FUNC_ENTRY(g_cat_interp_drag, QString("gotoVertex %1").arg(p_new_vertex == nullptr ? m_root->dbgShort() : p_new_vertex->dbgShort()))

    m_being_dragged = nullptr;

    // blocks focus events
    MyEventFilter *l_filter = new MyEventFilter(this);
    this->installEventFilter(l_filter);
    // blocks update / repaint events
    this->setUpdatesEnabled(false);
    qCDebug(g_cat_interp_drag) << "before m_interp_list delete" << m_vb_layout->count() << m_scroll_area_widget->children().count();
    if(m_old_interp != nullptr){
        qCDebug(g_cat_interp_drag) << "delete m_old_interp" << m_old_interp->dbgString();
        delete m_old_interp;
        m_old_interp = nullptr;
    }
    for(M1MidPlane::Interp *l_interp : std::as_const(m_interp_list)) l_interp->blockFocusEvents();
    for(M1MidPlane::Interp *l_interp : std::as_const(m_interp_list)){
        l_interp->deleteProxy();
        if(l_interp == p_sender){
            qCDebug(g_cat_interp_drag) << "skipping seletion of sender: " << p_sender->dbgString();
            m_old_interp = l_interp;
        }
        else
            delete l_interp;
    }

    qCDebug(g_cat_interp_drag) << "before clear" << m_vb_layout->count() << m_scroll_area_widget->children().count();
    m_interp_list.clear();
    // remove stretch item at end
    m_vb_layout->removeItem(m_vb_layout->itemAt(0));
    qCDebug(g_cat_interp_drag) << "after clear" << m_vb_layout->count() << m_scroll_area_widget->children().count();

    for(int i=0; i<m_vb_layout->count(); i++)
        qCDebug(g_cat_interp_drag) << "Layout" << i << m_vb_layout->itemAt(i)->spacerItem() << m_vb_layout->itemAt(i)->widget();
    for(int i=0; i<this->widget()->children().count(); i++)
        qCDebug(g_cat_interp_drag) << "m_scroll_area_widget" << i << m_scroll_area_widget->children().at(i);

    if(p_new_vertex == nullptr)
        addInterp(m_root);
    else
        addInterp(p_new_vertex);

    this->setUpdatesEnabled(true);
    this->removeEventFilter(l_filter);
    delete l_filter;

    qCDebug(g_cat_interp_drag) << "after new filling" << m_vb_layout->count() << m_scroll_area_widget->children().count() << m_interp_list.count();
    M1_FUNC_EXIT
}

void M1UI::TreeDisplay::edgeTypeSelected(int p_index){
    qCDebug(g_cat_interp_drag) << "Edge type : " << M1Store::Storage::getSelectableEdgeTypes()[p_index]->mnemonic();
    m_new_edge_type = M1Store::Storage::getSelectableEdgeTypes()[p_index];
}
void M1UI::TreeDisplay::vertexTypeSelected(int p_index){
    qCDebug(g_cat_interp_drag) << "Vertex type : " << M1Store::Storage::getSelectableVertexTypes()[p_index]->mnemonic();
    m_new_vertex_type = M1Store::Storage::getSelectableVertexTypes()[p_index];
}

M1Store::SpecialItem* M1UI::TreeDisplay::newEdgeType(){
    qCDebug(g_cat_interp_drag) << "Current Edge type : " << m_new_edge_type->mnemonic();
    return m_new_edge_type;
}

M1Store::SpecialItem* M1UI::TreeDisplay::newVertexType(){
    qCDebug(g_cat_interp_drag) << "Current Vertex type : " << m_new_vertex_type->mnemonic();
    return m_new_vertex_type;
}
// ------------------------------------------ Tests --------------------------------------------------------------
/*
void M1UI::TreeDisplay::variousTests(){
    M1_FUNC_ENTRY(g_cat_tree_display, "Various Tests")

    QFontMetrics l_fm(this->font());
    qDebug() << (
                    QString("Family        : %1\n").arg(this->font().family()) +
                    QString("pixelSize()   : %1\n").arg(this->font().pixelSize()) +
                    QString("pointSize()   : %1\n").arg(this->font().pointSize()) +
                    QString("pointSizeF()  : %1\n").arg(this->font().pointSizeF()) +
                    QString("size of 'a'   : %1 %2\n").arg(l_fm.boundingRect('a').width()).arg(l_fm.boundingRect('a').height()) +
                    QString("size of 'A'   : %1 %2\n").arg(l_fm.boundingRect('A').width()).arg(l_fm.boundingRect('A').height()) +
                    QString("size of 'W'   : %1 %2\n").arg(l_fm.boundingRect('W').width()).arg(l_fm.boundingRect('W').height()) +
                    QString("size of 'What the fuck is this pixel' : %1 %2\n").arg(l_fm.boundingRect("What the fuck is this pixel").width()).arg(l_fm.boundingRect("What the fuck is this pixel").height()) +
                    QString("Default Family: %1\n").arg(this->font().defaultFamily())
                    ).toUtf8().constData();

    M1Env::M1EnvStatic::setSilentMode(true);
    // QLoggingCategory::setFilterRules("*.debug=false\n"
    //                                  "dump.debug=true");

    QList<M1Store::Item_lv2*> l_the_list;
    M1Store::Item_lv2* l_item_1 = M1Store::Item_lv2::getExisting("HOME_");
    M1Store::Item_lv2* l_item_2 = M1Store::Item_lv2::getExisting("TWORD");
    M1Store::Item_lv2* l_item_3 = M1Store::Item_lv2::getExisting("ME___");
    l_the_list.append(l_item_1);
    l_the_list.append(l_item_2);
    l_the_list.append(l_item_3);
    qCDebug(g_cat_silence) << "l_item_1 :" << l_item_1;
    qCDebug(g_cat_silence) << "l_item_2 :" << l_item_2;
    qCDebug(g_cat_silence) << "l_item_3 :" << l_item_3;
    qCDebug(g_cat_silence) << "List to string: ";
    // qCDebug(g_cat_silence) << M1Env::vectorToString(l_the_list);
    qCDebug(g_cat_silence) << l_the_list;

    QList<M1Store::ItemID> l_list_2;
    l_list_2.append(45);
    l_list_2.append(38);
    l_list_2.append(6);
    qCDebug(g_cat_silence) << "List to string (l_list_2) : ";
    qCDebug(g_cat_silence) << l_list_2;

    qCDebug(g_cat_silence) << "lv0 l_item_1 :" << static_cast<M1Store::Item_lv0*>(l_item_1);

    // QLoggingCategory::setFilterRules("*.debug=true");
    // QLoggingCategory::setFilterRules("qt.*.debug=false");
    M1Env::M1EnvStatic::setSilentMode(true);

    M1_FUNC_EXIT
}

class TestWidget : public QWidget{
private:
    QColor m_color;
    QString m_name;
public:
    TestWidget(QColor p_color, const QString& p_name) : QWidget(){
        M1_FUNC_ENTRY(g_cat_tree_display, QString("Test Widget Constructor: ") + p_name);
        qCDebug(g_cat_tree_display) << "baseSize()" << this->baseSize();
        qCDebug(g_cat_tree_display) << "rect()" << this->rect();
        m_color = p_color;
        m_name = p_name;
        setMinimumHeight(25);
        setMaximumHeight(25);
        M1_FUNC_EXIT
    }
    void paintEvent(QPaintEvent* p_event){
        QPainter p(this);
        p.setPen(Qt::white);
        p.drawText(QPoint(5, 15), m_name);
        p.setPen(m_color);
        p.drawRect(this->rect().topLeft().x(),
                   this->rect().topLeft().y(),
                   this->rect().width()-1,
                   this->rect().height()-1);
        qCDebug(g_cat_tree_display) << m_name << " height: " << this->rect().height();
    }
    void resizeEvent(QResizeEvent *p_event){
        qCDebug(g_cat_tree_display) << m_name << " resized: " << p_event->size();
    }
};

void M1UI::TreeDisplay::addTestWidgets(){
    QVBoxLayout* l_vb = new QVBoxLayout();
    l_vb->setSpacing(0);
    l_vb->setContentsMargins(0, 0, 0, 0);
    int l_count = 30;
    for(int i=0; i<l_count; i++){
        auto l_col = Qt::white;
        switch (i%4) {
        case 0:
            l_col = Qt::red;
            break;
        case 1:
            l_col = Qt::blue;
            break;
        case 2:
            l_col = Qt::yellow;
            break;
        case 3:
            l_col = Qt::green;
            break;
        default:
            break;
        }
        // TestWidget* l_tw = new TestWidget(w, l_col, QString("TW%1").arg(i));
        TestWidget* l_tw = new TestWidget(l_col, QString("TW%1").arg(i));
        l_vb->addWidget(l_tw);
    }
    l_vb->addStretch();

    QWidget* l_scroll_area_widget = new QWidget();
    l_scroll_area_widget->setLayout(l_vb);

    this->setWidget(l_scroll_area_widget);
    this->setWidgetResizable(true);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->show();
}


*/
