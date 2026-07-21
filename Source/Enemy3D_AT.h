#pragma once
#include "DxLib.h"
#include "Object3D.h"

#include "DrawHp.h"
#include "Debug.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "Collision.h"

class Model;

/// @brief 遠距離・近接などの特定の攻撃アルゴリズムを持つ3D敵キャラクタークラス
class Enemy3D_AT : public Enemy
{
public:
	/// @brief Enemy3D_ATクラスのコンストラクタ
	/// @param filename 3Dモデルのファイルパス
	/// @param initPos 初期配置座標
	/// @param hp 初期体力
	/// @param speed 移動速度
	/// @param HitSize 被弾コライダーサイズ
	/// @param Serch1 索敵判定の半径
	/// @param Serch2 攻撃開始判定の半径
	/// @param Serch3 接近停止判定の半径
	/// @param Attack 攻撃力
	/// @param money 倒した際の所持金（獲得スコア）
	/// @param is_separate_anim_ アニメーション分離処理を行うかどうかのフラグ
	Enemy3D_AT(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, float Attack, int money, bool is_separate_anim_);

	/// @brief Enemy3D_ATクラスのデストラクタ
	~Enemy3D_AT();

	/// @brief 敵モデルおよび関連UI等の描画処理を行う
	void Draw() override;

	/// @brief 毎フレームの更新処理（AIステート進行・攻撃処理等）を行う
	void Update() override;
};