#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import xml.etree.ElementTree as ET

import regex
import re
import itertools

g_work_title = '   <text:p text:style-name="WorkTitle">{0}</text:p>\n'
g_sub_title = '   <text:p text:style-name="WorSubTitle">{0}</text:p>\n'
g_alt_title = '   <text:p text:style-name="WorkAltTitle">{0}</text:p>\n'
g_author = '   <text:p text:style-name="WorkAuthor">{0}</text:p>\n'
g_chapter = '   <text:p text:style-name="P1">{0}</text:p>\n'
g_sloka_title = '   <text:p text:style-name="SlokaTitle">{0}</text:p>\n'
# g_sloka_title = '   <text:p text:style-name="P2">{0}</text:p>\n'
g_sloka_line = '   <text:p text:style-name="P6">{0}</text:p>\n'
g_interTitle = '   <text:p text:style-name="InterTitle">{0}</text:p>\n'
g_translBhashya = '   <text:p text:style-name="P8">{0}</text:p>\n'
g_ftn = '<text:note text:id="ftn{0}" text:note-class="footnote"><text:note-citation>{1}</text:note-citation><text:note-body><text:p text:style-name="P12">{2}</text:p></text:note-body></text:note>'

g_table_init = """   <table:table table:name="Table1" table:style-name="Table1">
    <table:table-column table:style-name="Table1.A"/>
    <table:table-column table:style-name="Table1.B"/>
    <table:table-column table:style-name="Table1.C"/>
"""
g_table_end = '   </table:table>'
g_table_row = """    <table:table-row>
     <table:table-cell table:style-name="Table1.A1" table:number-rows-spanned="{0}" office:value-type="string">
      <text:p text:style-name="P10">{1}</text:p>
     </table:table-cell>
     <table:table-cell table:style-name="Table1.A1" table:number-rows-spanned="{0}" office:value-type="string">
      <text:p text:style-name="P10">{2}</text:p>
     </table:table-cell>
     <table:table-cell table:style-name="Table1.A1" office:value-type="string">
      <text:p text:style-name="P11">{3}</text:p>
     </table:table-cell>
    </table:table-row>"""
g_table_row_add = """<table:table-row>
     <table:covered-table-cell table:style-name="Table1.A1"/>
     <table:covered-table-cell table:style-name="Table1.A1"/>
     <table:table-cell table:style-name="Table1.A1" office:value-type="string">
      <text:p text:style-name="P11">{0}</text:p>
     </table:table-cell>
    </table:table-row>"""

g_sk_span = '<text:span text:style-name="T1">{0}</text:span>'
g_sk_span_big = '<text:span text:style-name="T4">{0}</text:span>'
g_author_span = '<text:span text:style-name="T3">{0}</text:span>'

g_code_transl = {
    'GMASC/GNFEM/GNEUT': 'All Genders',
    'GMASC/GNEUT/GNFEM': 'All Genders',
    'GNFEM/GMASC/GNEUT': 'All Genders',
    'GNFEM/GNEUT/GMASC': 'All Genders',
    'GNEUT/GMASC/GNFEM': 'All Genders',
    'GNEUT/GNFEM/GMASC': 'All Genders',
    '**no POS tag**': '',
    '**no analysis**': 'No grammatical analysis available',
    'NPPRN': 'Pron.',
    'NPVRB': 'Verb',
    'NPIIV': 'in initio verbi',
    'NPIND': 'Indecl.',
    'NPADV': 'Adv:',
    'NPIIC': 'in intio compositi',
    'NPNON': 'Nominal',
    'CSVOC': 'Vocative',
    'CSLOC': 'Locative',
    'CSDAT': 'Dative',
    'CSNOM': 'Nominative',
    'CSINS': 'Instrumental',
    'CSACC': 'Accusative',
    'CSGEN': 'Genitive',
    'CSABL': 'Ablative',
    'VCMID': 'Middle voice',
    'VCPSS': 'Passive Voice',
    'VCACT': 'Active Voice',
    'GNEUT': 'Neutral',
    'GNFEM': 'Feminine',
    'GMASC': 'Masculine',
    'NSING': 'Singular',
    'NDUAL': 'Dual',
    'NPLUR': 'Plural',
    'PRSN3': '3rd. Person',
    'PRSN2': '2nd. Person',
    'PRSN1': 'First Person',
    'TNIMP': 'Imperfect',
    'TNFUT': 'Future',
    'TNPRS': 'Present',
    'TNPQP': 'PlPf.',
    'TNPST': 'Past',
    'VBCJ1': 'Conj. 1',
    'VBCJ2': 'Conj. 2',
    'VBCJ3': 'Conj. 3',
    'VBCJ4': 'Conj. 4',
    'VBCJ5': 'Conj. 5',
    'VBCJ6': 'Conj. 6',
    'VBCJ7': 'Conj. 7',
    'VBCJ8': 'Conj. 8',
    'VBCJ9': 'Conj. 9',
    'VFGER': 'Gerund',
    'VFCAU': 'Causative',
    'VFINF': 'Infinitive',
    'MDOPT': 'Optative',
    'MDBEN': 'Benedictive',
    'MDDES': 'Desiderative',
    'MDSUB': 'Subjonctive',
    'MDIMP': 'Imperative',
    '__TSL': '',
}

def samskrit_beautify(s, p_big=False):
    l_span = g_sk_span_big if p_big else g_sk_span
    return regex.sub(r'([।॥\p{Devanagari}]+\s*[।॥\p{Devanagari}]*)', lambda m: l_span.format(m.group(1)), s)

def get_transl_bhashya_list(p_elem):
    l_translators = set()
    l_bhashya_authors = set()
    for l_div1_a in p_elem.findall(f'{l_tei_namespace}div1'):
        if l_div1_a.attrib['type'] == 'chapter':
            for l_div2_a in l_div1_a.findall(f'{l_tei_namespace}div2'):
                if l_div2_a.attrib['type'] == 'sloka-block':
                    for l_div3_a in l_div2_a.findall(f'{l_tei_namespace}div3'):
                        if l_div3_a.attrib['type'] in ['translations-block', 'commentaries-block']:
                            l_is_transl_a = l_div3_a.attrib['type'] == 'translations-block'
                            for l_div4_a in l_div3_a.findall(f'{l_tei_namespace}div4'):
                                l_author_txt = l_div4_a.find(f'{l_tei_namespace}author').text
                                l_lang = l_div4_a.attrib['{http://www.w3.org/XML/1998/namespace}lang'].capitalize()
                                if l_is_transl_a:
                                    l_translators.add((l_author_txt, f'{l_author_txt} ({l_lang})'))
                                else:
                                    l_bhashya_authors.add((l_author_txt, f'{l_author_txt} ({l_lang})'))

    return sorted(list(l_translators)), sorted(list(l_bhashya_authors))

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    l_tree = ET.parse('./bg_final_ex.xml')
    l_root = l_tree.getroot()

    l_tei_namespace = '{http://www.tei-c.org/ns/1.0}'

    l_title_section_elem = l_root.find(f'{l_tei_namespace}teiHeader/{l_tei_namespace}titleStmt')
    l_text_elem = l_root.find(f'{l_tei_namespace}text')
    # print(l_title_section_elem)
    # print(l_text_elem)

    l_translator_list, l_bhashya_author_list = get_transl_bhashya_list(l_text_elem)

    with open('gita_template.fodt', 'r') as l_fin:
        l_fodt_template = l_fin.read()

    l_xml_output = ''

    l_main_title = ''
    l_sub_title = ''
    l_alt_title = ''
    l_author = ''
    for l_elem in l_title_section_elem.findall('*'):
        # print(l_elem.attrib, l_elem.text)
        if l_elem.tag == f'{l_tei_namespace}title':
            # print(l_elem.attrib['type'])
            if l_elem.attrib['type'] == 'main': l_main_title = l_elem.text
            elif l_elem.attrib['type'] == 'alt': l_alt_title = l_elem.text
            elif l_elem.attrib['type'] == 'sub': l_sub_title = l_elem.text
        elif l_elem.tag == f'{l_tei_namespace}author':
            l_author = l_elem.text

    print(f'Work Title           : {l_main_title}')
    print(f'Work Sub-Title       : {l_sub_title}')
    print(f'Work Alternate Title : {l_alt_title}')
    print(f'Work Author          : {l_author}')

    l_col_1 = ['Available Translations:']
    for i in range(len(l_translator_list)):
        l_col_1.append(f'    {i+1:2}. {[l for _, l in l_translator_list][i]}')

    l_col_2 = ['Available Bhashyas:']
    for i in range(len(l_bhashya_author_list)):
        l_col_2.append(f'    {i+1:2}. {[l for _, l in l_bhashya_author_list][i]}')

    l_max_len_col_1 = 0
    for s in l_col_1:
        if len(s) > l_max_len_col_1: l_max_len_col_1 = len(s)

    for l_left, l_right in itertools.zip_longest(l_col_1, l_col_2, fillvalue=''):
        print(f'{l_left:<{l_max_len_col_1 + 5}} {l_right}')

    l_translator_choice = input('Translators choice (list of numbers separated by spaces): ')
    l_commentators_choice = input('Bhashya authors choice (list of numbers separated by spaces): ')
    l_wfw_choice = re.sub(r'[^yn]', '', input('Word-for-Word Translation? (Y/N): ').lower())
    l_wfw_choice = 'y' in l_wfw_choice

    l_wfw_gr_choice = False
    if l_wfw_choice:
        l_wfw_gr_choice = re.sub(r'[^yn]', '', input('Word-for-Word Translation with grammar? (Y/N): ').lower())
        l_wfw_gr_choice = 'y' in l_wfw_gr_choice

    l_translator_choice = re.sub(r'\s+', ' ', re.sub(r'[^\d ]+', '', l_translator_choice)).strip()
    l_commentators_choice = re.sub(r'\s+', ' ', re.sub(r'[^\d ]+', '', l_commentators_choice)).strip()

    l_transl_ch_list = [l_translator_list[int(l_num)-1][0] for l_num in l_translator_choice.split(' ') if int(l_num)-1 < len(l_translator_list)]
    l_comm_ch_list = [l_bhashya_author_list[int(l_num)-1][0] for l_num in l_commentators_choice.split(' ') if int(l_num)-1 < len(l_bhashya_author_list)]

    l_xml_output += g_work_title.format(l_main_title)
    l_xml_output += g_alt_title.format(samskrit_beautify(l_alt_title, p_big=True))
    l_xml_output += g_sub_title.format(samskrit_beautify(l_sub_title))
    l_xml_output += g_author.format('by: ' + samskrit_beautify(l_author))

    l_note_count = 0
    l_codes = set()
    for l_div1 in l_text_elem.findall(f'{l_tei_namespace}div1'):
        print(l_div1.attrib)
        if l_div1.attrib['type'] == 'chapter':
            l_chap_num = l_div1.attrib['n']

            l_xml_output += g_chapter.format(f'Chapter {l_chap_num}')

            for l_div2 in l_div1.findall(f'{l_tei_namespace}div2'):
                # print(l_div2.attrib)
                if l_div2.attrib['type'] == 'sloka-block':
                    l_sloka_num = l_div2.attrib['n']
                    l_sloka_iter = l_div2.find(f'{l_tei_namespace}l').itertext()
                    l_sloka_list = list(l_sloka_iter)
                    l_sloka_sk = ' । '.join(l_sloka_iter)
                    l_sloka_iast = l_div2.find(f'{l_tei_namespace}seg').text
                    print(f'Sloka {l_chap_num}.{l_sloka_num}: {l_sloka_sk} {l_sloka_iast}')
                    l_xml_output += g_sloka_title.format(f'Sloka {l_chap_num}.{l_sloka_num}')

                    print(l_sloka_list)
                    l_sloka_list[0] += ' ।'
                    for l_line in l_sloka_list:
                        l_xml_output += g_sloka_line.format(l_line)

                    l_xml_output += g_translBhashya.format(f'{g_author_span.format("IAST Transliteration")} : {l_sloka_iast}')

                for l_div3 in l_div2.findall(f'{l_tei_namespace}div3'):
                    # print(l_div3.attrib)
                    if l_div3.attrib['type'] == 'wfw-block' and l_wfw_choice:
                        print('    WfW')
                        l_xml_output += g_interTitle.format('Word-for-Word Translation')
                        l_xml_output += g_table_init
                        for l_seg in l_div3.findall(f'{l_tei_namespace}seg'):
                            l_sk_word = re.sub(r'\s+', ' ', l_seg.text).strip()

                            l_grammar_note = ''
                            if l_wfw_gr_choice:
                                l_grammar_list = []
                                l_interp_grp = l_seg.find(f'{l_tei_namespace}interpGrp')
                                for l_interp in l_interp_grp.findall(f'{l_tei_namespace}interp'):
                                    # <interp type="morphology" lemma="vac" pos="NPVRB" lemmaRef="https://sanskrit.inria.fr/MW/226.html#H_vac" msd="NSING-PRSN1/PRSN3-TNPQP-VCACT">uvāca</interp>
                                    l_form_iast = l_interp.text
                                    l_lemma = l_interp.attrib['lemma']
                                    l_pos = l_interp.attrib['pos']
                                    l_analysis = l_interp.attrib['msd']

                                    l_pos = re.sub(r'॥.*$', '', l_pos)
                                    # l_codes.add(l_pos)
                                    l_pos = g_code_transl[l_pos]

                                    l_lemma = re.sub(r'॥.*$', '', l_lemma)
                                    l_analysis = re.sub(r'॥.*$', '', l_analysis)

                                    for l_gr_alt in l_analysis.split('|'):
                                        for l_gr_block in l_gr_alt.split('-'):
                                            for l_gr_value in l_gr_block.split('/'):
                                                if l_gr_value not in g_code_transl.keys():
                                                    l_codes.add(l_gr_value)

                                    for l_gv in g_code_transl.keys():
                                        l_analysis = l_analysis.replace(l_gv, g_code_transl[l_gv])

                                    # print('l_analysis:', l_analysis)
                                    l_grammar_list.append(f'{l_form_iast} [{l_lemma} {l_pos}] {l_analysis}')

                                l_grammar_note = ' + '.join(l_grammar_list)

                            l_translit = ''
                            l_meaning = ''
                            for l_interp in l_seg.findall(f'{l_tei_namespace}interp'):
                                # print(l_interp.attrib)
                                if l_interp.attrib['type'] == 'transliteration':
                                    l_translit = l_interp.text
                                elif l_interp.attrib['type'] == 'translation':
                                    l_meaning = l_interp.text

                            l_meanings = l_meaning.split('|')

                            l_sk_form_text = samskrit_beautify(l_sk_word)
                            if len(l_grammar_note) > 0:
                                l_sk_form_text += g_ftn.format(l_note_count, l_note_count+1, l_grammar_note)
                                l_note_count += 1

                            l_xml_output += g_table_row.format(len(l_meanings), l_sk_form_text, l_translit, l_meanings[0])
                            for l_mn in l_meanings[1:]:
                                l_xml_output += g_table_row_add.format(l_mn)

                            # print(f'Word: {l_sk_word} {l_translit} --> {l_meaning}')

                        l_xml_output += g_table_end
                    elif l_div3.attrib['type'] == 'translations-block' or l_div3.attrib['type'] == 'commentaries-block':
                        print('    Translations / Commentaries')

                        l_is_transl = False
                        if l_div3.attrib['type'] == 'translations-block':
                            if len(l_transl_ch_list) > 0:
                                l_xml_output += g_interTitle.format('Translations')

                            l_type = '(t)'
                            l_is_transl = True
                        else:
                            if len(l_comm_ch_list) > 0:
                                l_xml_output += g_interTitle.format('Bhashyas')
                            l_type = '(c)'

                        for l_div4 in l_div3.findall(f'{l_tei_namespace}div4'):
                            l_language = l_div4.attrib['{http://www.w3.org/XML/1998/namespace}lang'].capitalize()
                            l_author_elem = l_div4.find(f'{l_tei_namespace}author')
                            l_author = l_author_elem.text

                            if (l_is_transl and l_author in l_transl_ch_list) or \
                                    (not l_is_transl and l_author in l_comm_ch_list):

                                l_text = l_author_elem.tail

                                if not l_text:
                                    l_text_list = []
                                    for l_p in l_div4.findall('*'):
                                        if l_p.tag == f'{l_tei_namespace}p':
                                            l_text_list.append(' '.join(l_p.itertext()) if l_p.text else '')
                                    l_text = ' '.join(l_text_list)

                                print(f'        {l_type} {l_language} [{l_author}] {l_text[:100]}')
                                l_text = samskrit_beautify(l_text)
                                l_xml_output += g_translBhashya.format(f'{g_author_span.format(l_author)} ({l_language}) : {l_text}')


    with open('output.fodt', 'w') as l_fout:
        l_fout.write(l_fodt_template.replace('_*_PAYLOAD_*_', l_xml_output))

    print(l_codes)