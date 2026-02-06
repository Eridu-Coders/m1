#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import sys
import xml.etree.ElementTree as ET
import re

g_parallel = dict()

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
def anandamak(p_file):
    with (open(p_file, 'r') as l_fin):
        l_txt = l_fin.read().replace('<br>', '<br/>')
        print('Anandamak:', p_file, len(l_txt))
        try:
            l_tree = ET.fromstring(f'{l_txt}')
        except ET.ParseError as e:
            print(f'XML parse ERROR: {e.code} {e.msg} in [{l_txt[:100]}]')

        for l_div_adhyaya in l_tree.findall('div'):
            print(l_div_adhyaya.attrib['class'], l_div_adhyaya.attrib['id'])
            for l_div_verse in l_div_adhyaya.findall('div'):
                if l_div_verse.attrib['class'] == 'introduction':
                    intro_text = ' '.join([p.text for p in l_div_verse.findall('p')])
                    print('    Intro:', intro_text)
                elif l_div_verse.attrib['class'] == 'verse':
                    # print('   ', l_div_verse.attrib['class'], l_div_verse.attrib['id'])
                    # <div class="verse" id="BS_C01_S01_V01" type="sutra" data-adhikaranaid="BS_C01_S01_A001" data-adhikarana="jijñāsādhikaraṇam">
                    l_verse_id_0 = l_div_verse.attrib['id'].strip()
                    l_verse_id = re.sub(fr'^{g_id_prefix}_', '',
                                        re.sub(r'_[CSV]0?', '_', l_verse_id_0))
                    # verse-text
                    l_verse_text = ' '.join([p.text for p in l_div_verse.findall("div[@class='verse-text']/p")])
                    g_parallel.setdefault(l_verse_id, dict())['Anandamak'] = l_verse_text
                    print('    Verse:', l_verse_id_0, '-->', l_verse_id, l_verse_text)
                    for l_div_bhashya_collection in l_div_verse.findall('div'):
                        if l_div_bhashya_collection.attrib['class'] == 'bhashya-collection':
                             for l_div_bhashya in l_div_bhashya_collection.findall('div'):
                                 if l_div_bhashya.attrib['class'].strip() == 'bhashya':
                                     l_text = l_div_bhashya.text.strip() if l_div_bhashya.text else ''
                                     print('    Bhashya', l_div_bhashya.attrib['id'], l_text)
                                     l_bhashya_text_list = [l_text] if len(l_text) > 0 else []
                                     l_p_count = 0
                                     for l_it in l_div_bhashya.iter():
                                         l_b_text = l_it.text.strip() if l_it.text else ''
                                         l_b_tail = l_it.tail.strip() if l_it.tail else ''
                                         print(f"        {l_it.tag:5} {l_b_text}❉{l_b_tail}")
                                         if l_it.tag != 'div':
                                             if l_it.tag == 'p':
                                                 if len(l_b_text) > 0:
                                                     if l_p_count > 0:
                                                         l_bhashya_text_list.append('<br/>' + l_b_text)
                                                     else:
                                                         l_bhashya_text_list.append(l_b_text)
                                                 l_p_count += 1
                                             else:
                                                l_bhashya_text_list.append(f'<{l_it.tag}>{l_b_text}</{l_it.tag}>')

                                             if len(l_b_tail) > 0:
                                                 l_bhashya_text_list.append(l_b_tail)

                                     l_full_bhashya_text = ' '.join(l_bhashya_text_list)
                                     l_full_bhashya_text = l_full_bhashya_text.replace('<br></br>', '<br/>')
                                     l_full_bhashya_text = l_full_bhashya_text.replace(' ॥</em>', '</em> ॥')
                                     l_full_bhashya_text = re.sub(r'(\S)-\s<em>', r'\1-<em>', l_full_bhashya_text)
                                     l_full_bhashya_text = re.sub(r'\s*<br/>\s*', r'<br/>', l_full_bhashya_text)
                                     print('        -->', l_full_bhashya_text)

def sethuila(p_file):
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

        for l_div_outer in l_tree.findall('div'):
            print(l_div_outer.attrib['class'])
            if l_div_outer.attrib['class'] == 'content':
                for l_div_inner in l_div_outer.findall('div'):
                    print('   ', l_div_inner.attrib['class'], end=' ')
                    if l_div_inner.attrib['class'] == 'Mula':
                        l_verse_text = l_div_inner.find('span').text
                        # Mula .. [oṃ] anāvṛttiḥ śabdādanāvṛttiḥ śabdāt [oṃ].. 4/4/23..
                        l_verse_id = re.match(r'^.*\[oṃ].*\[oṃ]\.\.\s+(\d+/\d+/\d+)\.\.', l_verse_text).group(1).replace('/', '_')
                        l_existing_dict = g_parallel.setdefault(l_verse_id, dict())
                        print(l_verse_id, g_parallel[l_verse_id], l_existing_dict, end=' ')
                        l_existing_dict['Sethuila'] = l_verse_text
                        print(l_existing_dict, g_parallel[l_verse_id])
                        # g_parallel[l_verse_id] = l_existing_dict
                        print(l_verse_text)
                    elif l_div_inner.attrib['class'] == 'Sarvamula':
                        print()
                        for l_span_1 in l_div_inner.findall('span'):
                            print('       ', l_span_1.attrib['class'], end=': ')
                            # padya iast
                            if l_span_1.attrib['class'] in ['padya iast', 'inline iast']:
                                print(l_span_1.text.strip(), end=' ')
                                l_note = l_span_1.find("span[@class='pathantara']")
                                if l_note:
                                    l_note_ref = l_note.find("a[@class='pathantara-ref iast']").attrib['href']
                                    print('Note:', l_note_ref, l_note.attrib['data-note'])
                                else:
                                    print()
                            # padya
                            elif l_span_1.attrib['class'] in ['padya', 'inline']:
                                l_span_2 = l_span_1.find("span[@class='pramana iast']")
                                # print(l_span_2)
                                if l_span_2 is None:
                                    l_span_2 = l_span_1.find("span[@class='pratika iast']")
                                # print(l_span_2)
                                l_frag_text = l_span_2.text
                                print(l_frag_text, end=' ')
                                l_note = l_span_1.find("span[@class='pathantara']")
                                if l_note:
                                    l_note_ref = l_note.find("a[@class='pathantara-ref iast']").attrib['href']
                                    print('Note:', l_note_ref, l_note.attrib['data-note'])
                                else:
                                    print()
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

def standardize_iast(s):
    return s.replace('ṁ', 'ṃ')

def internal_sandhi(s):
    return re.sub(r'[āa][āa]', 'ā', s)

def parallel_dump():
    # print(g_parallel)
    for l_bs_vn, d in g_parallel.items():
        print(f'{l_bs_vn}')
        l_comp = []
        for k, v in d.items():
            try:
                l_match = re.match(r'^.*?\[?oṃ]?\s+(.*?)\s+\[?oṃ]?.*$', standardize_iast(v))
                if l_match:
                    v = l_match.group(1)
                l_v_comp = re.sub(r'\s+', '', v)
                # print(k, v, re.match(r'^oṃ\s+(.*?)\s+oṃ', v), re.search(r'\[oṃ]\s+(.*?)\s+\[oṃ]', v))
                # l_comp.append(re.match(r'^oṃ\s+(.*?)\s+oṃ', v).group(1) if k == 'Anandamak' else re.search(r'\[oṃ]\s+(.*?)\s+\[oṃ]', v).group(1))
                l_comp.append(internal_sandhi(l_v_comp))
                print(f'    {k:10} : {v}')
            except AttributeError:
                print(k, v)
                sys.exit(0)
        if len(l_comp) < 2 or l_comp[0] != l_comp[1]:
            print(l_comp)
            sys.exit(0)

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    # anandamak('brahmasūtrabhāṣyam.html')
    # sethuila('brahmasūtrabhāṣyam_s.html')
    anandamak('śrīmadbhagavadgītābhāṣyam.html')
    # sethuila('brahmasūtrabhāṣyam_s.html')

    parallel_dump()

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
