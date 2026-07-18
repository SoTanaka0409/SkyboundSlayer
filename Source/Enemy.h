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

// 索敵・追跡・攻撃のAIを持ち、プレイヤーと敵対するキャラクターの基底クラス
class Enemy : public Object3D
{
public:
    void SetHitSizeFlag(bool flag) { is_hit_size_flag_ = flag; }
    bool IsHitSizeFlag() { return is_hit_size_flag_; }

    void SetNewEnemyFlag(bool flag) { is_new_enemy_flag_ = flag; }
    bool IsNewEnemyFlag() { return is_new_enemy_flag_; }

    virtual CapsuleCollider* GetEnemyCollider() { return capsule_collider_; }

private:
    bool is_hit_size_flag_; // 被ダメージ時のノックバックや特殊な判定サイズ変更を管理するフラグ
    bool is_new_enemy_flag_;// 生成直後の初期化処理（落下アニメーション等）の実行状態を管理するフラグ

public:
    // 入力: filename, 初期座標, HP, 移動速度, 攻撃力, 判定サイズ, 索敵半径群, 所持金, アニメ分離フラグ
    // 出力: なし / 副作用: 敵の3Dモデルや各用途（索敵・攻撃等）のコライダー群の動的確保と初期化
    Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_);
    virtual ~Enemy();

    virtual void Draw() override;
    virtual void Update() override;

    // 入力: なし / 出力: なし
    // 副作用: 現在のAIステート（待機・追跡等）に基づく目標座標への移動ベクトルの算出と座標更新
    virtual void Move();

    // 入力: なし / 出力: なし
    // 副作用: 移動方向へモデルの向きを補間計算し、急な振り向きによる不自然な描画を防ぐ
    virtual void RotationByMove();

    // 入力: damage(被ダメージ量) / 出力: なし
    // 副作用: HPの減算処理および、0以下になった際の死亡ステート（is_dead_）への移行発火
    virtual void Damage(float damage);

    virtual void AttackList();

    // 入力: なし / 出力: なし
    // 副作用: 攻撃インターバルの進行と、条件合致時におけるプレイヤーへのダメージ判定の生成
    virtual void Attack();

    virtual void Delete();
    virtual void DeathColliderPosition();

    // 入力: なし / 出力: なし
    // 副作用: 死亡アニメーションの再生開始や、不要になった索敵・攻撃コライダーの無効化を行う
    virtual void DeathEnemy();

    // 入力: なし / 出力: なし
    // 副作用: 死亡時に確率計算を行い、プレイヤーへ所持金(have_money_)やドロップアイテムを付与する
    virtual void GiveRewards();

    virtual void UpdateColliderPosition();

    float GetHp() { return hp_; }
    void SetHp(float hp) { hp_ = hp; }

    float GetMaxHp() { return max_hp_; }
    void SetMaxHp(float mhp) { max_hp_ = mhp; }

    bool GetInvisible() { return is_invisible_; }
    void SetInvisible(int Inv) { is_invisible_ = Inv; }

    VECTOR GetGoPosition() { return go_position_; }
    void SetGoPosition(VECTOR goplayer) { go_position_ = goplayer; }

    void SetInitPosition(VECTOR pos) { init_position_ = pos; }
    VECTOR GetInitPosition() { return init_position_; }

    float GetSize() { return size_; }

    void SetHitJudgmentFlagPlayer(bool flag) { is_hit_judgment_flag_player_ = flag; }
    bool IsHitJudgmentFlagPlayer() { return is_hit_judgment_flag_player_; }

    bool IsDead() { return is_dead_; }
    void SetIsDead(bool dead) { is_dead_ = dead; }

    bool IsHitSearchFlag() const { return is_hit_search_flag_; }

    // 入力: collider(自身の判定), check(相手の判定) / 出力: なし
    // 副作用: 索敵コライダーにプレイヤーが侵入した際、待機から追跡ステートへAIを切り替える
    virtual void OnEnter(Collider* collider, Collider* check) override;
    virtual void OnTrigger(Collider* collider, Collider* check) override;
    virtual void OnExit(Collider* collider, Collider* check) override;

protected:
    SphereCollider* attach_collider_;
    CapsuleCollider* capsule_collider_; // オブジェクト本体の物理的な衝突判定（壁抜け防止や被弾判定）
    SphereCollider* serch_collider_;    // プレイヤーを検知して追跡モードへ移行するための広域索敵判定
    SphereCollider* attack_collider_;   // プレイヤーがこの判定内に入ると攻撃アクションをトリガーする
    SphereCollider* stop_collider_;     // プレイヤーとの密着を防ぎ、適切な間合いを保つための停止判定
    Model* model_;
    Debug* debug_;

protected:
    float hp_;
    float max_hp_;
    float speed_;
    float normal_speed_;
    float max_speed_;
    float attack_;

    int walk_timer_;           // パトロール時の進行方向切り替えなどを計るためのタイマー
    int walk_count_;

    VECTOR hit_pos_ = VGet(0.0f, 0.0f, 0.0f);
    VECTOR go_position_;       // AIが現在向かおうとしているワールド空間上の目標座標
    VECTOR old_position_;
    VECTOR init_position_;     // ヘイトが切れた際に、元の位置へ帰還するためのスポーン座標
    VECTOR no_position_;
    VECTOR move_vec_;
    VECTOR up_move_vector_;
    VECTOR left_move_vector_;
    bool is_invisible_;        // 描画および当たり判定をスキップするための無敵/非表示フラグ
    int algorithm_id_;         // 待機、追跡、攻撃などのAIステートを切り替えるための状態ID
    int alg_hit_;

    int attack_interval_;
    int attack_count_;

    bool is_animation_;
    float target_angle_;       // 瞬時な振り向きを避け、滑らかに旋回するための目標角度
    float angle_;              // 現在のモデルのY軸回転角度（ラジアン）
    const float ROTATE_SPEED = 0.1f; // 旋回時の補間係数（大きすぎるとカクつき、小さすぎると追従が遅れる）

    float size_;
    float hit_search_;         // 索敵判定の半径（仕様制約に基づく）
    float hit_attack_search_;  // 攻撃トリガーとなる判定の半径
    float hit_stop_search_;    // 接近を停止する限界距離の半径
    bool is_dead_;

    bool is_hit_search_flag_;
    bool is_hit_attack_search_flag_;
    bool is_hit_search_stop_flag_;
    bool is_hit_attack_flag_;  // 現在攻撃モーション中であり、ダメージ判定を生成すべきかを示すフラグ

    int chance_;               // レアドロップなどのアイテムドロップ抽選に使用する確率値
    bool has_item_ = true;     // 死亡時に報酬としてアイテムをドロップするかを制御するフラグ

    bool is_attack_hit_judgment_flag_; // プレイヤーへの多段ヒットを防ぐため、1モーションにつき1回のダメージを保証するフラグ
    bool is_hit_judgment_flag_player_;

    int have_money_;           // 死亡時にプレイヤーに付与するスコア・通貨の量
};