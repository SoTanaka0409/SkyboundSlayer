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
	void SetHitSizeFlag(bool flag) { is_hit_size_flag_ = flag; }

	/// @brief 被ダメージ時の判定サイズ変更フラグを取得する
	/// @return bool フラグの状態
	bool IsHitSizeFlag() { return is_hit_size_flag_; }

	/// @brief 生成直後（初期化処理中）フラグを設定する
	/// @param flag 設定するフラグ値
	void SetNewEnemyFlag(bool flag) { is_new_enemy_flag_ = flag; }

	/// @brief 生成直後（初期化処理中）フラグを取得する
	/// @return bool フラグの状態
	bool IsNewEnemyFlag() { return is_new_enemy_flag_; }

	/// @brief 敵のカプセルコライダーを取得する
	/// @return CapsuleCollider* カプセルコライダーのポインタ
	virtual CapsuleCollider* GetEnemyCollider() { return capsule_collider_; }

private:
	bool is_hit_size_flag_;            ///< 被ダメージ時のノックバックや特殊な判定サイズ変更を管理するフラグ
	bool is_new_enemy_flag_;           ///< 生成直後の初期化処理（落下アニメーション等）の実行状態を管理するフラグ

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
	/// @param is_separate_anim_ アニメーション分離処理を行うかどうかのフラグ
	/// @details 敵の3Dモデルや各用途（索敵・攻撃等）のコライダー群の動的確保と初期化を行う
	Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_);

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
	/// @details HPの減算処理および、0以下になった際の死亡ステート（is_dead_）への移行発火を行う
	virtual void Damage(float damage);

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
	/// @details 死亡時に確率計算を行い、プレイヤーへ所持金(have_money_)やドロップアイテムを付与する
	virtual void GiveRewards();

	/// @brief 各種コライダーの座標をモデルの最新位置に同期させる
	virtual void UpdateColliderPosition();

	/// @brief 現在のHPを取得する
	/// @return float 現在のHP
	float GetHp() { return hp_; }

	/// @brief HPを設定する
	/// @param hp 設定するHP
	void SetHp(float hp) { hp_ = hp; }

	/// @brief 最大HPを取得する
	/// @return float 最大HP
	float GetMaxHp() { return max_hp_; }

	/// @brief 最大HPを設定する
	/// @param mhp 設定する最大HP
	void SetMaxHp(float mhp) { max_hp_ = mhp; }

	/// @brief 非表示・無敵状態フラグを取得する
	/// @return bool 非表示状態ならtrue
	bool GetInvisible() { return is_invisible_; }

	/// @brief 非表示・無敵状態フラグを設定する
	/// @param Inv 設定値（0以外で有効）
	void SetInvisible(int Inv) { is_invisible_ = Inv; }

	/// @brief 現在向かっている目標座標を取得する
	/// @return VECTOR 目標座標
	VECTOR GetGoPosition() { return go_position_; }

	/// @brief 目標座標を設定する
	/// @param goplayer 設定する目標座標
	void SetGoPosition(VECTOR goplayer) { go_position_ = goplayer; }

	/// @brief 初期配置座標を設定する
	/// @param pos 設定する初期位置
	void SetInitPosition(VECTOR pos) { init_position_ = pos; }

	/// @brief 初期配置座標を取得する
	/// @return VECTOR 初期位置
	VECTOR GetInitPosition() { return init_position_; }

	/// @brief オブジェクトのサイズスケールを取得する
	/// @return float サイズ
	float GetSize() { return size_; }

	/// @brief プレイヤーとのヒット判定フラグを設定する
	/// @param flag 設定するフラグ値
	void SetHitJudgmentFlagPlayer(bool flag) { is_hit_judgment_flag_player_ = flag; }

	/// @brief プレイヤーとのヒット判定フラグを取得する
	/// @return bool フラグの状態
	bool IsHitJudgmentFlagPlayer() { return is_hit_judgment_flag_player_; }

	/// @brief 死亡状態かどうかを取得する
	/// @return bool 死亡状態ならtrue
	bool IsDead() { return is_dead_; }

	/// @brief 死亡状態フラグを設定する
	/// @param dead 死亡状態にするならtrue
	void SetIsDead(bool dead) { is_dead_ = dead; }

	/// @brief 索敵判定にヒットしているかを取得する
	/// @return bool ヒット中ならtrue
	bool IsHitSearchFlag() const { return is_hit_search_flag_; }

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
	SphereCollider* attach_collider_;    ///< 追従・取りつけ用コライダー
	CapsuleCollider* capsule_collider_;  ///< オブジェクト本体の物理的な衝突判定（壁抜け防止や被弾判定）
	SphereCollider* serch_collider_;     ///< プレイヤーを検知して追跡モードへ移行するための広域索敵判定
	SphereCollider* attack_collider_;    ///< プレイヤーがこの判定内に入ると攻撃アクションをトリガーする判定
	SphereCollider* stop_collider_;      ///< プレイヤーとの密着を防ぎ、適切な間合いを保つための停止判定
	Model* model_;                       ///< 3Dモデルデータポインタ
	Debug* debug_;                       ///< デバッグ描画・ログ表示機能ポインタ

protected:
	float hp_;                           ///< 現在のHP
	float max_hp_;                       ///< 最大HP
	float speed_;                        ///< 現在の移動速度
	float normal_speed_;                 ///< 通常時の移動速度
	float max_speed_;                    ///< 追跡時などの最大移動速度
	float attack_;                       ///< 基礎攻撃力

	int walk_timer_;                     ///< パトロール時の進行方向切り替えなどを計るためのタイマー
	int walk_count_;                     ///< 歩行・パトロール状態の処理カウンタ

	VECTOR hit_pos_ = VGet(0.0f, 0.0f, 0.0f); ///< ヒット・被弾位置座標
	VECTOR go_position_;                 ///< AIが現在向かおうとしているワールド空間上の目標座標
	VECTOR old_position_;                ///< 1フレーム前のワールド座標
	VECTOR init_position_;               ///< ヘイトが切れた際に、元の位置へ帰還するためのスポーン座標
	VECTOR no_position_;                 ///< ダミー・退避用座標
	VECTOR move_vec_;                    ///< 現在の移動ベクトル
	VECTOR up_move_vector_;              ///< 上方向の移動ベクトル
	VECTOR left_move_vector_;            ///< 左方向の移動ベクトル
	bool is_invisible_;                  ///< 描画および当たり判定をスキップするための無敵/非表示フラグ
	int algorithm_id_;                   ///< 待機、追跡、攻撃などのAIステートを切り替えるための状態ID
	int alg_hit_;                        ///< アルゴリズム計算用ヒット識別値

	int attack_interval_;                ///< 攻撃間隔（クールダウン）の最大フレーム数
	int attack_count_;                   ///< 攻撃クールのカウントダウンタイマー

	bool is_animation_;                  ///< アニメーション再生中かどうかを示すフラグ
	float target_angle_;                 ///< 瞬時な振り向きを避け、滑らかに旋回するための目標角度
	float angle_;                        ///< 現在のモデルのY軸回転角度（ラジアン）
	const float ROTATE_SPEED = 0.1f;     ///< 旋回時の補間係数（大きすぎるとカクつき、小さすぎると追従が遅れる）

	float size_;                         ///< モデル・コライダーのサイズスケール
	float hit_search_;                   ///< 索敵判定の半径（仕様制約に基づく）
	float hit_attack_search_;            ///< 攻撃トリガーとなる判定の半径
	float hit_stop_search_;              ///< 接近を停止する限界距離の半径
	bool is_dead_;                       ///< 死亡判定フラグ（HP <= 0 でtrue）

	bool is_hit_search_flag_;            ///< プレイヤーを索敵判定内に捉えているかのフラグ
	bool is_hit_attack_search_flag_;     ///< プレイヤーを攻撃判定内に捉えているかのフラグ
	bool is_hit_search_stop_flag_;       ///< プレイヤーが停止判定内に入っているかのフラグ
	bool is_hit_attack_flag_;            ///< 現在攻撃モーション中であり、ダメージ判定を生成すべきかを示すフラグ

	int chance_;                         ///< レアドロップなどのアイテムドロップ抽選に使用する確率値
	bool has_item_ = true;               ///< 死亡時に報酬としてアイテムをドロップするかを制御するフラグ

	bool is_attack_hit_judgment_flag_;   ///< プレイヤーへの多段ヒットを防ぐため、1モーションにつき1回のダメージを保証するフラグ
	bool is_hit_judgment_flag_player_;   ///< プレイヤー側の攻撃判定とヒットしたかどうかのフラグ

	int have_money_;                     ///< 死亡時にプレイヤーに付与するスコア・通貨の量
};