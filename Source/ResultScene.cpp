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
	// ダークレッドと黒の斜めストライプ（アクション風）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, 1920, 1080, GetColor(10, 0, 0), TRUE);
	
	for (int i = -1000; i < 2000; i += 300) {
		int offset = (mnAllTimer * 12) % 300; // 速め
		DrawQuadrangle(i + offset, 0, i + 150 + offset, 0, i - 1080 + 150 + offset, 1080, i - 1080 + offset, 1080, GetColor(100, 0, 0), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int oldSize = GetFontSize();
	
	Scene::Draw();
	
	// Fallen... スライドイン（上から）
	float t1 = mnAllTimer / 20.0f;
	if (t1 > 1.0f) t1 = 1.0f;
	float ease1 = 1.0f - (1.0f - t1) * (1.0f - t1) * (1.0f - t1);
	int textY = (int)(-200 + (600 * ease1));
	
	if (mnAllTimer > 0)
	{
		SetFontSize(120);
		DrawFormatString(320 + 8, textY + 8, GetColor(30, 0, 0), "FALLEN...");
		DrawFormatString(320, textY, GetColor(255, 50, 50), "FALLEN...");
	}
	
	if (mnAllTimer >= 100)
	{
		int alpha = (mnAllTimer % 60 < 30) ? 255 : 100;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		SetFontSize(40);
		DrawFormatString(300, 700, GetColor(200, 200, 255), "Tap / Click to Retry");
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
