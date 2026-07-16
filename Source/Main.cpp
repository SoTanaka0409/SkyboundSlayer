
#include "DxLib.h"
#include "Config.h"
#include"Texture.h"
//#include"Texture Animation.h"
#include"Collision.h"
#include"SceneManager.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Player3D.h"
#include"camera.h"
#include"ScoreManager.h"
#include"ResourceManager.h"
#include"Debug.h"
#include"DrawHp.h"
#include"EffekseerManager.h"

#include"EnemyManager.h"
#include"ColliderManager.h"
#include"EffectPool.h"

#include"ItemManager.h"
#include"InfClassManager.h"
#include"InfClass.h"
#include"BuffManager.h"
#include"Chat.h"
#include"Save.h"
/**
* @note 繝ｪ繝輔ぃ繝ｬ繝ｳ繧?E?E?E? https://dxlib.xsrv.jp/dxfunc.html
*/

//繧?E?E?E?繝ｼ繝?縺?E?E?E?・泥繝｢繝?E?E  metaseq316
//https://www.d5render.com/ja/workflow/blender?utm_campaign=bingsearchILJPblender&utm_source=bing&utm_medium=cpc&msclkid=929170cec1521e953f1c187910ba1cae



/**
/**
* @fn WinMain
* @brief Main髢?E?E?E?謨?E?E?E?
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 豁E?E??E?蟶?E?E?E?邨めE?E??E?・?E?E?E?・1 繧?E?E?E?繝ｩ繝ｼ
* @details Main髢?E?E?E?謨?E?E?E?
*/

//Master繧?E?E?E?繝ｩ繧?E?E?E?縺?E?E?E?髱咏噪繝｡繝ｳ繝仙､画焚螳夂ｾ?E?E?E?
Player3D* Master::mpPlayer = nullptr;
SceneManager* Master::mpSceneManager = new SceneManager();//蜻?E?E?E?縺?E?E?E??E?E??E?縺・
SoundManager* Master::mpSoundManager = new SoundManager();
ResourceManager* Master::mpResourceManager = new ResourceManager();
Debug* Master::mpDebug = new Debug();
DrawHp* Master::mpDrawHp = new DrawHp();
Camera* Master::mpCamera = new Camera();
ScoreManager* Master::mpScoreManager = new ScoreManager(0);
EnemyManager* Master::mpEnemyManager = new EnemyManager();
ItemManager* Master::mpItemManager = new ItemManager();
InfClassManager* Master::mpInfClassManager = new InfClassManager();
BuffManager* Master::mpBuffManager = new BuffManager();
Chat* Master::mpChat = new Chat();
Save* Master::mpSave = new Save();

/// <summary>
/// ///////////////////////Enemy//////////////////
/// </summary>

bool Master::PauseOn = false;
bool Master::StatShopClassOn = false;
bool Master::SafePointOn = false;
bool Master::NearShopOn = false;
bool Master::mbSave = false;
bool Master::CutscenePlaying = false;


int Master::GameClearCount = 0;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 繧?E?E?E?繧?E?E?E?繝ｳ繝峨え繝｢繝ｼ繝峨〒襍ｷ蜍?E
	ChangeWindowMode(true);

	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	
	// DX繝ｩ繧?E?E?E?繝悶Λ繝ｪ蛻晁E?E??E?蛹・
	SetDoubleStartValidFlag(TRUE);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	EffekseerManager::GetInstance()->Init();

	// --- 蜈ｨ菴薙?E繝ｩ繧?E?E?E?繝?EぁE?E??E??E?繧?E?E?E?・育腸?E?E?E・繝ｻ螟ｪ髯?E?E?E?蜈会ｼ芽?E?E?E??E?E?E?螳・---
	SetLightEnable(TRUE);
	// 迺?E?E?E??E?E?E・・・mbColor・峨?E?E?E??E???E?E鬮倥?E?E?E?縺?E?E?E?險?E?E?E?螳壹?E?E縲∝ｽ?E?E?E?縺?E?E?E?縺?E?E?E?繧翫?E?E?E?E?E??E?吶?E?E?E?驛ｨ蛻・?E?E?E?亥?E?E?E?暮擁E?E??E?・・髱?E?E?E?・峨′逵溘▲鮟?E↓豐医∪縺?E?E?E?縺・?E?E?E?縺・↓縺吶?E?E?E?E
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	// 螟ｪ髯?E?E?E?蜈会ｼ医ョ繧?E?E?E?繝ｬ繧?E?E?E?繧?E?E?E?繝ｧ繝翫Ν繝ｩ繧?E?E?E?繝茨?E?E?E?峨・蜷代″繧呈丁E?E??E?∽?E?E?E?九?E蜷代?E?E繧・
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	// 螟ｪ髯?E?E?E?蜈峨・濶?E?E?E?・亥?E?E?E?代?E?E逋ｽ縺?E?E?E?縺後°縺?E?E?E?縺滓?E繧九＞濶?E?E?E?・・
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	// ------------------------------------------------


	//BGM縺?E?E?E?隱?E?E?E?縺?E?E?E?霎ｼ縺?E?E?E?

	//繧?E?E?E?繧?E?E?E?繝ｳ繝峨・繝阪・繧?E?E?E?繝｣繝ｼ縺?E?E?E?蛻晁E?E??E?蛹・
	SetUseASyncLoadFlag(TRUE);
	Master::mpSoundManager->Initialize();//縺吶∋縺?E?E?E?縺?E?E?E?繧?E?E?E?繧?E?E?E?繝ｳ繝峨′隱?E?E?E?縺?E?E?E?霎ｼ縺?E?E?E?繧罫u----

	//繧?E?E?E?繝ｼ繝ｳ繝槭ロ繝ｼ繧?E?E?E?繝｣繝ｼ縺?E?E?E?逕滓?E縺?E?E?E?蛻晁E?E??E?蛹・
	Master::mpSceneManager->Initialize();

	Master::mpScoreManager->Initialize();

	//繧?E?E?E?繝｡繝ｩ縺?E?E?E?譖ｴ譁E?E??E?
	Master::mpCamera->Initialize();

	



	//?E?冗判蜈郁?E?E?E??E?E?E?螳壹?E?E?E?陬冗判髱?E?E?E?縺?E?E?E?險?E?E?E?螳壹?E?E??E?・
	SetDrawScreen(DX_SCREEN_BACK);

	//Z繝?Eャ繝輔ぃ縺?E?E?E?譖ｸ縺崎ｾ?E?E?E?繧?E??E?E??E??E?E?E?E
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	SetFontSize(20);
	int animationCounter = 0;
	int textureCurrentNum = 0;

	//繧?E?E?E?繝ｼ繝?縺?E?E?E?繝｡繧?E?E?E?繝ｳ繝ｫ繝ｼ繝?E
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//逕ｻ髱?E?E?E?繧貞?E譛溷喧縺吶?E?E?E?E
		ClearDrawScreen();
		int time = GetNowCount();

		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}
		else
		{
				Master::mpDrawHp->Update();
				Master::mpCamera->Update();
		
				Master::mpSceneManager->Update();
				Master::mpInfClassManager->Update();
				EffekseerManager::GetInstance()->Update();
				EffectPool::GetInstance()->Update();
		}
		
		Master::mpSceneManager->Draw();
		Master::mpScoreManager->Draw();
		EffekseerManager::GetInstance()->Draw();
		EffectPool::GetInstance()->Draw();


		
		


		//陬冗判髱?E?E?E?縺?E?E?E??E?・?E?E?E??E?E?E?繧定｡?E?E?E?逕ｻ髱?E?E?E?縺?E?E?E?譏?縺・
		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}
		ScreenFlip();

		//・托ｼ励Α繝ｪ遘抵?E?E?E?育?E?E?E?呈?E髢楢?E?E?E??E?E?E?・?E?E??E?舌ヵ繝ｬ繝ｼ繝?縺?E?E?E?縺?E?E?E?縺溘▲蝣?E?E?E?蜷茨?E?E?E?代ヵ繝ｬ繝ｼ繝?蠖薙?E?E??E?翫・邨碁℃譎る俣
		//・托ｼ励Α繝ｪ遘抵?E?E?E?育?E?E?E?呈?E髢楢?E?E?E??E?E?E?・?E?E??E?舌ヵ繝ｬ繝ｼ繝縺縺?E?E?E?縺溘▲蝣?E?E?E?蜷茨?E?E?E?代ヵ繝ｬ繝ｼ繝?薙?E?E??E?翫・邨碁℃譎る俣
		//・托ｼ励Α繝ｪ遘抵E育E?E髢楢EE・?E舌ヵ繝ｬ繝ｼ繝縺E縺E縺溘▲蝣E蜷茨E代ヵ繝ｬ繝ｼ繝蠖薙?E翫・邨碁℃譎る俣
		//・托ｼ励Α繝ｪ遘抵E育E?E髢楢EE・?E舌ヵ繝ｬ繝ｼ繝縺縺E縺溘▲蝣E蜷茨E代ヵ繝ｬ繝ｼ繝薙?E翫・邨碁℃譎る俣
		//邨碁℃縺吶E縺E縺E縺薙！EE蠕E▽
		while (GetNowCount() - time < 17)
		{
			//蠕E▽縺縺代↑縺E縺E縺薙！EE縺E菴輔ｂ譖ｸ縺九E縺・
		}

		//蜑企勁縺吶E・E√E縺めE繧
		if (GetASyncLoadNum() == 0)
		{
			//削除するフラグがあるオブジェクトがあれば削除する
			Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
			Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
			ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();

			//ループ?E直前にシーン遷移をいれておく
			Master::mpSceneManager->ChangeSceneIfNeeded();
		}
		

		
	}
	//邨めE・・送EE
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;
	Master::mpScoreManager->Finalize();
	delete Master::mpScoreManager;
	Master::mpCamera->Finalize();
	delete Master::mpCamera;
	delete Master::mpResourceManager;

	ColliderManager::GetInstance()->Finalize();



	// DX繝ｩ繧?E?E?E?繝悶Λ繝ｪ菴?E?E?E?逕ｨ縺?E?E?E?邨めE?E??E?・
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	// 繧?E?E?E?繝輔ヨ縺?E?E?E?邨めE?E??E?・
	return 0;
}





