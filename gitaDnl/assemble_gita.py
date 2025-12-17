#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import json
import re
import string
import subprocess
import os
import sys
import devtrans
import urllib.request
import html

from indic_transliteration import sanscript
from indic_transliteration.sanscript import transliterate

g_lexicon_gr_forms = dict()
g_lexicon_lemmas = dict()
g_lexicon_2_lemma = dict()
g_lexicon_lemma_2_form = dict()

g_count_grammar = 0
g_grammar_values = dict()
g_case_values = dict()
g_tense_values = dict()
g_voice_values = dict()

g_iskcon_missing = []
g_iskcon_total = 0

g_indent = '    '
g_prefix_stack = []

g_grammar_translation = {
    'abl.': 'CSABL',
    'acc.': 'CSACC',
    'dat.': 'CSDAT',
    'g.': 'CSGEN',
    'i.': 'CSINS',
    'loc.': 'CSLOC',
    'nom.': 'CSNOM',
    'voc.': 'CSVOC',
    'f.': 'GNFEM',
    'm.': 'GMASC',
    'n.': 'GNEUT',
    'ben.': 'MDBEN',
    'cond.': 'MDCND',
    'des.': 'MDDES',
    'imp.': 'MDIMP',
    'inj.': 'MDJUS',
    'int.': 'MDITS',
    'opt.': 'MDOPT',
    'subj.': 'MDSUB',
    'du.': 'NDUAL',
    'pl.': 'NPLUR',
    'sg.': 'NSING',
    'pfp.': 'VFPRT TNFUT VCPSS',
    'pfu.': 'VFPRT TNFUT VCACT',
    'pp.': 'VFPRT TNPST VCPSS',
    'ppa.': 'VFPRT TNPST VCACT',
    'ppf.': 'VFPRT TNPQP VCACT',
    'ppr.': 'VFPRT TNPRS VCACT',
    '1': 'PRSN1',
    '2': 'PRSN2',
    '3': 'PRSN3',
    'adv.': 'NPADV',
    'ind.': 'NPIND',
    'prep.': 'NPPRT',
    'iic.': 'NPIIC',
    'iiv.': 'NPIIV',
    'aor.': 'TNPST',
    'fut.': 'TNFUT',
    'impft.': 'TNIMP',
    'pft.': 'TNPQP',
    'pr.': 'TNPRS',
    '[1]': 'VBCJ1',
    '[2]': 'VBCJ2',
    '[3]': 'VBCJ3',
    '[4]': 'VBCJ4',
    '[5]': 'VBCJ5',
    '[6]': 'VBCJ6',
    '[7]': 'VBCJ7',
    '[8]': 'VBCJ8',
    '[9]': 'VBCJ9',
    '[10]': 'VCJ10',
    '[vn.]': 'VFVNN',
    'abs.': 'VFGER',
    'ca.': 'VFCAU',
    'inf.': 'VFINF',
    'ac.': 'VCACT',
    'mo.': 'VCMID',
    'ps.': 'VCPSS',
    'act.': '??ACT',
    'agt.': '??AGT',
    'per.': '??PER',
    'tasil': '??TSL',
    '*': 'GNFEM/GNEUT/GMASC'
}

g_color_2_pos = {
'carmin': 'NPVRB',
'deep_sky': 'NPNON',
'cyan': 'NPNON',
'light_blue': 'NPPRN',
'orange':  'NPIIV',
'yellow': 'NPIIC',
'pink': 'NPIIC',
'kaki':  'NPIIC',
'mauve': '__IND',
'lawngreen': '__VOC',
}
g_header = """<TEI xmlns="http://www.tei-c.org/ns/1.0">
<teiHeader xml:lang="en">
<titleStmt>
<title type="main">Bhagavad-gītā</title>
<title type="alt">श्रीमद् भगवद्गीता</title>
<title type="sub">Original Sanskrit text with word-for-word translation, morphological analysis (INRIA), 12 sloka-by-sloka translations and 22 sloka-by-sloka commentaries</title>
<editor role="compiler">Nicolas A. L. Reimen (nicolas.reimen@gmail.com, WhatsApp: +91 9900192517)</editor>
<author>अपौरुषेय + various translators and commentators</author>
</titleStmt>
</teiHeader>
<text xml:lang="en, sk">
"""

g_footer = """</text>
</TEI>
"""

g_author_normalize = {
    'Sri Abhinavgupta': 'Sri Abhinavagupta',
    'Sri Abhinav Gupta': 'Sri Abhinavagupta'
}

def author_normalize(p_author):
    if p_author in g_author_normalize.keys():
        return g_author_normalize[p_author]
    else:
        return p_author

#                   re.sub(r'॥(\d+\.\d+)॥(\S)', r'॥\1॥ \2',
#                   re.sub(r'॥\s*(\d+\.\d+)\s*॥', r'॥\1॥',

def sloka_numbers_normalize(s):
    return re.sub(r'॥(\d+\.\d+)॥(\S)', r'॥\1॥ \2',
                  re.sub(r'॥\s*(\d+\.\d+)\s*॥(\s*\.)', r'॥\1॥',
                         re.sub(r'^(<p>\s*)?(\d+\.\d+)\s*', r'\1॥\2॥ ',
                                re.sub(r'^(<p>\s*)?॥?\s*(\d+\.\d+)\s*-?\s*(\d+\.\d+)\s*॥?\s*', r'\1॥\2 - \3॥ ',s)
                                )
                         )
                  )

def universal_cleanup(s):
    return re.sub(r'\s+', ' ',
                         s.replace('।।', '॥')
                  ).strip()

def cleanup_add_br(s):
    return universal_cleanup(re.sub(r'\n', r'<br/>', s))

def author_key(s):
    return re.sub(f'[{string.punctuation}]+', '', universal_cleanup(s).lower()).replace(' ', '_')

def record_grammar(p_gr_list):
    global g_count_grammar
    global g_grammar_values
    global g_case_values
    global g_tense_values
    global g_voice_values

    l_bg = p_gr_list[0]
    g_grammar_values.setdefault(l_bg, dict())
    for l_gr_item in p_gr_list[1:]:
        if ' ' in l_gr_item:
            print('l_gr:     ', l_gr_item, file=sys.stderr)
            print('p_gr_list:', p_gr_list, file=sys.stderr)
            sys.exit(0)

        l_type_dict = g_grammar_values[l_bg]
        l_type_dict[l_gr_item] = l_type_dict.setdefault(l_gr_item, 0) + 1
        g_grammar_values[l_bg] = l_type_dict
        g_count_grammar += 1

        l_match_name = re.match('(m\.|f\.|n\.|\*) (sg\.|pl\.|du\.) (\S+)$', l_gr_item)
        if l_match_name:
            l_case = l_match_name.group(3)
            g_case_values[l_case] = g_case_values.setdefault(l_case, 0) + 1

        l_match_verb = re.match('(\S+\.) (\[\d+])? (\S+\.) (sg\.|pl\.|du\.) \d$', l_gr_item)
        if l_match_verb:
            l_tense = l_match_verb.group(1)
            l_voice = l_match_verb.group(3)
            g_tense_values[l_tense] = g_tense_values.setdefault(l_tense, 0) + 1
            g_voice_values[l_voice] = g_voice_values.setdefault(l_voice, 0) + 1

        if g_count_grammar > 10000000:
            dump_grammar_values()
            sys.exit(0)

def dump_grammar_values():
    for l_back in g_grammar_values.keys():
        print(f'{l_back}:')
        for l_key, l_value in sorted(list(g_grammar_values[l_back].items()), key=lambda l_item: l_item[1]):
            print(f'    {l_value:5} <{l_key}>')

    print('\nCases:')
    for l_key, l_value in sorted(list(g_case_values.items()), key=lambda l_item: l_item[1]):
        print(f'    {l_value:5} {l_key}')

    print('\nTenses:')
    for l_key, l_value in sorted(list(g_tense_values.items()), key=lambda l_item: l_item[1]):
        print(f'    {l_value:5} {l_key}')

    print('\nVoices:')
    for l_key, l_value in sorted(list(g_voice_values.items()), key=lambda l_item: l_item[1]):
        print(f'    {l_value:5} {l_key}')

g_grammar_abbr_dict = dict()

def store_grammar_values(p_gv_list):
    global g_grammar_abbr_dict

    for l_gv in p_gv_list:
        for l_grabr in l_gv.strip().split(' '):
            g_grammar_abbr_dict.setdefault(l_grabr, set()).add(l_gv)

    return p_gv_list

import random

def group_near_identical(p_grammar_values_split):
    l_grammar_values_split = p_grammar_values_split
    print(f'{l_grammar_values_split} group_near_identical START', file=sys.stderr)
    l_attempts = 0

    while True:
        l_found_one = False
        l_gv_len = len(l_grammar_values_split)
        for l_comp_1 in range(l_gv_len):
            if l_found_one:
                break

            l_gv_l1 = l_grammar_values_split[l_comp_1]
            for l_comp_2 in range(l_gv_len):
                if l_found_one:
                    break

                if l_comp_2 != l_comp_1:
                    l_gv_l2 = l_grammar_values_split[l_comp_2]

                    # if split values lists are of the same length
                    if len(l_gv_l1) == len(l_gv_l2):
                        print(f'    {"-".join(l_gv_l1)} {"-".join(l_gv_l2)} --> ', file=sys.stderr, end='')
                        # search if they are all the same except in on place
                        for l_i_diff in range(len(l_gv_l1)):
                            # (A) l_i_diff is the prospective index where the 2 lists differ while being identical at all other positions
                            l_all_others_same = True
                            # checks whether the list are indeed identical except at l_i_diff
                            for l_i_same in range(len(l_gv_l1)):
                                if l_i_same != l_i_diff and l_gv_l1[l_i_same] != l_gv_l2[l_i_same]:
                                    l_all_others_same = False
                                    break
                            # if condition (A) above is met
                            if l_all_others_same:
                                # create a new value of the form [a, b/c, d] from [a, b, d] and [a, c, d]
                                l_new_value_split = []
                                for l_i_same_or_diff in range(len(l_gv_l1)):
                                    if l_i_same_or_diff == l_i_diff:
                                        # different case
                                        # using sets to eliminate duplicates
                                        l_new_value_set_str = '/'.join(sorted(list(
                                            set(l_gv_l1[l_i_same_or_diff].split('/')).union(
                                                set(l_gv_l2[l_i_same_or_diff].split('/'))))))
                                        l_new_value_split.append(l_new_value_set_str)
                                    else:
                                        # same case
                                        l_new_value_split.append(l_gv_l1[l_i_same_or_diff])

                                l_found_one = True
                                l_grammar_values_split.remove(l_gv_l1)
                                l_grammar_values_split.remove(l_gv_l2)
                                l_grammar_values_split.append(l_new_value_split)
                                print(f'Success {["-".join(s) for s in l_grammar_values_split]} ({l_attempts})', file=sys.stderr)
                                break

                        print(f'failure {["-".join(s) for s in l_grammar_values_split]} ({l_attempts})', file=sys.stderr)
                        l_attempts += 1

        if not l_found_one:
            print(f'{l_grammar_values_split} group_near_identical END', file=sys.stderr)
            break

    return l_grammar_values_split

def inria_recur(p_inner, p_background, p_position, p_depth=0):
    print(f"{'.' * p_position}{p_inner[p_position:]} (p_depth: {p_depth})", file=sys.stderr)
    l_ret_list = []

    l_um = re.match(r'\[([^]]+)]\{\?}', p_inner)
    if l_um:
        l_ret_list = [{'unknown-form': l_um.group(1)}]
    else:
        while True:
            if p_position == len(p_inner):
                break

            if p_inner[p_position] == ']':
                p_position += 1
                break

            if p_inner[p_position] == '[':
                p_position, l_sub_ret_list = inria_recur(p_inner, p_background, p_position + 1, p_depth + 1)
                print(f'{" " * (p_depth*4)}SL {l_sub_ret_list}', file=sys.stderr)
                l_ret_list.append(l_sub_ret_list)
                continue

            l_match_ref = re.match(r'<a href="/(DICO|MW)/(\d+)\.html#([^"]+)"><i>([^"]+)</i></a>\s*', p_inner[p_position:])
            if l_match_ref:
                p_position += len(l_match_ref.group(0))
                l_dict_id = l_match_ref.group(1)
                l_dict_num = int(l_match_ref.group(2))
                l_dict_anchor = l_match_ref.group(3)
                l_dict_entry = l_match_ref.group(4)
                print(f'{" " * (p_depth*4)}RF {l_dict_id} {l_dict_num} {l_dict_anchor} {l_dict_entry}', file=sys.stderr)
                l_ret_list.append({'id': l_dict_id, 'num': l_dict_num, 'anchor': l_dict_anchor, 'entry': l_dict_entry})
                continue

            l_match_grammar = re.match(r'\{\s*([^}]+)\s*}', p_inner[p_position:])
            if l_match_grammar:
                p_position += len(l_match_grammar.group(0))
                l_grammar = l_match_grammar.group(1).strip()
                print(f'{" " * (p_depth*4)}GR {l_grammar} ({g_count_grammar})', file=sys.stderr)

                l_grammar_values = store_grammar_values([f'{s.strip()}' for s in l_grammar.split('|')])
                print(f'{l_grammar_values} l_grammar_values', file=sys.stderr)
                l_grammar_values_split = (
                    group_near_identical(
                        [
                            # split/join/split to address the problem caused by the presence of spaces in certain values of g_grammar_translation
                            (' '.join([g_grammar_translation[v] for v in l_gv_str.split(' ')])).split(' ')
                        for l_gv_str in l_grammar_values]))

                # using set() to remove duplicates
                l_grammar_values = ['-'.join(sorted(list(set(l_gvs)))) for l_gvs in l_grammar_values_split]
                l_pos_color = g_color_2_pos[p_background]
                l_grammar_l = [l_pos_color] + l_grammar_values

                record_grammar(l_grammar_l)

                l_ret_list.append({'grammar-list': l_grammar_l})
                continue

            # l_match_lone = re.match(r'([^\[\]{}]+)\s+', p_inner[p_position:])
            l_match_lone = re.match(r'([^{}\-\[\]]+)\s+', p_inner[p_position:])
            if l_match_lone:
                p_position += len(l_match_lone.group(0))
                l_lone_form = l_match_lone.group(1)
                print(f'{" " * (p_depth*4)}LF {l_lone_form} ({g_count_grammar})', file=sys.stderr)
                l_ret_list.append({'lone-form': l_lone_form})
                continue

            if p_inner[p_position] == '-':
                p_position += 1
                continue

    # print(f'   -> {l_ret_list}')
    return p_position, l_ret_list

def print_im(p_im, p_depth=0, p_out=sys.stdout):
    if type(p_im) is list:
        for l_item in p_im:
            l_repr = print_im(l_item, p_depth + 1, p_out)
            if l_repr:
                print(f"{' ' * (p_depth*4)}{l_repr}", file=p_out)
    else:
        return str(p_im)

def extract_im(p_im, p_depth=0):
    if type(p_im) is list:
        l_lemma_components = []
        g = ''
        for l_item in p_im:
            l_sub_comp_list, g = extract_im(l_item, p_depth + 1)
            if p_depth < 2:
                l_lemma_components += l_sub_comp_list
            # print(f"{' ' * (p_depth * 4)}{p_depth} {l} <{g}> {l_lemma_components}", file=sys.stderr)
        return l_lemma_components, g
    else: # can only be a dict
        if 'grammar-list' in p_im.keys():
            return [], p_im['grammar-list']
        elif 'entry' in p_im.keys():
            # https://sanskrit.inria.fr/MW/204.html#H_mukha
            # {'id': 'MW', 'num': 212, 'anchor': 'H_yudh#1', 'entry': 'yudh_1'}
            return [(p_im['entry'], f"https://sanskrit.inria.fr/{p_im['id']}/{p_im['num']}.html#{p_im['anchor']}")], ''
        elif 'lone-form' in p_im.keys():
            return [(p_im['lone-form'], '')], ''
        else:
            return [], ''


def inria_sloka_words(p_chap_number, p_sloka_num, p_sloka_txt):
    print(f'------------------------------------------- INRIA -----------------------------------------------------------')
    l_deva_txt = re.sub(r'\s+', ' ',  # normalize spaces
                        re.sub(r'\d+\.\d+$', '',  # removes sloka number at end (if in ascii digits)
                               re.sub(r'॥[^॥]+॥', '',  # removes sloka number at end (if in Devanagari digits
                                      re.sub(r'<[^>]+>', '',  # removes any HTML tag (like <br/>)
                                             p_sloka_txt.replace('\u200d', '').replace('।', '')
                                             # removes single danda and Zero Width Joiner (unicode 0x200D)
                                             )
                                      )
                               )
                        ).strip()  # strip = part of space normalization

    # replacing Devanagari sign nukta (U+093C), which should of course not appear in Velthuis but sometimes does
    l_txt_devtrans = devtrans.dev2vel(l_deva_txt).replace('\u093c', '')
    l_velthuis_text = transliterate(l_deva_txt, sanscript.DEVANAGARI, sanscript.VELTHUIS).replace('\u093c', '')

    # https://sanskrit.inria.fr/cgi-bin/SKT/sktgraph.cgi?
    # lex=MW&st=t&us=f&font=roma
    # &text=bhogai%22svaryaprasaktaanaa.m+tayaapah.rtacetasaam+vyavasaayaatmikaa+buddhi.h+samaadhau+na+vidhiiyate
    # &t=VH&topic=&mode=g&corpmode=&corpdir=&sentno=

    l_velthuis_text_url = l_velthuis_text.replace('"', '%22').replace(' ', '+')
    l_url = 'https://sanskrit.inria.fr/cgi-bin/SKT/sktgraph.cgi?' \
            'lex=MW&st=t&us=f&font=roma' \
            f"&text={l_velthuis_text_url}" \
            '&t=VH&topic=&mode=g&corpmode=&corpdir=&sentno='

    l_url = l_url.replace(' ', '+').replace('.a', '%27').replace('\u200c', '').replace('\u200d', '')
    print(f'l_deva_txt:      [{l_deva_txt}]')
    print(f'l_txt_devtrans:  [{l_txt_devtrans}]')
    print(f'l_velthuis_text: [{l_velthuis_text}]')
    l_url_disp = l_url.replace('&t', '\n&t')
    print(f'l_url:\n{l_url_disp}')

    l_inria_cache_path = f'cache/inria_{p_chap_number}_{p_sloka_num}.html'
    if os.path.exists(l_inria_cache_path):
        print(f'Loading inria for v. {p_sloka_num} from cache')
        with open(l_inria_cache_path, 'r', encoding='utf-8') as l_f_inria:
            l_html = l_f_inria.read()
    else:
        print(f'Downloading inria v. {p_chap_number}.{p_sloka_num}')

        l_page = urllib.request.urlopen(l_url)

        # Decode the bytes into a string using UTF-8
        l_html = l_page.read().decode('utf8').strip().replace('</table>', '</table>\n')
        l_html = re.sub('<!DOCTYPE html>.*<div class="latin12"><table class="center">',
                        '', l_html, flags=re.MULTILINE | re.DOTALL)

        with open(l_inria_cache_path, 'w', encoding='utf-8') as l_f_inria:
            l_f_inria.write(l_html)

    l_word_list = []
    l_line_end_list = []
    for l_match in re.finditer(r'</tr>\s*<tr>', l_html, flags=re.MULTILINE | re.DOTALL):
        l_line_end_list.append(l_match.start())

    l_line_end_list_disp = ' / '.join([f'{v:,}' for v in l_line_end_list])
    print(f'{len(l_line_end_list)} lines [{l_line_end_list_disp}]')

    for l_match in re.finditer(r'<table class="([^"]+)">\s*<tr><td onclick="showBox\(\'.*?</td></tr>', l_html):
        l_match_txt = l_match.group(0)
        l_background = l_match.group(1).replace('_back', '')

        l_position = l_match.start()
        l_sk_form = ''
        m = re.search(r'this, event\)">([^<]+)</td>', l_match_txt)
        if m:
            l_sk_form = html.unescape(m.group(1)).strip()

        # inner structure of showBox
        l_match_inner = re.search(r'onclick="showBox\(\'([^\']*?)\', \'#[^\']*\', this, event\)', l_match_txt)
        l_inria_morph = []
        if l_match_inner:
            l_inner = html.unescape(l_match_inner.group(1))
            l_inner = re.sub('\s*<a href="javascript:hideBox\(\)"> ✘</a>\s*', '', l_inner)
            l_inner_list = l_inner.split('<br>')
            # print('l_inner', l_inner)
            sys.stdout.flush()

            for l_inn in l_inner_list:
                print('=====================================================================================', file=sys.stderr)
                _, l_im = inria_recur(l_inn, l_background, 0)
                l_inria_morph.append(l_im)

            # for l_im in l_inria_morph:
                # print('********')
                # print(l_im)
                # print_im(l_im)

        l_word_list.append({'form': devtrans.dev2iast(l_sk_form), 'position': l_position, 'morph': l_inria_morph})

    print(f'-------------------------------------------------------------------------------------------------------------')
    return l_word_list


# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    # chapters block creation ==========================================================================================
    l_indent_prefix = ''

    l_bg_chapters_xml = ''
    with open('chap.json', 'r', encoding='utf-8') as l_f_in:
        # Load the JSON data from the file
        l_json_chapters = json.load(l_f_in)

    for l_chap in l_json_chapters:
        l_verse_count = int(l_chap['verses_count'])
        l_chap_number = int(l_chap['chapter_number'])

        if l_chap_number == 2:
            break

        # TEI chapter start (<div1>)
        l_bg_chapters_xml += f'{l_indent_prefix}<div1 type="chapter" n="{l_chap_number}">\n'
        g_prefix_stack.append(l_indent_prefix)
        l_indent_prefix += g_indent
        l_bg_chapters_xml += f'{l_indent_prefix}<!-- Verses of chapter {l_chap_number} -->\n'

        print(f'Chapter {l_chap_number} ({l_verse_count} verses) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++')

        for l_verse_number in range(1, l_verse_count + 1):
            l_verse_id = f'BG_{l_chap_number}.{l_verse_number}'

            # data from Gita Supersite
            l_cache_file_gs = f'cache/gs_{l_chap_number}_{l_verse_number}.html'

            l_url_gs = 'https://www.gitasupersite.iitk.ac.in/srimad?\n' + \
                'scsh=1&scang=1&scram=1&scanand=1&scjaya=1&scmad=1&scval=1&scms=1&scsri=1&scvv=1&scpur=1&scneel=1&scdhan=1&ecsiva=1&etsiva=1&etpurohit=1\n' + \
                f'&etgb=1&setgb=1&etssa=1&etassa=1&etradi=1&etadi=1&choose=1&&language=dv&field_chapter_value={l_chap_number}&field_nsutra_value={l_verse_number}'
            print('l_url_gs:', l_url_gs)
            l_url_gs = l_url_gs.replace('\n','')
            if os.path.exists(l_cache_file_gs):
                # if is in cache
                print(f'Loading Gita Supersite sloka {l_chap_number}.{l_verse_number} from cache')
                with open(l_cache_file_gs, 'r', encoding='utf-8') as l_f_v_gs:
                    l_html_gs = l_f_v_gs.read()
            else:
                # not in cache --> download
                print(f'Downloading Gita Supersite sloka {l_chap_number}.{l_verse_number}')

                l_headers = {
                    'User-Agent': 'ShradhaaBot 1.0'}
                l_req = urllib.request.Request(l_url_gs, headers=l_headers)
                l_page_gs = urllib.request.urlopen(l_req)
                # Decode the bytes into a string using UTF-8
                l_html_gs = l_page_gs.read().decode('utf8').strip()
                with open(l_cache_file_gs, 'w', encoding='utf-8') as l_f_v_gs:
                    l_f_v_gs.write(l_html_gs)

            l_sk_gs = ''
            l_translit_gs = ''
            l_match_sk = re.search(
                r'<div class="views-field views-field-body".*?<p align="center"><font size="3px">(.*?)</font>',
                l_html_gs, flags=re.MULTILINE | re.DOTALL)
            if l_match_sk:
                l_sk_gs = re.sub(r'\s+', ' ',
                                 re.sub(r'(<br\s*/>)+', '<br/>',
                                        re.sub(r'>\s+<', '><',
                                               l_match_sk.group(1).replace('।।', '॥')
                                               )
                                        )
                                 ).strip()
                l_translit_gs = transliterate(
                    re.sub(r'\s+', ' ',
                           re.sub(r'॥[^॥]+॥', '', l_sk_gs).replace('।', '').replace('<br/>', '')
                           ).strip(), sanscript.DEVANAGARI, sanscript.IAST)
                print(f'l_sk_gs:       {l_sk_gs}')
                print(f'l_translit_gs: {l_translit_gs}')

            # data from the BG API
            l_cache_file = f'cache/v_{l_chap_number}_{l_verse_number}.json'
            if os.path.exists(l_cache_file):
                # if is in cache
                print(f'Loading API sloka {l_chap_number}.{l_verse_number} from cache')
                with open(l_cache_file, 'r', encoding='utf-8') as l_f_v_json:
                    l_json_raw = l_f_v_json.read()
            else:
                # not in cache --> download
                print(f'Downloading API sloka {l_chap_number}.{l_verse_number}')
                l_command = f"curl --request GET --url https://bhagavad-gita3.p.rapidapi.com/v2/chapters/{l_chap_number}/verses/{l_verse_number}/ --header 'x-rapidapi-host: bhagavad-gita3.p.rapidapi.com' --header 'x-rapidapi-key: c848c1cbbdmshf533af574fb945bp162be0jsnedd044de45f2'"
                l_response = subprocess.run(l_command, capture_output=True, text=True, shell=True)
                print(l_response.stderr)
                l_json_raw = l_response.stdout
                with open(l_cache_file, 'w', encoding='utf-8') as l_f_v_json:
                    l_f_v_json.write(l_json_raw)

            # decode the JSON source
            l_json_verse = json.loads(l_json_raw)

            # top-level attributes
            # sanskrit text
            l_sk = universal_cleanup(l_json_verse['text'])
            # IAST transliteration
            l_translit = universal_cleanup(l_json_verse['transliteration'])
            # Word-for-Word translation (API version)
            l_wfw_api_list = [s.strip().split('—') for s in universal_cleanup(l_json_verse['word_meanings']).split(';')]
            # translations
            l_translations = l_json_verse['translations']
            # commentaries
            l_commentaries = l_json_verse['commentaries']

            # Data from the local DB
            # bhagavadgita_chapter_1_slok_1.json
            l_local_file = f'kagl/slok/bhagavadgita_chapter_{l_chap_number}_slok_{l_verse_number}.json'

            l_local_translations = dict()
            l_local_commentaries = dict()
            with open(l_local_file, 'r', encoding='utf-8') as l_f_local_json:
                print('=============================================', l_local_file)
                l_json_local = json.load(l_f_local_json)

                l_sk_local = l_json_local['slok']
                l_translit_local = l_json_local['transliteration']

                l_source = "Kaggle/Bhagavad Gita API Database"
                for k in l_json_local:
                    # l_txt = re.sub(r'\s+', ' ', str(l_json_local[k]))
                    # print(f'[{k:10}] {l_txt}')
                    if type(l_json_local[k]) is dict and 'author' in l_json_local[k]:
                        d = l_json_local[k]
                        l_author_local = author_normalize(universal_cleanup(d['author']))
                        l_type_list_local = set(d.keys()).difference({'author'})
                        print(l_type_list_local, d)
                        for l_type in l_type_list_local:
                            l, z = list(l_type)
                            l_language_local = 'english' if l == 'e' else 'hindi' if l == 'h' else 'sanskrit' if l == 's' else 'unknown language'
                            l_text = universal_cleanup(d[l_type])
                            l_author_key = author_key(l_author_local)
                            if z == 't':
                                l_local_translations[l_author_key] = [l_author_local, l_language_local, l_text, l_source]
                            else:
                                l_local_commentaries[l_author_key] = [l_author_local, l_language_local, l_text, l_source]
                print('=============================================', l_local_file)
                for n in l_local_translations.keys():
                    print(f'(t) {n:35}: {l_local_translations[n]}')
                for n in l_local_commentaries.keys():
                    print(f'(c) {n:35}: {l_local_commentaries[n]}')
                print('=============================================', l_local_file)

            # merging of commentaries and translations
            l_source = "Ved Vyas Foundation"
            for t in l_translations:
                l_text = universal_cleanup(t['description'])
                l_author = author_normalize(universal_cleanup(t['author_name']))
                l_language = universal_cleanup(t['language'])
                l_local_translations[author_key(l_author)] = [l_author, l_language, l_text, l_source]

            for c in l_commentaries:
                l_text = cleanup_add_br(c['description'])
                l_author = author_normalize(universal_cleanup(c['author_name']))
                l_language = universal_cleanup(c['language'])
                l_local_commentaries[author_key(l_author)] = [l_author, l_language, l_text, l_source]

            # Adding commentaries and translations from Gita Supersite to l_local_commentaries/l_local_translations
            # <div class="views-field views-field-field-etadi"
            l_html_gs_corrected = re.sub(r"([a-z]+)='([^']+)'", r'\1="\2"',
                                         re.sub(r'\s*=\s*', '=', l_html_gs)
                                         )
            l_gs_count_item = 0
            l_gs_count_com = 0
            l_gs_count_tran = 0
            l_source = "Gita Supersite"
            for l_match_gs_tc in re.finditer(
                    r'<div class="views-field views-field-field-([a-z]+).*?<font color="#2c44bd" size="4px"><b>([^<]+)</b></font></p>(.*?)</div>',
                    l_html_gs_corrected, flags=re.MULTILINE | re.DOTALL):
                l_code_gs = l_match_gs_tc.group(1).strip()
                l_title_gs = l_match_gs_tc.group(2).strip()
                l_body_gs = re.sub(r'[ \t]+', ' ',
                                   re.sub(r'<(/)?font[^<]*>', '',
                                   re.sub(r'<p[^<]+>', '<p>',
                                   re.sub(r'(<br\s*(/)?>)+', '',
                                          re.sub(r'>\s+<', '><',
                                                 l_match_gs_tc.group(3).replace('।।', '॥').replace('&nbsp;', ' ')
                                                 )
                                          )
                                          )
                                          ).replace('<b></b>', '')
                                   ).strip()

                l_body_gs_disp = re.sub(r'\s*\n\s*', '\n', l_body_gs)
                print(f'---------------------------- {l_gs_count_item} --------------------------------')
                print(f'l_code_gs:  {l_code_gs}')
                print(f'l_title_gs: {l_title_gs}')
                print(f'l_body_gs:  {l_body_gs_disp}')
                l_body_gs = universal_cleanup(l_body_gs)

                l_language_gs = 'english' if 'english' in l_title_gs.lower() else 'sanskrit' if 'sanskrit' in l_title_gs.lower() else 'unknown'

                l_is_commentary = ('commentary' in l_title_gs.lower())
                if l_is_commentary:
                    l_gs_count_com += 1
                else:
                    l_gs_count_tran += 1

                l_match_author = re.search(r'(?:[Bb]y\s*)+(.*)$', l_title_gs)
                if l_match_author:
                    l_author_gs = author_normalize(l_match_author.group(1).strip())
                else:
                    print('No Author')
                    sys.exit(0)

                if l_is_commentary and 'translation' in l_title_gs.lower():
                    l_match_original_author = re.search(r"^English Translation [Oo]f ((?:Sri )?[A-Z][a-z]+)'s", l_title_gs)
                    if l_match_original_author:
                        l_original_author = l_match_original_author.group(1)
                        l_original_author = 'Sri ' + l_original_author if 'sri' not in l_original_author.lower() else l_original_author
                        l_author_gs = author_normalize(l_original_author) + f' (transl. {l_author_gs})'

                if l_is_commentary:
                    l_local_commentaries[author_key(l_author_gs)] = [l_author_gs, l_language_gs, l_body_gs, l_source]
                else:
                    l_local_translations[author_key(l_author_gs)] = [l_author_gs, l_language_gs, l_body_gs, l_source]

                l_gs_count_item += 1
            print(f'Gita Supersite: {l_gs_count_com} commentaries and {l_gs_count_tran} translations')

            # get ISKCON data
            l_wfw_iskcon_list = []
            l_sk_iskcon = ''
            l_trl_iskcon = ''
            l_cache_file_iskcon = f'cache/iskcon_v_{l_chap_number}_{l_verse_number}.html'
            if os.path.exists(l_cache_file_iskcon):
                print(f'Loading ISKCON sloka {l_chap_number}.{l_verse_number} from cache {l_cache_file_iskcon}')
                with open(l_cache_file_iskcon, 'r', encoding='utf-8') as l_f_v_iskcon:
                    l_html_iskcon = l_f_v_iskcon.read()

                # <div class="em-mb-4 em-leading-8 em-text-lg text-center">धृतराष्ट्र उवाच<br>धर्मक्षेत्रे कुरुक्षेत्रे समवेता युयुत्सव: ।<br>मामका: पाण्डवाश्चैव किमकुर्वत सञ्जय ॥ १ ॥</div>
                l_match_sk_iskcon = re.search(r'<div class="em-mb-4 em-leading-8 em-text-lg text-center">(.*?)</div>', l_html_iskcon)
                if l_match_sk_iskcon:
                    l_sk_iskcon = l_match_sk_iskcon.group(1).replace(':', 'ः').strip()
                    # <div class="em-mb-4 em-leading-8 em-text-base text-center italic"><em>dhṛtarāṣṭra uvāca</em><br>dharma-kṣetre kuru-kṣetre<br>samavetā yuyutsavaḥ<br>māmakāḥ pāṇḍavāś caiva<br>kim akurvata sañjaya</div></div>

                l_match_trl_iskcon = re.search(r'<div class="em-mb-4 em-leading-8 em-text-base text-center italic">(.*?)</div>', l_html_iskcon)
                if l_match_trl_iskcon:
                    l_trl_iskcon = l_match_trl_iskcon.group(1).strip()
                    # <div class="av-synonyms">
                    #
                    # <div class="em-mb-4 em-leading-8 em-text-base text-justify">

                l_match_wfw_iskcon = re.search(r'<div class="av-synonyms">.*?<div class="em-mb-4 em-leading-8 em-text-base text-justify">(.*?)</div>', l_html_iskcon)
                if l_match_wfw_iskcon:
                    l_wfw_iskcon = re.sub(r'\s+', ' ',
                                         re.sub(r'<[^>]+>', ' ',
                                                l_match_wfw_iskcon.group(1)
                                                )
                                         ).strip()
                    l_wfw_iskcon_list = [(a.strip(), b.strip()) for a, b in [s.split('—') for s in l_wfw_iskcon.split(';')]]
                    print(f'l_wfw_iskcon: {l_wfw_iskcon_list}')
            else:
                l_sk_iskcon = '<unavailable>'
                l_trl_iskcon = '<unavailable>'
                g_iskcon_missing.append(f'{l_chap_number:2}.{l_verse_number}')

                # iskcon_pv_12.12
                l_cache_file_prabh = f'cache/iskcon_pv_{l_chap_number}.{l_verse_number}.json'
                if os.path.exists(l_cache_file_prabh):
                    print(f'Loading ISKCON sloka {l_chap_number}.{l_verse_number} from cache {l_cache_file_prabh}')
                    with open(l_cache_file_prabh, 'r', encoding='utf-8') as l_f_v_json:
                        l_json_raw = l_f_v_json.read()

                    # decode the JSON source
                    l_wfw_iskcon_list = [(a, b) for a,b in json.loads(l_json_raw)]
                else:
                    l_wfw_iskcon_list = []

            print(f'l_wfw_iskcon_list: {l_wfw_iskcon_list}')

            # test of Sanskrit consistency
            l_sk_from_trans = re.sub(r'\s+', ' ', devtrans.iast2dev(l_translit)).strip()
            l_sk_test_local = re.sub(r'\s+', ' ', re.sub(r'\|\|[^|]+\|\|$', '', l_sk_local).replace('|', '')).strip()
            l_sk_test = re.sub(r'\s+', ' ',
                               re.sub(r'\d+\.\d+$', '',
                                      l_sk.replace('।', '')
                                      )
                               ).strip()
            l_sk_gs_test = re.sub(r'\s+', ' ',
                                  re.sub(r'॥[^॥]+॥', '', l_sk_gs).replace('।', '').replace('<br/>', '')
                                  ).strip()
            # l_trl_iskcon_comp = re.sub(r'\s+', ' ',
            #                                            re.sub(r'<[^>]+>', ' ', l_trl_iskcon)
            #                                            ).strip()
            # l_sk_iskcon_from_trl = devtrans.iast2dev(l_trl_iskcon_comp).replace('-', '')

            print('-------------------------- SK consistency check -------------------------------------------------')
            print(f"l_sk_test:            [{l_sk_test.replace(' ', '').strip()}]")
            print(f"l_sk_gs_comp:         [{l_sk_gs_test.replace(' ', '').strip()}]")
            print(f"l_sk_test_local:      [{l_sk_test_local.replace(' ', '').strip()}]")
            if l_sk_test != l_sk_gs_test:
                print('!!!!!!!!!!!!!!!!!!!!!!!!!!! SK discrepancy !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
                print(f'l_sk_from_trans:      {l_sk_from_trans}')
                print(f'l_translit:           {l_translit}')
                print(f'l_translit_local:     {l_translit}')
                print(f'IAST local:           {devtrans.dev2iast(l_sk_test_local)}')
                print(f'l_sk_gs:              {l_sk_gs}')
                print(f'l_sk_iskcon:          {l_sk_iskcon}')
                print(f'l_trl_iskcon:         {l_trl_iskcon}')
                # print(f'l_trl_iskcon_comp:    {l_trl_iskcon_comp}')
                # print(f"l_sk_iskcon_from_trl: [{l_sk_iskcon_from_trl.replace(' ', '')}]")

                l_sk = l_sk_gs
                print(f'   --> New l_sk: {l_sk}')
                print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')

            # get grammatical analysis from INRIA
            l_wfw_inria_list = inria_sloka_words(l_chap_number, l_verse_number, l_sk)
            for l_wfw_inria in l_wfw_inria_list:
                print('INRIA:', l_wfw_inria)

            # ISKCON wfw print for comparison purposes
            for l_wfw_iskcon in l_wfw_iskcon_list:
                print(l_wfw_iskcon)

            # matching INRIA morphology with wfw translation
            l_matching_list = [(k, k, t, []) for k, t in l_wfw_iskcon_list]
            for l_wfw_inria in l_wfw_inria_list:
                l_form_inria = l_wfw_inria['form']

                l_matching_list_new = []
                for l_consumable_key_wfw, k, t, l_morph_list in l_matching_list:
                    if l_form_inria in l_consumable_key_wfw:
                        l_consumable_key_wfw = l_consumable_key_wfw.replace(l_form_inria, '')
                        l_morph_list.append(l_wfw_inria)

                    l_matching_list_new.append((l_consumable_key_wfw, k, t, l_morph_list))

                l_matching_list = l_matching_list_new

            l_wfw_list = [(k, t, m) for _, k, t, m in l_matching_list]

            print(f'--------------------- {l_chap_number}.{l_verse_number} --------------------------------')
            for k, t, m in l_wfw_list:
                print(f'-----------------------------------------------------------\n{k}: {t}')
                for d in m:
                    print(f"    {d['form']}", file=sys.stderr)
                    print_im(d['morph'], p_depth=1, p_out=sys.stderr)

            # start of TEI output for this sloka +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            # start of sloka block (<div2>)
            l_bg_chapters_xml += f'{l_indent_prefix}<div2 type="sloka-block" n="{l_verse_number}" xml:id="{l_verse_id}">\n'
            g_prefix_stack.append(l_indent_prefix)
            l_indent_prefix += g_indent

            l_sk_out = re.sub(r'\s+', ' ',
                              re.sub(r'॥\s*(\d+\.\d+)\s*॥', lambda m: f' ॥ {devtrans.wx2dev(m.group(1))} ॥',
                                     re.sub('\s*।\s*', '<caesura/>',
                                            re.sub(r'<[^>]+>', '',
                                                   l_sk.replace('।।', '॥')
                                                   )
                                            )
                                     )
                              ).strip()

            print('Sanskrit        :', l_sk)
            print('l_sk_out 1      :', l_sk.replace('।।', '॥'))
            print('l_sk_out        :', l_sk_out)
            print('Transliteration :', l_translit)
            l_bg_chapters_xml += f'{l_indent_prefix}<l>{l_sk_out}</l>\n'
            l_bg_chapters_xml += f'{l_indent_prefix}<seg type="transliteration" standard="IAST">{l_translit_gs}</seg>\n'

            # Adding WfW block to TEI file
            # start of wfw block (<div3>)
            l_bg_chapters_xml += f'{l_indent_prefix}<div3 type="wfw-block" xml:id="{l_verse_id}_wfw">\n'
            g_prefix_stack.append(l_indent_prefix)
            l_indent_prefix += g_indent

            for k, t, m in l_wfw_list:
                l_form_list = []
                print(f'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n{k}: {t}')
                l_interp_list = []
                for d in m:
                    l_form = d['form']
                    l_form_list.append(l_form)
                    l_alt_lemma = []
                    l_alt_pos = []
                    l_alt_ref = []
                    l_alt_gr = []
                    for l_alternative in d['morph']:
                        l_ll, l_gl = extract_im(l_alternative)
                        l_pos = l_gl[0] if len(l_gl) > 0 else ''

                        l_lemma_join = '।'.join([s for s, _ in l_ll])
                        l_ref_join = '।'.join([r for _, r in l_ll])
                        l_grammar_join = '।'.join(l_gl[1:])

                        if l_pos == '__IND':
                            l_pos = 'NPADV' if 'ADV' in l_grammar_join else 'NPIND'
                        if l_pos == '__VOC':
                            l_pos = 'NPNON'

                        l_alt_lemma.append(l_lemma_join)
                        l_alt_pos.append(l_pos)
                        l_alt_ref.append(l_ref_join)
                        l_alt_gr.append(l_grammar_join)

                        l_lemma_key = f'{l_lemma_join}/{l_pos}'
                        g_lexicon_gr_forms.setdefault(l_form, set(l_gl[1:])).union(set(l_gl[1:]))
                        g_lexicon_2_lemma.setdefault(l_form, {l_lemma_key}).add(l_lemma_key)
                        g_lexicon_lemmas.setdefault(l_lemma_key, {l_ref_join}).add(l_ref_join)
                        g_lexicon_lemma_2_form.setdefault(l_lemma_key, set()).add(l_form)

                        print(f"    {d['form']}")
                        print(f"        {l_lemma_join} {[r for _, r in l_ll]}")
                        print(f"        {l_pos}")
                        print(f"        {l_gl[1:]}")

                    l_interp_list.append(
                        f'{l_indent_prefix}        <interp type="morphology" lemma="{"॥".join(l_alt_lemma)}" '''
                        f'pos="{"॥".join(l_alt_pos)}" lemmaRef="{"॥".join(l_alt_ref)}" msd="{"॥".join(l_alt_gr)}">{d["form"]}</interp>\n'
                    )

                l_form = ' '.join(l_form_list)  # <interpGrp>
                l_interp_list_join = ''.join(l_interp_list)
                t = re.sub(rf'\s+([.?:;!])', r'\1', t)

                l_bg_chapters_xml += (f'{l_indent_prefix}<seg type="wfw-unit">{devtrans.iast2dev(k)}\n'
                               f'{l_indent_prefix}    <interp type="transliteration" source="ISKCON" standard="IAST">{k}</interp>\n'
                               f'{l_indent_prefix}    <interp type="transliteration" source="INRIA" standard="IAST">{l_form}</interp>\n'
                               f'{l_indent_prefix}    <interp type="translation" source="ISKCON">{t}</interp>\n'
                               f'{l_indent_prefix}    <interpGrp type="morphology" source="INRIA">\n' +
                               l_interp_list_join +
                               f'{l_indent_prefix}    </interpGrp>\n'
                               f'{l_indent_prefix}</seg>\n')

            # end of wfw block (<div3>)
            l_bg_chapters_xml += f'{l_indent_prefix}<!-- End of wfw block for {l_verse_id} -->\n'
            l_indent_prefix = g_prefix_stack.pop()
            l_bg_chapters_xml += f'{l_indent_prefix}</div3>\n'

            print('    Translations    :', l_translations)
            # Adding Translations to TEI file
            # start of translations block (<div3>)
            l_bg_chapters_xml += f'{l_indent_prefix}<div3 type="translations-block" xml:id="{l_verse_id}_tran">\n'
            g_prefix_stack.append(l_indent_prefix)
            l_indent_prefix += g_indent

            for l_ak in l_local_translations:
                l_author, l_language, l_text, lsource = l_local_translations[l_ak]
                l_text = sloka_numbers_normalize(l_text)
                print(f'      Description (t): ', l_text)
                print(f'      Author         : ', l_author)
                print(f'      Language       : ', l_language)
                print(f'      Source         : ', lsource)
                l_bg_chapters_xml += f'{l_indent_prefix}<div4 type="translation" source="{lsource}" xml:lang="{l_language}"><author>{l_author}</author>{l_text}</div4>\n'

            # end of translations block (<div3>)
            l_bg_chapters_xml += f'{l_indent_prefix}<!-- End of translations block for {l_verse_id} -->\n'
            l_indent_prefix = g_prefix_stack.pop()
            l_bg_chapters_xml += f'{l_indent_prefix}</div3>\n'

            print('    Commentaries    :', l_commentaries)
            # Adding Commentaries to TEI file
            # start of commentaries block (<div3>)
            l_bg_chapters_xml += f'{l_indent_prefix}<div3 type="commentaries-block" xml:id="{l_verse_id}_com">\n'
            g_prefix_stack.append(l_indent_prefix)
            l_indent_prefix += g_indent

            for l_ak in l_local_commentaries:
                l_author, l_language, l_text, lsource = l_local_commentaries[l_ak]
                l_text = sloka_numbers_normalize(l_text)
                print(f'      Description (c): ', l_text)
                print(f'      Author         : ', l_author)
                print(f'      Language       : ', l_language)
                print(f'      Source         : ', lsource)
                l_bg_chapters_xml += f'{l_indent_prefix}<div4 type="commentary" source="{lsource}" xml:lang="{l_language}"><author>{l_author}</author>{l_text}</div4>\n'

            # end of commentaries block (<div3>)
            l_bg_chapters_xml += f'{l_indent_prefix}<!-- End of commentaries block for {l_verse_id} -->\n'
            l_indent_prefix = g_prefix_stack.pop()
            l_bg_chapters_xml += f'{l_indent_prefix}</div3>\n'

            # end of sloka block (<div2>)
            l_bg_chapters_xml += f'{l_indent_prefix}<!-- End of verse block {l_verse_number} -->\n'
            l_indent_prefix = g_prefix_stack.pop()
            l_bg_chapters_xml += f'{l_indent_prefix}</div2>\n'

        # end of chapter block (<div1>)
        l_bg_chapters_xml += f'{l_indent_prefix}<!-- End of chapter {l_chap_number} -->\n'
        l_indent_prefix = g_prefix_stack.pop()
        l_bg_chapters_xml += f'{l_indent_prefix}</div1>\n'

    # Lexicon block creation ===========================================================================================
    l_bg_lexicon_xml = ''
    l_bg_lexicon_xml += f'{l_indent_prefix}<div1 type="lexicon">\n'
    g_prefix_stack.append(l_indent_prefix)
    l_indent_prefix += g_indent
    l_bg_lexicon_xml += f'{l_indent_prefix}<!-- Start of Lexicon -->\n'

    for l_lmk in sorted(list(g_lexicon_lemma_2_form.keys())):
        l_bg_lexicon_xml += f'{l_indent_prefix}<entryFree>\n'
        g_prefix_stack.append(l_indent_prefix)
        l_indent_prefix += g_indent

        l_lm, l_pos = l_lmk.split('/')

        l_bg_lexicon_xml += f'{l_indent_prefix}<pos>{l_pos}</pos>\n'
        l_bg_lexicon_xml += f'{l_indent_prefix}<form type="lemma" corresp="{"|".join(g_lexicon_lemmas[l_lmk])}"><orth>{l_lm}</orth></form>\n'
        for l_form in sorted(list(g_lexicon_lemma_2_form[l_lmk])):
            l_grammar_list = '|'.join(['-'.join(l) for l in
                              group_near_identical([s.split('-') for s in list(g_lexicon_gr_forms[l_form])])])
            l_bg_lexicon_xml += f'{l_indent_prefix}<form type="inflected"><orth>{l_form}</orth><gram>{l_grammar_list}</gram></form>\n'

        l_indent_prefix = g_prefix_stack.pop()
        l_bg_lexicon_xml += f'{l_indent_prefix}</entryFree>\n'

    l_bg_lexicon_xml += f'{l_indent_prefix}<!-- End of Lexicon -->\n'
    l_indent_prefix = g_prefix_stack.pop()
    l_bg_lexicon_xml += f'{l_indent_prefix}</div1>\n'

    # writing to files =================================================================================================
    with open('bg.xml', 'w', encoding='utf-8') as l_bg_file_out:
        # TEI header
        l_bg_file_out.write(g_header)

        # output lexicon
        l_bg_file_out.write(l_bg_lexicon_xml)

        # output chapters
        l_bg_file_out.write(l_bg_chapters_xml)

        # end of TEI file
        l_bg_file_out.write(g_footer)

    with open('bg_lex.xml', 'w', encoding='utf-8') as l_bg_file_out:
        # TEI header
        l_bg_file_out.write(g_header)

        # output lexicon
        l_bg_file_out.write(l_bg_lexicon_xml)

        # end of TEI file
        l_bg_file_out.write(g_footer)

    # dump grammar values
    dump_grammar_values()

    # list of missing ISKCON slokas (multiple slokas in one Vedabase block)
    print('\nMissing ISKCON slokas ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++')
    for l_ref in g_iskcon_missing:
        print(l_ref)

    print('\nGrammar Abbreviations --> INRIA_used_abbr.csv')
    l_ga_list = list(sorted(list(g_grammar_abbr_dict.keys())))
    with open("INRIA_used_abbr.csv", "w") as l_fout:
        for l_ga in l_ga_list:
            l_fout.write(f'{l_ga:4}; {g_grammar_abbr_dict[l_ga]}\n')

    print('\nLexicon Forms ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++')
    for l_form in g_lexicon_gr_forms.keys():
        print(f'{l_form} ====================')
        l_grammar_list = ['-'.join(l) for l in group_near_identical([s.split('-') for s in list(g_lexicon_gr_forms[l_form])])]
        for l_gr in sorted(l_grammar_list):
            print(f'    GR {l_gr}')
        for l_lm in sorted(list(g_lexicon_2_lemma[l_form])):
            print(f'    LM {l_lm}')
    print('\nLexicon Lemmas ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++')
    for l_lemma in g_lexicon_lemmas.keys():
        print(f'{l_lemma:35} --> {g_lexicon_lemmas[l_lemma]}')
