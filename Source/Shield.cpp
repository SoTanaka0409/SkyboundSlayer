#include"Shield.h"
#include"Model.h"
#include"Player3D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Scene.h"




/*
 * 目的（ShieldのShield処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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


/*
 * 目的（ShieldのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Shield::Update()
{
	auto player_ = Master::player_;
	auto pPlayer = Master::player_;
	position_ = pPlayer->GetPosition();
}


/*
 * 目的（ShieldのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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
