#include <QMouseEvent>
#include <QPainter>

#include "m1_tree_display.h"
#include "m1_nav.h"

M1UI::TreeDisplay::TreeDisplay(QWidget *parent) : QWidget{parent}{
    qDebug() << "TreeDisplay started yay";

    setMouseTracking(true);

    // recursive tree-walk
    M1Store::Nav* l_nav = new M1Store::Nav(M1Store::ROOT_SPECIAL_ID);

    std::set<M1Store::ItemID> l_already_expanded;

    delete l_nav;
}

void M1UI::TreeDisplay::mouseMoveEvent(QMouseEvent *p_event){
    qDebug() << QString("TD_mouseMoveEvent %1 %2").arg(p_event->position().x()).arg(p_event->position().y());
}

void M1UI::TreeDisplay::paintEvent(QPaintEvent* p_event){
    qDebug() << QString("paintEvent (%1, %2 --> %3, %4)")
                    .arg(p_event->rect().topLeft().x())
                    .arg(p_event->rect().topLeft().y())
                    .arg(p_event->rect().bottomRight().x())
                    .arg(p_event->rect().bottomRight().y())
        ;

    QPainter p(this);
    p.drawLine(
        p_event->rect().topLeft().x(),
        p_event->rect().topLeft().y(),
        p_event->rect().bottomRight().x(),
        p_event->rect().bottomRight().y());

    QFontMetrics l_fm(this->font());
    qDebug() << (
        QString("Family        : %1\n").arg(this->font().family()) +
        QString("pixelSize()   : %1\n").arg(this->font().pixelSize()) +
        QString("pointSize()   : %1\n").arg(this->font().pointSize()) +
        QString("pointSizeF()  : %1\n").arg(this->font().pointSizeF()) +
        QString("size of 'a'   : %1 %2\n").arg(l_fm.boundingRect('a').width()).arg(l_fm.boundingRect('a').height()) +
        QString("size of 'A'   : %1 %2\n").arg(l_fm.boundingRect('A').width()).arg(l_fm.boundingRect('A').height()) +
        QString("size of 'W'   : %1 %2\n").arg(l_fm.boundingRect('W').width()).arg(l_fm.boundingRect('W').height()) +
        QString("size of 'What the fuck is this pixel' : %1 %2\n").arg(l_fm.boundingRect("What the fuck is this pixel").width()).arg(l_fm.boundingRect("What the fuck is this pixel").height()) +
        QString("Default Family: %1\n").arg(this->font().defaultFamily())
                 ).toUtf8().constData();;
}
