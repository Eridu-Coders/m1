#include <QApplication>

#include "m1D_main_window.h"
#include "m1A_env.h"
#include "m1B_store.h"

int main(int argc, char *argv[])
{
    qDebug() << "App starts";

    M1Env::M1Env::init();
    M1Store::Storage::storeSetUp();

    qCDebug(g_cat_store) << QString("ROOT_SPECIAL_ID = %1").arg(M1Env::ROOT_SPECIAL_ID);
    qCDebug(g_cat_store) << QString("HOME_SPECIAL_ID = %1").arg(M1Env::HOME_SPECIAL_ID);
    qCDebug(g_cat_store) << QString("TEXT_SPECIAL_ID = %1").arg(M1Env::TEXT_SPECIAL_ID);
    qCDebug(g_cat_store) << QString("T_WORD_SPECIAL_ID = %1").arg(M1Env::TEXT_WORD_SPECIAL_ID);

    QLoggingCategory::setFilterRules("qt.*.debug=false");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int l_ret = a.exec();

    M1Store::Storage::storeShutDown();
    qDebug() << "App ends";
    M1Env::M1Env::close();

    printf("Hurrah! No Core Dump ... Returning %d\n", l_ret);

    M1_FUNC_EXIT
    return l_ret;
}
