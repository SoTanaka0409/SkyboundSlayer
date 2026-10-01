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

/// @brief プレイヤーキャラクターの入力制御、物理挙動（移動・ジャンプ）、攻撃ステート、およびステータス管理を統括するクラス
class Player3D : public Object3D
{
public:
	enum AttackStateInfo {
		kAttackNormal,
		kAttackJump,
		kAttackSlide,
	} m_AttackState;

public:
/// @param filename, 初期座標, ジャンプ力, 移動速度, 初期HP, アニメ分離フラグ
/// @details プレイヤーの3Dモデル、各種アクション用コライダー、および関連マネージャー（装備・バフ等）の初期化を行う
	Player3D(std::string filename, VECTOR initPos, float jumpPower, float speed, float hp, bool m_IsSeparateAnim = false);

/// @details 動的確保した各マネージャーインスタンスや攻撃用コライダー群を破棄し、メモリリークを防ぐ
	~Player3D();

/// @details アニメーション、物理挙動、入力検知によるステート遷移（攻撃や回避など）を毎フレーム進行させる
	void Update() override;

/// @details 3Dモデル本体の姿勢を反映して描画し、加えてHPバーなどのプレイヤー専用空間UIをバッファへ登録する
	void Draw() override;

/// @details インベントリや装備効果、バフ効果の持続時間計算など、プレイヤーに紐付くサブシステムの更新を一括処理する
	void ManagerUpdate();

/// @details Y座標が一定値を下回った（奈落へ落下した）場合にダメージ処理を行い、安全な初期座標へ強制リスポーンさせる
	void CheckStageOut();

/// @details カメラの向いているベクトルとコントローラーの入力情報から実際の移動ベクトルを算出し、座標を更新する
	void MoveEx();

/// @details プレイヤーの入力状況と各スキルのクールダウン状態を判定し、適切な攻撃ステート（通常・スライド・ジャンプ）へ遷移させる
	void SelectAttack();

	void Attack();
	void AttackSlide();
	void AttackJump();

/// @param damage(被ダメージ量)
/// @details 無敵時間(m_IsInvisible)をチェックした上でHPを減算し、被弾アニメーションやヒットストップのトリガーを発火させる
	void Damage(float damage);

/// @details 攻撃や回避のモーション硬直が終了した際、内部ステートを初期化して次のユーザー入力を受け付ける状態に戻す
	void ResetNUETRAL();

	void SetInvisible(bool flag) { m_IsInvisible = flag; }
	bool GetInvisible() { return m_IsInvisible; }

/// @details 地面との接地判定を確認後、上方向への初速を与えて重力計算のステートを空中（ジャンプ中）へ切り替える
	void Jump();

/// @details ロックオンの有無やエイム状態に応じて、カメラの追従モードやプレイヤーの向き補間ルールを動的に切り替える
	void UpdateViewMode();

/// @details 索敵コライダー内で最も距離が近い敵オブジェクトを抽出し、ロックオン対象（m_Target）として設定する
	void SearchEnemy();

/// @details プレイヤー本体の座標移動に追従させる形で、攻撃や索敵、本体物理用の各種コライダー座標を再計算する
	void UpdateColliderPosition();

/// @details 各種攻撃スキルの再使用待機フレーム（クールダウン）を減算し、0になったスキルを入力可能状態にする
	void UpdateAttackCooldowns();

/// @details 指定方向へ高速移動しつつ、一時的に無敵フラグ(m_IsInvisible)を有効化して敵の攻撃をすり抜ける処理を行う
	void Evasion();

/// @details 攻撃モーションの終了時などに呼び出し、空間に残っている攻撃判定コライダーを安全に無効化・削除する
	void CollDelete();

/// @details 敵の死亡や画面外への消失を検知し、無効になったロックオン対象のポインタを安全に破棄する
	void UpdateTargetLock();

	float GetAngle() { return m_Angle; }

/// @details 移動先のベクトルに対してモデルのY軸回転角度(m_Angle)を徐々に補間し、不自然な瞬間振り向きを防止する
	void RotationByMove();

	float GetHp() { return m_Hp; }
	float GetMaxHp() { return m_MaxHp + m_UpgradeMaxHp; }
	void SetHp(float hp) { m_Hp = hp; }
	void StartDeath();
	bool IsDead() const { return m_IsDead; }
	bool IsDeathAnimationFinished() const;
	float GetAttack() { return m_Attack; }
	void SetAttack(float attack) { m_Attack = attack; }
	float GetSpeed() { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = m_Speed + speed; }
	float GetSize() { return m_Size; }

/// @param state(ステータス種類)
/// @return 基礎値にバフや装備補正を加算した最終値(float)
	/// @brief ダメージ計算や移動処理を行う際、現在有効なすべての補正値が適用された正確なステータスを取得するために用いる
	float GetAllStatusState(Object3D::StatusState state);

	/// @brief --- Upgrade Setters ---
	void AddUpgradeMaxHp(float add) { m_UpgradeMaxHp += add; }
	void AddUpgradeAttack(float add) { m_UpgradeAttack += add; }
	void AddUpgradeSpeed(float add) { m_UpgradeSpeed += add; }
	void AddUpgradeEvasionSpeed(float add) { m_UpgradeEvasionSpeed += add; }
	void AddUpgradeEvasionInvincibility(int add) { m_UpgradeEvasionInvincibility += add; }

	VECTOR GetFirstPos() { return m_FirstPosition; }
	bool GetStageOutFlag() { return m_IsStageOut; }

/// @details 2Dスクリーン座標上に投影計算を行い、プレイヤーの頭上にHPバーや各種ステータス状況を描画する
	void DrawStatusBars();

/// @param state(割り当てる状態), filename(モーションファイルのパス)
/// @details 指定した外部モーションファイル(.mv1)を読み込み、特定のアクションステートに紐付けて再生可能にする
	void AddAnimation(AnimationState state, std::string filename);

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	CapsuleCollider* GetCollisionCollider() { return m_CapsuleCollider; }
	SphereCollider* Get500Collider() { return m_SearchEnemyCollider; }

	Model* m_Model;
	ShortInventory* m_ShortInventory;
	BuffManager* m_BuffManager;
	ItemManager* m_ItemManager;
	HaveMoneyClass* m_HaveMoney;
	EquipmentManager* m_EquipmentManager;

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

	Object3D* m_Target;                  // ロックオン中の対象オブジェクト（カメラ追従やホーミング攻撃の基準点）
	float m_TargetAngle;                // 目標とする旋回角度（滑らかな振り向き補間に使用）
	float m_Angle;                       // 現在のY軸回転角度（ラジアン）
	VECTOR m_FirstPosition;             // 落下時やイベント時にリスポーンするための初期座標

	const float RotateSpeed = 0.2f;    // 移動方向への旋回補間速度（大きすぎるとカクつき、小さすぎると操作感が悪化する）
	const float JumpPower = 30.0f;     // ジャンプ時の初期上方向ベクトルの強さ

	bool m_IsJumping;                   // 現在ジャンプステート中か否かを示すフラグ
	bool m_IsJumpFalling;              // ジャンプの頂点を過ぎて落下ステートに入ったかを示すフラグ
	bool m_HasReachedJumpPeak;        // ジャンプの頂点到達フラグ（重力計算の切り替え用）
	bool m_IsJumpColliderActive;      // ジャンプ攻撃の落下時、地面への衝撃波判定が有効かを示すフラグ
	bool m_IsAttackSlideTargetFound; // スライド攻撃発動時、追尾すべきターゲットが射程内に存在したかを示すフラグ

	float m_JumpPower;                  // 現在のY軸方向への速度（重力によって毎フレーム減衰する）
	float m_Speed;                       // 装備やバフ補正を適用した現在の最終的な移動速度
	float m_EvasionSpeed;               // 回避アクション実行時のダッシュ速度
	float m_NormalSpeed;                // 補正を除外したキャラクターの基礎移動速度
	float m_AttackSlideSpeed;          // スライド（突進）攻撃時の踏み込み速度
	float m_Hp;                          // 現在の体力値
	float m_MaxHp;                      // 最大体力値
	float m_RideOldHp;                 // 乗り物搭乗前など、ステータス一時変更前の退避用HPバッファ
	float m_Size;                        // キャラクターの描画スケールと、壁や床との基本当たり判定半径

	/// @brief --- Upgrade Stats from StatShop ---
	float m_UpgradeMaxHp = 0.0f;           // ショップで購入した最大HPの永続加算値
	float m_UpgradeAttack = 0.0f;           // ショップで購入した基礎攻撃力の永続加算値
	float m_UpgradeSpeed = 0.0f;            // ショップで購入した移動速度の永続加算値
	float m_UpgradeEvasionSpeed = 0.0f;    // ショップで購入した回避距離（速度）の永続加算値
	int m_UpgradeEvasionInvincibility = 0; // ショップで購入した回避時の無敵時間（フレーム）の永続加算値
	int m_InvincibleTimer = 0;              // 被弾後や回避中における、無敵状態の残り持続フレーム数
	// -----------------------------------

	int m_AttackSlideCount;            // スライド攻撃のアニメーション進行フレームカウンター
	int m_AttackJumpCount;             // ジャンプ攻撃のアニメーション進行フレームカウンター
	int m_AttackCount;                  // 通常攻撃のコンボ段数または進行フレームカウンター

	int m_AttackSlideCooldown = 200;   // スライド攻撃の再使用待機フレーム数
	int m_AttackCooldown = 30;          // 通常攻撃の次弾発射（コンボ受付）猶予フレーム数
	int m_AttackJumpCooldown = 200;    // ジャンプ攻撃の再使用待機フレーム数

	float m_Attack;                      // バフや装備補正を適用した現在の最終攻撃力
	float m_NormalAttack;               // 通常攻撃のモーション値（倍率または基礎加算値）
	float m_JumpAttack;                 // ジャンプ攻撃のモーション値
	float m_SlideAttack;                // スライド攻撃のモーション値

	VECTOR m_MoveVec = VGet(0.0f, 0.0f, 0.0f); // 現在の移動ベクトル
	VECTOR m_PreviousMoveVec;          // 慣性計算や移動停止直後の向き保持のための前フレーム移動ベクトル
	VECTOR m_AttackSlideDirection;     // スライド攻撃で突進する対象への方向ベクトル
	VECTOR m_AttackSlideStep;          // スライド攻撃中の毎フレームの座標加算量

	bool m_IsInvisible;                 // 被弾直後や回避アクション中の完全無敵状態を示すフラグ
	bool m_IsDead = false;              // 死亡アニメーション中かどうか

	int m_TargetSearchCount;           // 索敵処理の負荷分散（数フレームに1回実行）用のカウンター
	float m_NearestTargetDistance;     // ロックオン・索敵判定に用いる、最も近い敵までの距離キャッシュ

	SphereCollider* m_AttachCollider;       // 武器の軌跡など、特定のボーンに追従して発生する攻撃判定
	SphereCollider* m_SearchEnemyCollider; // カメラの自動ロックオン対象を抽出するための広域索敵判定
	SphereCollider* m_AttackSlideCollider; // スライド攻撃（突進）の進行方向に展開される攻撃判定
	SphereCollider* m_AttackJumpCollider;  // ジャンプ攻撃着地時に周囲に展開される衝撃波判定
	CapsuleCollider* m_CapsuleCollider;     // 地形や敵とのめり込みを防ぐための、プレイヤー本体の物理空間判定

	int m_AttackSelectionIndex;        // UIや内部ロジックで選択されている、現在アクティブな攻撃スキルの識別ID
	bool m_IsStageOut;                 // 落下等によりステージ外に出た（リスポーン処理が必要）ことを示すフラグ
};
