
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
// [OCCUR] Special Vertex ID (Edge type) of occurrence edges
M1Env::SpecialItemID M1Env::OCCUR_SIID = G_NONEX_SI_ID;
// [FORM_] Special Vertex ID (Vertex type) of word form vertices
M1Env::SpecialItemID M1Env::FORM_SIID = G_NONEX_SI_ID;
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
// [PRABH] Special Vertex ID (Vertex type) of Gita translation source: Prabhupada
M1Env::SpecialItemID M1Env::TEXT_WFW_PRABUPADA_SIID = G_NONEX_SI_ID;
// [SIVAN] Special Vertex ID (Vertex type) of Gita translation source: Sivananda
M1Env::SpecialItemID M1Env::TEXT_WFW_SIVANANDA_SIID = G_NONEX_SI_ID;
// [GAMBI] Special Vertex ID (Vertex type) of Gita translation source: Gambirananda
M1Env::SpecialItemID M1Env::TEXT_WFW_GAMBIRANANDA_SIID = G_NONEX_SI_ID;
// [TRSLT] Special Vertex ID (Simple Edge type) of Gita transliteration field
M1Env::SpecialItemID M1Env::TEXT_WORD_TRANSLIT_SIID = G_NONEX_SI_ID;
// [DCTRF] Special Vertex ID (Simple Edge type) of Gita dictionary ref. field
M1Env::SpecialItemID M1Env::TEXT_WORD_DICT_REF_SIID = G_NONEX_SI_ID;
// [WFTRN] Special Vertex ID (Simple Edge type) WfW unit Translation field
M1Env::SpecialItemID M1Env::TEXT_WFW_TRANSL_SIID = G_NONEX_SI_ID;
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
M1Env::SpecialItemID M1Env::PERS_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ORG_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::ME_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::MSG_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::EMAIL_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::WHTSP_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::DSCRD_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::GCHAT_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::SMS_TYPE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::LEMMA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NLENT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NLPOS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NLTAG_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::STEPHANUS_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TXTVR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TXTNT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_CHUNK_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_BOOK_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SENTENCE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_LINE_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WORD_DREF_INRIA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_TRANSLATION_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_SLOKA_BHASHYA_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TEXT_WFW_UNIT_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPADJ_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPADP_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPADV_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPAUX_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPCNJ_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::NPDET_SIID = G_NONEX_SI_ID;
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

void M1Env::GraphInit::set_pseudo_constants(){
    M1Env::FOLDER_SIID = M1Store::Storage::getSpecialID("FOLDR");
    M1Env::AUTO_SIID = M1Store::Storage::getSpecialID("AUTO_");
    M1Env::OWNS_SIID = M1Store::Storage::getSpecialID("OWNS_");
    M1Env::BLNGS_SIID = M1Store::Storage::getSpecialID("BLNGS");
    M1Env::ISA_SIID = M1Store::Storage::getSpecialID("_ISA_");
    M1Env::ITO_SIID = M1Store::Storage::getSpecialID("_ITO_");
    M1Env::TEXT_WROTE_SIID = M1Store::Storage::getSpecialID("WROTE");
    M1Env::TEXT_WRITTEN_BY_SIID = M1Store::Storage::getSpecialID("WRTBY");
    M1Env::OCCUR_SIID = M1Store::Storage::getSpecialID("OCCUR");
    M1Env::FORM_SIID = M1Store::Storage::getSpecialID("FORM_");
    M1Env::CAPTL_SIID = M1Store::Storage::getSpecialID("CAPTL");
    M1Env::PCTLF_SIID = M1Store::Storage::getSpecialID("PCTLF");
    M1Env::PCTRT_SIID = M1Store::Storage::getSpecialID("PCTRT");
    M1Env::MKPLF_SIID = M1Store::Storage::getSpecialID("MKPLF");
    M1Env::MKPRT_SIID = M1Store::Storage::getSpecialID("MKPRT");
    M1Env::STPOS_SIID = M1Store::Storage::getSpecialID("STPOS");
    M1Env::TEXT_WFW_PRABUPADA_SIID = M1Store::Storage::getSpecialID("PRABH");
    M1Env::TEXT_WFW_SIVANANDA_SIID = M1Store::Storage::getSpecialID("SIVAN");
    M1Env::TEXT_WFW_GAMBIRANANDA_SIID = M1Store::Storage::getSpecialID("GAMBI");
    M1Env::TEXT_WORD_TRANSLIT_SIID = M1Store::Storage::getSpecialID("TRSLT");
    M1Env::TEXT_WORD_DICT_REF_SIID = M1Store::Storage::getSpecialID("DCTRF");
    M1Env::TEXT_WFW_TRANSL_SIID = M1Store::Storage::getSpecialID("WFTRN");
    M1Env::TEXT_URL_LINK_SIID = M1Store::Storage::getSpecialID("URLNK");
    M1Env::TW_SECTION_2_OCC_BEGIN_SIID = M1Store::Storage::getSpecialID("HS2OB");
    M1Env::TW_REV_SECTION_2_OCC_BEGIN_SIID = M1Store::Storage::getSpecialID("OC2HB");
    M1Env::TW_SECTION_2_OCC_END_SIID = M1Store::Storage::getSpecialID("HS2OE");
    M1Env::TW_REV_SECTION_2_OCC_END_SIID = M1Store::Storage::getSpecialID("OC2HE");
    M1Env::ROOT_SIID = M1Store::Storage::getSpecialID("ROOT_");
    M1Env::HOME_SIID = M1Store::Storage::getSpecialID("HOME_");
    M1Env::TYPE_NODE_SIID = M1Store::Storage::getSpecialID("TYPE_");
    M1Env::PERS_TYPE_SIID = M1Store::Storage::getSpecialID("PERSN");
    M1Env::ORG_TYPE_SIID = M1Store::Storage::getSpecialID("ORGN_");
    M1Env::ME_SIID = M1Store::Storage::getSpecialID("ME___");
    M1Env::MSG_TYPE_SIID = M1Store::Storage::getSpecialID("_MSG_");
    M1Env::EMAIL_TYPE_SIID = M1Store::Storage::getSpecialID("EMAIL");
    M1Env::WHTSP_TYPE_SIID = M1Store::Storage::getSpecialID("WHTSP");
    M1Env::DSCRD_TYPE_SIID = M1Store::Storage::getSpecialID("DSCRD");
    M1Env::GCHAT_TYPE_SIID = M1Store::Storage::getSpecialID("GCHAT");
    M1Env::SMS_TYPE_SIID = M1Store::Storage::getSpecialID("SMS__");
    M1Env::TEXT_SIID = M1Store::Storage::getSpecialID("TEXT_");
    M1Env::LEMMA_SIID = M1Store::Storage::getSpecialID("LEMMA");
    M1Env::NLENT_SIID = M1Store::Storage::getSpecialID("NLENT");
    M1Env::NLPOS_SIID = M1Store::Storage::getSpecialID("NLPOS");
    M1Env::NLTAG_SIID = M1Store::Storage::getSpecialID("NLTAG");
    M1Env::STEPHANUS_SIID = M1Store::Storage::getSpecialID("STPSC");
    M1Env::TXTVR_SIID = M1Store::Storage::getSpecialID("TXTVR");
    M1Env::TXTNT_SIID = M1Store::Storage::getSpecialID("TXTNT");
    M1Env::TEXT_CHUNK_SIID = M1Store::Storage::getSpecialID("TXTCK");
    M1Env::TEXT_BOOK_SIID = M1Store::Storage::getSpecialID("TXTBK");
    M1Env::TEXT_SENTENCE_SIID = M1Store::Storage::getSpecialID("TXSNT");
    M1Env::TEXT_SLOKA_LINE_SIID = M1Store::Storage::getSpecialID("SLKLN");
    M1Env::TEXT_WORD_DREF_INRIA_SIID = M1Store::Storage::getSpecialID("INRIA");
    M1Env::TEXT_SLOKA_TRANSLATION_SIID = M1Store::Storage::getSpecialID("SLTRN");
    M1Env::TEXT_SLOKA_BHASHYA_SIID = M1Store::Storage::getSpecialID("SLBHS");
    M1Env::TEXT_WFW_UNIT_SIID = M1Store::Storage::getSpecialID("TRLUN");
    M1Env::NPADJ_SIID = M1Store::Storage::getSpecialID("NPADJ");
    M1Env::NPADP_SIID = M1Store::Storage::getSpecialID("NPADP");
    M1Env::NPADV_SIID = M1Store::Storage::getSpecialID("NPADV");
    M1Env::NPAUX_SIID = M1Store::Storage::getSpecialID("NPAUX");
    M1Env::NPCNJ_SIID = M1Store::Storage::getSpecialID("NPCNJ");
    M1Env::NPDET_SIID = M1Store::Storage::getSpecialID("NPDET");
    M1Env::NPINT_SIID = M1Store::Storage::getSpecialID("NPINT");
    M1Env::NPNON_SIID = M1Store::Storage::getSpecialID("NPNON");
    M1Env::NPNUM_SIID = M1Store::Storage::getSpecialID("NPNUM");
    M1Env::NPPRT_SIID = M1Store::Storage::getSpecialID("NPPRT");
    M1Env::NPPRN_SIID = M1Store::Storage::getSpecialID("NPPRN");
    M1Env::NPPPN_SIID = M1Store::Storage::getSpecialID("NPPPN");
    M1Env::NPPCT_SIID = M1Store::Storage::getSpecialID("NPPCT");
    M1Env::NPSCJ_SIID = M1Store::Storage::getSpecialID("NPSCJ");
    M1Env::NPSYM_SIID = M1Store::Storage::getSpecialID("NPSYM");
    M1Env::NPVRB_SIID = M1Store::Storage::getSpecialID("NPVRB");
    M1Env::NPXXX_SIID = M1Store::Storage::getSpecialID("NPXXX");
    M1Env::NTGSE_SIID = M1Store::Storage::getSpecialID("NTGSE");
    M1Env::NTGCM_SIID = M1Store::Storage::getSpecialID("NTGCM");
    M1Env::NTLRB_SIID = M1Store::Storage::getSpecialID("NTLRB");
    M1Env::NTRRB_SIID = M1Store::Storage::getSpecialID("NTRRB");
    M1Env::NTOQM_SIID = M1Store::Storage::getSpecialID("NTOQM");
    M1Env::NTCQM_SIID = M1Store::Storage::getSpecialID("NTCQM");
    M1Env::NTCRM_SIID = M1Store::Storage::getSpecialID("NTCRM");
    M1Env::NTGCL_SIID = M1Store::Storage::getSpecialID("NTGCL");
    M1Env::NTDOL_SIID = M1Store::Storage::getSpecialID("NTDOL");
    M1Env::NTSRP_SIID = M1Store::Storage::getSpecialID("NTSRP");
    M1Env::NTAFX_SIID = M1Store::Storage::getSpecialID("NTAFX");
    M1Env::NTGCC_SIID = M1Store::Storage::getSpecialID("NTGCC");
    M1Env::NTGCD_SIID = M1Store::Storage::getSpecialID("NTGCD");
    M1Env::NTGDT_SIID = M1Store::Storage::getSpecialID("NTGDT");
    M1Env::NTGEX_SIID = M1Store::Storage::getSpecialID("NTGEX");
    M1Env::NTGFW_SIID = M1Store::Storage::getSpecialID("NTGFW");
    M1Env::NTHYP_SIID = M1Store::Storage::getSpecialID("NTHYP");
    M1Env::NTGIN_SIID = M1Store::Storage::getSpecialID("NTGIN");
    M1Env::NTGJJ_SIID = M1Store::Storage::getSpecialID("NTGJJ");
    M1Env::NTJJR_SIID = M1Store::Storage::getSpecialID("NTJJR");
    M1Env::NTJJS_SIID = M1Store::Storage::getSpecialID("NTJJS");
    M1Env::NTGLS_SIID = M1Store::Storage::getSpecialID("NTGLS");
    M1Env::NTGMD_SIID = M1Store::Storage::getSpecialID("NTGMD");
    M1Env::NTNIL_SIID = M1Store::Storage::getSpecialID("NTNIL");
    M1Env::NTGNN_SIID = M1Store::Storage::getSpecialID("NTGNN");
    M1Env::NTNNP_SIID = M1Store::Storage::getSpecialID("NTNNP");
    M1Env::NTNPP_SIID = M1Store::Storage::getSpecialID("NTNPP");
    M1Env::NTNNS_SIID = M1Store::Storage::getSpecialID("NTNNS");
    M1Env::NTPDT_SIID = M1Store::Storage::getSpecialID("NTPDT");
    M1Env::NTPOS_SIID = M1Store::Storage::getSpecialID("NTPOS");
    M1Env::NTPRP_SIID = M1Store::Storage::getSpecialID("NTPRP");
    M1Env::NTPR$_SIID = M1Store::Storage::getSpecialID("NTPR$");
    M1Env::NTGRB_SIID = M1Store::Storage::getSpecialID("NTGRB");
    M1Env::NTRBR_SIID = M1Store::Storage::getSpecialID("NTRBR");
    M1Env::NTRBS_SIID = M1Store::Storage::getSpecialID("NTRBS");
    M1Env::NTGRP_SIID = M1Store::Storage::getSpecialID("NTGRP");
    M1Env::NTGTO_SIID = M1Store::Storage::getSpecialID("NTGTO");
    M1Env::NTGUH_SIID = M1Store::Storage::getSpecialID("NTGUH");
    M1Env::NTGVB_SIID = M1Store::Storage::getSpecialID("NTGVB");
    M1Env::NTVBD_SIID = M1Store::Storage::getSpecialID("NTVBD");
    M1Env::NTVBG_SIID = M1Store::Storage::getSpecialID("NTVBG");
    M1Env::NTVBN_SIID = M1Store::Storage::getSpecialID("NTVBN");
    M1Env::NTVBP_SIID = M1Store::Storage::getSpecialID("NTVBP");
    M1Env::NTVBZ_SIID = M1Store::Storage::getSpecialID("NTVBZ");
    M1Env::NTWDT_SIID = M1Store::Storage::getSpecialID("NTWDT");
    M1Env::NTGWP_SIID = M1Store::Storage::getSpecialID("NTGWP");
    M1Env::NTWP$_SIID = M1Store::Storage::getSpecialID("NTWP$");
    M1Env::NTWRB_SIID = M1Store::Storage::getSpecialID("NTWRB");
    M1Env::NTGSP_SIID = M1Store::Storage::getSpecialID("NTGSP");
    M1Env::NTADD_SIID = M1Store::Storage::getSpecialID("NTADD");
    M1Env::NTNFP_SIID = M1Store::Storage::getSpecialID("NTNFP");
    M1Env::NTGGW_SIID = M1Store::Storage::getSpecialID("NTGGW");
    M1Env::NTGXX_SIID = M1Store::Storage::getSpecialID("NTGXX");
    M1Env::NTBES_SIID = M1Store::Storage::getSpecialID("NTBES");
    M1Env::NTHVS_SIID = M1Store::Storage::getSpecialID("NTHVS");
    M1Env::NT_SP_SIID = M1Store::Storage::getSpecialID("NT_SP");
}

void M1Env::GraphInit::init_base(){
    // Special Vertex ID (Vertex type) of folders (ordinary vertices with no special role)
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "FOLDR", M1Env::FOLDER_ICON_PATH);
    // Special Vertex ID (Edge type) of AUTO edges
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "AUTO_", M1Env::AUTO_ICON_PATH);
    // Special Vertex ID (Edge type) of ownership edges
    M1Store::Storage::getNewSpecialWithReciprocal(SI_IS_TYPE,
        "OWNS_", M1Env::OWNS_ICON_PATH,
        "BLNGS", M1Env::BLNGS_ICON_PATH,
        SI_IS_TYPE | SI_INSERT_AT_TOP);
    // Special Vertex ID (Edge type) of type edges
    M1Store::Storage::getNewSpecialWithReciprocal(SI_IS_TYPE | SI_IS_SPECIAL_EDGE,
        "_ISA_", M1Env::ISA_ICON_PATH,
        "_ITO_", M1Env::ITO_ICON_PATH,
        SI_IS_TYPE);
    // Special Vertex ID (Edge type) wrote / written by
    M1Store::Storage::getNewSpecialWithReciprocal(SI_IS_TYPE,
        "WROTE", M1Env::TEXT_WROTE_ICON_PATH,
        "WRTBY", M1Env::TEXT_WRITTEN_BY_ICON_PATH,
        SI_IS_TYPE);
    // Special Vertex ID (Edge type) of occurrence edges
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE | SI_IS_SPECIAL_EDGE, "OCCUR", M1Env::OCCURRENCE_ICON_PATH);
    // Special Vertex ID (Vertex type) of word form vertices
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE | SI_IS_ICON_TYPE, "FORM_", M1Env::FORM_ICON_PATH);
    // Special Vertex ID (Simple edge type) of Occurrence field capitalization flag
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "CAPTL", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field left punctuation
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "PCTLF", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field right punctuation
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "PCTRT", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field markup left
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "MKPLF", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field markup right
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "MKPRT", nullptr);
    // Special Vertex ID (Simple edge type) of Occurrence field sentence position
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "STPOS", nullptr);
    // Special Vertex ID (Vertex type) of Gita translation source: Prabhupada
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "PRABH", nullptr);
    // Special Vertex ID (Vertex type) of Gita translation source: Sivananda
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "SIVAN", nullptr);
    // Special Vertex ID (Vertex type) of Gita translation source: Gambirananda
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "GAMBI", nullptr);
    // Special Vertex ID (Simple Edge type) of Gita transliteration field
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "TRSLT", nullptr);
    // Special Vertex ID (Simple Edge type) of Gita dictionary ref. field
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "DCTRF", nullptr);
    // Special Vertex ID (Simple Edge type) WfW unit Translation field
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "WFTRN", nullptr);
    // Special Vertex ID (Vertex type) Url link
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "URLNK", M1Env::TEXT_URL_LINK_ICON_PATH);
    // Special Vertex ID (Edge type) begin section + reciprocal
    M1Store::Storage::getNewSpecialWithReciprocal(SI_IS_TYPE,
        "HS2OB", M1Env::TW_SECTION_2_OCC_BEGIN_ICON_PATH,
        "OC2HB", M1Env::TW_SECTION_2_OCC_BEGIN_ICON_PATH,
        SI_IS_TYPE);
    // Special Vertex ID (Edge type) end section + reciprocal
    M1Store::Storage::getNewSpecialWithReciprocal(SI_IS_TYPE,
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

    // creation of "Person (type)"
    qCDebug(g_cat_silence) << QString("Creating <[PERSN]-Person (type)> item");
    M1Store::Item_lv2* l_persn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Person (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PERSN",
        // icon path
        M1Env::PERSON_ICON_PATH);
    l_persn->setType("TYPE_");

    // creation of "Organization (type)"
    qCDebug(g_cat_silence) << QString("Creating <[ORGN_]-Organization (type)> item");
    M1Store::Item_lv2* l_orgn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Organization (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ORGN_",
        // icon path
        M1Env::ORGANIZATION_ICON_PATH);
    l_orgn->setType("TYPE_");

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

    // creation of "Texts Root (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TEXT_]-Texts Root (type)> item");
    M1Store::Item_lv2* l_text = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Texts Root (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TEXT_",
        // icon path
        M1Env::TEXT_ICON_PATH);
    l_text->setType("TYPE_");
    l_text->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_gratt->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_lemma->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_nlent->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_nlpos->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_nltag->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_stpsc->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_txtvr->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_txtnt->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "Text Chunk (type)"
    qCDebug(g_cat_silence) << QString("Creating <[TXTCK]-Text Chunk (type)> item");
    M1Store::Item_lv2* l_txtck = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Text Chunk (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TXTCK",
        // icon path
        M1Env::TEXT_CHUNK_ICON_PATH);
    l_txtck->setType("TYPE_");
    l_txtck->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_txtbk->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_txsnt->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "His Divine Grace A. C. Bhaktivedanta Swami Prabhupada"
    qCDebug(g_cat_silence) << QString("Creating <[PPRAB]-His Divine Grace A. C. Bhaktivedanta Swami Prabhupada> item");
    M1Store::Item_lv2* l_pprab = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "His Divine Grace A. C. Bhaktivedanta Swami Prabhupada",
        // Special Item flag
        0,
        // mnemonic
        "PPRAB",
        // icon path
        nullptr);
    l_pprab->setType("PERSN");
    l_pprab->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "Swami Sri Sivananda Saraswati"
    qCDebug(g_cat_silence) << QString("Creating <[PSIVA]-Swami Sri Sivananda Saraswati> item");
    M1Store::Item_lv2* l_psiva = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Swami Sri Sivananda Saraswati",
        // Special Item flag
        0,
        // mnemonic
        "PSIVA",
        // icon path
        nullptr);
    l_psiva->setType("PERSN");
    l_psiva->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "Swami Gambirananda"
    qCDebug(g_cat_silence) << QString("Creating <[PGAMB]-Swami Gambirananda> item");
    M1Store::Item_lv2* l_pgamb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Swami Gambirananda",
        // Special Item flag
        0,
        // mnemonic
        "PGAMB",
        // icon path
        nullptr);
    l_pgamb->setType("PERSN");
    l_pgamb->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_slkln->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "INRIA Dictionary Reference (type)"
    qCDebug(g_cat_silence) << QString("Creating <[INRIA]-INRIA Dictionary Reference (type)> item");
    M1Store::Item_lv2* l_inria = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "INRIA Dictionary Reference (type)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "INRIA",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_inria->setType("TYPE_");
    l_inria->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_sltrn->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_slbhs->linkTo(l_home, "BLNGS", nullptr, true);

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
    l_trlun->linkTo(l_home, "BLNGS", nullptr, true);

    // creation of "PronType-pronominal type (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[PTRNY]-PronType-pronominal type (Grammar Attr.)> item");
    M1Store::Item_lv2* l_ptrny = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "PronType-pronominal type (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTRNY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptrny->setType("GRATT");

    // creation of "Art-article (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTART]-Art-article (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptart = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Art-article (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTART",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptart->setType("TYPE_");
    l_ptart->setType("PTRNY");

    // creation of "Dem-demonstrative pronoun, determiner, numeral or adverb (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTDEM]-Dem-demonstrative pronoun, determiner, numeral or adverb (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptdem = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Dem-demonstrative pronoun, determiner, numeral or adverb (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTDEM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptdem->setType("TYPE_");
    l_ptdem->setType("PTRNY");

    // creation of "Emp-emphatic determiner (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTEMP]-Emp-emphatic determiner (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptemp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Emp-emphatic determiner (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTEMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptemp->setType("TYPE_");
    l_ptemp->setType("PTRNY");

    // creation of "Exc-exclamative determiner (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTEXC]-Exc-exclamative determiner (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptexc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Exc-exclamative determiner (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTEXC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptexc->setType("TYPE_");
    l_ptexc->setType("PTRNY");

    // creation of "Ind-indefinite pronoun, determiner, numeral or adverb (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTIND]-Ind-indefinite pronoun, determiner, numeral or adverb (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptind = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ind-indefinite pronoun, determiner, numeral or adverb (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTIND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptind->setType("TYPE_");
    l_ptind->setType("PTRNY");

    // creation of "Int-interrogative pronoun, determiner, numeral or adverb (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTINT]-Int-interrogative pronoun, determiner, numeral or adverb (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptint = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Int-interrogative pronoun, determiner, numeral or adverb (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTINT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptint->setType("TYPE_");
    l_ptint->setType("PTRNY");

    // creation of "Neg-negative pronoun, determiner or adverb (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTNEG]-Neg-negative pronoun, determiner or adverb (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptneg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Neg-negative pronoun, determiner or adverb (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTNEG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptneg->setType("TYPE_");
    l_ptneg->setType("PTRNY");

    // creation of "Prs-personal or possessive personal pronoun or determiner (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTPRS]-Prs-personal or possessive personal pronoun or determiner (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptprs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Prs-personal or possessive personal pronoun or determiner (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTPRS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptprs->setType("TYPE_");
    l_ptprs->setType("PTRNY");

    // creation of "Rcp-reciprocal pronoun (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTRCP]-Rcp-reciprocal pronoun (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptrcp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Rcp-reciprocal pronoun (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTRCP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptrcp->setType("TYPE_");
    l_ptrcp->setType("PTRNY");

    // creation of "Rel-relative pronoun, determiner, numeral or adverb (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTREL]-Rel-relative pronoun, determiner, numeral or adverb (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ptrel = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Rel-relative pronoun, determiner, numeral or adverb (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTREL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ptrel->setType("TYPE_");
    l_ptrel->setType("PTRNY");

    // creation of "Tot-total (collective) pronoun, determiner or adverb (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PTTOT]-Tot-total (collective) pronoun, determiner or adverb (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_pttot = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Tot-total (collective) pronoun, determiner or adverb (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PTTOT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_pttot->setType("TYPE_");
    l_pttot->setType("PTRNY");

    // creation of "Gender (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[GENDE]-Gender (Grammar Attr.)> item");
    M1Store::Item_lv2* l_gende = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Gender (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GENDE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gende->setType("GRATT");

    // creation of "Com-common gender (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[GNCOM]-Com-common gender (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_gncom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Com-common gender (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GNCOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gncom->setType("TYPE_");
    l_gncom->setType("GENDE");

    // creation of "Fem-feminine gender (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[GNFEM]-Fem-feminine gender (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_gnfem = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Fem-feminine gender (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GNFEM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gnfem->setType("TYPE_");
    l_gnfem->setType("GENDE");

    // creation of "Masc-masculine gender (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[GNMSC]-Masc-masculine gender (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_gnmsc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Masc-masculine gender (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GNMSC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gnmsc->setType("TYPE_");
    l_gnmsc->setType("GENDE");

    // creation of "Neut-neuter gender (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[GNDNT]-Neut-neuter gender (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_gndnt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Neut-neuter gender (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "GNDNT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_gndnt->setType("TYPE_");
    l_gndnt->setType("GENDE");

    // creation of "VerbForm-form of verb or deverbative (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[VFRBR]-VerbForm-form of verb or deverbative (Grammar Attr.)> item");
    M1Store::Item_lv2* l_vfrbr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "VerbForm-form of verb or deverbative (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFRBR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfrbr->setType("GRATT");

    // creation of "Conv-converb, transgressive, adverbial participle, verbal adve (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFCNV]-Conv-converb, transgressive, adverbial participle, verbal adve (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vfcnv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Conv-converb, transgressive, adverbial participle, verbal adve (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFCNV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfcnv->setType("TYPE_");
    l_vfcnv->setType("VFRBR");

    // creation of "Fin-finite verb (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFFIN]-Fin-finite verb (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vffin = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Fin-finite verb (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFFIN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vffin->setType("TYPE_");
    l_vffin->setType("VFRBR");

    // creation of "Gdv-gerundive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFGDV]-Gdv-gerundive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vfgdv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Gdv-gerundive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFGDV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfgdv->setType("TYPE_");
    l_vfgdv->setType("VFRBR");

    // creation of "Ger-gerund (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFGER]-Ger-gerund (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vfger = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ger-gerund (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFGER",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfger->setType("TYPE_");
    l_vfger->setType("VFRBR");

    // creation of "Inf-infinitive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFINF]-Inf-infinitive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vfinf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Inf-infinitive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFINF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfinf->setType("TYPE_");
    l_vfinf->setType("VFRBR");

    // creation of "Part-participle, verbal adjective (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFPRT]-Part-participle, verbal adjective (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vfprt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Part-participle, verbal adjective (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFPRT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfprt->setType("TYPE_");
    l_vfprt->setType("VFRBR");

    // creation of "Sup-supine (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFSUP]-Sup-supine (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vfsup = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sup-supine (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFSUP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfsup->setType("TYPE_");
    l_vfsup->setType("VFRBR");

    // creation of "Vnoun-verbal noun, masdar (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VFVNN]-Vnoun-verbal noun, masdar (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vfvnn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Vnoun-verbal noun, masdar (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VFVNN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vfvnn->setType("TYPE_");
    l_vfvnn->setType("VFRBR");

    // creation of "NumType-numeral type (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[NTMYP]-NumType-numeral type (Grammar Attr.)> item");
    M1Store::Item_lv2* l_ntmyp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "NumType-numeral type (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTMYP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntmyp->setType("GRATT");

    // creation of "Card-cardinal number or corresponding interrogative / relative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NTCRD]-Card-cardinal number or corresponding interrogative / relative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ntcrd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Card-cardinal number or corresponding interrogative / relative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTCRD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntcrd->setType("TYPE_");
    l_ntcrd->setType("NTMYP");

    // creation of "Dist-distributive numeral (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NTDST]-Dist-distributive numeral (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ntdst = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Dist-distributive numeral (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTDST",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntdst->setType("TYPE_");
    l_ntdst->setType("NTMYP");

    // creation of "Frac-fraction (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NTFRC]-Frac-fraction (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ntfrc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Frac-fraction (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTFRC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntfrc->setType("TYPE_");
    l_ntfrc->setType("NTMYP");

    // creation of "Mult-multiplicative numeral or corresponding interrogative / r (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NTMLT]-Mult-multiplicative numeral or corresponding interrogative / r (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ntmlt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Mult-multiplicative numeral or corresponding interrogative / r (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTMLT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntmlt->setType("TYPE_");
    l_ntmlt->setType("NTMYP");

    // creation of "Ord-ordinal number or corresponding interrogative / relative / (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NTORD]-Ord-ordinal number or corresponding interrogative / relative / (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ntord = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ord-ordinal number or corresponding interrogative / relative / (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTORD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntord->setType("TYPE_");
    l_ntord->setType("NTMYP");

    // creation of "Range-range of values (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NTRNG]-Range-range of values (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ntrng = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Range-range of values (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTRNG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntrng->setType("TYPE_");
    l_ntrng->setType("NTMYP");

    // creation of "Sets-number of sets of things; collective numeral (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NTSTS]-Sets-number of sets of things; collective numeral (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ntsts = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sets-number of sets of things; collective numeral (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NTSTS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ntsts->setType("TYPE_");
    l_ntsts->setType("NTMYP");

    // creation of "Animacy (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[ANMCY]-Animacy (Grammar Attr.)> item");
    M1Store::Item_lv2* l_anmcy = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Animacy (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANMCY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_anmcy->setType("GRATT");

    // creation of "Anim-animate (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ANANM]-Anim-animate (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ananm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Anim-animate (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANANM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ananm->setType("TYPE_");
    l_ananm->setType("ANMCY");

    // creation of "Hum-human (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ANHUM]-Hum-human (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_anhum = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Hum-human (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANHUM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_anhum->setType("TYPE_");
    l_anhum->setType("ANMCY");

    // creation of "Inan-inanimate (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ANINN]-Inan-inanimate (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_aninn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Inan-inanimate (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANINN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_aninn->setType("TYPE_");
    l_aninn->setType("ANMCY");

    // creation of "Nhum-non-human (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ANNHM]-Nhum-non-human (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_annhm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Nhum-non-human (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ANNHM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_annhm->setType("TYPE_");
    l_annhm->setType("ANMCY");

    // creation of "Mood (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[MOOD_]-Mood (Grammar Attr.)> item");
    M1Store::Item_lv2* l_mood = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Mood (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MOOD_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mood->setType("GRATT");

    // creation of "Adm-admirative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDADM]-Adm-admirative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdadm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Adm-admirative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDADM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdadm->setType("TYPE_");
    l_mdadm->setType("MOOD_");

    // creation of "Cnd-conditional (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDCND]-Cnd-conditional (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdcnd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Cnd-conditional (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDCND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdcnd->setType("TYPE_");
    l_mdcnd->setType("MOOD_");

    // creation of "Des-desiderative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDDES]-Des-desiderative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mddes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Des-desiderative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDDES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mddes->setType("TYPE_");
    l_mddes->setType("MOOD_");

    // creation of "Imp-imperative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDIMP]-Imp-imperative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdimp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Imp-imperative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDIMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdimp->setType("TYPE_");
    l_mdimp->setType("MOOD_");

    // creation of "Ind-indicative or realis (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDIND]-Ind-indicative or realis (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdind = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ind-indicative or realis (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDIND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdind->setType("TYPE_");
    l_mdind->setType("MOOD_");

    // creation of "Int-interrogative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDINT]-Int-interrogative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdint = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Int-interrogative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDINT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdint->setType("TYPE_");
    l_mdint->setType("MOOD_");

    // creation of "Irr-irrealis (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDIRR]-Irr-irrealis (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdirr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Irr-irrealis (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDIRR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdirr->setType("TYPE_");
    l_mdirr->setType("MOOD_");

    // creation of "Jus-jussive / injunctive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDJUS]-Jus-jussive / injunctive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdjus = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Jus-jussive / injunctive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDJUS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdjus->setType("TYPE_");
    l_mdjus->setType("MOOD_");

    // creation of "Nec-necessitative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDNEC]-Nec-necessitative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdnec = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Nec-necessitative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDNEC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdnec->setType("TYPE_");
    l_mdnec->setType("MOOD_");

    // creation of "Opt-optative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDOPT]-Opt-optative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdopt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Opt-optative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDOPT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdopt->setType("TYPE_");
    l_mdopt->setType("MOOD_");

    // creation of "Pot-potential (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDPOT]-Pot-potential (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdpot = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pot-potential (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDPOT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdpot->setType("TYPE_");
    l_mdpot->setType("MOOD_");

    // creation of "Prp-purposive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDPRP]-Prp-purposive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdprp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Prp-purposive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDPRP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdprp->setType("TYPE_");
    l_mdprp->setType("MOOD_");

    // creation of "Qot-quotative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDQOT]-Qot-quotative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdqot = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Qot-quotative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDQOT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdqot->setType("TYPE_");
    l_mdqot->setType("MOOD_");

    // creation of "Sub-subjunctive / conjunctive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[MDSUB]-Sub-subjunctive / conjunctive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_mdsub = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sub-subjunctive / conjunctive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "MDSUB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_mdsub->setType("TYPE_");
    l_mdsub->setType("MOOD_");

    // creation of "Poss-possessive (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[POSS_]-Poss-possessive (Grammar Attr.)> item");
    M1Store::Item_lv2* l_poss = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Poss-possessive (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "POSS_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_poss->setType("GRATT");

    // creation of "Yes-it is possessive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PSYES]-Yes-it is possessive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_psyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Yes-it is possessive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PSYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_psyes->setType("TYPE_");
    l_psyes->setType("POSS_");

    // creation of "NounClass-noun class (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[NCNLS]-NounClass-noun class (Grammar Attr.)> item");
    M1Store::Item_lv2* l_ncnls = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "NounClass-noun class (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCNLS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncnls->setType("GRATT");

    // creation of "Bantu1-singular, persons (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN1]-Bantu1-singular, persons (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu1-singular, persons (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn1->setType("TYPE_");
    l_ncbn1->setType("NCNLS");

    // creation of "Bantu2-plural, persons (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN2]-Bantu2-plural, persons (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu2-plural, persons (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn2->setType("TYPE_");
    l_ncbn2->setType("NCNLS");

    // creation of "Bantu3-singular, plants, thin objects (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN3]-Bantu3-singular, plants, thin objects (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn3 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu3-singular, plants, thin objects (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN3",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn3->setType("TYPE_");
    l_ncbn3->setType("NCNLS");

    // creation of "Bantu4-plural, plants, thin objects (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN4]-Bantu4-plural, plants, thin objects (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn4 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu4-plural, plants, thin objects (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN4",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn4->setType("TYPE_");
    l_ncbn4->setType("NCNLS");

    // creation of "Bantu5-singular, fruits, round objects, paired things (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN5]-Bantu5-singular, fruits, round objects, paired things (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn5 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu5-singular, fruits, round objects, paired things (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN5",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn5->setType("TYPE_");
    l_ncbn5->setType("NCNLS");

    // creation of "Bantu6-plural, fruits, round objects, paired things (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN6]-Bantu6-plural, fruits, round objects, paired things (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn6 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu6-plural, fruits, round objects, paired things (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN6",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn6->setType("TYPE_");
    l_ncbn6->setType("NCNLS");

    // creation of "Bantu7-singular, things, diminutives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN7]-Bantu7-singular, things, diminutives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn7 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu7-singular, things, diminutives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN7",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn7->setType("TYPE_");
    l_ncbn7->setType("NCNLS");

    // creation of "Bantu8-plural, things, diminutives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN8]-Bantu8-plural, things, diminutives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn8 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu8-plural, things, diminutives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN8",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn8->setType("TYPE_");
    l_ncbn8->setType("NCNLS");

    // creation of "Bantu9-singular, animals, things (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCBN9]-Bantu9-singular, animals, things (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncbn9 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu9-singular, animals, things (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCBN9",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncbn9->setType("TYPE_");
    l_ncbn9->setType("NCNLS");

    // creation of "Bantu10-plural, animals, things (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB10]-Bantu10-plural, animals, things (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb10 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu10-plural, animals, things (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB10",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb10->setType("TYPE_");
    l_ncb10->setType("NCNLS");

    // creation of "Bantu11-long thin objects, natural phenomena, abstracts (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB11]-Bantu11-long thin objects, natural phenomena, abstracts (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb11 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu11-long thin objects, natural phenomena, abstracts (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB11",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb11->setType("TYPE_");
    l_ncb11->setType("NCNLS");

    // creation of "Bantu12-singular, small things, diminutives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB12]-Bantu12-singular, small things, diminutives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb12 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu12-singular, small things, diminutives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB12",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb12->setType("TYPE_");
    l_ncb12->setType("NCNLS");

    // creation of "Bantu13-plural or mass, small amount of mass (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB13]-Bantu13-plural or mass, small amount of mass (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb13 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu13-plural or mass, small amount of mass (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB13",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb13->setType("TYPE_");
    l_ncb13->setType("NCNLS");

    // creation of "Bantu14-plural, diminutives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB14]-Bantu14-plural, diminutives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb14 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu14-plural, diminutives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB14",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb14->setType("TYPE_");
    l_ncb14->setType("NCNLS");

    // creation of "Bantu15-verbal nouns, infinitives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB15]-Bantu15-verbal nouns, infinitives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb15 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu15-verbal nouns, infinitives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB15",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb15->setType("TYPE_");
    l_ncb15->setType("NCNLS");

    // creation of "Bantu16-definite location, close to something (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB16]-Bantu16-definite location, close to something (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb16 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu16-definite location, close to something (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB16",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb16->setType("TYPE_");
    l_ncb16->setType("NCNLS");

    // creation of "Bantu17-indefinite location, direction, movement (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB17]-Bantu17-indefinite location, direction, movement (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb17 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu17-indefinite location, direction, movement (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB17",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb17->setType("TYPE_");
    l_ncb17->setType("NCNLS");

    // creation of "Bantu18-definite location, inside something (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB18]-Bantu18-definite location, inside something (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb18 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu18-definite location, inside something (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB18",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb18->setType("TYPE_");
    l_ncb18->setType("NCNLS");

    // creation of "Bantu19-little bit of, pejorative plural (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB19]-Bantu19-little bit of, pejorative plural (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb19 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu19-little bit of, pejorative plural (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB19",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb19->setType("TYPE_");
    l_ncb19->setType("NCNLS");

    // creation of "Bantu20-singular, augmentatives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB20]-Bantu20-singular, augmentatives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb20 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu20-singular, augmentatives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB20",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb20->setType("TYPE_");
    l_ncb20->setType("NCNLS");

    // creation of "Bantu21-singular, augmentatives, derogatives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB21]-Bantu21-singular, augmentatives, derogatives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb21 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu21-singular, augmentatives, derogatives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB21",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb21->setType("TYPE_");
    l_ncb21->setType("NCNLS");

    // creation of "Bantu22-plural, augmentatives (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB22]-Bantu22-plural, augmentatives (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb22 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu22-plural, augmentatives (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB22",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb22->setType("TYPE_");
    l_ncb22->setType("NCNLS");

    // creation of "Bantu23-location with place names (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCB23]-Bantu23-location with place names (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncb23 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bantu23-location with place names (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCB23",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncb23->setType("TYPE_");
    l_ncb23->setType("NCNLS");

    // creation of "Wol1-Wolof noun class 1/k (singular human) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL1]-Wol1-Wolof noun class 1/k (singular human) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol1-Wolof noun class 1/k (singular human) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl1->setType("TYPE_");
    l_ncwl1->setType("NCNLS");

    // creation of "Wol2-Wolof noun class 2/ñ (plural human) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL2]-Wol2-Wolof noun class 2/ñ (plural human) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol2-Wolof noun class 2/ñ (plural human) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl2->setType("TYPE_");
    l_ncwl2->setType("NCNLS");

    // creation of "Wol3-Wolof noun class 3/g (singular) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL3]-Wol3-Wolof noun class 3/g (singular) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl3 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol3-Wolof noun class 3/g (singular) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL3",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl3->setType("TYPE_");
    l_ncwl3->setType("NCNLS");

    // creation of "Wol4-Wolof noun class 4/j (singular) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL4]-Wol4-Wolof noun class 4/j (singular) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl4 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol4-Wolof noun class 4/j (singular) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL4",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl4->setType("TYPE_");
    l_ncwl4->setType("NCNLS");

    // creation of "Wol5-Wolof noun class 5/b (singular) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL5]-Wol5-Wolof noun class 5/b (singular) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl5 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol5-Wolof noun class 5/b (singular) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL5",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl5->setType("TYPE_");
    l_ncwl5->setType("NCNLS");

    // creation of "Wol6-Wolof noun class 6/m (singular) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL6]-Wol6-Wolof noun class 6/m (singular) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl6 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol6-Wolof noun class 6/m (singular) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL6",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl6->setType("TYPE_");
    l_ncwl6->setType("NCNLS");

    // creation of "Wol7-Wolof noun class 7/l (singular) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL7]-Wol7-Wolof noun class 7/l (singular) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl7 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol7-Wolof noun class 7/l (singular) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL7",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl7->setType("TYPE_");
    l_ncwl7->setType("NCNLS");

    // creation of "Wol8-Wolof noun class 8/y (plural non-human) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL8]-Wol8-Wolof noun class 8/y (plural non-human) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl8 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol8-Wolof noun class 8/y (plural non-human) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL8",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl8->setType("TYPE_");
    l_ncwl8->setType("NCNLS");

    // creation of "Wol9-Wolof noun class 9/s (singular) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCWL9]-Wol9-Wolof noun class 9/s (singular) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncwl9 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol9-Wolof noun class 9/s (singular) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCWL9",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncwl9->setType("TYPE_");
    l_ncwl9->setType("NCNLS");

    // creation of "Wol10-Wolof noun class 10/w (singular) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCW10]-Wol10-Wolof noun class 10/w (singular) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncw10 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol10-Wolof noun class 10/w (singular) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCW10",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncw10->setType("TYPE_");
    l_ncw10->setType("NCNLS");

    // creation of "Wol11-Wolof noun class 11/f (location) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCW11]-Wol11-Wolof noun class 11/f (location) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncw11 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol11-Wolof noun class 11/f (location) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCW11",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncw11->setType("TYPE_");
    l_ncw11->setType("NCNLS");

    // creation of "Wol12-Wolof noun class 12/n (manner) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NCW12]-Wol12-Wolof noun class 12/n (manner) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ncw12 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Wol12-Wolof noun class 12/n (manner) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NCW12",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ncw12->setType("TYPE_");
    l_ncw12->setType("NCNLS");

    // creation of "Tense (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[TENSE]-Tense (Grammar Attr.)> item");
    M1Store::Item_lv2* l_tense = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Tense (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TENSE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tense->setType("GRATT");

    // creation of "Fut-future tense (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[TNFUT]-Fut-future tense (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_tnfut = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Fut-future tense (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNFUT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnfut->setType("TYPE_");
    l_tnfut->setType("TENSE");

    // creation of "Imp-imperfect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[TNIMP]-Imp-imperfect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_tnimp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Imp-imperfect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNIMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnimp->setType("TYPE_");
    l_tnimp->setType("TENSE");

    // creation of "Past-past tense / preterite / aorist (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[TNPST]-Past-past tense / preterite / aorist (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_tnpst = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Past-past tense / preterite / aorist (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNPST",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnpst->setType("TYPE_");
    l_tnpst->setType("TENSE");

    // creation of "Pqp-pluperfect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[TNPQP]-Pqp-pluperfect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_tnpqp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pqp-pluperfect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNPQP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnpqp->setType("TYPE_");
    l_tnpqp->setType("TENSE");

    // creation of "Pres-present / non-past tense / aorist (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[TNPRS]-Pres-present / non-past tense / aorist (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_tnprs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pres-present / non-past tense / aorist (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TNPRS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tnprs->setType("TYPE_");
    l_tnprs->setType("TENSE");

    // creation of "Reflex-reflexive (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[REFLE]-Reflex-reflexive (Grammar Attr.)> item");
    M1Store::Item_lv2* l_refle = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Reflex-reflexive (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "REFLE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_refle->setType("GRATT");

    // creation of "Yes-it is reflexive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[RFYES]-Yes-it is reflexive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_rfyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Yes-it is reflexive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RFYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_rfyes->setType("TYPE_");
    l_rfyes->setType("REFLE");

    // creation of "Number (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[NUMBE]-Number (Grammar Attr.)> item");
    M1Store::Item_lv2* l_numbe = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Number (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NUMBE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_numbe->setType("GRATT");

    // creation of "Coll-collective / mass / singulare tantum (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMCLL]-Coll-collective / mass / singulare tantum (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmcll = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Coll-collective / mass / singulare tantum (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMCLL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmcll->setType("TYPE_");
    l_nmcll->setType("NUMBE");

    // creation of "Count-count plural (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMCNT]-Count-count plural (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmcnt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Count-count plural (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMCNT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmcnt->setType("TYPE_");
    l_nmcnt->setType("NUMBE");

    // creation of "Dual-dual number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMBDL]-Dual-dual number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmbdl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Dual-dual number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMBDL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmbdl->setType("TYPE_");
    l_nmbdl->setType("NUMBE");

    // creation of "Grpa-greater paucal number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMGRP]-Grpa-greater paucal number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmgrp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Grpa-greater paucal number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMGRP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmgrp->setType("TYPE_");
    l_nmgrp->setType("NUMBE");

    // creation of "Grpl-greater plural number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMGRL]-Grpl-greater plural number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmgrl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Grpl-greater plural number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMGRL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmgrl->setType("TYPE_");
    l_nmgrl->setType("NUMBE");

    // creation of "Inv-inverse number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMINV]-Inv-inverse number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nminv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Inv-inverse number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMINV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nminv->setType("TYPE_");
    l_nminv->setType("NUMBE");

    // creation of "Pauc-paucal number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMBPC]-Pauc-paucal number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmbpc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pauc-paucal number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMBPC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmbpc->setType("TYPE_");
    l_nmbpc->setType("NUMBE");

    // creation of "Plur-plural number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMPLR]-Plur-plural number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmplr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Plur-plural number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMPLR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmplr->setType("TYPE_");
    l_nmplr->setType("NUMBE");

    // creation of "Ptan-plurale tantum (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMPTN]-Ptan-plurale tantum (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmptn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ptan-plurale tantum (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMPTN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmptn->setType("TYPE_");
    l_nmptn->setType("NUMBE");

    // creation of "Sing-singular number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMSNG]-Sing-singular number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmsng = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sing-singular number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMSNG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmsng->setType("TYPE_");
    l_nmsng->setType("NUMBE");

    // creation of "Tri-trial number (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[NMTRI]-Tri-trial number (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_nmtri = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Tri-trial number (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NMTRI",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_nmtri->setType("TYPE_");
    l_nmtri->setType("NUMBE");

    // creation of "Aspect (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[ASPCT]-Aspect (Grammar Attr.)> item");
    M1Store::Item_lv2* l_aspct = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Aspect (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPCT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_aspct->setType("GRATT");

    // creation of "Hab-habitual aspect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ASHAB]-Hab-habitual aspect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_ashab = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Hab-habitual aspect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASHAB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_ashab->setType("TYPE_");
    l_ashab->setType("ASPCT");

    // creation of "Imp-imperfect aspect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ASIMP]-Imp-imperfect aspect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_asimp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Imp-imperfect aspect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASIMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asimp->setType("TYPE_");
    l_asimp->setType("ASPCT");

    // creation of "Iter-iterative / frequentative aspect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ASITR]-Iter-iterative / frequentative aspect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_asitr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Iter-iterative / frequentative aspect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASITR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asitr->setType("TYPE_");
    l_asitr->setType("ASPCT");

    // creation of "Perf-perfect aspect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ASPRF]-Perf-perfect aspect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_asprf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Perf-perfect aspect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPRF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asprf->setType("TYPE_");
    l_asprf->setType("ASPCT");

    // creation of "Prog-progressive aspect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ASPRG]-Prog-progressive aspect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_asprg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Prog-progressive aspect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPRG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asprg->setType("TYPE_");
    l_asprg->setType("ASPCT");

    // creation of "Prosp-prospective aspect (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ASPRS]-Prosp-prospective aspect (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_asprs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Prosp-prospective aspect (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ASPRS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_asprs->setType("TYPE_");
    l_asprs->setType("ASPCT");

    // creation of "Case (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[CASE_]-Case (Grammar Attr.)> item");
    M1Store::Item_lv2* l_case = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Case (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CASE_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_case->setType("GRATT");

    // creation of "Abe-abessive / caritive / privative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSABE]-Abe-abessive / caritive / privative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csabe = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Abe-abessive / caritive / privative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSABE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csabe->setType("TYPE_");
    l_csabe->setType("CASE_");

    // creation of "Ben-benefactive / destinative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSBEN]-Ben-benefactive / destinative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csben = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ben-benefactive / destinative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSBEN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csben->setType("TYPE_");
    l_csben->setType("CASE_");

    // creation of "Cau-causative / motivative / purposive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSCAU]-Cau-causative / motivative / purposive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cscau = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Cau-causative / motivative / purposive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCAU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscau->setType("TYPE_");
    l_cscau->setType("CASE_");

    // creation of "Cmp-comparative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSCMP]-Cmp-comparative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cscmp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Cmp-comparative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscmp->setType("TYPE_");
    l_cscmp->setType("CASE_");

    // creation of "Cns-considerative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSCNS]-Cns-considerative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cscns = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Cns-considerative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCNS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscns->setType("TYPE_");
    l_cscns->setType("CASE_");

    // creation of "Com-comitative / associative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSCOM]-Com-comitative / associative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cscom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Com-comitative / associative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSCOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cscom->setType("TYPE_");
    l_cscom->setType("CASE_");

    // creation of "Dat-dative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSDAT]-Dat-dative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csdat = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Dat-dative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSDAT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csdat->setType("TYPE_");
    l_csdat->setType("CASE_");

    // creation of "Dis-distributive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSDIS]-Dis-distributive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csdis = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Dis-distributive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSDIS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csdis->setType("TYPE_");
    l_csdis->setType("CASE_");

    // creation of "Equ-equative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSEQU]-Equ-equative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csequ = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Equ-equative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSEQU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csequ->setType("TYPE_");
    l_csequ->setType("CASE_");

    // creation of "Gen-genitive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSGEN]-Gen-genitive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csgen = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Gen-genitive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSGEN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csgen->setType("TYPE_");
    l_csgen->setType("CASE_");

    // creation of "Ins-instrumental / instructive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSINS]-Ins-instrumental / instructive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csins = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ins-instrumental / instructive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSINS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csins->setType("TYPE_");
    l_csins->setType("CASE_");

    // creation of "Par-partitive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSPAR]-Par-partitive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cspar = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Par-partitive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSPAR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cspar->setType("TYPE_");
    l_cspar->setType("CASE_");

    // creation of "Tem-temporal (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSTEM]-Tem-temporal (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cstem = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Tem-temporal (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSTEM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cstem->setType("TYPE_");
    l_cstem->setType("CASE_");

    // creation of "Abl-ablative / adelative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSABL]-Abl-ablative / adelative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csabl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Abl-ablative / adelative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSABL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csabl->setType("TYPE_");
    l_csabl->setType("CASE_");

    // creation of "Add-additive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSADD]-Add-additive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csadd = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Add-additive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSADD",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csadd->setType("TYPE_");
    l_csadd->setType("CASE_");

    // creation of "Ade-adessive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSADE]-Ade-adessive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csade = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ade-adessive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSADE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csade->setType("TYPE_");
    l_csade->setType("CASE_");

    // creation of "All-allative / adlative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSALL]-All-allative / adlative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csall = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "All-allative / adlative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSALL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csall->setType("TYPE_");
    l_csall->setType("CASE_");

    // creation of "Del-delative / superelative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSDEL]-Del-delative / superelative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csdel = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Del-delative / superelative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSDEL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csdel->setType("TYPE_");
    l_csdel->setType("CASE_");

    // creation of "Ela-elative / inelative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSELA]-Ela-elative / inelative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csela = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ela-elative / inelative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSELA",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csela->setType("TYPE_");
    l_csela->setType("CASE_");

    // creation of "Ess-essive / prolative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSESS]-Ess-essive / prolative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csess = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ess-essive / prolative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSESS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csess->setType("TYPE_");
    l_csess->setType("CASE_");

    // creation of "Ill-illative / inlative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSILL]-Ill-illative / inlative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csill = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ill-illative / inlative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSILL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csill->setType("TYPE_");
    l_csill->setType("CASE_");

    // creation of "Ine-inessive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSINE]-Ine-inessive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csine = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ine-inessive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSINE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csine->setType("TYPE_");
    l_csine->setType("CASE_");

    // creation of "Lat-lative / directional allative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSLAT]-Lat-lative / directional allative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cslat = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Lat-lative / directional allative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSLAT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cslat->setType("TYPE_");
    l_cslat->setType("CASE_");

    // creation of "Loc-locative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSLOC]-Loc-locative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csloc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Loc-locative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSLOC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csloc->setType("TYPE_");
    l_csloc->setType("CASE_");

    // creation of "Per-perlative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSPER]-Per-perlative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csper = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Per-perlative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSPER",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csper->setType("TYPE_");
    l_csper->setType("CASE_");

    // creation of "Sbe-subelative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSSBE]-Sbe-subelative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cssbe = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sbe-subelative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSBE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssbe->setType("TYPE_");
    l_cssbe->setType("CASE_");

    // creation of "Sbl-sublative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSSBL]-Sbl-sublative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cssbl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sbl-sublative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSBL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssbl->setType("TYPE_");
    l_cssbl->setType("CASE_");

    // creation of "Spl-superlative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSSPL]-Spl-superlative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_csspl = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Spl-superlative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSPL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_csspl->setType("TYPE_");
    l_csspl->setType("CASE_");

    // creation of "Sub-subessive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSSUB]-Sub-subessive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cssub = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sub-subessive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSUB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssub->setType("TYPE_");
    l_cssub->setType("CASE_");

    // creation of "Sup-superessive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSSUP]-Sup-superessive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cssup = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sup-superessive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSSUP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cssup->setType("TYPE_");
    l_cssup->setType("CASE_");

    // creation of "Ter-terminative / terminal allative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CSTER]-Ter-terminative / terminal allative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cster = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ter-terminative / terminal allative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CSTER",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cster->setType("TYPE_");
    l_cster->setType("CASE_");

    // creation of "Voice (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[VOICE]-Voice (Grammar Attr.)> item");
    M1Store::Item_lv2* l_voice = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Voice (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VOICE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_voice->setType("GRATT");

    // creation of "Act-active or actor-focus voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCACT]-Act-active or actor-focus voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcact = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Act-active or actor-focus voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCACT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcact->setType("TYPE_");
    l_vcact->setType("VOICE");

    // creation of "Antip-antipassive voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCANT]-Antip-antipassive voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcant = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Antip-antipassive voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCANT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcant->setType("TYPE_");
    l_vcant->setType("VOICE");

    // creation of "Bfoc-beneficiary-focus voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCBFC]-Bfoc-beneficiary-focus voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcbfc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bfoc-beneficiary-focus voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCBFC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcbfc->setType("TYPE_");
    l_vcbfc->setType("VOICE");

    // creation of "Cau-causative voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCCAU]-Cau-causative voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vccau = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Cau-causative voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCCAU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vccau->setType("TYPE_");
    l_vccau->setType("VOICE");

    // creation of "Dir-direct voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCDIR]-Dir-direct voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcdir = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Dir-direct voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCDIR",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcdir->setType("TYPE_");
    l_vcdir->setType("VOICE");

    // creation of "Inv-inverse voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCINV]-Inv-inverse voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcinv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Inv-inverse voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCINV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcinv->setType("TYPE_");
    l_vcinv->setType("VOICE");

    // creation of "Lfoc-location-focus voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCLFC]-Lfoc-location-focus voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vclfc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Lfoc-location-focus voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCLFC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vclfc->setType("TYPE_");
    l_vclfc->setType("VOICE");

    // creation of "Mid-middle voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCMID]-Mid-middle voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcmid = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Mid-middle voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCMID",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcmid->setType("TYPE_");
    l_vcmid->setType("VOICE");

    // creation of "Pass-passive or patient-focus voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCPSS]-Pass-passive or patient-focus voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcpss = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pass-passive or patient-focus voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCPSS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcpss->setType("TYPE_");
    l_vcpss->setType("VOICE");

    // creation of "Rcp-reciprocal voice (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[VCRCP]-Rcp-reciprocal voice (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_vcrcp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Rcp-reciprocal voice (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "VCRCP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_vcrcp->setType("TYPE_");
    l_vcrcp->setType("VOICE");

    // creation of "Abbr-abbreviation (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[ABBR_]-Abbr-abbreviation (Grammar Attr.)> item");
    M1Store::Item_lv2* l_abbr = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Abbr-abbreviation (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ABBR_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_abbr->setType("GRATT");

    // creation of "Yes-it is abbreviation (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[ABYES]-Yes-it is abbreviation (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_abyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Yes-it is abbreviation (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "ABYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_abyes->setType("TYPE_");
    l_abyes->setType("ABBR_");

    // creation of "Definite-definiteness or state (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[DEFIN]-Definite-definiteness or state (Grammar Attr.)> item");
    M1Store::Item_lv2* l_defin = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Definite-definiteness or state (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DEFIN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_defin->setType("GRATT");

    // creation of "Com-complex (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DFCOM]-Com-complex (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dfcom = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Com-complex (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFCOM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfcom->setType("TYPE_");
    l_dfcom->setType("DEFIN");

    // creation of "Cons-construct state / reduced definiteness (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DFCNS]-Cons-construct state / reduced definiteness (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dfcns = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Cons-construct state / reduced definiteness (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFCNS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfcns->setType("TYPE_");
    l_dfcns->setType("DEFIN");

    // creation of "Def-definite (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DFDEF]-Def-definite (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dfdef = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Def-definite (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFDEF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfdef->setType("TYPE_");
    l_dfdef->setType("DEFIN");

    // creation of "Ind-indefinite (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DFIND]-Ind-indefinite (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dfind = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ind-indefinite (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFIND",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfind->setType("TYPE_");
    l_dfind->setType("DEFIN");

    // creation of "Spec-specific indefinite (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DFSPC]-Spec-specific indefinite (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dfspc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Spec-specific indefinite (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DFSPC",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dfspc->setType("TYPE_");
    l_dfspc->setType("DEFIN");

    // creation of "Evident-evidentiality (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[EVDNT]-Evident-evidentiality (Grammar Attr.)> item");
    M1Store::Item_lv2* l_evdnt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Evident-evidentiality (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EVDNT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_evdnt->setType("GRATT");

    // creation of "Fh-firsthand (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EVDFH]-Fh-firsthand (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_evdfh = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Fh-firsthand (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EVDFH",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_evdfh->setType("TYPE_");
    l_evdfh->setType("EVDNT");

    // creation of "Nfh-non-firsthand (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EVNFH]-Nfh-non-firsthand (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_evnfh = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Nfh-non-firsthand (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EVNFH",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_evnfh->setType("TYPE_");
    l_evnfh->setType("EVDNT");

    // creation of "Typo-is this a misspelled word? (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[TYPO_]-Typo-is this a misspelled word? (Grammar Attr.)> item");
    M1Store::Item_lv2* l_typo = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Typo-is this a misspelled word? (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TYPO_",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_typo->setType("GRATT");

    // creation of "Yes-it is typo (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[TYYES]-Yes-it is typo (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_tyyes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Yes-it is typo (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "TYYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_tyyes->setType("TYPE_");
    l_tyyes->setType("TYPO_");

    // creation of "Deixis-relative location encoded in demonstratives (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[DEIXI]-Deixis-relative location encoded in demonstratives (Grammar Attr.)> item");
    M1Store::Item_lv2* l_deixi = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Deixis-relative location encoded in demonstratives (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DEIXI",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_deixi->setType("GRATT");

    // creation of "Abv-above the reference point (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DXABV]-Abv-above the reference point (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dxabv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Abv-above the reference point (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXABV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxabv->setType("TYPE_");
    l_dxabv->setType("DEIXI");

    // creation of "Bel-below the reference point (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DXBEL]-Bel-below the reference point (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dxbel = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Bel-below the reference point (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXBEL",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxbel->setType("TYPE_");
    l_dxbel->setType("DEIXI");

    // creation of "Even-at the same level as the reference point (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DXEVN]-Even-at the same level as the reference point (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dxevn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Even-at the same level as the reference point (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXEVN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxevn->setType("TYPE_");
    l_dxevn->setType("DEIXI");

    // creation of "Med-medial (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DXMED]-Med-medial (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dxmed = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Med-medial (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXMED",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxmed->setType("TYPE_");
    l_dxmed->setType("DEIXI");

    // creation of "Nvis-not visible (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DXNVS]-Nvis-not visible (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dxnvs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Nvis-not visible (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXNVS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxnvs->setType("TYPE_");
    l_dxnvs->setType("DEIXI");

    // creation of "Prox-proximate (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DXPRX]-Prox-proximate (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dxprx = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Prox-proximate (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXPRX",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxprx->setType("TYPE_");
    l_dxprx->setType("DEIXI");

    // creation of "Remt-remote, distal (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DXRMT]-Remt-remote, distal (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dxrmt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Remt-remote, distal (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DXRMT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dxrmt->setType("TYPE_");
    l_dxrmt->setType("DEIXI");

    // creation of "Polarity (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[PLRTY]-Polarity (Grammar Attr.)> item");
    M1Store::Item_lv2* l_plrty = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Polarity (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLRTY",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plrty->setType("GRATT");

    // creation of "Neg-negative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PLNEG]-Neg-negative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_plneg = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Neg-negative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLNEG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plneg->setType("TYPE_");
    l_plneg->setType("PLRTY");

    // creation of "Pos-positive, affirmative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PLPOS]-Pos-positive, affirmative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_plpos = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pos-positive, affirmative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLPOS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plpos->setType("TYPE_");
    l_plpos->setType("PLRTY");

    // creation of "Foreign-is this a foreign word? (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[FOREI]-Foreign-is this a foreign word? (Grammar Attr.)> item");
    M1Store::Item_lv2* l_forei = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Foreign-is this a foreign word? (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "FOREI",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_forei->setType("GRATT");

    // creation of "Yes-it is foreign (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[FRYES]-Yes-it is foreign (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_fryes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Yes-it is foreign (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "FRYES",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_fryes->setType("TYPE_");
    l_fryes->setType("FOREI");

    // creation of "DeixisRef-person to which deixis is relative (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[DRXSF]-DeixisRef-person to which deixis is relative (Grammar Attr.)> item");
    M1Store::Item_lv2* l_drxsf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "DeixisRef-person to which deixis is relative (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DRXSF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_drxsf->setType("GRATT");

    // creation of "1-deixis relative to the first person participant (speaker) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DRXS1]-1-deixis relative to the first person participant (speaker) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_drxs1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "1-deixis relative to the first person participant (speaker) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DRXS1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_drxs1->setType("TYPE_");
    l_drxs1->setType("DRXSF");

    // creation of "2-deixis relative to the second person participant (hearer) (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DRXS2]-2-deixis relative to the second person participant (hearer) (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_drxs2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "2-deixis relative to the second person participant (hearer) (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DRXS2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_drxs2->setType("TYPE_");
    l_drxs2->setType("DRXSF");

    // creation of "Person (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[PERSO]-Person (Grammar Attr.)> item");
    M1Store::Item_lv2* l_perso = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Person (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PERSO",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_perso->setType("GRATT");

    // creation of "0-zero person (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN0]-0-zero person (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_prsn0 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "0-zero person (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN0",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn0->setType("TYPE_");
    l_prsn0->setType("PERSO");

    // creation of "1-first person (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN1]-1-first person (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_prsn1 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "1-first person (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN1",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn1->setType("TYPE_");
    l_prsn1->setType("PERSO");

    // creation of "2-second person (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN2]-2-second person (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_prsn2 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "2-second person (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN2",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn2->setType("TYPE_");
    l_prsn2->setType("PERSO");

    // creation of "3-third person (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN3]-3-third person (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_prsn3 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "3-third person (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN3",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn3->setType("TYPE_");
    l_prsn3->setType("PERSO");

    // creation of "4-fourth person (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PRSN4]-4-fourth person (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_prsn4 = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "4-fourth person (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PRSN4",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_prsn4->setType("TYPE_");
    l_prsn4->setType("PERSO");

    // creation of "ExtPos-external part of speech (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[EPXTS]-ExtPos-external part of speech (Grammar Attr.)> item");
    M1Store::Item_lv2* l_epxts = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "ExtPos-external part of speech (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPXTS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epxts->setType("GRATT");

    // creation of "ADJ-adjective-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EPADJ]-ADJ-adjective-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_epadj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "ADJ-adjective-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPADJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epadj->setType("TYPE_");
    l_epadj->setType("EPXTS");

    // creation of "ADP-adposition-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EPADP]-ADP-adposition-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_epadp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "ADP-adposition-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPADP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epadp->setType("TYPE_");
    l_epadp->setType("EPXTS");

    // creation of "ADV-adverb-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EPADV]-ADV-adverb-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_epadv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "ADV-adverb-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPADV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epadv->setType("TYPE_");
    l_epadv->setType("EPXTS");

    // creation of "AUX-auxiliary-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EPAUX]-AUX-auxiliary-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_epaux = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "AUX-auxiliary-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPAUX",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epaux->setType("TYPE_");
    l_epaux->setType("EPXTS");

    // creation of "CCONJ-coordinating conjunction-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CCONJ]-CCONJ-coordinating conjunction-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_cconj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "CCONJ-coordinating conjunction-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CCONJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_cconj->setType("TYPE_");
    l_cconj->setType("EPXTS");

    // creation of "DET-determiner-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EPDET]-DET-determiner-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_epdet = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "DET-determiner-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPDET",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epdet->setType("TYPE_");
    l_epdet->setType("EPXTS");

    // creation of "INTJ-interjection-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EINTJ]-INTJ-interjection-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_eintj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "INTJ-interjection-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EINTJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_eintj->setType("TYPE_");
    l_eintj->setType("EPXTS");

    // creation of "PRON-pronoun-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[EPRON]-PRON-pronoun-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_epron = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "PRON-pronoun-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "EPRON",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_epron->setType("TYPE_");
    l_epron->setType("EPXTS");

    // creation of "PROPN-proper noun-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PROPN]-PROPN-proper noun-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_propn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "PROPN-proper noun-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PROPN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_propn->setType("TYPE_");
    l_propn->setType("EPXTS");

    // creation of "SCONJ-subordinator-like expression (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[SCONJ]-SCONJ-subordinator-like expression (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_sconj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "SCONJ-subordinator-like expression (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "SCONJ",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_sconj->setType("TYPE_");
    l_sconj->setType("EPXTS");

    // creation of "Degree (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[DEGRE]-Degree (Grammar Attr.)> item");
    M1Store::Item_lv2* l_degre = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Degree (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DEGRE",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_degre->setType("GRATT");

    // creation of "Abs-absolute superlative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DGABS]-Abs-absolute superlative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dgabs = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Abs-absolute superlative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGABS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgabs->setType("TYPE_");
    l_dgabs->setType("DEGRE");

    // creation of "Aug-augmentative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DGAUG]-Aug-augmentative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dgaug = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Aug-augmentative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGAUG",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgaug->setType("TYPE_");
    l_dgaug->setType("DEGRE");

    // creation of "Cmp-comparative, second degree (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DGCMP]-Cmp-comparative, second degree (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dgcmp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Cmp-comparative, second degree (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGCMP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgcmp->setType("TYPE_");
    l_dgcmp->setType("DEGRE");

    // creation of "Dim-diminutive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DGDIM]-Dim-diminutive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dgdim = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Dim-diminutive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGDIM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgdim->setType("TYPE_");
    l_dgdim->setType("DEGRE");

    // creation of "Equ-equative (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DGEQU]-Equ-equative (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dgequ = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Equ-equative (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGEQU",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgequ->setType("TYPE_");
    l_dgequ->setType("DEGRE");

    // creation of "Pos-positive, first degree (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DGPOS]-Pos-positive, first degree (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dgpos = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Pos-positive, first degree (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGPOS",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgpos->setType("TYPE_");
    l_dgpos->setType("DEGRE");

    // creation of "Sup-superlative, third degree (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[DGSUP]-Sup-superlative, third degree (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_dgsup = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Sup-superlative, third degree (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "DGSUP",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_dgsup->setType("TYPE_");
    l_dgsup->setType("DEGRE");

    // creation of "Polite-politeness (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[POLIT]-Polite-politeness (Grammar Attr.)> item");
    M1Store::Item_lv2* l_polit = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Polite-politeness (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "POLIT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_polit->setType("GRATT");

    // creation of "Elev-referent elevating (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PLELV]-Elev-referent elevating (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_plelv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Elev-referent elevating (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLELV",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plelv->setType("TYPE_");
    l_plelv->setType("POLIT");

    // creation of "Form-formal register (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PLFRM]-Form-formal register (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_plfrm = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Form-formal register (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLFRM",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plfrm->setType("TYPE_");
    l_plfrm->setType("POLIT");

    // creation of "Humb-speaker humbling (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PLHMB]-Humb-speaker humbling (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_plhmb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Humb-speaker humbling (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLHMB",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plhmb->setType("TYPE_");
    l_plhmb->setType("POLIT");

    // creation of "Infm-informal register (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[PLINF]-Infm-informal register (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_plinf = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Infm-informal register (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "PLINF",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_plinf->setType("TYPE_");
    l_plinf->setType("POLIT");

    // creation of "Clusivity (Grammar Attr.)"
    qCDebug(g_cat_silence) << QString("Creating <[CLSVT]-Clusivity (Grammar Attr.)> item");
    M1Store::Item_lv2* l_clsvt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Clusivity (Grammar Attr.)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CLSVT",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_clsvt->setType("GRATT");

    // creation of "Ex-exclusive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CLSEX]-Ex-exclusive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_clsex = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "Ex-exclusive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CLSEX",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_clsex->setType("TYPE_");
    l_clsex->setType("CLSVT");

    // creation of "In-inclusive (Gr. Attr. Value)"
    qCDebug(g_cat_silence) << QString("Creating <[CLSIN]-In-inclusive (Gr. Attr. Value)> item");
    M1Store::Item_lv2* l_clsin = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "In-inclusive (Gr. Attr. Value)",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "CLSIN",
        // icon path
        M1Env::FOLDER_ICON_PATH);
    l_clsin->setType("TYPE_");
    l_clsin->setType("CLSVT");

    // creation of "ADJ-adjective"
    qCDebug(g_cat_silence) << QString("Creating <[NPADJ]-ADJ-adjective> item");
    M1Store::Item_lv2* l_npadj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "ADJ-adjective",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPADJ",
        // icon path
        nullptr);
    l_npadj->setType("TYPE_");
    l_npadj->setType("NLPOS");

    // creation of "ADP-adposition"
    qCDebug(g_cat_silence) << QString("Creating <[NPADP]-ADP-adposition> item");
    M1Store::Item_lv2* l_npadp = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "ADP-adposition",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPADP",
        // icon path
        nullptr);
    l_npadp->setType("TYPE_");
    l_npadp->setType("NLPOS");

    // creation of "ADV-adverb"
    qCDebug(g_cat_silence) << QString("Creating <[NPADV]-ADV-adverb> item");
    M1Store::Item_lv2* l_npadv = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "ADV-adverb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPADV",
        // icon path
        nullptr);
    l_npadv->setType("TYPE_");
    l_npadv->setType("NLPOS");

    // creation of "AUX-auxiliary"
    qCDebug(g_cat_silence) << QString("Creating <[NPAUX]-AUX-auxiliary> item");
    M1Store::Item_lv2* l_npaux = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "AUX-auxiliary",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPAUX",
        // icon path
        nullptr);
    l_npaux->setType("TYPE_");
    l_npaux->setType("NLPOS");

    // creation of "CCONJ-coordinating conjunction"
    qCDebug(g_cat_silence) << QString("Creating <[NPCNJ]-CCONJ-coordinating conjunction> item");
    M1Store::Item_lv2* l_npcnj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "CCONJ-coordinating conjunction",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPCNJ",
        // icon path
        nullptr);
    l_npcnj->setType("TYPE_");
    l_npcnj->setType("NLPOS");

    // creation of "DET-determiner"
    qCDebug(g_cat_silence) << QString("Creating <[NPDET]-DET-determiner> item");
    M1Store::Item_lv2* l_npdet = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "DET-determiner",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPDET",
        // icon path
        nullptr);
    l_npdet->setType("TYPE_");
    l_npdet->setType("NLPOS");

    // creation of "INTJ-interjection"
    qCDebug(g_cat_silence) << QString("Creating <[NPINT]-INTJ-interjection> item");
    M1Store::Item_lv2* l_npint = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "INTJ-interjection",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPINT",
        // icon path
        nullptr);
    l_npint->setType("TYPE_");
    l_npint->setType("NLPOS");

    // creation of "NOUN-noun"
    qCDebug(g_cat_silence) << QString("Creating <[NPNON]-NOUN-noun> item");
    M1Store::Item_lv2* l_npnon = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "NOUN-noun",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPNON",
        // icon path
        nullptr);
    l_npnon->setType("TYPE_");
    l_npnon->setType("NLPOS");

    // creation of "NUM-numeral"
    qCDebug(g_cat_silence) << QString("Creating <[NPNUM]-NUM-numeral> item");
    M1Store::Item_lv2* l_npnum = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "NUM-numeral",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPNUM",
        // icon path
        nullptr);
    l_npnum->setType("TYPE_");
    l_npnum->setType("NLPOS");

    // creation of "PART-particle"
    qCDebug(g_cat_silence) << QString("Creating <[NPPRT]-PART-particle> item");
    M1Store::Item_lv2* l_npprt = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "PART-particle",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPRT",
        // icon path
        nullptr);
    l_npprt->setType("TYPE_");
    l_npprt->setType("NLPOS");

    // creation of "PRON-pronoun"
    qCDebug(g_cat_silence) << QString("Creating <[NPPRN]-PRON-pronoun> item");
    M1Store::Item_lv2* l_npprn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "PRON-pronoun",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPRN",
        // icon path
        nullptr);
    l_npprn->setType("TYPE_");
    l_npprn->setType("NLPOS");

    // creation of "PROPN-proper noun"
    qCDebug(g_cat_silence) << QString("Creating <[NPPPN]-PROPN-proper noun> item");
    M1Store::Item_lv2* l_npppn = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "PROPN-proper noun",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPPN",
        // icon path
        nullptr);
    l_npppn->setType("TYPE_");
    l_npppn->setType("NLPOS");

    // creation of "PUNCT-punctuation"
    qCDebug(g_cat_silence) << QString("Creating <[NPPCT]-PUNCT-punctuation> item");
    M1Store::Item_lv2* l_nppct = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "PUNCT-punctuation",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPPCT",
        // icon path
        nullptr);
    l_nppct->setType("TYPE_");
    l_nppct->setType("NLPOS");

    // creation of "SCONJ-subordinating conjunction"
    qCDebug(g_cat_silence) << QString("Creating <[NPSCJ]-SCONJ-subordinating conjunction> item");
    M1Store::Item_lv2* l_npscj = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "SCONJ-subordinating conjunction",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPSCJ",
        // icon path
        nullptr);
    l_npscj->setType("TYPE_");
    l_npscj->setType("NLPOS");

    // creation of "SYM-symbol"
    qCDebug(g_cat_silence) << QString("Creating <[NPSYM]-SYM-symbol> item");
    M1Store::Item_lv2* l_npsym = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "SYM-symbol",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPSYM",
        // icon path
        nullptr);
    l_npsym->setType("TYPE_");
    l_npsym->setType("NLPOS");

    // creation of "VERB-verb"
    qCDebug(g_cat_silence) << QString("Creating <[NPVRB]-VERB-verb> item");
    M1Store::Item_lv2* l_npvrb = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "VERB-verb",
        // Special Item flag
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "NPVRB",
        // icon path
        nullptr);
    l_npvrb->setType("TYPE_");
    l_npvrb->setType("NLPOS");

    // creation of "X-other"
    qCDebug(g_cat_silence) << QString("Creating <[NPXXX]-X-other> item");
    M1Store::Item_lv2* l_npxxx = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "X-other",
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

    // creation of "[CC]-conjunction, coordinating"
    qCDebug(g_cat_silence) << QString("Creating <[NTGCC]-[CC]-conjunction, coordinating> item");
    M1Store::Item_lv2* l_ntgcc = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[CC]-conjunction, coordinating",
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

    // creation of "[NIL]-missing tag"
    qCDebug(g_cat_silence) << QString("Creating <[NTNIL]-[NIL]-missing tag> item");
    M1Store::Item_lv2* l_ntnil = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[NIL]-missing tag",
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

    // creation of "[POS]-possessive ending"
    qCDebug(g_cat_silence) << QString("Creating <[NTPOS]-[POS]-possessive ending> item");
    M1Store::Item_lv2* l_ntpos = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,
        // label
        "[POS]-possessive ending",
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
    // creation of Republic text node
    qCDebug(g_cat_silence) << QString("Creating Republic text node");
    M1Store::Item_lv2* l_republic = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "The Republic (Dialogue)");
    l_republic->setType("TEXT_");

    // creation of Plato Person node
    qCDebug(g_cat_silence) << QString("Creating Plato node");
    M1Store::Item_lv2* l_plato = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Plato, son of Ariston of Collytus");
    l_plato->setType("PERSN");

    l_republic->linkTo(l_plato, "WRTBY");

    // creation of Republic text notes node
    qCDebug(g_cat_silence) << QString("Creating Republic text node");
    M1Store::Item_lv2* l_republic_notes = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Notes");
    l_republic_notes->setType("FOLDR");

    l_republic->linkTo(l_republic_notes, "OWNS_");

    M1Store::Item_lv2* l_note_array[3];
    // creation of note <A-SHR-079586>
    qCDebug(g_cat_silence) << QString("Creating note <A-SHR-079586>");
    l_note_array[0] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἠρέμα : Cf. Symposium 221 B. Plato's humorous use of this word is the source of Emerson's humorous use of “gently.”");
    l_note_array[0]->setType("TXTNT");
    l_note_array[0]->linkTo(l_republic_notes, "BLNGS");

    // creation of note <A-SHR-079665>
    qCDebug(g_cat_silence) << QString("Creating note <A-SHR-079665>");
    l_note_array[1] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "For the humor of the sudden shift to the second person cf. Juvenal, Satire i. “profer, Galla, caput.”");
    l_note_array[1]->setType("TXTNT");
    l_note_array[1]->linkTo(l_republic_notes, "BLNGS");

    // creation of note <A-SHR-079710>
    qCDebug(g_cat_silence) << QString("Creating note <A-SHR-079710>");
    l_note_array[2] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "To understand what follows it is necessary (1) to assume that Plato is not talking nonsense; (2) to make allowance for the necessity that he is under of combating contemporary fallacies and sophisms which may seem trivial to us (Cf. Unity of Plato's Thought , pp. 50 ff.); (3) to remember the greater richness of the Greek language in forms of the verb “to be”; and the misunderstandings introduced by the indiscriminate use of the abstract verbal noun “being” in English—a difficulty which I have tried to meet by varying the terms of the translation; (4) to recognize that apart from metaphysics Plato's main purpose is to insist on the ability to think abstractly as a prerequisite of the higher education; (5) to observe the qualifications and turns of phrase which indicate that Plato himself was not confused by the double meaning of “is not,” but was already aware of the distinctions explicitly explained in the Sophist . (Cf. Unity of Plato's Thought , pp. 53 ff. nn. 389 ff.)");
    l_note_array[2]->setType("TXTNT");
    l_note_array[2]->linkTo(l_republic_notes, "BLNGS");

    M1Store::Item_lv2* l_lemma_array[297];
    // creation of lemma <they‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <they‣PRON>");
    l_lemma_array[0] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "they");
    l_lemma_array[0]->setType("LEMMA");
    l_lemma_array[0]->setType("NPPRN");

    // creation of lemma <would‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <would‣VERB>");
    l_lemma_array[1] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "would");
    l_lemma_array[1]->setType("LEMMA");
    l_lemma_array[1]->setType("NPVRB");

    // creation of lemma <indeed‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <indeed‣ADV>");
    l_lemma_array[2] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "indeed");
    l_lemma_array[2]->setType("LEMMA");
    l_lemma_array[2]->setType("NPADV");

    // creation of lemma <he‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <he‣PRON>");
    l_lemma_array[3] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "he");
    l_lemma_array[3]->setType("LEMMA");
    l_lemma_array[3]->setType("NPPRN");

    // creation of lemma <then‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <then‣ADV>");
    l_lemma_array[4] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "then");
    l_lemma_array[4]->setType("LEMMA");
    l_lemma_array[4]->setType("NPADV");

    // creation of lemma <who‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <who‣PRON>");
    l_lemma_array[5] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "who");
    l_lemma_array[5]->setType("LEMMA");
    l_lemma_array[5]->setType("NPPRN");

    // creation of lemma <believe‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <believe‣VERB>");
    l_lemma_array[6] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "believe");
    l_lemma_array[6]->setType("LEMMA");
    l_lemma_array[6]->setType("NPVRB");

    // creation of lemma <in‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <in‣SCONJ>");
    l_lemma_array[7] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "in");
    l_lemma_array[7]->setType("LEMMA");
    l_lemma_array[7]->setType("NPSCJ");

    // creation of lemma <beautiful‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <beautiful‣ADJ>");
    l_lemma_array[8] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "beautiful");
    l_lemma_array[8]->setType("LEMMA");
    l_lemma_array[8]->setType("NPADJ");

    // creation of lemma <thing‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <thing‣NOUN>");
    l_lemma_array[9] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "thing");
    l_lemma_array[9]->setType("LEMMA");
    l_lemma_array[9]->setType("NPNON");

    // creation of lemma <but‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <but‣CCONJ>");
    l_lemma_array[10] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "but");
    l_lemma_array[10]->setType("LEMMA");
    l_lemma_array[10]->setType("NPCNJ");

    // creation of lemma <neither‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <neither‣CCONJ>");
    l_lemma_array[11] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "neither");
    l_lemma_array[11]->setType("LEMMA");
    l_lemma_array[11]->setType("NPCNJ");

    // creation of lemma <beauty‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <beauty‣NOUN>");
    l_lemma_array[12] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "beauty");
    l_lemma_array[12]->setType("LEMMA");
    l_lemma_array[12]->setType("NPNON");

    // creation of lemma <itself‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <itself‣PRON>");
    l_lemma_array[13] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "itself");
    l_lemma_array[13]->setType("LEMMA");
    l_lemma_array[13]->setType("NPPRN");

    // creation of lemma <nor‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <nor‣CCONJ>");
    l_lemma_array[14] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "nor");
    l_lemma_array[14]->setType("LEMMA");
    l_lemma_array[14]->setType("NPCNJ");

    // creation of lemma <be‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <be‣VERB>");
    l_lemma_array[15] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "be");
    l_lemma_array[15]->setType("LEMMA");
    l_lemma_array[15]->setType("NPVRB");

    // creation of lemma <able‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <able‣ADJ>");
    l_lemma_array[16] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "able");
    l_lemma_array[16]->setType("LEMMA");
    l_lemma_array[16]->setType("NPADJ");

    // creation of lemma <to‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <to‣PART>");
    l_lemma_array[17] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "to");
    l_lemma_array[17]->setType("LEMMA");
    l_lemma_array[17]->setType("NPPRT");

    // creation of lemma <follow‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <follow‣VERB>");
    l_lemma_array[18] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "follow");
    l_lemma_array[18]->setType("LEMMA");
    l_lemma_array[18]->setType("NPVRB");

    // creation of lemma <when‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <when‣SCONJ>");
    l_lemma_array[19] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "when");
    l_lemma_array[19]->setType("LEMMA");
    l_lemma_array[19]->setType("NPSCJ");

    // creation of lemma <someone‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <someone‣PRON>");
    l_lemma_array[20] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "someone");
    l_lemma_array[20]->setType("LEMMA");
    l_lemma_array[20]->setType("NPPRN");

    // creation of lemma <try‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <try‣VERB>");
    l_lemma_array[21] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "try");
    l_lemma_array[21]->setType("LEMMA");
    l_lemma_array[21]->setType("NPVRB");

    // creation of lemma <guide‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <guide‣VERB>");
    l_lemma_array[22] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "guide");
    l_lemma_array[22]->setType("LEMMA");
    l_lemma_array[22]->setType("NPVRB");

    // creation of lemma <to‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <to‣SCONJ>");
    l_lemma_array[23] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "to");
    l_lemma_array[23]->setType("LEMMA");
    l_lemma_array[23]->setType("NPSCJ");

    // creation of lemma <the‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <the‣PRON>");
    l_lemma_array[24] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "the");
    l_lemma_array[24]->setType("LEMMA");
    l_lemma_array[24]->setType("NPPRN");

    // creation of lemma <knowledge‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <knowledge‣NOUN>");
    l_lemma_array[25] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "knowledge");
    l_lemma_array[25]->setType("LEMMA");
    l_lemma_array[25]->setType("NPNON");

    // creation of lemma <of‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <of‣SCONJ>");
    l_lemma_array[26] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "of");
    l_lemma_array[26]->setType("LEMMA");
    l_lemma_array[26]->setType("NPSCJ");

    // creation of lemma <it‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <it‣PRON>");
    l_lemma_array[27] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "it");
    l_lemma_array[27]->setType("LEMMA");
    l_lemma_array[27]->setType("NPPRN");

    // creation of lemma <do‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <do‣VERB>");
    l_lemma_array[28] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "do");
    l_lemma_array[28]->setType("LEMMA");
    l_lemma_array[28]->setType("NPVRB");

    // creation of lemma <you‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <you‣PRON>");
    l_lemma_array[29] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "you");
    l_lemma_array[29]->setType("LEMMA");
    l_lemma_array[29]->setType("NPPRN");

    // creation of lemma <think‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <think‣VERB>");
    l_lemma_array[30] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "think");
    l_lemma_array[30]->setType("LEMMA");
    l_lemma_array[30]->setType("NPVRB");

    // creation of lemma <that‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <that‣SCONJ>");
    l_lemma_array[31] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "that");
    l_lemma_array[31]->setType("LEMMA");
    l_lemma_array[31]->setType("NPSCJ");

    // creation of lemma <his‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <his‣PRON>");
    l_lemma_array[32] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "his");
    l_lemma_array[32]->setType("LEMMA");
    l_lemma_array[32]->setType("NPPRN");

    // creation of lemma <life‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <life‣NOUN>");
    l_lemma_array[33] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "life");
    l_lemma_array[33]->setType("LEMMA");
    l_lemma_array[33]->setType("NPNON");

    // creation of lemma <a‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <a‣PRON>");
    l_lemma_array[34] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "a");
    l_lemma_array[34]->setType("LEMMA");
    l_lemma_array[34]->setType("NPPRN");

    // creation of lemma <dream‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <dream‣NOUN>");
    l_lemma_array[35] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dream");
    l_lemma_array[35]->setType("LEMMA");
    l_lemma_array[35]->setType("NPNON");

    // creation of lemma <or‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <or‣CCONJ>");
    l_lemma_array[36] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "or");
    l_lemma_array[36]->setType("LEMMA");
    l_lemma_array[36]->setType("NPCNJ");

    // creation of lemma <waking‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <waking‣NOUN>");
    l_lemma_array[37] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "waking");
    l_lemma_array[37]->setType("LEMMA");
    l_lemma_array[37]->setType("NPNON");

    // creation of lemma <just‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <just‣ADV>");
    l_lemma_array[38] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "just");
    l_lemma_array[38]->setType("LEMMA");
    l_lemma_array[38]->setType("NPADV");

    // creation of lemma <consider‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <consider‣VERB>");
    l_lemma_array[39] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "consider");
    l_lemma_array[39]->setType("LEMMA");
    l_lemma_array[39]->setType("NPVRB");

    // creation of lemma <not‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <not‣PART>");
    l_lemma_array[40] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "not");
    l_lemma_array[40]->setType("LEMMA");
    l_lemma_array[40]->setType("NPPRT");

    // creation of lemma <state‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <state‣NOUN>");
    l_lemma_array[41] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "state");
    l_lemma_array[41]->setType("LEMMA");
    l_lemma_array[41]->setType("NPNON");

    // creation of lemma <whether‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <whether‣SCONJ>");
    l_lemma_array[42] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "whether");
    l_lemma_array[42]->setType("LEMMA");
    l_lemma_array[42]->setType("NPSCJ");

    // creation of lemma <man‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <man‣NOUN>");
    l_lemma_array[43] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "man");
    l_lemma_array[43]->setType("LEMMA");
    l_lemma_array[43]->setType("NPNON");

    // creation of lemma <asleep‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <asleep‣ADJ>");
    l_lemma_array[44] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "asleep");
    l_lemma_array[44]->setType("LEMMA");
    l_lemma_array[44]->setType("NPADJ");

    // creation of lemma <awake‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <awake‣ADJ>");
    l_lemma_array[45] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "awake");
    l_lemma_array[45]->setType("LEMMA");
    l_lemma_array[45]->setType("NPADJ");

    // creation of lemma <this‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <this‣PRON>");
    l_lemma_array[46] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "this");
    l_lemma_array[46]->setType("LEMMA");
    l_lemma_array[46]->setType("NPPRN");

    // creation of lemma <mistaking‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <mistaking‣NOUN>");
    l_lemma_array[47] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "mistaking");
    l_lemma_array[47]->setType("LEMMA");
    l_lemma_array[47]->setType("NPNON");

    // creation of lemma <resemblance‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <resemblance‣NOUN>");
    l_lemma_array[48] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "resemblance");
    l_lemma_array[48]->setType("LEMMA");
    l_lemma_array[48]->setType("NPNON");

    // creation of lemma <for‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <for‣SCONJ>");
    l_lemma_array[49] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "for");
    l_lemma_array[49]->setType("LEMMA");
    l_lemma_array[49]->setType("NPSCJ");

    // creation of lemma <identity‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <identity‣NOUN>");
    l_lemma_array[50] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "identity");
    l_lemma_array[50]->setType("LEMMA");
    l_lemma_array[50]->setType("NPNON");

    // creation of lemma <i‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <i‣PRON>");
    l_lemma_array[51] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "i");
    l_lemma_array[51]->setType("LEMMA");
    l_lemma_array[51]->setType("NPPRN");

    // creation of lemma <should‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <should‣VERB>");
    l_lemma_array[52] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "should");
    l_lemma_array[52]->setType("LEMMA");
    l_lemma_array[52]->setType("NPVRB");

    // creation of lemma <certainly‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <certainly‣ADV>");
    l_lemma_array[53] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "certainly");
    l_lemma_array[53]->setType("LEMMA");
    l_lemma_array[53]->setType("NPADV");

    // creation of lemma <call‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <call‣VERB>");
    l_lemma_array[54] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "call");
    l_lemma_array[54]->setType("LEMMA");
    l_lemma_array[54]->setType("NPVRB");

    // creation of lemma <that‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <that‣PRON>");
    l_lemma_array[55] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "that");
    l_lemma_array[55]->setType("LEMMA");
    l_lemma_array[55]->setType("NPPRN");

    // creation of lemma <dreaming‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <dreaming‣NOUN>");
    l_lemma_array[56] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dreaming");
    l_lemma_array[56]->setType("LEMMA");
    l_lemma_array[56]->setType("NPNON");

    // creation of lemma <say‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <say‣VERB>");
    l_lemma_array[57] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "say");
    l_lemma_array[57]->setType("LEMMA");
    l_lemma_array[57]->setType("NPVRB");

    // creation of lemma <well‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <well‣ADV>");
    l_lemma_array[58] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "well");
    l_lemma_array[58]->setType("LEMMA");
    l_lemma_array[58]->setType("NPADV");

    // creation of lemma <take‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <take‣VERB>");
    l_lemma_array[59] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "take");
    l_lemma_array[59]->setType("LEMMA");
    l_lemma_array[59]->setType("NPVRB");

    // creation of lemma <opposite‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <opposite‣ADJ>");
    l_lemma_array[60] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opposite");
    l_lemma_array[60]->setType("LEMMA");
    l_lemma_array[60]->setType("NPADJ");

    // creation of lemma <case‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <case‣NOUN>");
    l_lemma_array[61] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "case");
    l_lemma_array[61]->setType("LEMMA");
    l_lemma_array[61]->setType("NPNON");

    // creation of lemma <whose‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <whose‣PRON>");
    l_lemma_array[62] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "whose");
    l_lemma_array[62]->setType("LEMMA");
    l_lemma_array[62]->setType("NPPRN");

    // creation of lemma <thought‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <thought‣NOUN>");
    l_lemma_array[63] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "thought");
    l_lemma_array[63]->setType("LEMMA");
    l_lemma_array[63]->setType("NPNON");

    // creation of lemma <recognize‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <recognize‣VERB>");
    l_lemma_array[64] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "recognize");
    l_lemma_array[64]->setType("LEMMA");
    l_lemma_array[64]->setType("NPVRB");

    // creation of lemma <and‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <and‣CCONJ>");
    l_lemma_array[65] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "and");
    l_lemma_array[65]->setType("LEMMA");
    l_lemma_array[65]->setType("NPCNJ");

    // creation of lemma <distinguish‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <distinguish‣VERB>");
    l_lemma_array[66] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "distinguish");
    l_lemma_array[66]->setType("LEMMA");
    l_lemma_array[66]->setType("NPVRB");

    // creation of lemma <self‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <self‣NOUN>");
    l_lemma_array[67] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "self");
    l_lemma_array[67]->setType("LEMMA");
    l_lemma_array[67]->setType("NPNON");

    // creation of lemma <self-beautiful‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <self-beautiful‣NOUN>");
    l_lemma_array[68] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "self-beautiful");
    l_lemma_array[68]->setType("LEMMA");
    l_lemma_array[68]->setType("NPNON");

    // creation of lemma <participate‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <participate‣VERB>");
    l_lemma_array[69] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "participate");
    l_lemma_array[69]->setType("LEMMA");
    l_lemma_array[69]->setType("NPVRB");

    // creation of lemma <suppose‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <suppose‣VERB>");
    l_lemma_array[70] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "suppose");
    l_lemma_array[70]->setType("LEMMA");
    l_lemma_array[70]->setType("NPVRB");

    // creation of lemma <participant‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <participant‣NOUN>");
    l_lemma_array[71] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "participant");
    l_lemma_array[71]->setType("LEMMA");
    l_lemma_array[71]->setType("NPNON");

    // creation of lemma <your‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <your‣PRON>");
    l_lemma_array[72] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "your");
    l_lemma_array[72]->setType("LEMMA");
    l_lemma_array[72]->setType("NPPRN");

    // creation of lemma <opinion‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <opinion‣NOUN>");
    l_lemma_array[73] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opinion");
    l_lemma_array[73]->setType("LEMMA");
    l_lemma_array[73]->setType("NPNON");

    // creation of lemma <wake‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <wake‣VERB>");
    l_lemma_array[74] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "wake");
    l_lemma_array[74]->setType("LEMMA");
    l_lemma_array[74]->setType("NPVRB");

    // creation of lemma <very‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <very‣ADV>");
    l_lemma_array[75] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "very");
    l_lemma_array[75]->setType("LEMMA");
    l_lemma_array[75]->setType("NPADV");

    // creation of lemma <much‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <much‣ADV>");
    l_lemma_array[76] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "much");
    l_lemma_array[76]->setType("LEMMA");
    l_lemma_array[76]->setType("NPADV");

    // creation of lemma <reply‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <reply‣VERB>");
    l_lemma_array[77] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "reply");
    l_lemma_array[77]->setType("LEMMA");
    l_lemma_array[77]->setType("NPVRB");

    // creation of lemma <could‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <could‣VERB>");
    l_lemma_array[78] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "could");
    l_lemma_array[78]->setType("LEMMA");
    l_lemma_array[78]->setType("NPVRB");

    // creation of lemma <we‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <we‣PRON>");
    l_lemma_array[79] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "we");
    l_lemma_array[79]->setType("LEMMA");
    l_lemma_array[79]->setType("NPPRN");

    // creation of lemma <rightly‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <rightly‣ADV>");
    l_lemma_array[80] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "rightly");
    l_lemma_array[80]->setType("LEMMA");
    l_lemma_array[80]->setType("NPADV");

    // creation of lemma <mental‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <mental‣ADJ>");
    l_lemma_array[81] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "mental");
    l_lemma_array[81]->setType("LEMMA");
    l_lemma_array[81]->setType("NPADJ");

    // creation of lemma <one‣NUM>
    qCDebug(g_cat_silence) << QString("Creating lemma <one‣NUM>");
    l_lemma_array[82] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "one");
    l_lemma_array[82]->setType("LEMMA");
    l_lemma_array[82]->setType("NPNUM");

    // creation of lemma <as‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <as‣SCONJ>");
    l_lemma_array[83] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "as");
    l_lemma_array[83]->setType("LEMMA");
    l_lemma_array[83]->setType("NPSCJ");

    // creation of lemma <knowing‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <knowing‣NOUN>");
    l_lemma_array[84] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "knowing");
    l_lemma_array[84]->setType("LEMMA");
    l_lemma_array[84]->setType("NPNON");

    // creation of lemma <other‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <other‣ADJ>");
    l_lemma_array[85] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "other");
    l_lemma_array[85]->setType("LEMMA");
    l_lemma_array[85]->setType("NPADJ");

    // creation of lemma <opining‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <opining‣NOUN>");
    l_lemma_array[86] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opining");
    l_lemma_array[86]->setType("LEMMA");
    l_lemma_array[86]->setType("NPNON");

    // creation of lemma <assuredly‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <assuredly‣ADV>");
    l_lemma_array[87] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "assuredly");
    l_lemma_array[87]->setType("LEMMA");
    l_lemma_array[87]->setType("NPADV");

    // creation of lemma <now‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <now‣ADV>");
    l_lemma_array[88] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "now");
    l_lemma_array[88]->setType("LEMMA");
    l_lemma_array[88]->setType("NPADV");

    // creation of lemma <opine‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <opine‣VERB>");
    l_lemma_array[89] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opine");
    l_lemma_array[89]->setType("LEMMA");
    l_lemma_array[89]->setType("NPVRB");

    // creation of lemma <know‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <know‣VERB>");
    l_lemma_array[90] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "know");
    l_lemma_array[90]->setType("LEMMA");
    l_lemma_array[90]->setType("NPVRB");

    // creation of lemma <angry‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <angry‣ADJ>");
    l_lemma_array[91] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "angry");
    l_lemma_array[91]->setType("LEMMA");
    l_lemma_array[91]->setType("NPADJ");

    // creation of lemma <challenge‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <challenge‣VERB>");
    l_lemma_array[92] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "challenge");
    l_lemma_array[92]->setType("LEMMA");
    l_lemma_array[92]->setType("NPVRB");

    // creation of lemma <our‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <our‣PRON>");
    l_lemma_array[93] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "our");
    l_lemma_array[93]->setType("LEMMA");
    l_lemma_array[93]->setType("NPPRN");

    // creation of lemma <statement‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <statement‣NOUN>");
    l_lemma_array[94] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "statement");
    l_lemma_array[94]->setType("LEMMA");
    l_lemma_array[94]->setType("NPNON");

    // creation of lemma <true‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <true‣ADJ>");
    l_lemma_array[95] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "true");
    l_lemma_array[95]->setType("LEMMA");
    l_lemma_array[95]->setType("NPADJ");

    // creation of lemma <can‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <can‣VERB>");
    l_lemma_array[96] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "can");
    l_lemma_array[96]->setType("LEMMA");
    l_lemma_array[96]->setType("NPVRB");

    // creation of lemma <find‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <find‣VERB>");
    l_lemma_array[97] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "find");
    l_lemma_array[97]->setType("LEMMA");
    l_lemma_array[97]->setType("NPVRB");

    // creation of lemma <any‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <any‣PRON>");
    l_lemma_array[98] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "any");
    l_lemma_array[98]->setType("LEMMA");
    l_lemma_array[98]->setType("NPPRN");

    // creation of lemma <way‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <way‣NOUN>");
    l_lemma_array[99] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "way");
    l_lemma_array[99]->setType("LEMMA");
    l_lemma_array[99]->setType("NPNON");

    // creation of lemma <soothe‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <soothe‣VERB>");
    l_lemma_array[100] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "soothe");
    l_lemma_array[100]->setType("LEMMA");
    l_lemma_array[100]->setType("NPVRB");

    // creation of lemma <gently‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <gently‣ADV>");
    l_lemma_array[101] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "gently");
    l_lemma_array[101]->setType("LEMMA");
    l_lemma_array[101]->setType("NPADV");

    // creation of lemma <win‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <win‣VERB>");
    l_lemma_array[102] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "win");
    l_lemma_array[102]->setType("LEMMA");
    l_lemma_array[102]->setType("NPVRB");

    // creation of lemma <over‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <over‣SCONJ>");
    l_lemma_array[103] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "over");
    l_lemma_array[103]->setType("LEMMA");
    l_lemma_array[103]->setType("NPSCJ");

    // creation of lemma <without‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <without‣SCONJ>");
    l_lemma_array[104] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "without");
    l_lemma_array[104]->setType("LEMMA");
    l_lemma_array[104]->setType("NPSCJ");

    // creation of lemma <tell‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <tell‣VERB>");
    l_lemma_array[105] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "tell");
    l_lemma_array[105]->setType("LEMMA");
    l_lemma_array[105]->setType("NPVRB");

    // creation of lemma <too‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <too‣ADV>");
    l_lemma_array[106] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "too");
    l_lemma_array[106]->setType("LEMMA");
    l_lemma_array[106]->setType("NPADV");

    // creation of lemma <plainly‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <plainly‣ADV>");
    l_lemma_array[107] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "plainly");
    l_lemma_array[107]->setType("LEMMA");
    l_lemma_array[107]->setType("NPADV");

    // creation of lemma <right‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <right‣ADJ>");
    l_lemma_array[108] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "right");
    l_lemma_array[108]->setType("LEMMA");
    l_lemma_array[108]->setType("NPADJ");

    // creation of lemma <mind‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <mind‣NOUN>");
    l_lemma_array[109] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "mind");
    l_lemma_array[109]->setType("LEMMA");
    l_lemma_array[109]->setType("NPNON");

    // creation of lemma <must‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <must‣VERB>");
    l_lemma_array[110] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "must");
    l_lemma_array[110]->setType("LEMMA");
    l_lemma_array[110]->setType("NPVRB");

    // creation of lemma <come‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <come‣VERB>");
    l_lemma_array[111] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "come");
    l_lemma_array[111]->setType("LEMMA");
    l_lemma_array[111]->setType("NPVRB");

    // creation of lemma <what‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <what‣PRON>");
    l_lemma_array[112] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "what");
    l_lemma_array[112]->setType("LEMMA");
    l_lemma_array[112]->setType("NPPRN");

    // creation of lemma <have‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <have‣VERB>");
    l_lemma_array[113] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "have");
    l_lemma_array[113]->setType("LEMMA");
    l_lemma_array[113]->setType("NPVRB");

    // creation of lemma <question‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <question‣VERB>");
    l_lemma_array[114] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "question");
    l_lemma_array[114]->setType("LEMMA");
    l_lemma_array[114]->setType("NPVRB");

    // creation of lemma <fashion‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <fashion‣NOUN>");
    l_lemma_array[115] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "fashion");
    l_lemma_array[115]->setType("LEMMA");
    l_lemma_array[115]->setType("NPNON");

    // creation of lemma <premise‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <premise‣VERB>");
    l_lemma_array[116] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "premise");
    l_lemma_array[116]->setType("LEMMA");
    l_lemma_array[116]->setType("NPVRB");

    // creation of lemma <if‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <if‣SCONJ>");
    l_lemma_array[117] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "if");
    l_lemma_array[117]->setType("LEMMA");
    l_lemma_array[117]->setType("NPSCJ");

    // creation of lemma <anything‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <anything‣PRON>");
    l_lemma_array[118] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "anything");
    l_lemma_array[118]->setType("LEMMA");
    l_lemma_array[118]->setType("NPPRN");

    // creation of lemma <nobody‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <nobody‣PRON>");
    l_lemma_array[119] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "nobody");
    l_lemma_array[119]->setType("LEMMA");
    l_lemma_array[119]->setType("NPPRN");

    // creation of lemma <grudge‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <grudge‣VERB>");
    l_lemma_array[120] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "grudge");
    l_lemma_array[120]->setType("LEMMA");
    l_lemma_array[120]->setType("NPVRB");

    // creation of lemma <glad‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <glad‣ADJ>");
    l_lemma_array[121] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "glad");
    l_lemma_array[121]->setType("LEMMA");
    l_lemma_array[121]->setType("NPADJ");

    // creation of lemma <see‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <see‣VERB>");
    l_lemma_array[122] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "see");
    l_lemma_array[122]->setType("LEMMA");
    l_lemma_array[122]->setType("NPVRB");

    // creation of lemma <something‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <something‣PRON>");
    l_lemma_array[123] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "something");
    l_lemma_array[123]->setType("LEMMA");
    l_lemma_array[123]->setType("NPPRN");

    // creation of lemma <nothing‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <nothing‣PRON>");
    l_lemma_array[124] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "nothing");
    l_lemma_array[124]->setType("LEMMA");
    l_lemma_array[124]->setType("NPPRN");

    // creation of lemma <behalf‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <behalf‣NOUN>");
    l_lemma_array[125] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "behalf");
    l_lemma_array[125]->setType("LEMMA");
    l_lemma_array[125]->setType("NPNON");

    // creation of lemma <will‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <will‣VERB>");
    l_lemma_array[126] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "will");
    l_lemma_array[126]->setType("LEMMA");
    l_lemma_array[126]->setType("NPVRB");

    // creation of lemma <how‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <how‣SCONJ>");
    l_lemma_array[127] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "how");
    l_lemma_array[127]->setType("LEMMA");
    l_lemma_array[127]->setType("NPSCJ");

    // creation of lemma <sense‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <sense‣NOUN>");
    l_lemma_array[128] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "sense");
    l_lemma_array[128]->setType("LEMMA");
    l_lemma_array[128]->setType("NPNON");

    // creation of lemma <no‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <no‣PRON>");
    l_lemma_array[129] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "no");
    l_lemma_array[129]->setType("LEMMA");
    l_lemma_array[129]->setType("NPPRN");

    // creation of lemma <absolute‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <absolute‣ADJ>");
    l_lemma_array[130] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "absolute");
    l_lemma_array[130]->setType("LEMMA");
    l_lemma_array[130]->setType("NPADJ");

    // creation of lemma <another‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <another‣PRON>");
    l_lemma_array[131] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "another");
    l_lemma_array[131]->setType("LEMMA");
    l_lemma_array[131]->setType("NPPRN");

    // creation of lemma <lead‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <lead‣VERB>");
    l_lemma_array[132] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "lead");
    l_lemma_array[132]->setType("LEMMA");
    l_lemma_array[132]->setType("NPVRB");

    // creation of lemma <unable‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <unable‣ADJ>");
    l_lemma_array[133] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "unable");
    l_lemma_array[133]->setType("LEMMA");
    l_lemma_array[133]->setType("NPADJ");

    // creation of lemma <such‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <such‣PRON>");
    l_lemma_array[134] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "such");
    l_lemma_array[134]->setType("LEMMA");
    l_lemma_array[134]->setType("NPPRN");

    // creation of lemma <an‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <an‣PRON>");
    l_lemma_array[135] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "an");
    l_lemma_array[135]->setType("LEMMA");
    l_lemma_array[135]->setType("NPPRN");

    // creation of lemma <ask‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ask‣VERB>");
    l_lemma_array[136] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ask");
    l_lemma_array[136]->setType("LEMMA");
    l_lemma_array[136]->setType("NPVRB");

    // creation of lemma <only‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <only‣ADV>");
    l_lemma_array[137] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "only");
    l_lemma_array[137]->setType("LEMMA");
    l_lemma_array[137]->setType("NPADV");

    // creation of lemma <reflect‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <reflect‣VERB>");
    l_lemma_array[138] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "reflect");
    l_lemma_array[138]->setType("LEMMA");
    l_lemma_array[138]->setType("NPVRB");

    // creation of lemma <dreamer‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <dreamer‣NOUN>");
    l_lemma_array[139] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dreamer");
    l_lemma_array[139]->setType("LEMMA");
    l_lemma_array[139]->setType("NPNON");

    // creation of lemma <sleep‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <sleep‣VERB>");
    l_lemma_array[140] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "sleep");
    l_lemma_array[140]->setType("LEMMA");
    l_lemma_array[140]->setType("NPVRB");

    // creation of lemma <liken‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <liken‣VERB>");
    l_lemma_array[141] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "liken");
    l_lemma_array[141]->setType("LEMMA");
    l_lemma_array[141]->setType("NPVRB");

    // creation of lemma <dissimilar‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <dissimilar‣ADJ>");
    l_lemma_array[142] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dissimilar");
    l_lemma_array[142]->setType("LEMMA");
    l_lemma_array[142]->setType("NPADJ");

    // creation of lemma <put‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <put‣VERB>");
    l_lemma_array[143] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "put");
    l_lemma_array[143]->setType("LEMMA");
    l_lemma_array[143]->setType("NPVRB");

    // creation of lemma <copy‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <copy‣NOUN>");
    l_lemma_array[144] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "copy");
    l_lemma_array[144]->setType("LEMMA");
    l_lemma_array[144]->setType("NPNON");

    // creation of lemma <place‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <place‣NOUN>");
    l_lemma_array[145] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "place");
    l_lemma_array[145]->setType("LEMMA");
    l_lemma_array[145]->setType("NPNON");

    // creation of lemma <real‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <real‣ADJ>");
    l_lemma_array[146] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "real");
    l_lemma_array[146]->setType("LEMMA");
    l_lemma_array[146]->setType("NPADJ");

    // creation of lemma <object‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <object‣NOUN>");
    l_lemma_array[147] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "object");
    l_lemma_array[147]->setType("LEMMA");
    l_lemma_array[147]->setType("NPNON");

    // creation of lemma <dream‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <dream‣VERB>");
    l_lemma_array[148] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dream");
    l_lemma_array[148]->setType("LEMMA");
    l_lemma_array[148]->setType("NPVRB");

    // creation of lemma <recognise‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <recognise‣VERB>");
    l_lemma_array[149] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "recognise");
    l_lemma_array[149]->setType("LEMMA");
    l_lemma_array[149]->setType("NPVRB");

    // creation of lemma <existence‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <existence‣NOUN>");
    l_lemma_array[150] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "existence");
    l_lemma_array[150]->setType("LEMMA");
    l_lemma_array[150]->setType("NPNON");

    // creation of lemma <idea‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <idea‣NOUN>");
    l_lemma_array[151] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "idea");
    l_lemma_array[151]->setType("LEMMA");
    l_lemma_array[151]->setType("NPNON");

    // creation of lemma <from‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <from‣SCONJ>");
    l_lemma_array[152] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "from");
    l_lemma_array[152]->setType("LEMMA");
    l_lemma_array[152]->setType("NPSCJ");

    // creation of lemma <which‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <which‣PRON>");
    l_lemma_array[153] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "which");
    l_lemma_array[153]->setType("LEMMA");
    l_lemma_array[153]->setType("NPPRN");

    // creation of lemma <wide‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <wide‣ADV>");
    l_lemma_array[154] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "wide");
    l_lemma_array[154]->setType("LEMMA");
    l_lemma_array[154]->setType("NPADV");

    // creation of lemma <may‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <may‣VERB>");
    l_lemma_array[155] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "may");
    l_lemma_array[155]->setType("LEMMA");
    l_lemma_array[155]->setType("NPVRB");

    // creation of lemma <one‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <one‣NOUN>");
    l_lemma_array[156] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "one");
    l_lemma_array[156]->setType("LEMMA");
    l_lemma_array[156]->setType("NPNON");

    // creation of lemma <latter‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <latter‣ADJ>");
    l_lemma_array[157] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "latter");
    l_lemma_array[157]->setType("LEMMA");
    l_lemma_array[157]->setType("NPADJ");

    // creation of lemma <quarrel‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <quarrel‣VERB>");
    l_lemma_array[158] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "quarrel");
    l_lemma_array[158]->setType("LEMMA");
    l_lemma_array[158]->setType("NPVRB");

    // creation of lemma <with‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <with‣SCONJ>");
    l_lemma_array[159] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "with");
    l_lemma_array[159]->setType("LEMMA");
    l_lemma_array[159]->setType("NPSCJ");

    // creation of lemma <dispute‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <dispute‣VERB>");
    l_lemma_array[160] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dispute");
    l_lemma_array[160]->setType("LEMMA");
    l_lemma_array[160]->setType("NPVRB");

    // creation of lemma <administer‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <administer‣VERB>");
    l_lemma_array[161] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "administer");
    l_lemma_array[161]->setType("LEMMA");
    l_lemma_array[161]->setType("NPVRB");

    // creation of lemma <soothing‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <soothing‣ADJ>");
    l_lemma_array[162] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "soothing");
    l_lemma_array[162]->setType("LEMMA");
    l_lemma_array[162]->setType("NPADJ");

    // creation of lemma <cordial‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <cordial‣NOUN>");
    l_lemma_array[163] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "cordial");
    l_lemma_array[163]->setType("LEMMA");
    l_lemma_array[163]->setType("NPNON");

    // creation of lemma <advice‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <advice‣NOUN>");
    l_lemma_array[164] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "advice");
    l_lemma_array[164]->setType("LEMMA");
    l_lemma_array[164]->setType("NPNON");

    // creation of lemma <reveal‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <reveal‣VERB>");
    l_lemma_array[165] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "reveal");
    l_lemma_array[165]->setType("LEMMA");
    l_lemma_array[165]->setType("NPVRB");

    // creation of lemma <there‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <there‣PRON>");
    l_lemma_array[166] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "there");
    l_lemma_array[166]->setType("LEMMA");
    l_lemma_array[166]->setType("NPPRN");

    // creation of lemma <sad‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <sad‣ADJ>");
    l_lemma_array[167] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "sad");
    l_lemma_array[167]->setType("LEMMA");
    l_lemma_array[167]->setType("NPADJ");

    // creation of lemma <disorder‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <disorder‣NOUN>");
    l_lemma_array[168] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "disorder");
    l_lemma_array[168]->setType("LEMMA");
    l_lemma_array[168]->setType("NPNON");

    // creation of lemma <wit‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <wit‣NOUN>");
    l_lemma_array[169] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "wit");
    l_lemma_array[169]->setType("LEMMA");
    l_lemma_array[169]->setType("NPNON");

    // creation of lemma <offer‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <offer‣VERB>");
    l_lemma_array[170] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "offer");
    l_lemma_array[170]->setType("LEMMA");
    l_lemma_array[170]->setType("NPVRB");

    // creation of lemma <some‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <some‣PRON>");
    l_lemma_array[171] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "some");
    l_lemma_array[171]->setType("LEMMA");
    l_lemma_array[171]->setType("NPPRN");

    // creation of lemma <good‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <good‣ADJ>");
    l_lemma_array[172] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "good");
    l_lemma_array[172]->setType("LEMMA");
    l_lemma_array[172]->setType("NPADJ");

    // creation of lemma <let‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <let‣VERB>");
    l_lemma_array[173] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "let");
    l_lemma_array[173]->setType("LEMMA");
    l_lemma_array[173]->setType("NPVRB");

    // creation of lemma <shall‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <shall‣VERB>");
    l_lemma_array[174] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "shall");
    l_lemma_array[174]->setType("LEMMA");
    l_lemma_array[174]->setType("NPVRB");

    // creation of lemma <begin‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <begin‣VERB>");
    l_lemma_array[175] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "begin");
    l_lemma_array[175]->setType("LEMMA");
    l_lemma_array[175]->setType("NPVRB");

    // creation of lemma <by‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <by‣SCONJ>");
    l_lemma_array[176] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "by");
    l_lemma_array[176]->setType("LEMMA");
    l_lemma_array[176]->setType("NPSCJ");

    // creation of lemma <assure‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <assure‣VERB>");
    l_lemma_array[177] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "assure");
    l_lemma_array[177]->setType("LEMMA");
    l_lemma_array[177]->setType("NPVRB");

    // creation of lemma <welcome‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <welcome‣ADJ>");
    l_lemma_array[178] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "welcome");
    l_lemma_array[178]->setType("LEMMA");
    l_lemma_array[178]->setType("NPADJ");

    // creation of lemma <rejoice‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <rejoice‣VERB>");
    l_lemma_array[179] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "rejoice");
    l_lemma_array[179]->setType("LEMMA");
    l_lemma_array[179]->setType("NPVRB");

    // creation of lemma <at‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <at‣SCONJ>");
    l_lemma_array[180] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "at");
    l_lemma_array[180]->setType("LEMMA");
    l_lemma_array[180]->setType("NPSCJ");

    // creation of lemma <like‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <like‣VERB>");
    l_lemma_array[181] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "like");
    l_lemma_array[181]->setType("LEMMA");
    l_lemma_array[181]->setType("NPVRB");

    // creation of lemma <question‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <question‣NOUN>");
    l_lemma_array[182] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "question");
    l_lemma_array[182]->setType("LEMMA");
    l_lemma_array[182]->setType("NPNON");

    // creation of lemma <answer‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <answer‣VERB>");
    l_lemma_array[183] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "answer");
    l_lemma_array[183]->setType("LEMMA");
    l_lemma_array[183]->setType("NPVRB");

    // creation of lemma <for‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <for‣CCONJ>");
    l_lemma_array[184] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "for");
    l_lemma_array[184]->setType("LEMMA");
    l_lemma_array[184]->setType("NPCNJ");

    // creation of lemma <ever‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <ever‣ADV>");
    l_lemma_array[185] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ever");
    l_lemma_array[185]->setType("LEMMA");
    l_lemma_array[185]->setType("NPADV");

    // creation of lemma <καί‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <καί‣CCONJ>");
    l_lemma_array[186] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καί");
    l_lemma_array[186]->setType("LEMMA");
    l_lemma_array[186]->setType("NPCNJ");

    // creation of lemma <μάλα‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <μάλα‣ADV>");
    l_lemma_array[187] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μάλα");
    l_lemma_array[187]->setType("LEMMA");
    l_lemma_array[187]->setType("NPADV");

    // creation of lemma <ὁ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὁ‣PRON>");
    l_lemma_array[188] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὁ");
    l_lemma_array[188]->setType("LEMMA");
    l_lemma_array[188]->setType("NPPRN");

    // creation of lemma <οὖν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὖν‣ADV>");
    l_lemma_array[189] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὖν");
    l_lemma_array[189]->setType("LEMMA");
    l_lemma_array[189]->setType("NPADV");

    // creation of lemma <καλός‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <καλός‣ADJ>");
    l_lemma_array[190] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καλός");
    l_lemma_array[190]->setType("LEMMA");
    l_lemma_array[190]->setType("NPADJ");

    // creation of lemma <μέν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <μέν‣ADV>");
    l_lemma_array[191] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μέν");
    l_lemma_array[191]->setType("LEMMA");
    l_lemma_array[191]->setType("NPADV");

    // creation of lemma <πρᾶγμα‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <πρᾶγμα‣NOUN>");
    l_lemma_array[192] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πρᾶγμα");
    l_lemma_array[192]->setType("LEMMA");
    l_lemma_array[192]->setType("NPNON");

    // creation of lemma <νομίζω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <νομίζω‣VERB>");
    l_lemma_array[193] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "νομίζω");
    l_lemma_array[193]->setType("LEMMA");
    l_lemma_array[193]->setType("NPVRB");

    // creation of lemma <αὐτός‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <αὐτός‣PRON>");
    l_lemma_array[194] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτός");
    l_lemma_array[194]->setType("LEMMA");
    l_lemma_array[194]->setType("NPPRN");

    // creation of lemma <δέ‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <δέ‣CCONJ>");
    l_lemma_array[195] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δέ");
    l_lemma_array[195]->setType("LEMMA");
    l_lemma_array[195]->setType("NPCNJ");

    // creation of lemma <κάλλος‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <κάλλος‣NOUN>");
    l_lemma_array[196] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "κάλλος");
    l_lemma_array[196]->setType("LEMMA");
    l_lemma_array[196]->setType("NPNON");

    // creation of lemma <μήτε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <μήτε‣ADV>");
    l_lemma_array[197] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μήτε");
    l_lemma_array[197]->setType("LEMMA");
    l_lemma_array[197]->setType("NPADV");

    // creation of lemma <ἄν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἄν‣ADV>");
    l_lemma_array[198] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἄν");
    l_lemma_array[198]->setType("LEMMA");
    l_lemma_array[198]->setType("NPADV");

    // creation of lemma <τις‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <τις‣PRON>");
    l_lemma_array[199] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τις");
    l_lemma_array[199]->setType("LEMMA");
    l_lemma_array[199]->setType("NPPRN");

    // creation of lemma <ἡγέομαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἡγέομαι‣VERB>");
    l_lemma_array[200] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἡγέομαι");
    l_lemma_array[200]->setType("LEMMA");
    l_lemma_array[200]->setType("NPVRB");

    // creation of lemma <ἐπί‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐπί‣SCONJ>");
    l_lemma_array[201] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐπί");
    l_lemma_array[201]->setType("LEMMA");
    l_lemma_array[201]->setType("NPSCJ");

    // creation of lemma <γνῶσις‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <γνῶσις‣NOUN>");
    l_lemma_array[202] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γνῶσις");
    l_lemma_array[202]->setType("LEMMA");
    l_lemma_array[202]->setType("NPNON");

    // creation of lemma <δύναμαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <δύναμαι‣VERB>");
    l_lemma_array[203] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δύναμαι");
    l_lemma_array[203]->setType("LEMMA");
    l_lemma_array[203]->setType("NPVRB");

    // creation of lemma <ἕπομαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἕπομαι‣VERB>");
    l_lemma_array[204] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἕπομαι");
    l_lemma_array[204]->setType("LEMMA");
    l_lemma_array[204]->setType("NPVRB");

    // creation of lemma <ὄναρ‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὄναρ‣NOUN>");
    l_lemma_array[205] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὄναρ");
    l_lemma_array[205]->setType("LEMMA");
    l_lemma_array[205]->setType("NPNON");

    // creation of lemma <ἤ‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἤ‣CCONJ>");
    l_lemma_array[206] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἤ");
    l_lemma_array[206]->setType("LEMMA");
    l_lemma_array[206]->setType("NPCNJ");

    // creation of lemma <ὕπαρ‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὕπαρ‣NOUN>");
    l_lemma_array[207] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὕπαρ");
    l_lemma_array[207]->setType("LEMMA");
    l_lemma_array[207]->setType("NPNON");

    // creation of lemma <δοκέω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <δοκέω‣VERB>");
    l_lemma_array[208] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δοκέω");
    l_lemma_array[208]->setType("LEMMA");
    l_lemma_array[208]->setType("NPVRB");

    // creation of lemma <σύ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <σύ‣PRON>");
    l_lemma_array[209] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "σύ");
    l_lemma_array[209]->setType("LEMMA");
    l_lemma_array[209]->setType("NPPRN");

    // creation of lemma <ζάω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ζάω‣VERB>");
    l_lemma_array[210] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ζάω");
    l_lemma_array[210]->setType("LEMMA");
    l_lemma_array[210]->setType("NPVRB");

    // creation of lemma <σκοπέω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <σκοπέω‣VERB>");
    l_lemma_array[211] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "σκοπέω");
    l_lemma_array[211]->setType("LEMMA");
    l_lemma_array[211]->setType("NPVRB");

    // creation of lemma <δέ‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <δέ‣PART>");
    l_lemma_array[212] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δέ");
    l_lemma_array[212]->setType("LEMMA");
    l_lemma_array[212]->setType("NPPRT");

    // creation of lemma <ὀνειρωττέω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὀνειρωττέω‣VERB>");
    l_lemma_array[213] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὀνειρωττέω");
    l_lemma_array[213]->setType("LEMMA");
    l_lemma_array[213]->setType("NPVRB");

    // creation of lemma <ἆρα‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἆρα‣PART>");
    l_lemma_array[214] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἆρα");
    l_lemma_array[214]->setType("LEMMA");
    l_lemma_array[214]->setType("NPPRT");

    // creation of lemma <οὐ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὐ‣ADV>");
    l_lemma_array[215] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐ");
    l_lemma_array[215]->setType("LEMMA");
    l_lemma_array[215]->setType("NPADV");

    // creation of lemma <ὅδε‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὅδε‣PRON>");
    l_lemma_array[216] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅδε");
    l_lemma_array[216]->setType("LEMMA");
    l_lemma_array[216]->setType("NPPRN");

    // creation of lemma <εἰμί‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <εἰμί‣VERB>");
    l_lemma_array[217] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἰμί");
    l_lemma_array[217]->setType("LEMMA");
    l_lemma_array[217]->setType("NPVRB");

    // creation of lemma <ἐάντε‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐάντε‣SCONJ>");
    l_lemma_array[218] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐάντε");
    l_lemma_array[218]->setType("LEMMA");
    l_lemma_array[218]->setType("NPSCJ");

    // creation of lemma <ἐν‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐν‣SCONJ>");
    l_lemma_array[219] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐν");
    l_lemma_array[219]->setType("LEMMA");
    l_lemma_array[219]->setType("NPSCJ");

    // creation of lemma <ὕπνος‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὕπνος‣NOUN>");
    l_lemma_array[220] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὕπνος");
    l_lemma_array[220]->setType("LEMMA");
    l_lemma_array[220]->setType("NPNON");

    // creation of lemma <ἐάω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐάω‣VERB>");
    l_lemma_array[221] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐάω");
    l_lemma_array[221]->setType("LEMMA");
    l_lemma_array[221]->setType("NPVRB");

    // creation of lemma <ἐγρηγώ‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐγρηγώ‣VERB>");
    l_lemma_array[222] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐγρηγώ");
    l_lemma_array[222]->setType("LEMMA");
    l_lemma_array[222]->setType("NPVRB");

    // creation of lemma <ὁμοῖος‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὁμοῖος‣ADJ>");
    l_lemma_array[223] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὁμοῖος");
    l_lemma_array[223]->setType("LEMMA");
    l_lemma_array[223]->setType("NPADJ");

    // creation of lemma <μή‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <μή‣ADV>");
    l_lemma_array[224] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μή");
    l_lemma_array[224]->setType("LEMMA");
    l_lemma_array[224]->setType("NPADV");

    // creation of lemma <ἀλλ᾽‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἀλλ᾽‣PART>");
    l_lemma_array[225] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀλλ᾽");
    l_lemma_array[225]->setType("LEMMA");
    l_lemma_array[225]->setType("NPPRT");

    // creation of lemma <ὅς‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὅς‣PRON>");
    l_lemma_array[226] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅς");
    l_lemma_array[226]->setType("LEMMA");
    l_lemma_array[226]->setType("NPPRN");

    // creation of lemma <ἔοικα‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἔοικα‣VERB>");
    l_lemma_array[227] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἔοικα");
    l_lemma_array[227]->setType("LEMMA");
    l_lemma_array[227]->setType("NPVRB");

    // creation of lemma <ἐγώ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐγώ‣PRON>");
    l_lemma_array[228] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐγώ");
    l_lemma_array[228]->setType("LEMMA");
    l_lemma_array[228]->setType("NPPRN");

    // creation of lemma <γοῦν‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <γοῦν‣PART>");
    l_lemma_array[229] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γοῦν");
    l_lemma_array[229]->setType("LEMMA");
    l_lemma_array[229]->setType("NPPRT");

    // creation of lemma <ἄν‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἄν‣PART>");
    l_lemma_array[230] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἄν");
    l_lemma_array[230]->setType("LEMMA");
    l_lemma_array[230]->setType("NPPRT");

    // creation of lemma <ἦ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἦ‣ADV>");
    l_lemma_array[231] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἦ");
    l_lemma_array[231]->setType("LEMMA");
    l_lemma_array[231]->setType("NPADV");

    // creation of lemma <φημί‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <φημί‣VERB>");
    l_lemma_array[232] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "φημί");
    l_lemma_array[232]->setType("LEMMA");
    l_lemma_array[232]->setType("NPVRB");

    // creation of lemma <τοιοῦτος‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <τοιοῦτος‣ADJ>");
    l_lemma_array[233] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τοιοῦτος");
    l_lemma_array[233]->setType("LEMMA");
    l_lemma_array[233]->setType("NPADJ");

    // creation of lemma <τίς‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <τίς‣PRON>");
    l_lemma_array[234] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τίς");
    l_lemma_array[234]->setType("LEMMA");
    l_lemma_array[234]->setType("NPPRN");

    // creation of lemma <ἐναντίος‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐναντίος‣ADJ>");
    l_lemma_array[235] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐναντίος");
    l_lemma_array[235]->setType("LEMMA");
    l_lemma_array[235]->setType("NPADJ");

    // creation of lemma <οὗτος‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὗτος‣PRON>");
    l_lemma_array[236] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὗτος");
    l_lemma_array[236]->setType("LEMMA");
    l_lemma_array[236]->setType("NPPRN");

    // creation of lemma <τε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <τε‣ADV>");
    l_lemma_array[237] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τε");
    l_lemma_array[237]->setType("LEMMA");
    l_lemma_array[237]->setType("NPADV");

    // creation of lemma <καθοράω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <καθοράω‣VERB>");
    l_lemma_array[238] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καθοράω");
    l_lemma_array[238]->setType("LEMMA");
    l_lemma_array[238]->setType("NPVRB");

    // creation of lemma <ἐκεῖνος‣PRON>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐκεῖνος‣PRON>");
    l_lemma_array[239] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐκεῖνος");
    l_lemma_array[239]->setType("LEMMA");
    l_lemma_array[239]->setType("NPPRN");

    // creation of lemma <μετέχω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <μετέχω‣VERB>");
    l_lemma_array[240] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μετέχω");
    l_lemma_array[240]->setType("LEMMA");
    l_lemma_array[240]->setType("NPVRB");

    // creation of lemma <οὔτε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὔτε‣ADV>");
    l_lemma_array[241] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὔτε");
    l_lemma_array[241]->setType("LEMMA");
    l_lemma_array[241]->setType("NPADV");

    // creation of lemma <ὑπάρω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὑπάρω‣VERB>");
    l_lemma_array[242] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὑπάρω");
    l_lemma_array[242]->setType("LEMMA");
    l_lemma_array[242]->setType("NPVRB");

    // creation of lemma <αὖ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <αὖ‣ADV>");
    l_lemma_array[243] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὖ");
    l_lemma_array[243]->setType("LEMMA");
    l_lemma_array[243]->setType("NPADV");

    // creation of lemma <καί‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <καί‣ADV>");
    l_lemma_array[244] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καί");
    l_lemma_array[244]->setType("LEMMA");
    l_lemma_array[244]->setType("NPADV");

    // creation of lemma <οὗτος‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὗτος‣ADJ>");
    l_lemma_array[245] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὗτος");
    l_lemma_array[245]->setType("LEMMA");
    l_lemma_array[245]->setType("NPADJ");

    // creation of lemma <οὐκοῦν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὐκοῦν‣ADV>");
    l_lemma_array[246] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐκοῦν");
    l_lemma_array[246]->setType("LEMMA");
    l_lemma_array[246]->setType("NPADV");

    // creation of lemma <διάνοια‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <διάνοια‣NOUN>");
    l_lemma_array[247] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "διάνοια");
    l_lemma_array[247]->setType("LEMMA");
    l_lemma_array[247]->setType("NPNON");

    // creation of lemma <ὡς‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὡς‣SCONJ>");
    l_lemma_array[248] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὡς");
    l_lemma_array[248]->setType("LEMMA");
    l_lemma_array[248]->setType("NPSCJ");

    // creation of lemma <γιγνώσκω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <γιγνώσκω‣VERB>");
    l_lemma_array[249] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γιγνώσκω");
    l_lemma_array[249]->setType("LEMMA");
    l_lemma_array[249]->setType("NPVRB");

    // creation of lemma <γνώμη‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <γνώμη‣NOUN>");
    l_lemma_array[250] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γνώμη");
    l_lemma_array[250]->setType("LEMMA");
    l_lemma_array[250]->setType("NPNON");

    // creation of lemma <ὀρθός‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὀρθός‣ADV>");
    l_lemma_array[251] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὀρθός");
    l_lemma_array[251]->setType("LEMMA");
    l_lemma_array[251]->setType("NPADV");

    // creation of lemma <φαίνω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <φαίνω‣VERB>");
    l_lemma_array[252] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "φαίνω");
    l_lemma_array[252]->setType("LEMMA");
    l_lemma_array[252]->setType("NPVRB");

    // creation of lemma <δόξα‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <δόξα‣NOUN>");
    l_lemma_array[253] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δόξα");
    l_lemma_array[253]->setType("LEMMA");
    l_lemma_array[253]->setType("NPNON");

    // creation of lemma <δοξάζω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <δοξάζω‣VERB>");
    l_lemma_array[254] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δοξάζω");
    l_lemma_array[254]->setType("LEMMA");
    l_lemma_array[254]->setType("NPVRB");

    // creation of lemma <πάνυ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <πάνυ‣ADV>");
    l_lemma_array[255] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πάνυ");
    l_lemma_array[255]->setType("LEMMA");
    l_lemma_array[255]->setType("NPADV");

    // creation of lemma <μέν‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <μέν‣PART>");
    l_lemma_array[256] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μέν");
    l_lemma_array[256]->setType("LEMMA");
    l_lemma_array[256]->setType("NPPRT");

    // creation of lemma <οὖν‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὖν‣PART>");
    l_lemma_array[257] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὖν");
    l_lemma_array[257]->setType("LEMMA");
    l_lemma_array[257]->setType("NPPRT");

    // creation of lemma <ἐάν‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐάν‣SCONJ>");
    l_lemma_array[258] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐάν");
    l_lemma_array[258]->setType("LEMMA");
    l_lemma_array[258]->setType("NPSCJ");

    // creation of lemma <χαλεπαίνω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <χαλεπαίνω‣VERB>");
    l_lemma_array[259] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "χαλεπαίνω");
    l_lemma_array[259]->setType("LEMMA");
    l_lemma_array[259]->setType("NPVRB");

    // creation of lemma <ἀμφισβήτης‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἀμφισβήτης‣NOUN>");
    l_lemma_array[260] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀμφισβήτης");
    l_lemma_array[260]->setType("LEMMA");
    l_lemma_array[260]->setType("NPNON");

    // creation of lemma <ἀληθής‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἀληθής‣ADJ>");
    l_lemma_array[261] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀληθής");
    l_lemma_array[261]->setType("LEMMA");
    l_lemma_array[261]->setType("NPADJ");

    // creation of lemma <λέγω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <λέγω‣VERB>");
    l_lemma_array[262] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "λέγω");
    l_lemma_array[262]->setType("LEMMA");
    l_lemma_array[262]->setType("NPVRB");

    // creation of lemma <ἔχω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἔχω‣VERB>");
    l_lemma_array[263] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἔχω");
    l_lemma_array[263]->setType("LEMMA");
    l_lemma_array[263]->setType("NPVRB");

    // creation of lemma <παραμυθέομαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <παραμυθέομαι‣VERB>");
    l_lemma_array[264] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "παραμυθέομαι");
    l_lemma_array[264]->setType("LEMMA");
    l_lemma_array[264]->setType("NPVRB");

    // creation of lemma <αὐτός‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <αὐτός‣ADJ>");
    l_lemma_array[265] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτός");
    l_lemma_array[265]->setType("LEMMA");
    l_lemma_array[265]->setType("NPADJ");

    // creation of lemma <πείθω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <πείθω‣VERB>");
    l_lemma_array[266] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πείθω");
    l_lemma_array[266]->setType("LEMMA");
    l_lemma_array[266]->setType("NPVRB");

    // creation of lemma <ἠρέμα‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἠρέμα‣NOUN>");
    l_lemma_array[267] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἠρέμα");
    l_lemma_array[267]->setType("LEMMA");
    l_lemma_array[267]->setType("NPNON");

    // creation of lemma <ἐπικρύπτω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐπικρύπτω‣VERB>");
    l_lemma_array[268] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐπικρύπτω");
    l_lemma_array[268]->setType("LEMMA");
    l_lemma_array[268]->setType("NPVRB");

    // creation of lemma <ὅτι‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὅτι‣SCONJ>");
    l_lemma_array[269] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅτι");
    l_lemma_array[269]->setType("LEMMA");
    l_lemma_array[269]->setType("NPSCJ");

    // creation of lemma <ὑγιαίνω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὑγιαίνω‣VERB>");
    l_lemma_array[270] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὑγιαίνω");
    l_lemma_array[270]->setType("LEMMA");
    l_lemma_array[270]->setType("NPVRB");

    // creation of lemma <δεῖ‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <δεῖ‣VERB>");
    l_lemma_array[271] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δεῖ");
    l_lemma_array[271]->setType("LEMMA");
    l_lemma_array[271]->setType("NPVRB");

    // creation of lemma <γε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <γε‣ADV>");
    l_lemma_array[272] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γε");
    l_lemma_array[272]->setType("LEMMA");
    l_lemma_array[272]->setType("NPADV");

    // creation of lemma <δή‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <δή‣ADV>");
    l_lemma_array[273] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δή");
    l_lemma_array[273]->setType("LEMMA");
    l_lemma_array[273]->setType("NPADV");

    // creation of lemma <εἶμι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <εἶμι‣VERB>");
    l_lemma_array[274] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἶμι");
    l_lemma_array[274]->setType("LEMMA");
    l_lemma_array[274]->setType("NPVRB");

    // creation of lemma <δή‣PART>
    qCDebug(g_cat_silence) << QString("Creating lemma <δή‣PART>");
    l_lemma_array[275] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δή");
    l_lemma_array[275]->setType("LEMMA");
    l_lemma_array[275]->setType("NPPRT");

    // creation of lemma <ἐρόω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐρόω‣VERB>");
    l_lemma_array[276] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐρόω");
    l_lemma_array[276]->setType("LEMMA");
    l_lemma_array[276]->setType("NPVRB");

    // creation of lemma <πρός‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <πρός‣SCONJ>");
    l_lemma_array[277] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πρός");
    l_lemma_array[277]->setType("LEMMA");
    l_lemma_array[277]->setType("NPSCJ");

    // creation of lemma <βούλομαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <βούλομαι‣VERB>");
    l_lemma_array[278] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "βούλομαι");
    l_lemma_array[278]->setType("LEMMA");
    l_lemma_array[278]->setType("NPVRB");

    // creation of lemma <ὧδε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὧδε‣ADV>");
    l_lemma_array[279] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὧδε");
    l_lemma_array[279]->setType("LEMMA");
    l_lemma_array[279]->setType("NPADV");

    // creation of lemma <πυνθάνομαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <πυνθάνομαι‣VERB>");
    l_lemma_array[280] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πυνθάνομαι");
    l_lemma_array[280]->setType("LEMMA");
    l_lemma_array[280]->setType("NPVRB");

    // creation of lemma <παρά‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <παρά‣SCONJ>");
    l_lemma_array[281] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "παρά");
    l_lemma_array[281]->setType("LEMMA");
    l_lemma_array[281]->setType("NPSCJ");

    // creation of lemma <αὐτοῦ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating lemma <αὐτοῦ‣ADV>");
    l_lemma_array[282] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτοῦ");
    l_lemma_array[282]->setType("LEMMA");
    l_lemma_array[282]->setType("NPADV");

    // creation of lemma <εἰ‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <εἰ‣SCONJ>");
    l_lemma_array[283] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἰ");
    l_lemma_array[283]->setType("LEMMA");
    l_lemma_array[283]->setType("NPSCJ");

    // creation of lemma <οἶδα‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <οἶδα‣VERB>");
    l_lemma_array[284] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οἶδα");
    l_lemma_array[284]->setType("LEMMA");
    l_lemma_array[284]->setType("NPVRB");

    // creation of lemma <οὐδείς‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <οὐδείς‣ADJ>");
    l_lemma_array[285] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐδείς");
    l_lemma_array[285]->setType("LEMMA");
    l_lemma_array[285]->setType("NPADJ");

    // creation of lemma <φθόνος‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating lemma <φθόνος‣NOUN>");
    l_lemma_array[286] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "φθόνος");
    l_lemma_array[286]->setType("LEMMA");
    l_lemma_array[286]->setType("NPNON");

    // creation of lemma <ἕννυμι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἕννυμι‣VERB>");
    l_lemma_array[287] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἕννυμι");
    l_lemma_array[287]->setType("LEMMA");
    l_lemma_array[287]->setType("NPVRB");

    // creation of lemma <εἶδον‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <εἶδον‣VERB>");
    l_lemma_array[288] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἶδον");
    l_lemma_array[288]->setType("LEMMA");
    l_lemma_array[288]->setType("NPVRB");

    // creation of lemma <εἶπον‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <εἶπον‣VERB>");
    l_lemma_array[289] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἶπον");
    l_lemma_array[289]->setType("LEMMA");
    l_lemma_array[289]->setType("NPVRB");

    // creation of lemma <τίς‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <τίς‣ADJ>");
    l_lemma_array[290] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τίς");
    l_lemma_array[290]->setType("LEMMA");
    l_lemma_array[290]->setType("NPADJ");

    // creation of lemma <ὑπέρ‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὑπέρ‣SCONJ>");
    l_lemma_array[291] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὑπέρ");
    l_lemma_array[291]->setType("LEMMA");
    l_lemma_array[291]->setType("NPSCJ");

    // creation of lemma <ἐκεῖνος‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐκεῖνος‣ADJ>");
    l_lemma_array[292] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐκεῖνος");
    l_lemma_array[292]->setType("LEMMA");
    l_lemma_array[292]->setType("NPADJ");

    // creation of lemma <ἀποκρίνω‣VERB>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἀποκρίνω‣VERB>");
    l_lemma_array[293] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀποκρίνω");
    l_lemma_array[293]->setType("LEMMA");
    l_lemma_array[293]->setType("NPVRB");

    // creation of lemma <πότερος‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <πότερος‣ADJ>");
    l_lemma_array[294] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πότερος");
    l_lemma_array[294]->setType("LEMMA");
    l_lemma_array[294]->setType("NPADJ");

    // creation of lemma <ἐός‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ἐός‣ADJ>");
    l_lemma_array[295] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐός");
    l_lemma_array[295]->setType("LEMMA");
    l_lemma_array[295]->setType("NPADJ");

    // creation of lemma <ὅς‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating lemma <ὅς‣ADJ>");
    l_lemma_array[296] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅς");
    l_lemma_array[296]->setType("LEMMA");
    l_lemma_array[296]->setType("NPADJ");

    M1Store::Item_lv2* l_form_array[356];
    // creation of form <they‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <they‣PRP>");
    l_form_array[0] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "they");
    l_form_array[0]->setType("FORM_");
    l_form_array[0]->setType("NTPRP");
    l_form_array[0]->linkTo(l_lemma_array[0], "BLNGS");

    // creation of form <would‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <would‣MD>");
    l_form_array[1] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "would");
    l_form_array[1]->setType("FORM_");
    l_form_array[1]->setType("NTGMD");
    l_form_array[1]->linkTo(l_lemma_array[1], "BLNGS");

    // creation of form <indeed‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <indeed‣RB>");
    l_form_array[2] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "indeed");
    l_form_array[2]->setType("FORM_");
    l_form_array[2]->setType("NTGRB");
    l_form_array[2]->linkTo(l_lemma_array[2], "BLNGS");

    // creation of form <he‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <he‣PRP>");
    l_form_array[3] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "he");
    l_form_array[3]->setType("FORM_");
    l_form_array[3]->setType("NTPRP");
    l_form_array[3]->linkTo(l_lemma_array[3], "BLNGS");

    // creation of form <then‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <then‣RB>");
    l_form_array[4] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "then");
    l_form_array[4]->setType("FORM_");
    l_form_array[4]->setType("NTGRB");
    l_form_array[4]->linkTo(l_lemma_array[4], "BLNGS");

    // creation of form <who‣WP>
    qCDebug(g_cat_silence) << QString("Creating form <who‣WP>");
    l_form_array[5] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "who");
    l_form_array[5]->setType("FORM_");
    l_form_array[5]->setType("NTGWP");
    l_form_array[5]->linkTo(l_lemma_array[5], "BLNGS");

    // creation of form <believes‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <believes‣VBZ>");
    l_form_array[6] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "believes");
    l_form_array[6]->setType("FORM_");
    l_form_array[6]->setType("NTVBZ");
    l_form_array[6]->linkTo(l_lemma_array[6], "BLNGS");

    // creation of form <in‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <in‣IN>");
    l_form_array[7] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "in");
    l_form_array[7]->setType("FORM_");
    l_form_array[7]->setType("NTGIN");
    l_form_array[7]->linkTo(l_lemma_array[7], "BLNGS");

    // creation of form <beautiful‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <beautiful‣JJ>");
    l_form_array[8] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "beautiful");
    l_form_array[8]->setType("FORM_");
    l_form_array[8]->setType("NTGJJ");
    l_form_array[8]->linkTo(l_lemma_array[8], "BLNGS");

    // creation of form <things‣NNS>
    qCDebug(g_cat_silence) << QString("Creating form <things‣NNS>");
    l_form_array[9] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "things");
    l_form_array[9]->setType("FORM_");
    l_form_array[9]->setType("NTNNS");
    l_form_array[9]->linkTo(l_lemma_array[9], "BLNGS");

    // creation of form <but‣CC>
    qCDebug(g_cat_silence) << QString("Creating form <but‣CC>");
    l_form_array[10] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "but");
    l_form_array[10]->setType("FORM_");
    l_form_array[10]->setType("NTGCC");
    l_form_array[10]->linkTo(l_lemma_array[10], "BLNGS");

    // creation of form <neither‣CC>
    qCDebug(g_cat_silence) << QString("Creating form <neither‣CC>");
    l_form_array[11] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "neither");
    l_form_array[11]->setType("FORM_");
    l_form_array[11]->setType("NTGCC");
    l_form_array[11]->linkTo(l_lemma_array[11], "BLNGS");

    // creation of form <beauty‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <beauty‣NN>");
    l_form_array[12] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "beauty");
    l_form_array[12]->setType("FORM_");
    l_form_array[12]->setType("NTGNN");
    l_form_array[12]->linkTo(l_lemma_array[12], "BLNGS");

    // creation of form <itself‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <itself‣PRP>");
    l_form_array[13] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "itself");
    l_form_array[13]->setType("FORM_");
    l_form_array[13]->setType("NTPRP");
    l_form_array[13]->linkTo(l_lemma_array[13], "BLNGS");

    // creation of form <nor‣CC>
    qCDebug(g_cat_silence) << QString("Creating form <nor‣CC>");
    l_form_array[14] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "nor");
    l_form_array[14]->setType("FORM_");
    l_form_array[14]->setType("NTGCC");
    l_form_array[14]->linkTo(l_lemma_array[14], "BLNGS");

    // creation of form <is‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <is‣VBZ>");
    l_form_array[15] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "is");
    l_form_array[15]->setType("FORM_");
    l_form_array[15]->setType("NTVBZ");
    l_form_array[15]->linkTo(l_lemma_array[15], "BLNGS");

    // creation of form <able‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <able‣JJ>");
    l_form_array[16] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "able");
    l_form_array[16]->setType("FORM_");
    l_form_array[16]->setType("NTGJJ");
    l_form_array[16]->linkTo(l_lemma_array[16], "BLNGS");

    // creation of form <to‣TO>
    qCDebug(g_cat_silence) << QString("Creating form <to‣TO>");
    l_form_array[17] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "to");
    l_form_array[17]->setType("FORM_");
    l_form_array[17]->setType("NTGTO");
    l_form_array[17]->linkTo(l_lemma_array[17], "BLNGS");

    // creation of form <follow‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <follow‣VB>");
    l_form_array[18] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "follow");
    l_form_array[18]->setType("FORM_");
    l_form_array[18]->setType("NTGVB");
    l_form_array[18]->linkTo(l_lemma_array[18], "BLNGS");

    // creation of form <when‣WRB>
    qCDebug(g_cat_silence) << QString("Creating form <when‣WRB>");
    l_form_array[19] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "when");
    l_form_array[19]->setType("FORM_");
    l_form_array[19]->setType("NTWRB");
    l_form_array[19]->linkTo(l_lemma_array[19], "BLNGS");

    // creation of form <someone‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <someone‣NN>");
    l_form_array[20] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "someone");
    l_form_array[20]->setType("FORM_");
    l_form_array[20]->setType("NTGNN");
    l_form_array[20]->linkTo(l_lemma_array[20], "BLNGS");

    // creation of form <tries‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <tries‣VBZ>");
    l_form_array[21] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "tries");
    l_form_array[21]->setType("FORM_");
    l_form_array[21]->setType("NTVBZ");
    l_form_array[21]->linkTo(l_lemma_array[21], "BLNGS");

    // creation of form <guide‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <guide‣VB>");
    l_form_array[22] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "guide");
    l_form_array[22]->setType("FORM_");
    l_form_array[22]->setType("NTGVB");
    l_form_array[22]->linkTo(l_lemma_array[22], "BLNGS");

    // creation of form <him‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <him‣PRP>");
    l_form_array[23] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "him");
    l_form_array[23]->setType("FORM_");
    l_form_array[23]->setType("NTPRP");
    l_form_array[23]->linkTo(l_lemma_array[3], "BLNGS");

    // creation of form <to‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <to‣IN>");
    l_form_array[24] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "to");
    l_form_array[24]->setType("FORM_");
    l_form_array[24]->setType("NTGIN");
    l_form_array[24]->linkTo(l_lemma_array[23], "BLNGS");

    // creation of form <the‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <the‣DT>");
    l_form_array[25] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "the");
    l_form_array[25]->setType("FORM_");
    l_form_array[25]->setType("NTGDT");
    l_form_array[25]->linkTo(l_lemma_array[24], "BLNGS");

    // creation of form <knowledge‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <knowledge‣NN>");
    l_form_array[26] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "knowledge");
    l_form_array[26]->setType("FORM_");
    l_form_array[26]->setType("NTGNN");
    l_form_array[26]->linkTo(l_lemma_array[25], "BLNGS");

    // creation of form <of‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <of‣IN>");
    l_form_array[27] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "of");
    l_form_array[27]->setType("FORM_");
    l_form_array[27]->setType("NTGIN");
    l_form_array[27]->linkTo(l_lemma_array[26], "BLNGS");

    // creation of form <it‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <it‣PRP>");
    l_form_array[28] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "it");
    l_form_array[28]->setType("FORM_");
    l_form_array[28]->setType("NTPRP");
    l_form_array[28]->linkTo(l_lemma_array[27], "BLNGS");

    // creation of form <do‣VBP>
    qCDebug(g_cat_silence) << QString("Creating form <do‣VBP>");
    l_form_array[29] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "do");
    l_form_array[29]->setType("FORM_");
    l_form_array[29]->setType("NTVBP");
    l_form_array[29]->linkTo(l_lemma_array[28], "BLNGS");

    // creation of form <you‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <you‣PRP>");
    l_form_array[30] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "you");
    l_form_array[30]->setType("FORM_");
    l_form_array[30]->setType("NTPRP");
    l_form_array[30]->linkTo(l_lemma_array[29], "BLNGS");

    // creation of form <think‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <think‣VB>");
    l_form_array[31] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "think");
    l_form_array[31]->setType("FORM_");
    l_form_array[31]->setType("NTGVB");
    l_form_array[31]->linkTo(l_lemma_array[30], "BLNGS");

    // creation of form <that‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <that‣IN>");
    l_form_array[32] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "that");
    l_form_array[32]->setType("FORM_");
    l_form_array[32]->setType("NTGIN");
    l_form_array[32]->linkTo(l_lemma_array[31], "BLNGS");

    // creation of form <his‣PRP$>
    qCDebug(g_cat_silence) << QString("Creating form <his‣PRP$>");
    l_form_array[33] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "his");
    l_form_array[33]->setType("FORM_");
    l_form_array[33]->setType("NTPR$");
    l_form_array[33]->linkTo(l_lemma_array[32], "BLNGS");

    // creation of form <life‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <life‣NN>");
    l_form_array[34] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "life");
    l_form_array[34]->setType("FORM_");
    l_form_array[34]->setType("NTGNN");
    l_form_array[34]->linkTo(l_lemma_array[33], "BLNGS");

    // creation of form <a‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <a‣DT>");
    l_form_array[35] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "a");
    l_form_array[35]->setType("FORM_");
    l_form_array[35]->setType("NTGDT");
    l_form_array[35]->linkTo(l_lemma_array[34], "BLNGS");

    // creation of form <dream‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <dream‣NN>");
    l_form_array[36] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dream");
    l_form_array[36]->setType("FORM_");
    l_form_array[36]->setType("NTGNN");
    l_form_array[36]->linkTo(l_lemma_array[35], "BLNGS");

    // creation of form <or‣CC>
    qCDebug(g_cat_silence) << QString("Creating form <or‣CC>");
    l_form_array[37] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "or");
    l_form_array[37]->setType("FORM_");
    l_form_array[37]->setType("NTGCC");
    l_form_array[37]->linkTo(l_lemma_array[36], "BLNGS");

    // creation of form <waking‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <waking‣NN>");
    l_form_array[38] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "waking");
    l_form_array[38]->setType("FORM_");
    l_form_array[38]->setType("NTGNN");
    l_form_array[38]->linkTo(l_lemma_array[37], "BLNGS");

    // creation of form <just‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <just‣RB>");
    l_form_array[39] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "just");
    l_form_array[39]->setType("FORM_");
    l_form_array[39]->setType("NTGRB");
    l_form_array[39]->linkTo(l_lemma_array[38], "BLNGS");

    // creation of form <consider‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <consider‣VB>");
    l_form_array[40] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "consider");
    l_form_array[40]->setType("FORM_");
    l_form_array[40]->setType("NTGVB");
    l_form_array[40]->linkTo(l_lemma_array[39], "BLNGS");

    // creation of form <not‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <not‣RB>");
    l_form_array[41] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "not");
    l_form_array[41]->setType("FORM_");
    l_form_array[41]->setType("NTGRB");
    l_form_array[41]->linkTo(l_lemma_array[40], "BLNGS");

    // creation of form <state‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <state‣NN>");
    l_form_array[42] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "state");
    l_form_array[42]->setType("FORM_");
    l_form_array[42]->setType("NTGNN");
    l_form_array[42]->linkTo(l_lemma_array[41], "BLNGS");

    // creation of form <whether‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <whether‣IN>");
    l_form_array[43] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "whether");
    l_form_array[43]->setType("FORM_");
    l_form_array[43]->setType("NTGIN");
    l_form_array[43]->linkTo(l_lemma_array[42], "BLNGS");

    // creation of form <man‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <man‣NN>");
    l_form_array[44] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "man");
    l_form_array[44]->setType("FORM_");
    l_form_array[44]->setType("NTGNN");
    l_form_array[44]->linkTo(l_lemma_array[43], "BLNGS");

    // creation of form <asleep‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <asleep‣JJ>");
    l_form_array[45] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "asleep");
    l_form_array[45]->setType("FORM_");
    l_form_array[45]->setType("NTGJJ");
    l_form_array[45]->linkTo(l_lemma_array[44], "BLNGS");

    // creation of form <awake‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <awake‣JJ>");
    l_form_array[46] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "awake");
    l_form_array[46]->setType("FORM_");
    l_form_array[46]->setType("NTGJJ");
    l_form_array[46]->linkTo(l_lemma_array[45], "BLNGS");

    // creation of form <this‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <this‣DT>");
    l_form_array[47] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "this");
    l_form_array[47]->setType("FORM_");
    l_form_array[47]->setType("NTGDT");
    l_form_array[47]->linkTo(l_lemma_array[46], "BLNGS");

    // creation of form <mistaking‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <mistaking‣NN>");
    l_form_array[48] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "mistaking");
    l_form_array[48]->setType("FORM_");
    l_form_array[48]->setType("NTGNN");
    l_form_array[48]->linkTo(l_lemma_array[47], "BLNGS");

    // creation of form <resemblance‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <resemblance‣NN>");
    l_form_array[49] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "resemblance");
    l_form_array[49]->setType("FORM_");
    l_form_array[49]->setType("NTGNN");
    l_form_array[49]->linkTo(l_lemma_array[48], "BLNGS");

    // creation of form <for‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <for‣IN>");
    l_form_array[50] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "for");
    l_form_array[50]->setType("FORM_");
    l_form_array[50]->setType("NTGIN");
    l_form_array[50]->linkTo(l_lemma_array[49], "BLNGS");

    // creation of form <identity‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <identity‣NN>");
    l_form_array[51] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "identity");
    l_form_array[51]->setType("FORM_");
    l_form_array[51]->setType("NTGNN");
    l_form_array[51]->linkTo(l_lemma_array[50], "BLNGS");

    // creation of form <i‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <i‣PRP>");
    l_form_array[52] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "i");
    l_form_array[52]->setType("FORM_");
    l_form_array[52]->setType("NTPRP");
    l_form_array[52]->linkTo(l_lemma_array[51], "BLNGS");

    // creation of form <should‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <should‣MD>");
    l_form_array[53] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "should");
    l_form_array[53]->setType("FORM_");
    l_form_array[53]->setType("NTGMD");
    l_form_array[53]->linkTo(l_lemma_array[52], "BLNGS");

    // creation of form <certainly‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <certainly‣RB>");
    l_form_array[54] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "certainly");
    l_form_array[54]->setType("FORM_");
    l_form_array[54]->setType("NTGRB");
    l_form_array[54]->linkTo(l_lemma_array[53], "BLNGS");

    // creation of form <call‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <call‣VB>");
    l_form_array[55] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "call");
    l_form_array[55]->setType("FORM_");
    l_form_array[55]->setType("NTGVB");
    l_form_array[55]->linkTo(l_lemma_array[54], "BLNGS");

    // creation of form <that‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <that‣DT>");
    l_form_array[56] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "that");
    l_form_array[56]->setType("FORM_");
    l_form_array[56]->setType("NTGDT");
    l_form_array[56]->linkTo(l_lemma_array[55], "BLNGS");

    // creation of form <dreaming‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <dreaming‣NN>");
    l_form_array[57] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dreaming");
    l_form_array[57]->setType("FORM_");
    l_form_array[57]->setType("NTGNN");
    l_form_array[57]->linkTo(l_lemma_array[56], "BLNGS");

    // creation of form <said‣VBD>
    qCDebug(g_cat_silence) << QString("Creating form <said‣VBD>");
    l_form_array[58] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "said");
    l_form_array[58]->setType("FORM_");
    l_form_array[58]->setType("NTVBD");
    l_form_array[58]->linkTo(l_lemma_array[57], "BLNGS");

    // creation of form <well‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <well‣RB>");
    l_form_array[59] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "well");
    l_form_array[59]->setType("FORM_");
    l_form_array[59]->setType("NTGRB");
    l_form_array[59]->linkTo(l_lemma_array[58], "BLNGS");

    // creation of form <take‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <take‣VB>");
    l_form_array[60] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "take");
    l_form_array[60]->setType("FORM_");
    l_form_array[60]->setType("NTGVB");
    l_form_array[60]->linkTo(l_lemma_array[59], "BLNGS");

    // creation of form <opposite‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <opposite‣JJ>");
    l_form_array[61] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opposite");
    l_form_array[61]->setType("FORM_");
    l_form_array[61]->setType("NTGJJ");
    l_form_array[61]->linkTo(l_lemma_array[60], "BLNGS");

    // creation of form <case‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <case‣NN>");
    l_form_array[62] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "case");
    l_form_array[62]->setType("FORM_");
    l_form_array[62]->setType("NTGNN");
    l_form_array[62]->linkTo(l_lemma_array[61], "BLNGS");

    // creation of form <whose‣WP$>
    qCDebug(g_cat_silence) << QString("Creating form <whose‣WP$>");
    l_form_array[63] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "whose");
    l_form_array[63]->setType("FORM_");
    l_form_array[63]->setType("NTWP$");
    l_form_array[63]->linkTo(l_lemma_array[62], "BLNGS");

    // creation of form <thought‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <thought‣NN>");
    l_form_array[64] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "thought");
    l_form_array[64]->setType("FORM_");
    l_form_array[64]->setType("NTGNN");
    l_form_array[64]->linkTo(l_lemma_array[63], "BLNGS");

    // creation of form <recognizes‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <recognizes‣VBZ>");
    l_form_array[65] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "recognizes");
    l_form_array[65]->setType("FORM_");
    l_form_array[65]->setType("NTVBZ");
    l_form_array[65]->linkTo(l_lemma_array[64], "BLNGS");

    // creation of form <and‣CC>
    qCDebug(g_cat_silence) << QString("Creating form <and‣CC>");
    l_form_array[66] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "and");
    l_form_array[66]->setType("FORM_");
    l_form_array[66]->setType("NTGCC");
    l_form_array[66]->linkTo(l_lemma_array[65], "BLNGS");

    // creation of form <distinguish‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <distinguish‣VB>");
    l_form_array[67] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "distinguish");
    l_form_array[67]->setType("FORM_");
    l_form_array[67]->setType("NTGVB");
    l_form_array[67]->linkTo(l_lemma_array[66], "BLNGS");

    // creation of form <self-beautiful‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <self-beautiful‣NN>");
    l_form_array[68] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "self-beautiful");
    l_form_array[68]->setType("FORM_");
    l_form_array[68]->setType("NTGNN");
    l_form_array[68]->linkTo(l_lemma_array[67], "BLNGS");
    l_form_array[68]->linkTo(l_lemma_array[68], "BLNGS");
    l_form_array[68]->linkTo(l_lemma_array[8], "BLNGS");

    // creation of form <that‣WDT>
    qCDebug(g_cat_silence) << QString("Creating form <that‣WDT>");
    l_form_array[69] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "that");
    l_form_array[69]->setType("FORM_");
    l_form_array[69]->setType("NTWDT");
    l_form_array[69]->linkTo(l_lemma_array[55], "BLNGS");

    // creation of form <participate‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <participate‣VB>");
    l_form_array[70] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "participate");
    l_form_array[70]->setType("FORM_");
    l_form_array[70]->setType("NTGVB");
    l_form_array[70]->linkTo(l_lemma_array[69], "BLNGS");

    // creation of form <supposes‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <supposes‣VBZ>");
    l_form_array[71] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "supposes");
    l_form_array[71]->setType("FORM_");
    l_form_array[71]->setType("NTVBZ");
    l_form_array[71]->linkTo(l_lemma_array[70], "BLNGS");

    // creation of form <participants‣NNS>
    qCDebug(g_cat_silence) << QString("Creating form <participants‣NNS>");
    l_form_array[72] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "participants");
    l_form_array[72]->setType("FORM_");
    l_form_array[72]->setType("NTNNS");
    l_form_array[72]->linkTo(l_lemma_array[71], "BLNGS");

    // creation of form <be‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <be‣VB>");
    l_form_array[73] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "be");
    l_form_array[73]->setType("FORM_");
    l_form_array[73]->setType("NTGVB");
    l_form_array[73]->linkTo(l_lemma_array[15], "BLNGS");

    // creation of form <your‣PRP$>
    qCDebug(g_cat_silence) << QString("Creating form <your‣PRP$>");
    l_form_array[74] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "your");
    l_form_array[74]->setType("FORM_");
    l_form_array[74]->setType("NTPR$");
    l_form_array[74]->linkTo(l_lemma_array[72], "BLNGS");

    // creation of form <opinion‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <opinion‣NN>");
    l_form_array[75] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opinion");
    l_form_array[75]->setType("FORM_");
    l_form_array[75]->setType("NTGNN");
    l_form_array[75]->linkTo(l_lemma_array[73], "BLNGS");

    // creation of form <waking‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <waking‣VBG>");
    l_form_array[76] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "waking");
    l_form_array[76]->setType("FORM_");
    l_form_array[76]->setType("NTVBG");
    l_form_array[76]->linkTo(l_lemma_array[74], "BLNGS");

    // creation of form <very‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <very‣RB>");
    l_form_array[77] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "very");
    l_form_array[77]->setType("FORM_");
    l_form_array[77]->setType("NTGRB");
    l_form_array[77]->linkTo(l_lemma_array[75], "BLNGS");

    // creation of form <much‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <much‣RB>");
    l_form_array[78] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "much");
    l_form_array[78]->setType("FORM_");
    l_form_array[78]->setType("NTGRB");
    l_form_array[78]->linkTo(l_lemma_array[76], "BLNGS");

    // creation of form <replied‣VBD>
    qCDebug(g_cat_silence) << QString("Creating form <replied‣VBD>");
    l_form_array[79] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "replied");
    l_form_array[79]->setType("FORM_");
    l_form_array[79]->setType("NTVBD");
    l_form_array[79]->linkTo(l_lemma_array[77], "BLNGS");

    // creation of form <could‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <could‣MD>");
    l_form_array[80] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "could");
    l_form_array[80]->setType("FORM_");
    l_form_array[80]->setType("NTGMD");
    l_form_array[80]->linkTo(l_lemma_array[78], "BLNGS");

    // creation of form <we‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <we‣PRP>");
    l_form_array[81] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "we");
    l_form_array[81]->setType("FORM_");
    l_form_array[81]->setType("NTPRP");
    l_form_array[81]->linkTo(l_lemma_array[79], "BLNGS");

    // creation of form <rightly‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <rightly‣RB>");
    l_form_array[82] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "rightly");
    l_form_array[82]->setType("FORM_");
    l_form_array[82]->setType("NTGRB");
    l_form_array[82]->linkTo(l_lemma_array[80], "BLNGS");

    // creation of form <mental‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <mental‣JJ>");
    l_form_array[83] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "mental");
    l_form_array[83]->setType("FORM_");
    l_form_array[83]->setType("NTGJJ");
    l_form_array[83]->linkTo(l_lemma_array[81], "BLNGS");

    // creation of form <one‣CD>
    qCDebug(g_cat_silence) << QString("Creating form <one‣CD>");
    l_form_array[84] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "one");
    l_form_array[84]->setType("FORM_");
    l_form_array[84]->setType("NTGCD");
    l_form_array[84]->linkTo(l_lemma_array[82], "BLNGS");

    // creation of form <as‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <as‣IN>");
    l_form_array[85] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "as");
    l_form_array[85]->setType("FORM_");
    l_form_array[85]->setType("NTGIN");
    l_form_array[85]->linkTo(l_lemma_array[83], "BLNGS");

    // creation of form <knowing‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <knowing‣NN>");
    l_form_array[86] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "knowing");
    l_form_array[86]->setType("FORM_");
    l_form_array[86]->setType("NTGNN");
    l_form_array[86]->linkTo(l_lemma_array[84], "BLNGS");

    // creation of form <other‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <other‣JJ>");
    l_form_array[87] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "other");
    l_form_array[87]->setType("FORM_");
    l_form_array[87]->setType("NTGJJ");
    l_form_array[87]->linkTo(l_lemma_array[85], "BLNGS");

    // creation of form <opining‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <opining‣NN>");
    l_form_array[88] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opining");
    l_form_array[88]->setType("FORM_");
    l_form_array[88]->setType("NTGNN");
    l_form_array[88]->linkTo(l_lemma_array[86], "BLNGS");

    // creation of form <assuredly‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <assuredly‣RB>");
    l_form_array[89] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "assuredly");
    l_form_array[89]->setType("FORM_");
    l_form_array[89]->setType("NTGRB");
    l_form_array[89]->linkTo(l_lemma_array[87], "BLNGS");

    // creation of form <suppose‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <suppose‣VB>");
    l_form_array[90] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "suppose");
    l_form_array[90]->setType("FORM_");
    l_form_array[90]->setType("NTGVB");
    l_form_array[90]->linkTo(l_lemma_array[70], "BLNGS");

    // creation of form <now‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <now‣RB>");
    l_form_array[91] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "now");
    l_form_array[91]->setType("FORM_");
    l_form_array[91]->setType("NTGRB");
    l_form_array[91]->linkTo(l_lemma_array[88], "BLNGS");

    // creation of form <say‣VBP>
    qCDebug(g_cat_silence) << QString("Creating form <say‣VBP>");
    l_form_array[92] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "say");
    l_form_array[92]->setType("FORM_");
    l_form_array[92]->setType("NTVBP");
    l_form_array[92]->linkTo(l_lemma_array[57], "BLNGS");

    // creation of form <opines‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <opines‣VBZ>");
    l_form_array[93] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "opines");
    l_form_array[93]->setType("FORM_");
    l_form_array[93]->setType("NTVBZ");
    l_form_array[93]->linkTo(l_lemma_array[89], "BLNGS");

    // creation of form <does‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <does‣VBZ>");
    l_form_array[94] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "does");
    l_form_array[94]->setType("FORM_");
    l_form_array[94]->setType("NTVBZ");
    l_form_array[94]->linkTo(l_lemma_array[28], "BLNGS");

    // creation of form <know‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <know‣VB>");
    l_form_array[95] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "know");
    l_form_array[95]->setType("FORM_");
    l_form_array[95]->setType("NTGVB");
    l_form_array[95]->linkTo(l_lemma_array[90], "BLNGS");

    // creation of form <angry‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <angry‣JJ>");
    l_form_array[96] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "angry");
    l_form_array[96]->setType("FORM_");
    l_form_array[96]->setType("NTGJJ");
    l_form_array[96]->linkTo(l_lemma_array[91], "BLNGS");

    // creation of form <challenge‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <challenge‣VB>");
    l_form_array[97] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "challenge");
    l_form_array[97]->setType("FORM_");
    l_form_array[97]->setType("NTGVB");
    l_form_array[97]->linkTo(l_lemma_array[92], "BLNGS");

    // creation of form <our‣PRP$>
    qCDebug(g_cat_silence) << QString("Creating form <our‣PRP$>");
    l_form_array[98] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "our");
    l_form_array[98]->setType("FORM_");
    l_form_array[98]->setType("NTPR$");
    l_form_array[98]->linkTo(l_lemma_array[93], "BLNGS");

    // creation of form <statement‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <statement‣NN>");
    l_form_array[99] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "statement");
    l_form_array[99]->setType("FORM_");
    l_form_array[99]->setType("NTGNN");
    l_form_array[99]->linkTo(l_lemma_array[94], "BLNGS");

    // creation of form <true‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <true‣JJ>");
    l_form_array[100] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "true");
    l_form_array[100]->setType("FORM_");
    l_form_array[100]->setType("NTGJJ");
    l_form_array[100]->linkTo(l_lemma_array[95], "BLNGS");

    // creation of form <can‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <can‣MD>");
    l_form_array[101] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "can");
    l_form_array[101]->setType("FORM_");
    l_form_array[101]->setType("NTGMD");
    l_form_array[101]->linkTo(l_lemma_array[96], "BLNGS");

    // creation of form <find‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <find‣VB>");
    l_form_array[102] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "find");
    l_form_array[102]->setType("FORM_");
    l_form_array[102]->setType("NTGVB");
    l_form_array[102]->linkTo(l_lemma_array[97], "BLNGS");

    // creation of form <any‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <any‣DT>");
    l_form_array[103] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "any");
    l_form_array[103]->setType("FORM_");
    l_form_array[103]->setType("NTGDT");
    l_form_array[103]->linkTo(l_lemma_array[98], "BLNGS");

    // creation of form <way‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <way‣NN>");
    l_form_array[104] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "way");
    l_form_array[104]->setType("FORM_");
    l_form_array[104]->setType("NTGNN");
    l_form_array[104]->linkTo(l_lemma_array[99], "BLNGS");

    // creation of form <soothing‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <soothing‣VBG>");
    l_form_array[105] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "soothing");
    l_form_array[105]->setType("FORM_");
    l_form_array[105]->setType("NTVBG");
    l_form_array[105]->linkTo(l_lemma_array[100], "BLNGS");

    // creation of form <gently‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <gently‣RB>");
    l_form_array[106] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "gently");
    l_form_array[106]->setType("FORM_");
    l_form_array[106]->setType("NTGRB");
    l_form_array[106]->linkTo(l_lemma_array[101], "BLNGS");

    // creation of form <winning‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <winning‣VBG>");
    l_form_array[107] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "winning");
    l_form_array[107]->setType("FORM_");
    l_form_array[107]->setType("NTVBG");
    l_form_array[107]->linkTo(l_lemma_array[102], "BLNGS");

    // creation of form <over‣RP>
    qCDebug(g_cat_silence) << QString("Creating form <over‣RP>");
    l_form_array[108] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "over");
    l_form_array[108]->setType("FORM_");
    l_form_array[108]->setType("NTGRP");
    l_form_array[108]->linkTo(l_lemma_array[103], "BLNGS");

    // creation of form <without‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <without‣IN>");
    l_form_array[109] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "without");
    l_form_array[109]->setType("FORM_");
    l_form_array[109]->setType("NTGIN");
    l_form_array[109]->linkTo(l_lemma_array[104], "BLNGS");

    // creation of form <telling‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <telling‣VBG>");
    l_form_array[110] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "telling");
    l_form_array[110]->setType("FORM_");
    l_form_array[110]->setType("NTVBG");
    l_form_array[110]->linkTo(l_lemma_array[105], "BLNGS");

    // creation of form <too‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <too‣RB>");
    l_form_array[111] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "too");
    l_form_array[111]->setType("FORM_");
    l_form_array[111]->setType("NTGRB");
    l_form_array[111]->linkTo(l_lemma_array[106], "BLNGS");

    // creation of form <plainly‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <plainly‣RB>");
    l_form_array[112] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "plainly");
    l_form_array[112]->setType("FORM_");
    l_form_array[112]->setType("NTGRB");
    l_form_array[112]->linkTo(l_lemma_array[107], "BLNGS");

    // creation of form <right‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <right‣JJ>");
    l_form_array[113] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "right");
    l_form_array[113]->setType("FORM_");
    l_form_array[113]->setType("NTGJJ");
    l_form_array[113]->linkTo(l_lemma_array[108], "BLNGS");

    // creation of form <mind‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <mind‣NN>");
    l_form_array[114] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "mind");
    l_form_array[114]->setType("FORM_");
    l_form_array[114]->setType("NTGNN");
    l_form_array[114]->linkTo(l_lemma_array[109], "BLNGS");

    // creation of form <must‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <must‣MD>");
    l_form_array[115] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "must");
    l_form_array[115]->setType("FORM_");
    l_form_array[115]->setType("NTGMD");
    l_form_array[115]->linkTo(l_lemma_array[110], "BLNGS");

    // creation of form <try‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <try‣VB>");
    l_form_array[116] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "try");
    l_form_array[116]->setType("FORM_");
    l_form_array[116]->setType("NTGVB");
    l_form_array[116]->linkTo(l_lemma_array[21], "BLNGS");

    // creation of form <come‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <come‣VB>");
    l_form_array[117] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "come");
    l_form_array[117]->setType("FORM_");
    l_form_array[117]->setType("NTGVB");
    l_form_array[117]->linkTo(l_lemma_array[111], "BLNGS");

    // creation of form <what‣WP>
    qCDebug(g_cat_silence) << QString("Creating form <what‣WP>");
    l_form_array[118] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "what");
    l_form_array[118]->setType("FORM_");
    l_form_array[118]->setType("NTGWP");
    l_form_array[118]->linkTo(l_lemma_array[112], "BLNGS");

    // creation of form <are‣VBP>
    qCDebug(g_cat_silence) << QString("Creating form <are‣VBP>");
    l_form_array[119] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "are");
    l_form_array[119]->setType("FORM_");
    l_form_array[119]->setType("NTVBP");
    l_form_array[119]->linkTo(l_lemma_array[15], "BLNGS");

    // creation of form <say‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <say‣VB>");
    l_form_array[120] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "say");
    l_form_array[120]->setType("FORM_");
    l_form_array[120]->setType("NTGVB");
    l_form_array[120]->linkTo(l_lemma_array[57], "BLNGS");

    // creation of form <have‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <have‣VB>");
    l_form_array[121] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "have");
    l_form_array[121]->setType("FORM_");
    l_form_array[121]->setType("NTGVB");
    l_form_array[121]->linkTo(l_lemma_array[113], "BLNGS");

    // creation of form <us‣PRP>
    qCDebug(g_cat_silence) << QString("Creating form <us‣PRP>");
    l_form_array[122] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "us");
    l_form_array[122]->setType("FORM_");
    l_form_array[122]->setType("NTPRP");
    l_form_array[122]->linkTo(l_lemma_array[79], "BLNGS");

    // creation of form <question‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <question‣VB>");
    l_form_array[123] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "question");
    l_form_array[123]->setType("FORM_");
    l_form_array[123]->setType("NTGVB");
    l_form_array[123]->linkTo(l_lemma_array[114], "BLNGS");

    // creation of form <fashion‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <fashion‣NN>");
    l_form_array[124] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "fashion");
    l_form_array[124]->setType("FORM_");
    l_form_array[124]->setType("NTGNN");
    l_form_array[124]->linkTo(l_lemma_array[115], "BLNGS");

    // creation of form <premising‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <premising‣VBG>");
    l_form_array[125] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "premising");
    l_form_array[125]->setType("FORM_");
    l_form_array[125]->setType("NTVBG");
    l_form_array[125]->linkTo(l_lemma_array[116], "BLNGS");

    // creation of form <if‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <if‣IN>");
    l_form_array[126] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "if");
    l_form_array[126]->setType("FORM_");
    l_form_array[126]->setType("NTGIN");
    l_form_array[126]->linkTo(l_lemma_array[117], "BLNGS");

    // creation of form <knows‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <knows‣VBZ>");
    l_form_array[127] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "knows");
    l_form_array[127]->setType("FORM_");
    l_form_array[127]->setType("NTVBZ");
    l_form_array[127]->linkTo(l_lemma_array[90], "BLNGS");

    // creation of form <anything‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <anything‣NN>");
    l_form_array[128] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "anything");
    l_form_array[128]->setType("FORM_");
    l_form_array[128]->setType("NTGNN");
    l_form_array[128]->linkTo(l_lemma_array[118], "BLNGS");

    // creation of form <nobody‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <nobody‣NN>");
    l_form_array[129] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "nobody");
    l_form_array[129]->setType("FORM_");
    l_form_array[129]->setType("NTGNN");
    l_form_array[129]->linkTo(l_lemma_array[119], "BLNGS");

    // creation of form <grudges‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <grudges‣VBZ>");
    l_form_array[130] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "grudges");
    l_form_array[130]->setType("FORM_");
    l_form_array[130]->setType("NTVBZ");
    l_form_array[130]->linkTo(l_lemma_array[120], "BLNGS");

    // creation of form <glad‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <glad‣JJ>");
    l_form_array[131] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "glad");
    l_form_array[131]->setType("FORM_");
    l_form_array[131]->setType("NTGJJ");
    l_form_array[131]->linkTo(l_lemma_array[121], "BLNGS");

    // creation of form <see‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <see‣VB>");
    l_form_array[132] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "see");
    l_form_array[132]->setType("FORM_");
    l_form_array[132]->setType("NTGVB");
    l_form_array[132]->linkTo(l_lemma_array[122], "BLNGS");

    // creation of form <knowing‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <knowing‣VBG>");
    l_form_array[133] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "knowing");
    l_form_array[133]->setType("FORM_");
    l_form_array[133]->setType("NTVBG");
    l_form_array[133]->linkTo(l_lemma_array[90], "BLNGS");

    // creation of form <something‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <something‣NN>");
    l_form_array[134] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "something");
    l_form_array[134]->setType("FORM_");
    l_form_array[134]->setType("NTGNN");
    l_form_array[134]->linkTo(l_lemma_array[123], "BLNGS");

    // creation of form <tell‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <tell‣VB>");
    l_form_array[135] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "tell");
    l_form_array[135]->setType("FORM_");
    l_form_array[135]->setType("NTGVB");
    l_form_array[135]->linkTo(l_lemma_array[105], "BLNGS");

    // creation of form <nothing‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <nothing‣NN>");
    l_form_array[136] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "nothing");
    l_form_array[136]->setType("FORM_");
    l_form_array[136]->setType("NTGNN");
    l_form_array[136]->linkTo(l_lemma_array[124], "BLNGS");

    // creation of form <reply‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <reply‣VB>");
    l_form_array[137] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "reply");
    l_form_array[137]->setType("FORM_");
    l_form_array[137]->setType("NTGVB");
    l_form_array[137]->linkTo(l_lemma_array[77], "BLNGS");

    // creation of form <behalf‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <behalf‣NN>");
    l_form_array[138] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "behalf");
    l_form_array[138]->setType("FORM_");
    l_form_array[138]->setType("NTGNN");
    l_form_array[138]->linkTo(l_lemma_array[125], "BLNGS");

    // creation of form <will‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <will‣MD>");
    l_form_array[139] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "will");
    l_form_array[139]->setType("FORM_");
    l_form_array[139]->setType("NTGMD");
    l_form_array[139]->linkTo(l_lemma_array[126], "BLNGS");

    // creation of form <how‣WRB>
    qCDebug(g_cat_silence) << QString("Creating form <how‣WRB>");
    l_form_array[140] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "how");
    l_form_array[140]->setType("FORM_");
    l_form_array[140]->setType("NTWRB");
    l_form_array[140]->linkTo(l_lemma_array[127], "BLNGS");

    // creation of form <having‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <having‣VBG>");
    l_form_array[141] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "having");
    l_form_array[141]->setType("FORM_");
    l_form_array[141]->setType("NTVBG");
    l_form_array[141]->linkTo(l_lemma_array[113], "BLNGS");

    // creation of form <sense‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <sense‣NN>");
    l_form_array[142] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "sense");
    l_form_array[142]->setType("FORM_");
    l_form_array[142]->setType("NTGNN");
    l_form_array[142]->linkTo(l_lemma_array[128], "BLNGS");

    // creation of form <has‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <has‣VBZ>");
    l_form_array[143] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "has");
    l_form_array[143]->setType("FORM_");
    l_form_array[143]->setType("NTVBZ");
    l_form_array[143]->linkTo(l_lemma_array[113], "BLNGS");

    // creation of form <no‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <no‣DT>");
    l_form_array[144] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "no");
    l_form_array[144]->setType("FORM_");
    l_form_array[144]->setType("NTGDT");
    l_form_array[144]->linkTo(l_lemma_array[129], "BLNGS");

    // creation of form <absolute‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <absolute‣JJ>");
    l_form_array[145] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "absolute");
    l_form_array[145]->setType("FORM_");
    l_form_array[145]->setType("NTGJJ");
    l_form_array[145]->linkTo(l_lemma_array[130], "BLNGS");

    // creation of form <another‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <another‣DT>");
    l_form_array[146] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "another");
    l_form_array[146]->setType("FORM_");
    l_form_array[146]->setType("NTGDT");
    l_form_array[146]->linkTo(l_lemma_array[131], "BLNGS");

    // creation of form <lead‣VBP>
    qCDebug(g_cat_silence) << QString("Creating form <lead‣VBP>");
    l_form_array[147] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "lead");
    l_form_array[147]->setType("FORM_");
    l_form_array[147]->setType("NTVBP");
    l_form_array[147]->linkTo(l_lemma_array[132], "BLNGS");

    // creation of form <unable‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <unable‣JJ>");
    l_form_array[148] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "unable");
    l_form_array[148]->setType("FORM_");
    l_form_array[148]->setType("NTGJJ");
    l_form_array[148]->linkTo(l_lemma_array[133], "BLNGS");

    // creation of form <such‣PDT>
    qCDebug(g_cat_silence) << QString("Creating form <such‣PDT>");
    l_form_array[149] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "such");
    l_form_array[149]->setType("FORM_");
    l_form_array[149]->setType("NTPDT");
    l_form_array[149]->linkTo(l_lemma_array[134], "BLNGS");

    // creation of form <an‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <an‣DT>");
    l_form_array[150] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "an");
    l_form_array[150]->setType("FORM_");
    l_form_array[150]->setType("NTGDT");
    l_form_array[150]->linkTo(l_lemma_array[135], "BLNGS");

    // creation of form <ask‣VBP>
    qCDebug(g_cat_silence) << QString("Creating form <ask‣VBP>");
    l_form_array[151] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ask");
    l_form_array[151]->setType("FORM_");
    l_form_array[151]->setType("NTVBP");
    l_form_array[151]->linkTo(l_lemma_array[136], "BLNGS");

    // creation of form <only‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <only‣RB>");
    l_form_array[152] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "only");
    l_form_array[152]->setType("FORM_");
    l_form_array[152]->setType("NTGRB");
    l_form_array[152]->linkTo(l_lemma_array[137], "BLNGS");

    // creation of form <reflect‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <reflect‣VB>");
    l_form_array[153] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "reflect");
    l_form_array[153]->setType("FORM_");
    l_form_array[153]->setType("NTGVB");
    l_form_array[153]->linkTo(l_lemma_array[138], "BLNGS");

    // creation of form <dreamer‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <dreamer‣NN>");
    l_form_array[154] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dreamer");
    l_form_array[154]->setType("FORM_");
    l_form_array[154]->setType("NTGNN");
    l_form_array[154]->linkTo(l_lemma_array[139], "BLNGS");

    // creation of form <sleeping‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <sleeping‣VBG>");
    l_form_array[155] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "sleeping");
    l_form_array[155]->setType("FORM_");
    l_form_array[155]->setType("NTVBG");
    l_form_array[155]->linkTo(l_lemma_array[140], "BLNGS");

    // creation of form <likens‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <likens‣VBZ>");
    l_form_array[156] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "likens");
    l_form_array[156]->setType("FORM_");
    l_form_array[156]->setType("NTVBZ");
    l_form_array[156]->linkTo(l_lemma_array[141], "BLNGS");

    // creation of form <dissimilar‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <dissimilar‣JJ>");
    l_form_array[157] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dissimilar");
    l_form_array[157]->setType("FORM_");
    l_form_array[157]->setType("NTGJJ");
    l_form_array[157]->linkTo(l_lemma_array[142], "BLNGS");

    // creation of form <puts‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <puts‣VBZ>");
    l_form_array[158] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "puts");
    l_form_array[158]->setType("FORM_");
    l_form_array[158]->setType("NTVBZ");
    l_form_array[158]->linkTo(l_lemma_array[143], "BLNGS");

    // creation of form <copy‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <copy‣NN>");
    l_form_array[159] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "copy");
    l_form_array[159]->setType("FORM_");
    l_form_array[159]->setType("NTGNN");
    l_form_array[159]->linkTo(l_lemma_array[144], "BLNGS");

    // creation of form <place‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <place‣NN>");
    l_form_array[160] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "place");
    l_form_array[160]->setType("FORM_");
    l_form_array[160]->setType("NTGNN");
    l_form_array[160]->linkTo(l_lemma_array[145], "BLNGS");

    // creation of form <real‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <real‣JJ>");
    l_form_array[161] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "real");
    l_form_array[161]->setType("FORM_");
    l_form_array[161]->setType("NTGJJ");
    l_form_array[161]->linkTo(l_lemma_array[146], "BLNGS");

    // creation of form <object‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <object‣NN>");
    l_form_array[162] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "object");
    l_form_array[162]->setType("FORM_");
    l_form_array[162]->setType("NTGNN");
    l_form_array[162]->linkTo(l_lemma_array[147], "BLNGS");

    // creation of form <was‣VBD>
    qCDebug(g_cat_silence) << QString("Creating form <was‣VBD>");
    l_form_array[163] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "was");
    l_form_array[163]->setType("FORM_");
    l_form_array[163]->setType("NTVBD");
    l_form_array[163]->linkTo(l_lemma_array[15], "BLNGS");

    // creation of form <dreaming‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <dreaming‣VBG>");
    l_form_array[164] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dreaming");
    l_form_array[164]->setType("FORM_");
    l_form_array[164]->setType("NTVBG");
    l_form_array[164]->linkTo(l_lemma_array[148], "BLNGS");

    // creation of form <recognises‣VBZ>
    qCDebug(g_cat_silence) << QString("Creating form <recognises‣VBZ>");
    l_form_array[165] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "recognises");
    l_form_array[165]->setType("FORM_");
    l_form_array[165]->setType("NTVBZ");
    l_form_array[165]->linkTo(l_lemma_array[149], "BLNGS");

    // creation of form <existence‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <existence‣NN>");
    l_form_array[166] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "existence");
    l_form_array[166]->setType("FORM_");
    l_form_array[166]->setType("NTGNN");
    l_form_array[166]->linkTo(l_lemma_array[150], "BLNGS");

    // creation of form <idea‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <idea‣NN>");
    l_form_array[167] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "idea");
    l_form_array[167]->setType("FORM_");
    l_form_array[167]->setType("NTGNN");
    l_form_array[167]->linkTo(l_lemma_array[151], "BLNGS");

    // creation of form <from‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <from‣IN>");
    l_form_array[168] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "from");
    l_form_array[168]->setType("FORM_");
    l_form_array[168]->setType("NTGIN");
    l_form_array[168]->linkTo(l_lemma_array[152], "BLNGS");

    // creation of form <objects‣NNS>
    qCDebug(g_cat_silence) << QString("Creating form <objects‣NNS>");
    l_form_array[169] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "objects");
    l_form_array[169]->setType("FORM_");
    l_form_array[169]->setType("NTNNS");
    l_form_array[169]->linkTo(l_lemma_array[147], "BLNGS");

    // creation of form <which‣WDT>
    qCDebug(g_cat_silence) << QString("Creating form <which‣WDT>");
    l_form_array[170] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "which");
    l_form_array[170]->setType("FORM_");
    l_form_array[170]->setType("NTWDT");
    l_form_array[170]->linkTo(l_lemma_array[153], "BLNGS");

    // creation of form <participate‣VBP>
    qCDebug(g_cat_silence) << QString("Creating form <participate‣VBP>");
    l_form_array[171] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "participate");
    l_form_array[171]->setType("FORM_");
    l_form_array[171]->setType("NTVBP");
    l_form_array[171]->linkTo(l_lemma_array[69], "BLNGS");

    // creation of form <putting‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <putting‣VBG>");
    l_form_array[172] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "putting");
    l_form_array[172]->setType("FORM_");
    l_form_array[172]->setType("NTVBG");
    l_form_array[172]->linkTo(l_lemma_array[143], "BLNGS");

    // creation of form <wide‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <wide‣RB>");
    l_form_array[173] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "wide");
    l_form_array[173]->setType("FORM_");
    l_form_array[173]->setType("NTGRB");
    l_form_array[173]->linkTo(l_lemma_array[154], "BLNGS");

    // creation of form <may‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <may‣MD>");
    l_form_array[174] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "may");
    l_form_array[174]->setType("FORM_");
    l_form_array[174]->setType("NTGMD");
    l_form_array[174]->linkTo(l_lemma_array[155], "BLNGS");

    // creation of form <one‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <one‣NN>");
    l_form_array[175] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "one");
    l_form_array[175]->setType("FORM_");
    l_form_array[175]->setType("NTGNN");
    l_form_array[175]->linkTo(l_lemma_array[156], "BLNGS");

    // creation of form <latter‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <latter‣JJ>");
    l_form_array[176] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "latter");
    l_form_array[176]->setType("FORM_");
    l_form_array[176]->setType("NTGJJ");
    l_form_array[176]->linkTo(l_lemma_array[157], "BLNGS");

    // creation of form <quarrel‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <quarrel‣VB>");
    l_form_array[177] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "quarrel");
    l_form_array[177]->setType("FORM_");
    l_form_array[177]->setType("NTGVB");
    l_form_array[177]->linkTo(l_lemma_array[158], "BLNGS");

    // creation of form <with‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <with‣IN>");
    l_form_array[178] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "with");
    l_form_array[178]->setType("FORM_");
    l_form_array[178]->setType("NTGIN");
    l_form_array[178]->linkTo(l_lemma_array[159], "BLNGS");

    // creation of form <dispute‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <dispute‣VB>");
    l_form_array[179] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "dispute");
    l_form_array[179]->setType("FORM_");
    l_form_array[179]->setType("NTGVB");
    l_form_array[179]->linkTo(l_lemma_array[160], "BLNGS");

    // creation of form <administer‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <administer‣VB>");
    l_form_array[180] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "administer");
    l_form_array[180]->setType("FORM_");
    l_form_array[180]->setType("NTGVB");
    l_form_array[180]->linkTo(l_lemma_array[161], "BLNGS");

    // creation of form <soothing‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <soothing‣JJ>");
    l_form_array[181] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "soothing");
    l_form_array[181]->setType("FORM_");
    l_form_array[181]->setType("NTGJJ");
    l_form_array[181]->linkTo(l_lemma_array[162], "BLNGS");

    // creation of form <cordial‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <cordial‣NN>");
    l_form_array[182] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "cordial");
    l_form_array[182]->setType("FORM_");
    l_form_array[182]->setType("NTGNN");
    l_form_array[182]->linkTo(l_lemma_array[163], "BLNGS");

    // creation of form <advice‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <advice‣NN>");
    l_form_array[183] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "advice");
    l_form_array[183]->setType("FORM_");
    l_form_array[183]->setType("NTGNN");
    l_form_array[183]->linkTo(l_lemma_array[164], "BLNGS");

    // creation of form <revealing‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <revealing‣VBG>");
    l_form_array[184] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "revealing");
    l_form_array[184]->setType("FORM_");
    l_form_array[184]->setType("NTVBG");
    l_form_array[184]->linkTo(l_lemma_array[165], "BLNGS");

    // creation of form <there‣EX>
    qCDebug(g_cat_silence) << QString("Creating form <there‣EX>");
    l_form_array[185] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "there");
    l_form_array[185]->setType("FORM_");
    l_form_array[185]->setType("NTGEX");
    l_form_array[185]->linkTo(l_lemma_array[166], "BLNGS");

    // creation of form <sad‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <sad‣JJ>");
    l_form_array[186] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "sad");
    l_form_array[186]->setType("FORM_");
    l_form_array[186]->setType("NTGJJ");
    l_form_array[186]->linkTo(l_lemma_array[167], "BLNGS");

    // creation of form <disorder‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <disorder‣NN>");
    l_form_array[187] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "disorder");
    l_form_array[187]->setType("FORM_");
    l_form_array[187]->setType("NTGNN");
    l_form_array[187]->linkTo(l_lemma_array[168], "BLNGS");

    // creation of form <wits‣NNS>
    qCDebug(g_cat_silence) << QString("Creating form <wits‣NNS>");
    l_form_array[188] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "wits");
    l_form_array[188]->setType("FORM_");
    l_form_array[188]->setType("NTNNS");
    l_form_array[188]->linkTo(l_lemma_array[169], "BLNGS");

    // creation of form <offer‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <offer‣VB>");
    l_form_array[189] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "offer");
    l_form_array[189]->setType("FORM_");
    l_form_array[189]->setType("NTGVB");
    l_form_array[189]->linkTo(l_lemma_array[170], "BLNGS");

    // creation of form <some‣DT>
    qCDebug(g_cat_silence) << QString("Creating form <some‣DT>");
    l_form_array[190] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "some");
    l_form_array[190]->setType("FORM_");
    l_form_array[190]->setType("NTGDT");
    l_form_array[190]->linkTo(l_lemma_array[171], "BLNGS");

    // creation of form <good‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <good‣JJ>");
    l_form_array[191] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "good");
    l_form_array[191]->setType("FORM_");
    l_form_array[191]->setType("NTGJJ");
    l_form_array[191]->linkTo(l_lemma_array[172], "BLNGS");

    // creation of form <let‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <let‣VB>");
    l_form_array[192] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "let");
    l_form_array[192]->setType("FORM_");
    l_form_array[192]->setType("NTGVB");
    l_form_array[192]->linkTo(l_lemma_array[173], "BLNGS");

    // creation of form <shall‣MD>
    qCDebug(g_cat_silence) << QString("Creating form <shall‣MD>");
    l_form_array[193] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "shall");
    l_form_array[193]->setType("FORM_");
    l_form_array[193]->setType("NTGMD");
    l_form_array[193]->linkTo(l_lemma_array[174], "BLNGS");

    // creation of form <begin‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <begin‣VB>");
    l_form_array[194] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "begin");
    l_form_array[194]->setType("FORM_");
    l_form_array[194]->setType("NTGVB");
    l_form_array[194]->linkTo(l_lemma_array[175], "BLNGS");

    // creation of form <by‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <by‣IN>");
    l_form_array[195] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "by");
    l_form_array[195]->setType("FORM_");
    l_form_array[195]->setType("NTGIN");
    l_form_array[195]->linkTo(l_lemma_array[176], "BLNGS");

    // creation of form <assuring‣VBG>
    qCDebug(g_cat_silence) << QString("Creating form <assuring‣VBG>");
    l_form_array[196] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "assuring");
    l_form_array[196]->setType("FORM_");
    l_form_array[196]->setType("NTVBG");
    l_form_array[196]->linkTo(l_lemma_array[177], "BLNGS");

    // creation of form <welcome‣JJ>
    qCDebug(g_cat_silence) << QString("Creating form <welcome‣JJ>");
    l_form_array[197] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "welcome");
    l_form_array[197]->setType("FORM_");
    l_form_array[197]->setType("NTGJJ");
    l_form_array[197]->linkTo(l_lemma_array[178], "BLNGS");

    // creation of form <rejoiced‣VBN>
    qCDebug(g_cat_silence) << QString("Creating form <rejoiced‣VBN>");
    l_form_array[198] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "rejoiced");
    l_form_array[198]->setType("FORM_");
    l_form_array[198]->setType("NTVBN");
    l_form_array[198]->linkTo(l_lemma_array[179], "BLNGS");

    // creation of form <at‣IN>
    qCDebug(g_cat_silence) << QString("Creating form <at‣IN>");
    l_form_array[199] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "at");
    l_form_array[199]->setType("FORM_");
    l_form_array[199]->setType("NTGIN");
    l_form_array[199]->linkTo(l_lemma_array[180], "BLNGS");

    // creation of form <like‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <like‣VB>");
    l_form_array[200] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "like");
    l_form_array[200]->setType("FORM_");
    l_form_array[200]->setType("NTGVB");
    l_form_array[200]->linkTo(l_lemma_array[181], "BLNGS");

    // creation of form <ask‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <ask‣VB>");
    l_form_array[201] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ask");
    l_form_array[201]->setType("FORM_");
    l_form_array[201]->setType("NTGVB");
    l_form_array[201]->linkTo(l_lemma_array[136], "BLNGS");

    // creation of form <question‣NN>
    qCDebug(g_cat_silence) << QString("Creating form <question‣NN>");
    l_form_array[202] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "question");
    l_form_array[202]->setType("FORM_");
    l_form_array[202]->setType("NTGNN");
    l_form_array[202]->linkTo(l_lemma_array[182], "BLNGS");

    // creation of form <answer‣VB>
    qCDebug(g_cat_silence) << QString("Creating form <answer‣VB>");
    l_form_array[203] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "answer");
    l_form_array[203]->setType("FORM_");
    l_form_array[203]->setType("NTGVB");
    l_form_array[203]->linkTo(l_lemma_array[183], "BLNGS");

    // creation of form <answer‣VBP>
    qCDebug(g_cat_silence) << QString("Creating form <answer‣VBP>");
    l_form_array[204] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "answer");
    l_form_array[204]->setType("FORM_");
    l_form_array[204]->setType("NTVBP");
    l_form_array[204]->linkTo(l_lemma_array[183], "BLNGS");

    // creation of form <for‣CC>
    qCDebug(g_cat_silence) << QString("Creating form <for‣CC>");
    l_form_array[205] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "for");
    l_form_array[205]->setType("FORM_");
    l_form_array[205]->setType("NTGCC");
    l_form_array[205]->linkTo(l_lemma_array[184], "BLNGS");

    // creation of form <ever‣RB>
    qCDebug(g_cat_silence) << QString("Creating form <ever‣RB>");
    l_form_array[206] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ever");
    l_form_array[206]->setType("FORM_");
    l_form_array[206]->setType("NTGRB");
    l_form_array[206]->linkTo(l_lemma_array[185], "BLNGS");

    // creation of form <known‣VBN>
    qCDebug(g_cat_silence) << QString("Creating form <known‣VBN>");
    l_form_array[207] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "known");
    l_form_array[207]->setType("FORM_");
    l_form_array[207]->setType("NTVBN");
    l_form_array[207]->linkTo(l_lemma_array[90], "BLNGS");

    // creation of form <καὶ‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <καὶ‣CCONJ>");
    l_form_array[208] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καὶ");
    l_form_array[208]->setType("FORM_");
    l_form_array[208]->setType("NPCNJ");
    l_form_array[208]->linkTo(l_lemma_array[186], "BLNGS");

    // creation of form <μάλα‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <μάλα‣ADV>");
    l_form_array[209] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μάλα");
    l_form_array[209]->setType("FORM_");
    l_form_array[209]->setType("NPADV");
    l_form_array[209]->linkTo(l_lemma_array[187], "BLNGS");

    // creation of form <ὁ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <ὁ‣PRON>");
    l_form_array[210] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὁ");
    l_form_array[210]->setType("FORM_");
    l_form_array[210]->setType("NPPRN");
    l_form_array[210]->linkTo(l_lemma_array[188], "BLNGS");

    // creation of form <οὖν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <οὖν‣ADV>");
    l_form_array[211] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὖν");
    l_form_array[211]->setType("FORM_");
    l_form_array[211]->setType("NPADV");
    l_form_array[211]->linkTo(l_lemma_array[189], "BLNGS");

    // creation of form <καλὰ‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <καλὰ‣ADJ>");
    l_form_array[212] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καλὰ");
    l_form_array[212]->setType("FORM_");
    l_form_array[212]->setType("NPADJ");
    l_form_array[212]->linkTo(l_lemma_array[190], "BLNGS");

    // creation of form <μὲν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <μὲν‣ADV>");
    l_form_array[213] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μὲν");
    l_form_array[213]->setType("FORM_");
    l_form_array[213]->setType("NPADV");
    l_form_array[213]->linkTo(l_lemma_array[191], "BLNGS");

    // creation of form <πράγματα‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <πράγματα‣NOUN>");
    l_form_array[214] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πράγματα");
    l_form_array[214]->setType("FORM_");
    l_form_array[214]->setType("NPNON");
    l_form_array[214]->linkTo(l_lemma_array[192], "BLNGS");

    // creation of form <νομίζων‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <νομίζων‣VERB>");
    l_form_array[215] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "νομίζων");
    l_form_array[215]->setType("FORM_");
    l_form_array[215]->setType("NPVRB");
    l_form_array[215]->linkTo(l_lemma_array[193], "BLNGS");

    // creation of form <αὐτὸ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <αὐτὸ‣PRON>");
    l_form_array[216] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτὸ");
    l_form_array[216]->setType("FORM_");
    l_form_array[216]->setType("NPPRN");
    l_form_array[216]->linkTo(l_lemma_array[194], "BLNGS");

    // creation of form <δὲ‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <δὲ‣CCONJ>");
    l_form_array[217] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δὲ");
    l_form_array[217]->setType("FORM_");
    l_form_array[217]->setType("NPCNJ");
    l_form_array[217]->linkTo(l_lemma_array[195], "BLNGS");

    // creation of form <κάλλος‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <κάλλος‣NOUN>");
    l_form_array[218] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "κάλλος");
    l_form_array[218]->setType("FORM_");
    l_form_array[218]->setType("NPNON");
    l_form_array[218]->linkTo(l_lemma_array[196], "BLNGS");

    // creation of form <μήτε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <μήτε‣ADV>");
    l_form_array[219] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μήτε");
    l_form_array[219]->setType("FORM_");
    l_form_array[219]->setType("NPADV");
    l_form_array[219]->linkTo(l_lemma_array[197], "BLNGS");

    // creation of form <ἄν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <ἄν‣ADV>");
    l_form_array[220] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἄν");
    l_form_array[220]->setType("FORM_");
    l_form_array[220]->setType("NPADV");
    l_form_array[220]->linkTo(l_lemma_array[198], "BLNGS");

    // creation of form <τις‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τις‣PRON>");
    l_form_array[221] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τις");
    l_form_array[221]->setType("FORM_");
    l_form_array[221]->setType("NPPRN");
    l_form_array[221]->linkTo(l_lemma_array[199], "BLNGS");

    // creation of form <ἡγῆται‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἡγῆται‣VERB>");
    l_form_array[222] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἡγῆται");
    l_form_array[222]->setType("FORM_");
    l_form_array[222]->setType("NPVRB");
    l_form_array[222]->linkTo(l_lemma_array[200], "BLNGS");

    // creation of form <ἐπὶ‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ἐπὶ‣SCONJ>");
    l_form_array[223] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐπὶ");
    l_form_array[223]->setType("FORM_");
    l_form_array[223]->setType("NPSCJ");
    l_form_array[223]->linkTo(l_lemma_array[201], "BLNGS");

    // creation of form <τὴν‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τὴν‣PRON>");
    l_form_array[224] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τὴν");
    l_form_array[224]->setType("FORM_");
    l_form_array[224]->setType("NPPRN");
    l_form_array[224]->linkTo(l_lemma_array[188], "BLNGS");

    // creation of form <γνῶσιν‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <γνῶσιν‣NOUN>");
    l_form_array[225] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γνῶσιν");
    l_form_array[225]->setType("FORM_");
    l_form_array[225]->setType("NPNON");
    l_form_array[225]->linkTo(l_lemma_array[202], "BLNGS");

    // creation of form <αὐτοῦ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <αὐτοῦ‣PRON>");
    l_form_array[226] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτοῦ");
    l_form_array[226]->setType("FORM_");
    l_form_array[226]->setType("NPPRN");
    l_form_array[226]->linkTo(l_lemma_array[194], "BLNGS");

    // creation of form <δυνάμενος‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <δυνάμενος‣VERB>");
    l_form_array[227] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δυνάμενος");
    l_form_array[227]->setType("FORM_");
    l_form_array[227]->setType("NPVRB");
    l_form_array[227]->linkTo(l_lemma_array[203], "BLNGS");

    // creation of form <ἕπεσθαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἕπεσθαι‣VERB>");
    l_form_array[228] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἕπεσθαι");
    l_form_array[228]->setType("FORM_");
    l_form_array[228]->setType("NPVRB");
    l_form_array[228]->linkTo(l_lemma_array[204], "BLNGS");

    // creation of form <ὄναρ‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <ὄναρ‣NOUN>");
    l_form_array[229] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὄναρ");
    l_form_array[229]->setType("FORM_");
    l_form_array[229]->setType("NPNON");
    l_form_array[229]->linkTo(l_lemma_array[205], "BLNGS");

    // creation of form <ἢ‣CCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ἢ‣CCONJ>");
    l_form_array[230] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἢ");
    l_form_array[230]->setType("FORM_");
    l_form_array[230]->setType("NPCNJ");
    l_form_array[230]->linkTo(l_lemma_array[206], "BLNGS");

    // creation of form <ὕπαρ‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <ὕπαρ‣NOUN>");
    l_form_array[231] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὕπαρ");
    l_form_array[231]->setType("FORM_");
    l_form_array[231]->setType("NPNON");
    l_form_array[231]->linkTo(l_lemma_array[207], "BLNGS");

    // creation of form <δοκεῖ‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <δοκεῖ‣VERB>");
    l_form_array[232] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δοκεῖ");
    l_form_array[232]->setType("FORM_");
    l_form_array[232]->setType("NPVRB");
    l_form_array[232]->linkTo(l_lemma_array[208], "BLNGS");

    // creation of form <σοι‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <σοι‣PRON>");
    l_form_array[233] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "σοι");
    l_form_array[233]->setType("FORM_");
    l_form_array[233]->setType("NPPRN");
    l_form_array[233]->linkTo(l_lemma_array[209], "BLNGS");

    // creation of form <ζῆν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ζῆν‣VERB>");
    l_form_array[234] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ζῆν");
    l_form_array[234]->setType("FORM_");
    l_form_array[234]->setType("NPVRB");
    l_form_array[234]->linkTo(l_lemma_array[210], "BLNGS");

    // creation of form <σκόπει‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <σκόπει‣VERB>");
    l_form_array[235] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "σκόπει");
    l_form_array[235]->setType("FORM_");
    l_form_array[235]->setType("NPVRB");
    l_form_array[235]->linkTo(l_lemma_array[211], "BLNGS");

    // creation of form <δέ‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <δέ‣PART>");
    l_form_array[236] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δέ");
    l_form_array[236]->setType("FORM_");
    l_form_array[236]->setType("NPPRT");
    l_form_array[236]->linkTo(l_lemma_array[212], "BLNGS");

    // creation of form <τὸ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τὸ‣PRON>");
    l_form_array[237] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τὸ");
    l_form_array[237]->setType("FORM_");
    l_form_array[237]->setType("NPPRN");
    l_form_array[237]->linkTo(l_lemma_array[188], "BLNGS");

    // creation of form <ὀνειρώττειν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ὀνειρώττειν‣VERB>");
    l_form_array[238] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὀνειρώττειν");
    l_form_array[238]->setType("FORM_");
    l_form_array[238]->setType("NPVRB");
    l_form_array[238]->linkTo(l_lemma_array[213], "BLNGS");

    // creation of form <ἆρα‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <ἆρα‣PART>");
    l_form_array[239] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἆρα");
    l_form_array[239]->setType("FORM_");
    l_form_array[239]->setType("NPPRT");
    l_form_array[239]->linkTo(l_lemma_array[214], "BLNGS");

    // creation of form <οὐ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <οὐ‣ADV>");
    l_form_array[240] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐ");
    l_form_array[240]->setType("FORM_");
    l_form_array[240]->setType("NPADV");
    l_form_array[240]->linkTo(l_lemma_array[215], "BLNGS");

    // creation of form <τόδε‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τόδε‣PRON>");
    l_form_array[241] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τόδε");
    l_form_array[241]->setType("FORM_");
    l_form_array[241]->setType("NPPRN");
    l_form_array[241]->linkTo(l_lemma_array[216], "BLNGS");

    // creation of form <ἐστίν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἐστίν‣VERB>");
    l_form_array[242] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐστίν");
    l_form_array[242]->setType("FORM_");
    l_form_array[242]->setType("NPVRB");
    l_form_array[242]->linkTo(l_lemma_array[217], "BLNGS");

    // creation of form <ἐάντε‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ἐάντε‣SCONJ>");
    l_form_array[243] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐάντε");
    l_form_array[243]->setType("FORM_");
    l_form_array[243]->setType("NPSCJ");
    l_form_array[243]->linkTo(l_lemma_array[218], "BLNGS");

    // creation of form <ἐν‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ἐν‣SCONJ>");
    l_form_array[244] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐν");
    l_form_array[244]->setType("FORM_");
    l_form_array[244]->setType("NPSCJ");
    l_form_array[244]->linkTo(l_lemma_array[219], "BLNGS");

    // creation of form <ὕπνῳ‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <ὕπνῳ‣NOUN>");
    l_form_array[245] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὕπνῳ");
    l_form_array[245]->setType("FORM_");
    l_form_array[245]->setType("NPNON");
    l_form_array[245]->linkTo(l_lemma_array[220], "BLNGS");

    // creation of form <ἐάντ᾽‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἐάντ᾽‣VERB>");
    l_form_array[246] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐάντ᾽");
    l_form_array[246]->setType("FORM_");
    l_form_array[246]->setType("NPVRB");
    l_form_array[246]->linkTo(l_lemma_array[221], "BLNGS");

    // creation of form <ἐγρηγορὼς‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἐγρηγορὼς‣VERB>");
    l_form_array[247] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐγρηγορὼς");
    l_form_array[247]->setType("FORM_");
    l_form_array[247]->setType("NPVRB");
    l_form_array[247]->linkTo(l_lemma_array[222], "BLNGS");

    // creation of form <ὅμοιόν‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <ὅμοιόν‣ADJ>");
    l_form_array[248] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅμοιόν");
    l_form_array[248]->setType("FORM_");
    l_form_array[248]->setType("NPADJ");
    l_form_array[248]->linkTo(l_lemma_array[223], "BLNGS");

    // creation of form <τῳ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τῳ‣PRON>");
    l_form_array[249] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τῳ");
    l_form_array[249]->setType("FORM_");
    l_form_array[249]->setType("NPPRN");
    l_form_array[249]->linkTo(l_lemma_array[199], "BLNGS");

    // creation of form <μὴ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <μὴ‣ADV>");
    l_form_array[250] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μὴ");
    l_form_array[250]->setType("FORM_");
    l_form_array[250]->setType("NPADV");
    l_form_array[250]->linkTo(l_lemma_array[224], "BLNGS");

    // creation of form <ὅμοιον‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <ὅμοιον‣ADJ>");
    l_form_array[251] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅμοιον");
    l_form_array[251]->setType("FORM_");
    l_form_array[251]->setType("NPADJ");
    l_form_array[251]->linkTo(l_lemma_array[223], "BLNGS");

    // creation of form <ἀλλ᾽‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <ἀλλ᾽‣PART>");
    l_form_array[252] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀλλ᾽");
    l_form_array[252]->setType("FORM_");
    l_form_array[252]->setType("NPPRT");
    l_form_array[252]->linkTo(l_lemma_array[225], "BLNGS");

    // creation of form <εἶναι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <εἶναι‣VERB>");
    l_form_array[253] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἶναι");
    l_form_array[253]->setType("FORM_");
    l_form_array[253]->setType("NPVRB");
    l_form_array[253]->linkTo(l_lemma_array[217], "BLNGS");

    // creation of form <ᾧ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <ᾧ‣PRON>");
    l_form_array[254] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ᾧ");
    l_form_array[254]->setType("FORM_");
    l_form_array[254]->setType("NPPRN");
    l_form_array[254]->linkTo(l_lemma_array[226], "BLNGS");

    // creation of form <ἔοικεν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἔοικεν‣VERB>");
    l_form_array[255] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἔοικεν");
    l_form_array[255]->setType("FORM_");
    l_form_array[255]->setType("NPVRB");
    l_form_array[255]->linkTo(l_lemma_array[227], "BLNGS");

    // creation of form <ἐγὼ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <ἐγὼ‣PRON>");
    l_form_array[256] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐγὼ");
    l_form_array[256]->setType("FORM_");
    l_form_array[256]->setType("NPPRN");
    l_form_array[256]->linkTo(l_lemma_array[228], "BLNGS");

    // creation of form <γοῦν‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <γοῦν‣PART>");
    l_form_array[257] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γοῦν");
    l_form_array[257]->setType("FORM_");
    l_form_array[257]->setType("NPPRT");
    l_form_array[257]->linkTo(l_lemma_array[229], "BLNGS");

    // creation of form <ἄν‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <ἄν‣PART>");
    l_form_array[258] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἄν");
    l_form_array[258]->setType("FORM_");
    l_form_array[258]->setType("NPPRT");
    l_form_array[258]->linkTo(l_lemma_array[230], "BLNGS");

    // creation of form <ἦ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <ἦ‣ADV>");
    l_form_array[259] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἦ");
    l_form_array[259]->setType("FORM_");
    l_form_array[259]->setType("NPADV");
    l_form_array[259]->linkTo(l_lemma_array[231], "BLNGS");

    // creation of form <δ᾽‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <δ᾽‣PART>");
    l_form_array[260] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δ᾽");
    l_form_array[260]->setType("FORM_");
    l_form_array[260]->setType("NPPRT");
    l_form_array[260]->linkTo(l_lemma_array[212], "BLNGS");

    // creation of form <ὅς‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <ὅς‣PRON>");
    l_form_array[261] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅς");
    l_form_array[261]->setType("FORM_");
    l_form_array[261]->setType("NPPRN");
    l_form_array[261]->linkTo(l_lemma_array[226], "BLNGS");

    // creation of form <φαίην‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <φαίην‣VERB>");
    l_form_array[262] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "φαίην");
    l_form_array[262]->setType("FORM_");
    l_form_array[262]->setType("NPVRB");
    l_form_array[262]->linkTo(l_lemma_array[232], "BLNGS");

    // creation of form <τὸν‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τὸν‣PRON>");
    l_form_array[263] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τὸν");
    l_form_array[263]->setType("FORM_");
    l_form_array[263]->setType("NPPRN");
    l_form_array[263]->linkTo(l_lemma_array[188], "BLNGS");

    // creation of form <τοιοῦτον‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <τοιοῦτον‣ADJ>");
    l_form_array[264] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τοιοῦτον");
    l_form_array[264]->setType("FORM_");
    l_form_array[264]->setType("NPADJ");
    l_form_array[264]->linkTo(l_lemma_array[233], "BLNGS");

    // creation of form <τί‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τί‣PRON>");
    l_form_array[265] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τί");
    l_form_array[265]->setType("FORM_");
    l_form_array[265]->setType("NPPRN");
    l_form_array[265]->linkTo(l_lemma_array[234], "BLNGS");

    // creation of form <τἀναντία‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <τἀναντία‣ADJ>");
    l_form_array[266] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τἀναντία");
    l_form_array[266]->setType("FORM_");
    l_form_array[266]->setType("NPADJ");
    l_form_array[266]->linkTo(l_lemma_array[235], "BLNGS");

    // creation of form <τούτων‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τούτων‣PRON>");
    l_form_array[267] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τούτων");
    l_form_array[267]->setType("FORM_");
    l_form_array[267]->setType("NPPRN");
    l_form_array[267]->linkTo(l_lemma_array[236], "BLNGS");

    // creation of form <ἡγούμενός‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἡγούμενός‣VERB>");
    l_form_array[268] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἡγούμενός");
    l_form_array[268]->setType("FORM_");
    l_form_array[268]->setType("NPVRB");
    l_form_array[268]->linkTo(l_lemma_array[200], "BLNGS");

    // creation of form <τέ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <τέ‣ADV>");
    l_form_array[269] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τέ");
    l_form_array[269]->setType("FORM_");
    l_form_array[269]->setType("NPADV");
    l_form_array[269]->linkTo(l_lemma_array[237], "BLNGS");

    // creation of form <τι‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τι‣PRON>");
    l_form_array[270] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τι");
    l_form_array[270]->setType("FORM_");
    l_form_array[270]->setType("NPPRN");
    l_form_array[270]->linkTo(l_lemma_array[199], "BLNGS");

    // creation of form <καλὸν‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <καλὸν‣ADJ>");
    l_form_array[271] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καλὸν");
    l_form_array[271]->setType("FORM_");
    l_form_array[271]->setType("NPADJ");
    l_form_array[271]->linkTo(l_lemma_array[190], "BLNGS");

    // creation of form <καθορᾶν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <καθορᾶν‣VERB>");
    l_form_array[272] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καθορᾶν");
    l_form_array[272]->setType("FORM_");
    l_form_array[272]->setType("NPVRB");
    l_form_array[272]->linkTo(l_lemma_array[238], "BLNGS");

    // creation of form <τὰ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τὰ‣PRON>");
    l_form_array[273] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τὰ");
    l_form_array[273]->setType("FORM_");
    l_form_array[273]->setType("NPPRN");
    l_form_array[273]->linkTo(l_lemma_array[188], "BLNGS");

    // creation of form <ἐκείνου‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <ἐκείνου‣PRON>");
    l_form_array[274] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐκείνου");
    l_form_array[274]->setType("FORM_");
    l_form_array[274]->setType("NPPRN");
    l_form_array[274]->linkTo(l_lemma_array[239], "BLNGS");

    // creation of form <μετέχοντα‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <μετέχοντα‣VERB>");
    l_form_array[275] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μετέχοντα");
    l_form_array[275]->setType("FORM_");
    l_form_array[275]->setType("NPVRB");
    l_form_array[275]->linkTo(l_lemma_array[240], "BLNGS");

    // creation of form <οὔτε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <οὔτε‣ADV>");
    l_form_array[276] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὔτε");
    l_form_array[276]->setType("FORM_");
    l_form_array[276]->setType("NPADV");
    l_form_array[276]->linkTo(l_lemma_array[241], "BLNGS");

    // creation of form <ἡγούμενος‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἡγούμενος‣VERB>");
    l_form_array[277] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἡγούμενος");
    l_form_array[277]->setType("FORM_");
    l_form_array[277]->setType("NPVRB");
    l_form_array[277]->linkTo(l_lemma_array[200], "BLNGS");

    // creation of form <ὕπαρ‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ὕπαρ‣VERB>");
    l_form_array[278] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὕπαρ");
    l_form_array[278]->setType("FORM_");
    l_form_array[278]->setType("NPVRB");
    l_form_array[278]->linkTo(l_lemma_array[242], "BLNGS");

    // creation of form <αὖ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <αὖ‣ADV>");
    l_form_array[279] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὖ");
    l_form_array[279]->setType("FORM_");
    l_form_array[279]->setType("NPADV");
    l_form_array[279]->linkTo(l_lemma_array[243], "BLNGS");

    // creation of form <καὶ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <καὶ‣ADV>");
    l_form_array[280] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "καὶ");
    l_form_array[280]->setType("FORM_");
    l_form_array[280]->setType("NPADV");
    l_form_array[280]->linkTo(l_lemma_array[244], "BLNGS");

    // creation of form <οὗτος‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <οὗτος‣ADJ>");
    l_form_array[281] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὗτος");
    l_form_array[281]->setType("FORM_");
    l_form_array[281]->setType("NPADJ");
    l_form_array[281]->linkTo(l_lemma_array[245], "BLNGS");

    // creation of form <ἔφη‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἔφη‣VERB>");
    l_form_array[282] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἔφη");
    l_form_array[282]->setType("FORM_");
    l_form_array[282]->setType("NPVRB");
    l_form_array[282]->linkTo(l_lemma_array[232], "BLNGS");

    // creation of form <οὐκοῦν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <οὐκοῦν‣ADV>");
    l_form_array[283] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐκοῦν");
    l_form_array[283]->setType("FORM_");
    l_form_array[283]->setType("NPADV");
    l_form_array[283]->linkTo(l_lemma_array[246], "BLNGS");

    // creation of form <τούτου‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τούτου‣PRON>");
    l_form_array[284] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τούτου");
    l_form_array[284]->setType("FORM_");
    l_form_array[284]->setType("NPPRN");
    l_form_array[284]->linkTo(l_lemma_array[236], "BLNGS");

    // creation of form <διάνοιαν‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <διάνοιαν‣NOUN>");
    l_form_array[285] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "διάνοιαν");
    l_form_array[285]->setType("FORM_");
    l_form_array[285]->setType("NPNON");
    l_form_array[285]->linkTo(l_lemma_array[247], "BLNGS");

    // creation of form <ὡς‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ὡς‣SCONJ>");
    l_form_array[286] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὡς");
    l_form_array[286]->setType("FORM_");
    l_form_array[286]->setType("NPSCJ");
    l_form_array[286]->linkTo(l_lemma_array[248], "BLNGS");

    // creation of form <γιγνώσκοντος‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <γιγνώσκοντος‣VERB>");
    l_form_array[287] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γιγνώσκοντος");
    l_form_array[287]->setType("FORM_");
    l_form_array[287]->setType("NPVRB");
    l_form_array[287]->linkTo(l_lemma_array[249], "BLNGS");

    // creation of form <γνώμην‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <γνώμην‣NOUN>");
    l_form_array[288] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γνώμην");
    l_form_array[288]->setType("FORM_");
    l_form_array[288]->setType("NPNON");
    l_form_array[288]->linkTo(l_lemma_array[250], "BLNGS");

    // creation of form <ἂν‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <ἂν‣ADV>");
    l_form_array[289] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἂν");
    l_form_array[289]->setType("FORM_");
    l_form_array[289]->setType("NPADV");
    l_form_array[289]->linkTo(l_lemma_array[198], "BLNGS");

    // creation of form <ὀρθῶς‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <ὀρθῶς‣ADV>");
    l_form_array[290] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὀρθῶς");
    l_form_array[290]->setType("FORM_");
    l_form_array[290]->setType("NPADV");
    l_form_array[290]->linkTo(l_lemma_array[251], "BLNGS");

    // creation of form <φαῖμεν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <φαῖμεν‣VERB>");
    l_form_array[291] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "φαῖμεν");
    l_form_array[291]->setType("FORM_");
    l_form_array[291]->setType("NPVRB");
    l_form_array[291]->linkTo(l_lemma_array[252], "BLNGS");

    // creation of form <τοῦ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τοῦ‣PRON>");
    l_form_array[292] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τοῦ");
    l_form_array[292]->setType("FORM_");
    l_form_array[292]->setType("NPPRN");
    l_form_array[292]->linkTo(l_lemma_array[188], "BLNGS");

    // creation of form <δόξαν‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <δόξαν‣NOUN>");
    l_form_array[293] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δόξαν");
    l_form_array[293]->setType("FORM_");
    l_form_array[293]->setType("NPNON");
    l_form_array[293]->linkTo(l_lemma_array[253], "BLNGS");

    // creation of form <δοξάζοντος‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <δοξάζοντος‣VERB>");
    l_form_array[294] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δοξάζοντος");
    l_form_array[294]->setType("FORM_");
    l_form_array[294]->setType("NPVRB");
    l_form_array[294]->linkTo(l_lemma_array[254], "BLNGS");

    // creation of form <πάνυ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <πάνυ‣ADV>");
    l_form_array[295] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πάνυ");
    l_form_array[295]->setType("FORM_");
    l_form_array[295]->setType("NPADV");
    l_form_array[295]->linkTo(l_lemma_array[255], "BLNGS");

    // creation of form <μὲν‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <μὲν‣PART>");
    l_form_array[296] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μὲν");
    l_form_array[296]->setType("FORM_");
    l_form_array[296]->setType("NPPRT");
    l_form_array[296]->linkTo(l_lemma_array[256], "BLNGS");

    // creation of form <οὖν‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <οὖν‣PART>");
    l_form_array[297] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὖν");
    l_form_array[297]->setType("FORM_");
    l_form_array[297]->setType("NPPRT");
    l_form_array[297]->linkTo(l_lemma_array[257], "BLNGS");

    // creation of form <ἐὰν‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ἐὰν‣SCONJ>");
    l_form_array[298] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐὰν");
    l_form_array[298]->setType("FORM_");
    l_form_array[298]->setType("NPSCJ");
    l_form_array[298]->linkTo(l_lemma_array[258], "BLNGS");

    // creation of form <ἡμῖν‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <ἡμῖν‣PRON>");
    l_form_array[299] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἡμῖν");
    l_form_array[299]->setType("FORM_");
    l_form_array[299]->setType("NPPRN");
    l_form_array[299]->linkTo(l_lemma_array[228], "BLNGS");

    // creation of form <χαλεπαίνῃ‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <χαλεπαίνῃ‣VERB>");
    l_form_array[300] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "χαλεπαίνῃ");
    l_form_array[300]->setType("FORM_");
    l_form_array[300]->setType("NPVRB");
    l_form_array[300]->linkTo(l_lemma_array[259], "BLNGS");

    // creation of form <ὅν‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <ὅν‣PRON>");
    l_form_array[301] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅν");
    l_form_array[301]->setType("FORM_");
    l_form_array[301]->setType("NPPRN");
    l_form_array[301]->linkTo(l_lemma_array[226], "BLNGS");

    // creation of form <φαμεν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <φαμεν‣VERB>");
    l_form_array[302] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "φαμεν");
    l_form_array[302]->setType("FORM_");
    l_form_array[302]->setType("NPVRB");
    l_form_array[302]->linkTo(l_lemma_array[232], "BLNGS");

    // creation of form <δοξάζειν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <δοξάζειν‣VERB>");
    l_form_array[303] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δοξάζειν");
    l_form_array[303]->setType("FORM_");
    l_form_array[303]->setType("NPVRB");
    l_form_array[303]->linkTo(l_lemma_array[254], "BLNGS");

    // creation of form <γιγνώσκειν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <γιγνώσκειν‣VERB>");
    l_form_array[304] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γιγνώσκειν");
    l_form_array[304]->setType("FORM_");
    l_form_array[304]->setType("NPVRB");
    l_form_array[304]->linkTo(l_lemma_array[249], "BLNGS");

    // creation of form <ἀμφισβητῇ‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <ἀμφισβητῇ‣NOUN>");
    l_form_array[305] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀμφισβητῇ");
    l_form_array[305]->setType("FORM_");
    l_form_array[305]->setType("NPNON");
    l_form_array[305]->linkTo(l_lemma_array[260], "BLNGS");

    // creation of form <οὐκ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <οὐκ‣ADV>");
    l_form_array[306] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐκ");
    l_form_array[306]->setType("FORM_");
    l_form_array[306]->setType("NPADV");
    l_form_array[306]->linkTo(l_lemma_array[215], "BLNGS");

    // creation of form <ἀληθῆ‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <ἀληθῆ‣ADJ>");
    l_form_array[307] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀληθῆ");
    l_form_array[307]->setType("FORM_");
    l_form_array[307]->setType("NPADJ");
    l_form_array[307]->linkTo(l_lemma_array[261], "BLNGS");

    // creation of form <λέγομεν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <λέγομεν‣VERB>");
    l_form_array[308] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "λέγομεν");
    l_form_array[308]->setType("FORM_");
    l_form_array[308]->setType("NPVRB");
    l_form_array[308]->linkTo(l_lemma_array[262], "BLNGS");

    // creation of form <ἕξομέν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἕξομέν‣VERB>");
    l_form_array[309] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἕξομέν");
    l_form_array[309]->setType("FORM_");
    l_form_array[309]->setType("NPVRB");
    l_form_array[309]->linkTo(l_lemma_array[263], "BLNGS");

    // creation of form <παραμυθεῖσθαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <παραμυθεῖσθαι‣VERB>");
    l_form_array[310] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "παραμυθεῖσθαι");
    l_form_array[310]->setType("FORM_");
    l_form_array[310]->setType("NPVRB");
    l_form_array[310]->linkTo(l_lemma_array[264], "BLNGS");

    // creation of form <αὐτὸν‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <αὐτὸν‣ADJ>");
    l_form_array[311] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτὸν");
    l_form_array[311]->setType("FORM_");
    l_form_array[311]->setType("NPADJ");
    l_form_array[311]->linkTo(l_lemma_array[265], "BLNGS");

    // creation of form <πείθειν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <πείθειν‣VERB>");
    l_form_array[312] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πείθειν");
    l_form_array[312]->setType("FORM_");
    l_form_array[312]->setType("NPVRB");
    l_form_array[312]->linkTo(l_lemma_array[266], "BLNGS");

    // creation of form <ἠρέμα‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <ἠρέμα‣NOUN>");
    l_form_array[313] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἠρέμα");
    l_form_array[313]->setType("FORM_");
    l_form_array[313]->setType("NPNON");
    l_form_array[313]->linkTo(l_lemma_array[267], "BLNGS");

    // creation of form <ἐπικρυπτόμενοι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἐπικρυπτόμενοι‣VERB>");
    l_form_array[314] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐπικρυπτόμενοι");
    l_form_array[314]->setType("FORM_");
    l_form_array[314]->setType("NPVRB");
    l_form_array[314]->linkTo(l_lemma_array[268], "BLNGS");

    // creation of form <ὅτι‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ὅτι‣SCONJ>");
    l_form_array[315] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὅτι");
    l_form_array[315]->setType("FORM_");
    l_form_array[315]->setType("NPSCJ");
    l_form_array[315]->linkTo(l_lemma_array[269], "BLNGS");

    // creation of form <οὐχ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <οὐχ‣ADV>");
    l_form_array[316] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐχ");
    l_form_array[316]->setType("FORM_");
    l_form_array[316]->setType("NPADV");
    l_form_array[316]->linkTo(l_lemma_array[215], "BLNGS");

    // creation of form <ὑγιαίνει‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ὑγιαίνει‣VERB>");
    l_form_array[317] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὑγιαίνει");
    l_form_array[317]->setType("FORM_");
    l_form_array[317]->setType("NPVRB");
    l_form_array[317]->linkTo(l_lemma_array[270], "BLNGS");

    // creation of form <δεῖ‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <δεῖ‣VERB>");
    l_form_array[318] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δεῖ");
    l_form_array[318]->setType("FORM_");
    l_form_array[318]->setType("NPVRB");
    l_form_array[318]->linkTo(l_lemma_array[271], "BLNGS");

    // creation of form <γέ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <γέ‣ADV>");
    l_form_array[319] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γέ");
    l_form_array[319]->setType("FORM_");
    l_form_array[319]->setType("NPADV");
    l_form_array[319]->linkTo(l_lemma_array[272], "BLNGS");

    // creation of form <τοι‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <τοι‣PRON>");
    l_form_array[320] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τοι");
    l_form_array[320]->setType("FORM_");
    l_form_array[320]->setType("NPPRN");
    l_form_array[320]->linkTo(l_lemma_array[209], "BLNGS");

    // creation of form <δή‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <δή‣ADV>");
    l_form_array[321] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δή");
    l_form_array[321]->setType("FORM_");
    l_form_array[321]->setType("NPADV");
    l_form_array[321]->linkTo(l_lemma_array[273], "BLNGS");

    // creation of form <ἴθι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἴθι‣VERB>");
    l_form_array[322] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἴθι");
    l_form_array[322]->setType("FORM_");
    l_form_array[322]->setType("NPVRB");
    l_form_array[322]->linkTo(l_lemma_array[274], "BLNGS");

    // creation of form <δή‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <δή‣PART>");
    l_form_array[323] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "δή");
    l_form_array[323]->setType("FORM_");
    l_form_array[323]->setType("NPPRT");
    l_form_array[323]->linkTo(l_lemma_array[275], "BLNGS");

    // creation of form <ἐροῦμεν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἐροῦμεν‣VERB>");
    l_form_array[324] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐροῦμεν");
    l_form_array[324]->setType("FORM_");
    l_form_array[324]->setType("NPVRB");
    l_form_array[324]->linkTo(l_lemma_array[276], "BLNGS");

    // creation of form <πρὸς‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <πρὸς‣SCONJ>");
    l_form_array[325] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πρὸς");
    l_form_array[325]->setType("FORM_");
    l_form_array[325]->setType("NPSCJ");
    l_form_array[325]->linkTo(l_lemma_array[277], "BLNGS");

    // creation of form <αὐτόν‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <αὐτόν‣ADJ>");
    l_form_array[326] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτόν");
    l_form_array[326]->setType("FORM_");
    l_form_array[326]->setType("NPADJ");
    l_form_array[326]->linkTo(l_lemma_array[265], "BLNGS");

    // creation of form <βούλει‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <βούλει‣VERB>");
    l_form_array[327] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "βούλει");
    l_form_array[327]->setType("FORM_");
    l_form_array[327]->setType("NPVRB");
    l_form_array[327]->linkTo(l_lemma_array[278], "BLNGS");

    // creation of form <ὧδε‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <ὧδε‣ADV>");
    l_form_array[328] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὧδε");
    l_form_array[328]->setType("FORM_");
    l_form_array[328]->setType("NPADV");
    l_form_array[328]->linkTo(l_lemma_array[279], "BLNGS");

    // creation of form <πυνθανώμεθα‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <πυνθανώμεθα‣VERB>");
    l_form_array[329] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πυνθανώμεθα");
    l_form_array[329]->setType("FORM_");
    l_form_array[329]->setType("NPVRB");
    l_form_array[329]->linkTo(l_lemma_array[280], "BLNGS");

    // creation of form <παρ᾽‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <παρ᾽‣SCONJ>");
    l_form_array[330] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "παρ᾽");
    l_form_array[330]->setType("FORM_");
    l_form_array[330]->setType("NPSCJ");
    l_form_array[330]->linkTo(l_lemma_array[281], "BLNGS");

    // creation of form <αὐτοῦ‣ADV>
    qCDebug(g_cat_silence) << QString("Creating form <αὐτοῦ‣ADV>");
    l_form_array[331] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτοῦ");
    l_form_array[331]->setType("FORM_");
    l_form_array[331]->setType("NPADV");
    l_form_array[331]->linkTo(l_lemma_array[282], "BLNGS");

    // creation of form <λέγοντες‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <λέγοντες‣VERB>");
    l_form_array[332] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "λέγοντες");
    l_form_array[332]->setType("FORM_");
    l_form_array[332]->setType("NPVRB");
    l_form_array[332]->linkTo(l_lemma_array[262], "BLNGS");

    // creation of form <εἴ‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <εἴ‣SCONJ>");
    l_form_array[333] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἴ");
    l_form_array[333]->setType("FORM_");
    l_form_array[333]->setType("NPSCJ");
    l_form_array[333]->linkTo(l_lemma_array[283], "BLNGS");

    // creation of form <οἶδεν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <οἶδεν‣VERB>");
    l_form_array[334] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οἶδεν");
    l_form_array[334]->setType("FORM_");
    l_form_array[334]->setType("NPVRB");
    l_form_array[334]->linkTo(l_lemma_array[284], "BLNGS");

    // creation of form <οὐδεὶς‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <οὐδεὶς‣ADJ>");
    l_form_array[335] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐδεὶς");
    l_form_array[335]->setType("FORM_");
    l_form_array[335]->setType("NPADJ");
    l_form_array[335]->linkTo(l_lemma_array[285], "BLNGS");

    // creation of form <αὐτῷ‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <αὐτῷ‣ADJ>");
    l_form_array[336] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "αὐτῷ");
    l_form_array[336]->setType("FORM_");
    l_form_array[336]->setType("NPADJ");
    l_form_array[336]->linkTo(l_lemma_array[265], "BLNGS");

    // creation of form <φθόνος‣NOUN>
    qCDebug(g_cat_silence) << QString("Creating form <φθόνος‣NOUN>");
    l_form_array[337] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "φθόνος");
    l_form_array[337]->setType("FORM_");
    l_form_array[337]->setType("NPNON");
    l_form_array[337]->linkTo(l_lemma_array[286], "BLNGS");

    // creation of form <ἅσμενοι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἅσμενοι‣VERB>");
    l_form_array[338] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἅσμενοι");
    l_form_array[338]->setType("FORM_");
    l_form_array[338]->setType("NPVRB");
    l_form_array[338]->linkTo(l_lemma_array[287], "BLNGS");

    // creation of form <ἂν‣PART>
    qCDebug(g_cat_silence) << QString("Creating form <ἂν‣PART>");
    l_form_array[339] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἂν");
    l_form_array[339]->setType("FORM_");
    l_form_array[339]->setType("NPPRT");
    l_form_array[339]->linkTo(l_lemma_array[230], "BLNGS");

    // creation of form <ἴδοιμεν‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἴδοιμεν‣VERB>");
    l_form_array[340] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἴδοιμεν");
    l_form_array[340]->setType("FORM_");
    l_form_array[340]->setType("NPVRB");
    l_form_array[340]->linkTo(l_lemma_array[288], "BLNGS");

    // creation of form <εἰδότα‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <εἰδότα‣VERB>");
    l_form_array[341] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἰδότα");
    l_form_array[341]->setType("FORM_");
    l_form_array[341]->setType("NPVRB");
    l_form_array[341]->linkTo(l_lemma_array[284], "BLNGS");

    // creation of form <εἰπὲ‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <εἰπὲ‣VERB>");
    l_form_array[342] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "εἰπὲ");
    l_form_array[342]->setType("FORM_");
    l_form_array[342]->setType("NPVRB");
    l_form_array[342]->linkTo(l_lemma_array[289], "BLNGS");

    // creation of form <γιγνώσκων‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <γιγνώσκων‣VERB>");
    l_form_array[343] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γιγνώσκων");
    l_form_array[343]->setType("FORM_");
    l_form_array[343]->setType("NPVRB");
    l_form_array[343]->linkTo(l_lemma_array[249], "BLNGS");

    // creation of form <γιγνώσκει‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <γιγνώσκει‣VERB>");
    l_form_array[344] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "γιγνώσκει");
    l_form_array[344]->setType("FORM_");
    l_form_array[344]->setType("NPVRB");
    l_form_array[344]->linkTo(l_lemma_array[249], "BLNGS");

    // creation of form <τὶ‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <τὶ‣ADJ>");
    l_form_array[345] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "τὶ");
    l_form_array[345]->setType("FORM_");
    l_form_array[345]->setType("NPADJ");
    l_form_array[345]->linkTo(l_lemma_array[290], "BLNGS");

    // creation of form <οὐδέν‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <οὐδέν‣ADJ>");
    l_form_array[346] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "οὐδέν");
    l_form_array[346]->setType("FORM_");
    l_form_array[346]->setType("NPADJ");
    l_form_array[346]->linkTo(l_lemma_array[285], "BLNGS");

    // creation of form <σὺ‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <σὺ‣PRON>");
    l_form_array[347] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "σὺ");
    l_form_array[347]->setType("FORM_");
    l_form_array[347]->setType("NPPRN");
    l_form_array[347]->linkTo(l_lemma_array[209], "BLNGS");

    // creation of form <μοι‣PRON>
    qCDebug(g_cat_silence) << QString("Creating form <μοι‣PRON>");
    l_form_array[348] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "μοι");
    l_form_array[348]->setType("FORM_");
    l_form_array[348]->setType("NPPRN");
    l_form_array[348]->linkTo(l_lemma_array[228], "BLNGS");

    // creation of form <ὑπὲρ‣SCONJ>
    qCDebug(g_cat_silence) << QString("Creating form <ὑπὲρ‣SCONJ>");
    l_form_array[349] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὑπὲρ");
    l_form_array[349]->setType("FORM_");
    l_form_array[349]->setType("NPSCJ");
    l_form_array[349]->linkTo(l_lemma_array[291], "BLNGS");

    // creation of form <ἐκείνου‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <ἐκείνου‣ADJ>");
    l_form_array[350] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἐκείνου");
    l_form_array[350]->setType("FORM_");
    l_form_array[350]->setType("NPADJ");
    l_form_array[350]->linkTo(l_lemma_array[292], "BLNGS");

    // creation of form <ἀποκρίνου‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἀποκρίνου‣VERB>");
    l_form_array[351] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀποκρίνου");
    l_form_array[351]->setType("FORM_");
    l_form_array[351]->setType("NPVRB");
    l_form_array[351]->linkTo(l_lemma_array[293], "BLNGS");

    // creation of form <ἀποκρινοῦμαι‣VERB>
    qCDebug(g_cat_silence) << QString("Creating form <ἀποκρινοῦμαι‣VERB>");
    l_form_array[352] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ἀποκρινοῦμαι");
    l_form_array[352]->setType("FORM_");
    l_form_array[352]->setType("NPVRB");
    l_form_array[352]->linkTo(l_lemma_array[293], "BLNGS");

    // creation of form <πότερον‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <πότερον‣ADJ>");
    l_form_array[353] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "πότερον");
    l_form_array[353]->setType("FORM_");
    l_form_array[353]->setType("NPADJ");
    l_form_array[353]->linkTo(l_lemma_array[294], "BLNGS");

    // creation of form <ὂν‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <ὂν‣ADJ>");
    l_form_array[354] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὂν");
    l_form_array[354]->setType("FORM_");
    l_form_array[354]->setType("NPADJ");
    l_form_array[354]->linkTo(l_lemma_array[295], "BLNGS");

    // creation of form <ὄν‣ADJ>
    qCDebug(g_cat_silence) << QString("Creating form <ὄν‣ADJ>");
    l_form_array[355] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "ὄν");
    l_form_array[355]->setType("FORM_");
    l_form_array[355]->setType("NPADJ");
    l_form_array[355]->linkTo(l_lemma_array[296], "BLNGS");

    // creation of Republic books node
    qCDebug(g_cat_silence) << QString("Creating Republic books node");
    M1Store::Item_lv2* l_republic_books = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Books");
    l_republic_books->setType("FOLDR");
    l_republic->linkTo(l_republic_books, "OWNS_");

    M1Store::Item_lv2* l_book[10];
    // creation of Republic book 10 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 10 node");
    l_book[9] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 10");
    l_book[9]->setType("TXTBK");
    l_republic_books->linkTo(l_book[9], "OWNS_");

    // creation of Republic book 9 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 9 node");
    l_book[8] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 9");
    l_book[8]->setType("TXTBK");
    l_republic_books->linkTo(l_book[8], "OWNS_");

    // creation of Republic book 8 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 8 node");
    l_book[7] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 8");
    l_book[7]->setType("TXTBK");
    l_republic_books->linkTo(l_book[7], "OWNS_");

    // creation of Republic book 7 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 7 node");
    l_book[6] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 7");
    l_book[6]->setType("TXTBK");
    l_republic_books->linkTo(l_book[6], "OWNS_");

    // creation of Republic book 6 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 6 node");
    l_book[5] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 6");
    l_book[5]->setType("TXTBK");
    l_republic_books->linkTo(l_book[5], "OWNS_");

    // creation of Republic book 5 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 5 node");
    l_book[4] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 5");
    l_book[4]->setType("TXTBK");
    l_republic_books->linkTo(l_book[4], "OWNS_");

    // creation of Republic book 4 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 4 node");
    l_book[3] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 4");
    l_book[3]->setType("TXTBK");
    l_republic_books->linkTo(l_book[3], "OWNS_");

    // creation of Republic book 3 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 3 node");
    l_book[2] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 3");
    l_book[2]->setType("TXTBK");
    l_republic_books->linkTo(l_book[2], "OWNS_");

    // creation of Republic book 2 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 2 node");
    l_book[1] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 2");
    l_book[1]->setType("TXTBK");
    l_republic_books->linkTo(l_book[1], "OWNS_");

    // creation of Republic book 1 node
    qCDebug(g_cat_silence) << QString("Creating Republic book 1 node");
    l_book[0] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Book 1");
    l_book[0]->setType("TXTBK");
    l_republic_books->linkTo(l_book[0], "OWNS_");

    // creation of Republic sections node
    qCDebug(g_cat_silence) << QString("Creating Republic sections node");
    M1Store::Item_lv2* l_republic_sections = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Stephanus Section");
    l_republic_sections->setType("FOLDR");
    l_republic->linkTo(l_republic_sections, "OWNS_");

    M1Store::Item_lv2* l_stephanus_array[3];
    // creation of section <476c>
    qCDebug(g_cat_silence) << QString("Creating section <476c>");
    l_stephanus_array[0] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "476c");
    l_stephanus_array[0]->setType("STPSC");

    // creation of section <476d>
    qCDebug(g_cat_silence) << QString("Creating section <476d>");
    l_stephanus_array[1] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "476d");
    l_stephanus_array[1]->setType("STPSC");

    // creation of section <476e>
    qCDebug(g_cat_silence) << QString("Creating section <476e>");
    l_stephanus_array[2] = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "476e");
    l_stephanus_array[2]->setType("STPSC");

    // creation of Republic versions node
    qCDebug(g_cat_silence) << QString("Creating Republic versions node");
    M1Store::Item_lv2* l_republic_versions = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Versions");
    l_republic_versions->setType("FOLDR");
    l_republic->linkTo(l_republic_versions, "OWNS_");

    M1Store::Item_lv2* l_cur_occ;
    M1Store::Item_lv2* l_cur_ver_occ = nullptr;
    M1Store::Item_lv2* l_cur_book_sentence = nullptr;
    M1Store::Item_lv2* l_cur_sentence_occ = nullptr;
    M1Store::Item_lv2* l_cur_sentence = nullptr;
    // creation of Republic version Shorey node
    qCDebug(g_cat_silence) << QString("Creating Republic version Shorey node");
    M1Store::Item_lv2* l_version_shorey = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX  | M1Env::IS_SPECIAL,
        // label
        "Shorey (version)",
        // Special Item flags
        SI_IS_TYPE  | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RVSHR",
        // icon path
        FOLDER_ICON_PATH);
    l_version_shorey->setType("TYPE_");
    l_version_shorey->setType("TXTVR");
    l_republic_versions->linkTo(l_version_shorey, "OWNS_");

    l_cur_ver_occ = nullptr;
    l_cur_sentence_occ = nullptr;
    l_cur_sentence = nullptr;
    // creation of Republic occurrence A-SHR-079347 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079347 node");
    l_cur_occ = l_republic->linkTo(l_form_array[0]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079347");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_stephanus_array[0]->linkTo(l_cur_occ, "OWNS_");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 1> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 1> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 1");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079348 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079348 node");
    l_cur_occ = l_republic->linkTo(l_form_array[1]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079348");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079350 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079350 node");
    l_cur_occ = l_republic->linkTo(l_form_array[2]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079350");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079354 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079354 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079354");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 2> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 2> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 2");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079356 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079356 node");
    l_cur_occ = l_republic->linkTo(l_form_array[4]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079356");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079358 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079358 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079358");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079359 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079359 node");
    l_cur_occ = l_republic->linkTo(l_form_array[6]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079359");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079360 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079360 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079360");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079361 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079361 node");
    l_cur_occ = l_republic->linkTo(l_form_array[8]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079361");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079362 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079362 node");
    l_cur_occ = l_republic->linkTo(l_form_array[9]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079362");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079364 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079364 node");
    l_cur_occ = l_republic->linkTo(l_form_array[10]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079364");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079365 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079365 node");
    l_cur_occ = l_republic->linkTo(l_form_array[11]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079365");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079366 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079366 node");
    l_cur_occ = l_republic->linkTo(l_form_array[6]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079366");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079367 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079367 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079367");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079368 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079368 node");
    l_cur_occ = l_republic->linkTo(l_form_array[12]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079368");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079369 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079369 node");
    l_cur_occ = l_republic->linkTo(l_form_array[13]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079369");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079370 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079370 node");
    l_cur_occ = l_republic->linkTo(l_form_array[14]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079370");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079371 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079371 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079371");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079372 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079372 node");
    l_cur_occ = l_republic->linkTo(l_form_array[16]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079372");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079373 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079373 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079373");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079374 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079374 node");
    l_cur_occ = l_republic->linkTo(l_form_array[18]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079374");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079375 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079375 node");
    l_cur_occ = l_republic->linkTo(l_form_array[19]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079375");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079376 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079376 node");
    l_cur_occ = l_republic->linkTo(l_form_array[20]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079376");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079377 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079377 node");
    l_cur_occ = l_republic->linkTo(l_form_array[21]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079377");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079378 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079378 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079378");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079379 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079379 node");
    l_cur_occ = l_republic->linkTo(l_form_array[22]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079379");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079380 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079380 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079380");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079381 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079381 node");
    l_cur_occ = l_republic->linkTo(l_form_array[24]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079381");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079382 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079382 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079382");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079383 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079383 node");
    l_cur_occ = l_republic->linkTo(l_form_array[26]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079383");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079384 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079384 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079384");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079385 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079385 node");
    l_cur_occ = l_republic->linkTo(l_form_array[28]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079385");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079387 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079387 node");
    l_cur_occ = l_republic->linkTo(l_form_array[29]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079387");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079388 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079388 node");
    l_cur_occ = l_republic->linkTo(l_form_array[30]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079388");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079389 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079389 node");
    l_cur_occ = l_republic->linkTo(l_form_array[31]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079389");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079390 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079390 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079390");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079391 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079391 node");
    l_cur_occ = l_republic->linkTo(l_form_array[33]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079391");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079392 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079392 node");
    l_cur_occ = l_republic->linkTo(l_form_array[34]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079392");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079393 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079393 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079393");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079394 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079394 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079394");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079395 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079395 node");
    l_cur_occ = l_republic->linkTo(l_form_array[36]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079395");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079396 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079396 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079396");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079397 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079397 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079397");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079398 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079398 node");
    l_cur_occ = l_republic->linkTo(l_form_array[38]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079398");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079400 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079400 node");
    l_cur_occ = l_republic->linkTo(l_form_array[39]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079400");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 3> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 3> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 3");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079401 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079401 node");
    l_cur_occ = l_republic->linkTo(l_form_array[40]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079401");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079403 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079403 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079403");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 4> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 4> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 4");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079404 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079404 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079404");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079405 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079405 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079405");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079406 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079406 node");
    l_cur_occ = l_republic->linkTo(l_form_array[36]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079406");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079407 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079407 node");
    l_cur_occ = l_republic->linkTo(l_form_array[42]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079407");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079409 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079409 node");
    l_cur_occ = l_republic->linkTo(l_form_array[43]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079409");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079410 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079410 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079410");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079411 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079411 node");
    l_cur_occ = l_republic->linkTo(l_form_array[44]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079411");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079412 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079412 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079412");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079413 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079413 node");
    l_cur_occ = l_republic->linkTo(l_form_array[45]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079413");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079414 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079414 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079414");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079415 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079415 node");
    l_cur_occ = l_republic->linkTo(l_form_array[46]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079415");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079417 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079417 node");
    l_cur_occ = l_republic->linkTo(l_form_array[39]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079417");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079418 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079418 node");
    l_cur_occ = l_republic->linkTo(l_form_array[47]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079418");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(":", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079420 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079420 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079420");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079421 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079421 node");
    l_cur_occ = l_republic->linkTo(l_form_array[48]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079421");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079422 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079422 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079422");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079423 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079423 node");
    l_cur_occ = l_republic->linkTo(l_form_array[49]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079423");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079424 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079424 node");
    l_cur_occ = l_republic->linkTo(l_form_array[50]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079424");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079425 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079425 node");
    l_cur_occ = l_republic->linkTo(l_form_array[51]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079425");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("?”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079429 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079429 node");
    l_cur_occ = l_republic->linkTo(l_form_array[52]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079429");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 5> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 5> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 5");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079430 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079430 node");
    l_cur_occ = l_republic->linkTo(l_form_array[53]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079430");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079431 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079431 node");
    l_cur_occ = l_republic->linkTo(l_form_array[54]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079431");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079432 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079432 node");
    l_cur_occ = l_republic->linkTo(l_form_array[55]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079432");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079433 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079433 node");
    l_cur_occ = l_republic->linkTo(l_form_array[56]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079433");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079434 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079434 node");
    l_cur_occ = l_republic->linkTo(l_form_array[57]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079434");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",”", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079437 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079437 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079437");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079438 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079438 node");
    l_cur_occ = l_republic->linkTo(l_form_array[58]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079438");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079441 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079441 node");
    l_cur_occ = l_republic->linkTo(l_form_array[59]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079441");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 6> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 6> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 6");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079443 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079443 node");
    l_cur_occ = l_republic->linkTo(l_form_array[4]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079443");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079445 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079445 node");
    l_cur_occ = l_republic->linkTo(l_form_array[60]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079445");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079446 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079446 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079446");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079447 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079447 node");
    l_cur_occ = l_republic->linkTo(l_form_array[61]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079447");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079448 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079448 node");
    l_cur_occ = l_republic->linkTo(l_form_array[62]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079448");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(":", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079450 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079450 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079450");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079451 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079451 node");
    l_cur_occ = l_republic->linkTo(l_form_array[44]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079451");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079452 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079452 node");
    l_cur_occ = l_republic->linkTo(l_form_array[63]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079452");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079453 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079453 node");
    l_cur_occ = l_republic->linkTo(l_form_array[64]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079453");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079454 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079454 node");
    l_cur_occ = l_republic->linkTo(l_form_array[65]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079454");
    l_cur_occ->setType("RVSHR");
    l_stephanus_array[1]->linkTo(l_cur_occ, "OWNS_");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079455 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079455 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079455");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079456 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079456 node");
    l_cur_occ = l_republic->linkTo(l_form_array[12]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079456");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079457 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079457 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079457");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079458 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079458 node");
    l_cur_occ = l_republic->linkTo(l_form_array[13]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079458");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079460 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079460 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079460");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079461 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079461 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079461");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079462 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079462 node");
    l_cur_occ = l_republic->linkTo(l_form_array[16]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079462");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079463 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079463 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079463");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079464 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079464 node");
    l_cur_occ = l_republic->linkTo(l_form_array[67]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079464");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079465 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079465 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079465");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079467 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079467 node");
    l_cur_occ = l_republic->linkTo(l_form_array[68]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079467");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079469 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079469 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079469");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079470 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079470 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079470");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079471 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079471 node");
    l_cur_occ = l_republic->linkTo(l_form_array[9]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079471");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079472 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079472 node");
    l_cur_occ = l_republic->linkTo(l_form_array[69]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079472");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079473 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079473 node");
    l_cur_occ = l_republic->linkTo(l_form_array[70]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079473");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079474 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079474 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079474");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079475 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079475 node");
    l_cur_occ = l_republic->linkTo(l_form_array[28]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079475");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079477 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079477 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079477");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079478 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079478 node");
    l_cur_occ = l_republic->linkTo(l_form_array[11]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079478");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079479 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079479 node");
    l_cur_occ = l_republic->linkTo(l_form_array[71]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079479");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079480 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079480 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079480");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079481 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079481 node");
    l_cur_occ = l_republic->linkTo(l_form_array[72]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079481");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079482 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079482 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079482");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079483 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079483 node");
    l_cur_occ = l_republic->linkTo(l_form_array[73]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079483");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079484 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079484 node");
    l_cur_occ = l_republic->linkTo(l_form_array[28]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079484");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079485 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079485 node");
    l_cur_occ = l_republic->linkTo(l_form_array[14]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079485");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079486 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079486 node");
    l_cur_occ = l_republic->linkTo(l_form_array[28]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079486");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079487 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079487 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079487");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079488 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079488 node");
    l_cur_occ = l_republic->linkTo(l_form_array[72]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079488");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 7> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 7> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 7");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079490 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079490 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079490");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079491 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079491 node");
    l_cur_occ = l_republic->linkTo(l_form_array[33]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079491");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079492 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079492 node");
    l_cur_occ = l_republic->linkTo(l_form_array[34]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079492");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079494 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079494 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079494");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079495 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079495 node");
    l_cur_occ = l_republic->linkTo(l_form_array[74]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079495");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079496 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079496 node");
    l_cur_occ = l_republic->linkTo(l_form_array[75]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079496");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079498 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079498 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079498");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079499 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079499 node");
    l_cur_occ = l_republic->linkTo(l_form_array[76]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079499");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079500 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079500 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079500");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079501 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079501 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079501");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079502 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079502 node");
    l_cur_occ = l_republic->linkTo(l_form_array[36]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079502");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079503 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079503 node");
    l_cur_occ = l_republic->linkTo(l_form_array[42]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079503");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("?”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079507 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079507 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079507");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 8> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 8> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 8");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079508 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079508 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079508");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079509 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079509 node");
    l_cur_occ = l_republic->linkTo(l_form_array[77]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079509");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079510 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079510 node");
    l_cur_occ = l_republic->linkTo(l_form_array[78]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079510");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079511 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079511 node");
    l_cur_occ = l_republic->linkTo(l_form_array[46]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079511");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",”", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079514 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079514 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079514");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079515 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079515 node");
    l_cur_occ = l_republic->linkTo(l_form_array[79]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079515");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079518 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079518 node");
    l_cur_occ = l_republic->linkTo(l_form_array[80]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079518");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 9> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 9> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 9");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079519 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079519 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079519");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079520 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079520 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079520");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079521 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079521 node");
    l_cur_occ = l_republic->linkTo(l_form_array[82]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079521");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079523 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079523 node");
    l_cur_occ = l_republic->linkTo(l_form_array[4]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079523");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079525 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079525 node");
    l_cur_occ = l_republic->linkTo(l_form_array[55]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079525");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079526 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079526 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079526");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079527 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079527 node");
    l_cur_occ = l_republic->linkTo(l_form_array[83]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079527");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079528 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079528 node");
    l_cur_occ = l_republic->linkTo(l_form_array[42]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079528");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079529 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079529 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079529");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079530 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079530 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079530");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079531 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079531 node");
    l_cur_occ = l_republic->linkTo(l_form_array[84]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079531");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079532 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079532 node");
    l_cur_occ = l_republic->linkTo(l_form_array[85]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079532");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079533 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079533 node");
    l_cur_occ = l_republic->linkTo(l_form_array[86]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079533");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079535 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079535 node");
    l_cur_occ = l_republic->linkTo(l_form_array[26]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079535");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079537 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079537 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079537");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079538 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079538 node");
    l_cur_occ = l_republic->linkTo(l_form_array[56]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079538");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079539 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079539 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079539");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079540 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079540 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079540");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079541 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079541 node");
    l_cur_occ = l_republic->linkTo(l_form_array[87]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079541");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079542 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079542 node");
    l_cur_occ = l_republic->linkTo(l_form_array[85]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079542");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079543 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079543 node");
    l_cur_occ = l_republic->linkTo(l_form_array[88]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079543");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079545 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079545 node");
    l_cur_occ = l_republic->linkTo(l_form_array[75]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079545");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("?”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079549 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079549 node");
    l_cur_occ = l_republic->linkTo(l_form_array[89]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079549");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField(".”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079553 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079553 node");
    l_cur_occ = l_republic->linkTo(l_form_array[90]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079553");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 10> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 10> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 10");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079555 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079555 node");
    l_cur_occ = l_republic->linkTo(l_form_array[91]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079555");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079557 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079557 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079557");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079558 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079558 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079558");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079559 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079559 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079559");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079560 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079560 node");
    l_cur_occ = l_republic->linkTo(l_form_array[92]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079560");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079561 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079561 node");
    l_cur_occ = l_republic->linkTo(l_form_array[93]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079561");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079562 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079562 node");
    l_cur_occ = l_republic->linkTo(l_form_array[10]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079562");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079563 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079563 node");
    l_cur_occ = l_republic->linkTo(l_form_array[94]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079563");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079564 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079564 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079564");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079565 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079565 node");
    l_cur_occ = l_republic->linkTo(l_form_array[95]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079565");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079566 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079566 node");
    l_cur_occ = l_republic->linkTo(l_form_array[53]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079566");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079567 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079567 node");
    l_cur_occ = l_republic->linkTo(l_form_array[73]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079567");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079568 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079568 node");
    l_cur_occ = l_republic->linkTo(l_form_array[96]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079568");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079569 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079569 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079569");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079570 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079570 node");
    l_cur_occ = l_republic->linkTo(l_form_array[97]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079570");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079571 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079571 node");
    l_cur_occ = l_republic->linkTo(l_form_array[98]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079571");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079572 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079572 node");
    l_cur_occ = l_republic->linkTo(l_form_array[99]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079572");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079573 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079573 node");
    l_cur_occ = l_republic->linkTo(l_form_array[85]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079573");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079574 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079574 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079574");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079575 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079575 node");
    l_cur_occ = l_republic->linkTo(l_form_array[100]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079575");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079577 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079577 node");
    l_cur_occ = l_republic->linkTo(l_form_array[101]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079577");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_stephanus_array[2]->linkTo(l_cur_occ, "OWNS_");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 11> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 11> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 11");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079578 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079578 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079578");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079579 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079579 node");
    l_cur_occ = l_republic->linkTo(l_form_array[102]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079579");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079580 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079580 node");
    l_cur_occ = l_republic->linkTo(l_form_array[103]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079580");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079581 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079581 node");
    l_cur_occ = l_republic->linkTo(l_form_array[104]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079581");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079582 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079582 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079582");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079583 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079583 node");
    l_cur_occ = l_republic->linkTo(l_form_array[105]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079583");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079584 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079584 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079584");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079585 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079585 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079585");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079586 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079586 node");
    l_cur_occ = l_republic->linkTo(l_form_array[106]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079586");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079587 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079587 node");
    l_cur_occ = l_republic->linkTo(l_form_array[107]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079587");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079588 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079588 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079588");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079589 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079589 node");
    l_cur_occ = l_republic->linkTo(l_form_array[108]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079589");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079591 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079591 node");
    l_cur_occ = l_republic->linkTo(l_form_array[109]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079591");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079592 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079592 node");
    l_cur_occ = l_republic->linkTo(l_form_array[110]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079592");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079593 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079593 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079593");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079594 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079594 node");
    l_cur_occ = l_republic->linkTo(l_form_array[111]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079594");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079595 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079595 node");
    l_cur_occ = l_republic->linkTo(l_form_array[112]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079595");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079596 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079596 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079596");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079597 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079597 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079597");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079598 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079598 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079598");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079599 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079599 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079599");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079600 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079600 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079600");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079601 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079601 node");
    l_cur_occ = l_republic->linkTo(l_form_array[33]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079601");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079602 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079602 node");
    l_cur_occ = l_republic->linkTo(l_form_array[113]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079602");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079603 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079603 node");
    l_cur_occ = l_republic->linkTo(l_form_array[114]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079603");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("?”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079607 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079607 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079607");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 12> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 12> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 12");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079608 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079608 node");
    l_cur_occ = l_republic->linkTo(l_form_array[115]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079608");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079609 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079609 node");
    l_cur_occ = l_republic->linkTo(l_form_array[116]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079609");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",”", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079612 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079612 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079612");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079613 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079613 node");
    l_cur_occ = l_republic->linkTo(l_form_array[58]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079613");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079616 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079616 node");
    l_cur_occ = l_republic->linkTo(l_form_array[117]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079616");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 13> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 13> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 13");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079618 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079618 node");
    l_cur_occ = l_republic->linkTo(l_form_array[4]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079618");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079620 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079620 node");
    l_cur_occ = l_republic->linkTo(l_form_array[40]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079620");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079621 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079621 node");
    l_cur_occ = l_republic->linkTo(l_form_array[118]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079621");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079622 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079622 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079622");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079623 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079623 node");
    l_cur_occ = l_republic->linkTo(l_form_array[119]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079623");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079624 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079624 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079624");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079625 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079625 node");
    l_cur_occ = l_republic->linkTo(l_form_array[120]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079625");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079626 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079626 node");
    l_cur_occ = l_republic->linkTo(l_form_array[24]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079626");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079627 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079627 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079627");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079629 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079629 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079629");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079630 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079630 node");
    l_cur_occ = l_republic->linkTo(l_form_array[1]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079630");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079631 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079631 node");
    l_cur_occ = l_republic->linkTo(l_form_array[30]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079631");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079632 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079632 node");
    l_cur_occ = l_republic->linkTo(l_form_array[121]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079632");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079633 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079633 node");
    l_cur_occ = l_republic->linkTo(l_form_array[122]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079633");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079634 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079634 node");
    l_cur_occ = l_republic->linkTo(l_form_array[123]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079634");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079635 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079635 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079635");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079636 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079636 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079636");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079637 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079637 node");
    l_cur_occ = l_republic->linkTo(l_form_array[47]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079637");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079638 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079638 node");
    l_cur_occ = l_republic->linkTo(l_form_array[124]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079638");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079640 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079640 node");
    l_cur_occ = l_republic->linkTo(l_form_array[125]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079640");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079641 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079641 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079641");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079642 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079642 node");
    l_cur_occ = l_republic->linkTo(l_form_array[126]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079642");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079643 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079643 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079643");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079644 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079644 node");
    l_cur_occ = l_republic->linkTo(l_form_array[127]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079644");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079645 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079645 node");
    l_cur_occ = l_republic->linkTo(l_form_array[128]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079645");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079647 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079647 node");
    l_cur_occ = l_republic->linkTo(l_form_array[129]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079647");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079648 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079648 node");
    l_cur_occ = l_republic->linkTo(l_form_array[130]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079648");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079649 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079649 node");
    l_cur_occ = l_republic->linkTo(l_form_array[28]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079649");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079650 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079650 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079650");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079652 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079652 node");
    l_cur_occ = l_republic->linkTo(l_form_array[10]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079652");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079653 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079653 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079653");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079654 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079654 node");
    l_cur_occ = l_republic->linkTo(l_form_array[53]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079654");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079655 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079655 node");
    l_cur_occ = l_republic->linkTo(l_form_array[73]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079655");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079656 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079656 node");
    l_cur_occ = l_republic->linkTo(l_form_array[77]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079656");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079657 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079657 node");
    l_cur_occ = l_republic->linkTo(l_form_array[131]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079657");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079658 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079658 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079658");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079659 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079659 node");
    l_cur_occ = l_republic->linkTo(l_form_array[132]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079659");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079660 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079660 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079660");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079661 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079661 node");
    l_cur_occ = l_republic->linkTo(l_form_array[133]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079661");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079662 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079662 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079662");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079664 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079664 node");
    l_cur_occ = l_republic->linkTo(l_form_array[10]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079664");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079665 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079665 node");
    l_cur_occ = l_republic->linkTo(l_form_array[135]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079665");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079666 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079666 node");
    l_cur_occ = l_republic->linkTo(l_form_array[122]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079666");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079667 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079667 node");
    l_cur_occ = l_republic->linkTo(l_form_array[47]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079667");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(":", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079669 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079669 node");
    l_cur_occ = l_republic->linkTo(l_form_array[94]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079669");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079670 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079670 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079670");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079671 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079671 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079671");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079672 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079672 node");
    l_cur_occ = l_republic->linkTo(l_form_array[127]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079672");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079673 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079673 node");
    l_cur_occ = l_republic->linkTo(l_form_array[95]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079673");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079674 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079674 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079674");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079675 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079675 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079675");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079676 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079676 node");
    l_cur_occ = l_republic->linkTo(l_form_array[136]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079676");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079678 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079678 node");
    l_cur_occ = l_republic->linkTo(l_form_array[29]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079678");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 14> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 14> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 14");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079679 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079679 node");
    l_cur_occ = l_republic->linkTo(l_form_array[30]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079679");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079680 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079680 node");
    l_cur_occ = l_republic->linkTo(l_form_array[137]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079680");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079681 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079681 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079681");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079682 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079682 node");
    l_cur_occ = l_republic->linkTo(l_form_array[33]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079682");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079683 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079683 node");
    l_cur_occ = l_republic->linkTo(l_form_array[138]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079683");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079687 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079687 node");
    l_cur_occ = l_republic->linkTo(l_form_array[52]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079687");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 15> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 15> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 15");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079688 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079688 node");
    l_cur_occ = l_republic->linkTo(l_form_array[139]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079688");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079689 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079689 node");
    l_cur_occ = l_republic->linkTo(l_form_array[137]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079689");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",”", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079692 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079692 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079692");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079693 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079693 node");
    l_cur_occ = l_republic->linkTo(l_form_array[58]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079693");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079696 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079696 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079696");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079697 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079697 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079697");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079698 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079698 node");
    l_cur_occ = l_republic->linkTo(l_form_array[127]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079698");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079699 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079699 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079699");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079703 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079703 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079703");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 16> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 16> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 16");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079704 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079704 node");
    l_cur_occ = l_republic->linkTo(l_form_array[28]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079704");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079705 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079705 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079705");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079706 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079706 node");
    l_cur_occ = l_republic->linkTo(l_form_array[69]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079706");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079707 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079707 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079707");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079708 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079708 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079708");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079709 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079709 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079709");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079710 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079710 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079710");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("?”", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079714 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079714 node");
    l_cur_occ = l_republic->linkTo(l_form_array[56]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079714");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("“", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 17> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 17> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 17");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079715 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079715 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079715");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079717 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079717 node");
    l_cur_occ = l_republic->linkTo(l_form_array[140]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079717");
    l_cur_occ->setType("RVSHR");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Shorey - Sentence 18> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Shorey - Sentence 18> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Shorey - Sentence 18");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVSHR");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence A-SHR-079718 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence A-SHR-079718 node");
    l_cur_occ = l_republic->linkTo(l_form_array[80]->item_id(), "OCCUR");
    l_cur_occ->setText("A-SHR-079718");
    l_cur_occ->setType("RVSHR");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic version Jowett node
    qCDebug(g_cat_silence) << QString("Creating Republic version Jowett node");
    M1Store::Item_lv2* l_version_jowett = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX  | M1Env::IS_SPECIAL,
        // label
        "Jowett (version)",
        // Special Item flags
        SI_IS_TYPE  | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RVJWT",
        // icon path
        FOLDER_ICON_PATH);
    l_version_jowett->setType("TYPE_");
    l_version_jowett->setType("TXTVR");
    l_republic_versions->linkTo(l_version_jowett, "OWNS_");

    l_cur_ver_occ = nullptr;
    l_cur_sentence_occ = nullptr;
    l_cur_sentence = nullptr;
    // creation of Republic occurrence B-JWT-070758 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070758 node");
    l_cur_occ = l_republic->linkTo(l_form_array[77]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070758");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_stephanus_array[0]->linkTo(l_cur_occ, "OWNS_");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 1> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 1> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 1");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070759 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070759 node");
    l_cur_occ = l_republic->linkTo(l_form_array[100]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070759");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070761 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070761 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070761");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 2> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 2> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 2");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070762 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070762 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070762");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070763 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070763 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070763");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070765 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070765 node");
    l_cur_occ = l_republic->linkTo(l_form_array[141]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070765");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070766 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070766 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070766");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070767 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070767 node");
    l_cur_occ = l_republic->linkTo(l_form_array[142]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070767");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070768 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070768 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070768");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070769 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070769 node");
    l_cur_occ = l_republic->linkTo(l_form_array[8]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070769");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070770 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070770 node");
    l_cur_occ = l_republic->linkTo(l_form_array[9]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070770");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070771 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070771 node");
    l_cur_occ = l_republic->linkTo(l_form_array[143]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070771");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070772 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070772 node");
    l_cur_occ = l_republic->linkTo(l_form_array[144]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070772");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070773 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070773 node");
    l_cur_occ = l_republic->linkTo(l_form_array[142]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070773");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070774 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070774 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070774");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070775 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070775 node");
    l_cur_occ = l_republic->linkTo(l_form_array[145]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070775");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070776 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070776 node");
    l_cur_occ = l_republic->linkTo(l_form_array[12]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070776");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070778 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070778 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070778");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070779 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070779 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070779");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070781 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070781 node");
    l_cur_occ = l_republic->linkTo(l_form_array[126]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070781");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070782 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070782 node");
    l_cur_occ = l_republic->linkTo(l_form_array[146]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070782");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070783 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070783 node");
    l_cur_occ = l_republic->linkTo(l_form_array[147]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070783");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070784 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070784 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070784");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070785 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070785 node");
    l_cur_occ = l_republic->linkTo(l_form_array[24]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070785");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070786 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070786 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070786");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070787 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070787 node");
    l_cur_occ = l_republic->linkTo(l_form_array[26]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070787");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070788 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070788 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070788");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070789 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070789 node");
    l_cur_occ = l_republic->linkTo(l_form_array[56]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070789");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070790 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070790 node");
    l_cur_occ = l_republic->linkTo(l_form_array[12]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070790");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070791 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070791 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070791");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070792 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070792 node");
    l_cur_occ = l_republic->linkTo(l_form_array[148]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070792");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070793 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070793 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070793");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070794 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070794 node");
    l_cur_occ = l_republic->linkTo(l_form_array[18]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070794");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070796 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070796 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070796");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070797 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070797 node");
    l_cur_occ = l_republic->linkTo(l_form_array[149]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070797");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070798 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070798 node");
    l_cur_occ = l_republic->linkTo(l_form_array[150]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070798");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070799 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070799 node");
    l_cur_occ = l_republic->linkTo(l_form_array[84]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070799");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070800 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070800 node");
    l_cur_occ = l_republic->linkTo(l_form_array[52]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070800");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070801 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070801 node");
    l_cur_occ = l_republic->linkTo(l_form_array[151]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070801");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070803 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070803 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070803");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070804 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070804 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070804");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070805 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070805 node");
    l_cur_occ = l_republic->linkTo(l_form_array[46]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070805");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070806 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070806 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070806");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070807 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070807 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070807");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070808 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070808 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070808");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070809 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070809 node");
    l_cur_occ = l_republic->linkTo(l_form_array[36]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070809");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070810 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070810 node");
    l_cur_occ = l_republic->linkTo(l_form_array[152]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070810");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070812 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070812 node");
    l_cur_occ = l_republic->linkTo(l_form_array[153]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070812");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField(":", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 3> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 3> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 3");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070814 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070814 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070814");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070815 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070815 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070815");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070816 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070816 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070816");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070817 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070817 node");
    l_cur_occ = l_republic->linkTo(l_form_array[154]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070817");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070819 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070819 node");
    l_cur_occ = l_republic->linkTo(l_form_array[155]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070819");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070820 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070820 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070820");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070821 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070821 node");
    l_cur_occ = l_republic->linkTo(l_form_array[76]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070821");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070823 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070823 node");
    l_cur_occ = l_republic->linkTo(l_form_array[84]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070823");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070824 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070824 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070824");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070825 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070825 node");
    l_cur_occ = l_republic->linkTo(l_form_array[156]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070825");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070826 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070826 node");
    l_cur_occ = l_republic->linkTo(l_form_array[157]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070826");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070827 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070827 node");
    l_cur_occ = l_republic->linkTo(l_form_array[9]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070827");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070829 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070829 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070829");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070830 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070830 node");
    l_cur_occ = l_republic->linkTo(l_form_array[158]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070830");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070831 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070831 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070831");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070832 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070832 node");
    l_cur_occ = l_republic->linkTo(l_form_array[159]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070832");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070833 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070833 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070833");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070834 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070834 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070834");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070835 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070835 node");
    l_cur_occ = l_republic->linkTo(l_form_array[160]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070835");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070836 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070836 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070836");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070837 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070837 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070837");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070838 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070838 node");
    l_cur_occ = l_republic->linkTo(l_form_array[161]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070838");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070839 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070839 node");
    l_cur_occ = l_republic->linkTo(l_form_array[162]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070839");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070841 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070841 node");
    l_cur_occ = l_republic->linkTo(l_form_array[52]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070841");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 4> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 4> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 4");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070842 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070842 node");
    l_cur_occ = l_republic->linkTo(l_form_array[53]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070842");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070843 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070843 node");
    l_cur_occ = l_republic->linkTo(l_form_array[54]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070843");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070844 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070844 node");
    l_cur_occ = l_republic->linkTo(l_form_array[120]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070844");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070845 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070845 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070845");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070846 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070846 node");
    l_cur_occ = l_republic->linkTo(l_form_array[149]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070846");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070847 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070847 node");
    l_cur_occ = l_republic->linkTo(l_form_array[150]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070847");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070848 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070848 node");
    l_cur_occ = l_republic->linkTo(l_form_array[84]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070848");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070849 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070849 node");
    l_cur_occ = l_republic->linkTo(l_form_array[163]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070849");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070850 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070850 node");
    l_cur_occ = l_republic->linkTo(l_form_array[164]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070850");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070852 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070852 node");
    l_cur_occ = l_republic->linkTo(l_form_array[10]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070852");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 5> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 5> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 5");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070853 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070853 node");
    l_cur_occ = l_republic->linkTo(l_form_array[60]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070853");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070854 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070854 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070854");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070855 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070855 node");
    l_cur_occ = l_republic->linkTo(l_form_array[62]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070855");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070856 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070856 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070856");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070857 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070857 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070857");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070858 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070858 node");
    l_cur_occ = l_republic->linkTo(l_form_array[87]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070858");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070860 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070860 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070860");
    l_cur_occ->setType("RVJWT");
    l_stephanus_array[1]->linkTo(l_cur_occ, "OWNS_");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070861 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070861 node");
    l_cur_occ = l_republic->linkTo(l_form_array[165]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070861");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070862 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070862 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070862");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070863 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070863 node");
    l_cur_occ = l_republic->linkTo(l_form_array[166]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070863");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070864 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070864 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070864");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070865 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070865 node");
    l_cur_occ = l_republic->linkTo(l_form_array[145]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070865");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070866 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070866 node");
    l_cur_occ = l_republic->linkTo(l_form_array[12]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070866");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070867 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070867 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070867");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070868 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070868 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070868");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070869 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070869 node");
    l_cur_occ = l_republic->linkTo(l_form_array[16]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070869");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070870 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070870 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070870");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070871 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070871 node");
    l_cur_occ = l_republic->linkTo(l_form_array[67]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070871");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070872 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070872 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070872");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070873 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070873 node");
    l_cur_occ = l_republic->linkTo(l_form_array[167]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070873");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070874 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070874 node");
    l_cur_occ = l_republic->linkTo(l_form_array[168]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070874");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070875 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070875 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070875");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070876 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070876 node");
    l_cur_occ = l_republic->linkTo(l_form_array[169]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070876");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070877 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070877 node");
    l_cur_occ = l_republic->linkTo(l_form_array[170]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070877");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070878 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070878 node");
    l_cur_occ = l_republic->linkTo(l_form_array[171]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070878");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070879 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070879 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070879");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070880 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070880 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070880");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070881 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070881 node");
    l_cur_occ = l_republic->linkTo(l_form_array[167]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070881");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070883 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070883 node");
    l_cur_occ = l_republic->linkTo(l_form_array[11]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070883");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070884 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070884 node");
    l_cur_occ = l_republic->linkTo(l_form_array[172]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070884");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070885 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070885 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070885");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070886 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070886 node");
    l_cur_occ = l_republic->linkTo(l_form_array[169]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070886");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070887 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070887 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070887");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070888 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070888 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070888");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070889 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070889 node");
    l_cur_occ = l_republic->linkTo(l_form_array[160]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070889");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070890 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070890 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070890");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070891 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070891 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070891");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070892 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070892 node");
    l_cur_occ = l_republic->linkTo(l_form_array[167]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070892");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070893 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070893 node");
    l_cur_occ = l_republic->linkTo(l_form_array[14]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070893");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070894 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070894 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070894");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070895 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070895 node");
    l_cur_occ = l_republic->linkTo(l_form_array[167]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070895");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070896 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070896 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070896");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070897 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070897 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070897");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070898 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070898 node");
    l_cur_occ = l_republic->linkTo(l_form_array[160]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070898");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070899 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070899 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070899");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070900 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070900 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070900");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070901 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070901 node");
    l_cur_occ = l_republic->linkTo(l_form_array[169]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070901");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070903 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070903 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070903");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070904 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070904 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070904");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070905 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070905 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070905");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070906 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070906 node");
    l_cur_occ = l_republic->linkTo(l_form_array[154]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070906");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070908 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070908 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070908");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070909 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070909 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070909");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070910 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070910 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070910");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070911 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070911 node");
    l_cur_occ = l_republic->linkTo(l_form_array[46]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070911");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070913 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070913 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070913");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 6> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 6> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 6");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070914 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070914 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070914");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070915 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070915 node");
    l_cur_occ = l_republic->linkTo(l_form_array[173]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070915");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070916 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070916 node");
    l_cur_occ = l_republic->linkTo(l_form_array[46]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070916");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070918 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070918 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070918");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 7> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 7> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 7");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070919 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070919 node");
    l_cur_occ = l_republic->linkTo(l_form_array[174]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070919");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070920 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070920 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070920");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070921 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070921 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070921");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070922 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070922 node");
    l_cur_occ = l_republic->linkTo(l_form_array[120]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070922");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070923 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070923 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070923");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070924 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070924 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070924");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070925 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070925 node");
    l_cur_occ = l_republic->linkTo(l_form_array[114]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070925");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070926 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070926 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070926");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070927 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070927 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070927");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070928 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070928 node");
    l_cur_occ = l_republic->linkTo(l_form_array[175]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070928");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070929 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070929 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070929");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070930 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070930 node");
    l_cur_occ = l_republic->linkTo(l_form_array[127]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070930");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070931 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070931 node");
    l_cur_occ = l_republic->linkTo(l_form_array[143]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070931");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070932 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070932 node");
    l_cur_occ = l_republic->linkTo(l_form_array[26]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070932");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070934 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070934 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070934");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070935 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070935 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070935");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070936 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070936 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070936");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070937 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070937 node");
    l_cur_occ = l_republic->linkTo(l_form_array[114]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070937");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070938 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070938 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070938");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070939 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070939 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070939");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070940 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070940 node");
    l_cur_occ = l_republic->linkTo(l_form_array[87]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070940");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070942 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070942 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070942");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070943 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070943 node");
    l_cur_occ = l_republic->linkTo(l_form_array[93]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070943");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070944 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070944 node");
    l_cur_occ = l_republic->linkTo(l_form_array[152]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070944");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070946 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070946 node");
    l_cur_occ = l_republic->linkTo(l_form_array[143]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070946");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070947 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070947 node");
    l_cur_occ = l_republic->linkTo(l_form_array[75]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070947");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070949 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070949 node");
    l_cur_occ = l_republic->linkTo(l_form_array[54]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070949");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070951 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070951 node");
    l_cur_occ = l_republic->linkTo(l_form_array[10]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070951");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 8> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 8> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 8");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070952 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070952 node");
    l_cur_occ = l_republic->linkTo(l_form_array[90]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070952");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070953 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070953 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070953");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070954 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070954 node");
    l_cur_occ = l_republic->linkTo(l_form_array[25]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070954");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070955 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070955 node");
    l_cur_occ = l_republic->linkTo(l_form_array[176]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070955");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070956 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070956 node");
    l_cur_occ = l_republic->linkTo(l_form_array[53]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070956");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070957 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070957 node");
    l_cur_occ = l_republic->linkTo(l_form_array[177]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070957");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070958 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070958 node");
    l_cur_occ = l_republic->linkTo(l_form_array[178]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070958");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070959 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070959 node");
    l_cur_occ = l_republic->linkTo(l_form_array[122]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070959");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070960 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070960 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070960");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070961 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070961 node");
    l_cur_occ = l_republic->linkTo(l_form_array[179]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070961");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070962 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070962 node");
    l_cur_occ = l_republic->linkTo(l_form_array[98]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070962");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070963 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070963 node");
    l_cur_occ = l_republic->linkTo(l_form_array[99]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070963");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070965 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070965 node");
    l_cur_occ = l_republic->linkTo(l_form_array[101]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070965");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070966 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070966 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070966");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070967 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070967 node");
    l_cur_occ = l_republic->linkTo(l_form_array[180]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070967");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070968 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070968 node");
    l_cur_occ = l_republic->linkTo(l_form_array[103]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070968");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070969 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070969 node");
    l_cur_occ = l_republic->linkTo(l_form_array[181]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070969");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070970 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070970 node");
    l_cur_occ = l_republic->linkTo(l_form_array[182]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070970");
    l_cur_occ->setType("RVJWT");
    l_stephanus_array[2]->linkTo(l_cur_occ, "OWNS_");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070971 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070971 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070971");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070972 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070972 node");
    l_cur_occ = l_republic->linkTo(l_form_array[183]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070972");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070973 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070973 node");
    l_cur_occ = l_republic->linkTo(l_form_array[24]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070973");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070974 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070974 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070974");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070976 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070976 node");
    l_cur_occ = l_republic->linkTo(l_form_array[109]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070976");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070977 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070977 node");
    l_cur_occ = l_republic->linkTo(l_form_array[184]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070977");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070978 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070978 node");
    l_cur_occ = l_republic->linkTo(l_form_array[24]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070978");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070979 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070979 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070979");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070980 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070980 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070980");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070981 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070981 node");
    l_cur_occ = l_republic->linkTo(l_form_array[185]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070981");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070982 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070982 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070982");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070983 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070983 node");
    l_cur_occ = l_republic->linkTo(l_form_array[186]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070983");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070984 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070984 node");
    l_cur_occ = l_republic->linkTo(l_form_array[187]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070984");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070985 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070985 node");
    l_cur_occ = l_republic->linkTo(l_form_array[7]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070985");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070986 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070986 node");
    l_cur_occ = l_republic->linkTo(l_form_array[33]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070986");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070987 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070987 node");
    l_cur_occ = l_republic->linkTo(l_form_array[188]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070987");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070989 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070989 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070989");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 9> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 9> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 9");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070990 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070990 node");
    l_cur_occ = l_republic->linkTo(l_form_array[115]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070990");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070991 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070991 node");
    l_cur_occ = l_republic->linkTo(l_form_array[54]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070991");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070992 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070992 node");
    l_cur_occ = l_republic->linkTo(l_form_array[189]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070992");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070993 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070993 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070993");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070994 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070994 node");
    l_cur_occ = l_republic->linkTo(l_form_array[190]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070994");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070995 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070995 node");
    l_cur_occ = l_republic->linkTo(l_form_array[191]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070995");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070996 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070996 node");
    l_cur_occ = l_republic->linkTo(l_form_array[183]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070996");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070998 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070998 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070998");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-070999 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-070999 node");
    l_cur_occ = l_republic->linkTo(l_form_array[79]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-070999");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071001 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071001 node");
    l_cur_occ = l_republic->linkTo(l_form_array[117]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071001");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 10> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 10> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 10");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071003 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071003 node");
    l_cur_occ = l_republic->linkTo(l_form_array[4]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071003");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071005 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071005 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071005");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071006 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071006 node");
    l_cur_occ = l_republic->linkTo(l_form_array[192]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071006");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071007 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071007 node");
    l_cur_occ = l_republic->linkTo(l_form_array[122]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071007");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071008 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071008 node");
    l_cur_occ = l_republic->linkTo(l_form_array[31]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071008");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071009 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071009 node");
    l_cur_occ = l_republic->linkTo(l_form_array[27]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071009");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071010 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071010 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071010");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071011 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071011 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071011");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071012 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071012 node");
    l_cur_occ = l_republic->linkTo(l_form_array[120]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071012");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071013 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071013 node");
    l_cur_occ = l_republic->linkTo(l_form_array[24]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071013");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071014 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071014 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071014");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071016 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071016 node");
    l_cur_occ = l_republic->linkTo(l_form_array[193]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071016");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 11> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 11> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 11");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071017 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071017 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071017");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071018 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071018 node");
    l_cur_occ = l_republic->linkTo(l_form_array[194]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071018");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071019 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071019 node");
    l_cur_occ = l_republic->linkTo(l_form_array[195]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071019");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071020 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071020 node");
    l_cur_occ = l_republic->linkTo(l_form_array[196]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071020");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071021 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071021 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071021");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071022 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071022 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071022");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071023 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071023 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071023");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071024 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071024 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071024");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071025 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071025 node");
    l_cur_occ = l_republic->linkTo(l_form_array[197]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071025");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071026 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071026 node");
    l_cur_occ = l_republic->linkTo(l_form_array[24]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071026");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071027 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071027 node");
    l_cur_occ = l_republic->linkTo(l_form_array[103]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071027");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071028 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071028 node");
    l_cur_occ = l_republic->linkTo(l_form_array[26]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071028");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071029 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071029 node");
    l_cur_occ = l_republic->linkTo(l_form_array[170]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071029");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071030 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071030 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071030");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071031 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071031 node");
    l_cur_occ = l_republic->linkTo(l_form_array[174]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071031");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071032 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071032 node");
    l_cur_occ = l_republic->linkTo(l_form_array[121]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071032");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071034 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071034 node");
    l_cur_occ = l_republic->linkTo(l_form_array[66]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071034");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071035 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071035 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071035");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071036 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071036 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071036");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071037 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071037 node");
    l_cur_occ = l_republic->linkTo(l_form_array[119]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071037");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071038 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071038 node");
    l_cur_occ = l_republic->linkTo(l_form_array[198]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071038");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071039 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071039 node");
    l_cur_occ = l_republic->linkTo(l_form_array[199]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071039");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071040 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071040 node");
    l_cur_occ = l_republic->linkTo(l_form_array[33]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071040");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071041 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071041 node");
    l_cur_occ = l_republic->linkTo(l_form_array[141]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071041");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071042 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071042 node");
    l_cur_occ = l_republic->linkTo(l_form_array[28]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071042");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071044 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071044 node");
    l_cur_occ = l_republic->linkTo(l_form_array[10]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071044");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 12> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 12> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 12");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071045 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071045 node");
    l_cur_occ = l_republic->linkTo(l_form_array[81]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071045");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071046 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071046 node");
    l_cur_occ = l_republic->linkTo(l_form_array[53]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071046");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071047 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071047 node");
    l_cur_occ = l_republic->linkTo(l_form_array[200]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071047");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071048 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071048 node");
    l_cur_occ = l_republic->linkTo(l_form_array[17]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071048");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071049 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071049 node");
    l_cur_occ = l_republic->linkTo(l_form_array[201]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071049");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071050 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071050 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071050");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071051 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071051 node");
    l_cur_occ = l_republic->linkTo(l_form_array[35]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071051");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071052 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071052 node");
    l_cur_occ = l_republic->linkTo(l_form_array[202]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071052");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(":", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071054 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071054 node");
    l_cur_occ = l_republic->linkTo(l_form_array[94]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071054");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071055 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071055 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071055");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071056 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071056 node");
    l_cur_occ = l_republic->linkTo(l_form_array[5]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071056");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071057 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071057 node");
    l_cur_occ = l_republic->linkTo(l_form_array[143]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071057");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071058 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071058 node");
    l_cur_occ = l_republic->linkTo(l_form_array[26]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071058");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071059 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071059 node");
    l_cur_occ = l_republic->linkTo(l_form_array[95]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071059");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071060 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071060 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071060");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071061 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071061 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071061");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071062 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071062 node");
    l_cur_occ = l_republic->linkTo(l_form_array[136]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071062");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071065 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071065 node");
    l_cur_occ = l_republic->linkTo(l_form_array[30]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071065");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("(", false, M1Env::PCTLF_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 13> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 13> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 13");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071066 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071066 node");
    l_cur_occ = l_republic->linkTo(l_form_array[115]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071066");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071067 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071067 node");
    l_cur_occ = l_republic->linkTo(l_form_array[203]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071067");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071068 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071068 node");
    l_cur_occ = l_republic->linkTo(l_form_array[50]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071068");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071069 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071069 node");
    l_cur_occ = l_republic->linkTo(l_form_array[23]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071069");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(".)", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071072 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071072 node");
    l_cur_occ = l_republic->linkTo(l_form_array[52]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071072");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 14> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 14> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 14");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071073 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071073 node");
    l_cur_occ = l_republic->linkTo(l_form_array[204]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071073");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071074 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071074 node");
    l_cur_occ = l_republic->linkTo(l_form_array[32]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071074");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071075 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071075 node");
    l_cur_occ = l_republic->linkTo(l_form_array[3]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071075");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071076 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071076 node");
    l_cur_occ = l_republic->linkTo(l_form_array[127]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071076");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071077 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071077 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071077");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071079 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071079 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071079");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 15> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 15> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 15");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071080 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071080 node");
    l_cur_occ = l_republic->linkTo(l_form_array[69]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071080");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071081 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071081 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071081");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071082 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071082 node");
    l_cur_occ = l_republic->linkTo(l_form_array[37]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071082");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071083 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071083 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071083");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071084 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071084 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071084");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071086 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071086 node");
    l_cur_occ = l_republic->linkTo(l_form_array[134]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071086");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("true", false, M1Env::CAPTL_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Jowett - Sentence 16> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Jowett - Sentence 16> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Jowett - Sentence 16");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVJWT");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071087 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071087 node");
    l_cur_occ = l_republic->linkTo(l_form_array[69]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071087");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071088 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071088 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071088");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071090 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071090 node");
    l_cur_occ = l_republic->linkTo(l_form_array[205]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071090");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071091 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071091 node");
    l_cur_occ = l_republic->linkTo(l_form_array[140]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071091");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071092 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071092 node");
    l_cur_occ = l_republic->linkTo(l_form_array[101]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071092");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071093 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071093 node");
    l_cur_occ = l_republic->linkTo(l_form_array[56]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071093");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071094 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071094 node");
    l_cur_occ = l_republic->linkTo(l_form_array[170]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071094");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071095 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071095 node");
    l_cur_occ = l_republic->linkTo(l_form_array[15]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071095");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071096 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071096 node");
    l_cur_occ = l_republic->linkTo(l_form_array[41]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071096");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071097 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071097 node");
    l_cur_occ = l_republic->linkTo(l_form_array[206]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071097");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071098 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071098 node");
    l_cur_occ = l_republic->linkTo(l_form_array[73]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071098");
    l_cur_occ->setType("RVJWT");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence B-JWT-071099 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence B-JWT-071099 node");
    l_cur_occ = l_republic->linkTo(l_form_array[207]->item_id(), "OCCUR");
    l_cur_occ->setText("B-JWT-071099");
    l_cur_occ->setType("RVJWT");
    l_cur_occ->setField("?", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic version Greek node
    qCDebug(g_cat_silence) << QString("Creating Republic version Greek node");
    M1Store::Item_lv2* l_version_greek = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX  | M1Env::IS_SPECIAL,
        // label
        "Greek (version)",
        // Special Item flags
        SI_IS_TYPE  | M1Env::SI_REQUIRES_EDGE,
        // mnemonic
        "RVGRK",
        // icon path
        FOLDER_ICON_PATH);
    l_version_greek->setType("TYPE_");
    l_version_greek->setType("TXTVR");
    l_republic_versions->linkTo(l_version_greek, "OWNS_");

    l_cur_ver_occ = nullptr;
    l_cur_sentence_occ = nullptr;
    l_cur_sentence = nullptr;
    // creation of Republic occurrence Z-GRK-054455 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054455 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054455");
    l_cur_occ->setType("RVGRK");
    l_stephanus_array[0]->linkTo(l_cur_occ, "OWNS_");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 1> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 1> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 1");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054456 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054456 node");
    l_cur_occ = l_republic->linkTo(l_form_array[209]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054456");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054458 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054458 node");
    l_cur_occ = l_republic->linkTo(l_form_array[210]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054458");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 2> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 2> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 2");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054459 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054459 node");
    l_cur_occ = l_republic->linkTo(l_form_array[211]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054459");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054460 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054460 node");
    l_cur_occ = l_republic->linkTo(l_form_array[212]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054460");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054461 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054461 node");
    l_cur_occ = l_republic->linkTo(l_form_array[213]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054461");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054462 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054462 node");
    l_cur_occ = l_republic->linkTo(l_form_array[214]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054462");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054463 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054463 node");
    l_cur_occ = l_republic->linkTo(l_form_array[215]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054463");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054465 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054465 node");
    l_cur_occ = l_republic->linkTo(l_form_array[216]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054465");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054466 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054466 node");
    l_cur_occ = l_republic->linkTo(l_form_array[217]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054466");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054467 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054467 node");
    l_cur_occ = l_republic->linkTo(l_form_array[218]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054467");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054468 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054468 node");
    l_cur_occ = l_republic->linkTo(l_form_array[219]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054468");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054469 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054469 node");
    l_cur_occ = l_republic->linkTo(l_form_array[215]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054469");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054470 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054470 node");
    l_cur_occ = l_republic->linkTo(l_form_array[219]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054470");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054472 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054472 node");
    l_cur_occ = l_republic->linkTo(l_form_array[220]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054472");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054473 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054473 node");
    l_cur_occ = l_republic->linkTo(l_form_array[221]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054473");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054474 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054474 node");
    l_cur_occ = l_republic->linkTo(l_form_array[222]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054474");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054475 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054475 node");
    l_cur_occ = l_republic->linkTo(l_form_array[223]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054475");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054476 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054476 node");
    l_cur_occ = l_republic->linkTo(l_form_array[224]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054476");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054477 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054477 node");
    l_cur_occ = l_republic->linkTo(l_form_array[225]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054477");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054478 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054478 node");
    l_cur_occ = l_republic->linkTo(l_form_array[226]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054478");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054480 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054480 node");
    l_cur_occ = l_republic->linkTo(l_form_array[227]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054480");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054481 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054481 node");
    l_cur_occ = l_republic->linkTo(l_form_array[228]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054481");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054483 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054483 node");
    l_cur_occ = l_republic->linkTo(l_form_array[229]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054483");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054484 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054484 node");
    l_cur_occ = l_republic->linkTo(l_form_array[230]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054484");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054485 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054485 node");
    l_cur_occ = l_republic->linkTo(l_form_array[231]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054485");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054486 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054486 node");
    l_cur_occ = l_republic->linkTo(l_form_array[232]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054486");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054487 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054487 node");
    l_cur_occ = l_republic->linkTo(l_form_array[233]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054487");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054488 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054488 node");
    l_cur_occ = l_republic->linkTo(l_form_array[234]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054488");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054490 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054490 node");
    l_cur_occ = l_republic->linkTo(l_form_array[235]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054490");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 3> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 3> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 3");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054491 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054491 node");
    l_cur_occ = l_republic->linkTo(l_form_array[236]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054491");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054493 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054493 node");
    l_cur_occ = l_republic->linkTo(l_form_array[237]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054493");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 4> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 4> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 4");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054494 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054494 node");
    l_cur_occ = l_republic->linkTo(l_form_array[238]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054494");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054495 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054495 node");
    l_cur_occ = l_republic->linkTo(l_form_array[239]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054495");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054496 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054496 node");
    l_cur_occ = l_republic->linkTo(l_form_array[240]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054496");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054497 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054497 node");
    l_cur_occ = l_republic->linkTo(l_form_array[241]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054497");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054498 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054498 node");
    l_cur_occ = l_republic->linkTo(l_form_array[242]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054498");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054500 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054500 node");
    l_cur_occ = l_republic->linkTo(l_form_array[243]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054500");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054501 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054501 node");
    l_cur_occ = l_republic->linkTo(l_form_array[244]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054501");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054502 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054502 node");
    l_cur_occ = l_republic->linkTo(l_form_array[245]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054502");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054503 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054503 node");
    l_cur_occ = l_republic->linkTo(l_form_array[221]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054503");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054504 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054504 node");
    l_cur_occ = l_republic->linkTo(l_form_array[246]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054504");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054505 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054505 node");
    l_cur_occ = l_republic->linkTo(l_form_array[247]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054505");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054506 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054506 node");
    l_cur_occ = l_republic->linkTo(l_form_array[237]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054506");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054507 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054507 node");
    l_cur_occ = l_republic->linkTo(l_form_array[248]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054507");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054508 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054508 node");
    l_cur_occ = l_republic->linkTo(l_form_array[249]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054508");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054509 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054509 node");
    l_cur_occ = l_republic->linkTo(l_form_array[250]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054509");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054510 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054510 node");
    l_cur_occ = l_republic->linkTo(l_form_array[251]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054510");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054511 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054511 node");
    l_cur_occ = l_republic->linkTo(l_form_array[252]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054511");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054512 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054512 node");
    l_cur_occ = l_republic->linkTo(l_form_array[216]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054512");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054513 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054513 node");
    l_cur_occ = l_republic->linkTo(l_form_array[222]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054513");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054514 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054514 node");
    l_cur_occ = l_republic->linkTo(l_form_array[253]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054514");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054515 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054515 node");
    l_cur_occ = l_republic->linkTo(l_form_array[254]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054515");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054516 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054516 node");
    l_cur_occ = l_republic->linkTo(l_form_array[255]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054516");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054518 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054518 node");
    l_cur_occ = l_republic->linkTo(l_form_array[256]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054518");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 5> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 5> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 5");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054519 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054519 node");
    l_cur_occ = l_republic->linkTo(l_form_array[257]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054519");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054520 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054520 node");
    l_cur_occ = l_republic->linkTo(l_form_array[258]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054520");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054522 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054522 node");
    l_cur_occ = l_republic->linkTo(l_form_array[259]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054522");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054523 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054523 node");
    l_cur_occ = l_republic->linkTo(l_form_array[260]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054523");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054524 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054524 node");
    l_cur_occ = l_republic->linkTo(l_form_array[261]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054524");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054526 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054526 node");
    l_cur_occ = l_republic->linkTo(l_form_array[262]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054526");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054527 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054527 node");
    l_cur_occ = l_republic->linkTo(l_form_array[238]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054527");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054528 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054528 node");
    l_cur_occ = l_republic->linkTo(l_form_array[263]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054528");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054529 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054529 node");
    l_cur_occ = l_republic->linkTo(l_form_array[264]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054529");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054531 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054531 node");
    l_cur_occ = l_republic->linkTo(l_form_array[265]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054531");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 6> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 6> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 6");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054532 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054532 node");
    l_cur_occ = l_republic->linkTo(l_form_array[236]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054532");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054534 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054534 node");
    l_cur_occ = l_republic->linkTo(l_form_array[210]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054534");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 7> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 7> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 7");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054535 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054535 node");
    l_cur_occ = l_republic->linkTo(l_form_array[266]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054535");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054536 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054536 node");
    l_cur_occ = l_republic->linkTo(l_form_array[267]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054536");
    l_cur_occ->setType("RVGRK");
    l_stephanus_array[1]->linkTo(l_cur_occ, "OWNS_");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054537 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054537 node");
    l_cur_occ = l_republic->linkTo(l_form_array[268]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054537");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054538 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054538 node");
    l_cur_occ = l_republic->linkTo(l_form_array[269]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054538");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054539 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054539 node");
    l_cur_occ = l_republic->linkTo(l_form_array[270]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054539");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054540 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054540 node");
    l_cur_occ = l_republic->linkTo(l_form_array[216]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054540");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054541 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054541 node");
    l_cur_occ = l_republic->linkTo(l_form_array[271]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054541");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054542 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054542 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054542");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054543 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054543 node");
    l_cur_occ = l_republic->linkTo(l_form_array[227]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054543");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054544 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054544 node");
    l_cur_occ = l_republic->linkTo(l_form_array[272]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054544");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054545 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054545 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054545");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054546 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054546 node");
    l_cur_occ = l_republic->linkTo(l_form_array[216]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054546");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054547 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054547 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054547");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054548 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054548 node");
    l_cur_occ = l_republic->linkTo(l_form_array[273]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054548");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054549 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054549 node");
    l_cur_occ = l_republic->linkTo(l_form_array[274]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054549");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054550 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054550 node");
    l_cur_occ = l_republic->linkTo(l_form_array[275]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054550");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054552 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054552 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054552");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054553 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054553 node");
    l_cur_occ = l_republic->linkTo(l_form_array[276]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054553");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054554 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054554 node");
    l_cur_occ = l_republic->linkTo(l_form_array[273]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054554");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054555 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054555 node");
    l_cur_occ = l_republic->linkTo(l_form_array[275]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054555");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054556 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054556 node");
    l_cur_occ = l_republic->linkTo(l_form_array[216]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054556");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054557 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054557 node");
    l_cur_occ = l_republic->linkTo(l_form_array[276]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054557");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054558 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054558 node");
    l_cur_occ = l_republic->linkTo(l_form_array[216]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054558");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054559 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054559 node");
    l_cur_occ = l_republic->linkTo(l_form_array[273]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054559");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054560 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054560 node");
    l_cur_occ = l_republic->linkTo(l_form_array[275]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054560");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054561 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054561 node");
    l_cur_occ = l_republic->linkTo(l_form_array[277]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054561");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054563 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054563 node");
    l_cur_occ = l_republic->linkTo(l_form_array[278]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054563");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054564 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054564 node");
    l_cur_occ = l_republic->linkTo(l_form_array[230]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054564");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054565 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054565 node");
    l_cur_occ = l_republic->linkTo(l_form_array[229]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054565");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054566 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054566 node");
    l_cur_occ = l_republic->linkTo(l_form_array[279]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054566");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054567 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054567 node");
    l_cur_occ = l_republic->linkTo(l_form_array[280]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054567");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054568 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054568 node");
    l_cur_occ = l_republic->linkTo(l_form_array[281]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054568");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054569 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054569 node");
    l_cur_occ = l_republic->linkTo(l_form_array[232]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054569");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054570 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054570 node");
    l_cur_occ = l_republic->linkTo(l_form_array[233]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054570");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054571 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054571 node");
    l_cur_occ = l_republic->linkTo(l_form_array[234]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054571");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054573 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054573 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054573");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 8> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 8> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 8");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054574 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054574 node");
    l_cur_occ = l_republic->linkTo(l_form_array[209]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054574");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054576 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054576 node");
    l_cur_occ = l_republic->linkTo(l_form_array[282]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054576");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054578 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054578 node");
    l_cur_occ = l_republic->linkTo(l_form_array[231]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054578");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054580 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054580 node");
    l_cur_occ = l_republic->linkTo(l_form_array[283]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054580");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 9> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 9> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 9");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054581 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054581 node");
    l_cur_occ = l_republic->linkTo(l_form_array[284]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054581");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054582 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054582 node");
    l_cur_occ = l_republic->linkTo(l_form_array[213]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054582");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054583 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054583 node");
    l_cur_occ = l_republic->linkTo(l_form_array[224]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054583");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054584 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054584 node");
    l_cur_occ = l_republic->linkTo(l_form_array[285]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054584");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054585 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054585 node");
    l_cur_occ = l_republic->linkTo(l_form_array[286]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054585");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054586 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054586 node");
    l_cur_occ = l_republic->linkTo(l_form_array[287]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054586");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054587 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054587 node");
    l_cur_occ = l_republic->linkTo(l_form_array[288]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054587");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054588 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054588 node");
    l_cur_occ = l_republic->linkTo(l_form_array[289]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054588");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054589 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054589 node");
    l_cur_occ = l_republic->linkTo(l_form_array[290]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054589");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054590 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054590 node");
    l_cur_occ = l_republic->linkTo(l_form_array[291]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054590");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054591 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054591 node");
    l_cur_occ = l_republic->linkTo(l_form_array[253]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054591");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054593 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054593 node");
    l_cur_occ = l_republic->linkTo(l_form_array[292]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054593");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054594 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054594 node");
    l_cur_occ = l_republic->linkTo(l_form_array[217]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054594");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054595 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054595 node");
    l_cur_occ = l_republic->linkTo(l_form_array[293]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054595");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054596 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054596 node");
    l_cur_occ = l_republic->linkTo(l_form_array[286]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054596");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054597 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054597 node");
    l_cur_occ = l_republic->linkTo(l_form_array[294]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054597");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054599 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054599 node");
    l_cur_occ = l_republic->linkTo(l_form_array[295]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054599");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 10> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 10> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 10");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054600 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054600 node");
    l_cur_occ = l_republic->linkTo(l_form_array[296]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054600");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054601 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054601 node");
    l_cur_occ = l_republic->linkTo(l_form_array[297]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054601");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054603 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054603 node");
    l_cur_occ = l_republic->linkTo(l_form_array[265]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054603");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 11> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 11> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 11");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054604 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054604 node");
    l_cur_occ = l_republic->linkTo(l_form_array[211]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054604");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054605 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054605 node");
    l_cur_occ = l_republic->linkTo(l_form_array[298]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054605");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054606 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054606 node");
    l_cur_occ = l_republic->linkTo(l_form_array[299]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054606");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054607 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054607 node");
    l_cur_occ = l_republic->linkTo(l_form_array[300]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054607");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054608 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054608 node");
    l_cur_occ = l_republic->linkTo(l_form_array[281]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054608");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054610 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054610 node");
    l_cur_occ = l_republic->linkTo(l_form_array[301]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054610");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054611 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054611 node");
    l_cur_occ = l_republic->linkTo(l_form_array[302]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054611");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054612 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054612 node");
    l_cur_occ = l_republic->linkTo(l_form_array[303]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054612");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054613 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054613 node");
    l_cur_occ = l_republic->linkTo(l_form_array[252]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054613");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054614 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054614 node");
    l_cur_occ = l_republic->linkTo(l_form_array[240]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054614");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054615 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054615 node");
    l_cur_occ = l_republic->linkTo(l_form_array[304]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054615");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054617 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054617 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054617");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054618 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054618 node");
    l_cur_occ = l_republic->linkTo(l_form_array[305]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054618");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054619 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054619 node");
    l_cur_occ = l_republic->linkTo(l_form_array[286]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054619");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054620 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054620 node");
    l_cur_occ = l_republic->linkTo(l_form_array[306]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054620");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054621 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054621 node");
    l_cur_occ = l_republic->linkTo(l_form_array[307]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054621");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054622 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054622 node");
    l_cur_occ = l_republic->linkTo(l_form_array[308]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054622");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054624 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054624 node");
    l_cur_occ = l_republic->linkTo(l_form_array[309]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054624");
    l_cur_occ->setType("RVGRK");
    l_stephanus_array[2]->linkTo(l_cur_occ, "OWNS_");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 12> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 12> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 12");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054625 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054625 node");
    l_cur_occ = l_republic->linkTo(l_form_array[270]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054625");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054626 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054626 node");
    l_cur_occ = l_republic->linkTo(l_form_array[310]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054626");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054627 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054627 node");
    l_cur_occ = l_republic->linkTo(l_form_array[311]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054627");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054628 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054628 node");
    l_cur_occ = l_republic->linkTo(l_form_array[208]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054628");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054629 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054629 node");
    l_cur_occ = l_republic->linkTo(l_form_array[312]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054629");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054630 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054630 node");
    l_cur_occ = l_republic->linkTo(l_form_array[313]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054630");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054632 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054632 node");
    l_cur_occ = l_republic->linkTo(l_form_array[314]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054632");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054633 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054633 node");
    l_cur_occ = l_republic->linkTo(l_form_array[315]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054633");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054634 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054634 node");
    l_cur_occ = l_republic->linkTo(l_form_array[316]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054634");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054635 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054635 node");
    l_cur_occ = l_republic->linkTo(l_form_array[317]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054635");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054637 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054637 node");
    l_cur_occ = l_republic->linkTo(l_form_array[318]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054637");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 13> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 13> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 13");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054638 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054638 node");
    l_cur_occ = l_republic->linkTo(l_form_array[319]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054638");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054639 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054639 node");
    l_cur_occ = l_republic->linkTo(l_form_array[320]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054639");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054640 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054640 node");
    l_cur_occ = l_republic->linkTo(l_form_array[321]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054640");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054642 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054642 node");
    l_cur_occ = l_republic->linkTo(l_form_array[282]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054642");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054644 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054644 node");
    l_cur_occ = l_republic->linkTo(l_form_array[322]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054644");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 14> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 14> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 14");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054645 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054645 node");
    l_cur_occ = l_republic->linkTo(l_form_array[323]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054645");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054647 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054647 node");
    l_cur_occ = l_republic->linkTo(l_form_array[235]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054647");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054648 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054648 node");
    l_cur_occ = l_republic->linkTo(l_form_array[265]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054648");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054649 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054649 node");
    l_cur_occ = l_republic->linkTo(l_form_array[324]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054649");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054650 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054650 node");
    l_cur_occ = l_republic->linkTo(l_form_array[325]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054650");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054651 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054651 node");
    l_cur_occ = l_republic->linkTo(l_form_array[326]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054651");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054653 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054653 node");
    l_cur_occ = l_republic->linkTo(l_form_array[230]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054653");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 15> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 15> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 15");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054654 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054654 node");
    l_cur_occ = l_republic->linkTo(l_form_array[327]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054654");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054655 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054655 node");
    l_cur_occ = l_republic->linkTo(l_form_array[328]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054655");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054656 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054656 node");
    l_cur_occ = l_republic->linkTo(l_form_array[329]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054656");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054657 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054657 node");
    l_cur_occ = l_republic->linkTo(l_form_array[330]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054657");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054658 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054658 node");
    l_cur_occ = l_republic->linkTo(l_form_array[331]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054658");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054660 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054660 node");
    l_cur_occ = l_republic->linkTo(l_form_array[332]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054660");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054661 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054661 node");
    l_cur_occ = l_republic->linkTo(l_form_array[286]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054661");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054662 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054662 node");
    l_cur_occ = l_republic->linkTo(l_form_array[333]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054662");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054663 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054663 node");
    l_cur_occ = l_republic->linkTo(l_form_array[270]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054663");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054664 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054664 node");
    l_cur_occ = l_republic->linkTo(l_form_array[334]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054664");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054665 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054665 node");
    l_cur_occ = l_republic->linkTo(l_form_array[335]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054665");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054666 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054666 node");
    l_cur_occ = l_republic->linkTo(l_form_array[336]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054666");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054667 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054667 node");
    l_cur_occ = l_republic->linkTo(l_form_array[337]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054667");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054669 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054669 node");
    l_cur_occ = l_republic->linkTo(l_form_array[252]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054669");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054670 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054670 node");
    l_cur_occ = l_republic->linkTo(l_form_array[338]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054670");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054671 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054671 node");
    l_cur_occ = l_republic->linkTo(l_form_array[339]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054671");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054672 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054672 node");
    l_cur_occ = l_republic->linkTo(l_form_array[340]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054672");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054673 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054673 node");
    l_cur_occ = l_republic->linkTo(l_form_array[341]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054673");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054674 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054674 node");
    l_cur_occ = l_republic->linkTo(l_form_array[270]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054674");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054676 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054676 node");
    l_cur_occ = l_republic->linkTo(l_form_array[252]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054676");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 16> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 16> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 16");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054677 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054677 node");
    l_cur_occ = l_republic->linkTo(l_form_array[299]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054677");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054678 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054678 node");
    l_cur_occ = l_republic->linkTo(l_form_array[342]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054678");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054679 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054679 node");
    l_cur_occ = l_republic->linkTo(l_form_array[241]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054679");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054681 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054681 node");
    l_cur_occ = l_republic->linkTo(l_form_array[210]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054681");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 17> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 17> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 17");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054682 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054682 node");
    l_cur_occ = l_republic->linkTo(l_form_array[343]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054682");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054683 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054683 node");
    l_cur_occ = l_republic->linkTo(l_form_array[344]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054683");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054684 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054684 node");
    l_cur_occ = l_republic->linkTo(l_form_array[345]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054684");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054685 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054685 node");
    l_cur_occ = l_republic->linkTo(l_form_array[230]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054685");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054686 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054686 node");
    l_cur_occ = l_republic->linkTo(l_form_array[346]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054686");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054688 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054688 node");
    l_cur_occ = l_republic->linkTo(l_form_array[347]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054688");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 18> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 18> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 18");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054689 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054689 node");
    l_cur_occ = l_republic->linkTo(l_form_array[297]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054689");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054690 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054690 node");
    l_cur_occ = l_republic->linkTo(l_form_array[348]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054690");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054691 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054691 node");
    l_cur_occ = l_republic->linkTo(l_form_array[349]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054691");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054692 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054692 node");
    l_cur_occ = l_republic->linkTo(l_form_array[350]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054692");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054693 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054693 node");
    l_cur_occ = l_republic->linkTo(l_form_array[351]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054693");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054695 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054695 node");
    l_cur_occ = l_republic->linkTo(l_form_array[352]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054695");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 19> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 19> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 19");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054697 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054697 node");
    l_cur_occ = l_republic->linkTo(l_form_array[282]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054697");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(",", false, M1Env::PCTRT_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054699 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054699 node");
    l_cur_occ = l_republic->linkTo(l_form_array[315]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054699");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054700 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054700 node");
    l_cur_occ = l_republic->linkTo(l_form_array[344]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054700");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054701 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054701 node");
    l_cur_occ = l_republic->linkTo(l_form_array[265]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054701");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(".", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054703 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054703 node");
    l_cur_occ = l_republic->linkTo(l_form_array[353]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054703");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField("SS", false, M1Env::STPOS_SIID);
    // creation of Republic sentence <Greek - Sentence 20> node
    qCDebug(g_cat_silence) << QString("Creating Republic sentence <Greek - Sentence 20> node");
    l_cur_sentence = M1Store::Item_lv2::getNew(
        // vertex flags
        M1Env::FULL_VERTEX,
        // label
        "Greek - Sentence 20");
    l_cur_sentence->setType("TXSNT");
    l_cur_sentence->setType("RVGRK");
    l_cur_book_sentence = l_book[4]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);
    l_cur_sentence_occ = nullptr;
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054704 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054704 node");
    l_cur_occ = l_republic->linkTo(l_form_array[354]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054704");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054705 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054705 node");
    l_cur_occ = l_republic->linkTo(l_form_array[230]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054705");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054706 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054706 node");
    l_cur_occ = l_republic->linkTo(l_form_array[306]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054706");
    l_cur_occ->setType("RVGRK");
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);

    // creation of Republic occurrence Z-GRK-054707 node
    qCDebug(g_cat_silence) << QString("Creating Republic occurrence Z-GRK-054707 node");
    l_cur_occ = l_republic->linkTo(l_form_array[355]->item_id(), "OCCUR");
    l_cur_occ->setText("Z-GRK-054707");
    l_cur_occ->setType("RVGRK");
    l_cur_occ->setField(";", false, M1Env::PCTRT_SIID);
    l_cur_occ->setField("SE", false, M1Env::STPOS_SIID);
    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);
}
