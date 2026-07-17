#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "DxLib.h"

class Texture
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    Texture(std::string filename, VECTOR centerPosition, int transFlag);  // コンストラクタ

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    ~Texture(); // デストラクタ

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void Draw();    // 描画
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void Update();  // 更新

    // セッター関数 //
    // ポジション設定
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetPosition(VECTOR centerPosition) { position_ = centerPosition; }

    // ゲッター関数 //
    // ポジション取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    VECTOR GetPosition() { return position_; }
    // サイズ取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    int GetSizeX() { return size_x_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    int GetSizeY() { return size_y_; }

private:
    int handle_;       // 読み込んだ画像のハンドル
    VECTOR position_;  // ポジション
    int size_x_;        // 画像の幅
    int size_y_;        // 画像の高さ
    int trans_flag_;    // 画像の透過を有効にするか
};

#endif