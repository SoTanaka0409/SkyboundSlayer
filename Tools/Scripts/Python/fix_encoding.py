# -*- coding: utf-8 -*-
import os

def replace_in_file(filepath, old_str, new_str):
    try:
        with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
            content = f.read()
        if old_str in content:
            content = content.replace(old_str, new_str)
            with open(filepath, 'w', encoding='shift_jis', errors='replace') as f:
                f.write(content)
            print("Updated " + filepath)
        else:
            print("String not found in " + filepath)
    except Exception as e:
        print("Failed to update " + filepath + ": " + str(e))

# StageObject.h
stage_h = r'Source\StageObject.h'
old_h = '''	Model* model_;
	float mfScale;
	float mfHitRadius;
};'''
new_h = '''	Model* model_;
	float mfScale;
	float mfHitRadius;
	bool mbHasFollowedTerrain;
};'''
replace_in_file(stage_h, old_h, new_h)

# StageObject.cpp
stage_cpp = r'Source\StageObject.cpp'
old_cpp1 = '''StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius)
	:Object3D(initPos), mfHitRadius(hitRadius)'''
new_cpp1 = '''StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius)
	:Object3D(initPos), mfHitRadius(hitRadius), mbHasFollowedTerrain(false)'''

old_cpp2 = '''void StageObject::Update()
{
	TerrainFollow();'''
new_cpp2 = '''void StageObject::Update()
{
	if (!mbHasFollowedTerrain)
	{
		TerrainFollow();
		mbHasFollowedTerrain = true;
	}'''
replace_in_file(stage_cpp, old_cpp1, new_cpp1)
replace_in_file(stage_cpp, old_cpp2, new_cpp2)

# StatShop.cpp
shop_cpp = r'Source\StatShop.cpp'
old_shop1 = '''		if (InputManager::CheckDownKey(KEY_INPUT_ESCAPE) || InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::StatShopClassOn = false;
			Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
		}'''
new_shop1 = '''		static int oldEsc = 0;
		static int oldBack = 0;
		int currentEsc = CheckHitKey(KEY_INPUT_ESCAPE);
		int currentBack = CheckHitKey(KEY_INPUT_BACK);
		
		if ((currentEsc && !oldEsc) || (currentBack && !oldBack))
		{
			Master::StatShopClassOn = false;
			Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
		}
		oldEsc = currentEsc;
		oldBack = currentBack;'''

old_shop2 = '''	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{

		doBuy = true;
	}'''
new_shop2 = '''	static int oldReturn = 0;
	int currentReturn = CheckHitKey(KEY_INPUT_RETURN);
	if (currentReturn && !oldReturn)
	{
		doBuy = true;
	}
	oldReturn = currentReturn;'''

old_shop3 = '''			if (InputManager::CheckDownKey(KEY_INPUT_RETURN) && !Master::StatShopClassOn && !Master::StatShopClassOn)
			{
				Master::StatShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
			}'''
new_shop3 = '''			static int oldEnter = 0;
			int currentEnter = CheckHitKey(KEY_INPUT_RETURN);
			if (currentEnter && !oldEnter && !Master::StatShopClassOn)
			{
				Master::StatShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
			}
			oldEnter = currentEnter;'''

replace_in_file(shop_cpp, old_shop1, new_shop1)
replace_in_file(shop_cpp, old_shop2, new_shop2)
replace_in_file(shop_cpp, old_shop3, new_shop3)

