#pragma once
#include "dxlib.h"
#include "Object3D.h"
#include "Texture.h"

class SphereCollider;
class CapsuleCollider;

// レベルデザイン：ステージ上に配置され、プレイヤーが到達した際にボス戦へのフェーズ移行やイベントをトリガーするための透明な空間判定オブジェクト
class StageCollider : public Object3D
{
public:
/// @details ボスエリア移行用の判定コライダーおよび演出用テクスチャリソースの動的生成
	StageCollider();

/// @details 確保したコライダーとテクスチャリソースの安全な解放
	~StageCollider();

/// @details プレイヤーが判定内にいる際の「警告」や「画面フェード」など、移行演出レイヤーの2Dオーバーレイ描画
	void Draw() override;

/// @details トランジション演出用のタイマー（Count
	void Update() override;

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
/// @details プレイヤーの侵入検知時の初期処理（フェード演出タイマーの起動など）
	virtual void OnEnter(Collider* collider, Collider* check) override;

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
/// @details プレイヤーが判定内に留まっている間、ゲームマネージャーへボスフェーズへの移行シグナルを送信する
	virtual void OnTrigger(Collider* collider, Collider* check) override;

/// @param collider = 自身の判定領域, check = 衝突相手のコライダー
/// @details プレイヤーが判定から離脱した際の演出キャンセルや状態変数のリセット
	virtual void OnExit(Collider* collider, Collider* check) override;

/// @details 地形の移動等に合わせて、トリガーコライダーの空間座標を最新の位置へ追従更新する
	void ColliderUpdate();

private:
	// レベルデザイン：この空間内にプレイヤーが侵入したことを検知し、強制的にエリア移動・ボス戦へ引きずり込むためのトリガーボリューム（見えない球体）
	SphereCollider* go_boss_collider_;

	// 演出仕様：ボス戦突入時の警告UI（WARNING等）や、シーン切り替えをシームレスに見せるためのフェード処理用画像
	Texture* texture_;
	Texture* texture2_;

	// UX仕様：トリガーに触れてから実際に画面が切り替わるまでの「タメ（猶予時間）」を作り、唐突な場面遷移によるプレイヤーの混乱を防ぐためのタイマー群
	int Count;
	int Time;
};