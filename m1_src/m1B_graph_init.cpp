
#include "m1B_graph_init.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"

// [FOLDR] Special Vertex ID (Vertex type) of folders (ordinary vertices with no special role)
M1Env::SpecialItemID M1Env::FOLDER_SIID = G_NONEX_SI_ID;
// [AUTO_] Special Vertex ID (Edge type) of AUTO edges
M1Env::SpecialItemID M1Env::AUTO_SIID = G_NONEX_SI_ID;
// [OWNS_] Special Vertex ID (Edge type) of ownership edges
M1Env::SpecialItemID M1Env::OWNS_SIID = G_NONEX_SI_ID;
// [BLNGS] Special Vertex ID (Edge type) of ownership edges (reciprocal of OWNS_ above))
M1Env::SpecialItemID M1Env::BLNGS_SIID = G_NONEX_SI_ID;
// [_ISA_] Special Vertex ID (Edge type) of type edges
M1Env::SpecialItemID M1Env::ISA_SIID = G_NONEX_SI_ID;
// [_ITO_] Special Vertex ID (Edge type) of type edges (reciprocal of _ISA_ above))
M1Env::SpecialItemID M1Env::ITO_SIID = G_NONEX_SI_ID;
// [WROTE] Special Vertex ID (Edge type) wrote / written by
M1Env::SpecialItemID M1Env::TEXT_WROTE_SIID = G_NONEX_SI_ID;
// [WRTBY] Special Vertex ID (Edge type) wrote / written by (reciprocal of WROTE above))
M1Env::SpecialItemID M1Env::TEXT_WRITTEN_BY_SIID = G_NONEX_SI_ID;
// [DTSDN] Special Vertex ID (Edge type) DS from / to
M1Env::SpecialItemID M1Env::DATA_SOURCE_FROM_SIID = G_NONEX_SI_ID;
// [DTSUP] Special Vertex ID (Edge type) DS from / to (reciprocal of DTSDN above))
M1Env::SpecialItemID M1Env::DATA_SOURCE_TO_SIID = G_NONEX_SI_ID;
// [OCCUR] Special Vertex ID (Edge type) of form occurrence edges
M1Env::SpecialItemID M1Env::OCCUR_SIID = G_NONEX_SI_ID;
// [OCCIN] Special Vertex ID (Edge type) of form to point of occurrence edges
M1Env::SpecialItemID M1Env::OCCURS_IN_SIID = G_NONEX_SI_ID;
// [WFORM] Special Vertex ID (Vertex type) of word form vertices
M1Env::SpecialItemID M1Env::WFORM_SIID = G_NONEX_SI_ID;
// [TXATL] Special Vertex ID (Simple edge type) alternate text title
M1Env::SpecialItemID M1Env::TEXT_ALT_TITLE_SIID = G_NONEX_SI_ID;
// [TXSBT] Special Vertex ID (Simple edge type) text subtitle
M1Env::SpecialItemID M1Env::TEXT_SUB_TITLE_SIID = G_NONEX_SI_ID;
// [CAPTL] Special Vertex ID (Simple edge type) of Occurrence field capitalization flag
M1Env::SpecialItemID M1Env::CAPTL_SIID = G_NONEX_SI_ID;
// [PCTLF] Special Vertex ID (Simple edge type) of Occurrence field left punctuation
M1Env::SpecialItemID M1Env::PCTLF_SIID = G_NONEX_SI_ID;
// [PCTRT] Special Vertex ID (Simple edge type) of Occurrence field right punctuation
M1Env::SpecialItemID M1Env::PCTRT_SIID = G_NONEX_SI_ID;
// [MKPLF] Special Vertex ID (Simple edge type) of Occurrence field markup left
M1Env::SpecialItemID M1Env::MKPLF_SIID = G_NONEX_SI_ID;
// [MKPRT] Special Vertex ID (Simple edge type) of Occurrence field markup right
M1Env::SpecialItemID M1Env::MKPRT_SIID = G_NONEX_SI_ID;
// [STPOS] Special Vertex ID (Simple edge type) of Occurrence field sentence position
M1Env::SpecialItemID M1Env::STPOS_SIID = G_NONEX_SI_ID;
// [HLCLR] Color of text highlight category
M1Env::SpecialItemID M1Env::HLCLR_SIID = G_NONEX_SI_ID;
// [TRSLT] Special Vertex ID (Simple Edge type) of Gita transliteration field
M1Env::SpecialItemID M1Env::TEXT_WORD_TRANSLIT_SIID = G_NONEX_SI_ID;
// [DCTRF] Special Vertex ID (Simple Edge type) of Gita dictionary ref. field
M1Env::SpecialItemID M1Env::TEXT_WORD_DICT_REF_SIID = G_NONEX_SI_ID;
// [TRSLT] Special Vertex ID (Simple Edge type) of Gita transliteration field
M1Env::SpecialItemID M1Env::TEXT_WFW_TRANSLIT_SIID = G_NONEX_SI_ID;
// [WFTRN] Special Vertex ID (Simple Edge type) WfW unit Translation field
M1Env::SpecialItemID M1Env::TEXT_WFW_TRANSLAT_SIID = G_NONEX_SI_ID;
// [WFPOS] Special Vertex ID (Simple Edge type) WfW form POS tag
M1Env::SpecialItemID M1Env::TEXT_WFW_POS_SIID = G_NONEX_SI_ID;
// [WFGRM] Special Vertex ID (Simple Edge type) WfW form Grammatical analysis
M1Env::SpecialItemID M1Env::TEXT_WFW_GRM_SIID = G_NONEX_SI_ID;
// [URLNK] Special Vertex ID (Vertex type) Url link
M1Env::SpecialItemID M1Env::TEXT_URL_LINK_SIID = G_NONEX_SI_ID;
// [HS2OB] Special Vertex ID (Edge type) begin section + reciprocal
M1Env::SpecialItemID M1Env::TW_SECTION_2_OCC_BEGIN_SIID = G_NONEX_SI_ID;
// [OC2HB] Special Vertex ID (Edge type) begin section + reciprocal (reciprocal of HS2OB above))
M1Env::SpecialItemID M1Env::TW_REV_SECTION_2_OCC_BEGIN_SIID = G_NONEX_SI_ID;
// [HS2OE] Special Vertex ID (Edge type) end section + reciprocal
M1Env::SpecialItemID M1Env::TW_SECTION_2_OCC_END_SIID = G_NONEX_SI_ID;
// [OC2HE] Special Vertex ID (Edge type) end section + reciprocal (reciprocal of HS2OE above))
M1Env::SpecialItemID M1Env::TW_REV_SECTION_2_OCC_END_SIID = G_NONEX_SI_ID;

M1Env::SpecialItemID M1Env::ROOT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::HOME_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TYPE_NODE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WORK_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::PERS_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ORG_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ROLE_FLDR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::MSG_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ME_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::EMAIL_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::WHTSP_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::DSCRD_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::GCHAT_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::SMS_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::GRAMMAR_ATTR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::LEMMA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NLENT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NLPOS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NLTAG_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::STEPHANUS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TXTVR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TXTNT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_CHUNK_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_BOOK_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_CHAPTER_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SENTENCE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::SLOKA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_HIGHLIGHT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_HIGHLIGHT_CAT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_HIGHLIGHT_FLDR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_HIGHLIGHT_CAT_FLDR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_HIGHLIGHT_CHUNK_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_FLDR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_HIGHLIGHT_QUOTE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_LINE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WORD_DREF_INRIA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_TRANSLATION_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_TRANSLIT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_BHASHYA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WFW_UNIT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WFW_FORM_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ABBR__SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ANMCY_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ASPCT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::CASE__SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::CTNJY_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::CLSVT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::DEFIN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::DEGRE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::DEIXI_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::DRXSF_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::EVDNT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::EPXTS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::FOREI_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::GENDE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::MOOD__SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NCNLS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTMYP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NUMBE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::PERSO_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::PLRTY_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::POLIT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::POSS__SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::PTRNY_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::REFLE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::II____SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TENSE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TYPO__SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::VFRBR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::VCRBN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::VTRBY_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::UNDCD_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::VOICE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPIIC_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPIIV_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPADJ_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPADP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPADV_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPAUX_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPCNJ_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPDET_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPIND_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPINT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPNON_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPNUM_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPPRT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPPRN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPPPN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPPCT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPSCJ_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPSYM_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPVRB_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPXXX_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGSE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGCM_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTLRB_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTRRB_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTOQM_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTCQM_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTCRM_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGCL_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTDOL_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTSRP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTAFX_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGCC_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGCD_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGDT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGEX_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGFW_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTHYP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGIN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGJJ_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTJJR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTJJS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGLS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGMD_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTNIL_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGNN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTNNP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTNPP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTNNS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTPDT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTPOS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTPRP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTPR$_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGRB_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTRBR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTRBS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGRP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGTO_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGUH_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGVB_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTVBD_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTVBG_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTVBN_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTVBP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTVBZ_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTWDT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGWP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTWP$_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTWRB_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGSP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTADD_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTNFP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGGW_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTGXX_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTBES_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NTHVS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NT_SP_SIID = G_NONEX_SI_ID;
QList<M1Env::SpecialItemID> M1Env::GraphInit::cm_gram_attr_list;

void M1Env::GraphInit::set_pseudo_constants(){
    M1Env::FOLDER_SIID = M1Store::StorageStatic::getSpecialID("FOLDR");
    M1Env::AUTO_SIID = M1Store::StorageStatic::getSpecialID("AUTO_");
    M1Env::OWNS_SIID = M1Store::StorageStatic::getSpecialID("OWNS_");
    M1Env::BLNGS_SIID = M1Store::StorageStatic::getSpecialID("BLNGS");
    M1Env::ISA_SIID = M1Store::StorageStatic::getSpecialID("_ISA_");
    M1Env::ITO_SIID = M1Store::StorageStatic::getSpecialID("_ITO_");
    M1Env::TEXT_WROTE_SIID = M1Store::StorageStatic::getSpecialID("WROTE");
    M1Env::TEXT_WRITTEN_BY_SIID = M1Store::StorageStatic::getSpecialID("WRTBY");
    M1Env::DATA_SOURCE_FROM_SIID = M1Store::StorageStatic::getSpecialID("DTSDN");
    M1Env::DATA_SOURCE_TO_SIID = M1Store::StorageStatic::getSpecialID("DTSUP");
    M1Env::OCCUR_SIID = M1Store::StorageStatic::getSpecialID("OCCUR");
    M1Env::OCCURS_IN_SIID = M1Store::StorageStatic::getSpecialID("OCCIN");
    M1Env::WFORM_SIID = M1Store::StorageStatic::getSpecialID("WFORM");
    M1Env::TEXT_ALT_TITLE_SIID = M1Store::StorageStatic::getSpecialID("TXATL");
    M1Env::TEXT_SUB_TITLE_SIID = M1Store::StorageStatic::getSpecialID("TXSBT");
    M1Env::CAPTL_SIID = M1Store::StorageStatic::getSpecialID("CAPTL");
    M1Env::PCTLF_SIID = M1Store::StorageStatic::getSpecialID("PCTLF");
    M1Env::PCTRT_SIID = M1Store::StorageStatic::getSpecialID("PCTRT");
    M1Env::MKPLF_SIID = M1Store::StorageStatic::getSpecialID("MKPLF");
    M1Env::MKPRT_SIID = M1Store::StorageStatic::getSpecialID("MKPRT");
    M1Env::STPOS_SIID = M1Store::StorageStatic::getSpecialID("STPOS");
    M1Env::HLCLR_SIID = M1Store::StorageStatic::getSpecialID("HLCLR");
    M1Env::TEXT_WORD_TRANSLIT_SIID = M1Store::StorageStatic::getSpecialID("TRSLT");
    M1Env::TEXT_WORD_DICT_REF_SIID = M1Store::StorageStatic::getSpecialID("DCTRF");
    M1Env::TEXT_WFW_TRANSLIT_SIID = M1Store::StorageStatic::getSpecialID("TRSLT");
    M1Env::TEXT_WFW_TRANSLAT_SIID = M1Store::StorageStatic::getSpecialID("WFTRN");
    M1Env::TEXT_WFW_POS_SIID = M1Store::StorageStatic::getSpecialID("WFPOS");
    M1Env::TEXT_WFW_GRM_SIID = M1Store::StorageStatic::getSpecialID("WFGRM");
    M1Env::TEXT_URL_LINK_SIID = M1Store::StorageStatic::getSpecialID("URLNK");
    M1Env::TW_SECTION_2_OCC_BEGIN_SIID = M1Store::StorageStatic::getSpecialID("HS2OB");
    M1Env::TW_REV_SECTION_2_OCC_BEGIN_SIID = M1Store::StorageStatic::getSpecialID("OC2HB");
    M1Env::TW_SECTION_2_OCC_END_SIID = M1Store::StorageStatic::getSpecialID("HS2OE");
    M1Env::TW_REV_SECTION_2_OCC_END_SIID = M1Store::StorageStatic::getSpecialID("OC2HE");
    M1Env::ROOT_SIID = M1Store::StorageStatic::getSpecialID("ROOT_");
    M1Env::HOME_SIID = M1Store::StorageStatic::getSpecialID("HOME_");
    M1Env::TYPE_NODE_SIID = M1Store::StorageStatic::getSpecialID("TYPE_");
    M1Env::TEXT_WORK_SIID = M1Store::StorageStatic::getSpecialID("TXWKK");
    M1Env::PERS_TYPE_SIID = M1Store::StorageStatic::getSpecialID("PERSN");
    M1Env::ORG_TYPE_SIID = M1Store::StorageStatic::getSpecialID("ORGN_");
    M1Env::ROLE_FLDR_SIID = M1Store::StorageStatic::getSpecialID("RLFLD");
    M1Env::TEXT_SIID = M1Store::StorageStatic::getSpecialID("TEXT_");
    M1Env::MSG_TYPE_SIID = M1Store::StorageStatic::getSpecialID("_MSG_");
    M1Env::ME_SIID = M1Store::StorageStatic::getSpecialID("ME___");
    M1Env::EMAIL_TYPE_SIID = M1Store::StorageStatic::getSpecialID("EMAIL");
    M1Env::WHTSP_TYPE_SIID = M1Store::StorageStatic::getSpecialID("WHTSP");
    M1Env::DSCRD_TYPE_SIID = M1Store::StorageStatic::getSpecialID("DSCRD");
    M1Env::GCHAT_TYPE_SIID = M1Store::StorageStatic::getSpecialID("GCHAT");
    M1Env::SMS_TYPE_SIID = M1Store::StorageStatic::getSpecialID("SMS__");
    M1Env::GRAMMAR_ATTR_SIID = M1Store::StorageStatic::getSpecialID("GRATT");
    M1Env::LEMMA_SIID = M1Store::StorageStatic::getSpecialID("LEMMA");
    M1Env::NLENT_SIID = M1Store::StorageStatic::getSpecialID("NLENT");
    M1Env::NLPOS_SIID = M1Store::StorageStatic::getSpecialID("NLPOS");
    M1Env::NLTAG_SIID = M1Store::StorageStatic::getSpecialID("NLTAG");
    M1Env::STEPHANUS_SIID = M1Store::StorageStatic::getSpecialID("STPSC");
    M1Env::TXTVR_SIID = M1Store::StorageStatic::getSpecialID("TXTVR");
    M1Env::TXTNT_SIID = M1Store::StorageStatic::getSpecialID("TXTNT");
    M1Env::TEXT_CHUNK_SIID = M1Store::StorageStatic::getSpecialID("TXTCK");
    M1Env::TEXT_BOOK_SIID = M1Store::StorageStatic::getSpecialID("TXTBK");
    M1Env::TEXT_CHAPTER_SIID = M1Store::StorageStatic::getSpecialID("TXTCH");
    M1Env::TEXT_SENTENCE_SIID = M1Store::StorageStatic::getSpecialID("TXSNT");
    M1Env::SLOKA_SIID = M1Store::StorageStatic::getSpecialID("TXSLK");
    M1Env::TEXT_HIGHLIGHT_SIID = M1Store::StorageStatic::getSpecialID("TXHLT");
    M1Env::TEXT_HIGHLIGHT_CAT_SIID = M1Store::StorageStatic::getSpecialID("TXHLC");
    M1Env::TEXT_HIGHLIGHT_FLDR_SIID = M1Store::StorageStatic::getSpecialID("TXHLF");
    M1Env::TEXT_HIGHLIGHT_CAT_FLDR_SIID = M1Store::StorageStatic::getSpecialID("TXHCF");
    M1Env::TEXT_HIGHLIGHT_CHUNK_SIID = M1Store::StorageStatic::getSpecialID("TXHCK");
    M1Env::TEXT_SLOKA_FLDR_SIID = M1Store::StorageStatic::getSpecialID("SKFLD");
    M1Env::TEXT_HIGHLIGHT_QUOTE_SIID = M1Store::StorageStatic::getSpecialID("TXHQT");
    M1Env::TEXT_SLOKA_LINE_SIID = M1Store::StorageStatic::getSpecialID("SLKLN");
    M1Env::TEXT_WORD_DREF_INRIA_SIID = M1Store::StorageStatic::getSpecialID("INRIA");
    M1Env::TEXT_SLOKA_TRANSLATION_SIID = M1Store::StorageStatic::getSpecialID("SLTRN");
    M1Env::TEXT_SLOKA_TRANSLIT_SIID = M1Store::StorageStatic::getSpecialID("SLTRL");
    M1Env::TEXT_SLOKA_BHASHYA_SIID = M1Store::StorageStatic::getSpecialID("SLBHS");
    M1Env::TEXT_WFW_UNIT_SIID = M1Store::StorageStatic::getSpecialID("TRLUN");
    M1Env::TEXT_WFW_FORM_SIID = M1Store::StorageStatic::getSpecialID("WFFRM");
    M1Env::ABBR__SIID = M1Store::StorageStatic::getSpecialID("ABBR_");
    M1Env::ANMCY_SIID = M1Store::StorageStatic::getSpecialID("ANMCY");
    M1Env::ASPCT_SIID = M1Store::StorageStatic::getSpecialID("ASPCT");
    M1Env::CASE__SIID = M1Store::StorageStatic::getSpecialID("CASE_");
    M1Env::CTNJY_SIID = M1Store::StorageStatic::getSpecialID("CTNJY");
    M1Env::CLSVT_SIID = M1Store::StorageStatic::getSpecialID("CLSVT");
    M1Env::DEFIN_SIID = M1Store::StorageStatic::getSpecialID("DEFIN");
    M1Env::DEGRE_SIID = M1Store::StorageStatic::getSpecialID("DEGRE");
    M1Env::DEIXI_SIID = M1Store::StorageStatic::getSpecialID("DEIXI");
    M1Env::DRXSF_SIID = M1Store::StorageStatic::getSpecialID("DRXSF");
    M1Env::EVDNT_SIID = M1Store::StorageStatic::getSpecialID("EVDNT");
    M1Env::EPXTS_SIID = M1Store::StorageStatic::getSpecialID("EPXTS");
    M1Env::FOREI_SIID = M1Store::StorageStatic::getSpecialID("FOREI");
    M1Env::GENDE_SIID = M1Store::StorageStatic::getSpecialID("GENDE");
    M1Env::MOOD__SIID = M1Store::StorageStatic::getSpecialID("MOOD_");
    M1Env::NCNLS_SIID = M1Store::StorageStatic::getSpecialID("NCNLS");
    M1Env::NTMYP_SIID = M1Store::StorageStatic::getSpecialID("NTMYP");
    M1Env::NUMBE_SIID = M1Store::StorageStatic::getSpecialID("NUMBE");
    M1Env::PERSO_SIID = M1Store::StorageStatic::getSpecialID("PERSO");
    M1Env::PLRTY_SIID = M1Store::StorageStatic::getSpecialID("PLRTY");
    M1Env::POLIT_SIID = M1Store::StorageStatic::getSpecialID("POLIT");
    M1Env::POSS__SIID = M1Store::StorageStatic::getSpecialID("POSS_");
    M1Env::PTRNY_SIID = M1Store::StorageStatic::getSpecialID("PTRNY");
    M1Env::REFLE_SIID = M1Store::StorageStatic::getSpecialID("REFLE");
    M1Env::II____SIID = M1Store::StorageStatic::getSpecialID("II___");
    M1Env::TENSE_SIID = M1Store::StorageStatic::getSpecialID("TENSE");
    M1Env::TYPO__SIID = M1Store::StorageStatic::getSpecialID("TYPO_");
    M1Env::VFRBR_SIID = M1Store::StorageStatic::getSpecialID("VFRBR");
    M1Env::VCRBN_SIID = M1Store::StorageStatic::getSpecialID("VCRBN");
    M1Env::VTRBY_SIID = M1Store::StorageStatic::getSpecialID("VTRBY");
    M1Env::UNDCD_SIID = M1Store::StorageStatic::getSpecialID("UNDCD");
    M1Env::VOICE_SIID = M1Store::StorageStatic::getSpecialID("VOICE");
    M1Env::NPIIC_SIID = M1Store::StorageStatic::getSpecialID("NPIIC");
    M1Env::NPIIV_SIID = M1Store::StorageStatic::getSpecialID("NPIIV");
    M1Env::NPADJ_SIID = M1Store::StorageStatic::getSpecialID("NPADJ");
    M1Env::NPADP_SIID = M1Store::StorageStatic::getSpecialID("NPADP");
    M1Env::NPADV_SIID = M1Store::StorageStatic::getSpecialID("NPADV");
    M1Env::NPAUX_SIID = M1Store::StorageStatic::getSpecialID("NPAUX");
    M1Env::NPCNJ_SIID = M1Store::StorageStatic::getSpecialID("NPCNJ");
    M1Env::NPDET_SIID = M1Store::StorageStatic::getSpecialID("NPDET");
    M1Env::NPIND_SIID = M1Store::StorageStatic::getSpecialID("NPIND");
    M1Env::NPINT_SIID = M1Store::StorageStatic::getSpecialID("NPINT");
    M1Env::NPNON_SIID = M1Store::StorageStatic::getSpecialID("NPNON");
    M1Env::NPNUM_SIID = M1Store::StorageStatic::getSpecialID("NPNUM");
    M1Env::NPPRT_SIID = M1Store::StorageStatic::getSpecialID("NPPRT");
    M1Env::NPPRN_SIID = M1Store::StorageStatic::getSpecialID("NPPRN");
    M1Env::NPPPN_SIID = M1Store::StorageStatic::getSpecialID("NPPPN");
    M1Env::NPPCT_SIID = M1Store::StorageStatic::getSpecialID("NPPCT");
    M1Env::NPSCJ_SIID = M1Store::StorageStatic::getSpecialID("NPSCJ");
    M1Env::NPSYM_SIID = M1Store::StorageStatic::getSpecialID("NPSYM");
    M1Env::NPVRB_SIID = M1Store::StorageStatic::getSpecialID("NPVRB");
    M1Env::NPXXX_SIID = M1Store::StorageStatic::getSpecialID("NPXXX");
    M1Env::NTGSE_SIID = M1Store::StorageStatic::getSpecialID("NTGSE");
    M1Env::NTGCM_SIID = M1Store::StorageStatic::getSpecialID("NTGCM");
    M1Env::NTLRB_SIID = M1Store::StorageStatic::getSpecialID("NTLRB");
    M1Env::NTRRB_SIID = M1Store::StorageStatic::getSpecialID("NTRRB");
    M1Env::NTOQM_SIID = M1Store::StorageStatic::getSpecialID("NTOQM");
    M1Env::NTCQM_SIID = M1Store::StorageStatic::getSpecialID("NTCQM");
    M1Env::NTCRM_SIID = M1Store::StorageStatic::getSpecialID("NTCRM");
    M1Env::NTGCL_SIID = M1Store::StorageStatic::getSpecialID("NTGCL");
    M1Env::NTDOL_SIID = M1Store::StorageStatic::getSpecialID("NTDOL");
    M1Env::NTSRP_SIID = M1Store::StorageStatic::getSpecialID("NTSRP");
    M1Env::NTAFX_SIID = M1Store::StorageStatic::getSpecialID("NTAFX");
    M1Env::NTGCC_SIID = M1Store::StorageStatic::getSpecialID("NTGCC");
    M1Env::NTGCD_SIID = M1Store::StorageStatic::getSpecialID("NTGCD");
    M1Env::NTGDT_SIID = M1Store::StorageStatic::getSpecialID("NTGDT");
    M1Env::NTGEX_SIID = M1Store::StorageStatic::getSpecialID("NTGEX");
    M1Env::NTGFW_SIID = M1Store::StorageStatic::getSpecialID("NTGFW");
    M1Env::NTHYP_SIID = M1Store::StorageStatic::getSpecialID("NTHYP");
    M1Env::NTGIN_SIID = M1Store::StorageStatic::getSpecialID("NTGIN");
    M1Env::NTGJJ_SIID = M1Store::StorageStatic::getSpecialID("NTGJJ");
    M1Env::NTJJR_SIID = M1Store::StorageStatic::getSpecialID("NTJJR");
    M1Env::NTJJS_SIID = M1Store::StorageStatic::getSpecialID("NTJJS");
    M1Env::NTGLS_SIID = M1Store::StorageStatic::getSpecialID("NTGLS");
    M1Env::NTGMD_SIID = M1Store::StorageStatic::getSpecialID("NTGMD");
    M1Env::NTNIL_SIID = M1Store::StorageStatic::getSpecialID("NTNIL");
    M1Env::NTGNN_SIID = M1Store::StorageStatic::getSpecialID("NTGNN");
    M1Env::NTNNP_SIID = M1Store::StorageStatic::getSpecialID("NTNNP");
    M1Env::NTNPP_SIID = M1Store::StorageStatic::getSpecialID("NTNPP");
    M1Env::NTNNS_SIID = M1Store::StorageStatic::getSpecialID("NTNNS");
    M1Env::NTPDT_SIID = M1Store::StorageStatic::getSpecialID("NTPDT");
    M1Env::NTPOS_SIID = M1Store::StorageStatic::getSpecialID("NTPOS");
    M1Env::NTPRP_SIID = M1Store::StorageStatic::getSpecialID("NTPRP");
    M1Env::NTPR$_SIID = M1Store::StorageStatic::getSpecialID("NTPR$");
    M1Env::NTGRB_SIID = M1Store::StorageStatic::getSpecialID("NTGRB");
    M1Env::NTRBR_SIID = M1Store::StorageStatic::getSpecialID("NTRBR");
    M1Env::NTRBS_SIID = M1Store::StorageStatic::getSpecialID("NTRBS");
    M1Env::NTGRP_SIID = M1Store::StorageStatic::getSpecialID("NTGRP");
    M1Env::NTGTO_SIID = M1Store::StorageStatic::getSpecialID("NTGTO");
    M1Env::NTGUH_SIID = M1Store::StorageStatic::getSpecialID("NTGUH");
    M1Env::NTGVB_SIID = M1Store::StorageStatic::getSpecialID("NTGVB");
    M1Env::NTVBD_SIID = M1Store::StorageStatic::getSpecialID("NTVBD");
    M1Env::NTVBG_SIID = M1Store::StorageStatic::getSpecialID("NTVBG");
    M1Env::NTVBN_SIID = M1Store::StorageStatic::getSpecialID("NTVBN");
    M1Env::NTVBP_SIID = M1Store::StorageStatic::getSpecialID("NTVBP");
    M1Env::NTVBZ_SIID = M1Store::StorageStatic::getSpecialID("NTVBZ");
    M1Env::NTWDT_SIID = M1Store::StorageStatic::getSpecialID("NTWDT");
    M1Env::NTGWP_SIID = M1Store::StorageStatic::getSpecialID("NTGWP");
    M1Env::NTWP$_SIID = M1Store::StorageStatic::getSpecialID("NTWP$");
    M1Env::NTWRB_SIID = M1Store::StorageStatic::getSpecialID("NTWRB");
    M1Env::NTGSP_SIID = M1Store::StorageStatic::getSpecialID("NTGSP");
    M1Env::NTADD_SIID = M1Store::StorageStatic::getSpecialID("NTADD");
    M1Env::NTNFP_SIID = M1Store::StorageStatic::getSpecialID("NTNFP");
    M1Env::NTGGW_SIID = M1Store::StorageStatic::getSpecialID("NTGGW");
    M1Env::NTGXX_SIID = M1Store::StorageStatic::getSpecialID("NTGXX");
    M1Env::NTBES_SIID = M1Store::StorageStatic::getSpecialID("NTBES");
    M1Env::NTHVS_SIID = M1Store::StorageStatic::getSpecialID("NTHVS");
    M1Env::NT_SP_SIID = M1Store::StorageStatic::getSpecialID("NT_SP");
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::ABBR__SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::ANMCY_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::ASPCT_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::CASE__SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::CTNJY_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::CLSVT_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::DEFIN_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::DEGRE_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::DEIXI_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::DRXSF_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::EVDNT_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::EPXTS_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::FOREI_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::GENDE_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::MOOD__SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::NCNLS_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::NTMYP_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::NUMBE_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::PERSO_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::PLRTY_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::POLIT_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::POSS__SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::PTRNY_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::REFLE_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::II____SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::TENSE_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::TYPO__SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::VFRBR_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::VCRBN_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::VTRBY_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::UNDCD_SIID);
    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::VOICE_SIID);
}

void M1Env::GraphInit::dbg_dump_pseudo_constants(){
    qCDebug(g_cat_silence) << QString("================ Constants dump ========");

    qCDebug(g_cat_silence) << QString("0x%1 M1Env::FOLDER_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("FOLDR"), 4, 16, QChar('0')).arg("FOLDR");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::AUTO_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("AUTO_"), 4, 16, QChar('0')).arg("AUTO_");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::OWNS_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("OWNS_"), 4, 16, QChar('0')).arg("OWNS_");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::ISA_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("_ISA_"), 4, 16, QChar('0')).arg("_ISA_");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_WROTE_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("WROTE"), 4, 16, QChar('0')).arg("WROTE");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::DATA_SOURCE_FROM_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("DTSDN"), 4, 16, QChar('0')).arg("DTSDN");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::OCCUR_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("OCCUR"), 4, 16, QChar('0')).arg("OCCUR");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::OCCURS_IN_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("OCCIN"), 4, 16, QChar('0')).arg("OCCIN");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::WFORM_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("WFORM"), 4, 16, QChar('0')).arg("WFORM");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_ALT_TITLE_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("TXATL"), 4, 16, QChar('0')).arg("TXATL");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_SUB_TITLE_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("TXSBT"), 4, 16, QChar('0')).arg("TXSBT");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::CAPTL_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("CAPTL"), 4, 16, QChar('0')).arg("CAPTL");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::PCTLF_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("PCTLF"), 4, 16, QChar('0')).arg("PCTLF");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::PCTRT_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("PCTRT"), 4, 16, QChar('0')).arg("PCTRT");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::MKPLF_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("MKPLF"), 4, 16, QChar('0')).arg("MKPLF");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::MKPRT_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("MKPRT"), 4, 16, QChar('0')).arg("MKPRT");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::STPOS_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("STPOS"), 4, 16, QChar('0')).arg("STPOS");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::HLCLR_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("HLCLR"), 4, 16, QChar('0')).arg("HLCLR");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_WORD_TRANSLIT_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("TRSLT"), 4, 16, QChar('0')).arg("TRSLT");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_WORD_DICT_REF_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("DCTRF"), 4, 16, QChar('0')).arg("DCTRF");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_WFW_TRANSLIT_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("TRSLT"), 4, 16, QChar('0')).arg("TRSLT");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_WFW_TRANSLAT_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("WFTRN"), 4, 16, QChar('0')).arg("WFTRN");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_WFW_POS_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("WFPOS"), 4, 16, QChar('0')).arg("WFPOS");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_WFW_GRM_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("WFGRM"), 4, 16, QChar('0')).arg("WFGRM");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TEXT_URL_LINK_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("URLNK"), 4, 16, QChar('0')).arg("URLNK");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TW_SECTION_2_OCC_BEGIN_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("HS2OB"), 4, 16, QChar('0')).arg("HS2OB");
    qCDebug(g_cat_silence) << QString("0x%1 M1Env::TW_SECTION_2_OCC_END_SIID <-- %2").arg(M1Store::StorageStatic::getSpecialID("HS2OE"), 4, 16, QChar('0')).arg("HS2OE");
}

void M1Env::GraphInit::init_base(){
    // Special Vertex ID (Vertex type) of folders (ordinary vertices with no special role)
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE | SI_IS_SELECTABLE, "FOLDR", M1Env::FOLDER_ICON_PATH);
    // Special Vertex ID (Edge type) of AUTO edges
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE | SI_EDGE_TYPE, "AUTO_", M1Env::AUTO_ICON_PATH);
    // Special Vertex ID (Edge type) of ownership edges
    M1Store::StorageStatic::getNewSpecialWithReciprocal(SI_IS_TYPE | SI_EDGE_TYPE | SI_IS_SELECTABLE,
        "OWNS_", M1Env::OWNS_ICON_PATH,
        "BLNGS", M1Env::BLNGS_ICON_PATH,
        SI_IS_TYPE | SI_EDGE_TYPE | SI_INSERT_AT_TOP | SI_IS_SELECTABLE);
    // Special Vertex ID (Edge type) of type edges
    M1Store::StorageStatic::getNewSpecialWithReciprocal(SI_IS_TYPE | SI_EDGE_TYPE | SI_IS_SPECIAL_EDGE | SI_IS_SELECTABLE,
        "_ISA_", M1Env::ISA_ICON_PATH,
        "_ITO_", M1Env::ITO_ICON_PATH,
        SI_IS_TYPE | SI_EDGE_TYPE | SI_IS_SELECTABLE);
    // Special Vertex ID (Edge type) wrote / written by
    M1Store::StorageStatic::getNewSpecialWithReciprocal(SI_IS_TYPE | SI_EDGE_TYPE,
        "WROTE", M1Env::TEXT_WROTE_ICON_PATH,
        "WRTBY", M1Env::TEXT_WRITTEN_BY_ICON_PATH,
        SI_IS_TYPE | SI_EDGE_TYPE);
    // Special Vertex ID (Edge type) DS from / to
    M1Store::StorageStatic::getNewSpecialWithReciprocal(SI_IS_TYPE | SI_EDGE_TYPE,
        "DTSDN", M1Env::DATA_SOURCE_ICON_PATH,
        "DTSUP", M1Env::DATA_SOURCE_ICON_PATH,
        SI_IS_TYPE | SI_EDGE_TYPE);
    // Special Vertex ID (Edge type) of form occurrence edges
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE | SI_EDGE_TYPE | SI_IS_SPECIAL_EDGE, "OCCUR", M1Env::OCCURRENCE_ICON_PATH);
    // Special Vertex ID (Edge type) of form to point of occurrence edges
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE | SI_EDGE_TYPE, "OCCIN", M1Env::OCCURRENCE_ICON_PATH);
    // Special Vertex ID (Vertex type) of word form vertices
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE | SI_IS_ICON_TYPE, "WFORM", M1Env::WFORM_ICON_PATH);
    // Special Vertex ID (Simple edge type) alternate text title
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "TXATL", nullptr);
    // Special Vertex ID (Simple edge type) text subtitle
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "TXSBT", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field capitalization flag
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "CAPTL", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field left punctuation
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "PCTLF", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field right punctuation
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "PCTRT", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field markup left
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "MKPLF", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field markup right
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "MKPRT", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field sentence position
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "STPOS", nullptr);
    // Color of text highlight category
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "HLCLR", nullptr);
    // Special Vertex ID (Simple Edge type) of Gita transliteration field
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "TRSLT", nullptr);
    // Special Vertex ID (Simple Edge type) of Gita dictionary ref. field
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "DCTRF", nullptr);
    // Special Vertex ID (Simple Edge type) of Gita transliteration field
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "TRSLT", nullptr);
    // Special Vertex ID (Simple Edge type) WfW unit Translation field
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "WFTRN", nullptr);
    // Special Vertex ID (Simple Edge type) WfW form POS tag
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "WFPOS", nullptr);
    // Special Vertex ID (Simple Edge type) WfW form Grammatical analysis
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "WFGRM", nullptr);
    // Special Vertex ID (Vertex type) Url link
    M1Store::StorageStatic::getNewSpecialNoItem(SI_IS_TYPE, "URLNK", M1Env::TEXT_URL_LINK_ICON_PATH);
    // Special Vertex ID (Edge type) begin section + reciprocal
    M1Store::StorageStatic::getNewSpecialWithReciprocal(SI_IS_TYPE,
        "HS2OB", M1Env::TW_SECTION_2_OCC_BEGIN_ICON_PATH,
        "OC2HB", M1Env::TW_SECTION_2_OCC_BEGIN_ICON_PATH,
        SI_IS_TYPE);
    // Special Vertex ID (Edge type) end section + reciprocal
    M1Store::StorageStatic::getNewSpecialWithReciprocal(SI_IS_TYPE,
        "HS2OE", M1Env::TW_SECTION_2_OCC_END_ICON_PATH,
        "OC2HE", M1Env::TW_SECTION_2_OCC_END_ICON_PATH,
        SI_IS_TYPE);

    // creation of "Global graph root"
    qCDebug(g_cat_silence) << QString("Creating <[ROOT_]-Global graph root> item");
    M1Store::Item_lv2* l_root = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Global graph root",
        // Special Item flag
        0,
        // mnemonic
        "ROOT_",
        // icon path
        nullptr);
    l_root->setType("FOLDR");

    // creation of "Home"
    qCDebug(g_cat_silence) << QString("Creating <[HOME_]-Home> item");
    M1Store::Item_lv2* l_home = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Home",
        // Special Item flag
        0,
        // mnemonic
        "HOME_",
        // icon path
        nullptr);
    l_home->setType("FOLDR");
    l_home->linkTo(l_root, "BLNGS", nullptr, true);

    // creation of "Root of all types"
    qCDebug(g_cat_silence) << QString("Creating <[TYPE_]-Root of all types> item");
    M1Store::Item_lv2* l_type = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Root of all types",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TYPE_",
        // icon path
        M1Env::TYPE_NODE_ICON_PATH);
    l_type->setType("TYPE_");
    l_type->linkTo(l_root, "BLNGS", nullptr, true);

    // creation of "Texts Work Area"
    qCDebug(g_cat_silence) << QString("Creating <[TXWKK]-Texts Work Area> item");
    M1Store::Item_lv2* l_txwkk = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Texts Work Area",
        // Special Item flag
        0,
        // mnemonic
        "TXWKK",
        // icon path
        nullptr);
    l_txwkk->setType("FOLDR");
    l_txwkk->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "Person (type)"
    qCDebug(g_cat_silence) << QString("Creating <[PERSN]-Person (type)> item");
    M1Store::Item_lv2* l_persn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Person (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE | M1Env::SI_IS_SELECTABLE,
        // mnemonic
        "PERSN",
        // icon path
        M1Env::PERSON_ICON_PATH);
    l_persn->setType("TYPE_");
    l_persn->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Organization (type)"
    qCDebug(g_cat_silence) << QString("Creating <[ORGN_]-Organization (type)> item");
    M1Store::Item_lv2* l_orgn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Organization (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE | M1Env::SI_IS_SELECTABLE,
        // mnemonic
        "ORGN_",
        // icon path
        M1Env::ORGANIZATION_ICON_PATH);
    l_orgn->setType("TYPE_");
    l_orgn->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Role folder"
    qCDebug(g_cat_silence) << QString("Creating <[RLFLD]-Role folder> item");
    M1Store::Item_lv2* l_rlfld = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Role folder",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RLFLD",
        // icon path
        M1Env::ROLE_FOLDER_ICON_PATH);
    l_rlfld->setType("TYPE_");

    // creation of "Texts Root (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TEXT_]-Texts Root (type)> item");
    M1Store::Item_lv2* l_text = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Texts Root (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE | M1Env::SI_IS_SELECTABLE,
        // mnemonic
        "TEXT_",
        // icon path
        M1Env::TEXT_ICON_PATH);
    l_text->setType("TYPE_");
    l_text->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "Inboxes / Message Root Type"
    qCDebug(g_cat_silence) << QString("Creating <[_MSG_]-Inboxes / Message Root Type> item");
    M1Store::Item_lv2* l_msg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Inboxes / Message Root Type",
        // Special Item flag
        M1Env::SI_IS_TYPE,
        // mnemonic
        "_MSG_",
        // icon path
        nullptr);
    l_msg->setType("TYPE_");
    l_msg->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "Me"
    qCDebug(g_cat_silence) << QString("Creating <[ME___]-Me> item");
    M1Store::Item_lv2* l_me = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Me",
        // Special Item flag
        0,
        // mnemonic
        "ME___",
        // icon path
        nullptr);
    l_me->setType("PERSN");
    l_me->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "Email Inbox"
    qCDebug(g_cat_silence) << QString("Creating <[EMAIL]-Email Inbox> item");
    M1Store::Item_lv2* l_email = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Email Inbox",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EMAIL",
        // icon path
        nullptr);
    l_email->setType("TYPE_");
    l_email->linkTo(l_msg, "BLNGS", nullptr, true);

    // creation of "Whatsapp Inbox"
    qCDebug(g_cat_silence) << QString("Creating <[WHTSP]-Whatsapp Inbox> item");
    M1Store::Item_lv2* l_whtsp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Whatsapp Inbox",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "WHTSP",
        // icon path
        nullptr);
    l_whtsp->setType("TYPE_");
    l_whtsp->linkTo(l_msg, "BLNGS", nullptr, true);

    // creation of "Discord Inbox"
    qCDebug(g_cat_silence) << QString("Creating <[DSCRD]-Discord Inbox> item");
    M1Store::Item_lv2* l_dscrd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Discord Inbox",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DSCRD",
        // icon path
        nullptr);
    l_dscrd->setType("TYPE_");
    l_dscrd->linkTo(l_msg, "BLNGS", nullptr, true);

    // creation of "GChat Inbox"
    qCDebug(g_cat_silence) << QString("Creating <[GCHAT]-GChat Inbox> item");
    M1Store::Item_lv2* l_gchat = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "GChat Inbox",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GCHAT",
        // icon path
        nullptr);
    l_gchat->setType("TYPE_");
    l_gchat->linkTo(l_msg, "BLNGS", nullptr, true);

    // creation of "SMS Inbox"
    qCDebug(g_cat_silence) << QString("Creating <[SMS__]-SMS Inbox> item");
    M1Store::Item_lv2* l_sms = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "SMS Inbox",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SMS__",
        // icon path
        nullptr);
    l_sms->setType("TYPE_");
    l_sms->linkTo(l_msg, "BLNGS", nullptr, true);

    // creation of "Grammar Attributes (type)"
    qCDebug(g_cat_silence) << QString("Creating <[GRATT]-Grammar Attributes (type)> item");
    M1Store::Item_lv2* l_gratt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Grammar Attributes (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GRATT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gratt->setType("TYPE_");
    l_gratt->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Lemma (type)"
    qCDebug(g_cat_silence) << QString("Creating <[LEMMA]-Lemma (type)> item");
    M1Store::Item_lv2* l_lemma = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Lemma (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE | M1Env::SI_IS_ICON_TYPE,
        // mnemonic
        "LEMMA",
        // icon path
        M1Env::LEMMA_ICON_PATH);
    l_lemma->setType("TYPE_");
    l_lemma->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "NLP Entity (type)"
    qCDebug(g_cat_silence) << QString("Creating <[NLENT]-NLP Entity (type)> item");
    M1Store::Item_lv2* l_nlent = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "NLP Entity (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NLENT",
        // icon path
        M1Env::ENTITY_ICON_PATH);
    l_nlent->setType("TYPE_");
    l_nlent->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "NLP Pos code (type)"
    qCDebug(g_cat_silence) << QString("Creating <[NLPOS]-NLP Pos code (type)> item");
    M1Store::Item_lv2* l_nlpos = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "NLP Pos code (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NLPOS",
        // icon path
        M1Env::POS_ICON_PATH);
    l_nlpos->setType("TYPE_");
    l_nlpos->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "NLP Tag code (type)"
    qCDebug(g_cat_silence) << QString("Creating <[NLTAG]-NLP Tag code (type)> item");
    M1Store::Item_lv2* l_nltag = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "NLP Tag code (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NLTAG",
        // icon path
        M1Env::TAG_ICON_PATH);
    l_nltag->setType("TYPE_");
    l_nltag->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Stephanus Section (type)"
    qCDebug(g_cat_silence) << QString("Creating <[STPSC]-Stephanus Section (type)> item");
    M1Store::Item_lv2* l_stpsc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Stephanus Section (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "STPSC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_stpsc->setType("TYPE_");
    l_stpsc->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Text Version (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXTVR]-Text Version (type)> item");
    M1Store::Item_lv2* l_txtvr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Text Version (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXTVR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_txtvr->setType("TYPE_");
    l_txtvr->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Notes (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXTNT]-Notes (type)> item");
    M1Store::Item_lv2* l_txtnt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Notes (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXTNT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_txtnt->setType("TYPE_");
    l_txtnt->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Text Chunk (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXTCK]-Text Chunk (type)> item");
    M1Store::Item_lv2* l_txtck = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Text Chunk (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE | M1Env::SI_IS_SELECTABLE,
        // mnemonic
        "TXTCK",
        // icon path
        M1Env::TEXT_CHUNK_ICON_PATH);
    l_txtck->setType("TYPE_");

    // creation of "Book (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXTBK]-Book (type)> item");
    M1Store::Item_lv2* l_txtbk = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Book (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXTBK",
        // icon path
        M1Env::BOOK_ICON_PATH);
    l_txtbk->setType("TYPE_");
    l_txtbk->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Chapter (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXTCH]-Chapter (type)> item");
    M1Store::Item_lv2* l_txtch = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Chapter (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXTCH",
        // icon path
        M1Env::CHAPTER_ICON_PATH);
    l_txtch->setType("TYPE_");
    l_txtch->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Sentence (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXSNT]-Sentence (type)> item");
    M1Store::Item_lv2* l_txsnt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sentence (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXSNT",
        // icon path
        M1Env::SENTENCE_ICON_PATH);
    l_txsnt->setType("TYPE_");

    // creation of "Sloka (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXSLK]-Sloka (type)> item");
    M1Store::Item_lv2* l_txslk = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sloka (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXSLK",
        // icon path
        M1Env::SLOKA_ICON_PATH);
    l_txslk->setType("TYPE_");
    l_txslk->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Highlight (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXHLT]-Highlight (type)> item");
    M1Store::Item_lv2* l_txhlt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Highlight (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXHLT",
        // icon path
        M1Env::HIGHLIGHTER_ICON_PATH);
    l_txhlt->setType("TYPE_");
    l_txhlt->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Highlight category (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXHLC]-Highlight category (type)> item");
    M1Store::Item_lv2* l_txhlc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Highlight category (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXHLC",
        // icon path
        nullptr);
    l_txhlc->setType("TYPE_");
    l_txhlc->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Highlight Folder (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXHLF]-Highlight Folder (type)> item");
    M1Store::Item_lv2* l_txhlf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Highlight Folder (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXHLF",
        // icon path
        nullptr);
    l_txhlf->setType("TYPE_");

    // creation of "Highlight Categories Folder"
    qCDebug(g_cat_silence) << QString("Creating <[TXHCF]-Highlight Categories Folder> item");
    M1Store::Item_lv2* l_txhcf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Highlight Categories Folder",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXHCF",
        // icon path
        nullptr);
    l_txhcf->setType("TYPE_");

    // creation of "Highlight Chunk"
    qCDebug(g_cat_silence) << QString("Creating <[TXHCK]-Highlight Chunk> item");
    M1Store::Item_lv2* l_txhck = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Highlight Chunk",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXHCK",
        // icon path
        M1Env::HL_CHUNK_ICON_PATH);
    l_txhck->setType("TYPE_");

    // creation of "Sloka folder"
    qCDebug(g_cat_silence) << QString("Creating <[SKFLD]-Sloka folder> item");
    M1Store::Item_lv2* l_skfld = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sloka folder",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SKFLD",
        // icon path
        M1Env::SLOKA_FOLDER_ICON_PATH);
    l_skfld->setType("TYPE_");

    // creation of "Highlight Quotation (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXHQT]-Highlight Quotation (type)> item");
    M1Store::Item_lv2* l_txhqt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Highlight Quotation (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE | M1Env::SI_EDGE_TYPE | M1Env::SI_IS_SELECTABLE,
        // mnemonic
        "TXHQT",
        // icon path
        M1Env::HL_QUOTE_ICON_PATH);
    l_txhqt->setType("TYPE_");

    // creation of "Sloka Lines (type)"
    qCDebug(g_cat_silence) << QString("Creating <[SLKLN]-Sloka Lines (type)> item");
    M1Store::Item_lv2* l_slkln = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sloka Lines (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SLKLN",
        // icon path
        M1Env::TEXT_SLOKA_LINE_ICON_PATH);
    l_slkln->setType("TYPE_");
    l_slkln->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "INRIA Dictionary Reference"
    qCDebug(g_cat_silence) << QString("Creating <[INRIA]-INRIA Dictionary Reference> item");
    M1Store::Item_lv2* l_inria = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "INRIA Dictionary Reference",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "INRIA",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_inria->setType("TYPE_");
    l_inria->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Sloka Translation (type)"
    qCDebug(g_cat_silence) << QString("Creating <[SLTRN]-Sloka Translation (type)> item");
    M1Store::Item_lv2* l_sltrn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sloka Translation (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SLTRN",
        // icon path
        M1Env::TEXT_SLOKA_TRANSLATION_ICON_PATH);
    l_sltrn->setType("TYPE_");
    l_sltrn->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Sloka Transliteration (type)"
    qCDebug(g_cat_silence) << QString("Creating <[SLTRL]-Sloka Transliteration (type)> item");
    M1Store::Item_lv2* l_sltrl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sloka Transliteration (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SLTRL",
        // icon path
        M1Env::TEXT_SLOKA_TRANSLIT_ICON_PATH);
    l_sltrl->setType("TYPE_");
    l_sltrl->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Sloka Bhashya (type)"
    qCDebug(g_cat_silence) << QString("Creating <[SLBHS]-Sloka Bhashya (type)> item");
    M1Store::Item_lv2* l_slbhs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sloka Bhashya (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SLBHS",
        // icon path
        M1Env::TEXT_SLOKA_BHASHYA_ICON_PATH);
    l_slbhs->setType("TYPE_");
    l_slbhs->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "WfW Translation Unit (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TRLUN]-WfW Translation Unit (type)> item");
    M1Store::Item_lv2* l_trlun = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "WfW Translation Unit (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TRLUN",
        // icon path
        M1Env::TEXT_WFW_UNIT_ICON_PATH);
    l_trlun->setType("TYPE_");
    l_trlun->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "WfW Form (type)"
    qCDebug(g_cat_silence) << QString("Creating <[WFFRM]-WfW Form (type)> item");
    M1Store::Item_lv2* l_wffrm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "WfW Form (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "WFFRM",
        // icon path
        M1Env::TEXT_WFW_FORM_ICON_PATH);
    l_wffrm->setType("TYPE_");
    l_wffrm->linkTo(l_txwkk, "BLNGS", nullptr, true);

    // creation of "Abbreviation"
    qCDebug(g_cat_silence) << QString("Creating <[ABBR_]-Abbreviation> item");
    M1Store::Item_lv2* l_abbr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Abbreviation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ABBR_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_abbr->setType("GRATT");

    // creation of "[Yes]-it is abbreviation"
    qCDebug(g_cat_silence) << QString("Creating <[ABYES]-[Yes]-it is abbreviation> item");
    M1Store::Item_lv2* l_abyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Yes]-it is abbreviation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ABYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_abyes->setType("TYPE_");
    l_abyes->setType("ABBR_");

    // creation of "Animacy"
    qCDebug(g_cat_silence) << QString("Creating <[ANMCY]-Animacy> item");
    M1Store::Item_lv2* l_anmcy = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Animacy",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANMCY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_anmcy->setType("GRATT");

    // creation of "[Anim]-animate"
    qCDebug(g_cat_silence) << QString("Creating <[ANANM]-[Anim]-animate> item");
    M1Store::Item_lv2* l_ananm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Anim]-animate",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANANM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ananm->setType("TYPE_");
    l_ananm->setType("ANMCY");

    // creation of "[Hum]-human"
    qCDebug(g_cat_silence) << QString("Creating <[ANHUM]-[Hum]-human> item");
    M1Store::Item_lv2* l_anhum = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Hum]-human",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANHUM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_anhum->setType("TYPE_");
    l_anhum->setType("ANMCY");

    // creation of "[Inan]-inanimate"
    qCDebug(g_cat_silence) << QString("Creating <[ANINN]-[Inan]-inanimate> item");
    M1Store::Item_lv2* l_aninn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Inan]-inanimate",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANINN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_aninn->setType("TYPE_");
    l_aninn->setType("ANMCY");

    // creation of "[Nhum]-non-human"
    qCDebug(g_cat_silence) << QString("Creating <[ANNHM]-[Nhum]-non-human> item");
    M1Store::Item_lv2* l_annhm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Nhum]-non-human",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANNHM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_annhm->setType("TYPE_");
    l_annhm->setType("ANMCY");

    // creation of "Aspect"
    qCDebug(g_cat_silence) << QString("Creating <[ASPCT]-Aspect> item");
    M1Store::Item_lv2* l_aspct = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Aspect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPCT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_aspct->setType("GRATT");

    // creation of "[Hab]-habitual aspect"
    qCDebug(g_cat_silence) << QString("Creating <[ASHAB]-[Hab]-habitual aspect> item");
    M1Store::Item_lv2* l_ashab = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Hab]-habitual aspect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASHAB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ashab->setType("TYPE_");
    l_ashab->setType("ASPCT");

    // creation of "[Imp]-imperfect aspect"
    qCDebug(g_cat_silence) << QString("Creating <[ASIMP]-[Imp]-imperfect aspect> item");
    M1Store::Item_lv2* l_asimp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Imp]-imperfect aspect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASIMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asimp->setType("TYPE_");
    l_asimp->setType("ASPCT");

    // creation of "[Iter]-iterative / frequentative aspect"
    qCDebug(g_cat_silence) << QString("Creating <[ASITR]-[Iter]-iterative / frequentative aspect> item");
    M1Store::Item_lv2* l_asitr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Iter]-iterative / frequentative aspect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASITR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asitr->setType("TYPE_");
    l_asitr->setType("ASPCT");

    // creation of "[Perf]-perfect aspect"
    qCDebug(g_cat_silence) << QString("Creating <[ASPRF]-[Perf]-perfect aspect> item");
    M1Store::Item_lv2* l_asprf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Perf]-perfect aspect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPRF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asprf->setType("TYPE_");
    l_asprf->setType("ASPCT");

    // creation of "[Prog]-progressive aspect"
    qCDebug(g_cat_silence) << QString("Creating <[ASPRG]-[Prog]-progressive aspect> item");
    M1Store::Item_lv2* l_asprg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Prog]-progressive aspect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPRG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asprg->setType("TYPE_");
    l_asprg->setType("ASPCT");

    // creation of "[Prosp]-prospective aspect"
    qCDebug(g_cat_silence) << QString("Creating <[ASPRS]-[Prosp]-prospective aspect> item");
    M1Store::Item_lv2* l_asprs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Prosp]-prospective aspect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPRS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asprs->setType("TYPE_");
    l_asprs->setType("ASPCT");

    // creation of "Case"
    qCDebug(g_cat_silence) << QString("Creating <[CASE_]-Case> item");
    M1Store::Item_lv2* l_case = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Case",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CASE_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_case->setType("GRATT");

    // creation of "[Abs]-absolutive"
    qCDebug(g_cat_silence) << QString("Creating <[CSABS]-[Abs]-absolutive> item");
    M1Store::Item_lv2* l_csabs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Abs]-absolutive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSABS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csabs->setType("TYPE_");
    l_csabs->setType("CASE_");

    // creation of "[Acc]-accusative / oblique"
    qCDebug(g_cat_silence) << QString("Creating <[CSACC]-[Acc]-accusative / oblique> item");
    M1Store::Item_lv2* l_csacc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Acc]-accusative / oblique",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSACC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csacc->setType("TYPE_");
    l_csacc->setType("CASE_");

    // creation of "[Erf]-ergative"
    qCDebug(g_cat_silence) << QString("Creating <[CSERG]-[Erf]-ergative> item");
    M1Store::Item_lv2* l_cserg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Erf]-ergative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSERG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cserg->setType("TYPE_");
    l_cserg->setType("CASE_");

    // creation of "[Nom]-nominative / direct"
    qCDebug(g_cat_silence) << QString("Creating <[CSNOM]-[Nom]-nominative / direct> item");
    M1Store::Item_lv2* l_csnom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Nom]-nominative / direct",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSNOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csnom->setType("TYPE_");
    l_csnom->setType("CASE_");

    // creation of "[Abe]-abessive / caritive / privative"
    qCDebug(g_cat_silence) << QString("Creating <[CSABE]-[Abe]-abessive / caritive / privative> item");
    M1Store::Item_lv2* l_csabe = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Abe]-abessive / caritive / privative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSABE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csabe->setType("TYPE_");
    l_csabe->setType("CASE_");

    // creation of "[Ben]-benefactive / destinative"
    qCDebug(g_cat_silence) << QString("Creating <[CSBEN]-[Ben]-benefactive / destinative> item");
    M1Store::Item_lv2* l_csben = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ben]-benefactive / destinative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSBEN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csben->setType("TYPE_");
    l_csben->setType("CASE_");

    // creation of "[Cau]-causative / motivative / purposive"
    qCDebug(g_cat_silence) << QString("Creating <[CSCAU]-[Cau]-causative / motivative / purposive> item");
    M1Store::Item_lv2* l_cscau = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cau]-causative / motivative / purposive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCAU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscau->setType("TYPE_");
    l_cscau->setType("CASE_");

    // creation of "[Cmp]-comparative"
    qCDebug(g_cat_silence) << QString("Creating <[CSCMP]-[Cmp]-comparative> item");
    M1Store::Item_lv2* l_cscmp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cmp]-comparative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscmp->setType("TYPE_");
    l_cscmp->setType("CASE_");

    // creation of "[Cns]-considerative"
    qCDebug(g_cat_silence) << QString("Creating <[CSCNS]-[Cns]-considerative> item");
    M1Store::Item_lv2* l_cscns = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cns]-considerative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCNS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscns->setType("TYPE_");
    l_cscns->setType("CASE_");

    // creation of "[Com]-comitative / associative"
    qCDebug(g_cat_silence) << QString("Creating <[CSCOM]-[Com]-comitative / associative> item");
    M1Store::Item_lv2* l_cscom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Com]-comitative / associative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscom->setType("TYPE_");
    l_cscom->setType("CASE_");

    // creation of "[Dat]-dative"
    qCDebug(g_cat_silence) << QString("Creating <[CSDAT]-[Dat]-dative> item");
    M1Store::Item_lv2* l_csdat = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Dat]-dative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSDAT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csdat->setType("TYPE_");
    l_csdat->setType("CASE_");

    // creation of "[Dis]-distributive"
    qCDebug(g_cat_silence) << QString("Creating <[CSDIS]-[Dis]-distributive> item");
    M1Store::Item_lv2* l_csdis = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Dis]-distributive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSDIS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csdis->setType("TYPE_");
    l_csdis->setType("CASE_");

    // creation of "[Equ]-equative"
    qCDebug(g_cat_silence) << QString("Creating <[CSEQU]-[Equ]-equative> item");
    M1Store::Item_lv2* l_csequ = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Equ]-equative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSEQU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csequ->setType("TYPE_");
    l_csequ->setType("CASE_");

    // creation of "[Gen]-genitive"
    qCDebug(g_cat_silence) << QString("Creating <[CSGEN]-[Gen]-genitive> item");
    M1Store::Item_lv2* l_csgen = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Gen]-genitive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSGEN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csgen->setType("TYPE_");
    l_csgen->setType("CASE_");

    // creation of "[Ins]-instrumental / instructive"
    qCDebug(g_cat_silence) << QString("Creating <[CSINS]-[Ins]-instrumental / instructive> item");
    M1Store::Item_lv2* l_csins = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ins]-instrumental / instructive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSINS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csins->setType("TYPE_");
    l_csins->setType("CASE_");

    // creation of "[Par]-partitive"
    qCDebug(g_cat_silence) << QString("Creating <[CSPAR]-[Par]-partitive> item");
    M1Store::Item_lv2* l_cspar = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Par]-partitive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSPAR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cspar->setType("TYPE_");
    l_cspar->setType("CASE_");

    // creation of "[Tem]-temporal"
    qCDebug(g_cat_silence) << QString("Creating <[CSTEM]-[Tem]-temporal> item");
    M1Store::Item_lv2* l_cstem = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Tem]-temporal",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSTEM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cstem->setType("TYPE_");
    l_cstem->setType("CASE_");

    // creation of "[Tra]-translative / factive"
    qCDebug(g_cat_silence) << QString("Creating <[CSTRA]-[Tra]-translative / factive> item");
    M1Store::Item_lv2* l_cstra = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Tra]-translative / factive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSTRA",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cstra->setType("TYPE_");
    l_cstra->setType("CASE_");

    // creation of "[Voc]-vocative"
    qCDebug(g_cat_silence) << QString("Creating <[CSVOC]-[Voc]-vocative> item");
    M1Store::Item_lv2* l_csvoc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Voc]-vocative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSVOC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csvoc->setType("TYPE_");
    l_csvoc->setType("CASE_");

    // creation of "[Abl]-ablative / adelative"
    qCDebug(g_cat_silence) << QString("Creating <[CSABL]-[Abl]-ablative / adelative> item");
    M1Store::Item_lv2* l_csabl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Abl]-ablative / adelative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSABL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csabl->setType("TYPE_");
    l_csabl->setType("CASE_");

    // creation of "[Add]-additive"
    qCDebug(g_cat_silence) << QString("Creating <[CSADD]-[Add]-additive> item");
    M1Store::Item_lv2* l_csadd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Add]-additive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSADD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csadd->setType("TYPE_");
    l_csadd->setType("CASE_");

    // creation of "[Ade]-adessive"
    qCDebug(g_cat_silence) << QString("Creating <[CSADE]-[Ade]-adessive> item");
    M1Store::Item_lv2* l_csade = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ade]-adessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSADE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csade->setType("TYPE_");
    l_csade->setType("CASE_");

    // creation of "[All]-allative / adlative"
    qCDebug(g_cat_silence) << QString("Creating <[CSALL]-[All]-allative / adlative> item");
    M1Store::Item_lv2* l_csall = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[All]-allative / adlative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSALL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csall->setType("TYPE_");
    l_csall->setType("CASE_");

    // creation of "[Del]-delative / superelative"
    qCDebug(g_cat_silence) << QString("Creating <[CSDEL]-[Del]-delative / superelative> item");
    M1Store::Item_lv2* l_csdel = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Del]-delative / superelative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSDEL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csdel->setType("TYPE_");
    l_csdel->setType("CASE_");

    // creation of "[Ela]-elative / inelative"
    qCDebug(g_cat_silence) << QString("Creating <[CSELA]-[Ela]-elative / inelative> item");
    M1Store::Item_lv2* l_csela = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ela]-elative / inelative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSELA",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csela->setType("TYPE_");
    l_csela->setType("CASE_");

    // creation of "[Ess]-essive / prolative"
    qCDebug(g_cat_silence) << QString("Creating <[CSESS]-[Ess]-essive / prolative> item");
    M1Store::Item_lv2* l_csess = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ess]-essive / prolative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSESS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csess->setType("TYPE_");
    l_csess->setType("CASE_");

    // creation of "[Ill]-illative / inlative"
    qCDebug(g_cat_silence) << QString("Creating <[CSILL]-[Ill]-illative / inlative> item");
    M1Store::Item_lv2* l_csill = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ill]-illative / inlative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSILL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csill->setType("TYPE_");
    l_csill->setType("CASE_");

    // creation of "[Ine]-inessive"
    qCDebug(g_cat_silence) << QString("Creating <[CSINE]-[Ine]-inessive> item");
    M1Store::Item_lv2* l_csine = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ine]-inessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSINE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csine->setType("TYPE_");
    l_csine->setType("CASE_");

    // creation of "[Lat]-lative / directional allative"
    qCDebug(g_cat_silence) << QString("Creating <[CSLAT]-[Lat]-lative / directional allative> item");
    M1Store::Item_lv2* l_cslat = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Lat]-lative / directional allative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSLAT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cslat->setType("TYPE_");
    l_cslat->setType("CASE_");

    // creation of "[Loc]-locative"
    qCDebug(g_cat_silence) << QString("Creating <[CSLOC]-[Loc]-locative> item");
    M1Store::Item_lv2* l_csloc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Loc]-locative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSLOC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csloc->setType("TYPE_");
    l_csloc->setType("CASE_");

    // creation of "[Per]-perlative"
    qCDebug(g_cat_silence) << QString("Creating <[CSPER]-[Per]-perlative> item");
    M1Store::Item_lv2* l_csper = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Per]-perlative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSPER",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csper->setType("TYPE_");
    l_csper->setType("CASE_");

    // creation of "[Sbe]-subelative"
    qCDebug(g_cat_silence) << QString("Creating <[CSSBE]-[Sbe]-subelative> item");
    M1Store::Item_lv2* l_cssbe = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sbe]-subelative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSBE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssbe->setType("TYPE_");
    l_cssbe->setType("CASE_");

    // creation of "[Sbl]-sublative"
    qCDebug(g_cat_silence) << QString("Creating <[CSSBL]-[Sbl]-sublative> item");
    M1Store::Item_lv2* l_cssbl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sbl]-sublative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSBL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssbl->setType("TYPE_");
    l_cssbl->setType("CASE_");

    // creation of "[Spl]-superlative"
    qCDebug(g_cat_silence) << QString("Creating <[CSSPL]-[Spl]-superlative> item");
    M1Store::Item_lv2* l_csspl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Spl]-superlative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSPL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csspl->setType("TYPE_");
    l_csspl->setType("CASE_");

    // creation of "[Sub]-subessive"
    qCDebug(g_cat_silence) << QString("Creating <[CSSUB]-[Sub]-subessive> item");
    M1Store::Item_lv2* l_cssub = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sub]-subessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSUB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssub->setType("TYPE_");
    l_cssub->setType("CASE_");

    // creation of "[Sup]-superessive"
    qCDebug(g_cat_silence) << QString("Creating <[CSSUP]-[Sup]-superessive> item");
    M1Store::Item_lv2* l_cssup = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sup]-superessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSUP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssup->setType("TYPE_");
    l_cssup->setType("CASE_");

    // creation of "[Ter]-terminative / terminal allative"
    qCDebug(g_cat_silence) << QString("Creating <[CSTER]-[Ter]-terminative / terminal allative> item");
    M1Store::Item_lv2* l_cster = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ter]-terminative / terminal allative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSTER",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cster->setType("TYPE_");
    l_cster->setType("CASE_");

    // creation of "Conjunction type"
    qCDebug(g_cat_silence) << QString("Creating <[CTNJY]-Conjunction type> item");
    M1Store::Item_lv2* l_ctnjy = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Conjunction type",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CTNJY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ctnjy->setType("GRATT");

    // creation of "[Cmp]-comparing conjunction"
    qCDebug(g_cat_silence) << QString("Creating <[CTCMP]-[Cmp]-comparing conjunction> item");
    M1Store::Item_lv2* l_ctcmp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cmp]-comparing conjunction",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CTCMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ctcmp->setType("TYPE_");
    l_ctcmp->setType("CTNJY");

    // creation of "[Oper]-mathematical operator"
    qCDebug(g_cat_silence) << QString("Creating <[CTOPR]-[Oper]-mathematical operator> item");
    M1Store::Item_lv2* l_ctopr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Oper]-mathematical operator",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CTOPR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ctopr->setType("TYPE_");
    l_ctopr->setType("CTNJY");

    // creation of "[Pred]-subordinating conjunction introducing a secondary predi"
    qCDebug(g_cat_silence) << QString("Creating <[CTPRD]-[Pred]-subordinating conjunction introducing a secondary predi> item");
    M1Store::Item_lv2* l_ctprd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pred]-subordinating conjunction introducing a secondary predi",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CTPRD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ctprd->setType("TYPE_");
    l_ctprd->setType("CTNJY");

    // creation of "Clusivity"
    qCDebug(g_cat_silence) << QString("Creating <[CLSVT]-Clusivity> item");
    M1Store::Item_lv2* l_clsvt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Clusivity",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CLSVT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_clsvt->setType("GRATT");

    // creation of "[Ex]-exclusive"
    qCDebug(g_cat_silence) << QString("Creating <[CLSEX]-[Ex]-exclusive> item");
    M1Store::Item_lv2* l_clsex = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ex]-exclusive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CLSEX",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_clsex->setType("TYPE_");
    l_clsex->setType("CLSVT");

    // creation of "[In]-inclusive"
    qCDebug(g_cat_silence) << QString("Creating <[CLSIN]-[In]-inclusive> item");
    M1Store::Item_lv2* l_clsin = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[In]-inclusive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CLSIN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_clsin->setType("TYPE_");
    l_clsin->setType("CLSVT");

    // creation of "Definiteness or state"
    qCDebug(g_cat_silence) << QString("Creating <[DEFIN]-Definiteness or state> item");
    M1Store::Item_lv2* l_defin = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Definiteness or state",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DEFIN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_defin->setType("GRATT");

    // creation of "[Com]-complex"
    qCDebug(g_cat_silence) << QString("Creating <[DFCOM]-[Com]-complex> item");
    M1Store::Item_lv2* l_dfcom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Com]-complex",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFCOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfcom->setType("TYPE_");
    l_dfcom->setType("DEFIN");

    // creation of "[Cons]-construct state / reduced definiteness"
    qCDebug(g_cat_silence) << QString("Creating <[DFCNS]-[Cons]-construct state / reduced definiteness> item");
    M1Store::Item_lv2* l_dfcns = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cons]-construct state / reduced definiteness",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFCNS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfcns->setType("TYPE_");
    l_dfcns->setType("DEFIN");

    // creation of "[Def]-definite"
    qCDebug(g_cat_silence) << QString("Creating <[DFDEF]-[Def]-definite> item");
    M1Store::Item_lv2* l_dfdef = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Def]-definite",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFDEF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfdef->setType("TYPE_");
    l_dfdef->setType("DEFIN");

    // creation of "[Ind]-indefinite"
    qCDebug(g_cat_silence) << QString("Creating <[DFIND]-[Ind]-indefinite> item");
    M1Store::Item_lv2* l_dfind = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ind]-indefinite",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFIND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfind->setType("TYPE_");
    l_dfind->setType("DEFIN");

    // creation of "[Spec]-specific indefinite"
    qCDebug(g_cat_silence) << QString("Creating <[DFSPC]-[Spec]-specific indefinite> item");
    M1Store::Item_lv2* l_dfspc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Spec]-specific indefinite",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFSPC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfspc->setType("TYPE_");
    l_dfspc->setType("DEFIN");

    // creation of "Degree"
    qCDebug(g_cat_silence) << QString("Creating <[DEGRE]-Degree> item");
    M1Store::Item_lv2* l_degre = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Degree",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DEGRE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_degre->setType("GRATT");

    // creation of "[Abs]-absolute superlative"
    qCDebug(g_cat_silence) << QString("Creating <[DGABS]-[Abs]-absolute superlative> item");
    M1Store::Item_lv2* l_dgabs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Abs]-absolute superlative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGABS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgabs->setType("TYPE_");
    l_dgabs->setType("DEGRE");

    // creation of "[Auf]-augmentative"
    qCDebug(g_cat_silence) << QString("Creating <[DGAUG]-[Auf]-augmentative> item");
    M1Store::Item_lv2* l_dgaug = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Auf]-augmentative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGAUG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgaug->setType("TYPE_");
    l_dgaug->setType("DEGRE");

    // creation of "[Cmp]-comparative, second degree"
    qCDebug(g_cat_silence) << QString("Creating <[DGCMP]-[Cmp]-comparative, second degree> item");
    M1Store::Item_lv2* l_dgcmp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cmp]-comparative, second degree",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGCMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgcmp->setType("TYPE_");
    l_dgcmp->setType("DEGRE");

    // creation of "[Dim]-diminutive"
    qCDebug(g_cat_silence) << QString("Creating <[DGDIM]-[Dim]-diminutive> item");
    M1Store::Item_lv2* l_dgdim = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Dim]-diminutive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGDIM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgdim->setType("TYPE_");
    l_dgdim->setType("DEGRE");

    // creation of "[Equ]-equative"
    qCDebug(g_cat_silence) << QString("Creating <[DGEQU]-[Equ]-equative> item");
    M1Store::Item_lv2* l_dgequ = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Equ]-equative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGEQU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgequ->setType("TYPE_");
    l_dgequ->setType("DEGRE");

    // creation of "[Pos]-positive, first degree"
    qCDebug(g_cat_silence) << QString("Creating <[DGPOS]-[Pos]-positive, first degree> item");
    M1Store::Item_lv2* l_dgpos = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pos]-positive, first degree",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGPOS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgpos->setType("TYPE_");
    l_dgpos->setType("DEGRE");

    // creation of "[Sup]-superlative, third degree"
    qCDebug(g_cat_silence) << QString("Creating <[DGSUP]-[Sup]-superlative, third degree> item");
    M1Store::Item_lv2* l_dgsup = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sup]-superlative, third degree",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGSUP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgsup->setType("TYPE_");
    l_dgsup->setType("DEGRE");

    // creation of "Relative location encoded in demonstratives"
    qCDebug(g_cat_silence) << QString("Creating <[DEIXI]-Relative location encoded in demonstratives> item");
    M1Store::Item_lv2* l_deixi = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Relative location encoded in demonstratives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DEIXI",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_deixi->setType("GRATT");

    // creation of "[Abv]-above the reference point"
    qCDebug(g_cat_silence) << QString("Creating <[DXABV]-[Abv]-above the reference point> item");
    M1Store::Item_lv2* l_dxabv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Abv]-above the reference point",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXABV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxabv->setType("TYPE_");
    l_dxabv->setType("DEIXI");

    // creation of "[Bel]-below the reference point"
    qCDebug(g_cat_silence) << QString("Creating <[DXBEL]-[Bel]-below the reference point> item");
    M1Store::Item_lv2* l_dxbel = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bel]-below the reference point",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXBEL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxbel->setType("TYPE_");
    l_dxbel->setType("DEIXI");

    // creation of "[Even]-at the same level as the reference point"
    qCDebug(g_cat_silence) << QString("Creating <[DXEVN]-[Even]-at the same level as the reference point> item");
    M1Store::Item_lv2* l_dxevn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Even]-at the same level as the reference point",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXEVN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxevn->setType("TYPE_");
    l_dxevn->setType("DEIXI");

    // creation of "[Med]-medial"
    qCDebug(g_cat_silence) << QString("Creating <[DXMED]-[Med]-medial> item");
    M1Store::Item_lv2* l_dxmed = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Med]-medial",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXMED",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxmed->setType("TYPE_");
    l_dxmed->setType("DEIXI");

    // creation of "[Nvis]-not visible"
    qCDebug(g_cat_silence) << QString("Creating <[DXNVS]-[Nvis]-not visible> item");
    M1Store::Item_lv2* l_dxnvs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Nvis]-not visible",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXNVS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxnvs->setType("TYPE_");
    l_dxnvs->setType("DEIXI");

    // creation of "[Prox]-proximate"
    qCDebug(g_cat_silence) << QString("Creating <[DXPRX]-[Prox]-proximate> item");
    M1Store::Item_lv2* l_dxprx = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Prox]-proximate",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXPRX",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxprx->setType("TYPE_");
    l_dxprx->setType("DEIXI");

    // creation of "[Remt]-remote, distal"
    qCDebug(g_cat_silence) << QString("Creating <[DXRMT]-[Remt]-remote, distal> item");
    M1Store::Item_lv2* l_dxrmt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Remt]-remote, distal",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXRMT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxrmt->setType("TYPE_");
    l_dxrmt->setType("DEIXI");

    // creation of "Person to which deixis is relative"
    qCDebug(g_cat_silence) << QString("Creating <[DRXSF]-Person to which deixis is relative> item");
    M1Store::Item_lv2* l_drxsf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Person to which deixis is relative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DRXSF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_drxsf->setType("GRATT");

    // creation of "[1]-deixis relative to the first person participant (speaker)"
    qCDebug(g_cat_silence) << QString("Creating <[DRXS1]-[1]-deixis relative to the first person participant (speaker)> item");
    M1Store::Item_lv2* l_drxs1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[1]-deixis relative to the first person participant (speaker)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DRXS1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_drxs1->setType("TYPE_");
    l_drxs1->setType("DRXSF");

    // creation of "[2]-deixis relative to the second person participant (hearer)"
    qCDebug(g_cat_silence) << QString("Creating <[DRXS2]-[2]-deixis relative to the second person participant (hearer)> item");
    M1Store::Item_lv2* l_drxs2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[2]-deixis relative to the second person participant (hearer)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DRXS2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_drxs2->setType("TYPE_");
    l_drxs2->setType("DRXSF");

    // creation of "Evidentiality"
    qCDebug(g_cat_silence) << QString("Creating <[EVDNT]-Evidentiality> item");
    M1Store::Item_lv2* l_evdnt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Evidentiality",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EVDNT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_evdnt->setType("GRATT");

    // creation of "[Fh]-firsthand"
    qCDebug(g_cat_silence) << QString("Creating <[EVDFH]-[Fh]-firsthand> item");
    M1Store::Item_lv2* l_evdfh = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Fh]-firsthand",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EVDFH",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_evdfh->setType("TYPE_");
    l_evdfh->setType("EVDNT");

    // creation of "[Nfh]-non-firsthand"
    qCDebug(g_cat_silence) << QString("Creating <[EVNFH]-[Nfh]-non-firsthand> item");
    M1Store::Item_lv2* l_evnfh = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Nfh]-non-firsthand",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EVNFH",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_evnfh->setType("TYPE_");
    l_evnfh->setType("EVDNT");

    // creation of "External part of speech"
    qCDebug(g_cat_silence) << QString("Creating <[EPXTS]-External part of speech> item");
    M1Store::Item_lv2* l_epxts = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "External part of speech",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPXTS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epxts->setType("GRATT");

    // creation of "[ADJ]-adjective-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[EPADJ]-[ADJ]-adjective-like expression> item");
    M1Store::Item_lv2* l_epadj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[ADJ]-adjective-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPADJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epadj->setType("TYPE_");
    l_epadj->setType("EPXTS");

    // creation of "[ADP]-adposition-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[EPADP]-[ADP]-adposition-like expression> item");
    M1Store::Item_lv2* l_epadp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[ADP]-adposition-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPADP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epadp->setType("TYPE_");
    l_epadp->setType("EPXTS");

    // creation of "[ADV]-adverb-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[EPADV]-[ADV]-adverb-like expression> item");
    M1Store::Item_lv2* l_epadv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[ADV]-adverb-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPADV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epadv->setType("TYPE_");
    l_epadv->setType("EPXTS");

    // creation of "[AUX]-auxiliary-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[EPAUX]-[AUX]-auxiliary-like expression> item");
    M1Store::Item_lv2* l_epaux = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[AUX]-auxiliary-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPAUX",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epaux->setType("TYPE_");
    l_epaux->setType("EPXTS");

    // creation of "[CCONJ]-coordinating conjunction-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[CCONJ]-[CCONJ]-coordinating conjunction-like expression> item");
    M1Store::Item_lv2* l_cconj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[CCONJ]-coordinating conjunction-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CCONJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cconj->setType("TYPE_");
    l_cconj->setType("EPXTS");

    // creation of "[DET]-determiner-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[EPDET]-[DET]-determiner-like expression> item");
    M1Store::Item_lv2* l_epdet = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[DET]-determiner-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPDET",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epdet->setType("TYPE_");
    l_epdet->setType("EPXTS");

    // creation of "[INTJ]-interjection-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[EINTJ]-[INTJ]-interjection-like expression> item");
    M1Store::Item_lv2* l_eintj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[INTJ]-interjection-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EINTJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_eintj->setType("TYPE_");
    l_eintj->setType("EPXTS");

    // creation of "[PRON]-pronoun-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[EPRON]-[PRON]-pronoun-like expression> item");
    M1Store::Item_lv2* l_epron = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PRON]-pronoun-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPRON",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epron->setType("TYPE_");
    l_epron->setType("EPXTS");

    // creation of "[PROPN]-proper noun-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[PROPN]-[PROPN]-proper noun-like expression> item");
    M1Store::Item_lv2* l_propn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PROPN]-proper noun-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PROPN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_propn->setType("TYPE_");
    l_propn->setType("EPXTS");

    // creation of "[SCONJ]-subordinator-like expression"
    qCDebug(g_cat_silence) << QString("Creating <[SCONJ]-[SCONJ]-subordinator-like expression> item");
    M1Store::Item_lv2* l_sconj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[SCONJ]-subordinator-like expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SCONJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_sconj->setType("TYPE_");
    l_sconj->setType("EPXTS");

    // creation of "Is this a foreign word?"
    qCDebug(g_cat_silence) << QString("Creating <[FOREI]-Is this a foreign word?> item");
    M1Store::Item_lv2* l_forei = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Is this a foreign word?",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "FOREI",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_forei->setType("GRATT");

    // creation of "[Yes]-it is foreign"
    qCDebug(g_cat_silence) << QString("Creating <[FRYES]-[Yes]-it is foreign> item");
    M1Store::Item_lv2* l_fryes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Yes]-it is foreign",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "FRYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_fryes->setType("TYPE_");
    l_fryes->setType("FOREI");

    // creation of "Gender"
    qCDebug(g_cat_silence) << QString("Creating <[GENDE]-Gender> item");
    M1Store::Item_lv2* l_gende = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Gender",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GENDE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gende->setType("GRATT");

    // creation of "[Com]-common gender"
    qCDebug(g_cat_silence) << QString("Creating <[GNCOM]-[Com]-common gender> item");
    M1Store::Item_lv2* l_gncom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Com]-common gender",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GNCOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gncom->setType("TYPE_");
    l_gncom->setType("GENDE");

    // creation of "[Fem]-feminine gender"
    qCDebug(g_cat_silence) << QString("Creating <[GNFEM]-[Fem]-feminine gender> item");
    M1Store::Item_lv2* l_gnfem = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Fem]-feminine gender",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GNFEM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gnfem->setType("TYPE_");
    l_gnfem->setType("GENDE");

    // creation of "[Masc]-masculine gender"
    qCDebug(g_cat_silence) << QString("Creating <[GMASC]-[Masc]-masculine gender> item");
    M1Store::Item_lv2* l_gmasc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Masc]-masculine gender",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GMASC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gmasc->setType("TYPE_");
    l_gmasc->setType("GENDE");

    // creation of "[Neut]-neuter gender"
    qCDebug(g_cat_silence) << QString("Creating <[GNEUT]-[Neut]-neuter gender> item");
    M1Store::Item_lv2* l_gneut = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Neut]-neuter gender",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GNEUT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gneut->setType("TYPE_");
    l_gneut->setType("GENDE");

    // creation of "Mood"
    qCDebug(g_cat_silence) << QString("Creating <[MOOD_]-Mood> item");
    M1Store::Item_lv2* l_mood = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Mood",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MOOD_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mood->setType("GRATT");

    // creation of "[Ben]-benedictive / precative"
    qCDebug(g_cat_silence) << QString("Creating <[MDBEN]-[Ben]-benedictive / precative> item");
    M1Store::Item_lv2* l_mdben = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ben]-benedictive / precative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDBEN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdben->setType("TYPE_");
    l_mdben->setType("MOOD_");

    // creation of "[Adm]-admirative"
    qCDebug(g_cat_silence) << QString("Creating <[MDADM]-[Adm]-admirative> item");
    M1Store::Item_lv2* l_mdadm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Adm]-admirative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDADM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdadm->setType("TYPE_");
    l_mdadm->setType("MOOD_");

    // creation of "[Cnd]-conditional"
    qCDebug(g_cat_silence) << QString("Creating <[MDCND]-[Cnd]-conditional> item");
    M1Store::Item_lv2* l_mdcnd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cnd]-conditional",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDCND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdcnd->setType("TYPE_");
    l_mdcnd->setType("MOOD_");

    // creation of "[Des]-desiderative"
    qCDebug(g_cat_silence) << QString("Creating <[MDDES]-[Des]-desiderative> item");
    M1Store::Item_lv2* l_mddes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Des]-desiderative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDDES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mddes->setType("TYPE_");
    l_mddes->setType("MOOD_");

    // creation of "[Imp]-imperative"
    qCDebug(g_cat_silence) << QString("Creating <[MDIMP]-[Imp]-imperative> item");
    M1Store::Item_lv2* l_mdimp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Imp]-imperative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDIMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdimp->setType("TYPE_");
    l_mdimp->setType("MOOD_");

    // creation of "[Its]-intensive"
    qCDebug(g_cat_silence) << QString("Creating <[MDITS]-[Its]-intensive> item");
    M1Store::Item_lv2* l_mdits = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Its]-intensive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDITS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdits->setType("TYPE_");
    l_mdits->setType("MOOD_");

    // creation of "[Ind]-indicative or realis"
    qCDebug(g_cat_silence) << QString("Creating <[MDIND]-[Ind]-indicative or realis> item");
    M1Store::Item_lv2* l_mdind = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ind]-indicative or realis",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDIND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdind->setType("TYPE_");
    l_mdind->setType("MOOD_");

    // creation of "[Int]-interrogative"
    qCDebug(g_cat_silence) << QString("Creating <[MDINT]-[Int]-interrogative> item");
    M1Store::Item_lv2* l_mdint = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Int]-interrogative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDINT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdint->setType("TYPE_");
    l_mdint->setType("MOOD_");

    // creation of "[Irr]-irrealis"
    qCDebug(g_cat_silence) << QString("Creating <[MDIRR]-[Irr]-irrealis> item");
    M1Store::Item_lv2* l_mdirr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Irr]-irrealis",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDIRR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdirr->setType("TYPE_");
    l_mdirr->setType("MOOD_");

    // creation of "[Jus]-jussive / injunctive"
    qCDebug(g_cat_silence) << QString("Creating <[MDJUS]-[Jus]-jussive / injunctive> item");
    M1Store::Item_lv2* l_mdjus = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Jus]-jussive / injunctive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDJUS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdjus->setType("TYPE_");
    l_mdjus->setType("MOOD_");

    // creation of "[Nec]-necessitative"
    qCDebug(g_cat_silence) << QString("Creating <[MDNEC]-[Nec]-necessitative> item");
    M1Store::Item_lv2* l_mdnec = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Nec]-necessitative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDNEC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdnec->setType("TYPE_");
    l_mdnec->setType("MOOD_");

    // creation of "[Opt]-optative"
    qCDebug(g_cat_silence) << QString("Creating <[MDOPT]-[Opt]-optative> item");
    M1Store::Item_lv2* l_mdopt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Opt]-optative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDOPT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdopt->setType("TYPE_");
    l_mdopt->setType("MOOD_");

    // creation of "[Pot]-potential"
    qCDebug(g_cat_silence) << QString("Creating <[MDPOT]-[Pot]-potential> item");
    M1Store::Item_lv2* l_mdpot = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pot]-potential",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDPOT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdpot->setType("TYPE_");
    l_mdpot->setType("MOOD_");

    // creation of "[Prp]-purposive"
    qCDebug(g_cat_silence) << QString("Creating <[MDPRP]-[Prp]-purposive> item");
    M1Store::Item_lv2* l_mdprp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Prp]-purposive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDPRP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdprp->setType("TYPE_");
    l_mdprp->setType("MOOD_");

    // creation of "[Qot]-quotative"
    qCDebug(g_cat_silence) << QString("Creating <[MDQOT]-[Qot]-quotative> item");
    M1Store::Item_lv2* l_mdqot = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Qot]-quotative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDQOT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdqot->setType("TYPE_");
    l_mdqot->setType("MOOD_");

    // creation of "[Sub]-subjunctive / conjunctive"
    qCDebug(g_cat_silence) << QString("Creating <[MDSUB]-[Sub]-subjunctive / conjunctive> item");
    M1Store::Item_lv2* l_mdsub = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sub]-subjunctive / conjunctive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDSUB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdsub->setType("TYPE_");
    l_mdsub->setType("MOOD_");

    // creation of "Noun class"
    qCDebug(g_cat_silence) << QString("Creating <[NCNLS]-Noun class> item");
    M1Store::Item_lv2* l_ncnls = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Noun class",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCNLS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncnls->setType("GRATT");

    // creation of "[Bantu1]-singular, persons"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN1]-[Bantu1]-singular, persons> item");
    M1Store::Item_lv2* l_ncbn1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu1]-singular, persons",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn1->setType("TYPE_");
    l_ncbn1->setType("NCNLS");

    // creation of "[Bantu2]-plural, persons"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN2]-[Bantu2]-plural, persons> item");
    M1Store::Item_lv2* l_ncbn2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu2]-plural, persons",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn2->setType("TYPE_");
    l_ncbn2->setType("NCNLS");

    // creation of "[Bantu3]-singular, plants, thin objects"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN3]-[Bantu3]-singular, plants, thin objects> item");
    M1Store::Item_lv2* l_ncbn3 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu3]-singular, plants, thin objects",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN3",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn3->setType("TYPE_");
    l_ncbn3->setType("NCNLS");

    // creation of "[Bantu4]-plural, plants, thin objects"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN4]-[Bantu4]-plural, plants, thin objects> item");
    M1Store::Item_lv2* l_ncbn4 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu4]-plural, plants, thin objects",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN4",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn4->setType("TYPE_");
    l_ncbn4->setType("NCNLS");

    // creation of "[Bantu5]-singular, fruits, round objects, paired things"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN5]-[Bantu5]-singular, fruits, round objects, paired things> item");
    M1Store::Item_lv2* l_ncbn5 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu5]-singular, fruits, round objects, paired things",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN5",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn5->setType("TYPE_");
    l_ncbn5->setType("NCNLS");

    // creation of "[Bantu6]-plural, fruits, round objects, paired things"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN6]-[Bantu6]-plural, fruits, round objects, paired things> item");
    M1Store::Item_lv2* l_ncbn6 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu6]-plural, fruits, round objects, paired things",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN6",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn6->setType("TYPE_");
    l_ncbn6->setType("NCNLS");

    // creation of "[Bantu7]-singular, things, diminutives"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN7]-[Bantu7]-singular, things, diminutives> item");
    M1Store::Item_lv2* l_ncbn7 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu7]-singular, things, diminutives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN7",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn7->setType("TYPE_");
    l_ncbn7->setType("NCNLS");

    // creation of "[Bantu8]-plural, things, diminutives"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN8]-[Bantu8]-plural, things, diminutives> item");
    M1Store::Item_lv2* l_ncbn8 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu8]-plural, things, diminutives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN8",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn8->setType("TYPE_");
    l_ncbn8->setType("NCNLS");

    // creation of "[Bantu9]-singular, animals, things"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN9]-[Bantu9]-singular, animals, things> item");
    M1Store::Item_lv2* l_ncbn9 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu9]-singular, animals, things",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN9",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn9->setType("TYPE_");
    l_ncbn9->setType("NCNLS");

    // creation of "[Bantu10]-plural, animals, things"
    qCDebug(g_cat_silence) << QString("Creating <[NCB10]-[Bantu10]-plural, animals, things> item");
    M1Store::Item_lv2* l_ncb10 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu10]-plural, animals, things",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB10",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb10->setType("TYPE_");
    l_ncb10->setType("NCNLS");

    // creation of "[Bantu11]-long thin objects, natural phenomena, abstracts"
    qCDebug(g_cat_silence) << QString("Creating <[NCB11]-[Bantu11]-long thin objects, natural phenomena, abstracts> item");
    M1Store::Item_lv2* l_ncb11 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu11]-long thin objects, natural phenomena, abstracts",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB11",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb11->setType("TYPE_");
    l_ncb11->setType("NCNLS");

    // creation of "[Bantu12]-singular, small things, diminutives"
    qCDebug(g_cat_silence) << QString("Creating <[NCB12]-[Bantu12]-singular, small things, diminutives> item");
    M1Store::Item_lv2* l_ncb12 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu12]-singular, small things, diminutives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB12",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb12->setType("TYPE_");
    l_ncb12->setType("NCNLS");

    // creation of "[Bantu13]-plural or mass, small amount of mass"
    qCDebug(g_cat_silence) << QString("Creating <[NCB13]-[Bantu13]-plural or mass, small amount of mass> item");
    M1Store::Item_lv2* l_ncb13 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu13]-plural or mass, small amount of mass",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB13",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb13->setType("TYPE_");
    l_ncb13->setType("NCNLS");

    // creation of "[Bantu14]-plural, diminutives"
    qCDebug(g_cat_silence) << QString("Creating <[NCB14]-[Bantu14]-plural, diminutives> item");
    M1Store::Item_lv2* l_ncb14 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu14]-plural, diminutives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB14",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb14->setType("TYPE_");
    l_ncb14->setType("NCNLS");

    // creation of "[Bantu15]-verbal nouns, infinitives"
    qCDebug(g_cat_silence) << QString("Creating <[NCB15]-[Bantu15]-verbal nouns, infinitives> item");
    M1Store::Item_lv2* l_ncb15 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu15]-verbal nouns, infinitives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB15",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb15->setType("TYPE_");
    l_ncb15->setType("NCNLS");

    // creation of "[Bantu16]-definite location, close to somethinf"
    qCDebug(g_cat_silence) << QString("Creating <[NCB16]-[Bantu16]-definite location, close to somethinf> item");
    M1Store::Item_lv2* l_ncb16 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu16]-definite location, close to somethinf",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB16",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb16->setType("TYPE_");
    l_ncb16->setType("NCNLS");

    // creation of "[Bantu17]-indefinite location, direction, movement"
    qCDebug(g_cat_silence) << QString("Creating <[NCB17]-[Bantu17]-indefinite location, direction, movement> item");
    M1Store::Item_lv2* l_ncb17 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu17]-indefinite location, direction, movement",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB17",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb17->setType("TYPE_");
    l_ncb17->setType("NCNLS");

    // creation of "[Bantu18]-definite location, inside somethinf"
    qCDebug(g_cat_silence) << QString("Creating <[NCB18]-[Bantu18]-definite location, inside somethinf> item");
    M1Store::Item_lv2* l_ncb18 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu18]-definite location, inside somethinf",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB18",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb18->setType("TYPE_");
    l_ncb18->setType("NCNLS");

    // creation of "[Bantu19]-little bit of, pejorative plural"
    qCDebug(g_cat_silence) << QString("Creating <[NCB19]-[Bantu19]-little bit of, pejorative plural> item");
    M1Store::Item_lv2* l_ncb19 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu19]-little bit of, pejorative plural",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB19",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb19->setType("TYPE_");
    l_ncb19->setType("NCNLS");

    // creation of "[Bantu20]-singular, augmentatives"
    qCDebug(g_cat_silence) << QString("Creating <[NCB20]-[Bantu20]-singular, augmentatives> item");
    M1Store::Item_lv2* l_ncb20 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu20]-singular, augmentatives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB20",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb20->setType("TYPE_");
    l_ncb20->setType("NCNLS");

    // creation of "[Bantu21]-singular, augmentatives, derogatives"
    qCDebug(g_cat_silence) << QString("Creating <[NCB21]-[Bantu21]-singular, augmentatives, derogatives> item");
    M1Store::Item_lv2* l_ncb21 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu21]-singular, augmentatives, derogatives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB21",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb21->setType("TYPE_");
    l_ncb21->setType("NCNLS");

    // creation of "[Bantu22]-plural, augmentatives"
    qCDebug(g_cat_silence) << QString("Creating <[NCB22]-[Bantu22]-plural, augmentatives> item");
    M1Store::Item_lv2* l_ncb22 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu22]-plural, augmentatives",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB22",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb22->setType("TYPE_");
    l_ncb22->setType("NCNLS");

    // creation of "[Bantu23]-location with place names"
    qCDebug(g_cat_silence) << QString("Creating <[NCB23]-[Bantu23]-location with place names> item");
    M1Store::Item_lv2* l_ncb23 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bantu23]-location with place names",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB23",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb23->setType("TYPE_");
    l_ncb23->setType("NCNLS");

    // creation of "[Wol1]-Wolof noun class 1/k (singular human)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL1]-[Wol1]-Wolof noun class 1/k (singular human)> item");
    M1Store::Item_lv2* l_ncwl1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol1]-Wolof noun class 1/k (singular human)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl1->setType("TYPE_");
    l_ncwl1->setType("NCNLS");

    // creation of "[Wol2]-Wolof noun class 2/ñ (plural human)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL2]-[Wol2]-Wolof noun class 2/ñ (plural human)> item");
    M1Store::Item_lv2* l_ncwl2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol2]-Wolof noun class 2/ñ (plural human)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl2->setType("TYPE_");
    l_ncwl2->setType("NCNLS");

    // creation of "[Wol3]-Wolof noun class 3/g (singular)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL3]-[Wol3]-Wolof noun class 3/g (singular)> item");
    M1Store::Item_lv2* l_ncwl3 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol3]-Wolof noun class 3/g (singular)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL3",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl3->setType("TYPE_");
    l_ncwl3->setType("NCNLS");

    // creation of "[Wol4]-Wolof noun class 4/j (singular)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL4]-[Wol4]-Wolof noun class 4/j (singular)> item");
    M1Store::Item_lv2* l_ncwl4 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol4]-Wolof noun class 4/j (singular)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL4",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl4->setType("TYPE_");
    l_ncwl4->setType("NCNLS");

    // creation of "[Wol5]-Wolof noun class 5/b (singular)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL5]-[Wol5]-Wolof noun class 5/b (singular)> item");
    M1Store::Item_lv2* l_ncwl5 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol5]-Wolof noun class 5/b (singular)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL5",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl5->setType("TYPE_");
    l_ncwl5->setType("NCNLS");

    // creation of "[Wol6]-Wolof noun class 6/m (singular)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL6]-[Wol6]-Wolof noun class 6/m (singular)> item");
    M1Store::Item_lv2* l_ncwl6 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol6]-Wolof noun class 6/m (singular)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL6",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl6->setType("TYPE_");
    l_ncwl6->setType("NCNLS");

    // creation of "[Wol7]-Wolof noun class 7/l (singular)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL7]-[Wol7]-Wolof noun class 7/l (singular)> item");
    M1Store::Item_lv2* l_ncwl7 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol7]-Wolof noun class 7/l (singular)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL7",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl7->setType("TYPE_");
    l_ncwl7->setType("NCNLS");

    // creation of "[Wol8]-Wolof noun class 8/y (plural non-human)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL8]-[Wol8]-Wolof noun class 8/y (plural non-human)> item");
    M1Store::Item_lv2* l_ncwl8 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol8]-Wolof noun class 8/y (plural non-human)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL8",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl8->setType("TYPE_");
    l_ncwl8->setType("NCNLS");

    // creation of "[Wol9]-Wolof noun class 9/s (singular)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL9]-[Wol9]-Wolof noun class 9/s (singular)> item");
    M1Store::Item_lv2* l_ncwl9 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol9]-Wolof noun class 9/s (singular)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL9",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl9->setType("TYPE_");
    l_ncwl9->setType("NCNLS");

    // creation of "[Wol10]-Wolof noun class 10/w (singular)"
    qCDebug(g_cat_silence) << QString("Creating <[NCW10]-[Wol10]-Wolof noun class 10/w (singular)> item");
    M1Store::Item_lv2* l_ncw10 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol10]-Wolof noun class 10/w (singular)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCW10",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncw10->setType("TYPE_");
    l_ncw10->setType("NCNLS");

    // creation of "[Wol11]-Wolof noun class 11/g (location)"
    qCDebug(g_cat_silence) << QString("Creating <[NCW11]-[Wol11]-Wolof noun class 11/g (location)> item");
    M1Store::Item_lv2* l_ncw11 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol11]-Wolof noun class 11/g (location)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCW11",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncw11->setType("TYPE_");
    l_ncw11->setType("NCNLS");

    // creation of "[Wol12]-Wolof noun class 12/n (manner)"
    qCDebug(g_cat_silence) << QString("Creating <[NCW12]-[Wol12]-Wolof noun class 12/n (manner)> item");
    M1Store::Item_lv2* l_ncw12 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Wol12]-Wolof noun class 12/n (manner)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCW12",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncw12->setType("TYPE_");
    l_ncw12->setType("NCNLS");

    // creation of "Numeral type"
    qCDebug(g_cat_silence) << QString("Creating <[NTMYP]-Numeral type> item");
    M1Store::Item_lv2* l_ntmyp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Numeral type",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTMYP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntmyp->setType("GRATT");

    // creation of "[Card]-cardinal number or corresponding interrogative / relati"
    qCDebug(g_cat_silence) << QString("Creating <[NTCRD]-[Card]-cardinal number or corresponding interrogative / relati> item");
    M1Store::Item_lv2* l_ntcrd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Card]-cardinal number or corresponding interrogative / relati",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTCRD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntcrd->setType("TYPE_");
    l_ntcrd->setType("NTMYP");

    // creation of "[Dist]-distributive numeral"
    qCDebug(g_cat_silence) << QString("Creating <[NTDST]-[Dist]-distributive numeral> item");
    M1Store::Item_lv2* l_ntdst = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Dist]-distributive numeral",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTDST",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntdst->setType("TYPE_");
    l_ntdst->setType("NTMYP");

    // creation of "[Frac]-fraction"
    qCDebug(g_cat_silence) << QString("Creating <[NTFRC]-[Frac]-fraction> item");
    M1Store::Item_lv2* l_ntfrc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Frac]-fraction",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTFRC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntfrc->setType("TYPE_");
    l_ntfrc->setType("NTMYP");

    // creation of "[Mult]-multiplicative numeral or corresponding interrogative /"
    qCDebug(g_cat_silence) << QString("Creating <[NTMLT]-[Mult]-multiplicative numeral or corresponding interrogative /> item");
    M1Store::Item_lv2* l_ntmlt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Mult]-multiplicative numeral or corresponding interrogative /",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTMLT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntmlt->setType("TYPE_");
    l_ntmlt->setType("NTMYP");

    // creation of "[Ord]-ordinal number or corresponding interrogative / relative"
    qCDebug(g_cat_silence) << QString("Creating <[NTORD]-[Ord]-ordinal number or corresponding interrogative / relative> item");
    M1Store::Item_lv2* l_ntord = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ord]-ordinal number or corresponding interrogative / relative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTORD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntord->setType("TYPE_");
    l_ntord->setType("NTMYP");

    // creation of "[Range]-range of values"
    qCDebug(g_cat_silence) << QString("Creating <[NTRNG]-[Range]-range of values> item");
    M1Store::Item_lv2* l_ntrng = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Range]-range of values",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTRNG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntrng->setType("TYPE_");
    l_ntrng->setType("NTMYP");

    // creation of "[Sets]-number of sets of things; collective numeral"
    qCDebug(g_cat_silence) << QString("Creating <[NTSTS]-[Sets]-number of sets of things; collective numeral> item");
    M1Store::Item_lv2* l_ntsts = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sets]-number of sets of things; collective numeral",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTSTS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntsts->setType("TYPE_");
    l_ntsts->setType("NTMYP");

    // creation of "Number"
    qCDebug(g_cat_silence) << QString("Creating <[NUMBE]-Number> item");
    M1Store::Item_lv2* l_numbe = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NUMBE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_numbe->setType("GRATT");

    // creation of "[Coll]-collective / mass / singulare tantum"
    qCDebug(g_cat_silence) << QString("Creating <[NMCLL]-[Coll]-collective / mass / singulare tantum> item");
    M1Store::Item_lv2* l_nmcll = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Coll]-collective / mass / singulare tantum",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMCLL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmcll->setType("TYPE_");
    l_nmcll->setType("NUMBE");

    // creation of "[Count]-count plural"
    qCDebug(g_cat_silence) << QString("Creating <[NMCNT]-[Count]-count plural> item");
    M1Store::Item_lv2* l_nmcnt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Count]-count plural",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMCNT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmcnt->setType("TYPE_");
    l_nmcnt->setType("NUMBE");

    // creation of "[Dual]-dual number"
    qCDebug(g_cat_silence) << QString("Creating <[NDUAL]-[Dual]-dual number> item");
    M1Store::Item_lv2* l_ndual = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Dual]-dual number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NDUAL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ndual->setType("TYPE_");
    l_ndual->setType("NUMBE");

    // creation of "[Grpa]-greater paucal number"
    qCDebug(g_cat_silence) << QString("Creating <[NMGRP]-[Grpa]-greater paucal number> item");
    M1Store::Item_lv2* l_nmgrp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Grpa]-greater paucal number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMGRP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmgrp->setType("TYPE_");
    l_nmgrp->setType("NUMBE");

    // creation of "[Grpl]-greater plural number"
    qCDebug(g_cat_silence) << QString("Creating <[NMGRL]-[Grpl]-greater plural number> item");
    M1Store::Item_lv2* l_nmgrl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Grpl]-greater plural number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMGRL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmgrl->setType("TYPE_");
    l_nmgrl->setType("NUMBE");

    // creation of "[Inv]-inverse number"
    qCDebug(g_cat_silence) << QString("Creating <[NMINV]-[Inv]-inverse number> item");
    M1Store::Item_lv2* l_nminv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Inv]-inverse number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMINV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nminv->setType("TYPE_");
    l_nminv->setType("NUMBE");

    // creation of "[Pauc]-paucal number"
    qCDebug(g_cat_silence) << QString("Creating <[NMBPC]-[Pauc]-paucal number> item");
    M1Store::Item_lv2* l_nmbpc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pauc]-paucal number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMBPC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmbpc->setType("TYPE_");
    l_nmbpc->setType("NUMBE");

    // creation of "[Plur]-plural number"
    qCDebug(g_cat_silence) << QString("Creating <[NPLUR]-[Plur]-plural number> item");
    M1Store::Item_lv2* l_nplur = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Plur]-plural number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPLUR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nplur->setType("TYPE_");
    l_nplur->setType("NUMBE");

    // creation of "[Ptan]-plurale tantum"
    qCDebug(g_cat_silence) << QString("Creating <[NMPTN]-[Ptan]-plurale tantum> item");
    M1Store::Item_lv2* l_nmptn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ptan]-plurale tantum",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMPTN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmptn->setType("TYPE_");
    l_nmptn->setType("NUMBE");

    // creation of "[Sing]-singular number"
    qCDebug(g_cat_silence) << QString("Creating <[NSING]-[Sing]-singular number> item");
    M1Store::Item_lv2* l_nsing = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sing]-singular number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NSING",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nsing->setType("TYPE_");
    l_nsing->setType("NUMBE");

    // creation of "[Tri]-trial number"
    qCDebug(g_cat_silence) << QString("Creating <[NMTRI]-[Tri]-trial number> item");
    M1Store::Item_lv2* l_nmtri = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Tri]-trial number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMTRI",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmtri->setType("TYPE_");
    l_nmtri->setType("NUMBE");

    // creation of "Person"
    qCDebug(g_cat_silence) << QString("Creating <[PERSO]-Person> item");
    M1Store::Item_lv2* l_perso = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Person",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PERSO",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_perso->setType("GRATT");

    // creation of "[0]-zero person"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN0]-[0]-zero person> item");
    M1Store::Item_lv2* l_prsn0 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[0]-zero person",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN0",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn0->setType("TYPE_");
    l_prsn0->setType("PERSO");

    // creation of "[1]-first person"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN1]-[1]-first person> item");
    M1Store::Item_lv2* l_prsn1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[1]-first person",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn1->setType("TYPE_");
    l_prsn1->setType("PERSO");

    // creation of "[2]-second person"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN2]-[2]-second person> item");
    M1Store::Item_lv2* l_prsn2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[2]-second person",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn2->setType("TYPE_");
    l_prsn2->setType("PERSO");

    // creation of "[3]-third person"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN3]-[3]-third person> item");
    M1Store::Item_lv2* l_prsn3 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[3]-third person",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN3",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn3->setType("TYPE_");
    l_prsn3->setType("PERSO");

    // creation of "[4]-fourth person"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN4]-[4]-fourth person> item");
    M1Store::Item_lv2* l_prsn4 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[4]-fourth person",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN4",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn4->setType("TYPE_");
    l_prsn4->setType("PERSO");

    // creation of "Polarity"
    qCDebug(g_cat_silence) << QString("Creating <[PLRTY]-Polarity> item");
    M1Store::Item_lv2* l_plrty = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Polarity",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLRTY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plrty->setType("GRATT");

    // creation of "[Neg]-negative"
    qCDebug(g_cat_silence) << QString("Creating <[PLNEG]-[Neg]-negative> item");
    M1Store::Item_lv2* l_plneg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Neg]-negative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLNEG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plneg->setType("TYPE_");
    l_plneg->setType("PLRTY");

    // creation of "[Pos]-positive, affirmative"
    qCDebug(g_cat_silence) << QString("Creating <[PLPOS]-[Pos]-positive, affirmative> item");
    M1Store::Item_lv2* l_plpos = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pos]-positive, affirmative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLPOS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plpos->setType("TYPE_");
    l_plpos->setType("PLRTY");

    // creation of "Politeness"
    qCDebug(g_cat_silence) << QString("Creating <[POLIT]-Politeness> item");
    M1Store::Item_lv2* l_polit = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Politeness",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "POLIT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_polit->setType("GRATT");

    // creation of "[Elev]-referent elevatinf"
    qCDebug(g_cat_silence) << QString("Creating <[PLELV]-[Elev]-referent elevatinf> item");
    M1Store::Item_lv2* l_plelv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Elev]-referent elevatinf",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLELV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plelv->setType("TYPE_");
    l_plelv->setType("POLIT");

    // creation of "[Form]-formal register"
    qCDebug(g_cat_silence) << QString("Creating <[PLFRM]-[Form]-formal register> item");
    M1Store::Item_lv2* l_plfrm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Form]-formal register",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLFRM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plfrm->setType("TYPE_");
    l_plfrm->setType("POLIT");

    // creation of "[Humb]-speaker humblinf"
    qCDebug(g_cat_silence) << QString("Creating <[PLHMB]-[Humb]-speaker humblinf> item");
    M1Store::Item_lv2* l_plhmb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Humb]-speaker humblinf",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLHMB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plhmb->setType("TYPE_");
    l_plhmb->setType("POLIT");

    // creation of "[Infm]-informal register"
    qCDebug(g_cat_silence) << QString("Creating <[PLINF]-[Infm]-informal register> item");
    M1Store::Item_lv2* l_plinf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Infm]-informal register",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLINF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plinf->setType("TYPE_");
    l_plinf->setType("POLIT");

    // creation of "Possessive"
    qCDebug(g_cat_silence) << QString("Creating <[POSS_]-Possessive> item");
    M1Store::Item_lv2* l_poss = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Possessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "POSS_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_poss->setType("GRATT");

    // creation of "[Yes]-it is possessive"
    qCDebug(g_cat_silence) << QString("Creating <[PSYES]-[Yes]-it is possessive> item");
    M1Store::Item_lv2* l_psyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Yes]-it is possessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PSYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_psyes->setType("TYPE_");
    l_psyes->setType("POSS_");

    // creation of "Pronominal type"
    qCDebug(g_cat_silence) << QString("Creating <[PTRNY]-Pronominal type> item");
    M1Store::Item_lv2* l_ptrny = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pronominal type",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTRNY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptrny->setType("GRATT");

    // creation of "[Art]-article"
    qCDebug(g_cat_silence) << QString("Creating <[PTART]-[Art]-article> item");
    M1Store::Item_lv2* l_ptart = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Art]-article",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTART",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptart->setType("TYPE_");
    l_ptart->setType("PTRNY");

    // creation of "[Dem]-demonstrative pronoun, determiner, numeral or adverb"
    qCDebug(g_cat_silence) << QString("Creating <[PTDEM]-[Dem]-demonstrative pronoun, determiner, numeral or adverb> item");
    M1Store::Item_lv2* l_ptdem = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Dem]-demonstrative pronoun, determiner, numeral or adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTDEM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptdem->setType("TYPE_");
    l_ptdem->setType("PTRNY");

    // creation of "[Emp]-emphatic determiner"
    qCDebug(g_cat_silence) << QString("Creating <[PTEMP]-[Emp]-emphatic determiner> item");
    M1Store::Item_lv2* l_ptemp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Emp]-emphatic determiner",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTEMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptemp->setType("TYPE_");
    l_ptemp->setType("PTRNY");

    // creation of "[Exc]-exclamative determiner"
    qCDebug(g_cat_silence) << QString("Creating <[PTEXC]-[Exc]-exclamative determiner> item");
    M1Store::Item_lv2* l_ptexc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Exc]-exclamative determiner",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTEXC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptexc->setType("TYPE_");
    l_ptexc->setType("PTRNY");

    // creation of "[Ind]-indefinite pronoun, determiner, numeral or adverb"
    qCDebug(g_cat_silence) << QString("Creating <[PTIND]-[Ind]-indefinite pronoun, determiner, numeral or adverb> item");
    M1Store::Item_lv2* l_ptind = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ind]-indefinite pronoun, determiner, numeral or adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTIND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptind->setType("TYPE_");
    l_ptind->setType("PTRNY");

    // creation of "[Int]-interrogative pronoun, determiner, numeral or adverb"
    qCDebug(g_cat_silence) << QString("Creating <[PTINT]-[Int]-interrogative pronoun, determiner, numeral or adverb> item");
    M1Store::Item_lv2* l_ptint = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Int]-interrogative pronoun, determiner, numeral or adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTINT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptint->setType("TYPE_");
    l_ptint->setType("PTRNY");

    // creation of "[Neg]-negative pronoun, determiner or adverb"
    qCDebug(g_cat_silence) << QString("Creating <[PTNEG]-[Neg]-negative pronoun, determiner or adverb> item");
    M1Store::Item_lv2* l_ptneg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Neg]-negative pronoun, determiner or adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTNEG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptneg->setType("TYPE_");
    l_ptneg->setType("PTRNY");

    // creation of "[Prs]-personal or possessive personal pronoun or determiner"
    qCDebug(g_cat_silence) << QString("Creating <[PTPRS]-[Prs]-personal or possessive personal pronoun or determiner> item");
    M1Store::Item_lv2* l_ptprs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Prs]-personal or possessive personal pronoun or determiner",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTPRS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptprs->setType("TYPE_");
    l_ptprs->setType("PTRNY");

    // creation of "[Rcp]-reciprocal pronoun"
    qCDebug(g_cat_silence) << QString("Creating <[PTRCP]-[Rcp]-reciprocal pronoun> item");
    M1Store::Item_lv2* l_ptrcp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Rcp]-reciprocal pronoun",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTRCP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptrcp->setType("TYPE_");
    l_ptrcp->setType("PTRNY");

    // creation of "[Rel]-relative pronoun, determiner, numeral or adverb"
    qCDebug(g_cat_silence) << QString("Creating <[PTREL]-[Rel]-relative pronoun, determiner, numeral or adverb> item");
    M1Store::Item_lv2* l_ptrel = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Rel]-relative pronoun, determiner, numeral or adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTREL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptrel->setType("TYPE_");
    l_ptrel->setType("PTRNY");

    // creation of "[Tot]-total (collective) pronoun, determiner or adverb"
    qCDebug(g_cat_silence) << QString("Creating <[PTTOT]-[Tot]-total (collective) pronoun, determiner or adverb> item");
    M1Store::Item_lv2* l_pttot = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Tot]-total (collective) pronoun, determiner or adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTTOT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_pttot->setType("TYPE_");
    l_pttot->setType("PTRNY");

    // creation of "Reflexive"
    qCDebug(g_cat_silence) << QString("Creating <[REFLE]-Reflexive> item");
    M1Store::Item_lv2* l_refle = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Reflexive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "REFLE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_refle->setType("GRATT");

    // creation of "[Yes]-it is reflexive"
    qCDebug(g_cat_silence) << QString("Creating <[RFYES]-[Yes]-it is reflexive> item");
    M1Store::Item_lv2* l_rfyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Yes]-it is reflexive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RFYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_rfyes->setType("TYPE_");
    l_rfyes->setType("REFLE");

    // creation of "In initio"
    qCDebug(g_cat_silence) << QString("Creating <[II___]-In initio> item");
    M1Store::Item_lv2* l_ii = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "In initio",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "II___",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ii->setType("GRATT");

    // creation of "[iic]-in initio compositi"
    qCDebug(g_cat_silence) << QString("Creating <[IICOM]-[iic]-in initio compositi> item");
    M1Store::Item_lv2* l_iicom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[iic]-in initio compositi",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "IICOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_iicom->setType("TYPE_");
    l_iicom->setType("II___");

    // creation of "[iiv]-in initio verbi"
    qCDebug(g_cat_silence) << QString("Creating <[IIVRB]-[iiv]-in initio verbi> item");
    M1Store::Item_lv2* l_iivrb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[iiv]-in initio verbi",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "IIVRB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_iivrb->setType("TYPE_");
    l_iivrb->setType("II___");

    // creation of "Tense"
    qCDebug(g_cat_silence) << QString("Creating <[TENSE]-Tense> item");
    M1Store::Item_lv2* l_tense = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Tense",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TENSE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tense->setType("GRATT");

    // creation of "[Fut]-future tense"
    qCDebug(g_cat_silence) << QString("Creating <[TNFUT]-[Fut]-future tense> item");
    M1Store::Item_lv2* l_tnfut = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Fut]-future tense",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNFUT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnfut->setType("TYPE_");
    l_tnfut->setType("TENSE");

    // creation of "[Imp]-imperfect"
    qCDebug(g_cat_silence) << QString("Creating <[TNIMP]-[Imp]-imperfect> item");
    M1Store::Item_lv2* l_tnimp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Imp]-imperfect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNIMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnimp->setType("TYPE_");
    l_tnimp->setType("TENSE");

    // creation of "[Past]-past tense / preterite / aorist"
    qCDebug(g_cat_silence) << QString("Creating <[TNPST]-[Past]-past tense / preterite / aorist> item");
    M1Store::Item_lv2* l_tnpst = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Past]-past tense / preterite / aorist",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNPST",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnpst->setType("TYPE_");
    l_tnpst->setType("TENSE");

    // creation of "[Pqp]-pluperfect"
    qCDebug(g_cat_silence) << QString("Creating <[TNPQP]-[Pqp]-pluperfect> item");
    M1Store::Item_lv2* l_tnpqp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pqp]-pluperfect",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNPQP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnpqp->setType("TYPE_");
    l_tnpqp->setType("TENSE");

    // creation of "[Pres]-present / non-past tense / aorist"
    qCDebug(g_cat_silence) << QString("Creating <[TNPRS]-[Pres]-present / non-past tense / aorist> item");
    M1Store::Item_lv2* l_tnprs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pres]-present / non-past tense / aorist",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNPRS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnprs->setType("TYPE_");
    l_tnprs->setType("TENSE");

    // creation of "Is this a misspelled word?"
    qCDebug(g_cat_silence) << QString("Creating <[TYPO_]-Is this a misspelled word?> item");
    M1Store::Item_lv2* l_typo = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Is this a misspelled word?",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TYPO_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_typo->setType("GRATT");

    // creation of "[Yes]-it is typo"
    qCDebug(g_cat_silence) << QString("Creating <[TYYES]-[Yes]-it is typo> item");
    M1Store::Item_lv2* l_tyyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Yes]-it is typo",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TYYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tyyes->setType("TYPE_");
    l_tyyes->setType("TYPO_");

    // creation of "Form of verb or deverbative"
    qCDebug(g_cat_silence) << QString("Creating <[VFRBR]-Form of verb or deverbative> item");
    M1Store::Item_lv2* l_vfrbr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Form of verb or deverbative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFRBR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfrbr->setType("GRATT");

    // creation of "[Conv]-converb, transgressive, adverbial participle, verbal ad"
    qCDebug(g_cat_silence) << QString("Creating <[VFCNV]-[Conv]-converb, transgressive, adverbial participle, verbal ad> item");
    M1Store::Item_lv2* l_vfcnv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Conv]-converb, transgressive, adverbial participle, verbal ad",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFCNV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfcnv->setType("TYPE_");
    l_vfcnv->setType("VFRBR");

    // creation of "[Fin]-finite verb"
    qCDebug(g_cat_silence) << QString("Creating <[VFFIN]-[Fin]-finite verb> item");
    M1Store::Item_lv2* l_vffin = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Fin]-finite verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFFIN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vffin->setType("TYPE_");
    l_vffin->setType("VFRBR");

    // creation of "[Gdv]-gerundive"
    qCDebug(g_cat_silence) << QString("Creating <[VFGDV]-[Gdv]-gerundive> item");
    M1Store::Item_lv2* l_vfgdv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Gdv]-gerundive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFGDV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfgdv->setType("TYPE_");
    l_vfgdv->setType("VFRBR");

    // creation of "[Ger]-gerund"
    qCDebug(g_cat_silence) << QString("Creating <[VFGER]-[Ger]-gerund> item");
    M1Store::Item_lv2* l_vfger = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Ger]-gerund",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFGER",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfger->setType("TYPE_");
    l_vfger->setType("VFRBR");

    // creation of "[Inf]-infinitive"
    qCDebug(g_cat_silence) << QString("Creating <[VFINF]-[Inf]-infinitive> item");
    M1Store::Item_lv2* l_vfinf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Inf]-infinitive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFINF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfinf->setType("TYPE_");
    l_vfinf->setType("VFRBR");

    // creation of "[Part]-participle, verbal adjective"
    qCDebug(g_cat_silence) << QString("Creating <[VFPRT]-[Part]-participle, verbal adjective> item");
    M1Store::Item_lv2* l_vfprt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Part]-participle, verbal adjective",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFPRT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfprt->setType("TYPE_");
    l_vfprt->setType("VFRBR");

    // creation of "[Sup]-supine"
    qCDebug(g_cat_silence) << QString("Creating <[VFSUP]-[Sup]-supine> item");
    M1Store::Item_lv2* l_vfsup = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Sup]-supine",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFSUP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfsup->setType("TYPE_");
    l_vfsup->setType("VFRBR");

    // creation of "[Cau]-causative"
    qCDebug(g_cat_silence) << QString("Creating <[VFCAU]-[Cau]-causative> item");
    M1Store::Item_lv2* l_vfcau = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cau]-causative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFCAU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfcau->setType("TYPE_");
    l_vfcau->setType("VFRBR");

    // creation of "[Vnoun]-verbal noun, masdar"
    qCDebug(g_cat_silence) << QString("Creating <[VFVNN]-[Vnoun]-verbal noun, masdar> item");
    M1Store::Item_lv2* l_vfvnn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Vnoun]-verbal noun, masdar",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFVNN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfvnn->setType("TYPE_");
    l_vfvnn->setType("VFRBR");

    // creation of "Verb conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VCRBN]-Verb conjugation> item");
    M1Store::Item_lv2* l_vcrbn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Verb conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCRBN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcrbn->setType("GRATT");

    // creation of "[[1]]-first conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ1]-[[1]]-first conjugation> item");
    M1Store::Item_lv2* l_vbcj1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[1]]-first conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj1->setType("TYPE_");
    l_vbcj1->setType("VCRBN");

    // creation of "[[2]]-second conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ2]-[[2]]-second conjugation> item");
    M1Store::Item_lv2* l_vbcj2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[2]]-second conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj2->setType("TYPE_");
    l_vbcj2->setType("VCRBN");

    // creation of "[[3]]-third conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ3]-[[3]]-third conjugation> item");
    M1Store::Item_lv2* l_vbcj3 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[3]]-third conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ3",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj3->setType("TYPE_");
    l_vbcj3->setType("VCRBN");

    // creation of "[[4]]-4th conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ4]-[[4]]-4th conjugation> item");
    M1Store::Item_lv2* l_vbcj4 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[4]]-4th conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ4",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj4->setType("TYPE_");
    l_vbcj4->setType("VCRBN");

    // creation of "[[5]]-5th conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ5]-[[5]]-5th conjugation> item");
    M1Store::Item_lv2* l_vbcj5 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[5]]-5th conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ5",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj5->setType("TYPE_");
    l_vbcj5->setType("VCRBN");

    // creation of "[[6]]-6th conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ6]-[[6]]-6th conjugation> item");
    M1Store::Item_lv2* l_vbcj6 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[6]]-6th conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ6",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj6->setType("TYPE_");
    l_vbcj6->setType("VCRBN");

    // creation of "[[7]]-7th conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ7]-[[7]]-7th conjugation> item");
    M1Store::Item_lv2* l_vbcj7 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[7]]-7th conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ7",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj7->setType("TYPE_");
    l_vbcj7->setType("VCRBN");

    // creation of "[[8]]-8th conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ8]-[[8]]-8th conjugation> item");
    M1Store::Item_lv2* l_vbcj8 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[8]]-8th conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ8",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj8->setType("TYPE_");
    l_vbcj8->setType("VCRBN");

    // creation of "[[9]]-9th conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VBCJ9]-[[9]]-9th conjugation> item");
    M1Store::Item_lv2* l_vbcj9 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[9]]-9th conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VBCJ9",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vbcj9->setType("TYPE_");
    l_vbcj9->setType("VCRBN");

    // creation of "[[10]]-10th conjugation"
    qCDebug(g_cat_silence) << QString("Creating <[VCJ10]-[[10]]-10th conjugation> item");
    M1Store::Item_lv2* l_vcj10 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[[10]]-10th conjugation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCJ10",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcj10->setType("TYPE_");
    l_vcj10->setType("VCRBN");

    // creation of "Verb type"
    qCDebug(g_cat_silence) << QString("Creating <[VTRBY]-Verb type> item");
    M1Store::Item_lv2* l_vtrby = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Verb type",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VTRBY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vtrby->setType("GRATT");

    // creation of "[Aux]-auxiliary verb"
    qCDebug(g_cat_silence) << QString("Creating <[VTAUX]-[Aux]-auxiliary verb> item");
    M1Store::Item_lv2* l_vtaux = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Aux]-auxiliary verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VTAUX",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vtaux->setType("TYPE_");
    l_vtaux->setType("VTRBY");

    // creation of "[Cop]-copula verb"
    qCDebug(g_cat_silence) << QString("Creating <[VTCOP]-[Cop]-copula verb> item");
    M1Store::Item_lv2* l_vtcop = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cop]-copula verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VTCOP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vtcop->setType("TYPE_");
    l_vtcop->setType("VTRBY");

    // creation of "[Mod]-modal verb"
    qCDebug(g_cat_silence) << QString("Creating <[VTMOD]-[Mod]-modal verb> item");
    M1Store::Item_lv2* l_vtmod = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Mod]-modal verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VTMOD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vtmod->setType("TYPE_");
    l_vtmod->setType("VTRBY");

    // creation of "[Light]-light (support) verb"
    qCDebug(g_cat_silence) << QString("Creating <[VTLGH]-[Light]-light (support) verb> item");
    M1Store::Item_lv2* l_vtlgh = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Light]-light (support) verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VTLGH",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vtlgh->setType("TYPE_");
    l_vtlgh->setType("VTRBY");

    // creation of "[Quasi]-quasi-verb"
    qCDebug(g_cat_silence) << QString("Creating <[VTRQS]-[Quasi]-quasi-verb> item");
    M1Store::Item_lv2* l_vtrqs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Quasi]-quasi-verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VTRQS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vtrqs->setType("TYPE_");
    l_vtrqs->setType("VTRBY");

    // creation of "Undecided codes"
    qCDebug(g_cat_silence) << QString("Creating <[UNDCD]-Undecided codes> item");
    M1Store::Item_lv2* l_undcd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Undecided codes",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "UNDCD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_undcd->setType("GRATT");

    // creation of "[act]-"
    qCDebug(g_cat_silence) << QString("Creating <[__ACT]-[act]-> item");
    M1Store::Item_lv2* l_act = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[act]-",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "__ACT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_act->setType("TYPE_");
    l_act->setType("UNDCD");

    // creation of "[agt]-"
    qCDebug(g_cat_silence) << QString("Creating <[__AGT]-[agt]-> item");
    M1Store::Item_lv2* l_agt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[agt]-",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "__AGT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_agt->setType("TYPE_");
    l_agt->setType("UNDCD");

    // creation of "[per]-"
    qCDebug(g_cat_silence) << QString("Creating <[__PER]-[per]-> item");
    M1Store::Item_lv2* l_per = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[per]-",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "__PER",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_per->setType("TYPE_");
    l_per->setType("UNDCD");

    // creation of "[tasil]-"
    qCDebug(g_cat_silence) << QString("Creating <[__TSL]-[tasil]-> item");
    M1Store::Item_lv2* l_tsl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[tasil]-",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "__TSL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tsl->setType("TYPE_");
    l_tsl->setType("UNDCD");

    // creation of "Voice"
    qCDebug(g_cat_silence) << QString("Creating <[VOICE]-Voice> item");
    M1Store::Item_lv2* l_voice = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VOICE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_voice->setType("GRATT");

    // creation of "[Act]-active or actor-focus voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCACT]-[Act]-active or actor-focus voice> item");
    M1Store::Item_lv2* l_vcact = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Act]-active or actor-focus voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCACT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcact->setType("TYPE_");
    l_vcact->setType("VOICE");

    // creation of "[Antip]-antipassive voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCANT]-[Antip]-antipassive voice> item");
    M1Store::Item_lv2* l_vcant = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Antip]-antipassive voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCANT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcant->setType("TYPE_");
    l_vcant->setType("VOICE");

    // creation of "[Bfoc]-beneficiary-focus voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCBFC]-[Bfoc]-beneficiary-focus voice> item");
    M1Store::Item_lv2* l_vcbfc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Bfoc]-beneficiary-focus voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCBFC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcbfc->setType("TYPE_");
    l_vcbfc->setType("VOICE");

    // creation of "[Cau]-causative voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCCAU]-[Cau]-causative voice> item");
    M1Store::Item_lv2* l_vccau = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Cau]-causative voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCCAU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vccau->setType("TYPE_");
    l_vccau->setType("VOICE");

    // creation of "[Dir]-direct voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCDIR]-[Dir]-direct voice> item");
    M1Store::Item_lv2* l_vcdir = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Dir]-direct voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCDIR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcdir->setType("TYPE_");
    l_vcdir->setType("VOICE");

    // creation of "[Inv]-inverse voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCINV]-[Inv]-inverse voice> item");
    M1Store::Item_lv2* l_vcinv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Inv]-inverse voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCINV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcinv->setType("TYPE_");
    l_vcinv->setType("VOICE");

    // creation of "[Lfoc]-location-focus voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCLFC]-[Lfoc]-location-focus voice> item");
    M1Store::Item_lv2* l_vclfc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Lfoc]-location-focus voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCLFC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vclfc->setType("TYPE_");
    l_vclfc->setType("VOICE");

    // creation of "[Mid]-middle voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCMID]-[Mid]-middle voice> item");
    M1Store::Item_lv2* l_vcmid = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Mid]-middle voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCMID",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcmid->setType("TYPE_");
    l_vcmid->setType("VOICE");

    // creation of "[Pass]-passive or patient-focus voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCPSS]-[Pass]-passive or patient-focus voice> item");
    M1Store::Item_lv2* l_vcpss = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Pass]-passive or patient-focus voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCPSS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcpss->setType("TYPE_");
    l_vcpss->setType("VOICE");

    // creation of "[Rcp]-reciprocal voice"
    qCDebug(g_cat_silence) << QString("Creating <[VCRCP]-[Rcp]-reciprocal voice> item");
    M1Store::Item_lv2* l_vcrcp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[Rcp]-reciprocal voice",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCRCP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcrcp->setType("TYPE_");
    l_vcrcp->setType("VOICE");

    // creation of "[IIC]-in intio compositi"
    qCDebug(g_cat_silence) << QString("Creating <[NPIIC]-[IIC]-in intio compositi> item");
    M1Store::Item_lv2* l_npiic = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[IIC]-in intio compositi",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPIIC",
        // icon path
        nullptr);
    l_npiic->setType("TYPE_");
    l_npiic->setType("NLPOS");

    // creation of "[IIV]-in intio verbi"
    qCDebug(g_cat_silence) << QString("Creating <[NPIIV]-[IIV]-in intio verbi> item");
    M1Store::Item_lv2* l_npiiv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[IIV]-in intio verbi",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPIIV",
        // icon path
        nullptr);
    l_npiiv->setType("TYPE_");
    l_npiiv->setType("NLPOS");

    // creation of "[ADJ]-adjective"
    qCDebug(g_cat_silence) << QString("Creating <[NPADJ]-[ADJ]-adjective> item");
    M1Store::Item_lv2* l_npadj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[ADJ]-adjective",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPADJ",
        // icon path
        nullptr);
    l_npadj->setType("TYPE_");
    l_npadj->setType("NLPOS");

    // creation of "[ADP]-adposition"
    qCDebug(g_cat_silence) << QString("Creating <[NPADP]-[ADP]-adposition> item");
    M1Store::Item_lv2* l_npadp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[ADP]-adposition",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPADP",
        // icon path
        nullptr);
    l_npadp->setType("TYPE_");
    l_npadp->setType("NLPOS");

    // creation of "[ADV]-adverb"
    qCDebug(g_cat_silence) << QString("Creating <[NPADV]-[ADV]-adverb> item");
    M1Store::Item_lv2* l_npadv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[ADV]-adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPADV",
        // icon path
        nullptr);
    l_npadv->setType("TYPE_");
    l_npadv->setType("NLPOS");

    // creation of "[AUX]-auxiliary"
    qCDebug(g_cat_silence) << QString("Creating <[NPAUX]-[AUX]-auxiliary> item");
    M1Store::Item_lv2* l_npaux = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[AUX]-auxiliary",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPAUX",
        // icon path
        nullptr);
    l_npaux->setType("TYPE_");
    l_npaux->setType("NLPOS");

    // creation of "[CCONJ]-coordinating conjunction"
    qCDebug(g_cat_silence) << QString("Creating <[NPCNJ]-[CCONJ]-coordinating conjunction> item");
    M1Store::Item_lv2* l_npcnj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[CCONJ]-coordinating conjunction",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPCNJ",
        // icon path
        nullptr);
    l_npcnj->setType("TYPE_");
    l_npcnj->setType("NLPOS");

    // creation of "[DET]-determiner"
    qCDebug(g_cat_silence) << QString("Creating <[NPDET]-[DET]-determiner> item");
    M1Store::Item_lv2* l_npdet = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[DET]-determiner",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPDET",
        // icon path
        nullptr);
    l_npdet->setType("TYPE_");
    l_npdet->setType("NLPOS");

    // creation of "[IND]-indeclinable"
    qCDebug(g_cat_silence) << QString("Creating <[NPIND]-[IND]-indeclinable> item");
    M1Store::Item_lv2* l_npind = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[IND]-indeclinable",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPIND",
        // icon path
        nullptr);
    l_npind->setType("TYPE_");
    l_npind->setType("NLPOS");

    // creation of "[INTJ]-interjection"
    qCDebug(g_cat_silence) << QString("Creating <[NPINT]-[INTJ]-interjection> item");
    M1Store::Item_lv2* l_npint = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[INTJ]-interjection",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPINT",
        // icon path
        nullptr);
    l_npint->setType("TYPE_");
    l_npint->setType("NLPOS");

    // creation of "[NOUN]-noun"
    qCDebug(g_cat_silence) << QString("Creating <[NPNON]-[NOUN]-noun> item");
    M1Store::Item_lv2* l_npnon = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NOUN]-noun",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPNON",
        // icon path
        nullptr);
    l_npnon->setType("TYPE_");
    l_npnon->setType("NLPOS");

    // creation of "[NUM]-numeral"
    qCDebug(g_cat_silence) << QString("Creating <[NPNUM]-[NUM]-numeral> item");
    M1Store::Item_lv2* l_npnum = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NUM]-numeral",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPNUM",
        // icon path
        nullptr);
    l_npnum->setType("TYPE_");
    l_npnum->setType("NLPOS");

    // creation of "[PART]-particle"
    qCDebug(g_cat_silence) << QString("Creating <[NPPRT]-[PART]-particle> item");
    M1Store::Item_lv2* l_npprt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PART]-particle",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPRT",
        // icon path
        nullptr);
    l_npprt->setType("TYPE_");
    l_npprt->setType("NLPOS");

    // creation of "[PRON]-pronoun"
    qCDebug(g_cat_silence) << QString("Creating <[NPPRN]-[PRON]-pronoun> item");
    M1Store::Item_lv2* l_npprn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PRON]-pronoun",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPRN",
        // icon path
        nullptr);
    l_npprn->setType("TYPE_");
    l_npprn->setType("NLPOS");

    // creation of "[PROPN]-proper noun"
    qCDebug(g_cat_silence) << QString("Creating <[NPPPN]-[PROPN]-proper noun> item");
    M1Store::Item_lv2* l_npppn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PROPN]-proper noun",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPPN",
        // icon path
        nullptr);
    l_npppn->setType("TYPE_");
    l_npppn->setType("NLPOS");

    // creation of "[PUNCT]-punctuation"
    qCDebug(g_cat_silence) << QString("Creating <[NPPCT]-[PUNCT]-punctuation> item");
    M1Store::Item_lv2* l_nppct = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PUNCT]-punctuation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPCT",
        // icon path
        nullptr);
    l_nppct->setType("TYPE_");
    l_nppct->setType("NLPOS");

    // creation of "[SCONJ]-subordinating conjunction"
    qCDebug(g_cat_silence) << QString("Creating <[NPSCJ]-[SCONJ]-subordinating conjunction> item");
    M1Store::Item_lv2* l_npscj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[SCONJ]-subordinating conjunction",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPSCJ",
        // icon path
        nullptr);
    l_npscj->setType("TYPE_");
    l_npscj->setType("NLPOS");

    // creation of "[SYM]-symbol"
    qCDebug(g_cat_silence) << QString("Creating <[NPSYM]-[SYM]-symbol> item");
    M1Store::Item_lv2* l_npsym = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[SYM]-symbol",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPSYM",
        // icon path
        nullptr);
    l_npsym->setType("TYPE_");
    l_npsym->setType("NLPOS");

    // creation of "[VERB]-verb"
    qCDebug(g_cat_silence) << QString("Creating <[NPVRB]-[VERB]-verb> item");
    M1Store::Item_lv2* l_npvrb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[VERB]-verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPVRB",
        // icon path
        nullptr);
    l_npvrb->setType("TYPE_");
    l_npvrb->setType("NLPOS");

    // creation of "[X]-other"
    qCDebug(g_cat_silence) << QString("Creating <[NPXXX]-[X]-other> item");
    M1Store::Item_lv2* l_npxxx = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[X]-other",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPXXX",
        // icon path
        nullptr);
    l_npxxx->setType("TYPE_");
    l_npxxx->setType("NLPOS");

    // creation of "[.]-punctuation mark, sentence closer"
    qCDebug(g_cat_silence) << QString("Creating <[NTGSE]-[.]-punctuation mark, sentence closer> item");
    M1Store::Item_lv2* l_ntgse = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[.]-punctuation mark, sentence closer",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGSE",
        // icon path
        nullptr);
    l_ntgse->setType("TYPE_");
    l_ntgse->setType("NLTAG");

    // creation of "[,]-punctuation mark, comma"
    qCDebug(g_cat_silence) << QString("Creating <[NTGCM]-[,]-punctuation mark, comma> item");
    M1Store::Item_lv2* l_ntgcm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[,]-punctuation mark, comma",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGCM",
        // icon path
        nullptr);
    l_ntgcm->setType("TYPE_");
    l_ntgcm->setType("NLTAG");

    // creation of "[-LRB-]-left round bracket"
    qCDebug(g_cat_silence) << QString("Creating <[NTLRB]-[-LRB-]-left round bracket> item");
    M1Store::Item_lv2* l_ntlrb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[-LRB-]-left round bracket",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTLRB",
        // icon path
        nullptr);
    l_ntlrb->setType("TYPE_");
    l_ntlrb->setType("NLTAG");

    // creation of "[-RRB-]-right round bracket"
    qCDebug(g_cat_silence) << QString("Creating <[NTRRB]-[-RRB-]-right round bracket> item");
    M1Store::Item_lv2* l_ntrrb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[-RRB-]-right round bracket",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTRRB",
        // icon path
        nullptr);
    l_ntrrb->setType("TYPE_");
    l_ntrrb->setType("NLTAG");

    // creation of "[``]-opening quotation mark"
    qCDebug(g_cat_silence) << QString("Creating <[NTOQM]-[``]-opening quotation mark> item");
    M1Store::Item_lv2* l_ntoqm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[``]-opening quotation mark",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTOQM",
        // icon path
        nullptr);
    l_ntoqm->setType("TYPE_");
    l_ntoqm->setType("NLTAG");

    // creation of "[""]-closing quotation mark"
    qCDebug(g_cat_silence) << QString("Creating <[NTCQM]-[""]-closing quotation mark> item");
    M1Store::Item_lv2* l_ntcqm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[""]-closing quotation mark",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTCQM",
        // icon path
        nullptr);
    l_ntcqm->setType("TYPE_");
    l_ntcqm->setType("NLTAG");

    // creation of "['']-closing quotation mark"
    qCDebug(g_cat_silence) << QString("Creating <[NTCRM]-['']-closing quotation mark> item");
    M1Store::Item_lv2* l_ntcrm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "['']-closing quotation mark",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTCRM",
        // icon path
        nullptr);
    l_ntcrm->setType("TYPE_");
    l_ntcrm->setType("NLTAG");

    // creation of "[:]-punctuation mark, colon or ellipsis"
    qCDebug(g_cat_silence) << QString("Creating <[NTGCL]-[:]-punctuation mark, colon or ellipsis> item");
    M1Store::Item_lv2* l_ntgcl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[:]-punctuation mark, colon or ellipsis",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGCL",
        // icon path
        nullptr);
    l_ntgcl->setType("TYPE_");
    l_ntgcl->setType("NLTAG");

    // creation of "[$]-symbol, currency"
    qCDebug(g_cat_silence) << QString("Creating <[NTDOL]-[$]-symbol, currency> item");
    M1Store::Item_lv2* l_ntdol = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[$]-symbol, currency",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTDOL",
        // icon path
        nullptr);
    l_ntdol->setType("TYPE_");
    l_ntdol->setType("NLTAG");

    // creation of "[#]-symbol, number sign"
    qCDebug(g_cat_silence) << QString("Creating <[NTSRP]-[#]-symbol, number sign> item");
    M1Store::Item_lv2* l_ntsrp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[#]-symbol, number sign",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTSRP",
        // icon path
        nullptr);
    l_ntsrp->setType("TYPE_");
    l_ntsrp->setType("NLTAG");

    // creation of "[AFX]-affix"
    qCDebug(g_cat_silence) << QString("Creating <[NTAFX]-[AFX]-affix> item");
    M1Store::Item_lv2* l_ntafx = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[AFX]-affix",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTAFX",
        // icon path
        nullptr);
    l_ntafx->setType("TYPE_");
    l_ntafx->setType("NLTAG");

    // creation of "[CC]-conjunction, coordinatinf"
    qCDebug(g_cat_silence) << QString("Creating <[NTGCC]-[CC]-conjunction, coordinatinf> item");
    M1Store::Item_lv2* l_ntgcc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[CC]-conjunction, coordinatinf",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGCC",
        // icon path
        nullptr);
    l_ntgcc->setType("TYPE_");
    l_ntgcc->setType("NLTAG");

    // creation of "[CD]-cardinal number"
    qCDebug(g_cat_silence) << QString("Creating <[NTGCD]-[CD]-cardinal number> item");
    M1Store::Item_lv2* l_ntgcd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[CD]-cardinal number",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGCD",
        // icon path
        nullptr);
    l_ntgcd->setType("TYPE_");
    l_ntgcd->setType("NLTAG");

    // creation of "[DT]-determiner"
    qCDebug(g_cat_silence) << QString("Creating <[NTGDT]-[DT]-determiner> item");
    M1Store::Item_lv2* l_ntgdt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[DT]-determiner",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGDT",
        // icon path
        nullptr);
    l_ntgdt->setType("TYPE_");
    l_ntgdt->setType("NLTAG");

    // creation of "[EX]-existential there"
    qCDebug(g_cat_silence) << QString("Creating <[NTGEX]-[EX]-existential there> item");
    M1Store::Item_lv2* l_ntgex = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[EX]-existential there",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGEX",
        // icon path
        nullptr);
    l_ntgex->setType("TYPE_");
    l_ntgex->setType("NLTAG");

    // creation of "[FW]-foreign word"
    qCDebug(g_cat_silence) << QString("Creating <[NTGFW]-[FW]-foreign word> item");
    M1Store::Item_lv2* l_ntgfw = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[FW]-foreign word",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGFW",
        // icon path
        nullptr);
    l_ntgfw->setType("TYPE_");
    l_ntgfw->setType("NLTAG");

    // creation of "[HYPH]-punctuation mark, hyphen"
    qCDebug(g_cat_silence) << QString("Creating <[NTHYP]-[HYPH]-punctuation mark, hyphen> item");
    M1Store::Item_lv2* l_nthyp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[HYPH]-punctuation mark, hyphen",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTHYP",
        // icon path
        nullptr);
    l_nthyp->setType("TYPE_");
    l_nthyp->setType("NLTAG");

    // creation of "[IN]-conjunction, subordinating or preposition"
    qCDebug(g_cat_silence) << QString("Creating <[NTGIN]-[IN]-conjunction, subordinating or preposition> item");
    M1Store::Item_lv2* l_ntgin = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[IN]-conjunction, subordinating or preposition",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGIN",
        // icon path
        nullptr);
    l_ntgin->setType("TYPE_");
    l_ntgin->setType("NLTAG");

    // creation of "[JJ]-adjective (English), other noun-modifier (Chinese)"
    qCDebug(g_cat_silence) << QString("Creating <[NTGJJ]-[JJ]-adjective (English), other noun-modifier (Chinese)> item");
    M1Store::Item_lv2* l_ntgjj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[JJ]-adjective (English), other noun-modifier (Chinese)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGJJ",
        // icon path
        nullptr);
    l_ntgjj->setType("TYPE_");
    l_ntgjj->setType("NLTAG");

    // creation of "[JJR]-adjective, comparative"
    qCDebug(g_cat_silence) << QString("Creating <[NTJJR]-[JJR]-adjective, comparative> item");
    M1Store::Item_lv2* l_ntjjr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[JJR]-adjective, comparative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTJJR",
        // icon path
        nullptr);
    l_ntjjr->setType("TYPE_");
    l_ntjjr->setType("NLTAG");

    // creation of "[JJS]-adjective, superlative"
    qCDebug(g_cat_silence) << QString("Creating <[NTJJS]-[JJS]-adjective, superlative> item");
    M1Store::Item_lv2* l_ntjjs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[JJS]-adjective, superlative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTJJS",
        // icon path
        nullptr);
    l_ntjjs->setType("TYPE_");
    l_ntjjs->setType("NLTAG");

    // creation of "[LS]-list item marker"
    qCDebug(g_cat_silence) << QString("Creating <[NTGLS]-[LS]-list item marker> item");
    M1Store::Item_lv2* l_ntgls = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[LS]-list item marker",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGLS",
        // icon path
        nullptr);
    l_ntgls->setType("TYPE_");
    l_ntgls->setType("NLTAG");

    // creation of "[MD]-verb, modal auxiliary"
    qCDebug(g_cat_silence) << QString("Creating <[NTGMD]-[MD]-verb, modal auxiliary> item");
    M1Store::Item_lv2* l_ntgmd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[MD]-verb, modal auxiliary",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGMD",
        // icon path
        nullptr);
    l_ntgmd->setType("TYPE_");
    l_ntgmd->setType("NLTAG");

    // creation of "[NIL]-missing taf"
    qCDebug(g_cat_silence) << QString("Creating <[NTNIL]-[NIL]-missing taf> item");
    M1Store::Item_lv2* l_ntnil = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NIL]-missing taf",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTNIL",
        // icon path
        nullptr);
    l_ntnil->setType("TYPE_");
    l_ntnil->setType("NLTAG");

    // creation of "[NN]-noun, singular or mass"
    qCDebug(g_cat_silence) << QString("Creating <[NTGNN]-[NN]-noun, singular or mass> item");
    M1Store::Item_lv2* l_ntgnn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NN]-noun, singular or mass",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGNN",
        // icon path
        nullptr);
    l_ntgnn->setType("TYPE_");
    l_ntgnn->setType("NLTAG");

    // creation of "[NNP]-noun, proper singular"
    qCDebug(g_cat_silence) << QString("Creating <[NTNNP]-[NNP]-noun, proper singular> item");
    M1Store::Item_lv2* l_ntnnp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NNP]-noun, proper singular",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTNNP",
        // icon path
        nullptr);
    l_ntnnp->setType("TYPE_");
    l_ntnnp->setType("NLTAG");

    // creation of "[NNPS]-noun, proper plural"
    qCDebug(g_cat_silence) << QString("Creating <[NTNPP]-[NNPS]-noun, proper plural> item");
    M1Store::Item_lv2* l_ntnpp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NNPS]-noun, proper plural",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTNPP",
        // icon path
        nullptr);
    l_ntnpp->setType("TYPE_");
    l_ntnpp->setType("NLTAG");

    // creation of "[NNS]-noun, plural"
    qCDebug(g_cat_silence) << QString("Creating <[NTNNS]-[NNS]-noun, plural> item");
    M1Store::Item_lv2* l_ntnns = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NNS]-noun, plural",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTNNS",
        // icon path
        nullptr);
    l_ntnns->setType("TYPE_");
    l_ntnns->setType("NLTAG");

    // creation of "[PDT]-predeterminer"
    qCDebug(g_cat_silence) << QString("Creating <[NTPDT]-[PDT]-predeterminer> item");
    M1Store::Item_lv2* l_ntpdt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PDT]-predeterminer",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTPDT",
        // icon path
        nullptr);
    l_ntpdt->setType("TYPE_");
    l_ntpdt->setType("NLTAG");

    // creation of "[POS]-possessive endinf"
    qCDebug(g_cat_silence) << QString("Creating <[NTPOS]-[POS]-possessive endinf> item");
    M1Store::Item_lv2* l_ntpos = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[POS]-possessive endinf",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTPOS",
        // icon path
        nullptr);
    l_ntpos->setType("TYPE_");
    l_ntpos->setType("NLTAG");

    // creation of "[PRP]-pronoun, personal"
    qCDebug(g_cat_silence) << QString("Creating <[NTPRP]-[PRP]-pronoun, personal> item");
    M1Store::Item_lv2* l_ntprp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PRP]-pronoun, personal",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTPRP",
        // icon path
        nullptr);
    l_ntprp->setType("TYPE_");
    l_ntprp->setType("NLTAG");

    // creation of "[PRP$]-pronoun, possessive"
    qCDebug(g_cat_silence) << QString("Creating <[NTPR$]-[PRP$]-pronoun, possessive> item");
    M1Store::Item_lv2* l_ntpr$ = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[PRP$]-pronoun, possessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTPR$",
        // icon path
        nullptr);
    l_ntpr$->setType("TYPE_");
    l_ntpr$->setType("NLTAG");

    // creation of "[RB]-adverb"
    qCDebug(g_cat_silence) << QString("Creating <[NTGRB]-[RB]-adverb> item");
    M1Store::Item_lv2* l_ntgrb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[RB]-adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGRB",
        // icon path
        nullptr);
    l_ntgrb->setType("TYPE_");
    l_ntgrb->setType("NLTAG");

    // creation of "[RBR]-adverb, comparative"
    qCDebug(g_cat_silence) << QString("Creating <[NTRBR]-[RBR]-adverb, comparative> item");
    M1Store::Item_lv2* l_ntrbr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[RBR]-adverb, comparative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTRBR",
        // icon path
        nullptr);
    l_ntrbr->setType("TYPE_");
    l_ntrbr->setType("NLTAG");

    // creation of "[RBS]-adverb, superlative"
    qCDebug(g_cat_silence) << QString("Creating <[NTRBS]-[RBS]-adverb, superlative> item");
    M1Store::Item_lv2* l_ntrbs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[RBS]-adverb, superlative",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTRBS",
        // icon path
        nullptr);
    l_ntrbs->setType("TYPE_");
    l_ntrbs->setType("NLTAG");

    // creation of "[RP]-adverb, particle"
    qCDebug(g_cat_silence) << QString("Creating <[NTGRP]-[RP]-adverb, particle> item");
    M1Store::Item_lv2* l_ntgrp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[RP]-adverb, particle",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGRP",
        // icon path
        nullptr);
    l_ntgrp->setType("TYPE_");
    l_ntgrp->setType("NLTAG");

    // creation of "[TO]-infinitival “to”"
    qCDebug(g_cat_silence) << QString("Creating <[NTGTO]-[TO]-infinitival “to”> item");
    M1Store::Item_lv2* l_ntgto = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[TO]-infinitival “to”",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGTO",
        // icon path
        nullptr);
    l_ntgto->setType("TYPE_");
    l_ntgto->setType("NLTAG");

    // creation of "[UH]-interjection"
    qCDebug(g_cat_silence) << QString("Creating <[NTGUH]-[UH]-interjection> item");
    M1Store::Item_lv2* l_ntguh = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[UH]-interjection",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGUH",
        // icon path
        nullptr);
    l_ntguh->setType("TYPE_");
    l_ntguh->setType("NLTAG");

    // creation of "[VB]-verb, base form"
    qCDebug(g_cat_silence) << QString("Creating <[NTGVB]-[VB]-verb, base form> item");
    M1Store::Item_lv2* l_ntgvb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[VB]-verb, base form",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGVB",
        // icon path
        nullptr);
    l_ntgvb->setType("TYPE_");
    l_ntgvb->setType("NLTAG");

    // creation of "[VBD]-verb, past tense"
    qCDebug(g_cat_silence) << QString("Creating <[NTVBD]-[VBD]-verb, past tense> item");
    M1Store::Item_lv2* l_ntvbd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[VBD]-verb, past tense",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTVBD",
        // icon path
        nullptr);
    l_ntvbd->setType("TYPE_");
    l_ntvbd->setType("NLTAG");

    // creation of "[VBG]-verb, gerund or present participle"
    qCDebug(g_cat_silence) << QString("Creating <[NTVBG]-[VBG]-verb, gerund or present participle> item");
    M1Store::Item_lv2* l_ntvbg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[VBG]-verb, gerund or present participle",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTVBG",
        // icon path
        nullptr);
    l_ntvbg->setType("TYPE_");
    l_ntvbg->setType("NLTAG");

    // creation of "[VBN]-verb, past participle"
    qCDebug(g_cat_silence) << QString("Creating <[NTVBN]-[VBN]-verb, past participle> item");
    M1Store::Item_lv2* l_ntvbn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[VBN]-verb, past participle",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTVBN",
        // icon path
        nullptr);
    l_ntvbn->setType("TYPE_");
    l_ntvbn->setType("NLTAG");

    // creation of "[VBP]-verb, non-3rd person singular present"
    qCDebug(g_cat_silence) << QString("Creating <[NTVBP]-[VBP]-verb, non-3rd person singular present> item");
    M1Store::Item_lv2* l_ntvbp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[VBP]-verb, non-3rd person singular present",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTVBP",
        // icon path
        nullptr);
    l_ntvbp->setType("TYPE_");
    l_ntvbp->setType("NLTAG");

    // creation of "[VBZ]-verb, 3rd person singular present"
    qCDebug(g_cat_silence) << QString("Creating <[NTVBZ]-[VBZ]-verb, 3rd person singular present> item");
    M1Store::Item_lv2* l_ntvbz = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[VBZ]-verb, 3rd person singular present",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTVBZ",
        // icon path
        nullptr);
    l_ntvbz->setType("TYPE_");
    l_ntvbz->setType("NLTAG");

    // creation of "[WDT]-wh-determiner"
    qCDebug(g_cat_silence) << QString("Creating <[NTWDT]-[WDT]-wh-determiner> item");
    M1Store::Item_lv2* l_ntwdt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[WDT]-wh-determiner",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTWDT",
        // icon path
        nullptr);
    l_ntwdt->setType("TYPE_");
    l_ntwdt->setType("NLTAG");

    // creation of "[WP]-wh-pronoun, personal"
    qCDebug(g_cat_silence) << QString("Creating <[NTGWP]-[WP]-wh-pronoun, personal> item");
    M1Store::Item_lv2* l_ntgwp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[WP]-wh-pronoun, personal",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGWP",
        // icon path
        nullptr);
    l_ntgwp->setType("TYPE_");
    l_ntgwp->setType("NLTAG");

    // creation of "[WP$]-wh-pronoun, possessive"
    qCDebug(g_cat_silence) << QString("Creating <[NTWP$]-[WP$]-wh-pronoun, possessive> item");
    M1Store::Item_lv2* l_ntwp$ = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[WP$]-wh-pronoun, possessive",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTWP$",
        // icon path
        nullptr);
    l_ntwp$->setType("TYPE_");
    l_ntwp$->setType("NLTAG");

    // creation of "[WRB]-wh-adverb"
    qCDebug(g_cat_silence) << QString("Creating <[NTWRB]-[WRB]-wh-adverb> item");
    M1Store::Item_lv2* l_ntwrb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[WRB]-wh-adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTWRB",
        // icon path
        nullptr);
    l_ntwrb->setType("TYPE_");
    l_ntwrb->setType("NLTAG");

    // creation of "[SP]-space (English), sentence-final particle (Chinese)"
    qCDebug(g_cat_silence) << QString("Creating <[NTGSP]-[SP]-space (English), sentence-final particle (Chinese)> item");
    M1Store::Item_lv2* l_ntgsp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[SP]-space (English), sentence-final particle (Chinese)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGSP",
        // icon path
        nullptr);
    l_ntgsp->setType("TYPE_");
    l_ntgsp->setType("NLTAG");

    // creation of "[ADD]-email"
    qCDebug(g_cat_silence) << QString("Creating <[NTADD]-[ADD]-email> item");
    M1Store::Item_lv2* l_ntadd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[ADD]-email",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTADD",
        // icon path
        nullptr);
    l_ntadd->setType("TYPE_");
    l_ntadd->setType("NLTAG");

    // creation of "[NFP]-superfluous punctuation"
    qCDebug(g_cat_silence) << QString("Creating <[NTNFP]-[NFP]-superfluous punctuation> item");
    M1Store::Item_lv2* l_ntnfp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NFP]-superfluous punctuation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTNFP",
        // icon path
        nullptr);
    l_ntnfp->setType("TYPE_");
    l_ntnfp->setType("NLTAG");

    // creation of "[GW]-additional word in multi-word expression"
    qCDebug(g_cat_silence) << QString("Creating <[NTGGW]-[GW]-additional word in multi-word expression> item");
    M1Store::Item_lv2* l_ntggw = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[GW]-additional word in multi-word expression",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGGW",
        // icon path
        nullptr);
    l_ntggw->setType("TYPE_");
    l_ntggw->setType("NLTAG");

    // creation of "[XX]-unknown"
    qCDebug(g_cat_silence) << QString("Creating <[NTGXX]-[XX]-unknown> item");
    M1Store::Item_lv2* l_ntgxx = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[XX]-unknown",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTGXX",
        // icon path
        nullptr);
    l_ntgxx->setType("TYPE_");
    l_ntgxx->setType("NLTAG");

    // creation of "[BES]-auxiliary “be”"
    qCDebug(g_cat_silence) << QString("Creating <[NTBES]-[BES]-auxiliary “be”> item");
    M1Store::Item_lv2* l_ntbes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[BES]-auxiliary “be”",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTBES",
        // icon path
        nullptr);
    l_ntbes->setType("TYPE_");
    l_ntbes->setType("NLTAG");

    // creation of "[HVS]-forms of “have”"
    qCDebug(g_cat_silence) << QString("Creating <[NTHVS]-[HVS]-forms of “have”> item");
    M1Store::Item_lv2* l_nthvs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[HVS]-forms of “have”",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTHVS",
        // icon path
        nullptr);
    l_nthvs->setType("TYPE_");
    l_nthvs->setType("NLTAG");

    // creation of "[_SP]-whitespace"
    qCDebug(g_cat_silence) << QString("Creating <[NT_SP]-[_SP]-whitespace> item");
    M1Store::Item_lv2* l_ntsp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[_SP]-whitespace",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NT_SP",
        // icon path
        nullptr);
    l_ntsp->setType("TYPE_");
    l_ntsp->setType("NLTAG");
}

void M1Env::GraphInit::init_plato(){
    
}
