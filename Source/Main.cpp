
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
#include"ScreenSize.h"
#include"ScoreManager.h"
#include"ResourceManager.h"
#include"Debug.h"
#include"EnemySearch.h"
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
* @note 郢晢ｽｪ郢晁ｼ斐＜郢晢ｽｬ郢晢ｽｳ郢ｧ・ｹ https://dxlib.xsrv.jp/dxfunc.html
*/

//郢ｧ・ｲ郢晢ｽｼ郢晢ｿｽ邵ｺ・ｮ繝ｻ豕･郢晢ｽ｢郢昴・ﾎ・  metaseq316
//https://www.d5render.com/ja/workflow/blender?utm_campaign=bingsearchILJPblender&utm_source=bing&utm_medium=cpc&msclkid=929170cec1521e953f1c187910ba1cae



/**
/**
* @fn WinMain
* @brief Main鬮｢・｢隰ｨ・ｰ
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 雎・ｽ｣陝ｶ・ｸ驍ｨ繧・ｽｺ繝ｻ・ｼ繝ｻ1 郢ｧ・ｨ郢晢ｽｩ郢晢ｽｼ
* @details Main鬮｢・｢隰ｨ・ｰ
*/

//Master郢ｧ・ｯ郢晢ｽｩ郢ｧ・ｹ邵ｺ・ｮ鬮ｱ蜥丞飭郢晢ｽ｡郢晢ｽｳ郢昜ｻ呻ｽ､逕ｻ辟夊楜螟ゑｽｾ・ｩ
Player3D* Master::mpPlayer = nullptr;
SceneManager* Master::mpSceneManager = new SceneManager();//陷ｻ・ｼ邵ｺ・ｳ陷・ｽｺ邵ｺ繝ｻ
SoundManager* Master::mpSoundManager = new SoundManager();
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
bool Master::StatShopClassOn = false;
bool Master::SafePointOn = false;
bool Master::NearShopOn = false;
bool Master::mbSave = false;


int Master::GameClearCount = 0;




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 郢ｧ・ｦ郢ｧ・､郢晢ｽｳ郢晏ｳｨ縺育ｹ晢ｽ｢郢晢ｽｼ郢晏ｳｨ縲定･搾ｽｷ陷阪・
	ChangeWindowMode(true);

	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	
	// DX郢晢ｽｩ郢ｧ・､郢晄じﾎ帷ｹ晢ｽｪ陋ｻ譎・ｄ陋ｹ繝ｻ
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	EffekseerManager::GetInstance()->Init();

	// --- 陷茨ｽｨ闖ｴ阮吶・郢晢ｽｩ郢ｧ・､郢昴・縺・ｹ晢ｽｳ郢ｧ・ｰ繝ｻ閧ｲ閻ｸ陟・・繝ｻ郢晢ｽｻ陞滂ｽｪ鬮ｯ・ｽ陷井ｼ夲ｽｼ闃ｽ・ｨ・ｭ陞ｳ繝ｻ---
	SetLightEnable(TRUE);
	// 霑ｺ・ｰ陟・・繝ｻ繝ｻ繝ｻmbColor繝ｻ蟲ｨ・定氣莉｣・鬯ｮ蛟･・∫ｸｺ・ｫ髫ｪ・ｭ陞ｳ螢ｹ・邵ｲ竏晢ｽｽ・ｱ邵ｺ・ｫ邵ｺ・ｪ郢ｧ鄙ｫ・・ｸｺ蜷ｶ・樣ｩ幢ｽｨ陋ｻ繝ｻ・ｼ莠･・ｺ證ｮ謫・ｹｧ繝ｻ繝ｻ鬮ｱ・｢繝ｻ蟲ｨ窶ｲ騾ｵ貅倪夢魄溷・竊楢ｱ仙現竏ｪ邵ｺ・ｪ邵ｺ繝ｻ・育ｸｺ繝ｻ竊鍋ｸｺ蜷ｶ・・
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	// 陞滂ｽｪ鬮ｯ・ｽ陷井ｼ夲ｽｼ蛹ｻ繝ｧ郢ｧ・｣郢晢ｽｬ郢ｧ・ｯ郢ｧ・ｷ郢晢ｽｧ郢晉ｿｫﾎ晉ｹ晢ｽｩ郢ｧ・､郢晁肩・ｼ蟲ｨ繝ｻ陷ｷ莉｣窶ｳ郢ｧ蜻井ｸ・ｹｧ竏ｽ・ｸ荵昶・陷ｷ莉｣・郢ｧ繝ｻ
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	// 陞滂ｽｪ鬮ｯ・ｽ陷亥ｳｨ繝ｻ豼ｶ・ｲ繝ｻ莠･・ｰ莉｣・騾具ｽｽ邵ｺ・ｿ邵ｺ蠕個ｰ邵ｺ・｣邵ｺ貊薙・郢ｧ荵晢ｼ樊ｿｶ・ｲ繝ｻ繝ｻ
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	// ------------------------------------------------


	//BGM邵ｺ・ｮ髫ｱ・ｭ邵ｺ・ｿ髴趣ｽｼ邵ｺ・ｿ

	//郢ｧ・ｵ郢ｧ・ｦ郢晢ｽｳ郢晏ｳｨ繝ｻ郢晞亂繝ｻ郢ｧ・ｸ郢晢ｽ｣郢晢ｽｼ邵ｺ・ｮ陋ｻ譎・ｄ陋ｹ繝ｻ
	Master::mpSoundManager->Initialize();//邵ｺ蜷ｶ竏狗ｸｺ・ｦ邵ｺ・ｮ郢ｧ・ｵ郢ｧ・ｦ郢晢ｽｳ郢晏ｳｨ窶ｲ髫ｱ・ｭ邵ｺ・ｿ髴趣ｽｼ邵ｺ・ｾ郢ｧ鄂ｫu----

	//郢ｧ・ｷ郢晢ｽｼ郢晢ｽｳ郢晄ｧｭ繝ｭ郢晢ｽｼ郢ｧ・ｸ郢晢ｽ｣郢晢ｽｼ邵ｺ・ｮ騾墓ｻ薙・邵ｺ・ｨ陋ｻ譎・ｄ陋ｹ繝ｻ
	Master::mpSceneManager->Initialize();

	Master::mpScoreManager->Initialize();

	//郢ｧ・ｫ郢晢ｽ｡郢晢ｽｩ邵ｺ・ｮ隴厄ｽｴ隴・ｽｰ
	Master::mpCamera->Initialize();

	



	//隰蜀怜愛陷磯メ・ｨ・ｭ陞ｳ螢ｹ・帝勳蜀怜愛鬮ｱ・｢邵ｺ・ｫ髫ｪ・ｭ陞ｳ螢ｹ笘・ｹｧ繝ｻ
	SetDrawScreen(DX_SCREEN_BACK);

	//Z郢晁・繝｣郢晁ｼ斐＜邵ｺ・ｫ隴厄ｽｸ邵ｺ蟠趣ｽｾ・ｼ郢ｧﾂ雋・摩・・
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	SetFontSize(20);
	int animationCounter = 0;
	int textureCurrentNum = 0;

	//郢ｧ・ｲ郢晢ｽｼ郢晢ｿｽ邵ｺ・ｮ郢晢ｽ｡郢ｧ・､郢晢ｽｳ郢晢ｽｫ郢晢ｽｼ郢昴・
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//騾包ｽｻ鬮ｱ・｢郢ｧ雋槭・隴帶ｺｷ蝟ｧ邵ｺ蜷ｶ・・
		ClearDrawScreen();
		int time = GetNowCount();

		Master::mpDrawHp->Update();
		Master::mpCamera->Update();
		
		//隴厄ｽｴ隴・ｽｰ
		Master::mpSceneManager->Update();
		Master::mpInfClassManager->Update();
		EffekseerManager::GetInstance()->Update();
	
		//隰蜀怜愛
		Master::mpSceneManager->Draw();
		Master::mpScoreManager->Draw();
		EffekseerManager::GetInstance()->Draw();


		
		


		//髯ｬ蜀怜愛鬮ｱ・｢邵ｺ・ｮ陷繝ｻ・ｮ・ｹ郢ｧ螳夲ｽ｡・ｨ騾包ｽｻ鬮ｱ・｢邵ｺ・ｫ隴擾ｿｽ邵ｺ繝ｻ
		ScreenFlip();

		//繝ｻ謇假ｽｼ蜉ｱﾎ醍ｹ晢ｽｪ驕俶慣・ｼ閧ｲ・ｧ蜻育・鬮｢讌｢・ｨ・ｳ繝ｻ蜴・ｽｼ闊後Ψ郢晢ｽｬ郢晢ｽｼ郢晢ｿｽ邵ｺ・ｽ邵ｺ・｣邵ｺ貅倪夢陜｣・ｴ陷ｷ闌ｨ・ｼ莉｣繝ｵ郢晢ｽｬ郢晢ｽｼ郢晢ｿｽ陟冶侭笳・ｹｧ鄙ｫ繝ｻ驍ｨ遒≫с隴弱ｋ菫｣
		//繝ｻ謇假ｽｼ蜉ｱﾎ醍ｹ晢ｽｪ驕俶慣・ｼ閧ｲ・ｧ蜻育・鬮｢讌｢・ｨ・ｳ繝ｻ蜴・ｽｼ闊後Ψ郢晢ｽｬ郢晢ｽｼ郢晉ｸｺ邵ｺ・｣邵ｺ貅倪夢陜｣・ｴ陷ｷ闌ｨ・ｼ莉｣繝ｵ郢晢ｽｬ郢晢ｽｼ郢晁冶侭笳・ｹｧ鄙ｫ繝ｻ驍ｨ遒≫с隴弱ｋ菫｣
		//驍ｨ遒≫с邵ｺ蜷ｶ・狗ｸｺ・ｾ邵ｺ・ｧ邵ｺ阮呻ｼ・ｸｺ・ｧ陟輔・笆ｽ
		while (GetNowCount() - time < 17)
		{
			//陟輔・笆ｽ邵ｺ邵ｺ莉｣竊醍ｸｺ・ｮ邵ｺ・ｧ邵ｺ阮呻ｼ・ｸｺ・ｫ邵ｺ・ｯ闖ｴ霈費ｽりｭ厄ｽｸ邵ｺ荵昶・邵ｺ繝ｻ
		}

		//陷台ｼ∝求邵ｺ蜷ｶ・玖｢繝ｻ・ｦ竏壹・邵ｺ繧・ｽ狗ｹｧ・ｪ郢晄じ縺夂ｹｧ・ｧ郢ｧ・ｯ郢晏現窶ｲ邵ｺ繧・ｽ檎ｸｺ・ｰ陷台ｼ∝求邵ｺ蜷ｶ・・
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
		ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();
		

		//郢晢ｽｫ郢晢ｽｼ郢晏干笘・ｹｧ迢怜ｳｩ陷鷹亂竊鍋ｹｧ・ｷ郢晢ｽｼ郢晢ｽｳ鬩包ｽｷ驕假ｽｻ郢昶・縺臥ｹ昴・縺醍ｹｧ雋槭・郢ｧ蠕娯ｻ邵ｺ鄙ｫ・･
		Master::mpSceneManager->ChangeSceneIfNeeded();

		
	}
	//驍ｨ繧・ｽｺ繝ｻ繝ｻ騾・・
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



	// DX郢晢ｽｩ郢ｧ・､郢晄じﾎ帷ｹ晢ｽｪ闖ｴ・ｿ騾包ｽｨ邵ｺ・ｮ驍ｨ繧・ｽｺ繝ｻ
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	// 郢ｧ・ｽ郢晁ｼ斐Κ邵ｺ・ｮ驍ｨ繧・ｽｺ繝ｻ
	return 0;
}




