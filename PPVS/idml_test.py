#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

from simple_idml import idml
from lxml import etree

my_idml_package = idml.IDMLPackage('./Indesign Exports/Bhagavatam-1-Final.idml')

print(my_idml_package.spreads)
print(my_idml_package.backing_story.tostring().decode('utf-8'))
print(my_idml_package.stories)
# print(etree.tostring(my_idml_package.backing_story.get_root().to_xml_structure_element(), pretty_print=True))
print(my_idml_package.read('Stories/Story_u10621.xml').decode('utf-8'))
