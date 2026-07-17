#include "Fade.h"
#include "DxLib.h"


/*
 * 目的（FadeのFade処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Fade::Fade() 
	: state_(State::None)
	, alpha_(0)
	, fade_speed_(10)
{
}

Fade::~Fade()
{
}


/*
 * 目的（FadeのInitialize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Fade::Initialize()
{
	state_ = State::None;
	alpha_ = 0;
}


/*
 * 目的（FadeのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（FadeのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Fade::Draw()
{
	if (alpha_ > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


/*
 * 目的（FadeのStartFadeIn処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Fade::StartFadeIn()
{
	state_ = State::FadeIn;
	alpha_ = 255;
}


/*
 * 目的（FadeのStartFadeOut処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Fade::StartFadeOut()
{
	state_ = State::FadeOut;
	alpha_ = 0;
}
