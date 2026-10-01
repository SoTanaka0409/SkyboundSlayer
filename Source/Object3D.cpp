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
	: m_Position(initPos)
	, m_Rotation(VGet(0.0f, 0.0f, 0.0f))
	, m_DeleteFlag(false)
	, m_Tag(Tag3D::None3D)
	, m_DrawFlag(true)
{
	Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
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
/// @details 現在の座標（m_Position）をステージの高さにスナップ、または障害物との衝突による座標押し出し
void Object3D::TerrainFollow(float capsuleBottomY, float capsuleTopY, float capsuleRadius, float lineTopY, float lineBottomY, float gravity)
{
	VECTOR m_HitPos = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;

	// アーキテクチャ設計：全オブジェクトが共通して利用できる地形追従処理を基底クラスに集約することで、キャラクターの接地処理や障害物のめり込み防止を各クラスで再実装する手間を省く
	const auto& objList = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
	for (int i = 0; i < objList.size(); i++)
	{
		Stage* pStage = objList.at(i)->CastTo<Stage>();
		if (pStage != nullptr)
		{
			if (pStage->CheckHit_Capsule(VAdd(m_Position, VGet(0.0f, capsuleBottomY, 0.0f)), VAdd(m_Position, VGet(0.0f, capsuleTopY, 0.0f)), capsuleRadius))
			{
				m_HitPos = pStage->CheckHit_Line(
					VAdd(m_Position, VGet(0.0f, lineTopY, 0.0f)),
					VAdd(m_Position, VGet(0.0f, lineBottomY, 0.0f))
				);
				isHit = true;
			}
		}
	}

	// 物理挙動：接地判定時は地形の高さを反映し、非接地時は重力による落下処理を適用することで、物理エンジンなしでも自然な接地感を担保
	if (isHit)
	{
		m_Position.y = m_HitPos.y;
	}
	else
	{
		m_Position.y -= gravity;
		if (m_Position.y <= 0.0f || m_Position.y <= m_HitPos.y)
		{
			m_Position.y = (m_HitPos.y > 0.0f) ? m_HitPos.y : m_Position.y;
			if (m_Position.y < 0.0f) m_Position.y = 0.0f;
		}
	}

	// 衝突解決：ステージオブジェクト（小物など）との物理的な重なりを検知し、距離の逆数を用いた押し出し処理でオブジェクト同士のめり込みを即座に解消
	if (m_Tag != Object3D::Tag3D_Object && m_Tag != Object3D::Tag3D_Stage)
	{
		const auto& objs = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Object);
		for (int i = 0; i < objs.size(); i++)
		{
			StageObject* stObj = objs.at(i)->CastTo<StageObject>();
			if (stObj != nullptr && stObj != this && stObj->IsHitEnabled())
			{
				VECTOR objPos = stObj->GetPosition();
				float objRadius = stObj->GetHitRadius();
				float myRadius = capsuleRadius;

				float dx = m_Position.x - objPos.x;
				float dz = m_Position.z - objPos.z;
				float distSq = dx * dx + dz * dz;
				float hitDist = objRadius + myRadius;

				if (distSq > 0.0001f && distSq < hitDist * hitDist)
				{
					float dist = sqrtf(distSq);
					float pushLen = hitDist - dist;
					m_Position.x += (dx / dist) * pushLen;
					m_Position.z += (dz / dist) * pushLen;
				}
			}
		}
	}
}