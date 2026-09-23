#include "ShortInventory.h"
#include "Config.h"
#include "InputManager.h"
#include "Master.h"
#include <iterator>

/// @brief ShortInventoryクラスのコンストラクタ
ShortInventory::ShortInventory()
	: selected_index_(0)
{
}

/// @brief ShortInventoryクラスのデストラクタ
ShortInventory::~ShortInventory()
{
}

/// @brief ショートカットインベントリの選択切り替え・使用入力判定などの毎フレーム更新処理を行う
void ShortInventory::Update()
{
	int itemCount = GetItemCount();
	if (itemCount == 0)
	{
		return;
	}

	ClampSelectedIndex(itemCount);
	HandleSelectionInput(itemCount);

	if (InputManager::CheckDownKey(KEY_INPUT_R))
	{
		UseSelectedItem();
	}
}

/// @brief インベントリ内に保持されているアイテムの種類数を取得する
/// @return int 所持アイテムの種類数
int ShortInventory::GetItemCount() const
{
	if (!Master::item_manager_)
	{
		return 0;
	}

	return static_cast<int>(Master::item_manager_->item_list_.size());
}

/// @brief 選択インデックスがアイテムリストの範囲外にならないようクランプ（ループ補正）処理を行う
/// @param itemCount 現在の所持アイテム総数
void ShortInventory::ClampSelectedIndex(int itemCount)
{
	if (itemCount <= 0)
	{
		selected_index_ = 0;
		return;
	}

	if (selected_index_ >= itemCount)
	{
		selected_index_ = 0;
	}
	else if (selected_index_ < 0)
	{
		selected_index_ = itemCount - 1;
	}
}

/// @brief 左右キー入力による選択アイテムのインデックス切り替え処理を行う
/// @param itemCount 現在の所持アイテム総数
void ShortInventory::HandleSelectionInput(int itemCount)
{
	if (InputManager::CheckDownKey(KEY_INPUT_RIGHT))
	{
		selected_index_++;
		ClampSelectedIndex(itemCount);
	}

	if (InputManager::CheckDownKey(KEY_INPUT_LEFT))
	{
		selected_index_--;
		ClampSelectedIndex(itemCount);
	}
}

/// @brief 現在選択されているアイテムを消費・使用する
void ShortInventory::UseSelectedItem()
{
	Item::ItemInformation* info = GetSelectedItem();
	if (info && info->Count > 0)
	{
		Master::item_manager_->UseItem(info->ID);
	}
}

/// @brief 現在選択されているアイテムのデータ構造体へのポインタを取得する
/// @return Item::ItemInformation* 選択中のアイテム情報ポインタ（選択不可時はnullptr）
Item::ItemInformation* ShortInventory::GetSelectedItem() const
{
	if (!Master::item_manager_)
	{
		return nullptr;
	}

	if (selected_index_ < 0 || selected_index_ >= GetItemCount())
	{
		return nullptr;
	}

	auto it = Master::item_manager_->item_list_.begin();
	std::advance(it, selected_index_);
	if (it == Master::item_manager_->item_list_.end())
	{
		return nullptr;
	}

	return *it;
}

/// @brief ショートカットUIの描画処理を行う
void ShortInventory::Draw()
{
	int itemCount = GetItemCount();
	if (itemCount == 0)
	{
		return;
	}

	ClampSelectedIndex(itemCount);
	Item::ItemInformation* info = GetSelectedItem();
	if (!info)
	{
		return;
	}

	DrawItemPanel(info);
}

/// @brief 画面右下にアイテム枠・選択名・所持数・操作ガイドUIを描画する
/// @param info 描画対象となる選択中のアイテム情報構造体ポインタ
void ShortInventory::DrawItemPanel(const Item::ItemInformation* info)
{
	const int boxW = 360;
	const int boxH = 74;
	const int boxX = Config::ScreenWidth - boxW - 28;
	const int boxY = Config::ScreenHeight - boxH - 28;

	const int panel = GetColor(18, 17, 20);
	const int panelLight = GetColor(38, 35, 38);
	const int gold = GetColor(198, 154, 64);
	const int goldDark = GetColor(98, 73, 32);

	// 背景シャドウ半透明描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(boxX - 6, boxY - 4, boxX + boxW + 6, boxY + boxH + 6, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// UI枠パネル描画
	DrawBox(boxX, boxY, boxX + boxW, boxY + boxH, panel, TRUE);
	DrawBox(boxX + 6, boxY + 6, boxX + boxW - 6, boxY + 12, panelLight, TRUE);
	DrawLine(boxX, boxY, boxX + boxW, boxY, gold, 1);
	DrawLine(boxX, boxY + boxH, boxX + boxW, boxY + boxH, goldDark, 1);
	DrawLine(boxX, boxY, boxX, boxY + boxH, goldDark, 1);
	DrawLine(boxX + boxW, boxY, boxX + boxW, boxY + boxH, gold, 1);

	// アイテム情報テキスト描画
	DrawFormatString(boxX + 18, boxY + 12, GetColor(245, 226, 174), "ITEM");
	DrawFormatString(boxX + 100, boxY + 12, GetColor(238, 238, 238), "%s", info->Name.c_str());
	DrawFormatString(boxX + 100, boxY + 42, GetColor(205, 210, 216), "x %d", info->Count);
	DrawFormatString(boxX + 18, boxY + 42, GetColor(255, 210, 70), "R"); DrawFormatString(boxX + 50, boxY + 42, GetColor(220, 220, 220), "USE");
	DrawFormatString(boxX + 300, boxY + 42, GetColor(200, 200, 0), "< >");
}