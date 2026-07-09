import re

with open('Source/Main.cpp', 'r', encoding='shift_jis') as f:
    content = f.read()

# Remove the exact if block
target = r'''\t\tif \(GetASyncLoadNum\(\) > 0\)
\t\t\{
\t\t\tDrawFormatString\(600, 360, GetColor\(255, 255, 255\), "NOW LOADING... %d", GetASyncLoadNum\(\)\);
\t\t\}
\t\telse
\t\t\{
\t\t\t\tMaster::mpDrawHp->Update\(\);'''

replacement = r'''\t\t{
\t\t\t\tMaster::mpDrawHp->Update();'''

content = re.sub(target, replacement, content)

# Add it back before ScreenFlip
target2 = r'''\t\tScreenFlip\(\);'''

replacement2 = r'''\t\tif (GetASyncLoadNum() > 0)
\t\t{
\t\t\tDrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
\t\t}
\t\tScreenFlip();'''

content = re.sub(target2, replacement2, content)

with open('Source/Main.cpp', 'w', encoding='shift_jis') as f:
    f.write(content)
