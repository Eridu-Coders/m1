#include <QDebug>

#include "m1A_env.h"
#include "m1B_store.h"

Q_LOGGING_CATEGORY(g_main_test, "lv2.main_test")

int main(int argc, char *argv[])
{
    M1Store::M1EnvStatic::init();
    M1_FUNC_ENTRY(g_main_test, QString("Starting tests"))

    // set-up storage and reset it
    M1Store::Storage::storeSetUp(true);

    M1Store::Storage::storeString(QString("Dummy Start [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    // screen logging excluded categories
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.item_type");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.members_access");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("lv0.test");
    M1Store::M1EnvStatic::addExcludeCatergoryForScreen("store.storage");

    qDebug() << "sizeof(M1Store::ItemType):" << sizeof(M1Store::ItemType);
    qDebug() << "sizeof(M1Store::Item):" << sizeof(M1Store::Item_lv0);

    M1Store::Storage::storeString(QString("Dummy End [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    M1Store::Storage::storeShutDown();
    M1Store::M1EnvStatic::close();

    M1_FUNC_EXIT
    return 0;
}
