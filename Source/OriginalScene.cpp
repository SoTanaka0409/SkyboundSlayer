#include"OriginalScene.h"
#include <fstream>

OriginalScene::OriginalScene()
	:ifIsName(false)
	,AllTimer(0)
{


}
OriginalScene::~OriginalScene()
{


}

void OriginalScene::Initialize()
{ 

}
void OriginalScene::Finalize()
{

}

void OriginalScene::Draw()
{
	Scene::Draw();

    DrawString(100, 100, "名前を入力してください（Enterで確定）", GetColor(255, 255, 255));
    DrawString(100, 140, name.c_str(), GetColor(255, 255, 0));

    // A～Z
    for (int i = KEY_INPUT_A; i <= KEY_INPUT_Z; i++)
    {
        if (CheckHitKey(i))
        {
            name += char('A' + (i - KEY_INPUT_A));
            WaitTimer(150); // 連続入力防止
        }
    }

    // BackSpace
    if (CheckHitKey(KEY_INPUT_BACK) && !name.empty())
    {
        name.pop_back();
        WaitTimer(150);
    }

    // Enterで確定
    if (CheckHitKey(KEY_INPUT_RETURN))
    {
        endInput = true;
    }

#include <fstream>

    std::ofstream ofs("name.dat");
    ofs << name;
    ofs.close();

}

void OriginalScene::Update()
{
	Scene::Update();
	AllTimer++;

}