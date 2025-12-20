#ifndef INTERP_H
#define INTERP_H

#include "m1B_lv2_item.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QDrag>
#include <QEvent>
#include <QVBoxLayout>
#include <QTimer>
#include <QTextEdit>

#include <memory>
using namespace std;

class MyEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit MyEventFilter(QObject *p_parent = nullptr) : QObject(p_parent) {}

protected:
    bool eventFilter(QObject *p_watched, QEvent *p_event) override
    {
        if (p_watched->isWidgetType() && (p_event->type() == QEvent::FocusIn || p_event->type() == QEvent::FocusOut)) {
            qCDebug(g_cat_interp_drag) << "focus event: " << p_event << " intercepted";
            return true;
        }
        return QObject::eventFilter(p_watched, p_event); // Pass unhandled events to the base class
    }
};


namespace M1MidPlane{
    class Interp;
}

namespace M1UI{

class TreeDisplay;

/*
class InterpProxyWidget : public QWidget{
        Q_OBJECT
private:
    M1MidPlane::Interp* m_main_instance;
public:
    InterpProxyWidget(M1MidPlane::Interp* p_main_instance, QWidget* p_parent) : QWidget(p_parent){m_main_instance = p_main_instance;}
    //~InterpProxyWidget();

    virtual void paintEvent(QPaintEvent* p_event);
    virtual void resizeEvent(QResizeEvent *p_event);
    virtual void mouseDoubleClickEvent(QMouseEvent *p_event);
    virtual void mousePressEvent(QMouseEvent *p_event);
    virtual void mouseReleaseEvent(QMouseEvent *p_event);
    virtual void focusOutEvent(QFocusEvent *p_event);
    virtual void focusInEvent(QFocusEvent *p_event);
    virtual void contextMenuEvent(QContextMenuEvent *p_event);

    virtual void dragEnterEvent(QDragEnterEvent *p_event);
    virtual void dragMoveEvent(QDragMoveEvent *p_event);
    virtual void dragLeaveEvent(QDragLeaveEvent *p_event);
    virtual void dropEvent(QDropEvent *p_event);
};*/

/*
class InterpStaticConstructor{
public:
    InterpStaticConstructor();
};
*/

class TreeRow : public QWidget
{
    Q_OBJECT
    // friend class InterpStaticConstructor;
private:
    static QIcon cm_open;
    static QIcon cm_closed;
    // static QList<M1Store::Item_lv2*> cm_gratt;

    int m_depth;
    int m_target_height;
    int m_target_baseline;
    int m_target_padding;
    int m_icon_size;
    int m_oc_x;

    M1UI::TreeDisplay* m_td_parent;

    bool m_drag_top = false;
    bool m_drag_bottom = false;
    bool m_block_emit = false;

    QTimer m_hold_timer;

    QTextEdit* m_text_edit = nullptr;
protected:
    M1Store::Item_lv2* m_edge;
    shared_ptr<M1MidPlane::Interp> m_target;

    void paintOC(QPainter& p);
    virtual QString getHtml();
    virtual QString inTreedisplayText();
    virtual bool displayOpenClose(){ return true; }
    virtual QIcon* edgeIcon();
    virtual QIcon* vertexIcon();
    virtual QWidget *get_edit_widget();
    void restore_acept_drops();

    void initiateDrag();
    void emitSignals();
    // static InterpStaticConstructor cm_the_init;
public:
    static void init();
    static TreeRow* getTreeRow(M1Store::Item_lv2* p_edge, M1UI::TreeDisplay* p_parent, int p_depth);

    TreeRow(M1Store::Item_lv2* p_edge, M1UI::TreeDisplay* p_tree, int p_depth);
    ~TreeRow();

    void blockFocusEvents();

    virtual void paintEvent(QPaintEvent* p_event);
    virtual void resizeEvent(QResizeEvent *p_event);
    virtual void mouseDoubleClickEvent(QMouseEvent *p_event);
    virtual void mousePressEvent(QMouseEvent *p_event);
    virtual void mouseReleaseEvent(QMouseEvent *p_event);
    virtual void focusOutEvent(QFocusEvent *p_event);
    virtual void focusInEvent(QFocusEvent *p_event);
    virtual void contextMenuEvent(QContextMenuEvent *p_event);

    virtual void dragEnterEvent(QDragEnterEvent *p_event);
    virtual void dragMoveEvent(QDragMoveEvent *p_event);
    virtual void dragLeaveEvent(QDragLeaveEvent *p_event);
    virtual void dropEvent(QDropEvent *p_event);

    virtual void setFocus(Qt::FocusReason p_reason);

    // virtual void mouseMoveEvent(QMouseEvent *p_event);

    // void deleteProxy();
    // void invalidateProxy();
    virtual QString dbgString();
public slots:
    void create_descendant();
    void handleMouseHold();
    void save_text_edit();
signals:
    void gotoVertex(M1Store::Item_lv2* p_new_vertex, M1UI::TreeRow* p_sender);
    void emitHtml(const QString& p_html);
    void emitEdit(QWidget *p_edit_widget);
};

class Drag : public QDrag{
private:
    M1UI::TreeDisplay* m_td_parent;
public:
    Drag(TreeRow* p_source, M1UI::TreeDisplay* p_td_parent) : QDrag(p_source){m_td_parent = p_td_parent;}
    virtual ~Drag();
};

} // namespace M1MidPlane
namespace M1MidPlane{

class Interp : public QObject{
private:
    static void init();
    static QMap<M1Env::ItemID, shared_ptr<Interp>> cm_interp_cache;

    M1Store::Item_lv2* m_myself;
protected:
    Interp(M1Store::Item_lv2* p_myself);

    virtual QString getHtml();
    virtual QString inTreedisplayText();
    virtual bool displayOpenClose(){ return true; }
    virtual QIcon* edgeIcon();
    virtual QIcon* vertexIcon();
    virtual QWidget *get_edit_widget();
public:
    static shared_ptr<Interp> getInterp(M1Store::Item_lv2* p_myself);
};

class AutoInterp : public Interp
{
    Q_OBJECT
public:
    static AutoInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);
    // static AutoInterp* getOneIfMatch(M1Store::Item_lv2* p_myself, M1UI::TreeDisplay* p_parent, int p_depth);

    virtual QString getHtml();
    AutoInterp(M1Store::Item_lv2* p_myself, M1UI::TreeDisplay* p_parent, int p_depth);
    AutoInterp(M1Store::Item_lv2* p_myself);
    virtual void paintEvent(QPaintEvent* p_event);
    virtual bool displayOpenClose(){ return false; }
    virtual QString dbgString();
};

class FieldInterp : public Interp
{
    Q_OBJECT
public:
    static FieldInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);
    // static FieldInterp* getOneIfMatch(M1Store::Item_lv2* p_myself, M1UI::TreeDisplay* p_parent, int p_depth);

    virtual QIcon* edgeIcon();
    virtual QIcon* vertexIcon();
    virtual QString getHtml();
    FieldInterp(M1Store::Item_lv2* p_myself, M1UI::TreeDisplay* p_parent, int p_depth);
    FieldInterp(M1Store::Item_lv2* p_myself);
    virtual QString inTreedisplayText();
    virtual void paintEvent(QPaintEvent* p_event);
    virtual QString dbgString();
};

} // namespace M1MidPlane
#endif // INTERP_H
