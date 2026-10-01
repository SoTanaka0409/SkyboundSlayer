#include "TextureAnimation.h"

/// @brief コンストラクタ
TextureAnimation::TextureAnimation(
    VECTOR position,
    std::string filename,
    int allNum,
    int xNum,
    int yNum,
    int interval
)
: m_Position(position)
, m_Counter(0)
, m_Interval(interval)
, m_CurrentNum(0)
, m_AllNum(allNum)
{
    m_HandleList = new int[allNum];

    // 画像ファイル読み込み
    int handle = LoadGraph(filename.c_str());
    if (handle == -1)
    {
        return; // 読み込み失敗していたら以降は処理しない
    }

    // サイズ取得
    int sizeX, sizeY;
    GetGraphSize(handle, &sizeX, &sizeY);

    // テクスチャの分割読み込み
    int success = LoadDivGraph(
        filename.c_str(),
        allNum,
        xNum,
        yNum,
        sizeX / xNum,
        sizeY / yNum,
        m_HandleList
    );
}

/// @brief デストラクタ
TextureAnimation::~TextureAnimation()
{
}

void TextureAnimation::Update()
{
    // カウンタをインクリメント
    m_Counter++;
    if (m_Counter % m_Interval == 0)
    {
        m_Counter = 0;  // カウンタを戻す
        m_CurrentNum++; // テクスチャ番号を進める
        if (m_CurrentNum >= m_AllNum)   // 分割数を超えるならループさせる
        {
            m_CurrentNum = 0;   // ループさせる
        }
    }
}

void TextureAnimation::Draw()
{
    DrawGraph(static_cast<int>(m_Position.x), static_cast<int>(m_Position.y), m_HandleList[m_CurrentNum], true);
}

