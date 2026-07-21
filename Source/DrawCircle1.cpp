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
    ,center_(centerPos)
{
	//^O
	SetTag(Object3D::Tag3D_Object);

    
	graph_handle_ = LoadGraph(filename.c_str());

    auto player_ = Master::player_;
    auto pPlayer = Master::player_;
    OldPosition = pPlayer->GetPosition();

    
    vtx.reserve(div * 3);
}



DrawCircle1::~DrawCircle1()
{

	DeleteGraph(graph_handle_);
}



/// @brief DrawCircle1の状態更新処理
void DrawCircle1::Update()
{
    
}
//`

/// @brief DrawCircle1の描画処理
void DrawCircle1::Draw()
{
    

}