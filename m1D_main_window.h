#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSplitter>
#include <QWebEngineView>

#include "m1D_tree_display.h"

class MainWindow;

class Ui_MainWindow
{
public:
    QSplitter *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    Ui_MainWindow(){}

    M1UI::TreeDisplay* setupUi(MainWindow *MainWindow);
    void retranslateUi(QMainWindow *MainWindow);
};

class MainWindow: public QMainWindow {
    Q_OBJECT
    friend class Ui_MainWindow;
private:
    Ui_MainWindow* m_ui;
    QWebEngineView* m_view;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void htmlReceive(const QString& p_html);
};

#endif // MAINWINDOW_H
