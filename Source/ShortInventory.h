#pragma once
#include "DxLib.h"
#include "ItemManager.h"
#include "Item.h"

// メニューを開かずに戦闘中などリアルタイムでアイテムを選択・使用するための、ショートカットUI（クイックインベントリ）を管理する
class ShortInventory
{
public:
/// @details 選択カーソルの初期位置(selected_index_)など、UIの内部状態を安全な値にセットアップする
    ShortInventory();

/// @details なし（ポインタ等の動的リソースを持たないため、特殊な破棄処理は行わずデフォルトに任せる）
    ~ShortInventory();

/// @details プレイヤーの入力に基づくカーソル移動や、使用ボタン押下によるアイテム消費ロジックを毎フレーム進行させる
    void Update();

/// @details 画面の隅に所持アイテムのアイコンや残量、現在選択されている枠（カーソル）などの2D UIを描画バッファへ登録する
    void Draw();

private:
/// @return インベントリ内の有効なアイテム種類数(int)
    // UIの描画枠数の決定や、カーソル移動時の限界値制限（ループ処理など）を計算するために用いる
    int GetItemCount() const;

/// @param itemCount(現在のアイテム総数)
/// @details 最後のアイテムを消費してリストが縮小した際、カーソルが配列の範囲外を指してクラッシュするのを防ぐため値を丸める
    void ClampSelectedIndex(int itemCount);

/// @param itemCount(現在のアイテム総数)
/// @details 十字キーやマウスホイールなどの入力を検知し、選択インデックス(selected_index_)を増減させてカーソルを移動する
    void HandleSelectionInput(int itemCount);

/// @details 現在選択されているアイテムの効果（HP回復など）をプレイヤーに適用し、インベントリ内の該当アイテム所持数を1減らす
    void UseSelectedItem();

/// @return 選択中アイテムのメタデータへのポインタ
    // アイテム使用時の効果判定ロジックへの引き渡しや、UI上に名前・説明文を表示するためにデータを取得する
    Item::ItemInformation* GetSelectedItem() const;

/// @param info(描画対象のアイテムデータ)
/// @details 指定された単一アイテムのアイコン、枠線、残量テキストをスクリーン上の適切なパネル座標へ描画する
    void DrawItemPanel(const Item::ItemInformation* info);

    int selected_index_; // 現在UI上でハイライト（選択）されており、使用キー押下時に消費される対象のインデックス
};