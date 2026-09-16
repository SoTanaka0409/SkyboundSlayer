
import codecs
with codecs.open('Player3D.cpp', 'r', 'cp932') as f:
    lines = f.readlines()

for i, line in enumerate(lines):
    if 'attack_count_ % 5 == 0' in line:
        lines[i] = line.replace('attack_count_ % 5 == 0', 'attack_count_ == 0')
    if i == 507 or i == 508:
        if '1.3f' in line:
            lines[i] = line.replace('1.3f', '0.9f')
        if '0.5f' in line:
            lines[i] = line.replace('0.5f', '0.35f')

with codecs.open('Player3D.cpp', 'w', 'cp932') as f:
    f.writelines(lines)

