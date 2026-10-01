#include"Background.h"
#include"Object2D.h"
#include"ObjectManager.h"
#include"Master.h"
#include"Scene.h"
#include"InputManager.h"


/// @brief haikeiの初期化（コンストラクタ）
Background::Background(VECTOR initPos, std::string filename)
	:Object2D(initPos)

{
	m_Texture = new Texture("", initPos, true);
}
Background::~Background()
{
}


/// @brief haikeiの状態更新処理
void Background::Update()
{
	Object2D::Update();
	m_Texture->Update();
}


/// @brief haikeiの描画処理
void Background::Draw()
{
	m_Texture->Draw();
	Object2D::Draw();
}

