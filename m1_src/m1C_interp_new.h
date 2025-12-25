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
class FocusEventsBlocker : public QObject
{
    Q_OBJECT
public:
    explicit FocusEventsBlocker(QObject *p_parent = nullptr) : QObject(p_parent) {}

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

class TreeRow : public QWidget{
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

    const int HOLD_DELAY = 800;

    QTimer m_hold_timer;

    M1Store::Item_lv2* m_edge;
    std::shared_ptr<M1MidPlane::Interp> m_target;

    void paintOpenClose(QPainter& p);
    void initiateDrag();
    void emitSignals();
    // static InterpStaticConstructor cm_the_init;
public:
    static void init();
    static TreeRow* getTreeRow(M1Store::Item_lv2* p_edge, M1UI::TreeDisplay* p_parent, int p_depth);

    TreeRow(M1Store::Item_lv2* p_edge, M1UI::TreeDisplay* p_tree, int p_depth);
    ~TreeRow();

    void blockFocusEvents();
    void performPostUpdate();

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

    void restore_acept_drops();
    // virtual void mouseMoveEvent(QMouseEvent *p_event);

    // void deleteProxy();
    // void invalidateProxy();
    QString dbgOneLiner();
public slots:
    void create_descendant();
    void handleMouseHold();
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
    Q_OBJECT
private:
    static std::map<M1Env::ItemID, std::shared_ptr<Interp>> cm_interp_map;

    M1Store::ItemID m_edge_cache_iid = M1Store::G_VOID_ITEM_ID;
    QString m_html_cache;
    QTextEdit* m_text_edit = nullptr;
    M1UI::TreeRow* m_tree_row = nullptr;

    QString m_dbgOneLinerCache;
protected:
    M1Store::Item_lv2* m_myself  = nullptr;
    static QString cm_html_template;

    Interp();
    Interp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "BaseInterp";}

    QString base_html_fragment();
    QString base_edge_html_fragment(const M1Store::Item_lv2* p_edge);

    virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    virtual QString dbgOneLinerVirtual();
public:
    static void init(){cm_interp_map.clear();}
    static void invalidateAllCaches();
    static std::shared_ptr<Interp> getInterp(M1Store::Item_lv2* p_myself);
    static QString dbgMapContents();

    bool isEmpty(){return m_myself == nullptr;}
    QString getHtml(const M1Store::Item_lv2* p_edge);
    void invalidateCache();
    void setParent(M1UI::TreeRow* p_tree_row){m_tree_row = p_tree_row;}

    virtual QWidget *get_edit_widget();
    virtual bool displayOpenClose(){ return true; }
    virtual QString inTreeDisplayText();
    virtual QIcon* edgeIcon(const M1Store::Item_lv2* p_edge);
    virtual QIcon* vertexIcon();
    virtual M1Store::Item_lv2* targetForGotoVertex(){return m_myself;}
    virtual void createDescendant(M1Store::SpecialItem* p_new_edge_type, M1Store::SpecialItem* p_new_vertex_type);

    QString dbgOneLiner(){
        m_dbgOneLinerCache = dbgOneLinerVirtual();
        return m_dbgOneLinerCache;
    }
    ~Interp();
public slots:
    void save_text_edit();
};

/*
class AutoInterp : public Interp{
    Q_OBJECT
public:
    static AutoInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    AutoInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "AutoInterp";}
    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    // virtual void paintEvent(QPaintEvent* p_event);
    virtual bool displayOpenClose(){ return false; }
};
*/

class FieldInterp : public Interp{
    Q_OBJECT
public:
    static FieldInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    FieldInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "FieldInterp";}
    virtual QIcon* edgeIcon(const M1Store::Item_lv2* p_edge);
    virtual QIcon* vertexIcon();
    virtual bool displayOpenClose(){ return false; }
    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    virtual QString inTreeDisplayText();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class TextInterp : public Interp{
    Q_OBJECT
public:
    static TextInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    TextInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "TextInterp";}
    // virtual QIcon* edgeIcon(const M1Store::Item_lv2* p_edge);
    // virtual QIcon* vertexIcon();
    // virtual bool displayOpenClose(){ return false; }
    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    virtual QString inTreeDisplayText();
    virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    // virtual void paintEvent(QPaintEvent* p_event);
};

} // namespace M1MidPlane
#endif // INTERP_H
