#pragma once
#include "Model.h"
#include "Object3D.h"

// ステージ上に配置される、破壊可能なオブジェクトやインタラクティブな小道具（箱、壺など）を管理するクラス
class StageObject : public Object3D
{
public:
/// @param 初期座標, モデルパス, スケール, テクスチャパス, 判定半径, Yオフセット
/// @details モデルをロードし、必要に応じて接地判定用のオフセットを保持する
    StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename = "", float hitRadius = 0.0f, float yOffset = 0.0f);

/// @return 衝突判定半径(float)
    float GetHitRadius() const { return hit_radius_; }

/// @return 衝突判定が有効か(bool)
    bool IsHitEnabled() const { return hit_radius_ > 0.0f; }

/// @details 保持しているモデルリソースを破棄し、メモリリークを防ぐ
    ~StageObject();

/// @details 落下や揺れなど、オブジェクト固有の挙動を毎フレーム進行させる
    void Update();

/// @details ワールド座標へモデルを描画バッファへ登録する
    void Draw();

private:
    Model* model_;              // 表示用3Dモデルの管理インスタンス
    float scale_;               // 描画および当たり判定に適用する倍率
    float hit_radius_;          // プレイヤーや敵との接触判定を行う球状コライダーの半径
    bool has_followed_terrain_; // 初期配置時に地形接地処理(TerrainFollow)が完了したかを示すフラグ
    float y_offset_;            // 地形に対してモデルを少し浮かせる（または埋める）ための調整値
};