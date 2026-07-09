#include "ResultScene.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"

ResultScene::ResultScene() : mnAllTimer(0), mnColorFade(0), mbColorFlag(false)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

void ResultScene::Update()
{
	mnAllTimer++;
	Scene::Update();
}

void ResultScene::Draw()
{
	// 画面全体を暗くする半透明の矩形を描画（シリアスな青黒いフィルター）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, 1920, 1080, GetColor(10, 10, 30), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int oldSize = GetFontSize();
	
	SetFontSize(80);
	DrawFormatString(320, 400, GetColor(150, 150, 200), "Fallen...");
	
	Scene::Draw();
	
	if (mnAllTimer >= 150)
	{
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
		
		SetFontSize(40);
		DrawFormatString(300, 600, GetColor(mnColorFade, mnColorFade, 255), "再起するには画面をタップ（クリック）");
		
		if (InputManager::CheckMouseClickLeft() || InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}
	
	SetFontSize(oldSize);
}

void ResultScene::Finalize()
{
}
