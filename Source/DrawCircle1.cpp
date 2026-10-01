#include"DrawCircle1.h"
#include"Master.h"
#include"Dxlib.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Player3D.h"



/// @brief DrawCircle1の初期化（コンストラクタ）
DrawCircle1::DrawCircle1(std::string filename, VECTOR centerPos)
	:Object3D(centerPos)
    ,radius(600)
    ,Maxradius(400)
    ,m_Center(centerPos)
{
	// ^O
	SetTag(Object3D::Tag3D_Object);

    
	m_GraphHandle = LoadGraph(filename.c_str());

    auto m_Player = Master::m_Player;
    auto pPlayer = Master::m_Player;
    OldPosition = pPlayer->GetPosition();

    
    vtx.reserve(div * 3);
}



DrawCircle1::~DrawCircle1()
{

	DeleteGraph(m_GraphHandle);
}



/// @brief DrawCircle1の状態更新処理
void DrawCircle1::Update()
{
    
}
/// @brief `

/// @brief DrawCircle1の描画処理
void DrawCircle1::Draw()
{
    

}