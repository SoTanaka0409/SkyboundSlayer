#include"haikei.h"
#include"Object2D.h"
#include"ObjectManager.h"
#include"Master.h"
#include"Scene.h"
#include"InputManager.h"


/// @brief haikeiの初期化（コンストラクタ）
haikei::haikei(VECTOR initPos, std::string filename)
	:Object2D(initPos)

{
	texture_ = new Texture("", initPos, true);
}
haikei::~haikei()
{
}


/// @brief haikeiの状態更新処理
void haikei::Update()
{
	Object2D::Update();
	texture_->Update();
}


/// @brief haikeiの描画処理
void haikei::Draw()
{
	texture_->Draw();
	Object2D::Draw();
}

