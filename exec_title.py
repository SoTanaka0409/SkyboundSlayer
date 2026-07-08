# -*- coding: utf-8 -*-
import os

cpp_path = r'Source\TitleScene.cpp'

new_content = '''#include "TitleScene.h"
#include "Texture.h"
#include "InputManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ColliderManager.h"

TitleScene::TitleScene()
: mnPause(0), C(false), S(false), E(false), T(false)
, C_Enter(false), S_Enter(false), E_Enter(false)
, Color1(1), Colorflag(false)
{
}

TitleScene::~TitleScene()
{
	delete mpTexture;
}

void TitleScene::Initialize()
{
	Master::TutorialFlag = false;
	Master::TutorialCount = 0;
	Master::mpCamera->Initialize();
	Master::GameClearCount = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();
	
	mpTexture = new Texture("Resource/Title", VGet(500,420, 0), true);
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
}

void TitleScene::Update()
{
	Scene::Update();
	
	int mx, my;
	InputManager::GetMousePos(mx, my);
	
	// ボタンの領域 (Game Start: y=750~800, Rule: y=820~870)
	bool hoverStart = (mx >= 50 && mx <= 400 && my >= 750 && my <= 800);
	bool hoverRule  = (mx >= 50 && mx <= 400 && my >= 820 && my <= 870);
	
	if (InputManager::CheckMouseClickLeft())
	{
		if (hoverStart)
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
		}
		else if (hoverRule)
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_RULE);
		}
	}
}

void TitleScene::Draw()
{
	// Fade effect
	if (Colorflag)
	{
		Color1 -= 4;
		if (Color1 <= 0) { Color1 = 0; Colorflag = false; }
	}
	else
	{
		Color1 += 4;
		if (Color1 >= 255) { Color1 = 255; Colorflag = true; }
	}
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	Scene::Draw();
	DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), true); // changed to 1920x1080 for proper full clear
	
	int oldSize = GetFontSize();
	
	SetFontSize(60);
	DrawFormatString(300, 10, GetColor(255, 255, 255), "Dino&Hunter");
	mpTexture->Draw();
	
	Master::mpScoreManager->LoadHighScore();
	
	int mx, my;
	InputManager::GetMousePos(mx, my);
	
	bool hoverStart = (mx >= 50 && mx <= 400 && my >= 750 && my <= 800);
	bool hoverRule  = (mx >= 50 && mx <= 400 && my >= 820 && my <= 870);
	
	SetFontSize(35);
	
	// Game Start Button
	if (hoverStart)
	{
		DrawFormatString(50, 750, GetColor(0, 255, 255), "Game Start!");
	}
	else
	{
		DrawFormatString(50, 750, GetColor(255, 255, 255), "Game Start!");
	}
	
	// Rule Button
	if (hoverRule)
	{
		DrawFormatString(50, 820, GetColor(0, 255, 255), "ルール (Rule)");
	}
	else
	{
		DrawFormatString(50, 820, GetColor(255, 255, 255), "ルール (Rule)");
	}
	
	SetFontSize(20);
	DrawFormatString(750, 960, GetColor(255, 255, 255), "マウスで項目をタップ（クリック）して決定");
	
	SetFontSize(oldSize);
}

void TitleScene::Finalize()
{
}
'''

with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(new_content)

print("TitleScene.cpp completely rewritten.")
