#pragma once
#include "Collider.h"

/// @brief 球体（Sphere）形状の衝突判定コライダーを表すクラス
class SphereCollider : public Collider
{
public:
	/// @brief SphereColliderのコンストラクタ
	/// @param parent コライダーが追従・所有する親Object3Dポインタ
	/// @param center 球体コライダーの中心ワールド座標
	/// @param radius 球体コライダーの判定半径
	SphereCollider(Object3D* parent, VECTOR center, float radius);

	/// @brief SphereColliderの仮想デストラクタ
	virtual ~SphereCollider();

	/// @brief 相手コライダーとの衝突判定の計算および更新処理を行う
	/// @param check 衝突判定チェック対象の相手コライダーポインタ
	void Update(Collider* check) override;

	/// @brief デバッグ描画モード時に球体コライダーをワイヤーフレーム描画する
	void Draw() override;

	/// @brief 相手コライダーと接触（衝突）した瞬間の割り込みイベント処理
	void OnEnter() override;

	/// @brief 相手コライダーと継続して接触中の割り込みイベント処理
	void OnTrigger() override;

	/// @brief 相手コライダーと接触状態から離れた瞬間の割り込みイベント処理
	void OnExit() override;
};