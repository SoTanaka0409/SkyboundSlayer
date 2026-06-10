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
	mpModel=new Model(filename, initPos, 1.0f);
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	auto pPlayer = dynamic_cast<Player3D*>(mpPlayer);

	mvPosition = pPlayer->GetPosition();
}

Shield::~Shield()
{
	if (mpModel == nullptr)
	{
		delete mpModel;
	}
}

void Shield::Update()
{
	//mpModel->Update();
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	auto pPlayer = dynamic_cast<Player3D*>(mpPlayer);
	mvPosition = pPlayer->GetPosition();
}

void Shield::Draw()
{
	DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 80.0f, 0.0f)),
		mnSizeS,
		8,
		GetColor(0, 255, 255),
		GetColor(0, 255, 255),
		false
	);
	//mpModel->Draw();

}
