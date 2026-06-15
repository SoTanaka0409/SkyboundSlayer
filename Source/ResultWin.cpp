#include"ResultWin.h"
#include"Master.h"
#include"SceneManager.h"
#include"InputManager.h"
#include"Master.h"

ResultWin::ResultWin()
	:AllTimer(0)
	,Name(0)
{
}

ResultWin::~ResultWin()
{

}

void ResultWin::Initialize()
{
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);
}

void ResultWin::Update()
{
	AllTimer++;

}

void ResultWin::Draw()
{
	
	mpTexture = new Texture("Resource/result.png", VGet(500, 500, 0), true);
	mpTexture->Draw();
	int size = GetFontSize();
	Master::mpScoreManager->SaveHighScore();//スコアを保存
	
	unsigned int Color;
	Color = GetColor(0, 255, 255);
	if (AllTimer >= 100)
	{
		if (Master::mpScoreManager->GetScore() >= 0)
		{
			SetFontSize(40);
			/*DrawFormatString(300, 280, Color, "%sさんのスコアは",Master::GetName());
			DrawKeyInputString(300, 280, mpTitle->Name);
			DrawFormatString(300, 280, Color, "　　　　　　　　%d！！",Master::mpScoreManager->GetScore());*/
			DrawFormatString(320, 660, Color, "   %sはクリアした",Master::NameTest);
		}
		else
		{
		}
	}
	
	SetFontSize(size);
	if (AllTimer >= 200)
	{
		
		DrawFormatString(400, 750, GetColor(255, 255, 255), "   BackSpaceを押して");
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_NAME);
		}
	}
	////// 半透明の黒い矩形を描画
	////// ブレンドモードを元に戻す（重要）
}

void ResultWin::Finalize()
{
	Master::mpSoundManager->StopBGM();
}