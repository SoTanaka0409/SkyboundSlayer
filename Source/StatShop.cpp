#include "StatShop.h"
#include <fstream>

#include "ModelUtility.h"
#include "InputManager.h"
#include "Master.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "SphereCollider.h"
#include "Player3D.h"
#include "Chat.h"
#include "CapsuleCollider.h"
#include "SceneGame.h"
#include "GameManager.h"
#include "ItemManager.h"

namespace
{
	bool IsShopPhaseActive() {
		auto currentScene = Master::mpSceneManager->GetCurrentScene();
		SceneGame* sceneGame = Master::mpSceneManager->GetSceneGame();
		if (sceneGame) {
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
	, mnLevelMaxHp(0)
	, mnLevelAttack(0)
	, mnLevelSpeed(0)
	, mnLevelEvasionSpeed(0)
	, mnLevelEvasionInvincibility(0)
	, mFloatAngle(0.0f)
	, mBaseY(vec.y)
	
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

	if (mShopState == ShopState::WAIT_PHASE) return;
	if (!IsShopPhaseActive()) return;

	auto mpPlayer = Master::mpPlayer;
	Player3D* player = Master::mpPlayer;

	if (!player) return;

	if (Master::StatShopClassOn)
	{
		player->mpHaveMoney->Draw();

		// 髢ｭ譴ｧ蜍ｹ郢ｧﾐ､I
		
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
		DrawBox(300, 100, 1620, 800, GetColor(0, 0, 50), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawBox(300, 100, 1620, 800, GetColor(255, 255, 255), FALSE);
		
		int fontSize = GetFontSize();
		SetFontSize(40);
		DrawFormatString(350, 120, GetColor(255, 255, 255), "--- STATUS UPGRADE SHOP ---");
		SetFontSize(30);
		DrawFormatString(350, 180, GetColor(255, 255, 0), "Money: %d", player->mpHaveMoney->HaveMoney());

		const char* options[] = {
			"Max HP UP",
			"Attack UP",
			"Speed UP",
			"Evasion Distance UP",
			"Healing Potion"
		};
		int levels[] = { mnLevelMaxHp, mnLevelAttack, mnLevelSpeed, mnLevelEvasionSpeed, mnLevelEvasionInvincibility };
		int icons[] = { mnIconMaxHpHandle, mnIconAttackHandle, mnIconSpeedHandle, mnIconEvasionDistHandle, mnIconEvasionInvHandle };

		for (int i = 0; i <= mnSelectMax; i++)
		{
			int color = (i == mnSelect) ? GetColor(255, 0, 0) : GetColor(255, 255, 255);
			if (i == mnSelect) DrawFormatString(330, 250 + i * 60, color, ">");
			
			// 繧｢繧､繧ｳ繝ｳ謠冗判 (40x40 繧ｵ繧､繧ｺ縺ｫ邵ｮ蟆上＠縺ｦ陦ｨ遉ｺ)
			DrawExtendGraph(360, 245 + i * 60, 360 + 40, 245 + i * 60 + 40, icons[i], TRUE);

			if (i == 4) {
				DrawFormatString(415, 250 + i * 60, color, "%s - Cost: 100", options[i]);
			} else {
				DrawFormatString(415, 250 + i * 60, color, "%s (Lv.%d) - Cost: %d", options[i], levels[i], GetCost(levels[i]));
			}
		}
		
		DrawFormatString(350, 700, GetColor(200, 200, 200), "Up/Down: Select   Enter: Buy   Escape/Back: Close");
		SetFontSize(fontSize);
	}
	else
	{
		VECTOR DrawName3D = VAdd(position_, VGet(0.0f, 250.0f, 0.0f));
		VECTOR DrawNameWorld = ConvWorldPosToScreenPos(DrawName3D);
		
		VECTOR playerPos = player->GetPosition();
		float dx = playerPos.x - position_.x;
		float dz = playerPos.z - position_.z;
		float dist = sqrtf(dx * dx + dz * dz);
		
		// ショップ名と操作説明
		DrawFormatString(DrawNameWorld.x - 30, DrawNameWorld.y, GetColor(255, 255, 0), "[ ステータスショップ ]");
		DrawFormatString(DrawNameWorld.x - 10, DrawNameWorld.y + 20, GetColor(255, 255, 255), "Enterキーで開く");

		// プレイヤーが近づいたらメッセージを表示
		if (dist < 1000.0f)
		{
			DrawFormatString(DrawNameWorld.x - 60, DrawNameWorld.y - 30, GetColor(100, 255, 100), "「いらっしゃい！ 何か買いたいものはあるかい？」");
		}

		model_->Draw();
	}
}

void StatShop::Update()
{
	static bool isChecked = false;
	if (!isChecked) {
		std::ofstream ofs("shop_anim_debug.txt");
		int tempHandle = MV1LoadModel("Resource/Model/shop.mv1");
		if (tempHandle != -1) {
			int animNum = MV1GetAnimNum(tempHandle);
			ofs << "Total Animations: " << animNum << "\n";
			for(int i=0; i<animNum; ++i) {
				ofs << "Anim " << i << ": " << MV1GetAnimName(tempHandle, i) << "\n";
			}
			MV1DeleteModel(tempHandle);
		} else {
			ofs << "Failed to load shop.mv1\n";
		}
		ofs.close();
		isChecked = true;
	}


	
	if (mShopState == ShopState::WAIT_PHASE) return;
	
	// WALKING_OUT邵ｺｽｮ隴弱ｅ繝ｻ邵ｲ竏壹Ψ郢ｧｽｧ郢晢ｽｼ郢ｧｽｺ邵ｺ讙趣ｽｵ繧ｽｺ繝ｻｼ邵ｺｽｦ邵ｺ繝ｻ窶ｻ郢ｧ繧会ｽｧｽｻ陷榊供繝ｻ騾繝ｻｽ帝け螢ｹｿ郢ｧ蜿･ｽｿ繝ｻｽｦ竏壺ｲ邵ｺ繧ｽ狗ｸｺ貅假ｽ∫ｸｲ竏夲ｼ邵ｺ阮吶定崕繝ｻｽｲ闊鯉ｼ邵ｺｽｾ邵ｺ蜷ｶﾂ繝ｻ
	if (!IsShopPhaseActive() && mShopState != ShopState::WALKING_OUT) return;

	auto currentScene = Master::mpSceneManager->GetCurrentScene();
	SceneGame* sceneGame = Master::mpSceneManager->GetSceneGame();
	if (sceneGame && sceneGame->mpGameManager) {
		if (sceneGame->mpGameManager->GetShopTimer() <= 60 && sceneGame->mpGameManager->GetCurrentPhase() != GameManager::Phase::SHOP_3) {
			if (Master::StatShopClassOn) {
				Master::StatShopClassOn = false;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
			}
		}
	}

	if (Master::StatShopClassOn)
	{
		SelectClass();
		BuyClass();

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

	movePosition();
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
			mBaseY = position_.y;
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

int StatShop::GetCost(int level)
{
	return 100 + (level * 50); // Level 0 -> 100, Level 1 -> 150, etc.
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
	auto mpPlayer = Master::mpPlayer;
	Player3D* player = Master::mpPlayer;

	if (!player) return;

	bool isMouseDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool isMouseClick = (isMouseDown && !mbOldMouseDown);
	mbOldMouseDown = isMouseDown;

	
	int mx, my;
	GetMousePoint(&mx, &my);
	
	static int oldMx = 0, oldMy = 0;
	if (mx != oldMx || my != oldMy) {
		for (int i = 0; i <= mnSelectMax; i++)
		{
			int optY = 250 + i * 60;
			if (mx >= 350 && mx <= 1500 && my >= optY && my <= optY + 50)
			{
				if (mnSelect != i) {
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
			if (player->mpHaveMoney->HaveMoney() >= cost)
			{
				player->mpHaveMoney->PullMoney(cost);
				
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
			int* targetLevel = nullptr;
			if (mnSelect == 0) targetLevel = &mnLevelMaxHp;
			else if (mnSelect == 1) targetLevel = &mnLevelAttack;
			else if (mnSelect == 2) targetLevel = &mnLevelSpeed;
			else if (mnSelect == 3) targetLevel = &mnLevelEvasionSpeed;

			if (targetLevel)
			{
				int cost = GetCost(*targetLevel);
				if (player->mpHaveMoney->HaveMoney() >= cost)
				{
					player->mpHaveMoney->PullMoney(cost);
					(*targetLevel)++;

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
