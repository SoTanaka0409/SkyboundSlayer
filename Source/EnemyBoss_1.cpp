#include"EnemyBoss_1.h"
#include"Model.h"
#include"Master.h"
#include"Player3D.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"GameScene.h"
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

/// @param 蛻晄悄蛹悶ヱ繝ｩ繝｡繝ｼ繧ｿ・井ｽ咲ｽｮ縲？P縲・溷ｺｦ縲∝ｽ薙◆繧雁愛螳壹し繧､繧ｺ縲∵爾邏｢遽・峇縲∵園謖・≡縺ｪ縺ｩ・・
/// @details 3D繝｢繝・Ν縲√い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縲∝ｽ薙◆繧雁愛螳夂畑縺ｮ繝｡繝｢繝ｪ遒ｺ菫昴→險ｭ螳・
EnemyBoss_1::EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_)
	:Enemy(filename, initPos, hp, speed, 2, HitSize, Serch1, Serch2, Serch3, money, is_separate_anim_)
{
	mfjumpPower = 150.0f;       // 繧ｸ繝｣繝ｳ繝玲判謦・凾縺ｮ譛螟ｧ蛻ｰ驕秘ｫ伜ｺｦ
	HighPositionFlag = false;   // 繧ｸ繝｣繝ｳ繝励・鬆らせ蛻ｰ驕皮憾諷九・邂｡逅・
	attack_type_ = BossAttackType::kCombo;           // 迴ｾ蝨ｨ縺ｮ謾ｻ謦・ヱ繧ｿ繝ｼ繝ｳ縺ｮ遞ｮ鬘・
	attack1_combo_count_ = 0;   // 騾｣邯夐ｭ疲ｳ墓判謦・・谿九ｊ逋ｺ蜍募屓謨ｰ
	chance_ = kAttackChanceThreshold;               // 謾ｻ謦・ｻ蠎ｦ縺ｮ驥阪∩莉倥￠繝代Λ繝｡繝ｼ繧ｿ
	attack_interval_ = 60;      // 騾｣邯壽判謦・ｒ髦ｲ縺舌◆繧√・繧ｯ繝ｼ繝ｫ繧ｿ繧､繝・医ヵ繝ｬ繝ｼ繝・・
	attack_count_ = 0;          // 繧ｯ繝ｼ繝ｫ繧ｿ繧､繝險域ｸｬ逕ｨ繧ｫ繧ｦ繝ｳ繧ｿ
	jump_charge_timer_ = 0;
	jump_velocity_ = 0.0f;
	gravity_ = 4.0f;

	SetTag(Object3D::Tag3D_Enemy3D);

	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/model/character/11_idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/model/character/12_run.mv1");
	model_->AddAnimation(ANIMATION_DYING, "Resource/model/character/13_die.mv1");
	model_->AddAnimation(ANIMATION_ATTACKMAGIC, "Resource/model/character/14_magic_attack.mv1");
	model_->AddAnimation(ANIMATION_ATTACK, "Resource/model/character/17_jump_attack.mv1");

	model_->SetScale(VGet(4.0f, 4.0f, 4.0f));

	jump_attack_coiider_ = new SphereCollider(this, position_, 400.0f);
}

EnemyBoss_1::~EnemyBoss_1()
{

}

/// @details 繝懊せ縺ｮ蠎ｧ讓呎峩譁ｰ縲∵判謦・愛螳壹√い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ騾ｲ陦・
void EnemyBoss_1::Update()
{
	SceneGame* game = Master::scene_manager_->GetSceneGame();
	if (game && game->game_manager_) {
		auto phase = game->game_manager_->GetCurrentPhase();
		// 逕ｻ髱｢驕ｷ遘ｻ荳ｭ縺ｫ莠域悄縺帙〓謾ｻ謦・ｄ蠎ｧ讓咏ｧｻ蜍輔′逋ｺ逕溘☆繧九ヰ繧ｰ繧帝亟縺舌◆繧∝・逅・ｒ蛛懈ｭ｢
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
			// 謾ｻ謦・Δ繝ｼ繧ｷ繝ｧ繝ｳ荳ｭ縺ｮ荳崎・辟ｶ縺ｪ貊代ｊ遘ｻ蜍輔ｒ髦ｲ縺舌◆繧∝ｺｧ讓呎峩譁ｰ繧貞●豁｢
			if (model_->GetNowState() != ANIMATION_ATTACK && model_->GetNowState() != ANIMATION_ATTACKJUMP)
			{
				RotationByMove();
				Move();
			}

			UpdateJumpPhysics();

			// 謾ｻ謦・ｸｭ・・ove()縺悟他縺ｰ繧後↑縺・俣・峨ｂ繝｢繝・Ν縺ｮ蠎ｧ讓吶ｒ迚ｩ逅・ｺｧ讓吶↓蜷梧悄縺輔○繧・
			model_->SetPosition(position_);

			model_->Update();
			UpdateColliderPosition();
			jump_attack_coiider_->position_ = position_;

			// 蝨ｰ髱｢謚懊￠繝舌げ繧帝亟縺舌◆繧√・Y蠎ｧ讓吶・荳矩剞陬懈ｭ｣
			if (position_.y < init_position_.y)
			{
				position_.y = init_position_.y;
				model_->SetPosition(position_);
			}

		}
	}
}

/// @details 繝懊せ繝｢繝・Ν縺ｨ繝・ヰ繝・げ逕ｨ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ逕ｻ髱｢謠冗判
void EnemyBoss_1::Draw()
{
	if (model_ != nullptr)
	{
		model_->Draw();
	}
}

/// @details 荵ｱ謨ｰ縺ｫ繧医ｋ謾ｻ謦・ヱ繧ｿ繝ｼ繝ｳ縺ｮ豎ｺ螳壹→鬲疲ｳ輔が繝悶ず繧ｧ繧ｯ繝医・逕滓・
void EnemyBoss_1::Attack()
{
	AnimationState now = model_->GetNowState();

	// 繧ｯ繝ｼ繝ｫ繧ｿ繧､繝豸亥喧貂医∩縺九▽繧ｿ繝ｼ繧ｲ繝・ヨ繧呈黒謐峨＠縺ｦ縺・ｋ蝣ｴ蜷医・縺ｿ謾ｻ謦・幕蟋・
	if (attack_count_ >= attack_interval_ && is_hit_attack_search_flag_)
	{
		attack_count_ = 0;
		is_hit_attack_search_flag_ = false;

		attack_type_ = static_cast<BossAttackType>(GetRand(2));

		if (attack_type_ == BossAttackType::kCombo)
		{
			attack1_combo_count_ = 3;
		}
		else if (attack_type_ == BossAttackType::kMagic)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);

			// 繝懊せ縺ｮ蠑ｾ縺ｮ繧ｵ繧､繧ｺ縺ｨ蠖薙◆繧雁愛螳壹ｒ1.5蛟阪↓縺吶ｋ (50.0f -> 75.0f)
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), kMagicScale, kMagicDamage, kMagicSpeed, go_position_, 0, kMagicLifetime);
			VECTOR leftGo = VTransform(go_position_, MGetRotY(-30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), kMagicScale, kMagicDamage, kMagicSpeed, leftGo, 0, kMagicLifetime);
			VECTOR rightGo = VTransform(go_position_, MGetRotY(30.0f * DX_PI_F / 180.0f));
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), kMagicScale, kMagicDamage, kMagicSpeed, rightGo, 0, kMagicLifetime);
		}
		else if (attack_type_ == BossAttackType::kJump)
		{
			model_->ChangeAnimation(ANIMATION_ATTACK);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);
			
			// 繧ｸ繝｣繝ｳ繝鈴幕蟋句燕縺ｮ繧ｿ繝｡譎る俣・医い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ蜷梧悄・峨・縺溘ａ縺ｫ蛻晄悄蛹・
			jump_charge_timer_ = 0;
			forward_speed_ = 0.0f;
			jump_velocity_ = 0.0f;
			HighPositionFlag = false;
		}
	}

	// 繝代ち繝ｼ繝ｳ0縺ｮ蝣ｴ蜷医√Δ繝ｼ繧ｷ繝ｧ繝ｳ螳御ｺ・↓蜷医ｏ縺帙※谿ｵ髫守噪縺ｫ鬲疲ｳ輔ｒ逕滓・縺吶ｋ莉墓ｧ・
	if (attack_type_ == BossAttackType::kCombo && attack1_combo_count_ > 0)
	{
		if (now == ANIMATION_NEUTRAL || now == ANIMATION_RUN)
		{
			model_->ChangeAnimation(ANIMATION_ATTACKMAGIC);
			model_->SetLoop(false);
			model_->SetLoopFinishState(ANIMATION_NEUTRAL);

			// 繝懊せ縺ｮ蠑ｾ縺ｮ繧ｵ繧､繧ｺ縺ｨ蠖薙◆繧雁愛螳壹ｒ1.5蛟阪↓縺吶ｋ (100.0f -> 150.0f)
			new Magic_Ene("Resource/image/battle/01_damage.png", VAdd(position_, VGet(0.0f, 100.0f, 0.0f)), 150.0f, 5, 30.0f, go_position_, 0, kMagicLifetime);

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

/// @param 閾ｪ霄ｫ縺ｮ繧ｳ繝ｩ繧､繝繝ｼ縲∬｡晉ｪ∝ｯｾ雎｡縺ｮ繧ｳ繝ｩ繧､繝繝ｼ
/// @details 繝励Ξ繧､繝､繝ｼ縺ｮHP貂帛ｰ代→縲√ヲ繝・ヨ貂医∩繝輔Λ繧ｰ縺ｮ險ｭ螳・
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
				// 螟壽ｮｵ繝偵ャ繝医ｒ髦ｲ縺舌◆繧√・蝗槭・繧ｸ繝｣繝ｳ繝玲判謦・↓縺､縺阪ム繝｡繝ｼ繧ｸ縺ｯ1蠎ｦ縺ｮ縺ｿ
				if (now == ANIMATION_ATTACK && !is_attack_hit_judgment_flag_)
				{
					pPlayer->Damage(kJumpAttackDamage);
					is_attack_hit_judgment_flag_ = true;
				}
			}
		}
	}
}

/// @details 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ螳御ｺ・ｾ後・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ遐ｴ譽・ｺ育ｴ・√♀繧医・繧ｯ繝ｪ繧｢繝輔Λ繧ｰ縺ｮ譖ｴ譁ｰ
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
		// 繧ｲ繝ｼ繝騾ｲ陦檎ｮ｡逅・ｸ翫√・繧ｹ縺ｮ險惹ｼ先焚繧偵け繝ｪ繧｢譚｡莉ｶ縺ｨ縺励※縺・ｋ縺溘ａ縺ｮ繧ｫ繧ｦ繝ｳ繝医い繝・・
		Master::game_clear_count_++;

		Delete();
		SetDeleteFlag(true);
	}

	model_->Update();
}

/// @details 菫晄戟縺励※縺・ｋ繧ｳ繝ｩ繧､繝繝ｼ縺ｮ繝｡繝｢繝ｪ隗｣謾ｾ繝輔Λ繧ｰ險ｭ螳・
void EnemyBoss_1::Delete()
{
	Enemy::Delete();
	// 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ髦ｲ豁｢縺ｮ縺溘ａ縲∝虚逧・｢ｺ菫昴＠縺溷崋譛峨さ繝ｩ繧､繝繝ｼ繧堤ｴ譽・☆繧・
	if (jump_attack_coiider_ != nullptr)
	{
		jump_attack_coiider_->SetDeleteFlag(true);
		jump_attack_coiider_ = nullptr;
	}
}

/// @details 謾ｻ謦・ち繧､繝・譎ゅ・繝懊せ縺ｮY蠎ｧ讓吶♀繧医・霆碁％險育ｮ励・譖ｴ譁ｰ
void EnemyBoss_1::UpdateJumpPhysics()
{
	if (model_->GetNowState() == ANIMATION_ATTACK && attack_type_ == BossAttackType::kJump)
	{
		jump_charge_timer_++;
		
		// 貅懊ａ譛滄俣荳ｭ・・0繝輔Ξ繝ｼ繝逶ｮ縺ｾ縺ｧ・峨・繝励Ξ繧､繝､繝ｼ縺ｮ譁ｹ蜷代ｒ蜷代￥
		if (jump_charge_timer_ <= kJumpChargeFrames)
		{
			VECTOR toPlayer = VSub(Master::player_->GetPosition(), position_);
			target_angle_ = atan2f(toPlayer.x, toPlayer.z);
			RotationByMove();
		}

		// 30繝輔Ξ繝ｼ繝逶ｮ・医い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ貅懊ａ縺檎ｵゅｏ繧九ち繧､繝溘Φ繧ｰ・峨〒繧ｸ繝｣繝ｳ繝励・迚ｩ逅・ヱ繝ｩ繝｡繝ｼ繧ｿ繧定ｨ育ｮ励・險ｭ螳・
		if (jump_charge_timer_ == kJumpChargeFrames)
		{
			jump_velocity_ = kJumpInitialVelocity; // EnemyMonster縺ｨ蜷後§蛻晞・
			
			Player3D* pPlayer = Master::player_;
			float dist = 0.0f;
			if (pPlayer) {
				VECTOR toPlayer = VSub(pPlayer->GetPosition(), position_);
				toPlayer.y = 0.0f;
				dist = VSize(toPlayer);
				if (dist > 0.1f) {
					jump_target_dir_ = VNorm(toPlayer);
				} else {
					jump_target_dir_ = go_position_;
				}
			} else {
				jump_target_dir_ = go_position_;
			}
			
			// 繧ｸ繝｣繝ｳ繝励・邱上ヵ繝ｬ繝ｼ繝謨ｰ = (80 / 4) * 2 = 40繝輔Ξ繝ｼ繝
			float jump_time = (jump_velocity_ / gravity_) * 2.0f;
			forward_speed_ = dist / jump_time;
			
			if (forward_speed_ > 60.0f) {
				forward_speed_ = 60.0f;
			}
		}
		
		// 30繝輔Ξ繝ｼ繝逶ｮ莉･髯阪°繧牙ｮ滄圀縺ｮ遘ｻ蜍輔ｒ髢句ｧ・
		if (jump_charge_timer_ > 30)
		{
			position_.y += jump_velocity_;
			position_.x += jump_target_dir_.x * forward_speed_;
			position_.z += jump_target_dir_.z * forward_speed_;
			jump_velocity_ -= gravity_;

			// 逹蝨ｰ蛻､螳・
			if (position_.y <= init_position_.y)
			{
				if (jump_velocity_ < 0.0f) {
					Master::sound_manager_->PlaySE(SoundManager::SE_BOSS_JUMP);
				}
				position_.y = init_position_.y;
				// 逹蝨ｰ縺励◆繧画ｨｪ貊代ｊ・域ｰｴ蟷ｳ遘ｻ蜍包ｼ峨ｒ蛛懈ｭ｢
				forward_speed_ = 0.0f;
				jump_velocity_ = 0.0f;
			}
		}
	}
	else
	{
		jump_charge_timer_ = 0;
	}
}