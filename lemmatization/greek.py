#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

from cltk import NLP

l_txt = "κατέβην χθὲς εἰς Πειραιᾶ μετὰ Γλαύκωνος τοῦ Ἀρίστωνος προσευξόμενός τε τῇ θεῷ καὶ ἅμα τὴν ἑορτὴν βουλόμενος θεάσασθαι τίνα τρόπον ποιήσουσιν ἅτε νῦν πρῶτον ἄγοντες. καλὴ μὲν οὖν μοι καὶ ἡ τῶν ἐπιχωρίων πομπὴ ἔδοξεν εἶναι, οὐ μέντοι ἧττον ἐφαίνετο πρέπειν ἣν οἱ Θρᾷκες ἔπεμπον."
cltk_nlp = NLP(language="grc")
cltk_doc = cltk_nlp.analyze(text=l_txt)

for t, l, p, m in zip(cltk_doc.tokens, cltk_doc.lemmata, cltk_doc.pos, cltk_doc.morphosyntactic_features):
    print(f'{t:15} {l:15} {p:8} {m}')