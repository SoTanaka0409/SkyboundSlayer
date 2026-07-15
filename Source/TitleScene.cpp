#include "TitleScene.h"
#include "InputManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ColliderManager.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include "Stage.h"
#include "StageObject.h"
#include "SkyBox.h"
#include "Config.h"

TitleScene::TitleScene()
: mnColorFade(1), mbColorFlag(false), mCameraAngle(0.0f)
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
	
	Master::mpScoreManager->LoadHighScore();
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
	
	// 本編と同じステージとオブジェクトを読み込む
	new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", VGet(200.0f, 100.0f, 200.0f));
	new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1",VGet(3.0f,0.3f,3.0f));
	
	std::ifstream file("Resource/CSV/stage_objects.csv");
	if (file.is_open())
	{
		std::string line;
		std::getline(file, line); // ヘッダーをスキップ
		while (std::getline(file, line))
		{
			if (line.empty()) continue;
			std::stringstream ss(line);
			std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr, isRelativeStr;
			std::getline(ss, type, ',');
			std::getline(ss, model, ',');
			std::getline(ss, xStr, ',');
			std::getline(ss, yStr, ',');
			std::getline(ss, zStr, ',');
			std::getline(ss, sxStr, ',');
			std::getline(ss, syStr, ',');
			std::getline(ss, szStr, ',');
			std::getline(ss, texture, ',');
			std::getline(ss, colSizeStr, ',');
			std::getline(ss, isRelativeStr, ',');
			
			float x = std::stof(xStr);
			float y = std::stof(yStr);
			float z = std::stof(zStr);
			float sx = std::stof(sxStr);
			float sy = std::stof(syStr);
			float sz = std::stof(szStr);
			int isRelative = 0;
			if (!isRelativeStr.empty()) isRelative = std::stoi(isRelativeStr);
			VECTOR pos = VGet(x, y, z);
			if (isRelative == 1) { pos = VAdd(Config::GetStageCenter(), pos); }
			VECTOR scale = VGet(sx, sy, sz);
			
			if (type == "StageObject") {
				float colSize = 0.0f;
				if (!colSizeStr.empty()) colSize = std::stof(colSizeStr);
				new StageObject(pos, model, scale, "", colSize);
			} else if (type == "Stage") {
				new Stage(pos, model, model, scale, texture);
			}
		}
		file.close();
	}
	
	SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x",VGet(0,0,-5000));
	float scale = 13.0f;
	pSkyBox->SetScale(VGet(scale, scale, scale));
	pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");
	
	// タイトル用にテレポーターも一つ置いておく
	new StageObject(VGet(0.0f, 0.0f, 500.0f), "Resource/3D/portal/source/portal.mv1", VGet(3.0f, 3.0f, 3.0f));
	
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
	// 3D背景の描画は Scene::Draw() が ObjectManager 経由で自動的に行います
	
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
	DrawFormatString(300, 100, GetColor(255, 215, 0), "Sky Castle Hunter");
	
	
	
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
	DrawFormatString(700, 960, GetColor(200, 200, 200), "マウスで項目をタップ（クリック）して決定 ");
	
	SetFontSize(oldSize);
}

void TitleScene::Finalize()
{
}

