#include "EffectPool.h"

EffectPool* EffectPool::sInstance = nullptr;

EffectPool* EffectPool::GetInstance() {
    if (!sInstance) {
        sInstance = new EffectPool();
    }
    return sInstance;
}

EffectPool::EffectPool() {
    for (int i = 0; i < POOL_SIZE; i++) {
        mPool[i] = new Effect();
    }
}

EffectPool::~EffectPool() {
    for (int i = 0; i < POOL_SIZE; i++) {
        delete mPool[i];
    }
}

void EffectPool::Update() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (mPool[i]->IsActive()) {
            mPool[i]->Update();
        }
    }
}

void EffectPool::Draw() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (mPool[i]->IsActive()) {
            mPool[i]->Draw();
        }
    }
}

void EffectPool::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!mPool[i]->IsActive()) {
            mPool[i]->Play(initPos, filename, Changecolor, Size, VisibleTime);
            return;
        }
    }
}
