#include"Effect.h"


Effect::Effect()
{
	mpEffect = new EffectInfo();
	mnGraphHandle = -1;
	mActive = false;
}

void Effect::Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime)
{
	mActive = true;
	mpEffect->color = Changecolor;
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		mpEffect->particle[i].pos = initPos;
		mpEffect->particle[i].dir.x = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.y = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].dir.z = ((float)GetRand(200) - 100.0f) / 100.0f;
		mpEffect->particle[i].speed = ((float)GetRand(SPEED_RAND_MAX) + SPEED_RAND_MIN) / 100.0f;
		mpEffect->particle[i].alpha = 1.0f;
		mpEffect->particle[i].size = Size;
		mpEffect->particle[i].visibleTime = VisibleTime;
	}
	if (mnGraphHandle == -1) {
		int oldFlag = GetUseASyncLoadFlag();
		SetUseASyncLoadFlag(FALSE);
		mnGraphHandle = LoadGraph(filename.c_str());
		SetUseASyncLoadFlag(oldFlag);
	}
}

Effect::~Effect()
{
	delete mpEffect;

	DeleteGraph(mnGraphHandle);

}


void Effect::Update()
{
	bool isEnd = true;
	float stepTime = 1.0f / 60.0f;

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		isEnd = false;
		if (mpEffect->particle[i].speed > 0.0f)
		{
			mpEffect->particle[i].pos = VAdd(mpEffect->particle[i].pos, VScale(mpEffect->particle[i].dir, mpEffect->particle[i].speed));


			mpEffect->particle[i].speed -= 2.0f * stepTime;

			if (mpEffect->particle[i].speed <= 0.0f)
			{
				mpEffect->particle[i].visibleTime = 0.0f;

			}


		}
		if (mpEffect->particle[i].visibleTime > 0.0f)
		{
			mpEffect->particle[i].visibleTime -= 0.75f * stepTime;
		}
		else
		{
			mpEffect->particle[i].alpha -= 12.0f * stepTime;
		}

	}
	if (isEnd)
	{
		mActive = false;
	}





}

void Effect::Draw()
{
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(FALSE);
	SetDrawBright(mpEffect->color.r, mpEffect->color.g, mpEffect->color.b);

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_INVSRC, static_cast<int>(mpEffect->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			mpEffect->particle[i].pos, 0.5f, 0.5f,
			mpEffect->particle[i].size * mpEffect->particle[i].alpha,
			0.0f,
			mnGraphHandle,
			true
		);


	}

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>(mpEffect->particle[i].alpha * 255.0f));
		DrawBillboard3D(
			mpEffect->particle[i].pos, 0.5f, 0.5f,
			mpEffect->particle[i].size * mpEffect->particle[i].alpha,
			0.0f,
			mnGraphHandle,
			true
		);


	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	SetDrawBright(255, 255, 255);

	SetUseZBufferFlag(TRUE);

	SetWriteZBufferFlag(TRUE);



}
