#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "m1A_env.h"
#include "m1D_main_window.h"
#include "m1D_tree_display.h"

#include <QSplitter>
#include <QWebEngineView>
#include <QUrl>
#include <QGraphicsView>

// g_cat_main_window
Q_LOGGING_CATEGORY(g_cat_main_window, "main_window")

void loadWords(){}

M1UI::MainWindow::MainWindow(QWidget *p_parent) : QMainWindow(p_parent){
    this->resize(1200, 800);
    //centralwidget = new QWidget(MainWindow);
    m_central_widget = new QSplitter(this);
    m_central_widget->setObjectName("centralwidget");
    this->setCentralWidget(m_central_widget);

    M1UI::TreeDisplay* l_tree_display = new M1UI::TreeDisplay(m_central_widget, this);
    m_central_widget->addWidget(l_tree_display);

    m_tab_widget = new QTabWidget(m_central_widget);
    m_central_widget->addWidget(m_tab_widget);

    m_web_view = new QWebEngineView(m_central_widget);
    m_web_view->load(QUrl("https://www.lefigaro.fr"));
    m_web_view->resize(600, 800);
    m_web_view->show();
    m_tab_widget->addTab(m_web_view, "HTML");

    m_menubar = new QMenuBar(this);
    m_menubar->setObjectName("menubar");
    m_menubar->setGeometry(QRect(0, 0, 800, 20));
    this->setMenuBar(m_menubar);
    m_statusbar = new QStatusBar(this);
    m_statusbar->setObjectName("statusbar");
    this->setStatusBar(m_statusbar);

    this->setWindowTitle(QCoreApplication::translate("m1 Main", "m1 Main", nullptr));
    QMetaObject::connectSlotsByName(this);

    QObject::connect(l_tree_display, &M1UI::TreeDisplay::emitHtml,
                     this, &MainWindow::htmlReceive);
}

void M1UI::MainWindow::htmlReceive(const QString& p_html){
    // printf("%s\n", p_html.toUtf8().constData());
    M1_FUNC_ENTRY(g_cat_main_window, QString("Html"));
    if(p_html.left(4) == "http")
        m_web_view->setUrl(QUrl(p_html));
    else
        m_web_view->setHtml(p_html);
    M1_FUNC_EXIT
}

void M1UI::MainWindow::editReceive(QWidget *p_edit_widget){
    M1_FUNC_ENTRY(g_cat_main_window, QString("Item"));

    if(m_tab_widget->count() > 0)
        m_tab_widget->removeTab(1);
    m_tab_widget->addTab(p_edit_widget, "Edit");
    qCDebug(g_cat_td_signals) << QString("p_edit_widget geometry") << p_edit_widget->geometry();
    M1_FUNC_EXIT
}

M1UI::MainWindow::~MainWindow(){
}
