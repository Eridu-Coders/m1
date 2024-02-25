#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "m1_main_window.h"

#include "m1_lv2_item.h"
#include "m1_store.h"
#include "m1_env.h"
#include "m1_tree_display.h"

void loadWords(){
    qDebug() << "opening file";
    QString l_path("../Enoch-words.txt");
    int l_count = 0;
    if(boost::filesystem::exists(l_path.toStdString())){
        boost::filesystem::ifstream fileHandler(l_path.toStdString());
        std::string line;
        while(getline(fileHandler, line)){
            /*
                QLoggingCategory::setFilterRules("*.debug=false\n"
                                                 "dump.debug=true");
                M1Store::ItemWrapper::recurGraphStart(M1Store::Storage::getSpecial("ROOT_")->itemId());
                QLoggingCategory::setFilterRules("*.debug=true");
                */
            qDebug() << line;
            M1Store::ItemWrapper* l_word = M1Store::ItemWrapper::mbd_getNew(
                // category & attributes
                M1Store::FULL_VERTEX,
                // type
                M1Store::ItemType(
                    M1Store::T_WORD_SPECIAL_ID,
                    M1Store::G_VOID_SI_ID,
                    M1Store::G_VOID_SI_ID,
                    M1Store::G_VOID_SI_ID),
                // label
                line.c_str()
                );
            delete l_word;
            // if(l_count == 3) break;
            l_count += 1;
        }
    }
    else
        qDebug() << "can't open";
}

void Ui_MainWindow::setupUi(QMainWindow *MainWindow){
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName("MainWindow");
    MainWindow->resize(800, 600);
    //centralwidget = new QWidget(MainWindow);
    centralwidget = new M1UI::TreeDisplay(MainWindow);
    centralwidget->setObjectName("centralwidget");
    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName("menubar");
    menubar->setGeometry(QRect(0, 0, 800, 20));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName("statusbar");
    MainWindow->setStatusBar(statusbar);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
} // setupUi

void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow){
    MainWindow->setWindowTitle(QCoreApplication::translate("m1 Main", "m1 Main", nullptr));
} // retranslateUi

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    qDebug() << "App starts";

    M1Store::M1Env::init();
    M1Store::Storage::storeSetUp();

    qCDebug(g_cat_store) << QString("ROOT_SPECIAL_ID = %1").arg(M1Store::ROOT_SPECIAL_ID);
    qCDebug(g_cat_store) << QString("HOME_SPECIAL_ID = %1").arg(M1Store::HOME_SPECIAL_ID);
    qCDebug(g_cat_store) << QString("TEXT_SPECIAL_ID = %1").arg(M1Store::TEXT_SPECIAL_ID);
    qCDebug(g_cat_store) << QString("T_WORD_SPECIAL_ID = %1").arg(M1Store::T_WORD_SPECIAL_ID);

    QLoggingCategory::setFilterRules("qt.*.debug=false");

    m_ui = new Ui_MainWindow();
    m_ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete m_ui;

    M1Store::Storage::storeShutDown();
    M1Store::M1Env::close();

    qDebug() << "App ends";
}
