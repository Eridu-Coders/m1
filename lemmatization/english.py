#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import re
import psycopg2
import sys
import string
import json
import unicodedata
import copy

# for English
import spacy

# for Ancient Greek
import stanza

# DB connection parameters
g_dbServer = 'localhost'
g_dbDatabase = 'Perseus'
g_dbUser = 'postgres'
g_dbPassword = 'murugan!'

# headers for en and grc, respectively
g_header_line_en = '------+------------------------+------------+----------+----------------------+------------+-----------------+--------+-------+-------+----------+------------------------------------------+---------------------------------------'
g_header_titl_en = ' sec  | word                   | POS CG     | POS FG   | lemma                | Dep        | Father          | i      | end   | start | Type     | Note                                     | Morphology'

g_header_line_gr = '------+--------+-----------------+-------+-----------------+------------+------------+-----------------+---------------------------------------'
g_header_titl_gr = ' sec  | idx    | word            | POS   | lemma           | xpos       | Dep        | Father          | Morphology'

# spaCy for English, Stanza (Stanford NLP) for Greek
g_nlp_en = spacy.load('en_core_web_trf')
g_nlp_gr = stanza.Pipeline('grc')

# monitoring collections
g_lemma_form_dict = dict()
g_anno = dict()
g_hyphens = []
g_morph_attr = dict()
g_xpos_dict = dict()
g_punctuation_groups = dict()
g_punctuation_group_context = dict()
g_dep_to_pos = dict()
g_raw_text_without_notes_or_markup = ''
g_punctuation_set = set()
g_pos_to_lemma = dict()
g_tag_to_form = dict()
g_sections_set = set()

# output collections
g_occurences = dict()       # word occurrences. Key = XXX-nnn, with XXX = 'SHR' (Shorey), 'JWT' (Jowett) or 'GRC' (Greek)
g_form_dict = dict()        # word forms. Key = <form>‣POS, with POS = tag_ for Spacy (English) and upos for Stanza (Greek)
g_lemma_dict = dict()       # lemmas. Key = <lemma>‣POS, with POS = pos_ for Spacy (English) and upos for Stanza (Greek)
g_entities_dict = dict()    # named entities. Key = 'LOC', 'NORP' or 'PERSON'
g_notes_dict = dict()       # notes (from Shorey). Key = SHR-nnn, same as occurrence key of same toke

g_base_tokens = []

g_occur_restricted = dict()
g_form_restricted = dict()
g_lemma_restricted = dict()

# complete text (either in English or Greek)
g_republic_txt = ''
g_rt_list = []

# dictionaries for grammar values normalization (created when CLTK was used. No longer used after move to Stanza for Greek)
g_grammar_keys_dict = {
    'Aspect': 'Aspect',
    'Case': 'Case',
    'ConjType': 'ConjType',
    'Definite': 'Definiteness',
    'Degree': 'Degree',
    'Foreign': 'Foreign',
    'Gender': 'Gender',
    'Mood': 'Mood',
    'NumType': 'NumType',
    'Number': 'Number',
    'Person': 'Person',
    'Polarity': 'Polarity',
    'Poss': 'Possessive',
    'PronType': 'PronominalType',
    'PunctSide': 'PunctSide',
    'PunctType': 'PunctType',
    'Reflex': 'Reflexive',
    'Tense': 'Tense',
    'VerbForm': 'VerbForm',
    'VerbType': 'VerbType',
}

g_grammar_values_dict = {
    'Aspect-Perf': 'perfective',
    'Case-Acc': 'accusative',
    'ConjType-Cmp': 'comparative',
    'Definite-Def': 'definite',
    'Degree-Cmp': 'comparative',
    'Gender-Fem': 'feminine',
    'Mood-Ind': 'indicative',
    'NumType-Card': 'cardinal',
    'Number-Plur': 'plural',
    'Person-1': 'first',
    'Polarity-Neg': 'neg',
    'Poss-Yes': 'pos',
    'PronType-Art': 'article',
    'PunctSide-Fin': 'final',
    'Tense-Past': 'past',
    'VerbForm-Fin': 'finite',
    'Case-Nom': 'nominative',
    'Definite-Ind': 'indefinite',
    'Degree-Pos': 'positive',
    'Gender-Masc': 'masculine',
    'Number-Sing': 'singular',
    'Person-2': 'second',
    'PronType-Dem': 'demonstrative',
    'PunctSide-Ini': 'initial',
    'Tense-Pres': 'present',
    'VerbForm-Ger': 'gerundive',
    'Degree-Sup': 'superlative',
    'Gender-Neut': 'neuter',
    'Person-3': 'third',
    'VerbForm-Inf': 'infinitive',
    'PronType-Prs': 'personal',
    'VerbForm-Part': 'participle',
    'PronType-Rel': 'relative',
}

g_rep_chunk_list = [
    ('cry, passed like smoke beneath the earth.’ And,— ‘As bats in hollow of mystic cavern, whenever',
     'cry, passed like smoke beneath the earth.’ And, ‘As bats in hollow of mystic cavern, whenever'),
    ('we recognise them wherever they are found: True— Or, as we recognise the reflection of letters',
     'we recognise them wherever they are found: True. Or, as we recognise the reflection of letters'),
    ('cry, passed like smoke beneath the earth .’ And,— ‘As bats in hollow of mystic cavern, whenever',
     'cry, passed like smoke beneath the earth .’ And, ‘As bats in hollow of mystic cavern, whenever'),
    ('the path of vice with the words of Hesiod;— ‘Vice may be had in abundance without trouble;',
     'the path of vice with the words of Hesiod; ‘Vice may be had in abundance without trouble;'),
    ('lots, but to whom he gives unmixed evil— Hunger devouring drives him, a wanderer',
     'lots, but to whom he gives unmixed evil, Hunger devouring drives him, a wanderer'),
    ('en I beheld what I had never seen before—Thrasymachus blushing. But when we did ',
     'en I beheld what I had never seen before, Thrasymachus blushing. But when we did '),
    ('shed yourselves in the battle of Megara —’Sons of Ariston, whose race from a glo',
     'shed yourselves in the battle of Megara, Sons of Ariston, whose race from a glo'),
    ('whomsoever Zeus gives of both commingled— Now upon evil he chances and now again',
     'whomsoever Zeus gives of both commingled. Now upon evil he chances and now again'),
    ('it our youth to hear what Aeschylus says— A god implants the guilty cause in men',
     'it our youth to hear what Aeschylus says: A god implants the guilty cause in men'),
    ('retold the happy fortunes of her issue’ — Their days prolonged, from pain and si',
     'retold the happy fortunes of her issue’. Their days prolonged, from pain and si'),
    ('s turn downwards the vision of the soul —If, I say, freed from these, it had suf',
     's turn downwards the vision of the soul. If, I say, freed from these, it had suf'),
    ('he poets call the ‘threshold of old age’—Is life harder towards the end, or what',
     'he poets call the ‘threshold of old age’. Is life harder towards the end, or what'),
    ('ing to draw him out, that he might go on— Yes, Cephalus, I said: but I rather su',
     'ing to draw him out, that he might go on. Yes, Cephalus, I said: but I rather su'),
    ('obey them is justice. Yes, Polemarchus, —Thrasymachus said that for subjects to ',
     'obey them is justice. Yes, Polemarchus, Thrasymachus said that for subjects to '),
    ('path of vice with the words of Hesiod; — ‘Vice may be had in abundance without t',
     'path of vice with the words of Hesiod; ‘Vice may be had in abundance without t'),
    ('e greater enquiry which is our final end—How do justice and injustice grow up in',
     'e greater enquiry which is our final end. How do justice and injustice grow up in'),
    ('ut no mind at all !’ Again of Tiresias: — ‘(To him even after death did Persepho',
     'ut no mind at all !’ Again of Tiresias: ‘(To him even after death did Persepho'),
    ('ut no mind at all !’ Again of Tiresias: — ‘ that he alone should be wise; but th',
     'ut no mind at all !’ Again of Tiresias: ‘that he alone should be wise; but th'),
    ('ed like smoke beneath the earth .’ And, — ‘As bats in hollow of mystic cavern, w',
     'ed like smoke beneath the earth .’ And, ‘As bats in hollow of mystic cavern, w'),
    ('ing names which describe the world below—Cocytus and Styx, ghosts under the eart',
     'ing names which describe the world below, Cocytus and Styx, ghosts under the eart'),
    ('nise them wherever they are found : True— Or, as we recognise the reflection of ',
     'nise them wherever they are found : True. Or, as we recognise the reflection of '),
    ('giving us the knowledge of both: Exactly— Even so, as I maintain, neither we nor',
     'giving us the knowledge of both: Exactly. Even so, as I maintain, neither we nor'),
    ('elative terms, having clearly a relation— Yes, thirst is relative to drink. And ',
     'elative terms, having clearly a relation. Yes, thirst is relative to drink. And '),
    ('ent, ought to perform the same actions?’—What defence will you make for us, my g',
     'nt, ought to perform the same actions?’ What defence will you make for us, my g'),
    ('re to be held by women as well as by men— Yes— The proper officers will take the',
     're to be held by women as well as by men; Yes. The proper officers will take the'),
    ('nce and wisdom and reason are the judges— The only inference possible, he replie',
     'nce and wisdom and reason are the judges. The only inference possible, he replie'),
    ('h their voices the harmony of the sirens—Lachesis singing of the past, Clotho of',
     'h their voices the harmony of the sirens, Lachesis singing of the past, Clotho of'),
    ('; the responsibility is with the chooser—God is justified.’ When the Interpreter',
     '; the responsibility is with the chooser, God is justified.’ When the Interpreter')
]

def remove_accents(input_str):
    """
    Normalize the string to decompose combined characters and then removes diacritics (for Greek here but would work on any diacritic)

    :param input_str: the string to clean of accents
    :return: the cleaned-up string
    """
    l_normalized_str = unicodedata.normalize('NFD', input_str)
    # Eliminate 'Nonspacing Marks', which include diacritics
    return ''.join(c for c in l_normalized_str if unicodedata.category(c) != 'Mn')

def morph_standardize(p_pos, p_txt, p_morph, p_suspend=False):
    """
    Grammar normalization. Since move to Stanza for Greek, p_suspend always true

    :param p_pos: token POS tag (for punctuation analysis)
    :param p_txt: token text (for punctuation analysis)
    :param p_morph: morphology in Spacy form
    :param p_suspend: if True, do nothing (just transform to ordinary dict)
    :return: the normalized morphology as a dict
    """
    if p_suspend:
        l_morph_dict = p_morph
    else:
        l_morph_dict = dict()
        for l_key in p_morph.keys():
            l_key = re.sub(r'\s+', '', l_key).strip()
            v = p_morph.get(l_key)[0]
            l_norm_key = f'{l_key}-{v}'
            if l_norm_key in g_grammar_values_dict.keys():
                l_morph_dict[g_grammar_keys_dict[l_key]] = g_grammar_values_dict[l_norm_key]
            else:
                l_morph_dict[g_grammar_keys_dict[l_key]] = v.lower()

    # punctuation position (‘“ '- !),.:;?’”…
    if p_pos == 'PUNCT':
        if p_txt in '(‘“':
            l_morph_dict['PunctSide'] = 'Left'
        elif p_txt in '!),.:;?’”…':
            l_morph_dict['PunctSide'] = 'Right'
        else:
            l_morph_dict['PunctSide'] = 'None'
    return l_morph_dict

# dictionary recording the char indexes (within g_republic_txt) of the latest and previous occurrence of a given form.
# key = <form>‣POS, like in g_form_dict. See add_data()
g_last_form_key_context = dict()

def add_data_restricted(p_occ_key, p_occ_dict, p_form_key, p_form_data, p_lemma_key, p_lemma_data):
    """
    Add data to the restricted data structures

    :param p_occ_key:
    :param p_occ_dict:
    :param p_form_key:
    :param p_form_data:
    :param p_lemma_key:
    :param p_lemma_data:
    :return: nothing
    """
    global g_occur_restricted
    global g_form_restricted
    global g_lemma_restricted
    # Occurrence recording
    if p_occ_key:
        g_occur_restricted[p_occ_key] = p_occ_dict
    # Form recording
    if p_form_key:
        g_form_restricted[p_form_key] = p_form_data
    # Lemma recording
    if p_lemma_key:
        g_lemma_restricted[p_lemma_key] = p_lemma_data

def add_data(p_current_section, p_occ_key, p_occ_dict, p_form_key, p_form_data, p_lemma_key, p_lemma_data, p_test_greek_lemmas=False):
    """
    Add data to the output collections

    :param p_current_section: current Stephanus section id (for message anomalies only)
    :param p_occ_key: Occurrence key (XXX-nnn, with XXX = 'SHR' (Shorey), 'JWT' (Jowett) or 'GRC' (Greek)
    :param p_occ_dict: Occurrence dict
    :param p_form_key: form key (<form>‣POS, with POS = tag_ for Spacy (English) and upos for Stanza (Greek))
    :param p_form_data: form dict
    :param p_lemma_key: lemma key (<lemma>‣POS, with POS = pos_ for Spacy (English) and upos for Stanza (Greek))
    :param p_lemma_data: lemma dict
    :param p_test_greek_lemmas: False --> no test (used for English)
    :return: nothing
    """
    global g_occurences
    global g_form_dict
    global g_lemma_dict

    if p_form_key in g_form_dict.keys() and g_form_dict[p_form_key] != p_form_data:
        if p_test_greek_lemmas: # Greek lemma sanity test
            # lemma keys (old and new)
            _, _, l_lk_old = g_form_dict[p_form_key]
            _, _, l_lk_new = p_form_data

            # lemmas, old and new, without accents
            l_lemma_old, _ = remove_accents(l_lk_old).split('‣')
            l_lemma_new, _ = remove_accents(l_lk_new).split('‣')

            if l_lemma_old in g_greek_lemmas:
                # new data can only be worse (or equal) than old data --> no update
                p_form_key = None
                p_lemma_key = None
            elif l_lemma_old not in g_greek_lemmas and l_lemma_new in g_greek_lemmas:
                # new data better than before --> update
                pass
            else:
                # both new and old data bad --> report problem
                l_comparison_list = ' '.join([f'{a}/{b}' for a, b in zip(g_form_dict[p_form_key], p_form_data)])

                l_idx_cur, l_idx_prev = g_last_form_key_context[p_form_key]
                l_form_txt, _ = p_form_key.split('‣')
                l_anno_txt = (f'{p_current_section} {l_form_txt:15} {l_lk_old:20} {l_lk_new:20}'+
                              f'{g_republic_txt[l_idx_prev:l_idx_prev+60]:60}[{l_idx_prev:6}] '+
                              f'{g_republic_txt[l_idx_cur:l_idx_cur+60]:60}[{l_idx_cur:6}]  {l_comparison_list}')

                g_anno.setdefault('YFORM', []).append(l_anno_txt)

                # no update
                p_form_key = None
                p_lemma_key = None

    # Occurrence recording
    if p_occ_key:
        g_occurences[p_occ_key] = p_occ_dict
        g_base_tokens.append((p_occ_key, p_occ_dict))
    # Form recording
    if p_form_key:
        g_form_dict[p_form_key] = p_form_data
    # Lemma recording
    if p_lemma_key:
        # if p_lemma_key in g_lemma_dict.keys() and g_lemma_dict[p_lemma_key] != p_lemma_data:
        #     g_anno.setdefault('ZLEMM', []).append(g'{p_current_section} {p_lemma_key:20} {g_lemma_dict[p_lemma_key]}/{p_lemma_data}')
        g_lemma_dict[p_lemma_key] = p_lemma_data

def pos_reduce(p_pos):
    """
    Reduce the number of POS codes to avoid multiplying lemmas unnecessarily

    :param p_pos: POS code
    :return: reduced set POS code
    """
    if p_pos == 'AUX':
        return 'VERB'
    elif p_pos == 'DET':
        return 'PRON'
    elif p_pos == 'ADP':
        return 'SCONJ'
    return p_pos

# dictionary to correct Spacy NER mistakes
g_convert_entities = {
    "LOC-Argive": 'NORP',
    "LOC-Glaucon": 'PERSON',
    "LOC-Jupiter": 'PERSON',
    "LOC-Mars": 'PERSON',
    "LOC-Mercury": 'PERSON',
    "LOC-Saturn": 'PERSON',
    "LOC-Venus": 'PERSON',
    "PERSON-Lydian": 'NORP',
    "PERSON-Peiraeus": 'LOC',
    "PERSON-Styx": 'LOC',
    "PERSON-Troy": 'LOC'
}

def process_cursor_list_en(p_cursor_list, p_txt_key, p_phase_ii=True):
    """
    English processing function. Called for Shorey and Jowett

    This is done in two phases:
    - Phase I: construction of the note-less version of the text, while building lists of notes and Stephanus sections start indexes
    - Phase II: token processing after Spacy call on the note-less text

    :param p_cursor_list: Cursor list coming from sql request
    :param p_txt_key: 'SHR' or 'JWT'
    :param p_phase_ii: True --> perform phase II
    :return: nothing
    """
    global g_lemma_form_dict
    global g_anno
    global g_hyphens
    global g_punctuation_groups
    global g_punctuation_group_context
    global g_republic_txt
    global g_rt_list
    global g_dep_to_pos
    global g_raw_text_without_notes_or_markup
    global g_punctuation_set
    # global g_tokens_no_punctuation
    global g_pos_to_lemma
    global g_tag_to_form
    global g_sections_set

    print(f'[{p_txt_key}] English Cursor length: {len(p_cursor_list)}', file=sys.stderr)

    # ==================================================== Phase I =====================================================
    # notes dict (for pairing with tokens: position idx in text --> note)
    l_notes_dict = {}
    # Stephanus section start indexes (for pairing with tokens: position idx in text --> stephanus number)
    l_sections_dict = {}
    # position dict for [br], [i], etc (for pairing with tokens: position idx in text --> [br], [i], etc)
    l_markup_dict = {}
    # current section start position (character index) while constructing note-less text
    l_cur_section_start_idx = 0
    # full text, including notes
    l_txt_with_notes = ''
    # full text, without notes
    l_txt_no_notes = ''

    l_annotations_positions = []
    def display_annotations_with_context(p_update_dicts=False):
        """
        Displays all annotations in l_annotations_positions with context (40 char on both sides) plus
        the full text of each Stephanus section

        :param p_update_dicts: True --> updates l_notes_dict, l_sections_dict, l_markup_dict
        :return: nothing
        """
        nonlocal l_notes_dict
        nonlocal l_sections_dict
        nonlocal l_markup_dict

        l_count_annot = 0
        # construction of a dictionary giving end index for each section
        # deep copy to get an actually separate copy
        l_ap_copy_sections = copy.deepcopy(l_annotations_positions)
        # keys of the dict: all Stephanus section names
        l_next_idx_keys = [l_payload_annot for _, l_type_annot, l_payload_annot in l_ap_copy_sections if l_type_annot == 'S']
        # next indexes = indexes shifted by one position downwards (pop(0) = append at the end)
        l_next_idx_idx = [l_pos_idx for l_pos_idx, l_type_annot, _ in l_ap_copy_sections if l_type_annot == 'S']
        l_next_idx_idx.pop(0)
        # end index of the last section = length of the text
        l_next_idx_idx.append(len(l_republic_txt))
        # dict construction
        l_next_idx_dict = dict(zip(l_next_idx_keys, l_next_idx_idx))
        for l_pos_idx, l_type_annot, l_payload_annot in sorted(l_annotations_positions, key=lambda l_t: f'{l_t[1]}-{l_t[0]:08}'):
            if p_update_dicts:
                if l_type_annot == 'N':
                    l_notes_dict[l_pos_idx] = l_payload_annot
                elif l_type_annot == 'S':
                    l_sections_dict[l_pos_idx] = l_payload_annot
                elif l_type_annot == 'M':
                    l_markup_dict[l_pos_idx] = l_payload_annot

            # context boundaries
            l_start_context = l_pos_idx-40
            l_start_context = 0 if l_start_context < 0 else l_start_context
            l_end_context = l_pos_idx + 40
            l_end_context = len(l_republic_txt) if l_end_context > len(l_republic_txt) else l_end_context
            # context string, with | at the position of the annotation
            l_before_after = f'{l_republic_txt[l_start_context:l_pos_idx]:40}|{l_republic_txt[l_pos_idx:l_end_context]:40}'
            # ALERT if the | does not haver a space on one side
            l_anno_mark = '*' if re.search(r'\S\|\S', l_before_after) else ' '
            # full text of a section if this is one
            l_full_section = f' [{l_republic_txt[l_pos_idx:l_next_idx_dict[l_payload_annot]]}]' if l_type_annot == 'S' else ''
            print(f'[{l_count_annot:5}] {l_pos_idx:6} {l_type_annot} {l_anno_mark} {l_before_after} {l_payload_annot}{l_full_section}')
            l_count_annot += 1
        print()

    def display_annotations_and_update_dicts():
        display_annotations_with_context(p_update_dicts=True)

    def replace_one_by_one(p_comment, p_func, p_txt):
        """
        Execute a regex replacement function (p_func) repeatedly, until no more matches are found

        The regex in p_func must have count=1

        :param p_comment: Information to be displayed in the printout
        :param p_func: The replacement function (a lambda)
        :param p_txt: the text to act on
        :return: the resulting text
        """
        l_replaced_text = p_txt
        l_finis = False
        print(f'--------------------------------- [{p_comment}] -----------------------------------')
        while not l_finis:
            l_new_text = p_func(l_replaced_text)
            l_finis = (l_new_text == l_replaced_text)
            l_replaced_text = l_new_text
        return l_replaced_text

    if p_cursor_list:
        g_raw_text_without_notes_or_markup += f'[{p_txt_key}] '
        # cleanup of the sections data and extraction of notes
        for l_id, l_book, l_section, l_en in p_cursor_list:
            print(f'------------------------------------{l_book} {l_section}------------------------------------')
            print(l_en)

            # record section number
            g_sections_set.add(l_section)

            # References to classical works (creation of additional notes)
            for l_wkn in [r'Simonides, Fr\.',
                          r'Aesch\. Frag\.',
                          r'Aesch\. Seven',
                          r'Aesch\.',
                          r'Hes\. WD',
                          r'Hom\. Od\.',
                          r'Hom\. Il\.',
                          r'Eur\. Orest\.',
                          r'Hdt\.']:
                l_en = re.sub(f'\s+({l_wkn} \d+\.\d+-\d+(?:\s*ff\.)?)', r'{{\1}}', l_en)
                l_en = re.sub(f'\s+({l_wkn} \d+-\d+(?:\s*ff\.)?)', r'{{\1}}', l_en)
                l_en = re.sub(f'\s+({l_wkn} \d+\.\d+(?:\s*ff\.)?)', r'{{\1}}', l_en)
                l_en = re.sub(f'\s+({l_wkn} \d+(?:\s*ff\.)?)', r'{{\1}}', l_en)
            l_en = re.sub(r'\s+(Pindar Frag\. \d+, Loeb)', r'{{\1}}', l_en)
            l_en = re.sub(r'\s+(Aesch\. Niobe Fr\.)', r'{{\1}}', l_en)
            # , Aesch.
            l_en = l_en.replace('stream. Aesch.', 'stream.')
            l_en = l_en.replace('house, Aesch.', 'house,')

            # their—the rulers'—advantage
            l_en = re.sub(r"their—the rulers'—advantage", r"their (the rulers') advantage", l_en)
            # thither they neither look (Jowett)
            l_en = re.sub(r"thither they neither look", r"towards which they neither look", l_en)

            # obsolete English words
            l_en = re.sub(r"(\W+)o'er(\W+)", r'\1over\2', l_en)
            l_en = re.sub(r"(\W+)forsooth(\W+)", r'\1indeed\2', l_en)
            l_en = re.sub(r"(\W+)fain(\W+)", r'\1gladly\2', l_en)
            l_en = re.sub(r"(\W+)[’']tis(\W+)", r'\1it is\2', l_en)
            l_en = re.sub(r"(\W+)[’']Tis(\W+)", r'\1It is\2', l_en)
            l_en = re.sub(r"(\W+)Liefer(\W+)", r'\1even\2', l_en)
            l_en = re.sub(r"(\W+)e[’']en(\W+)", r'\1even\2', l_en)
            l_en = re.sub(r"(\W+)ere(\W+)", r'\1until\2', l_en)
            l_en = re.sub(r"(\W+)viand(\W+)", r'\1food\2', l_en)
            l_en = re.sub(r"(\W+)must\s+needs(\W+)", r'\1must\2', l_en)
            l_en = re.sub(r"(\W+)o[’']er(\W+)", r'\1over\2', l_en)
            l_en = l_en.replace("said I.", 'I said.')

            # spelling mistakes
            l_en = l_en.replace('Laden with', 'laden with')
            l_en = l_en.replace('/I', 'I')
            l_en = l_en.replace('WeIl , why', 'Why')

            # final cleanup (normalize all space sequences to a single <space>
            l_en = re.sub(r'\s+', ' ', l_en).strip()

            l_txt_with_notes += l_en + ' '
            print('l_txt_with_notes, count of {{:', len(re.findall(r'\{\{', l_txt_with_notes)))

            # notes elimination and processing, called from re.sub() below
            def process_note_or_markup(p_match: re.Match):
                """
                Processing of a regex match for a note or [br], [i], [/i] (called from re.sub() below)

                The substitution string (return value) is ' ' if the match had preceding or trailing spaces, or if it was a [br]

                :param p_match: the match object
                :return: the substitution string
                """
                print(f'  {p_match.start():6} {p_match.group()}')
                l_match_txt_ws = p_match.group()
                l_match_txt = l_match_txt_ws.strip()
                # count spaces on the left of the match (if any)
                l_left_spaces = len(re.match('^\s*', l_match_txt_ws).group(0))
                l_surrounding_spaces = (len(l_match_txt_ws) > len(l_match_txt))
                if l_match_txt[:2] == '{{':
                    # text cleanup (separators removal and replacement of underscores by spaces)
                    l_payload_txt = re.sub(r'\{\{', '', l_match_txt).replace('}}', '').replace('_', ' ').strip()
                    l_payload_type = 'N'
                    l_replace = ' ' if l_surrounding_spaces else ''
                else:
                    l_payload_txt = l_match_txt
                    l_payload_type = 'M'
                    l_replace = ' ' if l_payload_txt == '[br]' or l_surrounding_spaces else ''
                # calculation of note index within  l_republic_txt. If a note, will correspond to the END of a token (notes are placed AFTER a word)
                # left spaces are added to the start index (case of an [i])
                l_start_idx = l_cur_section_start_idx + p_match.start() + l_left_spaces
                # record note start
                l_annotations_positions.append((l_start_idx, l_payload_type, l_payload_txt))
                return l_replace

            # record section start
            l_annotations_positions.append((l_cur_section_start_idx, 'S', l_section))

            # notes elimination and storage
            l_section_txt_no_notes = replace_one_by_one('Notes Elimination',
                                                        lambda p_txt: re.sub(r'(?:\s*\{\{.*?}}\s*)|(?:\s*\[(?:br|i|/i)]\s*)',
                                                                             process_note_or_markup,
                                                                             p_txt, count=1),
                                                        l_en)

            # turn remaining square brackets into round ones
            l_section_txt_no_notes = l_section_txt_no_notes.replace('[', '(').strip()
            l_section_txt_no_notes = l_section_txt_no_notes.replace(']', ')').strip()

            # accumulate this section within l_txt_no_notes (and g_raw_text_without_notes_or_markup)
            l_txt_no_notes += l_section_txt_no_notes + ' '
            g_raw_text_without_notes_or_markup += l_section_txt_no_notes + ' '

            # start of next section
            l_cur_section_start_idx = len(l_txt_no_notes)

    # eliminate final space
    l_txt_with_notes = l_txt_with_notes.strip()
    l_txt_no_notes = l_txt_no_notes.strip()
    g_raw_text_without_notes_or_markup = g_raw_text_without_notes_or_markup.strip() + '\n'

    # note-less text in construction
    l_republic_txt = l_txt_no_notes

    print('------------------- Before complex regexes -------------------------------')
    display_annotations_with_context()

    def preserve_annotation_positions(p_match:re.Match, p_replace, p_large=False):
        """
        Function called from re.sub() to adjust the positions of annotations in case the replacement string is of a different
        length than the original

        CAUTION: will only work if substitutions done ONE AT A TIME --> re.sub(..., count=1)

        Special care must be paid to cases when annotations exist "inside" the original string

        :param p_match: Match object generated by re.sub()
        :param p_replace: replacement string
        :param p_large: true --> applies special strategy to cases of annotation inside match
        :return: p_replace_or_modified_segment
        """
        # modifies l_annotations_positions from the outer scope
        nonlocal l_annotations_positions

        # start / end indexes of the match
        l_match_start_idx = p_match.start()
        l_match_end_idx = p_match.end()
        # original string to be replaced by p_replace_or_modified_segment
        l_original = p_match.group(0)
        l_txt_searched = p_match.string

        # indexes for context (for printout)
        l_start_context = l_match_start_idx - 40
        l_start_context = 0 if l_start_context < 0 else l_start_context
        # p_match_end_idx = p_match_start_idx + len(p_original)
        l_end_context = l_match_end_idx + 40
        l_end_context = len(l_txt_searched) if l_end_context > len(l_txt_searched) else l_end_context

        l_context_before = f'∥{l_txt_searched[l_start_context:l_match_start_idx]}∥{l_original}∥{l_txt_searched[l_match_end_idx:l_end_context]}∥'
        l_context_after = f'∥{l_txt_searched[l_start_context:l_match_start_idx]}∥{p_replace}∥{l_txt_searched[l_match_end_idx:l_end_context]}∥'

        # l_replace_or_modified_segment = p_replace
        # determine modified segment inside large replacements
        def modified_segment(p_large_flag, p_original, p_replace_or_modified_segment, p_match_start_idx, p_match_end_idx):
            if p_large_flag:
                l_left_same_last = 0
                for l_idx in range(0, min(len(p_replace_or_modified_segment), len(p_original))):
                    if p_replace_or_modified_segment[l_idx] == p_original[l_idx]:
                        l_left_same_last = l_idx
                    else:
                        break
                l_right_same_first = 0
                for l_idx in range(-1, -min(len(p_replace_or_modified_segment), len(p_original)), -1):
                    if p_replace_or_modified_segment[l_idx] == p_original[l_idx]:
                        l_right_same_first = l_idx
                    else:
                        break
                return (p_replace_or_modified_segment[l_left_same_last:l_right_same_first + 1],
                        p_match_start_idx + l_left_same_last,
                        p_match_end_idx + l_right_same_first)
            else:
                return p_replace_or_modified_segment, p_match_start_idx, p_match_end_idx
            # p_match_start_idx += l_left_same_last
            # p_match_end_idx += l_right_same_first + 1  # NB: l_right_same_first is a NEGATIVE number

        def place_inside(p_large_flag, p_replace_or_modified_segment, p_pos_idx, p_match_start_idx, p_type_annot, p_payload_annot, p_len_indent):
            nonlocal l_new_ap
            l_pos_inside = p_pos_idx - p_match_start_idx

            l_mtc = None
            if re.match(r'^\S+\s+\S+$', p_replace_or_modified_segment):  # the replacement string must be of the form "xxxxx yyyyy"
                if p_type_annot == 'S' or p_payload_annot == '[i]':
                    l_mtc = re.search(r'^(\S+\s+\S)\S*$', p_replace_or_modified_segment)
                else:
                    l_mtc = re.search(r'^(\S+)', p_replace_or_modified_segment)

            if l_mtc:
                l_new_pos_inside = len(l_mtc.group(1)) - 1
                l_new_ap.append((p_match_start_idx + l_new_pos_inside, p_type_annot, p_payload_annot))
                print(f'{" "*p_len_indent} {p_pos_idx:,d} ' +
                      f'{"L" if p_large_flag else ""}"{p_replace_or_modified_segment[:20]}{"..." if len(p_replace_or_modified_segment) > 20 else ""}" ' +
                      f'[{l_mtc.group(0)}⏊{l_mtc.group(1)}] ' +
                      f'{l_pos_inside}➵{l_new_pos_inside} [{p_type_annot}-{p_payload_annot}]')
            else:
                # no slide otherwise
                l_new_ap.append((p_pos_idx, p_type_annot, p_payload_annot))

        # the new l_annotations_positions list under construction
        l_new_ap = []

        l_first_line = f'{l_match_start_idx:7,} [{l_match_end_idx - l_match_start_idx:5,}] {l_match_end_idx:7,}'
        print(f'{l_first_line} {l_context_before}')
        print(f'{" " * len(l_first_line)} {l_context_after}')

        # only something to do if original and replacement of different lengths
        if len(p_replace) == len(l_original):
            # equal length, no position adjustments needed
            if p_replace == l_original:
                # nothing to do
                l_new_ap = copy.deepcopy(l_annotations_positions)
            else:
                l_replace_or_modified_segment, l_match_start_idx, l_match_end_idx = modified_segment(p_large,
                                                                                                     l_original,
                                                                                                     p_replace,
                                                                                                     l_match_start_idx,
                                                                                                     l_match_end_idx)
                # Check for possible annotation shifts inside changed segment
                for l_pos_idx, l_type_annot, l_payload_annot in l_annotations_positions:
                    if l_match_start_idx <= l_pos_idx < l_match_end_idx: # Annotation is located inside match
                        place_inside(p_large, l_replace_or_modified_segment, l_pos_idx, l_match_start_idx, l_type_annot,
                                     l_payload_annot, len(l_first_line))
                    else:
                        # no slide, either before or after bc len(p_replace) == len(l_original)
                        l_new_ap.append((l_pos_idx, l_type_annot, l_payload_annot))
        else: # len(p_replace) != len(l_original):
            # amount by which the annotation must "slide" (can be positive or negative), if it is AFTER the substitution
            l_slide_amount = len(p_replace) - len(l_original)

            l_replace_or_modified_segment, l_match_start_idx, l_match_end_idx = modified_segment(p_large, l_original,
                                                                                                 p_replace,
                                                                                                 l_match_start_idx,
                                                                                                 l_match_end_idx)

            for l_pos_idx, l_type_annot, l_payload_annot in l_annotations_positions:
                if l_pos_idx < l_match_start_idx:
                    # Annotation is located before match --> no change
                    l_new_ap.append((l_pos_idx, l_type_annot, l_payload_annot))
                elif l_match_end_idx <= l_pos_idx:
                    # Annotation is located after match --> slide by l_slide_amount
                    l_new_ap.append((l_pos_idx + l_slide_amount, l_type_annot, l_payload_annot))
                else:
                    # l_match_start_idx <= l_pos_idx < l_match_end_idx:
                    # Annotation is located inside match
                    place_inside(p_large, l_replace_or_modified_segment, l_pos_idx, l_match_start_idx, l_type_annot,
                                 l_payload_annot, len(l_first_line))

        l_annotations_positions = l_new_ap
        return p_replace

    # apostrophe save
    l_republic_txt = re.sub(r"([A-Za-z])'([a-z])", r'\1⏅\2', l_republic_txt)
    # dangling right possessive save
    l_republic_txt = re.sub(r"([a-z])'([\s!),.:;?’”…])", r'\1⍫\2', l_republic_txt)
    # Double quote normalization
    print('--------------------- Double quote normalization ---------------------')
    l_republic_txt = re.sub(r'["“](\s*[^"“”]+\s*)["”]',
                            lambda m: preserve_annotation_positions(m, m.expand(r'“\1”')), l_republic_txt)
    # Single quote normalization \w ,;!?—\-. ^'‘’
    print('--------------------- Single quote normalization ---------------------')
    l_republic_txt = re.sub(r"['‘](\s*[\w ,;!?—\-.]+\s*)['’]",
                            lambda m: preserve_annotation_positions(m, m.expand(r'‘\1’')), l_republic_txt)

    l_funct_list_1 = [ # Dash transformation (‘“ '- !),.:;?’”…
        ('Small dash normalization',
         lambda l_txt: re.sub("(\w)(?:\s+-|-\s+)(\w)", # Small dash normalization
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1-\2')), l_txt, count=1)),
        ('Left punctuation unwanted space elimination',
         lambda l_txt: re.sub("(\S)\s*([(‘“]+)\s+([(‘“\w])", # Left punctuation unwanted space elimination
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1 \2\3')), l_txt, count=1)),
        ('Right punctuation unwanted space elimination',
         lambda l_txt: re.sub("([!),.:;?’”…\w])\s+([!),.:;?’”…]+)\s*(\S)", # Right punctuation unwanted space elimination
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1\2 \3')), l_txt, count=1)),
        ('Double double quote elimination',
         lambda l_txt: re.sub('““', # Double double quote elimination
                              lambda m: preserve_annotation_positions(m, m.expand(r'“')), l_txt, count=1)),
        ('Single + double quote elimination',
         lambda l_txt: re.sub('“‘([^’]+)’“', # Double double quote elimination
                              lambda m: preserve_annotation_positions(m, m.expand(r'“\1“')), l_txt, count=1)),
        ('?’—',
         lambda l_txt: re.sub('(\w)\s*\?’\s*—\s*(\w)', # ?’—
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1?’, \2')), l_txt, count=1)),
        (') —',
         lambda l_txt: re.sub('(\w)\s*\)\s+—\s*(\w)', # ) —
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1), \2')), l_txt, count=1)),
        (', “—',
         lambda l_txt: re.sub('(\w)\s*,\s+“—\s*(\w)', # , “—
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1, “\2')), l_txt, count=1)),
        ('e. —A n? —W',
         lambda l_txt: re.sub(r'(\w)([.?!])\s*—\s*([‘“]*[A-Z])', # e. —A n? —W
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1\2 \3')), l_txt, count=1)),
        ('capitalize after .?!',
         lambda l_txt: re.sub(r'(\w)([.?!])\s*—\s*([a-z])', # capitalize after .?!
                              lambda m: preserve_annotation_positions(m, f'{m.group(1)}{m.group(2)} {m.group(3).upper()}'), l_txt, count=1)),
        ('d, —h',
         lambda l_txt: re.sub(r'(\w)([,;])\s*—\s*([‘“]*[a-z])', # d;— ‘V
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1\2 \3')), l_txt, count=1)),
        ('d, —I ',
         lambda l_txt: re.sub(r'(\w)([,;])\s*—\s*([‘“]*I )',
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1\2 \3')), l_txt, count=1)),
        ('y—” “I',
         lambda l_txt: re.sub(r'(\w)—”\s+“([A-Z])', # y—” “I
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1.” “\2')), l_txt, count=1)),
        ('d: —W',
         lambda l_txt: re.sub(r'(\w):\s*—\s*([‘“(]*\w)', # d: —W
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1: \2')), l_txt, count=1)),
        ('canonical dash replacement as a comma',
         lambda l_txt: re.sub(r'([\w’”])\s*—\s*([a-z‘“])', # canonical dash replacement as a comma
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1, \2')), l_txt, count=1)),
        ('special case of I',
         lambda l_txt: re.sub(r'([\w’”])\s*—\s*I ', # special case of I
                              lambda m: preserve_annotation_positions(m, m.expand(r'\1, I ')), l_txt, count=1))
    ]

    # generic punctuation adjustment regexes
    for l_comment, l_sub_func in l_funct_list_1:
        l_republic_txt = replace_one_by_one(l_comment, l_sub_func, l_republic_txt)

    def sanitize_for_re(p):
        return p.replace('.', r'\.').replace('(', r'\(').replace(')', r'\)')

    # chunk replacements for dash elimination
    for l_left_rep, l_right_rep in g_rep_chunk_list:
        l_sub_func = lambda l_txt: re.sub(re.escape(l_left_rep),
        # l_sub_func = lambda l_txt: re.sub(sanitize_for_re(l_left_rep),
                                          lambda m: preserve_annotation_positions(m, l_right_rep, p_large=True),
                                          l_txt, count=1)
        l_comment = f'{l_left_rep} --> {l_right_rep}'
        l_republic_txt = replace_one_by_one(l_comment, l_sub_func, l_republic_txt)

    # apostrophe/dangling possessive restore
    l_republic_txt = re.sub(r'[⍫⏅]', "'", l_republic_txt)

    print('------------------- After complex regexes -------------------------------')
    display_annotations_and_update_dicts()

    g_republic_txt = l_republic_txt
    g_rt_list.append(l_republic_txt)

    # print whole no-notes text
    print('[Text without notes &c.]', l_republic_txt)
    # print all notes below their insertion points (marked by '|') from l_txt_with_notes
    print('[Text with notes &c.   ]', re.sub(r'\{\{', '|\n-->{{', l_txt_with_notes))

    # ==================================================== Phase II ====================================================
    if not p_phase_ii:
        return
    print('Phase II', file=sys.stderr)

    def headers_print_en():
        """
        Headers printing (for display at start of each sentence)
        :return: nothing
        """
        print(g_header_line_en)
        print(g_header_titl_en)
        print(g_header_line_en)

    # Spacy analysis
    l_doc = g_nlp_en(l_republic_txt.strip())

    # NER Entities processing
    l_ent_i_dict = dict()
    for l_ent in l_doc.ents:
        l_type = l_ent.label_
        # only accepted NERs are those that can be reduced to LOC/NORP/PERSON
        if l_type in ['FAC', 'GPE', 'NORP', 'LOC', 'ORG', 'PERSON']:
            # reduction to LOC / NORP / PERSON
            l_type = {'FAC': 'LOC',
                      'GPE': 'LOC',
                      'LOC': 'LOC',
                      'NORP': 'NORP',
                      'ORG': 'PERSON',
                      'PERSON': 'PERSON'}[l_type]
            l_form_txt = l_ent.text
            if l_form_txt not in ['State', 'States', 'temple', 'Apollo Me', 'democrat']:
                # refuse some spurious or too common entities
                if l_type == 'NORP':
                    # removes plural for NORP adjectives
                    l_form_txt = re.sub(r's$', '', l_form_txt)
                try:
                    # corrects some mistakes
                    l_type = g_convert_entities[f'{l_type}-{l_form_txt}']
                except KeyError:
                    pass
                finally:
                    # entity key (not used in g_entities_dict, but included in the JSON file)
                    l_ent_key = f'{l_type}-{l_form_txt.replace(" ", "_")}'
                    # Start and end position (in token id within l_doc)
                    l_start_tk = l_ent.start
                    l_end_tk = l_ent.end

                    # Option 1: link entity to all its constituent tokens (no longer used)
                    # for i in range(l_start_tk, l_end_tk):
                    #     l_ent_i_dict[i] = l_ent_key

                    # Option 2: link entity to its start token only
                    l_ent_i_dict[l_start_tk] = l_ent_key
                    g_entities_dict.setdefault(l_type, dict()).setdefault(l_form_txt, []).append((
                        l_start_tk, f'{p_txt_key}-{l_start_tk}', l_end_tk , f'{p_txt_key}-{l_end_tk - 1}'))

    # current Stephanus section number (normally initialized on first token so default value reveals problem if seen)
    l_current_section = '----'
    # prefix = alternating '+' and ' ' to indicate section change, which may occur within a sentence
    l_prefix_section = ' '
    # 2 previous form keys, for anomalies messages
    l_previous_form_key = ''
    l_prev_prev_form_key = ''
    # token pipe. Middle position is current token
    l_token_neighborhood = [None, None, None]
    # punctuation pipe, for further analysis
    l_punctuation_pipe = []
    for t in l_doc:
        # determine current section based on l_sections_dict
        # l_new_current_section is set (to the value of l_current_section) on the first token of that section
        l_new_current_section = ''
        try:
            # if the current token's char id position correspond to one of the keys of l_sections_dict,
            # then it is the start token of that section
            l_current_section = l_sections_dict[t.idx]
            l_prefix_section = '+' if l_prefix_section == ' ' else ' '
            l_new_current_section = l_current_section
        except KeyError:
            pass

        # token neighborhood. Middle = current
        l_token_neighborhood[0] = l_token_neighborhood[1]
        l_token_neighborhood[1] = t
        l_token_neighborhood[2] = l_doc[t.i + 1] if t.i < l_doc.__len__() - 1 else None

        def get_tok_attr(p_token):
            """
            Get the attributes of one token

            01 l_t_cap     : token text, space normalized and stripped
            02 l_frm_txt   : form text, non-capitalized
            03 l_lem       : lemma string
            04 l_end_stc   : 'true' if sentence end, 'false' otherwise
            05 l_start_stc : 'true' if sentence start, 'false' otherwise
            06 l_tg        : Spacy tag_ value
            07 l_pos_univ  : Spacy universal POS value as reduced by pos_reduce()
            08 l_lem_key   : Lemma key : <lemma>‣POS
            09 l_frm_key   : Form key : <form>‣TAG
            10 l_fthr      : Dependency father, in string form, or '' if none
            11 l_morph_dict: grammar information as a dict, passed through morph_standardize()

            :param p_token: The token
            :return: the 11 values above
            """
            if p_token is not None:
                l_t_cap = re.sub(r'\s+', ' ', p_token.text).strip()
                l_frm_txt = l_t_cap.lower()
                l_lem = re.sub(r'\s+', ' ', p_token.lemma_).strip().lower()
                l_end_stc = 'true' if p_token.is_sent_end else 'false'
                l_start_stc = 'true' if p_token.is_sent_start else 'false'
                l_pos_univ = pos_reduce(p_token.pos_)
                l_tg = p_token.tag_
                l_lem_key = f'{l_lem}‣{l_pos_univ}'
                l_frm_key = f'{l_frm_txt}‣{l_tg}'

                g_dep_to_pos.setdefault(p_token.dep_, set()).add(l_pos_univ)

                if t.has_head():
                    l_fthr = p_token.head.text
                else:
                    l_fthr = ''

                # token mistakes corrections
                def tok_correction(p_lemma, p_lemma_key, p_pos, p_tg, p_wd_key, p_morph):
                    if p_wd_key[:4] in ['-‣VB', '-‣JJ', '-‣NN', '-‣RB']:
                        return '-', '-‣PUNCT', 'PUNCT', 'HYPH', '-‣HYPH', {'PunctType': 'Dash'}
                    if p_lemma_key == 'need‣VERB' and p_tg[:2] == 'NN':
                        return 'need', 'need‣NOUN', 'NOUN', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'comes‣VERB':
                        return 'come', 'come‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'goes‣VERB':
                        return 'go', 'go‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'said‣VERB':
                        return 'say', 'say‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'becomes‣VERB':
                        return 'become', 'become‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'having‣VERB':
                        return 'have', 'have‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'heard‣VERB':
                        return 'hear', 'hear‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'owing‣VERB':
                        return 'owe', 'owe‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'got‣VERB':
                        return 'get', 'get‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'getting‣VERB':
                        return 'get', 'get‣VERB', 'VERB', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'of‣VERB':
                        return 'of', 'of‣SCONJ', 'SCONJ', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'by‣VERB':
                        return 'by', 'by‣SCONJ', 'SCONJ', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'for‣VERB':
                        return 'for', 'for‣SCONJ', 'SCONJ', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'but‣ADV':
                        return 'but', 'but‣CCONJ', 'CCONJ', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'lest‣VERB':
                        return 'lest', 'lest‣SCONJ', 'SCONJ', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'to‣VERB':
                        return 'to', 'to‣SCONJ', 'SCONJ', p_tg, p_wd_key, p_morph
                    if p_lemma_key == 'not‣VERB':
                        return 'not', 'not‣PART', 'PART', p_tg, p_wd_key, p_morph
                    if p_lemma_key in ['‘‣VERB', '‘‣PUNCT']:
                        return '\'', '\'‣PUNCT', 'PUNCT', p_tg, p_wd_key, p_morph
                    return p_lemma, p_lemma_key, p_pos, p_tg, p_wd_key, p_morph

                # application of function above
                l_lem, l_lem_key, l_pos_univ, l_tg, l_frm_key, l_morph_dict = tok_correction(l_lem, l_lem_key, l_pos_univ, l_tg, l_frm_key, p_token.morph.to_dict())

                l_morph_dict = morph_standardize(l_pos_univ, p_token.text, l_morph_dict, p_suspend=True)

                # inventory of grammar keys and values
                for k_morph in l_morph_dict.keys():
                    # A = Spacy, M = common for both SPacy and Stanza (Z = Stanza)
                    g_morph_attr.setdefault(f'A-{k_morph}', set()).add(l_morph_dict[k_morph])
                    g_morph_attr.setdefault(f'M-{k_morph}', set()).add(l_morph_dict[k_morph])

                # return 11 values
                return (
                    l_t_cap,        # 01
                    l_frm_txt,      # 02
                    l_lem,          # 03
                    l_end_stc,      # 04
                    l_start_stc,    # 05
                    l_tg,           # 06
                    l_pos_univ,     # 07
                    l_lem_key,      # 08
                    l_frm_key,      # 09
                    l_fthr,         # 10
                    l_morph_dict    # 11
                )
            else:
                # return 11 '' if token was None
                return '', '', '', '', '', '', '', '', '', '', ''

        # get 11 values from current token
        (l_txt_cap, l_form_txt, l_lemma_txt, l_is_sent_end, l_is_sent_start, l_tag, l_pos,
         l_lemma_key, l_form_key, l_father, l_grammar_dict) = get_tok_attr(t)

        # get some values from previous and last token
        _, l_txt_p, l_lem_p, _, _, l_tag_p, _, _, l_prev_word_key,_ ,_ = get_tok_attr(l_token_neighborhood[0])
        _, l_txt_n, l_lem_n, _, _, l_tag_n, _, _, l_next_word_key,_ ,_ = get_tok_attr(l_token_neighborhood[2])

        # POS --> Lemma recording
        g_pos_to_lemma.setdefault(l_pos, set()).add(l_lemma_txt)
        g_tag_to_form.setdefault(l_tag, set()).add(l_form_txt)

        # get note, if one is attached to this token
        l_note = ''
        try:
            # notes are positioned just after the end of a token
            l_note = l_notes_dict[t.idx + len(t.text)]
            # if found, store it
            g_notes_dict[f'{p_txt_key}-{t.i}'] = l_note
        except KeyError:
            pass

        # get entities key if there is one (current token is first token of an entity)
        l_ent_key = ''
        try:
            l_ent_key = l_ent_i_dict[t.i]
        except KeyError:
            pass

        # get markup, if any
        # markup entries are consumed so as not to appear twice (case of a [/i] after a word but before a punctuation mark)
        l_markup_before = ''
        l_markup_after = ''
        try:
            l_markup_before = l_markup_dict[t.idx]
            del l_markup_dict[t.idx]
        except KeyError:
            pass
        try:
            l_markup_after = l_markup_dict[t.idx + len(t.text)]
            del l_markup_dict[t.idx + len(t.text)]
        except KeyError:
            pass

        # headers at start of sentence and header line only at end
        if t.is_sent_start:
            if t.i > 0:
                print(f'{g_header_line_en}\n')
            headers_print_en()

        # store hyphenated words (normal case) for further analysis
        if l_form_key == '-‣HYPH':
            g_hyphens.append(
                f'{l_current_section} {l_tag_p:5} {l_tag:5} {l_tag_n:5} {l_txt_p}{l_form_txt}{l_txt_n}/{l_lem_p}{l_lemma_txt}{l_lem_n}')

        # store lemma --> forms relationship for further analysis
        g_lemma_form_dict.setdefault(l_lemma_key, set()).add(l_form_key)
        # print the table row for the current token
        print(
            f'{l_prefix_section}{l_current_section:4} | {l_form_txt:22} | {l_pos:10} | {l_tag:8} | {l_lemma_txt:20} | {t.dep_:10} | {l_father:15} | {t.i:6} | ' +
            f'{l_is_sent_end:5} | {l_is_sent_start:5} | {t.ent_type_:8} | {l_note[:40]:40} | {l_grammar_dict}')

        # store data for the current occurrence, form and lemma
        add_data(l_current_section,
                 f'{p_txt_key}-{t.i:06}',
                 {
                    'Text': l_txt_cap,
                    'Pos': l_pos,
                    'Tag': l_tag,
                    'FormKey': [l_form_key],
                    'NewSection': l_new_current_section,
                    'Grammar': l_grammar_dict,
                    'EntityKey': l_ent_key,
                    'MarkupBefore': l_markup_before,
                    'MarkupAfter': l_markup_after,
                    'NoteKey': f'{p_txt_key}-{t.i}' if len(l_note) > 0 else '',
                    'SentencePos': ('SS' if t.is_sent_start else ('SE' if t.is_sent_end else ''))
                 },
                 l_form_key, {'Text': l_form_txt, 'Tag': l_tag, 'LemmaKey': l_lemma_key},
                 l_lemma_key, {'Text': l_lemma_txt, 'Pos': l_pos, 'EntType': t.ent_type_}
         )

        # detection of anomalies
        # observed types of punctuation problems (as instances of form key)
        l_punctuation_problems = ['‣_SP', '’‣PRP', "'‣PRP", "'‣VBP", '-‣JJ', '-‣NN', '-‣NNS',
                            '-‣SYM', '-‣VB', '-‣VBG', '.‣NN', '.‣NNP']
        l_anomaly_reason = ''
        l_anomaly_reason = 'NUMR' if re.search(r'\d+', l_form_txt) else l_anomaly_reason
        l_anomaly_reason = 'UNKN' if t.pos_ == 'X' and t.tag_ != 'FW' else l_anomaly_reason
        l_anomaly_reason = 'FRWD' if t.pos_ == 'X' and t.tag_ == 'FW' else l_anomaly_reason
        l_anomaly_reason = 'HYPHN' if l_form_key[:4] in ['-‣VB', '-‣JJ', '-‣NN'] else l_anomaly_reason
        l_anomaly_reason = l_form_key if l_form_key in l_punctuation_problems else l_anomaly_reason

        if l_anomaly_reason == 'HYPHN' and 0 < t.i < l_doc.__len__() - 1:
            if l_tag_p[:2] == l_tag[:2] or l_tag_n[:2] == l_tag[:2]:
                l_anomaly_reason = ''
                # also store 'abnormal' (but somewhat normal bc same tag_) hyphenated words for further analysis
                g_hyphens.append(
                    f'{l_current_section} {l_tag_p:5} {t.tag_:5} {l_tag_n:5} {l_txt_p}{l_form_txt}{l_txt_n}/{l_lem_p}{l_lemma_txt}{l_lem_n}')

        # context width
        l_idx_before = t.idx - 50
        l_idx_before = 0 if l_idx_before < 0 else l_idx_before
        l_context = f'{l_republic_txt[l_idx_before:t.idx]}|{l_republic_txt[t.idx:t.idx + 50]}'
        # anomaly message generation and storage
        if len(l_anomaly_reason) > 0:
            # anno message
            l_anno_txt = f'{l_current_section}: {l_prev_prev_form_key:20} {l_previous_form_key:20} {l_form_key:20} [{l_pos:5}] --> {l_context}'

            # anno storage, for further analysis
            g_anno.setdefault(l_anomaly_reason, []).append(l_anno_txt)

        # punctuation group analysis
        if l_pos == 'PUNCT':
            l_punctuation_pipe.append((l_form_txt, l_lemma_txt))

            if re.search(r'\w+', l_form_txt):
                l_anno_txt = f'{l_current_section}: {l_prev_prev_form_key:20} {l_previous_form_key:20} {l_form_key:20} [{l_pos:5}] {t.dep_} --> {l_context}'
                g_anno.setdefault('PUNCT', []).append(l_anno_txt)

        if (l_pos == 'PUNCT' and len(t.whitespace_) > 0) or (l_pos != 'PUNCT' and len(l_punctuation_pipe) > 0):
            l_p_list = ''.join([a for a, _ in l_punctuation_pipe])
            l_l_list = ''.join([b for _, b in l_punctuation_pipe])
            l_pgr_key = f'{l_p_list} ⇹ {l_l_list}'
            g_punctuation_groups.setdefault(len(l_p_list), set()).add(l_pgr_key)
            g_punctuation_group_context[l_pgr_key] = l_context
            if len(l_p_list) == 1:
                g_punctuation_set.add(l_p_list)
            l_punctuation_pipe = []

        # previous form key pipe update
        l_prev_prev_form_key = l_previous_form_key
        l_previous_form_key = l_form_key

    print(g_header_line_en)
    print()
    # ==================================================== Phase III ===================================================
    #                         try:
    #                             l_punt_is_right = l_tok_right['Grammar']['PunctSide'] == 'Right'
    #                         except KeyError:
    #                             print('Key error', file=sys.stderr)
    #                             print('l_tok_index:', l_tok_index, file=sys.stderr)
    #                             print('l_i_right  :', l_i_right, file=sys.stderr)
    #                             print(l_tok_right, file=sys.stderr)
    #                             sys.exit(0)

    print('Phase III', file=sys.stderr)

    class HyphenatedWordException(Exception):
        pass

    for l_tok_index in range(len(g_base_tokens)):
        l_tok_key, l_tok = g_base_tokens[l_tok_index]
        if l_tok['Pos'] != 'PUNCT':
            try: # catches the Hyphenated word cases when a hyphen is present before or after
                l_punctuation_left = '' # get punctuation on both side of non-punctuation token
                l_new_s_left = ''
                if l_tok_index > 0:
                    l_fin_left = False
                    l_i_left = l_tok_index - 1
                    if g_base_tokens[l_i_left][1]['Text'] == '-':
                        raise HyphenatedWordException # Hyphen before

                    while not l_fin_left:
                        l_tok_left = g_base_tokens[l_i_left][1]
                        if l_tok_left['Pos'] != 'PUNCT' or l_tok_left['Grammar']['PunctSide'] != 'Left':
                            l_fin_left = True
                        else:
                            l_punctuation_left = l_tok_left['Text'] + l_punctuation_left
                            l_new_s_left = l_tok_left['NewSection']
                            if l_i_left == 0:
                                l_fin_left = True
                            else:
                                l_i_left -= 1
                l_punctuation_right = ''
                if l_tok_index < len(g_base_tokens) - 1:
                    l_fin_right = False
                    l_i_right = l_tok_index + 1
                    if g_base_tokens[l_i_right][1]['Text'] == '-':
                        raise HyphenatedWordException # Hyphen after

                    while not l_fin_right:
                        l_tok_right = g_base_tokens[l_i_right][1]
                        if l_tok_right['Pos'] != 'PUNCT' or l_tok_right['Grammar']['PunctSide'] != 'Right':
                            l_fin_right = True
                        else:
                            l_punctuation_right = l_punctuation_right + l_tok_right['Text']
                            if l_i_right == len(g_base_tokens) - 1:
                                l_fin_right = True
                            else:
                                l_i_right += 1
                l_tok['PunctLeft'] = l_punctuation_left
                l_tok['PunctRight'] = l_punctuation_right
                # g_tokens_no_punctuation[l_tok_key] = l_tok
                l_form_key = l_tok['FormKey'][0]
                l_form = g_form_dict[l_form_key]
                l_lemma_key = l_form['LemmaKey']
                l_lemma = g_lemma_dict[l_lemma_key]
                if len(l_new_s_left) > 0:
                    l_tok['NewSection'] = l_new_s_left
                add_data_restricted(l_tok_key, l_tok, l_form_key, l_form, l_lemma_key, l_lemma)
            except HyphenatedWordException:
                pass # nothing to do
        elif l_tok['Text'] == '-': # Hyphenated words
            if l_tok_index == 0 or l_tok_index == len(g_base_tokens) - 1:
                print(f'Error: first or last token cannot be a hyphen [{l_tok_index}]', file=sys.stderr)
                sys.exit(0)

            def composite_value(p_v1, p_v2):
                return p_v1 if len(p_v2) == 0 else \
                            (p_v2 if len(p_v1) == 0 else
                             (p_v2 if p_v1 == p_v2 else
                              f'{p_v1}⧎{p_v2}'))

            def merge_grammar_dicts(p_d1: dict, p_d2):
                l_merged = dict()
                for l_dict_key in sorted(list(set(p_d1.keys()).union(set(p_d2.keys())))):
                    l_v1 = p_d1[l_dict_key] if l_dict_key in p_d1.keys() else ''
                    l_v2 = p_d2[l_dict_key] if l_dict_key in p_d2.keys() else ''
                    l_merged[l_dict_key] = composite_value(l_v1, l_v2)

                return l_merged

            # hyphenated words have 3 forms :
            # 1) the hyphenated word
            # 2) the left-hand half (_before below) of the hyphenated word
            # 3) the right-hand half (_after below) of the hyphenated word
            _, l_tok_before = g_base_tokens[l_tok_index-1]
            _, l_tok_after = g_base_tokens[l_tok_index+1]
            l_new_text = f"{l_tok_before['Text']}-{l_tok_after['Text']}"
            l_new_form_key = f"{l_new_text}‣{l_tok_before['Tag']}"
            l_new_tok = {
                'Text': l_new_text,
                'Pos': l_tok_before['Pos'],
                'Tag': l_tok_before['Tag'],
                'FormKey': l_tok_before['FormKey'] + l_tok_after['FormKey'] + [l_new_form_key],
                'NewSection': l_tok_before['NewSection'],
                'Grammar': merge_grammar_dicts(l_tok_before['Grammar'], l_tok_after['Grammar']),
                'EntityKey': '',
                'MarkupBefore': l_tok_before['MarkupBefore'],
                'MarkupAfter': l_tok_after['MarkupAfter'],
                'NoteKey': l_tok_before['NoteKey'] if len(l_tok_before['NoteKey']) > 0 else l_tok_after['NoteKey'],
                'SentencePos': ('SS' if l_tok_before['SentencePos'] == 'SS' else ('SE' if l_tok_after['SentencePos'] == 'SE' else ''))
            }
            l_form_key_before = l_tok_before['FormKey'][0]
            l_form_key_after = l_tok_after['FormKey'][0]
            l_form_before = g_form_dict[l_form_key_before]
            l_form_after = g_form_dict[l_form_key_after]
            l_lemma_before = g_lemma_dict[l_form_before['LemmaKey']]
            l_lemma_after = g_lemma_dict[l_form_after['LemmaKey']]
            l_new_lemma_txt = f"{l_lemma_before['Text']}-{l_lemma_after['Text']}"
            l_new_lemma_key = f"{l_tok_before['Pos']}‣{l_new_lemma_txt}"
            l_new_form = {'Text': f"{l_form_before['Text']}-{l_form_after['Text']}",
                          'Tag': l_tok_before['Tag'],
                          'LemmaKey': l_new_lemma_key}
            l_new_lemma = {'Text': l_new_lemma_txt,
                           'Pos': l_tok_before['Pos'],
                           'EntType': composite_value(l_lemma_before['EntType'], l_lemma_after['EntType'])}
            # g_tokens_no_punctuation[l_tok_key] = l_new_tok
            add_data_restricted(l_tok_key, l_new_tok, l_new_form_key, l_new_form, l_new_lemma_key, l_new_lemma)
            add_data_restricted(None, None, l_form_key_before, l_form_before, l_form_before['LemmaKey'], l_lemma_before)
            add_data_restricted(None, None, l_form_key_after, l_form_after, l_form_after['LemmaKey'], l_lemma_after)


def process_cursor_list_gr(p_cursor_list):
    """
    Ancient Greek ground text processing function. Called for Shorey and Jowett

    This is done in two phases:
    - Phase I: construction of the note-less version of the text, while building lists of notes and Stephanus sections start indexes
    - Phase II: token processing after Spacy call on the note-less text

    :param p_cursor_list: Cursor list coming from sql request
    :return: nothing
    """
    global g_republic_txt
    global g_xpos_dict

    print(f'Ancient Greek length: {len(p_cursor_list)}')

    l_cur_start = 0
    l_sections_start_list = []
    l_sections_dict = dict()
    l_republic_txt = ''
    if p_cursor_list:
        for l_id, l_book, l_section, l_gr in p_cursor_list:
            # greek punctuation
            l_gr = l_gr.replace(':', '·')
            # l_gr = l_gr.replace('—', '·')
            # ωδ 287-289
            l_gr = l_gr.replace('ωδ 287-289 ', '')

            # print(g'------------------------------------{l_book} {l_section}------------------------------------')
            l_republic_txt += l_gr + ' '

            # establish list of section start positions
            l_sections_start_list.append((l_section, l_cur_start))
            l_sections_dict[l_cur_start] = l_section
            l_cur_start = len(l_republic_txt)

    # eliminate end space
    g_republic_txt = l_republic_txt.strip()

    # for l_sec, l_sec_start in l_sections_start_list:
    #     print(g'{l_sec} {g_republic_txt[l_sec_start:l_sec_start + 200]}')

    print(g_republic_txt)

    l_doc = g_nlp_gr(g_republic_txt)
    l_current_section = '----'
    l_prefix_section = '-'
    l_previous_word = None
    l_prev_prev_word = None
    # l_idx_correction = 0
    l_token_id = 0
    for l_sentence in l_doc.sentences:
        print(g_header_line_gr)
        print(g_header_titl_gr)
        print(g_header_line_gr)
        l_sent_words = l_sentence.words
        l_sent_len = len(l_sent_words)
        l_sent_ents = l_sentence.entities

        for e in l_sent_ents:
            g_entities_dict.setdefault(e.type, set()).add(e.text)

        for t in l_sentence.tokens:
            # miscalculation of length (and therefore of idx) before ' ̓ ' (like in 'δ̓ ')
            # l_length_problem = (t.text == '̓')
            # if l_length_problem:
            #     l_idx_correction += 1

            l_ent_type = t.ner
            l_ent_type = l_ent_type if l_ent_type else ''
            # for e in l_sent_ents:
            #     if t in e.tokens:
            #         l_ent_type = e.type

            # l_idx = t.index_char_start - l_idx_correction
            l_idx = t.start_char
            # id starts at 1
            l_id_first_w = t.id[0]
            l_is_sent_start = l_id_first_w == 1
            l_id_last_w = (t.id[0] if len(t.id) == 1 else t.id[1])
            l_is_sent_end = l_id_last_w == l_sent_len

            # get new current section if applicable
            try:
                l_sec = l_sections_dict[l_idx]
            except KeyError:
                l_sec = l_current_section

            l_new_cur_section = (l_sec != l_current_section)
            if l_new_cur_section:
                l_prefix_section = '+' if l_prefix_section == ' ' else ' '
                l_current_section = l_sec

            l_txt_cap = re.sub(r'\s+', ' ', t.text).strip()
            l_txt = l_txt_cap.lower()

            l_lemma = re.sub(r'\s+', ' ', ' '.join([w.lemma for  w in t.words])).strip().lower()
            l_pos = '-'.join([pos_reduce(w.upos) for w in t.words])
            l_tag = l_pos
            l_dep = '-'.join([(w.deps if w.deps else '') for w in t.words])
            l_xpos = '-'.join([w.xpos for w in t.words])
            l_grammar = '-'.join([w.feats if w.feats else '' for w in t.words])
            l_father = '-'.join([l_sent_words[w.head].text for w in t.words])
            g_xpos_dict.setdefault(l_xpos, set()).add(f'{l_pos}‣{l_grammar}')
            l_lemma_key = f'{l_lemma}‣{l_pos}'
            l_word_key = f'{l_txt}‣{l_tag}'

            l_grammar_dict_0 = dict()
            for w in t.words:
                if w.feats:
                    for kv in w.feats.split('|'):
                        k, v = kv.split('=')
                        l_grammar_dict_0.setdefault(k, set()).add(v)
                        g_morph_attr.setdefault(f'Z-{k}', set()).add(v)
                        g_morph_attr.setdefault(f'M-{k}', set()).add(v)

            l_grammar_dict = dict()
            for k in l_grammar_dict_0:
                v = list(l_grammar_dict_0[k])
                if len(v) == 1:
                    l_grammar_dict[k] = v[0]
                else:
                    l_grammar_dict[k] = v

            # l_context_from_txt = l_republic_txt[w.index_char_start:w.index_char_start+20]
            # l_word_from_txt = l_republic_txt[w.index_char_start:w.index_char_stop].lower().strip()
            # print(g'{l_txt:15} {l_word_from_txt:15}', l_context_from_txt, w)
            # if l_txt != l_word_from_txt:
            #     raise Exception

            # print(g'{l_prefix_section}{l_current_section:4} | {l_idx:6} | {l_txt:15} | {l_pos:5} | {l_lemma:15} |' +
            #       g' {l_xpos:10} | {l_dep:10} | {l_father:15} | {len(l_txt):2} {l_length_problem:5} {repr(t.string)} {l_grammar}')
            print(f'{l_prefix_section}{l_current_section:4} | {l_idx:6} | {l_txt:15} | {l_pos:5} | {l_lemma:15} |' +
                  f' {l_xpos:10} | {l_dep:10} | {l_father:15} | {len(l_txt):2}  {l_grammar}')

            g_last_form_key_context.setdefault(l_word_key, (None, None))
            g_last_form_key_context[l_word_key] = (l_idx, g_last_form_key_context[l_word_key][0])
            add_data(l_current_section,
                     f'Z-GRK-{l_token_id:06}',
                     {
                         'Text': l_txt_cap,
                         'pos': l_pos,
                         'Tag': l_pos,
                         'FormKey': [l_word_key],
                         'NewSection': (l_current_section if l_new_cur_section else ''),
                         'Grammar': l_grammar_dict,
                         'EntityKey': '',
                         'MarkupBefore': '',
                         'MarkupAfter': '',
                         'NoteKey': '',
                         'SentencePos': ('SS' if l_is_sent_start else ('SE' if l_is_sent_end else ''))
                     },
                     l_word_key, {'Text':l_txt, 'Tag':l_tag, 'LemmaKey':l_lemma_key},
                     l_lemma_key, {'Text':l_lemma, 'Pos':l_pos, 'EntType':l_ent_type}
            )
            l_token_id += 1

            l_word_key = f'{l_txt}‣{l_pos}'
            l_anno_reason = \
                'X' if l_pos == 'X' else\
                'NUM' if l_pos == 'NUM' and re.search(r'\d+', l_txt) else ''

            if len(l_anno_reason) > 0:
                l_i_before = l_id_first_w - 5
                l_i_before = 1 if l_i_before < 1 else l_i_before
                l_i_after = l_id_last_w + 6
                l_i_after = l_sent_len+1 if l_i_after > l_sent_len else l_i_after

                l_tok_left = ' '.join([l_sent_words[i].text for i in range(l_i_before, l_id_first_w+1)])
                l_tok_right = ' '.join([l_sent_words[i].text for i in range(l_id_first_w+1, l_i_after)])

                l_anno_txt = f'{l_current_section}: {l_prev_prev_word:20} {l_previous_word:20} {l_word_key:20} [{l_pos:5}] --> ' \
                             f'{l_tok_left:>40}|{l_tok_right}'

                g_anno.setdefault(l_anno_reason, []).append(l_anno_txt)

            l_prev_prev_word = l_previous_word
            l_previous_word = l_word_key

        print(g_header_line_gr)
        print()
    # for t, l, p, m in zip(l_doc.tokens, l_doc.lemmata, l_doc.pos, l_doc.morphosyntactic_features, l_doc.):
    #     print(g'{t:15} | {l:15} | {p:8} | {m}')


# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    l_db_connection = psycopg2.connect(
        host=g_dbServer,
        database=g_dbDatabase,
        user=g_dbUser,
        password=g_dbPassword
    )

    l_cursor_list_shorey = None
    l_cursor_list_jowett = None
    l_cursor_list_greek = None

    # load Shorey from DB
    l_cursor_read = l_db_connection.cursor()
    try:
        l_cursor_read.execute(
            """
                select "ID_SEC", "N_BOOK", "ID_SECTION", "TX_SHOREY"
                from "TB_SECTION"
                order by "ID_SECTION" ASC;
            """
        )
        l_cursor_list_shorey = list(l_cursor_read)
    except Exception as e:
        print('DB ERROR:', repr(e))
        print(l_cursor_read.query)

        sys.exit(0)
    finally:
        # release DB objects once finished
        l_cursor_read.close()

    # load Jowett from DB
    l_cursor_read = l_db_connection.cursor()
    try:
        l_cursor_read.execute(
            """
                select "ID_SEC", "N_BOOK", "ID_SECTION", "TX_JOWETT"
                from "TB_SECTION"
                order by "ID_SECTION" ASC;
            """
        )
        l_cursor_list_jowett = list(l_cursor_read)
    except Exception as e:
        print('DB ERROR:', repr(e))
        print(l_cursor_read.query)

        sys.exit(0)
    finally:
        # release DB objects once finished
        l_cursor_read.close()

    # load Greek text from DB
    l_cursor_read = l_db_connection.cursor()
    try:
        l_cursor_read.execute(
            """
                select "ID_SEC", "N_BOOK", "ID_SECTION", "TX_GR"
                from "TB_SECTION"
                order by "ID_SECTION" ASC;
            """
        )
        l_cursor_list_greek = list(l_cursor_read)
    except Exception as e:
        print('DB ERROR:', repr(e))
        print(l_cursor_read.query)

        sys.exit(0)
    finally:
        # release DB objects once finished
        l_cursor_read.close()

    with open('./Dictionaries/lsj_lemmas.txt') as l_fin:
        g_greek_lemmas = [remove_accents(l.rstrip()) for l in l_fin]
    with open('./Dictionaries/lsj_lemmas_noacc.txt', 'w') as l_fout:
        for l in g_greek_lemmas:
            l_fout.write(f'{l}\n')

    # Shorey
    print('Processing Shorey', file=sys.stderr)
    process_cursor_list_en(l_cursor_list_shorey, 'A-SHR', p_phase_ii=False)

    # Jowett
    print('Processing Jowett', file=sys.stderr)
    process_cursor_list_en(l_cursor_list_jowett, 'B-JWT', p_phase_ii=False)

    # Greek
    print('Processing Ancient Greek text', file=sys.stderr)
    # process_cursor_list_gr(l_cursor_list_greek)

    def print_lemma_2_words():
        print('================= Lemmas --> words =======================')
        for l_lem in sorted(g_lemma_form_dict.keys()):
            print(f'{l_lem:20} {sorted(g_lemma_form_dict[l_lem])}')
        print()

    def print_hyphens():
        print('================= Hyphenated words =======================')
        for h in g_hyphens:
            print(h)
        print()

    def print_anno():
        print('================= Annomalies =======================')
        for k in g_anno.keys():
            print(f'------------------ {k} [{len(g_anno[k])}] --------------------------')
            for a in g_anno[k]:
                print(a)
            print(f'------------------ {k} [{len(g_anno[k])}] --------------------------')
            print()


    def print_entities():
        print('================= Entities, detailed =======================')
        for k in sorted(g_entities_dict.keys()):
            print(f'----------- {k} --------------')
            for k_nm in sorted(g_entities_dict[k].keys()):
                print(k_nm)
                for s_i, s_k, e_i, e_k in g_entities_dict[k][k_nm]:
                    print(f'   {s_i:6} {e_i:6} {g_occurences[s_k]}')
                    print(f'                 {g_occurences[e_k]}')
        print()
        # dump entities
        print('================= Entities, summary =======================')
        for l_k_ent in sorted(g_entities_dict.keys()):
            l_k_list = sorted(list(set(g_entities_dict[l_k_ent].keys())))
            print(f'{k}: {l_k_list}')
        print()


    def print_xpos():
        print('================= XPOS =======================')
        for k in sorted(list(g_xpos_dict.keys())):
            print(f'----------- [{k}] --------------')
            for n in sorted(list(g_xpos_dict[k])):
                print('  ', n)
        print()

    def print_morph_attr():
        print('================= Grammar Attributes =======================')
        for k in sorted(list(g_morph_attr.keys())):
            print(f'{k:10}: {sorted(list(g_morph_attr[k]))}')
        print()

    def print_punctuation_groups():
        print('================= Punctuation groups (Tokens) =======================')
        for l_len in sorted(g_punctuation_groups.keys()):
            print(f'----------------- {l_len} ---------------------')
            for l_pg in sorted(list(g_punctuation_groups[l_len])):
                print('  ', l_pg, g_punctuation_group_context[l_pg])
        print()


    def print_dash_instances():
        print('================= Dash regex trial =======================')
        # — instances
        l_center_len = 60
        for l_summary in [True, False]:
            l_remaining = []
            for l_rt, l_name_author in zip(g_rt_list, ['Shorey', 'Jowett']):
                l_context_len = 20 if l_summary else 40
                print(f'------------------------- {l_name_author} ----------------------------')
                l_count = 0
                for l_match in re.finditer(r'(—[^—.?!;:]+[—.?!;:])', l_rt):
                    l_left = l_match.group(1)[:l_center_len]
                    l_right = l_match.group(1)[-l_center_len:]

                    l_start = l_match.start() - l_context_len
                    l_start = 0 if l_start < 0 else l_start
                    l_end = l_match.end() + l_context_len
                    l_end = len(l_rt) if l_end > len(l_rt) else l_end
                    if l_summary:
                        print(f'{l_rt[l_start:l_match.start()]:>{l_context_len}}{l_left:{l_center_len}} ' +
                              f'...[{l_match.end() - l_match.start():5}]... ' +
                              f'{l_right:>{l_center_len}}{l_rt[l_match.end():l_end]}')
                    else:
                        l_old = l_rt[l_start:l_end]
                        l_top = f'{l_count:4}: {l_rt[l_start:l_match.start()]:>{l_context_len}}' + \
                                f'∥{l_match.group(0)}∥' + \
                                f'{l_rt[l_match.end():l_end]}'
                        print(l_top)

                        l_new = l_old
                        # Dash transformation , “—
                        l_new = l_new.replace(', “—', ', “')
                        # e. —A n? —W
                        l_new = re.sub(r'(\w)([.?!])\s+—\s*([‘“]*[A-Z])', r'\1\2 \3', l_new)
                        # capitalize after .?!
                        l_new = re.sub(r'(\w)([.?!])\s+—\s*([a-z])',
                                       lambda m: f'{m.group(1)}{m.group(2)} {m.group(3).upper()}', l_new)
                        # d, —h
                        l_new = re.sub(r'(\w)([,;])\s+—\s*([‘“]*[a-z])', r'\1\2 \3', l_new)
                        l_new = re.sub(r'(\w)([,;])\s+—\s*([‘“]*I )', r'\1\2 \3', l_new)
                        # y—” “I
                        l_new = re.sub(r'(\w)—”\s+“([A-Z])', r'\1.” “\2', l_new)
                        # d: —W
                        l_new = re.sub(r'(\w):\s+—\s*([‘“]*\w)', r'\1: \2', l_new)

                        # canonical replacement as a comma
                        l_new = re.sub(r'([\w’”])\s*—\s*([a-z‘“])', r'\1, \2', l_new)
                        # special case of I
                        l_new = re.sub(r'([\w’”])\s*—\s*I ', r'\1, I ', l_new)

                        for l_left, l_right in g_rep_chunk_list:
                            l_new = l_new.replace(l_left, l_right)

                        l_mark = '+' if l_new == l_old else ' '
                        l_bottom = f'   {l_mark}  {l_new}'
                        print(l_bottom)

                        if l_mark == '+':
                            l_remaining.append(f'{l_top}\n{l_bottom}')
                    l_count += 1
                print('Total     :', l_count)
                print('Dash count:', len(re.findall('—', l_rt)))
                print()
            if len(l_remaining) > 0:
                print(
                    f'--------------------------------- Remaining [{len(l_remaining)}]----------------------------------')
                for l_item in l_remaining:
                    print(l_item)
                print()
                for l_item in l_remaining:
                    l_item = re.sub(r'\s*\d+:\s+', '', l_item).replace('∥', '')
                    print(f"('{l_item[:80]}',")
                    print(f" '{l_item[:80]}'),")

            print()

    def print_punctuation_groups_re():
        print('================= Punctuation Groups (re) =======================')
        l_punctuation_grp_dict = dict()
        for l_rt, l_name_author in zip(g_rt_list, ['Shorey', 'Jowett']):
            # !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
            for l_match in re.findall(f'(\w[{string.punctuation} ‘“’”]+\w)', l_rt):
                l_match = re.sub(r'\w', '_', l_match)
                l_punctuation_grp_dict.setdefault(len(l_match), set()).add(l_match)

        for l_len in sorted(l_punctuation_grp_dict.keys()):
            print(f'{l_len-2:2}:', end=' ')
            print('‖'.join([f'{l_pg}' for l_pg in sorted(list(l_punctuation_grp_dict[l_len]))]))
            print()
        print()
        print('Punctuation used: ', ' '.join(sorted(list(g_punctuation_set))))
        print()

    def print_apostrophes():
        print('================= Apostrophes =======================')
        l_apostrophe_grp_dict = dict()
        for l_rt, l_name_author in zip(g_rt_list, ['Shorey', 'Jowett']):
            for l_match in re.findall(f"(\w'\w)", l_rt):
                l_match = re.sub(r"\w'", "_'", l_match)
                l_apostrophe_grp_dict[l_match] = l_apostrophe_grp_dict.setdefault(l_match, 0) + 1
        print(''.join([f'{l_pg}: {l_apostrophe_grp_dict[l_pg]}\n' for l_pg in sorted(list(l_apostrophe_grp_dict.keys()))]))

    def print_dep_2_pos():
        # dump dep_ --> POS
        print('================= dep_ --> POS =======================')
        for k in sorted(g_dep_to_pos.keys()):
            print(f'{k:10}: {sorted(list(g_dep_to_pos[k]))}')
        print()

    def print_pos_2_lemma():
        # POS to Lemma
        print('================= POS --> Lemmas =======================')
        l_pos_length = [(len(g_pos_to_lemma[l_pos_key]), l_pos_key) for l_pos_key in sorted(g_pos_to_lemma.keys())]
        for l_len, l_pos_key in sorted(l_pos_length):
            l_words = sorted(list(g_pos_to_lemma[l_pos_key]))
            print(f"{l_pos_key:7} {l_len:8,} {l_words[:50]}{'...' if l_len > 50 else ''}")
        print()
        # TAG to forms
        print('================= TAG --> Forms =======================')
        l_tag_length = [(len(g_tag_to_form[l_tag_key]), l_tag_key) for l_tag_key in sorted(g_tag_to_form.keys())]
        for l_len, l_tag_key in sorted(l_tag_length):
            l_words = sorted(list(g_tag_to_form[l_tag_key]))
            print(f"{l_tag_key:7} {spacy.explain(l_tag_key).strip()[:40]:40} {l_len:6,} {l_words[:50]}{'...' if l_len > 50 else ''}")
        print()

    def print_missing():
        print('================= Missing sections =======================')
        l_current_prefix = None
        l_section_set = set()
        g_occur_restricted['Z-ZZZ-000000'] = {'NewSection': ''}
        for l_tok_key in sorted(g_occur_restricted.keys()):
            l_tok = g_occur_restricted[l_tok_key]

            l_section = l_tok['NewSection']
            if len(l_section) > 0:
                l_section_set.add(l_section)

            l_prefix = l_tok_key.split('-')[1]
            if l_prefix != l_current_prefix:
                if l_current_prefix:
                    if l_section_set == g_sections_set:
                        print(f'No missing sections in [{l_current_prefix}] ({len(l_section_set)}/{len(g_sections_set)})')
                    else:
                        l_missing_list = sorted(list(g_sections_set.difference(l_section_set)))
                        print(f'Missing sections in [{l_current_prefix}]: {len(l_missing_list)} {l_missing_list}')
                    l_section_set = set()
                l_current_prefix = l_prefix
        del g_occur_restricted['Z-ZZZ-000000']

    print()
    print()
    print(re.sub(r"([A-Za-z])['’]([a-z])", r'\1⏅\2', g_raw_text_without_notes_or_markup))

    # print_lemma_2_words()
    # print_hyphens()
    # print_dash_instances()
    # print_xpos()
    # print_morph_attr()
    # print_entities()
    # print_anno()
    # print_punctuation_groups()

    print_punctuation_groups()
    print_punctuation_groups_re()
    print_apostrophes()
    print_anno()
    print_pos_2_lemma()
    print_missing()

    with open('base_tok.json', 'w') as f:
        json.dump({'Base_Tok': g_base_tokens}, f, indent=4, ensure_ascii=False)

    with open('occ.json', 'w') as f:
        json.dump(g_occurences, f, indent=4, ensure_ascii=False)

    with open('forms.json', 'w') as f:
        json.dump(g_form_dict, f, indent=4, ensure_ascii=False)

    with open('lemmas.json', 'w') as f:
        json.dump(g_lemma_dict, f, indent=4, ensure_ascii=False)

    g_entities_selected_dict = dict()
    for k in sorted(g_entities_dict.keys()):
        l_k = sorted(list(set(g_entities_dict[k].keys())))
        g_entities_selected_dict[k] = [(f'{k}-{kn}', kn) for kn in l_k]

    with open('entities.json', 'w') as f:
        json.dump(g_entities_selected_dict, f, indent=4, ensure_ascii=False)

    # Dump notes
    with open('notes.json', 'w') as f:
        json.dump(g_notes_dict, f, indent=4, ensure_ascii=False)

    # g_tokens_no_punctuation
    # with open('occ_no_punc.json', 'w') as g:
    #     json.dump(g_tokens_no_punctuation, g, indent=4, ensure_ascii=False)

    with open('occ_restricted.json', 'w') as f:
        json.dump(g_occur_restricted, f, indent=4, ensure_ascii=False)

    with open('forms_restricted.json', 'w') as f:
        json.dump(g_form_restricted, f, indent=4, ensure_ascii=False)

    with open('lemmas_restricted.json', 'w') as f:
        json.dump(g_lemma_restricted, f, indent=4, ensure_ascii=False)
