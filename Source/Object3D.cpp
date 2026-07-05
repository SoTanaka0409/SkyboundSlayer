#include"Object3D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Scene.h"
#include"Tree.h"
#include"Stage.h"
#include"wall.h"



Object3D::Object3D(VECTOR initPos)
	:position_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag3D::None3D)
	,draw_flag_(true)
{
	//現在のシーンのobjectManagerに自信（this)を追加する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

//デストラクタ
Object3D::~Object3D()
{

}
//描画
void Object3D::Draw()
{

}

//更新
void Object3D::Update()
{

}

void Object3D::OnEnter(Collider* collider, Collider* check)
{

}

void Object3D::OnTrigger(Collider* collider, Collider* check)
{

}

void Object3D::OnExit(Collider* collider, Collider* check)
{

}


void Object3D::TerrainFollow(float capsuleBottomY, float capsuleTopY, float capsuleRadius, float lineTopY, float lineBottomY, float gravity)
{
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;
	
	auto objList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
	for (int i = 0; i < objList.size(); i++)
	{
		Stage* pStage = objList.at(i)->CastTo<Stage>();
		if (pStage != nullptr)
		{
			if (pStage->CheckHit_Capsule(VAdd(position_, VGet(0.0f, capsuleBottomY, 0.0f)), VAdd(position_, VGet(0.0f, capsuleTopY, 0.0f)), capsuleRadius))
			{
				hitPos = pStage->CheckHit_Line(
					VAdd(position_, VGet(0.0f, lineTopY, 0.0f)),
					VAdd(position_, VGet(0.0f, lineBottomY, 0.0f))
				);
				isHit = true;
			}
		}
	}

	if (isHit)
	{
		position_.y = hitPos.y;
	}
	else
	{
		position_.y -= gravity;
		if (position_.y <= 0.0f || position_.y <= hitPos.y)
		{
			// Player uses hitPos.y even if not hit (it's 0.0f).
			position_.y = (hitPos.y > 0.0f) ? hitPos.y : position_.y;
			if(position_.y < 0.0f) position_.y = 0.0f;
		}
	}
}