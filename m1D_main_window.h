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

class MainWindow: public QMainWindow {
    Q_OBJECT
private:
    QWebEngineView* m_web_view;
    QSplitter *m_central_widget;
    QMenuBar *m_menubar;
    QStatusBar *m_statusbar;

    QTabWidget *m_tab_widget;
public:
    MainWindow(QWidget *p_parent = nullptr);
    ~MainWindow();
public slots:
    void htmlReceive(const QString& p_html);
    void editReceive(QWidget *p_edit_widget);
};

}

#endif // MAINWINDOW_H
