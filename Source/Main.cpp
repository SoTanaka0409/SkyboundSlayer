
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
#include"EffectPool.h"

#include"ItemManager.h"
#include"InfClassManager.h"
#include"InfClass.h"
#include"BuffManager.h"
#include"Chat.h"
#include"Save.h"
/**
* @note 郢晢ｽｪ郢晁ｼ斐＜郢晢ｽｬ郢晢ｽｳ郢ｧ・ｽE・ｽ https://dxlib.xsrv.jp/dxfunc.html
*/

//郢ｧ・ｽE・ｽ郢晢ｽｼ郢晢ｿｽ邵ｺ・ｽE・ｽ繝ｻ豕･郢晢ｽ｢郢晢ｿｽEﾎ・  metaseq316
//https://www.d5render.com/ja/workflow/blender?utm_campaign=bingsearchILJPblender&utm_source=bing&utm_medium=cpc&msclkid=929170cec1521e953f1c187910ba1cae



/**
/**
* @fn WinMain
* @brief Main鬮｢・ｽE・ｽ隰ｨ・ｽE・ｽ
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 雎・・ｽ・ｽ陝ｶ・ｽE・ｽ驍ｨ繧・・ｽ・ｽ繝ｻ・ｽE・ｽ繝ｻ1 郢ｧ・ｽE・ｽ郢晢ｽｩ郢晢ｽｼ
* @details Main鬮｢・ｽE・ｽ隰ｨ・ｽE・ｽ
*/

//Master郢ｧ・ｽE・ｽ郢晢ｽｩ郢ｧ・ｽE・ｽ邵ｺ・ｽE・ｽ鬮ｱ蜥丞飭郢晢ｽ｡郢晢ｽｳ郢昜ｻ呻ｽ､逕ｻ辟夊楜螟ゑｽｾ・ｽE・ｽ
Player3D* Master::mpPlayer = nullptr;
SceneManager* Master::mpSceneManager = new SceneManager();//陷ｻ・ｽE・ｽ邵ｺ・ｽE・ｽ陷・・ｽ・ｽ邵ｺ繝ｻ
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
	// 郢ｧ・ｽE・ｽ郢ｧ・ｽE・ｽ郢晢ｽｳ郢晏ｳｨ縺育ｹ晢ｽ｢郢晢ｽｼ郢晏ｳｨ縲定･搾ｽｷ陷搾ｿｽE
	ChangeWindowMode(true);

	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	
	// DX郢晢ｽｩ郢ｧ・ｽE・ｽ郢晄じﾎ帷ｹ晢ｽｪ陋ｻ譎・・ｽ・ｽ陋ｹ繝ｻ
	SetDoubleStartValidFlag(TRUE);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	EffekseerManager::GetInstance()->Init();

	// --- 陷茨ｽｨ闖ｴ阮呻ｿｽE郢晢ｽｩ郢ｧ・ｽE・ｽ郢晢ｿｽE縺・・ｽ・ｽ・ｳ郢ｧ・ｽE・ｽ繝ｻ閧ｲ閻ｸ陟・・ｽE繝ｻ郢晢ｽｻ陞滂ｽｪ鬮ｯ・ｽE・ｽ陷井ｼ夲ｽｼ闃ｽ・ｽE・ｽ・ｽE・ｽ陞ｳ繝ｻ---
	SetLightEnable(TRUE);
	// 霑ｺ・ｽE・ｽ陟・・ｽE繝ｻ繝ｻ繝ｻmbColor繝ｻ蟲ｨ・ｽE・ｽ陝・ｻ｣・ｽE鬯ｮ蛟･・ｽE・ｽ邵ｺ・ｽE・ｽ髫ｪ・ｽE・ｽ陞ｳ螢ｹ・ｽE邵ｲ竏晢ｽｽ・ｽE・ｽ邵ｺ・ｽE・ｽ邵ｺ・ｽE・ｽ郢ｧ鄙ｫ・ｽE・ｽE・ｽ・ｽ蜷ｶ・ｽE・ｽ鬩幢ｽｨ陋ｻ繝ｻ・ｽE・ｽ莠･・ｽE・ｽ證ｮ謫・・ｽ・ｽ繝ｻ繝ｻ鬮ｱ・ｽE・ｽ繝ｻ蟲ｨ窶ｲ騾ｵ貅倪夢魄滂ｿｽE竊楢ｱ仙現竏ｪ邵ｺ・ｽE・ｽ邵ｺ繝ｻ・ｽE・ｽ邵ｺ繝ｻ竊鍋ｸｺ蜷ｶ・ｽE・ｽE
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	// 陞滂ｽｪ鬮ｯ・ｽE・ｽ陷井ｼ夲ｽｼ蛹ｻ繝ｧ郢ｧ・ｽE・ｽ郢晢ｽｬ郢ｧ・ｽE・ｽ郢ｧ・ｽE・ｽ郢晢ｽｧ郢晉ｿｫﾎ晉ｹ晢ｽｩ郢ｧ・ｽE・ｽ郢晁肩・ｽE・ｽ蟲ｨ繝ｻ陷ｷ莉｣窶ｳ郢ｧ蜻井ｸ・・ｽ・ｽ竏ｽ・ｽE・ｽ荵晢ｿｽE陷ｷ莉｣・ｽE郢ｧ繝ｻ
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	// 陞滂ｽｪ鬮ｯ・ｽE・ｽ陷亥ｳｨ繝ｻ豼ｶ・ｽE・ｽ繝ｻ莠･・ｽE・ｽ莉｣・ｽE騾具ｽｽ邵ｺ・ｽE・ｽ邵ｺ蠕個ｰ邵ｺ・ｽE・ｽ邵ｺ貊難ｿｽE郢ｧ荵晢ｼ樊ｿｶ・ｽE・ｽ繝ｻ繝ｻ
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	// ------------------------------------------------


	//BGM邵ｺ・ｽE・ｽ髫ｱ・ｽE・ｽ邵ｺ・ｽE・ｽ髴趣ｽｼ邵ｺ・ｽE・ｽ

	//郢ｧ・ｽE・ｽ郢ｧ・ｽE・ｽ郢晢ｽｳ郢晏ｳｨ繝ｻ郢晞亂繝ｻ郢ｧ・ｽE・ｽ郢晢ｽ｣郢晢ｽｼ邵ｺ・ｽE・ｽ陋ｻ譎・・ｽ・ｽ陋ｹ繝ｻ
	SetUseASyncLoadFlag(TRUE);
	Master::mpSoundManager->Initialize();//邵ｺ蜷ｶ竏狗ｸｺ・ｽE・ｽ邵ｺ・ｽE・ｽ郢ｧ・ｽE・ｽ郢ｧ・ｽE・ｽ郢晢ｽｳ郢晏ｳｨ窶ｲ髫ｱ・ｽE・ｽ邵ｺ・ｽE・ｽ髴趣ｽｼ邵ｺ・ｽE・ｽ郢ｧ鄂ｫu----

	//郢ｧ・ｽE・ｽ郢晢ｽｼ郢晢ｽｳ郢晄ｧｭ繝ｭ郢晢ｽｼ郢ｧ・ｽE・ｽ郢晢ｽ｣郢晢ｽｼ邵ｺ・ｽE・ｽ騾墓ｻ難ｿｽE邵ｺ・ｽE・ｽ陋ｻ譎・・ｽ・ｽ陋ｹ繝ｻ
	Master::mpSceneManager->Initialize();

	Master::mpScoreManager->Initialize();

	//郢ｧ・ｽE・ｽ郢晢ｽ｡郢晢ｽｩ邵ｺ・ｽE・ｽ隴厄ｽｴ隴・・ｽ・ｽ
	Master::mpCamera->Initialize();

	



	//・ｽ蜀怜愛陷磯メ・ｽE・ｽ・ｽE・ｽ陞ｳ螢ｹ・ｽE・ｽ髯ｬ蜀怜愛鬮ｱ・ｽE・ｽ邵ｺ・ｽE・ｽ髫ｪ・ｽE・ｽ陞ｳ螢ｹ笘・・ｽ・ｽ繝ｻ
	SetDrawScreen(DX_SCREEN_BACK);

	//Z郢晢ｿｽE繝｣郢晁ｼ斐＜邵ｺ・ｽE・ｽ隴厄ｽｸ邵ｺ蟠趣ｽｾ・ｽE・ｽ郢ｧ・ｽ雋・・ｽ・ｽ・ｽE・ｽE
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	SetFontSize(20);
	int animationCounter = 0;
	int textureCurrentNum = 0;

	//郢ｧ・ｽE・ｽ郢晢ｽｼ郢晢ｿｽ邵ｺ・ｽE・ｽ郢晢ｽ｡郢ｧ・ｽE・ｽ郢晢ｽｳ郢晢ｽｫ郢晢ｽｼ郢晢ｿｽE
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//騾包ｽｻ鬮ｱ・ｽE・ｽ郢ｧ雋橸ｿｽE隴帶ｺｷ蝟ｧ邵ｺ蜷ｶ・ｽE・ｽE
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


		
		


		//髯ｬ蜀怜愛鬮ｱ・ｽE・ｽ邵ｺ・ｽE・ｽ・ｽ繝ｻ・ｽE・ｽ・ｽE・ｽ郢ｧ螳夲ｽ｡・ｽE・ｽ騾包ｽｻ鬮ｱ・ｽE・ｽ邵ｺ・ｽE・ｽ隴擾ｿｽ邵ｺ繝ｻ
		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}
		ScreenFlip();

		//繝ｻ謇假ｽｼ蜉ｱﾎ醍ｹ晢ｽｪ驕俶慣・ｽE・ｽ閧ｲ・ｽE・ｽ蜻茨ｿｽE鬮｢讌｢・ｽE・ｽ・ｽE・ｽ繝ｻ蜴・・ｽ・ｽ闊後Ψ郢晢ｽｬ郢晢ｽｼ郢晢ｿｽ邵ｺ・ｽE・ｽ邵ｺ・ｽE・ｽ邵ｺ貅倪夢陜｣・ｽE・ｽ陷ｷ闌ｨ・ｽE・ｽ莉｣繝ｵ郢晢ｽｬ郢晢ｽｼ郢晢ｿｽ陟冶侭笳・・ｽ・ｽ鄙ｫ繝ｻ驍ｨ遒≫с隴弱ｋ菫｣
		//繝ｻ謇假ｽｼ蜉ｱﾎ醍ｹ晢ｽｪ驕俶慣・ｽE・ｽ閧ｲ・ｽE・ｽ蜻茨ｿｽE鬮｢讌｢・ｽE・ｽ・ｽE・ｽ繝ｻ蜴・・ｽ・ｽ闊後Ψ郢晢ｽｬ郢晢ｽｼ郢晉ｸｺ邵ｺ・ｽE・ｽ邵ｺ貅倪夢陜｣・ｽE・ｽ陷ｷ闌ｨ・ｽE・ｽ莉｣繝ｵ郢晢ｽｬ郢晢ｽｼ郢晢ｿｽ阮吮浴E・ｽ・ｽ鄙ｫ繝ｻ驍ｨ遒≫с隴弱ｋ菫｣
		//繝ｻ謇假ｽｼ蜉ｱﾎ醍ｹ晢ｽｪ驕俶慣E閧ｲE蜻・鬮｢讌｢EE繝ｻ蜴・闊後Ψ郢晢ｽｬ郢晢ｽｼ郢晉ｸｺE邵ｺE邵ｺ貅倪夢陜｣E陷ｷ闌ｨE莉｣繝ｵ郢晢ｽｬ郢晢ｽｼ郢晁冶侭笳・鄙ｫ繝ｻ驍ｨ遒≫с隴弱ｋ菫｣
		//繝ｻ謇假ｽｼ蜉ｱﾎ醍ｹ晢ｽｪ驕俶慣E閧ｲE蜻・鬮｢讌｢EE繝ｻ蜴・闊後Ψ郢晢ｽｬ郢晢ｽｼ郢晉ｸｺ邵ｺE邵ｺ貅倪夢陜｣E陷ｷ闌ｨE莉｣繝ｵ郢晢ｽｬ郢晢ｽｼ郢晁侭笳・鄙ｫ繝ｻ驍ｨ遒≫с隴弱ｋ菫｣
		//驍ｨ遒≫с邵ｺ蜷ｶE邵ｺE邵ｺE邵ｺ阮呻ｼ・E陟髭笆ｽ
		while (GetNowCount() - time < 17)
		{
			//陟髭笆ｽ邵ｺ邵ｺ莉｣竊醍ｸｺE邵ｺE邵ｺ阮呻ｼ・E邵ｺE闖ｴ霈費ｽりｭ厄ｽｸ邵ｺ荵戲邵ｺ繝ｻ
		}

		//陷台ｼ∝求邵ｺ蜷ｶE繝ｻE竏哘邵ｺ繧・郢ｧ
		if (GetASyncLoadNum() == 0)
		{
			//蜑企勁縺吶ｋ繝輔Λ繧ｰ縺後≠繧九が繝悶ず繧ｧ繧ｯ繝医′縺ゅｌ縺ｰ蜑企勁縺吶ｋ
			Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
			Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
			ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();

			//繝ｫ繝ｼ繝励・逶ｴ蜑阪↓繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ繧偵＞繧後※縺翫￥
			Master::mpSceneManager->ChangeSceneIfNeeded();
		}
		

		
	}
	//驍ｨ繧・繝ｻ繝ｻ騾・E
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



	// DX郢晢ｽｩ郢ｧ・ｽE・ｽ郢晄じﾎ帷ｹ晢ｽｪ闖ｴ・ｽE・ｽ騾包ｽｨ邵ｺ・ｽE・ｽ驍ｨ繧・・ｽ・ｽ繝ｻ
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	// 郢ｧ・ｽE・ｽ郢晁ｼ斐Κ邵ｺ・ｽE・ｽ驍ｨ繧・・ｽ・ｽ繝ｻ
	return 0;
}




