#ifndef TREEDISPLAY_H
#define TREEDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>

#include "m1B_lv2_item.h"
#include "m1C_interp.h"

namespace M1UI{
    class TreeDisplay : public QScrollArea
    {
        Q_OBJECT
    private:
        void addTestWidgets();
        void addTestInterp(M1Store::Item_lv2* p_root);
        M1MidPlane::Interp* addTestInterpRecur(M1Store::Item_lv2* p_root, int p_depth, QVBoxLayout* p_vb, QVector<int>& p_alrady_seen);

        void mouseMoveEvent(QMouseEvent *event);
        void variousTests();

        M1Store::Item_lv2* m_root;
    public:
        explicit TreeDisplay(QWidget *parent = nullptr);

        void paintEvent(QPaintEvent *);
    public slots:
        void gotoVertex(M1Store::Item_lv2* p_new_vertex);
        void htmlReceive(const QString& p_html);
    signals:
        void emitHtml(const QString& p_html);
    };
}

#endif // TREEDISPLAY_H
