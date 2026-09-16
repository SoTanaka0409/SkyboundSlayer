#include "DxLib.h"
#include "Config.h"
#include "Texture.h"
#include "Collision.h"
#include "SceneManager.h"
#include "Master.h"
#include "ObjectManager.h"
#include "GameScene.h"
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

// サービスロケーターとしてのMasterクラス（静的メンバ変数）の実体化
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

int Master::hit_stop_timer_ = 0;
bool Master::is_pause_on_ = false;
bool Master::is_stat_shop_on_ = false;
bool Master::is_safe_point_on_ = false;
bool Master::is_near_shop_on_ = false;
bool Master::is_save_ = false;
bool Master::is_cutscene_playing_ = false;
int Master::game_clear_count_ = 0;

/// @brief Windowsアプリケーションのエントリーポイント（メイン関数）
/// @param hInstance アプリケーションの現在インスタンスのハンドル
/// @param hPrevInstance 以前のインスタンスのハンドル（常にNULL）
/// @param lpCmdLine アプリケーションのコマンドライン引数
/// @param nCmdShow ウィンドウの表示状態を指定するフラグ
/// @return int WinMainの終了コード（0で正常終了、-1で初期化エラー）
/// @details DxLibの初期化、3D環境光・ライティング設定、メインループの制御（ロード監視・更新・描画・60FPS同期）、破棄予約オブジェクトの安全な解放処理を行う
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	SetGraphMode(Config::ScreenWidth, Config::ScreenHeight, 32);
	SetWindowSize(Config::ScreenWidth, Config::ScreenHeight);

	SetDoubleStartValidFlag(TRUE);
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_SHIFTJIS);
	SetMainWindowText("Skybound Slayer");
	if (DxLib_Init() == -1) return -1;

	// 学生っぽさを消すため、高品位なフォントとアンチエイリアスを使用する
	ChangeFont("メイリオ");
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);

	EffekseerManager::GetInstance()->Init();

	// 環境光の設定：3D空間全体を均一に照らし、暗部でのモデル視認性を確保するためのベースライト設定
	SetLightEnable(TRUE);
	SetLightAmbColor(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));
	SetLightDirection(VGet(-1.0f, -1.0f, 1.0f));
	SetLightDifColor(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));

	// 非同期読み込み：大量のアセットを抱える本編において、ロード中のフリーズを防ぐため非同期ロードを必須とする
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

		// ローディング監視：ロード完了までゲーム進行（Update）をロックし、遷移時の不整合を防止
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

		// アーキテクチャ設計：高リフレッシュレート環境でも物理演算が崩壊しないよう、デルタタイムに頼らず固定17ms（約60FPS）ウエイトで同期を確保
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
	delete Master::debug_;
	delete Master::draw_hp_;
	delete Master::enemy_manager_;
	delete Master::item_manager_;
	delete Master::inf_class_manager_;
	delete Master::buff_manager_;
	delete Master::chat_;
	delete Master::save_;

	ColliderManager::GetInstance()->Finalize();
	EffekseerManager::GetInstance()->End();
	DxLib_End();

	return 0;
}