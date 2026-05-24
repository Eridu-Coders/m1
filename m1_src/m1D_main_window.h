#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSplitter>
#include <QWebEngineView>
#include <QTabWidget>


namespace M1UI{

class TreeDisplay;

class MainWindow: public QMainWindow {
    Q_OBJECT
private:
    QWebEngineView* m_web_view;
    QSplitter *m_central_widget;
    QMenuBar *m_menubar;
    QStatusBar *m_statusbar;

    QTabWidget *m_tab_widget;

    M1UI::TreeDisplay* m_tree_display_left;
    M1UI::TreeDisplay* m_tree_display_right;

    void closeEvent(QCloseEvent *event);
public:
    MainWindow(QWidget *p_parent = nullptr);
    ~MainWindow();

    void purge();
    // virtual void mouseMoveEvent(QMouseEvent *p_event);
public slots:
    void htmlReceive(const QString& p_html);
    void editReceive(QWidget *p_edit_widget);
};

}

#endif // MAINWINDOW_H
