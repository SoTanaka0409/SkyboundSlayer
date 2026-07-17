#include "EffectPool.h"

EffectPool* EffectPool::sInstance = nullptr;

EffectPool* 
/*
 * 目的（EffectPoolのGetInstance処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
EffectPool::GetInstance() {
    if (!sInstance) {
        sInstance = new EffectPool();
    }
    return sInstance;
}


/*
 * 目的（EffectPoolのEffectPool処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（EffectPoolのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EffectPool::Update() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (pool_[i]->IsActive()) {
            pool_[i]->Update();
        }
    }
}


/*
 * 目的（EffectPoolのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EffectPool::Draw() {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (pool_[i]->IsActive()) {
            pool_[i]->Draw();
        }
    }
}


/*
 * 目的（EffectPoolのPlay処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EffectPool::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!pool_[i]->IsActive()) {
            pool_[i]->Play(initPos, filename, Changecolor, Size, VisibleTime);
            return;
        }
    }
}
