#include"EnemyBoss_1.h"
#include"Model.h"
#include"Master.h"
#include"Player3D.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Stage.h"
#include"DrawHp.h"
#include"SceneGame.h"
#include"GameManager.h"
#include"Wall.h"
#include"Scene.h"
#include"Tree.h"
#include"Effect.h"
#include"InputManager.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Magic_Ene.h"


/*
 * 目的（EnemyBoss_1のEnemyBoss_1処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
EnemyBoss_1::EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3,int money, bool is_separate_anim_)
	:Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3,money, is_separate_anim_)



{
	mfjumpPower = 150.0f;
	HighPositionFlag = false;
	attack_type_ = 0;
	attack1_combo_count_ = 0;
	chance_ = 30;
	attack_interval_ = 60;
	attack_count_ = 0;
	SetTag(Object3D::Tag3D_Enemy3D);
	
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	model_->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/Model/MagicAttack.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Jump Attack.mv1");

	model_->SetScale(VGet(4.0f, 4.0f, 4.0f));
	
	jump_attack_coiider_ = new SphereCollider(this, position_, 400.0f);

	
}

EnemyBoss_1::~EnemyBoss_1()
{

}


/*
 * 目的（EnemyBoss_1のUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyBoss_1::Update()
{
	SceneGame* game = Master::scene_manager_->GetSceneGame();
	if (game && game->game_manager_) {
		auto phase = game->game_manager_->GetCurrentPhase();
		if (phase == GameManager::Phase::kFadeOutToBoss || phase == GameManager::Phase::kFadeInBoss) {
			return;
		}
	}

	if (is_dead_)
	{
		DeathEnemy();
	}
	else
	{
		if (model_ != nullptr)
		{
		
			Attack();
			if (model_->GetNowState() != ANIMATION_ATTACK && model_->GetNowState() != ANIMATION_ATTACKJUMP)
			{
				RotationByMove();
				Move();
			}

			UpdateJumpPhysics();

			model_->Update();
			UpdateColliderPosition();
			jump_attack_coiider_->position_ = position_;
			
			if (position_.y < init_position_.y)
			{
				position_.y = init_position_.y;
			}

		}
	}
}


/*
 * 目的（EnemyBoss_1のDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyBoss_1::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
	if (Master::debug_->Getdebug() == true)
	{
		DrawCapsule3D(position_, VAdd(position_, VGet(0.0f, 150.0f, 0.0f)),
			size_,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
	}


}


/*
 * 目的（EnemyBoss_1のAttack処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyBoss_1::Attack()
{
	AnimationState now = model_->GetNowState();

	if (attack_count_ >= attack_interval_ && is_hit_attack_search_flag_)
	{
		attack_count_ = 0;
		is_hit_attack_search_flag_ = false;
		
		attack_type_ = GetRand(2);
		
		if (attack_type_ == 0)
		{
			attack1_combo_count_ = 3;
		}
		else if (attack_type_ == 1)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, go_position_, 0, 150);
			VECTOR leftGo = VTransform(go_position_, MGetRotY(-30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, leftGo, 0, 150);
			VECTOR rightGo = VTransform(go_position_, MGetRotY(30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, rightGo, 0, 150);
		}
		else if (attack_type_ == 2)
		{
			model_->ChangeAnimation(ANIMATION_ATTACK);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			mfjumpPower = 400.0f;
			HighPositionFlag = false;
		}
	}

	if (attack_type_ == 0 && attack1_combo_count_ > 0)
	{
		if (now == ANIMATION_NEUTRAL || now == ANIMATION_RUN)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			new Magic_Ene("Resource/2d/Damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 50.0f, 5, 30.0f, go_position_, 0, 150);
			
			attack1_combo_count_--;
		}
	}

	if (!(now == ANIMATION_ATTACKMAGIC) && !(now == ANIMATION_ATTACK))
	{
		if (attack1_combo_count_ <= 0)
		{
			attack_count_++;
		}
		is_attack_hit_judgment_flag_ = false;
	}
}

/*
 * 目的（EnemyBoss_1のOnTrigger処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyBoss_1::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0)return;
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK)
	{
		if (collider == jump_attack_coiider_ && check->parent_object_->GetTag() == Tag3D_Player3D)
		{
			Player3D* pPlayer = Master::player_;
			if (pPlayer == nullptr) return;
			if (check == pPlayer->GetCollisionCollider())
			{
				if (now == ANIMATION_ATTACK && !is_attack_hit_judgment_flag_)
				{

					pPlayer->Damage(attack_);
					is_attack_hit_judgment_flag_ = true;
				}
			}

		}
	}
}


/*
 * 目的（EnemyBoss_1のDeathEnemy処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyBoss_1::DeathEnemy()
{
	is_dead_ = true;
	model_->ChangeAnimation(ANIMATION_DYING);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_MAX);
	DeathColliderPosition();

	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		Master::game_clear_count_++;
		
		Delete();
		SetDeleteFlag(true);
	}

	model_->Update();
}


/*
 * 目的（EnemyBoss_1のDelete処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void EnemyBoss_1::Delete()
{
	Enemy::Delete();
	if (jump_attack_coiider_ != nullptr)
	{
		jump_attack_coiider_->SetDeleteFlag(true);
		jump_attack_coiider_ = nullptr;
	}
}


/*
 * 目的（EnemyBoss_1のUpdateJumpPhysics処理を行うため）
 * [入力] なし
 * [出力] なし
 * [副作用] ジャンプ時の座標更新
 */
void EnemyBoss_1::UpdateJumpPhysics()
{
	if (model_->GetNowState() == ANIMATION_ATTACK && attack_type_ == 2)
	{
		if (!HighPositionFlag)
		{
			position_ = VAdd(position_, VGet(0.0f, kJumpAscendSpeed, 0.0f));
			if (position_.y >= init_position_.y + mfjumpPower)
			{
				HighPositionFlag = true;
			}
		}
		else
		{
			position_ = VAdd(position_, VGet(0.0f, kJumpDescendSpeed, 0.0f));
		}

		if (position_.y <= init_position_.y)
		{
			position_.y = init_position_.y;
		}
	}
	else
	{
		HighPositionFlag = false;
	}
}
