#ifndef M1D_PASSAGES_PANEL_H
#define M1D_PASSAGES_PANEL_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>

#include "m1B_lv2_item.h"

namespace M1UI{

class WordItem: public QGraphicsSimpleTextItem{
public:
    WordItem(const QString& p_txt, QGraphicsItem *p_parent = nullptr): QGraphicsSimpleTextItem(p_txt, p_parent){}
    virtual QSizeF sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint = QSizeF()) const;
    virtual void setGeometry(const QRectF &p_rect);
protected:
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget);
};

class StephanusItem: public WordItem{
public:
    StephanusItem(const QString& p_txt, QGraphicsItem *p_parent = nullptr): WordItem(p_txt, p_parent){}
protected:
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget);
};

class PassageEditor: public QGraphicsObject{
    friend class PassagesPanel;
private:
    static QFont cm_font;
    QString m_id;
    QList<WordItem*> m_item_list;
    QSizeF m_editor_size;
    int m_margin_pe = 5;
    int m_spacing = 10;
    M1Store::Item_lv2* m_current_start = nullptr;

    void populate();
    QRectF do_layout(const QRectF& p_rect);
    void move_forward(int p_steps=1);
    void move_backwards(int p_steps=1);
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget) override;
    virtual QVariant itemChange(GraphicsItemChange p_change, const QVariant &p_value) override;
    virtual bool sceneEvent(QEvent *p_event) override;
public:
    QString& id(){return m_id;}
    PassageEditor(M1Store::Item_lv2* p_occur_start, const QString& p_id, QGraphicsItem *p_parent=nullptr);
    QRectF get_outer_rect(const QPointF& p_top_left, int p_editor_width);
};

class PassagesPanel: public QGraphicsObject{
    Q_OBJECT
    friend class View;
private:
    int m_view_width = -1;
    QSizeF m_size_panel;
    int m_margin_pp = 30;
    QList<PassageEditor*> m_pe_list;
    void calculate_positions();
protected:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget) override;
public:
    PassagesPanel(QGraphicsItem *p_parent=nullptr): QGraphicsObject(p_parent){}
    void add_passage_editor(PassageEditor* p_pe){m_pe_list.append(p_pe);}
    QSizeF set_panel_width(const QPointF& p_top_left, int p_panel_width);
public slots:
    void move_forward_one();
    void move_backwards_one();
    void move_forward_ten();
    void move_backwards_ten();
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
    int m_margin_view = 10;
public:
    View(QGraphicsScene *p_scene):QGraphicsView(p_scene){}
    void set_panel(PassagesPanel* p_outer_panel){m_outer_panel = p_outer_panel;}
protected:
    virtual bool event(QEvent *p_event);
    virtual void paintEvent(QPaintEvent *p_event);
    virtual void resizeEvent(QResizeEvent *p_event);
};

}
#endif // M1D_PASSAGES_PANEL_H
