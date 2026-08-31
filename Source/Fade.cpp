#include "Fade.h"
#include "DxLib.h"


/// @brief Fadeの初期化（コンストラクタ）
Fade::Fade() 
	: state_(State::None)
	, alpha_(0)
	, fade_speed_(10)
{
}

Fade::~Fade()
{
}


/// @brief Fadeの初期化処理
void Fade::Initialize()
{
	state_ = State::None;
	alpha_ = 0;
}


/// @brief Fadeの状態更新処理
void Fade::Update()
{
	if (state_ == State::FadeIn)
	{
		alpha_ -= fade_speed_;
		if (alpha_ <= 0)
		{
			alpha_ = 0;
			state_ = State::None;
		}
	}
	else if (state_ == State::FadeOut)
	{
		alpha_ += fade_speed_;
		if (alpha_ >= 255)
		{
			alpha_ = 255;
		}
	}
}


/// @brief Fadeの描画処理
void Fade::Draw()
{
	if (alpha_ > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


/// @brief FadeのStartFadeIn処理
void Fade::StartFadeIn()
{
	state_ = State::FadeIn;
	alpha_ = 255;
}


/// @brief FadeのStartFadeOut処理
void Fade::StartFadeOut()
{
	state_ = State::FadeOut;
	alpha_ = 0;
}
