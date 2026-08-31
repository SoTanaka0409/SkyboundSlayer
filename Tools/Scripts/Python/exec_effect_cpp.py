# -*- coding: utf-8 -*-
import os

cpp_path = r'Source\Effect.cpp'
with open(cpp_path, 'r', encoding='shift_jis', errors='ignore') as f:
    cpp_content = f.read()

# Replace constructor
old_ctor = '''Effect::Effect(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime)
	:Object3D(initPos)
{
	mpEffect = new EffectInfo();

	//mpEffect->color = GetColorU8(128, 16, 16, 255);//今は固定で赤っぽい色
	mpEffect->color = Changecolor;
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		mpEffect->particle[i].pos = initPos;//初期座標

		mpEffect->particle[i].dir.x = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.y = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.z = ((float)GetRand(200) - 100.0f) / 100.0f;

		//速度
		mpEffect->particle[i].speed = ((float)GetRand(SPEED_RAND_MAX) + SPEED_RAND_MIN) / 100.0f;

		//負透明度
		mpEffect->particle[i].alpha = 1.0f;

		//大きさ
		mpEffect->particle[i].size = Size;

		//表示時間
		mpEffect->particle[i].visibleTime = VisibleTime;


	}
	//画像の読み込み
	mnGraphHandle = LoadGraph(filename.c_str());

}'''

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

cpp_content = cpp_content.replace(old_ctor, new_ctor)

# In Update(), check isEnd and set mActive = false, and remove SetDeleteFlag
old_update_end = '''	if (isEnd == true)
	{
		SetDeleteFlag(true);
	}
}'''
new_update_end = '''	if (isEnd == true)
	{
		mActive = false;
	}
}'''
cpp_content = cpp_content.replace(old_update_end, new_update_end)

with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(cpp_content)
print("Effect.cpp updated.")
