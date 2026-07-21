#pragma once
#include <DxLib.h>
#include <vector>

/// @brief Uキー長押し操作に対応した魔法陣エフェクト（Effekseer）の生成・追従・制御を行うクラス
class EffectU
{
public:
	EffectU() = default;
	~EffectU();

	/// @brief 魔法陣エフェクト（.efk）リソースの事前読み込みを行う
	/// @return bool 読み込みに成功した場合はtrue
	bool Load();

	/// @brief Uキーを押した瞬間に魔法陣エフェクトをプレイヤー足元へ生成・再生開始する
	/// @param playerPos 発生起点となるプレイヤーのワールド座標
	void StartHold(const VECTOR& playerPos);

	/// @brief Uキーを離した瞬間に魔法陣エフェクトの再生を停止（または消滅演出へ移行）する
	void ReleaseAndShatter();

	/// @brief 毎フレームの更新処理を行い、再生中の魔法陣をプレイヤーの現在位置へ追従させる
	/// @param playerPos 追従先となるプレイヤーの最新ワールド座標
	void UpdateFollow(const VECTOR& playerPos);

	/// @brief エフェクトの描画処理を行う（EffekseerManagerが一括描画するため空実装）
	void Draw() const;

private:
	int playing_handle_ = -1;  ///< 現在再生中のEffekseerエフェクトハンドル
	bool is_holding_ = false;   ///< 現在Uキーのホールド（魔法陣展開）状態が継続しているかを示すフラグ
	float y_offset_ = 2.0f;     ///< プレイヤー足元のめり込みを防ぐためのY軸高さオフセット量
};