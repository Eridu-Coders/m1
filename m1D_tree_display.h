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
        M1MidPlane::Interp* addInterpRecur(M1Store::Item_lv2* p_root,
                                           int p_depth,
                                           QVBoxLayout* p_vb,
                                           QVector<M1Store::ItemID>& p_edges_alrady_traversed);

        void variousTests();

        M1Store::Item_lv2* m_root;
        MainWindow *m_main_window;
        M1Store::SpecialItem* m_new_edge_type;
        M1Store::SpecialItem* m_new_vertex_type;
        QList<M1MidPlane::Interp*> m_interp_list;
        M1MidPlane::Interp* m_old_interp = nullptr;
        QWidget* m_scroll_area_widget = nullptr;
        QVBoxLayout* m_vb_layout = nullptr;
    public:
        explicit TreeDisplay(QWidget *p_parent, MainWindow *p_main_window);

        virtual void paintEvent(QPaintEvent *);
        M1Store::SpecialItem* newEdgeType();
        M1Store::SpecialItem* newVertexType();
        // virtual void mouseMoveEvent(QMouseEvent *p_event);
    public slots:
        void gotoVertex(M1Store::Item_lv2* p_new_vertex, M1MidPlane::Interp* p_sender);
        void htmlReceive(const QString& p_html);
        void edgeTypeSelected(int p_index);
        void vertexTypeSelected(int p_index);
    signals:
        void emitHtml(const QString& p_html);
    };
}

#endif // TREEDISPLAY_H
