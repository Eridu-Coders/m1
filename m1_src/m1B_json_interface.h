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
    static int cm_cur_chapter_number;
    static QString cm_indent;
    // static QMap<QString, M1Store::Item_lv2*> m_form_map;
    static std::map<QString, M1Store::Item_lv2*> cm_lemma_map;

    static M1Store::Item_lv2* create_form(const QString& p_form_text, const QString& p_tag_text, const QJsonArray& p_lemma_keys);
public:
    static void init();
    static void loadJson(const QString& p_file_path);
};

} // namespace M1Store

#endif // M1B_JSON_INTERFACE_H
