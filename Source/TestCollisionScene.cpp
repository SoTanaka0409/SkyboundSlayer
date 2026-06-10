#include "TestCollisionScene.h"

TestCollisionScene::TestCollisionScene() {
}

TestCollisionScene::~TestCollisionScene() {
}

void TestCollisionScene::Initialize() {
    // Player settings (starts high up)
    mPlayerPos.current = VGet(0.0f, 300.0f, 0.0f);
    mPlayerPos.previous = mPlayerPos.current;
    mPlayerVelocity = VGet(0.0f, 0.0f, 0.0f);
    mPlayerRadius = 15.0f;
    mIsGrounded = false;

    // Block settings
    mBlockPos = VGet(0.0f, 100.0f, 0.0f);
    mBlockSize = VGet(500.0f, 20.0f, 500.0f); // Half extents: 500 in X/Z, 20 in Y

    // Load Player Model
    mPlayerModelHandle = MV1LoadModel("Resource/3D/Hero.mv1");
}

void TestCollisionScene::Update() {
    // Save previous position
    mPlayerPos.previous = mPlayerPos.current;

    // Apply gravity
    mPlayerVelocity.y -= 0.5f; // Gravity strength

    // Input for moving around (optional, to test edge cases)
    if (CheckHitKey(KEY_INPUT_LEFT))  mPlayerVelocity.x -= 0.5f;
    if (CheckHitKey(KEY_INPUT_RIGHT)) mPlayerVelocity.x += 0.5f;
    if (CheckHitKey(KEY_INPUT_UP))    mPlayerVelocity.z += 0.5f;
    if (CheckHitKey(KEY_INPUT_DOWN))  mPlayerVelocity.z -= 0.5f;

    // Friction
    mPlayerVelocity.x *= 0.9f;
    mPlayerVelocity.z *= 0.9f;

    // Jump
    if (mIsGrounded && CheckHitKey(KEY_INPUT_SPACE)) {
        mPlayerVelocity.y = 12.0f;
        mIsGrounded = false;
    }

    // Update current position based on velocity
    mPlayerPos.current = VAdd(mPlayerPos.current, mPlayerVelocity);

    // Collision detection
    mIsGrounded = false;
    float hitY = 0.0f;

    // Top-Only collision check: Only check if player is falling (velocity.y <= 0)
    if (mPlayerVelocity.y <= 0.0f) { 
        if (CheckTopOnlyCollision(mPlayerPos, mPlayerRadius, mBlockPos, mBlockSize, hitY)) {
            // Correct position to exactly top of the floor
            mPlayerPos.current.y = hitY;
            mPlayerVelocity.y = 0.0f;
            mIsGrounded = true;
        }
    }

    // Fall out of bounds reset
    if (mPlayerPos.current.y < -200.0f) {
        mPlayerPos.current = VGet(0.0f, 400.0f, 0.0f);
        mPlayerVelocity = VGet(0.0f, 0.0f, 0.0f);
    }
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
    VECTOR camPos = VGet(mPlayerPos.current.x, mPlayerPos.current.y + 150.0f, mPlayerPos.current.z - 300.0f);
    SetCameraPositionAndTarget_UpVecY(camPos, mPlayerPos.current);

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
}

void TestCollisionScene::Finalize() {
    MV1DeleteModel(mPlayerModelHandle);
}
