#include"NameScene.h"
#include"Master.h"
#include"InputManager.h"


NameScene::NameScene()
: Color1(1)
, Colorflag(false)
{
	mpTexture = new Texture("Resource/NameScene.png", VGet(500, 500, 0), true);
	
}

NameScene::~NameScene()
{

}

void NameScene::Initialize()
{

}

void NameScene::Draw()
{
	
	if (Colorflag == true)
	{
		Color1 -= 4;
		if (Color1 <= 0)
		{
			Color1 = 0;
			Colorflag = false;
		}
	}
	if (Color1 >= 0 && Colorflag == false)
	{
		Color1 += 4;
		if (Color1 >= 255)
		{
			Color1 = 255;
			Colorflag = true;
		}
	}

	mpTexture->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100); // 0～255（128＝50%透明）
	DrawBox(0, 0, 1280, 1000, GetColor(0, 0, 0), TRUE);
	DrawBox(100, 650, 850, 670, GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// キー入力ハンドルを作る(キャンセルなし全角文字有り数値入力じゃなし)
	InputHandle = MakeKeyInput(50, FALSE, FALSE, FALSE);

	// 作成したキー入力ハンドルをアクティブにする
	SetActiveKeyInput(InputHandle);

	while (ProcessMessage() == 0)
	{
		
		/*if (InputManager::CheckDownKey(KEY_INPUT_))
		{
			InputHandle = 0;
		}*/
		// 入力が終了している場合は終了
		
		/*if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_RESET);
			break;
		}*/
		

		SetFontSize(60);
		DrawFormatString(-50, 200, GetColor(200, 255, 0), "        名前を入れてください");
		SetFontSize(40);
		DrawFormatString(100, 300, GetColor(255, 255, 255), "\n（このように書いてください->TANAKASOU)");
		DrawFormatString(100, 300, GetColor(255, 255, 255), "\n\n     大文字で行間を作らないように");
		DrawFormatString(200, 800, GetColor(-Color1, -Color1, -Color1), "BackSpaceでやり直し  Enterで完了");/*0でやり直し\n*/

		if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
			break;
			SetFontSize(size);
		}
		if(InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_RESET);
			break;
			SetFontSize(size);
		}

		// 入力モードを描画
		DrawKeyInputModeString(400, 360);

		// 入力途中の文字列を描画
		DrawKeyInputString(300, 600, InputHandle);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
		

	}

	GetKeyInputString(Master::NameTest, InputHandle);


	// 用済みのインプットハンドルを削除する
	DeleteKeyInput(InputHandle);

	


	
	SetFontSize(size);
}

void NameScene::Update()
{

}

void NameScene::Finalize()
{

}