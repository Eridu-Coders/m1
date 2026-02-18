#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import sys
import xml.etree.ElementTree as ET
import re
import cydifflib as difflib
import devtrans
import copy

g_parallel = dict()
g_parallel_bhashya = dict()

# for brahmasūtrabhāṣyam
# def anandamak(p_file):
#     with open(p_file, 'r') as l_fin:
#         l_txt = l_fin.read().replace('<br>', '<br/>')
#         print('Anandamak:', p_file, len(l_txt))
#         try:
#             l_tree = ET.fromstring(f'{l_txt}')
#         except ET.ParseError as e:
#             print(f'XML parse ERROR: {e.code} {e.msg} in [{l_txt[:100]}]')
#
#         for l_div_adhyaya in l_tree.findall('div'):
#             print(l_div_adhyaya.attrib['data-name'])
#             for l_div_paada in l_div_adhyaya.findall('div'):
#                 print('   ', l_div_paada.attrib['title'].strip())
#                 for l_section in l_div_paada.findall('section'):
#                     print('       ', l_section.attrib['title'].strip())
#                     for l_div_verse in l_section.findall('div'):
#                         # <div class="verse" id="BS_C01_S01_V01" type="sutra" data-adhikaranaid="BS_C01_S01_A001" data-adhikarana="jijñāsādhikaraṇam">
#                         l_verse_id_0 = l_div_verse.attrib['id'].strip()
#                         l_verse_id = re.sub(r'^BS_', '',
#                                             re.sub(r'_[CSV]0?', '_', l_verse_id_0))
#                         # verse-text
#                         l_verse_text = l_div_verse.find("div[@class='verse-text']/p").text
#                         g_parallel.setdefault(l_verse_id, dict())['Anandamak'] = l_verse_text
#                         print('           >', l_verse_id_0, l_verse_id, l_div_verse.attrib['data-adhikarana'].strip(), l_verse_text)
#                         for l_div_bhashya_collection in l_div_verse.findall('div'):
#                             if l_div_bhashya_collection.attrib['class'] == 'bhashya-collection':
#                                  for l_div_bhashya in l_div_bhashya_collection.findall('div'):
#                                      if l_div_bhashya.attrib['class'].strip() == 'bhashya':
#                                          l_text = l_div_bhashya.text.strip()
#                                          if len(l_text) > 0:
#                                             print('               ', l_div_bhashya.attrib['id'], l_text)
#                                          else:
#                                              print('               ', l_div_bhashya.attrib['class'], l_div_bhashya.attrib['id'])
#                                              for l_div_p_in_bhashya in l_div_bhashya.findall('p'):
#                                                  print('                   ', l_div_p_in_bhashya.text)
#                                      else:
#                                          print('               ', l_div_bhashya.attrib['class'])
#                                          for l_div_bhashya_in_pramana in l_div_bhashya.findall('div'):
#                                              print('                   ', l_div_bhashya_in_pramana.attrib['class'])
#                                              for l_div_p_in_pramana in l_div_bhashya_in_pramana.findall('p'):
#                                                  print('                       ', l_div_p_in_pramana.text)

# for śrīmadbhagavadgītābhāṣyam
g_id_prefix = 'BGB'

def amak_text_cleanup(p_full_bhashya_text):
    def equalize_space_comma(s):
        return re.sub(r",\s*(\d)", r", \1", s)

    l_full_bhashya_text_r = re.sub(r'(?:</?br/?>)+', '<br/>', p_full_bhashya_text)
    l_full_bhashya_text_r = l_full_bhashya_text_r.replace('<br>', '<br/>')
    l_full_bhashya_text_r = l_full_bhashya_text_r.replace(' ॥</em>', '</em> ॥')
    l_full_bhashya_text_r = re.sub(r'(\S)-\s+<em>', r'\1-<em>', l_full_bhashya_text_r)
    l_full_bhashya_text_r = re.sub(r'\s*<br/>\s*', r'<br/>', l_full_bhashya_text_r)
    l_full_bhashya_text_r = re.sub(r'॥\s*(\d+)\s*॥', r'॥ \1 ॥', l_full_bhashya_text_r)
    l_full_bhashya_text_r = re.sub(r'(\S)\s*॥\s*(\d+)</em>\s*॥', r'\1</em> ॥ \2 ॥', l_full_bhashya_text_r)
    l_full_bhashya_text_r = re.sub(r'॥\s*(\d+(?:,\s*\d+)+)\s*॥', lambda m: f'॥ {equalize_space_comma(m.group(1))} ॥', l_full_bhashya_text_r)

    return l_full_bhashya_text_r

def anandamak(p_file):
    with (open(p_file, 'r') as l_fin):
        l_txt = l_fin.read().replace('<br>', '<br/>')
        print('Anandamak:', p_file, len(l_txt))
        try:
            l_tree = ET.fromstring(f'{l_txt}')
        except ET.ParseError as e:
            print(f'XML parse ERROR: {e.code} {e.msg} in [{l_txt[:100]}]')

        l_intro_line_list = []
        l_verse_id = '0_0'
        l_intro_text = ''
        for l_div_adhyaya in l_tree.findall('div'):
            print('l_div_adhyaya', l_div_adhyaya.attrib['class'], l_div_adhyaya.attrib['id'])
            for l_div_verse in l_div_adhyaya.findall('div'):
                print('l_div_verse', l_div_verse.attrib['class'])
                if l_div_verse.attrib['class'] == 'introduction':
                    l_intro_frag = ' '.join([p.text for p in l_div_verse.findall('p')])
                    print('    Intro:', l_intro_frag)
                    if len(l_intro_line_list) > 0:
                        l_intro_line_list.append('<br/>' + l_intro_frag)
                    else:
                        l_intro_line_list.append(l_intro_frag)

                    l_intro_text = amak_text_cleanup('\n'.join(l_intro_line_list))
                elif l_div_verse.attrib['class'] == 'verse':
                    if l_verse_id == '0_0':
                        print(f'0_0 intro text: {l_intro_text}')
                        g_parallel_bhashya.setdefault('0_0', dict())['Anandamak'] = (l_intro_text, '')
                        l_intro_text = ''
                        l_intro_line_list = []

                    # <blockquote class="uvaaca">dhṛtarāṣṭra uvāca</blockquote>
                    l_uvaca = l_div_verse.find("blockquote[@class='uvaaca']")
                    if l_uvaca is not None:
                        l_uvaca_prefix = l_uvaca.text.strip() + ' '
                    else:
                        l_uvaca_prefix = ''
                    # print('   ', l_div_verse.attrib['class'], l_div_verse.attrib['id'])
                    # <div class="verse" id="BS_C01_S01_V01" type="sutra" data-adhikaranaid="BS_C01_S01_A001" data-adhikarana="jijñāsādhikaraṇam">
                    l_verse_id_0 = l_div_verse.attrib['id'].strip()
                    l_verse_id = re.sub(fr'^{g_id_prefix}_', '',
                                        re.sub(r'_[CSV]0?', '_', l_verse_id_0))
                    # verse-text
                    l_verse_text = l_uvaca_prefix + '|'.join([p.text for p in l_div_verse.findall("div[@class='verse-text']/p")])
                    g_parallel.setdefault(l_verse_id, dict())['Anandamak'] = l_verse_text
                    print('    Verse:', l_verse_id_0, '-->', l_verse_id, l_verse_text)
                    for l_div_bhashya_collection in l_div_verse.findall('div'):
                        if l_div_bhashya_collection.attrib['class'] == 'bhashya-collection':
                            l_bhashya_line_list = []
                            for l_div_bhashya in l_div_bhashya_collection.findall('div'):
                                if l_div_bhashya.attrib['class'].strip() == 'bhashya':
                                    l_text = l_div_bhashya.text.strip() if l_div_bhashya.text else ''
                                    print('    Bhashya', l_div_bhashya.attrib['id'], l_text)

                                    l_bhashya_frag_list = []
                                    if len(l_text) > 0:
                                        l_bhashya_frag_list.append(l_text)

                                    l_p_count = 0
                                    for l_it in l_div_bhashya.iter():
                                        l_b_text = l_it.text.strip() if l_it.text else ''
                                        l_b_tail = l_it.tail.strip() if l_it.tail else ''
                                        print(f"        {l_it.tag:5} {l_b_text}❉{l_b_tail}")
                                        if l_it.tag != 'div':
                                            if l_it.tag == 'p':
                                                if len(l_b_text) > 0:
                                                    if l_p_count > 0:
                                                        l_bhashya_frag_list.append('<br/>' + l_b_text)
                                                    else:
                                                        l_bhashya_frag_list.append(l_b_text)
                                                l_p_count += 1
                                            else:
                                                l_bhashya_frag_list.append(f'<{l_it.tag}>{l_b_text}</{l_it.tag}>')
                                            if len(l_b_tail) > 0:
                                                l_bhashya_frag_list.append(l_b_tail)

                                    l_bhashya_line_list.append(' '.join(l_bhashya_frag_list))

                            l_full_bhashya_text = amak_text_cleanup(' '.join(l_bhashya_line_list))
                            print('        -->', l_full_bhashya_text)
                            l_intro_text =  f'{l_intro_text}<br/>\n' if len(l_intro_text) > 0 else ''
                            g_parallel_bhashya.setdefault(l_verse_id, dict())['Anandamak'] = (l_intro_text, l_full_bhashya_text)
                            l_intro_text = ''
                            l_intro_line_list = []

def find_note(p_element):
    l_note_elem = p_element.find("span[@class='pathantara']")
    if l_note_elem:
        l_note_ref = l_note_elem.find("a[@class='pathantara-ref iast']").attrib['href']
        return l_note_ref, l_note_elem.attrib['data-note']
    else:
        return None

g_sethuila_mula_note = dict()
g_sethuila_sarvamula_note = dict()

def sethuila(p_file):
    global g_sethuila_mula_note
    global g_sethuila_sarvamula_note
    with (open(p_file, 'r') as l_fin):
        l_txt = l_fin.read()
        print('Sethuila:', p_file, len(l_txt))
        # l_txt = l_txt.replace('<br>', '<br/>')
        #  .replace('&nbsp;', '&#160;')
        # data-note="प्राणैः= शिरः= इत्यधुनातनीये= आयुष्यसूक्ते।="
        # l_txt = l_txt.replace('data-note="" प्राणैः="" शिरः"="" इत्यधुनातनीये="" आयुष्यसूक्ते।"=""', 'data-note="प्राणैः= शिरः= इत्यधुनातनीये= आयुष्यसूक्ते।="')
        # <img draggable="false" role="img" class="emoji" alt="🔗" src="https://s.w.org/images/core/emoji/17.0.2/svg/1f517.svg">
        # l_txt = re.sub(r'<img([^>]+)>', r'<img\1/>', l_txt)
        try:
            l_tree = ET.fromstring(f'{l_txt}')
        except ET.ParseError as e:
            print(f'XML parse ERROR: {e.code} {e.msg} in [{l_txt[:100]}]')

        l_uvaca_prefix = ''
        l_verse_id = '0_0'
        l_bhashya_list = []
        for l_div_outer in l_tree.findall('div'):
            print(l_div_outer.attrib['class'])
            if l_div_outer.attrib['class'] == 'content':
                for l_div_inner in l_div_outer.findall('div'):
                    print('    <div>', l_div_inner.attrib['class'], end=' ')
                    if l_div_inner.attrib['class'] == 'Mula':
                        if len(l_bhashya_list) > 0:
                            l_existing_dict = g_parallel_bhashya.setdefault(l_verse_id, dict())
                            # l_bhashya_text_list = [(t, re.sub(r'(\S)\s*\.\.\s*(\d+)\s*\.\.', r'\1 ॥ \2 ॥', l_text), n) for t, l_text, n in l_bhashya_list]
                            # l_existing_dict['Sethuila'] = l_bhashya_text_list
                            l_existing_dict['Sethuila'] = l_bhashya_list
                        l_bhashya_list = []

                        # for l_it in l_div_inner.iter():
                        #     print(f'<{l_it.tag}>', l_it.text, end='')
                        l_uvaca = l_div_inner.find("span[@class='inline iast']")
                        # l_uvaca = l_div_inner.find('span')
                        # print(l_uvaca)
                        if l_uvaca is not None:
                            l_uvaca_prefix = l_uvaca.text.strip() + ' '
                            print(l_uvaca_prefix, l_uvaca.attrib['class'])
                            continue

                        l_verse_text_list = []
                        l_notes_list = []
                        for l_span_padya in l_div_inner.findall("span[@class='padya iast']"):
                            l_padya_text_list = [l_span_padya.text]
                            l_notes_list.append(find_note(l_span_padya))
                            for l_span_inline in l_span_padya.findall("span[@class='inline iast']"):
                                l_padya_text_list.append(l_span_inline.text)
                            l_verse_text_list.append(' '.join(l_padya_text_list))

                        l_verse_text = l_uvaca_prefix + '|'.join(l_verse_text_list)
                        print(l_uvaca_prefix, l_verse_text_list)
                        l_uvaca_prefix = ''
                        # Mula .. [oṃ] anāvṛttiḥ śabdādanāvṛttiḥ śabdāt [oṃ].. 4/4/23..
                        l_match = re.search(r'(\d+/\d+/\d+|\d+/\d+)\.\.$', l_verse_text)
                        if l_match is not None:
                            l_verse_id = l_match.group(1).replace('/', '_')

                        if len(l_notes_list) > 0 and l_notes_list != [None, None]:
                            g_sethuila_mula_note[l_verse_id] = l_notes_list

                        # l_verse_id = re.match(r'^.*\[oṃ].*\[oṃ]\.\.\s+(\d+/\d+/\d+)\.\.', l_verse_text).group(1).replace('/', '_')
                        print(l_verse_id, l_verse_text, end=' ')

                        l_existing_dict = g_parallel.setdefault(l_verse_id, dict())
                        # print(l_verse_id, g_parallel[l_verse_id], l_existing_dict, end=' ')
                        l_existing_dict['Sethuila'] = l_verse_text
                        print(g_parallel[l_verse_id])
                        # print(l_existing_dict, g_parallel[l_verse_id])
                        # g_parallel[l_verse_id] = l_existing_dict
                    elif l_div_inner.attrib['class'] == 'Sarvamula':
                        def sarvamula_process(s): # ॥\s*([- \d]*\d)\s*॥
                            s = re.sub(r'(\S)\s*\.\.\s*([- \d]*\d)\s*\.\.', r'\1 ॥ \2 ॥', s)
                            s = re.sub(r'(\S)\s*\.\.', r'\1 ॥', s)
                            s = re.sub(r'na\W+([^0-9\W_])', lambda m: external_sandhi(f'na⌿{m.group(1)}'), s)
                            s = re.sub(r'([^0-9\W_])\W+ca', lambda m: external_sandhi(f'{m.group(1)}⌿ca'), s)
                            s = re.sub(r'([^0-9\W_])\W+([^0-9\W_]{2,3})', lambda m: external_sandhi(f'{m.group(1)}⌿{m.group(2)}'), s)
                            s = re.sub(r'([^0-9\W_]{2,3})\W+([^0-9\W_])', lambda m: external_sandhi(f'{m.group(1)}⌿{m.group(2)}'), s)
                            return s

                        print()
                        for l_span_1 in l_div_inner.findall('span'):
                            print('       ', l_span_1.attrib['class'], end=': ')
                            # padya iast
                            if l_span_1.attrib['class'] in ['padya iast', 'inline iast']:
                                l_bhashya_fragment = sarvamula_process(l_span_1.text.strip())
                                # l_bhashya_fragment = re.sub(r'(\S)\s*\.\.\s*(\d+)\s*\.\.', r'\1 ॥ \2 ॥', l_bhashya_fragment)
                                # l_bhashya_fragment = re.sub(r'(\S)\s*\.\.', r'\1 ॥', l_bhashya_fragment)
                                print(l_bhashya_fragment, end=' ')
                                # l_bhashya_list.append(f'[iast] {l_bhashya_fragment}')
                                # l_bhashya_list.append(('[iast]', l_bhashya_fragment))

                                l_note_elem = find_note(l_span_1)
                                l_note_ref = None
                                if l_note_elem is not None:
                                    l_note_ref = l_note_elem[0]
                                    g_sethuila_sarvamula_note.setdefault(l_verse_id, []).append(l_note_elem)
                                    print(f'Note: {l_note_elem}')
                                else:
                                    print()

                                l_bhashya_list.append(('{iast}', l_bhashya_fragment, [l_note_ref]))

                            # padya
                            elif l_span_1.attrib['class'] in ['padya', 'inline']:
                                l_span_2 = l_span_1.find("span[@class='pramana iast']")
                                # print(l_span_2)
                                if l_span_2 is None:
                                    l_span_2 = l_span_1.find("span[@class='pratika iast']")
                                # print(l_span_2)

                                l_frag_text = sarvamula_process(l_span_2.text.strip())
                                # l_frag_text = re.sub(r'(\S)\s*\.\.\s*(\d+)\s*\.\.', r'\1 ॥ \2 ॥', l_frag_text)
                                # l_frag_text = re.sub(r'(\S)\s*\.\.', r'\1 ॥', l_frag_text)
                                print(l_frag_text, end=' ')

                                l_note_elem = find_note(l_span_1)
                                l_note_ref_1 = None
                                if l_note_elem is not None:
                                    l_note_ref_1 = l_note_elem[0]
                                    g_sethuila_sarvamula_note.setdefault(l_verse_id, []).append(l_note_elem)
                                    print(f'Note 1: {l_note_elem}')

                                l_note_elem = find_note(l_span_2)
                                l_note_ref_2 = None
                                if l_note_elem is not None:
                                    l_note_ref_2 = l_note_elem[0]
                                    g_sethuila_sarvamula_note.setdefault(l_verse_id, []).append(l_note_elem)
                                    print(f'Note 2: {l_note_elem}')
                                else:
                                    print()

                                l_bhashya_list.append(('{-}', l_frag_text, [l_note_ref_1, l_note_ref_2]))
                            else:
                                print()
                    elif l_div_inner.attrib['class'] == 'Heading3':
                        # span class="heading-number iast">1.1.1.</span>
                        l_heading_number = l_div_inner.find("span[@class='heading-number iast']").text
                        # <span class="heading-text"><span class="inline iast" id="292bcde8-de3f-4295-9b06-8f151822c967">
                        l_heading_text = l_div_inner.find("span[@class='heading-text']/span[@class='inline iast']").text
                        print(l_heading_number, l_heading_text)

                    else:
                        print()

def list_candidates(p_k):
    l_list_candidate = [p_k,
                        # Initial vowels
                        re.sub(r' cet', r'cait', p_k),
                        re.sub(r'( |^)e(.)', r'\1i\2', p_k),
                        re.sub(r'( |^)a(.)', r'\1ā\2', p_k),
                        re.sub(r'( |^)a(.)', r'\1\2', p_k),
                        re.sub(r'( |^)a(.)', r'\1ऽ\2', p_k),
                        re.sub(r' a(.)', r'ऽ\1', p_k),
                        re.sub(r'^a(.)', r'ऽ\1', p_k),
                        re.sub(r'( |^)ā(.)', r'\1ऽ\2', p_k),
                        re.sub(r' ā(.)', r'ऽ\1', p_k),
                        re.sub(r'^ā(.)', r'ऽ\1', p_k),
                        re.sub(r'( |^)ā(.)', r'\1\2', p_k),
                        re.sub(r'( |^)u(.)', r'\1o\2', p_k),
                        re.sub(r'( |^)u(.)', r'\1ū\2', p_k),
                        re.sub(r'( |^)u(.)', r'\1\2', p_k),
                        re.sub(r'( |^)ū(.)', r'\1\2', p_k),
                        re.sub(r'( |^)i(.)', r'\1e\2', p_k),
                        re.sub(r'( |^)ī(.)', r'\1e\2', p_k),
                        re.sub(r'( |^)i(.)', r'\1ī\2', p_k),
                        re.sub(r'( |^)ī(.)', r'\1ī\2', p_k),

                        # initial consonants
                        re.sub(r'( |^)ś(.)', r'\1c\2', p_k),
                        re.sub(r'( |^)s(.)', r'\1\2', p_k),
                        re.sub(r'( |^)ś(.)', r'\1\2', p_k),
                        re.sub(r'( |^)c(.)', r'\1\2', p_k),
                        re.sub(r'( |^)r(.)', r'\1\2', p_k),
                        re.sub(r'( |^)l(.)', r'\1ll\2', p_k),
                        re.sub(r'( |^)saṃ(.)', r'\1ṇaṃ\2', p_k),

                        # final vowels
                        re.sub(r'(.)e( |$)', r'\1a\2', p_k),
                        re.sub(r'(.)a( |$)', r'\1\2', p_k),
                        re.sub(r'(.)a( |$)', r'\1o\2', p_k),
                        re.sub(r'(.)ā( |$)', r'\1a\2', p_k),
                        re.sub(r'(.)au( |$)', r'\1āv\2', p_k),
                        re.sub(r'(.)u( |$)', r'\1v\2', p_k),
                        re.sub(r'(.)u( |$)', r'\1\2', p_k),
                        re.sub(r'(.)[iī]( |$)', r'\1y\2', p_k),
                        re.sub(r'(.)i( |$)', r'\1\2', p_k),

                        # final consonants
                        re.sub(r'(.)ḥ( |$)', r'\1ś\2', p_k),
                        re.sub(r'(.)ḥ( |$)', r'\1s\2', p_k),
                        re.sub(r'(.)ḥ( |$)', r'\1r\2', p_k),
                        re.sub(r'(.)ḥ( |$)', r'\1\2', p_k),
                        re.sub(r'(.)aḥ( |$)', r'\1o\2', p_k),
                        re.sub(r'(.)m( |$)', r'\1\2', p_k),
                        re.sub(r'(.)m( |$)', r'\1ṃ\2', p_k),
                        re.sub(r'(.)m ', r'\1ṃ ', p_k),
                        re.sub(r'(.)m$', r'\1ṃ', p_k),
                        re.sub(r'(.)n( |$)', r'\1ṃ\2', p_k),
                        re.sub(r'(.)n( |$)', r'\1ṃs\2', p_k),
                        re.sub(r'(.)n( |$)', r'\1ṃś\2', p_k),
                        re.sub(r'(.)ṃ( |$)', r'\1ñ\2', p_k),
                        # re.sub(r'(.)n( |$)', r'\1nn\2', p_k),
                        re.sub(r'(.)t( |$)', r'\1d\2', p_k),
                        re.sub(r'(.)t( |$)', r'\1j\2', p_k),
                        re.sub(r'(.)t( |$)', r'\1n\2', p_k),
                        re.sub(r'(.)t( |$)', r'\1l\2', p_k),
                        re.sub(r'(.)ṭ( |$)', r'\1ṇ\2', p_k),
                        re.sub(r'(.)d( |$)', r'\1c\2', p_k),
                        re.sub(r'(.)l_verse_k( |$)', r'\1g\2', p_k),
                        re.sub(r'(.)l_verse_k( |$)', r'\1ṃ\2', p_k),

                        # ktiḥ ktim antr ṃ kalp aranś l_verse_k tribhiḥ trividham saṃnyāsam
                        re.sub(r'(.)ktiḥ( |$)', r'\1kitar\2', p_k),
                        re.sub(r'(.)ktim( |$)', r'\1kitam\2', p_k),
                        re.sub(r'(.)antr(.)', r'\1aṃtr\2', p_k),
                        re.sub(r'(.)kalp(.)', r'\1kamp\2', p_k),
                        re.sub(r'(.)aranś(.)', r'\1oṃऽś\2', p_k),
                        re.sub(r'(.)kti(.)', r'\1kita\2', p_k),
                        re.sub(r'aṃśa', r'ṃऽśa', p_k),
                        re.sub(r'tribhiḥ', r'itrabhir', p_k),
                        re.sub(r'trividham', r'itravidhaṃ', p_k),
                        ]
    # 힣 = Unicode U+D7A3 --> after every Devanagari block so will be first in a reverse=true search
    l_list_candidate = [(f'{len(p_k):04}-{"힣"*len(l_cand) if l_cand == p_k else l_cand}', l_cand) for l_cand in list(set(l_list_candidate))]
    return l_list_candidate

def remove_avagraha(s):
    return s.replace('ऽ', '')

def remove_invisibles(s):
    return re.sub(r'[\u2060\u3164\u2800\u00A0]+', ' ', re.sub(r'[\u200b\u200c\u200d]+', '', s))

def standardize_iast(s): # ātmalm̐labhante
    return remove_invisibles(s.replace('ṁ', 'ṃ')
             .replace('ṅ', 'ṃ')
             .replace('lm̐l', 'ṃl')
             .replace(':', 'ḥ')
             .replace('o़\'', 'o\'')
             .replace('ṛ़', 'ṝ') # to apply to original text/files ़
    )

def internal_sandhi(s): # ⌿
    return (
        re.sub(r'[āa][āa]', 'ā', 
        re.sub(r'[īi][īi]', 'ī', 
        re.sub(r'[uū][uū]', 'ū', 
        # re.sub(r"[āa]'?i", 'e', t⌿ś
        re.sub(r'(.[aāiīuūoe])nny', r'\1ṃny', 
               s))))
        .replace('ḥs', 'ss')
        .replace('ṃj', 'ñj')
        .replace('ṃc', 'ñc')
        .replace('dn', 'nn')
    )
def external_sandhi(s, p_insert_invisible=False): # taddhi agnyāderapi
    l_invisible_space = '\u200b' if p_insert_invisible else ''
    return re.sub('(\w)⌿(\w)', rf'\1{l_invisible_space}\2',
        re.sub(r'[āa]⌿[āa]', 'ā',
        re.sub(r'[īi]⌿[īi]', 'ī',
        re.sub(r'(.)[īi]⌿([āauū])', fr'\1y{l_invisible_space}\2',
        re.sub(r'[uū]⌿[uū]', 'ū',
        re.sub(r"[āa]⌿i", 'e',
        re.sub(r'(.[aāiīuūoe])[mn]⌿([pbtdkgm])', rf'\1ṃ{l_invisible_space}\2',
        re.sub(r'ḥ⌿([aāiīuūoe])', rf'r{l_invisible_space}\1',
        re.sub(r'e⌿([aāiīuūo])', rf'ay{l_invisible_space}\1',
        re.sub(r'(.)t⌿([bdgaāiīuūoey])', rf'\1d{l_invisible_space}\2',
               s))))))))) # t⌿ś
        .replace('ḥ⌿s', 'ss')
        .replace('t⌿ś', 'cch')
        .replace('ḥ⌿t', 'st')
        .replace('ṃ⌿j', 'ñj')
        .replace('ṃ⌿c', 'ñc')
        .replace('d⌿n', 'nn')
    )

g_tei_changes = {
    # '2_1': [('kṛpayā\'viṣṭam', 'kṛpayāviṣṭam')]
    # '2_7': [('dharmasaṃmūḍhacetāḥ', 'dharmasammūḍhacetāḥ')] śarīrayātrā'pi
    '2_8': [('ṛddham', 'ṛddhaṃ')]
    , '2_9': [('parantapa । na', 'parantapaḥ । na')]
    , '2_26': [('naivaṃ', 'nainaṃ')]
    , '2_29': [('śrutvāpyenaṃ', 'śrutvā\'pyenaṃ'), ('śrṛṇoti', 'śṛṇoti')]
    , '2_31': [('yuddhāchreyo', 'yuddhācchreyo')]
    , '2_44': [('tayāpahṛtacetasām', 'tayā\'pahṛtacetasām')]
    , '2_52': [('gantāsi', 'gantā\'si')]
    , '2_55': [("ātmanyevātmanā", "ātmanyevā'tmanā")]
    , '2_65': [('paryavatiṣṭhate', 'paryavatiṣṭhati')]
    , '2_71': [('niḥspṛhaḥ', 'nispṛhaḥ'), ('śāṃtimadhigacchati', 'śāntimadhigacchati')]
    , '3_2': [('vyāmiśreṇeva', 'vyāmiśreṇaiva')]
    , '3_3': [('mayānagha', 'mayā\'nagha')]
    , '3_7': [('niyamyārabhate\'rjuna', 'niyamyā\'rabhate\'rjuna')]
    , '3_8': [('śarīrayātrāpi', 'śarīrayātrā\'pi')] #
    , '3_25': [('vidvāṃstathāsakta', 'vidvāṃstathā\'sakta')]
    , '3_29': [('tānakṛtsnavido', 'tānakṛsnavido')]
    , '4_22': [('kṛtvāpi', 'kṛtvā\'pi')]
    , '4_28': [('yogayajñāstathāpare', 'yogayajñāstathā\'pare')]
    , '4_29': [('prāṇa prāṇe\'pānaṃ', 'prāṇaṃ prāṇe\'pānaṃ')]
    # , '5_8': [('spṛśañjidhrannaśnan', 'spṛśañcidhrannaśnan')]
}

g_html_changes = { #
    '1_18': [('śaṃkhaṃ', 'śaṃkhāṃ')],
    '1_20': [('śastrasampāte', 'śastrasaṃpāte')],
    '1_23': [('^sañjaya uvāca ', '')],
    '1_24': [('^evamukto', 'sañjaya uvāca evamukto')],
    '1_28': [('^kṛpayā', 'arjuna uvāca kṛpayā')],
    '1_44': [('narake niyataṃ', 'narake\'niyataṃ')],
    '2_1': [('kṛpayā\'\'viṣṭam', 'kṛpayā\'viṣṭam')],
    '2_4': [('ajurna', 'arjuna')],
    '2_5': [('hatvā\'rthakāmāṃstu', 'hatvārthakāmāṃstu')],
    '2_7': [('dharmasammūḍhacetāḥ', 'dharmasaṃmūḍhacetāḥ')],
    '2_9': [('parantapa । na', 'parantapaḥ । na')],
    '2_16': [("vidyate'bhāvo", "vidyatebhāvo")],
    '2_18': [("yuddhyasva", "yudhyasva")],
    '2_26': [("tathā'pi", "tathāpi")],
    '2_29': [("kaścidenamāścaryavadāścaryavad", "kaścidenamāścaryavadvadati"), ("śṛṇotiśrutvāśrutvāpyenaṃ", "śṛṇoti śrutvāpyenaṃ")],
    '2_31': [("cā'vekṣya", "cāvekṣya")],
    '2_33': [("atha cet tvamimaṃ", "atha cait tvamimaṃ")],
    '2_34': [("te'vyayām।sambhāvitasya", "te'vyayām।saṃbhāvitasya")],
    '2_53': [("yathā", "yadā")],
    '2_54': [('^sthitaprajñasya', 'arjuna uvāca sthitaprajñasya')],
    '2_55': [("ātmanyevātmanā", "ātmanyevā'tmanā")],
    '2_63': [("buddhināśād vina\(praṇa\)śyati", "buddhināśāt praṇaśyati"), ("d vinaśyati", "t praṇaśyati")],
    '2_71': [("niḥspṛhaḥ", "nispṛhaḥ")],
    '2_72': [("nirbāṇam", "nirvāṇam")],
    '3_7': [('niyamyārabhate\'rjuna', 'niyamyā\'rabhate\'rjuna')],
    '3_8': [("prasidhyedakarmaṇaḥ", "prasiddhyedakarmaṇaḥ")],
    '3_13': [("tvadhaṃ", "tvaghaṃ")],
    '3_25': [("vidvān tathā", "vidvāṃstathā")],
    '3_27': [("kartāhamiti", "kartā\'hamiti")],
    '3_30': [('yudadhyasva', 'yudhyasva'), ('yuddhyasva', 'yudhyasva')],
    '3_37': [('vidhyenamiha', 'viddhyenamiha')],
    '4_13': [('vidhyakartāramavyayam', 'viddhyakartāramavyayam')],
    '4_14': [('baddhyate', 'badhyate')],
    '4_16': [('tat te\(\'\)karma', 'tatte karma')],
    '4_20': [('nityatṛpto\(\'\)nirāśrayaḥ', 'nityatṛpto nirāśrayaḥ'), ('karmaphalā\'saṃgaṃ', 'karmaphalāsaṃgaṃ')],
    '4_21': [('kurvan nā\'pnoti', 'kurvan nāpnoti')],
    '4_22': [('nibaddhyate', 'nibadhyate')],
    '4_23': [('yajñāyā\'carataḥ', 'yajñāyācarataḥ')],
    '4_28': [('yatayaśśaṃsitavratāḥ', 'yatayaḥ saṃśitavratāḥ')],
    '4_29': [('apāne juhvani prāṇaṃ', 'apāne juhvati prāṇaṃ')],
    '4_30': [('niyatā\'hārāḥ', 'niyatāhārāḥ')],
    '4_33': [('karmā\(\'\)khilaṃ', 'karmākhilaṃ'), ('yajñād jñānayajñaḥ', 'yajñājjñānayajñaḥ')],
    '4_34': [('paripraśrena', 'paripraśnena'), ('jñāninastatvadarśinaḥ', 'jñāninastattvadarśinaḥ')],
    '4_38': [('kālenā\'tmani', 'kālenātmani')],
    '4_39': [('śraddhāvāllaṃbhate', 'śraddhāvāṃllabhate'), ('śraddhāvālm̐labhate', 'śraddhāvāṃllabhate'), ('jñānaṃ matparaḥ', 'jñānaṃ tatparaḥ')],
    '5_8': [('paśyan śṛṇvan', 'paśyañśṛṇvan'), ('spṛśan jighran aśnan', 'spṛśañcidhrannaśnan'), ('svapan śvasan', 'svapañśvasan'), ('spṛśañcidhrannaśnan', 'spṛśañjighrannaśnan')],
    '5_9': [('gṛhṇan unmiṣan', 'gṛhṇannunmiṣan')],
    '5_12': [('nibaddhyate', 'nibadhyate')],
    '5_13': [('saṃnyasyā\'ste', 'saṃnyasyāste')],
    '5_15': [('nā\'datte', 'nādatte'), ('ajñānenā\'vṛtaṃ', 'ajñānenāvṛtaṃ')],
    '5_17': [('tadbuddhayastadātmānaḥ tanniṣṭhāstatparāyaṇāḥ', 'tadbuddhayastadātmānastanniṣṭhāstatparāyaṇāḥ')],
    '5_21': [('bāhmasparśeṣvasaktātmā', 'bāhyasparśeṣvasaktātmā')],
    '5_24': [('ntarārāmastathā\'ntarjyotireva', 'ntarārāmastathāntarjyotireva')],
    '5_25': [('brahmanirbā\(vā\)ṇam', 'brahmanirvāṇam'), ('chinnadvaidhā\(?\'\'\)?yatātmānaḥ', 'chinnadvaidhā yatātmānaḥ')],
    '5_28': [('buddhiḥ munirmokṣa', 'buddhirmunirmokṣa')],
}
# yatayaśśaṃsitavratāḥ yajñād
def display_dif(p_base, p_other):
    print('-----------------------------------------------------------------------------')
    l_sm = difflib.SequenceMatcher(None, p_base, p_other)
    l_turn = 0
    l_gap_start = -1
    l_gap_end = -1
    for a, _, s in l_sm.get_matching_blocks():
        if s > 0:
            if l_turn == 0: l_gap_start = a + s
            if l_turn == 1: l_gap_end = a
            print(f'{a:3} {s:3} {" " * a}{p_base[a:a + s]}')
            print(f'        {" " * a}{p_other[a:a + s]}')
            l_turn += 1
    if l_gap_start >= 0 and l_gap_end >= 0:
        # print(l_gap_start, l_gap_end, len(p_base), len(p_other))
        if l_gap_end == l_gap_start:
            l_gap_end += 1
        print(f'{l_gap_start:3} {l_gap_end:3} {" " * l_gap_start}{p_base[l_gap_start:l_gap_end]} {ord(p_base[l_gap_start:l_gap_end][0]):x}')
        print(f'        {" " * l_gap_start}{p_other[l_gap_start:l_gap_end]} {ord(p_other[l_gap_start:l_gap_end][0]):x}')
    for l_string in [p_base, p_other]:
        for i in range(0, len(l_string)):
            print(l_string[i], end='_')
        print()

g_gita_tei_file = dict()
g_gita_tei_file_bc = dict()
def load_gita_tei():
    global g_gita_tei_file
    global g_gita_tei_file_bc
    with (open('../../gitaDnl/bg_final.xml', 'r') as l_fin):
        l_txt = l_fin.read().replace('<br>', '<br/>')
        try:
            l_tree = ET.fromstring(f'{l_txt}')
        except ET.ParseError as e:
            print(f'XML parse ERROR: {e.code} {e.msg} in [{l_txt[:100]}]')

        # for l_elem in l_tree.iter():
        #     print(l_elem.tag)
        l_text = l_tree.find('{http://www.tei-c.org/ns/1.0}text')
        for l_div1 in l_text.findall("{http://www.tei-c.org/ns/1.0}div1[@type='chapter']"):
            for l_sloka in l_div1.findall("{http://www.tei-c.org/ns/1.0}div2[@type='sloka-block']"):
                l_sloka_l = l_sloka.find('{http://www.tei-c.org/ns/1.0}l')
                # print(l_sloka.attrib)
                l_sloka_id = l_sloka.attrib['{http://www.w3.org/XML/1998/namespace}id'].replace('BG_', '').replace('.', '_')
                l_sloka_sk = devtrans.dev2iast(l_sloka_l.text + '|' + l_sloka_l.find('{http://www.tei-c.org/ns/1.0}caesura').tail)
                l_sloka_sk = re.sub(r'\s*\.\.\s*\d+\.\d+\s*\.\.$', '', l_sloka_sk).replace('|', ' । ')

                g_gita_tei_file_bc[l_sloka_id] = l_sloka_sk

                l_sloka_sk = l_sloka_sk.replace('-', '')
                for l_id_k, l_mod_list in g_tei_changes.items():
                    if l_id_k == l_sloka_id:
                        for l_vt1, l_vt2 in  l_mod_list:
                            l_sloka_sk = l_sloka_sk.replace(l_vt1, l_vt2)

                print(l_sloka_id, l_sloka_sk)
                g_gita_tei_file[l_sloka_id] = l_sloka_sk

def parallel_dump():
    load_gita_tei()

    g_sethuila_mula_note['2_63'] = [('#note-pathantara-9990', '‘vinaśyati’')]
    g_sethuila_mula_note['4_16'] = [('#note-pathantara-9991', '‘tat te\'karma’')]
    g_sethuila_mula_note['4_20'] = [('#note-pathantara-9992', '‘nityatṛpto\'nirāśrayaḥ’')]
    g_sethuila_mula_note['4_33'] = [('#note-pathantara-9993', '‘karmā\'khilaṃ’')]
    g_sethuila_mula_note['5_8'] = [('#note-pathantara-9993', '‘spṛśañcidhrannaśnan’')]
    g_sethuila_mula_note['5_25'] = [('#note-pathantara-9994', '‘brahmanirbāṇamṛṣayaḥ’'),
                                    ('#note-pathantara-9996', '‘chinnadvaidhā''yatātmānaḥ’')]
    for l_bs_vn, d in g_parallel.items(): # chinnadvaidhā''yatātmānaḥ
        print(f'{l_bs_vn} {g_sethuila_mula_note[l_bs_vn] if l_bs_vn in g_sethuila_mula_note else ""}')
        l_comp = []
        for k, v in d.items():
            try:
                v = standardize_iast(v)

                # l_match = re.match(r'^.*?\[?oṃ]?\s+(.*?)\s+\[?oṃ]?.*$', v) # for Brahma Sutra Bhashya
                # if l_match:
                #     v = l_match.group(1)

                v = v.replace('–', '')
                v = v.replace('-', '')
                v = v.replace(' ।|', ' । ')
                v = v.replace(' ॥|', ' । ') #  । |
                v = v.replace('.|', ' । ')
                v = v.replace(' । |', ' । ')
                v = v.replace('..', '॥')
                v = v.replace('. । ', ' । ')
                v = v.replace('\u200d', '')
                v = re.sub(r'(\S)\|(\S)', r'\1 \2', v)
                v = re.sub(r'\s*॥\s*(?:\d+/)?(\d+)\s*॥', r' ॥ \1 ॥', v)
                v = re.sub(r'\s+', ' ', v)

                for l_id_k, l_mod_list in g_html_changes.items():
                    if l_id_k == l_bs_vn:
                        for l_vt1, l_vt2 in  l_mod_list:
                            v = re.sub(l_vt1, l_vt2, v)
                            #print (l_vt1, l_vt2, v)

                # print(l_verse_k, v, re.match(r'^oṃ\s+(.*?)\s+oṃ', v), re.search(r'\[oṃ]\s+(.*?)\s+\[oṃ]', v))
                # l_comp.append(re.match(r'^oṃ\s+(.*?)\s+oṃ', v).group(1) if l_verse_k == 'Anandamak' else re.search(r'\[oṃ]\s+(.*?)\s+\[oṃ]', v).group(1))

                l_v_comp = re.sub(r'॥\d+॥', '', re.sub(r'\s+', '', v))
                l_v_comp = internal_sandhi(l_v_comp)

                l_comp.append(l_v_comp)
                print(f'    {k:10} : {v}')
            except AttributeError:
                print(k, v)
                sys.exit(0)
        print(f'    TEI        : {g_gita_tei_file[l_bs_vn]}')
        l_ref_text = internal_sandhi(standardize_iast(g_gita_tei_file[l_bs_vn].replace(' ', '')))
        l_base = l_comp[0]
        l_other = l_comp[1]
        if len(l_comp) < 2 or not (l_base == l_other and l_base == l_ref_text):
            print('    p_base  :', l_base)
            print('    p_other :', l_other)
            print('    ref     :', l_ref_text)

            if l_base != l_other:
                display_dif(l_base, l_other)
            if l_base != l_ref_text:
                display_dif(l_base, l_ref_text)
            # sys.exit(0)
            return

g_html_header = """<html>
<head>
    <meta charset="UTF-8"> 
    <style>
        table, th, td.outer{
            border: 1px solid black;
            border-collapse: collapse;
            vertical_align: top;
            padding: .5em .5em .5em 1em; 
        }
        td.inner{
            border: 1px solid black;
            border-collapse: collapse;
            vertical_align: top;
            padding: .2em .5em .2em .5em; 
        }
    </style>
</head>
    <body>
    <table>
    <tr><th>Sloka</th><th>Anandamakaranda</th><th>Sethuila</th></tr>
"""

def process_anandamak(p_amak_raw_list):
    """

    :param p_amak_raw_list: list of triplets (l_underscore_k, l_intro_text, l_full_bhashya_text)
    :return:
    """
    def process_for_display(s):
        r = s
        # r = re.sub(r'\([^\d(]*\d+[^)]*\)', r' ', r) # (ṛ.10.72.2) (bhāga.1.2.31) (bṛ.u.6.3.7)
        r = re.sub(r'(\s*\.\s*\d+)+\.?', lambda m: m.group(1).replace(' ', ''), r) # cleanup of sequences of dots and numbers
        r = re.sub(r'\(\s+(\S)', r'(\1', r)               # space after (
        r = re.sub(r'(\S)\s+\)', r'\1)', r)               # space before )
        r = re.sub(r'(\S)\s*॥\s*([- \d]*\d)\s*॥', r'\1&#xA0;॥&#xA0;\2&#xA0;॥', r)
        r = re.sub(r'(\S)\s*।', r'\1&#xA0;।', r)
        r = re.sub(r'(\w)[’\'](\w)', r"\1ऽ\2", r)         # Avagraha (single one)
        r = re.sub(r'(\w)(?:’’|\'\')(\w)', r"\1ऽऽ\2", r)  # Avagraha (2 of them) “ ”

        return r

    l_amak_text_x_list = []
    l_amak_text_display_list = []
    l_amak_text_display_list_sk = []
    for l_underscore_k_h, l_intro_text_h, l_full_bhashya_text_h in p_amak_raw_list:
        l_amak_text_x_list.append(re.sub(r'[“”‘’,;:\'\-?!]', r' ',              # punctuation removal
                                  re.sub(r'\([^\d(]*\d+[^)]*\)', r' ',          # (ṛ.10.72.2) (bhāga.1.2.31) (bṛ.u.6.3.7)
                                  re.sub(r'(\s*\.\s*\d+)+\.?', lambda m: m.group(1).replace(' ', ''),  # cleanup of sequences of dots and numbers
                                  re.sub(r'\(\s+(\S)', r'(\1',                  # space after (
                                  re.sub(r'(\S)\s+\)', r'\1)',                  # space before )
                                  re.sub(r'<[^>]+>', r'',                       # HTML tag removal
                                  re.sub(r'॥\s*[- \d]+\s*॥', r'',                # remove final verse references
                                  re.sub(r'(\w)[’\'](\w)', r"\1ऽ\2",            # Avagraha (single one)
                                  re.sub(r'(\w)(?:’’|\'\')(\w)', r"\1ऽऽ\2",     # Avagraha (2 of them)
                                 f'{l_intro_text_h} {l_full_bhashya_text_h}'))))))))))

        l_intro_text_h = re.sub(r'\([^\d(]*\d+[^)]*\)', r' ',       # (ṛ.10.72.2) (bhāga.1.2.31) (bṛ.u.6.3.7)
                         re.sub(r'(\s*\.\s*\d+)+\.?', lambda m: m.group(1).replace(' ', ''),  # cleanup of sequences of dots and numbers
                         re.sub(r'\(\s+(\S)', r'(\1',               # space after (
                         re.sub(r'(\S)\s+\)', r'\1)',               # space before )
                         re.sub(r'(\w)[’\'](\w)', r"\1ऽ\2",         # Avagraha (single one)
                         re.sub(r'(\w)(?:’’|\'\')(\w)', r"\1ऽऽ\2",  # Avagraha (2 of them)
                                              l_intro_text_h))))))

        l_amak_text_display_list.append(f'{process_for_display(l_intro_text_h)}<br/>[{l_underscore_k_h.replace("_", ".")}] {process_for_display(l_full_bhashya_text_h)}')
        l_amak_text_display_list_sk.append(f'{process_for_display(l_intro_text_h)}<br/>{l_full_bhashya_text_h} ॥{l_underscore_k_h.replace("_", ".")}॥')

        # chunk (⌅) and word (⌿) separators ('⌬' = end)
    l_amak_text_x_1 = re.sub(r'[=॥।–.)(\[\]]', '⌅', ' '.join(l_amak_text_x_list) + '⌬')
    l_amak_text_x_2 = re.sub(r'\s+', '⌿', l_amak_text_x_1)
    l_amak_text_x_3 = re.sub(r'⌿*(?:⌿*⌅)+⌿*', '⌅', l_amak_text_x_2)
    l_amak_text_x_4 = re.sub(r'^⌿|⌅⌬|⌿⌬', '', l_amak_text_x_3)
    l_amak_chunk_list_r = [remove_avagraha(standardize_iast(s).replace('⌬', '')) for s in l_amak_text_x_4.split('⌅')]

    l_amak_text_display = '\n'.join(l_amak_text_display_list)
    l_v_display = standardize_iast(l_amak_text_display)
    # l_v_display = standardize_iast(v) + f'\n<br/><b>l_amak_chunk_list</b>: <br/>[{l_amak_text_x}]<br/>[{l_amak_text_x_1}]<br/>[{l_amak_text_x_2}]<br/>[{l_amak_text_x_3}]<br/>[{l_amak_text_x_4}] <br/>{l_amak_chunk_list}'

    l_amak_text_display_sk = '\n'.join(l_amak_text_display_list_sk)
    l_v_display_sk = devtrans.iast2dev(re.sub(r'<[^>]+>', '', standardize_iast(l_amak_text_display_sk)))
    # l_v_display_sk = devtrans.iast2dev(standardize_iast(l_amak_text_display))

    l_anandamak_cell_r = f'<td class="outer"><b>Anandamakaranda</b>: {l_v_display}</td>\n'
    l_anandamak_cell_sk_r = f'<td class="outer"><b>Anandamakaranda</b>: {l_v_display_sk}</td>\n'

    return l_amak_chunk_list_r, l_anandamak_cell_r, l_anandamak_cell_sk_r

def process_seth(p_amak_chunk_list, p_seth_chunk_list):
    """

    :param p_amak_chunk_list:
    :param p_seth_chunk_list: original Sethuila Chunks with words split (no Sandhi yet)
    :return:
    """
    def format_ref(p_ref):
        return f'[{p_ref.replace("#note-pathantara-", "")}]'

    l_amak_chunk_list_display = copy.deepcopy(p_amak_chunk_list)

    # 2-d array of un-sandhied Sethuila words, still with punctuation attached
    l_seth_word_array = [l_words_list for _, l_words_list, _ in p_seth_chunk_list]
    l_seth_word_array_2 = copy.deepcopy(l_seth_word_array)
    l_seth_word_array_2_sk = [[devtrans.iast2dev(l_word) for l_word in l_row] for l_row in copy.deepcopy(l_seth_word_array)]

    # list of all Sethuila words, together with their outer and inner ids (ref to l_seth_word_array)
    l_seth_word_list = []
    for l_word_id_outer, (_, l_inner_word_list, _) in enumerate(p_seth_chunk_list):
        # l_inner_word_list = p_seth_chunk_list[l_word_id_outer][1]
        for l_word_id_inner,  l_word in enumerate(l_inner_word_list):
            l_seth_word_list.append((l_word_id_outer, l_word_id_inner, l_word))

    # Sethuila word list + candidates. Punctuation removed and internal Sandhi applied
    l_seth_word_list_expanded = []
    for l_id_outer, l_id_inner, l_word_cmp in l_seth_word_list:
        if not l_word_cmp == '॥' and not re.match(r'\d+', l_word_cmp):
            l_word_cmp = re.sub(r'[=॥।“”‘’–.,;:?!\'\-)(\[\]]', '',  # remove punctuation
                         re.sub(r'(\w)[’\'](\w)', r"\1ऽ\2",         # single avagraha
                         re.sub(r'(\w)(?:’’|\'\')(\w)', r"\1ऽऽ\2",  # double avagraha
                                l_word_cmp)))
            for l_word_candidate in list_candidates(internal_sandhi(standardize_iast(remove_avagraha(l_word_cmp)))):
                l_seth_word_list_expanded.append((l_id_outer, l_id_inner, l_word_candidate))

    for l_item in sorted(l_seth_word_list_expanded, key=lambda t: t[2][0], reverse=True): print(l_item)
    # 힣 = U+D7A3 --> above every latin & Devanagari blocks in a reverse=True search (added in the key field by list_candidates())
    l_seth_word_list_expanded = [(l_id_outer, l_id_inner, l_word_candidate, '힣' in l_key)
                                 for l_id_outer, l_id_inner, (l_key, l_word_candidate) in sorted(l_seth_word_list_expanded, key=lambda t: t[2][0], reverse=True)]

    # list of id pairs (in l_seth_word_array) that do not need to be processed anymore bc they have been found already
    l_forbidden_id_list = []
    l_prev_cand_len = 0
    # try to fit Sethuila words into Anandamakaranda chunks list
    # l_is_original = True -> candidate in its original form and not one added by list_candidates()
    for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
        if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 3:
            continue

        print(f'1P: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
        print(f'{l_underscore_k} 1P: {(float(l_pc_i) * 100) / len(l_seth_word_list_expanded):.1f}%' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
        l_prev_cand_len = len(l_candidate)
        l_is_breaking = False
        for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
            # print(f'    trying {l_candidate} in {l_amak_chunk}')
            if l_candidate in l_amak_chunk or remove_avagraha(l_candidate) in remove_avagraha(l_amak_chunk):
                # un-sandhied matching
                # replace only the first encountered match bc the following ones will correspond to other words (candidtates)
                l_amak_chunk_list[l_amak_id] = re.sub('^⌿', '', re.sub('⌿$', '', re.sub(l_candidate, '', l_amak_chunk, count=1)))
                print(f'    Candidate    [{l_candidate}] found in {l_amak_chunk}', l_amak_chunk_list[l_amak_id], l_amak_chunk_list)
                l_is_breaking = True
            else:
                l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                # print(f'    trying {l_candidate} in {l_amak_sandhied_chunk}')
                if l_candidate in l_amak_sandhied_chunk or remove_avagraha(l_candidate) in remove_avagraha(l_amak_sandhied_chunk):
                    # sandhied matching
                    print(f'    Candidate    [{l_candidate}] found in (Sandhied): {l_amak_sandhied_chunk}', l_amak_chunk_list)
                    l_is_breaking = True

            if l_is_breaking:
                l_color_style = 'green;' if l_is_original else 'green; text-decoration: underline;'
                l_display_candidate = '' if l_is_original else f' [<span style="color: CornflowerBlue;">{l_candidate}</span>]'
                l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                l_seth_word_array_2[l_id_outer][l_id_inner] = \
                    f'<span style="color: {l_color_style}">{l_seth_word_array_2[l_id_outer][l_id_inner]}</span>{l_display_candidate}'
                l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                    f'<span style="color: {l_color_style}">{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</span>'
                break
    print(f'{l_underscore_k}     ', ' ' * l_prev_cand_len * 3, end='\r', file=sys.stderr)

    # fitting Sethuila words using difflib's SequenceMatcher
    for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
        # if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5 or not l_is_original:
        if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5:
            continue

        print(f'xP: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
        print(f'{l_underscore_k} xP: {(float(l_pc_i) * 100) / len(l_seth_word_list_expanded):.1f}%' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
        l_prev_cand_len = len(l_candidate)
        l_is_breaking = False
        # allowed difference of increasing size
        for l_critical in [1, 2, 3, 4]:
            if l_is_breaking:
                break

            for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
                if l_is_breaking:
                    break

                l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                l_strings_to_match = []
                for l_stm_block in [l_amak_chunk, remove_avagraha(l_amak_chunk), l_amak_sandhied_chunk, remove_avagraha(l_amak_sandhied_chunk)]:
                    l_strings_to_match += [s for s in re.split('⌿+', l_stm_block)]
                for l_string_to_match in l_strings_to_match:
                    if l_candidate == 'bahutarasneha': print('   ', l_critical, l_string_to_match)

                    l_sm = difflib.SequenceMatcher(None, l_candidate, l_string_to_match)
                    l_frag_list = []
                    l_begin_b = 0
                    l_begin_a = 0
                    l_diff_count = 0
                    l_match_count = 0

                    l_start_found_in_b = -1
                    for a, b, s in l_sm.get_matching_blocks():
                        l_frag_dif_a = l_candidate[l_begin_a:a] if a > l_begin_a else ''
                        l_frag_dif_b = l_string_to_match[l_begin_b:b] if b > l_begin_b else ''

                        if l_start_found_in_b == -1:
                            l_start_found_in_b = b

                        if len(l_frag_dif_a + l_frag_dif_b) > 0 and \
                                not (l_begin_b == 0 and len(l_frag_dif_a) == 0) and \
                                not (s == 0 and len(l_frag_dif_a) == 0):
                            l_diff_count += len(l_frag_dif_a) if len(l_frag_dif_a) > len(l_frag_dif_b) else len(l_frag_dif_b)
                            if l_candidate == 'bahutarasneha':
                                l_frag_list.append(f'[{len(l_frag_dif_a)}]{l_frag_dif_a}/[{len(l_frag_dif_b)}]{l_frag_dif_b}<{l_diff_count}>')
                            else:
                                l_frag_list.append(f'{l_frag_dif_a}/{l_frag_dif_b}')
                            # if l_candidate == 'niṣkarmatāṃ': print('       ', f'{l_diff_count}')

                        l_match_count += s
                        if s > 0:
                            l_frag_list.append(f'(<span style="color: CornflowerBlue;">{l_string_to_match[b: b + s]}</span>)')

                        l_begin_b = b + s  # start of next differing fragment in b
                        l_begin_a = a + s  # start of next differing fragment in a

                    l_span = l_diff_count + l_match_count
                    l_sm_frag_display = '_'.join(l_frag_list)
                    l_span_gap = abs(l_span - len(l_candidate))
                    l_match_gap = abs(l_match_count - len(l_candidate))
                    if l_candidate == 'bahutarasneha': print('       ', f'd: {l_diff_count} m: {l_match_count} sp: {l_span} len: {len(l_candidate)}', l_span_gap, l_match_gap, l_sm_frag_display)
                    if l_span_gap > 2 * l_critical + 1 or l_match_gap > l_critical + 1 or '⌿' in l_sm_frag_display:
                        continue

                    if l_diff_count <= l_critical:
                        print(f'    Found: {l_sm_frag_display}')
                        l_color_style = 'DarkGreen;' if l_diff_count <= 1 else 'Maroon;'
                        l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                        # f'[{len(l_candidate)} {l_span_b} [{l_start_found_in_b} {l_begin_b} {l_string_to_match}] {abs(l_span_b - len(l_candidate))}] ' + \
                        l_seth_word_array_2[l_id_outer][l_id_inner] = \
                            f'<span style="font-weight: bold; color: {l_color_style}">{l_seth_word_array_2[l_id_outer][l_id_inner]}</span> ' + \
                            f'[{l_diff_count} <span style="color: CornflowerBlue;">{l_candidate}</span> {l_sm_frag_display}]'
                        l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                            f'<span style="font-weight: bold; color: {l_color_style}">{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</span>'
                        l_is_breaking = True
                        break
    print(f'{l_underscore_k}     ', ' ' * l_prev_cand_len * 3, end='\r', file=sys.stderr)

    # try to fit Sethuila words into Anandamakaranda chunks list, 2nd and 3rd pass: 1 and 2 letters change
    if l_do_one_two_letters:
        # 1 letter change
        for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
            if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5:
                continue

            print(f'2P: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
            print(f'{l_underscore_k} 2P: {(float(l_pc_i) * 100) / len(l_seth_word_list_expanded):.1f}' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
            l_prev_cand_len = len(l_candidate)
            l_is_breaking = False
            for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
                if l_is_breaking:
                    break

                l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                l_re_base = l_candidate
                print(f'    trying {l_candidate} in ONE {l_amak_chunk}/{l_amak_sandhied_chunk}')
                for l_i_change in range(len(l_candidate)):
                    if l_is_breaking:
                        break

                    for z, l_re_1 in [('e', l_re_base[0:l_i_change] + '.' + l_re_base[l_i_change:]),  # expansion
                                      ('i', l_re_base[0:l_i_change] + '.' + l_re_base[l_i_change + 1:]),  # iso
                                      ('c', l_re_base[0:l_i_change] + '-' + l_re_base[l_i_change + 1:])]:  # contraction

                        # kenacidapauruṣeyamityuktamuktavākyasamam
                        # if l_word_cmp == 'īśvarānityatvasyāprastutatvād':
                        #      print(l_w_mod_1)

                        l_re_x = (l_re_1
                                  .replace('(', r'\(')
                                  .replace(')', r'\)')
                                  .replace('-', r'')
                                  .replace('ऽ', r'ऽ?'))
                        try:  # l_amak_chunk
                            # l_match = re.search(l_w_mod_x, l_amak_sandhied_chunk)

                            # 2 ⌬ to prevent matching over the boundary (1 letter difference)
                            l_string_to_match = f'{l_amak_chunk}⌬⌬{remove_avagraha(l_amak_chunk)}'
                            l_match = re.search(l_re_x, l_string_to_match)
                            # print(f'    trying {l_re_x} in {l_amak_chunk}')
                            if l_match is not None:
                                l_is_breaking = True
                                l_amak_chunk_list[l_amak_id] = re.sub('^⌿', '', re.sub('⌿$', '', re.sub(l_re_x, '', l_amak_chunk)))
                                print(f'    Hillbilly 1: [{l_candidate}] {l_re_1} found in: {l_amak_chunk}', l_amak_chunk_list[l_amak_id], l_amak_chunk_list)
                            else:
                                # 2 ⌬ to prevent matching over the boundary (1 letter difference)
                                l_string_to_match = f'{l_amak_sandhied_chunk}⌬⌬{remove_avagraha(l_amak_sandhied_chunk)}'
                                l_match = re.search(l_re_x, l_string_to_match)
                                # print(f'    trying {l_re_x} in {l_amak_sandhied_chunk}')
                                if l_match is not None:
                                    l_is_breaking = True
                                    print(f'    Hillbilly 1: [{l_candidate}] {l_re_1} found in (Sandhied): {l_amak_sandhied_chunk}')

                            if l_is_breaking:
                                print(f'    NS {l_amak_chunk}')
                                print(f'    S  {l_amak_sandhied_chunk}')

                                #                                              a               b
                                l_sm = difflib.SequenceMatcher(None, l_candidate, l_string_to_match)
                                l_frag_list = []
                                l_begin_b = 0
                                l_begin_a = 0
                                for a, b, s in l_sm.get_matching_blocks():
                                    if s > 0:
                                        l_frag_dif_a = l_candidate[l_begin_a:a] if a > l_begin_a else ''
                                        l_frag_dif_b = l_string_to_match[l_begin_b:b] if b > l_begin_b else ''

                                        if len(l_frag_dif_a + l_frag_dif_b) > 0 and l_begin_b > 0:
                                            l_frag_list.append(f'{l_frag_dif_a}/{l_frag_dif_b}')

                                        l_frag_list.append(f'({l_string_to_match[b: b + s]})')
                                        l_begin_b = b + s
                                        l_begin_a = a + s
                                l_sm_frag_display = '_'.join(l_frag_list)

                                l_re_display = l_re_1.replace('.', '-')
                                l_common_len = len(l_re_display.replace('-', ''))
                                l_blocks_display = '{' + f'{z} [c: {l_candidate} re: {l_re_display}] {l_sm_frag_display}' + '}'
                                l_blocks_display_sk = '{' + f'{z} {devtrans.iast2dev(l_re_display)}' + '}'

                                l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                                l_seth_word_array_2[l_id_outer][l_id_inner] = \
                                    f'<span style="color: DarkGreen;"><b>{l_seth_word_array_2[l_id_outer][l_id_inner]}</b></span> ONE {l_blocks_display}'
                                l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                                    f'<span style="color: DarkGreen;"><b>{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</b></span> ONE {l_blocks_display_sk}'
                                break
                        except re.error as e:
                            print(e, l_re_1, file=sys.stderr)
                            sys.exit(0)
        print(f'{l_underscore_k}      ', ' ' * l_prev_cand_len * 3, end='\r', file=sys.stderr)

        # 2 letters change
        l_prev_cand_len = 0
        for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
            if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5:
                continue

            print(f'3P: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
            print(f'{l_underscore_k} 3P: {(float(l_pc_i) * 100) / len(l_seth_word_list_expanded):.1f} [{l_candidate}]' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
            l_prev_cand_len = len(l_candidate)
            l_is_breaking = False
            for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
                if l_is_breaking:
                    break

                l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                l_re_base = l_candidate
                print(f'    trying {l_candidate} in TWO {l_amak_chunk}/{l_amak_sandhied_chunk}')
                for l_i_change in range(1, len(l_candidate)):
                    if l_is_breaking: break
                    for l_j_change in range(0, l_i_change + 1):
                        if l_is_breaking: break

                        # if l_candidate == 'natveveti': print(f'    {l_i_change} {l_j_change}')
                        for z, l_re_1 in [('ee', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change:l_i_change] + '.' + l_re_base[l_i_change:]),  # expansion
                                          ('ei', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change:l_i_change] + '.' + l_re_base[l_i_change + 1:]),  # exp + iso
                                          ('ie', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change:]),  # iso + exp
                                          ('ec', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change:l_i_change] + '-' + l_re_base[l_i_change + 1:]),  # exp + contraction
                                          ('ce', l_re_base[0:l_j_change] + '-' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change:]),  # contraction + exp
                                          ('ii', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change + 1:]),  # full iso
                                          ('ci', l_re_base[0:l_j_change] + '-' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change + 1:]),  # contraction + iso
                                          ('ic', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change + 1:l_i_change] + '-' + l_re_base[l_i_change + 1:]),  # iso + contraction
                                          ('cc', l_re_base[0:l_j_change] + '-' + l_re_base[l_j_change + 1:l_i_change] + '-' + l_re_base[l_i_change + 1:])  # full contraction
                                          ]:
                            l_re_x = (l_re_1
                                      .replace('(', r'\(')
                                      .replace(')', r'\)')
                                      .replace('-', r''))
                            try:  # l_amak_chunk
                                # l_match = re.search(l_w_mod_x, l_amak_sandhied_chunk)
                                # 3 ⌬ to prevent matching over the boundary (2 letters difference)
                                l_string_to_match = f'{l_amak_chunk}⌬⌬⌬{remove_avagraha(l_amak_chunk)}'
                                l_match = re.search(l_re_x, l_string_to_match)
                                # print(f'    trying {l_re_x} in {l_amak_chunk}')
                                if l_candidate == 'kenacidapauruṣeyamityuktamuktavākyasamam': print(f'    {z} Trying {l_re_x} ({l_re_1}) in {l_amak_chunk}')
                                if l_match is not None:
                                    l_is_breaking = True
                                    l_amak_chunk_list[l_amak_id] = re.sub('^⌿', '',
                                                                          re.sub('⌿$', '',
                                                                                 re.sub(l_re_x, '', l_amak_chunk)))
                                    print(f'Hillbilly 2: [{l_candidate}] {l_re_1} found in: {l_amak_chunk}', l_amak_chunk_list[l_amak_id], l_amak_chunk_list)
                                else:
                                    # 3 ⌬ to prevent matching over the boundary (2 letters difference)
                                    l_string_to_match = f'{l_amak_sandhied_chunk}⌬⌬⌬{remove_avagraha(l_amak_sandhied_chunk)}'
                                    l_match = re.search(l_re_x, l_string_to_match)
                                    # print(f'    trying {l_re_x} in {l_amak_sandhied_chunk}')
                                    # if l_candidate == 'kenacidapauruṣeyamityuktamuktavākyasamam': print(f'    {z} Trying {l_re_x} ({l_re_1}) in {l_amak_sandhied_chunk}')
                                    if l_match is not None:
                                        l_is_breaking = True
                                        print(f'Hillbilly 2: [{l_candidate}] {l_re_1} found in (Sandhied): {l_amak_sandhied_chunk}')

                                if l_is_breaking:
                                    print(f'    NS {l_amak_chunk}')
                                    print(f'    S  {l_amak_sandhied_chunk}')

                                    l_sm = difflib.SequenceMatcher(None, l_candidate, l_string_to_match)
                                    l_frag_list = []
                                    l_begin_b = 0
                                    l_begin_a = 0
                                    for a, b, s in l_sm.get_matching_blocks():
                                        if s > 0:
                                            l_frag_dif_a = l_candidate[l_begin_a:a] if a > l_begin_a else ''
                                            l_frag_dif_b = l_string_to_match[l_begin_b:b] if b > l_begin_b else ''

                                            if len(l_frag_dif_a + l_frag_dif_b) > 0 and l_begin_b > 0:
                                                l_frag_list.append(f'{l_frag_dif_a}/{l_frag_dif_b}')

                                            l_frag_list.append(f'({l_string_to_match[b: b + s]})')
                                            l_begin_b = b + s
                                            l_begin_a = a + s
                                    l_sm_frag_display = '_'.join(l_frag_list)

                                    l_re_display = l_re_1.replace('.', '-')
                                    l_common_len = len(l_re_display.replace('-', ''))
                                    l_blocks_display = '{' + \
                                                       f'{z} [c: {l_candidate} re: {l_re_display}] ' + \
                                                       f'{l_string_to_match[0:l_match.start()]} ⇉ {l_match.group(0)} \u21D0 {l_string_to_match[l_match.end():]} ' + \
                                                       f'[{l_sm_frag_display}]' + '}'
                                    l_blocks_display_sk = '{' + f'{z} ' + devtrans.iast2dev(l_re_display) + '}'

                                    l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                                    l_seth_word_array_2[l_id_outer][l_id_inner] = \
                                        f'<span style="color: Maroon;"><b>{l_seth_word_array_2[l_id_outer][l_id_inner]}</b></span> TWO {l_blocks_display}'
                                    l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                                        f'<span style="color: Maroon;"><b>{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</b></span> TWO {l_blocks_display_sk}'
                                    break
                            except re.error as e:
                                print(e, l_re_1, file=sys.stderr)
                                sys.exit(0)
        print(f'{l_underscore_k}     ', ' ' * l_prev_cand_len * 3, end='\n', file=sys.stderr)
    else:
        print(file=sys.stderr)

    # Display all remaining words in red
    for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
        if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5 or not l_is_original:
            continue
        l_seth_word_array_2[l_id_outer][l_id_inner] = \
            f'<span style="font-weight: bold; color: red">{l_seth_word_array_2[l_id_outer][l_id_inner]}</span>'
        l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
            f'<span style="font-weight: bold; color: red">{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</span>'

    # new display list with fitted words from Sethuila
    l_v_display_list_2 = [(t,
                           ' '.join(l_seth_word_array_2[l_outer_id]),
                           f'<b>{" ".join([format_ref(l_ref) for l_ref in n if l_ref is not None])}</b>')
                          for l_outer_id, (t, _, n) in enumerate(p_seth_chunk_list)]

    l_v_display_list_2_sk = [(t,
                              ' '.join(l_seth_word_array_2_sk[l_outer_id]),
                              f'<b>{" ".join([format_ref(l_ref) for l_ref in n if l_ref is not None])}</b>')
                             for l_outer_id, (t, _, n) in enumerate(p_seth_chunk_list)]

    def format_ref(p_ref):
        return f'[{p_ref.replace("#note-pathantara-", "")}]'

    l_v_display = ' '.join([f'{t}: {remove_avagraha(l_text)} {l_n}' for t, l_text, l_n in l_v_display_list_2])
    l_v_display_sk = ' '.join([f'{l_text} {l_n}' for t, l_text, l_n in l_v_display_list_2_sk])

    l_amak_frag_table = f'<table style="margin-top: .5em;"><tr><th colspan=2>Amak Chunks</th></tr>\n<tr><th>Un-sandhied</th><th>Sandhied</th></tr>\n'
    for s in l_amak_chunk_list_display:
        l_disp_s = remove_avagraha(s.replace('⌿', '⌿\u200b'))
        l_amak_frag_table += f'<tr><td class="inner">{l_disp_s}</td><td class="inner">{remove_avagraha(external_sandhi(s, p_insert_invisible=True))}</td></tr>'
    l_amak_frag_table += '</table>'

    # iti kṛ pāṭhaḥ iti go pāṭhaḥ “ityabhidhānam”
    if l_underscore_k in g_sethuila_sarvamula_note.keys():
        l_notes_sloka_list = [(format_ref(l_ref),
                               re.sub(r'“([^”]+)”[-,\s]*iti kṛ pāṭhaḥ', r'<span style="color: Coral;">\1</span> (kṛ)',
                               re.sub(r'“([^”]+)”[-,\s]*iti go pāṭhaḥ', r'<span style="color: DarkOrange;">\1</span> (go)',
                               #re.sub(r'iti go pāṭhaḥ', '<span style="color: DarkOrange;">iti go pāṭhaḥ</span>',
                               devtrans.dev2iast(l_note_text))))
                              for l_ref, l_note_text in g_sethuila_sarvamula_note[l_underscore_k]]
        l_notes_block = f'<br/><b>Notes</b>:<br/>{"<br/>".join([f"{l_ref}: {l_note_text}" for l_ref, l_note_text in l_notes_sloka_list])}'
        l_notes_block_sk = f'<br/><b>Notes</b>:<br/>{"<br/>".join([f"{format_ref(l_ref)}: {l_note_text}" for l_ref, l_note_text in g_sethuila_sarvamula_note[l_underscore_k]])}'
    else:
        l_notes_block = ''
        l_notes_block_sk = ''

    l_v_display += f'{l_notes_block}{l_amak_frag_table}'
    l_v_display_sk += f'{l_notes_block_sk}'

    l_seth_cell_r = f'<td class="outer"><b>Sethuila</b>: {l_v_display}</td>'
    l_seth_cell_sk_r = f'<td class="outer"><b>Sethuila</b>: {l_v_display_sk}</td>'

    return l_seth_cell_r, l_seth_cell_sk_r
# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    # anandamak('brahmasūtrabhāṣyam.html')
    # sethuila('brahmasūtrabhāṣyam_s.html')
    anandamak('śrīmadbhagavadgītābhāṣyam.html')
    sethuila('śrīmadbhagavadgītābhāṣyam_s.html')

    parallel_dump()

    for l_verse_k in g_sethuila_mula_note.keys():
        print(f'{l_verse_k:5} {g_sethuila_mula_note[l_verse_k]}')

    for l_verse_k, v in g_sethuila_sarvamula_note.items():
        print(f'{l_verse_k:5} {v}')

    l_fout_sk = open('bhasyha_compare_sk.html', 'w', encoding="utf-8")

    l_do_one_two_letters = False

    with open('bhasyha_compare.html', 'w', encoding="utf-8") as l_fout:
        l_fout.write(g_html_header)
        l_fout_sk.write(g_html_header)
        l_anandamak_prev = []
        l_seth_prev = []
        # for _, l_underscore_k in sorted([(re.sub(r'^(\d)_', r'0\1_', re.sub(r'_(\d)$', r'_0\1', l_k)),
        #                                   l_k
        #                                  ) for l_k in ['0_0'] + list(g_gita_tei_file.keys())]):
        for l_underscore_k in sorted(['0_0'] + list(g_gita_tei_file.keys()), key=lambda k: re.sub(r'^(\d)_', r'0\1_', re.sub(r'_(\d)$', r'_0\1', k))):
            if l_underscore_k == '5_1':
                break # 2_18 2_45 3_2 2_25

            print(f'BEGIN {l_underscore_k}', end=' ')

            l_amak_found = False
            l_seth_found = False

            if l_underscore_k in g_parallel_bhashya.keys():
                # in the case of Anandamak, v contains a (l_intro_text, l_full_bhashya_text) pair
                # in the case of Sethuila, it contains a list of chunks with their class and notes
                for l_source_key, v in g_parallel_bhashya[l_underscore_k].items():
                    if l_source_key == 'Anandamak':
                        l_amak_found = True
                        l_intro_text, l_full_bhashya_text = v
                        print(l_intro_text, l_full_bhashya_text)
                        l_anandamak_prev.append((l_underscore_k, remove_invisibles(l_intro_text), remove_invisibles(l_full_bhashya_text)))
                        print(f'ANANDAMAK {len(l_anandamak_prev)} {len(l_seth_prev)}')
                    else: # l_source_key == 'Sethuila'
                        l_seth_found = True
                        l_seth_prev += [(t, remove_invisibles(l_text).split(' '), n) for t, l_text, n in v]
                        print(f'SETHUILA {len(l_anandamak_prev)} {len(l_seth_prev)}')

            # purge accumulators if found on both sides
            if l_amak_found and l_seth_found:
                l_rowspan = 2
            else:
                print(f'MULA ONLY')
                l_rowspan = 1

            l_mula = g_gita_tei_file[l_underscore_k] + f" ॥ {l_underscore_k.replace('_', '.')} ॥" if l_underscore_k != "0_0" else "Introduction"
            l_fout.write(f'<tr><td class="outer" rowspan="{l_rowspan}">{l_underscore_k}</td>' +
                         f'<td class="outer" style="text-align: center; background-color: #ccc;" colspan="2">{l_mula} {len(l_anandamak_prev)}</td></tr>\n')
            # l_fout.write(f'<tr><td class="outer">{l_underscore_k:5}</td>\n')

            l_mula_sk = devtrans.iast2dev(g_gita_tei_file[l_underscore_k]) + f" ॥ {l_underscore_k.replace('_', '.')} ॥" if l_underscore_k != "0_0" else "Introduction"
            l_fout_sk.write(f'<tr><td class="outer" rowspan="{l_rowspan}">{l_underscore_k}</td>' +
                            f'<td class="outer" style="text-align: center; background-color: #ccc;" colspan="2">{l_mula_sk}</td></tr>\n')

            if l_amak_found and l_seth_found:
                l_amak_chunk_list, l_anandamak_cell, l_anandamak_cell_sk = process_anandamak(l_anandamak_prev)
                l_seth_cell, l_seth_cell_sk = process_seth(l_amak_chunk_list, l_seth_prev)

                l_anandamak_prev = []
                l_seth_prev = []

                l_fout.write(f'<tr>{l_anandamak_cell}{l_seth_cell}</tr>\n')
                l_fout_sk.write(f'<tr>{l_anandamak_cell_sk}{l_seth_cell_sk}</tr>\n')

        l_fout.write(f"""</table>
        </body>
            </html>
        """)

    # for l_verse_k in g_tei_changes.keys():
    #     l_original = g_gita_tei_file_bc[l_verse_k]
    #     print(f'{l_verse_k:5}', l_original)
    #     for l_v1, l_v2 in g_tei_changes[l_verse_k]:
    #         l_original = l_original.replace(l_v1, l_v2)
    #         print(f'      {l_original} ({l_v1} --> {l_v2}')


# if l_bs_vn == '1_1_29':
#     l_v_comp = l_v_comp.replace('cedadhyātmasambhandhabhūmā', 'cedadhyātmasambandhabhūmā')
# elif l_bs_vn == '1_2_22':
#     l_v_comp = l_v_comp.replace('ṃnetarau', 'ṃcanetarau')
# elif l_bs_vn == '1_3_23':
#     l_v_comp = l_v_comp.replace('āpi', 'api')
# elif l_bs_vn == '1_3_27':
#     l_v_comp = l_v_comp.replace('karmaṇeti', 'karmaṇīti')
# elif l_bs_vn == '1_3_34':
#     l_v_comp = l_v_comp.replace('tadā\'\'dravaṇāt', 'tadādravaṇāt')
# elif l_bs_vn == '1_4_1':
#     l_v_comp = l_v_comp.replace('śarīrarūpakavinyastagṛhīterdaśaryati', 'śarīrarūpakavinyastagṛhīterdarśayati')
# elif l_bs_vn == '1_4_5':
#     l_v_comp = l_v_comp.replace('vadatītī', 'vadatīti')
# elif l_bs_vn == '2_1_16':
#     l_v_comp = l_v_comp.replace('copalabdeḥ', 'copalabdheḥ')

# l_amak_text_0 = ''
# if len(l_anandamak_prev) > 0:
#     l_amak_found = True
#     l_amak_chunk_list, l_anandamak_cell, l_anandamak_cell_sk = process_anandamak(' '.join(l_anandamak_prev))
# else:
#     l_anandamak_cell = '<td class="outer"></td>\n'
#     l_anandamak_cell_sk = '<td class="outer"></td>\n'

# l_amak_text_2 = remove_invisibles(v)
#
# l_amak_text_x = re.sub(r'[“”‘’,;:\-?!]', r' ',
#                 re.sub(r'\([^\d(]*\d+[^)]*\)', r' ',        # (ṛ.10.72.2) (bhāga.1.2.31) (bṛ.u.6.3.7)
#                 re.sub(r'(\s*\.\s*\d+)+\.?', lambda m : m.group(1).replace(' ', ''), # cleanup of sequences of dots and numbers
#                 re.sub(r'\(\s+(\S)', r'(\1',                # space after (
#                 re.sub(r'(\S)\s+\)', r'\1)',                # space before )
#                 re.sub(r'<[^>]+>', r'',
#                 re.sub(r'॥\s*[ \d]+\s*॥', r'',
#                 re.sub(r'(\w)(?:’’|\'\')(\w)', r"\1ऽ\2",    # Avagraha (2 of them)
#                 re.sub(r'(\w)[’\'](\w)', r"\1ऽ\2",          # Avagraha (single one)
#                        l_amak_text_2)))))))))
#
# # chunk (⌅) and word (⌿) separators ('⌬' = end)
# l_amak_text_x_1 = re.sub(r'[=॥।–.)(\[\]]', '⌅', l_amak_text_x + '⌬')
# l_amak_text_x_2 = re.sub(r'\s+', '⌿', l_amak_text_x_1)
# l_amak_text_x_3 = re.sub(r'⌿*(?:⌿*⌅)+⌿*', '⌅', l_amak_text_x_2)
# l_amak_text_x_4 = re.sub(r'^⌿|⌅⌬|⌿⌬', '', l_amak_text_x_3)
# l_amak_chunk_list = [standardize_iast(s).replace('⌬', '') for s in l_amak_text_x_4.split('⌅')]
#
# l_v_display = standardize_iast(remove_avagraha(l_amak_text_x))
# # l_v_display = standardize_iast(v) + f'\n<br/><b>l_amak_chunk_list</b>: <br/>[{l_amak_text_x}]<br/>[{l_amak_text_x_1}]<br/>[{l_amak_text_x_2}]<br/>[{l_amak_text_x_3}]<br/>[{l_amak_text_x_4}] <br/>{l_amak_chunk_list}'
# l_v_display_sk = devtrans.iast2dev(re.sub(r'<[^>]+>', '', standardize_iast(v)))
#
# l_source_key = 'Anandamakaranda'
# l_v_display_2 = ''
#
# l_anandamak_cell = f'<td class="outer"><b>{l_source_key}</b>: {l_v_display}</td>\n'
# l_anandamak_cell_sk = f'<td class="outer"><b>{l_source_key}</b>: {l_v_display_sk}</td>\n'

# l_v_display_list_2 = []

                    # l_amak_chunk_list_display = []

                    # def format_ref(p_ref):
                    #     return f'[{p_ref.replace("#note-pathantara-", "")}]'
                    #
                    # if not l_amak_found:
                    #     l_v_display_list_2 = [(t, ' '.join(l_tl), ' '.join([f'<b>{format_ref(l_n)}</b>' for l_n in n_list if l_n is not None])) for t, l_tl, n_list in l_v_display_list]
                    #     l_v_display_list_2_sk = [(t, ' '.join([devtrans.iast2dev(w) for w in l_tl]), ' '.join([f'<b>{format_ref(l_n)}</b>' for l_n in n_list if l_n is not None])) for t, l_tl, n_list in l_v_display_list]
                    # else:
                    #     l_amak_chunk_list_display = copy.deepcopy(l_amak_chunk_list)
                    #
                    #     # original Sethuila Chunks with words split (no Sandhi yet)
                    #     l_seth_chunks_list = [(t, l_text.split(' '), n) for t, l_text, n in v]
                    #     # 2-d array of un-sandhied Sethuila words, still with punctuation attached
                    #     l_seth_word_array = [l_words_list for _, l_words_list, _ in l_seth_chunks_list]
                    #     l_seth_word_array_2 = copy.deepcopy(l_seth_word_array)
                    #     l_seth_word_array_2_sk = [[devtrans.iast2dev(l_word) for l_word in l_row] for l_row in copy.deepcopy(l_seth_word_array)]
                    #
                    #     # list of all Sethuila words, together with their outer and inner ids (ref to l_seth_word_array)
                    #     l_seth_word_list = []
                    #     for l_word_id_outer in range(len(l_seth_chunks_list)):
                    #         l_inner_word_list = l_seth_chunks_list[l_word_id_outer][1]
                    #         for l_word_id_inner in range(len(l_inner_word_list)):
                    #             l_seth_word_list.append((l_word_id_outer, l_word_id_inner, l_inner_word_list[l_word_id_inner]))
                    #
                    #     print(f'---------------------------- {l_verse_k} ----------------------------------\n', l_seth_word_list)
                    #
                    #     # Sethuila word list + candidates. Punctuation removed and internal Sandhi applied
                    #     l_seth_word_list_expanded = []
                    #     for l_id_outer, l_id_inner, l_word_cmp in l_seth_word_list:
                    #         if not l_word_cmp == '॥' and not re.match(r'\d+', l_word_cmp):
                    #             l_word_cmp = re.sub(r'[=॥।“”‘’–.,;:?!\-)(\[\]]', '', re.sub(r'(\w)’(\w)', r"\1'\2", l_word_cmp))
                    #             for l_word_candidate in list_candidates(internal_sandhi(standardize_iast(l_word_cmp.replace('\'', 'ऽ')))):
                    #                 l_seth_word_list_expanded.append((l_id_outer, l_id_inner, l_word_candidate))
                    #     for l_item in sorted(l_seth_word_list_expanded, key=lambda t: t[2][0], reverse=True):
                    #         print(l_item)
                    #     # 힣 = U+D7A3 --> above every latin & Devanagari blocks in a reverse=True search (added in the key field by list_candidates())
                    #     l_seth_word_list_expanded = [(l_id_outer, l_id_inner, l_word_candidate, '힣' in l_key)
                    #                                  for l_id_outer, l_id_inner, (l_key, l_word_candidate) in sorted(l_seth_word_list_expanded, key=lambda t: t[2][0], reverse=True)]
                    #
                    #     # list of id pairs (in l_seth_word_array) that do not need to be processed anymore bc they have been found already
                    #     l_forbidden_id_list = []
                    #     l_prev_cand_len = 0
                    #     # try to fit Sethuila words into Anandamakaranda chunks list
                    #     # l_is_original = True -> candidate in its original form and not one added by list_candidates()
                    #     for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
                    #         if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5:
                    #             continue
                    #
                    #         print(f'1P: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
                    #         print(f'{l_underscore_k} 1P: {(float(l_pc_i) * 100)/len(l_seth_word_list_expanded):.1f}%' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
                    #         l_prev_cand_len = len(l_candidate)
                    #         l_is_breaking = False
                    #         for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
                    #             # print(f'    trying {l_candidate} in {l_amak_chunk}')
                    #             if l_candidate in l_amak_chunk or remove_avagraha(l_candidate) in remove_avagraha(l_amak_chunk):
                    #                 # un-sandhied matching
                    #                 # replace only the first encountered match bc the following ones will correspond to other words (candidtates)
                    #                 l_amak_chunk_list[l_amak_id] = re.sub('^⌿', '', re.sub('⌿$', '', re.sub(l_candidate, '', l_amak_chunk, count=1)))
                    #                 print(f'    Candidate    [{l_candidate}] found in {l_amak_chunk}', l_amak_chunk_list[l_amak_id], l_amak_chunk_list)
                    #                 l_is_breaking = True
                    #             else:
                    #                 l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                    #                 # print(f'    trying {l_candidate} in {l_amak_sandhied_chunk}')
                    #                 if l_candidate in l_amak_sandhied_chunk or remove_avagraha(l_candidate) in remove_avagraha(l_amak_sandhied_chunk):
                    #                     # sandhied matching
                    #                     print(f'    Candidate    [{l_candidate}] found in (Sandhied): {l_amak_sandhied_chunk}', l_amak_chunk_list)
                    #                     l_is_breaking = True
                    #
                    #             if l_is_breaking:
                    #                 l_color_style = 'green;' if l_is_original else 'green; text-decoration: underline;'
                    #                 l_display_candidate = '' if l_is_original else f' [{l_candidate}]'
                    #                 l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                    #                 l_seth_word_array_2[l_id_outer][l_id_inner] = \
                    #                     f'<span style="color: {l_color_style}">{l_seth_word_array_2[l_id_outer][l_id_inner]}</span>{l_display_candidate}'
                    #                 l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                    #                     f'<span style="color: {l_color_style}">{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</span>'
                    #                 break
                    #     print(f'{l_underscore_k}     ', ' ' * l_prev_cand_len * 3, end='\r', file=sys.stderr)
                    #
                    #     # fitting Sethuila words using difflib's SequenceMatcher
                    #     for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
                    #         # if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5 or not l_is_original:
                    #         if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5:
                    #             continue
                    #
                    #         print(f'xP: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
                    #         print(f'{l_underscore_k} xP: {(float(l_pc_i) * 100)/len(l_seth_word_list_expanded):.1f}%' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
                    #         l_prev_cand_len = len(l_candidate)
                    #         l_is_breaking = False
                    #         # allowed difference of increasing size
                    #         for l_critical in [1, 2, 3, 4]:
                    #             if l_is_breaking:
                    #                 break
                    #
                    #             for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
                    #                 if l_is_breaking:
                    #                     break
                    #
                    #                 l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                    #                 l_strings_to_match = []
                    #                 for l_stm_block in [l_amak_chunk, remove_avagraha(l_amak_chunk), l_amak_sandhied_chunk, remove_avagraha(l_amak_sandhied_chunk)]:
                    #                     l_strings_to_match += [s for s in re.split('⌿+', l_stm_block)]
                    #                 for l_string_to_match in l_strings_to_match:
                    #                     if l_candidate == 'bahutarasneha': print('   ', l_critical, l_string_to_match)
                    #
                    #                     l_sm = difflib.SequenceMatcher(None, l_candidate, l_string_to_match)
                    #                     l_frag_list = []
                    #                     l_begin_b = 0
                    #                     l_begin_a = 0
                    #                     l_diff_count = 0
                    #                     l_match_count = 0
                    #
                    #                     l_start_found_in_b = -1
                    #                     for a, b, s in l_sm.get_matching_blocks():
                    #                         l_frag_dif_a = l_candidate[l_begin_a:a] if a > l_begin_a else ''
                    #                         l_frag_dif_b = l_string_to_match[l_begin_b:b] if b > l_begin_b else ''
                    #
                    #                         if l_start_found_in_b == -1:
                    #                             l_start_found_in_b = b
                    #
                    #                         if len(l_frag_dif_a + l_frag_dif_b) > 0 and \
                    #                                 not (l_begin_b == 0 and len(l_frag_dif_a) == 0) and \
                    #                                 not (s == 0 and len(l_frag_dif_a) == 0):
                    #                             l_diff_count += len(l_frag_dif_a) if len(l_frag_dif_a) > len(l_frag_dif_b) else len(l_frag_dif_b)
                    #                             if l_candidate == 'bahutarasneha':
                    #                                 l_frag_list.append(f'[{len(l_frag_dif_a)}]{l_frag_dif_a}/[{len(l_frag_dif_b)}]{l_frag_dif_b}<{l_diff_count}>')
                    #                             else:
                    #                                 l_frag_list.append(f'{l_frag_dif_a}/{l_frag_dif_b}')
                    #                             # if l_candidate == 'niṣkarmatāṃ': print('       ', f'{l_diff_count}')
                    #
                    #                         l_match_count += s
                    #                         if s > 0:
                    #                             l_frag_list.append(f'({l_string_to_match[b: b + s]})')
                    #
                    #                         l_begin_b = b + s # start of next differing fragment in b
                    #                         l_begin_a = a + s # start of next differing fragment in a
                    #
                    #                     l_span = l_diff_count + l_match_count
                    #                     l_sm_frag_display = '_'.join(l_frag_list)
                    #                     l_span_gap = abs(l_span - len(l_candidate))
                    #                     l_match_gap = abs(l_match_count - len(l_candidate))
                    #                     if l_candidate == 'bahutarasneha': print('       ', f'd: {l_diff_count} m: {l_match_count} sp: {l_span} len: {len(l_candidate)}', l_span_gap, l_match_gap, l_sm_frag_display)
                    #                     if  l_span_gap > 2*l_critical + 1 or l_match_gap > l_critical + 1 or '⌿' in l_sm_frag_display:
                    #                         continue
                    #
                    #                     if l_diff_count <= l_critical:
                    #                         print(f'    Found: {l_sm_frag_display}')
                    #                         l_color_style = 'DarkGreen;' if l_diff_count <= 1 else 'Maroon;'
                    #                         l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                    #                         # f'[{len(l_candidate)} {l_span_b} [{l_start_found_in_b} {l_begin_b} {l_string_to_match}] {abs(l_span_b - len(l_candidate))}] ' + \
                    #                         l_seth_word_array_2[l_id_outer][l_id_inner] = \
                    #                             f'<span style="font-weight: bold; color: {l_color_style}">{l_seth_word_array_2[l_id_outer][l_id_inner]}</span> ' + \
                    #                             f'[{l_diff_count} {l_candidate} {l_sm_frag_display}]'
                    #                         l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                    #                             f'<span style="font-weight: bold; color: {l_color_style}">{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</span>'
                    #                         l_is_breaking = True
                    #                         break
                    #     print(f'{l_underscore_k}     ', ' ' * l_prev_cand_len * 3, end='\r', file=sys.stderr)
                    #
                    #     # try to fit Sethuila words into Anandamakaranda chunks list, 2nd and 3rd pass: 1 and 2 letters change
                    #     if l_do_one_two_letters:
                    #         # 1 letter change
                    #         for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
                    #             if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5:
                    #                 continue
                    #
                    #             print(f'2P: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
                    #             print(f'{l_underscore_k} 2P: {(float(l_pc_i) * 100)/len(l_seth_word_list_expanded):.1f}' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
                    #             l_prev_cand_len = len(l_candidate)
                    #             l_is_breaking = False
                    #             for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
                    #                 if l_is_breaking:
                    #                     break
                    #
                    #                 l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                    #                 l_re_base = l_candidate
                    #                 print(f'    trying {l_candidate} in ONE {l_amak_chunk}/{l_amak_sandhied_chunk}')
                    #                 for l_i_change in range(len(l_candidate)):
                    #                     if l_is_breaking:
                    #                         break
                    #
                    #                     for  z, l_re_1 in [('e', l_re_base[0:l_i_change] + '.' + l_re_base[l_i_change:]),       # expansion
                    #                                        ('i', l_re_base[0:l_i_change] + '.' + l_re_base[l_i_change + 1:]),   # iso
                    #                                        ('c', l_re_base[0:l_i_change] + '-' + l_re_base[l_i_change + 1:])]:  # contraction
                    #
                    #                         # kenacidapauruṣeyamityuktamuktavākyasamam
                    #                         # if l_word_cmp == 'īśvarānityatvasyāprastutatvād':
                    #                         #      print(l_w_mod_1)
                    #
                    #                         l_re_x = (l_re_1
                    #                                         .replace('(', r'\(')
                    #                                         .replace(')', r'\)')
                    #                                         .replace('-', r'')
                    #                                         .replace('ऽ', r'ऽ?'))
                    #                         try: # l_amak_chunk
                    #                             # l_match = re.search(l_w_mod_x, l_amak_sandhied_chunk)
                    #
                    #                             # 2 ⌬ to prevent matching over the boundary (1 letter difference)
                    #                             l_string_to_match = f'{l_amak_chunk}⌬⌬{remove_avagraha(l_amak_chunk)}'
                    #                             l_match = re.search(l_re_x, l_string_to_match)
                    #                             # print(f'    trying {l_re_x} in {l_amak_chunk}')
                    #                             if l_match is not None:
                    #                                 l_is_breaking = True
                    #                                 l_amak_chunk_list[l_amak_id] = re.sub('^⌿', '', re.sub('⌿$', '', re.sub(l_re_x, '', l_amak_chunk)))
                    #                                 print(f'    Hillbilly 1: [{l_candidate}] {l_re_1} found in: {l_amak_chunk}', l_amak_chunk_list[l_amak_id], l_amak_chunk_list)
                    #                             else:
                    #                                 # 2 ⌬ to prevent matching over the boundary (1 letter difference)
                    #                                 l_string_to_match = f'{l_amak_sandhied_chunk}⌬⌬{remove_avagraha(l_amak_sandhied_chunk)}'
                    #                                 l_match = re.search(l_re_x, l_string_to_match)
                    #                                 # print(f'    trying {l_re_x} in {l_amak_sandhied_chunk}')
                    #                                 if l_match is not None:
                    #                                     l_is_breaking = True
                    #                                     print(f'    Hillbilly 1: [{l_candidate}] {l_re_1} found in (Sandhied): {l_amak_sandhied_chunk}')
                    #
                    #                             if l_is_breaking:
                    #                                 print(f'    NS {l_amak_chunk}')
                    #                                 print(f'    S  {l_amak_sandhied_chunk}')
                    #
                    #                                 #                                              a               b
                    #                                 l_sm = difflib.SequenceMatcher(None, l_candidate, l_string_to_match)
                    #                                 l_frag_list = []
                    #                                 l_begin_b = 0
                    #                                 l_begin_a = 0
                    #                                 for a, b, s in l_sm.get_matching_blocks():
                    #                                     if s > 0:
                    #                                         l_frag_dif_a = l_candidate[l_begin_a:a] if a > l_begin_a else ''
                    #                                         l_frag_dif_b = l_string_to_match[l_begin_b:b] if b > l_begin_b else ''
                    #
                    #                                         if len(l_frag_dif_a + l_frag_dif_b) > 0 and l_begin_b > 0:
                    #                                             l_frag_list.append(f'{l_frag_dif_a}/{l_frag_dif_b}')
                    #
                    #                                         l_frag_list.append(f'({l_string_to_match[b: b+s]})')
                    #                                         l_begin_b = b + s
                    #                                         l_begin_a = a + s
                    #                                 l_sm_frag_display = '_'.join(l_frag_list)
                    #
                    #                                 l_re_display = l_re_1.replace('.', '-')
                    #                                 l_common_len = len(l_re_display.replace('-', ''))
                    #                                 l_blocks_display = '{' + f'{z} [c: {l_candidate} re: {l_re_display}] {l_sm_frag_display}' + '}'
                    #                                 l_blocks_display_sk = '{' + f'{z} {devtrans.iast2dev(l_re_display)}' + '}'
                    #
                    #                                 l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                    #                                 l_seth_word_array_2[l_id_outer][l_id_inner] = \
                    #                                     f'<span style="color: DarkGreen;"><b>{l_seth_word_array_2[l_id_outer][l_id_inner]}</b></span> ONE {l_blocks_display}'
                    #                                 l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                    #                                     f'<span style="color: DarkGreen;"><b>{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</b></span> ONE {l_blocks_display_sk}'
                    #                                 break
                    #                         except re.error as e:
                    #                             print(e, l_re_1, file=sys.stderr)
                    #                             sys.exit(0)
                    #         print(f'{l_underscore_k}      ', ' ' * l_prev_cand_len * 3, end='\r', file=sys.stderr)
                    #
                    #         # 2 letters change
                    #         l_prev_cand_len = 0
                    #         for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
                    #             if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5:
                    #                 continue
                    #
                    #             print(f'3P: {l_candidate}', l_id_outer, l_id_inner, l_amak_chunk_list)
                    #             print(f'{l_underscore_k} 3P: {(float(l_pc_i) * 100) / len(l_seth_word_list_expanded):.1f} [{l_candidate}]' + ' ' * l_prev_cand_len * 2, end='\r', file=sys.stderr)
                    #             l_prev_cand_len = len(l_candidate)
                    #             l_is_breaking = False
                    #             for l_amak_id, l_amak_chunk in [(i, s) for i, s in enumerate(l_amak_chunk_list) if len(s) > 0 and len(s) + 2 >= len(l_candidate)]:
                    #                 if l_is_breaking:
                    #                     break
                    #
                    #                 l_amak_sandhied_chunk = external_sandhi(l_amak_chunk)
                    #                 l_re_base = l_candidate
                    #                 print(f'    trying {l_candidate} in TWO {l_amak_chunk}/{l_amak_sandhied_chunk}')
                    #                 for l_i_change in range(1, len(l_candidate)):
                    #                     if l_is_breaking: break
                    #                     for l_j_change in range(0, l_i_change+1):
                    #                         if l_is_breaking: break
                    #
                    #                         # if l_candidate == 'natveveti': print(f'    {l_i_change} {l_j_change}')
                    #                         for z, l_re_1 in [('ee', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change    :l_i_change] + '.' + l_re_base[l_i_change:]),      # expansion
                    #                                           ('ei', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change    :l_i_change] + '.' + l_re_base[l_i_change + 1:]),  # exp + iso
                    #                                           ('ie', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change:]),      # iso + exp
                    #                                           ('ec', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change    :l_i_change] + '-' + l_re_base[l_i_change + 1:]),  # exp + contraction
                    #                                           ('ce', l_re_base[0:l_j_change] + '-' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change:]),      # contraction + exp
                    #                                           ('ii', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change + 1:]),  # full iso
                    #                                           ('ci', l_re_base[0:l_j_change] + '-' + l_re_base[l_j_change + 1:l_i_change] + '.' + l_re_base[l_i_change + 1:]),  # contraction + iso
                    #                                           ('ic', l_re_base[0:l_j_change] + '.' + l_re_base[l_j_change + 1:l_i_change] + '-' + l_re_base[l_i_change + 1:]),  # iso + contraction
                    #                                           ('cc', l_re_base[0:l_j_change] + '-' + l_re_base[l_j_change + 1:l_i_change] + '-' + l_re_base[l_i_change + 1:])   # full contraction
                    #                                           ]:
                    #                             l_re_x = (l_re_1
                    #                                       .replace('(', r'\(')
                    #                                       .replace(')', r'\)')
                    #                                       .replace('-', r''))
                    #                             try:  # l_amak_chunk
                    #                                 # l_match = re.search(l_w_mod_x, l_amak_sandhied_chunk)
                    #                                 # 3 ⌬ to prevent matching over the boundary (2 letters difference)
                    #                                 l_string_to_match = f'{l_amak_chunk}⌬⌬⌬{remove_avagraha(l_amak_chunk)}'
                    #                                 l_match = re.search(l_re_x, l_string_to_match)
                    #                                 # print(f'    trying {l_re_x} in {l_amak_chunk}')
                    #                                 if l_candidate == 'kenacidapauruṣeyamityuktamuktavākyasamam': print(f'    {z} Trying {l_re_x} ({l_re_1}) in {l_amak_chunk}')
                    #                                 if l_match is not None:
                    #                                     l_is_breaking = True
                    #                                     l_amak_chunk_list[l_amak_id] = re.sub('^⌿', '',
                    #                                                                           re.sub('⌿$', '',
                    #                                                                                  re.sub(l_re_x, '', l_amak_chunk)))
                    #                                     print(f'Hillbilly 2: [{l_candidate}] {l_re_1} found in: {l_amak_chunk}', l_amak_chunk_list[l_amak_id], l_amak_chunk_list)
                    #                                 else:
                    #                                     # 3 ⌬ to prevent matching over the boundary (2 letters difference)
                    #                                     l_string_to_match = f'{l_amak_sandhied_chunk}⌬⌬⌬{remove_avagraha(l_amak_sandhied_chunk)}'
                    #                                     l_match = re.search(l_re_x, l_string_to_match)
                    #                                     # print(f'    trying {l_re_x} in {l_amak_sandhied_chunk}')
                    #                                     # if l_candidate == 'kenacidapauruṣeyamityuktamuktavākyasamam': print(f'    {z} Trying {l_re_x} ({l_re_1}) in {l_amak_sandhied_chunk}')
                    #                                     if l_match is not None:
                    #                                         l_is_breaking = True
                    #                                         print(f'Hillbilly 2: [{l_candidate}] {l_re_1} found in (Sandhied): {l_amak_sandhied_chunk}')
                    #
                    #                                 if l_is_breaking:
                    #                                     print(f'    NS {l_amak_chunk}')
                    #                                     print(f'    S  {l_amak_sandhied_chunk}')
                    #
                    #                                     l_sm = difflib.SequenceMatcher(None, l_candidate, l_string_to_match)
                    #                                     l_frag_list = []
                    #                                     l_begin_b = 0
                    #                                     l_begin_a = 0
                    #                                     for a, b, s in l_sm.get_matching_blocks():
                    #                                         if s > 0:
                    #                                             l_frag_dif_a = l_candidate[l_begin_a:a] if a > l_begin_a else ''
                    #                                             l_frag_dif_b = l_string_to_match[l_begin_b:b] if b > l_begin_b else ''
                    #
                    #                                             if len(l_frag_dif_a + l_frag_dif_b) > 0 and l_begin_b > 0:
                    #                                                 l_frag_list.append(f'{l_frag_dif_a}/{l_frag_dif_b}')
                    #
                    #                                             l_frag_list.append(f'({l_string_to_match[b: b + s]})')
                    #                                             l_begin_b = b + s
                    #                                             l_begin_a = a + s
                    #                                     l_sm_frag_display = '_'.join(l_frag_list)
                    #
                    #                                     l_re_display = l_re_1.replace('.', '-')
                    #                                     l_common_len = len(l_re_display.replace('-', ''))
                    #                                     l_blocks_display = '{' + \
                    #                                         f'{z} [c: {l_candidate} re: {l_re_display}] ' + \
                    #                                         f'{l_string_to_match[0:l_match.start()]} ⇉ {l_match.group(0)} \u21D0 {l_string_to_match[l_match.end():]} ' + \
                    #                                         f'[{l_sm_frag_display}]' + '}'
                    #                                     l_blocks_display_sk = '{' + f'{z} ' + devtrans.iast2dev(l_re_display) + '}'
                    #
                    #                                     l_forbidden_id_list.append(f'{l_id_outer}-{l_id_inner}')
                    #                                     l_seth_word_array_2[l_id_outer][l_id_inner] = \
                    #                                         f'<span style="color: Maroon;"><b>{l_seth_word_array_2[l_id_outer][l_id_inner]}</b></span> TWO {l_blocks_display}'
                    #                                     l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                    #                                         f'<span style="color: Maroon;"><b>{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</b></span> TWO {l_blocks_display_sk}'
                    #                                     break
                    #                             except re.error as e:
                    #                                 print(e, l_re_1, file=sys.stderr)
                    #                                 sys.exit(0)
                    #         print(f'{l_underscore_k}     ', ' ' * l_prev_cand_len * 3, end='\n', file=sys.stderr)
                    #     else:
                    #         print(file=sys.stderr)
                    #
                    #     # Display all remaining words in red
                    #     for l_pc_i, (l_id_outer, l_id_inner, l_candidate, l_is_original) in enumerate(l_seth_word_list_expanded):
                    #         if f'{l_id_outer}-{l_id_inner}' in l_forbidden_id_list or len(l_candidate) <= 5 or not l_is_original:
                    #             continue
                    #         l_seth_word_array_2[l_id_outer][l_id_inner] = \
                    #             f'<span style="font-weight: bold; color: red">{l_seth_word_array_2[l_id_outer][l_id_inner]}</span>'
                    #         l_seth_word_array_2_sk[l_id_outer][l_id_inner] = \
                    #             f'<span style="font-weight: bold; color: red">{l_seth_word_array_2_sk[l_id_outer][l_id_inner]}</span>'
                    #
                    #     # new display list with fitted words from Sethuila
                    #     l_v_display_list_2 = [(t,
                    #                            ' '.join(l_seth_word_array_2[l_outer_id]),
                    #                            f'<b>{" ".join([format_ref(l_ref) for l_ref in n if l_ref is not None])}</b>')
                    #                           for l_outer_id, (t, _, n) in enumerate(l_v_display_list)]
                    #
                    #     l_v_display_list_2_sk = [(t,
                    #                            ' '.join(l_seth_word_array_2_sk[l_outer_id]),
                    #                            f'<b>{" ".join([format_ref(l_ref) for l_ref in n if l_ref is not None])}</b>')
                    #                           for l_outer_id, (t, _, n) in enumerate(l_v_display_list)]
                    #
                    # def format_ref(p_ref):
                    #     return f'[{p_ref.replace("#note-pathantara-", "")}]'
                    #
                    # l_v_display = ' '.join([f'{t}: {remove_avagraha(l_text)} {l_n}' for t, l_text, l_n in l_v_display_list_2])
                    # l_v_display_sk = ' '.join([f'{l_text} {l_n}' for t, l_text, l_n in l_v_display_list_2_sk])
                    #
                    # l_amak_frag_table = f'<table style="margin-top: .5em;"><tr><th colspan=2>Amak Chunks</th></tr>\n<tr><th>Un-sandhied</th><th>Sandhied</th></tr>\n'
                    # for s in l_amak_chunk_list_display:
                    #     l_disp_s = remove_avagraha(s.replace('⌿', '⌿\u200b'))
                    #     l_amak_frag_table += f'<tr><td class="inner">{l_disp_s}</td><td class="inner">{remove_avagraha(external_sandhi(s, p_insert_invisible=True))}</td></tr>'
                    # l_amak_frag_table += '</table>'
                    #
                    # if l_underscore_k in g_sethuila_sarvamula_note.keys():
                    #     l_notes_block = f'<br/><b>Notes</b>:<br/>{"<br/>".join([f"{format_ref(l_ref)}: {devtrans.dev2iast(l_note_text)}" for l_ref, l_note_text in g_sethuila_sarvamula_note[l_underscore_k]])}'
                    #     l_notes_block_sk = f'<br/><b>Notes</b>:<br/>{"<br/>".join([f"{format_ref(l_ref)}: {l_note_text}" for l_ref, l_note_text in g_sethuila_sarvamula_note[l_underscore_k]])}'
                    #
                    # l_v_display += f'{l_notes_block}{l_amak_frag_table}'
                    # l_v_display_sk += f'{l_notes_block_sk}'
                    #
                    # l_seth_cell = f'<td class="outer"><b>{l_source_key}</b> {l_v_display}{l_notes_block}{l_amak_frag_table}</td>'
                    # l_seth_cell_sk = f'<td class="outer"><b>{l_source_key}</b> {l_v_display_sk}{l_notes_block_sk}</td>'
