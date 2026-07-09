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
	,mfSize(getSize)//縺ゅ◆繧雁愛螳壹・縺｣繧ｵ繧､繧ｺ
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
	// 蝨ｰ蠖｢縺ｫ豐ｿ縺・・逅・
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

