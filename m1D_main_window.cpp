#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "m1D_main_window.h"
#include "m1D_tree_display.h"

#include <QSplitter>
#include <QWebEngineView>
#include <QUrl>

// g_cat_main_window
Q_LOGGING_CATEGORY(g_cat_main_window, "main_window")

void loadWords(){}

M1UI::TreeDisplay* Ui_MainWindow::setupUi(MainWindow *MainWindow){
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName("MainWindow");
    MainWindow->resize(1700, 800);
    //centralwidget = new QWidget(MainWindow);
    centralwidget = new QSplitter(MainWindow);
    centralwidget->setObjectName("centralwidget");
    MainWindow->setCentralWidget(centralwidget);

    M1UI::TreeDisplay* l_tree_display = new M1UI::TreeDisplay(centralwidget);
    centralwidget->addWidget(l_tree_display);

    MainWindow->m_view = new QWebEngineView(centralwidget);
    MainWindow->m_view->load(QUrl("https://www.lefigaro.fr"));
    MainWindow->m_view->resize(600, 800);
    MainWindow->m_view->show();
    centralwidget->addWidget(MainWindow->m_view);

    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName("menubar");
    menubar->setGeometry(QRect(0, 0, 800, 20));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName("statusbar");
    MainWindow->setStatusBar(statusbar);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);

    return l_tree_display;
} // setupUi

void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow){
    MainWindow->setWindowTitle(QCoreApplication::translate("m1 Main", "m1 Main", nullptr));
} // retranslateUi

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    m_ui = new Ui_MainWindow();
    M1UI::TreeDisplay* l_tree_display = m_ui->setupUi(this);
    QObject::connect(l_tree_display, &M1UI::TreeDisplay::emitHtml,
                     this, &MainWindow::htmlReceive);
}

void MainWindow::htmlReceive(const QString& p_html){
    printf("%s\n", p_html.toUtf8().constData());
    if(p_html.left(4) == "http")
        m_view->setUrl(QUrl(p_html));
    else
        m_view->setHtml(p_html);
}

MainWindow::~MainWindow(){
    delete m_ui;
}
