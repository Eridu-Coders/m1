#include <QRegularExpression>

#include <gtest/gtest.h>

#include "m1B_lv2_item.h"
#include "m1A_env.h"

Q_LOGGING_CATEGORY(g_cat_lv2_test, "lv2.test")

QRegularExpression g_re_punc(R"(^(\W*)(\w+)(\W*))");
QRegularExpression g_re_space(R"(\s+)");

TEST(ItemLv2Test, BasicTest){
    M1_FUNC_ENTRY(g_main_test, QString("ItemLv2Test/BasicTest test "))

    M1Store::Item_lv2* l_root = M1Store::Item_lv2::getExisting("TYPE_");
    M1Env::M1EnvStatic::setSilentMode(true);

    qCDebug(g_cat_silence) << "+++++++++++++++++++++++++++++++++++++++++++++++++++++";
    qCDebug(g_cat_silence) << l_root->dbgString().toUtf8().constData();
    qCDebug(g_cat_silence) << "+++++++++++++++++++++++++++++++++++++++++++++++++++++";
    //M1Env::M1EnvStatic::setSilentMode(false);
    //M1Store::Item_lv2_iterator it = l_root->getIteratorTop();
    //qCDebug(g_cat_lv2_test) << "+++++++++++++++++++++++++++++++++++++++++++++++++++++";
    for(M1Store::Item_lv2_iterator it = l_root->getIteratorTop(); !it.beyondEnd(); it.next())
        qCDebug(g_cat_silence) << it.at()->dbgShort();
    qCDebug(g_cat_silence) << "+++++++++++++++++++++++++++++++++++++++++++++++++++++";
    for(M1Store::Item_lv2_iterator it = l_root->getIteratorSpecial(); !it.beyondEnd(); it.next())
        qCDebug(g_cat_silence) << it.at()->dbgShort();

    qCDebug(g_cat_silence) << "=====================================================";
    M1Store::Item_lv2* l_new = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX,
        M1Store::ItemType(),    // type
        "Mr. Test Vertex"       // label
        );
    l_new->linkTo(l_root, "BLNGS", nullptr, true);
    l_new->setType("PERSN");

    M1Store::Item_lv2* l_taratata = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX,
        M1Store::ItemType(),    // type
        "Taratata"              // label
        );
    M1Store::Item_lv2* l_blng_edge = l_taratata->linkTo(l_new, "BLNGS", nullptr, true);
    qCDebug(g_cat_silence) << QString("Edge to be used as p_edge_above: %1").arg(l_blng_edge->dbgShort());

    M1Store::Item_lv2* l_toto = M1Store::Item_lv2::getNew(
        // category & attributes
        M1Store::FULL_VERTEX,
        M1Store::ItemType(),    // type
        "toto"                  // label
        );
    l_taratata->linkTo(l_toto, "OWNS_", l_blng_edge);
    qCDebug(g_cat_silence) << l_taratata->dbgString().toUtf8().constData();

    //--------------------------------- words -----------------------------------------------------------
    // lexicon building
    M1Store::Item_lv2* l_words_root = M1Store::Item_lv2::getExisting(M1Env::TEXT_WORD_SPECIAL_ID);
    QFile l_fin_words("../../Enoch-words.txt");
    l_fin_words.open(QFile::ReadOnly | QFile::Text);
    QTextStream l_in_words(&l_fin_words);
    QString l_txt_words(l_in_words.readAll());
    QStringList l_lex_entries = l_txt_words.split("\n");
    std::map<QString, M1Store::Item_lv2*> l_lex_2_word;
    for ( const auto& l_lex : l_lex_entries ){
        qCDebug(g_cat_silence) << "Lexical element: " + l_lex;
        M1Store::Item_lv2* l_lex_item = M1Store::Item_lv2::getNew(
            M1Store::FULL_VERTEX,                           // category & attributes (flags)
            M1Store::ItemType(M1Env::TEXT_WORD_SPECIAL_ID), // type
            l_lex.toUtf8().constData()                      // label
            );
        l_lex_item->linkTo(l_words_root, "BLNGS", nullptr, true);
        l_lex_2_word[l_lex] = l_lex_item;
    }

    // Text and its sections vertex
    M1Store::Item_lv2* l_enoch_text = M1Store::Item_lv2::getNew(
        M1Store::FULL_VERTEX,                      // category & attributes (flags)
        M1Store::ItemType(M1Env::TEXT_SPECIAL_ID), // type
        "Book of Enoch, version B"                 // label
        );
    M1Store::Item_lv2* l_text_root = M1Store::Item_lv2::getExisting(M1Env::TEXT_SPECIAL_ID);
    l_enoch_text->linkTo(l_text_root, "BLNGS", nullptr, true);

    M1Store::Item_lv2* l_enoch_sections = M1Store::Item_lv2::getNew(
        M1Store::FULL_VERTEX,  // category & attributes (flags)
        M1Store::ItemType(),   // type (none)
        "Sentences of EB"      // label
        );
    l_enoch_sections->linkTo(l_enoch_text, "BLNGS", nullptr, true);

    // Occurence edges and their connexions
    QFile l_fin("../../Enoch B.txt");
    l_fin.open(QFile::ReadOnly | QFile::Text);
    QTextStream l_in(&l_fin);
    QString l_txt(l_in.readAll());
    QStringList l_words = l_txt.split(" ");
    M1Store::Item_lv2* l_last_edge = nullptr;
    M1Store::Item_lv2* l_current_section = nullptr;
    M1Store::Item_lv2* l_sections_last_edge = nullptr;
    M1Store::Item_lv2* l_cursec_last_edge = nullptr;

    int l_section_count = 1;
    for ( const auto& l_word : l_words ){
        // Extract punctuation from word, if any
        QRegularExpressionMatch l_match = g_re_punc.match(l_word);
        QString l_punc_left, l_punc_right;
        QString l_bare_word = l_word;
        if(l_match.hasMatch()){
            l_punc_left = l_match.captured(1).replace(g_re_space, "");
            l_bare_word = l_match.captured(2);
            l_punc_right = l_match.captured(3).replace(g_re_space, "");
        }

        // Create the word occurence
        M1Store::Item_lv2* l_the_lex_word = l_lex_2_word[l_bare_word];
        l_last_edge = l_enoch_text->linkTo(l_the_lex_word, M1Env::TW_WORD_OCC_SIID, l_last_edge, false);
        l_the_lex_word->linkTo(l_last_edge, M1Env::TW_REV_WORD_OCC_SIID);

        // Create a new section (sentence) if appropriate
        if(l_current_section == nullptr){
            l_current_section = M1Store::Item_lv2::getNew(
                M1Store::FULL_VERTEX,                                                 // category & attributes (flags)
                M1Store::ItemType(M1Env::TEXT_SECTION_SPECIAL_ID),                    // type
                QString("EB Sentence %1").arg(l_section_count++).toUtf8().constData() // label
                );
            l_sections_last_edge = l_enoch_sections->linkTo(l_current_section, "OWNS_", l_sections_last_edge, false);
            l_cursec_last_edge = l_current_section->linkTo(l_last_edge, M1Store::TW_SECTION_2_OCC_BEGIN_SIID);
        }
        else
        // Terminates the current section (sentence) if appropriate
        if(l_punc_right.contains(".")){
            l_current_section->linkTo(l_last_edge, M1Store::TW_REV_SECTION_2_OCC_END_SIID, l_cursec_last_edge, false);
            l_current_section = nullptr;
            l_cursec_last_edge = nullptr;
        }

        qCDebug(g_cat_silence) << QString("[%1] %2 [%3]")
                                      .arg(l_punc_left)
                                      .arg(l_bare_word)
                                      .arg(l_punc_right).toUtf8().constData();;
    }

    M1Env::M1EnvStatic::setSilentMode(false);
    M1_FUNC_EXIT
}
