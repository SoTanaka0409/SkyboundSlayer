import codecs, re
for file in ['Source/Player3D.cpp', 'Source/Magic_Ene.cpp', 'Source/TitleScene.cpp']:
    with codecs.open(file, 'r', 'cp932') as f:
        text = f.read()
    for m in set(re.findall(r'Resource/[^"]+', text)):
        if not all(ord(c) < 128 for c in m):
            print(f'{file}:', m)
