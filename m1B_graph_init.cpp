
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

void M1Env::GraphInit::set_pseudo_constants(){
    M1Env::FOLDER_SIID = M1Store::Storage::getSpecialID("FOLDR");
    M1Env::AUTO_SIID = M1Store::Storage::getSpecialID("AUTO_");
    M1Env::OWNS_SIID = M1Store::Storage::getSpecialID("OWNS_");
    M1Env::BLNGS_SIID = M1Store::Storage::getSpecialID("BLNGS");
    M1Env::ISA_SIID = M1Store::Storage::getSpecialID("_ISA_");
    M1Env::ITO_SIID = M1Store::Storage::getSpecialID("_ITO_");
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

    // creation of "Global graph root"
    qCDebug(g_cat_store) << QString("Creating <[ROOT_]-Global graph root> item");
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
    qCDebug(g_cat_store) << QString("Creating <[HOME_]-Home> item");
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
    qCDebug(g_cat_store) << QString("Creating <[TYPE_]-Root of all types> item");
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
    qCDebug(g_cat_store) << QString("Creating <[PERSN]-Person (type)> item");
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
    qCDebug(g_cat_store) << QString("Creating <[ORGN_]-Organization (type)> item");
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
    qCDebug(g_cat_store) << QString("Creating <[ME___]-Me> item");
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
    qCDebug(g_cat_store) << QString("Creating <[_MSG_]-Inboxes / Message Root Type> item");
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
    qCDebug(g_cat_store) << QString("Creating <[EMAIL]-Email Inbox> item");
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
    qCDebug(g_cat_store) << QString("Creating <[WHTSP]-Whatsapp Inbox> item");
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
    qCDebug(g_cat_store) << QString("Creating <[DSCRD]-Discord Inbox> item");
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
    qCDebug(g_cat_store) << QString("Creating <[GCHAT]-GChat Inbox> item");
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
    qCDebug(g_cat_store) << QString("Creating <[SMS__]-SMS Inbox> item");
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
}

void M1Env::GraphInit::init_text(){
    
}
