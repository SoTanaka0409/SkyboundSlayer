#pragma once
#include"DxLib.h"
#include<string>
#include"Object3D.h"
#include"Model.h"

#include"camera.h"
#include"Debug.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ItemManager.h"
#include"ShortInventory.h"
#include"InfClassManager.h"
#include"BuffManager.h"
#include"HaveMoneyClass.h"
#include"EquipmentManager.h"

class SphereCollider;
class CapsuleCollider;

// プレイヤーキャラクターの入力制御、物理挙動（移動・ジャンプ）、攻撃ステート、およびステータス管理を統括するクラス
class Player3D : public Object3D
{
public:
	enum AttackStateInfo {
		kAttackNormal,
		kAttackJump,
		kAttackSlide,
	} attack_state_;

public:
	// 入力: filename, 初期座標, ジャンプ力, 移動速度, 初期HP, アニメ分離フラグ / 出力: なし
	// 副作用: プレイヤーの3Dモデル、各種アクション用コライダー、および関連マネージャー（装備・バフ等）の初期化を行う
	Player3D(std::string filename, VECTOR initPos, float junppower, float speed, float hp, bool is_separate_anim_ = false);

	// 入力: なし / 出力: なし
	// 副作用: 動的確保した各マネージャーインスタンスや攻撃用コライダー群を破棄し、メモリリークを防ぐ
	~Player3D();

	// 入力: なし / 出力: なし
	// 副作用: アニメーション、物理挙動、入力検知によるステート遷移（攻撃や回避など）を毎フレーム進行させる
	void Update() override;

	// 入力: なし / 出力: なし
	// 副作用: 3Dモデル本体の姿勢を反映して描画し、加えてHPバーなどのプレイヤー専用空間UIをバッファへ登録する
	void Draw() override;

	// 入力: なし / 出力: なし
	// 副作用: インベントリや装備効果、バフ効果の持続時間計算など、プレイヤーに紐付くサブシステムの更新を一括処理する
	void ManagerUpdate();

	// 入力: なし / 出力: なし
	// 副作用: Y座標が一定値を下回った（奈落へ落下した）場合にダメージ処理を行い、安全な初期座標へ強制リスポーンさせる
	void CheckStageOut();

	// 入力: なし / 出力: なし
	// 副作用: カメラの向いているベクトルとコントローラーの入力情報から実際の移動ベクトルを算出し、座標を更新する
	void MoveEx();

	// 入力: なし / 出力: なし
	// 副作用: プレイヤーの入力状況と各スキルのクールダウン状態を判定し、適切な攻撃ステート（通常・スライド・ジャンプ）へ遷移させる
	void SelectAttack();

	void Attack();
	void AttackSlide();
	void AttackJump();

	// 入力: damage(被ダメージ量) / 出力: なし
	// 副作用: 無敵時間(is_invisible_)をチェックした上でHPを減算し、被弾アニメーションやヒットストップのトリガーを発火させる
	void Damage(float damage);

	// 入力: なし / 出力: なし
	// 副作用: 攻撃や回避のモーション硬直が終了した際、内部ステートを初期化して次のユーザー入力を受け付ける状態に戻す
	void ResetNUETRAL();

	void SetInvisible(bool flag) { is_invisible_ = flag; }
	bool GetInvisible() { return is_invisible_; }

	// 入力: なし / 出力: なし
	// 副作用: 地面との接地判定を確認後、上方向への初速を与えて重力計算のステートを空中（ジャンプ中）へ切り替える
	void Jump();

	// 入力: なし / 出力: なし
	// 副作用: ロックオンの有無やエイム状態に応じて、カメラの追従モードやプレイヤーの向き補間ルールを動的に切り替える
	void UpdateViewMode();

	// 入力: なし / 出力: なし
	// 副作用: 索敵コライダー内で最も距離が近い敵オブジェクトを抽出し、ロックオン対象（target_）として設定する
	void SearchEnemy();

	// 入力: なし / 出力: なし
	// 副作用: プレイヤー本体の座標移動に追従させる形で、攻撃や索敵、本体物理用の各種コライダー座標を再計算する
	void UpdateColliderPosition();

	// 入力: なし / 出力: なし
	// 副作用: 各種攻撃スキルの再使用待機フレーム（クールダウン）を減算し、0になったスキルを入力可能状態にする
	void UpdateAttackCooldowns();

	// 入力: なし / 出力: なし
	// 副作用: 指定方向へ高速移動しつつ、一時的に無敵フラグ(is_invisible_)を有効化して敵の攻撃をすり抜ける処理を行う
	void Evasion();

	// 入力: なし / 出力: なし
	// 副作用: 攻撃モーションの終了時などに呼び出し、空間に残っている攻撃判定コライダーを安全に無効化・削除する
	void CollDelete();

	// 入力: なし / 出力: なし
	// 副作用: 敵の死亡や画面外への消失を検知し、無効になったロックオン対象のポインタを安全に破棄する
	void UpdateTargetLock();

	float GetAngle() { return angle_; }

	// 入力: なし / 出力: なし
	// 副作用: 移動先のベクトルに対してモデルのY軸回転角度(angle_)を徐々に補間し、不自然な瞬間振り向きを防止する
	void RotationByMove();

	float GetHp() { return hp_; }
	void SetHp(float hp) { hp_ = hp; }
	void StartDeath();
	bool IsDead() const { return is_dead_; }
	bool IsDeathAnimationFinished() const;
	float GetAttack() { return attack_; }
	void SetAttack(float attack) { attack_ = attack; }
	float GetSpeed() { return speed_; }
	void SetSpeed(float speed) { speed_ = speed_ + speed; }
	float GetSize() { return size_; }

	// 入力: state(ステータス種類) / 出力: 基礎値にバフや装備補正を加算した最終値(float) / 副作用: なし
	// ダメージ計算や移動処理を行う際、現在有効なすべての補正値が適用された正確なステータスを取得するために用いる
	float GetAllStatusState(Object3D::StatusState state);

	// --- Upgrade Setters ---
	void AddUpgradeMaxHp(float add) { upgrade_max_hp_ += add; }
	void AddUpgradeAttack(float add) { upgrade_attack_ += add; }
	void AddUpgradeSpeed(float add) { upgrade_speed_ += add; }
	void AddUpgradeEvasionSpeed(float add) { upgrade_evasion_speed_ += add; }
	void AddUpgradeEvasionInvincibility(int add) { upgrade_evasion_invincibility_ += add; }

	VECTOR GetFirstPos() { return first_position_; }
	bool GetStageOutFlag() { return is_stage_out_; }

	// 入力: なし / 出力: なし
	// 副作用: 2Dスクリーン座標上に投影計算を行い、プレイヤーの頭上にHPバーや各種ステータス状況を描画する
	void DrawStatusBars();

	// 入力: state(割り当てる状態), filename(モーションファイルのパス) / 出力: なし
	// 副作用: 指定した外部モーションファイル(.mv1)を読み込み、特定のアクションステートに紐付けて再生可能にする
	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	CapsuleCollider* GetCollisionCollider() { return capsule_collider_; }
	SphereCollider* Get500Collider() { return search_enemy_collider_; }

	Model* model_;
	ShortInventory* short_inventory_;
	BuffManager* buff_manager_;
	ItemManager* item_manager_;
	HaveMoneyClass* have_money_;
	EquipmentManager* equipment_manager_;

private:
	void UpdateInvincibilityTimer();
	void UpdatePlayerSystems();
	bool ShouldSkipGameplayUpdate() const;
	bool IsBossFadeActive() const;
	void ValidateTarget();
	void UpdateGameplayActions();
	bool CanDrawPlayer() const;
	void DrawPlayerModel();
	void DrawDebugInfo();
	void DrawAttachmentDebug();
	void ApplyJumpAttackHit(Collider* collider, Collider* check);

	Object3D* target_;                  // ロックオン中の対象オブジェクト（カメラ追従やホーミング攻撃の基準点）
	float target_angle_;                // 目標とする旋回角度（滑らかな振り向き補間に使用）
	float angle_;                       // 現在のY軸回転角度（ラジアン）
	VECTOR first_position_;             // 落下時やイベント時にリスポーンするための初期座標

	const float kRotateSpeed = 0.2f;    // 移動方向への旋回補間速度（大きすぎるとカクつき、小さすぎると操作感が悪化する）
	const float kJumpPower = 30.0f;     // ジャンプ時の初期上方向ベクトルの強さ

	bool is_jumping_;                   // 現在ジャンプステート中か否かを示すフラグ
	bool is_jump_falling_;              // ジャンプの頂点を過ぎて落下ステートに入ったかを示すフラグ
	bool has_reached_jump_peak_;        // ジャンプの頂点到達フラグ（重力計算の切り替え用）
	bool is_jump_collider_active_;      // ジャンプ攻撃の落下時、地面への衝撃波判定が有効かを示すフラグ
	bool is_attack_slide_target_found_; // スライド攻撃発動時、追尾すべきターゲットが射程内に存在したかを示すフラグ

	float jump_power_;                  // 現在のY軸方向への速度（重力によって毎フレーム減衰する）
	float speed_;                       // 装備やバフ補正を適用した現在の最終的な移動速度
	float evasion_speed_;               // 回避アクション実行時のダッシュ速度
	float normal_speed_;                // 補正を除外したキャラクターの基礎移動速度
	float attack_slide_speed_;          // スライド（突進）攻撃時の踏み込み速度
	float hp_;                          // 現在の体力値
	float max_hp_;                      // 最大体力値
	float ride_old_hp_;                 // 乗り物搭乗前など、ステータス一時変更前の退避用HPバッファ
	float size_;                        // キャラクターの描画スケールと、壁や床との基本当たり判定半径

	// --- Upgrade Stats from StatShop ---
	float upgrade_max_hp_ = 0.0f;           // ショップで購入した最大HPの永続加算値
	float upgrade_attack_ = 0.0f;           // ショップで購入した基礎攻撃力の永続加算値
	float upgrade_speed_ = 0.0f;            // ショップで購入した移動速度の永続加算値
	float upgrade_evasion_speed_ = 0.0f;    // ショップで購入した回避距離（速度）の永続加算値
	int upgrade_evasion_invincibility_ = 0; // ショップで購入した回避時の無敵時間（フレーム）の永続加算値
	int invincible_timer_ = 0;              // 被弾後や回避中における、無敵状態の残り持続フレーム数
	// -----------------------------------

	int attack_slide_count_;            // スライド攻撃のアニメーション進行フレームカウンター
	int attack_jump_count_;             // ジャンプ攻撃のアニメーション進行フレームカウンター
	int attack_count_;                  // 通常攻撃のコンボ段数または進行フレームカウンター

	int attack_slide_cooldown_ = 200;   // スライド攻撃の再使用待機フレーム数
	int attack_cooldown_ = 30;          // 通常攻撃の次弾発射（コンボ受付）猶予フレーム数
	int attack_jump_cooldown_ = 200;    // ジャンプ攻撃の再使用待機フレーム数

	float attack_;                      // バフや装備補正を適用した現在の最終攻撃力
	float normal_attack_;               // 通常攻撃のモーション値（倍率または基礎加算値）
	float jump_attack_;                 // ジャンプ攻撃のモーション値
	float slide_attack_;                // スライド攻撃のモーション値

	VECTOR move_vec_ = VGet(0.0f, 0.0f, 0.0f); // 現在の移動ベクトル
	VECTOR previous_move_vec_;          // 慣性計算や移動停止直後の向き保持のための前フレーム移動ベクトル
	VECTOR attack_slide_direction_;     // スライド攻撃で突進する対象への方向ベクトル
	VECTOR attack_slide_step_;          // スライド攻撃中の毎フレームの座標加算量

	bool is_invisible_;                 // 被弾直後や回避アクション中の完全無敵状態を示すフラグ
	bool is_dead_ = false;              // 死亡アニメーション中かどうか

	int target_search_count_;           // 索敵処理の負荷分散（数フレームに1回実行）用のカウンター
	float nearest_target_distance_;     // ロックオン・索敵判定に用いる、最も近い敵までの距離キャッシュ

	SphereCollider* attach_collider_;       // 武器の軌跡など、特定のボーンに追従して発生する攻撃判定
	SphereCollider* search_enemy_collider_; // カメラの自動ロックオン対象を抽出するための広域索敵判定
	SphereCollider* attack_slide_collider_; // スライド攻撃（突進）の進行方向に展開される攻撃判定
	SphereCollider* attack_jump_collider_;  // ジャンプ攻撃着地時に周囲に展開される衝撃波判定
	CapsuleCollider* capsule_collider_;     // 地形や敵とのめり込みを防ぐための、プレイヤー本体の物理空間判定

	int attack_selection_index_;        // UIや内部ロジックで選択されている、現在アクティブな攻撃スキルの識別ID
	bool is_stage_out_;                 // 落下等によりステージ外に出た（リスポーン処理が必要）ことを示すフラグ
};
