#include"Chat.h"
#include"InputManager.h"
#include"Master.h"



/*
 * 目的（ChatのChat処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Chat::Chat()
    :first_font_(0)
{
    texture_= new Texture("Resource/2D/Chat.png", VGet(500, 200, 0), true);
}

Chat::~Chat()
{

}


/*
 * 目的（ChatのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Chat::Draw(std::string chatdate)
{
   
    first_font_++;
    if (first_font_ == 1) { fontsize = GetFontSize(); (30); }
    if (Master::is_stat_shop_on_) { (fontsize); return; }
    texture_->Draw();
    DrawBox(280, 180, 700, 240, GetColor(0, 0, 0), true);
    DrawFormatString(300, 200, GetColor(255, 255, 255), "%s", chatdate.c_str());
   
}

/*
 * 目的（ChatのReset処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Chat::Reset()
{
    (fontsize);
    first_font_ = 0;
}
