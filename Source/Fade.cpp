#include "Fade.h"
#include "DxLib.h"


/// @brief Fadeの初期化（コンストラクタ）
Fade::Fade() 
	: m_State(State::None)
	, m_Alpha(0)
	, m_FadeSpeed(10)
{
}

Fade::~Fade()
{
}


/// @brief Fadeの初期化処理
void Fade::Initialize()
{
	m_State = State::None;
	m_Alpha = 0;
}


/// @brief Fadeの状態更新処理
void Fade::Update()
{
	if (m_State == State::FadeIn)
	{
		m_Alpha -= m_FadeSpeed;
		if (m_Alpha <= 0)
		{
			m_Alpha = 0;
			m_State = State::None;
		}
	}
	else if (m_State == State::FadeOut)
	{
		m_Alpha += m_FadeSpeed;
		if (m_Alpha >= 255)
		{
			m_Alpha = 255;
		}
	}
}


/// @brief Fadeの描画処理
void Fade::Draw()
{
	if (m_Alpha > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_Alpha);
		DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


/// @brief FadeのStartFadeIn処理
void Fade::StartFadeIn()
{
	m_State = State::FadeIn;
	m_Alpha = 255;
}


/// @brief FadeのStartFadeOut処理
void Fade::StartFadeOut()
{
	m_State = State::FadeOut;
	m_Alpha = 0;
}
