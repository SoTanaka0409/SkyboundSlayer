#pragma once
#include "DxLib.h"
#include <string>

class TextureAnimation
{
public:
    /// @brief コンストラクタ
    TextureAnimation(
        VECTOR position,
        std::string filename,
        int allNum,
        int xNum,
        int yNum,
        int interval
    );
    /// @brief デストラクタ
    ~TextureAnimation();

    void Update();  // 更新
    void Draw();    // 描画

private:
    VECTOR m_Position;  // ポジション
    int m_Counter;      // アニメーションカウンタ
    int m_Interval;     // テクスチャ切り替えのフレーム数
    int m_CurrentNum;   // 何番目のテクスチャを表示するか
    int* m_HandleList;  // 分割されたテクスチャのハンドルリスト
    int m_AllNum;       // テクスチャ分割数
};