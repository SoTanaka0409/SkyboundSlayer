#include "EffectU.h"
#include "EffekseerManager.h"

EffectU::~EffectU()
{
    if (m_PlayingHandle != -1) {
        EffekseerManager::GetInstance()->StopEffect(m_PlayingHandle);
        m_PlayingHandle = -1;
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
    if (m_PlayingHandle != -1) {
        EffekseerManager::GetInstance()->StopEffect(m_PlayingHandle);
    }
    m_IsHolding = true;

    VECTOR pos = playerPos;
    pos.y += m_YOffset;

    m_PlayingHandle = EffekseerManager::GetInstance()->PlayEffect("Mahoujin", pos);
}

void EffectU::ReleaseAndShatter()
{
    if (!m_IsHolding) return;

    if (m_PlayingHandle != -1) {
        EffekseerManager::GetInstance()->StopEffect(m_PlayingHandle);
        m_PlayingHandle = -1;
    }
    m_IsHolding = false;
}

void EffectU::UpdateFollow(const VECTOR& playerPos)
{
    if (m_IsHolding && m_PlayingHandle != -1)
    {
        VECTOR pos = playerPos;
        pos.y += m_YOffset;

        if (EffekseerManager::GetInstance()->IsPlaying(m_PlayingHandle)) {
            EffekseerManager::GetInstance()->SetEffectPosition(m_PlayingHandle, pos);
        }
        else {
            m_PlayingHandle = -1;
        }
    }
}

void EffectU::Draw() const
{
    // Load the hold effect resource.
}