#include"ResultScene.h"
#include"Master.h"
#include"SceneManager.h"
#include"InputManager.h"
#include"ObjectManager.h"
#include"Player3D.h"
#include"ResultChange.h"

#include"Scene3D.h"
#include"haikei.h"

ResultScene::ResultScene()
:AllTimer(0)
,Color1(1)
,Colorflag(false)
{
	

}

ResultScene::~ResultScene()
{

}
void ResultScene::Initialize()
{
	mpTexture = new Texture("Resource/result.png", VGet(500, 500, 0), true);
	mpTexture2= new Texture("Resource/street.png", VGet(500, 850, 0), true);
	auto mpPlayer = Master::mpPlayer;
	auto pPlayer = Master::mpPlayer;

	

}

void ResultScene::Draw()
{
	const TCHAR* text = _T("RAINBOW TEXT");
	int colors[] = {
		GetColor(255, 0, 0),     // 赤
		GetColor(255, 165, 0),   // オレンジ
		GetColor(255, 255, 0),   // 黄
		GetColor(0, 255, 0),     // 緑
		GetColor(0, 0, 255),     // 青
		GetColor(128, 0, 128)    // 紫
	};
	int Size = GetFontSize();
	/*mpTexture = new Texture("", VGet(0, 0, 0), true);
	auto mpPlayer = Master::mpPlayer;
	auto pPlayer = Master::mpPlayer;*/
	
	
	
	
	mpTexture->Draw();
	mpTexture2->Draw();
	//mpTexture

	(50);
	int Color = GetColor(255, 0, 255);
	DrawFormatString(320, 700, Color, "リベンジしてね");
	Scene::Draw();
	
	if (AllTimer >= 200)
	{
		int x = 300;
		int y = 750;

		if (Colorflag==true)
		{
			Color1-=4;
			if (Color1 <= 0)
			{
				Color1 = 0;
				Colorflag = false;
			}
		}
		if (Color1 >= 0&&Colorflag==false)
		{
			Color1+=4;
			if (Color1 >= 255)
			{
				Color1 = 255;
				Colorflag = true;
			}
		}
		DrawFormatString(x,y, GetColor(Color1, Color1,0), "BackSpaceを押して");
		
		
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{

			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}
	(Size);
	////// 半透明の黒い矩形を描画
	////// ブレンドモードを元に戻す（重要）
}
void ResultScene::Update()
{
	AllTimer++;
	
	Scene::Update();

}

void ResultScene::Finalize()
{
}
