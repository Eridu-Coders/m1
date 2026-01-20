#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import json

# input collections
g_occurences = dict()       # word occurrences. Key = XXX-nnn, with XXX = 'SHR' (Shorey), 'JWT' (Jowett) or 'GRC' (Greek)
g_form_dict = dict()        # word forms. Key = <form>‣POS, with POS = tag_ for Spacy (English) and upos for Stanza (Greek)
g_lemma_dict = dict()       # lemmas. Key = <lemma>‣POS, with POS = pos_ for Spacy (English) and upos for Stanza (Greek)
g_entities_dict = dict()    # named entities. Key = 'LOC', 'NORP' or 'PERSON'
g_notes_dict = dict()       # notes (from Shorey). Key = SHR-nnn, same as occurrence key of same toke

g_section_selection = ['476c', '476d', '476e', '477a', '477b', '477c', '477d', '477e']
# g_section_selection = ['476c', '476d', '476e']

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
g_nlp_pos_dict = dict([(b, a) for a, b, _ in g_nlp_pos_list])
# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    with open('lemmatization/occ_restricted.json', 'r') as f:
        g_occurences = json.load(f)
    with open('lemmatization/notes.json', 'r') as f:
        g_notes_dict = json.load(f)
    with open('lemmatization/forms_restricted.json', 'r') as f:
        g_form_dict = json.load(f)
    with open('lemmatization/lemmas_restricted.json', 'r') as f:
        g_lemma_dict = json.load(f)
    with open('lemmatization/entities.json', 'r') as f:
        g_entities_dict = json.load(f)

    for l_key in g_lemma_dict.keys():
        g_lemma_dict[l_key] = {
            "EntType": g_lemma_dict[l_key]["EntType"],
            "Pos": g_nlp_pos_dict[g_lemma_dict[l_key]["Pos"]],
            "Text": g_lemma_dict[l_key]["Text"]}

    l_shorey_occ = dict()
    l_jowett_occ = dict()
    l_greek_occ = dict()

    l_prefix_count = dict()
    l_shorey_cur_sec = None
    l_jowett_cur_sec = None
    l_greek_cur_sec = None
    l_key_set = set()
    l_form_key_list = []
    l_lemma_key_list = []
    l_entity_key_list = []
    for l_key_occ in sorted(g_occurences.keys()):
        l_occ = g_occurences[l_key_occ]
        l_key_prefix = l_key_occ[:5]
        l_prefix_count[l_key_prefix] = l_prefix_count.setdefault(l_key_prefix, 0) + 1
        if l_key_prefix == 'A-SHR':
            l_shorey_cur_sec = l_occ['NewSection'] if len(l_occ['NewSection']) > 0 else l_shorey_cur_sec
            if l_shorey_cur_sec in g_section_selection:
                l_key_set.add(l_key_occ)
                l_form_key_list.append(l_occ['FormKey'])
                l_lemma_key_list+= g_form_dict[l_occ['FormKey']]['LemmaKey']
                l_shorey_occ[l_key_occ] = l_occ
        elif l_key_prefix == 'B-JWT':
            l_jowett_cur_sec = l_occ['NewSection'] if len(l_occ['NewSection']) > 0 else l_jowett_cur_sec
            if l_jowett_cur_sec in g_section_selection:
                l_key_set.add(l_key_occ)
                l_form_key_list.append(l_occ['FormKey'])
                l_lemma_key_list+= g_form_dict[l_occ['FormKey']]['LemmaKey']
                l_jowett_occ[l_key_occ] = l_occ
        elif l_key_prefix == 'Z-GRK':
            l_greek_cur_sec = l_occ['NewSection'] if len(l_occ['NewSection']) > 0 else l_greek_cur_sec
            if l_greek_cur_sec in g_section_selection:
                l_key_set.add(l_key_occ)
                l_form_key_list.append(l_occ['FormKey'])
                l_lemma_key_list+= g_form_dict[l_occ['FormKey']]['LemmaKey']
                l_greek_occ[l_key_occ] = l_occ


    for l_key_prefix in sorted(l_prefix_count.keys()):
        print(l_key_prefix, f'{l_prefix_count[l_key_prefix]:7,}')

    l_republic = {
        'Title': 'The Republic',
        'Author': 'Plato',
        'Sections': g_section_selection,
        'Notes': dict([(l_note_key, g_notes_dict[l_note_key]) for l_note_key in g_notes_dict if l_note_key in l_key_set]),
        'Forms': dict([(l_form_key, g_form_dict[l_form_key]) for l_form_key in l_form_key_list]),
        'Lemmas': dict([(l_lemma_key, g_lemma_dict[l_lemma_key]) for l_lemma_key in sorted(l_lemma_key_list)]),
        'Versions': {
            'Shorey': l_shorey_occ,
            'Jowett': l_jowett_occ,
            'Greek': l_greek_occ,
        }
    }

    with open('republic.json', 'w') as f:
        json.dump(l_republic, f, indent=4, ensure_ascii=False, sort_keys=True)