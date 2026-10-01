#include "Texture.h"
#include "DxLib.h"



/// @brief Textureの初期化（コンストラクタ）
Texture::Texture(std::string filename, VECTOR centerPosition, int transFlag)
    : m_Handle(-1)
    , m_Position(centerPosition)
    , m_SizeX(0)
    , m_SizeY(0)
    , m_TransFlag(transFlag)
{
    // 画像の読み込み
    m_Handle = LoadGraph(filename.c_str());

    // 画像のサイズを取得する
    GetGraphSize(m_Handle, &m_SizeX, &m_SizeY);
}

Texture::~Texture()
{
    // 読みこんだ画像の破棄
    DeleteGraph(m_Handle);
}


/// @brief Textureの描画処理
void Texture::Draw()
{
    // 画像の表示
    DrawGraph(static_cast<int>(m_Position.x) - (m_SizeX / 2), static_cast<int>(m_Position.y) - (m_SizeY / 2), m_Handle, m_TransFlag);
}


/// @brief Textureの状態更新処理
void Texture::Update()
{
}
