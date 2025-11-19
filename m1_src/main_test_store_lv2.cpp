#include <QDebug>

#include <gtest/gtest.h>

#include "m1A_env.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"

Q_LOGGING_CATEGORY(g_main_test, "lv2.main_test")

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    M1Store::M1EnvStatic::init();
    M1_FUNC_ENTRY(g_main_test, QString("Starting tests"))

    // screen logging excluded categories
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.item_type");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.special_item");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv1.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.constructors");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.iterators");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.test");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("store.storage");

    M1Store::Storage::storeSetUp(true);

    int l_ret = RUN_ALL_TESTS();
    qCDebug(g_main_test) << "After Tests";

    M1Store::Storage::storeShutDown();

    M1_FUNC_EXIT
    M1Store::M1EnvStatic::close();
    return l_ret;
}
