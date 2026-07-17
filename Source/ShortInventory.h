#pragma once
#include "DxLib.h"
#include "ItemManager.h"
#include "Item.h"

class ShortInventory
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	ShortInventory();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~ShortInventory();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int GetItemCount() const;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void ClampSelectedIndex(int itemCount);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void HandleSelectionInput(int itemCount);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void UseSelectedItem();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Item::Information* GetSelectedItem() const;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void DrawItemPanel(const Item::Information* info);

	int selected_index_;
};
