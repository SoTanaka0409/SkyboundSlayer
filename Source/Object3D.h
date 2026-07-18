#pragma once
#include "DxLib.h"
#include <string>

class Collider;

// 3D空間に配置される全オブジェクトの基底クラス。座標変換、コリジョン応答、および半透明描画のためのZソート機能を提供する
class Object3D
{
public:
    // オブジェクトの役割を定義するタグ。衝突判定時の対象フィルタリングや、マネージャーからの特定アクター検索に使用する
    enum Tag3D
    {
        None3D = 0,
        Tag3D_Player3D = 2000,
        Tag3D_Wall3D = 2100,
        Tag3D_Enemy3D = 2200,
        Tag3D_Stage = 2300,
        Tag3D_Bullet = 2400,
        Tag3D_Weapon = 2500,
        Tag_3D_Camera = 2700,
        Tag_3D_Shield = 2800,
        Tag3D_Obj = 2900,
        Tag3D_Object = 3050,
        Tag3D_Shop = 4000,
    };

    // オブジェクトが付与されているステータスバフ/デバフの状態を識別し、エフェクトやUI表示を分岐させるための列挙型
    enum StatusState
    {
        None_Status = 0,
        Status_Attack,
        Status_Speed,
        Status_Hp,
    } state;

public:
    // 入力: distance (カメラからの距離) / 出力: なし / 副作用: ソート用の基準値(current_camera_distance_)を更新する
    // 半透明オブジェクトの描画順序を制御し、アルファブレンドのZテスト破綻を防ぐために毎フレーム計算する
    void SetCameraDistance(float distance) { current_camera_distance_ = distance; }

    // std::sort等で使用する比較用関数オブジェクト。半透明の描画破綻を防ぐため、カメラから遠いオブジェクトを先に描画させる
    struct CompareZOrder {
        bool operator()(Object3D* a, Object3D* b) const {
            return a->current_camera_distance_ > b->current_camera_distance_;
        }
    };

public:
    // 入力: initPos (初期ワールド座標) / 出力: なし
    // 副作用: 座標やフラグ類を安全な初期値にセットアップする
    Object3D(VECTOR initPos);

    // 入力: なし / 出力: 派生クラスTへのキャストポインタ / 副作用: なし
    // 衝突検知時など、基底ポインタから派生クラス固有の関数（ダメージ処理等）を安全に呼び出すために使用する
    template <typename T>
    T* CastTo() {
        return dynamic_cast<T*>(this);
    }

    // 入力: なし / 出力: なし
    // 副作用: 派生クラスで動的確保されたリソースを安全に解放するため、仮想デストラクタとして定義する
    virtual ~Object3D();

    virtual void Update();
    virtual void Draw();

    // 入力: collider(自身の判定), check(相手の判定) / 出力: なし
    // 副作用: コリジョンへの侵入(Enter)、滞在(Trigger)、離脱(Exit)を検知し、派生クラスごとのダメージやギミック処理を発火させる
    virtual void OnEnter(Collider* collider, Collider* check);
    virtual void OnTrigger(Collider* collider, Collider* check);
    virtual void OnExit(Collider* collider, Collider* check);

    // 入力: 判定用カプセルとレイの上下端オフセット, 半径, 落下重力 / 出力: なし
    // 副作用: 3D地形モデル(Stage)に対するレイキャストを行い、接地している場合はキャラクターのY座標を地形にスナップさせる
    void TerrainFollow(float capsuleBottomY = -150.0f, float capsuleTopY = 150.0f, float capsuleRadius = 40.0f, float lineTopY = 1000.0f, float lineBottomY = -1000.0f, float gravity = 8.0f);

public:
    void SetPosition(VECTOR pos) { position_ = pos; };
    VECTOR GetPosition() { return position_; }

    void SetOldPosition(VECTOR pos) { old_position_ = pos; }
    VECTOR GetOldPosition() { return old_position_; }

    void SetRotation(VECTOR rot) { rotation_ = rot; }
    VECTOR GetRotation() { return rotation_; } // タイポ（GETRotation）を修正し命名規則を統一

    void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
    bool IsDeleteFlag() { return delete_flag_; }

    // 入力: flag (描画可否) / 出力: なし / 副作用: 内部の描画フラグ(draw_flag_)の更新
    // カメラの視錐台カリングや、透明化スキル発動時に、当たり判定や更新処理を残したまま描画だけをスキップする
    void SetDrawFlag(bool flag) { draw_flag_ = flag; }
    bool IsDrawFlag() { return draw_flag_; }

    void SetTag(Tag3D tag) { tag_ = tag; }
    Tag3D GetTag() { return tag_; }

protected:
    VECTOR position_;      // 3Dワールド空間上での現在座標
    VECTOR rotation_;      // オブジェクトの回転姿勢（ラジアン）
    VECTOR old_position_;  // 前フレームの座標。壁との衝突判定時の押し出し（めり込み解消）計算などに使用する

private:
    bool delete_flag_;               // true時、オブジェクトマネージャーの終端処理にて安全にメモリから解放される
    Tag3D tag_;                      // 衝突相手の判別やオブジェクトのグルーピングに使用する識別用タグ
    bool draw_flag_;                 // 描画パイプラインへ送るかどうかの制御フラグ（表示/非表示の切り替え）
    float current_camera_distance_;  // Zソート計算用に毎フレーム更新される、アクティブカメラからの直線距離
};