#ifndef M1B_JSON_INTERFACE_H
#define M1B_JSON_INTERFACE_H

#include "m1B_lv2_item.h"

class QJsonArray;

namespace M1Store {

class JsonInterface
{
private:
    static M1Store::Item_lv2* cm_text_root;
    static M1Store::Item_lv2* cm_lexicon_root;
    static M1Store::Item_lv2* cm_cur_book;
    static M1Store::Item_lv2* cm_cur_sentence;
    static M1Store::Item_lv2* cm_section_folder;
    static QString cm_indent;
    // static QMap<QString, M1Store::Item_lv2*> m_form_map;
    static std::map<QString, M1Store::Item_lv2*> cm_lemma_map;
    static std::map<QString, M1Store::Item_lv2*> cm_form_map;
    static std::map<QString, M1Store::Item_lv2*> cm_section_map;
    static QStringList cm_form_gr_done;
    static int cm_cur_sentence_number;
    static int cm_cur_book_number;

    static M1Store::Item_lv2* create_form(const QString& p_form_text, const QString& p_tag_text, const QJsonArray& p_lemma_keys);
    static void add_word(const QString& p_occ_text,
                         const QString& p_occ_id,
                         const QString& p_new_stephanus_section,
                         const QString& p_pos,
                         const QString& p_tag,
                         const QString& p_sentence_position,
                         const QString& p_punct_left,
                         const QString& p_punct_right,
                         const QString& p_mkp_before,
                         const QString& p_mkp_after,
                         const QString& p_form_key,
                         const QStringList& p_gram_penta_list,
                         const QString& p_note_key);
public:
    static void init();
    static void loadJson(const QString& p_file_path);
};

} // namespace M1Store

#endif // M1B_JSON_INTERFACE_H
