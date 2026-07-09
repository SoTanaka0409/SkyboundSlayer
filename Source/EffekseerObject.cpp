#include "EffekseerObject.h"
#include "EffekseerManager.h"

EffekseerObject::EffekseerObject(const std::string& name, const char* filepath, VECTOR initPos, Object3D* parent, bool isFollow, float magnification, float speed)
	: Object3D(initPos)
	, mpParent(parent)
	, mbIsFollow(isFollow)
	, mPlayingHandle(-1)
{
	// Load effect (returns existing handle if already loaded)
	EffekseerManager::GetInstance()->LoadEffect(name, filepath, magnification);

	// Play effect
	mPlayingHandle = EffekseerManager::GetInstance()->PlayEffect(name, initPos);
	EffekseerManager::GetInstance()->SetEffectSpeed(mPlayingHandle, speed);

	if (mpParent != nullptr)
	{
		// Store relative offset to the target
		mOffset = VSub(initPos, mpParent->GetPosition());
	}
	else
	{
		mOffset = VGet(0, 0, 0);
	}
}

EffekseerObject::~EffekseerObject()
{
	if (mPlayingHandle != -1 && EffekseerManager::GetInstance()->IsPlaying(mPlayingHandle))
	{
		EffekseerManager::GetInstance()->StopEffect(mPlayingHandle);
	}
}

void EffekseerObject::Update()
{
	if (mPlayingHandle == -1 || !EffekseerManager::GetInstance()->IsPlaying(mPlayingHandle))
	{
		// Auto delete when playback finishes
		SetDeleteFlag(true);
		return;
	}

	if (mbIsFollow && mpParent != nullptr)
	{
		if (mpParent->IsDeleteFlag())
		{
			// Stop following if parent is deleted
			mpParent = nullptr;
		}
		else
		{
			// Update position to follow parent
			position_ = VAdd(mpParent->GetPosition(), mOffset);
			EffekseerManager::GetInstance()->SetEffectPosition(mPlayingHandle, position_);
		}
	}
}

void EffekseerObject::Draw()
{
	// Handled by EffekseerManager::Draw()
}
