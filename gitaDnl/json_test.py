#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import json
import re
import requests
import subprocess

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    with open('1.1.json', 'r', encoding='utf-8') as f:
        # Load the JSON data from the file
        l_json = json.load(f)
        for k in l_json:
            l_txt = re.sub(r'\s+', ' ', str(l_json[k]))
            print(f'[{k:10}] {l_txt}')

        print('------------------ translations -------------------')
        for l_tr in l_json['translations']:
            print(re.sub(r'\s+', ' ', str(l_tr)))
        print('------------------ bhashyas -------------------')
        for l_com in l_json['commentaries']:
            print(re.sub(r'\s+', ' ', str(l_com)))
        print('------------------ wfw -------------------')
        for w in (l_json['word_meanings']).split(';'):
            t, m = (w.strip()).split('—')
            print(re.sub(r'\s+', ' ', f'{t}: {m}'))

    print('------------------ chap.json -------------------')
    with open('chap.json', 'r', encoding='utf-8') as g:
        # Load the JSON data from the file
        l_json = json.load(g)
        for l_chap in l_json:
            print(l_chap)
            for k in l_chap:
                l_txt = re.sub(r'\s+', ' ', str(l_chap[k]))
                print(f'    [{k:10}] {l_txt}')

    # --header 'x-rapidapi-host: bhagavad-gita3.p.rapidapi.com' --header 'x-rapidapi-key: c848c1cbbdmshf533af574fb945bp162be0jsnedd044de45f2'
    #     l_response = requests.get('https://api.bhagavadgita.io/v2/chapters/1/verses/1/',
    #                               headers={'x-rapidapi-host': 'hagavad-gita3.p.rapidapi.com',
    #                                        'x-rapidapi-key': 'c848c1cbbdmshf533af574fb945bp162be0jsnedd044de45f2'})

    # curl --request GET --url https://bhagavad-gita3.p.rapidapi.com/v2/chapters/1/verses/1/ --header 'x-rapidapi-host: bhagavad-gita3.p.rapidapi.com' --header 'x-rapidapi-key: c848c1cbbdmshf533af574fb945bp162be0jsnedd044de45f2'

    print('------------------ get verse url -------------------')
    l_command = "curl --request GET --url https://bhagavad-gita3.p.rapidapi.com/v2/chapters/1/verses/1/ --header 'x-rapidapi-host: bhagavad-gita3.p.rapidapi.com' --header 'x-rapidapi-key: c848c1cbbdmshf533af574fb945bp162be0jsnedd044de45f2'"
    l_response = subprocess.run(l_command, capture_output=True, text=True, shell=True)
    print('Stderr:', l_response.stderr)
    l_json = json.loads(l_response.stdout)
    for k in l_json:
        l_txt = re.sub(r'\s+', ' ', str(l_json[k]))
        print(f'[{k:10}] {l_txt}')

