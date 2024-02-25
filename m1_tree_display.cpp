#include <QMouseEvent>

#include "m1_tree_display.h"

M1UI::TreeDisplay::TreeDisplay(QWidget *parent) : QWidget{parent}{
    qDebug() << "TreeDisplay started yay";

    setMouseTracking(true);
}

void M1UI::TreeDisplay::mouseMoveEvent(QMouseEvent *p_event){
    qDebug() << QString("TD_mouseMoveEvent %1 %2").arg(p_event->position().x()).arg(p_event->position().y());
}
