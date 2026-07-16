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

GameManager::GameManager(EnemyManager* enemyManager, Difficulty diff)
    	: enemy_manager_(enemyManager), difficulty_(diff), current_phase_(Phase::kPhase1), shop_timer_(0), fade_alpha_(0), boss_portal_pos_(VGet(0,0,0)), boss_cutscene_timer_(0), cutscene_start_pos_(VGet(0,0,0))
{
    // Start the first wave
    SpawnPhaseEnemies();

    // Initialize Boss Portal at the start of the game, opposite to player spawn
    VECTOR playerStartPos = VGet(-1200.0f, 20.0f, -1000.0f);
    VECTOR center = Config::GetStageCenter();
    VECTOR dir = VSub(playerStartPos, center);
    dir.y = 0.0f;
    if (VSize(dir) < 1.0f) dir = VGet(0.0f, 0.0f, 1.0f);
    else dir = VNorm(dir);
    boss_portal_pos_ = VAdd(center, VScale(dir, -5000.0f));
    
    
    // Portal Base
    float portalSize = 100.0f;
    new Stage(VAdd(boss_portal_pos_, VGet(0.0f, -570.0f, 0.0f)), "Resource/3D/portal/source/portal.mv1", "Resource/3D/portal/source/portal.mv1", VGet(portalSize, portalSize, portalSize));
    
    // Portal Magic Circle Effect (slightly above the portal base to prevent clipping)
    new EffekseerObject("Mahoujin", "Resource/effect/MAGICAL/Mahoujin.efkproj", VAdd(boss_portal_pos_, VGet(0.0f, -565.0f, 0.0f)), nullptr, true, 1.0f, 1.0f);
}

GameManager::~GameManager()
{
}

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

bool GameManager::UpdateBossCutscene()
{
    if (!Master::CutscenePlaying)
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
    Master::mpCamera->SetCutsceneTarget(currentPos);

    if (boss_cutscene_timer_ > 240)
    {
        Master::CutscenePlaying = false;
        Master::mpCamera->SetCutsceneMode(false);
    }

    return true;
}

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
            Master::mpPlayer->SetPosition(VAdd(Config::GetStageBossCenter(), VGet(500.0f, 0.0f, -2000.0f)));

            if (Master::mpSoundManager)
            {
                Master::mpSoundManager->PlaySE(SoundManager::SE_WARP);
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

void GameManager::StartShopPhase(Phase nextPhase)
{
    current_phase_ = nextPhase;
    shop_timer_ = 60 * 20;
    SendShopsIn();
}

void GameManager::StartBossTransition()
{
    current_phase_ = Phase::kFadeOutToBoss;
    fade_alpha_ = 0;
}

void GameManager::StartBossGateCutscene()
{
    Master::CutscenePlaying = true;
    boss_cutscene_timer_ = 0;
    cutscene_start_pos_ = Master::mpPlayer->GetPosition();
    Master::mpCamera->SetCutsceneMode(true);
}

void GameManager::SendShopsIn()
{
    const auto& shops = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
    for (auto s : shops)
    {
        StatShop* shop = s->CastTo<StatShop>();
        if (shop)
        {
            shop->StartWalkingIn();
        }
    }
}

void GameManager::SendShopsOut()
{
    const auto& shops = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
    for (auto s : shops)
    {
        StatShop* shop = s->CastTo<StatShop>();
        if (shop)
        {
            shop->StartWalkingOut();
        }
    }
}

bool GameManager::AreShopsArrived() const
{
    const auto& shops = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
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

bool GameManager::IsShopPhase() const
{
    return current_phase_ == Phase::kShop1 || current_phase_ == Phase::kShop2 || current_phase_ == Phase::kShop3;
}

bool GameManager::IsBossFadePhase() const
{
    return current_phase_ == Phase::kFadeOutToBoss || current_phase_ == Phase::kFadeInBoss;
}

bool GameManager::IsBossGateTouched() const
{
    if (!Master::mpPlayer)
    {
        return false;
    }

    Player3D* player = Master::mpPlayer->CastTo<Player3D>();
    if (!player)
    {
        return false;
    }

    float dist = VSize(VSub(player->GetPosition(), boss_portal_pos_));
    return dist < 150.0f;
}

bool GameManager::IsDebugControlsEnabled() const
{
    return Master::mpDebug != nullptr && Master::mpDebug->Getdebug();
}

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

void GameManager::DebugKillEnemies()
{
    const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    for (auto obj : enemies)
    {
        Enemy* enemy = obj->CastTo<Enemy>();
        if (enemy)
        {
            enemy->Damage(999999.0f);
        }
    }
}

void GameManager::DebugGoBoss()
{
    if (current_phase_ == Phase::kBoss ||
        current_phase_ == Phase::kFadeOutToBoss ||
        current_phase_ == Phase::kFadeInBoss)
    {
        return;
    }

    Master::CutscenePlaying = false;
    if (Master::mpCamera)
    {
        Master::mpCamera->SetCutsceneMode(false);
    }

    const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    for (auto obj : enemies)
    {
        obj->SetDeleteFlag(true);
    }
    Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();

    SendShopsOut();
    StartBossTransition();
}
void GameManager::Draw()
{
    DrawPhaseHud();
    DrawMinimap();
    DrawShopBanner();
    DrawBossFade();
    DrawDebugPanel();
}


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

int GameManager::GetEnemyCount() const
{
    const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    return static_cast<int>(enemies.size());
}

void GameManager::DrawPhaseHud()
{
    int fontSize = GetFontSize();
    SetFontSize(24);

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

void GameManager::DrawShopBanner()
{
    if (!IsShopPhase())
    {
        return;
    }

    int fontSize = GetFontSize();
    SetFontSize(28);

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
        DrawFormatString(bannerX + 210, bannerY + 14, GetColor(255, 238, 156), "NEXT WAVE IN %d", seconds);
    }
    else
    {
        DrawFormatString(bannerX + 108, bannerY + 14, GetColor(141, 239, 255), "ENTER THE BLUE TELEPORTER TO START BOSS");
    }

    SetFontSize(fontSize);
}

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
    // boss count should remain 1 usually
    if (e.tag != EnemyManager::boss_stage1)
    {
        e.Count = static_cast<int>(std::ceil(e.Count * countMultiplier));
        if (e.Count < 1)
        {
            e.Count = 1;
        }
    }
}

void GameManager::SpawnPhaseEnemies()
{
    // 隰ｨ・ｽ・ｽ邵ｺ蠕後○郢晢ｿｽ繝ｻ郢ｧ・ｽ・ｽ邵ｺ荵晢ｽ芽棔謔ｶ・ｽ・ｽ邵ｺ・ｽ・ｽ髣懶ｽｽ闕ｳ荵晢ｿｽ陜灘玄・ｽ・ｽ・ｽ・ｽ邵ｺ蜉ｱ竊醍ｸｺ繝ｻ・ｽ・ｽ邵ｺ繝ｻ竊鍋ｸｲ・ｽ・ｽonfig邵ｺ・ｽ・ｽ郢ｧ・ｽ・ｽ郢晢ｿｽ繝ｻ郢ｧ・ｽ・ｽ闕ｳ・ｽ・ｽ陟｢繝ｻ・ｽ・ｽ・ｽ・ｽ隶灘生・ｽ・ｽ雋会ｽｧ邵ｺ蝣ｺ・ｽ・ｽ蜥ｲ・ｽ・ｽ・ｽ・ｽ邵ｺ・ｽ・ｽ陜難ｽｺ・ｽ謔ｶ竊堤ｸｺ蜷ｶ・ｽ
    VECTOR centerPos = Config::GetStageCenter();
if (current_phase_ == Phase::kPhase1)
{
        // Wave 1
        EnemyManager::enemydate e;
        e.filename = "Resource/Model/T.mv1";
        e.spawnCenter = centerPos;
        e.initPos = VGet(3000.0f, 100.0f, 3000.0f);
        e.hp = 20;
        e.speed = 3.0f;
        e.attack = 2.0f;
        e.HitSize = 60.0f;
        e.Serch1 = 1000.0f;
        e.Serch2 = 100.0f;
        e.Serch3 = 100.0f;
        e.isSeparateAnim = true;
        e.money = 200;
        e.tag = EnemyManager::night_stage1;
        e.Count = 10;
        
        ApplyDifficultyMultipliers(e);
        enemy_manager_->NewEnemyList(e);
    }
    else if (current_phase_ == Phase::kPhase2)
{
        // Wave 2: 鬯ｲ逍ｲ・ｽ・ｽ蜍滂ｿｽ陞｢・ｽ・ｽ
        EnemyManager::enemydate e1;
        e1.filename = "Resource/Model/T.mv1";
        e1.spawnCenter = centerPos;
        e1.initPos = VGet(8000.0f, 100.0f, 8000.0f);
        e1.hp = 20;
        e1.speed = 3.0f;
        e1.attack = 2.0f;
        e1.HitSize = 60.0f;
        e1.money = 200;
        e1.Serch1 = 2000.0f;
        e1.Serch2 = 1000.0f;
        e1.Serch3 = 1000.0f;
        e1.isSeparateAnim = true;
        e1.tag = EnemyManager::archerl_stage1;
        e1.Count = 8;
        
        ApplyDifficultyMultipliers(e1);
        enemy_manager_->NewEnemyList(e1);

        // Wave 2: 髴醍ｬｬ逎∬恆・ｽ・ｽ陞｢・ｽ・ｽ
        EnemyManager::enemydate e2;
        e2.filename = "Resource/Model/T.mv1";
        e2.spawnCenter = centerPos;
        e2.initPos = VGet(2000.0f, 100.0f, 2000.0f);
        e2.hp = 20;
        e2.speed = 4.0f;
        e2.attack = 3.0f;
        e2.HitSize = 60.0f;
        e2.Serch1 = 1000.0f;
        e2.Serch2 = 100.0f;
        e2.Serch3 = 100.0f;
        e2.isSeparateAnim = true;
        e2.money = 200;
        e2.tag = EnemyManager::night_stage1;
        e2.Count = 6;

        ApplyDifficultyMultipliers(e2);
        enemy_manager_->NewEnemyList(e2);
    }
    else if (current_phase_ == Phase::kPhase3)
{
        // Wave 3: 鬩･蝓ｼ纃ｼ驍丞｣ｻ・ｽ・ｽ・ｽ・ｽ騾包ｽｨ
        EnemyManager::enemydate e_heavy;
        e_heavy.filename = "Resource/Model/monster.mv1";
        e_heavy.spawnCenter = centerPos;
        e_heavy.initPos = VGet(12000.0f, 100.0f, 12000.0f);
        e_heavy.hp = 100;
        e_heavy.speed = 2.0f;
        e_heavy.attack = 5.0f;
        e_heavy.HitSize = 100.0f;
        e_heavy.Serch1 = 1000.0f;
        e_heavy.Serch2 = 100.0f;
        e_heavy.Serch3 = 100.0f;
        e_heavy.isSeparateAnim = true;
        e_heavy.money = 500;
        e_heavy.tag = EnemyManager::monster_stage1;
        e_heavy.Count = 5;

        ApplyDifficultyMultipliers(e_heavy);
        enemy_manager_->NewEnemyList(e_heavy);

        // Wave 3: 鬯ｲ逍ｲ・ｽ・ｽ蜍滂ｿｽ陞｢・ｽ・ｽ
        EnemyManager::enemydate e_magic;
        e_magic.filename = "Resource/Model/T.mv1";
        e_magic.spawnCenter = centerPos;
        e_magic.initPos = VGet(6000.0f,100.0f, 6000.0f);
        e_magic.hp = 20;
        e_magic.speed = 3.0f;
        e_magic.attack = 2.0f;
        e_magic.HitSize = 60.0f;
        e_magic.Serch1 = 2000.0f;
        e_magic.Serch2 = 1000.0f;
        e_magic.Serch3 = 1000.0f;
        e_magic.isSeparateAnim = true;
        e_magic.money = 200;
        e_magic.tag = EnemyManager::archerl_stage1;
        e_magic.Count = 5;

        ApplyDifficultyMultipliers(e_magic);
        enemy_manager_->NewEnemyList(e_magic);

        // Wave 3: 髴醍ｬｬ逎∬怦・ｽ・ｽ陞｢・ｽ・ｽ
        EnemyManager::enemydate e_melee;
        e_melee.filename = "Resource/Model/T.mv1";
        e_melee.spawnCenter = centerPos;
        e_melee.initPos = VGet(6000.0f, 100.0f, 6000.0f);
        e_melee.hp = 20;
        e_melee.speed = 3.0f;
        e_melee.attack = 2.0f;
        e_melee.HitSize = 60.0f;
        e_melee.Serch1 = 1000.0f;
        e_melee.Serch2 = 100.0f;
        e_melee.Serch3 = 100.0f;
        e_melee.isSeparateAnim = true;
        e_melee.money = 200;
        e_melee.tag = EnemyManager::night_stage1;
        e_melee.Count = 5;

        ApplyDifficultyMultipliers(e_melee);
        enemy_manager_->NewEnemyList(e_melee);
    }
    else if (current_phase_ == Phase::kBoss)
{
        EnemyManager::enemydate e2;
        e2.filename = "Resource/Model/Boss1.mv1";
        e2.spawnCenter = Config::GetStageBossCenter();
        e2.initPos = VAdd(Config::GetStageBossCenter(), VGet(-500.0f, 0.0f, 2000.0f));
        e2.hp = 300;
        e2.speed = 10.0f;
        e2.attack = 20.0f;
        e2.HitSize = 300.0f;
        e2.Serch1 = 2000.0f;
        e2.Serch2 = 1000.0f;
        e2.Serch3 = 1000.0f;
        e2.money = 3000;
        e2.isSeparateAnim = true;
        e2.tag = EnemyManager::boss_stage1;
        e2.Count = 1;

        ApplyDifficultyMultipliers(e2);
        enemy_manager_->NewEnemyList(e2);
    }
}
void GameManager::DrawMinimap()
{
    // Minimap dimensions and position
    const float mapSize = 220.0f;
    const float mapX = Config::ScreenWidth - mapSize - 28.0f;
    const float mapY = 198.0f;
    
    // Draw minimap background (semi-transparent black)
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize), GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawBox((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize), GetColor(255, 255, 255), FALSE);
    
    // Coordinate mapping (Player-centric)
    VECTOR playerPos = VGet(0,0,0);
    if (Master::mpPlayer) playerPos = Master::mpPlayer->GetPosition();

    // View range defines how much of the world fits from center to edge of minimap
    const float viewRange = 6000.0f;
    float mapCenterX = mapX + mapSize / 2.0f;
    float mapCenterY = mapY + mapSize / 2.0f;
    float maxD = mapSize / 2.0f;
    
    auto GetMapOffset = [&](VECTOR pos) -> VECTOR {
        float diffX = pos.x - playerPos.x;
        float diffZ = pos.z - playerPos.z;
        float mapRelX = (diffX / viewRange) * maxD;
        float mapRelY = (-diffZ / viewRange) * maxD; // Z goes forward, 2D Y goes down
        return VGet(mapRelX, mapRelY, 0.0f);
    };

    auto ClampToEdge = [&](VECTOR offset) -> VECTOR {
        if (abs(offset.x) <= maxD && abs(offset.y) <= maxD) return offset;
        float scaleX = abs(offset.x) > 0.001f ? maxD / abs(offset.x) : 999.0f;
        float scaleY = abs(offset.y) > 0.001f ? maxD / abs(offset.y) : 999.0f;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;
        return VGet(offset.x * scale, offset.y * scale, 0.0f);
    };

    // Draw Boss Portal (clamped to edge)
    VECTOR portalOffset = GetMapOffset(boss_portal_pos_);
    bool portalFar = (abs(portalOffset.x) > maxD || abs(portalOffset.y) > maxD);
    VECTOR portalClamped = ClampToEdge(portalOffset);
    DrawCircle((int)(mapCenterX + portalClamped.x), (int)(mapCenterY + portalClamped.y), 6, GetColor(0, 255, 255), TRUE);
    if (portalFar)
{
        DrawCircle((int)(mapCenterX + portalClamped.x), (int)(mapCenterY + portalClamped.y), 9, GetColor(0, 255, 255), FALSE);
    }

    // Draw Shops (clamped to edge)
    const auto& shops = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
    for (auto obj : shops)
    {
        VECTOR shopOffset = GetMapOffset(obj->GetPosition());
        VECTOR shopClamped = ClampToEdge(shopOffset);
        DrawCircle((int)(mapCenterX + shopClamped.x), (int)(mapCenterY + shopClamped.y), 5, GetColor(255, 255, 0), TRUE);
    }

    // Before drawing enemies, clip to minimap rectangle
    SetDrawArea((int)mapX, (int)mapY, (int)(mapX + mapSize), (int)(mapY + mapSize));

    // Draw Enemies (not clamped, disappear when outside view)
    const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    for (auto obj : enemies)
    {
        VECTOR eOffset = GetMapOffset(obj->GetPosition());
        DrawCircle((int)(mapCenterX + eOffset.x), (int)(mapCenterY + eOffset.y), 4, GetColor(255, 0, 0), TRUE);
    }

    // Draw Player
    if (Master::mpPlayer)
    {
        // Player dot
        DrawCircle((int)mapCenterX, (int)mapCenterY, 5, GetColor(0, 255, 0), TRUE);
        
        // Player direction line
        float pAngle = Master::mpPlayer->GetAngle();
        float dirX = sinf(pAngle) * 15.0f;
        float dirY = -cosf(pAngle) * 15.0f;
        
        DrawLine((int)mapCenterX, (int)mapCenterY, (int)(mapCenterX + dirX), (int)(mapCenterY + dirY), GetColor(0, 255, 0), 2);
    }
    
    // Restore clipping area
    SetDrawArea(0, 0, 1920, 1080);
}






