#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import re
import devtrans
import urllib.request
import html
import sys

import xml.etree.ElementTree as ET
from collections import namedtuple
import copy

g_fout = None

Sloka = namedtuple('Sloka', ['id', 'form', 'lemma', 'translit'])

def get_xml_source(p):
    return re.sub(r'[ \t]+', ' ',
                  re.sub('xmlns:ns0="[^"]+"', '',
                         ET.tostring(p, encoding='utf-8').decode('utf8').replace('ns0:', ''))).strip()
def process_sloka(p_s):
    """

    :param p_s: the sentence (sloka) to process, as an ET node
    :return: nothing
    """
    g_fout.write('<s>\n')
    for r in p_s.iter('{http://www.tei-c.org/ns/1.0}ref'):
        print(r.text, '/', r.get('cRef'))
        g_fout.write(f'{get_xml_source(r)}\n')
    for l in p_s.iter('{http://www.tei-c.org/ns/1.0}lg'):
        g_fout.write(f'{get_xml_source(l)}\n')

    l_word_dict = dict()
    for w in p_s.iter('{http://www.tei-c.org/ns/1.0}w'):
        # print('------------------------------------------------')
        l_id = None
        for k in w.keys():
            if '}id' in k:
                l_id = w.get(k)
        l_lemma = w.get('lemma')
        l_form = w.text
        l_translit = None
        for t in w.iter('{http://www.tei-c.org/ns/1.0}interp'):
            if t.get('type') == 'transliteration':
                l_translit = t.text

        l_xml_source = get_xml_source(w)
        print('   ', l_id, l_form, l_lemma, l_translit, l_xml_source)
        l_word_dict[l_id] = Sloka(l_id, l_form, l_lemma, l_translit)
        g_fout.write(f'{l_xml_source}\n')
        # print(w.attrib)
        # print(ET.dump(w))
        # print('++++++++++++++++++++++++++++++++++++++++++++++++')

    print(l_word_dict)
    l_wfw_dict = dict()
    for l_anno in p_s.iter('{http://www.tei-c.org/ns/1.0}annotation'):
        if l_anno.get('type') == 'lexicon':
            print(l_anno.get('source'))
            l_source = l_anno.get('source')
            l_lex_text = l_anno.text.replace(',', ';')
            l_word_dict_copy = copy.deepcopy(l_word_dict)
            for l_lex_entry in l_lex_text.split(';'):
                if len(l_lex_entry.strip()) > 0:
                    print('   ', l_lex_entry.strip(), end=' --> ')
                    l_frm_lex = l_lex_entry.split(':')[0].strip().replace('-','')
                    l_tr_lex = l_lex_entry.split(':')[1].strip()
                    print(f'{l_frm_lex} / {l_tr_lex}')
                    l_k_found = []
                    for k in l_word_dict_copy.keys():
                        l_w_translit = l_word_dict_copy[k].translit
                        if l_w_translit in l_frm_lex:
                            print('      found:', l_word_dict[k])
                            l_k_found.append(k)
                        if re.search(f'{l_w_translit}$', l_frm_lex):
                            for l_k_del in l_k_found:
                                print(f'      delete {l_word_dict_copy[l_k_del]}')
                                del l_word_dict_copy[l_k_del]
                            l_wfw_dict.setdefault(f'from="{l_k_found[0]}" to="{l_k_found[-1]}"', []).append(
                                f'<interp type="wfw-tr" source="{l_source}">{l_tr_lex}</interp>')
                            break

    for k in sorted(list(l_wfw_dict.keys())):
        g_fout.write(f'<span type="wfw-tu" {k}>\n')
        for t in l_wfw_dict[k]:
            g_fout.write(f'{t}\n')
        g_fout.write('</span>\n')

    for i in p_s.iter('{http://www.tei-c.org/ns/1.0}interp'):
        if i.get('type') in ['bhashya', 'translation']:
            g_fout.write(f'{get_xml_source(i)}\n')
    g_fout.write('</s>\n')


# ---------------------------------------------------- Main section ----------------------------------------------------
if __name__ == "__main__":
    g_fout = open('BG12-TEI+.xml', 'w')
    with open('BG12-final.xml', 'r') as l_fin:
        l_xml_string = l_fin.read()
        # print(l_xml_string)
        l_xml_root = ET.fromstring(l_xml_string)

        for l_header in l_xml_root.iter('{http://www.tei-c.org/ns/1.0}teiHeader'):
            g_fout.write(f'{get_xml_source(l_header)}\n')

        for l_s in l_xml_root.iter('{http://www.tei-c.org/ns/1.0}s'):
            process_sloka(l_s)
