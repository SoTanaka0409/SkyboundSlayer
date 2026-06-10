#pragma once
#include"DxLib.h"
#include"Object3D.h"


class Effect :public Object3D
{
public:
	static const int PARTICLE_NUM = 256; //パーティクルの最大数
	//パーティクルの構造体
	//粒一つ一つのデータ
	struct ParticleInfo
	{
		VECTOR pos;  //座標
		VECTOR dir;  //進行方向
		float speed;  //速度
		float size;  //大きさ
		float alpha; //負透明度
		float visibleTime;//表示時間



	};
	//effect情報
	//粒をまとめた１つのエフェクトとしてのデータ
	struct EffectInfo
	{
		COLOR_U8 color; //エフェクトの色
		ParticleInfo particle[PARTICLE_NUM];//パーティクル情報
	};



public:
	Effect(VECTOR initPos, std::string filename, COLOR_U8 Changecolor,float Size,float VisibleTime);

	~Effect();

	void Update()override;

	void Draw()override;

private:
	int mnGraphHandle;  //画像ハンドル
	EffectInfo* mpEffect;  //エフェクトのデータ

	const int SPEED_RAND_MAX = 550;//速度の最大乱数地
	const int SPEED_RAND_MIN = 200;//策殿最小覧数値

	const int SIZE_RAND_MAX = 1600;//サイズの最大乱数地
	const int SIZE_RAND_MIN = 800;//サイズの最小乱数地

	const int VISIBLE_TIME_RAND_MAX = 30;//表示時間の最大乱数地
	const int VISIBLE_TIME_RAND_MIN = 5;//表示時間の最小乱数地



};