#include <QMouseEvent>
#include <QPainter>
#include <QBoxLayout>

#include "m1D_tree_display.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_iterators.h"

Q_LOGGING_CATEGORY(g_cat_tree_display, "tree_display")

M1UI::TreeDisplay::TreeDisplay(QWidget *p_parent, MainWindow *p_main_window) : QScrollArea{p_parent}{
    M1_FUNC_ENTRY(g_cat_tree_display, QString("TreeDisplay started yay"));

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


    this->createScrollWidget();
    addRow(M1Store::Item_lv2::getExisting(M1Env::HOME_SIID));

    m_new_edge_type = M1Store::StorageStatic::getSelectableEdgeTypes()[0];
    m_new_vertex_type = M1Store::StorageStatic::getSelectableVertexTypes()[0];
    qCDebug(g_cat_tree_display) << "Default Edge type : " << m_new_edge_type->mnemonic();

    // this->setAcceptDrops(true);
    // this->setMouseTracking(true);

    M1_FUNC_EXIT
}

void M1UI::TreeDisplay::createScrollWidget(){
    m_scroll_area_widget = new QWidget(this);
    m_vb_layout = new QVBoxLayout(m_scroll_area_widget);
    m_scroll_area_widget->setLayout(m_vb_layout);
    this->setWidget(m_scroll_area_widget);
    m_vb_layout->setSpacing(0);
    m_vb_layout->setContentsMargins(0, 0, 0, 0);
}

/*
void M1UI::TreeDisplay::mouseMoveEvent(QMouseEvent *p_event){
    qCDebug(g_cat_tree_display) << QString("TD mouse move event") << p_event->position();
}*/

void M1UI::TreeDisplay::htmlReceive(const QString& p_html){
    M1_FUNC_ENTRY(g_cat_tmp_spotlight, QString("html received - len: %1").arg(p_html.length()))
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

/**
 * @brief M1UI::TreeDisplay::addRow
 * @param p_root
 */
void M1UI::TreeDisplay::addRow(M1Store::Item_lv2* p_root){
    M1_FUNC_ENTRY(g_cat_tree_display, QString("p_root: %1").arg(p_root->dbgShort()));
    m_root = p_root;

    QVector<M1Store::ItemID> l_alrady_traversed;
    qCDebug(g_cat_tmp_spotlight) << "before recur" << m_vb_layout->count() << this->widget()->children().count() << m_tree_row_list.count();
    M1UI::TreeRow* l_auto_edge_row = addRowRecur(p_root, 0, l_alrady_traversed);
    qCDebug(g_cat_tmp_spotlight) << "after recur" << m_vb_layout->count() << this->widget()->children().count() << m_tree_row_list.count();

    m_vb_layout->addStretch();
    qCDebug(g_cat_tree_display) << "after stretch" << m_vb_layout->count() << this->widget()->children().count() << m_tree_row_list.count();
    // std::cout << "Class name: " << typeid(m_vb_layout->).name() << std::endl;
    // l_vb->addStretch();

    this->setWidgetResizable(true);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->show();

    if(l_auto_edge_row != nullptr) l_auto_edge_row->setFocus(Qt::OtherFocusReason);

    qCDebug(g_cat_tree_display) << "Font family : " << this->fontInfo().family();
    qCDebug(g_cat_tree_display) << "Font style  : " << this->fontInfo().style();
    qCDebug(g_cat_tree_display) << "Font height : " << this->fontMetrics().height();
    M1_FUNC_EXIT
}

/**
 * @brief M1UI::TreeDisplay::addRowRecur
 *
 * \todo generalize special case of edges to edges (occurrence edges and TW_SECTION_2_OCC_BEGIN_SIID / TW_SECTION_2_OCC_END_SIID below)
 *
 * @param p_root
 * @param p_depth
 * @param p_edges_alrady_traversed
 * @return
 */
M1UI::TreeRow* M1UI::TreeDisplay::addRowRecur(
    M1Store::Item_lv2* p_root,
    int p_depth,
    QVector<M1Store::ItemID>& p_edges_alrady_traversed)
{
    M1_FUNC_ENTRY(g_cat_tree_display, QString("current p_root: %1").arg(p_root->dbgShort()));

    // the row that materializes the AUTO edge and will be returned to addRow()
    M1UI::TreeRow* l_auto_edge_row = nullptr;

    for(M1Store::Item_lv2_iterator it = p_root->getIteratorTop(); !it.beyondEnd(); it.next()){
        M1UI::TreeRow* l_tr = nullptr;
        // instantiate the line for this edge
        if(it.at()->isOfType(M1Env::AUTO_SIID)){
            qCDebug(g_cat_tree_display) << "current edge (auto) : " << it.at()->dbgShort() << " Auto? " << it.at()->isOfType(M1Env::AUTO_SIID);
            if(p_depth == 0){
                l_tr = new M1UI::TreeRow(it.at(), this, p_depth);
                l_auto_edge_row = l_tr;
            }
            // AUTO edges are ignored if encountered at depths > 0
            else continue;
        }
        else if(p_edges_alrady_traversed.contains(it.at()->item_id()))
            continue;
        else {
            // any other edge, i.e. not AUTO or already traversed
            qCDebug(g_cat_tree_display) << "current edge (regular) : " << it.at()->dbgShort();
            l_tr = new M1UI::TreeRow(it.at(), this, p_depth);
        }
        // because of the else clause above, l_ti is always non-null at this point
        m_tree_row_list.append(l_tr);

        QObject::connect(l_tr, &M1UI::TreeRow::gotoVertex,
                         this, &M1UI::TreeDisplay::gotoVertex);
        QObject::connect(l_tr, &M1UI::TreeRow::emitHtml,
                         this, &M1UI::TreeDisplay::htmlReceive);
        QObject::connect(l_tr, &M1UI::TreeRow::emitEdit,
                         m_main_window, &M1UI::MainWindow::editReceive);

        // recur if this edge is open and not already traversed
        if(it.at()->flags() & M1Env::EDGE_IS_OPEN && !p_edges_alrady_traversed.contains(it.at()->item_id())){
            qCDebug(g_cat_tree_display) << "current edge (open) : " << it.at()->dbgShort();
            p_edges_alrady_traversed.append(it.at()->item_id());

            if(it.at()->hasReciprocal())
                p_edges_alrady_traversed.append(it.at()->getReciprocalEdge_lv2()->item_id());
            addRowRecur(l_tr->where_to_go(), p_depth+1, p_edges_alrady_traversed);

            // special case of edges to edges (occurrence endges) --> to be generalized (see todo item in doc header)
            /*
            if(it.at()->isOfType(M1Env::TW_SECTION_2_OCC_BEGIN_SIID) || it.at()->isOfType(M1Env::TW_SECTION_2_OCC_END_SIID))
                addRowRecur(it.at()->getTarget_lv2()->getTarget_lv2(), p_depth+1, p_edges_alrady_traversed);
            else{
                p_edges_alrady_traversed.append(it.at()->getReciprocalEdge_lv2()->item_id());
                addRowRecur(it.at()->getTarget_lv2(), p_depth+1, p_edges_alrady_traversed);
            }
            */
        }
    }
    M1_FUNC_EXIT
    return l_auto_edge_row;
}

/**
 * @brief M1UI::TreeDisplay::goHome
 */
void M1UI::TreeDisplay::goHome(){
    M1_FUNC_ENTRY(g_cat_tree_display, QString("go Home"))
    gotoVertex(M1Store::Item_lv2::getExisting(M1Env::HOME_SIID));
    M1_FUNC_EXIT
}

/**
 * @brief M1UI::TreeDisplay::gotoVertex
 * @param p_new_vertex
 * @param p_sender
 */
void M1UI::TreeDisplay::gotoVertex(M1Store::Item_lv2* p_new_vertex){
    M1_FUNC_ENTRY(g_cat_tree_display, QString("gotoVertex %1").arg(p_new_vertex == nullptr ? m_root->dbgShort() : p_new_vertex->dbgShort()))

    m_being_dragged = nullptr;

    // blocks focus events
    FocusEventsBlocker *l_filter = new FocusEventsBlocker(this);
    this->installEventFilter(l_filter);
    // blocks update / repaint events
    this->setUpdatesEnabled(false);
    qCDebug(g_cat_tmp_spotlight) << QString("before m_scroll_area_widget delete: L: %1 W: %2 M: %3")
                                       .arg(m_vb_layout->count())
                                       .arg(m_scroll_area_widget->children().count())
                                       .arg(m_tree_row_list.count());

    QWidget *l_widget_to_delete = this->takeWidget();
    // Or for a specific type:
    for(M1UI::TreeRow* l_row: m_tree_row_list){
        qCDebug(g_cat_tmp_spotlight) << "Child Deparenting:" << l_row->dbgOneLiner();
        l_row->deParentTarget();
    }
    if (l_widget_to_delete) delete l_widget_to_delete;

    // both have been deleted by the operation above so are set to nullptr for safety's sake
    m_scroll_area_widget = nullptr;
    m_vb_layout = nullptr;

    // erase TreeRow list
    M1UI::TreeRow::clear_row_list();

    // re-create the scroll area widget and its layout
    this->createScrollWidget();

    qCDebug(g_cat_tree_display) << QString("before m_tree_row_list clear: L: %1 W: %2 M: %3")
                                       .arg(m_vb_layout->count())
                                       .arg(m_scroll_area_widget->children().count())
                                       .arg(m_tree_row_list.count());
    m_tree_row_list.clear();
    // remove stretch item at end
    qCDebug(g_cat_tree_display) << QString("after m_tree_row_list clear: L: %1 W: %2 M: %3")
                                       .arg(m_vb_layout->count())
                                       .arg(m_scroll_area_widget->children().count())
                                       .arg(m_tree_row_list.count());

    /*
    if(m_old_tree_row != nullptr){
        qCDebug(g_cat_tree_display) << "delete m_old_interp" << m_old_tree_row->dbgOneLiner();
        delete m_old_tree_row;
        m_old_tree_row = nullptr;
    }
    for(M1UI::TreeRow *l_tree_row : std::as_const(m_tree_row_list)) l_tree_row->blockFocusEvents();
    for(M1UI::TreeRow *l_tree_row : std::as_const(m_tree_row_list)){
        // l_interp->deleteProxy();
        if(l_tree_row == p_sender){
            qCDebug(g_cat_tree_display) << "skipping seletion of sender: " << p_sender->dbgOneLiner();
            m_old_tree_row = l_tree_row;
        }
        else
            delete l_tree_row;
    m_vb_layout->removeItem(m_vb_layout->itemAt(0));

    for(int i=0; i<m_vb_layout->count(); i++)
        qCDebug(g_cat_tree_display) << "Layout" << i << m_vb_layout->itemAt(i)->spacerItem() << m_vb_layout->itemAt(i)->widget();
    for(int i=0; i<this->widget()->children().count(); i++)
        qCDebug(g_cat_tree_display) << "m_scroll_area_widget" << i << m_scroll_area_widget->children().at(i);
    }*/

    // re-create list of rows ...
    if(p_new_vertex == nullptr)
        // ... from the same root (if only for update purposes)
        addRow(m_root);
    else
        // ... or from the new root given in parameter if not null
        addRow(p_new_vertex);

    // restore normal operations
    this->setUpdatesEnabled(true);
    this->removeEventFilter(l_filter);
    delete l_filter;

    qCDebug(g_cat_tree_display) << QString("at end: L: %1 W: %2 M: %3")
                                       .arg(m_vb_layout->count())
                                       .arg(m_scroll_area_widget->children().count())
                                       .arg(m_tree_row_list.count());
    M1_FUNC_EXIT
}

/**
 * @brief M1UI::TreeDisplay::edgeTypeSelected
 * @param p_index
 */
void M1UI::TreeDisplay::edgeTypeSelected(int p_index){
    qCDebug(g_cat_tree_display) << "Edge type : " << M1Store::StorageStatic::getSelectableEdgeTypes()[p_index]->mnemonic();
    m_new_edge_type = M1Store::StorageStatic::getSelectableEdgeTypes()[p_index];
}

/**
 * @brief M1UI::TreeDisplay::vertexTypeSelected
 * @param p_index
 */
void M1UI::TreeDisplay::vertexTypeSelected(int p_index){
    qCDebug(g_cat_tree_display) << "Vertex type : " << M1Store::StorageStatic::getSelectableVertexTypes()[p_index]->mnemonic();
    m_new_vertex_type = M1Store::StorageStatic::getSelectableVertexTypes()[p_index];
}

/**
 * @brief M1UI::TreeDisplay::newEdgeType
 * @return
 */
M1Store::SpecialItem* M1UI::TreeDisplay::newEdgeType(){
    qCDebug(g_cat_tree_display) << "Current Edge type : " << m_new_edge_type->mnemonic();
    return m_new_edge_type;
}

/**
 * @brief M1UI::TreeDisplay::newVertexType
 * @return
 */
M1Store::SpecialItem* M1UI::TreeDisplay::newVertexType(){
    qCDebug(g_cat_tree_display) << "Current Vertex type : " << m_new_vertex_type->mnemonic();
    return m_new_vertex_type;
}

void M1UI::TreeDisplay::create_descendant_auto(){create_descendant(M1Store::InsertionPoint::below_auto);}
void M1UI::TreeDisplay::create_descendant_bottom(){create_descendant(M1Store::InsertionPoint::at_bottom);}
/**
 * @brief M1UI::TreeRow::create_descendant
 */
void M1UI::TreeDisplay::create_descendant(M1Store::InsertionPoint p_where){
    M1Store::SpecialItem* l_new_edge_type = newEdgeType();
    M1Store::SpecialItem* l_new_vertex_type = newVertexType();
    qCDebug(g_cat_tree_row) << QString("Create New Descendant") <<
        "Edge Type:" << l_new_edge_type->mnemonic() <<
        "Vertex Type:" << l_new_vertex_type->mnemonic() <<
        m_target_for_menu_actions->dbgOneLiner();

    m_target_for_menu_actions->createDescendant(l_new_edge_type, l_new_vertex_type, p_where);
    gotoVertex(nullptr);
}
void M1UI::TreeDisplay::dbg_interp_cache(){
    qCDebug(g_cat_tmp_spotlight()) << "dbg_interp_cache";
    emit emitHtml(M1MidPlane::Interp::dbgMapContents(true));
}

void M1UI::TreeDisplay::garbageCollect(){
    qCDebug(g_cat_tmp_spotlight()) << "Garbage Collection";
    M1MidPlane::Interp::garbageCollect();
    emit emitHtml(M1MidPlane::Interp::dbgMapContents(true));
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
