#include <QDebug>

#include <gtest/gtest.h>

#include "m1A_env.h"
#include "m1B_store.h"

Q_LOGGING_CATEGORY(g_main_test, "lv1.main_test")

int main(int argc, char *argv[])
{
    //::testing::InitGoogleTest(&argc, argv);
    M1Store::M1EnvStatic::init();
    M1Store::M1EnvStatic::setNormalFilter("*.debug=true\n"
                                          "lv1.members_access.debug=false");
    // M1Store::M1EnvStatic::setNormalFilter("*.debug=true");
    M1_FUNC_ENTRY(g_main_test, QString("Starting tests"))

    M1Store::Storage::storeSetUp(true);

    // screen logging excluded categories
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.item_type");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.test");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv1.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv2.constructors");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("store.storage");

    //int l_ret = RUN_ALL_TESTS();
    qDebug() << "After Tests";

    M1Store::Storage::storeShutDown();

    M1_FUNC_EXIT
    M1Store::M1EnvStatic::close();
}
