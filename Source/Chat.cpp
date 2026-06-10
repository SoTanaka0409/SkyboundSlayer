#include"Chat.h"
#include"InputManager.h"
#include"Master.h"


Chat::Chat()
    :mnFirstFont(0)
{
    mpTexture= new Texture("Resource/2D/Chat.png", VGet(500, 200, 0), true);
}

Chat::~Chat()
{

}

void Chat::Draw(std::string chatdate)
{
   
    mnFirstFont++;
    if (mnFirstFont == 1) { fontsize = GetFontSize(); (30); }
    if (Master::InventoryClasOn || Master::ShopClassOn) { (fontsize); return; }
    mpTexture->Draw();
    DrawBox(280, 180, 700, 240, GetColor(0, 0, 0), true);
    DrawFormatString(300, 200, GetColor(255, 255, 255), "%s", chatdate.c_str());
   
}
void Chat::Reset()
{
    (fontsize);
    mnFirstFont = 0;
}