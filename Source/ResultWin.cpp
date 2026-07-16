#include "ResultWin.h"
#include "Master.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Texture.h"
#include "Model.h"

ResultWin::ResultWin() : mnAllTimer(0), mNameStr(0), mpPlayerModel(nullptr)
{
}

ResultWin::~ResultWin()
{
}

void ResultWin::Initialize()
{
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);
	
	// Load Player Model
	mpPlayerModel = new Model("Resource/Model/Player.mv1", VGet(30.0f, -30.0f, 0.0f));
	mpPlayerModel->SetRotation(VGet(0.0f, 3.1415f * 1.1f, 0.0f)); 
	mpPlayerModel->SetScale(VGet(0.5f, 0.5f, 0.5f)); // Scale it so it fits nicely 

	// Set Camera for the Result Scene
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -150.0f), VGet(0.0f, 0.0f, 0.0f));
	SetCameraNearFar(1.0f, 1000.0f);
}

void ResultWin::Update()
{
	mnAllTimer++;
	if (mpPlayerModel) {
		mpPlayerModel->Update();
	}
}

void ResultWin::Draw()
{
	// 3Dモデルを描画
	if (mpPlayerModel) {
		mpPlayerModel->Draw();
	}

	// アクションRPG風のUIパネル
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, 700, 1080, GetColor(0, 0, 0), TRUE); // 左側に黒い帯
	DrawBox(700, 0, 800, 1080, GetColor(255, 200, 0), TRUE); // アクセントの黄色いライン
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int size = GetFontSize();
	Master::mpScoreManager->SaveHighScore();
	
	// タイトル
	float t1 = mnAllTimer / 30.0f;
	if (t1 > 1.0f) t1 = 1.0f;
	float ease1 = 1.0f - (1.0f - t1) * (1.0f - t1) * (1.0f - t1);
	int textX1 = (int)(-800 + (850 * ease1));
	
	if (mnAllTimer > 0)
	{
		SetFontSize(80);
		DrawFormatString(textX1 + 5, 100 + 5, GetColor(50, 50, 0), "QUEST CLEARED");
		DrawFormatString(textX1, 100, GetColor(255, 255, 255), "QUEST CLEARED");
	}
	
	// 戦績とステータス
	if (mnAllTimer >= 30)
	{
		float t2 = (mnAllTimer - 30) / 30.0f;
		if (t2 > 1.0f) t2 = 1.0f;
		float ease2 = 1.0f - (1.0f - t2) * (1.0f - t2) * (1.0f - t2);
		int textX2 = (int)(-800 + (850 * ease2));
		
		SetFontSize(40);
		DrawFormatString(textX2, 250, GetColor(200, 200, 255), "--- Battle Result ---");
		
		SetFontSize(35);
		DrawFormatString(textX2, 330, GetColor(255, 255, 255), "Defeated Enemies : %d", Master::mpScoreManager->GetDefeatedEnemies());
		DrawFormatString(textX2, 390, GetColor(255, 255, 255), "Potions Used     : %d", Master::mpScoreManager->GetUsedPotions());

		SetFontSize(40);
		DrawFormatString(textX2, 500, GetColor(255, 200, 200), "--- Final Status ---");
		
		SetFontSize(35);
		DrawFormatString(textX2, 580, GetColor(100, 255, 100), "Max HP   : %.0f", Master::mpScoreManager->GetFinalHp());
		DrawFormatString(textX2, 640, GetColor(255, 100, 100), "Attack   : %.0f", Master::mpScoreManager->GetFinalAttack());
		DrawFormatString(textX2, 700, GetColor(100, 200, 255), "Speed    : %.0f", Master::mpScoreManager->GetFinalSpeed());
	}
	
	// 戻るボタン
	if (mnAllTimer >= 100)
	{
		int alpha = (mnAllTimer % 60 < 30) ? 255 : 100;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		SetFontSize(40);
		DrawFormatString(100, 900, GetColor(255, 255, 255), "Tap / Click to Return");
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (InputManager::CheckMouseClickLeft() || InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}
	
	SetFontSize(size);
}

void ResultWin::Finalize()
{
	if (mpPlayerModel) {
		delete mpPlayerModel;
		mpPlayerModel = nullptr;
	}
}
