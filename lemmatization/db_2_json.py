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

# NLTK for PUNCT error correction
import nltk
from nltk.stem import WordNetLemmatizer

# for Ancient Greek
import stanza

# DB connection parameters
g_dbServer = 'localhost'
g_dbDatabase = 'Perseus'
g_dbUser = 'postgres'
g_dbPassword = 'murugan!'

g_min_left_set = {'«', '‘', '“', '‛', '', '', '#', '', '¡', '§', '¿'}
g_min_mid_set = {'_', '-', '@', '·'}
g_min_right_set = {',', '.', ':', ';', '…', ')', ']', '}', '»', '’', '”', '‚', '„', '′', '″', '‴', '⁗', '%', '‰', '‱',
                   '&', '*', '†', '‡', '⁂', '⁎', '/', '।', '॥', '‖', '?', '!', '‼', '‽', '⁇', '⁈', '⁉', '·'}

# values set at the start of main()
g_left_punctuation = None
g_mid_punctuation = None
g_right_punctuation = None
g_all_punctuation = None

# headers for en and grc, respectively
g_header_line_en = '------+------------------------+------------+----------+----------------------+------------+-----------------+--------+-------+-------+----------+------------------------------------------+---------------------------------------'
g_header_titl_en = ' sec  | word                   | POS CG     | POS FG   | lemma                | Dep        | Father          | i      | end   | start | Type     | Note                                     | Morphology'

g_header_line_gr = '------+---------+-----------------+-------+-----------------+------------+------------+-----------------+---------------------------------------'
g_header_titl_gr = ' sec  | idx     | word            | POS   | lemma           | xpos       | Dep        | Father          | Morphology'

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
g_notes_list = []
g_dashes_remaining = []

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
    ('ied to contest it. But when he had agreed—“Can we deny, then,” said I, “that neither',
    r'ied to contest it. But when he had agreed. “Can we deny, then,” said I, “that neither'),
    ('hen I beheld what I had never seen before—Thrasymachus blushing([ДЖБабгдежзикл]+). But when w',
    r'hen I beheld what I had never seen before, Thrasymachus blushing\1. But when w'),
    ('whomsoever Zeus gives of both commingled— Now upon evil he chances and now again go',
    r'whomsoever Zeus gives of both commingled. Now upon evil he chances and now again go'),
    ('e lots, but to whom he gives unmixed evil— Hunger devouring drives him, a wanderer o',
    r'e lots, but to whom he gives unmixed evil. Hunger devouring drives him, a wanderer o'),
    ('mit our youth to hear what Aeschylus says— A god implants the guilty cause in men Wh',
    r'mit our youth to hear what Aeschylus says. A god implants the guilty cause in men Wh'),
    ('ting to draw him out, that he might go on— ([ДЖБабгдежзикл]+)Yes, Cephalus, I said: but I rather suspe',
    r'ting to draw him out, that he might go on. \1Yes, Cephalus, I said: but I rather suspe'),
    ('([ДЖБабгдежзикл]+)make the oaks of the just([ДЖБабгдежзикл]+)— ‘To bear acorns at their summit, and bees',
    r'\1make the oaks of the just\2. ‘To bear acorns at their summit, and bees'),
    ('or he speaks of one whose fame is([ДЖБабгдежзикл]+)— ‘As the fame of some blameless king who, l',
    r'or he speaks of one whose fame is\1. ‘As the fame of some blameless king who, l'),
    ('to himself in the words of Pindar([ДЖБабгдежзикл]+)— ‘Can I by justice or by crooked ways of de',
    r'to himself in the words of Pindar\1 ‘Can I by justice or by crooked ways of de'),
    ('he greater enquiry which is our final end—How do justice and injustice grow up in S',
    r'he greater enquiry which is our final end. How do justice and injustice grow up in S'),
    ('earth([ДЖБабгдежзикл]+).’ ([ДЖБабгдежзикл]+)And again([ДЖБабгдежзикл]+)— ‘Zeus, who is the dispenser of good and ev',
    r'earth\1.’ \2And again\3. ‘Zeus, who is the dispenser of good and ev'),
    ('ling names which describe the world below—Cocytus and Styx, ([ДЖБабгдежзикл]+)ghosts under th',
    r'ling names which describe the world below, Cocytus and Styx, \1ghosts under th'),
    ('t of the gods, as to make him say([ДЖБабгдежзикл]+)— ‘O heavens! with my eyes verily I behold a',
    r't of the gods, as to make him say\1. ‘O heavens! with my eyes verily I behold a'),
    ('s are always being perpetrated by([ДЖБабгдежзикл]+)— ‘The kindred of the gods, the relatives of',
    r's are always being perpetrated by\1. ‘The kindred of the gods, the relatives of'),
    ('ever they are foundЖаабддзЖ: True([ДЖБабгдежзикл]+)— Or, as we recognise the reflection of let',
    r'ever they are foundЖаабддзЖ: True\1. Or, as we recognise the reflection of let'),
    ('nowledge of bothЖаабддкЖ: Exactly([ДЖБабгдежзикл]+)— Even so, as I maintain, neither we nor ou',
    r'nowledge of bothЖаабддкЖ: Exactly\1. Even so, as I maintain, neither we nor ou'),
    ('terms, having clearly a relation([ДЖБабгдежзикл]+)— Yes, thirst is relative to drinkЖаагааиЖ.',
    r'terms, having clearly a relation\1. Yes, thirst is relative to drinkЖаагааиЖ.'),
    ('e held by women as well as by men([ДЖБабгдежзикл]+)— Yes— ([ДЖБабгдежзикл]+)The proper officers will tak',
    r'e held by women as well as by men\1. Yes. \2The proper officers will tak'),
    ('e got thus far in their tyrannical career—‘Let not the people⏅s friend,’ as they say',
    r'e got thus far in their tyrannical career. ‘Let not the people⏅s friend,’ as they say'),
    ('wisdom and reason are the judges([ДЖБабгдежзикл]+)— The only inference possible, he replied,',
    r'wisdom and reason are the judges\1. The only inference possible, he replied,'),
    ('th their voices the harmony of the sirens—Lachesis singing of the past, Clotho of t',
    r'th their voices the harmony of the sirens, Lachesis singing of the past, Clotho of t'),
    ('r; the responsibility is with the chooser—God is justified.’ When the Interpreter h',
    r'r; the responsibility is with the chooser, God is justified.’ When the Interpreter h'),
    ('ourselves in the battle of Megara([ДЖБабгдежзикл]+) —‘Sons of Ariston([ДЖБабгдежзикл]+), whose race from',
     r'ourselves in the battle of Megara\1: ‘Sons of Ariston\2, whose race from'),
    ('d the happy fortunes of her issue([ДЖБабгдежзикл]+)’ — Their days prolonged, from pain and sickn',
     r'd the happy fortunes of her issue\1’. Their days prolonged, from pain and sickn'),
    ('downwards the vision of the soul([ДЖБабгдежзикл]+) —If, I say, freed from these, it had suffe',
     r'downwards the vision of the soul\1. If, I say, freed from these, it had suffe'),
    ('the poets call the ‘threshold of old age’—Is life harder towards the end, or what r',
     'the poets call the ‘threshold of old age’. Is life harder towards the end, or what r')
]
# [ДЖБабгдежзикл]+

# globals to receive spaCy, NLTK and Stanza NLP objects respectively
g_nlp_en = None
g_nlp_en_nltk = None
g_nlp_gr = None

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
        if p_txt in g_left_punctuation:
            l_morph_dict['PunctSide'] = 'Left'
        elif p_txt in g_mid_punctuation:
            l_morph_dict['PunctSide'] = 'Mid'
        elif p_txt in g_right_punctuation:
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

    if p_form_key in g_form_dict.keys() and g_form_dict[p_form_key] != p_form_data and p_test_greek_lemmas: # Greek lemma sanity test
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

def get_context(p_match, p_replace=''):
    l_original = p_match.group(0)
    # start / end indexes of the match
    l_match_start_idx = p_match.start()
    l_match_end_idx = p_match.end()
    l_txt_searched = p_match.string

    # indexes for context (for printout)
    l_start_context = l_match_start_idx - 40
    l_start_context = 0 if l_start_context < 0 else l_start_context
    # p_match_end_idx = p_match_start_idx + len(p_original)
    l_end_context = l_match_end_idx + 40
    l_end_context = len(l_txt_searched) if l_end_context > len(l_txt_searched) else l_end_context

    return (
    f'∥{l_txt_searched[l_start_context:l_match_start_idx]}∥{l_original}∥{l_txt_searched[l_match_end_idx:l_end_context]}∥',
    f'∥{l_txt_searched[l_start_context:l_match_start_idx]}∥{p_replace}∥{l_txt_searched[l_match_end_idx:l_end_context]}∥')

def process_cursor_list_en(p_cursor_list, p_txt_key, p_phase_ii=True):
    """
    English processing function. Called for Shorey and Jowett

    This is done in two phases:
    - Phase I: construction of the note-less version of the text, while building lists of notes and Stephanus sections start indexes
    - Phase II: token processing after Spacy call on the note-less text

    :param p_cursor_list: Cursor list coming from sql request
    :param p_txt_key: 'A-SHR' or 'B-JWT'
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
    global g_notes_list
    global g_dashes_remaining

    print(f'[{p_txt_key}] English Cursor length: {len(p_cursor_list)}', file=sys.stderr)

    # ==================================================== Phase I =====================================================
    # notes dict (for pairing with tokens: position idx in text --> note)
    l_notes_dict = dict()
    # Stephanus section start indexes (for pairing with tokens: position idx in text --> stephanus number)
    l_sections_dict = dict()
    # position dict for [br], [i], etc (for pairing with tokens: position idx in text --> [br], [i], etc)
    l_markup_dict = dict()
    # position dict for book headings
    l_book_dict = {0: 'BOOK I.'} if p_txt_key == 'B-JWT' else dict()
    # current section start position (character index) while constructing note-less text
    #l_cur_section_start_idx = 0
    # full text, including notes
    l_txt_with_notes = ''
    # full text, without notes
    l_txt_no_notes = ''

    l_next_annotation_number = 0
    def number_2_cyr(p_number):
        l_number = f'{p_number:06}'
        l_cyr = ''
        for l_digit in l_number:
            l_cyr += 'абгдежзикл'[ord(l_digit) - ord('0')]
        return l_cyr

    def get_next_cyr():
        nonlocal l_next_annotation_number
        l_cyr = number_2_cyr(l_next_annotation_number)
        l_next_annotation_number += 1
        return l_cyr

    l_annotations_data = dict()
    def display_annotations_with_context():
        """
        Displays all annotations in l_annotations_data with context (40 char on both sides) plus
        the full text of each Stephanus section

        :return: nothing
        """
        l_annotations_positions = [(re.search(l_key, l_republic_txt).start(), l_annotations_data[l_key][0], l_annotations_data[l_key][1])
                                   for l_key in l_annotations_data.keys()]

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
            # context boundaries
            l_start_context = l_pos_idx-40
            l_start_context = 0 if l_start_context < 0 else l_start_context
            l_end_context = l_pos_idx + 40
            l_end_context = len(l_republic_txt) if l_end_context > len(l_republic_txt) else l_end_context
            # context string, with | at the position of the annotation
            l_before_after = f'{l_republic_txt[l_start_context:l_pos_idx]:>40}|{l_republic_txt[l_pos_idx:l_end_context]:40}'
            # full text of a section if this is one
            l_full_section = f' [{l_republic_txt[l_pos_idx:l_next_idx_dict[l_payload_annot]]}]' if l_type_annot == 'S' else ''
            l_payload_annot = f'{{{l_payload_annot}}}' if l_type_annot == 'N' else l_payload_annot
            print(f'[{l_count_annot:5}] {l_pos_idx:6} {l_type_annot} {l_before_after} {l_payload_annot}{l_full_section}')
            l_count_annot += 1
        print()

    def populate_dicts():
        nonlocal l_notes_dict
        nonlocal l_sections_dict
        nonlocal l_markup_dict
        nonlocal l_book_dict
        nonlocal l_republic_txt

        for l_anno_key in l_annotations_data.keys():
            l_position = re.search(l_anno_key, l_republic_txt).start()
            l_type_annot, l_payload_annot = l_annotations_data[l_anno_key]
            if l_type_annot == 'N':
                l_notes_dict.setdefault(l_position, []).append(l_payload_annot)
            elif l_type_annot == 'S':
                l_sections_dict[l_position] = l_payload_annot
            elif l_type_annot == 'M':
                l_markup_dict.setdefault(l_position, []).append(l_payload_annot)
            elif l_type_annot == 'B':
                l_book_dict[l_position] = l_payload_annot
            l_republic_txt = l_republic_txt.replace(l_anno_key, '')

    if not p_cursor_list:
        return

    g_raw_text_without_notes_or_markup += f'[{p_txt_key}] '
    l_section_2_book = dict() # dictionary recording section --> book relationship
    # cleanup of the sections data and extraction of notes, markup, etc
    for l_id, l_book, l_section, l_en in p_cursor_list:
        print(f'------------------------------------{l_book} {l_section}------------------------------------')
        print('Before:', l_en)
        l_section_2_book[l_section] = l_book

        # initial strip (for initial capitalized word)
        l_en = l_en.strip()

        # initial capitalized word
        l_en = re.sub(r'^[A-Z]+', lambda m: m.group(0)[0] + m.group(0)[1:].lower() , l_en)

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
            l_en = re.sub(f'\s*({l_wkn} (?:\d+\.)?\d+(?:-\d+)?(?:\s*ff\.)?)', r'{{\1}}', l_en)

        l_en = re.sub(r'\s+(Pindar Frag\. \d+, Loeb)', r'{{\1}}', l_en)
        l_en = re.sub(r'\s+(Aesch\. Niobe Fr\.)', r'{{\1}}', l_en)
        # , Aesch.
        l_en = l_en.replace('stream. Aesch.', 'stream.')
        l_en = l_en.replace('house, Aesch.', 'house,')

        # spaces between contiguous notes elimination
        l_en = re.sub(r'}}\s+\{\{', r'}}{{', l_en)

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
        # thither they neither look (Jowett)
        l_en = re.sub(r"thither they neither look", r"towards which they neither look", l_en)

        # their—the rulers'—advantage
        l_en = re.sub(r"their—the rulers'—advantage", r"their (the rulers') advantage", l_en)

        # spelling mistakes
        l_en = l_en.replace('Laden with', 'laden with')
        l_en = l_en.replace('/I', 'I')
        l_en = l_en.replace('WeIl , why', 'Why')
        l_en = l_en.replace('before they were Ill.', 'before they were ill.')
        l_en = l_en.replace('Imean', 'I mean')

        # turn multiple [br] into [brN] with N = number of consecutive [br]
        l_en = re.sub(r'\[br](?:\s+\[br])+',
                      lambda m: '[br' + str(len(re.findall(r"]\s*\[", m.group(0))) + 1) + ']',
                      l_en)
        # final cleanup (normalize all space sequences to a single <space>
        # adds final space for connection with next section
        l_en = re.sub(r'\s+', ' ', l_en).strip() + ' '

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
            # count spaces on the left and right of the match (if any)
            l_left_space = ' ' if len(re.search('^\s*', l_match_txt_ws).group(0)) > 0 else ''
            l_right_space = ' ' if (re.match('\[br\d*]', l_match_txt)
                                    or len(re.search('\s*$', l_match_txt_ws).group(0)) > 0) else ''
            l_cyr_anno = get_next_cyr()
            if l_match_txt[:2] == '{{': # notes
                # text cleanup (separators removal and replacement of underscores by spaces)
                l_payload_txt = re.sub(r'\{\{', '', l_match_txt).replace('}}', '').replace('_', ' ').strip()
                l_payload_type = 'N'
                l_cyr_anno = 'Ж' + l_cyr_anno + 'Ж'
                l_replace =  l_cyr_anno + l_right_space
                g_notes_list.append(l_payload_txt)
            elif 'BOOK' in l_match_txt:
                l_cyr_anno = f'Б{l_cyr_anno}Б'
                # gobbles up final space of the section so that the book heading marker has no space with the next section marker
                l_replace = f'{l_cyr_anno}'
                l_payload_type = 'B'
                l_payload_txt = l_match_txt
            else: # markup
                l_payload_txt = l_match_txt
                l_payload_type = 'M'
                l_cyr_anno = 'Д' + l_cyr_anno + 'Д' if l_payload_txt in ['[i]'] else 'Ж' + l_cyr_anno + 'Ж'
                l_replace = l_left_space + l_cyr_anno if l_payload_txt in ['[i]'] else l_cyr_anno + l_right_space

            # store annotation info
            l_annotations_data[l_cyr_anno] = (l_payload_type, l_payload_txt)
            return l_replace

        print('After :', l_en)
        # record section start
        l_cyr_section = 'Д' + get_next_cyr() + 'Д'
        l_annotations_data[l_cyr_section] = ('S', l_section)
        l_en = l_cyr_section + l_en

        # notes elimination and storage
        # l_section_txt_no_notes = re.sub(r'(?:\s*\{\{.*?}}\s*)|(?:\s*\[(?:br|i|/i)]\s*)',
        l_section_txt_no_notes = re.sub(r'\s*\{\{.*?}}\s*|\s*\[(?:br\d*|i|/i)]\s*|\s*BOOK\s+[XVI]+\.\s*',
                                        process_note_or_markup, l_en)

        # turn remaining square brackets into round ones
        l_section_txt_no_notes = l_section_txt_no_notes.replace('[', '(')
        l_section_txt_no_notes = l_section_txt_no_notes.replace(']', ')')

        print('Final :', l_section_txt_no_notes)

        # accumulate this section within l_txt_with_notes, l_txt_no_notes and g_raw_text_without_notes_or_markup
        # no separating spaces are added because they are inserted at the end of l_en above (or not if there is a book marker)
        l_txt_with_notes += l_en
        print('l_txt_with_notes, count of {{:', len(re.findall(r'\{\{', l_txt_with_notes)))
        l_txt_no_notes += l_section_txt_no_notes
        g_raw_text_without_notes_or_markup += l_section_txt_no_notes
    # end for l_id, l_book, l_section, l_en in p_cursor_list:

    # eliminate final space
    l_txt_with_notes = l_txt_with_notes.strip()
    l_txt_no_notes = l_txt_no_notes.strip()
    g_raw_text_without_notes_or_markup = g_raw_text_without_notes_or_markup.strip() + '\n'

    # note-less text in construction
    l_republic_txt = l_txt_no_notes
    print('l_republic_txt A:', l_republic_txt)

    display_annotations_with_context()
    print('========================= Before all-text regexes =========================')

    l_count_substitutions = 0
    def display_sub(p_match, p_replace):
        nonlocal l_count_substitutions
        # original string to be replaced by p_replace_or_modified_segment
        l_original = p_match.group(0)
        if l_original != p_replace:
            l_context_before, l_context_after = get_context(p_match, p_replace)
            l_match_start_idx = p_match.start()
            l_match_end_idx = p_match.end()

            l_first_line = f'{l_count_substitutions:5,} {l_match_start_idx:7,} [{l_match_end_idx - l_match_start_idx:5,}] {l_match_end_idx:7,}'
            print(f'{l_first_line} {l_context_before}')
            print(f'{" " * len(l_first_line)} {l_context_after}')
            l_count_substitutions += 1
        return p_replace

    # annotation markers normalization --> push next to closest word
    # in both cases, the punctuation character set includes space because these spaces have not yet been eliminated
    print('--------------------------- anno marker next to closest word ---------------------------')
    l_count_substitutions = 0
    pl = re.escape(g_left_punctuation)
    pr = re.escape(g_right_punctuation)

    # the Ж and Д are included in the set of characters [абгдежзикл] below because of cases like ЖаааззгЖЖааазздЖ,
    # — and ' must be present in the punctuation character set because they are neither in pl or pr but may still be present in l_republic_txt
    l_republic_txt = re.sub(f"(Д[Дабгдежзикл]+Д)([\s—'{pr}{pl}]*)(\w)",
                            lambda m: display_sub(m, m.expand(r'\2\1\3')), l_republic_txt)
    l_republic_txt = re.sub(f"(\w)([\s—'{pr}{pl}]*)(Ж[Жабгдежзикл]+Ж)",
                            lambda m: display_sub(m, m.expand(r'\1\3\2')), l_republic_txt)
    # Book markers. No Б inside [абгдежзикл] because they cannot be contiguous, contrary to Ж and Д markers
    l_republic_txt = re.sub(f"(Б[абгдежзикл]+Б)([\s—'{pr}{pl}]*)(\w)",
                            lambda m: display_sub(m, m.expand(r'\2\1\3')), l_republic_txt)

    print('l_republic_txt B:', l_republic_txt)
    print('--------------------------- Apostrophe save ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(r"([A-Za-z])[’']([a-z])",
                            lambda m: display_sub(m, m.expand(r'\1⏅\2')), l_republic_txt)

    #     print('--------------------------- Dangling right possessive save ---------------------------')
    #     l_count_substitutions = 0
    #     l_republic_txt = re.sub(f"([a-z])'([\s{re.escape(g_right_punctuation)}])",
    #                             lambda m: display_sub(m, m.expand(r'\1⍫\2')), l_republic_txt)

    print('--------------------------- Left punctuation unwanted space elimination ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(f"(\S)\s+([\s{re.escape(g_left_punctuation)}]{{2,}})(\w)",
                            lambda m: display_sub(m,
                                                  # m.expand(r'\1 \2\3')
                                                  m.group(1) + ' ' + re.sub(r'\s+', '', m.group(2)) + m.group(3)
                                                  ),
                            l_republic_txt)
    print('--------------------------- right punctuation unwanted space elimination ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(f"(\w)([\s{re.escape(g_right_punctuation)}]{{2,}})\s+(\S)",
                            lambda m: display_sub(m,
                                                  # m.expand(rr'\1\2 \3')
                                                  m.group(1) + re.sub(r'\s+', '', m.group(2)) + ' ' + m.group(3)
                                                  ),
                            l_republic_txt)

    print('--------------------------- Single quote normalization ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(r"['‘](\s*[\w ,;!?—\-.]+\s*)['’]",
                            lambda m: display_sub(m, m.expand(r'‘\1’')), l_republic_txt)

    print('--------------------------- Double quote normalization ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(r'["“‟](\s*[^"“‟”″]+\s*)["”″]',
                            lambda m: display_sub(m, m.expand(r'“\1”')), l_republic_txt)

    print('--------------------------- Duplicate punctuation sign elimination ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(f'([{re.escape(g_all_punctuation)}])\\1+',
                            lambda m: display_sub(m, m.expand(r'\1')), l_republic_txt)

    print('--------------------------- small dash spaces removal ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(r"(\w)(?:\s+-|-\s+)(\w)",
                            lambda m: display_sub(m, m.expand(r'\1-\2')), l_republic_txt)

    print('--------------------------- Single + Double quote elimination ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(r'“‘([^’]+)’“',
                            lambda m: display_sub(m, m.expand(r'“\1“')), l_republic_txt)

    print('--------------------------- Dash I - drop after :;, ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(f'(\w)([\s{pr}]*[:;,][\s{pr}]*)([\s{pl}]*—[\s{pl}]*)(\w)',
                            lambda m: display_sub(m,
                                                  re.sub(r'\s', '', f'{m.group(1)}{m.group(2)}') +
                                                  ' ' +
                                                  re.sub(r'[\s—]', '', f'{m.group(3)}{m.group(4)}')
                                                  # m.expand(r'\1\2 \3')
                                                  ), l_republic_txt)
    print('--------------------------- Dash II - capitalize after .?! ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(f'(\w)([\s{pr}]*[.?!:][\s{pr}]*)([\s{pl}]*—[\s{pl}]*)(\w)',
                            lambda m: display_sub(m,
                                                  re.sub(r'\s', '', f'{m.group(1)}{m.group(2)}') +
                                                  ' ' +
                                                  re.sub(r'[\s—]', '', f'{m.group(3)}{m.group(4).upper()}')
                                                  ), l_republic_txt)
    print('--------------------------- Dash III - Other right punctuation --> replace with , ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(f'(\w)([\s{pr}]*)([\s{pl}]*—[\s{pl}]*)([ДБабгдежзикл]*(?:[a-z]|I ))',
                            lambda m: display_sub(m,
                                                  re.sub(r'\s', '', f'{m.group(1)}{m.group(2)}') +
                                                  ', ' +
                                                  re.sub(r'[\s—]', '', m.group(3)) + m.group(4)
                                                  ), l_republic_txt)

    print('--------------------------- Chunks replacement ---------------------------')
    l_count_substitutions = 0
    for l_pattern, l_sub in g_rep_chunk_list:
        l_republic_txt = re.sub(l_pattern, lambda m: display_sub(m, m.expand(l_sub)), l_republic_txt)

    # list remaining dashes, if any
    for l_match in re.finditer(f'(\w)([\s{pr}]*)([\s{pl}]*—[\s{pl}]*)(\w)', l_republic_txt):
        l_context, _ = get_context(l_match)
        g_dashes_remaining.append((p_txt_key, l_match.start(), l_context))

    print('--------------------------- apostrophe/dangling possessive restore ---------------------------')
    l_count_substitutions = 0
    l_republic_txt = re.sub(r'[⍫⏅]',
                            lambda m: display_sub(m, m.expand("'")), l_republic_txt)

    print('========================= After all-text regexes =========================')
    display_annotations_with_context()
    populate_dicts()

    g_republic_txt = l_republic_txt
    g_rt_list.append(l_republic_txt)

    # print whole no-notes text
    print('[Text without notes &c.]', l_republic_txt)
    # print all notes below their insertion points (marked by '|') from l_txt_with_notes
    print('[Text with notes &c.   ]', re.sub(r'\{\{', '|\n-->{{', l_txt_with_notes).replace('}}', '}}\n'))

    print('-------------------------- Notes/Sections/Markup/Books --------------------------')
    l_key_list = sorted(list(
        set(l_notes_dict.keys())
        .union(set(l_sections_dict.keys())
               .union(set(l_markup_dict.keys())
                      .union(set(l_book_dict.keys()))
                      )
               )
    ))
    for l_idx in l_key_list:
        print(f'{l_idx:7,}', end=' ')
        try:
            print(l_notes_dict[l_idx], end=' ')
        except KeyError:
            pass
        try:
            print(l_sections_dict[l_idx], end=' ')
        except KeyError:
            pass
        try:
            print(l_markup_dict[l_idx], end=' ')
        except KeyError:
            pass
        try:
            print(l_book_dict[l_idx], end=' ')
        except KeyError:
            pass
        print()

    # ==================================================== Phase II ====================================================
    if not p_phase_ii:
        return
    print('Phase II - spaCy', file=sys.stderr)

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
    print('spaCy complete', file=sys.stderr)

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
                        l_start_tk, f'{p_txt_key}-{l_start_tk:06}', l_end_tk , f'{p_txt_key}-{l_end_tk - 1:06}'))

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

            :param p_token: The spaCy token object
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
        (l_txt_cap, l_form_txt, l_lemma_txt, l_is_sent_end, l_is_sent_start, l_tag, l_new_pos,
         l_lemma_key, l_form_key, l_father, l_grammar_dict) = get_tok_attr(t)

        # get some values from previous and last token
        _, l_txt_p, l_lem_p, _, _, l_tag_p, _, _, l_prev_word_key,_ ,_ = get_tok_attr(l_token_neighborhood[0])
        _, l_txt_n, l_lem_n, _, _, l_tag_n, _, _, l_next_word_key,_ ,_ = get_tok_attr(l_token_neighborhood[2])

        # POS --> Lemma recording
        g_pos_to_lemma.setdefault(l_new_pos, set()).add(l_lemma_txt)
        g_tag_to_form.setdefault(l_tag, set()).add(l_form_txt)

        # get note, if one is attached to this token
        l_notes = []
        l_tok_key = f'{p_txt_key}-{t.i:06}'
        try:
            # notes are positioned just after the end of a token
            l_notes = l_notes_dict[t.idx + len(t.text)]
            # if found, store it
            g_notes_dict[l_tok_key] = l_notes
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
        l_markup_before = []
        l_markup_after = []
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

        # get book heading, if any
        l_book_heading = ''
        try:
            l_book_heading = l_book_dict[t.idx]
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
            f'{l_prefix_section}{l_current_section:4} | {l_form_txt:22} | {l_new_pos:10} | {l_tag:8} | {l_lemma_txt:20} | {t.dep_:10} | {l_father:15} | {t.i:6} | ' +
            f'{l_is_sent_end:5} | {l_is_sent_start:5} | {t.ent_type_:8} | {str(l_notes)[:40]:40} | {l_grammar_dict} {l_tok_key}')

        # store data for the current occurrence, form and lemma
        add_data(l_current_section, l_tok_key,
                 {
                    'Text': l_txt_cap,
                    'Pos': l_new_pos,
                    'Tag': l_tag,
                    'FormKey': l_form_key,
                    'NewSection': l_new_current_section,
                    'BookNumber': l_section_2_book[l_new_current_section] if len(l_new_current_section) > 0 else -1,
                    'BookTitle': l_book_heading,
                    'Grammar': l_grammar_dict,
                    'EntityKey': l_ent_key,
                    'MarkupBefore': l_markup_before,
                    'MarkupAfter': l_markup_after,
                    'NoteKey': l_tok_key if len(l_notes) > 0 else '',
                    'SentencePos': ('SS' if t.is_sent_start else ('SE' if t.is_sent_end else ''))
                 },
                 l_form_key, {'Text': l_form_txt, 'Tag': l_tag, 'LemmaKey': [l_lemma_key]},
                 l_lemma_key, {'Text': l_lemma_txt, 'Pos': l_new_pos, 'EntType': t.ent_type_}
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
            l_anno_txt = f'{l_current_section}: {l_prev_prev_form_key:20} {l_previous_form_key:20} {l_form_key:20} [{l_new_pos:5}] --> {l_context}'

            # anno storage, for further analysis
            g_anno.setdefault(l_anomaly_reason, []).append(l_anno_txt)

        # punctuation group analysis
        if l_new_pos == 'PUNCT':
            l_punctuation_pipe.append((l_form_txt, l_lemma_txt))

            if re.search(r'\w+', l_form_txt):
                l_nltk_lem_list = ' '.join([f'[{l_pos_choice}]{g_nlp_en_nltk.lemmatize(l_form_txt, l_pos_choice)}'
                                            for l_pos_choice in ['n', 'v', 'a', 'r', 's']])
                l_anno_txt = (f'{l_current_section}: {l_prev_prev_form_key:20} {l_previous_form_key:20} ' +
                              f'{l_form_key:20} [{l_new_pos:5}] {l_tok_key} {t.dep_}{"/is_punct" if t.is_punct else "_"} {l_nltk_lem_list} --> {l_context}')
                g_anno.setdefault('PUNCT', []).append(l_anno_txt)

        if (l_new_pos == 'PUNCT' and len(t.whitespace_) > 0) or (l_new_pos != 'PUNCT' and len(l_punctuation_pipe) > 0):
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
    # print('Phase III - Grouping punctuation into tokens', file=sys.stderr)
    # group_punctuation()

def group_punctuation(p_en=True):
    class HyphenatedWordException(Exception):
        pass

    # first pass for punctuation fields creation in each token
    l_occur_restricted_tmp_list = []
    for l_tok_index in range(len(g_base_tokens)):
        l_tok_key, l_tok = g_base_tokens[l_tok_index]
        l_tok_form = l_tok['Text'].lower()

        # preprocessing of words misidentified as punctuation
        if l_tok['Pos'] == 'PUNCT' and re.search(r'\w+', l_tok_form):
            l_new_pos = ''
            l_lemma_text = None
            if p_en:
                for l_pos_choice in ['n', 'v', 'a', 'r', 's']:
                    l_lemma_text = g_nlp_en_nltk.lemmatize(l_tok_form, l_pos_choice)
                    if l_lemma_text != l_tok_form:
                        l_new_pos = l_pos_choice
                        break
            else: # grk
                for l_pos_choice in ['NOUN', 'VERB', 'ADJ', 'ADV']:
                    l_pd = stanza.Document([[{'id': 1, 'text': l_tok_form, 'upos': l_pos_choice}]])
                    if g_nlp_gr(l_pd)[0][0]['lemma'] != l_tok_form:
                        l_new_pos = l_pos_choice
                        break

            if l_new_pos != '':
                if l_new_pos not in ['NOUN', 'VERB', 'ADJ', 'ADV']:
                    l_new_pos = {'n': 'NOUN', 'v': 'VERB', 'a': 'ADJ', 'r': 'ADV', 's': 'ADJ'}[l_new_pos]
            else:
                l_pos_length = [(len(g_pos_to_lemma[l_pos_key]), l_pos_key) for l_pos_key in
                                sorted(g_pos_to_lemma.keys()) if l_pos_key != 'PUNCT']
                for _, l_pos_key in sorted(l_pos_length):
                    if l_tok_form in g_pos_to_lemma[l_pos_key]:
                        l_new_pos = l_pos_key
                        break

            if l_new_pos == '':
                l_new_pos = 'X'

            l_new_lemma_key = f'{l_lemma_text}‣{l_new_pos}'
            l_new_form_key = ''
            l_new_data_added = ''
            if l_new_lemma_key not in g_lemma_dict.keys(): # lemma not already in g_lemma_dict
                l_new_form_key = f'{l_tok_form}‣{l_new_pos}'
                # create lemma and form records
                add_data(None,
                         None, None,
                         l_new_form_key, {'Text': l_tok_form, 'Tag': l_new_pos, 'LemmaKey': [l_new_lemma_key]},
                         l_new_lemma_key, {'Text': l_lemma_text, 'Pos': l_new_pos, 'EntType': ''}
                         )
                l_new_data_added = ' New Data'
            else: # use existing lemma
                for l_fk in g_form_dict.keys():
                    if g_form_dict[l_fk]['LemmaKey'][0] == l_new_lemma_key and l_fk.split('‣')[0] == l_tok_form:
                        l_new_form_key = l_fk
                        break

                if l_new_form_key == '':
                    l_new_form_key = f'{l_tok_form}‣{l_new_pos}'

            l_tok['Pos'] = l_new_pos
            l_tok['Tag'] = l_new_form_key.split('‣')[1]
            l_tok['FormKey'] = l_new_form_key
            g_anno.setdefault('PUNCT', []).append(f'Correction {l_tok_key} {l_tok_form:15} PUNCT --> {l_new_form_key}/{l_new_lemma_key}{l_new_data_added}')

        if l_tok['Pos'] != 'PUNCT' and l_tok['Text'] != '-': # get punctuation on both side of non-punctuation token
            # try: # eliminate hyphenated word cases when a hyphen is present before or after (treated below)
            l_new_sent_pos = l_tok['SentencePos']
            l_punctuation_left = ''
            l_new_s_left = ''
            if l_tok_index > 0:
                l_fin_left = False
                l_i_left = l_tok_index - 1
                # if g_base_tokens[l_i_left][1]['Text'] == '-':
                #     raise HyphenatedWordException # Hyphen before

                while not l_fin_left:
                    l_tok_left = g_base_tokens[l_i_left][1]
                    if l_tok_left['Pos'] != 'PUNCT' or l_tok_left['Grammar']['PunctSide'] != 'Left':
                        l_fin_left = True
                    else:
                        l_punctuation_left = l_tok_left['Text'] + l_punctuation_left
                        l_new_sent_pos = l_tok_left['SentencePos'] if len(l_tok_left['SentencePos']) > 0 else l_new_sent_pos
                        l_new_s_left = l_tok_left['NewSection']
                        if l_i_left == 0:
                            l_fin_left = True
                        else:
                            l_i_left -= 1
            l_punctuation_right = ''
            if l_tok_index < len(g_base_tokens) - 1:
                l_fin_right = False
                l_i_right = l_tok_index + 1
                # if g_base_tokens[l_i_right][1]['Text'] == '-':
                #     raise HyphenatedWordException # Hyphen after

                while not l_fin_right:
                    l_tok_right = g_base_tokens[l_i_right][1]
                    if l_tok_right['Pos'] != 'PUNCT' or l_tok_right['Grammar']['PunctSide'] != 'Right':
                        l_fin_right = True
                    else:
                        l_punctuation_right = l_punctuation_right + l_tok_right['Text']
                        l_new_sent_pos = l_tok_right['SentencePos'] if len(l_tok_right['SentencePos']) > 0 else l_new_sent_pos
                        if l_i_right == len(g_base_tokens) - 1:
                            l_fin_right = True
                        else:
                            l_i_right += 1
            l_tok['PunctLeft'] = l_punctuation_left
            l_tok['PunctRight'] = l_punctuation_right
            l_tok['SentencePos'] = l_new_sent_pos
            # g_tokens_no_punctuation[l_tok_key] = l_tok
            # l_form_key = l_tok['FormKey'][0]
            # l_form = g_form_dict[l_form_key]
            # l_lemma_key = l_form['LemmaKey']
            # l_lemma_text = g_lemma_dict[l_lemma_key]
            if len(l_new_s_left) > 0:
                l_tok['NewSection'] = l_new_s_left
            l_occur_restricted_tmp_list.append((l_tok_key, l_tok))
            # g_occur_restricted_tmp[l_tok_key] = l_tok
            # except HyphenatedWordException:
            #     pass # nothing to do - Hyphenated words treated below
        elif l_tok['Text'] == '-':
            l_occur_restricted_tmp_list.append((l_tok_key, l_tok))

    # second pass to take care of hyphenated words
    l_prev_text = ''
    for l_tok_index in range(len(l_occur_restricted_tmp_list)):
        l_tok_key, l_tok = l_occur_restricted_tmp_list[l_tok_index]
        l_cur_text = l_tok['Text']
        l_next_text = l_occur_restricted_tmp_list[l_tok_index + 1][1]['Text'] if l_tok_index < len(l_occur_restricted_tmp_list) -1 else ''
        if l_prev_text != '-' and l_cur_text != '-' and l_next_text != '-':
            # Non-hyphenated words (and ignore tokens adjacent to a dash)
            l_form_key = l_tok['FormKey']
            l_form = g_form_dict[l_form_key]
            l_lemma_key = l_form['LemmaKey'][0]
            l_lemma = g_lemma_dict[l_lemma_key]
            add_data_restricted(l_tok_key, l_tok, l_form_key, l_form, l_lemma_key, l_lemma)
        elif l_cur_text == '-':
            # Hyphenated words (tokens adjacent to a dash are taken care of here)
            if l_tok_index == 0 or l_tok_index == len(l_occur_restricted_tmp_list) - 1:
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
            _, l_tok_before = l_occur_restricted_tmp_list[l_tok_index-1]
            _, l_tok_after = l_occur_restricted_tmp_list[l_tok_index+1]
            l_new_text = f"{l_tok_before['Text']}-{l_tok_after['Text']}"
            l_new_form_key = f"{l_new_text}‣{l_tok_before['Tag']}"
            l_new_tok = {
                'Text': l_new_text,
                'Pos': l_tok_before['Pos'],
                'Tag': l_tok_before['Tag'],
                # 'FormKey': l_tok_before['FormKey'] + l_tok_after['FormKey'] + [l_new_form_key],
                'FormKey': l_new_form_key,
                'NewSection': l_tok_before['NewSection'],
                'BookTitle': l_tok_before['BookTitle'],
                'BookNumber': l_tok_before['BookNumber'],
                'Grammar': merge_grammar_dicts(l_tok_before['Grammar'], l_tok_after['Grammar']),
                'EntityKey': '',
                'MarkupBefore': l_tok_before['MarkupBefore'],
                'MarkupAfter': l_tok_after['MarkupAfter'],
                'NoteKey': l_tok_before['NoteKey'] if len(l_tok_before['NoteKey']) > 0 else l_tok_after['NoteKey'],
                'SentencePos': ('SS' if l_tok_before['SentencePos'] == 'SS' else ('SE' if l_tok_after['SentencePos'] == 'SE' else '')),
                'PunctLeft': l_tok_before['PunctLeft'],
                'PunctRight': l_tok_after['PunctRight']
            }
            l_form_key_before = l_tok_before['FormKey']
            l_form_key_after = l_tok_after['FormKey']
            l_form_before = g_form_dict[l_form_key_before]
            l_form_after = g_form_dict[l_form_key_after]
            l_lemma_key_before = l_form_before['LemmaKey'][0]
            l_lemma_key_after = l_form_after['LemmaKey'][0]
            l_lemma_before = g_lemma_dict[l_lemma_key_before]
            l_lemma_after = g_lemma_dict[l_lemma_key_after]
            l_new_lemma_txt = f"{l_lemma_before['Text']}-{l_lemma_after['Text']}"
            l_new_lemma_key = f"{l_new_lemma_txt}‣{l_tok_before['Pos']}"
            l_new_form = {'Text': f"{l_form_before['Text']}-{l_form_after['Text']}",
                          'Tag': l_tok_before['Tag'],
                          'LemmaKey': [l_lemma_key_before, l_new_lemma_key, l_lemma_key_after] }
            l_new_lemma = {'Text': l_new_lemma_txt,
                           'Pos': l_tok_before['Pos'],
                           'EntType': composite_value(l_lemma_before['EntType'], l_lemma_after['EntType'])}
            # g_tokens_no_punctuation[l_tok_key] = l_new_tok
            add_data_restricted(l_tok_key, l_new_tok, l_new_form_key, l_new_form, l_new_lemma_key, l_new_lemma)
            add_data_restricted(None, None, None, None, l_lemma_key_before, l_lemma_before)
            add_data_restricted(None, None, None, None, l_lemma_key_after, l_lemma_after)

        l_prev_text = l_cur_text

def process_cursor_list_gr(p_cursor_list):
    """
    Ancient Greek ground text processing function, Done with Stanza.

    :param p_cursor_list: Cursor list coming from sql request
    :return: nothing
    """
    global g_republic_txt
    global g_xpos_dict

    print(f'Ancient Greek length: {len(p_cursor_list)}', file=sys.stderr)

    # position --> Stephanus number
    l_sections_dict = dict()
    # current position of next section, to build l_sections_dict
    l_cur_start = 0
    # ancient Greek text being built
    l_republic_txt = ''
    # section --> book number
    l_section_2_book = dict()
    if p_cursor_list:
        for l_id, l_book, l_section, l_gr in p_cursor_list:
            # greek punctuation
            l_gr = l_gr.replace(':', '.')
            # Dashes elimination
            l_gr = re.sub(r'\s*—\s*', ', ', l_gr)

            # references to works
            for l_wkn in [r'ηομ\. ιλ\.',
                          r'ηδτ\.',
                          r'σεϝεν',
                          r'αεσξη\. φραγ\.',
                          r'ηες. ωδ(?:\.)?',
                          r'πινδαρ φραγ\.',
                          r'ηομ\. οδ\.',
                          r'ηομ\. ωδ(?:\.)?']:
                l_gr = re.sub(f'\s*({l_wkn} (?:\d+\.)?\d+(?:-\d+)?(?:\s*ff\.)?)', r'{{\1}}', l_gr)
            l_gr = re.sub(r'φρ\. \d+ βεργκ, ανδ ευρ\. ορεστ\. \d+\.', '', l_gr)

            # notes elimination
            l_gr = re.sub(r'\{\{[^}]+}}', '', l_gr)

            # print(f'------------------------------------{l_book} {l_section}------------------------------------')
            print(f'{l_book:2} {l_section} {l_gr}')

            # accumulate full text
            l_republic_txt += l_gr + ' '

            # detect initial non-word char to place section start at first \w char
            pl = re.escape(g_left_punctuation)
            pr = re.escape(g_right_punctuation)

            l_match = re.search(f'^[\s{pr}{pl}]+', l_gr)

            # establish list of section start positions
            l_sections_dict[l_cur_start + (len(l_match.group(0)) if l_match else 0)] = l_section
            l_section_2_book[l_section] = l_book
            l_cur_start = len(l_republic_txt)

    # eliminate final space
    g_republic_txt = l_republic_txt.strip()

    print(g_republic_txt)

    for l_match in re.finditer('—', g_republic_txt):
        print(get_context(l_match)[0])

    # call to Stanza
    print(f'Phase II - Stanza', file=sys.stderr)
    l_doc = g_nlp_gr(g_republic_txt)
    print(f'Stanza complete', file=sys.stderr)

    # current stephanus number
    l_current_section = '----'
    # prefix to differentiate sections from one another (+ or space)
    l_prefix_section = '-'
    # 2 previous words (tokens) pipe
    l_previous_word = None
    l_prev_prev_word = None
    # l_idx_correction = 0
    # counter to give a number to each token (equivalent of t.i in spaCy)
    l_token_id = 0
    # loop through sentences (then tokens in each sentence)
    for l_sentence in l_doc.sentences:
        print(g_header_line_gr)
        print(g_header_titl_gr)
        print(g_header_line_gr)

        # words, not tokens
        l_sent_words = l_sentence.words
        # length of sentence in number of words
        l_sent_len = len(l_sent_words)
        # entities
        l_sent_entities = l_sentence.entities

        for l_ent in l_sent_entities:
            g_entities_dict.setdefault(l_ent.type, set()).add(l_ent.text)

        # loop through tokens in one sentence
        for t in l_sentence.tokens:
            # entity type
            l_ent_type = t.ner
            l_ent_type = l_ent_type if l_ent_type else ''

            # l_idx = t.index_char_start - l_idx_correction
            # position of token in text
            l_idx = t.start_char
            # CAUTION id (position of words in sentence) starts at 1
            # A token may consist of two words
            l_id_first_w = t.id[0]
            l_is_sent_start = l_id_first_w == 1
            l_id_last_w = (t.id[0] if len(t.id) == 1 else t.id[1])
            l_is_sent_end = l_id_last_w == l_sent_len

            # get new current section if applicable
            try:
                l_sec = l_sections_dict[l_idx]
            except KeyError:
                l_sec = l_current_section

            l_new_book_number = -1
            # l_new_cur_section will contain the stephanus number only on the first toke of the section
            l_new_cur_section = (l_sec != l_current_section)
            if l_new_cur_section:
                # change prefix
                l_prefix_section = '+' if l_prefix_section == ' ' else ' '
                l_new_book_number = l_section_2_book[l_sec]
                l_current_section = l_sec

            # raw tex (stripped)
            l_txt_cap = re.sub(r'\s+', ' ', t.text).strip()
            # uncapitalized text
            l_form_txt = l_txt_cap.lower()

            # for most attributes below, the value is a joined list in case of multiple words in token
            l_lemma_txt = re.sub(r'\s+', ' ', ' '.join([w.lemma for  w in t.words])).strip().lower()
            l_pos = '⎇'.join([pos_reduce(w.upos) for w in t.words])
            # tag = pos because xpos is too complicated
            l_tag = l_pos
            # dependencies in sentence tree
            l_dep = '-'.join([(w.deps if w.deps else '') for w in t.words])
            l_xpos = '-'.join([w.xpos for w in t.words])
            # series of k=v groups separated by |
            l_grammar = '-'.join([w.feats if w.feats else '' for w in t.words])
            # father in sentence tree
            l_father = '-'.join([l_sent_words[w.head].text for w in t.words])
            # gathering of xpos for further analysis
            g_xpos_dict.setdefault(l_xpos, set()).add(f'{l_pos}‣{l_grammar}')

            if l_pos == 'PUNCT':
                g_punctuation_set.add(l_form_txt)

            # keys
            l_lemma_key = f'{l_lemma_txt}‣{l_pos}'
            l_form_key = f'{l_form_txt}‣{l_tag}'

            # construction of grammar dict
            # Phase 1 - from individual words separately --> sets of values
            l_grammar_dict_0 = dict()
            for w in t.words:
                if w.feats:
                    for kv in w.feats.split('|'):
                        l_key, l_value = kv.split('=')
                        l_grammar_dict_0.setdefault(l_key, set()).add(l_value)
                        # Z = combined values, M = Greek only
                        g_morph_attr.setdefault(f'Z-{l_key}', set()).add(l_value)
                        g_morph_attr.setdefault(f'M-{l_key}', set()).add(l_value)

            # Phase 2 - reduce sets of values to a single one, whenever possible
            l_grammar_dict = dict()
            for l_key in l_grammar_dict_0:
                l_value = list(l_grammar_dict_0[l_key])
                if len(l_value) == 1:
                    l_grammar_dict[l_key] = l_value[0]
                else:
                    l_grammar_dict[l_key] = l_value

            l_grammar_dict = morph_standardize(l_pos, l_txt_cap, l_grammar_dict, p_suspend=True)

            l_sent_marker = '⇧' if l_is_sent_start else ('⇩' if l_is_sent_end else ' ')
            print(f'{l_prefix_section}{l_current_section:4} | {l_idx:5}{l_sent_marker} | {l_form_txt:15} | {l_pos:5} | {l_lemma_txt:15} |' +
                  f' {l_xpos:10} | {l_dep:10} | {l_father:15} | {len(l_form_txt):2}  {l_grammar}')

            # POS --> Lemma recording
            g_pos_to_lemma.setdefault(l_pos, set()).add(l_lemma_txt)
            g_tag_to_form.setdefault(l_tag, set()).add(l_form_txt)

            # context for lemma sanity check inside add_data()
            g_last_form_key_context.setdefault(l_form_key, (None, None))
            g_last_form_key_context[l_form_key] = (l_idx, g_last_form_key_context[l_form_key][0])
            # store token data
            add_data(l_current_section,
                     f'Z-GRK-{l_token_id:06}',
                     {
                         'Text': l_txt_cap,
                         'Pos': l_pos,
                         'Tag': l_pos,
                         'FormKey': l_form_key,
                         'NewSection': (l_current_section if l_new_cur_section else ''),
                         'BookTitle': '',
                         'BookNumber': l_new_book_number,
                         'Grammar': l_grammar_dict,
                         'EntityKey': '',
                         'MarkupBefore': '',
                         'MarkupAfter': '',
                         'NoteKey': '',
                         'SentencePos': ('SS' if l_is_sent_start else ('SE' if l_is_sent_end else ''))
                     },
                     l_form_key, {'Text': l_form_txt, 'Tag': l_tag, 'LemmaKey': [l_lemma_key]},
                     l_lemma_key, {'Text': l_lemma_txt, 'Pos': l_pos, 'EntType': l_ent_type}
            )
            # increment token number used in token id above
            l_token_id += 1

            # anomalies detection
            l_anno_reason = \
                'X' if l_pos == 'X' else\
                'PUNCT' if l_pos == 'PUNCT' and re.search(r'\w+', l_form_txt) else \
                'NUM' if l_pos == 'NUM' and re.search(r'\d+', l_form_txt) else ''

            if len(l_anno_reason) > 0:
                # context building (5 tokens before, 6 after)
                l_i_before = l_id_first_w - 5
                l_i_before = 1 if l_i_before < 1 else l_i_before
                l_i_after = l_id_last_w + 6
                l_i_after = l_sent_len+1 if l_i_after > l_sent_len else l_i_after

                l_tok_left = ' '.join([l_sent_words[i].text for i in range(l_i_before, l_id_first_w+1)])
                l_tok_right = ' '.join([l_sent_words[i].text for i in range(l_id_first_w+1, l_i_after)])

                l_anno_txt = f'{l_current_section}: {l_prev_prev_word:20} {l_previous_word:20} {l_form_key:20} [{l_pos:5}] --> ' \
                             f'{l_tok_left:>40}|{l_tok_right}'

                g_anno.setdefault(l_anno_reason, []).append(l_anno_txt)

            # form key pipeline
            l_prev_prev_word = l_previous_word
            l_previous_word = l_form_key

        print(g_header_line_gr)
        print()

    # print(f'Phase III - Grouping punctuation into tokens', file=sys.stderr)
    # group_punctuation()


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
    for l_k_ent in sorted(g_entities_dict.keys()):
        print(f'----------- {l_k_ent} --------------')
        for k_nm in sorted(g_entities_dict[l_k_ent].keys()):
            print(k_nm)
            for s_i, s_k, e_i, e_k in g_entities_dict[l_k_ent][k_nm]:
                print(f'   {s_i:6} {e_i:6} {g_occurences[s_k]}')
                print(f'                 {g_occurences[e_k]}')
    print()
    # dump entities
    print('================= Entities, summary =======================')
    for l_k_ent in sorted(g_entities_dict.keys()):
        l_k_list = sorted(list(set(g_entities_dict[l_k_ent].keys())))
        # print(f'{l_k_ent}: {l_k_list}')
        print('\n'.join([f"'{l_ent}': '{l_k_ent}'," for l_ent in l_k_list]))
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
    for l_key in sorted(list(g_morph_attr.keys())):
        print(f'{l_key:10}: {sorted(list(g_morph_attr[l_key]))}')
    print()
    # l_key_explained = ''
    #     for l_key in sorted(list(g_morph_attr.keys())):
    #         if re.search('^M-', l_key):
    #             l_key_short = l_key.replace('M-', '')
    #             l_key_explained += f"('{l_key_short}', '{spacy.explain(l_key_short)}')\n"
    #             print(f"'{l_key}': {[(l_val, spacy.explain(l_val)) for l_val in g_morph_attr[l_key]]},")
    #     print()
    #     print(l_key_explained)


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
        print(f'{l_len - 2:2}:', end=' ')
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
        print(
            f"{l_tag_key:7} {spacy.explain(l_tag_key).strip()[:40]:40} {l_len:6,} {l_words[:50]}{'...' if l_len > 50 else ''}")
    print()


def print_missing():
    print('================= Missing sections =======================')
    l_current_prefix = None
    l_last_section = None
    l_section_set = set()
    l_notes_list = []
    l_notes_key_list = []
    l_notes_section_list = []
    g_occur_restricted['Z-ZZZ-000000'] = {'NewSection': '', 'NoteKey': []}
    for l_tok_key in sorted(g_occur_restricted.keys()):
        l_tok = g_occur_restricted[l_tok_key]

        l_section = l_tok['NewSection']
        l_note_key = l_tok['NoteKey']
        l_prefix = l_tok_key.split('-')[1]
        if l_prefix != l_current_prefix:
            if l_current_prefix:
                # print(f'{l_current_prefix}: {sorted(list(l_section_set))}')
                if l_section_set == g_sections_set:
                    print(f'No missing sections in [{l_current_prefix}] ({len(l_section_set)}/{len(g_sections_set)})')
                else:
                    l_missing_list = sorted(list(g_sections_set.difference(l_section_set)))
                    print(f'Missing sections in [{l_current_prefix}]: {len(l_missing_list)} {l_missing_list}')

                if l_notes_list == g_notes_list:
                    print(f'No missing notes in [{l_current_prefix}] ({len(l_notes_list)}/{len(g_notes_list)})')
                else:
                    for l_is, l_key_is, l_sec_is, l_should in zip(l_notes_list, l_notes_key_list, l_notes_section_list,
                                                                  g_notes_list):
                        if l_is != l_should:
                            print(f'Notes discrepancy [{l_current_prefix}] {l_sec_is}: [{l_key_is}]{l_is}/{l_should}')
                            break
                        else:
                            print(f'Notes consistency {l_sec_is}: [{l_key_is}]{l_is}')
                l_section_set = set()
                l_notes_list = []
            l_current_prefix = l_prefix

        # print(f'{l_tok_key}: {l_section}')
        if len(l_note_key) > 0:
            l_notes_list += [n for n in g_notes_dict[l_note_key]]
            l_notes_key_list += [l_note_key for n in g_notes_dict[l_note_key]]
            l_notes_section_list += [l_last_section for n in g_notes_dict[l_note_key]]
        if len(l_section) > 0:
            l_last_section = l_section
            l_section_set.add(l_section)

    del g_occur_restricted['Z-ZZZ-000000']


# print()
# print(re.sub(r"([A-Za-z])['’]([a-z])", r'\1⏅\2', g_raw_text_without_notes_or_markup))

def print_remaining_dashes():
    print('================= Remaining dashes =======================')
    for l_txt_code, l_idx, l_context in g_dashes_remaining:
        print(f'{l_txt_code} {l_idx:8} {l_context}')
    print()
    for l_txt_code, l_idx, l_context in g_dashes_remaining:
        l_context = l_context.replace('∥', '').strip()
        print(f"('{l_context}',")
        print(f"r'{l_context}'),")
    print()

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    # punctuation categories

    g_left_set = copy.deepcopy(g_min_left_set)
    g_mid_set = copy.deepcopy(g_min_mid_set)
    g_right_set = copy.deepcopy(g_min_right_set)

    for c in range(0x20, 0x2E7F):
        l_char = chr(c)
        l_category = unicodedata.category(l_char)
        # special case '
        # special case "
        if l_category[:1] == 'P' and l_char not in ["'", '"']:
            if l_category in ['Pd']:
                g_mid_set.add(l_char)
            elif l_category in ['Ps', 'Pi']:
                g_left_set.add(l_char)
            elif l_category in ['Pf', 'Pe']:
                g_right_set.add(l_char)

    g_left_punctuation = ''.join(list(g_left_set))
    g_mid_punctuation = ''.join(list(g_mid_set))
    g_right_punctuation = ''.join(list(g_right_set))
    g_all_punctuation = g_left_punctuation + g_mid_punctuation + g_right_punctuation

    print('g_left_punctuation :', g_left_punctuation)
    print('g_mid_punctuation  :', g_mid_punctuation)
    print('g_right_punctuation:', g_right_punctuation)

    # establish DB connection
    l_db_connection = psycopg2.connect(
        host=g_dbServer,
        database=g_dbDatabase,
        user=g_dbUser,
        password=g_dbPassword
    )

    l_cursor_list_shorey = None
    l_cursor_list_jowett = None
    l_cursor_list_greek = None

    # get cursors
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

    # Greek lemmas without diacritics (to check for Greek lemmatization errors)
    with open('./Dictionaries/lsj_lemmas.txt') as l_fin:
        g_greek_lemmas = set([remove_accents(l.rstrip()) for l in l_fin])
    with open('./Dictionaries/lsj_lemmas_noacc.txt', 'w') as l_fout:
        for l in sorted(list(g_greek_lemmas)):
            l_fout.write(f'{l}\n')

    l_do_english = True
    l_perform_phase_II = True
    l_do_greek = True

    # spaCy for English, Stanza (Stanford NLP) for Greek
    if l_perform_phase_II:
        g_nlp_en = spacy.load('en_core_web_trf')
        nltk.download('wordnet')
        g_nlp_en_nltk = WordNetLemmatizer()

    if l_do_english:
        # Shorey
        print('********** Processing Shorey', file=sys.stderr)
        process_cursor_list_en(l_cursor_list_shorey, 'A-SHR', p_phase_ii=l_perform_phase_II)

        # Jowett
        print('********** Processing Jowett', file=sys.stderr)
        process_cursor_list_en(l_cursor_list_jowett, 'B-JWT', p_phase_ii=l_perform_phase_II)

    # Greek
    if l_do_greek:
        if l_perform_phase_II:
            g_nlp_gr = stanza.Pipeline('grc')
        print('********** Processing Ancient Greek text', file=sys.stderr)
        process_cursor_list_gr(l_cursor_list_greek)

    print('********** Dumping unrestricted JSON files', file=sys.stderr)
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

    # ==================================================== Phase III ===================================================
    print('Phase III - Grouping punctuation into tokens', file=sys.stderr)
    group_punctuation()

    print('********** Control tables', file=sys.stderr)
    # print_lemma_2_words()
    # print_hyphens()
    # print_xpos()
    # print_entities()

    print_punctuation_groups()
    print_punctuation_groups_re()
    print_apostrophes()
    print_anno()
    print_pos_2_lemma()
    print_missing()
    print_remaining_dashes()
    print_morph_attr()

    # g_tokens_no_punctuation
    # with open('occ_no_punc.json', 'w') as f:
    #     json.dump(g_tokens_no_punctuation, f, indent=4, ensure_ascii=False)

    print('********** Dumping restricted JSON files', file=sys.stderr)
    with open('occ_restricted.json', 'w') as f:
        json.dump(g_occur_restricted, f, indent=4, ensure_ascii=False)

    with open('forms_restricted.json', 'w') as f:
        json.dump(g_form_restricted, f, indent=4, ensure_ascii=False)

    with open('lemmas_restricted.json', 'w') as f:
        json.dump(g_lemma_restricted, f, indent=4, ensure_ascii=False)
