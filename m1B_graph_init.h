
#include "m1A_constants.h"

namespace M1Env{

// [FOLDR] Special Vertex ID (Vertex type) of folders (ordinary vertices with no special role)
extern M1Env::SpecialItemID FOLDER_SIID;
// [AUTO_] Special Vertex ID (Edge type) of AUTO edges
extern M1Env::SpecialItemID AUTO_SIID;
// [OWNS_] Special Vertex ID (Edge type) of ownership edges
extern M1Env::SpecialItemID OWNS_SIID;
// [BLNGS] Special Vertex ID (Edge type) of ownership edges (reciprocal of OWNS_ above))
extern M1Env::SpecialItemID BLNGS_SIID;
// [_ISA_] Special Vertex ID (Edge type) of type edges
extern M1Env::SpecialItemID ISA_SIID;
// [_ITO_] Special Vertex ID (Edge type) of type edges (reciprocal of _ISA_ above))
extern M1Env::SpecialItemID ITO_SIID;
// [WROTE] Special Vertex ID (Edge type) wrote / written by
extern M1Env::SpecialItemID TEXT_WROTE_SIID;
// [WRTBY] Special Vertex ID (Edge type) wrote / written by (reciprocal of WROTE above))
extern M1Env::SpecialItemID TEXT_WRITTEN_BY_SIID;
// [OCCUR] Special Vertex ID (Edge type) of occurrence edges
extern M1Env::SpecialItemID OCCUR_SIID;
// [FORM_] Special Vertex ID (Vertex type) of word form vertices
extern M1Env::SpecialItemID FORM_SIID;
// [CAPTL] Special Vertex ID (Simple edge type) of Occurrence field capitalization flag
extern M1Env::SpecialItemID CAPTL_SIID;
// [PCTLF] Special Vertex ID (Simple edge type) of Occurrence field left punctuation
extern M1Env::SpecialItemID PCTLF_SIID;
// [PCTRT] Special Vertex ID (Simple edge type) of Occurrence field right punctuation
extern M1Env::SpecialItemID PCTRT_SIID;
// [MKPLF] Special Vertex ID (Simple edge type) of Occurrence field markup left
extern M1Env::SpecialItemID MKPLF_SIID;
// [MKPRT] Special Vertex ID (Simple edge type) of Occurrence field markup right
extern M1Env::SpecialItemID MKPRT_SIID;
// [STPOS] Special Vertex ID (Simple edge type) of Occurrence field sentence position
extern M1Env::SpecialItemID STPOS_SIID;
// [HLCLR] Color of text highlight category
extern M1Env::SpecialItemID HLCLR_SIID;
// [PRABH] Special Vertex ID (Vertex type) of Gita translation source: Prabhupada
extern M1Env::SpecialItemID TEXT_WFW_PRABUPADA_SIID;
// [SIVAN] Special Vertex ID (Vertex type) of Gita translation source: Sivananda
extern M1Env::SpecialItemID TEXT_WFW_SIVANANDA_SIID;
// [GAMBI] Special Vertex ID (Vertex type) of Gita translation source: Gambirananda
extern M1Env::SpecialItemID TEXT_WFW_GAMBIRANANDA_SIID;
// [TRSLT] Special Vertex ID (Simple Edge type) of Gita transliteration field
extern M1Env::SpecialItemID TEXT_WORD_TRANSLIT_SIID;
// [DCTRF] Special Vertex ID (Simple Edge type) of Gita dictionary ref. field
extern M1Env::SpecialItemID TEXT_WORD_DICT_REF_SIID;
// [WFTRN] Special Vertex ID (Simple Edge type) WfW unit Translation field
extern M1Env::SpecialItemID TEXT_WFW_TRANSL_SIID;
// [URLNK] Special Vertex ID (Vertex type) Url link
extern M1Env::SpecialItemID TEXT_URL_LINK_SIID;
// [HS2OB] Special Vertex ID (Edge type) begin section + reciprocal
extern M1Env::SpecialItemID TW_SECTION_2_OCC_BEGIN_SIID;
// [OC2HB] Special Vertex ID (Edge type) begin section + reciprocal (reciprocal of HS2OB above))
extern M1Env::SpecialItemID TW_REV_SECTION_2_OCC_BEGIN_SIID;
// [HS2OE] Special Vertex ID (Edge type) end section + reciprocal
extern M1Env::SpecialItemID TW_SECTION_2_OCC_END_SIID;
// [OC2HE] Special Vertex ID (Edge type) end section + reciprocal (reciprocal of HS2OE above))
extern M1Env::SpecialItemID TW_REV_SECTION_2_OCC_END_SIID;

// [ROOT_] Special Vertex ID of "Global graph root"
extern M1Env::SpecialItemID ROOT_SIID;
// [HOME_] Special Vertex ID of "Home"
extern M1Env::SpecialItemID HOME_SIID;
// [TYPE_] Special Vertex ID of "Root of all types"
extern M1Env::SpecialItemID TYPE_NODE_SIID;
// [PERSN] Special Vertex ID of "Person (type)"
extern M1Env::SpecialItemID PERS_TYPE_SIID;
// [ORGN_] Special Vertex ID of "Organization (type)"
extern M1Env::SpecialItemID ORG_TYPE_SIID;
// [ME___] Special Vertex ID of "Me"
extern M1Env::SpecialItemID ME_SIID;
// [_MSG_] Special Vertex ID of "Inboxes / Message Root Type"
extern M1Env::SpecialItemID MSG_TYPE_SIID;
// [EMAIL] Special Vertex ID of "Email Inbox"
extern M1Env::SpecialItemID EMAIL_TYPE_SIID;
// [WHTSP] Special Vertex ID of "Whatsapp Inbox"
extern M1Env::SpecialItemID WHTSP_TYPE_SIID;
// [DSCRD] Special Vertex ID of "Discord Inbox"
extern M1Env::SpecialItemID DSCRD_TYPE_SIID;
// [GCHAT] Special Vertex ID of "GChat Inbox"
extern M1Env::SpecialItemID GCHAT_TYPE_SIID;
// [SMS__] Special Vertex ID of "SMS Inbox"
extern M1Env::SpecialItemID SMS_TYPE_SIID;
// [TEXT_] Special Vertex ID of "Texts Root (type)"
extern M1Env::SpecialItemID TEXT_SIID;
// [GRATT] Special Vertex ID of "Grammar Attributes (type)"
extern M1Env::SpecialItemID GRAMMAR_ATTR_SIID;
// [LEMMA] Special Vertex ID of "Lemma (type)"
extern M1Env::SpecialItemID LEMMA_SIID;
// [NLENT] Special Vertex ID of "NLP Entity (type)"
extern M1Env::SpecialItemID NLENT_SIID;
// [NLPOS] Special Vertex ID of "NLP Pos code (type)"
extern M1Env::SpecialItemID NLPOS_SIID;
// [NLTAG] Special Vertex ID of "NLP Tag code (type)"
extern M1Env::SpecialItemID NLTAG_SIID;
// [STPSC] Special Vertex ID of "Stephanus Section (type)"
extern M1Env::SpecialItemID STEPHANUS_SIID;
// [TXTVR] Special Vertex ID of "Text Version (type)"
extern M1Env::SpecialItemID TXTVR_SIID;
// [TXTNT] Special Vertex ID of "Notes (type)"
extern M1Env::SpecialItemID TXTNT_SIID;
// [TXTCK] Special Vertex ID of "Text Chunk (type)"
extern M1Env::SpecialItemID TEXT_CHUNK_SIID;
// [TXTBK] Special Vertex ID of "Book (type)"
extern M1Env::SpecialItemID TEXT_BOOK_SIID;
// [TXSNT] Special Vertex ID of "Sentence (type)"
extern M1Env::SpecialItemID TEXT_SENTENCE_SIID;
// [TXHLT] Special Vertex ID of "Highlight (type)"
extern M1Env::SpecialItemID TEXT_HIGHLIGHT_SIID;
// [TXHLC] Special Vertex ID of "Highlight category (type)"
extern M1Env::SpecialItemID TEXT_HIGHLIGHT_CAT_SIID;
// [TXHLF] Special Vertex ID of "Highlight Folder (type)"
extern M1Env::SpecialItemID TEXT_HIGHLIGHT_FLDR_SIID;
// [TXHCF] Special Vertex ID of "Highlight Categories Folder"
extern M1Env::SpecialItemID TEXT_HIGHLIGHT_CAT_FLDR_SIID;
// [TXHCK] Special Vertex ID of "Highlight Chunk"
extern M1Env::SpecialItemID TEXT_HIGHLIGHT_CHUNK_SIID;
// [SLKLN] Special Vertex ID of "Sloka Lines (type)"
extern M1Env::SpecialItemID TEXT_SLOKA_LINE_SIID;
// [INRIA] Special Vertex ID of "INRIA Dictionary Reference (type)"
extern M1Env::SpecialItemID TEXT_WORD_DREF_INRIA_SIID;
// [SLTRN] Special Vertex ID of "Sloka Translation (type)"
extern M1Env::SpecialItemID TEXT_SLOKA_TRANSLATION_SIID;
// [SLBHS] Special Vertex ID of "Sloka Bhashya (type)"
extern M1Env::SpecialItemID TEXT_SLOKA_BHASHYA_SIID;
// [TRLUN] Special Vertex ID of "WfW Translation Unit (type)"
extern M1Env::SpecialItemID TEXT_WFW_UNIT_SIID;
// [ABBR_] Special Vertex ID of "[Abbr]-abbreviation"
extern M1Env::SpecialItemID ABBR__SIID;
// [ANMCY] Special Vertex ID of "Animacy"
extern M1Env::SpecialItemID ANMCY_SIID;
// [ASPCT] Special Vertex ID of "Aspect"
extern M1Env::SpecialItemID ASPCT_SIID;
// [CASE_] Special Vertex ID of "Case"
extern M1Env::SpecialItemID CASE__SIID;
// [CTNJY] Special Vertex ID of "[ConjType]-conjunction type"
extern M1Env::SpecialItemID CTNJY_SIID;
// [CLSVT] Special Vertex ID of "Clusivity"
extern M1Env::SpecialItemID CLSVT_SIID;
// [DEFIN] Special Vertex ID of "[Definite]-definiteness or state"
extern M1Env::SpecialItemID DEFIN_SIID;
// [DEGRE] Special Vertex ID of "Degree"
extern M1Env::SpecialItemID DEGRE_SIID;
// [DEIXI] Special Vertex ID of "[Deixis]-relative location encoded in demonstratives"
extern M1Env::SpecialItemID DEIXI_SIID;
// [DRXSF] Special Vertex ID of "[DeixisRef]-person to which deixis is relative"
extern M1Env::SpecialItemID DRXSF_SIID;
// [EVDNT] Special Vertex ID of "[Evident]-evidentiality"
extern M1Env::SpecialItemID EVDNT_SIID;
// [EPXTS] Special Vertex ID of "[ExtPos]-external part of speech"
extern M1Env::SpecialItemID EPXTS_SIID;
// [FOREI] Special Vertex ID of "[Foreign]-is this a foreign word?"
extern M1Env::SpecialItemID FOREI_SIID;
// [GENDE] Special Vertex ID of "Gender"
extern M1Env::SpecialItemID GENDE_SIID;
// [MOOD_] Special Vertex ID of "Mood"
extern M1Env::SpecialItemID MOOD__SIID;
// [NCNLS] Special Vertex ID of "[NounClass]-noun class"
extern M1Env::SpecialItemID NCNLS_SIID;
// [NTMYP] Special Vertex ID of "[NumType]-numeral type"
extern M1Env::SpecialItemID NTMYP_SIID;
// [NUMBE] Special Vertex ID of "Number"
extern M1Env::SpecialItemID NUMBE_SIID;
// [PERSO] Special Vertex ID of "Person"
extern M1Env::SpecialItemID PERSO_SIID;
// [PLRTY] Special Vertex ID of "Polarity"
extern M1Env::SpecialItemID PLRTY_SIID;
// [POLIT] Special Vertex ID of "[Polite]-politeness"
extern M1Env::SpecialItemID POLIT_SIID;
// [POSS_] Special Vertex ID of "[Poss]-possessive"
extern M1Env::SpecialItemID POSS__SIID;
// [PTRNY] Special Vertex ID of "[PronType]-pronominal type"
extern M1Env::SpecialItemID PTRNY_SIID;
// [REFLE] Special Vertex ID of "[Reflex]-reflexive"
extern M1Env::SpecialItemID REFLE_SIID;
// [TENSE] Special Vertex ID of "Tense"
extern M1Env::SpecialItemID TENSE_SIID;
// [TYPO_] Special Vertex ID of "[Typo]-is this a misspelled word?"
extern M1Env::SpecialItemID TYPO__SIID;
// [VFRBR] Special Vertex ID of "[VerbForm]-form of verb or deverbative"
extern M1Env::SpecialItemID VFRBR_SIID;
// [VTRBY] Special Vertex ID of "[VerbType]-verb type"
extern M1Env::SpecialItemID VTRBY_SIID;
// [VOICE] Special Vertex ID of "Voice"
extern M1Env::SpecialItemID VOICE_SIID;
// [NPADJ] Special Vertex ID of "[ADJ]-adjective"
extern M1Env::SpecialItemID NPADJ_SIID;
// [NPADP] Special Vertex ID of "[ADP]-adposition"
extern M1Env::SpecialItemID NPADP_SIID;
// [NPADV] Special Vertex ID of "[ADV]-adverb"
extern M1Env::SpecialItemID NPADV_SIID;
// [NPAUX] Special Vertex ID of "[AUX]-auxiliary"
extern M1Env::SpecialItemID NPAUX_SIID;
// [NPCNJ] Special Vertex ID of "[CCONJ]-coordinating conjunction"
extern M1Env::SpecialItemID NPCNJ_SIID;
// [NPDET] Special Vertex ID of "[DET]-determiner"
extern M1Env::SpecialItemID NPDET_SIID;
// [NPINT] Special Vertex ID of "[INTJ]-interjection"
extern M1Env::SpecialItemID NPINT_SIID;
// [NPNON] Special Vertex ID of "[NOUN]-noun"
extern M1Env::SpecialItemID NPNON_SIID;
// [NPNUM] Special Vertex ID of "[NUM]-numeral"
extern M1Env::SpecialItemID NPNUM_SIID;
// [NPPRT] Special Vertex ID of "[PART]-particle"
extern M1Env::SpecialItemID NPPRT_SIID;
// [NPPRN] Special Vertex ID of "[PRON]-pronoun"
extern M1Env::SpecialItemID NPPRN_SIID;
// [NPPPN] Special Vertex ID of "[PROPN]-proper noun"
extern M1Env::SpecialItemID NPPPN_SIID;
// [NPPCT] Special Vertex ID of "[PUNCT]-punctuation"
extern M1Env::SpecialItemID NPPCT_SIID;
// [NPSCJ] Special Vertex ID of "[SCONJ]-subordinating conjunction"
extern M1Env::SpecialItemID NPSCJ_SIID;
// [NPSYM] Special Vertex ID of "[SYM]-symbol"
extern M1Env::SpecialItemID NPSYM_SIID;
// [NPVRB] Special Vertex ID of "[VERB]-verb"
extern M1Env::SpecialItemID NPVRB_SIID;
// [NPXXX] Special Vertex ID of "[X]-other"
extern M1Env::SpecialItemID NPXXX_SIID;
// [NTGSE] Special Vertex ID of "[.]-punctuation mark, sentence closer"
extern M1Env::SpecialItemID NTGSE_SIID;
// [NTGCM] Special Vertex ID of "[,]-punctuation mark, comma"
extern M1Env::SpecialItemID NTGCM_SIID;
// [NTLRB] Special Vertex ID of "[-LRB-]-left round bracket"
extern M1Env::SpecialItemID NTLRB_SIID;
// [NTRRB] Special Vertex ID of "[-RRB-]-right round bracket"
extern M1Env::SpecialItemID NTRRB_SIID;
// [NTOQM] Special Vertex ID of "[``]-opening quotation mark"
extern M1Env::SpecialItemID NTOQM_SIID;
// [NTCQM] Special Vertex ID of "[""]-closing quotation mark"
extern M1Env::SpecialItemID NTCQM_SIID;
// [NTCRM] Special Vertex ID of "['']-closing quotation mark"
extern M1Env::SpecialItemID NTCRM_SIID;
// [NTGCL] Special Vertex ID of "[:]-punctuation mark, colon or ellipsis"
extern M1Env::SpecialItemID NTGCL_SIID;
// [NTDOL] Special Vertex ID of "[$]-symbol, currency"
extern M1Env::SpecialItemID NTDOL_SIID;
// [NTSRP] Special Vertex ID of "[#]-symbol, number sign"
extern M1Env::SpecialItemID NTSRP_SIID;
// [NTAFX] Special Vertex ID of "[AFX]-affix"
extern M1Env::SpecialItemID NTAFX_SIID;
// [NTGCC] Special Vertex ID of "[CC]-conjunction, coordinating"
extern M1Env::SpecialItemID NTGCC_SIID;
// [NTGCD] Special Vertex ID of "[CD]-cardinal number"
extern M1Env::SpecialItemID NTGCD_SIID;
// [NTGDT] Special Vertex ID of "[DT]-determiner"
extern M1Env::SpecialItemID NTGDT_SIID;
// [NTGEX] Special Vertex ID of "[EX]-existential there"
extern M1Env::SpecialItemID NTGEX_SIID;
// [NTGFW] Special Vertex ID of "[FW]-foreign word"
extern M1Env::SpecialItemID NTGFW_SIID;
// [NTHYP] Special Vertex ID of "[HYPH]-punctuation mark, hyphen"
extern M1Env::SpecialItemID NTHYP_SIID;
// [NTGIN] Special Vertex ID of "[IN]-conjunction, subordinating or preposition"
extern M1Env::SpecialItemID NTGIN_SIID;
// [NTGJJ] Special Vertex ID of "[JJ]-adjective (English), other noun-modifier (Chinese)"
extern M1Env::SpecialItemID NTGJJ_SIID;
// [NTJJR] Special Vertex ID of "[JJR]-adjective, comparative"
extern M1Env::SpecialItemID NTJJR_SIID;
// [NTJJS] Special Vertex ID of "[JJS]-adjective, superlative"
extern M1Env::SpecialItemID NTJJS_SIID;
// [NTGLS] Special Vertex ID of "[LS]-list item marker"
extern M1Env::SpecialItemID NTGLS_SIID;
// [NTGMD] Special Vertex ID of "[MD]-verb, modal auxiliary"
extern M1Env::SpecialItemID NTGMD_SIID;
// [NTNIL] Special Vertex ID of "[NIL]-missing tag"
extern M1Env::SpecialItemID NTNIL_SIID;
// [NTGNN] Special Vertex ID of "[NN]-noun, singular or mass"
extern M1Env::SpecialItemID NTGNN_SIID;
// [NTNNP] Special Vertex ID of "[NNP]-noun, proper singular"
extern M1Env::SpecialItemID NTNNP_SIID;
// [NTNPP] Special Vertex ID of "[NNPS]-noun, proper plural"
extern M1Env::SpecialItemID NTNPP_SIID;
// [NTNNS] Special Vertex ID of "[NNS]-noun, plural"
extern M1Env::SpecialItemID NTNNS_SIID;
// [NTPDT] Special Vertex ID of "[PDT]-predeterminer"
extern M1Env::SpecialItemID NTPDT_SIID;
// [NTPOS] Special Vertex ID of "[POS]-possessive ending"
extern M1Env::SpecialItemID NTPOS_SIID;
// [NTPRP] Special Vertex ID of "[PRP]-pronoun, personal"
extern M1Env::SpecialItemID NTPRP_SIID;
// [NTPR$] Special Vertex ID of "[PRP$]-pronoun, possessive"
extern M1Env::SpecialItemID NTPR$_SIID;
// [NTGRB] Special Vertex ID of "[RB]-adverb"
extern M1Env::SpecialItemID NTGRB_SIID;
// [NTRBR] Special Vertex ID of "[RBR]-adverb, comparative"
extern M1Env::SpecialItemID NTRBR_SIID;
// [NTRBS] Special Vertex ID of "[RBS]-adverb, superlative"
extern M1Env::SpecialItemID NTRBS_SIID;
// [NTGRP] Special Vertex ID of "[RP]-adverb, particle"
extern M1Env::SpecialItemID NTGRP_SIID;
// [NTGTO] Special Vertex ID of "[TO]-infinitival “to”"
extern M1Env::SpecialItemID NTGTO_SIID;
// [NTGUH] Special Vertex ID of "[UH]-interjection"
extern M1Env::SpecialItemID NTGUH_SIID;
// [NTGVB] Special Vertex ID of "[VB]-verb, base form"
extern M1Env::SpecialItemID NTGVB_SIID;
// [NTVBD] Special Vertex ID of "[VBD]-verb, past tense"
extern M1Env::SpecialItemID NTVBD_SIID;
// [NTVBG] Special Vertex ID of "[VBG]-verb, gerund or present participle"
extern M1Env::SpecialItemID NTVBG_SIID;
// [NTVBN] Special Vertex ID of "[VBN]-verb, past participle"
extern M1Env::SpecialItemID NTVBN_SIID;
// [NTVBP] Special Vertex ID of "[VBP]-verb, non-3rd person singular present"
extern M1Env::SpecialItemID NTVBP_SIID;
// [NTVBZ] Special Vertex ID of "[VBZ]-verb, 3rd person singular present"
extern M1Env::SpecialItemID NTVBZ_SIID;
// [NTWDT] Special Vertex ID of "[WDT]-wh-determiner"
extern M1Env::SpecialItemID NTWDT_SIID;
// [NTGWP] Special Vertex ID of "[WP]-wh-pronoun, personal"
extern M1Env::SpecialItemID NTGWP_SIID;
// [NTWP$] Special Vertex ID of "[WP$]-wh-pronoun, possessive"
extern M1Env::SpecialItemID NTWP$_SIID;
// [NTWRB] Special Vertex ID of "[WRB]-wh-adverb"
extern M1Env::SpecialItemID NTWRB_SIID;
// [NTGSP] Special Vertex ID of "[SP]-space (English), sentence-final particle (Chinese)"
extern M1Env::SpecialItemID NTGSP_SIID;
// [NTADD] Special Vertex ID of "[ADD]-email"
extern M1Env::SpecialItemID NTADD_SIID;
// [NTNFP] Special Vertex ID of "[NFP]-superfluous punctuation"
extern M1Env::SpecialItemID NTNFP_SIID;
// [NTGGW] Special Vertex ID of "[GW]-additional word in multi-word expression"
extern M1Env::SpecialItemID NTGGW_SIID;
// [NTGXX] Special Vertex ID of "[XX]-unknown"
extern M1Env::SpecialItemID NTGXX_SIID;
// [NTBES] Special Vertex ID of "[BES]-auxiliary “be”"
extern M1Env::SpecialItemID NTBES_SIID;
// [NTHVS] Special Vertex ID of "[HVS]-forms of “have”"
extern M1Env::SpecialItemID NTHVS_SIID;
// [NT_SP] Special Vertex ID of "[_SP]-whitespace"
extern M1Env::SpecialItemID NT_SP_SIID;


    class GraphInit{
        public:
            static void set_pseudo_constants();
            static void init_base();
            static void init_plato();
            static QList<M1Env::SpecialItemID> cm_gram_attr_list; 
    };
    
} // end namespace M1Env
