#!/usr/bin/python3
# -*- coding: utf-8 -*-

__author__ = 'fi11222'

import re
import devtrans
import urllib.request
import html
import sys

# https://sanskrit.inria.fr/cgi-bin/SKT/sktgraph2.cgi?
# lex=SH&
# st=t&
# us=g&
# font=deva&
# text=arjuna+uvaaca+eva.m+satatayuktaa+ye+bhaktaastvaa.m+paryupaasate+%7C+ye+caapyak.saramavyakta.m+te.saa.m+ke+yogavittamaa%3A+%7C%7C++%7C%7C&
# t=VH&
# topic=&
# mode=g&
# corpmode=&
# corpdir=&
# sentno=

def inria_sloka_words(p_sloka_txt):
    l_txt = devtrans.dev2vel(p_sloka_txt.replace('\u200d', '').replace('।', ''))
    l_url = 'https://sanskrit.inria.fr/cgi-bin/SKT/sktgraph2.cgi?lex=SH&st=t&us=f&font=deva&' \
            f'text={l_txt}&t=VH&topic=&mode=g&corpmode=&corpdir=&sentno='
    l_url = l_url.replace(' ', '+').replace('.a', '%27')
    print(l_url, file=sys.stderr)
    l_html = urllib.request.urlopen(l_url).read()
    l_html = l_html.decode('utf8').strip().replace('</table>', '</table>\n')
    l_html = re.sub('<!DOCTYPE html>.*<div class="latin12"><table class="center">',
                    '', l_html, flags=re.MULTILINE | re.DOTALL)

    l_word_list = []
    l_line_end_list = []
    for l_match in re.finditer(r'</tr>\s*<tr>', l_html, flags=re.MULTILINE | re.DOTALL):
        l_line_end_list.append(l_match.start())

    l_line_end_list_disp = ' / '.join([f'{v:,}' for v in l_line_end_list])
    print(f'{len(l_line_end_list)} lines [{l_line_end_list_disp}]', file=sys.stderr)

    l_end_second_line = l_line_end_list[1]
    l_word_num = 1
    for l_match in re.finditer(r'<td onclick="showBox\(\'.*?</td></tr></table>', l_html):
        l_match_txt = l_match.group(0)

        l_pos = l_match.start()
        l_form = ''
        m = re.search(r'this, event\)">([^<]+)</td>', l_match_txt)
        if m:
            l_form = html.unescape(m.group(1)).strip()

        l_dic_idx_list = []
        l_lemma_list = []
        for m in re.finditer(r'<a href=&quot;/DICO/\d+.html#([^&]*)&quot;>\s*<i>([^<]+)</i></a>', l_match_txt):
            l_dic_idx_list.append(html.unescape(m.group(1).strip()))
            l_lemma_list.append(html.unescape(m.group(2).strip()))

        l_dict_index = '-'.join(l_dic_idx_list)
        l_lemma = '-'.join(l_lemma_list)
        l_is_below = '*' if l_match.start() >= l_end_second_line else ''
        l_pos_disp = f'{l_pos:6,.0f}{l_is_below}'
        print(
            f'{l_pos_disp:7} {l_dict_index:20}  {devtrans.dev2iast(l_form):15} --> '
            f'{devtrans.dev2iast(l_lemma):15} [{l_form} --> {l_lemma}]', file=sys.stderr)

        l_tei_word = f'<w xml:id="BG_12.{l_sloka_num}.{l_word_num}" lemma="{l_lemma}">{l_form}' \
                     f'<transliteration standard="ISO 15919">{devtrans.dev2iast(l_form)}</transliteration>' \
                     f'<dict-ref dict-name="INRIA">{l_dict_index}</dict-ref</w>'
        l_word_num += 1
        if not l_is_below:
            l_word_list.append(l_tei_word)

    return '\n'.join(l_word_list)

g_wfw = [
'arjunaḥ uvāca — Arjuna said; evam — thus; satata — always; yuktāḥ — engaged; ye — those who; bhaktāḥ — devotees; tvām — You; paryupāsate — properly worship; ye — those who; ca — also; api — again; akṣaram — beyond the senses; avyaktam — the unmanifested; teṣām — of them; ke — who; yoga-vit-tamāḥ — the most perfect in knowledge of yoga.',
'śrī-bhagavān uvāca — the Supreme Personality of Godhead said; mayi — upon Me; āveśya — fixing; manaḥ — the mind; ye — those who; mām — Me; nitya — always; yuktāḥ — engaged; upāsate — worship; śraddhayā — with faith; parayā — transcendental; upetāḥ — endowed; te — they; me — by Me; yukta-tamāḥ — most perfect in yoga; matāḥ — are considered.',
'ye — those who; tu — but; akṣaram — that which is beyond the perception of the senses; anirdeśyam — indefinite; avyaktam — unmanifested; paryupāsate — completely engage in worshiping; sarvatra-gam — all-pervading; acintyam — inconceivable; ca — also; kūṭa-stham — unchanging; acalam — immovable; dhruvam — fixed;',
'sanniyamya — controlling; indriya-grāmam — all the senses; sarvatra — everywhere; sama-buddhayaḥ — equally disposed; te — they; prāpnuvanti — achieve; mām — Me; eva — certainly; sarva-bhūta-hite — for the welfare of all living entities; ratāḥ — engaged.',
'kleśaḥ — trouble; adhika-taraḥ — very much; teṣām — of them; avyakta — to the unmanifested; āsakta — attached; cetasām — of those whose minds; avyaktā — toward the unmanifested; hi — certainly; gatiḥ — progress; duḥkham — with trouble; deha-vadbhiḥ — by the embodied; avāpyate — is achieved.',
'ye — those who; tu — but; sarvāṇi — all; karmāṇi — activities; mayi — unto Me; sannyasya — giving up; mat-parāḥ — being attached to Me; ananyena — without division; eva — certainly; yogena — by practice of such bhakti-yoga; mām — upon Me; dhyāyantaḥ — meditating; upāsate — worship;',
'teṣām — of them; aham — I; samuddhartā — the deliverer; mṛtyu — of death; saṁsāra — in material existence; sāgarāt — from the ocean; bhavāmi — I become; na — not; cirāt — after a long time; pārtha — O son of Pṛthā; mayi — upon Me; āveśita — fixed; cetasām — of those whose minds.',
'mayi — upon Me; eva — certainly; manaḥ — mind; ādhatsva — fix; mayi — upon Me; buddhim — intelligence; niveśaya — apply; nivasiṣyasi — you will live; mayi — in Me; eva — certainly; ataḥ ūrdhvam — thereafter; na — never; saṁśayaḥ — doubt.'
]
g_transl = [
'Arjuna inquired: Which are considered to be more perfect, those who are always properly engaged in Your devotional service or those who worship the impersonal Brahman, the unmanifested?',
'The Supreme Personality of Godhead said: Those who fix their minds on My personal form and are always engaged in worshiping Me with great and transcendental faith are considered by Me to be most perfect.',
'But those who fully worship the unmanifested, that which lies beyond the perception of the senses, the all-pervading, inconceivable, unchanging, fixed and immovable – the impersonal conception of the Absolute Truth –',
'by controlling the various senses and being equally disposed to everyone, such persons, engaged in the welfare of all, at last achieve Me.',
'For those whose minds are attached to the unmanifested, impersonal feature of the Supreme, advancement is very troublesome. To make progress in that discipline is always difficult for those who are embodied.',
'But those who worship Me, giving up all their activities unto Me and being devoted to Me without deviation, engaged in devotional service and always meditating upon Me,',
'having fixed their minds upon Me, O son of Pṛthā – for them I am the swift deliverer from the ocean of birth and death.',
'Just fix your mind upon Me, the Supreme Personality of Godhead, and engage all your intelligence in Me. Thus you will live in Me always, without a doubt.'
]
g_comment = [
"""
Kṛṣṇa has now explained about the personal, the impersonal and the universal and has described all kinds of devotees and yogīs. Generally, the transcendentalists can be divided into two classes. One is the impersonalist, and the other is the personalist. The personalist devotee engages himself with all energy in the service of the Supreme Lord. The impersonalist also engages himself, not directly in the service of Kṛṣṇa but in meditation on the impersonal Brahman, the unmanifested.

We find in this chapter that of the different processes for realization of the Absolute Truth, bhakti-yoga, devotional service, is the highest. If one at all desires to have the association of the Supreme Personality of Godhead, then he must take to devotional service.

Those who worship the Supreme Lord directly by devotional service are called personalists. Those who engage themselves in meditation on the impersonal Brahman are called impersonalists. Arjuna is here questioning which position is better. There are different ways to realize the Absolute Truth, but Kṛṣṇa indicates in this chapter that bhakti-yoga, or devotional service to Him, is the highest of all. It is the most direct, and it is the easiest means for association with the Godhead.

In the Second Chapter of Bhagavad-gītā, the Supreme Lord explained that a living entity is not the material body; he is a spiritual spark. And the Absolute Truth is the spiritual whole. In the Seventh Chapter He spoke of the living entity as being part and parcel of the supreme whole and recommended that he transfer his attention fully to the whole. Then again in the Eighth Chapter it was said that anyone who thinks of Kṛṣṇa at the time of quitting his body is at once transferred to the spiritual sky, to the abode of Kṛṣṇa. And at the end of the Sixth Chapter the Lord clearly said that of all yogīs, one who always thinks of Kṛṣṇa within himself is considered the most perfect. So in practically every chapter the conclusion has been that one should be attached to the personal form of Kṛṣṇa, for that is the highest spiritual realization.

Nevertheless, there are those who are not attached to the personal form of Kṛṣṇa. They are so firmly detached that even in the preparation of commentaries to Bhagavad-gītā they want to distract other people from Kṛṣṇa and transfer all devotion to the impersonal brahma-jyotir. They prefer to meditate on the impersonal form of the Absolute Truth, which is beyond the reach of the senses and is not manifest.

And so, factually, there are two classes of transcendentalists. Now Arjuna is trying to settle the question of which process is easier and which of the classes is most perfect. In other words, he is clarifying his own position because he is attached to the personal form of Kṛṣṇa. He is not attached to the impersonal Brahman. He wants to know whether his position is secure. The impersonal manifestation, either in this material world or in the spiritual world of the Supreme Lord, is a problem for meditation. Actually, one cannot perfectly conceive of the impersonal feature of the Absolute Truth. Therefore Arjuna wants to say, “What is the use of such a waste of time?” Arjuna experienced in the Eleventh Chapter that to be attached to the personal form of Kṛṣṇa is best because he could thus understand all other forms at the same time and there was no disturbance to his love for Kṛṣṇa. This important question asked of Kṛṣṇa by Arjuna will clarify the distinction between the impersonal and personal conceptions of the Absolute Truth.
""",
"""
In answer to Arjuna’s question, Kṛṣṇa clearly says that he who concentrates upon His personal form and who worships Him with faith and devotion is to be considered most perfect in yoga. For one in such Kṛṣṇa consciousness there are no material activities, because everything is done for Kṛṣṇa. A pure devotee is constantly engaged. Sometimes he chants, sometimes he hears or reads books about Kṛṣṇa, or sometimes he cooks prasādam or goes to the marketplace to purchase something for Kṛṣṇa, or sometimes he washes the temple or the dishes – whatever he does, he does not let a single moment pass without devoting his activities to Kṛṣṇa. Such action is in full samādhi.
""",
"""
""",
"""
[3+4] Those who do not directly worship the Supreme Godhead, Kṛṣṇa, but who attempt to achieve the same goal by an indirect process, also ultimately achieve the same goal, Śrī Kṛṣṇa. “After many births the man of wisdom seeks refuge in Me, knowing that Vāsudeva is all.” When a person comes to full knowledge after many births, he surrenders unto Lord Kṛṣṇa. If one approaches the Godhead by the method mentioned in this verse, he has to control the senses, render service to everyone and engage in the welfare of all beings. It is inferred that one has to approach Lord Kṛṣṇa, otherwise there is no perfect realization. Often there is much penance involved before one fully surrenders unto Him.

In order to perceive the Supersoul within the individual soul, one has to cease the sensual activities of seeing, hearing, tasting, working, etc. Then one comes to understand that the Supreme Soul is present everywhere. Realizing this, one envies no living entity – he sees no difference between man and animal because he sees soul only, not the outer covering. But for the common man, this method of impersonal realization is very difficult.
""",
"""
The group of transcendentalists who follow the path of the inconceivable, unmanifested, impersonal feature of the Supreme Lord are called jñāna-yogīs, and persons who are in full Kṛṣṇa consciousness, engaged in devotional service to the Lord, are called bhakti-yogīs. Now, here the difference between jñāna-yoga and bhakti-yoga is definitely expressed. The process of jñāna-yoga, although ultimately bringing one to the same goal, is very troublesome, whereas the path of bhakti-yoga, the process of being in direct service to the Supreme Personality of Godhead, is easier and is natural for the embodied soul. The individual soul is embodied since time immemorial. It is very difficult for him to simply theoretically understand that he is not the body. Therefore, the bhakti-yogī accepts the Deity of Kṛṣṇa as worshipable because there is some bodily conception fixed in the mind, which can thus be applied. Of course, worship of the Supreme Personality of Godhead in His form within the temple is not idol worship. There is evidence in the Vedic literature that worship may be saguṇa or nirguṇa – of the Supreme possessing or not possessing attributes. Worship of the Deity in the temple is saguṇa worship, for the Lord is represented by material qualities. But the form of the Lord, though represented by material qualities such as stone, wood or oil paint, is not actually material. That is the absolute nature of the Supreme Lord.

A crude example may be given here. We may find some mailboxes on the street, and if we post our letters in those boxes, they will naturally go to their destination without difficulty. But any old box, or an imitation which we may find somewhere but which is not authorized by the post office, will not do the work. Similarly, God has an authorized representation in the Deity form, which is called arcā-vigraha. This arcā-vigraha is an incarnation of the Supreme Lord. God will accept service through that form. The Lord is omnipotent, all-powerful; therefore, by His incarnation as arcā-vigraha He can accept the services of the devotee, just to make it convenient for the man in conditioned life.

So for a devotee there is no difficulty in approaching the Supreme immediately and directly, but for those who are following the impersonal way to spiritual realization the path is difficult. They have to understand the unmanifested representation of the Supreme through such Vedic literatures as the Upaniṣads, and they have to learn the language, understand the nonperceptual feelings, and realize all these processes. This is not very easy for a common man. A person in Kṛṣṇa consciousness, engaged in devotional service, simply by the guidance of the bona fide spiritual master, simply by offering regulative obeisances unto the Deity, simply by hearing the glories of the Lord, and simply by eating the remnants of foodstuffs offered to the Lord, realizes the Supreme Personality of Godhead very easily. There is no doubt that the impersonalists are unnecessarily taking a troublesome path with the risk of not realizing the Absolute Truth at the ultimate end. But the personalist, without any risk, trouble or difficulty, approaches the Supreme Personality directly. A similar passage appears in Śrīmad-Bhāgavatam. It is stated there that if one ultimately has to surrender unto the Supreme Personality of Godhead (this surrendering process is called bhakti), but instead takes the trouble to understand what is Brahman and what is not Brahman and spends his whole life in that way, the result is simply troublesome. Therefore it is advised here that one should not take up this troublesome path of self-realization, because there is uncertainty in the ultimate result.

A living entity is eternally an individual soul, and if he wants to merge into the spiritual whole, he may accomplish the realization of the eternal and knowledgeable aspects of his original nature, but the blissful portion is not realized. By the grace of some devotee, such a transcendentalist, highly learned in the process of jñāna-yoga, may come to the point of bhakti-yoga, or devotional service. At that time, long practice in impersonalism also becomes a source of trouble, because he cannot give up the idea. Therefore an embodied soul is always in difficulty with the unmanifest, both at the time of practice and at the time of realization. Every living soul is partially independent, and one should know for certain that this unmanifested realization is against the nature of his spiritual, blissful self. One should not take up this process. For every individual living entity the process of Kṛṣṇa consciousness, which entails full engagement in devotional service, is the best way. If one wants to ignore this devotional service, there is the danger of turning to atheism. Thus the process of centering attention on the unmanifested, the inconceivable, which is beyond the approach of the senses, as already expressed in this verse, should never be encouraged at any time, especially in this age. It is not advised by Lord Kṛṣṇa.
""",
"""
""",
"""
It is explicitly stated here that the devotees are very fortunate to be delivered very soon from material existence by the Lord. In pure devotional service one comes to the realization that God is great and that the individual soul is subordinate to Him. His duty is to render service to the Lord – and if he does not, then he will render service to māyā.

As stated before, the Supreme Lord can be appreciated only by devotional service. Therefore, one should be fully devoted. One should fix his mind fully on Kṛṣṇa in order to achieve Him. One should work only for Kṛṣṇa. It does not matter in what kind of work one engages, but that work should be done only for Kṛṣṇa. That is the standard of devotional service. The devotee does not desire any achievement other than pleasing the Supreme Personality of Godhead. His life’s mission is to please Kṛṣṇa, and he can sacrifice everything for Kṛṣṇa’s satisfaction, just as Arjuna did in the Battle of Kurukṣetra. The process is very simple: one can devote himself in his occupation and engage at the same time in chanting Hare Kṛṣṇa, Hare Kṛṣṇa, Kṛṣṇa Kṛṣṇa, Hare Hare/ Hare Rāma, Hare Rāma, Rāma Rāma, Hare Hare. Such transcendental chanting attracts the devotee to the Personality of Godhead.

The Supreme Lord herein promises that without delay He will deliver a pure devotee thus engaged from the ocean of material existence. Those who are advanced in yoga practice can willfully transfer the soul to whatever planet they like by the yoga process, and others take the opportunity in various ways, but as far as the devotee is concerned, it is clearly stated here that the Lord Himself takes him. The devotee does not need to wait to become very experienced in order to transfer himself to the spiritual sky.

In the Varāha Purāṇa this verse appears:

nayāmi paramaṁ sthānam
arcir-ādi-gatiṁ vinā
garuḍa-skandham āropya
yatheccham anivāritaḥ

The purport of this verse is that a devotee does not need to practice aṣṭāṅga-yoga in order to transfer his soul to the spiritual planets. The responsibility is taken by the Supreme Lord Himself. He clearly states here that He Himself becomes the deliverer. A child is completely cared for by his parents, and thus his position is secure. Similarly, a devotee does not need to endeavor to transfer himself by yoga practice to other planets. Rather, the Supreme Lord, by His great mercy, comes at once, riding on His bird carrier Garuḍa, and at once delivers the devotee from material existence. Although a man who has fallen in the ocean may struggle very hard and may be very expert in swimming, he cannot save himself. But if someone comes and picks him up from the water, then he is easily rescued. Similarly, the Lord picks up the devotee from this material existence. One simply has to practice the easy process of Kṛṣṇa consciousness and fully engage himself in devotional service. Any intelligent man should always prefer the process of devotional service to all other paths. In the Nārāyaṇīya this is confirmed as follows:

yā vai sādhana-sampattiḥ
puruṣārtha-catuṣṭaye
tayā vinā tad āpnoti
naro nārāyaṇāśrayaḥ

The purport of this verse is that one should not engage in the different processes of fruitive activity or cultivate knowledge by the mental speculative process. One who is devoted to the Supreme Personality can attain all the benefits derived from other yogic processes, speculation, rituals, sacrifices, charities, etc. That is the specific benediction of devotional service.

Simply by chanting the holy name of Kṛṣṇa – Hare Kṛṣṇa, Hare Kṛṣṇa, Kṛṣṇa Kṛṣṇa, Hare Hare/ Hare Rāma, Hare Rāma, Rāma Rāma, Hare Hare – a devotee of the Lord can approach the supreme destination easily and happily, but this destination cannot be approached by any other process of religion.

The conclusion of Bhagavad-gītā is stated in the Eighteenth Chapter:

sarva-dharmān parityajya
mām ekaṁ śaraṇaṁ vraja
ahaṁ tvāṁ sarva-pāpebhyo
mokṣayiṣyāmi mā śucaḥ

One should give up all other processes of self-realization and simply execute devotional service in Kṛṣṇa consciousness. That will enable one to reach the highest perfection of life. There is no need for one to consider the sinful actions of his past life, because the Supreme Lord fully takes charge of him. Therefore one should not futilely try to deliver himself in spiritual realization. Let everyone take shelter of the supreme omnipotent Godhead, Kṛṣṇa. That is the highest perfection of life.
""",
"""
One who is engaged in Lord Kṛṣṇa’s devotional service lives in a direct relationship with the Supreme Lord, so there is no doubt that his position is transcendental from the very beginning. A devotee does not live on the material plane – he lives in Kṛṣṇa. The holy name of the Lord and the Lord are nondifferent; therefore when a devotee chants Hare Kṛṣṇa, Kṛṣṇa and His internal potency are dancing on the tongue of the devotee. When he offers Kṛṣṇa food, Kṛṣṇa directly accepts these eatables, and the devotee becomes Kṛṣṇa-ized by eating the remnants. One who does not engage in such service cannot understand how this is so, although this is a process recommended in the Bhagavad-gītā and in other Vedic literatures.
"""
]
g_sloka_ref_list = [
    (12, 1, 'अर्जुन उवाच एवं सततयुक्ता ये भक्तास्त्वां पर्युपासते । ये चाप्यक्षरमव्यक्तं तेषां के योगवित्तमा:', g_wfw[0], g_transl[0], g_comment[0].strip()),
    (12, 2, 'श्रीभगवानुवाच मय्यावेश्य मनो ये मां नित्ययुक्ता उपासते । श्रद्धया परयोपेतास्ते मे युक्ततमा मता:', g_wfw[1], g_transl[1], g_comment[1].strip()),
    (12, 3, 'ये त्वक्षरमनिर्देश्यमव्यक्तं पर्युपासते । सर्वत्रगमचिन्त्यं च कूटस्थमचलं ध्रुवम्', g_wfw[2], g_transl[2], g_comment[2].strip()),
    (12, 4, 'सन्नियम्येन्द्रियग्रामं सर्वत्र समबुद्धय: । ते प्राप्नुवन्ति मामेव सर्वभूतहिते रता:', g_wfw[3], g_transl[3], g_comment[3].strip()),
    (12, 5, 'क्ल‍ेशोऽधिकतरस्तेषामव्यक्तासक्तचेतसाम् । अव्यक्ता हि गतिर्दु:खं देहवद्भ‍िरवाप्यते', g_wfw[4], g_transl[4], g_comment[4].strip()),
    (12, 6, 'ये तु सर्वाणि कर्माणि मयि सन्न्यस्य मत्परा: । अनन्येनैव योगेन मां ध्यायन्त उपासते', g_wfw[5], g_transl[5], g_comment[5].strip()),
    (12, 7, 'तेषामहं समुद्धर्ता मृत्युसंसारसागरात् । भवामि न चिरात्पार्थ मय्यावेशितचेतसाम्', g_wfw[6], g_transl[6], g_comment[6].strip()),
    (12, 8, 'मय्येव मन आधत्स्व मयि बुद्धिं निवेशय । निवसिष्यसि मय्येव अत ऊर्ध्वं न संशय:', g_wfw[7], g_transl[7], g_comment[7].strip())
]
# ---------------------------------------------------- Main section ----------------------------------------------------
if __name__ == "__main__":
    l_sloka_list = []
    l_sloka_num = 1
    for l_chapter_num, l_sloka_num, l_sloka_txt, l_vocab, l_transl, l_comment in g_sloka_ref_list:
        l_sloka_words = inria_sloka_words(l_sloka_txt)
        l_sloka_halves = '\n'.join([f'<l>{l.strip()}</l>' for l in l_sloka_txt.split('।')])

        l_url_iit = 'https://www.gitasupersite.iitk.ac.in/srimad?language=dv&' + \
            f'field_chapter_value={l_chapter_num}&field_nsutra_value={l_sloka_num}&' + \
            'ecsiva=1&etsiva=1&etpurohit=1&etgb=1&setgb=1&etssa=1&etradi=1'
        print(l_url_iit, file=sys.stderr)
        l_html_iit = urllib.request.urlopen(l_url_iit).read()
        l_html_iit = l_html_iit.decode('utf8').strip()
        # print(l_html_iit)

        l_match = re.search(r'<div class="views-field views-field-field-etgb".*?<font size = \'3px\'>([^<]+)</font>', l_html_iit)
        l_gambirananda = ''
        if l_match:
            l_gambirananda = l_match.group(1).strip()
            l_gambirananda = re.sub(r'^\d+\.\d+', '', l_gambirananda).strip()
            print(l_gambirananda, file=sys.stderr)

        #                                                                               <font size = '3px'><br><br>
        l_match = re.search(r'<div class="views-field views-field-field-ecsiva".*?<font size = \'3px\'>(.*?)</font>', l_html_iit)
        l_sivananda = ''
        if l_match:
            l_sivananda = l_match.group(1).strip()
            l_sivananda = re.sub(r'^<br><br>', '', l_sivananda).strip()
            #l_sivananda = re.sub(r'^\d+\.\d+', '', l_sivananda).strip()
            print(l_sivananda, file=sys.stderr)

        l_match = re.search(r'^\d+\.\d+\s*([^<]+)<br/><br/><b>Commentary:</b><br/>(.*)$', l_sivananda)
        l_sivananda_vocab = ''
        if l_match:
            l_sivananda_vocab = re.sub(r'[\u0900-\u097f\ua8e0-\ua8ff]+\s+[\u0900-\u097f\ua8e0-\ua8ff]*',
                                       lambda x: devtrans.dev2iast(x.group()).strip() + ': ',
                                       l_match.group(1).strip())
            l_sivananda = l_match.group(2).strip()

        # Unicode range	U+0900–U+097F Devanagari,
        # U+A8E0–U+A8FF Devanagari Extended,
        # U+11B00–11B5F Devanagari Extended-A,
        # U+1CD0–U+1CFF Vedic Extensions
        # print(list(re.findall(r'[\u0900-\u097f\ua8e0-\ua8ff]+', l_sivananda_vocab)), file=sys.stderr)

        l_sloka_list.append(
            f'<ref cRef="BG 12.{l_sloka_num}">Bhagavad-gītā Chapter {l_chapter_num}, Sloka {l_sloka_num}</ref>\n' +
            f'<lg type="sloka">\n{l_sloka_halves}\n</lg>\n' + l_sloka_words +
            f'\n<annotation>{l_vocab}</annotation>' +
            f'\n<annotation>{l_sivananda_vocab}</annotation>' +
            f'\n<interp type="translation">{l_transl}</interp>' +
            f'\n<interp type="prabhupada">{l_comment}</interp>' +
            f'\n<interp type="sivananda">{l_sivananda}</interp>' +
            f'\n<interp type="gambirananda">{l_gambirananda}</interp>'
        )

    print('\n'.join([f'<s>\n{s}\n</s>' for s in l_sloka_list]))
