#include "GameManager.h"
#include "Player3D.h"
#include "Enemy.h"
#include "StatShop.h"
#include <cmath>
#include <DxLib.h>
#include "Stage.h"
#include "StageObject.h"
#include "Tree.h"
#include "InputManager.h"
#include "EffekseerObject.h"
#include "DamageUI.h"

namespace
{
	/// @brief デバッグ用ボタンの領域情報を保持する構造体
	struct DebugButton
	{
		int x;           ///< ボタン左上X座標
		int y;           ///< ボタン左上Y座標
		int w;           ///< ボタンの幅
		int h;           ///< ボタンの高さ
		const char* label; ///< ボタンに表示するラベル文字列
	};

	/// @brief マウスカーソルがデバッグボタンの領域内にあるか判定する
	/// @param button 判定対象のボタン構造体
	/// @param mouseX マウスのX座標
	/// @param mouseY マウスのY座標
	/// @return bool ボタン領域内にある場合はtrue
	bool IsMouseInButton(const DebugButton& button, int mouseX, int mouseY)
	{
		return mouseX >= button.x &&
			mouseX <= button.x + button.w &&
			mouseY >= button.y &&
			mouseY <= button.y + button.h;
	}

	/// @brief デバッグ用ボタンを描画する
	/// @param button 描画対象のボタン構造体
	/// @param hover マウスホバー中かどうかのフラグ
	void DrawDebugButton(const DebugButton& button, bool hover)
	{
		const int bg = hover ? GetColor(55, 46, 32) : GetColor(24, 23, 26);
		const int edge = hover ? GetColor(235, 188, 82) : GetColor(116, 86, 40);
		const int text = hover ? GetColor(255, 238, 164) : GetColor(230, 216, 184);

		DrawBox(button.x, button.y, button.x + button.w, button.y + button.h, bg, true);
		DrawBox(button.x, button.y, button.x + button.w, button.y + button.h, edge, false);
		DrawFormatString(button.x + 14, button.y + 10, text, "%s", button.label);
	}
}

/// @brief GameManagerのコンストラクタ
/// @param enemyManager 敵オブジェクトの管理を担当するマネージャーポインタ
/// @param diff ゲームの難易度設定
/// @details 各種変数の初期化、フェーズ1の開始およびポータル・エフェクトの生成を行う
GameManager::GameManager(EnemyManager* enemyManager, Difficulty diff)
	: enemy_manager_(enemyManager), difficulty_(diff), current_phase_(Phase::kPhase1), shop_timer_(0), fade_alpha_(0), boss_portal_pos_(VGet(0, 0, 0)), boss_cutscene_timer_(0), cutscene_start_pos_(VGet(0, 0, 0))
{
	// 最初のウェーブの敵を生成
	SpawnPhaseEnemies();

	// ゲーム開始時にプレイヤーの反対側にボス用のポータル座標を計算して初期化
	VECTOR playerStartPos = VGet(-1200.0f, 20.0f, -1000.0f);
	VECTOR center = Config::GetStageCenter();
	VECTOR dir = VSub(playerStartPos, center);
	dir.y = 0.0f;
	if (VSize(dir) < 1.0f) dir = VGet(0.0f, 0.0f, 1.0f);
	else dir = VNorm(dir);
	boss_portal_pos_ = VAdd(center, VScale(dir, -5000.0f));

	// ポータルの土台モデルを生成
	float portalSize = 100.0f;
	new Stage(VAdd(boss_portal_pos_, VGet(0.0f, -570.0f, 0.0f)), "Resource/model/props/portal/01_portal.mv1", "Resource/model/props/portal/01_portal.mv1", VGet(portalSize, portalSize, portalSize));

	// ポータル上の魔法陣エフェクトを生成
	new EffekseerObject("Mahoujin", "Resource/effect/magic/02_magic_effect_playback.efk", VAdd(boss_portal_pos_, VGet(0.0f, -565.0f, 0.0f)), nullptr, true, 1.0f, 1.0f);
}

/// @brief GameManagerのデストラクタ
GameManager::~GameManager()
{
	DamageUIManager::GetInstance()->Unload();
}

/// @brief 毎フレームの更新処理を行う
/// @details フェーズ移行、ボスカットシーン、ショップ進行、バトルフェーズの管理を統合処理する
void GameManager::Update()
{
	UpdateDebugControls();

	if (UpdateBossCutscene())
	{
		return;
	}

	if (UpdateBossFade())
	{
		return;
	}

	if (IsShopPhase())
	{
		UpdateShopPhase();
	}
	else
	{
		UpdateBattlePhase();
	}

	DamageUIManager::GetInstance()->Update();
}

/// @brief ボスカットシーン中のカメラ座標やタイマーを更新する
/// @return bool カットシーン処理中の場合はtrue、完了・非再生時はfalse
/// @details カメラの注視点移動およびイージング処理を行う
bool GameManager::UpdateBossCutscene()
{
	if (!Master::is_cutscene_playing_)
	{
		return false;
	}

	boss_cutscene_timer_++;

	float t = static_cast<float>(boss_cutscene_timer_) / 180.0f;
	if (t > 1.0f)
	{
		t = 1.0f;
	}

	float easeT = t * t * (3.0f - 2.0f * t);
	VECTOR currentPos = VAdd(VScale(cutscene_start_pos_, 1.0f - easeT), VScale(boss_portal_pos_, easeT));
	Master::camera_->SetCutsceneTarget(currentPos);

	if (boss_cutscene_timer_ > 240)
	{
		Master::is_cutscene_playing_ = false;
		Master::camera_->SetCutsceneMode(false);
	}

	return true;
}

/// @brief ボス戦前後の画面暗転・フェード処理を更新する
/// @return bool フェード処理中の場合はtrue
/// @details fade_alpha_の増減を行い、暗転完了時にボス戦マップへのワープと敵生成を実行する
bool GameManager::UpdateBossFade()
{
	if (current_phase_ == Phase::kFadeOutToBoss)
	{
		fade_alpha_ += 5;
		if (fade_alpha_ >= 255)
		{
			fade_alpha_ = 255;
			current_phase_ = Phase::kBoss;
			SpawnPhaseEnemies();

			current_phase_ = Phase::kFadeInBoss;
			Master::player_->SetPosition(VAdd(Config::GetStageBossCenter(), VGet(500.0f, 0.0f, -2000.0f)));

			if (Master::sound_manager_)
			{
				Master::sound_manager_->PlaySE(SoundManager::SE_WARP);
			}
		}

		return true;
	}

	if (current_phase_ == Phase::kFadeInBoss)
	{
		fade_alpha_ -= 5;
		if (fade_alpha_ <= 0)
		{
			fade_alpha_ = 0;
			current_phase_ = Phase::kBoss;
		}

		return true;
	}

	return false;
}

/// @brief ショップフェーズのタイマーおよび退場判定を管理する
/// @details 制限時間の減算を行い、0になったらショップNPCを退場させて次の戦闘フェーズへ移行する
void GameManager::UpdateShopPhase()
{
	if (current_phase_ == Phase::kShop3)
	{
		if (IsBossGateTouched())
		{
			SendShopsOut();
			StartBossTransition();
		}

		return;
	}

	if (AreShopsArrived())
	{
		shop_timer_--;
	}

	if (shop_timer_ > 0)
	{
		return;
	}

	SendShopsOut();

	if (current_phase_ == Phase::kShop1)
	{
		current_phase_ = Phase::kPhase2;
	}
	else if (current_phase_ == Phase::kShop2)
	{
		current_phase_ = Phase::kPhase3;
	}

	SpawnPhaseEnemies();
}

/// @brief バトルフェーズの全滅検知・フェーズ進行を管理する
/// @details フィールド上の敵が全滅した場合、ショップフェーズまたはボス戦への移行処理を開始する
void GameManager::UpdateBattlePhase()
{
	if (GetEnemyCount() > 0)
	{
		return;
	}

	if (current_phase_ == Phase::kPhase1)
	{
		StartShopPhase(Phase::kShop1);
	}
	else if (current_phase_ == Phase::kPhase2)
	{
		StartShopPhase(Phase::kShop2);
	}
	else if (current_phase_ == Phase::kPhase3)
	{
		StartShopPhase(Phase::kShop3);
		StartBossGateCutscene();
	}
	else if (current_phase_ == Phase::kBoss)
	{
		current_phase_ = Phase::kClear;
	}
}

/// @brief ショップフェーズへの移行を開始する
/// @param nextPhase 移行先のショップフェーズ（kShop1〜kShop3）
/// @details 制限時間タイマーをセットし、ショップNPCに入場を開始させる
void GameManager::StartShopPhase(Phase nextPhase)
{
	current_phase_ = nextPhase;
	shop_timer_ = 60 * 20;
	SendShopsIn();
}

/// @brief ボス戦前暗転フェード演出を開始する
/// @details フェーズをフェードアウト状態にし、アルファ値を初期化する
void GameManager::StartBossTransition()
{
	current_phase_ = Phase::kFadeOutToBoss;
	fade_alpha_ = 0;
}

/// @brief ボスゲート出現時のカメラカットシーンを開始する
/// @details プレイヤーの操作を制限し、ゲート位置へカメラを向けるカットシーンフラグを有効化する
void GameManager::StartBossGateCutscene()
{
	Master::is_cutscene_playing_ = true;
	boss_cutscene_timer_ = 0;
	cutscene_start_pos_ = Master::player_->GetPosition();
	Master::camera_->SetCutsceneMode(true);
}

/// @brief ショップNPC群をフィールド内へ入場させる
void GameManager::SendShopsIn()
{
	const auto& shops = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
	for (auto s : shops)
	{
		StatShop* shop = s->CastTo<StatShop>();
		if (shop)
		{
			shop->StartWalkingIn();
		}
	}
}

/// @brief ショップNPC群をフィールドから退場させる
void GameManager::SendShopsOut()
{
	const auto& shops = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
	for (auto s : shops)
	{
		StatShop* shop = s->CastTo<StatShop>();
		if (shop)
		{
			shop->StartWalkingOut();
		}
	}
}

/// @brief 全てのショップNPCが所定の目標位置へ到着したか判別する
/// @return bool 全て到着していればtrue、移動中があればfalse
bool GameManager::AreShopsArrived() const
{
	const auto& shops = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
	for (auto s : shops)
	{
		StatShop* shop = s->CastTo<StatShop>();
		if (shop && !shop->IsArrived())
		{
			return false;
		}
	}

	return true;
}

/// @brief 現在のフェーズがショップフェーズ（1〜3）かどうか判定する
/// @return bool ショップフェーズであればtrue
bool GameManager::IsShopPhase() const
{
	return current_phase_ == Phase::kShop1 || current_phase_ == Phase::kShop2 || current_phase_ == Phase::kShop3;
}

/// @brief 現在ボス戦への暗転フェード中（フェードイン/アウト）かどうか判定する
/// @return bool フェード中であればtrue
bool GameManager::IsBossFadePhase() const
{
	return current_phase_ == Phase::kFadeOutToBoss || current_phase_ == Phase::kFadeInBoss;
}

/// @brief プレイヤーがボスワープ用のポータルに接触したか判定する
/// @return bool 接触範囲内（150 unit以内）にいればtrue
bool GameManager::IsBossGateTouched() const
{
	if (!Master::player_)
	{
		return false;
	}

	Player3D* player = Master::player_->CastTo<Player3D>();
	if (!player)
	{
		return false;
	}

	float dist = VSize(VSub(player->GetPosition(), boss_portal_pos_));
	return dist < 150.0f;
}

/// @brief デバッグ操作が有効化されているか判定する
/// @return bool デバッグフラグが真であればtrue
bool GameManager::IsDebugControlsEnabled() const
{
	return Master::debug_ != nullptr && Master::debug_->Getdebug();
}

/**
 * @brief 開発・デバッグ用の専用入力・キー操作処理
 * @details デバッグフラグが有効な場合のみ、F5（全滅）やF6（ボス即時移行）などのショートカットやUI操作を許可する
 */
void GameManager::UpdateDebugControls()
{
	if (!IsDebugControlsEnabled())
	{
		return;
	}

	int mouseX = 0;
	int mouseY = 0;
	InputManager::GetMousePos(mouseX, mouseY);

	const DebugButton killButton = { 28, 104, 164, 40, "KILL ENEMIES" };
	const DebugButton bossButton = { 202, 104, 132, 40, "GO BOSS" };
	const bool clicked = InputManager::CheckMouseClickLeft() != 0;

	if ((clicked && IsMouseInButton(killButton, mouseX, mouseY)) ||
		InputManager::CheckDownKey(KEY_INPUT_F5) != 0)
	{
		DebugKillEnemies();
	}

	if ((clicked && IsMouseInButton(bossButton, mouseX, mouseY)) ||
		InputManager::CheckDownKey(KEY_INPUT_F6) != 0)
	{
		DebugGoBoss();
	}
}

/// @brief デバッグ機能：フィールド上の全ての敵を即座に死亡させる
void GameManager::DebugKillEnemies()
{
	if (Master::scene_manager_ == nullptr ||
		Master::scene_manager_->GetCurrentScene() == nullptr ||
		Master::scene_manager_->GetCurrentScene()->GetObjectManager() == nullptr)
	{
		return;
	}

	const auto enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	for (auto obj : enemies)
	{
		if (obj == nullptr || obj->IsDeleteFlag()) continue;

		Enemy* enemy = obj->CastTo<Enemy>();
		if (enemy == nullptr || enemy->IsDead()) continue;

		enemy->Damage(999999.0f);
	}
}

/// @brief デバッグ機能：強制的にボス戦フェーズへ移行する
void GameManager::DebugGoBoss()
{
	if (current_phase_ == Phase::kBoss ||
		current_phase_ == Phase::kFadeOutToBoss ||
		current_phase_ == Phase::kFadeInBoss)
	{
		return;
	}

	Master::is_cutscene_playing_ = false;
	if (Master::camera_)
	{
		Master::camera_->SetCutsceneMode(false);
	}

	if (Master::scene_manager_ == nullptr ||
		Master::scene_manager_->GetCurrentScene() == nullptr ||
		Master::scene_manager_->GetCurrentScene()->GetObjectManager() == nullptr)
	{
		return;
	}

	const auto enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	for (auto obj : enemies)
	{
		if (obj == nullptr || obj->IsDeleteFlag()) continue;

		Enemy* enemy = obj->CastTo<Enemy>();
		if (enemy != nullptr)
		{
			enemy->Delete();
		}

		obj->SetDeleteFlag(true);
	}

	SendShopsOut();
	StartBossTransition();
}

/// @brief GameManagerに関連するHUD・ミニマップ等の描画を一括で行う
void GameManager::Draw()
{
	DrawPhaseHud();
	DrawMinimap();
	DrawShopBanner();
	DrawBossFade();
	DrawDebugPanel();
	DamageUIManager::GetInstance()->Draw();
}

/// @brief デバッグ用操作パネルおよびボタンを画面上に描画する
void GameManager::DrawDebugPanel()
{
	if (!IsDebugControlsEnabled())
	{
		return;
	}

	int mouseX = 0;
	int mouseY = 0;
	InputManager::GetMousePos(mouseX, mouseY);

	const int panelX = 22;
	const int panelY = 96;
	const int panelW = 318;
	const int panelH = 56;
	const DebugButton killButton = { 28, 104, 164, 40, "KILL ENEMIES" };
	const DebugButton bossButton = { 202, 104, 132, 40, "GO BOSS" };

	static int font18 = -1;
	if (font18 == -1) font18 = CreateFontToHandle(NULL, 18, -1);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 165);
	DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(92, 68, 30), false);
	DrawFormatStringToHandle(panelX + 8, panelY - 18, GetColor(190, 170, 120), font18, "DEBUG  F5 / F6");

	DrawDebugButton(killButton, IsMouseInButton(killButton, mouseX, mouseY));
	DrawDebugButton(bossButton, IsMouseInButton(bossButton, mouseX, mouseY));
}

/// @brief 現在の進行フェーズに対応するメイン表示テキストを取得する
/// @return const char* フェーズ名文字列（"PHASE 1", "SHOP" など）
const char* GameManager::GetPhaseLabel() const
{
	switch (current_phase_)
	{
	case Phase::kPhase1:
		return "PHASE 1";
	case Phase::kPhase2:
		return "PHASE 2";
	case Phase::kPhase3:
		return "PHASE 3";
	case Phase::kBoss:
		return "BOSS";
	case Phase::kShop1:
	case Phase::kShop2:
		return "SHOP";
	case Phase::kShop3:
		return "BOSS GATE";
	case Phase::kClear:
		return "CLEAR";
	default:
		return "READY";
	}
}

/// @brief 現在の進行フェーズに対応するサブ説明テキストを取得する
/// @return const char* サブ説明文字列（"HUNT ALL", "PREPARE NEXT WAVE" など）
const char* GameManager::GetPhaseSubLabel() const
{
	switch (current_phase_)
	{
	case Phase::kPhase1:
	case Phase::kPhase2:
		return "HUNT ALL";
	case Phase::kPhase3:
		return "BOSS GATE SOON";
	case Phase::kBoss:
		return "FINAL BATTLE";
	case Phase::kShop1:
	case Phase::kShop2:
		return "PREPARE NEXT WAVE";
	case Phase::kShop3:
		return "ENTER TELEPORTER";
	case Phase::kClear:
		return "QUEST COMPLETE";
	default:
		return "";
	}
}

/// @brief フィールド上に生存している敵の現在の総数を取得する
/// @return int 敵の残り生存数
int GameManager::GetEnemyCount() const
{
	const auto& enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	return static_cast<int>(enemies.size());
}

/// @brief 画面右上にフェーズ名および敵残数を表示するHUDを描画する
void GameManager::DrawPhaseHud()
{
	static int font24 = -1;
	if (font24 == -1) font24 = CreateFontToHandle(NULL, 24, -1);

	const int panelX = Config::ScreenWidth - 356;
	const int panelY = 28;
	const int panelW = 328;
	const int panelH = 82;
	const int panel = GetColor(18, 17, 20);
	const int panelLight = GetColor(46, 42, 45);
	const int gold = GetColor(198, 154, 64);
	const int goldDark = GetColor(98, 73, 32);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 185);
	DrawBox(panelX - 6, panelY - 4, panelX + panelW + 6, panelY + panelH + 6, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, panel, true);
	DrawBox(panelX + 6, panelY + 6, panelX + panelW - 6, panelY + 13, panelLight, true);
	DrawLine(panelX, panelY, panelX + panelW, panelY, gold, 1);
	DrawLine(panelX, panelY + panelH, panelX + panelW, panelY + panelH, goldDark, 1);
	DrawLine(panelX, panelY, panelX, panelY + panelH, goldDark, 1);
	DrawLine(panelX + panelW, panelY, panelX + panelW, panelY + panelH, gold, 1);
	
	DrawFormatStringToHandle(panelX + 18, panelY + 19, GetColor(245, 226, 174), font24, "%s", GetPhaseLabel());
	DrawFormatStringToHandle(panelX + 18, panelY + 48, GetColor(205, 210, 216), font24, "%s", GetPhaseSubLabel());
	DrawFormatStringToHandle(panelX + 230, panelY + 48, GetColor(238, 238, 238), font24, "x%02d", GetEnemyCount());
}

/// @brief ショップフェーズ中のタイマー・アナウンス用上部バナーを描画する
void GameManager::DrawShopBanner()
{
	if (!IsShopPhase())
	{
		return;
	}

	static int font28 = -1;
	if (font28 == -1) font28 = CreateFontToHandle(NULL, 28, -1);

	const int bannerW = 700;
	const int bannerX = Config::ScreenWidth / 2 - bannerW / 2;
	const int bannerY = 26;
	const int gold = GetColor(198, 154, 64);
	const int goldDark = GetColor(98, 73, 32);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	DrawBox(bannerX, bannerY, bannerX + bannerW, bannerY + 54, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawLine(bannerX, bannerY, bannerX + bannerW, bannerY, gold, 1);
	DrawLine(bannerX, bannerY + 54, bannerX + bannerW, bannerY + 54, goldDark, 1);

	if (current_phase_ != Phase::kShop3)
	{
		int seconds = shop_timer_ / 60;
		DrawFormatStringToHandle(bannerX + 210, bannerY + 14, GetColor(255, 238, 156), font28, "NEXT WAVE IN %d", seconds);
	}
	else
	{
		DrawFormatStringToHandle(bannerX + 108, bannerY + 14, GetColor(141, 239, 255), font28, "ENTER THE BLUE TELEPORTER TO START BOSS");
	}
}

/// @brief ボス戦遷移時のブラックアウト・暗転演出を描画する
void GameManager::DrawBossFade()
{
	if (!IsBossFadePhase())
	{
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha_);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// @brief 難易度設定に基づき、生成する敵のステータスやスポーン数の補正倍率を適用する
/// @param e 補正値を書き込む対象の敵設定構造体への参照
void GameManager::ApplyDifficultyMultipliers(EnemyManager::enemydate& e)
{
	float statMultiplier = 1.0f;
	float countMultiplier = 1.0f;

	switch (difficulty_)
	{
	case Difficulty::kEasy:
		statMultiplier = 0.8f;
		countMultiplier = 0.8f;
		break;
	case Difficulty::kNormal:
		statMultiplier = 1.0f;
		countMultiplier = 1.0f;
		break;
	case Difficulty::kHard:
		statMultiplier = 1.5f;
		countMultiplier = 1.5f;
		break;
	}

	e.hp = e.hp * statMultiplier;
	e.attack = e.attack * statMultiplier;

	// ボスの数は難易度に関わらず常に1体を維持
	if (e.tag != EnemyManager::boss_stage1)
	{
		e.Count = static_cast<int>(std::ceil(e.Count * countMultiplier));
		if (e.Count < 1)
		{
			e.Count = 1;
		}
	}
}

/// @brief 現在のフェーズに対応する敵キャラクター群をスポーン（生成）させる
void GameManager::SpawnPhaseEnemies()
{
	VECTOR centerPos = Config::GetStageCenter();

	switch (current_phase_)
	{
	case Phase::kPhase1:
		SpawnPhase1Enemies(centerPos);
		break;
	case Phase::kPhase2:
		SpawnPhase2Enemies(centerPos);
		break;
	case Phase::kPhase3:
		SpawnPhase3Enemies(centerPos);
		break;
	case Phase::kBoss:
		SpawnBossEnemy();
		break;
	default:
		break;
	}
}

/// @brief フェーズ1用の敵構成を生成する
/// @param centerPos 基準となる中心座標
void GameManager::SpawnPhase1Enemies(const VECTOR& centerPos)
{
	AddEnemy(MakeEnemyData(
		EnemyManager::night_stage1,
		"Resource/model/character/01_human.mv1",
		centerPos,
		VGet(3000.0f, 100.0f, 3000.0f),
		20,
		3.0f,
		2.0f,
		60.0f,
		1000.0f,
		100.0f,
		100.0f,
		true,
		200,
		10));
}

/// @brief フェーズ2用の敵構成を生成する
/// @param centerPos 基準となる中心座標
void GameManager::SpawnPhase2Enemies(const VECTOR& centerPos)
{
	AddEnemy(MakeEnemyData(
		EnemyManager::archerl_stage1,
		"Resource/model/character/01_human.mv1",
		centerPos,
		VGet(8000.0f, 100.0f, 8000.0f),
		20,
		3.0f,
		2.0f,
		60.0f,
		2000.0f,
		1000.0f,
		1000.0f,
		true,
		200,
		8));

	AddEnemy(MakeEnemyData(
		EnemyManager::night_stage1,
		"Resource/model/character/01_human.mv1",
		centerPos,
		VGet(2000.0f, 100.0f, 2000.0f),
		20,
		4.0f,
		3.0f,
		60.0f,
		1000.0f,
		100.0f,
		100.0f,
		true,
		200,
		6));
}

/// @brief フェーズ3用の敵構成を生成する
/// @param centerPos 基準となる中心座標
void GameManager::SpawnPhase3Enemies(const VECTOR& centerPos)
{
	AddEnemy(MakeEnemyData(
		EnemyManager::monster_stage1,
		"Resource/model/character/02_enemy_monster_model.mv1",
		centerPos,
		VGet(12000.0f, 100.0f, 12000.0f),
		100,
		2.0f,
		5.0f,
		100.0f,
		1000.0f,
		100.0f,
		100.0f,
		true,
		500,
		5));

	AddEnemy(MakeEnemyData(
		EnemyManager::archerl_stage1,
		"Resource/model/character/01_human.mv1",
		centerPos,
		VGet(6000.0f, 100.0f, 6000.0f),
		20,
		3.0f,
		2.0f,
		60.0f,
		2000.0f,
		1000.0f,
		1000.0f,
		true,
		200,
		5));

	AddEnemy(MakeEnemyData(
		EnemyManager::night_stage1,
		"Resource/model/character/01_human.mv1",
		centerPos,
		VGet(6000.0f, 100.0f, 6000.0f),
		20,
		3.0f,
		2.0f,
		60.0f,
		1000.0f,
		100.0f,
		100.0f,
		true,
		200,
		5));
}

/// @brief ボスキャラクターを生成する
void GameManager::SpawnBossEnemy()
{
	VECTOR bossCenter = Config::GetStageBossCenter();
	AddEnemy(MakeEnemyData(
		EnemyManager::boss_stage1,
		"Resource/model/character/03_boss.mv1",
		bossCenter,
		VAdd(bossCenter, VGet(-500.0f, 0.0f, 2000.0f)),
		300,
		10.0f,
		20.0f,
		300.0f,
		2000.0f,
		1000.0f,
		1000.0f,
		true,
		3000,
		1));
}

/// @brief 敵生成パラメータをまとめた構造体データを生成する補助関数
/// @param tag 敵の種別識別タグ
/// @param filename 使用する3Dモデルのパス
/// @param spawnCenter スポーン基準中心座標
/// @param initPos 初期配置座標
/// @param hp 最大体力
/// @param speed 移動速度
/// @param attack 攻撃力
/// @param hitSize 当たり判定サイズ
/// @param search1 索敵範囲1
/// @param search2 索敵範囲2
/// @param search3 索敵範囲3
/// @param isSeparateAnim アニメーション分離フラグ
/// @param money 倒した際の獲得資金
/// @param count 一括生成する個体数
/// @return EnemyManager::enemydate 作成された敵データ構造体
EnemyManager::enemydate GameManager::MakeEnemyData(
	EnemyManager::enemy_tag tag,
	const std::string& filename,
	const VECTOR& spawnCenter,
	const VECTOR& initPos,
	float hp,
	float speed,
	float attack,
	float hitSize,
	float search1,
	float search2,
	float search3,
	bool isSeparateAnim,
	int money,
	int count) const
{
	EnemyManager::enemydate enemyData = {};
	enemyData.tag = tag;
	enemyData.filename = filename;
	enemyData.spawnCenter = spawnCenter;
	enemyData.initPos = initPos;
	enemyData.hp = hp;
	enemyData.speed = speed;
	enemyData.attack = attack;
	enemyData.HitSize = hitSize;
	enemyData.Serch1 = search1;
	enemyData.Serch2 = search2;
	enemyData.Serch3 = search3;
	enemyData.is_separate_anim_ = isSeparateAnim;
	enemyData.money = money;
	enemyData.Count = count;
	return enemyData;
}

/// @brief 難易度補正を計算した上で敵リストに追加・登録する
/// @param enemyData 追加する敵データの設定構造体
void GameManager::AddEnemy(EnemyManager::enemydate enemyData)
{
	ApplyDifficultyMultipliers(enemyData);
	enemy_manager_->NewEnemyList(enemyData);
}

/// @brief 画面上にミニマップUIおよび各種アイコンを描画する
void GameManager::DrawMinimap()
{
	const float mapSize = 220.0f;
	const float mapX = Config::ScreenWidth - mapSize - 28.0f;
	const float mapY = 198.0f;
	const float mapCenterX = mapX + mapSize / 2.0f;
	const float mapCenterY = mapY + mapSize / 2.0f;
	const float viewRange = 6000.0f;
	const float maxDistance = mapSize / 2.0f;

	VECTOR playerPos = VGet(0, 0, 0);
	if (Master::player_ != nullptr)
	{
		playerPos = Master::player_->GetPosition();
	}

	DrawMinimapFrame(mapX, mapY, mapSize);
	DrawMinimapPortal(mapCenterX, mapCenterY, playerPos, viewRange, maxDistance);
	DrawMinimapShops(mapCenterX, mapCenterY, playerPos, viewRange, maxDistance);
	DrawMinimapEnemies(mapX, mapY, mapSize, mapCenterX, mapCenterY, playerPos, viewRange, maxDistance);
	DrawMinimapPlayer(mapCenterX, mapCenterY);
}

/// @brief ミニマップの背景外枠パネルを描画する
/// @param mapX マップ描画開始X座標
/// @param mapY マップ描画開始Y座標
/// @param mapSize マップの1辺のピクセルサイズ
void GameManager::DrawMinimapFrame(float mapX, float mapY, float mapSize) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize), GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize), GetColor(255, 255, 255), FALSE);
}

/// @brief プレイヤー座標からの相対位置からミニマップ上の表示オフセットピクセル値を計算する
/// @param pos 判定対象オブジェクトのワールド座標
/// @param playerPos プレイヤーのワールド座標
/// @param viewRange マップに収めるワールド空間の表示視野距離
/// @param maxDistance ミニマップの中心から端までの最大ピクセル距離
/// @return VECTOR ミニマップ中心からのオフセットピクセル座標（X, Y）
VECTOR GameManager::GetMinimapOffset(VECTOR pos, VECTOR playerPos, float viewRange, float maxDistance) const
{
	const float diffX = pos.x - playerPos.x;
	const float diffZ = pos.z - playerPos.z;
	const float mapRelX = (diffX / viewRange) * maxDistance;
	const float mapRelY = (-diffZ / viewRange) * maxDistance;
	return VGet(mapRelX, mapRelY, 0.0f);
}

/// @brief ミニマップ外にハミ出るアイコンの描画オフセットを枠線上にクランプ固定する
/// @param offset 算出されたマップ中心からのオフセットピクセル値
/// @param maxDistance 枠線の最大半径ピクセル値
/// @return VECTOR クランプ補正後の表示オフセット座標
VECTOR GameManager::ClampMinimapOffset(VECTOR offset, float maxDistance) const
{
	if (fabsf(offset.x) <= maxDistance && fabsf(offset.y) <= maxDistance)
	{
		return offset;
	}

	const float scaleX = fabsf(offset.x) > 0.001f ? maxDistance / fabsf(offset.x) : 999.0f;
	const float scaleY = fabsf(offset.y) > 0.001f ? maxDistance / fabsf(offset.y) : 999.0f;
	const float scale = (scaleX < scaleY) ? scaleX : scaleY;
	return VGet(offset.x * scale, offset.y * scale, 0.0f);
}

/// @brief ミニマップ上にボス用ポータルの位置アイコンを描画する
/// @param mapCenterX ミニマップ中心X座標
/// @param mapCenterY ミニマップ中心Y座標
/// @param playerPos プレイヤー座標
/// @param viewRange 視野表示範囲
/// @param maxDistance 最大描画半径
void GameManager::DrawMinimapPortal(float mapCenterX, float mapCenterY, VECTOR playerPos, float viewRange, float maxDistance) const
{
	VECTOR portalOffset = GetMinimapOffset(boss_portal_pos_, playerPos, viewRange, maxDistance);
	const bool portalFar = (fabsf(portalOffset.x) > maxDistance || fabsf(portalOffset.y) > maxDistance);
	VECTOR portalClamped = ClampMinimapOffset(portalOffset, maxDistance);

	DrawCircle((int)(mapCenterX + portalClamped.x), (int)(mapCenterY + portalClamped.y), 6, GetColor(0, 255, 255), TRUE);
	if (portalFar)
	{
		DrawCircle((int)(mapCenterX + portalClamped.x), (int)(mapCenterY + portalClamped.y), 9, GetColor(0, 255, 255), FALSE);
	}
}

/// @brief ミニマップ上にショップNPCの位置アイコンを描画する
/// @param mapCenterX ミニマップ中心X座標
/// @param mapCenterY ミニマップ中心Y座標
/// @param playerPos プレイヤー座標
/// @param viewRange 視野表示範囲
/// @param maxDistance 最大描画半径
void GameManager::DrawMinimapShops(float mapCenterX, float mapCenterY, VECTOR playerPos, float viewRange, float maxDistance) const
{
	const auto& shops = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
	for (auto obj : shops)
	{
		VECTOR shopOffset = GetMinimapOffset(obj->GetPosition(), playerPos, viewRange, maxDistance);
		VECTOR shopClamped = ClampMinimapOffset(shopOffset, maxDistance);
		DrawCircle((int)(mapCenterX + shopClamped.x), (int)(mapCenterY + shopClamped.y), 5, GetColor(255, 255, 0), TRUE);
	}
}

/// @brief ミニマップ上に敵キャラクターの位置アイコンを描画する（枠外クリッピング対応）
/// @param mapX マップ左上X座標
/// @param mapY マップ左上Y座標
/// @param mapSize マップの一辺サイズ
/// @param mapCenterX ミニマップ中心X座標
/// @param mapCenterY ミニマップ中心Y座標
/// @param playerPos プレイヤー座標
/// @param viewRange 視野表示範囲
/// @param maxDistance 最大描画半径
void GameManager::DrawMinimapEnemies(float mapX, float mapY, float mapSize, float mapCenterX, float mapCenterY, VECTOR playerPos, float viewRange, float maxDistance) const
{
	SetDrawArea((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize));

	const auto& enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	for (auto obj : enemies)
	{
		VECTOR enemyOffset = GetMinimapOffset(obj->GetPosition(), playerPos, viewRange, maxDistance);
		DrawCircle((int)(mapCenterX + enemyOffset.x), (int)(mapCenterY + enemyOffset.y), 4, GetColor(255, 0, 0), TRUE);
	}

	SetDrawArea(0, 0, 1920, 1080);
}

/// @brief ミニマップの中心に自プレイヤーの自機アイコンおよび向きベクトルを描画する
/// @param mapCenterX ミニマップ中心X座標
/// @param mapCenterY ミニマップ中心Y座標
void GameManager::DrawMinimapPlayer(float mapCenterX, float mapCenterY) const
{
	if (Master::player_ == nullptr)
	{
		return;
	}

	DrawCircle((int)mapCenterX, (int)mapCenterY, 5, GetColor(0, 255, 0), TRUE);

	const float playerAngle = Master::player_->GetAngle();
	const float dirX = sinf(playerAngle) * 15.0f;
	const float dirY = -cosf(playerAngle) * 15.0f;
	DrawLine((int)mapCenterX, (int)mapCenterY, (int)(mapCenterX + dirX), (int)(mapCenterY + dirY), GetColor(0, 255, 0), 2);
}