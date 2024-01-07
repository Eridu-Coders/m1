#include <QDebug>

#include <gtest/gtest.h>

#include "m1_env.h"
#include "m1_store.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    M1Store::M1Env::init();
    M1Store::Storage::init();

    M1Store::Storage::storeString(QString("Dummy Start [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    // testing::GTEST_FLAG(filter) = "ItemTest.ValuesRoundTrip:ItemTest.AppropriateConfig";
    // testing::GTEST_FLAG(filter) = "ItemTest.ValuesRoundTrip";

    int l_ret = RUN_ALL_TESTS();
    qDebug() << "After Tests";

    qDebug() << "sizeof(M1Store::ItemType):" << sizeof(M1Store::ItemType);
    qDebug() << "sizeof(M1Store::Item):" << sizeof(M1Store::Item_lv0);

    M1Store::Storage::storeString(QString("Dummy End [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    M1Store::Storage::close();
    M1Store::M1Env::close();

    return l_ret;
    //return 0;
}
