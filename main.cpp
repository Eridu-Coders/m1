#include <QDebug>

#include <gtest/gtest.h>

#include "m1_env.h"
#include "m1_lv1_store.h"

int main(int argc, char *argv[])
{
    M1Store::M1Env::init();
    M1Store::Storage::init();

    M1Store::Storage::storeString(QString("Start [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    qDebug() << "sizeof(M1Store::ItemType):" << sizeof(M1Store::ItemType);
    qDebug() << "sizeof(M1Store::Item):" << sizeof(M1Store::Item);
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

    M1Store::Storage::storeString(QString("End [%1]").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));

    M1Store::Storage::close();
    M1Store::M1Env::close();

    return 0;
}
