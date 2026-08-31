#pragma once
#include "DxLib.h"
#include <string>

class TextureAnimation
{
public:
    // コンストラクタ
    TextureAnimation(
        VECTOR position,
        std::string filename,
        int allNum,
        int xNum,
        int yNum,
        int interval
    );
    // デストラクタ
    ~TextureAnimation();

    void Update();  // 更新
    void Draw();    // 描画

private:
    VECTOR position_;  // ポジション
    int counter_;      // アニメーションカウンタ
    int interval_;     // テクスチャ切り替えのフレーム数
    int current_num_;   // 何番目のテクスチャを表示するか
    int* handle_list_;  // 分割されたテクスチャのハンドルリスト
    int all_num_;       // テクスチャ分割数
};