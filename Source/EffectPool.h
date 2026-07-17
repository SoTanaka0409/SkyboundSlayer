#pragma once
#include "Effect.h"
#include <vector>

class EffectPool {
private:
    static const int POOL_SIZE = 50;
    Effect* pool_[POOL_SIZE];
    
    static EffectPool* sInstance;

public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    static EffectPool* GetInstance();
    
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    EffectPool();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    ~EffectPool();
    
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void Update();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void Draw();
    
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime);
};
