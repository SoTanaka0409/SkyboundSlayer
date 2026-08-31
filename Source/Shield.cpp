#include"Shield.h"
#include"Model.h"
#include"Player3D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"GameScene.h"
#include"SceneManager.h"
#include"Scene.h"




/// @brief Shieldの初期化（コンストラクタ）
Shield::Shield(std::string filename, VECTOR initPos,int hp)
	:Object3D(initPos)
	,NewShield(true)
	,hp_(hp)
	,size_s_(200.0f)
{
	SetTag(Object3D::Tag_3D_Shield);
	model_=new Model(filename, initPos, 1.0f);
	auto player_ = Master::player_;
	auto pPlayer = Master::player_;

	position_ = pPlayer->GetPosition();
}

Shield::~Shield()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
}


/// @brief Shieldの状態更新処理
void Shield::Update()
{
	auto player_ = Master::player_;
	auto pPlayer = Master::player_;
	position_ = pPlayer->GetPosition();
}


/// @brief Shieldの描画処理
void Shield::Draw()
{
	DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 80.0f, 0.0f)),
		size_s_,
		8,
		GetColor(0, 255, 255),
		GetColor(0, 255, 255),
		false
	);

}
