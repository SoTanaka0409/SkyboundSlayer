#include"Effect.h"



/// @brief Effectの初期化（コンストラクタ）
Effect::Effect()
{
	effect_ = new EffectInfo();
	graph_handle_ = -1;
	active_ = false;
}


/// @brief EffectのPlay処理
void Effect::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime)
{
	active_ = true;
	effect_->color = Changecolor;
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		effect_->particle[i].pos = initPos;
		effect_->particle[i].dir.x = ((float)GetRand(200) - 100.0f) / 100.0f;
		effect_->particle[i].dir.y = ((float)GetRand(200) - 100.0f) / 100.0f;
		effect_->particle[i].dir.z = ((float)GetRand(200) - 100.0f) / 100.0f;
		effect_->particle[i].speed = ((float)GetRand(SPEED_RAND_MAX) + SPEED_RAND_MIN) / 100.0f;
		effect_->particle[i].alpha = 1.0f;
		effect_->particle[i].size = Size;
		effect_->particle[i].visibleTime = VisibleTime;
	}
	if (graph_handle_ == -1) {
		int oldFlag = GetUseASyncLoadFlag();
		SetUseASyncLoadFlag(FALSE);
		graph_handle_ = LoadGraph(filename.c_str());
		SetUseASyncLoadFlag(oldFlag);
	}
}

Effect::~Effect()
{
	delete effect_;

	DeleteGraph(graph_handle_);

}



/// @brief Effectの状態更新処理
void Effect::Update()
{
	bool isEnd = true;
	float stepTime = 1.0f / 60.0f;

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (effect_->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		isEnd = false;
		if (effect_->particle[i].speed > 0.0f)
		{
			effect_->particle[i].pos = VAdd(effect_->particle[i].pos, VScale(effect_->particle[i].dir, effect_->particle[i].speed));


			effect_->particle[i].speed -= 2.0f * stepTime;

			if (effect_->particle[i].speed <= 0.0f)
			{
				effect_->particle[i].visibleTime = 0.0f;

			}


		}
		if (effect_->particle[i].visibleTime > 0.0f)
		{
			effect_->particle[i].visibleTime -= 0.75f * stepTime;
		}
		else
		{
			effect_->particle[i].alpha -= 12.0f * stepTime;
		}

	}
	if (isEnd)
	{
		active_ = false;
	}





}


/// @brief Effectの描画処理
void Effect::Draw()
{
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(FALSE);
	SetDrawBright(effect_->color.r, effect_->color.g, effect_->color.b);

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (effect_->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_INVSRC, static_cast<int>(effect_->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			effect_->particle[i].pos, 0.5f, 0.5f,
			effect_->particle[i].size * effect_->particle[i].alpha,
			0.0f,
			graph_handle_,
			true
		);


	}

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (effect_->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>(effect_->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			effect_->particle[i].pos, 0.5f, 0.5f,
			effect_->particle[i].size * effect_->particle[i].alpha,
			0.0f,
			graph_handle_,
			true
		);


	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetDrawBright(255, 255, 255);

	SetUseZBufferFlag(TRUE);

	SetWriteZBufferFlag(TRUE);



}
