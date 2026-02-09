#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import sys
import xml.etree.ElementTree as ET
import re
import difflib
import devtrans

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

                            def equalize_space_comma(s):
                                return re.sub(r",\s*(\d)", r", \1", s)

                            l_full_bhashya_text = ' '.join(l_bhashya_line_list)
                            l_full_bhashya_text = l_full_bhashya_text.replace('<br></br>', '<br/>')
                            l_full_bhashya_text = l_full_bhashya_text.replace(' ॥</em>', '</em> ॥')
                            l_full_bhashya_text = re.sub(r'(\S)-\s+<em>', r'\1-<em>', l_full_bhashya_text)
                            l_full_bhashya_text = re.sub(r'\s*<br/>\s*', r'<br/>', l_full_bhashya_text)
                            l_full_bhashya_text = re.sub(r'॥\s*(\d+)\s*॥', r'॥ \1 ॥', l_full_bhashya_text)
                            l_full_bhashya_text = re.sub(r'(\S)\s*॥\s*(\d+)</em>\s*॥', r'\1</em> ॥ \2 ॥', l_full_bhashya_text)
                            l_full_bhashya_text = re.sub(r'॥\s*(\d+(?:,\s*\d+)+)\s*॥', lambda m: f'॥ {equalize_space_comma(m.group(1))} ॥', l_full_bhashya_text)
                            print('        -->', l_full_bhashya_text)
                            g_parallel_bhashya.setdefault(l_verse_id, dict())['Anandamak'] = l_full_bhashya_text

def find_note(p_element):
    l_note = p_element.find("span[@class='pathantara']")
    if l_note:
        l_note_ref = l_note.find("a[@class='pathantara-ref iast']").attrib['href']
        return l_note_ref, l_note.attrib['data-note']
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
                        print()
                        for l_span_1 in l_div_inner.findall('span'):
                            print('       ', l_span_1.attrib['class'], end=': ')
                            # padya iast
                            if l_span_1.attrib['class'] in ['padya iast', 'inline iast']:
                                l_bhashya_fragment = l_span_1.text.strip()
                                l_bhashya_fragment = re.sub(r'(\S)\s*\.\.\s*(\d+)\s*\.\.', r'\1 ॥ \2 ॥', l_bhashya_fragment)
                                l_bhashya_fragment = re.sub(r'(\S)\s*\.\.', r'\1 ॥', l_bhashya_fragment)
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

                                l_bhashya_list.append(('[iast]', l_bhashya_fragment, [l_note_ref]))

                                # l_note = l_span_1.find("span[@class='pathantara']")
                                # if l_note:
                                #     l_note_ref = l_note.find("a[@class='pathantara-ref iast']").attrib['href']
                                #     print('Note:', l_note_ref, l_note.attrib['data-note'])
                                # else:
                                #     print()

                            # padya
                            elif l_span_1.attrib['class'] in ['padya', 'inline']:
                                l_span_2 = l_span_1.find("span[@class='pramana iast']")
                                # print(l_span_2)
                                if l_span_2 is None:
                                    l_span_2 = l_span_1.find("span[@class='pratika iast']")
                                # print(l_span_2)

                                l_frag_text = l_span_2.text
                                l_frag_text = re.sub(r'(\S)\s*\.\.\s*(\d+)\s*\.\.', r'\1 ॥ \2 ॥', l_frag_text)
                                l_frag_text = re.sub(r'(\S)\s*\.\.', r'\1 ॥', l_frag_text)
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

                                l_bhashya_list.append(('[none]', l_frag_text, [l_note_ref_1, l_note_ref_2]))
                                # l_note = l_span_1.find("span[@class='pathantara']")
                                # if l_note:
                                #     l_note_ref = l_note.find("a[@class='pathantara-ref iast']").attrib['href']
                                #     print('Note:', l_note_ref, l_note.attrib['data-note'])
                                # else:
                                #     print()
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
    return (s.replace('ṁ', 'ṃ')
             .replace('ṅ', 'ṃ')
             .replace(':', 'ḥ')
             .replace('o़\'', 'o\'')
             .replace('ṛ़', 'ṝ')) # to apply to original text/files ़

def internal_sandhi(s): # amp
    return (re.sub(r'[āa][āa]', 'ā',
                   re.sub(r'(.[aāiīuūoe])[mn]([pbtdkgm])', r'\1ṃ\2',
                          re.sub(r'(.[aāiīuūoe])nny', r'\1ṃny', s)))
            .replace('ty', 'dy').replace('ḥs', 'ss').replace('ṃj', 'ñj').replace('ṃc', 'ñc'))

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

                # print(k, v, re.match(r'^oṃ\s+(.*?)\s+oṃ', v), re.search(r'\[oṃ]\s+(.*?)\s+\[oṃ]', v))
                # l_comp.append(re.match(r'^oṃ\s+(.*?)\s+oṃ', v).group(1) if k == 'Anandamak' else re.search(r'\[oṃ]\s+(.*?)\s+\[oṃ]', v).group(1))

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

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    # anandamak('brahmasūtrabhāṣyam.html')
    # sethuila('brahmasūtrabhāṣyam_s.html')
    anandamak('śrīmadbhagavadgītābhāṣyam.html')
    sethuila('śrīmadbhagavadgītābhāṣyam_s.html')

    parallel_dump()

    for k in g_sethuila_mula_note.keys():
        print(f'{k:5} {g_sethuila_mula_note[k]}')

    for k, v in g_sethuila_sarvamula_note.items():
        print(f'{k:5} {v}')

    with open('bhasyha_compare.html', 'w') as l_fout:
        l_fout.write("""<html>
<head>
    <style>
        table, td, th{
            border: 1px solid black;
            border-collapse: collapse;
            vertical_align: top;
            padding: .5em .5em .5em 1em; 
        }
    </style>
</head>
    <body>
    <table>
    <tr><th>Sloka</th><th>Anandamakaranda</th><th>Sethuila</th></tr>
        """)
        for k, k_o in sorted([(re.sub(r'^(\d)_', r'0\1_', re.sub(r'_(\d)$', r'_0\1', l_k)), l_k) for l_k in g_parallel_bhashya.keys()]):
            l_fout.write(f'<tr><td>{k_o:5}</td>\n')
            l_anandamak_found = False
            l_note = ''
            l_anandamak_text = ''
            for l_source_key, v in g_parallel_bhashya[k_o].items():
                l_empty_cell = ''
                if l_source_key == 'Anandamak':
                    l_anandamak_found = True
                    l_anandamak_text_1 = internal_sandhi(standardize_iast(re.sub(r'\W', r' ', re.sub(r'<[^>]+>', r'', v))))
                    l_anandamak_text = re.sub(r'\s*', '', l_anandamak_text_1)
                    l_v_display = v # + ' / ' + l_anandamak_text_1
                    l_source_key = 'Anandamakaranda'
                else:
                    def format_ref(p_ref):
                        return f'[{p_ref.replace("#note-pathantara-", "")}]'

                    l_v_display_list = [(t, l_text.split(' '), n) for t, l_text, n in v]
                    l_v_display_list = [(t,
                                         ' '.join(
                                             [f'<span style="color: green;"><b>{l_tf}</b></span>'
                                              if internal_sandhi(standardize_iast(re.sub(r'\s+', '', re.sub(r'\W', '', l_tf)))) in l_anandamak_text else l_tf
                                              for l_tf in l_text]
                                         ),
                                         f'<b>{" ".join([format_ref(l_ref) for l_ref in n if l_ref is not None])}</b>') for t, l_text, n in l_v_display_list]
                    l_v_display = ' '.join([f'{t}: {l_text} {l_n}' for t, l_text, l_n in l_v_display_list])

                    if k_o in g_sethuila_sarvamula_note.keys():
                        l_note = f'<br/><b>Notes</b>:<br/>{"<br/>".join([f"{format_ref(l_ref)}: {devtrans.dev2iast(l_note_text)}" for l_ref, l_note_text in g_sethuila_sarvamula_note[k_o]])}'

                    if not l_anandamak_found:
                        l_empty_cell = '<td></td>'
                l_fout.write(f'{l_empty_cell}<td><b>{l_source_key}</b> {l_v_display}{l_note}</td>\n')
            l_fout.write('</tr>\n')
        l_fout.write("""</table>
        </body>
            </html>
        """)

    # for k in g_tei_changes.keys():
    #     l_original = g_gita_tei_file_bc[k]
    #     print(f'{k:5}', l_original)
    #     for l_v1, l_v2 in g_tei_changes[k]:
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
