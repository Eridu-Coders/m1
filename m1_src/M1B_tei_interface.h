#ifndef M1B_TEI_INTERFACE_H
#define M1B_TEI_INTERFACE_H

#include <QString>
#include <QXmlStreamReader>

namespace M1Store{

class TEIInterface{
private:
    static QString skipUntil(int p_indent_count, QString& p_indent, QXmlStreamReader& p_xml_reader, const QString& p_elem_close);
    static void loadTeiInternal(const QString& p_file_path, bool p_full_load=false);
public:
    static void init();
    static void loadTei(const QString& p_file_path);
};

} // end of namespace M1Store

#endif // M1B_TEI_INTERFACE_H
