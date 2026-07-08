#pragma once
#include "Effect.h"
#include <vector>

class EffectPool {
private:
    static const int POOL_SIZE = 50;
    Effect* mPool[POOL_SIZE];
    
    static EffectPool* sInstance;

public:
    static EffectPool* GetInstance();
    
    EffectPool();
    ~EffectPool();
    
    void Update();
    void Draw();
    
    void Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime);
};
