#include "EffectU.h"
#include "EffekseerManager.h"

EffectU::~EffectU()
{
    if (mPlayingHandle != -1) {
        EffekseerManager::GetInstance()->StopEffect(mPlayingHandle);
        mPlayingHandle = -1;
    }
}

bool EffectU::Load()
{
    // Effekseerのエフェクトをロード
    EffekseerManager::GetInstance()->LoadEffect("Mahoujin", "Resource/effect/MAGICAL/魔法陣.efkproj", 1.0f);
    return true;
}

void EffectU::StartHold(const VECTOR& playerPos)
{
    // 既に再生中の場合は停止
    if (mPlayingHandle != -1) {
        EffekseerManager::GetInstance()->StopEffect(mPlayingHandle);
    }
    mIsHolding = true;

    VECTOR pos = playerPos;
    pos.y += mYOffset;
    
    // エフェクト再生開始
    mPlayingHandle = EffekseerManager::GetInstance()->PlayEffect("Mahoujin", pos);
}

void EffectU::ReleaseAndShatter()
{
    if (!mIsHolding) return;
    
    // ボタンを離したときのエフェクト終了
    if (mPlayingHandle != -1) {
        EffekseerManager::GetInstance()->StopEffect(mPlayingHandle);
        mPlayingHandle = -1;
    }
    
    mIsHolding = false;
}

void EffectU::UpdateFollow(const VECTOR& playerPos)
{
    if (mIsHolding && mPlayingHandle != -1)
    {
        VECTOR pos = playerPos;
        pos.y += mYOffset;
        
        // 再生中かチェックし、終了していたらハンドルをリセット
        if (EffekseerManager::GetInstance()->IsPlaying(mPlayingHandle)) {
            EffekseerManager::GetInstance()->SetEffectPosition(mPlayingHandle, pos);
        } else {
            mPlayingHandle = -1;
        }
    }
}

void EffectU::Draw() const
{
    // Effekseerのエフェクト描画はEffekseerManagerの管轄なので、ここでは何もしない
}