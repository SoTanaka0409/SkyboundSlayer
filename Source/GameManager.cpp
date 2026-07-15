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

GameManager::GameManager(EnemyManager* enemyManager, Difficulty diff)
    	: mpEnemyManager(enemyManager), mDifficulty(diff), mCurrentPhase(Phase::PHASE_1), mShopTimer(0), mFadeAlpha(0), mBossPortalPos(VGet(0,0,0)), mBossCutsceneTimer(0), mCutsceneStartPos(VGet(0,0,0))
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
    mBossPortalPos = VAdd(center, VScale(dir, -5000.0f));
    
    
    // Portal Base
    float portalSize = 100.0f;
    new Stage(VAdd(mBossPortalPos, VGet(0.0f, -570.0f, 0.0f)), "Resource/3D/portal/source/portal.mv1", "Resource/3D/portal/source/portal.mv1", VGet(portalSize, portalSize, portalSize));
    
    // Portal Magic Circle Effect (slightly above the portal base to prevent clipping)
    new EffekseerObject("Mahoujin", "Resource/effect/MAGICAL/Mahoujin.efkproj", VAdd(mBossPortalPos, VGet(0.0f, -565.0f, 0.0f)), nullptr, true, 1.0f, 1.0f);
}

GameManager::~GameManager()
{
}

void GameManager::Update()
{
    if (Master::CutscenePlaying) {
        mBossCutsceneTimer++;
        float t = (float)mBossCutsceneTimer / 180.0f; // 3 seconds to move
        if (t > 1.0f) t = 1.0f;
        float easeT = t * t * (3.0f - 2.0f * t); // smoothstep
        VECTOR currentPos = VAdd(VScale(mCutsceneStartPos, 1.0f - easeT), VScale(mBossPortalPos, easeT));
        Master::mpCamera->SetCutsceneTarget(currentPos);

        if (mBossCutsceneTimer > 240) { // 3s move + 1s hold
            Master::CutscenePlaying = false;
            Master::mpCamera->SetCutsceneMode(false);
        }
        return;
    }
    // DEBUG: Press '0' to wipe out all enemies in the current phase
    if (CheckHitKey(KEY_INPUT_0)) {
        const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
        for (auto enemy : enemies)
		{
			Enemy* e = enemy->CastTo<Enemy>();
            if (e) {
                e->Damage(e->GetMaxHp()); // Deal max HP damage to trigger death animation
            }
        }
    }

    // DEBUG: Press 'P' to instantly skip the current phase
    if (InputManager::CheckDownKey(KEY_INPUT_P)) {
        const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
        for (auto enemy : enemies) {
            Enemy* e = enemy->CastTo<Enemy>();
            if (e) {
                e->Delete();
            }
            enemy->SetDeleteFlag(true);
        }

        if (mCurrentPhase == Phase::SHOP_1 || mCurrentPhase == Phase::SHOP_2) {
            mShopTimer = 1;
        } else if (mCurrentPhase == Phase::SHOP_3) {
            mCurrentPhase = Phase::FADE_OUT_TO_BOSS;
            mFadeAlpha = 0;
            if (Master::mpSoundManager) {
                Master::mpSoundManager->PlaySE(SoundManager::SE_WARP);
            }
        }
    }

    if (mCurrentPhase == Phase::FADE_OUT_TO_BOSS) {
        mFadeAlpha += 5;
        if (mFadeAlpha >= 255) {
            mFadeAlpha = 255;
            mCurrentPhase = Phase::BOSS; // �時的にBOSSにして出現させ�
            SpawnPhaseEnemies(); // ここでボス出現

            mCurrentPhase = Phase::FADE_IN_BOSS;
            
            // ボスエリアへプレイヤーをワープさせる
            Master::mpPlayer->SetPosition(VAdd(Config::GetStageBossCenter(), VGet(500.0f, 0.0f, -2000.0f)));
            if (Master::mpSoundManager) {
                Master::mpSoundManager->PlaySE(SoundManager::SE_WARP);
            }
        }
        return; // フェード中は他�更新をスキ��
    } else if (mCurrentPhase == Phase::FADE_IN_BOSS) {
        mFadeAlpha -= 5;
        if (mFadeAlpha <= 0) {
            mFadeAlpha = 0;
            mCurrentPhase = Phase::BOSS;
        }
        return; // フェード中は他�更新をスキ��
    }

    if (mCurrentPhase == Phase::SHOP_1 || mCurrentPhase == Phase::SHOP_2 || mCurrentPhase == Phase::SHOP_3)
    {
        if (mCurrentPhase != Phase::SHOP_3) {
            bool allShopsArrived = true;
            const auto& shops = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
            for (auto s : shops) {
                StatShop* shop = s->CastTo<StatShop>();
                if (shop && !shop->IsArrived()) {
                    allShopsArrived = false;
                    break;
                }
            }

            if (allShopsArrived) {
                mShopTimer--;
            }

            if (mShopTimer <= 0) {
                for (auto s : shops) {
                    StatShop* shop = s->CastTo<StatShop>();
                    if (shop) shop->StartWalkingOut();
                }

                if (mCurrentPhase == Phase::SHOP_1) {
                    mCurrentPhase = Phase::PHASE_2;
                } else if (mCurrentPhase == Phase::SHOP_2) {
                    mCurrentPhase = Phase::PHASE_3;
                }
                SpawnPhaseEnemies();

            }
        } 
        else
        {
            // SHOP_3: No time limit. Wait for player to enter teleporter.
            auto p = Master::mpPlayer;
            if (p) {
                Player3D* player = p->CastTo<Player3D>();
                VECTOR playerPos = player->GetPosition();
                
                // Placeholder teleporter position (center of stage, offset)
                VECTOR teleporterPos = mBossPortalPos;
                
                float dist = VSize(VSub(playerPos, teleporterPos));
                if (dist < 150.0f) { // 150 radius to enter
                    const auto& shops = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
                    for (auto s : shops) {
                        StatShop* shop = s->CastTo<StatShop>();
                        if (shop) shop->StartWalkingOut();
                    }
                    mCurrentPhase = Phase::FADE_OUT_TO_BOSS;
                    mFadeAlpha = 0;
                }
            }
        }
    } else {
        const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
        if (enemies.empty())
        {
            Phase oldPhase = mCurrentPhase;
            if (mCurrentPhase == Phase::PHASE_1) {
                mCurrentPhase = Phase::SHOP_1;
                mShopTimer = 60 * 20; // 20遘�
            } else if (mCurrentPhase == Phase::PHASE_2) {
                mCurrentPhase = Phase::SHOP_2;
                mShopTimer = 60 * 20; // 20遘�
                        } else if (mCurrentPhase == Phase::PHASE_3) {
                mCurrentPhase = Phase::SHOP_3;
                mShopTimer = 60 * 20; // 20秒
                Master::CutscenePlaying = true;
                mBossCutsceneTimer = 0;
                mCutsceneStartPos = Master::mpPlayer->GetPosition();
                Master::mpCamera->SetCutsceneMode(true);
            } else if (mCurrentPhase == Phase::BOSS) {
                mCurrentPhase = Phase::CLEAR;
            }

            if (oldPhase != mCurrentPhase && (mCurrentPhase == Phase::SHOP_1 || mCurrentPhase == Phase::SHOP_2 || mCurrentPhase == Phase::SHOP_3)) {
                const auto& shops = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Shop);
                for (auto s : shops) {
                    StatShop* shop = s->CastTo<StatShop>();
                    if (shop) shop->StartWalkingIn();
                }
            }
        }
    }
}

void GameManager::Draw()
{
    int fontSize = GetFontSize();
    SetFontSize(30);

    const char* phaseStr = "";
    switch (mCurrentPhase) {
    case Phase::PHASE_1: phaseStr = "Phase 1"; break;
    case Phase::PHASE_2: phaseStr = "Phase 2"; break;
    case Phase::PHASE_3: phaseStr = "Phase 3"; break;
    case Phase::BOSS:    phaseStr = "BOSS Phase"; break;
    case Phase::SHOP_1:  phaseStr = "Shop 1"; break;
    case Phase::SHOP_2:  phaseStr = "Shop 2"; break;
    case Phase::SHOP_3:  phaseStr = "Shop 3"; break;
    case Phase::CLEAR:   phaseStr = "CLEAR!"; break;
    }

    const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    int enemyCount = (int)enemies.size();

    DrawFormatString(20, 100, GetColor(255, 255, 255), "Current Phase: %s", phaseStr);
    DrawFormatString(20, 140, GetColor(255, 255, 255), "Enemies Remaining: %d", enemyCount);

    SetFontSize(fontSize);

    DrawMinimap();

    if (mCurrentPhase == Phase::SHOP_1 || mCurrentPhase == Phase::SHOP_2 || mCurrentPhase == Phase::SHOP_3) {
        if (mCurrentPhase != Phase::SHOP_3) {
            int seconds = mShopTimer / 60;
            DrawFormatString(1920 / 2 - 150, 50, GetColor(255, 255, 0), "SHOP PHASE - Next Wave in %d s", seconds);
        } else {
            DrawFormatString(1920 / 2 - 350, 50, GetColor(0, 255, 255), "SHOP PHASE - Enter the blue teleporter to start BOSS BATTLE");
            
            // Draw placeholder teleporter
            VECTOR teleporterPos = mBossPortalPos;
            // DrawCapsule3D(teleporterPos, VAdd(teleporterPos, VGet(0.0f, 200.0f, 0.0f)), 150.0f, 32, GetColor(0, 150, 255), GetColor(0, 150, 255), FALSE);
            // DrawSphere3D(VAdd(teleporterPos, VGet(0.0f, 50.0f, 0.0f)), 100.0f, 32, GetColor(0, 255, 255), GetColor(0, 255, 255), FALSE);
        }
    }

    if (mCurrentPhase == Phase::FADE_OUT_TO_BOSS || mCurrentPhase == Phase::FADE_IN_BOSS) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, mFadeAlpha);
        DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void GameManager::ApplyDifficultyMultipliers(EnemyManager::enemydate& e)
{
    float statMultiplier = 1.0f;
    float countMultiplier = 1.0f;

    switch (mDifficulty) {
    case Difficulty::EASY:
        statMultiplier = 0.8f;
        countMultiplier = 0.8f;
        break;
    case Difficulty::NORMAL:
        statMultiplier = 1.0f;
        countMultiplier = 1.0f;
        break;
    case Difficulty::HARD:
        statMultiplier = 1.5f;
        countMultiplier = 1.5f;
        break;
    }

    e.hp = static_cast<int>(e.hp * statMultiplier);
    e.attack = static_cast<int>(e.attack * statMultiplier);
    // boss count should remain 1 usually
    if (e.tag != EnemyManager::boss_stage1) {
        e.Count = static_cast<int>(std::ceil(e.Count * countMultiplier));
        if (e.Count < 1) e.Count = 1;
    }
}

void GameManager::SpawnPhaseEnemies()
{
auto p = Master::mpPlayer;
    Player3D* player = p->CastTo<Player3D>();
    // 謨��縺後せ繝�・繧��縺九ｉ螟悶��縺��關ｽ荳九�蝓区����縺励↑縺・��縺・↓縲��onfig縺��繧��繝�・繧��荳��蠢・����讓吶��貉ｧ縺堺��咲����縺��蝓ｺ�悶→縺吶�
    VECTOR centerPos = Config::GetStageCenter();
    if (player != nullptr)
    {
        VECTOR playercenterPos = player->GetPosition();
    }
    VECTOR centerbackPos = VAdd(Config::GetStageCenter(), VGet(0, 0, -2000));

    if (mCurrentPhase == Phase::PHASE_1) {
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
        e.xp = 30.0f;
        e.money = 200;
        e.tag = EnemyManager::night_stage1;
        e.Count = 10;
        
        ApplyDifficultyMultipliers(e);
        mpEnemyManager->NewEnemyList(e);
    }
    else if (mCurrentPhase == Phase::PHASE_2) {
        // Wave 2: 鬲疲��募�螢��
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
        e1.xp = 30.0f;
        e1.tag = EnemyManager::archerl_stage1;
        e1.Count = 8;
        
        ApplyDifficultyMultipliers(e1);
        mpEnemyManager->NewEnemyList(e1);

        // Wave 2: 霑第磁蜑��螢��
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
        e2.xp = 30.0f;
        e2.money = 200;
        e2.tag = EnemyManager::night_stage1;
        e2.Count = 6;

        ApplyDifficultyMultipliers(e2);
        mpEnemyManager->NewEnemyList(e2);
    }
    else if (mCurrentPhase == Phase::PHASE_3) {
        // Wave 3: 驥埼㍼邏壻����逕ｨ
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
        e_heavy.xp = 100.0f;
        e_heavy.money = 500;
        e_heavy.tag = EnemyManager::monster_stage1;
        e_heavy.Count = 5;

        ApplyDifficultyMultipliers(e_heavy);
        mpEnemyManager->NewEnemyList(e_heavy);

        // Wave 3: 鬲疲��募�螢��
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
        e_magic.xp = 30.0f;
        e_magic.money = 200;
        e_magic.tag = EnemyManager::archerl_stage1;
        e_magic.Count = 5;

        ApplyDifficultyMultipliers(e_magic);
        mpEnemyManager->NewEnemyList(e_magic);

        // Wave 3: 霑第磁蜈��螢��
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
        e_melee.xp = 30.0f;
        e_melee.money = 200;
        e_melee.tag = EnemyManager::night_stage1;
        e_melee.Count = 5;

        ApplyDifficultyMultipliers(e_melee);
        mpEnemyManager->NewEnemyList(e_melee);
    }
    else if (mCurrentPhase == Phase::BOSS) {
        EnemyManager::enemydate e2;
        e2.filename = "Resource/3D/Boss1.mv1";
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
        e2.xp = 300.0f;
        e2.tag = EnemyManager::boss_stage1;
        e2.Count = 1;

        ApplyDifficultyMultipliers(e2);
        mpEnemyManager->NewEnemyList(e2);
    }
}
void GameManager::DrawMinimap()
{
    // Minimap dimensions and position
    const float mapSize = 250.0f;
    const float mapX = 1920.0f - mapSize - 20.0f;
    const float mapY = 20.0f;
    
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
    VECTOR portalOffset = GetMapOffset(mBossPortalPos);
    bool portalFar = (abs(portalOffset.x) > maxD || abs(portalOffset.y) > maxD);
    VECTOR portalClamped = ClampToEdge(portalOffset);
    DrawCircle((int)(mapCenterX + portalClamped.x), (int)(mapCenterY + portalClamped.y), 6, GetColor(0, 255, 255), TRUE);
    if (portalFar) {
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



