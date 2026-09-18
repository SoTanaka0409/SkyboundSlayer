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
    // Load the hold effect resource.
    EffekseerManager::GetInstance()->LoadEffect("Mahoujin", "Resource/effect/magic/02_magic_effect_playback.efk", 1.0f);
    return true;
}

void EffectU::StartHold(const VECTOR& playerPos)
{
    if (playing_handle_ != -1) {
        EffekseerManager::GetInstance()->StopEffect(playing_handle_);
    }
    is_holding_ = true;

    VECTOR pos = playerPos;
    pos.y += y_offset_;

    playing_handle_ = EffekseerManager::GetInstance()->PlayEffect("Mahoujin", pos);
}

void EffectU::ReleaseAndShatter()
{
    if (!is_holding_) return;

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

        if (EffekseerManager::GetInstance()->IsPlaying(playing_handle_)) {
            EffekseerManager::GetInstance()->SetEffectPosition(playing_handle_, pos);
        }
        else {
            playing_handle_ = -1;
        }
    }
}

void EffectU::Draw() const
{
    // Load the hold effect resource.
}