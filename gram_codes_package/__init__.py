#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

from gram_codes_package.gram_codes_module import *

for l_family_code, _, l_code_list in g_attr_values:
    for l_code, l_penta, _ in l_code_list:
        g_gram_to_pentacode[f'{l_family_code}-{l_code}'] = l_penta

# g_nlp_pos_list
for l_penta, l_code, _ in g_nlp_pos_list:
    g_gram_to_pentacode[f'Pos-{l_code}'] = l_penta

for l_penta, l_code, _ in g_nlp_tag_list:
    g_gram_to_pentacode[f'Tag-{l_code}'] = l_penta
