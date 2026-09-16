
import codecs
with codecs.open('Source/EnemyBoss_1.cpp', 'r', 'utf-8-sig') as f:
    text = f.read()

text = text.replace('pPlayer->Damage(attack_);', 'pPlayer->Damage(10.0f);')

with codecs.open('Source/EnemyBoss_1.cpp', 'w', 'utf-8-sig') as f:
    f.write(text)

