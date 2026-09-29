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

/// @brief サービスロケーターとしてのMasterクラス（静的メンバ変数）の実体化
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
bool Master::is_quit_confirm_ = false;
int  Master::quit_confirm_timer_ = 0;

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
	AddFontFile("Resource/Genkai-Mincho-font/genkai-mincho.ttf");
	ChangeFont("源界明朝");
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

	static int prevEsc = 0;
	bool shouldQuit = false;

	while (ProcessMessage() == 0 && !shouldQuit)
	{
		ClearDrawScreen();
		int time = GetNowCount();

		// ESCキーの立ち上がりを検知
		int currentEsc = CheckHitKey(KEY_INPUT_ESCAPE);
		bool escPressed = (currentEsc && !prevEsc);
		prevEsc = currentEsc;

		// ESC終了確認ロジック
		if (Master::is_quit_confirm_)
		{
			// 1秒ロック中はESCを受け付けない（連打防止）
			if (Master::quit_confirm_timer_ > 0)
			{
				Master::quit_confirm_timer_--;
			}
			else if (escPressed)
			{
				// ロック解除後にESC → ゲーム終了
				shouldQuit = true;
			}

			// マウス左クリック or 右クリックでキャンセル
			bool mouseCancel = (GetMouseInput() & (MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT)) != 0;
			if (mouseCancel)
			{
				Master::is_quit_confirm_ = false;
				Master::quit_confirm_timer_ = 0;
			}
		}
		else
		{
			if (escPressed)
			{
				// はじめてESC → 確認モード開始・ゲーム一時停止
				Master::is_quit_confirm_ = true;
				Master::quit_confirm_timer_ = 60; // 1秒ロック (60fps)
			}
		}

		// ローディング監視：ロード完了までゲーム進行（Update）をロックし、遷移時の不整合を防止
		// 確認ダイアログ中はゲームを停止する
		if (GetASyncLoadNum() > 0)
		{
			DrawFormatString(600, 360, GetColor(255, 255, 255), "NOW LOADING... %d", GetASyncLoadNum());
		}
		else if (!Master::is_quit_confirm_)
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

		// ESC終了確認ダイアログ描画
		if (Master::is_quit_confirm_)
		{
			bool isLocked = (Master::quit_confirm_timer_ > 0);

			// 半透明オーバーレイ
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
			DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// ダイアログパネル
			const int px = 530, py = 390, pw = 900, ph = 240;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
			DrawBox(px, py, px + pw, py + ph, GetColor(12, 15, 22), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawLine(px,      py,      px + pw, py,      GetColor(218, 178, 86), 2);
			DrawLine(px,      py + ph, px + pw, py + ph, GetColor(98, 73, 32),   2);
			DrawLine(px,      py,      px,      py + ph, GetColor(98, 73, 32),   2);
			DrawLine(px + pw, py,      px + pw, py + ph, GetColor(218, 178, 86), 2);

			// タイトルテキスト
			SetFontSize(52);
			DrawFormatString(px + 18, py + 18, GetColor(0, 0, 0),       "QUIT GAME?");
			DrawFormatString(px + 14, py + 14, GetColor(255, 220, 100), "QUIT GAME?");

			// ロック中は「しばらくお待ちください」、解除後は「もう一度ESC」を表示
			SetFontSize(28);
			if (isLocked)
			{
				DrawFormatString(px + 30, py + 100, GetColor(160, 160, 160), "しばらくお待ちください...");
			}
			else
			{
				DrawFormatString(px + 30, py + 100, GetColor(255, 240, 160), "もう一度 [ESC] で終了します");
			}

			// マウスクリックでキャンセル（常に表示・大きく目立たせる）
			SetFontSize(26);
			DrawFormatString(px + 30, py + 150, GetColor(0, 0, 0),       "[ マウスクリック ] でキャンセル");
			DrawFormatString(px + 27, py + 147, GetColor(100, 210, 255), "[ マウスクリック ] でキャンセル");
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