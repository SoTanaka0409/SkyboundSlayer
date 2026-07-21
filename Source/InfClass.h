#pragma once
#include "DxLib.h"
#include "Object2D.h"
#include "Item.h"
#include <string>
#include <vector>

/// @brief 画面上にログやインフォメーションメッセージ（アイテム獲得通知など）を一時表示・管理するUIクラス
class InfClass : public Object2D
{
public:
	/// @brief InfClassのコンストラクタ
	/// @param Log_timer メッセージの表示時間（フレーム数）
	/// @param Inf 表示するログ・インフォメーションテキスト文字列
	/// @param num ログの表示スタイル・種別識別番号
	InfClass(int Log_timer, std::string Inf, int num);

	/// @brief ログのタイマー進行および表示位置の更新処理を行う
	void Update();

	/// @brief 画面上の描画位置を設定する
	/// @param pos 設定する描画座標ベクトル
	void SetPosition(VECTOR pos) { position_ = pos; }

	/// @brief ログメッセージおよび背景UIの描画処理を行う
	void Draw();

	/// @brief 表示期限切れによる消滅・削除対象フラグを取得する
	/// @return bool 削除対象であればtrue
	bool GetElaseFlag() { return elase_flag_; }

private:
	int LogTime;          ///< ログの表示継続時間（制限フレーム数）
	std::string LogDate;  ///< 表示・保持するログテキストデータ
	int LogCount;         ///< ログの表示位置移動・アニメーション用タイマーカウント
	bool elase_flag_;     ///< 表示終了後にリストから削除（erase）するためのフラグ

	int LogNumber;        ///< 使用するログのデザイン・背景UIの種類を選択する番号
};