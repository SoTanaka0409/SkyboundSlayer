
#include "DxLib.h"
#include "Config.h"
#include"Texture.h"
//#include"Texture Animation.h"
#include"Player.h"
#include"Collision.h"
#include"SceneManager.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Player3D.h"
#include"camera.h"
#include"Camera1.h"
#include"Weapon.h"
#include"WeaponManager.h"
#include"ScreenSize.h"
#include"ScoreManager.h"
#include"ResourceManager.h"
#include"Debugh.h"
#include"EnemySerch.h"
#include"DrawHp.h"
#include"EffekseerManager.h"

#include"EnemyManager.h"
#include"ColliderManager.h"

#include"ItemManager.h"
#include"InfClassManager.h"
#include"InfClass.h"
#include"BuffManager.h"
#include"Chat.h"
#include"Save.h"
/**
* @note 繝ｪ繝輔ぃ繝ｬ繝ｳ繧ｹ https://dxlib.xsrv.jp/dxfunc.html
*/

//繧ｲ繝ｼ繝縺ｮ・泥繝｢繝・Ν   metaseq316
//https://www.d5render.com/ja/workflow/blender?utm_campaign=bingsearchILJPblender&utm_source=bing&utm_medium=cpc&msclkid=929170cec1521e953f1c187910ba1cae



/**
/**
* @fn WinMain
* @brief Main髢｢謨ｰ
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 豁｣蟶ｸ邨ゆｺ・ｼ・1 繧ｨ繝ｩ繝ｼ
* @details Main髢｢謨ｰ
*/

//Master繧ｯ繝ｩ繧ｹ縺ｮ髱咏噪繝｡繝ｳ繝仙､画焚螳夂ｾｩ
Player3D* Master::mpPlayer = nullptr;
SceneManager* Master::mpSceneManager = new SceneManager();//蜻ｼ縺ｳ蜃ｺ縺・
SoundManager* Master::mpSoundManager = new SoundManager();
WeaponManager* Master::mpWeaponManager = new WeaponManager();
Weapon* Master::mpWeapon = new Weapon();
ResourceManager* Master::mpResourceManager = new ResourceManager();
Debug* Master::mpDebug = new Debug();
EnemySerch* Master::mpEnemySerch = new EnemySerch();
DrawHp* Master::mpDrawHp = new DrawHp();
Camera* Master::mpCamera = new Camera();
ScoreManager* Master::mpScoreManager = new ScoreManager(0);
LevelUp* Master::mpLevelUp = new LevelUp();
EnemyManager* Master::mpEnemyManager = new EnemyManager();
ItemManager* Master::mpItemManager = new ItemManager();
InfClassManager* Master::mpInfClassManager = new InfClassManager();
BuffManager* Master::mpBuffManager = new BuffManager();
Chat* Master::mpChat = new Chat();
Save* Master::mpSave = new Save();

char Master::NameTest[256];
int Master::TutorialCount;
int Master::TutorialClearCount;
bool Master::TutorialFlag=false;
/// <summary>
/// ///////////////////////Enemy//////////////////
/// </summary>

bool Master::PauseOn = false;
bool Master::ShopClassOn = false;
bool Master::StatShopClassOn = false;
bool Master::SafePointOn = false;
bool Master::NearShopOn = false;
bool Master::mbSave = false;


int Master::GameClearCount = 0;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 繧ｦ繧､繝ｳ繝峨え繝｢繝ｼ繝峨〒襍ｷ蜍・
	ChangeWindowMode(true);

	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	
	// DX繝ｩ繧､繝悶Λ繝ｪ蛻晄悄蛹・
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	EffekseerManager::GetInstance()->Init();

	// --- 蜈ｨ菴薙・繝ｩ繧､繝・ぅ繝ｳ繧ｰ・育腸蠅・・繝ｻ螟ｪ髯ｽ蜈会ｼ芽ｨｭ螳・---
	SetLightEnable(TRUE);
	// 迺ｰ蠅・・・・mbColor・峨ｒ蟆代＠鬮倥ａ縺ｫ險ｭ螳壹＠縲∝ｽｱ縺ｫ縺ｪ繧翫ｄ縺吶＞驛ｨ蛻・ｼ亥ｺ暮擇繧・・髱｢・峨′逵溘▲鮟偵↓豐医∪縺ｪ縺・ｈ縺・↓縺吶ｋ
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	// 螟ｪ髯ｽ蜈会ｼ医ョ繧｣繝ｬ繧ｯ繧ｷ繝ｧ繝翫Ν繝ｩ繧､繝茨ｼ峨・蜷代″繧呈万繧∽ｸ九↓蜷代￠繧・
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	// 螟ｪ髯ｽ蜈峨・濶ｲ・亥ｰ代＠逋ｽ縺ｿ縺後°縺｣縺滓・繧九＞濶ｲ・・
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	// ------------------------------------------------


	//BGM縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ

	//繧ｵ繧ｦ繝ｳ繝峨・繝阪・繧ｸ繝｣繝ｼ縺ｮ蛻晄悄蛹・
	Master::mpSoundManager->Initialize();//縺吶∋縺ｦ縺ｮ繧ｵ繧ｦ繝ｳ繝峨′隱ｭ縺ｿ霎ｼ縺ｾ繧罫u----

	//繧ｷ繝ｼ繝ｳ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｮ逕滓・縺ｨ蛻晄悄蛹・
	Master::mpSceneManager->Initialize();

	Master::mpScoreManager->Initialize();

	//繧ｫ繝｡繝ｩ縺ｮ譖ｴ譁ｰ
	Master::mpCamera->Initialize();

	



	//謠冗判蜈郁ｨｭ螳壹ｒ陬冗判髱｢縺ｫ險ｭ螳壹☆繧・
	SetDrawScreen(DX_SCREEN_BACK);

	//Z繝舌ャ繝輔ぃ縺ｫ譖ｸ縺崎ｾｼ繧貅門ｙ
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	SetFontSize(20);
	int animationCounter = 0;
	int textureCurrentNum = 0;

	//繧ｲ繝ｼ繝縺ｮ繝｡繧､繝ｳ繝ｫ繝ｼ繝・
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//逕ｻ髱｢繧貞・譛溷喧縺吶ｋ
		ClearDrawScreen();
		int time = GetNowCount();

		Master::mpDrawHp->Update();
		Master::mpCamera->Update();
		
		//譖ｴ譁ｰ
		Master::mpSceneManager->Update();
		Master::mpInfClassManager->Update();
		EffekseerManager::GetInstance()->Update();
	
		//謠冗判
		Master::mpSceneManager->Draw();
		Master::mpScoreManager->Draw();
		EffekseerManager::GetInstance()->Draw();


		
		


		//陬冗判髱｢縺ｮ蜀・ｮｹ繧定｡ｨ逕ｻ髱｢縺ｫ譏縺・
		ScreenFlip();

		//・托ｼ励Α繝ｪ遘抵ｼ育ｧ呈焚髢楢ｨｳ・厄ｼ舌ヵ繝ｬ繝ｼ繝縺縺｣縺溘▲蝣ｴ蜷茨ｼ代ヵ繝ｬ繝ｼ繝蠖薙◆繧翫・邨碁℃譎る俣
		//邨碁℃縺吶ｋ縺ｾ縺ｧ縺薙％縺ｧ蠕・▽
		while (GetNowCount() - time < 17)
		{
			//蠕・▽縺縺代↑縺ｮ縺ｧ縺薙％縺ｫ縺ｯ菴輔ｂ譖ｸ縺九↑縺・
		}

		//蜑企勁縺吶ｋ蠢・ｦ√・縺ゅｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医′縺ゅｌ縺ｰ蜑企勁縺吶ｋ
		ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
		

		//繝ｫ繝ｼ繝励☆繧狗峩蜑阪↓繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ繝√ぉ繝・け繧貞・繧後※縺翫￥
		Master::mpSceneManager->ChangeSceneIfNeeded();

		
	}
	//邨ゆｺ・・逅・
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



	// DX繝ｩ繧､繝悶Λ繝ｪ菴ｿ逕ｨ縺ｮ邨ゆｺ・
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	// 繧ｽ繝輔ヨ縺ｮ邨ゆｺ・
	return 0;
}




