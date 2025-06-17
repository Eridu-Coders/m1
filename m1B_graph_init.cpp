
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
M1Env::SpecialItemID M1Env::STPSC_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TXTVR_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TXTCK_SIID = G_NONEX_SI_ID;
M1Env::SpecialItemID M1Env::TXTNT_SIID = G_NONEX_SI_ID;
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
    M1Env::OCCUR_SIID = M1Store::Storage::getSpecialID("OCCUR");
    M1Env::FORM_SIID = M1Store::Storage::getSpecialID("FORM_");
    M1Env::CAPTL_SIID = M1Store::Storage::getSpecialID("CAPTL");
    M1Env::PCTLF_SIID = M1Store::Storage::getSpecialID("PCTLF");
    M1Env::PCTRT_SIID = M1Store::Storage::getSpecialID("PCTRT");
    M1Env::MKPLF_SIID = M1Store::Storage::getSpecialID("MKPLF");
    M1Env::MKPRT_SIID = M1Store::Storage::getSpecialID("MKPRT");
    M1Env::STPOS_SIID = M1Store::Storage::getSpecialID("STPOS");
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
    M1Env::STPSC_SIID = M1Store::Storage::getSpecialID("STPSC");
    M1Env::TXTVR_SIID = M1Store::Storage::getSpecialID("TXTVR");
    M1Env::TXTCK_SIID = M1Store::Storage::getSpecialID("TXTCK");
    M1Env::TXTNT_SIID = M1Store::Storage::getSpecialID("TXTNT");
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
    // Special Vertex ID (Edge type) of occurrence edges
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "OCCUR", M1Env::OCCURRENCE_ICON_PATH);
    // Special Vertex ID (Vertex type) of word form vertices
    M1Store::Storage::getNewSpecialNoItem(SI_IS_TYPE, "FORM_", M1Env::FORM_ICON_PATH);
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
        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,
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
        M1Env::FOLDER_ICON_PATH);
    l_txtck->setType("TYPE_");
    l_txtck->linkTo(l_home, "BLNGS", nullptr, true);

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

void M1Env::GraphInit::init_text(){
    
}
