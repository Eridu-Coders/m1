#ifndef M1B_TEI_INTERFACE_H
#define M1B_TEI_INTERFACE_H

#include <QString>
#include <QXmlStreamReader>

#include "m1B_lv2_item.h"

namespace M1Store{

enum class OrgOrPerson{
    Organization = 1,
    Person
};

enum class ReturnEntityOrRole{
    Entity = 1,
    RoleFolder
};

// list of secondary authors
class AuthorTEI{
private:
    QString m_name;
    QString m_role;
public:
    AuthorTEI(const QString& p_name, const QString& p_role){m_name = p_name; m_role = p_role;}

    const QString& name(){return m_name;}
    const QString& role(){return m_role;}
};

// Lexicon form Data
class FormLexicon{
private:
    QString m_orth;
    QString m_grammar;
public:
    FormLexicon(const QString& p_orth, const QString& p_grammar){m_orth = p_orth; m_grammar = p_grammar;}

    const QString& form(){return m_orth;}
    const QString& grammar(){return m_grammar;}
};

// Data for WfW
class Form_WfW{
private:
    QString m_form;
    QString m_lemma;
    QString m_pos;
    QString m_ref;
    QString m_grammar;
public:
    Form_WfW(const QString& p_form,
             const QString& p_lemma,
             const QString& p_pos,
             const QString& p_ref,
             const QString& p_grammar){
        m_form = p_form;
        m_lemma = p_lemma;
        m_pos = p_pos;
        m_ref = p_ref;
        m_grammar = p_grammar;
    }

    const QString& form(){return m_form;}
    const QString& lemma(){return m_lemma;}
    const QString& pos(){return m_pos;}
    const QString& ref(){return m_ref;}
    const QString& grammar(){return m_grammar;}

    const QString dbgOneLiner(){return QString("%1 [%2 %3 %5] %4").arg(m_form).arg(m_lemma).arg(m_pos).arg(m_ref).arg(m_grammar);}
};

class TEIInterface{
private:
    static M1Store::Item_lv2* cm_text_root;
    static M1Store::Item_lv2* cm_lexicon_root;
    static M1Store::Item_lv2* cm_cur_chapter_vertex;
    static M1Store::Item_lv2* cm_cur_sloka_vertex;
    static int cm_cur_chapter_number;
    static int cm_cur_sloka_number;
    static QString cm_indent;
    static QList<AuthorTEI> m_author_list;

    static QString skipUntil(int p_indent_count, QString& p_indent, QXmlStreamReader& p_xml_reader, const QString& p_elem_close);
    static void loadTeiInternal(const QString& p_file_path, bool p_validate_only=false);
    static void create_text(const QString& p_title, const QString& p_alt_title, const QString& p_sub_title, const QString& p_author_text);
    static void addTranslationBhashya(const QString& p_translation_text, const QString& p_source, const QString& p_author_text, const QString& p_author_role, bool p_is_translation);
    static void create_Lexicon_Entry(const QString& p_lemma_text, const QString& p_pos_text, const QString& p_url_dict_list, QList<FormLexicon>& p_form_list);
    static void create_wfw_unit(const QString& p_sk_segment, const QString& p_transliteration, const QString& p_translation, QList<Form_WfW>& p_form_list);

    static M1Store::Item_lv2* getPersonOrg(const QString& p_name, const QString& p_role, OrgOrPerson p_is_person, ReturnEntityOrRole p_return);
public:
    static void init();
    static void loadTei(const QString& p_file_path);
};

} // end of namespace M1Store

#endif // M1B_TEI_INTERFACE_H
