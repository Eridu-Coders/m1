#include <QDebug>

#include <gtest/gtest.h>

#include "m1_env.h"
#include "m1_store.h"
#include "m1_lv2_item.h"

#include <boost/filesystem.hpp>

#include <string>

int main(int argc, char *argv[])
{
    M1Store::M1Env::init();
    M1Store::Storage::storeSetUp();

    qDebug() << "Main start";

    if(false){
        qDebug() << "opening file";
        QString l_path("../Enoch-words.txt");
        int l_count = 0;
        if(boost::filesystem::exists(l_path.toStdString())){
            boost::filesystem::ifstream fileHandler(l_path.toStdString());
            std::string line;
            while(getline(fileHandler, line)){
                QLoggingCategory::setFilterRules("*.debug=false\n"
                                                 "dump.debug=true");
                std::set<M1Store::ItemID> l_already_expanded;
                M1Store::ItemWrapper::recurGraph(M1Store::Storage::getSpecialSlotPointer("ROOT_")->itemId(), l_already_expanded, "");
                QLoggingCategory::setFilterRules("*.debug=true");

                qDebug() << line;
                M1Store::ItemWrapper* l_word = M1Store::ItemWrapper::getNew(
                    // category & attributes
                    M1Store::FULL_VERTEX,
                    // type
                    M1Store::ItemType(
                        M1Store::WORD_SPECIAL_ID,
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

    qDebug() << "Main end";

    /*
    M1Store::Storage::storeString(QString("Start [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    qDebug() << "sizeof(M1Store::ItemType):" << sizeof(M1Store::ItemType);
    qDebug() << "sizeof(M1Store::Item_lv0):" << sizeof(M1Store::Item_lv0);
    qDebug() << "sizeof(M1Store::ItemWrapper):" << sizeof(M1Store::ItemWrapper);
    qDebug() << "sizeof(M1Store::ItemWrapperFullVertex):" << sizeof(M1Store::ItemWrapperFullVertex);

    char l_buf0[128];
    char l_buf1[128];
    M1Store::ItemWrapper* l_wi0 = M1Store::ItemWrapper::instantiateFromMMap((void*)l_buf0, 0, M1Store::FULL_VERTEX, M1Store::ItemType());
    M1Store::ItemWrapper* l_wi1 = M1Store::ItemWrapper::instantiateFromMMap((void*)l_buf1, 0, M1Store::FULL_EDGE, M1Store::ItemType());

    l_wi0->setText("toto");
    l_wi1->setText("tutu");

    qDebug() << l_wi0->dbgString().toUtf8().constData();
    qDebug() << l_wi1->dbgString().toUtf8().constData();

    delete l_wi0;
    delete l_wi1;

    unsigned long p0 = (unsigned long)M1Store::Storage::getItemSlotPointer(0);
    unsigned long p1 = (unsigned long)M1Store::Storage::getItemSlotPointer(1);

    qDebug() << QString("p0: 0x%01").arg(p0, 16, 16);
    qDebug() << QString("p1: 0x%01").arg(p1, 16, 16);
    qDebug() << "Difference: " << (p1 - p0);

    M1Store::Storage::storeString(QString("End [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));
    */

    M1Store::Storage::storeShutDown();
    M1Store::M1Env::close();

    return 0;
}
