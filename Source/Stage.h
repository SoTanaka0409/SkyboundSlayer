#pragma once
#include "Object3D.h"
#include <string>
#include "Debug.h"

// ゲームの足場や壁となるステージ地形を管理するクラス。描画用モデルと物理演算用コライダーモデルを分離して保持する
class Stage : public Object3D
{
public:
/// @param 初期座標, 描画モデルパス, コリジョンモデルパス, スケール, テクスチャパス
/// @details 描画用と物理判定用に二つの3Dモデルをロードし、それぞれハンドルを保持する
    Stage(VECTOR initPos, std::string StageModelName, std::string stageCollsionModelName, VECTOR scale = VGet(-1.0f, -1.0f, -1.0f), std::string textureFilename = "");

/// @details ロードした二つの3Dモデルハンドルを破棄し、シーン終了時のメモリリークを防ぐ
    ~Stage();

/// @details ステージ固有の動的演出があれば更新する（基本は静的な地形のため空実装）
    void Update() override;

/// @details 描画用モデルをワールド座標に配置して描画バッファへ登録する
    void Draw() override;

/// @param pos1, pos2 (カプセルの両端点), r (半径)
/// @return 衝突したか(bool)
    bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

/// @param pos1, pos2 (線の始点と終点)
/// @return 衝突座標(VECTOR)
    VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);

/// @param pos1, pos2
/// @return 衝突座標(VECTOR)
/// @details 判定結果をデバッグ表示し、レイがどの位置で地形に阻まれたかを可視化する（開発用）
    VECTOR CheckHit_LineDebug(VECTOR pos1, VECTOR pos2);

private:
    Debug* debug_;
    int model_handle_;     // 画面に描画される高品質な地形3Dモデルのハンドル
    int collision_handle_; // 物理計算専用の軽量化されたコリジョンモデルのハンドル（描画は行わない）
};