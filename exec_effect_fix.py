# -*- coding: utf-8 -*-
import os
import re

cpp_path = r'Source\Effect.cpp'
with open(cpp_path, 'r', encoding='shift_jis', errors='ignore') as f:
    cpp_content = f.read()

# Replace Effect::Effect(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime)
# :Object3D(initPos) ...
match_ctor = re.search(r'Effect::Effect\([^)]+\)[\s\S]*?:Object3D\(initPos\)[\s\S]*?\{[\s\S]*?mnGraphHandle = LoadGraph\(filename\.c_str\(\)\);[\s\S]*?\}', cpp_content)
if match_ctor:
    new_ctor = '''Effect::Effect()
{
	mpEffect = new EffectInfo();
	mnGraphHandle = -1;
	mActive = false;
}

void Effect::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime)
{
	mActive = true;
	mpEffect->color = Changecolor;
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		mpEffect->particle[i].pos = initPos;
		mpEffect->particle[i].dir.x = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.y = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.z = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].speed = ((float)GetRand(SPEED_RAND_MAX) + SPEED_RAND_MIN) / 100.0f;
		mpEffect->particle[i].alpha = 1.0f;
		mpEffect->particle[i].size = Size;
		mpEffect->particle[i].visibleTime = VisibleTime;
	}
	if (mnGraphHandle == -1) {
		mnGraphHandle = LoadGraph(filename.c_str());
	}
}'''
    cpp_content = cpp_content.replace(match_ctor.group(0), new_ctor)

# Replace SetDeleteFlag(true); with mActive = false;
cpp_content = cpp_content.replace('SetDeleteFlag(true);', 'mActive = false;')

with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(cpp_content)

print("Effect.cpp fixed.")
