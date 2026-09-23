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

/// @brief Enemy繧ｯ繝ｩ繧ｹ縺ｮ繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
/// @param filename 菴ｿ逕ｨ縺吶ｋ3D繝｢繝・Ν縺ｮ繝輔ぃ繧､繝ｫ繝代せ
/// @param initPos 蛻晄悄驟咲ｽｮ蠎ｧ讓・
/// @param hp 蛻晄悄繝ｻ譛螟ｧ菴灘鴨蛟､
/// @param speed 遘ｻ蜍暮溷ｺｦ
/// @param attack 謾ｻ謦・鴨
/// @param HitSize 繧ｫ繝励そ繝ｫ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ蛻､螳壹し繧､繧ｺ
/// @param Serch1 邏｢謨ｵ・郁ｿｽ蠕難ｼ臥畑遽・峇蜊雁ｾ・
/// @param Serch2 謾ｻ謦・幕蟋狗畑遽・峇蜊雁ｾ・
/// @param Serch3 謗･霑大●豁｢逕ｨ遽・峇蜊雁ｾ・
/// @param money 蛟偵＠縺滄圀縺ｫ迯ｲ蠕励〒縺阪ｋ雉・≡
/// @param is_separate_anim_ 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧貞・髮｢隱ｭ縺ｿ霎ｼ縺ｿ縺吶ｋ縺九←縺・°縺ｮ繝輔Λ繧ｰ
Enemy::Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_)
	: Object3D(initPos)
	, hp_(hp)
	, speed_(speed)
	, is_invisible_(false)
	, angle_(0.0f)
	, target_angle_(0.0f)
	, size_(HitSize)
	, hit_search_(Serch1)
	, hit_attack_search_(Serch2)
	, hit_stop_search_(Serch3)
	, alg_hit_(20)
	, attack_(attack)
	, no_position_(VGet(0, 0, 0))
	, is_attack_hit_judgment_flag_(false)
	, is_hit_attack_search_flag_(false)
	, is_hit_search_stop_flag_(false)
	, is_hit_attack_flag_(false)
	, is_hit_search_flag_(false)
	, is_hit_judgment_flag_player_(false)
	, walk_count_(0)
	, walk_timer_(0)
	, is_dead_(false)
	, max_hp_(hp)
	, have_money_(money)
{
	SetTag(Object3D::Tag3D_Enemy3D);
	model_ = new Model(filename, initPos, is_separate_anim_);
	model_->SetScale(VGet(1.3f, 1.3f, 1.3f));
	init_position_ = initPos;
	max_hp_ = hp_;
	normal_speed_ = speed_;

	capsule_collider_ = new CapsuleCollider(this, position_, VAdd(position_, VGet(0.0f, size_ / 2, 0.0f)), size_);
	attach_collider_ = new SphereCollider(this, model_->GetAttachmentPosition(), 50.0f);
	serch_collider_ = new SphereCollider(this, position_, hit_search_);
	attack_collider_ = new SphereCollider(this, position_, hit_attack_search_);
	stop_collider_ = new SphereCollider(this, position_, hit_stop_search_);
}

/// @brief Enemy繧ｯ繝ｩ繧ｹ縺ｮ繝・せ繝医Λ繧ｯ繧ｿ
Enemy::~Enemy()
{
	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}
}

/// @brief 豈弱ヵ繝ｬ繝ｼ繝縺ｮ迥ｶ諷区峩譁ｰ蜃ｦ逅・ｒ陦後≧
/// @details 豁ｻ莠｡蛻､螳壹∵判謦・虚菴懊∝推繧ｳ繝ｩ繧､繝繝ｼ蠎ｧ讓吝酔譛溘∫ｧｻ蜍輔♀繧医・蝗櫁ｻ｢蛻ｶ蠕｡繧貞ｮ溯｡後☆繧・
void Enemy::Update()
{
	if (model_ != nullptr)
	{
		DeathEnemy();
		Attack();
		UpdateColliderPosition();
		RotationByMove();
		Move();

		model_->Update();
	}
}

/// @brief Enemy縺ｮ謠冗判蜃ｦ逅・ｒ陦後≧
/// @details 繝｢繝・Ν縺ｮ謠冗判縺翫ｈ縺ｳ繝・ヰ繝・げ繝輔Λ繧ｰ譛牙柑譎ゅ・繝ｯ繧､繝､繝ｼ繝輔Ξ繝ｼ繝謠冗判繧定｡後≧
void Enemy::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

/// @brief 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ繝代ち繝ｼ繝ｳ邂｡逅・畑諡｡蠑ｵ繝｡繧ｽ繝・ラ
void Enemy::AttackList()
{
}

/// @brief 謾ｻ謦・い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ髢句ｧ句愛螳壹♀繧医・繧ｿ繧､繝槭・蛻ｶ蠕｡繧定｡後≧
void Enemy::Attack()
{
	AnimationState now = model_->GetNowState();

	if (attack_count_ >= attack_interval_ && is_hit_attack_search_flag_)
	{
		Master::sound_manager_->PlaySE(SoundManager::SE_ATTACKSLIDE);
		attack_count_ = 0;
		model_->ChangeAnimation(ANIMATION_ATTACK);
		model_->SetLoop(false);
		model_->SetLoopFinishState(ANIMATION_NEUTRAL);
		is_hit_attack_search_flag_ = false;
	}

	if (!(now == ANIMATION_ATTACK))
	{
		is_attack_hit_judgment_flag_ = false;
		attack_count_++;
	}
}

/// @brief 繝励Ξ繧､繝､繝ｼ霑ｽ蠕鍋ｧｻ蜍輔♀繧医・螢√・蝨ｰ蠖｢縺ｨ縺ｮ陦晉ｪ√せ繝ｩ繧､繝芽ｨ育ｮ励ｒ陦後≧
void Enemy::Move()
{
	AnimationState now = model_->GetNowState();
	if (now == ANIMATION_ATTACK || now == ANIMATION_ATTACKMAGIC || now == ANIMATION_ATTACKJUMP) return;
	if (Master::is_safe_point_on_) position_ = init_position_;

	Player3D* pPlayer = Master::player_;
	move_vec_ = VGet(0.0f, 0.0f, 0.0f);

	{
		if (is_hit_search_stop_flag_)
		{
			speed_ = 0;
		}
		else
		{
			speed_ = normal_speed_;
		}

		go_position_ = VSub(pPlayer->GetPosition(), position_);
		go_position_.y = 0.0f;
		if (VSquareSize(go_position_) > 0.0001f) go_position_ = VNorm(go_position_);
		move_vec_ = go_position_;

		bool isMove = (move_vec_.x != 0.0f || move_vec_.z != 0.0f);
		if (isMove)
		{
			model_->ChangeAnimation(ANIMATION_RUN);
			target_angle_ = atan2f(move_vec_.x, move_vec_.z);
		}
		else
		{
			model_->ChangeAnimation(ANIMATION_NEUTRAL);
		}

		VECTOR old_position_ = position_;
		position_ = VAdd(position_, VScale(move_vec_, speed_));

		TerrainFollow(0.0f, 150.0f, 40.0f, 150.0f, -40.0f, 4.0f);
		const auto& walls = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);

		if (!walls.empty())
		{
			// Wall・亥｣・ｼ峨→縺ｮ蠖薙◆繧雁愛螳壹→螢√★繧雁・逅・
			for (size_t i = 0; i < walls.size(); i++)
			{
				Wall* wall = walls.at(i)->CastTo<Wall>();
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					if (HitCheck_Capsule_Triangle(
						position_,
						VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
						HitCheck_Capsule_Triangle(
							position_,
							VAdd(position_, VGet(0.0f, 200.0f, 0.0f)),
							80.0f,
							vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
						)
					{
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);
						float a = VDot(VScale(move_vec_, -1.0f), vertex.at(0).norm);
						slide = VAdd(move_vec_, VScale(vertex.at(0).norm, a));

						position_ = old_position_;
						position_ = VAdd(position_, VScale(slide, speed_));
					}
				}
			}
		}

		model_->SetPosition(position_);
	}
}

/// @brief 遘ｻ蜍墓婿蜷代↓蠢懊§縺滓ｮｵ髫守噪蝗櫁ｻ｢陬憺俣蜃ｦ逅・ｒ陦後≧
void Enemy::RotationByMove()
{
	float subAngle = target_angle_ - angle_;

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
	angle_ = target_angle_ - subAngle;

	rotation_.y = angle_ + DX_PI_F;
	model_->SetRotation(rotation_);
}

/// @brief 繝繝｡繝ｼ繧ｸ驕ｩ逕ｨ蜃ｦ逅・
/// @param damage 貂帛ｰ代＆縺帙ｋHP驥・
void Enemy::Damage(float damage, bool play_sound)
{
	VECTOR pop_pos = position_;
	pop_pos.y += size_;
	DamageUIManager::GetInstance()->AddDamage((int)damage, pop_pos, damage >= 100.0f);
	if (play_sound) {
		Master::sound_manager_->PlaySE(SoundManager::SE_HIT_SLASH);
	}
	hp_ -= damage;
	if (hp_ <= 0)
	{
		if (!is_dead_ && Master::score_manager_ != nullptr)
		{
			Master::score_manager_->AddDefeatedEnemy();
		}
		hp_ = 0;
		is_dead_ = true;
	}
}

/// @brief 豁ｻ莠｡譎ゅ・貍泌・蜀咲函縺翫ｈ縺ｳ繧ｪ繝悶ず繧ｧ繧ｯ繝亥炎髯､莠育ｴ・ｒ陦後≧
void Enemy::DeathEnemy()
{
	if (!is_dead_) return;

	model_->ChangeAnimation(ANIMATION_DYING);
	model_->SetLoop(false);
	model_->SetLoopFinishState(ANIMATION_MAX);

	Delete();
	if (model_->IsAnimationLoopFinish())
	{
		GiveRewards();
		SetDeleteFlag(true);
	}

	model_->Update();
}

/// @brief 豁ｻ莠｡譎ゅ↓蜷・ｨｮ繧ｳ繝ｩ繧､繝繝ｼ繧堤判髱｢螟夜▼縺矩□縺上∈遘ｻ蜍輔＆縺帙※辟｡蜉ｹ蛹悶☆繧・
void Enemy::DeathColliderPosition()
{
	VECTOR pos = VGet(10000, 10000, 10000);
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = pos;
		capsule_collider_->position2_ = pos;
	}
	if (attach_collider_ != nullptr)
	{
		attach_collider_->position_ = pos;
	}
	if (serch_collider_ != nullptr)
	{
		serch_collider_->position_ = pos;
	}
	if (stop_collider_ != nullptr)
	{
		stop_collider_->SetDeleteFlag(true);
		stop_collider_->position_ = pos;
	}
	if (attack_collider_ != nullptr)
	{
		attack_collider_->position_ = pos;
	}
}

/// @brief 莉悶が繝悶ず繧ｧ繧ｯ繝医・繧ｳ繝ｩ繧､繝繝ｼ縺ｨ謗･隗ｦ繧帝幕蟋九＠縺溽椪髢薙・蜑ｲ繧願ｾｼ縺ｿ蜃ｦ逅・
/// @param collider 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ
/// @param check 逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
void Enemy::OnEnter(Collider* collider, Collider* check)
{
	if (hp_ <= 0) return;
	if (collider == capsule_collider_ && check->parent_object_->GetTag() == Tag3D_Obj)
	{
		position_ = init_position_;
	}

	if (check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::player_;
		if (player == nullptr) return;

		if (collider == serch_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_search_flag_ = true;
		}
		if (collider == attack_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_attack_search_flag_ = true;
		}
		if (collider == stop_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_search_stop_flag_ = true;
		}
	}
}

/// @brief 莉悶が繝悶ず繧ｧ繧ｯ繝医・繧ｳ繝ｩ繧､繝繝ｼ縺ｨ謗･隗ｦ荳ｭ縺ｮ邯咏ｶ壼・逅・ｼ医・繝ｬ繧､繝､繝ｼ縺ｸ縺ｮ謾ｻ謦・愛螳夂ｭ会ｼ・
/// @param collider 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ
/// @param check 逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
void Enemy::OnTrigger(Collider* collider, Collider* check)
{
	if (hp_ <= 0) return;
	AnimationState now = model_->GetNowState();

	if (collider == attach_collider_ && check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::player_;
		if (player == nullptr) return;

		if (check == player->GetCollisionCollider())
		{
			if (now == ANIMATION_ATTACK && !is_attack_hit_judgment_flag_)
			{
				player->Damage(attack_);
				is_attack_hit_judgment_flag_ = true;
			}
		}
	}
}

/// @brief 莉悶が繝悶ず繧ｧ繧ｯ繝医・繧ｳ繝ｩ繧､繝繝ｼ縺ｨ髮｢閼ｱ縺励◆迸ｬ髢薙・蜑ｲ繧願ｾｼ縺ｿ蜃ｦ逅・
/// @param collider 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ
/// @param check 逶ｸ謇九・繧ｳ繝ｩ繧､繝繝ｼ
void Enemy::OnExit(Collider* collider, Collider* check)
{
	if (hp_ <= 0) return;

	if (check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = Master::player_;
		if (player == nullptr) return;

		if (collider == serch_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_search_flag_ = false;
		}
		if (collider == attack_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_attack_search_flag_ = false;
		}
		if (collider == stop_collider_ && player->GetCollisionCollider() == check)
		{
			is_hit_search_stop_flag_ = false;
		}
	}
}

/// @brief 蜷・さ繝ｩ繧､繝繝ｼ縺ｮ霑ｽ蠕謎ｽ咲ｽｮ蠎ｧ讓吶ｒ豈弱ヵ繝ｬ繝ｼ繝蜷梧悄譖ｴ譁ｰ縺吶ｋ
void Enemy::UpdateColliderPosition()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->position_ = position_;
		capsule_collider_->position2_ = VAdd(position_, VGet(0.0f, 150.0f, 0.0f));
	}
	if (attach_collider_ != nullptr)
	{
		attach_collider_->position_ = model_->GetAttachmentPosition();
	}
	if (serch_collider_ != nullptr)
	{
		serch_collider_->position_ = position_;
	}
	if (stop_collider_ != nullptr)
	{
		stop_collider_->position_ = VAdd(position_, VGet(0.0f, size_ / 2, 0.0f));
	}
	if (attack_collider_ != nullptr)
	{
		attack_collider_->position_ = VAdd(position_, VGet(0.0f, size_ / 2, 0.0f));
	}
}

/// @brief 蜈ｨ縺ｦ縺ｮ逋ｻ骭ｲ貂医∩繧ｳ繝ｩ繧､繝繝ｼ繧貞炎髯､繝槭・繧ｯ縺苓ｧ｣謾ｾ貅門ｙ繧偵☆繧・
void Enemy::Delete()
{
	if (capsule_collider_ != nullptr)
	{
		capsule_collider_->SetDeleteFlag(true);
		capsule_collider_ = nullptr;
	}
	if (attach_collider_ != nullptr)
	{
		attach_collider_->SetDeleteFlag(true);
		attach_collider_ = nullptr;
	}
	if (serch_collider_ != nullptr)
	{
		serch_collider_->SetDeleteFlag(true);
		serch_collider_ = nullptr;
	}
	if (stop_collider_ != nullptr)
	{
		stop_collider_->SetDeleteFlag(true);
		stop_collider_ = nullptr;
	}
	if (attack_collider_ != nullptr)
	{
		attack_collider_->SetDeleteFlag(true);
		attack_collider_ = nullptr;
	}
}

/// @brief 謦・ｴ譎ゅ↓繝励Ξ繧､繝､繝ｼ縺ｸ蝣ｱ驟ｬ雉・≡繧貞刈邂嶺ｻ倅ｸ弱☆繧・
void Enemy::GiveRewards()
{
	Player3D* player = Master::player_;
	if (player != nullptr)
	{
		player->have_money_->AddMoney(have_money_);
	}
}