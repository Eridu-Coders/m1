#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

g_special_vertices = [
    ['FOLDR', 'SI_IS_TYPE',                      'FOLDER_ICON_PATH', 'FOLDER_SIID', None, 'Special Vertex ID (Vertex type) of folders (ordinary vertices with no special role)'],
    ['AUTO_', 'SI_IS_TYPE',                      'AUTO_ICON_PATH', 'AUTO_SIID', None, 'Special Vertex ID (Edge type) of AUTO edges'],
    # --------------------------- OWNS / BLNGS ---------------------------
    ['OWNS_', 'SI_IS_TYPE',                      'OWNS_ICON_PATH', 'OWNS_SIID',
    ('BLNGS', 'SI_IS_TYPE | SI_INSERT_AT_TOP',   'BLNGS_ICON_PATH', 'BLNGS_SIID'), 'Special Vertex ID (Edge type) of ownership edges'],
    # --------------------------- ISA / ITO ---------------------------
    ['_ISA_', 'SI_IS_TYPE | SI_IS_SPECIAL_EDGE', 'ISA_ICON_PATH', 'ISA_SIID',
    ('_ITO_', 'SI_IS_TYPE'                     , 'ITO_ICON_PATH', 'ITO_SIID'), 'Special Vertex ID (Edge type) of type edges']
]

g_vertices = [ #
#     ['ROOT_', ['FULL_VERTEX', 'IS_SPECIAL'], ['FOLDR'], None,                     'ROOT_SIID',         'Global graph root', None],
#     ['HOME_', ['FULL_VERTEX', 'IS_SPECIAL'], ['FOLDR'], None,                     'HOME_SIID',         'Home', [('BLNGS', 'ROOT_')]],
#     ['TYPE_', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], 'TYPE_NODE_ICON_PATH',    'TYPE_NODE_SIID',    'Root of all types', [('BLNGS', 'ROOT_')]],
#     ['PERSN', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], 'PERSON_ICON_PATH',       'PERS_TYPE_SIID',    'Person (type)', None],
#     ['ORGN_', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], 'ORGANIZATION_ICON_PATH', 'ORG_TYPE_SIID',     'Organization (type)', None],
#     ['ME___', ['FULL_VERTEX', 'IS_SPECIAL'], ['PERSN'], None,                     'ME_SIID',           'Me', [('BLNGS', 'HOME')]],
#     ['_MSG_', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], None,                     'MSG_TYPE_SIID',     'Inboxes / Message Root Type', None],
#     ['EMAIL', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], None,                     'EMAIL_TYPE_SIID',   'Email Inbox', [('BLNGS', '_MSG_')]],
#     ['WHTSP', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], None,                     'WHTSP_TYPE_SIID',   'Whatsapp Inbox', [('BLNGS', '_MSG_')]],
#     ['DSCRD', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], None,                     'DSCRD_TYPE_SIID',   'Discord Inbox', [('BLNGS', '_MSG_')]],
#     ['GCHAT', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], None,                     'GCHAT_TYPE_SIID',   'GChat Inbox', [('BLNGS', '_MSG_')]],
#     ['SMS__', ['FULL_VERTEX', 'IS_SPECIAL'], ['TYPE_'], None,                     'SMS_TYPE_SIID',     'SMS Inbox', [('BLNGS', '_MSG_')]],
# l_label,                       l_vertex_flag_list,              l_type_list, l_edges               l_mnemo, l_si_flag_list,                      l_icon_path,              l_pseudo_constant
['Global graph root',            ['FULL_VERTEX',  'IS_SPECIAL'],  ['FOLDR'],  None,                  'ROOT_', '0',                                 None,                     'ROOT_SIID'],
['Home',                         ['FULL_VERTEX',  'IS_SPECIAL'],  ['FOLDR'],  [('BLNGS',  'ROOT_')], 'HOME_', '0',                                 None,                     'HOME_SIID'],
['Root of all types',            ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'ROOT_')], 'TYPE_', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'TYPE_NODE_ICON_PATH',    'TYPE_NODE_SIID'],
['Person (type)',                ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'PERSN', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'PERSON_ICON_PATH',       'PERS_TYPE_SIID'],
['Organization (type)',          ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'ORGN_', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'ORGANIZATION_ICON_PATH', 'ORG_TYPE_SIID'],
['Me',                           ['FULL_VERTEX',  'IS_SPECIAL'],  ['PERSN'],  [('BLNGS',  'HOME_')], 'ME___', '0',                                 None,                     'ME_SIID'],
['Inboxes / Message Root Type',  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  '_MSG_', ['SI_IS_TYPE'],                      None,                     'MSG_TYPE_SIID'],
['Email Inbox',                  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'EMAIL', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'EMAIL_TYPE_SIID'],
['Whatsapp Inbox',               ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'WHTSP', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'WHTSP_TYPE_SIID'],
['Discord Inbox',                ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'DSCRD', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'DSCRD_TYPE_SIID'],
['GChat Inbox',                  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'GCHAT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'GCHAT_TYPE_SIID'],
['SMS Inbox',                    ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'SMS__', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'SMS_TYPE_SIID']
]

# downloaded from https://universaldependencies.org/u/feat/index.html
g_attr_values = [
    ('PronType', 'pronominal type', [('Art', 'article'), ('Dem', 'demonstrative pronoun, determiner, numeral or adverb'), ('Emp', 'emphatic determiner'), ('Exc', 'exclamative determiner'), ('Ind', 'indefinite pronoun, determiner, numeral or adverb'), ('Int', 'interrogative pronoun, determiner, numeral or adverb'), ('Neg', 'negative pronoun, determiner or adverb'), ('Prs', 'personal or possessive personal pronoun or determiner'), ('Rcp', 'reciprocal pronoun'), ('Rel', 'relative pronoun, determiner, numeral or adverb'), ('Tot', 'total (collective) pronoun, determiner or adverb')]),
    ('Gender', 'gender', [('Com', 'common gender'), ('Fem', 'feminine gender'), ('Masc', 'masculine gender'), ('Neut', 'neuter gender')]),
    ('VerbForm', 'form of verb or deverbative', [('Conv', 'converb, transgressive, adverbial participle, verbal adverb'), ('Fin', 'finite verb'), ('Gdv', 'gerundive'), ('Ger', 'gerund'), ('Inf', 'infinitive'), ('Part', 'participle, verbal adjective'), ('Sup', 'supine'), ('Vnoun', 'verbal noun, masdar')]),
    ('NumType', 'numeral type', [('Card', 'cardinal number or corresponding interrogative / relative / indefinite / demonstrative word'), ('Dist', 'distributive numeral'), ('Frac', 'fraction'), ('Mult', 'multiplicative numeral or corresponding interrogative / relative / indefinite / demonstrative word'), ('Ord', 'ordinal number or corresponding interrogative / relative / indefinite / demonstrative word'), ('Range', 'range of values'), ('Sets', 'number of sets of things; collective numeral')]),
    ('Animacy', 'animacy', [('Anim', 'animate'), ('Hum', 'human'), ('Inan', 'inanimate'), ('Nhum', 'non-human')]),
    ('Mood', 'mood', [('Adm', 'admirative'), ('Cnd', 'conditional'), ('Des', 'desiderative'), ('Imp', 'imperative'), ('Ind', 'indicative or realis'), ('Int', 'interrogative'), ('Irr', 'irrealis'), ('Jus', 'jussive / injunctive'), ('Nec', 'necessitative'), ('Opt', 'optative'), ('Pot', 'potential'), ('Prp', 'purposive'), ('Qot', 'quotative'), ('Sub', 'subjunctive / conjunctive')]),
    ('Poss', 'possessive', [('Yes', 'it is possessive')]),
    ('NounClass', 'noun class', [('Bantu1', 'singular, persons'), ('Bantu2', 'plural, persons'), ('Bantu3', 'singular, plants, thin objects'), ('Bantu4', 'plural, plants, thin objects'), ('Bantu5', 'singular, fruits, round objects, paired things'), ('Bantu6', 'plural, fruits, round objects, paired things'), ('Bantu7', 'singular, things, diminutives'), ('Bantu8', 'plural, things, diminutives'), ('Bantu9', 'singular, animals, things'), ('Bantu10', 'plural, animals, things'), ('Bantu11', 'long thin objects, natural phenomena, abstracts'), ('Bantu12', 'singular, small things, diminutives'), ('Bantu13', 'plural or mass, small amount of mass'), ('Bantu14', 'plural, diminutives'), ('Bantu15', 'verbal nouns, infinitives'), ('Bantu16', 'definite location, close to something'), ('Bantu17', 'indefinite location, direction, movement'), ('Bantu18', 'definite location, inside something'), ('Bantu19', 'little bit of, pejorative plural'), ('Bantu20', 'singular, augmentatives'), ('Bantu21', 'singular, augmentatives, derogatives'), ('Bantu22', 'plural, augmentatives'), ('Bantu23', 'location with place names'), ('Wol1', 'Wolof noun class 1/k (singular human)'), ('Wol2', 'Wolof noun class 2/ñ (plural human)'), ('Wol3', 'Wolof noun class 3/g (singular)'), ('Wol4', 'Wolof noun class 4/j (singular)'), ('Wol5', 'Wolof noun class 5/b (singular)'), ('Wol6', 'Wolof noun class 6/m (singular)'), ('Wol7', 'Wolof noun class 7/l (singular)'), ('Wol8', 'Wolof noun class 8/y (plural non-human)'), ('Wol9', 'Wolof noun class 9/s (singular)'), ('Wol10', 'Wolof noun class 10/w (singular)'), ('Wol11', 'Wolof noun class 11/f (location)'), ('Wol12', 'Wolof noun class 12/n (manner)')]),
    ('Tense', 'tense', [('Fut', 'future tense'), ('Imp', 'imperfect'), ('Past', 'past tense / preterite / aorist'), ('Pqp', 'pluperfect'), ('Pres', 'present / non-past tense / aorist')]),
    ('Reflex', 'reflexive', [('Yes', 'it is reflexive')]),
    ('Number', 'number', [('Coll', 'collective / mass / singulare tantum'), ('Count', 'count plural'), ('Dual', 'dual number'), ('Grpa', 'greater paucal number'), ('Grpl', 'greater plural number'), ('Inv', 'inverse number'), ('Pauc', 'paucal number'), ('Plur', 'plural number'), ('Ptan', 'plurale tantum'), ('Sing', 'singular number'), ('Tri', 'trial number')]),
    ('Aspect', 'aspect', [('Hab', 'habitual aspect'), ('Imp', 'imperfect aspect'), ('Iter', 'iterative / frequentative aspect'), ('Perf', 'perfect aspect'), ('Prog', 'progressive aspect'), ('Prosp', 'prospective aspect')]),
    ('Case', 'case', [('Abe', 'abessive / caritive / privative'), ('Ben', 'benefactive / destinative'), ('Cau', 'causative / motivative / purposive'), ('Cmp', 'comparative'), ('Cns', 'considerative'), ('Com', 'comitative / associative'), ('Dat', 'dative'), ('Dis', 'distributive'), ('Equ', 'equative'), ('Gen', 'genitive'), ('Ins', 'instrumental / instructive'), ('Par', 'partitive'), ('Tem', 'temporal'), ('Abl', 'ablative / adelative'), ('Add', 'additive'), ('Ade', 'adessive'), ('All', 'allative / adlative'), ('Del', 'delative / superelative'), ('Ela', 'elative / inelative'), ('Ess', 'essive / prolative'), ('Ill', 'illative / inlative'), ('Ine', 'inessive'), ('Lat', 'lative / directional allative'), ('Loc', 'locative'), ('Per', 'perlative'), ('Sbe', 'subelative'), ('Sbl', 'sublative'), ('Spl', 'superlative'), ('Sub', 'subessive'), ('Sup', 'superessive'), ('Ter', 'terminative / terminal allative')]),
    ('Voice', 'voice', [('Act', 'active or actor-focus voice'), ('Antip', 'antipassive voice'), ('Bfoc', 'beneficiary-focus voice'), ('Cau', 'causative voice'), ('Dir', 'direct voice'), ('Inv', 'inverse voice'), ('Lfoc', 'location-focus voice'), ('Mid', 'middle voice'), ('Pass', 'passive or patient-focus voice'), ('Rcp', 'reciprocal voice')]),
    ('Abbr', 'abbreviation', [('Yes', 'it is abbreviation')]),
    ('Definite', 'definiteness or state', [('Com', 'complex'), ('Cons', 'construct state / reduced definiteness'), ('Def', 'definite'), ('Ind', 'indefinite'), ('Spec', 'specific indefinite')]),
    ('Evident', 'evidentiality', [('Fh', 'firsthand'), ('Nfh', 'non-firsthand')]),
    ('Typo', 'is this a misspelled word?', [('Yes', 'it is typo')]),
    ('Deixis', 'relative location encoded in demonstratives', [('Abv', 'above the reference point'), ('Bel', 'below the reference point'), ('Even', 'at the same level as the reference point'), ('Med', 'medial'), ('Nvis', 'not visible'), ('Prox', 'proximate'), ('Remt', 'remote, distal')]),
    ('Polarity', 'polarity', [('Neg', 'negative'), ('Pos', 'positive, affirmative')]),
    ('Foreign', 'is this a foreign word?', [('Yes', 'it is foreign')]),
    ('DeixisRef', 'person to which deixis is relative', [('1', 'deixis relative to the first person participant (speaker)'), ('2', 'deixis relative to the second person participant (hearer)')]),
    ('Person', 'person', [('0', 'zero person'), ('1', 'first person'), ('2', 'second person'), ('3', 'third person'), ('4', 'fourth person')]),
    ('ExtPos', 'external part of speech', [('ADJ', 'adjective-like expression'), ('ADP', 'adposition-like expression'), ('ADV', 'adverb-like expression'), ('AUX', 'auxiliary-like expression'), ('CCONJ', 'coordinating conjunction-like expression'), ('DET', 'determiner-like expression'), ('INTJ', 'interjection-like expression'), ('PRON', 'pronoun-like expression'), ('PROPN', 'proper noun-like expression'), ('SCONJ', 'subordinator-like expression')]),
    ('Degree', 'degree', [('Abs', 'absolute superlative'), ('Aug', 'augmentative'), ('Cmp', 'comparative, second degree'), ('Dim', 'diminutive'), ('Equ', 'equative'), ('Pos', 'positive, first degree'), ('Sup', 'superlative, third degree')]),
    ('Polite', 'politeness', [('Elev', 'referent elevating'), ('Form', 'formal register'), ('Humb', 'speaker humbling'), ('Infm', 'informal register')]),
    ('Clusivity', 'clusivity', [('Ex', 'exclusive'), ('In', 'inclusive')])
]

g_declaration_template = """
#include "m1A_constants.h"

namespace M1Env{

__EXTERN_DECL__

    class GraphInit{
        public:
            static void set_pseudo_constants();
            static void init_base();
            static void init_text();
    };
    
} // end namespace M1Env
"""

g_implementation_template = """
#include "m1B_graph_init.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"

__PSEUDO_DECLARE__

void M1Env::GraphInit::set_pseudo_constants(){
    __PSEUDO_INIT__
}

void M1Env::GraphInit::init_base(){
    __INIT_BASE__
}

void M1Env::GraphInit::init_text(){
    __INIT_TEXT__
}
"""
# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    # Ancillaries
    l_initialization = ''
    l_declaration = ''
    l_extern_declaration = ''
    for l_mnemo, _, _, l_pseudo_constant, l_reciprocal, l_comment in g_special_vertices:
        l_extern_declaration += f'// [{l_mnemo}] {l_comment}\nextern M1Env::SpecialItemID {l_pseudo_constant};\n'
        l_declaration += f'// [{l_mnemo}] {l_comment}\nM1Env::SpecialItemID M1Env::{l_pseudo_constant} = G_NONEX_SI_ID;\n'
        l_initialization += f'    M1Env::{l_pseudo_constant} = M1Store::Storage::getSpecialID("{l_mnemo}");\n'
        if l_reciprocal:
            l_mnemo_r, l_flags_r, l_icon_path_r, l_pseudo_constant_r = l_reciprocal
            l_extern_declaration += (f'// [{l_mnemo_r}] {l_comment} (reciprocal of {l_mnemo} above))\n' +
                                     f'extern M1Env::SpecialItemID {l_pseudo_constant_r};\n')
            l_declaration += (f'// [{l_mnemo_r}] {l_comment} (reciprocal of {l_mnemo} above))\n' +
                              f'M1Env::SpecialItemID M1Env::{l_pseudo_constant_r} = G_NONEX_SI_ID;\n')
            l_initialization += f'    M1Env::{l_pseudo_constant_r} = M1Store::Storage::getSpecialID("{l_mnemo_r}");\n'

    l_extern_declaration += '\n'
    l_declaration += '\n'
    for l_label, _, _, _, l_mnemo, _, _, l_pseudo_constant in g_vertices:
        l_extern_declaration += f'// [{l_mnemo}] Special Vertex ID of "{l_label}"\nextern M1Env::SpecialItemID {l_pseudo_constant};\n'
        l_declaration += f'M1Env::SpecialItemID M1Env::{l_pseudo_constant} = G_NONEX_SI_ID;\n'
        l_initialization += f'    M1Env::{l_pseudo_constant} = M1Store::Storage::getSpecialID("{l_mnemo}");\n'

    print(l_extern_declaration)
    print(l_declaration)
    print(l_initialization)

    with open('m1B_graph_init.h', 'w') as l_fout:
        l_fout.write(g_declaration_template.replace('__EXTERN_DECL__', l_extern_declaration))

    # main code
    l_base_code = ''
    l_text_code = ''
    for l_mnemo, l_flags, l_icon_path, _, l_reciprocal, l_comment in g_special_vertices:
        if l_reciprocal:
            l_mnemo_r, l_flags_r, l_icon_path_r, l_pseudo_constant_r = l_reciprocal
            l_base_code += (f'    // {l_comment}\n' +
                            f'    M1Store::Storage::getNewSpecialWithReciprocal({l_flags},\n' +
                            f'        "{l_mnemo}", M1Env::{l_icon_path},\n' +
                            f'        "{l_mnemo_r}", M1Env::{l_icon_path_r},\n' +
                            f'        {l_flags_r});\n')
        else:
            l_base_code += f'    // {l_comment}\n    M1Store::Storage::getNewSpecialNoItem({l_flags}, "{l_mnemo}", M1Env::{l_icon_path});\n'

    l_base_code += '\n'
    l_memo_2_var = dict()
    for l_label, l_vertex_flag_list, l_type_list, l_edges, l_mnemo, l_si_flag_list, l_icon_path, l_pseudo_constant in g_vertices:
        l_var_name = 'l_' + l_mnemo.replace('_', '').lower()
        l_vertex_flag_string = ' | '.join([f'M1Env::{l_flag}' for l_flag in l_vertex_flag_list])
        l_debug_label = f'[{l_mnemo}]-{l_label}' if l_mnemo else f'{l_label}'
        l_base_code += (f'    // creation of "{l_label}"\n' +
                        f'    qCDebug(g_cat_store) << QString("Creating <{l_debug_label}> item");\n' +
                        f'    M1Store::Item_lv2* {l_var_name} = M1Store::Item_lv2::getNew(\n' +
                        f'        // vertex flags\n' +
                        f'        {l_vertex_flag_string},\n' +
                        f'        // label\n' +
                        f'        "{l_label}"')
        if not l_mnemo:
            l_base_code += ');\n'
        else:
            l_base_code += ',\n'
            l_memo_2_var[l_mnemo] = l_var_name
            l_si_flag_string = '0' if l_si_flag_list == '0' else ' | '.join([f'M1Env::{l_flag}' for l_flag in l_si_flag_list])
            l_icon_path = f'M1Env::{l_icon_path}' if l_icon_path else 'nullptr'
            l_base_code += (
                    f'        // Special Item flag\n' +
                    f'        {l_si_flag_string},\n' +
                    f'        // mnemonic\n' +
                    f'        "{l_mnemo}",\n' +
                    f'        // icon path\n' +
                    f'        {l_icon_path});\n')

        for l_type_mnemo in l_type_list:
            l_base_code += f'    {l_var_name}->setType("{l_type_mnemo}");\n'
        if l_edges:
            for l_edge_type, l_dest_mnemo in l_edges:
                l_base_code += f'    {l_var_name}->linkTo({l_memo_2_var[l_dest_mnemo]}, "{l_edge_type}", nullptr, true);\n'
        l_base_code += '\n'

    print(l_base_code)
    with open('m1B_graph_init.cpp', 'w') as l_fout:
        l_fout.write(g_implementation_template
                     .replace('__PSEUDO_DECLARE__', l_declaration.strip())
                     .replace('__PSEUDO_INIT__', l_initialization.strip())
                     .replace('__INIT_BASE__', l_base_code.strip())
                     .replace('__INIT_TEXT__', l_text_code.strip()))
