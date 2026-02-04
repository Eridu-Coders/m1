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
    static std::vector<TreeRow*> cm_row_list;
    const int HOLD_DELAY = 800;

    int m_depth;
    int m_target_height;
    int m_target_baseline;
    int m_target_padding;
    int m_icon_size;
    int m_oc_x;

    M1UI::TreeDisplay* m_td_parent = nullptr;;

    bool m_drag_top = false;
    bool m_drag_bottom = false;
    bool m_block_emit = false;

    QTimer m_hold_timer;

    M1Store::Item_lv2* m_edge;
    std::shared_ptr<M1MidPlane::Interp> m_target;

    QString m_html_cache;

    void paintOpenClose(QPainter& p);
    void initiateDrag();
    void emitSignals();
    void invalidateCache();
    // static InterpStaticConstructor cm_the_init;
public:
    static void init();
    static TreeRow* getTreeRow(M1Store::Item_lv2* p_edge, M1UI::TreeDisplay* p_parent, int p_depth);
    static void invalidateAllCaches();
    static void clear_row_list(){cm_row_list.clear();}

    TreeRow(M1Store::Item_lv2* p_edge, M1UI::TreeDisplay* p_tree, int p_depth);
    virtual ~TreeRow();
    TreeRow();
    TreeRow(const TreeRow& p_to_copy);
    TreeRow& operator=(const TreeRow p_to_copy);
    TreeRow& operator=(const TreeRow& p_to_copy);
    void copyFrom(const TreeRow& p_to_copy);

    void blockFocusEvents();
    void performPostUpdate();
    void deParentTarget();

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
    M1Store::Item_lv2* where_to_go();
    // virtual void mouseMoveEvent(QMouseEvent *p_event);

    // void deleteProxy();
    // void invalidateProxy();
    QString dbgOneLiner() const;
public slots:
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
    friend class SlokaInterp;
    friend class OccurInterp;
    friend class WfWUnit;
    friend class HighlightInterp;
    Q_OBJECT
private:
    static std::map<M1Env::ItemID, std::shared_ptr<Interp>> cm_interp_map;

    QTextEdit* m_text_edit = nullptr;
    M1UI::TreeRow* m_tree_row = nullptr;

    QString m_dbgOneLinerCache;
protected:
    M1Store::Item_lv2* m_myself  = nullptr;
    static QString cm_html_template;

    Interp();
    Interp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "BaseInterp";}
    void invalidateCache();

    QString base_html_fragment();
    QString base_edge_html_fragment(const M1Store::Item_lv2* p_edge);

    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
    virtual QString dbgOneLinerVirtual();
public:
    static void init(){cm_interp_map.clear();}
    static void invalidateAllCaches();
    static std::shared_ptr<Interp> getInterp(M1Store::Item_lv2* p_myself);
    static QString dbgMapContents(bool p_html);
    static void garbageCollect();

    bool isEmpty(){return m_myself == nullptr;}
    QString getHtml(const M1Store::Item_lv2* p_edge);
    void setParent(M1UI::TreeRow* p_tree_row){m_tree_row = p_tree_row;}

    virtual QWidget *get_edit_widget();
    virtual bool displayOpenClose(){ return true; }
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QIcon* edgeIcon(const M1Store::Item_lv2* p_edge);
    virtual QIcon* vertexIcon(const M1Store::Item_lv2* p_edge);
    virtual void createDescendant(M1Store::SpecialItem* p_new_edge_type, M1Store::SpecialItem* p_new_vertex_type, M1Store::InsertionPoint p_where);
    virtual M1Store::Item_lv2* where_to_go(const M1Store::Item_lv2* p_edge);
    virtual QString baseText(){return QString("Base: ") + m_myself->text();}
    virtual QString baseTextPlus(){return baseText();}

    QString dbgOneLiner(){
        m_dbgOneLinerCache = dbgOneLinerVirtual();
        return m_dbgOneLinerCache;
    }
    ~Interp();

    M1Store::Item_lv2* myself(){return m_myself;}
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
    virtual QIcon* vertexIcon(const M1Store::Item_lv2* p_edge=nullptr);
    virtual bool displayOpenClose(){ return false; }
    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    // virtual void paintEvent(QPaintEvent* p_event);
};

class TextInterp : public Interp{
    Q_OBJECT
private:
    std::vector<std::shared_ptr<Interp>> m_chunk_list;
    bool m_initialized = false;

    void initialize();
public:
    static TextInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    TextInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "TextInterp";}
    // virtual QIcon* edgeIcon(const M1Store::Item_lv2* p_edge);
    // virtual QIcon* vertexIcon(const M1Store::Item_lv2* p_edge=nullptr);
    // virtual bool displayOpenClose(){ return false; }
    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
    // virtual void paintEvent(QPaintEvent* p_event);
};

class RoleInterp : public Interp{
    Q_OBJECT
private:
    std::shared_ptr<Interp> m_author;

//    M1Store::Item_lv2* getlUtimateTarget(const M1Store::Item_lv2* p_edge);
public:
    static RoleInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    RoleInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "RoleInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QIcon* vertexIcon(const M1Store::Item_lv2* p_edge=nullptr);
    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
};

class WfWUnit : public Interp{
    Q_OBJECT
public:
    std::vector<std::shared_ptr<Interp>> m_forms_list;
public:
    static WfWUnit* getOneIfMatch(M1Store::Item_lv2* p_myself);

    WfWUnit(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "WfWUnit";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
};

class UrlInterp : public Interp{
    Q_OBJECT
public:
    static UrlInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    UrlInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "UrlInterp";}
    // virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
};

class OccurInterp : public Interp{
    Q_OBJECT
private:
    std::shared_ptr<Interp> m_target;
    bool m_is_capitalized = false;
    QString m_capitalized_fld;
    QString m_punct_left;
    QString m_punct_right;
public:
    static OccurInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    OccurInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "OccurInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual M1Store::Item_lv2* where_to_go(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
    virtual QString baseText();
    virtual QString baseTextPlus();
};

class LemmaInterp : public Interp{
    Q_OBJECT
public:
    QString m_pos;
public:
    static LemmaInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    LemmaInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "LemmaInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    // virtual QString getHtmlVirtual(const M1Store::Item_lv2* p_edge);
};

class FormInterp : public Interp{
    Q_OBJECT
private:
    std::shared_ptr<Interp> m_lemma;
public:
    static FormInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    FormInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "FormInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
    virtual QString baseText();
};

class TranslationBhashya : public Interp{
    Q_OBJECT
private:
    QString m_language;
    std::shared_ptr<Interp> m_author_1 = nullptr; // RoleInterp both
    std::shared_ptr<Interp> m_author_2 = nullptr;
public:
    static TranslationBhashya* getOneIfMatch(M1Store::Item_lv2* p_myself);

    TranslationBhashya(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "TranslationBhashya";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
};

class SlokaInterp : public Interp{
    Q_OBJECT
private:
    bool m_initialized = false;
    int m_chapter_num;
    int m_sloka_num;

    QString m_sk;
    QString m_iast;

    std::vector<std::shared_ptr<Interp>> m_translations_list;
    std::vector<std::shared_ptr<Interp>> m_bhashya_list;
    std::vector<std::shared_ptr<Interp>> m_wfw_list;

    void initialize();
public:
    static SlokaInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    SlokaInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "SlokaInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
};

class SentenceInterp : public Interp{
    Q_OBJECT
private:
    bool m_initialized = false;
    int m_book_num;
    int m_sentence_num;

    QString m_text;

    std::shared_ptr<Interp> m_word_begin_occ = nullptr;
    std::shared_ptr<Interp> m_word_end_occ = nullptr;

    std::vector<std::shared_ptr<Interp>> m_occ_list;

    void initialize();
public:
    static SentenceInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);
    static QString occur_to_text(const M1Store::Item_lv2* p_edge){ return "";}

    SentenceInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "SentenceInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
    // virtual QWidget* get_edit_widget();
};

class SectionInterp : public Interp{
    Q_OBJECT
private:
    bool m_initialized = false;
    // text version --> list of occ
    std::map<QString, std::vector<std::shared_ptr<Interp>>> m_occ_map;

    void initialize();
public:
    static SectionInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    SectionInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "SectionInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
    virtual QWidget* get_edit_widget();
};

class ChunkInterp : public Interp{
    Q_OBJECT
private:
    bool m_initialized = false;
    std::vector<std::shared_ptr<Interp>> m_occ_list;

    void initialize();
public:
    static ChunkInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    ChunkInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "ChunkInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
};

class HighlightInterp : public Interp{
    Q_OBJECT
private:
    bool m_initialized = false;
    std::shared_ptr<Interp> m_category;
    std::vector<std::shared_ptr<Interp>> m_chunk_list;

    void initialize();
public:
    static HighlightInterp* getOneIfMatch(M1Store::Item_lv2* p_myself);

    HighlightInterp(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "HighlightInterp";}
    virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
    virtual QString baseText();
};

class HighlightCategory : public Interp{
    Q_OBJECT
private:
    QString m_color;
public:
    static HighlightCategory* getOneIfMatch(M1Store::Item_lv2* p_myself);

    HighlightCategory(M1Store::Item_lv2* p_myself);
    virtual QString className() {return "HighlightCategory";}
    // virtual QString inTreeDisplayText(const M1Store::Item_lv2* p_edge);
    virtual QString getHtmlVirtual();
};
} // namespace M1MidPlane
#endif // INTERP_H
