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
		SceneGame* sceneGame = Master::m_SceneManager->GetSceneGame();
		if (sceneGame)
		{
			return sceneGame->IsShopPhase();
		}
		return false;
	}
}

/// @param filename(モデルパス), vec(初期生成座標)
/// @details 3DモデルとUIリソースのVRAMロード、およびショップ判定・安全地帯用コライダーの初期構築
StatShop::StatShop(std::string filename, VECTOR vec)
	: Object3D(vec)
	, mvStartPosition(vec)
	, m_ShopState(ShopState::WAIT_PHASE)
	, m_Select(0)
	, m_SelectMax(4)
	, m_SelectMin(0)
	, m_UpgradeMaxHpCount(0)
	, m_UpgradeAttackCount(0)
	, m_UpgradeSpeedCount(0)
	, m_UpgradeEvasionSpeedCount(0)
{
	SetTag(Tag3D_Shop);
	m_Model = new Model(filename, m_Position, true);
	m_Model->AddAnimation(ANIMATION_NEUTRAL, "Resource/model/character/11_idle.mv1");
	m_Model->AddAnimation(ANIMATION_RUN, "Resource/model/character/12_run.mv1");
	m_Model->ChangeAnimation(ANIMATION_NEUTRAL);
	m_ShopIn = new SphereCollider(this, m_Position, 200.0f);
	m_SafeZoon = new SphereCollider(this, m_Position, 1000.0f); // 敵の侵入を防ぎ、プレイヤーの安全を確保するための広域コライダー
	m_OldMouseDown = false;

	m_IconMaxHpHandle = LoadGraph("Resource/image/shop/01_hp_icon.png");
	m_IconAttackHandle = LoadGraph("Resource/image/shop/02_attack_icon.png");
	m_IconSpeedHandle = LoadGraph("Resource/image/shop/03_speed_icon.png");
	m_IconEvasionDistHandle = LoadGraph("Resource/image/shop/04_evade_dist_icon.png");
	m_IconEvasionInvHandle = LoadGraph("Resource/image/shop/05_evade_inv_icon.png");

	auto pPlayer = Master::m_Player;
	auto player = dynamic_cast<Player3D*>(pPlayer);
	m_TargetPosition = player->GetFirstPos();
}

/// @details UI画像やモデル、コライダーを確実に破棄し、シーン離脱時のメモリリークを防ぐ
StatShop::~StatShop()
{
	DeleteGraph(m_IconMaxHpHandle);
	DeleteGraph(m_IconAttackHandle);
	DeleteGraph(m_IconSpeedHandle);
	DeleteGraph(m_IconEvasionDistHandle);
	DeleteGraph(m_IconEvasionInvHandle);

	delete m_Model;
	if (m_ShopIn) m_ShopIn->SetDeleteFlag(true);
	if (m_SafeZoon) m_SafeZoon->SetDeleteFlag(true);
}

/// @details フェーズやプレイヤーのUI操作状態に応じて、NPCモデルまたはショップ画面UIの描画を排他制御する
void StatShop::Draw()
{
	if (m_ShopState == ShopState::WAIT_PHASE)
	{
		return;
	}
	if (!IsShopPhaseActive())
	{
		return;
	}

	Player3D* player = Master::m_Player;
	if (!player)
	{
		return;
	}

	if (Master::m_IsStatShopOn)
	{
		DrawShopMenu(player);
	}
	else
	{
		DrawShopNpc(player);
	}
}

/// @param player (所持金参照用)
/// @details ショップ画面の半透明背景パネルと、ヘッダー・選択肢・フッターの各UI要素を合成描画する
void StatShop::DrawShopMenu(Player3D* player)
{
	// 全画面の背景を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(0, 0, 0), TRUE);

	// ショップ全体の背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 210);
	DrawBox(300, 100, 1620, 800, GetColor(20, 20, 30), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(300, 100, 1620, 160, GetColor(50, 50, 80), TRUE); // ヘッダー部分の強調
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// シンプルな白線ではなく、ゴールド系の枠線で高級感を出す
	DrawBox(300, 100, 1620, 800, GetColor(200, 170, 80), FALSE);
	DrawBox(298, 98, 1622, 802, GetColor(100, 80, 30), FALSE); // 外側の細い縁取り

	int fontSize = GetFontSize();
	DrawShopHeader(player);
	DrawShopOptions();
	DrawShopFooter();
	SetFontSize(fontSize);
}

/// @param player
/// @details プレイヤーの現在の所持金(Money)など、状態に依存する変動情報をUI上部に描画する
void StatShop::DrawShopHeader(Player3D* player)
{
	SetFontSize(40);
	DrawFormatString(350, 120, GetColor(255, 255, 255), "--- STATUS UPGRADE SHOP ---");
	SetFontSize(30);
	DrawFormatString(350, 180, GetColor(255, 255, 0), "Money: %d", player->m_HaveMoney->HaveMoney());
}

/// @details 現在のアップグレード回数に基づいて動的にコストを計算し、商品リストとカーソルを描画する
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
		m_UpgradeMaxHpCount,
		m_UpgradeAttackCount,
		m_UpgradeSpeedCount,
		m_UpgradeEvasionSpeedCount
	};
	int icons[] =
	{
		m_IconMaxHpHandle,
		m_IconAttackHandle,
		m_IconSpeedHandle,
		m_IconEvasionDistHandle,
		m_IconEvasionInvHandle
	};

	for (int i = 0; i <= m_SelectMax; i++)
	{
		bool is_selected = (i == m_Select);
		int expand = is_selected ? 4 : 0;
		int bgColor = is_selected ? GetColor(60, 50, 20) : GetColor(30, 30, 40);
		int color = is_selected ? GetColor(255, 255, 200) : GetColor(200, 200, 200);
		int optionY = 240 + i * 80;
		
		// 背景パネル
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, is_selected ? 240 : 150);
		DrawBox(330 - expand, optionY - expand, 1580 + expand, optionY + 60 + expand, bgColor, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
		if (is_selected) {
			DrawLine(330 - expand, optionY - expand, 1580 + expand, optionY - expand, GetColor(255, 215, 100), 2);
			DrawFormatString(350 - expand, optionY + 15, color, ">");
		}

		DrawExtendGraph(390, optionY + 10, 430, optionY + 50, icons[i], TRUE);
		
		SetFontSize(is_selected ? 30 : 28);
		if (i == 4)
		{
			DrawFormatString(450, optionY + 15, color, "%s - Cost: 100", options[i]);
		}
		else
		{
			DrawFormatString(450, optionY + 15, color, "%s (UP %d) - Cost: %d", options[i], upgradeCounts[i], GetCost(upgradeCounts[i]));
		}
		SetFontSize(24);
	}
}

/// @details プレイヤーの操作を誘導するためのキーガイドテキストを画面下部に静的描画する
void StatShop::DrawShopFooter()
{
	DrawFormatString(350, 700, GetColor(200, 200, 200), "Up/Down: Select   Enter: Buy   BackSpace: Close");
}

/// @param player (距離計算用)
/// @details プレイヤーとの距離を計算し、接近時のみNPCの頭上に吹き出しテキストを動的に表示する
void StatShop::DrawShopNpc(Player3D* player)
{
	VECTOR drawName3D = VAdd(m_Position, VGet(0.0f, 250.0f, 0.0f));
	VECTOR drawNameWorld = ConvWorldPosToScreenPos(drawName3D);

	VECTOR playerPos = player->GetPosition();
	float dx = playerPos.x - m_Position.x;
	float dz = playerPos.z - m_Position.z;
	float dist = sqrtf(dx * dx + dz * dz);

	// カメラ後方にある場合は描画しない
	if (drawNameWorld.z >= 0.0f && drawNameWorld.z <= 1.0f)
	{
		int drawX = static_cast<int>(drawNameWorld.x);
		int drawY = static_cast<int>(drawNameWorld.y);

		DrawFormatString(drawX - 30, drawY, GetColor(255, 255, 0), "[ ステータスショップ ]");
		DrawFormatString(drawX - 10, drawY + 20, GetColor(255, 255, 255), "Enterキーで開く");

		if (dist < 1000.0f)
		{
			DrawFormatString(drawX - 60, drawY - 30, GetColor(100, 255, 100), "「いらっしゃい！ 何か買いたいものはあるかい？」 ");
		}
	}

	m_Model->Draw();
}

/// @details ショップの営業時間監視、UI操作の受け付け、およびNPCの入場・退場アニメーション処理を統括する
void StatShop::Update()
{
	if (!CanUpdateShop())
	{
		return;
	}

	CloseShopIfPhaseEnding();

	static bool was_shop_open = false;
	if (Master::m_IsStatShopOn)
	{
		if (was_shop_open)
		{
			UpdateShopMenu();
		}
		was_shop_open = true;
	}
	else
	{
		was_shop_open = false;
	}

	movePosition();
}

/// @return 更新可能か(bool)
/// @details なし（待機状態や営業時間外など、不要な更新処理を弾くためのガード条件判定）
bool StatShop::CanUpdateShop() const
{
	if (m_ShopState == ShopState::WAIT_PHASE)
	{
		return false;
	}

	if (!IsShopPhaseActive() && m_ShopState != ShopState::WALKING_OUT)
	{
		return false;
	}

	return true;
}

/// @details 制限時間間際になった際、強制的にショップUIを閉じてプレイヤーの行動を通常ステートへ引き戻す
void StatShop::CloseShopIfPhaseEnding()
{
	SceneGame* sceneGame = Master::m_SceneManager->GetSceneGame();
	if (!sceneGame || !sceneGame->m_GameManager)
	{
		return;
	}

	bool isClosingSoon = sceneGame->m_GameManager->GetShopTimer() <= 60;
	bool isFinalShop = sceneGame->m_GameManager->GetCurrentPhase() == GameManager::Phase::kShop3;
	if (isClosingSoon && !isFinalShop && Master::m_IsStatShopOn)
	{
		Master::m_IsStatShopOn = false;
		Master::m_SoundManager->PlaySE(SoundManager::SE_WINDOW);
	}
}

/// @details メニュー内のカーソル移動、アイテム購入判定、およびUIキャンセル入力を一括処理する
void StatShop::UpdateShopMenu()
{
	SelectClass();
	BuyClass();
	HandleShopCloseInput();
}

/// @details BackSpaceキーによるショップ終了操作を検知し、全体のUI表示フラグを下ろす
void StatShop::HandleShopCloseInput()
{
	static int oldBack = 0;
	int currentBack = CheckHitKey(KEY_INPUT_BACK);

	if (currentBack && !oldBack)
	{
		Master::m_IsStatShopOn = false;
		Master::m_SoundManager->PlaySE(SoundManager::SE_WINDOW);
	}
	oldBack = currentBack;
}

/// @details 入場・退場ステートに応じた座標移動処理と、コライダーやモデルトランスフォームの同期を行う
void StatShop::movePosition()
{
	if (m_ShopState == ShopState::WALKING_IN)
	{
		UpdateWalkIn();
	}
	else if (m_ShopState == ShopState::WALKING_OUT)
	{
		UpdateWalkOut();
	}
	else if (m_ShopState == ShopState::ARRIVED)
	{
		m_Model->ChangeAnimation(ANIMATION_NEUTRAL);
	}

	UpdateShopColliderVisibility();
	SyncModelTransform();
}

/// @details 目標地点への接近計算を行い、到達時にショップの営業状態（ARRIVED）へ遷移させる
void StatShop::UpdateWalkIn()
{
	VECTOR dir = VSub(m_TargetPosition, m_Position);
	dir.y = 0.0f;
	float dist = VSize(dir);
	if (dist < 10.0f)
	{
		m_Position.x = m_TargetPosition.x;
		m_Position.z = m_TargetPosition.z;
		m_ShopState = ShopState::ARRIVED;
		m_Model->ChangeAnimation(ANIMATION_NEUTRAL);
		return;
	}

	VECTOR nDir = VNorm(dir);
	m_Position = VAdd(m_Position, VScale(nDir, 4.0f));
	m_Rotation.y = atan2f(-nDir.x, -nDir.z);
	m_Model->ChangeAnimation(ANIMATION_RUN);
}

/// @details フェーズ終了時に初期位置へ帰還する移動計算を行い、完了後に待機状態へ戻す
void StatShop::UpdateWalkOut()
{
	VECTOR startPos = VGet(mvStartPosition.x, mvStartPosition.y, mvStartPosition.z);
	VECTOR dir = VSub(startPos, m_Position);
	dir.y = 0.0f;
	float dist = VSize(dir);
	if (dist < 10.0f)
	{
		m_ShopState = ShopState::WAIT_PHASE;
		m_Model->ChangeAnimation(ANIMATION_NEUTRAL);
		return;
	}

	VECTOR nDir = VNorm(dir);
	m_Position = VAdd(m_Position, VScale(nDir, 4.0f));
	m_Rotation.y = atan2f(-nDir.x, -nDir.z);
	m_Model->ChangeAnimation(ANIMATION_RUN);
}

/// @details 営業時間外はコライダーを地下へ退避させ、他オブジェクトとの予期せぬ接触バグを回避する
void StatShop::UpdateShopColliderVisibility()
{
	if (m_ShopState == ShopState::ARRIVED)
	{
		m_ShopIn->m_Position = m_Position;
		m_SafeZoon->m_Position = m_Position;
	}
	else
	{
		VECTOR hidePos = VGet(m_Position.x, m_Position.y - 10000.0f, m_Position.z);
		m_ShopIn->m_Position = hidePos;
		m_SafeZoon->m_Position = hidePos;
	}
}

/// @details 内部の座標・回転データをDxLib側のモデルインスタンスへ確実に反映させる
void StatShop::SyncModelTransform()
{
	if (m_Model)
	{
		m_Model->SetPosition(m_Position);
		m_Model->SetRotation(m_Rotation);
		m_Model->Update();
	}
}

/// @details ショップフェーズ開始時にNPCを入場ステートへ切り替え、初期座標からの移動を開始させる
void StatShop::StartWalkingIn()
{
	if (m_ShopState == ShopState::WAIT_PHASE || m_ShopState == ShopState::WALKING_OUT)
	{
		m_ShopState = ShopState::WALKING_IN;
		m_Position = VGet(mvStartPosition.x, mvStartPosition.y, mvStartPosition.z);
	}
}

/// @details フェーズ終了時にUIを強制非表示にし、NPCを退場ステートへ切り替えて撤収を開始させる
void StatShop::StartWalkingOut()
{
	if (m_ShopState == ShopState::ARRIVED || m_ShopState == ShopState::WALKING_IN)
	{
		m_ShopState = ShopState::WALKING_OUT;
		Master::m_IsStatShopOn = false;
	}
}

/// @param upgradeCount(現在の強化回数)
/// @return 必要なコスト
/// @details なし（強化回数に応じた価格インフレの計算式）
int StatShop::GetCost(int upgradeCount)
{
	return 100 + (upgradeCount * 50);
}

/// @details キーボード入力によるカーソル位置(m_Select)の更新と、範囲外アクセスを防ぐループ処理
void StatShop::SelectClass()
{
	static int oldUp = 0;
	static int oldDown = 0;
	int currentUp = CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W);
	int currentDown = CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S);

	if (currentUp && !oldUp)
	{
		m_Select--;
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
	}
	if (currentDown && !oldDown)
	{
		m_Select++;
		Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
	}

	oldUp = currentUp;
	oldDown = currentDown;

	if (m_Select < m_SelectMin) m_Select = m_SelectMax;
	if (m_Select > m_SelectMax) m_Select = m_SelectMin;
}

/// @details マウスやキーボードによる購入確定を検知し、資金消費・ステータス反映・アイテム付与を行う
void StatShop::BuyClass()
{
	Player3D* player = Master::m_Player;

	if (!player) return;

	bool isMouseDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool isMouseClick = (isMouseDown && !m_OldMouseDown);
	m_OldMouseDown = isMouseDown;


	int mx, my;
	GetMousePoint(&mx, &my);

	static int oldMx = 0, oldMy = 0;
	if (mx != oldMx || my != oldMy)
	{
		for (int i = 0; i <= m_SelectMax; i++)
		{
			int optY = 250 + i * 60;
			if (mx >= 350 && mx <= 1500 && my >= optY && my <= optY + 50)
			{
				if (m_Select != i)
				{
					m_Select = i;
					Master::m_SoundManager->PlaySE(SoundManager::SE_SELECT);
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
		for (int i = 0; i <= m_SelectMax; i++)
		{
			int optY = 250 + i * 60;
			if (mx >= 350 && mx <= 1500 && my >= optY && my <= optY + 50)
			{
				m_Select = i;
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
		if (m_Select == 4)
		{
			int cost = 100;
			if (player->m_HaveMoney->HaveMoney() >= cost)
			{
				player->m_HaveMoney->PullMoney(cost);

				Item::ItemInformation* info = new Item::ItemInformation();
				info->ID = Item::ItemID::HEAL;
				info->Count = 1;
				info->m_IsLog = true;
				Master::m_ItemManager->AddItem(info);

				Master::m_SoundManager->PlaySE(SoundManager::SE_SHOP);
			}
			else
			{
				Master::m_SoundManager->PlaySE(SoundManager::SE_WINDOW); // as error sound
			}
		}
		else
		{
			int* targetUpgradeCount = nullptr;
			if (m_Select == 0) targetUpgradeCount = &m_UpgradeMaxHpCount;
			else if (m_Select == 1) targetUpgradeCount = &m_UpgradeAttackCount;
			else if (m_Select == 2) targetUpgradeCount = &m_UpgradeSpeedCount;
			else if (m_Select == 3) targetUpgradeCount = &m_UpgradeEvasionSpeedCount;

			if (targetUpgradeCount)
			{
				int cost = GetCost(*targetUpgradeCount);
				if (player->m_HaveMoney->HaveMoney() >= cost)
				{
					player->m_HaveMoney->PullMoney(cost);
					(*targetUpgradeCount)++;

					// ステータスボーナスを適用
					if (m_Select == 0) player->AddUpgradeMaxHp(10.0f); // HP +10
					else if (m_Select == 1) player->AddUpgradeAttack(1.0f); // Attack +1
					else if (m_Select == 2) player->AddUpgradeSpeed(0.5f); // Speed +0.5
					else if (m_Select == 3) player->AddUpgradeEvasionSpeed(2.0f); // Evasion Speed +2

					Master::m_SoundManager->PlaySE(SoundManager::SE_SHOP);
				}
				else
				{
					Master::m_SoundManager->PlaySE(SoundManager::SE_WINDOW); // as error sound
				}
			}
		}
	}
}

/// @param collider(自身の判定), check(相手の判定)
/// @details プレイヤーがアクセス範囲内にいる状態でEnterキーを押下した際、ショップUIを展開する
void StatShop::OnEnter(Collider* collider, Collider* check)
{
	if (!IsShopPhaseActive()) return;

	if (check->m_ParentObject && check->m_ParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = check->m_ParentObject->CastTo<Player3D>();
		if (pPlayer && collider == m_ShopIn && pPlayer->GetCollisionCollider() == check)
		{
			static int oldEnter = 0;
			int currentEnter = InputManager::CheckDownKey(KEY_INPUT_RETURN);
			if (currentEnter && !oldEnter && !Master::m_IsStatShopOn)
			{
				Master::m_IsStatShopOn = true;
				Master::m_SoundManager->PlaySE(SoundManager::SE_WINDOW);
			}
			oldEnter = currentEnter;
		}
	}
}

/// @param collider(自身の判定), check(相手の判定)
/// @details なし（仕様上、接触中の継続処理は不要なため空実装とする）
void StatShop::OnTrigger(Collider* collider, Collider* check)
{
}

/// @param collider(自身の判定), check(相手の判定)
/// @details なし（仕様上、離脱時の特殊処理は不要なため空実装とする）
void StatShop::OnExit(Collider* collider, Collider* check)
{
}
