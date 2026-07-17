#include "EffekseerObject.h"
#include "EffekseerManager.h"


/*
 * 目的（EffekseerObjectのEffekseerObject処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
EffekseerObject::EffekseerObject(const std::string& name, const char* filepath, VECTOR initPos, Object3D* parent, bool isFollow, float magnification, float speed)
	: Object3D(initPos)
	, parent_(parent)
	, is_follow_(isFollow)
	, playing_handle_(-1)
{
	
	EffekseerManager::GetInstance()->LoadEffect(name, filepath, magnification);

	// エフェクトの再生
	playing_handle_ = EffekseerManager::GetInstance()->PlayEffect(name, initPos);
	EffekseerManager::GetInstance()->SetEffectSpeed(playing_handle_, speed);

	if (parent_ != nullptr)
	{
		// ターゲットとの相対オフセットを保持
		offset_ = VSub(initPos, parent_->GetPosition());
	}
	else
	{
		offset_ = VGet(0, 0, 0);
	}
}

EffekseerObject::~EffekseerObject()
{
	if (playing_handle_ != -1 && EffekseerManager::GetInstance()->IsPlaying(playing_handle_))
	{
		EffekseerManager::GetInstance()->StopEffect(playing_handle_);
	}
}


/*
 * 目的（EffekseerObjectのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EffekseerObject::Update()
{
	if (playing_handle_ == -1 || !EffekseerManager::GetInstance()->IsPlaying(playing_handle_))
	{
		// 再生終了時に自動削除
		SetDeleteFlag(true);
		return;
	}

	if (is_follow_ && parent_ != nullptr)
	{
		if (parent_->IsDeleteFlag())
		{
			// 親が削除された場合は追従を停止
			parent_ = nullptr;
		}
		else
		{
			// 親に追従して座標を更新
			position_ = VAdd(parent_->GetPosition(), offset_);
			EffekseerManager::GetInstance()->SetEffectPosition(playing_handle_, position_);
		}
	}
}


/*
 * 目的（EffekseerObjectのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EffekseerObject::Draw()
{
	// Handled by EffekseerManager::Draw()
}
