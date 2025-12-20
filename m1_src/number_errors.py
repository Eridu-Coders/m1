#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import re
# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    with open('m1B_tei_interface.cpp', 'r') as l_fin:
        l_cpp_text = l_fin.read()

    l_error_number = 1000
    def error_number():
        global l_error_number

        l_error_number += 1
        return l_error_number - 1
    # throw M1Env::M1Exception("<title> not followed by Characters", 0);
    l_cpp_text = re.sub(r'throw M1Env::M1Exception\("([^"]+)", \d+\);', lambda m: f'throw M1Env::M1Exception("{m.group(1)}", {error_number()});', l_cpp_text)

    with open('m1B_tei_interface_numbered.cpp', 'w') as l_fout:
        l_fout.write(l_cpp_text)
