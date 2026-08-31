# -*- coding: utf-8 -*-
import os
import math

cpp_path = r'Source\TitleScene.cpp'

new_content = '''#include "TitleScene.h"
#include "InputManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ColliderManager.h"
#include <math.h>

TitleScene::TitleScene()
: mnColorFade(1), mbColorFlag(false), mCameraAngle(0.0f)
, mnSkyBoxHandle(-1), mnStageHandle(-1), mnCastleHandle(-1)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Master::TutorialFlag = false;
	Master::TutorialCount = 0;
	Master::mpCamera->Initialize();
	Master::GameClearCount = 0;
	ColliderManager::GetInstance()->DeleteAllCollider();
	
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
	
	// 3Dモデルのロード
	mnSkyBoxHandle = MV1LoadModel("Resource/3D/SkyBox/SkyBox.x");
	mnStageHandle = MV1LoadModel("Resource/3D/stage_sky/source/Flooting_Stage.mv1");
	mnCastleHandle = MV1LoadModel("Resource/3D/Stage_casule/source/Parede castelo.mv1");
	
	// スケールと位置の設定
	MV1SetScale(mnSkyBoxHandle, VGet(13.0f, 13.0f, 13.0f));
	MV1SetPosition(mnSkyBoxHandle, VGet(0, 0, -5000));
	
	MV1SetScale(mnStageHandle, VGet(200.0f, 100.0f, 200.0f));
	MV1SetPosition(mnStageHandle, VGet(0.0f, 5000.0f, -20000.0f));
	
	MV1SetScale(mnCastleHandle, VGet(10.0f, 10.0f, 10.0f));
	MV1SetPosition(mnCastleHandle, VGet(0.0f, 0.0f, 0.0f));
	
	mCameraAngle = 0.0f;
}

void TitleScene::Update()
{
	Scene::Update();
	
	// カメラをゆっくり回す
	mCameraAngle += 0.002f;
	if (mCameraAngle >= DX_PI_F * 2.0f) mCameraAngle -= DX_PI_F * 2.0f;
	
	VECTOR camPos = VGet(cosf(mCameraAngle) * 3000.0f, 2000.0f, sinf(mCameraAngle) * 3000.0f);
	VECTOR camTarget = VGet(0.0f, 1000.0f, 0.0f);
	SetCameraPositionAndTarget_UpVecY(camPos, camTarget);
	
	int mx, my;
	InputManager::GetMousePos(mx, my);
	
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
	// 3D背景の描画
	MV1DrawModel(mnSkyBoxHandle);
	MV1DrawModel(mnStageHandle);
	MV1DrawModel(mnCastleHandle);
	
	// UIのフェード用
	if (mbColorFlag)
	{
		mnColorFade -= 4;
		if (mnColorFade <= 0) { mnColorFade = 0; mbColorFlag = false; }
	}
	else
	{
		mnColorFade += 4;
		if (mnColorFade >= 255) { mnColorFade = 255; mbColorFlag = true; }
	}
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	Scene::Draw();
	
	int oldSize = GetFontSize();
	
	SetFontSize(80);
	// ゴールド系の色でタイトル
	DrawFormatString(300, 100, GetColor(255, 215, 0), "Skybound Slayer");
	
	Master::mpScoreManager->LoadHighScore();
	
	int mx, my;
	InputManager::GetMousePos(mx, my);
	
	bool hoverStart = (mx >= 50 && mx <= 400 && my >= 750 && my <= 800);
	bool hoverRule  = (mx >= 50 && mx <= 400 && my >= 820 && my <= 870);
	
	SetFontSize(40);
	
	// 淡い青 / 白の色合い
	int colorNormal = GetColor(220, 240, 255);
	int colorHover = GetColor(255, 255, 255);
	
	if (hoverStart)
	{
		DrawFormatString(50, 750, colorHover, "> Game Start");
	}
	else
	{
		DrawFormatString(50, 750, colorNormal, "  Game Start");
	}
	
	if (hoverRule)
	{
		DrawFormatString(50, 820, colorHover, "> ルール (Rule)");
	}
	else
	{
		DrawFormatString(50, 820, colorNormal, "  ルール (Rule)");
	}
	
	SetFontSize(25);
	DrawFormatString(700, 960, GetColor(200, 200, 200), "マウスで項目をタップ（クリック）して決定");
	
	SetFontSize(oldSize);
}

void TitleScene::Finalize()
{
	MV1DeleteModel(mnSkyBoxHandle);
	MV1DeleteModel(mnStageHandle);
	MV1DeleteModel(mnCastleHandle);
}
'''

with open(cpp_path, 'w', encoding='shift_jis', errors='replace') as f:
    f.write(new_content)

print("TitleScene.cpp updated.")
