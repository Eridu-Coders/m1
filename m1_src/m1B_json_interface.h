#ifndef M1B_JSON_INTERFACE_H
#define M1B_JSON_INTERFACE_H

#include "m1B_lv2_item.h"

namespace M1Store {

class JsonInterface
{
private:
    static M1Store::Item_lv2* cm_text_root;
    static M1Store::Item_lv2* cm_lexicon_root;
    static int cm_cur_chapter_number;
    static QString cm_indent;
    static QMap<QString, M1Store::Item_lv2*> m_form_map;

public:
    static void init();
    static void loadJson(const QString& p_file_path);
};

} // namespace M1Store

#endif // M1B_JSON_INTERFACE_H
