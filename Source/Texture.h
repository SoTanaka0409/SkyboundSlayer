#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "DxLib.h"

class Texture
{
public:
    Texture(std::string filename, VECTOR centerPosition, int transFlag);  // コンストラクタ

    ~Texture(); // デストラクタ

    void Draw();    // 描画
    void Update();  // 更新

    // セッター関数 //
    // ポジション設定
    void SetPosition(VECTOR centerPosition) { position_ = centerPosition; }

    // ゲッター関数 //
    // ポジション取得
    VECTOR GetPosition() { return position_; }
    // サイズ取得
    int GetSizeX() { return mnSizeX; }
    int GetSizeY() { return mnSizeY; }

private:
    int mnHandle;       // 読み込んだ画像のハンドル
    VECTOR position_;  // ポジション
    int mnSizeX;        // 画像の幅
    int mnSizeY;        // 画像の高さ
    int mnTransFlag;    // 画像の透過を有効にするか
};

#endif