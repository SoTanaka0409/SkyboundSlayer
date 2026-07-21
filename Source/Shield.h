#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include <string>

/// @brief プレイヤーやオブジェクトを保護・遮蔽するシールド（盾）オブジェクトクラス
class Shield : public Object3D
{
public:
	/// @brief Shieldクラスのコンストラクタ
	/// @param filename シールド用3Dモデルのファイルパス
	/// @param initPos 初期配置座標
	/// @param hp シールドの初期耐久値（HP）
	Shield(std::string filename, VECTOR initPos, int hp);

	/// @brief Shieldクラスのデストラクタ
	~Shield();

	/// @brief シールドの移動・モデルアニメーション等の毎フレーム更新処理を行う
	void Update();

	/// @brief シールド3Dモデルの描画処理を行う
	void Draw();

	/// @brief 現在のシールドの耐久値（HP）を取得する
	/// @return int 現在のHP
	int GetHp() { return hp_; }

	/// @brief シールドの耐久値（HP）を設定する
	/// @param hp 設定するHP値
	void SetHp(int hp) { hp_ = hp; }

	/// @brief シールドの判定サイズ（半径等）を取得する
	/// @return float シールドのサイズ値
	float GetSize() { return size_s_; }

	/// @brief 新しく生成された直後のシールドかどうかの判定フラグを取得する
	/// @return bool 新規生成シールドであればtrue
	bool GetNew() { return NewShield; }

	/// @brief 新規生成フラグを設定する
	/// @param New 設定するフラグ値
	void SetNew(bool New) { NewShield = New; }

private:
	Model* model_;   ///< シールドの3Dモデル管理ポインタ
	int hp_;         ///< シールドの耐久値（HP）
	float size_s_;   ///< シールドの衝突判定用サイズ
	bool NewShield;  ///< 新しく生成された状態かを示すフラグ
};