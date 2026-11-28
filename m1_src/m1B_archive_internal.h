#ifndef M1B_ARCHIVE_INTERNAL_H
#define M1B_ARCHIVE_INTERNAL_H

/**
 * @file m1B_archive_internal.h
 * @brief (internal to m1B_archive*.cpp) archive log format constants, serialization helpers and transaction body parser
 */

#include <cstring>

#include <QByteArray>
#include <QString>
#include <QVector>

#include "m1B_store.h"

namespace M1Store{
namespace ArchiveInternal{

const quint32 MAGIC_BEGIN  = 0x4742314D;    ///< 'M1BG' (little endian)
const quint32 MAGIC_TX     = 0x5854314D;    ///< 'M1TX'
const quint32 MAGIC_COMMIT = 0x4D43314D;    ///< 'M1CM'
/// 1: initial format; 2: COUNTERS record also holds the counter values at transaction start (for undo)
const quint32 FORMAT_VERSION = 2;

// record types
const quint8 REC_END      = 0xFF;
const quint8 REC_COUNTERS = 1;
const quint8 REC_ITEM     = 2;
const quint8 REC_SPECIAL  = 3;
const quint8 REC_STR_PUT  = 4;
const quint8 REC_STR_DEL  = 5;
const quint8 REC_UTIL_U32 = 6;

const qsizetype ITEM_BYTES = sizeof(M1Store::Item_lv0);
const qsizetype SPECIAL_BYTES = sizeof(M1Store::SpecialItem);

// ------------------------------- serialization helpers -------------------------------
template<typename T> inline void put(QByteArray& p_buf, const T p_val){p_buf.append(reinterpret_cast<const char*>(&p_val), sizeof(T));}
inline void putBytes(QByteArray& p_buf, const QByteArray& p_data){put<quint32>(p_buf, p_data.size()); p_buf.append(p_data);}

/// bounds-checked reader over a byte buffer
struct Reader{
    const char* m_data;
    qsizetype m_len;
    qsizetype m_pos = 0;
    bool m_ok = true;
    Reader(const char* p_data, qsizetype p_len) : m_data(p_data), m_len(p_len){}
    template<typename T> T get(){
        T l_val{};
        if(!m_ok || m_pos + (qsizetype)sizeof(T) > m_len){m_ok = false; return l_val;}
        memcpy(&l_val, m_data + m_pos, sizeof(T));
        m_pos += sizeof(T);
        return l_val;
    }
    const char* raw(qsizetype p_n){
        if(!m_ok || p_n < 0 || m_pos + p_n > m_len){m_ok = false; return nullptr;}
        const char* l_ret = m_data + m_pos;
        m_pos += p_n;
        return l_ret;
    }
    QByteArray bytes(){
        quint32 l_n = get<quint32>();
        const char* l_p = raw(l_n);
        return l_p == nullptr ? QByteArray() : QByteArray(l_p, l_n);
    }
};

/// standard CRC-32 (IEEE 802.3, reflected, poly 0xEDB88320)
inline quint32 crc32(const char* p_data, qsizetype p_len){
    static quint32 l_table[256];
    static bool l_init = false;
    if(!l_init){
        for(quint32 i = 0; i < 256; i++){
            quint32 c = i;
            for(int k = 0; k < 8; k++) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            l_table[i] = c;
        }
        l_init = true;
    }
    quint32 l_crc = 0xFFFFFFFFu;
    for(qsizetype i = 0; i < p_len; i++)
        l_crc = l_table[(l_crc ^ static_cast<quint8>(p_data[i])) & 0xFF] ^ (l_crc >> 8);
    return l_crc ^ 0xFFFFFFFFu;
}

// ------------------------------- parsed transaction body -------------------------------
// (pointers point into the body buffer, which must outlive the ParsedTx)

struct ParsedItem{
    ItemID m_id;
    bool m_new;                 ///< the item was allocated by the transaction
    const char* m_before;       ///< ITEM_BYTES
    const char* m_after;        ///< ITEM_BYTES
};
struct ParsedSpecial{
    SpecialItemID m_id;
    const char* m_before;       ///< SPECIAL_BYTES
    const char* m_after;        ///< SPECIAL_BYTES
    QString m_icon_path;        ///< icon path after the transaction
};
struct ParsedOp{
    quint8 m_type;              ///< REC_STR_PUT, REC_STR_DEL or REC_UTIL_U32
    quint64 m_id = 0;           ///< string ID (string ops)
    QByteArray m_bytes;         ///< string value (string ops)
    quint32 m_key = 0;          ///< util key (REC_UTIL_U32)
    quint32 m_before = 0;
    quint32 m_after = 0;
};
struct ParsedTx{
    bool m_ok = false;
    quint32 m_version = 0;
    quint64 m_seq = 0;
    qint64 m_timestamp_ms = 0;
    QString m_label;

    bool m_has_counters = false;
    bool m_has_before_counters = false;     ///< format version >= 2
    ItemID m_next_item = 0, m_next_item_before = 0;
    StringID m_next_string = 0, m_next_string_before = 0;
    SpecialItemID m_next_special = 0, m_next_special_before = 0;
    quint32 m_next_seg = 0;

    QVector<ParsedItem> m_items;
    QVector<ParsedSpecial> m_specials;
    QVector<ParsedOp> m_ops;                ///< chronological order
};

/// parse a transaction body (CRC already checked). Returns ParsedTx::m_ok
inline bool parseTransaction(const char* p_body, qsizetype p_len, ParsedTx& p_tx){
    Reader l_r(p_body, p_len);
    p_tx.m_version = l_r.get<quint32>();
    if(p_tx.m_version < 1 || p_tx.m_version > FORMAT_VERSION) return p_tx.m_ok = false;
    p_tx.m_seq = l_r.get<quint64>();
    p_tx.m_timestamp_ms = l_r.get<qint64>();
    p_tx.m_label = QString::fromUtf8(l_r.bytes());

    bool l_end = false;
    while(l_r.m_ok && !l_end){
        const quint8 l_type = l_r.get<quint8>();
        switch(l_type){
        case REC_END:
            l_end = true;
            break;
        case REC_COUNTERS:
            p_tx.m_has_counters = true;
            p_tx.m_next_item = l_r.get<quint64>();
            p_tx.m_next_string = l_r.get<quint64>();
            p_tx.m_next_special = l_r.get<quint16>();
            p_tx.m_next_seg = l_r.get<quint32>();
            if(p_tx.m_version >= 2){
                p_tx.m_has_before_counters = true;
                p_tx.m_next_item_before = l_r.get<quint64>();
                p_tx.m_next_string_before = l_r.get<quint64>();
                p_tx.m_next_special_before = l_r.get<quint16>();
            }
            break;
        case REC_ITEM:{
            ParsedItem l_item;
            l_item.m_id = l_r.get<quint64>();
            l_item.m_new = l_r.get<quint8>() != 0;
            l_item.m_before = l_r.raw(ITEM_BYTES);
            l_item.m_after = l_r.raw(ITEM_BYTES);
            if(l_r.m_ok) p_tx.m_items.append(l_item);
            break;
        }
        case REC_SPECIAL:{
            ParsedSpecial l_special;
            l_special.m_id = l_r.get<quint16>();
            l_special.m_before = l_r.raw(SPECIAL_BYTES);
            l_special.m_after = l_r.raw(SPECIAL_BYTES);
            l_special.m_icon_path = QString::fromUtf8(l_r.bytes());
            if(l_r.m_ok) p_tx.m_specials.append(l_special);
            break;
        }
        case REC_STR_PUT:
        case REC_STR_DEL:{
            ParsedOp l_op;
            l_op.m_type = l_type;
            l_op.m_id = l_r.get<quint64>();
            l_op.m_bytes = l_r.bytes();
            if(l_r.m_ok) p_tx.m_ops.append(l_op);
            break;
        }
        case REC_UTIL_U32:{
            ParsedOp l_op;
            l_op.m_type = l_type;
            l_op.m_key = l_r.get<quint32>();
            l_op.m_before = l_r.get<quint32>();
            l_op.m_after = l_r.get<quint32>();
            if(l_r.m_ok) p_tx.m_ops.append(l_op);
            break;
        }
        default:
            l_r.m_ok = false;
        }
    }
    return p_tx.m_ok = l_r.m_ok && l_end && p_tx.m_has_counters;
}

} // namespace ArchiveInternal
} // namespace M1Store

#endif // M1B_ARCHIVE_INTERNAL_H
