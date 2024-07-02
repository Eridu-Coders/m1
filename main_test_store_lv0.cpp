#include <QDebug>

#include <gtest/gtest.h>

#include "m1A_env.h"
#include "m1B_store.h"

Q_LOGGING_CATEGORY(g_main_test, "lv0.main_test")

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    M1Store::M1EnvStatic::init();
    M1_FUNC_ENTRY(g_main_test, QString("Starting tests"))

    M1Store::Storage::storeSetUp();

    M1Store::Storage::storeString(QString("Dummy Start [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));


    // testing::GTEST_FLAG(filter) = "ItemTest.ValuesRoundTrip:ItemTest.AppropriateConfig";
    // testing::GTEST_FLAG(filter) = "ItemTest.ValuesRoundTrip";

    // screen logging excluded categories
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.item_type");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.test");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("store.storage");

    int l_ret = RUN_ALL_TESTS();
    qDebug() << "After Tests";

    qDebug() << "sizeof(M1Store::ItemType):" << sizeof(M1Store::ItemType);
    qDebug() << "sizeof(M1Store::Item):" << sizeof(M1Store::Item_lv0);

    M1Store::Storage::storeString(QString("Dummy End [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    M1Store::Storage::storeShutDown();

    M1_FUNC_EXIT
    M1Store::M1EnvStatic::close();

    return l_ret;
}
