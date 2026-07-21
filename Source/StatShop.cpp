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
		SceneGame* sceneGame = Master::scene_manager_->GetSceneGame();
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
	, shop_state_(ShopState::WAIT_PHASE)
	, select_(0)
	, select_max_(4)
	, select_min_(0)
	, upgrade_max_hp_count_(0)
	, upgrade_attack_count_(0)
	, upgrade_speed_count_(0)
	, upgrade_evasion_speed_count_(0)
{
	SetTag(Tag3D_Shop);
	model_ = new Model(filename, position_, true);
	model_->AddAnimation(ANIMATION_NEUTRAL, "Resource/3Dモデル/キャラクターとアニメーション/11_待機アニメーション.mv1");
	model_->AddAnimation(ANIMATION_RUN, "Resource/3Dモデル/キャラクターとアニメーション/12_走りアニメーション.mv1");
	model_->ChangeAnimation(ANIMATION_NEUTRAL);
	shop_in_ = new SphereCollider(this, position_, 200.0f);
	safe_zoon_ = new SphereCollider(this, position_, 1000.0f); // 敵の侵入を防ぎ、プレイヤーの安全を確保するための広域コライダー
	old_mouse_down_ = false;

	icon_max_hp_handle_ = LoadGraph("Resource/画像/ショップ/01_最大HP強化アイコン.png");
	icon_attack_handle_ = LoadGraph("Resource/画像/ショップ/02_攻撃力強化アイコン.png");
	icon_speed_handle_ = LoadGraph("Resource/画像/ショップ/03_移動速度強化アイコン.png");
	icon_evasion_dist_handle_ = LoadGraph("Resource/画像/ショップ/04_回避距離強化アイコン.png");
	icon_evasion_inv_handle_ = LoadGraph("Resource/画像/ショップ/05_回避無敵時間強化アイコン.png");

	auto pPlayer = Master::player_;
	auto player = dynamic_cast<Player3D*>(pPlayer);
	target_position_ = player->GetFirstPos();
}

/// @details UI画像やモデル、コライダーを確実に破棄し、シーン離脱時のメモリリークを防ぐ
StatShop::~StatShop()
{
	DeleteGraph(icon_max_hp_handle_);
	DeleteGraph(icon_attack_handle_);
	DeleteGraph(icon_speed_handle_);
	DeleteGraph(icon_evasion_dist_handle_);
	DeleteGraph(icon_evasion_inv_handle_);

	delete model_;
	if (shop_in_) shop_in_->SetDeleteFlag(true);
	if (safe_zoon_) safe_zoon_->SetDeleteFlag(true);
}

/// @details フェーズやプレイヤーのUI操作状態に応じて、NPCモデルまたはショップ画面UIの描画を排他制御する
void StatShop::Draw()
{
	if (shop_state_ == ShopState::WAIT_PHASE)
	{
		return;
	}
	if (!IsShopPhaseActive())
	{
		return;
	}

	Player3D* player = Master::player_;
	if (!player)
	{
		return;
	}

	if (Master::is_stat_shop_on_)
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

/// @param player
/// @details プレイヤーの現在の所持金(Money)など、状態に依存する変動情報をUI上部に描画する
void StatShop::DrawShopHeader(Player3D* player)
{
	SetFontSize(40);
	DrawFormatString(350, 120, GetColor(255, 255, 255), "--- STATUS UPGRADE SHOP ---");
	SetFontSize(30);
	DrawFormatString(350, 180, GetColor(255, 255, 0), "Money: %d", player->have_money_->HaveMoney());
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
		upgrade_max_hp_count_,
		upgrade_attack_count_,
		upgrade_speed_count_,
		upgrade_evasion_speed_count_
	};
	int icons[] =
	{
		icon_max_hp_handle_,
		icon_attack_handle_,
		icon_speed_handle_,
		icon_evasion_dist_handle_,
		icon_evasion_inv_handle_
	};

	for (int i = 0; i <= select_max_; i++)
	{
		int color = (i == select_) ? GetColor(255, 0, 0) : GetColor(255, 255, 255);
		int optionY = 250 + i * 60;
		if (i == select_)
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

/// @details プレイヤーの操作を誘導するためのキーガイドテキストを画面下部に静的描画する
void StatShop::DrawShopFooter()
{
	DrawFormatString(350, 700, GetColor(200, 200, 200), "Up/Down: Select   Enter: Buy   Escape/Back: Close");
}

/// @param player (距離計算用)
/// @details プレイヤーとの距離を計算し、接近時のみNPCの頭上に吹き出しテキストを動的に表示する
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

/// @details ショップの営業時間監視、UI操作の受け付け、およびNPCの入場・退場アニメーション処理を統括する
void StatShop::Update()
{
	if (!CanUpdateShop())
	{
		return;
	}

	CloseShopIfPhaseEnding();

	if (Master::is_stat_shop_on_)
	{
		UpdateShopMenu();
	}

	movePosition();
}

/// @return 更新可能か(bool)
/// @details なし（待機状態や営業時間外など、不要な更新処理を弾くためのガード条件判定）
bool StatShop::CanUpdateShop() const
{
	if (shop_state_ == ShopState::WAIT_PHASE)
	{
		return false;
	}

	if (!IsShopPhaseActive() && shop_state_ != ShopState::WALKING_OUT)
	{
		return false;
	}

	return true;
}

/// @details 制限時間間際になった際、強制的にショップUIを閉じてプレイヤーの行動を通常ステートへ引き戻す
void StatShop::CloseShopIfPhaseEnding()
{
	SceneGame* sceneGame = Master::scene_manager_->GetSceneGame();
	if (!sceneGame || !sceneGame->game_manager_)
	{
		return;
	}

	bool isClosingSoon = sceneGame->game_manager_->GetShopTimer() <= 60;
	bool isFinalShop = sceneGame->game_manager_->GetCurrentPhase() == GameManager::Phase::kShop3;
	if (isClosingSoon && !isFinalShop && Master::is_stat_shop_on_)
	{
		Master::is_stat_shop_on_ = false;
		Master::sound_manager_->PlaySE(SoundManager::SE_WINDOW);
	}
}

/// @details メニュー内のカーソル移動、アイテム購入判定、およびUIキャンセル入力を一括処理する
void StatShop::UpdateShopMenu()
{
	SelectClass();
	BuyClass();
	HandleShopCloseInput();
}

/// @details EscまたはBackキーによるショップ終了操作を検知し、全体のUI表示フラグを下ろす
void StatShop::HandleShopCloseInput()
{
	static int oldEsc = 0;
	static int oldBack = 0;
	int currentEsc = CheckHitKey(KEY_INPUT_ESCAPE);
	int currentBack = CheckHitKey(KEY_INPUT_BACK);

	if ((currentEsc && !oldEsc) || (currentBack && !oldBack))
	{
		Master::is_stat_shop_on_ = false;
		Master::sound_manager_->PlaySE(SoundManager::SE_WINDOW);
	}
	oldEsc = currentEsc;
	oldBack = currentBack;
}

/// @details 入場・退場ステートに応じた座標移動処理と、コライダーやモデルトランスフォームの同期を行う
void StatShop::movePosition()
{
	if (shop_state_ == ShopState::WALKING_IN)
	{
		UpdateWalkIn();
	}
	else if (shop_state_ == ShopState::WALKING_OUT)
	{
		UpdateWalkOut();
	}
	else if (shop_state_ == ShopState::ARRIVED)
	{
		model_->ChangeAnimation(ANIMATION_NEUTRAL);
	}

	UpdateShopColliderVisibility();
	SyncModelTransform();
}

/// @details 目標地点への接近計算を行い、到達時にショップの営業状態（ARRIVED）へ遷移させる
void StatShop::UpdateWalkIn()
{
	VECTOR dir = VSub(target_position_, position_);
	dir.y = 0.0f;
	float dist = VSize(dir);
	if (dist < 10.0f)
	{
		position_.x = target_position_.x;
		position_.z = target_position_.z;
		shop_state_ = ShopState::ARRIVED;
		model_->ChangeAnimation(ANIMATION_NEUTRAL);
		return;
	}

	VECTOR nDir = VNorm(dir);
	position_ = VAdd(position_, VScale(nDir, 4.0f));
	rotation_.y = atan2f(-nDir.x, -nDir.z);
	model_->ChangeAnimation(ANIMATION_RUN);
}

/// @details フェーズ終了時に初期位置へ帰還する移動計算を行い、完了後に待機状態へ戻す
void StatShop::UpdateWalkOut()
{
	VECTOR startPos = VGet(mvStartPosition.x, mvStartPosition.y, mvStartPosition.z);
	VECTOR dir = VSub(startPos, position_);
	dir.y = 0.0f;
	float dist = VSize(dir);
	if (dist < 10.0f)
	{
		shop_state_ = ShopState::WAIT_PHASE;
		model_->ChangeAnimation(ANIMATION_NEUTRAL);
		return;
	}

	VECTOR nDir = VNorm(dir);
	position_ = VAdd(position_, VScale(nDir, 4.0f));
	rotation_.y = atan2f(-nDir.x, -nDir.z);
	model_->ChangeAnimation(ANIMATION_RUN);
}

/// @details 営業時間外はコライダーを地下へ退避させ、他オブジェクトとの予期せぬ接触バグを回避する
void StatShop::UpdateShopColliderVisibility()
{
	if (shop_state_ == ShopState::ARRIVED)
	{
		shop_in_->position_ = position_;
		safe_zoon_->position_ = position_;
	}
	else
	{
		VECTOR hidePos = VGet(position_.x, position_.y - 10000.0f, position_.z);
		shop_in_->position_ = hidePos;
		safe_zoon_->position_ = hidePos;
	}
}

/// @details 内部の座標・回転データをDxLib側のモデルインスタンスへ確実に反映させる
void StatShop::SyncModelTransform()
{
	if (model_)
	{
		model_->SetPosition(position_);
		model_->SetRotation(rotation_);
		model_->Update();
	}
}

/// @details ショップフェーズ開始時にNPCを入場ステートへ切り替え、初期座標からの移動を開始させる
void StatShop::StartWalkingIn()
{
	if (shop_state_ == ShopState::WAIT_PHASE || shop_state_ == ShopState::WALKING_OUT)
	{
		shop_state_ = ShopState::WALKING_IN;
		position_ = VGet(mvStartPosition.x, mvStartPosition.y, mvStartPosition.z);
	}
}

/// @details フェーズ終了時にUIを強制非表示にし、NPCを退場ステートへ切り替えて撤収を開始させる
void StatShop::StartWalkingOut()
{
	if (shop_state_ == ShopState::ARRIVED || shop_state_ == ShopState::WALKING_IN)
	{
		shop_state_ = ShopState::WALKING_OUT;
		Master::is_stat_shop_on_ = false;
	}
}

/// @param upgradeCount(現在の強化回数)
/// @return 必要なコスト
/// @details なし（強化回数に応じた価格インフレの計算式）
int StatShop::GetCost(int upgradeCount)
{
	return 100 + (upgradeCount * 50);
}

/// @details キーボード入力によるカーソル位置(select_)の更新と、範囲外アクセスを防ぐループ処理
void StatShop::SelectClass()
{
	static int oldUp = 0;
	static int oldDown = 0;
	int currentUp = CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W);
	int currentDown = CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S);

	if (currentUp && !oldUp)
	{
		select_--;
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
	}
	if (currentDown && !oldDown)
	{
		select_++;
		Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
	}

	oldUp = currentUp;
	oldDown = currentDown;

	if (select_ < select_min_) select_ = select_max_;
	if (select_ > select_max_) select_ = select_min_;
}

/// @details マウスやキーボードによる購入確定を検知し、資金消費・ステータス反映・アイテム付与を行う
void StatShop::BuyClass()
{
	Player3D* player = Master::player_;

	if (!player) return;

	bool isMouseDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	bool isMouseClick = (isMouseDown && !old_mouse_down_);
	old_mouse_down_ = isMouseDown;


	int mx, my;
	GetMousePoint(&mx, &my);

	static int oldMx = 0, oldMy = 0;
	if (mx != oldMx || my != oldMy)
	{
		for (int i = 0; i <= select_max_; i++)
		{
			int optY = 250 + i * 60;
			if (mx >= 350 && mx <= 1500 && my >= optY && my <= optY + 50)
			{
				if (select_ != i)
				{
					select_ = i;
					Master::sound_manager_->PlaySE(SoundManager::SE_SELECT);
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
		for (int i = 0; i <= select_max_; i++)
		{
			int optY = 250 + i * 60;
			if (mx >= 350 && mx <= 1500 && my >= optY && my <= optY + 50)
			{
				select_ = i;
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
		if (select_ == 4)
		{
			int cost = 100;
			if (player->have_money_->HaveMoney() >= cost)
			{
				player->have_money_->PullMoney(cost);

				Item::ItemInformation* info = new Item::ItemInformation();
				info->ID = Item::ItemID::HEAL;
				info->Count = 1;
				info->is_log_ = true;
				Master::item_manager_->AddItem(info);

				Master::sound_manager_->PlaySE(SoundManager::SE_SHOP);
			}
			else
			{
				Master::sound_manager_->PlaySE(SoundManager::SE_WINDOW); // as error sound
			}
		}
		else
		{
			int* targetUpgradeCount = nullptr;
			if (select_ == 0) targetUpgradeCount = &upgrade_max_hp_count_;
			else if (select_ == 1) targetUpgradeCount = &upgrade_attack_count_;
			else if (select_ == 2) targetUpgradeCount = &upgrade_speed_count_;
			else if (select_ == 3) targetUpgradeCount = &upgrade_evasion_speed_count_;

			if (targetUpgradeCount)
			{
				int cost = GetCost(*targetUpgradeCount);
				if (player->have_money_->HaveMoney() >= cost)
				{
					player->have_money_->PullMoney(cost);
					(*targetUpgradeCount)++;

					// ステータスボーナスを適用
					if (select_ == 0) player->AddUpgradeMaxHp(10.0f); // HP +10
					else if (select_ == 1) player->AddUpgradeAttack(1.0f); // Attack +1
					else if (select_ == 2) player->AddUpgradeSpeed(0.5f); // Speed +0.5
					else if (select_ == 3) player->AddUpgradeEvasionSpeed(2.0f); // Evasion Speed +2

					Master::sound_manager_->PlaySE(SoundManager::SE_SHOP);
				}
				else
				{
					Master::sound_manager_->PlaySE(SoundManager::SE_WINDOW); // as error sound
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

	if (check->parent_object_ && check->parent_object_->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = check->parent_object_->CastTo<Player3D>();
		if (pPlayer && collider == shop_in_ && pPlayer->GetCollisionCollider() == check)
		{
			static int oldEnter = 0;
			int currentEnter = CheckHitKey(KEY_INPUT_RETURN);
			if (currentEnter && !oldEnter && !Master::is_stat_shop_on_)
			{
				Master::is_stat_shop_on_ = true;
				Master::sound_manager_->PlaySE(SoundManager::SE_WINDOW);
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