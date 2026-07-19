# -*- coding: utf-8 -*-
import os

h_path = r'Source\Effect.h'
with open(h_path, 'r', encoding='shift_jis', errors='ignore') as f:
    h_content = f.read()

# Remove :public Object3D
h_content = h_content.replace('class Effect :public Object3D', 'class Effect')

# Remove override from Update and Draw
h_content = h_content.replace('void Update()override;', 'void Update();')
h_content = h_content.replace('void Draw()override;', 'void Draw();')

# Add Play method and active flag
h_content = h_content.replace('Effect(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime);', 
'''Effect();
	void Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime);
	bool IsActive() const { return mActive; }''')

h_content = h_content.replace('private:', 'private:\n\tbool mActive;\n')

with open(h_path, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(h_content)
print("Effect.h updated.")
