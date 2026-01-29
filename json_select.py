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

# g_section_selection = ['476c', '476d', '476e']
# g_section_selection = ['476c', '476d', '476e', '477a', '477b', '477c', '477d', '477e']
g_section_selection = ['476', '477', '478', '479', '480', '485', '486']

from gram_codes_package.gram_codes_module import g_gram_to_pentacode

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    for l_key, l_value in g_gram_to_pentacode.items():
        print(f'{l_key:20}: {l_value}')

    with open('lemmatization/occ_compacted.json', 'r') as f:
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
            "Pos": g_gram_to_pentacode['Pos-' + g_lemma_dict[l_key]["Pos"]],
            "Text": g_lemma_dict[l_key]["Text"]}

    for l_key in g_form_dict.keys():
        try:
            l_new_tag = g_gram_to_pentacode['Tag-' + g_form_dict[l_key]["Tag"]]
        except KeyError:
            l_new_tag = g_gram_to_pentacode['Pos-' + g_form_dict[l_key]["Tag"]]
        g_form_dict[l_key] = {
            "LemmaKey": g_form_dict[l_key]["LemmaKey"],
            "Tag": l_new_tag,
            "Text": g_form_dict[l_key]["Text"]}

    l_shorey_occ = dict()
    l_jowett_occ = dict()
    l_greek_occ = dict()

    l_prefix_count = dict()
    l_prefix_count_is = dict()
    l_shorey_cur_sec = None
    l_jowett_cur_sec = None
    l_greek_cur_sec = None

    l_cur_section = None
    l_sec_in_sel = False
    l_in_sel = False

    l_section_selection = set()

    l_key_set = set()
    l_form_key_list = []
    l_lemma_key_list = []
    l_entity_key_list = []
    for l_key_occ in sorted(g_occurences.keys()):
        l_occ = g_occurences[l_key_occ]
        l_key_prefix = l_key_occ[:5]
        l_prefix_count[l_key_prefix] = l_prefix_count.setdefault(l_key_prefix, 0) + 1

        if len(l_occ['NewSection']) > 0:
            l_cur_section = l_occ['NewSection']
            l_sec_in_sel = l_cur_section[:-1] in g_section_selection

        if l_occ['SentencePos'] in ['SS', 'SX'] and l_sec_in_sel:
            l_in_sel = True

        # print(f'{l_key_prefix:3} {l_cur_section:4} {"X" if l_in_sel else "_"}')
        if l_in_sel:
            l_section_selection.add(l_cur_section)
            l_prefix_count_is[l_key_prefix] = l_prefix_count_is.setdefault(l_key_prefix, 0) + 1

            l_key_set.add(l_key_occ)
            l_form_key_list.append(l_occ['FormKey'])
            l_lemma_key_list += g_form_dict[l_occ['FormKey']]['LemmaKey']

            if l_key_prefix == 'A-SHR':
                l_shorey_occ[l_key_occ] = l_occ
            elif l_key_prefix == 'B-JWT':
                l_jowett_occ[l_key_occ] = l_occ
            elif l_key_prefix == 'Z-GRK':
                l_greek_occ[l_key_occ] = l_occ

        if l_occ['SentencePos'] in ['SE', 'SX'] and not l_sec_in_sel:
            l_in_sel = False

    for l_key_prefix in sorted(l_prefix_count.keys()):
        print(l_key_prefix, f'{l_prefix_count[l_key_prefix]:7,} {l_prefix_count_is[l_key_prefix]:7,}')

    l_republic = {
        'Title': 'The Republic',
        'Author': 'Plato',
        'GroundTextVersion': 'Greek',
        'Sections': sorted(list(l_section_selection)),
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