#include"Tree.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Master.h"
#include"Stage.h"
#include"Scene.h"
#include"CapsuleCollider.h"

Tree::Tree(std::string filename, VECTOR initPos,int Size,float getSize,bool HitFlag)
	:Object3D(initPos)
	,mfSize(getSize)//あたり判定のっサイズ
{
	mpModel = new Model(filename, initPos);
	mpModel->SetScale(VGet(Size, Size, Size));
	SetTag(Object3D::Tag3D_Obj);

	mvPosition = initPos;
	mnHitFlag = HitFlag;
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
}
Tree::~Tree()
{
	delete mpModel;
}

void Tree::Update()
{
	// 地形に沿う処理
	TerrainFollow();

}

void Tree::Draw()
{
	
	/*DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, mnSize, 0.0f)),
		mnSize,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);*/
	mpModel->Draw();
}

void Tree::OnEnter(Collider* collider, Collider* check)
{
	
}
void Tree::OnTrigger(Collider* collider, Collider* check)
{

}
void Tree::OnExit(Collider* collider, Collider* check)
{

}

