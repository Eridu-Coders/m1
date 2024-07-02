#include <QMouseEvent>
#include <QPainter>

#include "m1D_tree_display.h"
#include "m1B_nav.h"

M1UI::TreeDisplay::TreeDisplay(QWidget *parent) : QWidget{parent}{
    qDebug() << "TreeDisplay started yay";

    // setMouseTracking(true);

    // recursive tree-walk
    M1Store::Nav* l_nav = new M1Store::Nav(M1Store::ROOT_SPECIAL_ID);

    QSet<M1Store::ItemID> l_already_expanded;
    recurBuildInterp(l_already_expanded, l_nav, 0, 0);

    delete l_nav;

    M1_FUNC_EXIT
}

void M1UI::TreeDisplay::recurBuildInterp(QSet<M1Store::ItemID> p_already_expanded, M1Store::Nav* p_nav, M1Store::FlagField p_left_lines, int p_depth){
    qDebug() << "TreeDisplay recurBuildInterp:" << Qt::endl \
             << "Already Expanded :" << p_already_expanded << Qt::endl \
             << "Nav              :" << p_nav << Qt::endl \
             << "Left lines       :" << p_left_lines << Qt::endl \
             << "Depth            :" << p_depth;

    M1_FUNC_EXIT
}

void M1UI::TreeDisplay::mouseMoveEvent(QMouseEvent *p_event){
    qDebug() << QString("TD_mouseMoveEvent %1 %2").arg(p_event->position().x()).arg(p_event->position().y());

    M1_FUNC_EXIT
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

    //variousTests();

    M1_FUNC_EXIT
}

void M1UI::TreeDisplay::variousTests(){
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
                    ).toUtf8().constData();

    QLoggingCategory::setFilterRules("*.debug=false\n"
                                     "dump.debug=true");

    QList<M1Store::Item_lv2*> l_the_list;
    M1Store::Item_lv2* l_item_1 = M1Store::Item_lv2::getExisting("HOME_");
    M1Store::Item_lv2* l_item_2 = M1Store::Item_lv2::getExisting("TWORD");
    M1Store::Item_lv2* l_item_3 = M1Store::Item_lv2::getExisting("ME___");
    l_the_list.append(l_item_1);
    l_the_list.append(l_item_2);
    l_the_list.append(l_item_3);
    qCDebug(g_cat_silence) << "l_item_1 :" << l_item_1;
    qCDebug(g_cat_silence) << "l_item_2 :" << l_item_2;
    qCDebug(g_cat_silence) << "l_item_3 :" << l_item_3;
    qCDebug(g_cat_silence) << "List to string: ";
    // qCDebug(g_cat_silence) << M1Env::vectorToString(l_the_list);
    qCDebug(g_cat_silence) << l_the_list;

    QList<M1Store::ItemID> l_list_2;
    l_list_2.append(45);
    l_list_2.append(38);
    l_list_2.append(6);
    qCDebug(g_cat_silence) << "List to string (l_list_2) : ";
    qCDebug(g_cat_silence) << l_list_2;

    qCDebug(g_cat_silence) << "lv0 l_item_1 :" << static_cast<M1Store::Item_lv0*>(l_item_1);

    QLoggingCategory::setFilterRules("*.debug=true");
    QLoggingCategory::setFilterRules("qt.*.debug=false");

    M1_FUNC_EXIT
}
