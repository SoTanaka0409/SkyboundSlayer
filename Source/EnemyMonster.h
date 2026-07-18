#pragma once

#include "Dxlib.h"
#include "Enemy.h"
#include "SphereCollider.h"

// 溜め動作からの跳躍・押し潰し攻撃（ジャンプ攻撃）を主体とする、重量級エネミーの派生クラス
class EnemyMonster : public Enemy
{
public:
    enum class AttackState {
        None,
        Charging,
        Jumping,
        Landing
    };

    // 入力: filename, 初期座標, ステータス群, 判定サイズ群, 所持金, アニメ分離フラグ
    // 出力: なし / 副作用: 跳躍攻撃専用のステートマシン初期化と、着地衝撃波用コライダーの確保
    EnemyMonster(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_);

    // 入力: なし / 出力: なし
    // 副作用: 専用で動的確保した着地攻撃用コライダー（landing_attack_collider_）を確実に破棄する
    ~EnemyMonster();

    // 入力: なし / 出力: なし
    // 副作用: AttackStateに基づくジャンプ攻撃の進行（溜め→跳躍→着地）と、専用の物理挙動を毎フレーム更新する
    void Update() override;

    // 入力: なし / 出力: なし
    // 副作用: 自身のモデル描画に加え、ジャンプ攻撃時の予兆（足元の影の拡大やオーラ等）を描画バッファへ登録する
    void Draw() override;

    // 入力: なし / 出力: なし
    // 副作用: プレイヤーとの距離を判定し、条件を満たした場合に跳躍攻撃の「溜め（Charging）」へステートを移行させる
    void Attack() override;

    // 入力: collider(自身の判定), check(相手の判定) / 出力: なし
    // 副作用: 跳躍中（Jumping）の無敵判定処理や、着地硬直中の被ダメージボーナスなど特殊な接触判定を処理する
    void OnTrigger(Collider* collider, Collider* check) override;

    // 入力: なし / 出力: なし
    // 副作用: 死亡アニメーション（巨体が倒れ込む等）の再生を開始し、進行中のジャンプ攻撃ステートを強制キャンセルする
    void DeathEnemy() override;

    // 入力: なし / 出力: なし
    // 副作用: 死亡演出の終了後、安全にインスタンスを破棄してメモリ管理マネージャーへ通知する
    void Delete() override;

private:
    // 入力: なし / 出力: なし / 副作用: 索敵状態の更新と、射程内に入った際の攻撃ステート移行発火
    void UpdateAttackIdle();
    // 入力: なし / 出力: なし / 副作用: プレイヤーに回避の猶予を与える溜め演出を進行させ、満了時に跳躍を開始する
    void UpdateAttackCharging();
    // 入力: なし / 出力: なし / 副作用: 専用の重力計算により放物線軌道で座標を更新し、着地判定を監視する
    void UpdateAttackJumping();
    // 入力: なし / 出力: なし / 副作用: 着地時の衝撃波コライダーを有効化し、一定の硬直時間経過後にIdleへ戻す
    void UpdateAttackLanding();
    // 入力: なし / 出力: 射程内か否か(bool) / 副作用: なし
    bool IsPlayerInJumpRange() const;
    // 入力: なし / 出力: なし / 副作用: 踏み切り時にターゲットへの方向ベクトルを算出し、空中でのホーミングを制限する
    void SetJumpDirectionToPlayer();
    // 入力: なし / 出力: なし / 副作用: ジャンプ用の初速や多段ヒット防止フラグを初期化し、物理挙動を開始する
    void StartJumpAttack();

    AttackState attack_state_;       // ジャンプ攻撃の一連のプロセス（溜め・跳躍・着地）を進行させるための内部ステート
    int charge_timer_;               // 攻撃前の隙（溜め時間）を計測し、プレイヤーに予兆を見せるためのタイマー
    int jump_timer_;                 // 跳躍時のモーション管理や、異常な滞空（地形引っ掛かり）を検知して強制落下させるためのタイマー

    SphereCollider* landing_attack_collider_; // 着地時に一瞬だけ発生する、広範囲の衝撃波ダメージ用判定
    bool has_landed_hit_;            // 着地判定の多段ヒットを防ぐため、1回のジャンプにつき1ダメージを保証するフラグ

    float jump_velocity_;            // 現在のY軸方向への速度（上方向への初速から重力で減衰していく）
    float gravity_;                  // 空中での落下加速度（通常の敵より重い挙動にするための専用値）
    float forward_speed_;            // 跳躍中の水平方向（プレイヤーへの接近）の移動速度
    VECTOR jump_target_dir_;         // 踏み切り時点で決定された進行方向（空中でプレイヤーを追尾し続けないように保持）
    float jump_start_y_;             // 着地判定（Y座標の比較）を正確に行うため、跳躍開始時の地面の高さ（Y座標）を記録する
};