#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "m1D_main_window.h"

#include "m1B_lv2_item.h"
#include "m1D_tree_display.h"

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
            M1Store::Item_lv2* l_word = M1Store::Item_lv2::getNew(
                // category & attributes
                M1Store::FULL_VERTEX,
                // type
                M1Store::ItemType(
                    M1Store::TEXT_WORD_SPECIAL_ID,
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

    M1_FUNC_EXIT
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

    M1_FUNC_EXIT
} // setupUi

void Ui_MainWindow::retranslateUi(QMainWindow *MainWindow){
    MainWindow->setWindowTitle(QCoreApplication::translate("m1 Main", "m1 Main", nullptr));

    M1_FUNC_EXIT
} // retranslateUi

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    m_ui = new Ui_MainWindow();
    m_ui->setupUi(this);

    M1_FUNC_EXIT
}

MainWindow::~MainWindow(){
    delete m_ui;

    M1_FUNC_EXIT
}
