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
import urllib.error
import html

from indic_transliteration import sanscript
from indic_transliteration.sanscript import transliterate

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    for l_chap_number in range(1, 19):
        # https://vedabase.io/en/library/bg/1/
        l_url_chap = f'https://vedabase.io/en/library/bg/{l_chap_number}/1'

        print(f'Chapter: {l_url_chap}')

        try:
            l_headers = {
                #'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/134.0.0.0'}
                'User-Agent': 'YetAnotherBot 1.0'}
            l_req = urllib.request.Request(l_url_chap, headers=l_headers)
            l_page_iskcon = urllib.request.urlopen(l_req)
            # Decode the bytes into a string using UTF-8
            l_html_chapter = l_page_iskcon.read().decode('utf8').strip()
        except urllib.error.HTTPError as e:
            print(e.name)
            print(e.headers)
            sys.exit(0)

        for l_match in re.finditer(rf'<a href="(/en/library/bg/{l_chap_number}/\d+/)"', l_html_chapter):
            l_url_sloka = f'https://vedabase.io/{l_match.group(1)}'
            print(l_url_sloka)


        # l_cache_file_iskcon = f'cache/iskcon_v_{l_chap_number}_{l_verse_number}.html'
        #         if os.path.exists(l_cache_file_iskcon):
        #             print(f'Loading ISKCON sloka {l_chap_number}.{l_verse_number} from cache')
        #             with open(l_cache_file_iskcon, 'r', encoding='utf-8') as l_f_v_iskcon:
        #                 l_html_iskcon = l_f_v_iskcon.read()
        #         else:
        #             print(f'Downloading ISKCON sloka {l_chap_number}.{l_verse_number}')
        #             l_url_iskcon = f'https://vedabase.io/en/library/bg/{l_chap_number}/{l_verse_number}/'
        #             print(l_url_iskcon)
        #             try:
        #                 l_headers = {
        #                     'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/134.0.0.0 Safari/537.36 Edg/134.0.0.0'}
        #                 l_req = urllib.request.Request(l_url_iskcon, headers=l_headers)
        #                 l_page_iskcon = urllib.request.urlopen(l_req)
        #                 # Decode the bytes into a string using UTF-8
        #                 l_html_iskcon = l_page_iskcon.read().decode('utf8').strip()
        #                 with open(l_cache_file_iskcon, 'w', encoding='utf-8') as l_f_v_iskcon:
        #                     l_f_v_iskcon.write(l_html_iskcon)
        #             except urllib.error.HTTPError:
        #                 l_iskcon_unavailable = True
        #                 g_iskcon_missing.append(f'{l_chap_number}.{l_verse_number}')