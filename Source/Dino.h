#pragma once
#include"DxLib.h"
#include"Model.h"
#include"Object3D.h"

class Dino:public Object3D
{
public:
	enum TagDINO_NUM
	{
		TagDINO_NONE=0,
		TagDINO_SP=100,
		TagDINO_TORI=300,
		TagDINO_MAX=500,

	};


public: 
	Dino(std::string filename, VECTOR initPos, float Hp, float speed);

	~Dino();

	void Draw();

	void Update();

	void Move();

	void RotationByMove();//移動による回転処理

	void Dead();

	float GetHp() { return mnHp; };
	float GetMaxHp() { return mnMaxHp; }
	void SetHp(int hp) { mnHp = hp; };
	void Invisible();

	void InvisibleNo();

	bool GetInvisible() { return mnInvisible; }
	void SetInvisible(int Inv) { mnInvisible = Inv; }

	void SetTagD(TagDINO_NUM tag) { mnTagD = tag; }
	TagDINO_NUM GetTagD() { return mnTagD; }
	float GetDinoSize() { return DinoSize; }
private:
	Model* mpModel;
	
	float mnHp;
	float mnMaxHp;
	float mnSpeed;

	bool mnInvisible;//無敵かどうか
	int mnAlgorithm;//ランダム移動
	float DinoSize;//見た目の大きさ


	bool Animation;
	float mfTargetAngle;//目標の回転地
	float mfAngle;//現在の回転地
	const float ROTATE_SPEED = 0.2f;//回転速度
	TagDINO_NUM mnTagD;

	////ランダムウオーク
	bool isWalk;
	VECTOR PurposePosition;//目的の座標の指定
	float  GetmvPositionX;//目的の座標の指定
	float  GetmvPositionZ;//目的の座標の指定
	

};