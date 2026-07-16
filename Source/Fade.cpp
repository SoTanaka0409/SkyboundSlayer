#include "Fade.h"
#include "DxLib.h"

Fade::Fade() 
	: mState(State::None)
	, mAlpha(0)
	, mFadeSpeed(10)
{
}

Fade::~Fade()
{
}

void Fade::Initialize()
{
	mState = State::None;
	mAlpha = 0;
}

void Fade::Update()
{
	if (mState == State::FadeIn)
	{
		mAlpha -= mFadeSpeed;
		if (mAlpha <= 0)
		{
			mAlpha = 0;
			mState = State::None;
		}
	}
	else if (mState == State::FadeOut)
	{
		mAlpha += mFadeSpeed;
		if (mAlpha >= 255)
		{
			mAlpha = 255;
		}
	}
}

void Fade::Draw()
{
	if (mAlpha > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Fade::StartFadeIn()
{
	mState = State::FadeIn;
	mAlpha = 255;
}

void Fade::StartFadeOut()
{
	mState = State::FadeOut;
	mAlpha = 0;
}
