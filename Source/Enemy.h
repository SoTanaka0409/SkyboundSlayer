#pragma once
#include "DxLib.h"
#include "Texture.h"
#include "Object3D.h"
#include "Model.h"
#include "Debug.h"

#include "Item.h"
#include "ColliderManager.h"
#include "EquipmentManager.h"

class SphereCollider;
class CapsuleCollider;

/// @brief 索敵・追跡・攻撃のAIを持ち、プレイヤーと敵対するキャラクターの基底クラス
class Enemy : public Object3D
{
public:
	/// @brief 被ダメージ時の判定サイズ変更フラグを設定する
	/// @param flag 設定するフラグ値
	void SetHitSizeFlag(bool flag) { m_IsHitSizeFlag = flag; }

	/// @brief 被ダメージ時の判定サイズ変更フラグを取得する
	/// @return bool フラグの状態
	bool IsHitSizeFlag() { return m_IsHitSizeFlag; }

	/// @brief 生成直後（初期化処理中）フラグを設定する
	/// @param flag 設定するフラグ値
	void SetNewEnemyFlag(bool flag) { m_IsNewEnemyFlag = flag; }

	/// @brief 生成直後（初期化処理中）フラグを取得する
	/// @return bool フラグの状態
	bool IsNewEnemyFlag() { return m_IsNewEnemyFlag; }

	/// @brief 敵のカプセルコライダーを取得する
	/// @return CapsuleCollider* カプセルコライダーのポインタ
	virtual CapsuleCollider* GetEnemyCollider() { return m_CapsuleCollider; }

private:
	bool m_IsHitSizeFlag;            ///< 被ダメージ時のノックバックや特殊な判定サイズ変更を管理するフラグ
	bool m_IsNewEnemyFlag;           ///< 生成直後の初期化処理（落下アニメーション等）の実行状態を管理するフラグ

public:
	/// @brief Enemyクラスのコンストラクタ
	/// @param filename 3Dモデルのファイルパス
	/// @param initPos 初期配置座標
	/// @param hp 初期体力
	/// @param speed 移動速度
	/// @param attack 攻撃力
	/// @param HitSize 被弾コライダーサイズ
	/// @param Serch1 索敵判定の半径
	/// @param Serch2 攻撃開始判定の半径
	/// @param Serch3 接近停止判定の半径
	/// @param money 倒した際の所持金（獲得スコア）
	/// @param m_IsSeparateAnim アニメーション分離処理を行うかどうかのフラグ
	/// @details 敵の3Dモデルや各用途（索敵・攻撃等）のコライダー群の動的確保と初期化を行う
	Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool m_IsSeparateAnim);

	/// @brief Enemyクラスのデストラクタ
	virtual ~Enemy();

	/// @brief 敵モデルおよびデバッグ情報の描画処理を行う
	virtual void Draw() override;

	/// @brief 毎フレームの更新処理（AIステート更新、移動、攻撃など）を行う
	virtual void Update() override;

	/// @brief 現在のAIステート（待機・追跡等）に基づく目標座標への移動ベクトルの算出と座標更新を行う
	virtual void Move();

	/// @brief 移動方向へモデルの向きを補間計算し、急な振り向きによる不自然な描画を防ぐ
	virtual void RotationByMove();

	/// @brief 敵にダメージを与える
	/// @param damage 被ダメージ量
	/// @details HPの減算処理および、0以下になった際の死亡ステート（m_IsDead）への移行発火を行う
	virtual void Damage(float damage, bool play_sound = true);

	/// @brief 攻撃パターンのリスト処理を行う
	virtual void AttackList();

	/// @brief 攻撃の実行処理を行う
	/// @details 攻撃インターバルの進行と、条件合致時におけるプレイヤーへのダメージ判定の生成を行う
	virtual void Attack();

	/// @brief オブジェクトの破棄・削除準備を行う
	virtual void Delete();

	/// @brief 死亡時のコライダー位置の更新・調整を行う
	virtual void DeathColliderPosition();

	/// @brief 敵の死亡時処理を実行する
	/// @details 死亡アニメーションの再生開始や、不要になった索敵・攻撃コライダーの無効化を行う
	virtual void DeathEnemy();

	/// @brief プレイヤーへ倒した際の報酬を付与する
	/// @details 死亡時に確率計算を行い、プレイヤーへ所持金(m_HaveMoney)やドロップアイテムを付与する
	virtual void GiveRewards();

	/// @brief 各種コライダーの座標をモデルの最新位置に同期させる
	virtual void UpdateColliderPosition();

	/// @brief 現在のHPを取得する
	/// @return float 現在のHP
	float GetHp() { return m_Hp; }

	/// @brief HPを設定する
	/// @param hp 設定するHP
	void SetHp(float hp) { m_Hp = hp; }

	/// @brief 最大HPを取得する
	/// @return float 最大HP
	float GetMaxHp() { return m_MaxHp; }

	/// @brief 最大HPを設定する
	/// @param mhp 設定する最大HP
	void SetMaxHp(float mhp) { m_MaxHp = mhp; }

	/// @brief 非表示・無敵状態フラグを取得する
	/// @return bool 非表示状態ならtrue
	bool GetInvisible() { return m_IsInvisible; }

	/// @brief 非表示・無敵状態フラグを設定する
	/// @param Inv 設定値（0以外で有効）
	void SetInvisible(int Inv) { m_IsInvisible = Inv; }

	/// @brief 現在向かっている目標座標を取得する
	/// @return VECTOR 目標座標
	VECTOR GetGoPosition() { return m_GoPosition; }

	/// @brief 目標座標を設定する
	/// @param goplayer 設定する目標座標
	void SetGoPosition(VECTOR goplayer) { m_GoPosition = goplayer; }

	/// @brief 初期配置座標を設定する
	/// @param pos 設定する初期位置
	void SetInitPosition(VECTOR pos) { m_InitPosition = pos; }

	/// @brief 初期配置座標を取得する
	/// @return VECTOR 初期位置
	VECTOR GetInitPosition() { return m_InitPosition; }

	/// @brief オブジェクトのサイズスケールを取得する
	/// @return float サイズ
	float GetSize() { return m_Size; }

	/// @brief プレイヤーとのヒット判定フラグを設定する
	/// @param flag 設定するフラグ値
	void SetHitJudgmentFlagPlayer(bool flag) { m_IsHitJudgmentFlagPlayer = flag; }

	/// @brief プレイヤーとのヒット判定フラグを取得する
	/// @return bool フラグの状態
	bool IsHitJudgmentFlagPlayer() { return m_IsHitJudgmentFlagPlayer; }

	/// @brief 死亡状態かどうかを取得する
	/// @return bool 死亡状態ならtrue
	bool IsDead() { return m_IsDead; }

	/// @brief 死亡状態フラグを設定する
	/// @param dead 死亡状態にするならtrue
	void SetIsDead(bool dead) { m_IsDead = dead; }

	/// @brief 索敵判定にヒットしているかを取得する
	/// @return bool ヒット中ならtrue
	bool IsHitSearchFlag() const { return m_IsHitSearchFlag; }

	/// @brief 他のコライダーと接触した瞬間のイベント処理
	/// @param collider 自身のコライダー
	/// @param check 接触した相手のコライダー
	/// @details 索敵コライダーにプレイヤーが侵入した際、待機から追跡ステートへAIを切り替える
	virtual void OnEnter(Collider* collider, Collider* check) override;

	/// @brief 他のコライダーと接触し続けている間のイベント処理
	/// @param collider 自身のコライダー
	/// @param check 接触した相手のコライダー
	virtual void OnTrigger(Collider* collider, Collider* check) override;

	/// @brief 他のコライダーから離れた瞬間のイベント処理
	/// @param collider 自身のコライダー
	/// @param check 離れた相手のコライダー
	virtual void OnExit(Collider* collider, Collider* check) override;

protected:
	SphereCollider* m_AttachCollider;    ///< 追従・取りつけ用コライダー
	CapsuleCollider* m_CapsuleCollider;  ///< オブジェクト本体の物理的な衝突判定（壁抜け防止や被弾判定）
	SphereCollider* m_SerchCollider;     ///< プレイヤーを検知して追跡モードへ移行するための広域索敵判定
	SphereCollider* m_AttackCollider;    ///< プレイヤーがこの判定内に入ると攻撃アクションをトリガーする判定
	SphereCollider* m_StopCollider;      ///< プレイヤーとの密着を防ぎ、適切な間合いを保つための停止判定
	Model* m_Model;                       ///< 3Dモデルデータポインタ
	Debug* m_Debug;                       ///< デバッグ描画・ログ表示機能ポインタ

protected:
	float m_Hp;                           ///< 現在のHP
	float m_MaxHp;                       ///< 最大HP
	float m_Speed;                        ///< 現在の移動速度
	float m_NormalSpeed;                 ///< 通常時の移動速度
	float m_MaxSpeed;                    ///< 追跡時などの最大移動速度
	float m_Attack;                       ///< 基礎攻撃力

	int m_WalkTimer;                     ///< パトロール時の進行方向切り替えなどを計るためのタイマー
	int m_WalkCount;                     ///< 歩行・パトロール状態の処理カウンタ

	VECTOR m_HitPos = VGet(0.0f, 0.0f, 0.0f); ///< ヒット・被弾位置座標
	VECTOR m_GoPosition;                 ///< AIが現在向かおうとしているワールド空間上の目標座標
	VECTOR m_OldPosition;                ///< 1フレーム前のワールド座標
	VECTOR m_InitPosition;               ///< ヘイトが切れた際に、元の位置へ帰還するためのスポーン座標
	VECTOR m_NoPosition;                 ///< ダミー・退避用座標
	VECTOR m_MoveVec;                    ///< 現在の移動ベクトル
	VECTOR m_UpMoveVector;              ///< 上方向の移動ベクトル
	VECTOR m_LeftMoveVector;            ///< 左方向の移動ベクトル
	bool m_IsInvisible;                  ///< 描画および当たり判定をスキップするための無敵/非表示フラグ
	int m_AlgorithmId;                   ///< 待機、追跡、攻撃などのAIステートを切り替えるための状態ID
	int m_AlgHit;                        ///< アルゴリズム計算用ヒット識別値

	int m_AttackInterval;                ///< 攻撃間隔（クールダウン）の最大フレーム数
	int m_AttackCount;                   ///< 攻撃クールのカウントダウンタイマー

	bool m_IsAnimation;                  ///< アニメーション再生中かどうかを示すフラグ
	float m_TargetAngle;                 ///< 瞬時な振り向きを避け、滑らかに旋回するための目標角度
	float m_Angle;                        ///< 現在のモデルのY軸回転角度（ラジアン）
	const float ROTATE_SPEED = 0.1f;     ///< 旋回時の補間係数（大きすぎるとカクつき、小さすぎると追従が遅れる）

	float m_Size;                         ///< モデル・コライダーのサイズスケール
	float m_HitSearch;                   ///< 索敵判定の半径（仕様制約に基づく）
	float m_HitAttackSearch;            ///< 攻撃トリガーとなる判定の半径
	float m_HitStopSearch;              ///< 接近を停止する限界距離の半径
	bool m_IsDead;                       ///< 死亡判定フラグ（HP <= 0 でtrue）

	bool m_IsHitSearchFlag;            ///< プレイヤーを索敵判定内に捉えているかのフラグ
	bool m_IsHitAttackSearchFlag;     ///< プレイヤーを攻撃判定内に捉えているかのフラグ
	bool m_IsHitSearchStopFlag;       ///< プレイヤーが停止判定内に入っているかのフラグ
	bool m_IsHitAttackFlag;            ///< 現在攻撃モーション中であり、ダメージ判定を生成すべきかを示すフラグ

	int m_Chance;                         ///< レアドロップなどのアイテムドロップ抽選に使用する確率値
	bool m_HasItem = true;               ///< 死亡時に報酬としてアイテムをドロップするかを制御するフラグ

	bool m_IsAttackHitJudgmentFlag;   ///< プレイヤーへの多段ヒットを防ぐため、1モーションにつき1回のダメージを保証するフラグ
	bool m_IsHitJudgmentFlagPlayer;   ///< プレイヤー側の攻撃判定とヒットしたかどうかのフラグ

	int m_HaveMoney;                     ///< 死亡時にプレイヤーに付与するスコア・通貨の量
};