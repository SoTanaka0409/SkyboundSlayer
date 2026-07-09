#include"Object3D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Scene.h"
#include"Tree.h"
#include"StageObject.h"
#include"Stage.h"
#include"wall.h"



Object3D::Object3D(VECTOR initPos)
	:position_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag3D::None3D)
	,draw_flag_(true)
{
	//迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｮobjectManager縺ｫ閾ｪ菫｡ｼthis)繧定ｿｽ蜉縺吶ｋ
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

//繝繧ｹ繝医Λ繧ｯ繧ｿ
Object3D::~Object3D()
{

}
//謠冗判
void Object3D::Draw()
{

}

//譖ｴ譁ｰ
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
	
	const auto& objList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
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

	// StageObjectｼ域惠繧蟯ｩ縺ｪ縺ｩｼ峨→縺ｮ繧ｹ繝ｩ繧､繝牙愛螳
	if (tag_ != Object3D::Tag3D_Object && tag_ != Object3D::Tag3D_Stage)
	{
		const auto& objs = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Object);
		for (int i = 0; i < objs.size(); i++)
		{
			StageObject* stObj = objs.at(i)->CastTo<StageObject>();
			if (stObj != nullptr && stObj != this && stObj->IsHitEnabled())
			{
				VECTOR objPos = stObj->GetPosition();
				float objRadius = stObj->GetHitRadius();
				float myRadius = capsuleRadius;
				
				float dx = position_.x - objPos.x;
				float dz = position_.z - objPos.z;
				float distSq = dx * dx + dz * dz;
				float hitDist = objRadius + myRadius;
				
				if (distSq > 0.0001f && distSq < hitDist * hitDist)
				{
					float dist = sqrtf(distSq);
					float pushLen = hitDist - dist;
					position_.x += (dx / dist) * pushLen;
					position_.z += (dz / dist) * pushLen;
				}
			}
		}
	}
}