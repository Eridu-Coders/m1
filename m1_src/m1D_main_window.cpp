#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "m1A_env.h"
#include "m1D_main_window.h"
#include "m1D_tree_display.h"

#include <QSplitter>
#include <QWebEngineView>
#include <QUrl>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QComboBox>
#include <QPushButton>

// g_cat_main_window
Q_LOGGING_CATEGORY(g_cat_main_window, "main_window")

void loadWords(){}

const int HTML_TAB_INDEX = 0;
const int EDIT_TAB_INDEX = 2;
const int INIT_SIZE_X = 1200;
const int INIT_SIZE_Y = 1000;

M1UI::MainWindow::MainWindow(QWidget *p_parent) : QMainWindow(p_parent){
    this->resize(INIT_SIZE_X, INIT_SIZE_Y);

    m_central_widget = new QSplitter(this);
    m_central_widget->setObjectName("centralwidget");
    this->setCentralWidget(m_central_widget);

    QWidget* l_left_panel = new QWidget(m_central_widget);
    QVBoxLayout* l_panel_layout = new QVBoxLayout();
    l_left_panel->setLayout(l_panel_layout);
    m_central_widget->addWidget(l_left_panel);

    QWidget* l_buttons_bar = new QWidget(l_left_panel);
    l_panel_layout->addWidget(l_buttons_bar);
    QHBoxLayout* l_bar_layout = new QHBoxLayout();
    l_buttons_bar->setLayout(l_bar_layout);

    QComboBox* l_et_combo = new QComboBox(l_left_panel);
    l_bar_layout->addWidget(l_et_combo);

    QComboBox* l_vt_combo = new QComboBox(l_left_panel);
    l_bar_layout->addWidget(l_vt_combo);
    l_bar_layout->addStretch(1);

    for(const M1Store::SpecialItem* l_special : M1Store::StorageStatic::getSelectableEdgeTypes()) {
        qDebug() << l_special->mnemonic();
        l_et_combo->addItem(*M1Store::StorageStatic::getQIcon(l_special->specialId()), l_special->mnemonic());
    }
    for(const M1Store::SpecialItem* l_special : M1Store::StorageStatic::getSelectableVertexTypes()) {
        //qDebug() << l_special->mnemonic();
        l_vt_combo->addItem(*M1Store::StorageStatic::getQIcon(l_special->specialId()), l_special->mnemonic());
    }
    QPushButton* l_home_btn = new QPushButton("Home", l_buttons_bar);
    l_bar_layout->addWidget(l_home_btn);

    l_panel_layout->addWidget(l_buttons_bar);

    M1UI::TreeDisplay* l_tree_display_left = new M1UI::TreeDisplay(l_left_panel, this);
    l_panel_layout->addWidget(l_tree_display_left);

    m_tab_widget = new QTabWidget(m_central_widget);
    m_central_widget->addWidget(m_tab_widget);

    m_central_widget->setStretchFactor(0, 1);
    m_central_widget->setStretchFactor(1, 2);

    this->resize(INIT_SIZE_X, INIT_SIZE_Y);
    m_central_widget->setSizes(QList<int>() << INIT_SIZE_X/3 << 2*(INIT_SIZE_X/3) + INIT_SIZE_X%3);

    m_web_view = new QWebEngineView(m_central_widget);
    m_web_view->load(QUrl("https://www.deccanherald.com"));
    // m_web_view->resize(600, 800);
    m_web_view->show();
    m_tab_widget->addTab(m_web_view, "HTML");

    M1UI::TreeDisplay* l_tree_display_right = new M1UI::TreeDisplay(m_central_widget, this);
    m_tab_widget->addTab(l_tree_display_right, "Tree");

    m_menubar = new QMenuBar(this);
    m_menubar->setObjectName("menubar");
    // m_menubar->setGeometry(QRect(0, 0, 800, 20));
    this->setMenuBar(m_menubar);

    m_statusbar = new QStatusBar(this);
    m_statusbar->setObjectName("statusbar");
    this->setStatusBar(m_statusbar);

    this->setWindowTitle(QCoreApplication::translate("m1 Main", "m1 Main", nullptr));
    QMetaObject::connectSlotsByName(this);

    QObject::connect(l_tree_display_left, &M1UI::TreeDisplay::emitHtml,
                     this, &MainWindow::htmlReceive);
    QObject::connect(l_et_combo, &QComboBox::activated,
                     l_tree_display_left, &M1UI::TreeDisplay::edgeTypeSelected);
    QObject::connect(l_vt_combo, &QComboBox::activated,
                     l_tree_display_left, &M1UI::TreeDisplay::vertexTypeSelected);
    QObject::connect(l_home_btn, &QPushButton::clicked,
                     l_tree_display_left, &M1UI::TreeDisplay::goHome);

    // this->setMouseTracking(true);

    l_tree_display_left->setFocus();

    setAttribute(Qt::WA_AcceptDrops, false);
    setAttribute(Qt::WA_AcceptDrops, true);
}

/*
void M1UI::MainWindow::mouseMoveEvent(QMouseEvent *p_event){
    qCDebug(g_cat_main_window) << QString("mouse move event") << p_event->position();
}*/

void M1UI::MainWindow::htmlReceive(const QString& p_html){
    // printf("%s\n", p_html.toUtf8().constData());
    M1_FUNC_ENTRY(g_cat_tmp_spotlight, QString("Html: %1").arg(p_html.length()));
    qCDebug(g_cat_tmp_spotlight).noquote() << p_html;

    if(p_html.left(4) == "http")
        m_web_view->setUrl(QUrl(p_html));
    else
        m_web_view->setHtml(p_html);

    m_tab_widget->setCurrentIndex(HTML_TAB_INDEX);
    M1_FUNC_EXIT
}

void M1UI::MainWindow::editReceive(QWidget *p_edit_widget){
    M1_FUNC_ENTRY(g_cat_main_window, QString("Item"));

    if(m_tab_widget->count() > 0)
        m_tab_widget->removeTab(EDIT_TAB_INDEX);

    m_tab_widget->addTab(p_edit_widget, "Edit");

    qCDebug(g_cat_main_window) << QString("p_edit_widget geometry") << p_edit_widget->geometry();
    M1_FUNC_EXIT
}

M1UI::MainWindow::~MainWindow(){
}
