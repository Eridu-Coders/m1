#ifndef TREEDISPLAY_H
#define TREEDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsItem>

#include "m1B_lv2_item.h"
#include "m1C_interp.h"
#include "m1D_main_window.h"

namespace M1UI{
    class TreeDisplay : public QScrollArea
    {
        Q_OBJECT
    private:
        // void addTestWidgets();
        void addInterp(M1Store::Item_lv2* p_root);
        M1MidPlane::Interp* addInterpRecur(M1Store::Item_lv2* p_root, int p_depth, QVBoxLayout* p_vb, QVector<int>& p_alrady_seen);

        void mouseMoveEvent(QMouseEvent *event);
        void variousTests();

        M1Store::Item_lv2* m_root;
        MainWindow *m_main_window;
    public:
        explicit TreeDisplay(QWidget *p_parent, MainWindow *p_main_window);

        void paintEvent(QPaintEvent *);
    public slots:
        void gotoVertex(M1Store::Item_lv2* p_new_vertex);
        void htmlReceive(const QString& p_html);
    signals:
        void emitHtml(const QString& p_html);
    };
}

#endif // TREEDISPLAY_H
