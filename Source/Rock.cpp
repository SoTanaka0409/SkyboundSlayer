#include"Rock.h"
#include"CapsuleCollider.h"
#include"Model.h"

/*
 * 目的（RockのRock処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Rock::Rock(std::string filename,VECTOR pos, float r,float High,float size)
	:Object3D(VGet(0,0,0))
	, high_(High)
{
	SetTag(Object3D::Tag3D_Obj);
	capsule_collider_ = new CapsuleCollider(this, position_, VAdd(position_, VGet(0,High,0)),r);
	model_ = new Model(filename, pos, false);
	model_->SetScale(VGet(size, size, size));
}

Rock::~Rock()
{
	capsule_collider_->SetDeleteFlag(true);
	
}



/*
 * 目的（RockのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Rock::Draw()
{
	model_->Draw();
}


/*
 * 目的（RockのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Rock::Update()
{
	capsule_collider_->position_ = position_;
	capsule_collider_->position2_ = position_, VAdd(position_, VGet(0, high_, 0));
	position_.y = -100.0f;
}


/*
 * 目的（RockのOnEnter処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Rock::OnEnter(Collider* collider, Collider* check)
{

}


/*
 * 目的（RockのOnTrigger処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Rock::OnTrigger(Collider* collider, Collider* check)
{

}


/*
 * 目的（RockのOnExit処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Rock::OnExit(Collider* collider, Collider* check)
{

}

