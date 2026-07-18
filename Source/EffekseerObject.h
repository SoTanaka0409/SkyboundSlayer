#pragma once
#include "Object3D.h"
#include <string>

// エフェクトの再生状態をゲームオブジェクトとして扱い、キャラクターや武器への動的な追従を管理するクラス
class EffekseerObject : public Object3D
{
public:
    // 入力: name(登録名), filepath, initPos(初期座標), parent(追従先), isFollow, magnification(倍率), speed
    // 出力: なし / 副作用: エフェクトのロードおよび再生を開始し、親からの相対座標（オフセット）を計算・保持する
    EffekseerObject(const std::string& name, const char* filepath, VECTOR initPos, Object3D* parent = nullptr, bool isFollow = false, float magnification = 1.0f, float speed = 1.0f);

    // 入力: なし / 出力: なし
    // 副作用: オブジェクト破棄時に再生中のエフェクトを強制停止し、画面にエフェクトが残り続けるバグを防ぐ
    virtual ~EffekseerObject();

    // 入力: なし / 出力: なし
    // 副作用: 追従フラグ有効時、親の最新座標にオフセットを加算してエフェクトの再生座標を毎フレーム上書きする
    void Update() override;

    // 入力: なし / 出力: なし
    // 副作用: 描画はマネージャー側(EffekseerManager)で一括処理される仕様のため、二重描画を防ぐべく空実装とする
    void Draw() override;

private:
    int playing_handle_; // 座標の追従更新や、生存期間終了時の破棄処理を行うために保持する再生固有ハンドル
    Object3D* parent_;   // 座標同期の基準となる親オブジェクト（親破棄後のダングリングポインタ参照に注意）
    bool is_follow_;     // 座標を出現位置で固定するか、親の移動に合わせて毎フレーム追従計算を行うかの制御フラグ
    VECTOR offset_;      // 親の原点とエフェクト発生位置（剣の切っ先など）のズレを一定に保つための初期相対座標
};