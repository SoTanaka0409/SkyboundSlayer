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


/// @brief GameManager縺ｮ繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
/// @param EnemyManager* enemyManager, Difficulty diff
/// @details 蜷・ｨｮ螟画焚縺ｮ蛻晄悄蛹悶√ヵ繧ｧ繝ｼ繧ｺ1縺ｮ髢句ｧ・
GameManager::GameManager(EnemyManager* enemyManager, Difficulty diff)
    	: enemy_manager_(enemyManager), difficulty_(diff), current_phase_(Phase::kPhase1), shop_timer_(0), fade_alpha_(0), boss_portal_pos_(VGet(0,0,0)), boss_cutscene_timer_(0), cutscene_start_pos_(VGet(0,0,0))
{
    // 譛蛻昴・繧ｦ繧ｧ繝ｼ繝悶ｒ髢句ｧ・
    SpawnPhaseEnemies();

    // 繧ｲ繝ｼ繝髢句ｧ区凾縺ｫ繝励Ξ繧､繝､繝ｼ縺ｮ蜿榊ｯｾ蛛ｴ縺ｫ繝懊せ繝昴・繧ｿ繝ｫ繧貞・譛溷喧
    VECTOR playerStartPos = VGet(-1200.0f, 20.0f, -1000.0f);
    VECTOR center = Config::GetStageCenter();
    VECTOR dir = VSub(playerStartPos, center);
    dir.y = 0.0f;
    if (VSize(dir) < 1.0f) dir = VGet(0.0f, 0.0f, 1.0f);
    else dir = VNorm(dir);
    boss_portal_pos_ = VAdd(center, VScale(dir, -5000.0f));


    // 繝昴・繧ｿ繝ｫ縺ｮ蝨溷床
    float portalSize = 100.0f;
    new Stage(VAdd(boss_portal_pos_, VGet(0.0f, -570.0f, 0.0f)), "Resource/3D繝｢繝・Ν/蟆冗黄/繝昴・繧ｿ繝ｫ/01_繝昴・繧ｿ繝ｫ繝｢繝・Ν.mv1", "Resource/3D繝｢繝・Ν/蟆冗黄/繝昴・繧ｿ繝ｫ/01_繝昴・繧ｿ繝ｫ繝｢繝・Ν.mv1", VGet(portalSize, portalSize, portalSize));

   
    new EffekseerObject("Mahoujin", "Resource/繧ｨ繝輔ぉ繧ｯ繝・鬲疲ｳ暮劵/02_鬲疲ｳ暮劵繧ｨ繝輔ぉ繧ｯ繝亥・逕溽畑.efk", VAdd(boss_portal_pos_, VGet(0.0f, -565.0f, 0.0f)), nullptr, true, 1.0f, 1.0f);
}


/// @brief GameManager縺ｮ繝・せ繝医Λ繧ｯ繧ｿ
GameManager::~GameManager()
{
}


/// @brief 豈弱ヵ繝ｬ繝ｼ繝縺ｮ譖ｴ譁ｰ蜃ｦ逅・ｒ陦後≧縺溘ａ
/// @details 繝輔ぉ繝ｼ繧ｺ縺ｮ遘ｻ陦後∵雰縺ｮ蜃ｺ迴ｾ邂｡逅・√す繝ｧ繝・・蜃ｦ逅・↑縺ｩ
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


/// @brief 繝懊せ繧ｫ繝・ヨ繧ｷ繝ｼ繝ｳ荳ｭ縺ｮ繧ｫ繝｡繝ｩ蠎ｧ讓吶↑縺ｩ繧呈峩譁ｰ縺吶ｋ縺溘ａ
/// @return bool: 螳御ｺ・＠縺ｦ縺・ｌ縺ｰtrue
/// @details 繧ｫ繝｡繝ｩ縺ｮ豕ｨ隕也せ遘ｻ蜍輔√ち繧､繝槭・貂帛ｰ・
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


/// @brief 繝懊せ謌ｦ蜑阪・逕ｻ髱｢證苓ｻ｢繝輔ぉ繝ｼ繝峨ｒ譖ｴ譁ｰ縺吶ｋ縺溘ａ
/// @return bool: 螳御ｺ・＠縺ｦ縺・ｌ縺ｰtrue
/// @details fade_alpha_縺ｮ蠅玲ｸ・
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


/// @brief 繧ｷ繝ｧ繝・・繝輔ぉ繝ｼ繧ｺ縺ｮ繧ｿ繧､繝槭・繧堤ｮ｡逅・☆繧九◆繧・
/// @details 繧ｿ繧､繝槭・縺ｮ貂帛ｰ代・縺ｫ縺ｪ繧後・谺｡縺ｮ繝輔ぉ繝ｼ繧ｺ縺ｸ遘ｻ陦・
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


/// @brief 繝舌ヨ繝ｫ繝輔ぉ繝ｼ繧ｺ縺ｮ騾ｲ陦後ｒ邂｡逅・☆繧九◆繧・
/// @details 謨ｵ縺悟・貊・＠縺溷ｴ蜷医√す繝ｧ繝・・縺ｾ縺溘・繝懊せ縺ｸ縺ｮ遘ｻ陦悟・逅・ｒ陦後≧
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


/// @brief 繧ｷ繝ｧ繝・・繝輔ぉ繝ｼ繧ｺ縺ｸ縺ｮ遘ｻ陦悟・逅・ｒ陦後≧縺溘ａ
/// @param Phase nextPhase
/// @details 謨ｵ繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｮ蛛懈ｭ｢縲¨PC縺ｮ蜃ｺ迴ｾ縲ゞI繧ｿ繧､繝槭・縺ｮ繧ｻ繝・ヨ
void GameManager::StartShopPhase(Phase nextPhase)
{
    current_phase_ = nextPhase;
    shop_timer_ = 60 * 20;
    SendShopsIn();
}


/// @brief 繝懊せ謌ｦ蜑阪・繝輔ぉ繝ｼ繝画ｼ泌・繧帝幕蟋九☆繧九◆繧・
/// @details 繝励Ξ繧､繝､繝ｼ蛛懈ｭ｢縲√ヵ繧ｧ繝ｼ繝峨い繝ｫ繝輔ぃ蛟､縺ｮ蛻晄悄蛹・
void GameManager::StartBossTransition()
{
    current_phase_ = Phase::kFadeOutToBoss;
    fade_alpha_ = 0;
}


/// @brief 繝懊せ繧ｲ繝ｼ繝亥・迴ｾ縺ｮ貍泌・繧帝幕蟋九☆繧九◆繧・
/// @details 繧ｫ繝｡繝ｩ謫堺ｽ懊・辟｡蜉ｹ蛹悶↑縺ｩ
void GameManager::StartBossGateCutscene()
{
    Master::is_cutscene_playing_ = true;
    boss_cutscene_timer_ = 0;
    cutscene_start_pos_ = Master::player_->GetPosition();
    Master::camera_->SetCutsceneMode(true);
}


/// @brief 繧ｷ繝ｧ繝・・NPC繧偵ヵ繧｣繝ｼ繝ｫ繝峨↓蜃ｺ迴ｾ縺輔○繧九◆繧・
/// @details ObjectManager縺ｸ縺ｮStatShop霑ｽ蜉
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


/// @brief 繧ｷ繝ｧ繝・・NPC繧帝蝣ｴ縺輔○繧九◆繧・
/// @details StatShop繧ｪ繝悶ず繧ｧ繧ｯ繝医・騾蝣ｴ繝輔Λ繧ｰ譛牙柑蛹・
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


/// @brief 繧ｷ繝ｧ繝・・NPC縺梧園螳壹・菴咲ｽｮ縺ｫ蛻ｰ逹縺励◆縺句愛螳壹☆繧九◆繧・
/// @return bool: 蜈ｨ縺ｦ蛻ｰ逹縺励※縺・ｌ縺ｰtrue
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


/// @brief 迴ｾ蝨ｨ繧ｷ繝ｧ繝・・繝輔ぉ繝ｼ繧ｺ縺九←縺・°蛻､螳壹☆繧九◆繧・
/// @return bool: 繧ｷ繝ｧ繝・・繝輔ぉ繝ｼ繧ｺ縺ｪ繧液rue
bool GameManager::IsShopPhase() const
{
    return current_phase_ == Phase::kShop1 || current_phase_ == Phase::kShop2 || current_phase_ == Phase::kShop3;
}


/// @brief 迴ｾ蝨ｨ繝懊せ縺ｸ縺ｮ繝輔ぉ繝ｼ繝蛾・遘ｻ荳ｭ縺句愛螳壹☆繧九◆繧・
/// @return bool: 驕ｷ遘ｻ荳ｭ縺ｪ繧液rue
bool GameManager::IsBossFadePhase() const
{
    return current_phase_ == Phase::kFadeOutToBoss || current_phase_ == Phase::kFadeInBoss;
}


/// @brief 繝励Ξ繧､繝､繝ｼ縺後・繧ｹ繝昴・繧ｿ繝ｫ縺ｫ隗ｦ繧後◆縺句愛螳壹☆繧九◆繧・
/// @return bool: 隗ｦ繧後※縺・ｌ縺ｰtrue
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


/// @brief 繝・ヰ繝・げ繧ｳ繝ｳ繝医Ο繝ｼ繝ｫ縺梧怏蜉ｹ縺句愛螳壹☆繧九◆繧・
/// @return bool: 譛牙柑縺ｪ繧液rue
bool GameManager::IsDebugControlsEnabled() const
{
    return Master::debug_ != nullptr && Master::debug_->Getdebug();
}


/**
 * @brief 髢狗匱繝ｻ繝・ヰ繝・げ逕ｨ縺ｮ蟆ら畑蜈･蜉帛・逅・
 * @details 蟇ｩ譟ｻ繧・ユ繧ｹ繝医・繝ｬ繧､譎ゅ・蜍穂ｽ懃｢ｺ隱阪ｒ螳ｹ譏薙↓縺吶ｋ縺溘ａ縲・
 * 繝・ヰ繝・げ繝輔Λ繧ｰ縺梧怏蜉ｹ縺ｪ蝣ｴ蜷医・縺ｿ縲：5(謨ｵ蜈ｨ貊・繧Ё6(繝懊せ驕ｷ遘ｻ)縺ｪ縺ｩ縺ｮ讖溯・繧定ｨｱ蜿ｯ縺励※縺・∪縺吶・
 * 陬ｽ蜩∫沿繝薙Ν繝峨〒縺ｯ繝輔Λ繧ｰ蛻ｶ蠕｡縺ｫ繧医ｊ螳悟・縺ｫ繧ｹ繧ｭ繝・・縺輔ｌ縺ｾ縺吶・
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


/// @brief 繝・ヰ繝・げ逕ｨ・壽雰繧貞・貊・＆縺帙ｋ縺溘ａ
/// @details 蜈ｨ縺ｦ縺ｮ謨ｵ縺ｮHP繧偵ぞ繝ｭ縺ｫ縺吶ｋ
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


/// @brief 繝・ヰ繝・げ逕ｨ・壼ｼｷ蛻ｶ逧・↓繝懊せ謌ｦ縺ｸ遘ｻ陦後＆縺帙ｋ縺溘ａ
/// @details 繝輔ぉ繝ｼ繧ｺ螟画峩縺ｨ繝懊せ繝昴・繧ｿ繝ｫ縺ｮ蜃ｺ迴ｾ
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

/// @brief 繧ｲ繝ｼ繝繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｫ髢｢騾｣縺吶ｋHUD縺ｪ縺ｩ繧呈緒逕ｻ縺吶ｋ縺溘ａ
/// @details 逕ｻ髱｢謠冗判
void GameManager::Draw()
{
    DrawPhaseHud();
    DrawMinimap();
    DrawShopBanner();
    DrawBossFade();
    DrawDebugPanel();
}



/// @brief 繝・ヰ繝・げ逕ｨ縺ｮUI繝代ロ繝ｫ繧呈緒逕ｻ縺吶ｋ縺溘ａ
/// @details 逕ｻ髱｢謠冗判
void GameManager::DrawDebugPanel()
{
    if (!IsDebugControlsEnabled())
    {
        return;
    }

    int mouseX = 0;
    int mouseY = 0;
    InputManager::GetMousePos(mouseX, mouseY);

    // 繝・ヰ繝・げ繝代ロ繝ｫ縺ｮX蠎ｧ讓・
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
/// @brief 迴ｾ蝨ｨ縺ｮ繝輔ぉ繝ｼ繧ｺ縺ｮ譁・ｭ怜・繝ｩ繝吶Ν繧貞叙蠕励☆繧九◆繧・
/// @return const char*: 繝輔ぉ繝ｼ繧ｺ繝ｩ繝吶Ν
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
/// @brief 迴ｾ蝨ｨ縺ｮ繝輔ぉ繝ｼ繧ｺ縺ｮ繧ｵ繝悶Λ繝吶Ν・・ATTLE縺ｪ縺ｩ・峨ｒ蜿門ｾ励☆繧九◆繧・
/// @return const char*: 繧ｵ繝悶Λ繝吶Ν
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


/// @brief 迴ｾ蝨ｨ縺ｮ謨ｵ縺ｮ逕溷ｭ俶焚繧貞叙蠕励☆繧九◆繧・
/// @return int: 逕溷ｭ俶焚
int GameManager::GetEnemyCount() const
{
    const auto& enemies = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    return static_cast<int>(enemies.size());
}


/// @brief 迴ｾ蝨ｨ縺ｮ繝輔ぉ繝ｼ繧ｺ縺ｨ謨ｵ縺ｮ谿区焚繧堤判髱｢蜿ｳ荳翫↓謠冗判縺吶ｋ縺溘ａ
/// @details 逕ｻ髱｢謠冗判
void GameManager::DrawPhaseHud()
{
    int fontSize = GetFontSize();
    SetFontSize(24);

    // HUD繝代ロ繝ｫ縺ｮX蠎ｧ讓呻ｼ育判髱｢蜿ｳ蟇・○・・
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


/// @brief 繧ｷ繝ｧ繝・・繝輔ぉ繝ｼ繧ｺ縺ｮ谿九ｊ譎る俣繝舌リ繝ｼ繧堤判髱｢荳企Κ縺ｫ謠冗判縺吶ｋ縺溘ａ
/// @details 逕ｻ髱｢謠冗判
void GameManager::DrawShopBanner()
{
    if (!IsShopPhase())
    {
        return;
    }

    int fontSize = GetFontSize();
    SetFontSize(28);

    // 繝舌リ繝ｼ縺ｮ讓ｪ蟷・
    const int bannerW = 700;
    // 繝舌リ繝ｼ縺ｮX蠎ｧ讓呻ｼ育判髱｢荳ｭ螟ｮ・・
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


/// @brief 繝懊せ謌ｦ驕ｷ遘ｻ譎ゅ・繝輔ぉ繝ｼ繝会ｼ域囓霆｢・峨ｒ謠冗判縺吶ｋ縺溘ａ
/// @details 逕ｻ髱｢謠冗判
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

/// @brief 髮｣譏灘ｺｦ縺ｫ蠢懊§縺ｦ謨ｵ縺ｮ繧ｹ繝・・繧ｿ繧ｹ縺ｫ蛟咲紫繧帝←逕ｨ縺吶ｋ縺溘ａ
/// @param EnemyManager::enemydate& e
/// @details 蠑墓焚縺ｧ貂｡縺輔ｌ縺滓雰繝・・繧ｿ縺ｮHP繧・判謦・鴨縺ｪ縺ｩ縺ｮ螟画峩
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
    // 繝懊せ縺ｮ謨ｰ縺ｯ騾壼ｸｸ1菴薙ｒ邯ｭ謖√☆繧・
    if (e.tag != EnemyManager::boss_stage1)
    {
        e.Count = static_cast<int>(std::ceil(e.Count * countMultiplier));
        if (e.Count < 1)
        {
            e.Count = 1;
        }
    }
}


/// @brief 迴ｾ蝨ｨ縺ｮ繝輔ぉ繝ｼ繧ｺ縺ｫ蠢懊§縺滓雰繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ繧偵ヵ繧｣繝ｼ繝ｫ繝峨↓繧ｹ繝昴・繝ｳ縺輔○繧九◆繧・
/// @details EnemyManager縺ｸ縺ｮ謨ｵ霑ｽ蜉
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
        "Resource/3D繝｢繝・Ν/繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ/01_莠ｺ蝙九く繝｣繝ｩ繧ｯ繧ｿ繝ｼ繝｢繝・Ν.mv1",
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
        "Resource/3D繝｢繝・Ν/繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ/01_莠ｺ蝙九く繝｣繝ｩ繧ｯ繧ｿ繝ｼ繝｢繝・Ν.mv1",
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
        "Resource/3D繝｢繝・Ν/繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ/01_莠ｺ蝙九く繝｣繝ｩ繧ｯ繧ｿ繝ｼ繝｢繝・Ν.mv1",
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
        "Resource/3D繝｢繝・Ν/繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ/02_謨ｵ繝｢繝ｳ繧ｹ繧ｿ繝ｼ繝｢繝・Ν.mv1",
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
        "Resource/3D繝｢繝・Ν/繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ/01_莠ｺ蝙九く繝｣繝ｩ繧ｯ繧ｿ繝ｼ繝｢繝・Ν.mv1",
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
        "Resource/3D繝｢繝・Ν/繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ/01_莠ｺ蝙九く繝｣繝ｩ繧ｯ繧ｿ繝ｼ繝｢繝・Ν.mv1",
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
        "Resource/3D繝｢繝・Ν/繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ/03_繝懊せ繝｢繝・Ν.mv1",
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

/// @brief 繝溘ル繝槭ャ繝励ｒ謠冗判縺吶ｋ縺溘ａ
/// @details 逕ｻ髱｢謠冗判
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
