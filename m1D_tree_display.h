#ifndef TREEDISPLAY_H
#define TREEDISPLAY_H

#include <QObject>
#include <QWidget>

namespace M1UI{
    class TreeDisplay : public QWidget
    {
        Q_OBJECT
    private:
        void mouseMoveEvent(QMouseEvent *event);
    public:
        explicit TreeDisplay(QWidget *parent = nullptr);

        void paintEvent(QPaintEvent *);
    signals:

    };
}

#endif // TREEDISPLAY_H
