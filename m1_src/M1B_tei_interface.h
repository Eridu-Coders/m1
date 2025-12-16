#ifndef M1B_TEI_INTERFACE_H
#define M1B_TEI_INTERFACE_H

#include <QString>
#include <QXmlStreamReader>

namespace M1Store{

class Form{
private:
    QString m_lemma;
    QString m_grammar;
    QString m_pos;
public:
    Form(const QString& p_lemma, const QString& p_grammar, const QString& p_pos){m_lemma = p_lemma; m_grammar = p_grammar; m_pos = p_pos;}
};

class TEIInterface{
private:
    static QMap<QString, Form> cm_forms;
    static QString skipUntil(int p_indent_count, QString& p_indent, QXmlStreamReader& p_xml_reader, const QString& p_elem_close);
    static void loadTeiInternal(const QString& p_file_path, bool p_full_load=false);
public:
    static void init();
    static void loadTei(const QString& p_file_path);
};

} // end of namespace M1Store

#endif // M1B_TEI_INTERFACE_H
