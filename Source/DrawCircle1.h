#pragma once
#include "Object3D.h"
#include "DxLib.h"
#include <vector>

// 攻撃範囲の予告など、指定座標を中心に拡がる3D円形エフェクトを描画・管理するクラス
class DrawCircle1 : public Object3D
{
public:
    // 入力: filename(テクスチャパス), centerPos(円の中心座標) / 出力: なし
    // 副作用: エフェクト用画像ハンドルのVRAMロードおよび、円の初期サイズ・最大半径の設定
    DrawCircle1(std::string filename, VECTOR centerPos);

    // 入力: なし / 出力: なし
    // 副作用: 動的確保した画像ハンドルの破棄（シーン遷移時のメモリリーク防止）
    ~DrawCircle1();

    // 入力: なし / 出力: なし
    // 副作用: Maxradius（最大半径）に達するまでradiusを拡張させるアニメーション更新
    void Update() override;

    // 入力: なし / 出力: なし
    // 副作用: div(分割数)と現在のradiusに基づき頂点群を動的計算し、3D空間に円形ポリゴンを描画する
    void Draw() override;

    // 入力: なし / 出力: 基準となる3頂点の動的配列 / 副作用: なし
    // 外部から円の基本姿勢や、簡易的な当たり判定の基準となるポリゴン面を取得するために提供する
    std::vector<VERTEX3D> GetVertex()
    {
        std::vector<VERTEX3D> result;
        result.push_back(vertex_[0]);
        result.push_back(vertex_[1]);
        result.push_back(vertex_[2]);
        return result;
    }

private:
    int graph_handle_;         // 面に貼り付けるテクスチャハンドル
    VERTEX3D vertex_[3];       // 円の姿勢や基準面を定義するための3頂点（配列外アクセス防止のため要素数に準拠）
    float radius;              // 現在の描画半径（Updateで毎フレーム拡張される）
    float Maxradius;           // 拡張アニメーションの限界値（演出の終了判定に使用）
    const int div = 48;        // 円の滑らかさと頂点計算負荷のバランスを取るための固定分割数
    std::vector<VERTEX3D> vtx; // 毎フレーム計算される描画用の動的頂点バッファ
    int color = GetColor(255, 0, 0); // 円のベースカラー（デフォルトは警告用の赤）
    VECTOR center_;            // 円展開の基準となる3Dワールド座標
    VECTOR OldPosition;        // 前フレームの座標（追従処理や移動差分計算に使用）
};