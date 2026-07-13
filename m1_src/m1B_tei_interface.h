#ifndef M1B_TEI_INTERFACE_H
#define M1B_TEI_INTERFACE_H

#include <stack>

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

enum class TEIEntityType{
    Div = 1,
    Paragraph,
    ChunkName,
    ChunkHi,
    ChunkTerm,
    ChunkHeading,
    ChunkNote
};

// list of secondary authors
class AuthorTEI{
private:
    QString m_name;
    QString m_role;
public:
    AuthorTEI(const QString& p_name, const QString& p_role){set(p_name, p_role);}
    AuthorTEI(){}

    void set(const QString& p_name, const QString& p_role){m_name = p_name; m_role = p_role;}
    const QString& name() const {return m_name;}
    const QString& role() const {return m_role;}
    const bool isEmpty() const {return m_name.length() == 0;}
};

// Lexicon form Data
class FormLexicon{
private:
    QString m_orth;
    QString m_grammar;
public:
    FormLexicon(const QString& p_orth, const QString& p_grammar){m_orth = p_orth; m_grammar = p_grammar;}

    const QString& form() const {return m_orth;}
    const QString& grammar() const {return m_grammar;}
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

    const QString& form() const {return m_form;}
    const QString& lemma() const {return m_lemma;}
    const QString& pos() const {return m_pos;}
    const QString& ref() const {return m_ref;}
    const QString& grammar() const {return m_grammar;}

    const QString dbgOneLiner() const {return QString("%1 [%2 %3 %5] %4").arg(m_form).arg(m_lemma).arg(m_pos).arg(m_ref).arg(m_grammar);}
};

class TEIStackLevel{
private:
    TEIEntityType m_type;
    M1Store::Item_lv2* m_item;
public:
    TEIStackLevel(TEIEntityType p_type, M1Store::Item_lv2* p_item){m_type = p_type; m_item = p_item;}
    TEIEntityType type(){return m_type;}
    M1Store::Item_lv2* item(){return m_item;}
};

class TEIInterface{
    friend class JsonInterface;
private:
    // class members for loadTeiInternal() only
    static std::stack<TEIStackLevel> cm_tei_elements_stack;
    static int cm_cur_paragraph_number;

    // class members for both loadTeiInternal() and loadTeiInternal1()
    static M1Store::Item_lv2* cm_text_root;
    static QString cm_indent;
    static std::map<QString, M1Store::Item_lv2*> cm_cache_tei_ent;
    static QList<AuthorTEI> cm_author_list;

    // class members used only by loadTeiInternal1()
    static M1Store::Item_lv2* cm_lexicon_root;
    static M1Store::Item_lv2* cm_cur_chapter_vertex;
    static M1Store::Item_lv2* cm_cur_sloka_vertex;
    static int cm_cur_chapter_number;
    static int cm_cur_sloka_number;
    static QMap<QString, M1Store::Item_lv2*> cm_form_map;
    static QXmlStreamReader::TokenType cm_tt;
    static bool cm_no_read;

    static void loadTeiInternal(const QString& p_file_path, bool p_validate_only=false);
    static void loadTeiInternal1(const QString& p_file_path, bool p_validate_only=false);
    // static void loadTeiInternal2(const QString& p_file_path, bool p_validate_only=false);
    // static void loadTeiInternal3(const QString& p_file_path, bool p_validate_only=false);

    static M1Store::Item_lv2* getPersonOrg(
        const QString& p_name,
        const QString& p_role=QString(),
        OrgOrPerson p_is_person=OrgOrPerson::Person,
        ReturnEntityOrRole p_return=ReturnEntityOrRole::Entity);
    static QString skipUntil(int p_indent_count, QString& p_indent, QXmlStreamReader& p_xml_reader, const QString& p_elem_close);
    static AuthorTEI readPersName(QXmlStreamReader& p_xml_reader);
    static void create_text(
        const QString& p_title,
        const QString& p_alt_title,
        const QString& p_sub_title,
        const QString& p_author_text,
        const bool p_with_lexicon = false,
        M1Store::Item_lv2** p_text_root = nullptr,
        M1Store::Item_lv2** p_lexicon_root = nullptr
    );
    static void addTranslationBhashya(
        const QString& p_translation_text,
        const QString& p_language,
        const QString& p_source,
        const AuthorTEI& p_main_author,
        const AuthorTEI& p_secondary_author,
        bool p_is_translation,
        bool p_sep_above);
    static M1Store::Item_lv2* create_Lemma(const QString& p_lemma_text, const QString& p_pos_text, const QString& p_url_dict_list, M1Store::Item_lv2** p_lexicon_root = nullptr);
    static void create_Lexicon_Entry(const QString& p_lemma_text, const QString& p_pos_text, const QString& p_url_dict_list, QList<FormLexicon>& p_form_list);
    static void create_wfw_unit(
        const QString& p_sk_segment,
        const QString& p_transliteration,
        const QString& p_translation,
        QList<Form_WfW>& p_form_list,
        bool p_sep_above);
    static M1Store::Item_lv2* create_div(
        const QString& p_type,
        const QString& p_div_n,
        const QString& p_xml_id=QString());
    static M1Store::Item_lv2* create_paragraph();
    static M1Store::Item_lv2* create_chunk(
        const QString& p_text,
        const QString& p_tag,
        const QString& p_type=QString(),
        const QString& p_attr_n=QString(),
        const QString& p_note_place=QString(),
        const QString& p_hi_rend=QString(),
        const QString& p_xml_id=QString()
    );

    static QString format_tt_token(const QXmlStreamReader& p_xml_reader);
public:
    static void init();
    static void loadTei1(const QString& p_file_path);
    static void loadTeiGeneric(const QString& p_file_path);
    // static void loadTei2(const QString& p_file_path);
    // static void loadTei3(const QString& p_file_path);
};

} // end of namespace M1Store

#endif // M1B_TEI_INTERFACE_H
