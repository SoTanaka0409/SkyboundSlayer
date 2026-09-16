#pragma once

#include"Dxlib.h"
#include"Object3D.h"

#include"DrawHp.h"
#include"Debug.h"
#include"EnemyManager.h"
#include"Enemy.h"
#include"Collision.h"

class Model;

// プレイヤーの進行を阻む中ボス/大ボス用クラス。コンボ攻撃やジャンプなどの複雑な行動パターンを単独で管理する
class EnemyBoss_1 : public Enemy
{
public:
/// @param filename, 初期座標, ステータス群, 判定サイズ群, 所持金, アニメ分離フラグ
/// @details ボス専用の巨大なモデルや、ジャンプ攻撃用の特殊コライダーを動的確保して初期化する
    EnemyBoss_1(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_);

/// @details ボス固有の攻撃用コライダー（ジャンプ衝撃波など）を破棄し、シーン離脱時のメモリリークを防ぐ
    ~EnemyBoss_1();

/// @details モデル本体の描画に加え、画面上部に固定表示されるボス専用の長大なHPバーUIを描画バッファへ登録する
    void Draw() override;

/// @details HP残量（フェーズ）に応じた行動パターンの切り替えや、ジャンプ中の物理演算を毎フレーム進行させる
    void Update() override;

/// @details プレイヤーとの距離や乱数に基づき、通常コンボやジャンプ急降下などの攻撃モーションを動的に出し分ける
    void Attack() override;

/// @param collider(自身の判定), check(相手の判定)
/// @details プレイヤーの攻撃に対する怯み耐性（スーパーアーマー）や、部位破壊などのボス特有の接触判定を処理する
    void OnTrigger(Collider* collider, Collider* check) override;

/// @details 即座に消滅させず、スローモーションや爆発エフェクトなどの長尺な「ボス撃破演出」のトリガーを発火させる
    void DeathEnemy() override;

/// @details 撃破演出が完全に終了した後、安全にインスタンスを破棄してゲームクリアフラグ等へ繋ぐ
    void Delete() override;

private:
/// @details 共通の重力処理を無視し、ボス特有の「溜めからの急降下」を実現するためY軸座標を直接上書きする
    void UpdateJumpPhysics();

    // === 定数 ===
    static constexpr float kJumpAscendSpeed = 15.0f;  // ジャンプ攻撃時の浮上初速（プレイヤーの視界から消える高さ）
    static constexpr float kJumpDescendSpeed = -25.0f; // 回避の猶予を削るための急降下落下速度

    SphereCollider* jump_attack_coiider_; // 着地時に広範囲へ広がる衝撃波のダメージ判定用コライダー

    int attack_type_;         // 乱数やヘイトに応じて分岐する、現在実行中の攻撃アクションID
    int attack1_combo_count_; // 連続攻撃の段数（コンボルートの派生判定に使用）

    float mfjumpPower;        // 現在のY軸方向への推進力（滞空時間の計算用）
    bool HighPositionFlag;    // ジャンプの頂点に達し、急降下ステートへ移行すべきかを判定するフラグ
    VECTOR jump_target_dir_;  // ジャンプ時の移動方向ベクトル
    float forward_speed_;     // ジャンプ時の前進速度
    int jump_charge_timer_;   // ジャンプのタメ時間計測用
    float jump_velocity_;     // 現在のジャンプ速度
    float gravity_;           // 重力加速度
};