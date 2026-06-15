#include"TitleScene.h"
#include"Texture.h"
#include"InputManager.h"
#include"Master.h"
#include"SceneManager.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"ColliderManager.h"

TitleScene::TitleScene()
:mnPause(0)
, C(false)
, S(false)
, E(false)
,T(false)
, C_Enter(false)
, S_Enter(false)
, E_Enter(false)
, Color1(1)
, Colorflag(false)
{
	mnPause = 1;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	{/////////リセット///////////
		Master::TutorialFlag = false;
		Master::TutorialCount = 0;
		Master::mpCamera->Initialize();//最初にカメラをつける位置を変更
		Master::GameClearCount = 0;
		ColliderManager::GetInstance()->DeleteAllCollider();
	}
	
	mpTexture=new Texture("Resource/Title", VGet(500,420, 0), true);
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
	
}

void TitleScene::Draw()
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
	
	
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // 0～255（128＝50%透明）
		Scene::Draw();
		DrawBox(0, 0, 1000, 1000, GetColor(0, 0, 0), true);

		SetFontSize(60);
		DrawFormatString(300, 10, GetColor(255, 255, 255), "Dino&Hunter");
		mpTexture->Draw();
		C = false;
		E = false;
		S = false;
		T = false;
		/*C_Enter = false;
		S_Enter = false;
		E_Enter = false;*/
		int Color = GetColor(255, 255, 255);
		Master::mpScoreManager->LoadHighScore();
	

		if (InputManager::CheckDownKey(KEY_INPUT_W))
		{
			mnPause--;
			Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//効果音
			if (mnPause < 1)
			{
				mnPause = 3;
			}
		}
		if (InputManager::CheckDownKey(KEY_INPUT_S))
		{
			mnPause++;
			Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//効果音
			if (mnPause > 3)
			{
				mnPause = 1;
			}
		}
		switch (mnPause)
		{
			SetFontSize(35);
		case 1:
			
			DrawFormatString(50, 750, GetColor(0, 255, 255), "GameStart!");
			
			C = true;
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
			{
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
			}

			break;
		case 2:
			
			DrawFormatString(50, 810, GetColor(0, 255, 255), "操作方法");
			
			S = true;
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
			{
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_OPERATION);
			}
			break;
		
		case 3:
			(35);
			DrawFormatString(50, 870, GetColor(0, 255, 255), "チュートリアル");
			
			T = true;
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
			{
				Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TUTORIAL);
			}
			break;
		}
		
		if (C_Enter == true)
		{


		}
		if (S_Enter == true)
		{


		}

		SetFontSize(20);
			if (C == false)
			{
				DrawFormatString(50, 770, GetColor(255, 255, 255), "1:GameStart!");
			}
			if (S == false)
			{
				DrawFormatString(50, 820, GetColor(255, 255, 255), "2:操作方法");
			}
			
			if (T == false)
			{
				DrawFormatString(50, 870, GetColor(255, 255, 255), "3:チュートリアル");
			}
		
			DrawFormatString(750, 960, GetColor(255, 255, 255), "ENTERで決定:WSで選択");



		SetFontSize(size);
	
}

void TitleScene::Update()
{
	Scene::Update();
	
}

void TitleScene::Finalize()
{
	Master::mpSoundManager->StopBGM();
}