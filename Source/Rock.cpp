#include"Rock.h"
#include"CapsuleCollider.h"
#include"Model.h"

/// @brief Rockの初期化（コンストラクタ）
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



/// @brief Rockの描画処理
void Rock::Draw()
{
	model_->Draw();
}


/// @brief Rockの状態更新処理
void Rock::Update()
{
	capsule_collider_->position_ = position_;
	capsule_collider_->position2_ = position_, VAdd(position_, VGet(0, high_, 0));
	position_.y = -100.0f;
}


/// @brief RockのOnEnter処理
void Rock::OnEnter(Collider* collider, Collider* check)
{

}


/// @brief RockのOnTrigger処理
void Rock::OnTrigger(Collider* collider, Collider* check)
{

}


/// @brief RockのOnExit処理
void Rock::OnExit(Collider* collider, Collider* check)
{

}

