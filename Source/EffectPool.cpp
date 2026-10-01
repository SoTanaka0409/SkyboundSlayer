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
        m_Pool[i] = new Effect();
    }
}

EffectPool::~EffectPool() {
    for (int i = 0; i < POOL_SIZE; i++) {
        delete m_Pool[i];
    }
}


/// @brief EffectPoolの状態更新処理
void EffectPool::Update() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (m_Pool[i]->IsActive()) {
            m_Pool[i]->Update();
        }
    }
}


/// @brief EffectPoolの描画処理
void EffectPool::Draw() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (m_Pool[i]->IsActive()) {
            m_Pool[i]->Draw();
        }
    }
}


/// @brief EffectPoolのPlay処理
void EffectPool::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!m_Pool[i]->IsActive()) {
            m_Pool[i]->Play(initPos, filename, Changecolor, Size, VisibleTime);
            return;
        }
    }
}
