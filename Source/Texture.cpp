#include "Texture.h"
#include "DxLib.h"


Texture::Texture(std::string filename, VECTOR centerPosition, int transFlag)
    : mnHandle(-1)
    , mvPosition(centerPosition)
    , mnSizeX(0)
    , mnSizeY(0)
    , mnTransFlag(transFlag)
{
    // 画像の読み込み
    mnHandle = LoadGraph(filename.c_str());

    // 画像のサイズを取得する
    GetGraphSize(mnHandle, &mnSizeX, &mnSizeY);
}

Texture::~Texture()
{
    // 読みこんだ画像の破棄
    DeleteGraph(mnHandle);
}

void Texture::Draw()
{
    // 画像の表示
    DrawGraph(mvPosition.x - (mnSizeX / 2), mvPosition.y - (mnSizeY / 2), mnHandle, mnTransFlag);
}

void Texture::Update()
{
}