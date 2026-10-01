#include "EffekseerObject.h"
#include "EffekseerManager.h"


/// @brief EffekseerObjectの初期化（コンストラクタ）
EffekseerObject::EffekseerObject(const std::string& name, const char* filepath, VECTOR initPos, Object3D* parent, bool isFollow, float magnification, float speed)
	: Object3D(initPos)
	, m_Parent(parent)
	, m_IsFollow(isFollow)
	, m_PlayingHandle(-1)
{
	
	EffekseerManager::GetInstance()->LoadEffect(name, filepath, magnification);

	// エフェクトの再生
	m_PlayingHandle = EffekseerManager::GetInstance()->PlayEffect(name, initPos);
	EffekseerManager::GetInstance()->SetEffectSpeed(m_PlayingHandle, speed);

	if (m_Parent != nullptr)
	{
		// ターゲットとの相対オフセットを保持
		m_Offset = VSub(initPos, m_Parent->GetPosition());
	}
	else
	{
		m_Offset = VGet(0, 0, 0);
	}
}

EffekseerObject::~EffekseerObject()
{
	if (m_PlayingHandle != -1 && EffekseerManager::GetInstance()->IsPlaying(m_PlayingHandle))
	{
		EffekseerManager::GetInstance()->StopEffect(m_PlayingHandle);
	}
}


/// @brief EffekseerObjectの状態更新処理
void EffekseerObject::Update()
{
	if (m_PlayingHandle == -1 || !EffekseerManager::GetInstance()->IsPlaying(m_PlayingHandle))
	{
		// 再生終了時に自動削除
		SetDeleteFlag(true);
		return;
	}

	if (m_IsFollow && m_Parent != nullptr)
	{
		if (m_Parent->IsDeleteFlag())
		{
			// 親が削除された場合は追従を停止
			m_Parent = nullptr;
		}
		else
		{
			// 親に追従して座標を更新
			m_Position = VAdd(m_Parent->GetPosition(), m_Offset);
			EffekseerManager::GetInstance()->SetEffectPosition(m_PlayingHandle, m_Position);
		}
	}
}


/// @brief EffekseerObjectの描画処理
void EffekseerObject::Draw()
{
	// Handled by EffekseerManager::Draw()
}
