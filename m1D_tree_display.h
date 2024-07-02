#ifndef TREEDISPLAY_H
#define TREEDISPLAY_H

#include <QObject>
#include <QWidget>

#include "m1C_interp.h"

namespace M1UI{
    class TreeDisplay : public QWidget
    {
        Q_OBJECT
    private:
        std::vector<M1MidPlane::Interp*>  m_interp_list;

        void recurBuildInterp(QSet<M1Store::ItemID> p_already_expanded, M1Store::Nav* p_nav, M1Store::FlagField p_left_lines, int p_depth);

        void mouseMoveEvent(QMouseEvent *event);
        void variousTests();
    public:
        explicit TreeDisplay(QWidget *parent = nullptr);

        void paintEvent(QPaintEvent *);
    signals:

    };
}

#endif // TREEDISPLAY_H
