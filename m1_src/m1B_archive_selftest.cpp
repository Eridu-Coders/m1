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
 *
 * Rollback (undo) test:
 * 1. m1 --archive-selftest rollback : 3 transactions (create vertex with long label, add field, delete the edge to the vertex),
 *                                     rollback of the last 2, checks, rollback of the 1st, checks, then 1 new transaction
 *                                     and abort() (simulated crash)
 * 2. m1 --archive-selftest rbcheck  : the recovery must replay exactly 1 transaction (the post-rollback one) and the
 *                                     rolled-back changes must be absent
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
#include "m1B_lv2_iterators.h"

// LMDB helper macro (requires "int l_rc;")
#define E(expr) if((l_rc = (expr)) != MDB_SUCCESS) qFatal("LMDB error %d in %s", l_rc, #expr);

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
    else if(p_phase == "rollback"){
        if(cm_mode != Mode::Logging) return fail("archive not in logging mode");
        if(transactionCount() != 0) return fail("the log is not empty at session start");
        const ItemID l_item0 = StorageStatic::cm_next_item;
        const StringID l_string0 = StorageStatic::cm_next_string;
        M1Store::Item_lv2* l_home = M1Store::Item_lv2::getExisting(M1Env::HOME_SIID);

        M1Store::Item_lv2* l_vertex;
        {
            M1Store::Transaction l_tx("RB T1: create vertex");
            l_vertex = l_home->create_descendant(M1Env::OWNS_SIID, "temp", M1Env::FOLDER_SIID);
            l_vertex->setText_lv1(LONG_LABEL);
        }
        const ItemID l_vertex_id = l_vertex->item_id();
        const ItemID l_item1 = StorageStatic::cm_next_item;
        const StringID l_string1 = StorageStatic::cm_next_string;
        const StringID l_label_string = l_vertex->string_id();
        {
            M1Store::Transaction l_tx("RB T2: add field");
            l_vertex->setFieldEdge(FIELD_VALUE, M1Env::TEXT_SUB_TITLE_SIID);
        }
        M1Store::Item_lv2* l_edge = nullptr;
        for(M1Store::Item_lv2_iterator l_it = l_home->getIteratorTop(); !l_it.beyondEnd(); l_it.next())
            if(l_it.at()->isFullEdge() && l_it.at()->target_item_id() == l_vertex_id){l_edge = l_it.at(); break;}
        if(l_edge == nullptr) return fail("edge Home --> test vertex not found");
        const ItemID l_edge_id = l_edge->item_id();
        {
            M1Store::Transaction l_tx("RB T3: delete edge");
            l_edge->deleteEdge();
        }
        if(transactionCount() != 3) return fail(QString("%1 transactions in the log, expected 3").arg(transactionCount()));
        for(int i = 0; i < transactionCount(); i++){
            TxDescription l_desc = describeTransaction(i);
            std::cout << "  [" << l_desc.m_seq << "] " << l_desc.m_label.toStdString() << " | " << l_desc.m_summary.toStdString() << std::endl;
            for(const QString& l_line : std::as_const(l_desc.m_details)) std::cout << "      " << l_line.toStdString() << std::endl;
            if(!l_desc.m_ok || l_desc.m_summary.isEmpty()) return fail(QString("transaction %1 cannot be described").arg(i));
        }
        if(!describeTransaction(0).m_summary.startsWith("new ")) return fail("T1 not described as a new vertex");
        if(!describeTransaction(2).m_summary.startsWith("deleted ")) return fail("T3 not described as a deletion");

        // rollback of T3 + T2
        QString l_err = rollback(2);
        if(!l_err.isEmpty()) return fail("rollback(2): " + l_err);
        if(transactionCount() != 1) return fail("expected 1 transaction left after rollback(2)");
        if(StorageStatic::cm_next_item != l_item1 || StorageStatic::cm_next_string != l_string1)
            return fail("counters not restored by rollback(2)");
        if(M1Store::Item_lv2::getExisting(l_edge_id)->isDead()) return fail("edge still dead after rollback(2)");
        if(QString(l_vertex->text()) != LONG_LABEL) return fail("label lost by rollback(2)");
        if(!l_vertex->getField(M1Env::TEXT_SUB_TITLE_SIID).isEmpty()) return fail("field still present after rollback(2)");
        std::cout << "rollback(2) OK" << std::endl;

        // rollback of T1
        l_err = rollback(1);
        if(!l_err.isEmpty()) return fail("rollback(1): " + l_err);
        if(transactionCount() != 0) return fail("log not empty after rollback(1)");
        if(StorageStatic::cm_next_item != l_item0 || StorageStatic::cm_next_string != l_string0)
            return fail("counters not restored by rollback(1)");
        if(vertexAlive(l_vertex)) return fail("test vertex still alive after rollback(1)");
        for(M1Store::Item_lv2_iterator l_it = l_home->getIteratorTop(); !l_it.beyondEnd(); l_it.next())
            if(l_it.at()->isFullEdge() && l_it.at()->target_item_id() == l_vertex_id) return fail("edge to the test vertex still in Home's ring");
        {
            int l_rc;
            MDB_txn* l_txn;
            E(mdb_txn_begin(StorageStatic::cm_lmdb_env, NULL, MDB_RDONLY, &l_txn));
            StringID l_key_id = l_label_string;
            MDB_val l_key{sizeof(StringID), &l_key_id}, l_data;
            l_rc = mdb_get(l_txn, StorageStatic::cm_dbi_string, &l_key, &l_data);
            mdb_txn_abort(l_txn);
            if(l_rc != MDB_NOTFOUND) return fail("label string still in the string table after rollback(1)");
        }
        std::cout << "rollback(1) OK" << std::endl;

        // a new transaction after the rollbacks (reuses the rolled-back item IDs), then crash
        M1Store::Item_lv2* l_after;
        {
            M1Store::Transaction l_tx("RB T4: after rollback");
            l_after = l_home->create_descendant(M1Env::OWNS_SIID, "after rollback", M1Env::FOLDER_SIID);
        }
        if(l_after->item_id() != l_vertex_id) return fail("rolled-back item IDs not reused");
        QFile l_file(expectFile());
        if(!l_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return fail("cannot write " + expectFile());
        QTextStream(&l_file) << l_after->item_id() << "\n" << StorageStatic::cm_next_item << "\n" << StorageStatic::cm_next_string << "\n";
        l_file.close();
        std::cout << "simulating a crash (abort) after the rollbacks" << std::endl;
        std::abort();
    }
    else if(p_phase == "rbcheck"){
        QFile l_file(expectFile());
        if(!l_file.open(QIODevice::ReadOnly)) return fail("cannot read " + expectFile() + " (run phase 'rollback' first)");
        QTextStream l_in(&l_file);
        ItemID l_vertex_id, l_next_item;
        StringID l_next_string;
        l_in >> l_vertex_id >> l_next_item >> l_next_string;
        l_file.close();

        std::cout << "recovery report: " << cm_report.toStdString() << std::endl;
        if(!cm_report.contains(" 1 completed transaction")) return fail("expected exactly 1 replayed transaction");
        if(StorageStatic::cm_next_item != l_next_item || StorageStatic::cm_next_string != l_next_string)
            return fail("counters differ after recovery");
        M1Store::Item_lv2* l_vertex = M1Store::Item_lv2::getExisting(l_vertex_id);
        if(QString(l_vertex->text()) != "after rollback") return fail(QString("vertex label is [%1]").arg(l_vertex->text()));
        if(!l_vertex->getField(M1Env::TEXT_SUB_TITLE_SIID).isEmpty()) return fail("rolled-back field present after recovery");
        std::cout << "ARCHIVE SELF-TEST (rbcheck) PASSED" << std::endl;
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
