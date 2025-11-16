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
};

}

namespace M1MidPlane{
/*
class InterpStaticConstructor{
public:
    InterpStaticConstructor();
};
*/

class Interp : public QObject
{
    Q_OBJECT
    friend class TranslUnit;
    friend class FieldInterp;
    friend class AutoInterp;
    friend class SectionBeginEnd;
    friend class ChunkInterp;
    friend class UrlInterp;
    friend class BhashyaTranslation;
    friend class TextInterp;
    friend class TextOccurrence;
    friend class SentenceInterp;
    friend class BookInterp;
    friend class HighlightChunkInterp;
    friend class HighlightInterp;
    friend class SlokaInterp;
    friend class HighlightQuotationInterp;
    // friend class InterpStaticConstructor;
private:
    static QIcon cm_open;
    static QIcon cm_closed;
    static QList<M1Store::Item_lv2*> cm_gratt;

    M1Store::Item_lv2* m_myself;
    int m_depth;
    int m_target_height;
    int m_target_beseline;
    int m_target_padding;
    int m_icon_size;
    int m_oc_x;

    M1UI::TreeDisplay* m_td_parent;
    M1UI::InterpProxyWidget* m_proxy;

    bool m_drag_top = false;
    bool m_drag_bottom = false;
    bool m_block_emit = false;

    QTimer m_hold_timer;

    QTextEdit* m_text_edit = nullptr;

protected:
    void paintOC(QPainter& p);
    virtual QString getHtml();
    virtual QString displayText();
    virtual bool diplayOpenClose(){ return true; }
    virtual QIcon* edgeIcon();
    virtual QIcon* vertexIcon();

    void initiateDrag();
    void emitSignals();
    // static InterpStaticConstructor cm_the_init;
public:
    static void init();
    static Interp* getInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);

    Interp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_tree, int p_depth);
    ~Interp();

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

    virtual void setFocus(Qt::FocusReason p_reason){m_proxy->setFocus(p_reason);}

    // virtual void mouseMoveEvent(QMouseEvent *p_event);
    virtual QWidget *get_edit_widget();

    void deleteProxy();
    void invalidateProxy();
    void restore_acept_drops();
    void blockFocusEvents();

    virtual QString dbgString(){return "Interp for: " + m_myself->dbgShort();}
public slots:
    void create_descendant();
    void handleMouseHold();
    void save_text_edit();
signals:
    void gotoVertex(M1Store::Item_lv2* p_new_vertex, M1MidPlane::Interp* p_sender);
    void emitHtml(const QString& p_html);
    void emitEdit(QWidget *p_edit_widget);
};

class Drag : public QDrag{
private:
    M1UI::TreeDisplay* m_td_parent;
public:
    Drag(Interp* p_source, M1UI::TreeDisplay* p_td_parent) : QDrag(p_source){m_td_parent = p_td_parent;}
    virtual ~Drag();
};

class AutoInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    virtual QString getHtml();
    AutoInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
    virtual bool diplayOpenClose(){ return false; }
    virtual QString dbgString(){return "AutoInterp for: " + m_myself->dbgShort();}
};

class FieldInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    virtual QIcon* edgeIcon();
    virtual QIcon* vertexIcon();
    virtual QString getHtml();
    FieldInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString displayText();
    virtual void paintEvent(QPaintEvent* p_event);
};

class TranslUnit : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    virtual QString getHtml();
    TranslUnit(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString displayText();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class ChunkInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    ChunkInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString getHtml();
    // virtual QString displayText();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class SlokaInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    SlokaInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString getHtml();
    // virtual QString displayText();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class UrlInterp : public Interp
{
    Q_OBJECT
private:
    QString m_label;
    QString m_url;
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    UrlInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString getHtml();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class BhashyaTranslation : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    BhashyaTranslation(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString getHtml();
    virtual QString displayText();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class TextInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    TextInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString getHtml();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class SectionBeginEnd : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);
    virtual QString getHtml();

    SectionBeginEnd(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString displayText();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class TextOccurrence : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    virtual QString getHtml();
    TextOccurrence(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString displayText();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class SentenceInterp : public ChunkInterp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);
    static QString occur_to_text(const M1Store::Item_lv2* p_occur_edge);

    SentenceInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString getHtml();
    virtual QWidget *get_edit_widget();
};

class BookInterp : public ChunkInterp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    BookInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString getHtml();
};

class HighlightChunkInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    HighlightChunkInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString displayText();
    virtual QString getHtml();
};

class HighlightInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    HighlightInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString displayText();
    virtual QString getHtml();
};

class HighlightQuotationInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    HighlightQuotationInterp(M1Store::Item_lv2* p_myself, QVBoxLayout* p_vb, M1UI::TreeDisplay* p_parent, int p_depth);
    virtual QString displayText();
    virtual QString getHtml();
};

}
#endif // INTERP_H
