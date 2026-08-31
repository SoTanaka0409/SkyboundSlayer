#pragma once
#include "DxLib.h"
#include "Object2D.h"
#include "Object3D.h"

/// @brief 個別のバフ（ステータス強化・弱体化）効果パラメータおよびタイマー管理を行うクラス
class Buff
{
public:
	/// @brief バフ効果の詳細設定パラメータを保持する構造体
	struct effectDate
	{
		Object3D::StatusState type; ///< 対象となるステータスの種類（攻撃力、移動速度、HPなど）
		int Timer;                   ///< バフ効果が持続する経過フレームタイマー
		int Count;                   ///< バフ効果の最大継続時間（フレーム数）
		float Effect;                ///< ステータスに加算・補正される効果量
		bool use_ = false;           ///< バフ効果が現在有効・適用中かどうかの判定フラグ
	} efDate;                        ///< バフ効果設定構造体のインスタンス

public:
	/// @brief Buffクラスのコンストラクタ
	/// @param timer 持続継続フレーム時間
	/// @param effect ステータス補正効果量
	/// @param type 対象となるステータス種別（StatusState）
	Buff(int timer, float effect, Object3D::StatusState type);

	/// @brief Buffクラスのデストラクタ
	~Buff();

	/// @brief バフの経過タイマー進行や適用状態の更新処理を行う
	void Update();

private:
	float OldNum;   ///< バフ適用前の元のステータス値（効果終了時の戻し用）
	bool Do_buff;   ///< バフの適用処理が完了しているかを示す実行フラグ
};