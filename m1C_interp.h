#ifndef INTERP_H
#define INTERP_H

#include "m1B_lv2_item.h"

#include <QWidget>

namespace M1MidPlane{

/*
class InterpStaticConstructor{
public:
    InterpStaticConstructor();
};
*/

class Interp : public QWidget
{
    Q_OBJECT
    friend class TranslUnit;
    friend class FieldInterp;
    friend class AutoInterp;
    friend class SectionBeginEnd;
    friend class SectionInterp;
    friend class UrlInterp;
    friend class BhashyaTranslation;
    friend class TextInterp;
    // friend class InterpStaticConstructor;
private:
    M1Store::Item_lv2* m_myself;
    // bool m_active;
    int m_depth;
    int m_target_height;
    int m_target_beseline;
    int m_target_padding;
    int m_icon_size;
    int m_oc_x;

    static QIcon cm_open;
    static QIcon cm_closed;

    void paintOC(QPainter& p);
    virtual QString getHtml();
    // static InterpStaticConstructor cm_the_init;
public:
    static void init();
    static Interp* getInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    Interp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
    virtual void resizeEvent(QResizeEvent *p_event);
    virtual void mouseDoubleClickEvent(QMouseEvent *p_event);
    virtual void mousePressEvent(QMouseEvent *p_event);
    virtual void focusOutEvent(QFocusEvent *p_event);
    virtual void focusInEvent(QFocusEvent *p_event);
    ~Interp();
signals:
    void gotoVertex(M1Store::Item_lv2* p_new_vertex);
    void emitHtml(const QString& p_html);
};

class TranslUnit : public Interp
{
    Q_OBJECT
public:
    virtual QString getHtml();
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    TranslUnit(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};

class FieldInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    FieldInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};

class AutoInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    AutoInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};

class SectionBeginEnd : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);

    SectionBeginEnd(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};

class SectionInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);
    virtual QString getHtml();
    SectionInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};

class UrlInterp : public Interp
{
    Q_OBJECT
private:
    QString m_label;
    QString m_url;
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);
    virtual QString getHtml();
    UrlInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};

class BhashyaTranslation : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);
    virtual QString getHtml();
    BhashyaTranslation(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};


class TextInterp : public Interp
{
    Q_OBJECT
public:
    static bool wantIt(M1Store::Item_lv2* p_myself);
    virtual QString getHtml();
    TextInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth);
    virtual void paintEvent(QPaintEvent* p_event);
};

}
#endif // INTERP_H
