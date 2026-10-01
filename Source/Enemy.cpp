#include "Enemy.h"
#include "DamageUI.h"
#include "Model.h"
#include "Master.h"
#include "Player3D.h"
#include "Object3D.h"
#include "ObjectManager.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "Stage.h"
#include "DrawHp.h"
#include "Coin.h"
#include "Wall.h"
#include "Scene.h"
#include "Tree.h"
#include "ColliderManager.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

/// @brief Enemyクラスのコンストラクタ
/// @param filename 使用する3Dモデルのファイルパス
/// @param initPos 初期配置座標
/// @param hp 初期・最大体力値
/// @param speed 移動速度
/// @param attack 攻撃力
/// @param HitSize カプセルコライダーの判定サイズ
/// @param Serch1 索敵（追従）用範囲半径
/// @param Serch2 攻撃開始用範囲半径
/// @param Serch3 接近停止用範囲半径
/// @param money 倒した際に獲得できる資金
/// @param m_IsSeparateAnim アニメーションを分離読み込みするかのフラグ
Enemy::Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool m_IsSeparateAnim)
	: Object3D(initPos)
	, m_Hp(hp)
	, m_Speed(speed)
	, m_IsInvisible(false)
	, m_Angle(0.0f)
	, m_TargetAngle(0.0f)
	, m_Size(HitSize)
	, m_HitSearch(Serch1)
	, m_HitAttackSearch(Serch2)
	, m_HitStopSearch(Serch3)
	, m_AlgHit(20)
	, m_Attack(attack)
	, m_NoPosition(VGet(0, 0, 0))
	, m_IsAttackHitJudgmentFlag(false)
	, m_IsHitAttackSearchFlag(false)
	, m_IsHitSearchStopFlag(false)
	, m_IsHitAttackFlag(false)
	, m_IsHitSearchFlag(false)
	, m_IsHitJudgmentFlagPlayer(false)
	, m_WalkCount(0)
	, m_WalkTimer(0)
	, m_IsDead(false)
	, m_MaxHp(hp)
	, m_HaveMoney(money)
{
	SetTag(Object3D::Tag3D_Enemy3D);
	m_Model = new Model(filename, initPos, m_IsSeparateAnim);
	m_Model->SetScale(VGet(1.3f, 1.3f, 1.3f));
	m_InitPosition = initPos;
	m_MaxHp = m_Hp;
	m_NormalSpeed = m_Speed;

	m_CapsuleCollider = new CapsuleCollider(this, m_Position, VAdd(m_Position, VGet(0.0f, m_Size / 2, 0.0f)), m_Size);
	m_AttachCollider = new SphereCollider(this, m_Model->GetAttachmentPosition(), 50.0f);
	m_SerchCollider = new SphereCollider(this, m_Position, m_HitSearch);
	m_AttackCollider = new SphereCollider(this, m_Position, m_HitAttackSearch);
	m_StopCollider = new SphereCollider(this, m_Position, m_HitStopSearch);
}

/// @brief Enemyクラスのデストラクタ
Enemy::~Enemy()
{
	if (m_Model != nullptr)
	{
		delete m_Model;
		m_Model = nullptr;
	}
}

/// @brief 毎フレームの状態更新処理を行う
/// @details 死亡判定、攻撃動作、各コライダー座標同期、移動および回転制御を実行する
void Enemy::Update()
{
	if (m_Model != nullptr)
	{
		DeathEnemy();
		Attack();
		UpdateColliderPosition();
		RotationByMove();
		Move();

		m_Model->Update();
	}
}

/// @brief Enemyの描画処理を行う
/// @details モデルの描画およびデバッグフラグ有効時のワイヤーフレーム描画を行う
void Enemy::Draw()
{
	if (m_Model != nullptr)
	{
		m_Model->Draw();
	}
}

/// @brief 攻撃モーションパターン管理用拡張メソッド
void Enemy::AttackList()
{
}

/// @brief 攻撃アニメーションの開始判定およびタイマー制御を行う
void Enemy::Attack()
{
	AnimationState now = m_Model->GetNowState();

	if (m_AttackCount >= m_AttackInterval && m_IsHitAttackSearchFlag)
	{
		Master::m_SoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		m_AttackCount = 0;
		m_Model->ChangeAnimation(ANIMATION_ATTACK);
		m_Model->SetLoop(false);
		m_Model->SetLoopFinishState(ANIMATION_NEUTRAL);
		m_IsHitAttackSearchFlag = false;
	}

	if (!(now == ANIMATION_ATTACK))
	{
		m_IsAttackHitJudgmentFlag = false;
		m_AttackCount++;
	}
}

/// @brief プレイヤー追従移動および壁・地形との衝突スライド計算を行う
void Enemy::Move()
{
	AnimationState now = m_Model->GetNowState();
	if (now == ANIMATION_ATTACK || now == ANIMATION_ATTACKMAGIC || now == ANIMATION_ATTACKJUMP) return;
	if (Master::m_IsSafePointOn) m_Position = m_InitPosition;

	Player3D* pPlayer = Master::m_Player;
	m_MoveVec = VGet(0.0f, 0.0f, 0.0f);

	{
		if (m_IsHitSearchStopFlag)
		{
			m_Speed = 0;
		}
		else
		{
			m_Speed = m_NormalSpeed;
		}

		m_GoPosition = VSub(pPlayer->GetPosition(), m_Position);
		m_GoPosition.y = 0.0f;
		if (VSquareSize(m_GoPosition) > 0.0001f) m_GoPosition = VNorm(m_GoPosition);
		m_MoveVec = m_GoPosition;

		bool isMove = (m_MoveVec.x != 0.0f || m_MoveVec.z != 0.0f);
		if (isMove)
		{
			m_Model->ChangeAnimation(ANIMATION_RUN);
			m_TargetAngle = atan2f(m_MoveVec.x, m_MoveVec.z);
		}
		else
		{
			m_Model->ChangeAnimation(ANIMATION_NEUTRAL);
		}

		VECTOR m_OldPosition = m_Position;
		m_Position = VAdd(m_Position, VScale(m_MoveVec, m_Speed));

		TerrainFollow(0.0f, 150.0f, 40.0f, 150.0f, -40.0f, 4.0f);
		const auto& walls = Master::m_SceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);

		if (!walls.empty())
		{
			// Wall（壁）との当たり判定と壁ずり処理
			for (size_t i = 0; i < walls.size(); i++)
			{
				Wall* wall = walls.at(i)->CastTo<Wall>();
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					if (HitCheck_Capsule_Triangle(
						m_Position,
						VAdd(m_Position, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
						HitCheck_Capsule_Triangle(
							m_Position,
							VAdd(m_Position, VGet(0.0f, 200.0f, 0.0f)),
							80.0f,
							vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
						)
					{
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);
						float a = VDot(VScale(m_MoveVec, -1.0f), vertex.at(0).norm);
						slide = VAdd(m_MoveVec, VScale(vertex.at(0).norm, a));

						m_Position = m_OldPosition;
						m_Position = VAdd(m_Position, VScale(slide, m_Speed));
					}
				}
			}
		}

		m_Model->SetPosition(m_Position);
	}
}

/// @brief 移動方向に応じた段階的回転補間処理を行う
void Enemy::RotationByMove()
{
	float subAngle = m_TargetAngle - m_Angle;

	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	if (subAngle > 0.0f)
	{
		subAngle -= ROTATE_SPEED;
		if (subAngle < 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	else if (subAngle < 0.0f)
	{
		subAngle += ROTATE_SPEED;
		if (subAngle > 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	m_Angle = m_TargetAngle - subAngle;

	m_Rotation.y = m_Angle + DX_PI_F;
	m_Model->SetRotation(m_Rotation);
}

/// @brief ダメージ適用処理
/// @param damage 減少させるHP量
void Enemy::Damage(float damage, bool play_sound)
{
	VECTOR pop_pos = m_Position;
	pop_pos.y += m_Size;
	DamageUIManager::GetInstance()->AddDamage((int)damage, pop_pos, damage >= 100.0f);
	if (play_sound) {
		Master::m_SoundManager->PlaySE(SoundManager::SE_HIT_SLASH);
	}
	m_Hp -= damage;
	if (m_Hp <= 0)
	{
		if (!m_IsDead && Master::m_ScoreManager != nullptr)
		{
			Master::m_ScoreManager->AddDefeatedEnemy();
		}
		m_Hp = 0;
		m_IsDead = true;
	}
}

/// @brief 死亡時の演出再生およびオブジェクト削除予約を行う
void Enemy::DeathEnemy()
{
	if (!m_IsDead) return;

	m_Model->ChangeAnimation(ANIMATION_DYING);
	m_Model->SetLoop(false);
	m_Model->SetLoopFinishState(ANIMATION_MAX);

	Delete();
	if (m_Model->IsAnimationLoopFinish())
	{
		GiveRewards();
		SetDeleteFlag(true);
	}

	m_Model->Update();
}

/// @brief 死亡時に各種コライダーを画面外遥か遠くへ移動させて無効化する
void Enemy::DeathColliderPosition()
{
	VECTOR pos = VGet(10000, 10000, 10000);
	if (m_CapsuleCollider != nullptr)
	{
		m_CapsuleCollider->m_Position = pos;
		m_CapsuleCollider->m_Position2 = pos;
	}
	if (m_AttachCollider != nullptr)
	{
		m_AttachCollider->m_Position = pos;
	}
	if (m_SerchCollider != nullptr)
	{
		m_SerchCollider->m_Position = pos;
	}
	if (m_StopCollider != nullptr)
	{
		m_StopCollider->SetDeleteFlag(true);
		m_StopCollider->m_Position = pos;
	}
	if (m_AttackCollider != nullptr)
	{
		m_AttackCollider->m_Position = pos;
	}
}

/// @brief 他オブジェクトのコライダーと接触を開始した瞬間の割り込み処理
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
void Enemy::OnEnter(Collider* collider, Collider* check)
{
	if (m_Hp <= 0) return;
	if (collider == m_CapsuleCollider && check->m_ParentObject->GetTag() == Tag3D_Obj)
	{
		m_Position = m_InitPosition;
	}

	if (check->m_ParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::m_Player;
		if (player == nullptr) return;

		if (collider == m_SerchCollider && player->GetCollisionCollider() == check)
		{
			m_IsHitSearchFlag = true;
		}
		if (collider == m_AttackCollider && player->GetCollisionCollider() == check)
		{
			m_IsHitAttackSearchFlag = true;
		}
		if (collider == m_StopCollider && player->GetCollisionCollider() == check)
		{
			m_IsHitSearchStopFlag = true;
		}
	}
}

/// @brief 他オブジェクトのコライダーと接触中の継続処理（プレイヤーへの攻撃判定等）
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
void Enemy::OnTrigger(Collider* collider, Collider* check)
{
	if (m_Hp <= 0) return;
	AnimationState now = m_Model->GetNowState();

	if (collider == m_AttachCollider && check->m_ParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::m_Player;
		if (player == nullptr) return;

		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !m_IsAttackHitJudgmentFlag)
			{
				player->Damage(m_Attack);
				m_IsAttackHitJudgmentFlag = true;
			}
		}
	}
}

/// @brief 他オブジェクトのコライダーと離脱した瞬間の割り込み処理
/// @param collider 自身のコライダー
/// @param check 相手のコライダー
void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (m_Hp <= 0) return;

	if (check->m_ParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::m_Player;
		if (player == nullptr) return;

		if (collider == m_SerchCollider && player->GetCollisionCollider() == check)
		{
			m_IsHitSearchFlag = false;
		}
		if (collider == m_AttackCollider && player->GetCollisionCollider() == check)
		{
			m_IsHitAttackSearchFlag = false;
		}
		if (collider == m_StopCollider && player->GetCollisionCollider() == check)
		{
			m_IsHitSearchStopFlag = false;
		}
	}
}

/// @brief 各コライダーの追従位置座標を毎フレーム同期更新する
void Enemy::UpdateColliderPosition()
{
	if (m_CapsuleCollider != nullptr)
	{
		m_CapsuleCollider->m_Position = m_Position;
		m_CapsuleCollider->m_Position2 = VAdd(m_Position, VGet(0.0f, 150.0f, 0.0f));
	}
	if (m_AttachCollider != nullptr)
	{
		m_AttachCollider->m_Position = m_Model->GetAttachmentPosition();
	}
	if (m_SerchCollider != nullptr)
	{
		m_SerchCollider->m_Position = m_Position;
	}
	if (m_StopCollider != nullptr)
	{
		m_StopCollider->m_Position = VAdd(m_Position, VGet(0.0f, m_Size / 2, 0.0f));
	}
	if (m_AttackCollider != nullptr)
	{
		m_AttackCollider->m_Position = VAdd(m_Position, VGet(0.0f, m_Size / 2, 0.0f));
	}
}

/// @brief 全ての登録済みコライダーを削除マークし解放準備をする
void Enemy::Delete()
{
	if (m_CapsuleCollider != nullptr)
	{
		m_CapsuleCollider->SetDeleteFlag(true);
		m_CapsuleCollider = nullptr;
	}
	if (m_AttachCollider != nullptr)
	{
		m_AttachCollider->SetDeleteFlag(true);
		m_AttachCollider = nullptr;
	}
	if (m_SerchCollider != nullptr)
	{
		m_SerchCollider->SetDeleteFlag(true);
		m_SerchCollider = nullptr;
	}
	if (m_StopCollider != nullptr)
	{
		m_StopCollider->SetDeleteFlag(true);
		m_StopCollider = nullptr;
	}
	if (m_AttackCollider != nullptr)
	{
		m_AttackCollider->SetDeleteFlag(true);
		m_AttackCollider = nullptr;
	}
}

/// @brief 撃破時にプレイヤーへ報酬資金を加算付与する
void Enemy::GiveRewards()
{
	Player3D* player = Master::m_Player;
	if (player != nullptr)
	{
		player->m_HaveMoney->AddMoney(m_HaveMoney);
	}
}