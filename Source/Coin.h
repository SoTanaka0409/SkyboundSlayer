#pragma once
#include "Object3D.h"
#include "Model.h"
#include <string>

// プレイヤーが回収可能な収集アイテム。出現時の跳ね上がり演出と、近接時の吸い寄せ挙動を管理する
class Coin : public Object3D
{
public:
    // 入力: filename(モデルパス), pos(初期座標), value(貨幣価値) / 出力: なし
    // 副作用: モデルリソースのロードと、跳ね上がり演出用の初期パラメータ設定
    Coin(std::string filename, VECTOR pos, int value);

    // 入力: なし / 出力: なし
    // 副作用: 描画用モデルインスタンスの破棄とメモリ解放
    ~Coin();

    // 入力: なし / 出力: なし
    // 副作用: プレイヤー吸い寄せや物理挙動の状態更新
    void Update();

    // 入力: なし / 出力: なし
    // 副作用: モデル描画命令のバッファ登録
    void Draw();

private:
    // 入力: なし / 出力: なし
    // 副作用: 生成直後の座標微調整による「跳ねる」挙動（演出上のカタルシス）の更新
    void UpdatePopPhysics();

    // 入力: なし / 出力: なし
    // 副作用: プレイヤーとの距離を判定し、範囲内であれば自動で吸い寄せる移動処理
    void UpdateSuckToPlayer();

    // === 設定定数 ===
    static constexpr float kSpawnOffsetY = 30.0f;     // 出現時の地面との接地位置オフセット
    static constexpr float kScale = 150.0f;           // モデルの表示スケール
    static constexpr int kPopDuration = 20;           // 出現時の跳ね上がり演出の持続フレーム数
    static constexpr float kPopSpeedY = 2.0f;         // 跳ね上がり時の初速
    static constexpr float kSuckRadius = 600.0f;      // 吸い寄せ処理を開始するプレイヤーからの距離
    static constexpr float kSuckSpeed = 30.0f;        // 吸い寄せ時の移動速度
    static constexpr float kCollectRadius = 80.0f;    // 回収判定を行う接触半径

    Model* model_;            // 描画用の3Dモデルインスタンス
    int value_;               // ショップ等で使用する貨幣価値
    bool is_sucking_;         // プレイヤーへの吸い寄せ状態フラグ（吸い寄せ中の挙動切り替えに使用）
    bool collected_;          // 破棄リクエスト用フラグ（Managerでの管理に使用）
    int age_;                 // 演出の終了判定や寿命管理用の経過フレーム
    float speed_;             // 物理挙動や吸い寄せ移動に使用する現在の移動速度
};