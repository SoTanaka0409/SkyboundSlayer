# -*- coding: utf-8 -*-
import os

h_path = r'Source\TitleScene.h'
with open(h_path, 'r', encoding='shift_jis', errors='ignore') as f:
    h_content = f.read()

if 'mnSkyBoxHandle' not in h_content:
    h_content = h_content.replace('private:', 
'''private:
	int mnSkyBoxHandle;
	int mnStageHandle;
	int mnCastleHandle;
	float mCameraAngle;
''')
    with open(h_path, 'w', encoding='shift_jis', errors='replace') as f:
        f.write(h_content)
    print("TitleScene.h updated.")
