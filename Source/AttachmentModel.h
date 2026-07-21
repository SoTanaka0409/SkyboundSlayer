#pragma once
#include"DxLib.h"
#include"Object3D.h"

// 武器や装飾品などを、キャラクター等の特定のボーン（フレーム）に自動追従させるためのクラス
class AttachmentModel :public Object3D
{
public:
/// @param filename(パス), parentModelHandle(親モデル), parentFrameIndex(追従ボーン), offsetPos
/// @details 追従用モデルのVRAMロードと、親モデル同期用パラメータの初期設定
    AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex, VECTOR offsetPos = VGet(0.0f, 0.0f, 0.0f), VECTOR offsetRot = VGet(0.0f, 0.0f, 0.0f));

/// @details 動的確保したアタッチ用モデルリソースを破棄し、親モデル破棄時などのメモリリークを防ぐ
    ~AttachmentModel();

/// @details 親モデルの指定フレームの行列を取得し、オフセットを加味して自身の姿勢（座標・回転）を完全に同期する
    void Update() override;

/// @details 親に同期した最新のトランスフォーム情報を用いて、モデルを描画バッファへ登録する
    void Draw()override;

/// @return 自身のDxLibモデルハンドル
    // 外部からマテリアル変更やシェーダー適用などを直接行うために解放する（所有権は渡さない）
    int GetHandle() { return handle_; }

private:
    int handle_;             // アタッチされる側（武器等）の描画用DxLibモデルハンドル
    int parent_handle_;      // 座標同期の基準となる親モデル（キャラクター等）のハンドル
    int parent_frame_index_; // 追従先となる親モデル内の特定ボーン（手や背中など）のインデックス番号
    VECTOR offset_pos_;      // ボーンの原点とアタッチモデルの原点のズレを吸収するための位置補正値
    VECTOR offset_rot_;      // 持ち手の角度違いなどを吸収するための回転補正値
};