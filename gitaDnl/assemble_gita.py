#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import json
import re
import subprocess
import os

g_indent = '    '
g_prefix_stack = []

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    with open('chap.json', 'r', encoding='utf-8') as l_f_in:
        # Load the JSON data from the file
        l_json_chapters = json.load(l_f_in)

    l_indent_prefix = ''
    with open('bg.xml', 'w', encoding='utf-8') as l_bg_out:
        for l_chap in l_json_chapters:
            l_verse_count = int(l_chap['verses_count'])
            l_chap_number = int(l_chap['chapter_number'])

            l_bg_out.write(f'{l_indent_prefix}<div1 type="chapter" n="{l_chap_number}">\n')
            g_prefix_stack.append(l_indent_prefix)
            l_indent_prefix += g_indent
            l_bg_out.write(f'{l_indent_prefix}<!-- Verses of chapter {l_chap_number} -->\n')

            print(f'Chapter {l_chap_number} ({l_verse_count} verses) +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++')

            #   <div type="chapter" n="1">
            # <!-- text of part 1, chapter 1 -->
            #   </div>

            for l_verse_number in range(1, l_verse_count + 1):
                l_cache_file = f'cache/v_{l_chap_number}_{l_verse_number}.json'

                if os.path.exists(l_cache_file):
                    print(f'Loading v. {l_verse_number} from cache')
                    with open(l_cache_file, 'r', encoding='utf-8') as l_f_v_json:
                        l_json_raw = l_f_v_json.read()
                else:
                    print(f'Downloading v. {l_verse_number}')
                    l_command = f"curl --request GET --url https://bhagavad-gita3.p.rapidapi.com/v2/chapters/{l_chap_number}/verses/{l_verse_number}/ --header 'x-rapidapi-host: bhagavad-gita3.p.rapidapi.com' --header 'x-rapidapi-key: c848c1cbbdmshf533af574fb945bp162be0jsnedd044de45f2'"
                    l_response = subprocess.run(l_command, capture_output=True, text=True, shell=True)
                    print(l_response.stderr)
                    l_json_raw = l_response.stdout
                    with open(l_cache_file, 'w', encoding='utf-8') as l_f_v_json:
                        l_f_v_json.write(l_json_raw)

                l_json_verse = json.loads(l_json_raw)

                l_sk = re.sub(r'\s+', ' ', l_json_verse['text'])
                l_translit = re.sub(r'\s+', ' ', l_json_verse['transliteration']).strip()
                l_wfw = [s.strip().split('—') for s in re.sub(r'\s+', ' ', l_json_verse['word_meanings']).split(';')]
                l_translations = l_json_verse['translations']
                l_commentaries = l_json_verse['commentaries']
                #                 for k in l_json_verse:
                #                     l_txt = re.sub(r'\s+', ' ', str(l_json_verse[k]))
                #                     print(f'    [{k:10}] {l_txt}')

                l_verse_id = f'BG_{l_chap_number}.{l_verse_number}'

                l_bg_out.write(f'{l_indent_prefix}<div2 type="verse-block" n="{l_verse_number}" xml:id="{l_verse_id}">\n')
                g_prefix_stack.append(l_indent_prefix)
                l_indent_prefix += g_indent

                l_sk_out = re.sub('\s*।\s*', '<caesura>', re.sub('।।', '॥', l_sk.strip()))
                l_bg_out.write(f'{l_indent_prefix}<l>{l_sk_out}</l>\n')
                l_bg_out.write(f'{l_indent_prefix}<seg type="transliteration">{l_translit}</seg>\n')

                print('    Sanskrit        :', l_sk)
                print('    Transliteration :', l_translit)
                print('    Word-for-word   :', l_wfw)

                print('    Translations    :', l_translations)
                l_bg_out.write(f'{l_indent_prefix}<div3 type="translations-block" xml:id="{l_verse_id}_tran">\n')
                g_prefix_stack.append(l_indent_prefix)
                l_indent_prefix += g_indent
                for t in l_translations:
                    l_text = re.sub(r'\s+', ' ', t['description']).strip()
                    l_author = t['author_name'].strip()
                    l_language = t['language'].strip()
                    print(f'      Description: ', l_text)
                    print(f'      Author     : ', l_author)
                    print(f'      Language   : ', l_language)
                    l_bg_out.write(f'{l_indent_prefix}<div4 type="translations" xml:lang="{l_language}">{l_text}<author>{l_author}</author></div4>\n')

                l_bg_out.write(f'{l_indent_prefix}<!-- End of translations block for {l_verse_id} -->\n')
                l_indent_prefix = g_prefix_stack.pop()
                l_bg_out.write(f'{l_indent_prefix}</div3>\n')

                print('    Commentaries    :', l_commentaries)
                l_bg_out.write(f'{l_indent_prefix}<div3 type="commentaries-block" xml:id="{l_verse_id}_com">\n')
                g_prefix_stack.append(l_indent_prefix)
                l_indent_prefix += g_indent
                for c in l_commentaries:
                    l_text = re.sub(r'\n', r'<br/>', c['description'].strip())
                    l_text = re.sub(r'\s+', ' ', l_text)
                    l_author = c['author_name'].strip()
                    l_language = c['language'].strip()
                    print(f'      Description: ', re.sub(r'\s+', ' ', c['description']))
                    print(f'      Author     : ', c['author_name'])
                    print(f'      Language   : ', c['language'])
                    l_bg_out.write(f'{l_indent_prefix}<div4 type="commentary" xml:lang="{l_language}">{l_text}<author>{l_author}</author></div4>\n')

                l_bg_out.write(f'{l_indent_prefix}<!-- End of commentaries block for {l_verse_id} -->\n')
                l_indent_prefix = g_prefix_stack.pop()
                l_bg_out.write(f'{l_indent_prefix}</div3>\n')

                l_bg_out.write(f'{l_indent_prefix}<!-- End of verse block {l_verse_number} -->\n')
                l_indent_prefix = g_prefix_stack.pop()
                l_bg_out.write(f'{l_indent_prefix}</div2>\n')


            l_bg_out.write(f'{l_indent_prefix}<!-- End of chapter {l_chap_number} -->\n')
            l_indent_prefix = g_prefix_stack.pop()
            l_bg_out.write(f'{l_indent_prefix}</div1>\n')
