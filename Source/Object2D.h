#pragma once
#include <string>
#include <vector>
#include "DxLib.h"

/// @brief 画面に表示されるすべての2D要素（UI、HUD、2Dエフェクト等）の共通属性とインターフェースを定義する基底クラス
class Object2D
{
public:
    /// @brief 2Dオブジェクトの種類を識別するためのタグ。オブジェクトマネージャー等での一括処理や衝突判定の識別に用いる
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
    void SetPosition(VECTOR pos) { m_Position = pos; }
    VECTOR GetPosition() { return m_Position; }

    void SetRotation(VECTOR rot) { m_Rotation = rot; }
    VECTOR GetRotation() { return m_Rotation; } // 大文字のタイポを修正して命名規則を一貫化

    void SetDeleteFlag(bool flag) { m_DeleteFlag = flag; }
    bool IsDeleteFlag() { return m_DeleteFlag; }

/// @param flag (描画可否)
/// @details 内部の描画フラグ(m_DrawFlag)の更新
    /// @brief メニューの開閉やUIの非表示切り替え時、Update処理を維持したまま描画だけをスキップさせる制御に使用する
    void SetDrawFlag(bool flag) { m_DrawFlag = flag; }
    bool IsDrawFlag() { return m_DrawFlag; }

    void SetTag(Tag2D tag) { m_Tag = tag; }
    Tag2D GetTag() { return m_Tag; }

protected:
    VECTOR m_Position;  // 2Dスクリーン空間上での配置座標（Z成分は描画順のプライオリティ等に転用可能）
    VECTOR m_Rotation;  // 2D平面上での回転角度（ラジアン）

private:
    bool m_DeleteFlag; // trueのとき、管理マネージャーのクリーンアップ処理によって安全にメモリから解放される
    Tag2D m_Tag;        // オブジェクトの役割（背景、ボタン等）を動的に判別するための識別用タグ
    bool m_DrawFlag;   // 画面への描画を行うかどうかの判定フラグ（非表示状態の制御に使用）
};