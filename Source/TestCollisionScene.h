#pragma once
#include "Scene.h"
#include "DxLib.h"
#include"SceneGame.h"
struct TrackedVec3 {
    VECTOR current;
    VECTOR previous;
};

class TestCollisionScene : public SceneGame 
{
public:
    TestCollisionScene();
    ~TestCollisionScene();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize() override;

private:
    TrackedVec3 mPlayerPos;
    VECTOR mPlayerVelocity;
    float mPlayerRadius;
    bool mIsGrounded;

    VECTOR mBlockPos;
    VECTOR mBlockSize;
    int mPlayerModelHandle;

    // Top-only collision function
    bool CheckTopOnlyCollision(const TrackedVec3& playerPos, float playerRadius, const VECTOR& blockPos, const VECTOR& blockSize, float& outHitY);
};
