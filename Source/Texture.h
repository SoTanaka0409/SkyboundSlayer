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

    /// @brief セッター関数 //
    /// @brief ポジション設定
    void SetPosition(VECTOR centerPosition) { m_Position = centerPosition; }

    /// @brief ゲッター関数 //
    /// @brief ポジション取得
    VECTOR GetPosition() { return m_Position; }
    /// @brief サイズ取得
    int GetSizeX() { return m_SizeX; }
    int GetSizeY() { return m_SizeY; }

private:
    int m_Handle;       // 読み込んだ画像のハンドル
    VECTOR m_Position;  // ポジション
    int m_SizeX;        // 画像の幅
    int m_SizeY;        // 画像の高さ
    int m_TransFlag;    // 画像の透過を有効にするか
};

#endif