#pragma once
#include"dxlib.h"
#include"Collider.h"
#include"ColliderManager.h"
#include"Object3D.h"
#include"Model.h"

class SphereCollider;
class CapsuleCollider;

// プレイヤーや敵が放つ魔法（飛び道具）の基底クラス。軌道計算や寿命による自動消滅を管理する
class Magic : public Object3D
{
public:
/// @param filename, initPos(初期座標), r(判定半径), damage(攻撃力), speed, movevec(進行方向), count(初期時間), time(寿命)
/// @details 魔法の視覚リソースをロードし、空間に対して着弾判定用のSphereColliderを動的配置する
    Magic(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time);

/// @details 動的確保したコライダーやグラフィックハンドルを破棄し、弾幕展開時のメモリリークを確実に防ぐ
    ~Magic();

/// @details 生存タイマーを進行させ、寿命（DeleteTime）超過時にDeath()を呼び出して自身を破棄予約状態にする
    void Update();

/// @details 最新のワールド座標に追従させて、魔法のエフェクト画像（またはモデル）を描画バッファへ登録する
    void Draw();

/// @details 進行方向(move_vec_)と速度(speed_)に基づき座標を更新する（派生クラスでホーミング等の軌道計算を上書き想定）
    virtual void Move();

/// @details 着弾エフェクトの生成トリガーを発火し、マネージャーに対して自身の安全な破棄リクエスト(delete_)を送信する
    virtual void Death();

/// @param collider(自身の判定), check(相手の判定)
/// @details 敵や地形への着弾を検知し、対象へのダメージ適用と自身の消滅フラグを立てる
    virtual void OnEnter(Collider* collider, Collider* check) override;

/// @param collider(自身の判定), check(相手の判定)
/// @details 貫通弾など、接触中の継続ダメージ処理が必要な場合に使用する（基本は空実装）
    virtual void OnTrigger(Collider* collider, Collider* check) override;

/// @param collider(自身の判定), check(相手の判定)
/// @details 判定から抜けた際の特殊処理（基本は空実装）
    virtual void OnExit(Collider* collider, Collider* check) override;

protected:
    SphereCollider* hit_collider_; // 敵や壁への着弾を検知するため、魔法本体に追従させる球状の当たり判定
    int graph_handle_;             // 魔法の視覚表現（弾やエフェクト）を描画するためのグラフィックハンドル
    std::string Filename;          // 同一リソースの再ロードを防ぐキャッシュ検索や、デバッグ表示に用いるファイルパス
    float speed_;                  // 毎フレームの移動量（ホーミング等で動的に変動させるため変数として保持）
    float mfAttack_chara;          // 術者の基礎攻撃力（バフ等による変動を魔法生成後に再計算するためのベース値）
    float attack_;                 // 最終的に対象へ与える算出済みのダメージ量
    float magic_size_;             // 描画スケールおよび当たり判定の半径に適用する空間サイズ係数
    VECTOR move_vec_;              // 魔法が飛んでいく方向ベクトル（正規化済みを想定）
    int DeleteCount;               // 生成されてからの経過フレーム数（寿命判定に使用）
    int DeleteTime;                // 画面外へ飛んだ弾がメモリを圧迫し続けるのを防ぐための、最大生存フレーム数（仕様制約）
    bool delete_;                  // true時、次フレームのManager更新処理にてインスタンスが安全に破棄される（即時delete回避用）
};