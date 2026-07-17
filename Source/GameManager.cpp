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
namespace
{
    struct DebugButton
    {
        int x;
        int y;
        int w;
        int h;
        const char* label;
    };

    bool IsMouseInButton(const DebugButton& button, int mouseX, int mouseY)
    {
        return mouseX >= button.x &&
            mouseX <= button.x + button.w &&
            mouseY >= button.y &&
            mouseY <= button.y + button.h;
    }

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


/*
 * 目的（GameManagerのコンストラクタ）
 * [入力] EnemyManager* enemyManager, Difficulty diff
 * [出力] なし
 * [副作用] 各種変数の初期化、フェーズ1の開始
 */
GameManager::GameManager(EnemyManager* enemyManager, Difficulty diff)
    	: enemy_manager_(enemyManager), difficulty_(diff), current_phase_(Phase::kPhase1), shop_timer_(0), fade_alpha_(0), boss_portal_pos_(VGet(0,0,0)), boss_cutscene_timer_(0), cutscene_start_pos_(VGet(0,0,0))
{
    // 最初のウェーブを開始
    SpawnPhaseEnemies();

    // ゲーム開始時にプレイヤーの反対側にボスポータルを初期化
    VECTOR playerStartPos = VGet(-1200.0f, 20.0f, -1000.0f);
    VECTOR center = Config::GetStageCenter();
    VECTOR dir = VSub(playerStartPos, center);
    dir.y = 0.0f;
    if (VSize(dir) < 1.0f) dir = VGet(0.0f, 0.0f, 1.0f);
    else dir = VNorm(dir);
    boss_portal_pos_ = VAdd(center, VScale(dir, -5000.0f));


    // ポータルの土台
    float portalSize = 100.0f;
    new Stage(VAdd(boss_portal_pos_, VGet(0.0f, -570.0f, 0.0f)), "Resource/3D/portal/source/portal.mv1", "Resource/3D/portal/source/portal.mv1", VGet(portalSize, portalSize, portalSize));

    // Portal Magic Circle Effect (slightly above the portal base to prevent clipping)
    new EffekseerObject("Mahoujin", "Resource/effect/MAGICAL/Mahoujin.efkproj", VAdd(boss_portal_pos_, VGet(0.0f, -565.0f, 0.0f)), nullptr, true, 1.0f, 1.0f);
}


/*
 * 目的（GameManagerのデストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] なし
 */
GameManager::~GameManager()
{
}


/*
 * 目的（毎フレームの更新処理を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] フェーズの移行、敵の出現管理、ショップ処理など
 */
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
}


/*
 * 目的（ボスカットシーン中のカメラ座標などを更新するため）
 * [入力] なし
 * [出力] bool: 完了していればtrue
 * [副作用] カメラの注視点移動、タイマー減少
 */
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


/*
 * 目的（ボス戦前の画面暗転フェードを更新するため）
 * [入力] なし
 * [出力] bool: 完了していればtrue
 * [副作用] fade_alpha_の増減
 */
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


/*
 * 目的（ショップフェーズのタイマーを管理するため）
 * [入力] なし
 * [出力] なし
 * [副作用] タイマーの減少、0になれば次のフェーズへ移行
 */
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


/*
 * 目的（バトルフェーズの進行を管理するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 敵が全滅した場合、ショップまたはボスへの移行処理を行う
 */
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


/*
 * 目的（ショップフェーズへの移行処理を行うため）
 * [入力] Phase nextPhase
 * [出力] なし
 * [副作用] 敵マネージャーの停止、NPCの出現、UIタイマーのセット
 */
void GameManager::StartShopPhase(Phase nextPhase)
{
    current_phase_ = nextPhase;
    shop_timer_ = 60 * 20;
    SendShopsIn();
}


/*
 * 目的（ボス戦前のフェード演出を開始するため）
 * [入力] なし
 * [出力] なし
 * [副作用] プレイヤー停止、フェードアルファ値の初期化
 */
void GameManager::StartBossTransition()
{
    current_phase_ = Phase::kFadeOutToBoss;
    fade_alpha_ = 0;
}


/*
 * 目的（ボスゲート出現の演出を開始するため）
 * [入力] なし
 * [出力] なし
 * [副作用] カメラ操作の無効化など
 */
void GameManager::StartBossGateCutscene()
{
    Master::is_cutscene_playing_ = true;
    boss_cutscene_timer_ = 0;
    cutscene_start_pos_ = Master::player_->GetPosition();
    Master::camera_->SetCutsceneMode(true);
}


/*
 * 目的（ショップNPCをフィールドに出現させるため）
 * [入力] なし
 * [出力] なし
 * [副作用] ObjectManagerへのStatShop追加
 */
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


/*
 * 目的（ショップNPCを退場させるため）
 * [入力] なし
 * [出力] なし
 * [副作用] StatShopオブジェクトの退場フラグ有効化
 */
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


/*
 * 目的（ショップNPCが所定の位置に到着したか判定するため）
 * [入力] なし
 * [出力] bool: 全て到着していればtrue
 * [副作用] なし
 */
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


/*
 * 目的（現在ショップフェーズかどうか判定するため）
 * [入力] なし
 * [出力] bool: ショップフェーズならtrue
 * [副作用] なし
 */
bool GameManager::IsShopPhase() const
{
    return current_phase_ == Phase::kShop1 || current_phase_ == Phase::kShop2 || current_phase_ == Phase::kShop3;
}


/*
 * 目的（現在ボスへのフェード遷移中か判定するため）
 * [入力] なし
 * [出力] bool: 遷移中ならtrue
 * [副作用] なし
 */
bool GameManager::IsBossFadePhase() const
{
    return current_phase_ == Phase::kFadeOutToBoss || current_phase_ == Phase::kFadeInBoss;
}


/*
 * 目的（プレイヤーがボスポータルに触れたか判定するため）
 * [入力] なし
 * [出力] bool: 触れていればtrue
 * [副作用] なし
 */
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


/*
 * 目的（デバッグコントロールが有効か判定するため）
 * [入力] なし
 * [出力] bool: 有効ならtrue
 * [副作用] なし
 */
bool GameManager::IsDebugControlsEnabled() const
{
    return Master::debug_ != nullptr && Master::debug_->Getdebug();
}


/*
 * 目的（F5やF6キーによるデバッグ操作を更新するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 敵の全滅やフェーズの強制変更
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


/*
 * 目的（デバッグ用：敵を全滅させるため）
 * [入力] なし
 * [出力] なし
 * [副作用] 全ての敵のHPをゼロにする
 */
void GameManager::DebugKillEnemies()
{
    const auto& enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    for (auto obj : enemies)
    {
        Enemy* enemy = obj->CastTo<Enemy>();
        if (enemy)
        {
            enemy->Damage(999999.0f);
        }
    }
}


/*
 * 目的（デバッグ用：強制的にボス戦へ移行させるため）
 * [入力] なし
 * [出力] なし
 * [副作用] フェーズ変更とボスポータルの出現
 */
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

    const auto& enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    for (auto obj : enemies)
    {
        obj->SetDeleteFlag(true);
    }
    Master::scene_manager_->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();

    SendShopsOut();
    StartBossTransition();
}

/*
 * 目的（ゲームマネージャーに関連するHUDなどを描画するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 画面描画
 */
void GameManager::Draw()
{
    DrawPhaseHud();
    DrawMinimap();
    DrawShopBanner();
    DrawBossFade();
    DrawDebugPanel();
}



/*
 * 目的（デバッグ用のUIパネルを描画するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 画面描画
 */
void GameManager::DrawDebugPanel()
{
    if (!IsDebugControlsEnabled())
    {
        return;
    }

    int mouseX = 0;
    int mouseY = 0;
    InputManager::GetMousePos(mouseX, mouseY);

    // デバッグパネルのX座標
    const int panelX = 22;
    const int panelY = 96;
    const int panelW = 318;
    const int panelH = 56;
    const DebugButton killButton = { 28, 104, 164, 40, "KILL ENEMIES" };
    const DebugButton bossButton = { 202, 104, 132, 40, "GO BOSS" };

    int fontSize = GetFontSize();
    SetFontSize(18);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 165);
    DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(0, 0, 0), true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawBox(panelX, panelY, panelX + panelW, panelY + panelH, GetColor(92, 68, 30), false);
    DrawFormatString(panelX + 8, panelY - 18, GetColor(190, 170, 120), "DEBUG  F5 / F6");

    DrawDebugButton(killButton, IsMouseInButton(killButton, mouseX, mouseY));
    DrawDebugButton(bossButton, IsMouseInButton(bossButton, mouseX, mouseY));

    SetFontSize(fontSize);
}
const char*
/*
 * 目的（現在のフェーズの文字列ラベルを取得するため）
 * [入力] なし
 * [出力] const char*: フェーズラベル
 * [副作用] なし
 */
GameManager::GetPhaseLabel() const
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

const char*
/*
 * 目的（現在のフェーズのサブラベル（BATTLEなど）を取得するため）
 * [入力] なし
 * [出力] const char*: サブラベル
 * [副作用] なし
 */
GameManager::GetPhaseSubLabel() const
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


/*
 * 目的（現在の敵の生存数を取得するため）
 * [入力] なし
 * [出力] int: 生存数
 * [副作用] なし
 */
int GameManager::GetEnemyCount() const
{
    const auto& enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    return static_cast<int>(enemies.size());
}


/*
 * 目的（現在のフェーズと敵の残数を画面右上に描画するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 画面描画
 */
void GameManager::DrawPhaseHud()
{
    int fontSize = GetFontSize();
    SetFontSize(24);

    // HUDパネルのX座標（画面右寄せ）
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
    DrawFormatString(panelX + 18, panelY + 19, GetColor(245, 226, 174), "%s", GetPhaseLabel());
    DrawFormatString(panelX + 18, panelY + 48, GetColor(205, 210, 216), "%s", GetPhaseSubLabel());
    DrawFormatString(panelX + 230, panelY + 48, GetColor(238, 238, 238), "x%02d", GetEnemyCount());

    SetFontSize(fontSize);
}


/*
 * 目的（ショップフェーズの残り時間バナーを画面上部に描画するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 画面描画
 */
void GameManager::DrawShopBanner()
{
    if (!IsShopPhase())
    {
        return;
    }

    int fontSize = GetFontSize();
    SetFontSize(28);

    // バナーの横幅
    const int bannerW = 700;
    // バナーのX座標（画面中央）
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
        DrawFormatString(bannerX + 210, bannerY + 14, GetColor(255, 238, 156), "NEXT WAVE IN %d", seconds);
    }
    else
    {
        DrawFormatString(bannerX + 108, bannerY + 14, GetColor(141, 239, 255), "ENTER THE BLUE TELEPORTER TO START BOSS");
    }

    SetFontSize(fontSize);
}


/*
 * 目的（ボス戦遷移時のフェード（暗転）を描画するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 画面描画
 */
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

/*
 * 目的（難易度に応じて敵のステータスに倍率を適用するため）
 * [入力] EnemyManager::enemydate& e
 * [出力] なし
 * [副作用] 引数で渡された敵データのHPや攻撃力などの変更
 */
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
    // ボスの数は通常1体を維持する
    if (e.tag != EnemyManager::boss_stage1)
    {
        e.Count = static_cast<int>(std::ceil(e.Count * countMultiplier));
        if (e.Count < 1)
        {
            e.Count = 1;
        }
    }
}


/*
 * 目的（現在のフェーズに応じた敵キャラクターをフィールドにスポーンさせるため）
 * [入力] なし
 * [出力] なし
 * [副作用] EnemyManagerへの敵追加
 */
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

void GameManager::SpawnPhase1Enemies(const VECTOR& centerPos)
{
    AddEnemy(MakeEnemyData(
        EnemyManager::night_stage1,
        "Resource/Model/T.mv1",
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

void GameManager::SpawnPhase2Enemies(const VECTOR& centerPos)
{
    AddEnemy(MakeEnemyData(
        EnemyManager::archerl_stage1,
        "Resource/Model/T.mv1",
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
        "Resource/Model/T.mv1",
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

void GameManager::SpawnPhase3Enemies(const VECTOR& centerPos)
{
    AddEnemy(MakeEnemyData(
        EnemyManager::monster_stage1,
        "Resource/Model/monster.mv1",
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
        "Resource/Model/T.mv1",
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
        "Resource/Model/T.mv1",
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

void GameManager::SpawnBossEnemy()
{
    VECTOR bossCenter = Config::GetStageBossCenter();
    AddEnemy(MakeEnemyData(
        EnemyManager::boss_stage1,
        "Resource/Model/Boss1.mv1",
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

void GameManager::AddEnemy(EnemyManager::enemydate enemyData)
{
    ApplyDifficultyMultipliers(enemyData);
    enemy_manager_->NewEnemyList(enemyData);
}

/*
 * 目的（ミニマップを描画するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 画面描画
 */
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

void GameManager::DrawMinimapFrame(float mapX, float mapY, float mapSize) const
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize), GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawBox((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize), GetColor(255, 255, 255), FALSE);
}

VECTOR GameManager::GetMinimapOffset(VECTOR pos, VECTOR playerPos, float viewRange, float maxDistance) const
{
    const float diffX = pos.x - playerPos.x;
    const float diffZ = pos.z - playerPos.z;
    const float mapRelX = (diffX / viewRange) * maxDistance;
    const float mapRelY = (-diffZ / viewRange) * maxDistance;
    return VGet(mapRelX, mapRelY, 0.0f);
}

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
