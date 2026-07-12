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
	bool isEnd = true;  //パーティクルがすべて消えたかどうか
	float stepTime = 1.0f / 60.0f;//1フレームにどれだけ進めるかの基本値

	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		//ここまでくるということは少なくても一つはパーティクルが存在しているので終了させない
		isEnd = false;
		if (mpEffect->particle[i].speed > 0.0f)
		{
			//座標を移動させる
			mpEffect->particle[i].pos = VAdd(mpEffect->particle[i].pos, VScale(mpEffect->particle[i].dir, mpEffect->particle[i].speed));


			mpEffect->particle[i].speed -= 2.0f * stepTime;

			//速度が０以下になった場合
			if (mpEffect->particle[i].speed <= 0.0f)
			{
				mpEffect->particle[i].visibleTime = 0.0f;

			}


		}
		//
		if (mpEffect->particle[i].visibleTime > 0.0f)
		{
			//表示時間を減らしていく
			mpEffect->particle[i].visibleTime -= 0.75f * stepTime;
		}
		else
		{
			//アルファ値を落としていく（徐々に落としていく)
			mpEffect->particle[i].alpha -= 12.0f * stepTime;
		}

	}
	//パーティクルが１つも存在していないのであれば削除ウする
	if (isEnd)
	{
		mActive = false;
	}





}

void Effect::Draw()
{
	//
	SetUseZBufferFlag(TRUE);
	//Zbaxtufanikakikomisinai 
	SetWriteZBufferFlag(FALSE);
	//描画輝度
	SetDrawBright(mpEffect->color.r, mpEffect->color.g, mpEffect->color.b);

	//描画していく
	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		//アルファ値が０以下なら描画しない
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		//「反転ブレンド」を行いつつパーティクルをビルボードで描画
		// //反転ブレンド...色を反転してくれる
		//ビルボード。。。カメラ方向に常に向いてくれる３Dポリゴン
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, (int)mpEffect->particle[i].alpha * 255.0f);
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
		//アルファ値が０以下なら描画しない
		if (mpEffect->particle[i].alpha <= 0.0f)
		{
			continue;
		}

		//「加算ブレンド」を行いつつパーティクルをビルボードで描画
		// 加算ブレンド...重なったポリゴン部分の色が加算されていく描画方法
		// 減算ブレンド（SUB)色を減らしていく
		//ビルボード。。。カメラ方向に常に向いてくれる３Dポリゴン
		SetDrawBlendMode(DX_BLENDMODE_ADD, (int)mpEffect->particle[i].alpha * 255.0f);
		DrawBillboard3D(
			mpEffect->particle[i].pos, 0.5f, 0.5f,
			mpEffect->particle[i].size * mpEffect->particle[i].alpha,
			0.0f,
			mnGraphHandle,
			true
		);


	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//描画輝度を元に戻す
	SetDrawBright(255, 255, 255);

	//Zバッファを使用する
	SetUseZBufferFlag(TRUE);

	//Zバッファーへの書き込みはしない
	SetWriteZBufferFlag(TRUE);



}