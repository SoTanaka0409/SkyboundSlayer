
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

#include"EnemyManager.h"
#include"ColliderManager.h"
#include"Inventory.h"
#include"ItemManager.h"
#include"InfClassManager.h"
#include"InfClass.h"
#include"BuffManager.h"
#include"Chat.h"
#include"Save.h"
/**
* @note リファレンス https://dxlib.xsrv.jp/dxfunc.html
*/

//ゲームの３Dモデル   metaseq316
//https://www.d5render.com/ja/workflow/blender?utm_campaign=bingsearchILJPblender&utm_source=bing&utm_medium=cpc&msclkid=929170cec1521e953f1c187910ba1cae



/**
/**
* @fn WinMain
* @brief Main関数
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 正常終了／-1 エラー
* @details Main関数
*/

//Masterクラスの静的メンバ変数定義
SceneManager* Master::mpSceneManager = new SceneManager();//呼び出し
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
bool Master::InventoryClasOn = false;//inventory中はsceneを止める
bool Master::PauseOn = false;
bool Master::ShopClassOn = false;
bool Master::StatShopClassOn = false;
bool Master::SafePointOn = false;
bool Master::NearShopOn = false;
bool Master::mbSave = false;


int Master::GameClearCount = 0;


//Camera1* Master::mpCamerafa = new Camera1();


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(true);

	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	
	// DXライブラリ初期化
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// --- 全体のライティング（環境光・太陽光）設定 ---
	SetLightEnable(TRUE);
	// 環境光（AmbColor）を少し高めに設定し、影になりやすい部分（底面や側面）が真っ黒に沈まないようにする
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	// 太陽光（ディレクショナルライト）の向きを斜め下に向ける
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	// 太陽光の色（少し白みがかった明るい色）
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	// ------------------------------------------------


	//BGMの読み込み

	//サウンドマネージャーの初期化
	Master::mpSoundManager->Initialize();//すべてのサウンドが読み込まれru----

	//シーンマネージャーの生成と初期化
	Master::mpSceneManager->Initialize();

	Master::mpScoreManager->Initialize();

	//カメラの更新
	Master::mpCamera->Initialize();

	



	//描画先設定を裏画面に設定する
	SetDrawScreen(DX_SCREEN_BACK);

	//Zバッファに書き込む準備
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	SetFontSize(20);
	int animationCounter = 0;
	int textureCurrentNum = 0;
	//Player3D* player = new Player3D("Resource/3D/Hero.mv1", VGet(400.0f, 200.0f, 100.0f));

	//ゲームのメインループ
	//ProcessMesseage()==0^>ウインドウのxボタンが押されていないかどうか
	//CheckHiyKey(KEY_INPUT_ESCAPE)==0->エスケープキーが押されているかどうか
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//画面を初期化する
		ClearDrawScreen();
		int time = GetNowCount();

		Master::mpDrawHp->Update();
		Master::mpCamera->Update();
		
		Master::mpInfClassManager->Update();
		
		

		//更新
		Master::mpSceneManager->Update();
	
		//player->Update();
		//描画
		Master::mpSceneManager->Draw();
		Master::mpScoreManager->Draw();


		
		
		//player->Draw();


		//裏画面の内容を表画面に映す
		ScreenFlip();

		//１７ミリ秒（秒数間訳６０フレームだったっ場合１フレーム当たりの経過時間
		//経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
			//待つだけなのでここには何も書かない
		}

		//削除する必要のあるオブジェクトがあれば削除する
		ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
		

		//ループする直前にシーン遷移チェックを入れておく
		Master::mpSceneManager->ChangeSceneIfNeeded();

		
	}
	//終了処理
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



	// DXライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}



