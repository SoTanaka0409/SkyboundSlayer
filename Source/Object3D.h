#pragma once
#include "DxLib.h"
#include <string>

class Collider;

/// @brief 3D空間に配置される全オブジェクトの基底クラス。座標変換、コリジョン応答、および半透明描画のためのZソート機能を提供する
class Object3D
{
public:
    /// @brief オブジェクトの役割を定義するタグ。衝突判定時の対象フィルタリングや、マネージャーからの特定アクター検索に使用する
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

    /// @brief オブジェクトが付与されているステータスバフ/デバフの状態を識別し、エフェクトやUI表示を分岐させるための列挙型
    enum StatusState
    {
        None_Status = 0,
        Status_Attack,
        Status_Speed,
        Status_Hp,
    } state;

public:
/// @param distance (カメラからの距離)
/// @details ソート用の基準値(m_CurrentCameraDistance)を更新する
    /// @brief 半透明オブジェクトの描画順序を制御し、アルファブレンドのZテスト破綻を防ぐために毎フレーム計算する
    void SetCameraDistance(float distance) { m_CurrentCameraDistance = distance; }

    /// @brief std::sort等で使用する比較用関数オブジェクト。半透明の描画破綻を防ぐため、カメラから遠いオブジェクトを先に描画させる
    struct CompareZOrder {
        bool operator()(Object3D* a, Object3D* b) const {
            return a->m_CurrentCameraDistance > b->m_CurrentCameraDistance;
        }
    };

public:
/// @param initPos (初期ワールド座標)
/// @details 座標やフラグ類を安全な初期値にセットアップする
    Object3D(VECTOR initPos);

/// @return 派生クラスTへのキャストポインタ
    /// @brief 衝突検知時など、基底ポインタから派生クラス固有の関数（ダメージ処理等）を安全に呼び出すために使用する
    template <typename T>
    T* CastTo() {
        return dynamic_cast<T*>(this);
    }

/// @details 派生クラスで動的確保されたリソースを安全に解放するため、仮想デストラクタとして定義する
    virtual ~Object3D();

    virtual void Update();
    virtual void Draw();

/// @param collider(自身の判定), check(相手の判定)
/// @details コリジョンへの侵入(Enter)、滞在(Trigger)、離脱(Exit)を検知し、派生クラスごとのダメージやギミック処理を発火させる
    virtual void OnEnter(Collider* collider, Collider* check);
    virtual void OnTrigger(Collider* collider, Collider* check);
    virtual void OnExit(Collider* collider, Collider* check);

/// @param 判定用カプセルとレイの上下端オフセット, 半径, 落下重力
/// @details 3D地形モデル(Stage)に対するレイキャストを行い、接地している場合はキャラクターのY座標を地形にスナップさせる
    void TerrainFollow(float capsuleBottomY = -150.0f, float capsuleTopY = 150.0f, float capsuleRadius = 40.0f, float lineTopY = 1000.0f, float lineBottomY = -1000.0f, float gravity = 8.0f);

public:
    void SetPosition(VECTOR pos) { m_Position = pos; };
    VECTOR GetPosition() { return m_Position; }

    void SetOldPosition(VECTOR pos) { m_OldPosition = pos; }
    VECTOR GetOldPosition() { return m_OldPosition; }

    void SetRotation(VECTOR rot) { m_Rotation = rot; }
    VECTOR GetRotation() { return m_Rotation; } // タイポ（GETRotation）を修正し命名規則を統一

    void SetDeleteFlag(bool flag) { m_DeleteFlag = flag; }
    bool IsDeleteFlag() { return m_DeleteFlag; }

/// @param flag (描画可否)
/// @details 内部の描画フラグ(m_DrawFlag)の更新
    /// @brief カメラの視錐台カリングや、透明化スキル発動時に、当たり判定や更新処理を残したまま描画だけをスキップする
    void SetDrawFlag(bool flag) { m_DrawFlag = flag; }
    bool IsDrawFlag() { return m_DrawFlag; }

    void SetTag(Tag3D tag) { m_Tag = tag; }
    Tag3D GetTag() { return m_Tag; }

protected:
    VECTOR m_Position;      // 3Dワールド空間上での現在座標
    VECTOR m_Rotation;      // オブジェクトの回転姿勢（ラジアン）
    VECTOR m_OldPosition;  // 前フレームの座標。壁との衝突判定時の押し出し（めり込み解消）計算などに使用する

private:
    bool m_DeleteFlag;               // true時、オブジェクトマネージャーの終端処理にて安全にメモリから解放される
    Tag3D m_Tag;                      // 衝突相手の判別やオブジェクトのグルーピングに使用する識別用タグ
    bool m_DrawFlag;                 // 描画パイプラインへ送るかどうかの制御フラグ（表示/非表示の切り替え）
    float m_CurrentCameraDistance;  // Zソート計算用に毎フレーム更新される、アクティブカメラからの直線距離
};