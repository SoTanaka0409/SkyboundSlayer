
import codecs
with codecs.open('Player3D.cpp', 'r', 'cp932') as f:
    text = f.read()

import re
text = re.sub(r'16_[^\.]*\.mv1', '16_.mv1', text)
text = text.replace('attack_count_ % 5 == 0', 'attack_count_ == 0')

with codecs.open('Player3D.cpp', 'w', 'utf-8-sig') as f:
    f.write(text)

