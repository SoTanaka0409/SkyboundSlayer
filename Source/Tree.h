#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"
#include <string>

class SphereCollider;
class CapsuleCollider;

/// @brief フィールド上に配置される木（障害物・環境オブジェクト）クラス
class Tree : public Object3D
{
public:
	/// @brief Treeクラスのコンストラクタ
	/// @param filename 3Dモデルのファイルパス
	/// @param initPos 初期配置座標
	/// @param Size モデルの拡大スケール
	/// @param getSize 当たり判定（コライダー）のサイズ
	/// @param HitFlag 衝突判定を行うかどうかの初期フラグ
	Tree(std::string filename, VECTOR initPos, float Size, float getSize, bool HitFlag);

	/// @brief Treeクラスのデストラクタ
	~Tree();

	/// @brief 木モデルの描画処理を行う
	void Draw() override;

	/// @brief 木の毎フレームの更新処理（コライダー位置同期など）を行う
	void Update() override;

	
	float GetSize() { return size_; }


	bool GetHitFlag() { return is_hit_flag_; }

	/// @brief 他のコライダーと接触した瞬間のイベント処理
	/// @param collider 自身のコライダー
	/// @param check 接触した相手のコライダー
	virtual void OnEnter(Collider* collider, Collider* check) override;

	/// @brief 他のコライダーと接触し続けている間のイベント処理
	/// @param collider 自身のコライダー
	/// @param check 接触した相手のコライダー
	virtual void OnTrigger(Collider* collider, Collider* check) override;

	/// @brief 他のコライダーから離れた瞬間のイベント処理
	/// @param collider 自身のコライダー
	/// @param check 離れた相手のコライダー
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	CapsuleCollider* capsule_collider_; ///< 幹部分の物理衝突判定用カプセルコライダー
	Model* model_;                      ///< 3Dモデルデータへのポインタ
	float size_;                        ///< モデルおよび判定の拡大スケール
	int model_handle_;                  ///< 3Dモデルのグラフィックハンドル
	bool is_hit_flag_;                  ///< 衝突判定が有効かどうかの判定フラグ
};