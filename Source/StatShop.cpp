#include "StatShop.h"
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

namespace
{
	bool IsShopPhaseActive() {
		auto currentScene = Master::mpSceneManager->GetCurrentScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(currentScene);
		if (sceneGame) {
			return sceneGame->IsShopPhase();
		}
		return false;
	}
}

StatShop::StatShop(std::string filename, VECTOR vec)
	: Object3D(VGet(vec.x, vec.y, vec.z + 4000.0f))
	, mTargetPosition(vec)
	, mShopState(ShopState::WAIT_PHASE)
	, mnSelect(0)
	, mnSelectMax(4)
	, mnSelectMin(0)
	, mnLevelMaxHp(0)
	, mnLevelAttack(0)
	, mnLevelSpeed(0)
	, mnLevelEvasionSpeed(0)
	, mnLevelEvasionInvincibility(0)
{
	SetTag(Tag3D_Shop);
	mpModel = new Model(filename, mvPosition, false);
	mpShopIn = new SphereCollider(this, mvPosition, 200.0f);
	mpSafeZoon = new SphereCollider(this, mvPosition, 1000.0f); // 謨ｵ縺瑚ｿ代▼縺代↑縺・そ繝ｼ繝輔だ繝ｼ繝ｳ
	
	mnBgImageHandle = LoadGraph("Resource/stat_shop_bg.png");
	mbOldMouseDown = false;

	mnIconMaxHpHandle = LoadGraph("Resource/2D/icon_hp_up.png");
	mnIconAttackHandle = LoadGraph("Resource/2D/icon_attack_up.png");
	mnIconSpeedHandle = LoadGraph("Resource/2D/icon_speed_up.png");
	mnIconEvasionDistHandle = LoadGraph("Resource/2D/icon_evade_dist_up.png");
	mnIconEvasionInvHandle = LoadGraph("Resource/2D/icon_evade_inv_up.png");
}

StatShop::~StatShop()
{
	DeleteGraph(mnBgImageHandle);
	DeleteGraph(mnIconMaxHpHandle);
	DeleteGraph(mnIconAttackHandle);
	DeleteGraph(mnIconSpeedHandle);
	DeleteGraph(mnIconEvasionDistHandle);
	DeleteGraph(mnIconEvasionInvHandle);

	delete mpModel;
	delete mpShopIn;
	delete mpSafeZoon;
}

void StatShop::Draw()
{
	if (mShopState == ShopState::WAIT_PHASE) return;
	if (!IsShopPhaseActive()) return;

	auto mpPlayer = Master::mpPlayer;
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

	if (Master::StatShopClassOn)
	{
		player->mpHaveMoney->Draw();

		// 閭梧勹繧ФI
		DrawExtendGraph(300, 100, 1620, 800, mnBgImageHandle, TRUE);
		
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
			"Evasion Invincibility UP"
		};
		int levels[] = { mnLevelMaxHp, mnLevelAttack, mnLevelSpeed, mnLevelEvasionSpeed, mnLevelEvasionInvincibility };
		int icons[] = { mnIconMaxHpHandle, mnIconAttackHandle, mnIconSpeedHandle, mnIconEvasionDistHandle, mnIconEvasionInvHandle };

		for (int i = 0; i <= mnSelectMax; i++)
		{
			int color = (i == mnSelect) ? GetColor(255, 0, 0) : GetColor(255, 255, 255);
			if (i == mnSelect) DrawFormatString(330, 250 + i * 60, color, ">");
			
			// 繧｢繧､繧ｳ繝ｳ謠冗判 (40x40 繧ｵ繧､繧ｺ縺ｫ邵ｮ蟆上＠縺ｦ陦ｨ遉ｺ)
			DrawExtendGraph(360, 245 + i * 60, 360 + 40, 245 + i * 60 + 40, icons[i], TRUE);

			DrawFormatString(415, 250 + i * 60, color, "%s (Lv.%d) - Cost: %d", options[i], levels[i], GetCost(levels[i]));
		}
		
		DrawFormatString(350, 700, GetColor(200, 200, 200), "Up/Down: Select   Enter: Buy   Escape/Back: Close");
		SetFontSize(fontSize);
	}
	else
	{
		VECTOR DrawName3D = VAdd(mvPosition, VGet(0.0f, 250.0f, 0.0f));
		VECTOR DrawNameWorld = ConvWorldPosToScreenPos(DrawName3D);
		DrawFormatString(DrawNameWorld.x, DrawNameWorld.y, GetColor(255, 255, 0), "StatShop: Enter");

		mpModel->Draw();
	}
}

void StatShop::Update()
{
	if (mShopState == ShopState::WAIT_PHASE) return;
	
	// WALKING_OUT縺ｮ譎ゅ・縲√ヵ繧ｧ繝ｼ繧ｺ縺檎ｵゆｺ・＠縺ｦ縺・※繧らｧｻ蜍募・逅・ｒ邯壹￠繧句ｿ・ｦ√′縺ゅｋ縺溘ａ縲√％縺薙〒蛻・ｲ舌＠縺ｾ縺吶・
	if (!IsShopPhaseActive() && mShopState != ShopState::WALKING_OUT) return;

	auto currentScene = Master::mpSceneManager->GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(currentScene);
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

		if (InputManager::CheckDownKey(KEY_INPUT_ESCAPE) || InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::StatShopClassOn = false;
			Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
		}
	}

	movePosition();
}

void StatShop::movePosition()
{
	if (mShopState == ShopState::WALKING_IN)
	{
		VECTOR dir = VSub(mTargetPosition, mvPosition);
		dir.y = 0.0f;
		float dist = VSize(dir);
		if (dist < 10.0f)
		{
			mvPosition.x = mTargetPosition.x;
			mvPosition.z = mTargetPosition.z;
			mShopState = ShopState::ARRIVED;
			mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
		}
		else
		{
			VECTOR nDir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(nDir, 4.0f));
			mpModel->ChangeAnimation(ANIMATION_WALKING);
		}
	}
	else if (mShopState == ShopState::WALKING_OUT)
	{
		VECTOR startPos = VGet(mTargetPosition.x, mTargetPosition.y, mTargetPosition.z + 4000.0f);
		VECTOR dir = VSub(startPos, mvPosition);
		dir.y = 0.0f;
		float dist = VSize(dir);
		if (dist < 10.0f)
		{
			mShopState = ShopState::WAIT_PHASE;
			mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
		}
		else
		{
			VECTOR nDir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(nDir, 4.0f));
			mpModel->ChangeAnimation(ANIMATION_WALKING);
		}
	}
	else if (mShopState == ShopState::ARRIVED)
	{
		if (mvPosition.y > mTargetPosition.y) mvPosition.y -= 2.0f;
		mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
	}

	// 蠕・ｩ滉ｸｭ繧・蝣ｴ螳御ｺ・ｾ後・縲∝ｽ薙◆繧雁愛螳壹ｒ逕ｻ髱｢螟厄ｼ育樟蝨ｨ縺ｮ菴咲ｽｮ・峨↓遘ｻ蜍輔＆縺帙ｋ
	mpShopIn->mvPosition = mvPosition;
	mpSafeZoon->mvPosition = mvPosition;
	
	mpModel->SetPosition(mvPosition);
	mpModel->Update();
}

void StatShop::StartWalkingIn()
{
	if (mShopState == ShopState::WAIT_PHASE || mShopState == ShopState::WALKING_OUT)
	{
		mShopState = ShopState::WALKING_IN;
		mvPosition = VGet(mTargetPosition.x, mTargetPosition.y, mTargetPosition.z + 4000.0f);
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
	if (InputManager::CheckDownKey(KEY_INPUT_UP))
	{
		mnSelect--;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		mnSelect++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);
	}

	if (mnSelect < mnSelectMin) mnSelect = mnSelectMax;
	if (mnSelect > mnSelectMax) mnSelect = mnSelectMin;
}

void StatShop::BuyClass()
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

	bool isMouseDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool isMouseClick = (isMouseDown && !mbOldMouseDown);
	mbOldMouseDown = isMouseDown;

	int mx, my;
	GetMousePoint(&mx, &my);

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

	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{

		doBuy = true;
	}

	if (doBuy)
	{
		int* targetLevel = nullptr;
		if (mnSelect == 0) targetLevel = &mnLevelMaxHp;
		else if (mnSelect == 1) targetLevel = &mnLevelAttack;
		else if (mnSelect == 2) targetLevel = &mnLevelSpeed;
		else if (mnSelect == 3) targetLevel = &mnLevelEvasionSpeed;
		else if (mnSelect == 4) targetLevel = &mnLevelEvasionInvincibility;

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
				else if (mnSelect == 4) player->AddUpgradeEvasionInvincibility(5); // Extra 5 frames

				Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);
			}
			else
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW); // as error sound
			}
		}
	}
}

void StatShop::OnEnter(Collider* collider, Collider* check)
{
	if (!IsShopPhaseActive()) return;

	if (check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = dynamic_cast<Player3D*>(check->mpParentObject);
		if (pPlayer && collider == mpShopIn && pPlayer->GetCollisionCollider() == check)
		{
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN) && !Master::ShopClassOn && !Master::StatShopClassOn)
			{
				Master::StatShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
			}
		}
	}
}

void StatShop::OnTrigger(Collider* collider, Collider* check)
{
}

void StatShop::OnExit(Collider* collider, Collider* check)
{
}
