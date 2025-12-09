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

g_indent = '    '
g_prefix_stack = []

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

g_count_grammar = 0
g_grammar_values = dict()
g_case_values = dict()
g_tense_values = dict()
g_voice_values = dict()

def record_grammar(p_gr_list):
    global g_count_grammar
    global g_grammar_values
    global g_case_values
    global g_tense_values
    global g_voice_values

    l_bg = p_gr_list[0]
    for l_gr in p_gr_list[1:]:
        l_type_dict = g_grammar_values.setdefault(l_bg, dict())
        l_type_dict[l_gr] = l_type_dict.setdefault(l_gr, 0) + 1
        g_grammar_values[l_bg] = l_type_dict
        g_count_grammar += 1

        l_match_name = re.match('(m\.|f\.|n\.|\*) (sg\.|pl\.|du\.) (\S+)$', l_gr)
        if l_match_name:
            l_case = l_match_name.group(3)
            g_case_values[l_case] = g_case_values.setdefault(l_case, 0) + 1

        l_match_verb = re.match('(\S+\.) (\[\d+])? (\S+\.) (sg\.|pl\.|du\.) \d$', l_gr)
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
            print(f'    {l_value:5} {l_key}')

    print('\nCases:')
    for l_key, l_value in sorted(list(g_case_values.items()), key=lambda l_item: l_item[1]):
        print(f'    {l_value:5} {l_key}')

    print('\nTenses:')
    for l_key, l_value in sorted(list(g_tense_values.items()), key=lambda l_item: l_item[1]):
        print(f'    {l_value:5} {l_key}')

    print('\nVoices:')
    for l_key, l_value in sorted(list(g_voice_values.items()), key=lambda l_item: l_item[1]):
        print(f'    {l_value:5} {l_key}')


def inria_recur(p_inner, p_background, p_pos, p_depth=0):
    # print(f"{'.' * p_pos}{p_inner[p_pos:]} (p_depth: {p_depth})")
    l_ret_list = []

    l_um = re.match(r'\[([^]]+)]\{\?}', p_inner)
    if l_um:
        l_ret_list = [{'unknown-form': l_um.group(1)}]
    else:
        while True:
            if p_pos == len(p_inner):
                break

            if p_inner[p_pos] == ']':
                p_pos += 1
                break

            if p_inner[p_pos] == '[':
                p_pos, l_sub_ret_list = inria_recur(p_inner, p_background, p_pos + 1, p_depth + 1)
                # print(f'{" " * (p_depth*4)}SL {l_sub_ret_list}')
                l_ret_list.append(l_sub_ret_list)
                continue

            l_match_ref = re.match(r'<a href="/(DICO|MW)/(\d+)\.html#([^"]+)"><i>([^"]+)</i></a>\s*', p_inner[p_pos:])
            if l_match_ref:
                p_pos += len(l_match_ref.group(0))
                l_dict_id = l_match_ref.group(1)
                l_dict_num = int(l_match_ref.group(2))
                l_dict_anchor = l_match_ref.group(3)
                l_dict_entry = l_match_ref.group(4)
                # print(f'{" " * (p_depth*4)}RF {l_dict_id} {l_dict_num} {l_dict_anchor} {l_dict_entry}')
                l_ret_list.append({'id': l_dict_id, 'num': l_dict_num, 'anchor': l_dict_anchor, 'entry': l_dict_entry})
                continue

            l_match_grammar = re.match(r'\{\s*([^}]+)\s*}', p_inner[p_pos:])
            if l_match_grammar:
                p_pos += len(l_match_grammar.group(0))
                l_grammar = l_match_grammar.group(1).strip()
                # print(f'{" " * (p_depth*4)}GR {l_grammar} ({g_count_grammar})')
                l_grammar_l = [p_background] + [f'{s.strip()}' for s in l_grammar.split('|')]

                record_grammar(l_grammar_l)

                l_ret_list.append({'grammar-list': l_grammar_l})
                continue

            # l_match_lone = re.match(r'([^\[\]{}]+)\s+', p_inner[p_pos:])
            l_match_lone = re.match(r'([^{}\-\[\]]+)\s+', p_inner[p_pos:])
            if l_match_lone:
                p_pos += len(l_match_lone.group(0))
                l_lone_form = l_match_lone.group(1)
                # print(f'{" " * (p_depth*4)}LF {l_lone_form} ({g_count_grammar})')
                l_ret_list.append({'lone-form': l_lone_form})
                continue

            if p_inner[p_pos] == '-':
                p_pos += 1
                continue

    # print(f'   -> {l_ret_list}')
    return p_pos, l_ret_list

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
        l_lemma = []
        g = ''
        for l_item in p_im:
            l, g = extract_im(l_item, p_depth + 1)
            if p_depth < 3:
                l_lemma += l
            # print(f"{' ' * (p_depth * 4)}{p_depth} {l} <{g}> {l_lemma}", file=sys.stderr)
        return l_lemma, g
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

        l_pos = l_match.start()
        l_form = ''
        m = re.search(r'this, event\)">([^<]+)</td>', l_match_txt)
        if m:
            l_form = html.unescape(m.group(1)).strip()

        # inner structure of showBox
        l_match_inner = re.search(r'onclick="showBox\(\'([^\']*?)\', \'#[^\']*\', this, event\)', l_match_txt)
        if l_match_inner:
            l_inner = html.unescape(l_match_inner.group(1))
            l_inner = re.sub('\s*<a href="javascript:hideBox\(\)"> ✘</a>\s*', '', l_inner)
            l_inner_list = l_inner.split('<br>')
            # print('l_inner', l_inner)
            sys.stdout.flush()

            l_inria_morph = []
            for l_inn in l_inner_list:
                _, l_im = inria_recur(l_inn, l_background, 0)
                l_inria_morph.append(l_im)

            # for l_im in l_inria_morph:
                # print('********')
                # print(l_im)
                # print_im(l_im)

        l_word_list.append({'form': devtrans.dev2iast(l_form), 'position': l_pos, 'morph': l_inria_morph})

    print(f'-------------------------------------------------------------------------------------------------------------')
    return l_word_list


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

g_iskcon_missing = []
g_iskcon_total = 0

g_author_normalize = {
    'Sri Abhinavgupta': 'Sri Abhinavagupta',
    'Sri Abhinav Gupta': 'Sri Abhinavagupta'
}

def author_normalize(p_author):
    if p_author in g_author_normalize.keys():
        return g_author_normalize[p_author]
    else:
        return p_author

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    with open('chap.json', 'r', encoding='utf-8') as l_f_in:
        # Load the JSON data from the file
        l_json_chapters = json.load(l_f_in)

    l_indent_prefix = ''
    with open('bg.xml', 'w', encoding='utf-8') as l_bg_out:
        l_bg_out.write(g_header)
        for l_chap in l_json_chapters:
            l_verse_count = int(l_chap['verses_count'])
            l_chap_number = int(l_chap['chapter_number'])
            g_iskcon_total += l_verse_count

            l_bg_out.write(f'{l_indent_prefix}<div1 type="chapter" n="{l_chap_number}">\n')
            g_prefix_stack.append(l_indent_prefix)
            l_indent_prefix += g_indent
            l_bg_out.write(f'{l_indent_prefix}<!-- Verses of chapter {l_chap_number} -->\n')

            print(f'Chapter {l_chap_number} ({l_verse_count} verses) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++')

            for l_verse_number in range(1, l_verse_count + 1):
                l_cache_file = f'cache/v_{l_chap_number}_{l_verse_number}.json'
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
                                    l_local_translations[l_author_key] = [l_author_local, l_language_local, l_text]
                                else:
                                    l_local_commentaries[l_author_key] = [l_author_local, l_language_local, l_text]
                    print('=============================================', l_local_file)
                    for n in l_local_translations.keys():
                        print(f'(t) {n:35}: {l_local_translations[n]}')
                    for n in l_local_commentaries.keys():
                        print(f'(c) {n:35}: {l_local_commentaries[n]}')
                    print('=============================================', l_local_file)

                # merging of commentaries and translations
                for t in l_translations:
                    l_text = universal_cleanup(t['description'])
                    l_author = author_normalize(universal_cleanup(t['author_name']))
                    l_language = universal_cleanup(t['language'])
                    l_local_translations[author_key(l_author)] = [l_author, l_language, l_text]

                for c in l_commentaries:
                    l_text = cleanup_add_br(c['description'])
                    l_author = author_normalize(universal_cleanup(c['author_name']))
                    l_language = universal_cleanup(c['language'])
                    l_local_commentaries[author_key(l_author)] = [l_author, l_language, l_text]

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
                #                 l_trl_iskcon_comp = re.sub(r'\s+', ' ',
                #                                            re.sub(r'<[^>]+>', ' ', l_trl_iskcon)
                #                                            ).strip()
                #                 l_sk_iskcon_from_trl = devtrans.iast2dev(l_trl_iskcon_comp).replace('-', '')

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
                #                     print(f'l_trl_iskcon:         {l_trl_iskcon}')
                #                     print(f'l_trl_iskcon_comp:    {l_trl_iskcon_comp}')
                #                     print(f"l_sk_iskcon_from_trl: [{l_sk_iskcon_from_trl.replace(' ', '')}]")
                #
                    l_sk = l_sk_gs
                    print(f'   --> New l_sk: {l_sk}')
                    print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
                #                     #sys.exit(0)

                # Adding commentaries and translations from Gita Supersite to l_local_commentaries/l_local_translations
                # <div class="views-field views-field-field-etadi"
                l_html_gs_corrected = re.sub(r"([a-z]+)='([^']+)'", r'\1="\2"',
                                             re.sub(r'\s*=\s*', '=', l_html_gs)
                                             )
                l_gs_count_item = 0
                l_gs_count_com = 0
                l_gs_count_tran = 0
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

                    l_match_author = re.search(r'(?:(?:B|b)y\s*)+(.*)$', l_title_gs)
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
                        l_local_commentaries[author_key(l_author_gs)] = [l_author_gs, l_language_gs, l_body_gs]
                    else:
                        l_local_translations[author_key(l_author_gs)] = [l_author_gs, l_language_gs, l_body_gs]

                    l_gs_count_item += 1
                print(f'Gita Supersite: {l_gs_count_com} commentaries and {l_gs_count_tran} translations')

                # get grammatical analysis from INRIA
                l_wfw_inria_list = inria_sloka_words(l_chap_number, l_verse_number, l_sk)
                for l_wfw_inria in l_wfw_inria_list:
                    print('INRIA:', l_wfw_inria)

                # start of output for this sloka
                l_bg_out.write(f'{l_indent_prefix}<div2 type="sloka-block" n="{l_verse_number}" xml:id="{l_verse_id}">\n')
                g_prefix_stack.append(l_indent_prefix)
                l_indent_prefix += g_indent

                # if l_iskcon_unavailable:
                #     l_sk_out = re.sub('\s*।\s*', '<caesura>', re.sub('।।', '॥', l_sk.strip()))
                # else:
                l_sk_out = re.sub(r'\s+', ' ',
                                  re.sub(r'॥\s*(\d+\.\d+)\s*॥', lambda m: f' ॥ {devtrans.wx2dev(m.group(1))} ॥',
                                         re.sub('\s*।\s*', '<caesura>',
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
                l_bg_out.write(f'{l_indent_prefix}<l>{l_sk_out}</l>\n')
                l_bg_out.write(f'{l_indent_prefix}<seg type="transliteration" standard="IAST">{l_translit_gs}</seg>\n')

                #                 if not l_iskcon_unavailable:
                #                     for l_wfw_api in l_wfw_iskcon_list:
                #                         print(l_wfw_api)
                #
                #                     # matching INRIA morphology with wfw translation
                #                     l_matching_list = [(k, k, t, []) for k, t in l_wfw_iskcon_list]
                #                     for l_wfw_inria in l_wfw_inria_list:
                #                         l_form = l_wfw_inria['form']
                #
                #                         l_matching_list_new = []
                #                         for l_consumable_key, k, t, l_morph_list in l_matching_list:
                #                             if l_form in l_consumable_key:
                #                                 l_consumable_key = l_consumable_key.replace(l_form, '')
                #                                 l_morph_list.append(l_wfw_inria)
                #
                #                             l_matching_list_new.append((l_consumable_key, k, t, l_morph_list))
                #                         l_matching_list = l_matching_list_new
                #
                #                     l_wfw_list = [(k, t, m) for _, k, t, m in l_matching_list]
                #
                #                     print(f'--------------------- {l_chap_number}.{l_verse_number} --------------------------------', file=sys.stderr)
                #                     for k, t, m in l_wfw_list:
                #                         print(f'-----------------------------------------------------------\n{k}: {t}', file=sys.stderr)
                #                         for d in m:
                #                             print(f"    {d['form']}", file=sys.stderr)
                #                             print_im(d['morph'], p_depth=1, p_out=sys.stderr)
                #
                #                     # assemble form-lemma
                #                     l_bg_out.write(f'{l_indent_prefix}<div3 type="wfw-block" xml:id="{l_verse_id}_wfw">\n')
                #                     g_prefix_stack.append(l_indent_prefix)
                #                     l_indent_prefix += g_indent
                #
                #                     for k, t, m in l_wfw_list:
                #                         l_form_list = []
                #                         print(f'+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n{k}: {t}', file=sys.stderr)
                #                         l_interp_list = []
                #                         for d in m:
                #                             l_form_list.append(d['form'])
                #                             l_lemma_list, l_grammar_list = extract_im(d['morph'])
                #                             l_color = l_grammar_list[0] if len(l_grammar_list) > 0 else ''
                #                             l_pos = 'XXX'
                #                             if l_color == 'carmin':
                #                                 l_pos = 'VERB'
                #                             elif l_color == 'deep_sky':
                #                                 l_pos = 'NOUN'
                #                             elif l_color == 'light_blue':
                #                                 l_pos = 'PRON'
                #                             elif l_color == 'yellow':
                #                                 l_pos = 'PROPN'
                #                             elif l_color == 'lawngreen':
                #                                 l_pos = 'ADJ'
                #                             elif l_color == 'pink':
                #                                 l_pos = 'ADV'
                #                             elif l_color == 'cyan':
                #                                 l_pos = 'NOUN'
                #                             elif l_color == 'mauve':
                #                                 l_pos = 'ADP-CCONJ-SCONJ'
                #                             l_lemma_join = '-'.join([s for s,_ in l_lemma_list])
                #                             l_ref_join = '|'.join([r for _,r in l_lemma_list])
                #                             l_grammar_join = '|'.join(l_grammar_list[1:])
                #                             print(f"    {d['form']}", file=sys.stderr)
                #                             print(f"        {l_lemma_join} {[r for _,r in l_lemma_list]}", file=sys.stderr)
                #                             print(f"        {l_pos}-{l_color}", file=sys.stderr)
                #                             print(f"        {l_grammar_list[1:]}", file=sys.stderr)
                #                             l_interp_list.append(
                #                                 f'{l_indent_prefix}        <interp type="morphology" lemma="{l_lemma_join}" '''
                #                                 f'pos="{l_pos}" lemmaRef="{l_ref_join}" msd="{l_grammar_join}">{d["form"]}</interp>\n'
                #                             )
                #
                #                         l_form = ' '.join(l_form_list) # <interpGrp>
                #                         l_interp_list_join = ''.join(l_interp_list)
                #                         t = re.sub(rf'\s+([.?:;!])', r'\1', t)
                #                         l_bg_out.write(f'{l_indent_prefix}<seg type="wfw-unit">{devtrans.iast2dev(k)}\n'
                #                                        f'{l_indent_prefix}    <interp type="transliteration" standard="IAST">{k}</interp>\n'
                #                                        #f'{l_indent_prefix}    <interp type="transliteration" standard="IAST">{l_form}</interp>\n'
                #                                        f'{l_indent_prefix}    <interp type="translation" source="ISKCON">{t}</interp>\n'
                #                                        f'{l_indent_prefix}    <interpGrp type="morphology" source="INRIA">\n' +
                #                                        l_interp_list_join +
                #                                        f'{l_indent_prefix}    </interpGrp>\n'
                #                                        f'{l_indent_prefix}</seg>\n')
                #
                #                     l_bg_out.write(f'{l_indent_prefix}<!-- End of wfw block for {l_verse_id} -->\n')
                #                     l_indent_prefix = g_prefix_stack.pop()
                #                     l_bg_out.write(f'{l_indent_prefix}</div3>\n')

                print('    Translations    :', l_translations)
                l_bg_out.write(f'{l_indent_prefix}<div3 type="translations-block" xml:id="{l_verse_id}_tran">\n')
                g_prefix_stack.append(l_indent_prefix)
                l_indent_prefix += g_indent

                for l_ak in l_local_translations:
                    l_author, l_language, l_text = l_local_translations[l_ak]
                    l_text = sloka_numbers_normalize(l_text)
                    print(f'      Description (t): ', l_text)
                    print(f'      Author         : ', l_author)
                    print(f'      Language       : ', l_language)
                    l_bg_out.write(f'{l_indent_prefix}<div4 type="translation" xml:lang="{l_language}"><author>{l_author}</author>{l_text}</div4>\n')

                l_bg_out.write(f'{l_indent_prefix}<!-- End of translations block for {l_verse_id} -->\n')
                l_indent_prefix = g_prefix_stack.pop()
                l_bg_out.write(f'{l_indent_prefix}</div3>\n')

                print('    Commentaries    :', l_commentaries)
                l_bg_out.write(f'{l_indent_prefix}<div3 type="commentaries-block" xml:id="{l_verse_id}_com">\n')
                g_prefix_stack.append(l_indent_prefix)
                l_indent_prefix += g_indent

                for l_ak in l_local_commentaries:
                    l_author, l_language, l_text = l_local_commentaries[l_ak]
                    l_text = sloka_numbers_normalize(l_text)
                    print(f'      Description (c): ', l_text)
                    print(f'      Author         : ', l_author)
                    print(f'      Language       : ', l_language)
                    l_bg_out.write(f'{l_indent_prefix}<div4 type="commentary" xml:lang="{l_language}"><author>{l_author}</author>{l_text}</div4>\n')

                l_bg_out.write(f'{l_indent_prefix}<!-- End of commentaries block for {l_verse_id} -->\n')
                l_indent_prefix = g_prefix_stack.pop()
                l_bg_out.write(f'{l_indent_prefix}</div3>\n')

                l_bg_out.write(f'{l_indent_prefix}<!-- End of verse block {l_verse_number} -->\n')
                l_indent_prefix = g_prefix_stack.pop()
                l_bg_out.write(f'{l_indent_prefix}</div2>\n')

            l_bg_out.write(f'{l_indent_prefix}<!-- End of chapter {l_chap_number} -->\n')
            l_indent_prefix = g_prefix_stack.pop()
            l_bg_out.write(f'{l_indent_prefix}</div1>\n')

        # dump grammar values
        dump_grammar_values()
        print(f'({len(g_iskcon_missing)}/{g_iskcon_total}) {g_iskcon_missing}')

        # end of TEI file
        l_bg_out.write(g_footer)

#                 l_cache_file_iskcon = f'cache/iskcon_v_{l_chap_number}_{l_verse_number}.html'
#                 l_iskcon_unavailable = False
#                 if os.path.exists(l_cache_file_iskcon):
#                     print(f'Loading ISKCON sloka {l_chap_number}.{l_verse_number} from cache')
#                     with open(l_cache_file_iskcon, 'r', encoding='utf-8') as l_f_v_iskcon:
#                         l_html_iskcon = l_f_v_iskcon.read()
#                 else:
#                     print(f'Downloading ISKCON sloka {l_chap_number}.{l_verse_number}')
#                     l_url_iskcon = f'https://vedabase.io/en/library/bg/{l_chap_number}/{l_verse_number}/'
#                     print(l_url_iskcon)
#                     try:
#                         l_headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/134.0.0.0'}
#                         l_req = urllib.request.Request(l_url_iskcon, headers=l_headers)
#                         l_page_iskcon = urllib.request.urlopen(l_req)
#                         # Decode the bytes into a string using UTF-8
#                         l_html_iskcon = l_page_iskcon.read().decode('utf8').strip()
#                         with open(l_cache_file_iskcon, 'w', encoding='utf-8') as l_f_v_iskcon:
#                             l_f_v_iskcon.write(l_html_iskcon)
#                     except urllib.error.HTTPError:
#                         l_iskcon_unavailable = True
#                         g_iskcon_missing.append(f'{l_chap_number}.{l_verse_number}')
#
#                 # <div class="em-mb-4 em-leading-8 em-text-lg text-center">धृतराष्ट्र उवाच<br>धर्मक्षेत्रे कुरुक्षेत्रे समवेता युयुत्सव: ।<br>मामका: पाण्डवाश्चैव किमकुर्वत सञ्जय ॥ १ ॥</div>
#                 l_match_sk_iskcon = re.search(r'<div class="em-mb-4 em-leading-8 em-text-lg text-center">(.*?)</div>', l_html_iskcon)
#                 if l_match_sk_iskcon:
#                     l_sk_iskcon = l_match_sk_iskcon.group(1).replace(':', 'ः').strip()
#                 # <div class="em-mb-4 em-leading-8 em-text-base text-center italic"><em>dhṛtarāṣṭra uvāca</em><br>dharma-kṣetre kuru-kṣetre<br>samavetā yuyutsavaḥ<br>māmakāḥ pāṇḍavāś caiva<br>kim akurvata sañjaya</div></div>
#                 l_match_trl_iskcon = re.search(r'<div class="em-mb-4 em-leading-8 em-text-base text-center italic">(.*?)</div>', l_html_iskcon)
#                 if l_match_trl_iskcon:
#                     l_trl_iskcon = l_match_trl_iskcon.group(1).strip()
#
#                 # <div class="av-synonyms">
#                 #
#                 # <div class="em-mb-4 em-leading-8 em-text-base text-justify">
#                 l_match_wfw_iskcon = re.search(r'<div class="av-synonyms">.*?<div class="em-mb-4 em-leading-8 em-text-base text-justify">(.*?)</div>', l_html_iskcon)
#                 if l_match_wfw_iskcon:
#                     l_wfw_iskcon = re.sub(r'\s+', ' ',
#                                           re.sub(r'<[^>]+>', ' ',
#                                                  l_match_wfw_iskcon.group(1)
#                                                  )
#                                           ).strip()
#                     l_wfw_iskcon_list = [(a.strip(), b.strip()) for a, b in [s.split('—') for s in l_wfw_iskcon.split(';')]]
#                     # print(f'l_wfw_iskcon: {l_wfw_iskcon_list}')