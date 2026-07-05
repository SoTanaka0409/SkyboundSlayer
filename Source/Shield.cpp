#include"Shield.h"
#include"Model.h"
#include"Player3D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Scene.h"



Shield::Shield(std::string filename, VECTOR initPos,int hp)
	:Object3D(initPos)
	,NewShield(true)
	,mnHp(hp)
	,mnSizeS(200.0f)
{
	SetTag(Object3D::Tag_3D_Shield);
	model_=new Model(filename, initPos, 1.0f);
	auto mpPlayer = Master::mpPlayer;
	auto pPlayer = Master::mpPlayer;

	position_ = pPlayer->GetPosition();
}

Shield::~Shield()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
}

void Shield::Update()
{
	auto mpPlayer = Master::mpPlayer;
	auto pPlayer = Master::mpPlayer;
	position_ = pPlayer->GetPosition();
}

void Shield::Draw()
{
	DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 80.0f, 0.0f)),
		mnSizeS,
		8,
		GetColor(0, 255, 255),
		GetColor(0, 255, 255),
		false
	);

}
