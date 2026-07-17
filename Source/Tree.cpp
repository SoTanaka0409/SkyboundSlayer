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
	,size_(getSize)//縺ゅ◆繧雁愛螳壹・縺｣繧ｵ繧､繧ｺ
{
	model_ = new Model(filename, initPos);
	model_->SetScale(VGet(Size, Size, Size));
	SetTag(Object3D::Tag3D_Obj);

	position_ = initPos;
	is_hit_flag_ = HitFlag;
	capsule_collider_ = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, size_, 0.0f)), size_);
}
Tree::~Tree()
{
	delete model_;
}

/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void Tree::Update()
{
	// 蝨ｰ蠖｢縺ｫ豐ｿ縺・・逅・
	TerrainFollow();

}

/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
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

/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void Tree::OnEnter(Collider* collider, Collider* check)
{
	
}
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void Tree::OnTrigger(Collider* collider, Collider* check)
{

}
/*
 * [目的] 
 * [入力] 
 * [出力] 
 * [副作用] 
 */
void Tree::OnExit(Collider* collider, Collider* check)
{

}

