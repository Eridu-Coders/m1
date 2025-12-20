#ifndef M1B_TEI_INTERFACE_H
#define M1B_TEI_INTERFACE_H

#include <QString>
#include <QXmlStreamReader>

#include "m1B_lv2_item.h"

namespace M1Store{

class TEIInterface{
private:
    static M1Store::Item_lv2* cm_text_root;

    static QString skipUntil(int p_indent_count, QString& p_indent, QXmlStreamReader& p_xml_reader, const QString& p_elem_close);
    static void loadTeiInternal(const QString& p_file_path, bool p_validate_only=false);
    static void create_text(const QString& p_title, const QString& p_alt_title, const QString& p_sub_title, const QString& p_author_text);
public:
    static void init();
    static void loadTei(const QString& p_file_path);
};

} // end of namespace M1Store

#endif // M1B_TEI_INTERFACE_H
