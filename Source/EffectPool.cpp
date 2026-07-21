#include "EffectPool.h"

EffectPool* EffectPool::sInstance = nullptr;

EffectPool* 
/// @brief EffectPoolのGetInstance処理
EffectPool::GetInstance() {
    if (!sInstance) {
        sInstance = new EffectPool();
    }
    return sInstance;
}


/// @brief EffectPoolの初期化（コンストラクタ）
EffectPool::EffectPool() {
    for (int i = 0; i < POOL_SIZE; i++) {
        pool_[i] = new Effect();
    }
}

EffectPool::~EffectPool() {
    for (int i = 0; i < POOL_SIZE; i++) {
        delete pool_[i];
    }
}


/// @brief EffectPoolの状態更新処理
void EffectPool::Update() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (pool_[i]->IsActive()) {
            pool_[i]->Update();
        }
    }
}


/// @brief EffectPoolの描画処理
void EffectPool::Draw() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (pool_[i]->IsActive()) {
            pool_[i]->Draw();
        }
    }
}


/// @brief EffectPoolのPlay処理
void EffectPool::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!pool_[i]->IsActive()) {
            pool_[i]->Play(initPos, filename, Changecolor, Size, VisibleTime);
            return;
        }
    }
}
