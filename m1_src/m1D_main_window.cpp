#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "m1A_env.h"
#include "m1D_main_window.h"
#include "m1D_tree_display.h"
#include "m1D_fields_panel.h"
#include "m1D_rollback_panel.h"
#include "m1B_archive.h"

#include <QSplitter>
#include <QWebEngineView>
#include <QUrl>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>

// g_cat_main_window
Q_LOGGING_CATEGORY(g_cat_main_window, "main_window")

void loadWords(){}

const int HTML_TAB_INDEX = 0;
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

    // controls on left tree view
    QWidget* l_buttons_bar = new QWidget(l_left_panel);
    l_panel_layout->addWidget(l_buttons_bar);
    QHBoxLayout* l_bar_layout = new QHBoxLayout();
    l_buttons_bar->setLayout(l_bar_layout);

    QPushButton* l_home_btn = new QPushButton("Home", l_buttons_bar);
    l_bar_layout->addWidget(l_home_btn);
    QPushButton* l_refresh_btn = new QPushButton("Refresh", l_buttons_bar);
    l_bar_layout->addWidget(l_refresh_btn);
    QCheckBox* l_fields_check = new QCheckBox("fields", l_buttons_bar);
    l_fields_check->setChecked(true);
    l_bar_layout->addWidget(l_fields_check);
    l_bar_layout->addStretch(1);

    QComboBox* l_et_combo = new QComboBox(l_left_panel);
    l_bar_layout->addWidget(l_et_combo);

    QComboBox* l_vt_combo = new QComboBox(l_left_panel);
    l_bar_layout->addWidget(l_vt_combo);

    for(const M1Store::SpecialItem* l_special : M1Store::StorageStatic::getSelectableEdgeTypes()) {
        qDebug() << l_special->mnemonic();
        l_et_combo->addItem(*M1Store::StorageStatic::getQIcon(l_special->specialId()), l_special->mnemonicAndLabel());
    }
    for(const M1Store::SpecialItem* l_special : M1Store::StorageStatic::getSelectableVertexTypes()) {
        //qDebug() << l_special->mnemonic();
        l_vt_combo->addItem(*M1Store::StorageStatic::getQIcon(l_special->specialId()), l_special->mnemonicAndLabel());
    }
    l_panel_layout->addWidget(l_buttons_bar);

    m_tree_display_left = new M1UI::TreeDisplay(l_left_panel, this);
    l_panel_layout->addWidget(m_tree_display_left);

    m_tab_widget = new QTabWidget(m_central_widget);
    m_central_widget->addWidget(m_tab_widget);

    m_central_widget->setStretchFactor(0, 1);
    m_central_widget->setStretchFactor(1, 2);

    this->resize(INIT_SIZE_X, INIT_SIZE_Y);
    m_central_widget->setSizes(QList<int>() << INIT_SIZE_X/3 << 2*(INIT_SIZE_X/3) + INIT_SIZE_X%3);

    m_web_view = new QWebEngineView(m_central_widget);
    m_web_view->load(QUrl("https://www.deccanherald.com"));
    m_web_view->show();
    m_tab_widget->addTab(m_web_view, "HTML");

    m_tree_display_right = new M1UI::TreeDisplay(m_central_widget, this);
    m_tab_widget->addTab(m_tree_display_right, "Tree");

    // fields of the vertex highlighted in the left tree
    m_fields_panel = new M1UI::FieldsPanel(m_tab_widget);
    m_tab_widget->addTab(m_fields_panel, "fields");

    // transactions of the current session, with undo
    m_rollback_panel = new M1UI::RollbackPanel(m_tab_widget);
    m_tab_widget->addTab(m_rollback_panel, "Rollback");

    m_menubar = new QMenuBar(this);
    m_menubar->setObjectName("menubar");
    // m_menubar->setGeometry(QRect(0, 0, 800, 20));
    this->setMenuBar(m_menubar);

    m_statusbar = new QStatusBar(this);
    m_statusbar->setObjectName("statusbar");
    this->setStatusBar(m_statusbar);

    this->setWindowTitle(QCoreApplication::translate("m1 Main", "m1 Main", nullptr));
    QMetaObject::connectSlotsByName(this);

    QObject::connect(m_tree_display_left, &M1UI::TreeDisplay::emitHtml,
                     this, &MainWindow::htmlReceive);
    QObject::connect(m_tree_display_left, &M1UI::TreeDisplay::emitFocusedVertex,
                     m_fields_panel, &M1UI::FieldsPanel::setVertex);
    QObject::connect(m_fields_panel, &M1UI::FieldsPanel::fieldsSaved,
                     this, &MainWindow::fieldsSaved);
    QObject::connect(l_et_combo, &QComboBox::activated,
                     m_tree_display_left, &M1UI::TreeDisplay::edgeTypeSelected);
    QObject::connect(l_vt_combo, &QComboBox::activated,
                     m_tree_display_left, &M1UI::TreeDisplay::vertexTypeSelected);
    QObject::connect(l_home_btn, &QPushButton::clicked,
                     m_tree_display_left, &M1UI::TreeDisplay::goHome);
    QObject::connect(l_refresh_btn, &QPushButton::clicked,
                     this, &MainWindow::refresh);
    QObject::connect(m_rollback_panel, &M1UI::RollbackPanel::rollbackRequested,
                     this, &MainWindow::rollback);
    // "fields" tick box: show all edges or only full edges leading to full vertices, then full refresh
    QObject::connect(l_fields_check, &QCheckBox::toggled, this, [this](bool p_checked){
        m_tree_display_left->setShowFields(p_checked);
        refresh();
    });

    // this->setMouseTracking(true);

    m_tree_display_left->setFocus();

    setAttribute(Qt::WA_AcceptDrops, false);
    setAttribute(Qt::WA_AcceptDrops, true);
}

void M1UI::MainWindow::purge(){
    qCDebug(g_cat_tmp_spotlight) << "PURGE";
    m_tree_display_left->purge();
    m_tree_display_right->purge();
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

    // remove the previous edit widget's tab (found by pointer, so that the other tabs are never affected)
    if(m_edit_widget != nullptr){
        int l_index = m_tab_widget->indexOf(m_edit_widget);
        if(l_index >= 0) m_tab_widget->removeTab(l_index);
    }
    m_edit_widget = p_edit_widget;
    m_tab_widget->addTab(p_edit_widget, "Edit");

    qCDebug(g_cat_main_window) << QString("p_edit_widget geometry") << p_edit_widget->geometry();
    M1_FUNC_EXIT
}

/**
 * @brief [slot] fields have been modified in the "fields" tab --> refresh the left tree (fields may have been added/removed)
 */
void M1UI::MainWindow::fieldsSaved(){
    M1_FUNC_ENTRY(g_cat_main_window, QString("fields saved --> tree refresh"));
    M1Store::Item_lv2* l_vertex = m_fields_panel->vertex();
    M1MidPlane::Interp::invalidateAllCaches();
    M1UI::TreeRow::invalidateAllCaches();
    m_tree_display_left->gotoVertex(nullptr);
    // the tree rebuild may have moved the focus to another row --> keep editing the same vertex
    if(m_fields_panel->vertex() != l_vertex) m_fields_panel->setVertex(l_vertex);
    M1_FUNC_EXIT
}

/**
 * @brief [slot] "Refresh" button: completely clear the Interp cache and regenerate the left tree
 */
void M1UI::MainWindow::refresh(){
    M1_FUNC_ENTRY(g_cat_main_window, QString("Refresh"));
    // the current edit widget may belong to an Interp that is about to be released --> take it out of the tabs
    // (not deleted, consistent with editReceive(); the Interp may still hold pointers to it)
    if(m_edit_widget != nullptr){
        int l_index = m_tab_widget->indexOf(m_edit_widget);
        if(l_index >= 0) m_tab_widget->removeTab(l_index);
        m_edit_widget = nullptr;
    }
    M1Store::Item_lv2* l_vertex = m_fields_panel->vertex();
    M1MidPlane::Interp::clearCache();
    M1UI::TreeRow::invalidateAllCaches();
    // rebuild from the same root: new TreeRows --> new Interps; the old ones are released with the old rows
    m_tree_display_left->gotoVertex(nullptr);
    m_fields_panel->setVertex(l_vertex);
    M1_FUNC_EXIT
}

/**
 * @brief [slot] Rollback button of the "Rollback" tab: confirmation, undo of the p_count most recent transactions,
 * then full refresh (the tree root and the "fields" vertex are reset if they no longer exist)
 * @param p_count number of transactions to undo
 */
void M1UI::MainWindow::rollback(int p_count){
    M1_FUNC_ENTRY(g_cat_main_window, QString("Rollback %1 transaction(s)").arg(p_count));
    QString l_question = QString("Undo the %1 most recent transaction%2?\n\nThis cannot be redone.")
                             .arg(p_count).arg(p_count > 1 ? "s" : "");
    if(m_edit_widget != nullptr && m_tab_widget->indexOf(m_edit_widget) >= 0)
        l_question += "\nThe Edit tab will be closed (unsaved changes are lost).";
    if(QMessageBox::question(this, "Rollback", l_question) != QMessageBox::Yes){
        M1_FUNC_EXIT
        return;
    }

    // the Edit tab may show items that are about to disappear
    if(m_edit_widget != nullptr){
        int l_index = m_tab_widget->indexOf(m_edit_widget);
        if(l_index >= 0) m_tab_widget->removeTab(l_index);
        m_edit_widget = nullptr;
    }

    const QString l_error = M1Store::Archive::rollback(p_count);
    if(!l_error.isEmpty()) QMessageBox::warning(this, "Rollback", l_error);

    // pointers held by the UI may designate vertices created by the rolled-back transactions
    m_tree_display_left->ensureValidRoot();
    if(!M1Store::Archive::vertexAlive(m_fields_panel->vertex())) m_fields_panel->setVertex(nullptr);
    refresh();
    M1_FUNC_EXIT
}

M1UI::MainWindow::~MainWindow(){
    printf("MainWindow::~MainWindow()\n");
}

void M1UI::MainWindow::closeEvent(QCloseEvent *event){
    printf("MainWindow::closeEvent()\n");
}
