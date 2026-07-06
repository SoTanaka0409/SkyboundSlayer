#include"Tree.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Master.h"
#include"Stage.h"
#include"Scene.h"
#include"CapsuleCollider.h"

Tree::Tree(std::string filename, VECTOR initPos,float Size,float getSize,bool HitFlag)
	:Object3D(initPos)
	,mfSize(getSize)//あたり判定のっサイズ
{
	model_ = new Model(filename, initPos);
	model_->SetScale(VGet(Size, Size, Size));
	SetTag(Object3D::Tag3D_Obj);

	position_ = initPos;
	mnHitFlag = HitFlag;
	mpCapsuleCollider = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, mfSize, 0.0f)), mfSize);
}
Tree::~Tree()
{
	delete model_;
}

void Tree::Update()
{
	// 地形に沿う処理
	TerrainFollow();

}

void Tree::Draw()
{
	
	/*DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, mnSize, 0.0f)),
		mnSize,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);*/
	model_->Draw();
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

