#include"Tree.h"
#include"ObjectManager.h"
#include"GameScene.h"
#include"SceneManager.h"
#include"Master.h"
#include"Stage.h"
#include"Scene.h"
#include"CapsuleCollider.h"

Tree::Tree(std::string filename, VECTOR initPos,float Size,float getSize,bool HitFlag)
	:Object3D(initPos)
	,m_Size(getSize)
{
	m_Model = new Model(filename, initPos);
	m_Model->SetScale(VGet(Size, Size, Size));
	SetTag(Object3D::Tag3D_Obj);

	m_Position = initPos;
	m_IsHitFlag = HitFlag;
	m_CapsuleCollider = new CapsuleCollider(this, m_Position, VAdd(m_Position, VGet(0.0f, m_Size, 0.0f)), m_Size);
}
Tree::~Tree()
{
	delete m_Model;
}

void Tree::Update()
{
	
	TerrainFollow();

}

void Tree::Draw()
{
	
	/*DrawCapsule3D(m_Position, VAdd(m_Position, VGet(0.0f, mnSize, 0.0f)),
		mnSize,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);*/
	m_Model->Draw();
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

