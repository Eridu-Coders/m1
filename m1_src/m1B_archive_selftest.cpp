/**
 * @file m1B_archive_selftest.cpp
 * @brief (developer) end-to-end crash test of the archive log - hidden CLI option --archive-selftest
 *
 * ** Run it on a COPY of the storage directory, never on real data: phase "crash" adds test vertices and aborts the process. **
 *
 * 1. m1 --archive-selftest crash  : 2 committed transactions + 1 incomplete one, then abort() (simulated crash)
 * 2. m1 --archive-selftest check  : the store set-up has performed the recovery --> verify that the 2 committed transactions
 *                                   are present and the incomplete one is not, then shut down normally
 * 3. m1 --archive-selftest clean  : (optional) verify that after a clean shutdown no recovery takes place
 */
#include <cstdlib>
#include <iostream>

#include <QFile>
#include <QTextStream>

#include "m1A_env.h"
#include "m1B_store.h"
#include "m1B_archive.h"
#include "m1B_lv2_item.h"
#include "m1B_graph_init.h"

namespace {
    QString expectFile(){return QString("%1/%2/selftest_expect.txt").arg(M1Store::STORE_DATA_PATH, M1Store::ARCHIVE_DIR);}
    const char* LONG_LABEL = "Archive self-test vertex with a label long enough to go into the string table";
    const char* FIELD_VALUE = "archive self-test field value";

    int fail(const QString& p_msg){
        std::cout << "ARCHIVE SELF-TEST FAILED: " << p_msg.toStdString() << std::endl;
        return 1;
    }
}

/**
 * @brief run one phase of the archive self-test (see file header)
 * @param p_phase "crash", "check" or "clean"
 * @return process return code (0 = success). Phase "crash" never returns.
 */
int M1Store::Archive::selfTest(const QString& p_phase){
    std::cout << "Archive self-test, phase: " << p_phase.toStdString() << std::endl;

    if(p_phase == "crash"){
        if(cm_mode != Mode::Logging) return fail("archive not in logging mode");
        M1Store::Item_lv2* l_home = M1Store::Item_lv2::getExisting(M1Env::HOME_SIID);
        M1Store::Item_lv2* l_vertex;
        {
            M1Store::Transaction l_tx("Self-test T1: create vertex");
            l_vertex = l_home->create_descendant(M1Env::OWNS_SIID, "temp", M1Env::FOLDER_SIID);
            l_vertex->setText_lv1(LONG_LABEL);  // > 32 bytes --> string table
        }
        {
            M1Store::Transaction l_tx("Self-test T2: add field");
            l_vertex->setFieldEdge(FIELD_VALUE, M1Env::TEXT_SUB_TITLE_SIID);
        }
        const ItemID l_next_item = StorageStatic::cm_next_item;
        const StringID l_next_string = StorageStatic::cm_next_string;

        QFile l_file(expectFile());
        if(!l_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return fail("cannot write " + expectFile());
        QTextStream(&l_file) << l_vertex->item_id() << "\n" << l_next_item << "\n" << l_next_string << "\n";
        l_file.close();

        // incomplete transaction: modifications reach the mmap() files (MAP_SHARED) but are never committed
        M1Store::Archive::begin("Self-test T3: incomplete");
        l_vertex->setText_lv1("THIS LABEL MUST NOT SURVIVE THE CRASH");
        l_vertex->create_descendant(M1Env::OWNS_SIID, "must not survive", M1Env::FOLDER_SIID);
        std::cout << "simulating a crash (abort) in the middle of a transaction" << std::endl;
        std::abort();
    }
    else if(p_phase == "check"){
        QFile l_file(expectFile());
        if(!l_file.open(QIODevice::ReadOnly)) return fail("cannot read " + expectFile() + " (run phase 'crash' first)");
        QTextStream l_in(&l_file);
        ItemID l_vertex_id, l_next_item;
        StringID l_next_string;
        l_in >> l_vertex_id >> l_next_item >> l_next_string;
        l_file.close();

        std::cout << "recovery report: " << cm_report.toStdString() << std::endl;
        if(cm_report.isEmpty()) return fail("no recovery took place");
        if(!cm_report.contains("2 completed transaction")) return fail("expected 2 replayed transactions");
        if(!cm_report.contains("Self-test T3: incomplete")) return fail("incomplete transaction not reported");
        if(StorageStatic::cm_next_item != l_next_item)
            return fail(QString("next item %1, expected %2").arg(StorageStatic::cm_next_item).arg(l_next_item));
        if(StorageStatic::cm_next_string != l_next_string)
            return fail(QString("next string %1, expected %2").arg(StorageStatic::cm_next_string).arg(l_next_string));
        if(l_vertex_id >= StorageStatic::cm_next_item) return fail("test vertex lost");
        M1Store::Item_lv2* l_vertex = M1Store::Item_lv2::getExisting(l_vertex_id);
        if(QString(l_vertex->text()) != LONG_LABEL) return fail(QString("label is [%1]").arg(l_vertex->text()));
        if(l_vertex->getField(M1Env::TEXT_SUB_TITLE_SIID) != FIELD_VALUE)
            return fail(QString("field is [%1]").arg(l_vertex->getField(M1Env::TEXT_SUB_TITLE_SIID)));
        std::cout << "ARCHIVE SELF-TEST (check) PASSED" << std::endl;
        StorageStatic::storeShutDown();
        return 0;
    }
    else if(p_phase == "clean"){
        if(!cm_report.isEmpty()) return fail("unexpected recovery after a clean shutdown: " + cm_report);
        std::cout << "ARCHIVE SELF-TEST (clean) PASSED" << std::endl;
        StorageStatic::storeShutDown();
        return 0;
    }
    else if(p_phase == "stray"){
        // a write outside of any transaction --> Q_ASSERT (debug builds) / implicit transaction (release builds)
        M1Store::Item_lv2* l_home = M1Store::Item_lv2::getExisting(M1Env::HOME_SIID);
        l_home->setText_lv1(QString(l_home->text()));
        std::cout << "write outside of a transaction was accepted (release build: implicit transaction)" << std::endl;
        StorageStatic::storeShutDown();
        return 0;
    }
    return fail("unknown phase " + p_phase);
}
