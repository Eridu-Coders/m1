#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import copy
import unicodedata

# -------------------------------------- main() ------------------------------------------------------------------------

# special case '
# special case "

g_min_left_set = {'«', '‘', '“', '‛', '', '', '#', '', '¡', '§', '¿'}
g_min_mid_set = {'_', '-', '@', '·'}
g_min_right_set = {')', ']', '}', '»', '’', '”', '‚', '„', '!', '%', '&', '*', ',', '.', '/', ':', ';', '?', '…'}

if __name__ == '__main__':
    l_cat_dict = dict()
    l_left_set = copy.deepcopy(g_min_left_set)
    l_mid_set = copy.deepcopy(g_min_mid_set)
    l_right_set = copy.deepcopy(g_min_right_set)

    for c in range(0x20, 0x2E7F):
        l_char = chr(c)
        l_category = unicodedata.category(l_char)
        if l_category[:1] == 'P':
            l_cat_dict.setdefault(l_category, []).append(c)

            if l_char not in ["'", '"']:
                if l_category in ['Pd']:
                    l_mid_set.add(l_char)
                elif l_category in ['Ps', 'Pi']:
                    l_left_set.add(l_char)
                elif l_category in ['Pf', 'Pe']:
                    l_right_set.add(l_char)

    # l_ini_set = set()
    for l_cat in sorted(l_cat_dict.keys()):
        print(f'------------------ {l_cat} ------------------')
        for c in l_cat_dict[l_cat]:
            l_char = chr(c)
            l_name = unicodedata.name(l_char)
            # l_ini_word = l_name_class.split(' ')[0]
            # l_ini_set.add(l_ini_word)

            # if not(l_ini_word in ['BALINESE', 'BATAK', 'BENGALI', 'BUGINESE', 'COPTIC', 'ETHIOPIC', 'GUJARATI',
            #                                   'GURMUKHI', 'KANNADA', 'KHMER', 'MANDAIC', 'MONGOLIAN', 'MYANMAR', 'NKO', 'OGHAM',
            #                                   'PHILIPPINE', 'RUNIC', 'SAMARITAN', 'SUNDANESE', 'TAI', 'TIBETAN',
            #                                   'LEPCHA', 'LIMBU', 'TELUGU']
            #                    or re.search(r'^OL CHIKI', l_ini_word)):

            print(f'{c:4x} __{l_char}__ {l_name}')

    print()
    print('Left :', ''.join(list(l_left_set)))
    print('Mid  :', ''.join(list(l_mid_set)))
    print('Right:', ''.join(list(l_right_set)))

    # print()
    # print('\n'.join(sorted(list(l_ini_set))))
    print()
