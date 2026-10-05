#include"Chat.h"
#include"InputManager.h"
#include"Master.h"



/// @brief Chatの初期化（コンストラクタ）
Chat::Chat()
    :first_font_(0)
{
    texture_= new Texture("Resource/image/UI/01_dialog.png", VGet(500, 200, 0), true);
}

Chat::~Chat()
{

}


/// @brief Chatの描画処理
void Chat::Draw(std::string chatdate)
{
    static int font24 = -1;
    if (font24 == -1) font24 = CreateFontToHandle("源界明朝", 24, -1);

    if (Master::is_stat_shop_on_) { return; }
    
    // 背景テクスチャの上に半透明の黒帯を敷いて文字を読みやすくする
    texture_->Draw();
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(280, 180, 700, 240, GetColor(0, 0, 0), true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    
    DrawFormatStringToHandle(300, 200, GetColor(255, 255, 255), font24, "%s", chatdate.c_str());
}

/// @brief ChatのReset処理
void Chat::Reset()
{
}
