#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import re
import requests

# PronType	Gender	VerbForm NumType	Animacy	Mood Poss	NounClass	Tense Reflex	Number	Aspect Case	Voice Abbr	Definite	Evident Typo	Deixis	Polarity Foreign	DeixisRef	Person ExtPos	Degree	Polite Clusivity

# ------------- main() -------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    for l_attr in ['PronType', 'Gender', 'VerbForm', 'NumType', 'Animacy', 'Mood', 'Poss', 'NounClass', 'Tense', 'Reflex', 'Number', 'Aspect', 'Case', 'Voice', 'Abbr', 'Definite', 'Evident', 'Typo', 'Deixis', 'Polarity', 'Foreign', 'DeixisRef', 'Person', 'ExtPos', 'Degree', 'Polite', 'Clusivity']:
        l_url = f'https://universaldependencies.org/u/feat/{l_attr}.html'
        # print(l_url)
        l_html = requests.get(l_url).text
        # print(l_html)
        # <table class="typeindex" border="1">
        # <tr>
        #   <td style="background-color:cornflowerblue;color:white"><strong>Values:</strong> </td>
        #   <td><a href="#Art">Art</a></td>
        #   <td><a href="#Dem">Dem</a></td>
        #   <td><a href="#Emp">Emp</a></td>
        #   <td><a href="#Exc">Exc</a></td>
        #   <td><a href="#Ind">Ind</a></td>
        #   <td><a href="#Int">Int</a></td>
        #   <td><a href="#Neg">Neg</a></td>
        #   <td><a href="#Prs">Prs</a></td>
        #   <td><a href="#Rcp">Rcp</a></td>
        #   <td><a href="#Rel">Rel</a></td>
        #   <td><a href="#Tot">Tot</a></td>
        # </tr>
        # </table>
        l_table = re.search(r'<table class="typeindex" border="1">\s*<tr>\s*(.*?)\s*</tr>\s*</table>', l_html, re.MULTILINE | re.DOTALL).group(1)
        # print(l_table)
        # <h2><code>PronType</code>: pronominal type</h2>
        l_label_attr = re.search(f'<h2><code>{l_attr}</code>:\s+([^<]+)</h2>', l_html).group(1)
        l_val_list = []
        for l_value_match in re.finditer(r'<td><a href="#[^"]+">([^<]+)</a></td>', l_table):
            l_val = l_value_match.group(1)
            # Prs</code></a>: personal or possessive personal pronoun or determiner</h3>
            l_label = re.search(f'{l_val}</code></a>:\s+([^<]+)</h3>', l_html).group(1)
            l_val_list.append((l_val, l_label))
        print(f"('{l_attr}', '{l_label_attr}', {l_val_list}),")
