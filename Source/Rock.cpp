#include"Rock.h"
#include"CapsuleCollider.h"
#include"Model.h"
Rock::Rock(std::string filename,VECTOR pos, float r,float High,float size)
	:Object3D(VGet(0,0,0))
	, mfHigh(High)
{
	SetTag(Object3D::Tag3D_Obj);
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0,High,0)),r);
	mpModel = new Model(filename, pos, false);
	mpModel->SetScale(VGet(size, size, size));
}

Rock::~Rock()
{
	mpCapsuleCollider->SetDeleteFlag(true);
	
}


void Rock::Draw()
{
	mpModel->Draw();
}

void Rock::Update()
{
	mpCapsuleCollider->mvPosition = mvPosition;
	mpCapsuleCollider->mvPosition2 = mvPosition, VAdd(mvPosition, VGet(0, mfHigh, 0));
	mvPosition.y = -100.0f;
}

void Rock::OnEnter(Collider* collider, Collider* check)
{

}

void Rock::OnTrigger(Collider* collider, Collider* check)
{

}

void Rock::OnExit(Collider* collider, Collider* check)
{

}

