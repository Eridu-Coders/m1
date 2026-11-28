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
        void addRow(M1Store::Item_lv2* p_root);
        M1UI::TreeRow* addRowRecur(M1Store::Item_lv2* p_root,
                                           int p_depth,
                                           QVector<M1Store::ItemID>& p_edges_alrady_traversed);

        void variousTests();
        void createScrollWidget();

        M1Store::Item_lv2* m_root;
        MainWindow *m_main_window;

        M1Store::SpecialItem* m_new_edge_type;
        M1Store::SpecialItem* m_new_vertex_type;
        QWidget* m_scroll_area_widget = nullptr;
        QVBoxLayout* m_vb_layout = nullptr;

        QList<M1UI::TreeRow*> m_tree_row_list;

        /// true --> all edges displayed; false --> only full edges leading to full vertices (no fields)
        bool m_show_fields = true;

        // M1UI::TreeRow* m_old_tree_row = nullptr;
        M1UI::TreeRow* m_being_dragged = nullptr;
        std::shared_ptr<M1MidPlane::Interp> m_target_for_menu_actions;
        M1Store::Item_lv2* m_edge_for_menu_actions = nullptr;

        void create_descendant(M1Store::InsertionPoint p_where);
    public:
        explicit TreeDisplay(QWidget *p_parent, MainWindow *p_main_window);
        void purge();

        virtual void paintEvent(QPaintEvent *);
        M1Store::SpecialItem* newEdgeType();
        M1Store::SpecialItem* newVertexType();
        void setBeingDragged(M1UI::TreeRow* p_being_dragged){m_being_dragged = p_being_dragged;}
        void setTargetForMenuActions(std::shared_ptr<M1MidPlane::Interp> p_target_for_menu_actions){m_target_for_menu_actions = p_target_for_menu_actions;}
        void setEdgeForMenuActions(M1Store::Item_lv2* p_edge){m_edge_for_menu_actions = p_edge;}
        void restoreAcceptDrop();
        void setShowFields(bool p_show_fields){m_show_fields = p_show_fields;}
        bool showFields() const {return m_show_fields;}

        QVBoxLayout* tree_vb_layout(){return m_vb_layout;}
        QWidget* scroll_area_widget(){return m_scroll_area_widget;}
        // virtual void mouseMoveEvent(QMouseEvent *p_event);
    public slots:
        void gotoVertex(M1Store::Item_lv2* p_new_vertex);
        void htmlReceive(const QString& p_html);
        void focusedVertexReceive(M1Store::Item_lv2* p_vertex){emit emitFocusedVertex(p_vertex);}
        void edgeTypeSelected(int p_index);
        void vertexTypeSelected(int p_index);
        void goHome();
        void create_descendant_auto();
        void create_descendant_bottom();
        void dbg_interp_cache();
        void garbageCollect();
        void delete_edge();
    signals:
        void emitHtml(const QString& p_html);
        void emitFocusedVertex(M1Store::Item_lv2* p_vertex);
    };
}

#endif // TREEDISPLAY_H
