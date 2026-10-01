#include"Effect.h"



/// @brief Effectの初期化（コンストラクタ）
Effect::Effect()
{
	m_Effect = new EffectInfo();
	m_GraphHandle = -1;
	m_Active = false;
}


/// @brief EffectのPlay処理
void Effect::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime)
{
	m_Active = true;
	m_Effect->color = Changecolor;
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		m_Effect->particle[i].pos = initPos;
		m_Effect->particle[i].dir.x = ((float)GetRand(200) - 100.0f) / 100.0f;
		m_Effect->particle[i].dir.y = ((float)GetRand(200) - 100.0f) / 100.0f;
		m_Effect->particle[i].dir.z = ((float)GetRand(200) - 100.0f) / 100.0f;
		m_Effect->particle[i].speed = ((float)GetRand(SPEED_RAND_MAX) + SPEED_RAND_MIN) / 100.0f;
		m_Effect->particle[i].alpha = 1.0f;
		m_Effect->particle[i].size = Size;
		m_Effect->particle[i].visibleTime = VisibleTime;
	}
	if (m_GraphHandle == -1) {
		int oldFlag = GetUseASyncLoadFlag();
		SetUseASyncLoadFlag(FALSE);
		m_GraphHandle = LoadGraph(filename.c_str());
		SetUseASyncLoadFlag(oldFlag);
	}
}

Effect::~Effect()
{
	delete m_Effect;

	DeleteGraph(m_GraphHandle);

}



/// @brief Effectの状態更新処理
void Effect::Update()
{
	bool isEnd = true;
	float stepTime = 1.0f / 60.0f;

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (m_Effect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		isEnd = false;
		if (m_Effect->particle[i].speed > 0.0f)
		{
			m_Effect->particle[i].pos = VAdd(m_Effect->particle[i].pos, VScale(m_Effect->particle[i].dir, m_Effect->particle[i].speed));


			m_Effect->particle[i].speed -= 2.0f * stepTime;

			if (m_Effect->particle[i].speed <= 0.0f)
			{
				m_Effect->particle[i].visibleTime = 0.0f;

			}


		}
		if (m_Effect->particle[i].visibleTime > 0.0f)
		{
			m_Effect->particle[i].visibleTime -= 0.75f * stepTime;
		}
		else
		{
			m_Effect->particle[i].alpha -= 12.0f * stepTime;
		}

	}
	if (isEnd)
	{
		m_Active = false;
	}





}


/// @brief Effectの描画処理
void Effect::Draw()
{
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(FALSE);
	SetDrawBright(m_Effect->color.r, m_Effect->color.g, m_Effect->color.b);

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (m_Effect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_INVSRC, static_cast<int>(m_Effect->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			m_Effect->particle[i].pos, 0.5f, 0.5f,
			m_Effect->particle[i].size * m_Effect->particle[i].alpha,
			0.0f,
			m_GraphHandle,
			true
		);


	}

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (m_Effect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>(m_Effect->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			m_Effect->particle[i].pos, 0.5f, 0.5f,
			m_Effect->particle[i].size * m_Effect->particle[i].alpha,
			0.0f,
			m_GraphHandle,
			true
		);


	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetDrawBright(255, 255, 255);

	SetUseZBufferFlag(TRUE);

	SetWriteZBufferFlag(TRUE);



}
