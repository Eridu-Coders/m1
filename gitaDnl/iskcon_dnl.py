#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import json
import re
import os
import sys
import urllib.request
import urllib.error

def universal_cleanup(s):
    return re.sub(r'\s+', ' ',
                         s.replace('।।', '॥')
                  ).strip()

g_wfw_anomalies = []

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    for l_chap_number in range(1, 19):
        # data from PRABHUPADABOOKS.COM ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        # https://prabhupadabooks.com/bg/1?d=1
        l_url_chap_prabh = f'https://prabhupadabooks.com/bg/{l_chap_number}?d=1'
        print(f'Chapter (PRABHUPADABOOKS) : {l_url_chap_prabh}')

        l_cache_file_prabh = f'cache/iskcon_p_chap_{l_chap_number}.html'
        if os.path.exists(l_cache_file_prabh):
            print(f'    Loading ISKCON PRABHUPADABOOKS Chapter {l_chap_number} from cache {l_cache_file_prabh}')
            with open(l_cache_file_prabh, 'r', encoding='utf-8') as l_f_v_iskcon:
                l_html_prabh_chap = l_f_v_iskcon.read()
        else:
            print(f'    Downloading ISKCON PRABHUPADABOOKS Chapter {l_chap_number} --> {l_cache_file_prabh}')
            try:
                l_headers = {
                    # 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/134.0.0.0'}
                    'User-Agent': 'ShraddhaaBot 1.0'}
                l_req = urllib.request.Request(l_url_chap_prabh, headers=l_headers)
                l_page_iskcon = urllib.request.urlopen(l_req)
                # Decode the bytes into a string using UTF-8
                l_html_prabh_chap = l_page_iskcon.read().decode('utf8').strip()
                with open(l_cache_file_prabh, 'w', encoding='utf-8') as l_f_v_iskcon:
                    l_f_v_iskcon.write(l_html_prabh_chap)
            except urllib.error.HTTPError as e:
                print(e.name)
                print(e.headers)
                sys.exit(0)

        # chap number checking
        l_match_check_chap = re.search(r'<div class="Chapter-Desc">(\d+)\.', l_html_prabh_chap)
        if l_match_check_chap:
            l_chap_num_check = int(l_match_check_chap.group(1))
            if l_chap_num_check != l_chap_number:
                print(f'Chapter num discrepancy {l_chap_number}/{l_chap_num_check}')
                sys.exit(0)
        else:
            print(f'No Chapter num found in {l_cache_file_prabh}')
            sys.exit(0)

        # Data extraction
        for l_match in re.finditer(
                r'<div class="Textnum">TEXTS? (\d+|\d+-\d+)</div>.*?<div class="Synonyms(?:-SA)?">(.*?)</div>',
                l_html_prabh_chap, flags=re.MULTILINE | re.DOTALL):
            l_sloka_group = l_match.group(1).strip()
            l_sloka_number_list = [int(l_sloka_group), int(l_sloka_group)] if '-' not in l_sloka_group else [int(s) for s in l_sloka_group.split('-')]

            l_wfw_html = l_match.group(2).strip()
            print(l_wfw_html)
            l_wfw_str = universal_cleanup(re.sub(r'<[^>]+>', '', l_match.group(2).strip()))
            l_wfw_prabh_list = [s.strip().split('—') for s in l_wfw_str.split(';')]
            print(f'{l_chap_number}.{str(l_sloka_number_list):11}: {l_wfw_str}')
            l_wfw = []
            for l in l_wfw_prabh_list:
                if len(l) == 2:
                    l_wfw.append((l[0], l[1]))
                else:
                    ll = l[0].split('-')
                    if len(ll) == 2:
                        l_wfw.append((ll[0], ll[1]))
                    else:
                        if len(l) == 3:
                            l_left, l_mid, l_right = l
                            if ',' in l_mid:
                                m1, m2 = l_mid.split(',')
                                l_wfw.append((l_left, m1.strip()))
                                l_wfw.append((m2.strip(), l_right))
                                continue
                            elif ':' in l_mid:
                                m1, m2 = l_mid.split(':')
                                l_wfw.append((l_left, m1.strip()))
                                l_wfw.append((m2.strip(), l_right))
                                continue
                            elif '.' in l_mid:
                                m1, m2 = l_mid.split('.')
                                l_wfw.append((l_left, m1.strip()))
                                l_wfw.append((m2.strip(), l_right))
                                continue
                            elif ' ' in l_mid:
                                m1, m2 = l_mid.split(' ')
                                l_wfw.append((l_left, m1.strip()))
                                l_wfw.append((m2.strip(), l_right))
                                continue
                            elif l_mid == '':
                                l_wfw.append((l_left, ))
                                continue

                        g_wfw_anomalies.append((f'{l_chap_number}.{l_sloka_group}', l))
                        l_wfw.append(l)
                        print(l)

            l_start, l_end = l_sloka_number_list
            for l_sloka_number in range(l_start, l_end + 1):
                # l_translit = l_translit_list[l_sloka_number - l_start]
                #                 if l_start != l_end:
                #                     l_wfw = []
                #                     while True:
                #                         print(l_translit, l_wfw_prabh_list)
                #                         l_sk, l_trad = l_wfw_prabh_list.pop(0)
                #                         if l_sk == l_translit[:len(l_sk)]:
                #                             l_translit = re.sub(f'^{l_sk}\s*', '', l_translit).strip()
                #                             l_wfw.append((l_sk, l_trad))
                #                         else:
                #                             break
                print(f'{l_chap_number}.{l_sloka_number:3}: {l_wfw}')
                l_cache_file_pv = f'cache/iskcon_pvn_{l_chap_number}.{l_sloka_number}.json'
                with open(l_cache_file_pv, 'w', encoding='utf-8') as l_f_v_prabh:
                    json.dump(l_wfw, l_f_v_prabh, indent=4, ensure_ascii=False)


        # data from AISTIS.COM +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        # https://asitis.com/1/15.html
        l_url_chap_asitis = f'https://asitis.com/{l_chap_number}'
        print(f'Chapter (AISTIS): {l_url_chap_asitis}')

        try:
            l_headers = {
                #'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/134.0.0.0'}
                'User-Agent': 'ShraddhaaBot 1.0'}
            l_req = urllib.request.Request(l_url_chap_asitis, headers=l_headers)
            l_page_iskcon = urllib.request.urlopen(l_req)
            # Decode the bytes into a string using UTF-8
            l_html_chapter = l_page_iskcon.read().decode('utf8').strip()
            # print(l_html_chapter)
        except urllib.error.HTTPError as e:
            print(e.name)
            print(e.headers)
            sys.exit(0)

        # <h4><a href='https://asitis.com/1/1.html'>Chapter 1, Verse 1</a></h4>
        for l_match in re.finditer(rf"<h4><a href='https://asitis\.com/({l_chap_number}/[^.]+\.html)'>", l_html_chapter):
            l_id_sloka_path = l_match.group(1).strip()
            l_id_sloka = l_id_sloka_path.replace('/', '.').replace('.html', '')
            l_url_sloka = f'https://asitis.com/{l_id_sloka_path}'
            # print(l_url_sloka)

            l_cache_file_iskcon = f'cache/iskcon_a_v_{l_id_sloka}.html'
            if os.path.exists(l_cache_file_iskcon):
                print(f'    Loading AISTIS sloka {l_id_sloka} from cache')
                with open(l_cache_file_iskcon, 'r', encoding='utf-8') as l_f_v_iskcon:
                    l_html_iskcon = l_f_v_iskcon.read()
            else:
                print(f'    Downloading AISTIS sloka {l_id_sloka} --> {l_cache_file_iskcon}')
                try:
                    l_headers = {
                        # 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/134.0.0.0'}
                        'User-Agent': 'ShraddhaaBot 1.0'}
                    l_req = urllib.request.Request(l_url_sloka, headers=l_headers)
                    l_page_iskcon = urllib.request.urlopen(l_req)
                    # Decode the bytes into a string using UTF-8
                    l_html_iskcon = l_page_iskcon.read().decode('utf8').strip()
                    with open(l_cache_file_iskcon, 'w', encoding='utf-8') as l_f_v_iskcon:
                        l_f_v_iskcon.write(l_html_iskcon)
                except urllib.error.HTTPError as e:
                    print(e.name)
                    print(e.headers)
                    sys.exit(0)

    print('============================== WfW Anomalies ============================================')
    for x in g_wfw_anomalies:
        print(x)
