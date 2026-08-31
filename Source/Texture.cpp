#include "Texture.h"
#include "DxLib.h"



/// @brief Textureの初期化（コンストラクタ）
Texture::Texture(std::string filename, VECTOR centerPosition, int transFlag)
    : handle_(-1)
    , position_(centerPosition)
    , size_x_(0)
    , size_y_(0)
    , trans_flag_(transFlag)
{
    // 画像の読み込み
    handle_ = LoadGraph(filename.c_str());

    // 画像のサイズを取得する
    GetGraphSize(handle_, &size_x_, &size_y_);
}

Texture::~Texture()
{
    // 読みこんだ画像の破棄
    DeleteGraph(handle_);
}


/// @brief Textureの描画処理
void Texture::Draw()
{
    // 画像の表示
    DrawGraph(static_cast<int>(position_.x) - (size_x_ / 2), static_cast<int>(position_.y) - (size_y_ / 2), handle_, trans_flag_);
}


/// @brief Textureの状態更新処理
void Texture::Update()
{
}
