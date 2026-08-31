#include"Chat.h"
#include"InputManager.h"
#include"Master.h"



/// @brief Chatの初期化（コンストラクタ）
Chat::Chat()
    :first_font_(0)
{
    texture_= new Texture("Resource/画像/UI/01_会話ウィンドウ画像.png", VGet(500, 200, 0), true);
}

Chat::~Chat()
{

}


/// @brief Chatの描画処理
void Chat::Draw(std::string chatdate)
{
   
    first_font_++;
    if (first_font_ == 1) { fontsize = GetFontSize(); (30); }
    if (Master::is_stat_shop_on_) { (fontsize); return; }
    texture_->Draw();
    DrawBox(280, 180, 700, 240, GetColor(0, 0, 0), true);
    DrawFormatString(300, 200, GetColor(255, 255, 255), "%s", chatdate.c_str());
   
}

/// @brief ChatのReset処理
void Chat::Reset()
{
    (fontsize);
    first_font_ = 0;
}
