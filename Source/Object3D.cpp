#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "GameScene.h"
#include "Scene.h"
#include "Tree.h"
#include "StageObject.h"
#include "Stage.h"
#include "wall.h"

/// @param initPos = 初期座標
/// @details 生成されたインスタンスを現在のシーンのオブジェクトマネージャーへ登録
Object3D::Object3D(VECTOR initPos)
	: position_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag3D::None3D)
	, draw_flag_(true)
{
	Master::scene_manager_->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

Object3D::~Object3D()
{
}

void Object3D::Draw()
{
}

void Object3D::Update()
{
}

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
/// @details 衝突開始時の処理（必要に応じて派生クラスでオーバーライド）
void Object3D::OnEnter(Collider* collider, Collider* check)
{
}

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
/// @details 衝突継続時の処理（必要に応じて派生クラスでオーバーライド）
void Object3D::OnTrigger(Collider* collider, Collider* check)
{
}

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
/// @details 衝突終了時の処理（必要に応じて派生クラスでオーバーライド）
void Object3D::OnExit(Collider* collider, Collider* check)
{
}

/// @param カプセル
/// @details 現在の座標（position_）をステージの高さにスナップ、または障害物との衝突による座標押し出し
void Object3D::TerrainFollow(float capsuleBottomY, float capsuleTopY, float capsuleRadius, float lineTopY, float lineBottomY, float gravity)
{
	VECTOR hit_pos_ = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;

	// アーキテクチャ設計：全オブジェクトが共通して利用できる地形追従処理を基底クラスに集約することで、キャラクターの接地処理や障害物のめり込み防止を各クラスで再実装する手間を省く
	const auto& objList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
	for (int i = 0; i < objList.size(); i++)
	{
		Stage* pStage = objList.at(i)->CastTo<Stage>();
		if (pStage != nullptr)
		{
			if (pStage->CheckHit_Capsule(VAdd(position_, VGet(0.0f, capsuleBottomY, 0.0f)), VAdd(position_, VGet(0.0f, capsuleTopY, 0.0f)), capsuleRadius))
			{
				hit_pos_ = pStage->CheckHit_Line(
					VAdd(position_, VGet(0.0f, lineTopY, 0.0f)),
					VAdd(position_, VGet(0.0f, lineBottomY, 0.0f))
				);
				isHit = true;
			}
		}
	}

	// 物理挙動：接地判定時は地形の高さを反映し、非接地時は重力による落下処理を適用することで、物理エンジンなしでも自然な接地感を担保
	if (isHit)
	{
		position_.y = hit_pos_.y;
	}
	else
	{
		position_.y -= gravity;
		if (position_.y <= 0.0f || position_.y <= hit_pos_.y)
		{
			position_.y = (hit_pos_.y > 0.0f) ? hit_pos_.y : position_.y;
			if (position_.y < 0.0f) position_.y = 0.0f;
		}
	}

	// 衝突解決：ステージオブジェクト（小物など）との物理的な重なりを検知し、距離の逆数を用いた押し出し処理でオブジェクト同士のめり込みを即座に解消
	if (tag_ != Object3D::Tag3D_Object && tag_ != Object3D::Tag3D_Stage)
	{
		const auto& objs = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Object);
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