/**
 * @file m1B_archive_describe.cpp
 * @brief human-readable description of committed transactions (Rollback panel), computed from the before/after images
 * of the archive log
 *
 * The description is heuristic (it reconstructs graph operations from physical item images):
 * - new full vertex + new edge leading to it            --> "new <type> "label" under "origin""
 * - other new full edges                                --> "link <type>: "origin" → "target"" (field if the target is a simple vertex)
 * - new simple edges                                    --> "field <type> = "value" on "origin""
 * - EDGE_DEAD set / cleared                             --> "deleted / restored <type> edge ..."
 * - EDGE_IS_OPEN toggled                                --> "opened / closed "target""
 * - edge removed from its ring (unlinkEdge())          --> "removed ..."
 * - text change                                         --> "label / value / field ...: "old" → "new""
 * Reciprocal edges are described once. Every modified item also gets a technical detail line.
 */
#include <QDateTime>
#include <QSet>
#include <QRegularExpression>
#include <QLoggingCategory>

#include "m1A_env.h"
#include "m1B_store.h"
#include "m1B_archive.h"
#include "m1B_archive_internal.h"
#include "m1B_graph_init.h"

using namespace M1Store::ArchiveInternal;

namespace {
    /// an item image, copied out of the log body (properly aligned for Item_lv0)
    struct Img{
        alignas(M1Store::Item_lv0) char m_bytes[sizeof(M1Store::Item_lv0)];
        const M1Store::Item_lv0* item() const {return reinterpret_cast<const M1Store::Item_lv0*>(m_bytes);}
    };
    Img makeImg(const char* p_bytes){
        Img l_img;
        memcpy(l_img.m_bytes, p_bytes, sizeof(l_img.m_bytes));
        return l_img;
    }

    /// \0-terminated string in a fixed-size buffer (not necessarily terminated if full)
    QString fixedText(const char* p_text, size_t p_max){return QString::fromUtf8(p_text, qstrnlen(p_text, p_max));}

    /// "label" quoted, shortened
    QString q(const QString& p_text){
        QString l_text = p_text.simplified();
        if(l_text.length() > 40) l_text = l_text.left(39) + "…";
        return QString("“%1”").arg(l_text);
    }

    const QList<QPair<M1Env::FlagField, const char*>>& flagNames(){
        static const QList<QPair<M1Env::FlagField, const char*>> l_names = {
            {M1Env::ITEM_HAS_LOCAL_STRING, "local-string"},
            {M1Env::IS_SPECIAL, "special"},
            {M1Env::IS_AUTO, "auto"},
            {M1Env::EDGE_IS_OPEN, "open"},
            {M1Env::EDGE_SEPABOVE, "sep-above"},
            {M1Env::EDGE_DEAD, "dead"}};
        return l_names;
    }
    /// "+open -dead ..." for the flags changed between p_before and p_after
    QString flagDiff(M1Env::FlagField p_before, M1Env::FlagField p_after){
        QStringList l_ret;
        M1Env::FlagField l_known = 0;
        for(const auto& l_pair : flagNames()){
            l_known |= l_pair.first;
            if((p_before & l_pair.first) != (p_after & l_pair.first))
                l_ret << QString("%1%2").arg((p_after & l_pair.first) ? "+" : "-").arg(l_pair.second);
        }
        M1Env::FlagField l_other = (p_before ^ p_after) & ~l_known & ~M1Env::SPECIAL_ITEM_ID_MASK;
        if(l_other) l_ret << QString("flags 0x%1 → 0x%2").arg(p_before, 0, 16).arg(p_after, 0, 16);
        return l_ret.join(" ");
    }
    QString idStr(M1Env::ItemID p_id){return p_id == M1Env::G_VOID_ITEM_ID ? "-" : QString::number(p_id);}
}

/**
 * @brief text of an item image. Strings stored in the string table are looked up in p_strings first (values put/deleted by
 * the transaction), then in the current string table
 */
QString M1Store::Archive::imageText(const Item_lv0* p_img, const QHash<StringID, QByteArray>& p_strings){
    auto l_string = [&p_strings](StringID p_id) -> QString {
        if(p_id == G_VOID_ITEM_ID) return QString();
        if(p_strings.contains(p_id)) return QString::fromUtf8(p_strings.value(p_id).constData());
        // current string table (read-only, no failure if absent)
        MDB_txn* l_txn;
        if(mdb_txn_begin(StorageStatic::cm_lmdb_env, NULL, MDB_RDONLY, &l_txn) != MDB_SUCCESS) return QString("(string #%1)").arg(p_id);
        MDB_val l_key{sizeof(StringID), &p_id}, l_data;
        QString l_ret = (mdb_get(l_txn, StorageStatic::cm_dbi_string, &l_key, &l_data) == MDB_SUCCESS) ?
                            QString::fromUtf8(static_cast<const char*>(l_data.mv_data), qstrnlen(static_cast<const char*>(l_data.mv_data), l_data.mv_size)) :
                            QString("(string #%1)").arg(p_id);
        mdb_txn_abort(l_txn);
        return l_ret;
    };

    const FlagField l_flags = p_img->m_flags;
    const bool l_local = (l_flags & ITEM_HAS_LOCAL_STRING) != 0;
    switch(l_flags & ITEM_NATURE_MASK){
    case FULL_EDGE:     return fixedText(p_img->p.e.f.m_text, sizeof(p_img->p.e.f.m_text));
    case SIMPLE_VERTEX: return fixedText(p_img->p.v.s.m_text, sizeof(p_img->p.v.s.m_text));
    case SIMPLE_EDGE:   return l_local ? fixedText(p_img->p.e.s.m_text, sizeof(p_img->p.e.s.m_text)) : l_string(p_img->p.e.s.m_string_id);
    default:            return l_local ? fixedText(p_img->p.v.f.m_text, sizeof(p_img->p.v.f.m_text)) : l_string(p_img->p.v.f.m_string_id);
    }
}

/// type of an item image, as special item labels (" (type)" suffix removed) or mnemonics
QString M1Store::Archive::typeName(const Item_lv0* p_img){
    if(p_img->m_flags & TYPE_IS_ITEM_ID){
        ItemID l_type_id = p_img->m_type.getItemIDType();
        if(itemAlive(l_type_id)) return QString("[%1]").arg(QString(StorageStatic::getItemPointer_lv1(l_type_id)->text()).simplified().left(30));
        return QString("[item #%1]").arg(idStr(l_type_id));
    }
    QStringList l_names;
    for(unsigned int i = 0; i < 4; i++){
        SpecialItemID l_si = p_img->m_type.getSpecialType(i);
        if(l_si == G_VOID_SI_ID) continue;
        if(l_si >= StorageStatic::cm_next_special){l_names << QString("SI#%1").arg(l_si); continue;}
        SpecialItem* l_special = StorageStatic::getSpecialItemPointer(l_si);
        QString l_name = l_special->label();
        l_name.remove(QRegularExpression("\\s*\\(type\\)$"));
        l_names << (l_name.isEmpty() ? l_special->mnemonic() : l_name);
    }
    return l_names.isEmpty() ? QString("(untyped)") : l_names.join("/");
}

/**
 * @brief describe the transaction at p_index (0 = oldest) of the current session
 */
M1Store::TxDescription M1Store::Archive::describeTransaction(int p_index){
    TxDescription l_ret;
    if(p_index < 0 || p_index >= cm_tx_index.size()) return l_ret;
    l_ret.m_seq = cm_tx_index[p_index].m_seq;

    const QByteArray l_body = readBody(cm_tx_index[p_index]);
    ParsedTx l_tx;
    if(l_body.isEmpty() || !parseTransaction(l_body.constData(), l_body.size(), l_tx)){
        l_ret.m_label = "(unreadable transaction)";
        return l_ret;
    }
    l_ret.m_ok = true;
    l_ret.m_timestamp_ms = l_tx.m_timestamp_ms;
    l_ret.m_label = l_tx.m_label;
    l_ret.m_open_close = (l_tx.m_label == OPEN_CLOSE_LABEL);

    // strings involved in the transaction (put: new values, del: old values)
    QHash<StringID, QByteArray> l_strings;
    for(const ParsedOp& l_op : std::as_const(l_tx.m_ops))
        if(l_op.m_type == REC_STR_PUT || l_op.m_type == REC_STR_DEL) l_strings.insert(l_op.m_id, l_op.m_bytes);

    QHash<ItemID, Img> l_before, l_after;
    QHash<ItemID, bool> l_new;
    for(const ParsedItem& l_item : std::as_const(l_tx.m_items)){
        l_before.insert(l_item.m_id, makeImg(l_item.m_before));
        l_after.insert(l_item.m_id, makeImg(l_item.m_after));
        l_new.insert(l_item.m_id, l_item.m_new);
    }

    // text of any item: after-image if modified by the transaction, otherwise current store
    auto l_name = [&](ItemID p_id) -> QString {
        if(l_after.contains(p_id)) return q(imageText(l_after[p_id].item(), l_strings));
        if(itemAlive(p_id)) return q(imageText(StorageStatic::getItemPointer_lv1(p_id), l_strings));
        return QString("#%1").arg(idStr(p_id));
    };
    auto l_nature = [](const Item_lv0* p_img){return p_img->m_flags & ITEM_NATURE_MASK;};
    auto l_isSimpleVertex = [&](ItemID p_id) -> bool {
        if(l_after.contains(p_id)) return l_nature(l_after[p_id].item()) == SIMPLE_VERTEX;
        return itemAlive(p_id) && l_nature(StorageStatic::getItemPointer_lv1(p_id)) == SIMPLE_VERTEX;
    };
    auto l_isAuto = [](const Item_lv0* p_img){
        return (p_img->m_flags & IS_AUTO) || (!(p_img->m_flags & TYPE_IS_ITEM_ID) && p_img->m_type.getSpecialType(0) == M1Env::AUTO_SIID);
    };

    QStringList l_main;
    QSet<ItemID> l_described;   // edges already described (or whose reciprocal was)
    auto l_markEdge = [&](const Item_lv0* p_edge){
        l_described.insert(p_edge->m_item_id);
        if((p_edge->m_flags & ITEM_NATURE_MASK) == FULL_EDGE && p_edge->p.e.f.m_e_reciprocal != G_VOID_ITEM_ID)
            l_described.insert(p_edge->p.e.f.m_e_reciprocal);
    };

    // 1. new full vertices, with the edge that leads to them
    for(const ParsedItem& l_item : std::as_const(l_tx.m_items)){
        const Item_lv0* l_v = l_after[l_item.m_id].item();
        if(!l_item.m_new || l_nature(l_v) != FULL_VERTEX) continue;
        QString l_line = QString("new %1 %2").arg(typeName(l_v), q(imageText(l_v, l_strings)));
        for(const ParsedItem& l_edge_item : std::as_const(l_tx.m_items)){
            const Item_lv0* l_e = l_after[l_edge_item.m_id].item();
            if(!l_edge_item.m_new || l_nature(l_e) != FULL_EDGE || l_isAuto(l_e)) continue;
            if(l_e->p.e.f.m_v_target == l_item.m_id && l_e->p.e.f.m_v_origin != l_item.m_id){
                l_line += QString(" under %1 (%2)").arg(l_name(l_e->p.e.f.m_v_origin), typeName(l_e));
                l_markEdge(l_e);
                break;
            }
        }
        l_main << l_line;
    }
    // 2. other new edges
    for(const ParsedItem& l_item : std::as_const(l_tx.m_items)){
        const Item_lv0* l_e = l_after[l_item.m_id].item();
        if(!l_item.m_new || l_described.contains(l_item.m_id)) continue;
        if(l_nature(l_e) == FULL_EDGE){
            if(l_isAuto(l_e)) continue;
            const ItemID l_target = l_e->p.e.f.m_v_target;
            if(l_isSimpleVertex(l_target))
                l_main << QString("field %1 = %2 on %3").arg(typeName(l_e), l_name(l_target), l_name(l_e->p.e.f.m_v_origin));
            else
                l_main << QString("link %1: %2 → %3").arg(typeName(l_e), l_name(l_e->p.e.f.m_v_origin), l_name(l_target));
            l_markEdge(l_e);
        }
        else if(l_nature(l_e) == SIMPLE_EDGE){
            l_main << QString("field %1 = %2 on %3").arg(typeName(l_e), q(imageText(l_e, l_strings)), l_name(l_e->p.e.s.m_v_origin));
            l_markEdge(l_e);
        }
    }
    // 3. changes to existing items
    for(const ParsedItem& l_item : std::as_const(l_tx.m_items)){
        if(l_item.m_new) continue;
        const Item_lv0* l_b = l_before[l_item.m_id].item();
        const Item_lv0* l_a = l_after[l_item.m_id].item();
        const auto l_nat = l_nature(l_a);
        const bool l_edge = (l_nat == FULL_EDGE || l_nat == SIMPLE_EDGE);
        const FlagField l_changed = l_b->m_flags ^ l_a->m_flags;

        if(l_edge && !l_described.contains(l_item.m_id)){
            const ItemID l_origin = (l_nat == FULL_EDGE) ? l_a->p.e.f.m_v_origin : l_a->p.e.s.m_v_origin;
            const QString l_what = (l_nat == FULL_EDGE) ?
                QString("%1 edge %2 → %3").arg(typeName(l_a), l_name(l_origin), l_name(l_a->p.e.f.m_v_target)) :
                QString("field %1 %2 on %3").arg(typeName(l_a), q(imageText(l_a, l_strings)), l_name(l_origin));
            const ItemID l_next_b = (l_nat == FULL_EDGE) ? l_b->p.e.f.m_e_next : l_b->p.e.s.m_e_next;
            const ItemID l_next_a = (l_nat == FULL_EDGE) ? l_a->p.e.f.m_e_next : l_a->p.e.s.m_e_next;
            if(l_changed & EDGE_DEAD){
                l_main << QString("%1 %2").arg((l_a->m_flags & EDGE_DEAD) ? "deleted" : "restored", l_what);
                l_markEdge(l_a);
            }
            else if(l_next_b != l_item.m_id && l_next_a == l_item.m_id){
                l_main << QString("removed %1").arg(l_what);
                l_markEdge(l_a);
            }
            else if((l_changed & EDGE_IS_OPEN) && l_nat == FULL_EDGE){
                l_main << QString("%1 %2").arg((l_a->m_flags & EDGE_IS_OPEN) ? "opened" : "closed", l_name(l_a->p.e.f.m_v_target));
                l_described.insert(l_item.m_id);
            }
        }
        // text change (not for edges already described as deleted/removed)
        const QString l_text_b = imageText(l_b, l_strings);
        const QString l_text_a = imageText(l_a, l_strings);
        if(l_text_b != l_text_a && !(l_edge && l_described.contains(l_item.m_id))){
            if(l_nat == FULL_VERTEX) l_main << QString("label %1 → %2").arg(q(l_text_b), q(l_text_a));
            else if(l_nat == SIMPLE_VERTEX) l_main << QString("value %1 → %2").arg(q(l_text_b), q(l_text_a));
            else if(l_nat == SIMPLE_EDGE)
                l_main << QString("field %1 on %2: %3 → %4").arg(typeName(l_a), l_name(l_a->p.e.s.m_v_origin), q(l_text_b), q(l_text_a));
        }
        // type change
        if(typeName(l_b) != typeName(l_a))
            l_main << QString("type of %1: %2 → %3").arg(q(l_text_a), typeName(l_b), typeName(l_a));
    }

    // specials / util values
    for(const ParsedSpecial& l_special : std::as_const(l_tx.m_specials)){
        SpecialItem l_si;
        memcpy(static_cast<void*>(&l_si), l_special.m_after, SPECIAL_BYTES);
        l_main << QString("special item %1 (#%2)").arg(l_si.mnemonicAndLabel()).arg(l_special.m_id);
    }
    for(const ParsedOp& l_op : std::as_const(l_tx.m_ops))
        if(l_op.m_type == REC_UTIL_U32)
            l_main << (l_op.m_key == TEI_EDITOR_FONT_SIZE ?
                           QString("TEI editor font size %1 → %2").arg(l_op.m_before).arg(l_op.m_after) :
                           QString("setting 0x%1: %2 → %3").arg(l_op.m_key, 0, 16).arg(l_op.m_before).arg(l_op.m_after));

    // summary: first 3 main lines
    if(l_main.isEmpty()) l_ret.m_summary = l_tx.m_items.isEmpty() ? QString("(no item change)") : QString("link/ring updates");
    else{
        l_ret.m_summary = l_main.mid(0, 3).join("; ");
        if(l_main.size() > 3) l_ret.m_summary += QString(" (+%1 more)").arg(l_main.size() - 3);
    }

    // details: all main lines, then one technical line per item
    l_ret.m_details = l_main;
    static const char* l_nature_names[] = {"full edge", "full vertex", "simple edge", "simple vertex"};
    for(const ParsedItem& l_item : std::as_const(l_tx.m_items)){
        const Item_lv0* l_b = l_before[l_item.m_id].item();
        const Item_lv0* l_a = l_after[l_item.m_id].item();
        const auto l_nat = l_nature(l_a);
        // full edges: origin → target instead of their (normally empty) text
        const QString l_what = (l_nat == FULL_EDGE) ?
            QString("%1 → %2").arg(l_name(l_a->p.e.f.m_v_origin), l_name(l_a->p.e.f.m_v_target)) :
            (l_nat == SIMPLE_EDGE) ? QString("%1 on %2").arg(q(imageText(l_a, l_strings)), l_name(l_a->p.e.s.m_v_origin)) :
            q(imageText(l_a, l_strings));
        QString l_line = QString("#%1 %2 %3 %4").arg(l_item.m_id).arg(l_nature_names[l_nat], typeName(l_a), l_what);
        if(l_item.m_new) l_line += " — new";
        else{
            QStringList l_changes;
            QString l_flags = flagDiff(l_b->m_flags, l_a->m_flags);
            if(!l_flags.isEmpty()) l_changes << l_flags;
            const QString l_text_b = imageText(l_b, l_strings);
            const QString l_text_a = imageText(l_a, l_strings);
            if(l_text_b != l_text_a) l_changes << QString("text %1 → %2").arg(q(l_text_b), q(l_text_a));
            auto l_cmp = [&l_changes](const char* p_name, ItemID p_b, ItemID p_a){
                if(p_b != p_a) l_changes << QString("%1 %2 → %3").arg(p_name, idStr(p_b), idStr(p_a));
            };
            switch(l_nat){
            case FULL_VERTEX:
                l_cmp("first edge", l_b->p.v.f.m_first_edge, l_a->p.v.f.m_first_edge);
                l_cmp("first special edge", l_b->p.v.f.m_first_edge_special, l_a->p.v.f.m_first_edge_special);
                l_cmp("auto edge", l_b->p.v.f.m_auto_edge, l_a->p.v.f.m_auto_edge);
                if(l_b->p.v.f.m_incoming_edges != l_a->p.v.f.m_incoming_edges)
                    l_changes << QString("incoming %1 → %2").arg(l_b->p.v.f.m_incoming_edges).arg(l_a->p.v.f.m_incoming_edges);
                break;
            case FULL_EDGE:
                l_cmp("origin", l_b->p.e.f.m_v_origin, l_a->p.e.f.m_v_origin);
                l_cmp("target", l_b->p.e.f.m_v_target, l_a->p.e.f.m_v_target);
                l_cmp("previous", l_b->p.e.f.m_e_previous, l_a->p.e.f.m_e_previous);
                l_cmp("next", l_b->p.e.f.m_e_next, l_a->p.e.f.m_e_next);
                l_cmp("reciprocal", l_b->p.e.f.m_e_reciprocal, l_a->p.e.f.m_e_reciprocal);
                l_cmp("first edge", l_b->p.e.f.m_first_edge, l_a->p.e.f.m_first_edge);
                if(l_b->p.e.f.m_incoming_edges != l_a->p.e.f.m_incoming_edges)
                    l_changes << QString("incoming %1 → %2").arg(l_b->p.e.f.m_incoming_edges).arg(l_a->p.e.f.m_incoming_edges);
                break;
            case SIMPLE_EDGE:
                l_cmp("origin", l_b->p.e.s.m_v_origin, l_a->p.e.s.m_v_origin);
                l_cmp("previous", l_b->p.e.s.m_e_previous, l_a->p.e.s.m_e_previous);
                l_cmp("next", l_b->p.e.s.m_e_next, l_a->p.e.s.m_e_next);
                break;
            default:
                break;
            }
            if(typeName(l_b) != typeName(l_a)) l_changes << QString("type %1 → %2").arg(typeName(l_b), typeName(l_a));
            l_line += l_changes.isEmpty() ? QString(" — dates only") : QString(" — %1").arg(l_changes.join(", "));
        }
        l_ret.m_details << l_line;
    }
    return l_ret;
}
