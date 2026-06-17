#include "TestCollisionScene.h"
#include"Player3D.h"
TestCollisionScene::TestCollisionScene()
{
   
}

TestCollisionScene::~TestCollisionScene() {
}

void TestCollisionScene::Initialize() {
    // Player settings (starts high up)
    new Player3D("Resource/Model/T.mv1", VGet(-1200, 2000.0f, 0), 30.0f, 12.0f, 150.0f, true);//ジャンプ、アタック、スピード、ｈｐ

    // Block settings
    mBlockPos = VGet(0.0f, 100.0f, 0.0f);
    mBlockSize = VGet(500.0f, 20.0f, 500.0f); // Half extents: 500 in X/Z, 20 in Y

    // Load Player Model
    mPlayerModelHandle = MV1LoadModel("Resource/3D/Hero.mv1");
    // Wave 3: 重量級代用
    mpEnemyManager = new EnemyManager();
    EnemyManager::enemydate e_heavy;
    e_heavy.filename = "Resource/Model/monster.mv1";
    e_heavy.spawnCenter = VGet(0, 0, 0);
    e_heavy.initPos = VGet(2000.0f, 100.0f, 2000.0f);
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
    e_heavy.Count = 1;
    mpEnemyManager->NewEnemyList(e_heavy);
  
}

void TestCollisionScene::Update() {
    // Save previous position
    Scene::Update();
}

bool TestCollisionScene::CheckTopOnlyCollision(const TrackedVec3& playerPos, float playerRadius, const VECTOR& blockPos, const VECTOR& blockSize, float& outHitY) {
    // Calculate block boundaries
    float blockTop = blockPos.y + blockSize.y;
    float blockMinX = blockPos.x - blockSize.x;
    float blockMaxX = blockPos.x + blockSize.x;
    float blockMinZ = blockPos.z - blockSize.z;
    float blockMaxZ = blockPos.z + blockSize.z;

    // 1. AABB intersection in X and Z
    // Check if the player's XZ bounds overlap with the block's XZ bounds
    if (playerPos.current.x + playerRadius < blockMinX || playerPos.current.x - playerRadius > blockMaxX ||
        playerPos.current.z + playerRadius < blockMinZ || playerPos.current.z - playerRadius > blockMaxZ) {
        return false; // Out of horizontal bounds
    }

    // 2. Top collision logic using previous frame position to prevent falling through completely
    float prevBottom = playerPos.previous.y - playerRadius;
    float currBottom = playerPos.current.y - playerRadius;

    // If previously we were strictly above or at the top of the block, 
    // and now we are below or exactly at the top of the block...
    if (prevBottom >= blockTop && currBottom <= blockTop) {
        // Snap the bottom of the player to the top of the block
        outHitY = blockTop + playerRadius;
        return true;
    }

    return false;
}

void TestCollisionScene::Draw() {
    // Camera setup
  

    // Draw Floor Block (Floating)
    VECTOR blockPos1 = VGet(mBlockPos.x - mBlockSize.x, mBlockPos.y - mBlockSize.y, mBlockPos.z - mBlockSize.z);
    VECTOR blockPos2 = VGet(mBlockPos.x + mBlockSize.x, mBlockPos.y + mBlockSize.y, mBlockPos.z + mBlockSize.z);
    
    // Solid fill
    DrawCube3D(blockPos1, blockPos2, GetColor(80, 80, 80), GetColor(255, 255, 255), TRUE);
    // Wireframe for edges
    DrawCube3D(blockPos1, blockPos2, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);

    // Draw Player Model (Adjust Y so feet touch the collision bottom)
    VECTOR drawPos = mPlayerPos.current;
    drawPos.y -= mPlayerRadius;
    MV1SetPosition(mPlayerModelHandle, drawPos);
    
    // The player's rotation can be matched to velocity (optional). For now, just draw.
    MV1DrawModel(mPlayerModelHandle);

    // Debug text
    DrawFormatString(10, 10, GetColor(255,255,0), "Top-Only Collision Demo");
    DrawFormatString(10, 30, GetColor(255,255,255), "Player Y: %.2f", mPlayerPos.current.y);
    DrawFormatString(10, 50, GetColor(255,255,255), "Grounded: %s", mIsGrounded ? "TRUE" : "FALSE");
    DrawFormatString(10, 70, GetColor(255,255,255), "Block Top: %.2f", mBlockPos.y + mBlockSize.y);
    DrawFormatString(10, 100, GetColor(200,200,200), "Controls: Arrow Keys to move, SPACE to jump");
    // 地面のグリッド（ステージ）を描画
    const int count = 51;
    const float distance = 500.0f;
    for (int i = 0; i < count; i++)
    {
        float base = (count / 2 - i) * -distance;

        DrawLine3D(
            VGet(-distance * (count / 2), 0.0f, base),
            VGet(distance * (count / 2), 0.0f, base),
            GetColor(255, 255, 255)
        );

        DrawLine3D(
            VGet(base, 0.0f, -distance * (count / 2)),
            VGet(base, 0.0f, distance * (count / 2)),
            GetColor(255, 255, 255)
        );
    }
    Scene::Draw();
}

void TestCollisionScene::Finalize() 
{
    MV1DeleteModel(mPlayerModelHandle);
}
