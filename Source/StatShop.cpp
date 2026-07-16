#include "StatShop.h"
#include "InputManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "SphereCollider.h"
#include "Player3D.h"
#include "CapsuleCollider.h"
#include "SceneGame.h"
#include "GameManager.h"
#include "ItemManager.h"

namespace
{
	bool IsShopPhaseActive()
	{
		SceneGame* sceneGame = Master::mpSceneManager->GetSceneGame();
		if (sceneGame)
		{
			return sceneGame->IsShopPhase();
		}
		return false;
	}
}

StatShop::StatShop(std::string filename, VECTOR vec)
	: Object3D(vec)
	, mvStartPosition(vec)
	, mShopState(ShopState::WAIT_PHASE)
	, mnSelect(0)
	, mnSelectMax(4)
	, mnSelectMin(0)
	, mnUpgradeMaxHpCount(0)
	, mnUpgradeAttackCount(0)
	, mnUpgradeSpeedCount(0)
	, mnUpgradeEvasionSpeedCount(0)


	
{
	SetTag(Tag3D_Shop);
	model_ = new Model(filename, position_, true);
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	model_->ChangeAnimation(ANIMATION_NEUTRAL);
	mpShopIn = new SphereCollider(this, position_, 200.0f);
	mpSafeZoon = new SphereCollider(this, position_, 1000.0f); // 隰ｨｽｵ邵ｺ迹夲ｽｿ莉｣笆ｼ邵ｺ莉｣竊醍ｸｺ繝ｻ縺晉ｹ晢ｽｼ郢晁ｼ斐□郢晢ｽｼ郢晢ｽｳ
	mbOldMouseDown = false;

	mnIconMaxHpHandle = LoadGraph("Resource/2D/icon_hp_up.png");
	mnIconAttackHandle = LoadGraph("Resource/2D/icon_attack_up.png");
	mnIconSpeedHandle = LoadGraph("Resource/2D/icon_speed_up.png");
	mnIconEvasionDistHandle = LoadGraph("Resource/2D/icon_evade_dist_up.png");
	mnIconEvasionInvHandle = LoadGraph("Resource/2D/icon_evade_inv_up.png");

	auto pPlayer = Master::mpPlayer;
	auto player = dynamic_cast<Player3D*>(pPlayer);
	mTargetPosition = player->GetFirstPos();
}

StatShop::~StatShop()
{
	DeleteGraph(mnIconMaxHpHandle);
	DeleteGraph(mnIconAttackHandle);
	DeleteGraph(mnIconSpeedHandle);
	DeleteGraph(mnIconEvasionDistHandle);
	DeleteGraph(mnIconEvasionInvHandle);

	delete model_;
	if (mpShopIn) mpShopIn->SetDeleteFlag(true);
	if (mpSafeZoon) mpSafeZoon->SetDeleteFlag(true);
}

void StatShop::Draw()
{
	if (mShopState == ShopState::WAIT_PHASE)
	{
		return;
	}
	if (!IsShopPhaseActive())
	{
		return;
	}

	Player3D* player = Master::mpPlayer;
	if (!player)
	{
		return;
	}

	if (Master::StatShopClassOn)
	{
		DrawShopMenu(player);
	}
	else
	{
		DrawShopNpc(player);
	}
}

void StatShop::DrawShopMenu(Player3D* player)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(300, 100, 1620, 800, GetColor(0, 0, 50), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(300, 100, 1620, 800, GetColor(255, 255, 255), FALSE);

	int fontSize = GetFontSize();
	DrawShopHeader(player);
	DrawShopOptions();
	DrawShopFooter();
	SetFontSize(fontSize);
}

void StatShop::DrawShopHeader(Player3D* player)
{
	SetFontSize(40);
	DrawFormatString(350, 120, GetColor(255, 255, 255), "--- STATUS UPGRADE SHOP ---");
	SetFontSize(30);
	DrawFormatString(350, 180, GetColor(255, 255, 0), "Money: %d", player->have_money_->HaveMoney());
}

void StatShop::DrawShopOptions()
{
	const char* options[] =
	{
		"Max HP UP",
		"Attack UP",
		"Speed UP",
		"Evasion Distance UP",
		"Healing Potion"
	};
	int upgradeCounts[] =
	{
		mnUpgradeMaxHpCount,
		mnUpgradeAttackCount,
		mnUpgradeSpeedCount,
		mnUpgradeEvasionSpeedCount
	};
	int icons[] =
	{
		mnIconMaxHpHandle,
		mnIconAttackHandle,
		mnIconSpeedHandle,
		mnIconEvasionDistHandle,
		mnIconEvasionInvHandle
	};

	for (int i = 0; i <= mnSelectMax; i++)
	{
		int color = (i == mnSelect) ? GetColor(255, 0, 0) : GetColor(255, 255, 255);
		int optionY = 250 + i * 60;
		if (i == mnSelect)
		{
			DrawFormatString(330, optionY, color, ">");
		}

		DrawExtendGraph(360, 245 + i * 60, 400, 285 + i * 60, icons[i], TRUE);
		if (i == 4)
		{
			DrawFormatString(415, optionY, color, "%s - Cost: 100", options[i]);
		}
		else
		{
			DrawFormatString(415, optionY, color, "%s (UP %d) - Cost: %d", options[i], upgradeCounts[i], GetCost(upgradeCounts[i]));
		}
	}
}

void StatShop::DrawShopFooter()
{
	DrawFormatString(350, 700, GetColor(200, 200, 200), "Up/Down: Select   Enter: Buy   Escape/Back: Close");
}

void StatShop::DrawShopNpc(Player3D* player)
{
	VECTOR drawName3D = VAdd(position_, VGet(0.0f, 250.0f, 0.0f));
	VECTOR drawNameWorld = ConvWorldPosToScreenPos(drawName3D);

	VECTOR playerPos = player->GetPosition();
	float dx = playerPos.x - position_.x;
	float dz = playerPos.z - position_.z;
	float dist = sqrtf(dx * dx + dz * dz);

	int drawX = static_cast<int>(drawNameWorld.x);
	int drawY = static_cast<int>(drawNameWorld.y);

	DrawFormatString(drawX - 30, drawY, GetColor(255, 255, 0), "[ ステータスショップ ]");
	DrawFormatString(drawX - 10, drawY + 20, GetColor(255, 255, 255), "Enterキーで開く");

	if (dist < 1000.0f)
	{
		DrawFormatString(drawX - 60, drawY - 30, GetColor(100, 255, 100), "「いらっしゃい！ 何か買いたいものはあるかい？」");
	}

	model_->Draw();
}
void StatShop::Update()
{
	if (!CanUpdateShop())
	{
		return;
	}

	CloseShopIfPhaseEnding();

	if (Master::StatShopClassOn)
	{
		UpdateShopMenu();
	}

	movePosition();
}

bool StatShop::CanUpdateShop() const
{
	if (mShopState == ShopState::WAIT_PHASE)
	{
		return false;
	}

	if (!IsShopPhaseActive() && mShopState != ShopState::WALKING_OUT)
	{
		return false;
	}

	return true;
}

void StatShop::CloseShopIfPhaseEnding()
{
	SceneGame* sceneGame = Master::mpSceneManager->GetSceneGame();
	if (!sceneGame || !sceneGame->game_manager_)
	{
		return;
	}

	bool isClosingSoon = sceneGame->game_manager_->GetShopTimer() <= 60;
	bool isFinalShop = sceneGame->game_manager_->GetCurrentPhase() == GameManager::Phase::kShop3;
	if (isClosingSoon && !isFinalShop && Master::StatShopClassOn)
	{
		Master::StatShopClassOn = false;
		Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
	}
}

void StatShop::UpdateShopMenu()
{
	SelectClass();
	BuyClass();
	HandleShopCloseInput();
}

void StatShop::HandleShopCloseInput()
{
	static int oldEsc = 0;
	static int oldBack = 0;
	int currentEsc = CheckHitKey(KEY_INPUT_ESCAPE);
	int currentBack = CheckHitKey(KEY_INPUT_BACK);

	if ((currentEsc && !oldEsc) || (currentBack && !oldBack))
	{
		Master::StatShopClassOn = false;
		Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
	}
	oldEsc = currentEsc;
	oldBack = currentBack;
}
void StatShop::movePosition()
{
	if (mShopState == ShopState::WALKING_IN)
	{
		VECTOR dir = VSub(mTargetPosition, position_);
		dir.y = 0.0f;
		float dist = VSize(dir);
		if (dist < 10.0f)
		{
			position_.x = mTargetPosition.x;
			position_.z = mTargetPosition.z;
			mShopState = ShopState::ARRIVED;
			model_->ChangeAnimation(ANIMATION_NEUTRAL);
		}
		else
		{
			VECTOR nDir = VNorm(dir);
			position_ = VAdd(position_, VScale(nDir, 4.0f));
			rotation_.y = atan2f(-nDir.x, -nDir.z); // 騾ｲ陦梧婿蜷代ｒ蜷代￥
			model_->ChangeAnimation(ANIMATION_RUN);
		}
	}
	else if (mShopState == ShopState::WALKING_OUT)
	{
		VECTOR startPos = VGet(mvStartPosition.x, mvStartPosition.y, mvStartPosition.z);
		VECTOR dir = VSub(startPos, position_);
		dir.y = 0.0f;
		float dist = VSize(dir);
		if (dist < 10.0f)
		{
			mShopState = ShopState::WAIT_PHASE;
			model_->ChangeAnimation(ANIMATION_NEUTRAL);
		}
		else
		{
			VECTOR nDir = VNorm(dir);
			position_ = VAdd(position_, VScale(nDir, 4.0f));
			rotation_.y = atan2f(-nDir.x, -nDir.z); // 騾ｲ陦梧婿蜷代ｒ蜷代￥
			model_->ChangeAnimation(ANIMATION_RUN);
		}
	}
	else if (mShopState == ShopState::ARRIVED)
	{
		
		model_->ChangeAnimation(ANIMATION_NEUTRAL);
	}

	// 蛻､螳壹ｒ逕ｻ髱｢螟悶↓遘ｻ蜍輔＆縺帙ｋ蜃ｦ逅縺ｪ縺ｩ
	if (mShopState == ShopState::ARRIVED)
	{
		mpShopIn->position_ = position_;
		mpSafeZoon->position_ = position_;
	}
	else
	{
		VECTOR hidePos = VGet(position_.x, position_.y - 10000.0f, position_.z);
		mpShopIn->position_ = hidePos;
		mpSafeZoon->position_ = hidePos;
	}
	
	if(model_) {
		model_->SetPosition(position_);
		model_->SetRotation(rotation_);
		model_->Update(); // 縺薙％縺悟他縺ｰ繧後※縺縺ｪ縺九▲縺溘◆繧√√い繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ(繝｢繝ｼ繧ｷ繝ｧ繝ｳ)縺碁ｲ縺ｾ縺ｪ縺九▲縺
	}
}
void StatShop::StartWalkingIn()
{
	if (mShopState == ShopState::WAIT_PHASE || mShopState == ShopState::WALKING_OUT)
	{
		mShopState = ShopState::WALKING_IN;
		position_ = VGet(mvStartPosition.x, mvStartPosition.y, mvStartPosition.z );
	}
}

void StatShop::StartWalkingOut()
{
	if (mShopState == ShopState::ARRIVED || mShopState == ShopState::WALKING_IN)
	{
		mShopState = ShopState::WALKING_OUT;
		Master::StatShopClassOn = false;
	}
}

int StatShop::GetCost(int upgradeCount)
{
	return 100 + (upgradeCount * 50);
}

void StatShop::SelectClass()
{
	static int oldUp = 0;
	static int oldDown = 0;
	int currentUp = CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W);
	int currentDown = CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S);

	if (currentUp && !oldUp)
	{
		mnSelect--;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	if (currentDown && !oldDown)
	{
		mnSelect++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
	}

	oldUp = currentUp;
	oldDown = currentDown;

	if (mnSelect < mnSelectMin) mnSelect = mnSelectMax;
	if (mnSelect > mnSelectMax) mnSelect = mnSelectMin;
}

void StatShop::BuyClass()
{
	Player3D* player = Master::mpPlayer;

	if (!player) return;

	bool isMouseDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool isMouseClick = (isMouseDown && !mbOldMouseDown);
	mbOldMouseDown = isMouseDown;

	
	int mx, my;
	GetMousePoint(&mx, &my);
	
	static int oldMx = 0, oldMy = 0;
	if (mx != oldMx || my != oldMy)
			{
		for (int i = 0; i <= mnSelectMax; i++)
		{
			int optY = 250 + i * 60;
			if (mx >= 350 && mx <= 1500 && my >= optY && my <= optY + 50)
			{
				if (mnSelect != i)
			{
					mnSelect = i;
					Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
				}
				break;
			}
		}
		oldMx = mx;
		oldMy = my;
	}


	bool doBuy = false;

	if (isMouseClick)
	{
		for (int i = 0; i <= mnSelectMax; i++)
		{
			int optY = 250 + i * 60;
			if (mx >= 350 && mx <= 1500 && my >= optY && my <= optY + 50)
			{
				mnSelect = i;
				doBuy = true;
			}
		}
	}

	static int oldReturn = 0;
	int currentReturn = CheckHitKey(KEY_INPUT_RETURN);
	if (currentReturn && !oldReturn)
	{
		doBuy = true;
	}
	oldReturn = currentReturn;

	if (doBuy)
	{
		if (mnSelect == 4)
		{
			int cost = 100;
			if (player->have_money_->HaveMoney() >= cost)
			{
				player->have_money_->PullMoney(cost);
				
				Item::ItemInformation* info = new Item::ItemInformation();
				info->ID = Item::ItemID::HEAL;
				info->Count = 1;
				info->isLog = true;
				Master::mpItemManager->AddItem(info);
				
				Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);
			}
			else
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW); // as error sound
			}
		}
		else
		{
			int* targetUpgradeCount = nullptr;
			if (mnSelect == 0) targetUpgradeCount = &mnUpgradeMaxHpCount;
			else if (mnSelect == 1) targetUpgradeCount = &mnUpgradeAttackCount;
			else if (mnSelect == 2) targetUpgradeCount = &mnUpgradeSpeedCount;
			else if (mnSelect == 3) targetUpgradeCount = &mnUpgradeEvasionSpeedCount;

			if (targetUpgradeCount)
			{
				int cost = GetCost(*targetUpgradeCount);
				if (player->have_money_->HaveMoney() >= cost)
				{
					player->have_money_->PullMoney(cost);
					(*targetUpgradeCount)++;

					// Apply stat bonuses
					if (mnSelect == 0) player->AddUpgradeMaxHp(10.0f); // HP +10
					else if (mnSelect == 1) player->AddUpgradeAttack(1.0f); // Attack +1
					else if (mnSelect == 2) player->AddUpgradeSpeed(0.5f); // Speed +0.5
					else if (mnSelect == 3) player->AddUpgradeEvasionSpeed(2.0f); // Evasion Speed +2

					Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);
				}
				else
				{
					Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW); // as error sound
				}
			}
		}
	}
}

void StatShop::OnEnter(Collider* collider, Collider* check)
{
	if (!IsShopPhaseActive()) return;

	if (check->parent_object_ && check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = check->parent_object_->CastTo<Player3D>();
		if (pPlayer && collider == mpShopIn && pPlayer->GetCollisionCollider() == check)
		{
			static int oldEnter = 0;
			int currentEnter = CheckHitKey(KEY_INPUT_RETURN);
			if (currentEnter && !oldEnter && !Master::StatShopClassOn)
			{
				Master::StatShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
			}
			oldEnter = currentEnter;
		}
	}
}

void StatShop::OnTrigger(Collider* collider, Collider* check)
{
}

void StatShop::OnExit(Collider* collider, Collider* check)
{
}






