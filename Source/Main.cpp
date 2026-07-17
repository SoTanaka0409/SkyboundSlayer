
#include "DxLib.h"
#include "Config.h"
#include"Texture.h"
//#include"Texture is_animation_.h"
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
// Blender Model Download Reference:
// https://www.d5render.com/ja/workflow/blender?utm_campaign=bingsearchILJPblender&utm_source=bing&utm_medium=cpc&msclkid=929170cec1521e953f1c187910ba1cae

Player3D* Master::player_ = nullptr;
SceneManager* Master::scene_manager_ = new SceneManager();
SoundManager* Master::sound_manager_ = new SoundManager();
ResourceManager* Master::resource_manager_ = new ResourceManager();
Debug* Master::debug_ = new Debug();
DrawHp* Master::draw_hp_ = new DrawHp();
Camera* Master::camera_ = new Camera();
ScoreManager* Master::score_manager_ = new ScoreManager(0);
EnemyManager* Master::enemy_manager_ = new EnemyManager();
ItemManager* Master::item_manager_ = new ItemManager();
InfClassManager* Master::inf_class_manager_ = new InfClassManager();
BuffManager* Master::buff_manager_ = new BuffManager();
Chat* Master::chat_ = new Chat();
Save* Master::save_ = new Save();



bool Master::is_pause_on_ = false;
bool Master::is_stat_shop_on_ = false;
bool Master::is_safe_point_on_ = false;
bool Master::is_near_shop_on_ = false;
bool Master::is_save_ = false;
bool Master::is_cutscene_playing_ = false;


int Master::game_clear_count_ = 0;




/*
 * 目的（ゲームのメインループを実行し、各種マネージャーの更新と描画を行うため）
 * [入力] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
 * [出力] int: 終了コード
 * [副作用] ウィンドウの生成、メインループの実行、各マネージャーのメモリ確保と解放
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウモードで起動する
	ChangeWindowMode(true);

	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	
	// DxLibライブラリの初期化
	SetDoubleStartValidFlag(TRUE);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	EffekseerManager::GetInstance()->Init();

	// 3D空間のライト設定を有効にする
	SetLightEnable(TRUE);
	// 環境光（アンビエントライト）の色を設定?E?E??E?E?E・・・mbColor・峨?E?E?E??E???E?E鬮倥?E?E?E?縺?E?E?E?險?E?E?E?螳壹?E?E縲∝ｽ?E?E?E?縺?E?E?E?縺?E?E?E?繧翫?E?E?E?E?E??E?吶?E?E?E?驛ｨ蛻・?E?E?E?亥?E?E?E?暮擁E?E??E?・・髱?E?E?E?・峨′逵溘▲鮟?E↓豐医∪縺?E?E?E?縺・?E?E?E?縺・↓縺吶?E?E?E?E
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	// 平行光源の向きを設定
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	// 平行光源のディフューズ（拡散光）の色を設定
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	


	// BGMの読み込み（SoundManager内で行う）?E?E?

	// 非同期読み込み設定とサウンドマネージャーの初期化
	SetUseASyncLoadFlag(TRUE);
	Master::sound_manager_->Initialize();// すべてのサウンドを読み込む

	// シーンマネージャーの初期化
	Master::scene_manager_->Initialize();

	Master::score_manager_->Initialize();

	// カメラマネージャーの初期化??E?
	Master::camera_->Initialize();

	



	// 描画先を裏画面に設定
	SetDrawScreen(DX_SCREEN_BACK);

	// Zバッファを有効にする?E?E?譖ｸ縺崎ｾ?E?E?E?繧?E??E?E??E??E?E?E?E
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	SetFontSize(20);
	int animationCounter = 0;
	int textureCurrentNum = 0;

	// ゲームのメインループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		// 画面をクリアする?E?E
		ClearDrawScreen();
		int time = GetNowCount();

		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}
		else
		{
				Master::draw_hp_->Update();
				Master::camera_->Update();
		
				Master::scene_manager_->Update();
				Master::inf_class_manager_->Update();
				EffekseerManager::GetInstance()->Update();
				EffectPool::GetInstance()->Update();
		}
		
		Master::scene_manager_->Draw();
		Master::score_manager_->Draw();
		EffekseerManager::GetInstance()->Draw();
		EffectPool::GetInstance()->Draw();


		
		


		// 裏画面の描画内容を表画面に反映
		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}
		ScreenFlip();

		// 60FPSを維持するための待機処理（1フレーム約16.6ms）
		while (GetNowCount() - time < 17)
		{
			// 時間が経過するまで待機
		}

		// 非同期読み込みが完了している場合のみ、削除やシーン遷移を行う
		if (GetASyncLoadNum() == 0)
		{
			//削除するフラグがあるオブジェクトがあれば削除する
			Master::scene_manager_->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
			Master::scene_manager_->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
			ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();

			//ループ?E直前にシーン遷移をいれておく
			Master::scene_manager_->ChangeSceneIfNeeded();
		}
		

		
	}
	// ゲーム終了前の各マネージャーの解放処理
	Master::scene_manager_->Finalize();
	delete Master::scene_manager_;
	Master::sound_manager_->Finalize();
	delete Master::sound_manager_;
	Master::score_manager_->Finalize();
	delete Master::score_manager_;
	Master::camera_->Finalize();
	delete Master::camera_;
	delete Master::resource_manager_;

	ColliderManager::GetInstance()->Finalize();



	// EffekseerとDxLibの終了処理
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	// プログラムを正常終了する
	return 0;
}





