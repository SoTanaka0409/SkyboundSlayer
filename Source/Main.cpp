#include "DxLib.h"
#include "Config.h"
#include "Texture.h"
#include "Collision.h"
#include "SceneManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene3D.h"
#include "Player3D.h"
#include "Camera.h"
#include "ScoreManager.h"
#include "ResourceManager.h"
#include "Debug.h"
#include "DrawHp.h"
#include "EffekseerManager.h"
#include "EnemyManager.h"
#include "ColliderManager.h"
#include "EffectPool.h"
#include "ItemManager.h"
#include "InfClassManager.h"
#include "InfClass.h"
#include "BuffManager.h"
#include "Chat.h"
#include "Save.h"

// サービスロケーターとしてのマスタークラスの静的実体化
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	SetDoubleStartValidFlag(TRUE);
	if (DxLib_Init() == -1) return -1;

	EffekseerManager::GetInstance()->Init();

	// 環境光設定：3D空間全体を均一に照らし、暗部でのモデル視認性を確保するためのベースライト設定
	SetLightEnable(TRUE);
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));

	// 非同期読み込み：大量のアセットを抱える本作において、ロード中のフリーズを防ぐため非同期ロードを必須とする
	SetUseASyncLoadFlag(TRUE);
	Master::sound_manager_->Initialize();
	Master::scene_manager_->Initialize();
	Master::score_manager_->Initialize();
	Master::camera_->Initialize();

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);
	SetFontSize(20);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();
		int time = GetNowCount();

		// ローディング監視：ロード完了までゲーム進行（Update）をロックし、遷移の不整合を防止
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

		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}

		ScreenFlip();

		// アーキテクチャ設計：高リフレッシュレート環境でも物理演算が崩壊しないよう、デルタタイムに頼らず固定の17ms（約60FPS）ウェイトで同期を担保
		while (GetNowCount() - time < 17) {}

		// シーン遷移とメモリ掃除：メインループの最後で削除予約されたリソースをまとめて解放することで、Update/Draw中の不正アクセスを防ぐ
		if (GetASyncLoadNum() == 0)
		{
			Master::scene_manager_->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
			Master::scene_manager_->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
			ColliderManager::GetInstance()->DeleteAllColliderIfNeeded();
			Master::scene_manager_->ChangeSceneIfNeeded();
		}
	}

	// 終了処理：マネージャーのFinalizeとインスタンス削除
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
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	return 0;
}