#include"Rock.h"
#include"CapsuleCollider.h"
#include"Model.h"

/// @brief Rockの初期化（コンストラクタ）
Rock::Rock(std::string filename,VECTOR pos, float r,float High,float size)
	:Object3D(VGet(0,0,0))
	, m_High(High)
{
	SetTag(Object3D::Tag3D_Obj);
	m_CapsuleCollider = new CapsuleCollider(this, m_Position, VAdd(m_Position, VGet(0,High,0)),r);
	m_Model = new Model(filename, pos, false);
	m_Model->SetScale(VGet(size, size, size));
}

Rock::~Rock()
{
	m_CapsuleCollider->SetDeleteFlag(true);
	
}



/// @brief Rockの描画処理
void Rock::Draw()
{
	m_Model->Draw();
}


/// @brief Rockの状態更新処理
void Rock::Update()
{
	m_CapsuleCollider->m_Position = m_Position;
	m_CapsuleCollider->m_Position2 = m_Position, VAdd(m_Position, VGet(0, m_High, 0));
	m_Position.y = -100.0f;
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

