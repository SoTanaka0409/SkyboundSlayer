#include "EffectU.h"
#include "EffekseerManager.h"

EffectU::~EffectU()
{
    if (playing_handle_ != -1) {
        EffekseerManager::GetInstance()->StopEffect(playing_handle_);
        playing_handle_ = -1;
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
    if (playing_handle_ != -1) {
        EffekseerManager::GetInstance()->StopEffect(playing_handle_);
    }
    is_holding_ = true;

    VECTOR pos = playerPos;
    pos.y += y_offset_;
    
    // エフェクト再生開始
    playing_handle_ = EffekseerManager::GetInstance()->PlayEffect("Mahoujin", pos);
}

void EffectU::ReleaseAndShatter()
{
    if (!is_holding_) return;
    
    // ボタンを離したときのエフェクト終了
    if (playing_handle_ != -1) {
        EffekseerManager::GetInstance()->StopEffect(playing_handle_);
        playing_handle_ = -1;
    }
    
    is_holding_ = false;
}

void EffectU::UpdateFollow(const VECTOR& playerPos)
{
    if (is_holding_ && playing_handle_ != -1)
    {
        VECTOR pos = playerPos;
        pos.y += y_offset_;
        
        // 再生中かチェックし、終了していたらハンドルをリセット
        if (EffekseerManager::GetInstance()->IsPlaying(playing_handle_)) {
            EffekseerManager::GetInstance()->SetEffectPosition(playing_handle_, pos);
        } else {
            playing_handle_ = -1;
        }
    }
}

void EffectU::Draw() const
{
    // Effekseerのエフェクト描画はEffekseerManagerの管轄なので、ここでは何もしない
}