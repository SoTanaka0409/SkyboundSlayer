#pragma once
#include <string>
#include <vector>
#include "DxLib.h"

// 画面に表示されるすべての2D要素（UI、HUD、2Dエフェクト等）の共通属性とインターフェースを定義する基底クラス
class Object2D
{
public:
    // 2Dオブジェクトの種類を識別するためのタグ。オブジェクトマネージャー等での一括処理や衝突判定の識別に用いる
    enum Tag2D
    {
        None2D = 0,
    };

public:
/// @param initPos (初期のスクリーン座標)
/// @details 2D座標、描画フラグ、生存フラグなどの内部状態を安全な初期値にセットアップする
    Object2D(VECTOR initPos);

/// @details 派生クラスで動的確保されたリソースを安全に解放するため、仮想デストラクタとして定義する
    virtual ~Object2D();

/// @details 派生クラスのUIアニメーションや位置計算を毎フレーム進行させるための仮想更新メソッド
    virtual void Update();

/// @details 現在の座標・回転状態を適用し、2Dスプライトやテキストを描画バッファへ登録するための仮想描画メソッド
    virtual void Draw();

public: // ゲッター、セッター
    void SetPosition(VECTOR pos) { position_ = pos; }
    VECTOR GetPosition() { return position_; }

    void SetRotation(VECTOR rot) { rotation_ = rot; }
    VECTOR GetRotation() { return rotation_; } // 大文字のタイポを修正して命名規則を一貫化

    void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
    bool IsDeleteFlag() { return delete_flag_; }

/// @param flag (描画可否)
/// @details 内部の描画フラグ(draw_flag_)の更新
    // メニューの開閉やUIの非表示切り替え時、Update処理を維持したまま描画だけをスキップさせる制御に使用する
    void SetDrawFlag(bool flag) { draw_flag_ = flag; }
    bool IsDrawFlag() { return draw_flag_; }

    void SetTag(Tag2D tag) { tag_ = tag; }
    Tag2D GetTag() { return tag_; }

protected:
    VECTOR position_;  // 2Dスクリーン空間上での配置座標（Z成分は描画順のプライオリティ等に転用可能）
    VECTOR rotation_;  // 2D平面上での回転角度（ラジアン）

private:
    bool delete_flag_; // trueのとき、管理マネージャーのクリーンアップ処理によって安全にメモリから解放される
    Tag2D tag_;        // オブジェクトの役割（背景、ボタン等）を動的に判別するための識別用タグ
    bool draw_flag_;   // 画面への描画を行うかどうかの判定フラグ（非表示状態の制御に使用）
};