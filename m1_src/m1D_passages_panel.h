#ifndef M1D_PASSAGES_PANEL_H
#define M1D_PASSAGES_PANEL_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>

#include "m1B_lv2_item.h"

namespace M1MidPlane{
    class Interp;
}

namespace M1UI{

class PassageEditor;

class BasePassageItem: public QGraphicsSimpleTextItem{
private:
    int m_id = -1;
    int m_height = -1;
    bool m_start_selection = false;
    bool m_end_selection = false;
    bool m_in_selection = false;
    bool m_start_line = false;
    bool m_end_line = false;
    PassageEditor* m_editor = nullptr;
protected:
    static QFont cm_base_font;
    QString m_color;

    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *p_event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *p_event);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *p_event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *p_event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *p_event);
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget);

public:
    BasePassageItem(const int p_id, PassageEditor *p_parent = nullptr);
    virtual QSizeF sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint = QSizeF()) const;
    int id(){return m_id;}
    void set_selection(){m_in_selection = true;}
    void set_start_selection(){m_start_selection = true;}
    void set_end_selection(){m_end_selection = true;}
    void set_start_line(const bool p_state){m_start_line = p_state;}
    void set_end_line(const bool p_state){m_end_line = p_state;}
    void reset_selection_flags(){
        m_start_selection = false;
        m_end_selection = false;
        m_in_selection = false;
    }
    virtual QString highlight(M1Store::Item_lv2* p_chunk, M1Store::Item_lv2* p_category, M1Store::Item_lv2* p_color);
};

class WordItem: public BasePassageItem{
private:
    std::shared_ptr<M1MidPlane::Interp> m_occ;
public:
    WordItem(const int p_id, std::shared_ptr<M1MidPlane::Interp> p_occ, PassageEditor *p_parent = nullptr);
    virtual QString highlight(M1Store::Item_lv2* p_chunk, M1Store::Item_lv2* p_category, M1Store::Item_lv2* p_color);
};

class StephanusItem: public BasePassageItem{
private:
    static QFont cm_font_stephanus;

    QString m_stephanus_number;
public:
    StephanusItem(const int p_id, const QString& p_stephanus_number, PassageEditor *p_parent = nullptr);
};

class PassagesPanel;

class PassageEditor: public QGraphicsObject{
    Q_OBJECT
    friend class PassagesPanel;
private:
    QString m_id;
    QList<BasePassageItem*> m_item_list;
    QSizeF m_editor_size;
    int m_from_sel = -1;
    int m_to_sel = -1;
    int m_margin_pe = 5;
    int m_spacing = 10;
    // M1Store::Item_lv2* m_current_start = nullptr;
    const std::vector<std::shared_ptr<M1MidPlane::Interp>>& m_occur_list;
    M1Store::Item_lv2* m_version_vertex;
    PassagesPanel* m_panel;

    void populate();
    QRectF do_layout(const QRectF& p_rect);
    void move_forward(int p_steps=1);
    void move_backwards(int p_steps=1);
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget) override;
    virtual QVariant itemChange(GraphicsItemChange p_change, const QVariant &p_value) override;
    virtual bool sceneEvent(QEvent *p_event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *p_event) override;
public:
    PassageEditor(const QString& p_version_name, std::vector<std::shared_ptr<M1MidPlane::Interp>>& p_occur_list, const QString& p_id, QGraphicsItem *p_parent=nullptr);
    void unselect_all();
    int spacing(){return m_spacing;}
    QString& id(){return m_id;}
    QRectF get_outer_rect(const QPointF& p_top_left, int p_editor_width);
    void select_from_to(const int p_from, const int p_to);
    bool has_selection(){ return m_from_sel >= 0; }
    QString bake_highlight(M1Store::Item_lv2* p_highlight_vertex, M1Store::Item_lv2* p_category, M1Store::Item_lv2* p_color);
};

class PassagesPanel: public QGraphicsObject{
    Q_OBJECT
    friend class View;
private:
    int m_view_width = -1;
    QSizeF m_size_panel;
    int m_margin_pp = 20;
    QList<PassageEditor*> m_pe_list;
    QList<M1Store::Item_lv2*> m_cat_list;
    int m_current_cat = 0;
    M1Store::Item_lv2* m_highlight_folder;

    void calculate_positions();
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget) override;
public:
    PassagesPanel(M1Store::Item_lv2* p_highlight_folder, const QList<M1Store::Item_lv2*>& p_cat_list, QGraphicsItem *p_parent=nullptr): QGraphicsObject(p_parent){
        m_highlight_folder = p_highlight_folder;
        m_cat_list = p_cat_list;
    }
    void add_passage_editor(PassageEditor* p_pe){m_pe_list.append(p_pe);}
    QSizeF set_panel_width(const QPointF& p_top_left, int p_panel_width);
    void selection_changed();
public slots:
    void move_forward_one();
    void move_backwards_one();
    void move_forward_ten();
    void move_backwards_ten();
    void highlight();
    void cat_select(int p_index);
signals:
    void activate_highlight_button(bool p_enabled);
};

class Scene: public QGraphicsScene{
public:
    Scene():QGraphicsScene(){}
protected:
    virtual bool event(QEvent *p_event);
};

class View: public QGraphicsView{
private:
    PassagesPanel* m_outer_panel = nullptr;
    int m_margin_view = 0;
public:
    View(QGraphicsScene *p_scene);
    void set_panel(PassagesPanel* p_outer_panel){m_outer_panel = p_outer_panel;}
protected:
    virtual bool event(QEvent *p_event);
    virtual void paintEvent(QPaintEvent *p_event);
    virtual void resizeEvent(QResizeEvent *p_event);
};

}
#endif // M1D_PASSAGES_PANEL_H
