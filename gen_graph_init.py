#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import re
import json

# downloaded from https://universaldependencies.org/u/feat/index.html and formated with dnl_morph_attr.py
g_attr_values = [
    ('Abbr', 'abbreviation', [
        ('Yes', 'ABYES', 'it is abbreviation'),
    ]),
    ('Animacy', 'animacy', [
        ('Anim', 'ANANM', 'animate'),
        ('Hum', 'ANHUM', 'human'),
        ('Inan', 'ANINN', 'inanimate'),
        ('Nhum', 'ANNHM', 'non-human'),
    ]),
    ('Aspect', 'aspect', [
        ('Hab', 'ASHAB', 'habitual aspect'),
        ('Imp', 'ASIMP', 'imperfect aspect'),
        ('Iter', 'ASITR', 'iterative / frequentative aspect'),
        ('Perf', 'ASPRF', 'perfect aspect'),
        ('Prog', 'ASPRG', 'progressive aspect'),
        ('Prosp', 'ASPRS', 'prospective aspect'),
    ]),
    ('Case', 'case', [
        ('Abs', 'CSABS', 'absolutive'),
        ('Acc', 'CSACC', 'accusative / oblique'),
        ('Erf', 'CSERG', 'ergative'),
        ('Nom', 'CSNOM', 'nominative / direct'),
        ('Abe', 'CSABE', 'abessive / caritive / privative'),
        ('Ben', 'CSBEN', 'benefactive / destinative'),
        ('Cau', 'CSCAU', 'causative / motivative / purposive'),
        ('Cmp', 'CSCMP', 'comparative'),
        ('Cns', 'CSCNS', 'considerative'),
        ('Com', 'CSCOM', 'comitative / associative'),
        ('Dat', 'CSDAT', 'dative'),
        ('Dis', 'CSDIS', 'distributive'),
        ('Equ', 'CSEQU', 'equative'),
        ('Gen', 'CSGEN', 'genitive'),
        ('Ins', 'CSINS', 'instrumental / instructive'),
        ('Par', 'CSPAR', 'partitive'),
        ('Tem', 'CSTEM', 'temporal'),
        ('Tra', 'CSTRA', 'translative / factive'),
        ('Voc', 'CSVOC', 'vocative'),
        ('Abl', 'CSABL', 'ablative / adelative'),
        ('Add', 'CSADD', 'additive'),
        ('Ade', 'CSADE', 'adessive'),
        ('All', 'CSALL', 'allative / adlative'),
        ('Del', 'CSDEL', 'delative / superelative'),
        ('Ela', 'CSELA', 'elative / inelative'),
        ('Ess', 'CSESS', 'essive / prolative'),
        ('Ill', 'CSILL', 'illative / inlative'),
        ('Ine', 'CSINE', 'inessive'),
        ('Lat', 'CSLAT', 'lative / directional allative'),
        ('Loc', 'CSLOC', 'locative'),
        ('Per', 'CSPER', 'perlative'),
        ('Sbe', 'CSSBE', 'subelative'),
        ('Sbl', 'CSSBL', 'sublative'),
        ('Spl', 'CSSPL', 'superlative'),
        ('Sub', 'CSSUB', 'subessive'),
        ('Sup', 'CSSUP', 'superessive'),
        ('Ter', 'CSTER', 'terminative / terminal allative'),
    ]),
    ('ConjType', 'conjunction type', [
        ('Cmp', 'CTCMP', 'comparing conjunction'),
        ('Oper', 'CTOPR', 'mathematical operator'),
        ('Pred', 'CTPRD', 'subordinating conjunction introducing a secondary predicate'),
    ]),
    ('Clusivity', 'clusivity', [
        ('Ex', 'CLSEX', 'exclusive'),
        ('In', 'CLSIN', 'inclusive'),
    ]),
    ('Definite', 'definiteness or state', [
        ('Com', 'DFCOM', 'complex'),
        ('Cons', 'DFCNS', 'construct state / reduced definiteness'),
        ('Def', 'DFDEF', 'definite'),
        ('Ind', 'DFIND', 'indefinite'),
        ('Spec', 'DFSPC', 'specific indefinite'),
    ]),
    ('Degree', 'degree', [
        ('Abs', 'DGABS', 'absolute superlative'),
        ('Auf', 'DGAUG', 'augmentative'),
        ('Cmp', 'DGCMP', 'comparative, second degree'),
        ('Dim', 'DGDIM', 'diminutive'),
        ('Equ', 'DGEQU', 'equative'),
        ('Pos', 'DGPOS', 'positive, first degree'),
        ('Sup', 'DGSUP', 'superlative, third degree'),
    ]),
    ('Deixis', 'relative location encoded in demonstratives', [
        ('Abv', 'DXABV', 'above the reference point'),
        ('Bel', 'DXBEL', 'below the reference point'),
        ('Even', 'DXEVN', 'at the same level as the reference point'),
        ('Med', 'DXMED', 'medial'),
        ('Nvis', 'DXNVS', 'not visible'),
        ('Prox', 'DXPRX', 'proximate'),
        ('Remt', 'DXRMT', 'remote, distal'),
    ]),
    ('DeixisRef', 'person to which deixis is relative', [
        ('1', 'DRXS1', 'deixis relative to the first person participant (speaker)'),
        ('2', 'DRXS2', 'deixis relative to the second person participant (hearer)'),
    ]),
    ('Evident', 'evidentiality', [
        ('Fh', 'EVDFH', 'firsthand'),
        ('Nfh', 'EVNFH', 'non-firsthand'),
    ]),
    ('ExtPos', 'external part of speech', [
        ('ADJ', 'EPADJ', 'adjective-like expression'),
        ('ADP', 'EPADP', 'adposition-like expression'),
        ('ADV', 'EPADV', 'adverb-like expression'),
        ('AUX', 'EPAUX', 'auxiliary-like expression'),
        ('CCONJ', 'CCONJ', 'coordinating conjunction-like expression'),
        ('DET', 'EPDET', 'determiner-like expression'),
        ('INTJ', 'EINTJ', 'interjection-like expression'),
        ('PRON', 'EPRON', 'pronoun-like expression'),
        ('PROPN', 'PROPN', 'proper noun-like expression'),
        ('SCONJ', 'SCONJ', 'subordinator-like expression'),
    ]),
    ('Foreign', 'is this a foreign word?', [
        ('Yes', 'FRYES', 'it is foreign'),
    ]),
    ('Gender', 'gender', [
        ('Com', 'GNCOM', 'common gender'),
        ('Fem', 'GNFEM', 'feminine gender'),
        ('Masc', 'GMASC', 'masculine gender'),
        ('Neut', 'GNEUT', 'neuter gender'),
    ]),
    ('Mood', 'mood', [
        ('Ben', 'MDBEN', 'benedictive / precative'),
        ('Adm', 'MDADM', 'admirative'),
        ('Cnd', 'MDCND', 'conditional'),
        ('Des', 'MDDES', 'desiderative'),
        ('Imp', 'MDIMP', 'imperative'),
        ('Its', 'MDITS', 'intensive'),
        ('Ind', 'MDIND', 'indicative or realis'),
        ('Int', 'MDINT', 'interrogative'),
        ('Irr', 'MDIRR', 'irrealis'),
        ('Jus', 'MDJUS', 'jussive / injunctive'),
        ('Nec', 'MDNEC', 'necessitative'),
        ('Opt', 'MDOPT', 'optative'),
        ('Pot', 'MDPOT', 'potential'),
        ('Prp', 'MDPRP', 'purposive'),
        ('Qot', 'MDQOT', 'quotative'),
        ('Sub', 'MDSUB', 'subjunctive / conjunctive'),
    ]),
    ('NounClass', 'noun class', [
        ('Bantu1', 'NCBN1', 'singular, persons'),
        ('Bantu2', 'NCBN2', 'plural, persons'),
        ('Bantu3', 'NCBN3', 'singular, plants, thin objects'),
        ('Bantu4', 'NCBN4', 'plural, plants, thin objects'),
        ('Bantu5', 'NCBN5', 'singular, fruits, round objects, paired things'),
        ('Bantu6', 'NCBN6', 'plural, fruits, round objects, paired things'),
        ('Bantu7', 'NCBN7', 'singular, things, diminutives'),
        ('Bantu8', 'NCBN8', 'plural, things, diminutives'),
        ('Bantu9', 'NCBN9', 'singular, animals, things'),
        ('Bantu10', 'NCB10', 'plural, animals, things'),
        ('Bantu11', 'NCB11', 'long thin objects, natural phenomena, abstracts'),
        ('Bantu12', 'NCB12', 'singular, small things, diminutives'),
        ('Bantu13', 'NCB13', 'plural or mass, small amount of mass'),
        ('Bantu14', 'NCB14', 'plural, diminutives'),
        ('Bantu15', 'NCB15', 'verbal nouns, infinitives'),
        ('Bantu16', 'NCB16', 'definite location, close to somethinf'),
        ('Bantu17', 'NCB17', 'indefinite location, direction, movement'),
        ('Bantu18', 'NCB18', 'definite location, inside somethinf'),
        ('Bantu19', 'NCB19', 'little bit of, pejorative plural'),
        ('Bantu20', 'NCB20', 'singular, augmentatives'),
        ('Bantu21', 'NCB21', 'singular, augmentatives, derogatives'),
        ('Bantu22', 'NCB22', 'plural, augmentatives'),
        ('Bantu23', 'NCB23', 'location with place names'),
        ('Wol1', 'NCWL1', 'Wolof noun class 1/k (singular human)'),
        ('Wol2', 'NCWL2', 'Wolof noun class 2/ñ (plural human)'),
        ('Wol3', 'NCWL3', 'Wolof noun class 3/g (singular)'),
        ('Wol4', 'NCWL4', 'Wolof noun class 4/j (singular)'),
        ('Wol5', 'NCWL5', 'Wolof noun class 5/b (singular)'),
        ('Wol6', 'NCWL6', 'Wolof noun class 6/m (singular)'),
        ('Wol7', 'NCWL7', 'Wolof noun class 7/l (singular)'),
        ('Wol8', 'NCWL8', 'Wolof noun class 8/y (plural non-human)'),
        ('Wol9', 'NCWL9', 'Wolof noun class 9/s (singular)'),
        ('Wol10', 'NCW10', 'Wolof noun class 10/w (singular)'),
        ('Wol11', 'NCW11', 'Wolof noun class 11/g (location)'),
        ('Wol12', 'NCW12', 'Wolof noun class 12/n (manner)'),
    ]),
    ('NumType', 'numeral type', [
        (
        'Card', 'NTCRD', 'cardinal number or corresponding interrogative / relative / indefinite / demonstrative word'),
        ('Dist', 'NTDST', 'distributive numeral'),
        ('Frac', 'NTFRC', 'fraction'),
        ('Mult', 'NTMLT',
         'multiplicative numeral or corresponding interrogative / relative / indefinite / demonstrative word'),
        ('Ord', 'NTORD', 'ordinal number or corresponding interrogative / relative / indefinite / demonstrative word'),
        ('Range', 'NTRNG', 'range of values'),
        ('Sets', 'NTSTS', 'number of sets of things; collective numeral'),
    ]),
    ('Number', 'number', [
        ('Coll', 'NMCLL', 'collective / mass / singulare tantum'),
        ('Count', 'NMCNT', 'count plural'),
        ('Dual', 'NDUAL', 'dual number'),
        ('Grpa', 'NMGRP', 'greater paucal number'),
        ('Grpl', 'NMGRL', 'greater plural number'),
        ('Inv', 'NMINV', 'inverse number'),
        ('Pauc', 'NMBPC', 'paucal number'),
        ('Plur', 'NPLUR', 'plural number'),
        ('Ptan', 'NMPTN', 'plurale tantum'),
        ('Sing', 'NSING', 'singular number'),
        ('Tri', 'NMTRI', 'trial number'),
    ]),
    ('Person', 'person', [
        ('0', 'PRSN0', 'zero person'),
        ('1', 'PRSN1', 'first person'),
        ('2', 'PRSN2', 'second person'),
        ('3', 'PRSN3', 'third person'),
        ('4', 'PRSN4', 'fourth person'),
    ]),
    ('Polarity', 'polarity', [
        ('Neg', 'PLNEG', 'negative'),
        ('Pos', 'PLPOS', 'positive, affirmative'),
    ]),
    ('Polite', 'politeness', [
        ('Elev', 'PLELV', 'referent elevatinf'),
        ('Form', 'PLFRM', 'formal register'),
        ('Humb', 'PLHMB', 'speaker humblinf'),
        ('Infm', 'PLINF', 'informal register'),
    ]),
    ('Poss', 'possessive', [
        ('Yes', 'PSYES', 'it is possessive'),
    ]),
    ('PronType', 'pronominal type', [
        ('Art', 'PTART', 'article'),
        ('Dem', 'PTDEM', 'demonstrative pronoun, determiner, numeral or adverb'),
        ('Emp', 'PTEMP', 'emphatic determiner'),
        ('Exc', 'PTEXC', 'exclamative determiner'),
        ('Ind', 'PTIND', 'indefinite pronoun, determiner, numeral or adverb'),
        ('Int', 'PTINT', 'interrogative pronoun, determiner, numeral or adverb'),
        ('Neg', 'PTNEG', 'negative pronoun, determiner or adverb'),
        ('Prs', 'PTPRS', 'personal or possessive personal pronoun or determiner'),
        ('Rcp', 'PTRCP', 'reciprocal pronoun'),
        ('Rel', 'PTREL', 'relative pronoun, determiner, numeral or adverb'),
        ('Tot', 'PTTOT', 'total (collective) pronoun, determiner or adverb'),
    ]),
    ('Reflex', 'reflexive', [
        ('Yes', 'RFYES', 'it is reflexive'),
    ]),
    ('II', 'In Initio', [
        ('iic', 'IICOM', 'in initio compositi'),
        ('iiv', 'IIVRB', 'in initio verbi'),
    ]),
    ('Tense', 'tense', [
        ('Fut', 'TNFUT', 'future tense'),
        ('Imp', 'TNIMP', 'imperfect'),
        ('Past', 'TNPST', 'past tense / preterite / aorist'),
        ('Pqp', 'TNPQP', 'pluperfect'),
        ('Pres', 'TNPRS', 'present / non-past tense / aorist'),
    ]),
    ('Typo', 'is this a misspelled word?', [
        ('Yes', 'TYYES', 'it is typo'),
    ]),
    ('VerbForm', 'form of verb or deverbative', [
        ('Conv', 'VFCNV', 'converb, transgressive, adverbial participle, verbal adverb'),
        ('Fin', 'VFFIN', 'finite verb'),
        ('Gdv', 'VFGDV', 'gerundive'),
        ('Ger', 'VFGER', 'gerund'),
        ('Inf', 'VFINF', 'infinitive'),
        ('Part', 'VFPRT', 'participle, verbal adjective'),
        ('Sup', 'VFSUP', 'supine'),
        ('Cau', 'VFCAU', 'causative'),
        ('Vnoun', 'VFVNN', 'verbal noun, masdar'),
    ]),
    ('VerbConj', 'verb conjugation', [
        ('[1]', 'VBCJ1', 'first conjugation'),
        ('[2]', 'VBCJ2', 'second conjugation'),
        ('[3]', 'VBCJ3', 'third conjugation'),
        ('[4]', 'VBCJ4', '4th conjugation'),
        ('[5]', 'VBCJ5', '5th conjugation'),
        ('[6]', 'VBCJ6', '6th conjugation'),
        ('[7]', 'VBCJ7', '7th conjugation'),
        ('[8]', 'VBCJ8', '8th conjugation'),
        ('[9]', 'VBCJ9', '9th conjugation'),
        ('[10]', 'VCJ10', '10th conjugation'),
    ]),
    ('VerbType', 'verb type', [
        ('Aux', 'VTAUX', 'auxiliary verb'),
        ('Cop', 'VTCOP', 'copula verb'),
        ('Mod', 'VTMOD', 'modal verb'),
        ('Light', 'VTLGH', 'light (support) verb'),
        ('Quasi', 'VTRQS', 'quasi-verb'),
    ]),
    ('Undecided', 'undecided codes', [
        ('act', '__ACT', ''),
        ('agt', '__AGT', ''),
        ('per', '__PER', ''),
        ('tasil', '__TSL', ''),
    ]),
    ('Voice', 'voice', [
        ('Act', 'VCACT', 'active or actor-focus voice'),
        ('Antip', 'VCANT', 'antipassive voice'),
        ('Bfoc', 'VCBFC', 'beneficiary-focus voice'),
        ('Cau', 'VCCAU', 'causative voice'),
        ('Dir', 'VCDIR', 'direct voice'),
        ('Inv', 'VCINV', 'inverse voice'),
        ('Lfoc', 'VCLFC', 'location-focus voice'),
        ('Mid', 'VCMID', 'middle voice'),
        ('Pass', 'VCPSS', 'passive or patient-focus voice'),
        ('Rcp', 'VCRCP', 'reciprocal voice'),
    ])
]

g_nlp_pos_list = [
    ('NPIIC', 'IIC', 'in intio compositi'),
    ('NPIIV', 'IIV', 'in intio verbi'),
    ('NPADJ', 'ADJ', 'adjective'),
    ('NPADP', 'ADP', 'adposition'),
    ('NPADV', 'ADV', 'adverb'),
    ('NPAUX', 'AUX', 'auxiliary'),
    ('NPCNJ', 'CCONJ', 'coordinating conjunction'),
    ('NPDET', 'DET', 'determiner'),
    ('NPIND', 'IND', 'indeclinable'),
    ('NPINT', 'INTJ', 'interjection'),
    ('NPNON', 'NOUN', 'noun'),
    ('NPNUM', 'NUM', 'numeral'),
    ('NPPRT', 'PART', 'particle'),
    ('NPPRN', 'PRON', 'pronoun'),
    ('NPPPN', 'PROPN', 'proper noun'),
    ('NPPCT', 'PUNCT', 'punctuation'),
    ('NPSCJ', 'SCONJ', 'subordinating conjunction'),
    ('NPSYM', 'SYM', 'symbol'),
    ('NPVRB', 'VERB', 'verb'),
    ('NPXXX', 'X', 'other')
]

g_nlp_tag_list = [
    ('NTGSE', '.', 'punctuation mark, sentence closer'),
    ('NTGCM', ',', 'punctuation mark, comma'),
    ('NTLRB', '-LRB-', 'left round bracket'),
    ('NTRRB', '-RRB-', 'right round bracket'),
    ('NTOQM', '``', 'opening quotation mark'),
    ('NTCQM', '""', 'closing quotation mark'),
    ("NTCRM", "''", 'closing quotation mark'),
    ('NTGCL', ':', 'punctuation mark, colon or ellipsis'),
    ('NTDOL', '$', 'symbol, currency'),
    ('NTSRP', '#', 'symbol, number sign'),
    ('NTAFX', 'AFX', 'affix'),
    ('NTGCC', 'CC', 'conjunction, coordinatinf'),
    ('NTGCD', 'CD', 'cardinal number'),
    ('NTGDT', 'DT', 'determiner'),
    ('NTGEX', 'EX', 'existential there'),
    ('NTGFW', 'FW', 'foreign word'),
    ('NTHYP', 'HYPH', 'punctuation mark, hyphen'),
    ('NTGIN', 'IN', 'conjunction, subordinating or preposition'),
    ('NTGJJ', 'JJ', 'adjective (English), other noun-modifier (Chinese)'),
    ('NTJJR', 'JJR', 'adjective, comparative'),
    ('NTJJS', 'JJS', 'adjective, superlative'),
    ('NTGLS', 'LS', 'list item marker'),
    ('NTGMD', 'MD', 'verb, modal auxiliary'),
    ('NTNIL', 'NIL', 'missing taf'),
    ('NTGNN', 'NN', 'noun, singular or mass'),
    ('NTNNP', 'NNP', 'noun, proper singular'),
    ('NTNPP', 'NNPS', 'noun, proper plural'),
    ('NTNNS', 'NNS', 'noun, plural'),
    ('NTPDT', 'PDT', 'predeterminer'),
    ('NTPOS', 'POS', 'possessive endinf'),
    ('NTPRP', 'PRP', 'pronoun, personal'),
    ('NTPR$', 'PRP$', 'pronoun, possessive'),
    ('NTGRB', 'RB', 'adverb'),
    ('NTRBR', 'RBR', 'adverb, comparative'),
    ('NTRBS', 'RBS', 'adverb, superlative'),
    ('NTGRP', 'RP', 'adverb, particle'),
    ('NTGTO', 'TO', 'infinitival “to”'),
    ('NTGUH', 'UH', 'interjection'),
    ('NTGVB', 'VB', 'verb, base form'),
    ('NTVBD', 'VBD', 'verb, past tense'),
    ('NTVBG', 'VBG', 'verb, gerund or present participle'),
    ('NTVBN', 'VBN', 'verb, past participle'),
    ('NTVBP', 'VBP', 'verb, non-3rd person singular present'),
    ('NTVBZ', 'VBZ', 'verb, 3rd person singular present'),
    ('NTWDT', 'WDT', 'wh-determiner'),
    ('NTGWP', 'WP', 'wh-pronoun, personal'),
    ('NTWP$', 'WP$', 'wh-pronoun, possessive'),
    ('NTWRB', 'WRB', 'wh-adverb'),
    ('NTGSP', 'SP', 'space (English), sentence-final particle (Chinese)'),
    ('NTADD', 'ADD', 'email'),
    ('NTNFP', 'NFP', 'superfluous punctuation'),
    ('NTGGW', 'GW', 'additional word in multi-word expression'),
    ('NTGXX', 'XX', 'unknown'),
    ('NTBES', 'BES', 'auxiliary “be”'),
    ('NTHVS', 'HVS', 'forms of “have”'),
    ('NT_SP', '_SP', 'whitespace')
]

g_special_vertices = [
    ['FOLDR', ['SI_IS_TYPE', 'SI_IS_SELECTABLE'],          'FOLDER_ICON_PATH',           'FOLDER_SIID', None,     'Special Vertex ID (Vertex type) of folders (ordinary vertices with no special role)'],
    ['AUTO_', ['SI_IS_TYPE', 'SI_EDGE_TYPE'],              'AUTO_ICON_PATH',             'AUTO_SIID', None,       'Special Vertex ID (Edge type) of AUTO edges'],
    # --------------------------- OWNS / BLNGS ---------------------------
    ['OWNS_', ['SI_IS_TYPE', 'SI_EDGE_TYPE',
               'SI_IS_SELECTABLE'],                        'OWNS_ICON_PATH',             'OWNS_SIID',
    ('BLNGS', ['SI_IS_TYPE', 'SI_EDGE_TYPE',
               'SI_INSERT_AT_TOP', 'SI_IS_SELECTABLE'],    'BLNGS_ICON_PATH',            'BLNGS_SIID'),           'Special Vertex ID (Edge type) of ownership edges'],
    # --------------------------- ISA / ITO ---------------------------
    ['_ISA_', ['SI_IS_TYPE', 'SI_EDGE_TYPE',
               'SI_IS_SPECIAL_EDGE', 'SI_IS_SELECTABLE'],  'ISA_ICON_PATH',              'ISA_SIID',
    ('_ITO_', ['SI_IS_TYPE', 'SI_EDGE_TYPE',
               'SI_IS_SELECTABLE'],                        'ITO_ICON_PATH',              'ITO_SIID'),            'Special Vertex ID (Edge type) of type edges'],
    # --------------------------- WROTE / WRTBY ---------------------------
    ['WROTE', ['SI_IS_TYPE', 'SI_EDGE_TYPE'],              'TEXT_WROTE_ICON_PATH',      'TEXT_WROTE_SIID',
    ('WRTBY', ['SI_IS_TYPE', 'SI_EDGE_TYPE'],              'TEXT_WRITTEN_BY_ICON_PATH', 'TEXT_WRITTEN_BY_SIID'), 'Special Vertex ID (Edge type) wrote / written by'],
    # ---------------------------------------------------------------------
    ['OCCUR', ['SI_IS_TYPE', 'SI_EDGE_TYPE',
               'SI_IS_SPECIAL_EDGE'],                      'OCCURRENCE_ICON_PATH',      'OCCUR_SIID', None,          'Special Vertex ID (Edge type) of occurrence edges'],
    ['WFORM', 'SI_IS_TYPE | SI_IS_ICON_TYPE',              'WFORM_ICON_PATH',           'WFORM_SIID', None,          'Special Vertex ID (Vertex type) of word form vertices'],
    ['TXATL', 'SI_IS_TYPE',                                None,                        'TEXT_ALT_TITLE_SIID', None, 'Special Vertex ID (Simple edge type) alternate text title'],
    ['TXSBT', 'SI_IS_TYPE',                                None,                        'TEXT_SUB_TITLE_SIID', None, 'Special Vertex ID (Simple edge type) text subtitle'],
    ['CAPTL', 'SI_IS_TYPE',                                None,                        'CAPTL_SIID', None,          'Special Vertex ID (Simple edge type) of Occurrence field capitalization flag'],
    ['PCTLF', 'SI_IS_TYPE',                                None,                        'PCTLF_SIID', None,          'Special Vertex ID (Simple edge type) of Occurrence field left punctuation'],
    ['PCTRT', 'SI_IS_TYPE',                                None,                        'PCTRT_SIID', None,          'Special Vertex ID (Simple edge type) of Occurrence field right punctuation'],
    ['MKPLF', 'SI_IS_TYPE',                                None,                        'MKPLF_SIID', None,          'Special Vertex ID (Simple edge type) of Occurrence field markup left'],
    ['MKPRT', 'SI_IS_TYPE',                                None,                        'MKPRT_SIID', None,          'Special Vertex ID (Simple edge type) of Occurrence field markup right'],
    ['STPOS', 'SI_IS_TYPE',                                None,                        'STPOS_SIID', None,          'Special Vertex ID (Simple edge type) of Occurrence field sentence position'],
    ['HLCLR', 'SI_IS_TYPE',                                None,                        'HLCLR_SIID', None,          'Color of text highlight category'],
]

g_vertices = [
# 0123456789012345678901234567890
# |---------|---------|---------|---------|---------|---------|               |---------|---------|---------| |---------|---------|---------|      |---------|---------|---------|
# l_label,                       l_vertex_flag_list,              l_type_list, l_edges               l_mnemo, l_si_flag_list,                      l_icon_path,              l_pseudo_constant
['Global graph root',            ['FULL_VERTEX',  'IS_SPECIAL'],  ['FOLDR'],  None,                  'ROOT_', '0',                                 None,                     'ROOT_SIID'],
['Root of all types',            ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'ROOT_')], 'TYPE_', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'TYPE_NODE_ICON_PATH',    'TYPE_NODE_SIID'],
['Person (type)',                ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'PERSN', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE',
                                                                                                               'SI_IS_SELECTABLE'],                'PERSON_ICON_PATH',       'PERS_TYPE_SIID'],
['Organization (type)',          ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'ORGN_', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE',
                                                                                                               'SI_IS_SELECTABLE'],                'ORGANIZATION_ICON_PATH', 'ORG_TYPE_SIID'],
['Home',                         ['FULL_VERTEX',  'IS_SPECIAL'],  ['FOLDR'],  [('BLNGS',  'ROOT_')], 'HOME_', '0',                                 None,                     'HOME_SIID'],
['Texts Root (type)',            ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'HOME_')], 'TEXT_', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE',
                                                                                                               'SI_IS_SELECTABLE'],                'TEXT_ICON_PATH',         'TEXT_SIID'],
['Baghavad Gita',                ['FULL_VERTEX',  'IS_SPECIAL'],  ['FOLDR'],  [('BLNGS',  'HOME_')], 'BGRT_', '0',                                 None,                     'BG_ROOT_SIID'],
['The Republic (Plato)',         ['FULL_VERTEX',  'IS_SPECIAL'],  ['FOLDR'],  [('BLNGS',  'HOME_')], 'REPRT', '0',                                 None,                     'REPUBLIC_ROOT_SIID'],
['Inboxes / Message Root Type',  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'HOME_')], '_MSG_', ['SI_IS_TYPE'],                      None,                     'MSG_TYPE_SIID'],
['Me',                           ['FULL_VERTEX',  'IS_SPECIAL'],  ['PERSN'],  [('BLNGS',  'HOME_')], 'ME___', '0',                                 None,                     'ME_SIID'],
['Email Inbox',                  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'EMAIL', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'EMAIL_TYPE_SIID'],
['Whatsapp Inbox',               ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'WHTSP', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'WHTSP_TYPE_SIID'],
['Discord Inbox',                ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'DSCRD', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'DSCRD_TYPE_SIID'],
['GChat Inbox',                  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'GCHAT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'GCHAT_TYPE_SIID'],
['SMS Inbox',                    ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  '_MSG_')], 'SMS__', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'SMS_TYPE_SIID'],
['Grammar Attributes (type)',    ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'GRATT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'FOLDER_ICON_PATH',       'GRAMMAR_ATTR_SIID'],
['Lemma (type)',                 ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'LEMMA', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE',
                                                                                                               'SI_IS_ICON_TYPE'],                 'LEMMA_ICON_PATH',        'LEMMA_SIID'],
['NLP Entity (type)',            ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'NLENT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'ENTITY_ICON_PATH',       'NLENT_SIID'],
['NLP Pos code (type)',          ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'NLPOS', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'POS_ICON_PATH',          'NLPOS_SIID'],
['NLP Tag code (type)',          ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'NLTAG', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'TAG_ICON_PATH',          'NLTAG_SIID'],
['Stephanus Section (type)',     ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'STPSC', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'FOLDER_ICON_PATH',       'STEPHANUS_SIID'],
['Text Version (type)',          ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'TXTVR', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'FOLDER_ICON_PATH',       'TXTVR_SIID'],
['Notes (type)',                 ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'TXTNT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'FOLDER_ICON_PATH',       'TXTNT_SIID'],
['Text Chunk (type)',            ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'TXTCK', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE',
                                                                                                               'SI_IS_SELECTABLE'],                'TEXT_CHUNK_ICON_PATH',   'TEXT_CHUNK_SIID'],
['Book (type)',                  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'TXTBK', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'BOOK_ICON_PATH',         'TEXT_BOOK_SIID'],
['Sentence (type)',              ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'TXSNT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'SENTENCE_ICON_PATH',     'TEXT_SENTENCE_SIID'],
['Highlight (type)',             ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'TXHLT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  'HIGHLIGHTER_ICON_PATH',  'TEXT_HIGHLIGHT_SIID'],
['Highlight category (type)',    ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  [('BLNGS',  'REPRT')], 'TXHLC', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'TEXT_HIGHLIGHT_CAT_SIID'],
['Highlight Folder (type)',      ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'TXHLF', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'TEXT_HIGHLIGHT_FLDR_SIID'],
['Highlight Categories Folder',  ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'TXHCF', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  None,                     'TEXT_HIGHLIGHT_CAT_FLDR_SIID'],
['Highlight Chunk',              ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'TXHCK', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  "HL_CHUNK_ICON_PATH",     'TEXT_HIGHLIGHT_CHUNK_SIID'],
['Sloka folder',                 ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'SKFLD', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],  "SLOKA_FOLDER_ICON_PATH", 'TEXT_SLOKA_FLDR_SIID'],
['Highlight Quotation (type)',   ['FULL_VERTEX',  'IS_SPECIAL'],  ['TYPE_'],  None,                  'TXHQT', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE', 'SI_EDGE_TYPE',
                                                                                                               'SI_IS_SELECTABLE'],                "HL_QUOTE_ICON_PATH",     'TEXT_HIGHLIGHT_QUOTE_SIID'],
]

g_special_vertices_gita = [
    ['PRABH', 'SI_IS_TYPE', None,                      'TEXT_WFW_PRABUPADA_SIID',    None, 'Special Vertex ID (Vertex type) of Gita translation source: Prabhupada'],
    ['SIVAN', 'SI_IS_TYPE', None,                      'TEXT_WFW_SIVANANDA_SIID',    None, 'Special Vertex ID (Vertex type) of Gita translation source: Sivananda'],
    ['GAMBI', 'SI_IS_TYPE', None,                      'TEXT_WFW_GAMBIRANANDA_SIID', None, 'Special Vertex ID (Vertex type) of Gita translation source: Gambirananda'],
    ['TRSLT', 'SI_IS_TYPE', None,                      'TEXT_WORD_TRANSLIT_SIID',    None, 'Special Vertex ID (Simple Edge type) of Gita transliteration field'],
    ['DCTRF', 'SI_IS_TYPE', None,                      'TEXT_WORD_DICT_REF_SIID',    None, 'Special Vertex ID (Simple Edge type) of Gita dictionary ref. field'],
    ['WFTRN', 'SI_IS_TYPE', None,                      'TEXT_WFW_TRANSL_SIID',       None, 'Special Vertex ID (Simple Edge type) WfW unit Translation field'],
    ['URLNK', 'SI_IS_TYPE', 'TEXT_URL_LINK_ICON_PATH', 'TEXT_URL_LINK_SIID',         None, 'Special Vertex ID (Vertex type) Url link'],
    # --------------------------- HS2OB / OC2HB ---------------------------
    ['HS2OB', 'SI_IS_TYPE', 'TW_SECTION_2_OCC_BEGIN_ICON_PATH', 'TW_SECTION_2_OCC_BEGIN_SIID',
    ('OC2HB', 'SI_IS_TYPE', 'TW_SECTION_2_OCC_BEGIN_ICON_PATH', 'TW_REV_SECTION_2_OCC_BEGIN_SIID'), 'Special Vertex ID (Edge type) begin section + reciprocal'],
    # --------------------------- HS2OE / OC2HE ---------------------------
    ['HS2OE', 'SI_IS_TYPE', 'TW_SECTION_2_OCC_END_ICON_PATH',   'TW_SECTION_2_OCC_END_SIID',
    ('OC2HE', 'SI_IS_TYPE', 'TW_SECTION_2_OCC_END_ICON_PATH',   'TW_REV_SECTION_2_OCC_END_SIID'), 'Special Vertex ID (Edge type) end section + reciprocal'],
]

g_vertices_gita = [
['His Divine Grace A. C. Bhaktivedanta Swami Prabhupada', ['FULL_VERTEX',  'IS_SPECIAL'], ['PERSN'], [('BLNGS',  'BGRT_')], 'PPRAB', '0', None, None],
['Swami Sri Sivananda Saraswati'                        , ['FULL_VERTEX',  'IS_SPECIAL'], ['PERSN'], [('BLNGS',  'BGRT_')], 'PSIVA', '0', None, None],
['Swami Gambirananda',                                    ['FULL_VERTEX',  'IS_SPECIAL'], ['PERSN'], [('BLNGS',  'BGRT_')], 'PGAMB', '0', None, None],
['Sloka (type)',                                          ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'BGRT_')], 'TXSLK', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'], 'SLOKA_ICON_PATH', 'SLOKA_SIID'],
['Sloka Lines (type)',                                    ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'BGRT_')], 'SLKLN', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
 'TEXT_SLOKA_LINE_ICON_PATH', 'TEXT_SLOKA_LINE_SIID'],
['INRIA Dictionary Reference (type)',                     ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'BGRT_')], 'INRIA', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
 'FOLDER_ICON_PATH', 'TEXT_WORD_DREF_INRIA_SIID'],
['Sloka Translation (type)',                              ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'BGRT_')], 'SLTRN', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
 'TEXT_SLOKA_TRANSLATION_ICON_PATH', 'TEXT_SLOKA_TRANSLATION_SIID'],
['Sloka Bhashya (type)',                                  ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'BGRT_')], 'SLBHS', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
 'TEXT_SLOKA_BHASHYA_ICON_PATH', 'TEXT_SLOKA_BHASHYA_SIID'],
['WfW Translation Unit (type)',                           ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'BGRT_')], 'TRLUN', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
 'TEXT_WFW_UNIT_ICON_PATH', 'TEXT_WFW_UNIT_SIID'],
]

g_declaration_template = """
#include "m1A_constants.h"

namespace M1Env{

__EXTERN_DECL__

    class GraphInit{
        public:
            static void set_pseudo_constants();
            static void dbg_dump_pseudo_constants();
            static void init_base();
            static void init_plato();
            static QList<M1Env::SpecialItemID> cm_gram_attr_list; 
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

void M1Env::GraphInit::dbg_dump_pseudo_constants(){
    qCDebug(g_cat_silence) << QString("================ Constants dump ========");

    __PSEUDO_DUMP__
}

void M1Env::GraphInit::init_base(){
    __INIT_BASE__
}

void M1Env::GraphInit::init_plato(){
    __INIT_PLATO__
}
"""
# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    # gita
    g_vertices += g_vertices_gita
    g_special_vertices += g_special_vertices_gita

    l_attr_2_mn = dict()
    l_attr_class_ssid_list = []
    # Grammar attributes -----------------------------------------------------------------------------------------------
    for l_name_class, l_label_class, l_val_list in g_attr_values:
        l_mn_class = (re.sub(r'[a-z]', '', l_name_class) + re.sub(r'[aeiouA-Z]', '', l_name_class).upper())
        if len(l_mn_class) < 5:
            l_mn_class = l_name_class.upper()
        l_mn_class = (l_mn_class + '_' * 5)[:5]
        print(l_mn_class, l_name_class)

        # ['Email Inbox', ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  '_MSG_')], 'EMAIL', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'], None, 'EMAIL_TYPE_SIID'],
        # l_vertex_label = (f'[{l_name_class}]-{l_label_class}'[:64]
        #                           if l_name_class.lower() != l_label_class.lower()
        #                           else l_name_class[:64])
        l_vertex_label = l_label_class.capitalize()
        l_ssid_attr = f'{l_mn_class}_SIID'
        l_attr_class_ssid_list.append(l_ssid_attr)
        g_vertices.append([
            l_vertex_label,
            ['FULL_VERTEX',  'IS_SPECIAL'],
            ['GRATT'],
            None,
            l_mn_class,
            ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
            'FOLDER_ICON_PATH',
            l_ssid_attr])

        # [('Ex', 'CLSEX', 'exclusive'), ('In', 'CLSIN', 'inclusive')]
        for l_name_attr, l_mn_attr, l_label_attr in l_val_list:
            l_attr_key = f'{l_name_class}-{l_name_attr}'
            l_attr_2_mn[l_attr_key] = l_mn_attr

            l_vertex_label = f'[{l_name_attr}]-{l_label_attr}'[:62] \
                if l_name_attr.lower() != l_label_attr.lower() \
                else l_name_attr[:62]

            g_vertices.append([
                l_vertex_label,
                ['FULL_VERTEX',  'IS_SPECIAL'],
                ['TYPE_', l_mn_class],
                None,
                l_mn_attr,
                ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
                'FOLDER_ICON_PATH',
                None])

    l_mnemo_dict = dict()
    def add_mnemo(p_mn):
        global l_mnemo_dict
        l_mnemo_dict[p_mn] = l_mnemo_dict.setdefault(p_mn, 0) + 1

    # Tag & Pos --------------------------------------------------------------------------------------------------------
    l_pos_2_mn = dict()
    for l_mn, l_pos, l_label in g_nlp_pos_list:
        l_pos_2_mn[l_pos] = l_mn
        # ['NLP Tag code (type)', ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'HOME_')], 'NLTAG', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'], 'ENTITY_ICON_PATH', 'NLTAG_SIID'],
        g_vertices.append([
            f'[{l_pos}]-{l_label}',
            ['FULL_VERTEX', 'IS_SPECIAL'],
            ['TYPE_', 'NLPOS'],
            None,
            l_mn,
            ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
            None,
            f'{l_mn}_SIID'
        ])

    l_tag_2_mn = dict()
    for l_mn, l_tag, l_label in g_nlp_tag_list:
        l_tag_2_mn[l_tag] = l_mn
        # ['NLP Tag code (type)', ['FULL_VERTEX',  'IS_SPECIAL'], ['TYPE_'], [('BLNGS',  'HOME_')], 'NLTAG', ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'], 'ENTITY_ICON_PATH', 'NLTAG_SIID'],
        g_vertices.append([
            f'[{l_tag}]-{l_label}',
            ['FULL_VERTEX', 'IS_SPECIAL'],
            ['TYPE_', 'NLTAG'],
            None,
            l_mn,
            ['SI_IS_TYPE', 'SI_REQUIRES_EDGE'],
            None,
            f'{l_mn}_SIID'
        ])

    # |---------|---------|---------|---------|---------|---------|               |---------|---------|---------| |---------|---------|---------|      |---------|---------|---------|
    # l_label,                       l_vertex_flag_list,              l_type_list, l_edges               l_mnemo, l_si_flag_list,                      l_icon_path,              l_pseudo_constant
    for l_label, l_vertex_flag_list, l_type_list, l_edges, l_mnemo, l_si_flag_list, l_icon_path, l_pseudo_constant in g_vertices:
        print(f'{l_label:80} {str(l_vertex_flag_list):30} {l_type_list} {str(l_edges):25} {l_mnemo} {str(l_si_flag_list):35} {str(l_icon_path):25} {l_pseudo_constant}')

    # Ancillaries
    l_initialization = ''
    l_dump = ''
    l_declaration = ''
    l_extern_declaration = ''
    for l_mnemo, _, _, l_pseudo_constant, l_reciprocal, l_comment in g_special_vertices:
        l_extern_declaration += f'// [{l_mnemo}] {l_comment}\nextern M1Env::SpecialItemID {l_pseudo_constant};\n'
        l_declaration += f'// [{l_mnemo}] {l_comment}\nM1Env::SpecialItemID M1Env::{l_pseudo_constant} = G_NONEX_SI_ID;\n'
        l_initialization += f'    M1Env::{l_pseudo_constant} = M1Store::StorageStatic::getSpecialID("{l_mnemo}");\n'
        l_dump += f'    qCDebug(g_cat_silence) << QString("0x%1 M1Env::{l_pseudo_constant} <-- %2").arg(M1Store::StorageStatic::getSpecialID("{l_mnemo}"), 4, 16, QChar(\'0\')).arg("{l_mnemo}");\n'
        add_mnemo(l_mnemo)
        if l_reciprocal:
            l_mnemo_r, l_flags_r, l_icon_path_r, l_pseudo_constant_r = l_reciprocal
            add_mnemo(l_mnemo_r)
            l_extern_declaration += (f'// [{l_mnemo_r}] {l_comment} (reciprocal of {l_mnemo} above))\n' +
                                     f'extern M1Env::SpecialItemID {l_pseudo_constant_r};\n')
            l_declaration += (f'// [{l_mnemo_r}] {l_comment} (reciprocal of {l_mnemo} above))\n' +
                              f'M1Env::SpecialItemID M1Env::{l_pseudo_constant_r} = G_NONEX_SI_ID;\n')
            l_initialization += f'    M1Env::{l_pseudo_constant_r} = M1Store::StorageStatic::getSpecialID("{l_mnemo_r}");\n'

    l_extern_declaration += '\n'
    l_declaration += '\n'
    for l_label, _, _, _, l_mnemo, _, _, l_pseudo_constant in g_vertices:
        add_mnemo(l_mnemo)
        if l_pseudo_constant:
            l_extern_declaration += f'// [{l_mnemo}] Special Vertex ID of "{l_label}"\nextern M1Env::SpecialItemID {l_pseudo_constant};\n'
            l_declaration += f'M1Env::SpecialItemID M1Env::{l_pseudo_constant} = G_NONEX_SI_ID;\n'
            l_initialization += f'    M1Env::{l_pseudo_constant} = M1Store::StorageStatic::getSpecialID("{l_mnemo}");\n'

    # print(l_extern_declaration)
    # print(l_declaration)
    # print(l_initialization)

    with open('m1_src/m1B_graph_init.h', 'w') as l_fout:
        l_fout.write(g_declaration_template.replace('__EXTERN_DECL__', l_extern_declaration))

    # main code
    l_base_code = ''
    for l_mnemo, l_flags, l_icon_path, _, l_reciprocal, l_comment in g_special_vertices:
        if isinstance(l_flags, list):
            l_flags = ' | '.join(l_flags)
        l_icon_path = f'M1Env::{l_icon_path}' if l_icon_path else 'nullptr'
        if l_reciprocal:
            l_mnemo_r, l_flags_r, l_icon_path_r, l_pseudo_constant_r = l_reciprocal
            if isinstance(l_flags_r, list):
                l_flags_r = ' | '.join(l_flags_r)
            l_icon_path_r = f'M1Env::{l_icon_path_r}' if l_icon_path_r else 'nullptr'
            l_base_code += (f'    // {l_comment}\n' +
                            f'    M1Store::StorageStatic::getNewSpecialWithReciprocal({l_flags},\n' +
                            f'        "{l_mnemo}", {l_icon_path},\n' +
                            f'        "{l_mnemo_r}", {l_icon_path_r},\n' +
                            f'        {l_flags_r});\n')
        else:
            l_base_code += f'    // {l_comment}\n    M1Store::StorageStatic::getNewSpecialNoItem({l_flags}, "{l_mnemo}", {l_icon_path});\n'

    l_base_code += '\n'
    l_mnemo_2_var = dict()
    for l_label, l_vertex_flag_list, l_type_list, l_edges, l_mnemo, l_si_flag_list, l_icon_path, l_pseudo_constant in g_vertices:
        l_var_name = 'l_' + l_mnemo.replace('_', '').lower()
        l_vertex_flag_string = ' | '.join([f'M1Env::{l_flag}' for l_flag in l_vertex_flag_list])
        l_debug_label = f'[{l_mnemo}]-{l_label}' if l_mnemo else f'{l_label}'
        l_base_code += (f'    // creation of "{l_label}"\n' +
                        f'    qCDebug(g_cat_silence) << QString("Creating <{l_debug_label}> item");\n' +
                        f'    M1Store::Item_lv2* {l_var_name} = M1Store::Item_lv2::getNew(\n' +
                        f'        // vertex flags\n' +
                        f'        {l_vertex_flag_string},\n' +
                        f'        // label\n' +
                        f'        "{l_label}"')
        if not l_mnemo:
            l_base_code += ');\n'
        else:
            l_base_code += ',\n'
            l_mnemo_2_var[l_mnemo] = l_var_name
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
                l_base_code += f'    {l_var_name}->linkTo({l_mnemo_2_var[l_dest_mnemo]}, "{l_edge_type}", nullptr, true);\n'
        l_base_code += '\n'

    # Plato ------------------------------------------------------------------------------------------------------------
    print('\n================ Plato =====================')
    l_plato_code = ''

    with open('republic.json', 'r') as f:
        l_republic = json.load(f)
    print('Lemmas:', len(l_republic['Lemmas']))
    print('Forms :', len(l_republic['Forms']))
    print('Notes :', len(l_republic['Notes']))
    print('Shorey:', len(l_republic['Versions']['Shorey']))
    print('Jowett:', len(l_republic['Versions']['Jowett']))
    print('Greek :', len(l_republic['Versions']['Greek']))

    l_title = l_republic['Title']
    l_plato_code += (f'    // creation of Republic text node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Republic text node");\n' +
                     f'    M1Store::Item_lv2* l_republic = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "{l_title} (Dialogue)");\n')
    l_plato_code +=   '    l_republic->setType("TEXT_");\n\n'
    l_plato_code +=   '    l_republic->linkTo(M1Store::Item_lv2::getExisting("REPRT"), "BLNGS");\n\n'
    l_plato_code += (f'    // creation of Plato Person node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Plato node");\n' +
                     f'    M1Store::Item_lv2* l_plato = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "Plato, son of Ariston of Collytus");\n')
    l_plato_code +=   '    l_plato->setType("PERSN");\n'
    l_plato_code +=   '    l_republic->linkTo(l_plato, "WRTBY");\n\n'

    # Notes
    l_plato_code += (f'    // creation of Republic text notes node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Republic text node");\n' +
                     f'    M1Store::Item_lv2* l_republic_notes = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "Notes");\n')
    l_plato_code +=   '    l_republic_notes->setType("FOLDR");\n'
    # l_plato_code +=   '    l_republic_notes->linkTo(l_republic, "BLNGS");\n\n'
    l_plato_code += '    l_republic->linkTo(l_republic_notes, "OWNS_");\n\n'

    l_plato_code += f'    M1Store::Item_lv2* l_note_array[{len(l_republic["Notes"])}];\n'
    l_note_id = 0
    for l_note_key in l_republic["Notes"]:
        l_note_txt = l_republic["Notes"][l_note_key][0]
        l_plato_code += (f'    // creation of note <{l_note_key}>\n' +
                         f'    qCDebug(g_cat_silence) << QString("Creating note <{l_note_key}>");\n' +
                         f'    l_note_array[{l_note_id}] = M1Store::Item_lv2::getNew(\n' +
                         f'        // vertex flags\n' +
                         f'        M1Env::FULL_VERTEX,\n' +
                         f'        // label\n' +
                         f'        "{l_note_txt}");\n')
        l_plato_code +=  f'    l_note_array[{l_note_id}]->setType("TXTNT");\n'
        l_plato_code +=  f'    l_note_array[{l_note_id}]->linkTo(l_republic_notes, "BLNGS");\n\n'
        l_note_id += 1

    # Lemmas
    l_plato_code += f'    M1Store::Item_lv2* l_lemma_array[{len(l_republic["Lemmas"])}];\n'
    l_lemma_id = 0
    l_lemma_key_2_id = dict()
    for l_lemma_key in sorted(l_republic["Lemmas"].keys(), reverse=True):
        l_lemma_key_2_id[l_lemma_key] = l_lemma_id
        l_lemma_txt = l_republic["Lemmas"][l_lemma_key]['Text']
        l_lemma_pos = l_republic["Lemmas"][l_lemma_key]['Pos']
        l_plato_code += (f'    // creation of lemma <{l_lemma_key}>\n' +
                         f'    qCDebug(g_cat_silence) << QString("Creating lemma <{l_lemma_key}>");\n' +
                         f'    l_lemma_array[{l_lemma_id}] = M1Store::Item_lv2::getNew(\n' +
                         f'        // vertex flags\n' +
                         f'        M1Env::FULL_VERTEX,\n' +
                         f'        // label\n' +
                         f'        "{l_lemma_txt}");\n')
        l_plato_code +=  f'    l_lemma_array[{l_lemma_id}]->setType("LEMMA");\n'
        l_plato_code +=  f'    l_lemma_array[{l_lemma_id}]->setType("{l_pos_2_mn[l_lemma_pos]}");\n\n'
        l_lemma_id += 1

    # forms
    l_plato_code += f'    M1Store::Item_lv2* l_form_array[{len(l_republic["Forms"].keys())}];\n'
    l_form_id = 0
    l_form_key_2_id = dict()
    for l_form_key in l_republic["Forms"].keys():
        l_form_key_2_id[l_form_key] = l_form_id
        l_form_txt = l_republic["Forms"][l_form_key]['Text']
        l_form_tag = l_republic["Forms"][l_form_key]['Tag']
        l_plato_code += (f'    // creation of form <{l_form_key}>\n' +
                         f'    qCDebug(g_cat_silence) << QString("Creating form <{l_form_key}>");\n' +
                         f'    l_form_array[{l_form_id}] = M1Store::Item_lv2::getNew(\n' +
                         f'        // vertex flags\n' +
                         f'        M1Env::FULL_VERTEX,\n' +
                         f'        // label\n' +
                         f'        "{l_form_txt}");\n')
        l_plato_code +=  f'    l_form_array[{l_form_id}]->setType("WFORM");\n'
        l_tag_mn = l_tag_2_mn[l_form_tag] if l_form_tag in l_tag_2_mn.keys() else l_pos_2_mn[l_form_tag]
        l_plato_code +=  f'    l_form_array[{l_form_id}]->setType("{l_tag_mn}");\n'
        for l_lemma_key in l_republic["Forms"][l_form_key]['LemmaKey']:
            l_lemma_id = l_lemma_key_2_id[l_lemma_key]
            l_plato_code +=  f'    l_form_array[{l_form_id}]->linkTo(l_lemma_array[{l_lemma_id}], "BLNGS");\n'

        l_form_id += 1
        l_plato_code += "\n"

    # books
    l_plato_code += (f'    // creation of Republic books node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Republic books node");\n' +
                     f'    M1Store::Item_lv2* l_republic_books = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "Books");\n')
    l_plato_code +=   '    l_republic_books->setType("FOLDR");\n'
    l_plato_code +=   '    l_republic->linkTo(l_republic_books, "OWNS_");\n\n'
    l_plato_code +=   '    M1Store::Item_lv2* l_book[10];\n'
    for l_book_num in range(10, 0, -1):
        l_plato_code += (f'    // creation of Republic book {l_book_num} node\n' +
                         f'    qCDebug(g_cat_silence) << QString("Creating Republic book {l_book_num} node");\n' +
                         f'    l_book[{l_book_num - 1}] = M1Store::Item_lv2::getNew(\n' +
                         f'        // vertex flags\n' +
                         f'        M1Env::FULL_VERTEX,\n' +
                         f'        // label\n' +
                         f'        "Book {l_book_num}");\n')
        l_plato_code +=  f'    l_book[{l_book_num - 1}]->setType("TXTBK");\n'
        l_plato_code +=  f'    l_republic_books->linkTo(l_book[{l_book_num - 1}], "OWNS_");\n\n'

    # Sections
    l_plato_code += (f'    // creation of Republic sections node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Republic sections node");\n' +
                     f'    M1Store::Item_lv2* l_republic_sections = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "Stephanus Section");\n')
    l_plato_code +=   '    l_republic_sections->setType("FOLDR");\n'
    l_plato_code +=   '    l_republic->linkTo(l_republic_sections, "OWNS_");\n\n'

    # Highlights folder
    l_plato_code += (f'    // creation of Republic highlights node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Republic highlights folder");\n' +
                     f'    M1Store::Item_lv2* l_republic_highlights = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "Highlights");\n')
    l_plato_code +=   '    l_republic_highlights->setType("FOLDR");\n'
    l_plato_code +=   '    l_republic_highlights->setType("TXHLF");\n'
    l_plato_code +=   '    l_republic->linkTo(l_republic_highlights, "OWNS_");\n\n'

    # Highlights categories folder
    l_plato_code += (f'    // creation of Republic highlight categories node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Republic highlight categories folder");\n' +
                     f'    M1Store::Item_lv2* l_republic_highlight_cat = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "Highlight categories");\n')
    l_plato_code +=   '    l_republic_highlight_cat->setType("FOLDR");\n'
    l_plato_code +=   '    l_republic_highlight_cat->setType("TXHCF");\n'
    l_plato_code +=   '    l_republic->linkTo(l_republic_highlight_cat, "OWNS_");\n\n'

    # example highlights categories HLCLR
    l_plato_code += (f'    // creation of example highlights categories\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating example Republic highlight categories");\n' +
                     f'    M1Store::Item_lv2* l_republic_highlight_cat_1 = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,\n' +
                     f'        // label\n' +
                     f'        "Category 1",\n' +
                     f'        // Special Item flag\n' +
                     f'        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,\n' +
                     f'        // mnemonic\n' +
                     f'        "RCTG1",\n' +
                     f'        // icon path\n' +
                     f'        nullptr);\n')
    l_plato_code +=   '    l_republic_highlight_cat_1->setType("TYPE_");\n'
    l_plato_code +=   '    l_republic_highlight_cat_1->setType("TXHLC");\n'
    l_plato_code +=  f'    l_republic_highlight_cat_1->setFieldVertex("#f78a38", M1Env::HLCLR_SIID);\n' # light orange
    l_plato_code +=   '    l_republic_highlight_cat->linkTo(l_republic_highlight_cat_1, "OWNS_");\n\n'

    l_plato_code += (f'    M1Store::Item_lv2* l_republic_highlight_cat_2 = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,\n' +
                     f'        // label\n' +
                     f'        "Category 2",\n' +
                     f'        // Special Item flag\n' +
                     f'        M1Env::SI_IS_TYPE | M1Env::SI_REQUIRES_EDGE,\n' +
                     f'        // mnemonic\n' +
                     f'        "RCTG2",\n' +
                     f'        // icon path\n' +
                     f'        nullptr);\n')
    l_plato_code +=   '    l_republic_highlight_cat_2->setType("TYPE_");\n'
    l_plato_code +=   '    l_republic_highlight_cat_2->setType("TXHLC");\n'
    l_plato_code +=  f'    l_republic_highlight_cat_2->setFieldVertex("#3883f7", M1Env::HLCLR_SIID);\n'  # light blue
    l_plato_code +=   '    l_republic_highlight_cat->linkTo(l_republic_highlight_cat_2, "OWNS_");\n\n'

    l_section_list = l_republic["Sections"]
    l_plato_code += f'    M1Store::Item_lv2* l_stephanus_array[{len(l_section_list)}];\n'
    l_stephanus_2_id = dict()
    l_stephanus_id = 0
    for l_stephanus in l_section_list:
        l_stephanus_2_id[l_stephanus] = l_stephanus_id
        l_plato_code += (f'    // creation of section <{l_stephanus}>\n' +
                         f'    qCDebug(g_cat_silence) << QString("Creating section <{l_stephanus}>");\n' +
                         f'    l_stephanus_array[{l_stephanus_id}] = M1Store::Item_lv2::getNew(\n' +
                         f'        // vertex flags\n' +
                         f'        M1Env::FULL_VERTEX,\n' +
                         f'        // label\n' +
                         f'        "{l_stephanus}");\n')
        l_plato_code +=  f'    l_stephanus_array[{l_stephanus_id}]->setType("STPSC");\n\n'
        l_stephanus_id += 1

    # Occurences
    l_plato_code += (f'    // creation of Republic versions node\n' +
                     f'    qCDebug(g_cat_silence) << QString("Creating Republic versions node");\n' +
                     f'    M1Store::Item_lv2* l_republic_versions = M1Store::Item_lv2::getNew(\n' +
                     f'        // vertex flags\n' +
                     f'        M1Env::FULL_VERTEX,\n' +
                     f'        // label\n' +
                     f'        "Versions");\n')
    l_plato_code +=   '    l_republic_versions->setType("FOLDR");\n'
    l_plato_code +=   '    l_republic->linkTo(l_republic_versions, "OWNS_");\n\n'

    l_plato_code +=   '    M1Store::Item_lv2* l_cur_occ = nullptr;\n'
    l_plato_code +=   '    M1Store::Item_lv2* l_cur_ver_occ = nullptr;\n'
    l_plato_code +=   '    M1Store::Item_lv2* l_cur_book_sentence = nullptr;\n'
    l_plato_code +=   '    M1Store::Item_lv2* l_cur_sentence_occ = nullptr;\n'
    l_plato_code +=   '    M1Store::Item_lv2* l_cur_sentence = nullptr;\n'
    for l_version in l_republic['Versions'].keys():
        l_mnemo_ver = f'RV{re.sub("[aeiou]", "", l_version).upper()}'[:5]
        l_plato_code += (f'    // creation of Republic version {l_version} node\n' +
                         f'    qCDebug(g_cat_silence) << QString("Creating Republic version {l_version} node");\n' +
                         f'    M1Store::Item_lv2* l_version_{l_version.lower()} = M1Store::Item_lv2::getNew(\n' +
                         f'        // vertex flags\n' +
                         f'        M1Env::FULL_VERTEX | M1Env::IS_SPECIAL,\n' +
                         f'        // label\n' +
                         f'        "{l_version} v.",\n' +
                         f'        // Special Item flags\n' +
                         f'        SI_IS_TYPE  | M1Env::SI_REQUIRES_EDGE,\n' +
                         f'        // mnemonic\n' +
                         f'        "{l_mnemo_ver}",\n' +
                         f'        // icon path\n' +
                         f'        FOLDER_ICON_PATH);\n')
        l_plato_code +=  f'    l_version_{l_version.lower()}->setType("TYPE_");\n'
        l_plato_code +=  f'    l_version_{l_version.lower()}->setType("TXTVR");\n'
        l_plato_code +=  f'    l_republic_versions->linkTo(l_version_{l_version.lower()}, "OWNS_");\n\n'

        l_plato_code += '    l_cur_ver_occ = nullptr;\n'
        # l_plato_code += '    l_cur_book_sentence = nullptr;\n'
        l_plato_code += '    l_cur_sentence_occ = nullptr;\n'
        l_plato_code += '    l_cur_sentence = nullptr;\n'

        l_cur_bknum = 0
        l_cur_sent = 0
        for l_occ_key in sorted(l_republic['Versions'][l_version].keys()):
            l_occ = l_republic['Versions'][l_version][l_occ_key]
            l_form_key = l_occ['FormKey']
            l_form_id = l_form_key_2_id[l_form_key]
            l_plato_code += (f'    // creation of Republic occurrence {l_occ_key} node\n' +
                             f'    qCDebug(g_cat_silence) << QString("Creating Republic occurrence {l_occ_key} node");\n' +
                             f'    l_cur_occ = l_republic->linkTo(l_form_array[{l_form_id}]->item_id(), "OCCUR", l_cur_occ, false);\n')
            l_plato_code +=  f'    l_cur_occ->linkTo(l_cur_occ, "AUTO_");\n'
            l_plato_code +=  f'    l_cur_occ->setText_lv1("{l_occ_key[:15]}");\n'
            l_plato_code +=  f'    l_cur_occ->setType("{l_mnemo_ver}");\n'
            l_plato_code +=  f'    l_form_array[{l_form_id}]->linkTo(l_cur_occ, "OWNS_");\n'
            # setTarget
            # l_plato_code += f'    l_cur_occ->setTarget(l_form_array[{l_form_id}]->item_id());\n'
            l_occ_text = l_occ['Text']
            if re.search('[A-Z]', l_occ_text):
                l_plato_code +=  f'    l_cur_occ->setFieldEdge("true", M1Env::CAPTL_SIID);\n'
            l_occ_pctl = l_occ['PunctLeft']
            if len(l_occ_pctl) > 0:
                l_plato_code += f'    l_cur_occ->setFieldEdge("{l_occ_pctl}", M1Env::PCTLF_SIID);\n'
            l_occ_pctr = l_occ['PunctRight']
            if len(l_occ_pctr) > 0:
                l_plato_code += f'    l_cur_occ->setFieldEdge("{l_occ_pctr}", M1Env::PCTRT_SIID);\n'
            l_occ_section = l_occ['NewSection']
            if len(l_occ_section) > 0:
                l_plato_code += f'    l_stephanus_array[{l_stephanus_2_id[l_occ_section]}]->linkTo(l_cur_occ, "OWNS_");\n'

            l_occ_pos = l_occ['Pos']
            if len(l_occ_pos) > 0:
                l_mn_pos = l_pos_2_mn[l_occ_pos]
                l_plato_code +=  f'    l_cur_occ->setType("{l_mn_pos}");\n'
            l_occ_tag = l_occ['Tag']
            if len(l_occ_tag) > 0:
                try:
                    l_mn_tag = l_tag_2_mn[l_occ_tag]
                    l_plato_code +=  f'    l_cur_occ->setType("{l_mn_tag}");\n'
                except KeyError:
                    pass
            l_occ_grammar = l_occ['Grammar']
            if l_occ_grammar:
                print(l_occ_grammar)
                for l_attr_name, l_attr_value in l_occ_grammar.items():
                    l_attr_key = f'{l_attr_name}-{l_attr_value}'
                    l_plato_code += f'    l_cur_occ->setType("{l_attr_2_mn[l_attr_key]}");\n'

            # toto = dict()
            # print(toto.items())
            l_occ_bknum = int(l_occ['BookNumber'])
            if l_occ_bknum > 0:
                l_cur_bknum = l_occ_bknum
            l_occ_spos = l_occ['SentencePos']
            if len(l_occ_spos) > 0:
                l_plato_code += f'    l_cur_occ->setFieldEdge("{l_occ_spos}", M1Env::STPOS_SIID);\n'
            if l_occ_spos == 'SS':
                l_sentence_name = f'{l_version} - Sentence {l_cur_sent + 1}'
                l_cur_sent += 1
                l_plato_code += (f'    // creation of Republic sentence <{l_sentence_name}> node\n' +
                                 f'    qCDebug(g_cat_silence) << QString("Creating Republic sentence <{l_sentence_name}> node");\n' +
                                 f'    l_cur_sentence = M1Store::Item_lv2::getNew(\n' +
                                 f'        // vertex flags\n' +
                                 f'        M1Env::FULL_VERTEX,\n' +
                                 f'        // label\n' +
                                 f'        "{l_sentence_name}");\n')
                l_plato_code += f'    l_cur_sentence->setType("TXSNT");\n'
                l_plato_code += f'    l_cur_sentence->setType("{l_mnemo_ver}");\n'
                l_plato_code += f'    l_cur_book_sentence = l_book[{l_cur_bknum - 1}]->linkTo(l_cur_sentence, "OWNS_", l_cur_book_sentence, false);\n'
                l_plato_code +=  '    l_cur_sentence_occ = nullptr;\n'

            # l_plato_code += f'    l_cur_ver_occ = l_version_{l_version.lower()}->linkTo(l_cur_occ, "OWNS_", l_cur_ver_occ, false);\n'
            l_plato_code += f'    l_cur_sentence_occ = l_cur_sentence->linkTo(l_cur_occ, "OWNS_", l_cur_sentence_occ, false);\n'

            l_plato_code += "\n"

    # QList<M1Env::SpecialItemID> cm_gram_attr_list;
    l_declaration += 'QList<M1Env::SpecialItemID> M1Env::GraphInit::cm_gram_attr_list;\n'

    for l_ssid in l_attr_class_ssid_list:
        l_initialization += f'    M1Env::GraphInit::cm_gram_attr_list.append(M1Env::{l_ssid});\n'

    with open('m1_src/m1B_graph_init.cpp', 'w') as l_fout:
        l_fout.write(g_implementation_template
                     .replace('__PSEUDO_DECLARE__', l_declaration.strip())
                     .replace('__PSEUDO_DUMP__', l_dump.strip())
                     .replace('__PSEUDO_INIT__', l_initialization.strip())
                     .replace('__INIT_BASE__', l_base_code.strip())
                     .replace('__INIT_PLATO__', l_plato_code.strip()))

    print('\n================ Duplicate Mnemonics =====================')
    for l_count, l_mn in sorted([(l_mnemo_dict[l_mn], l_mn) for l_mn in l_mnemo_dict.keys()]):
        if l_count > 1:
            print(l_mn, l_count)
