#include"DrawCircle1.h"
#include"Master.h"
#include"Dxlib.h"
#include"SceneManager.h"
#include"ObjectManager.h"



/*
 * 目的（DrawCircle1のDrawCircle1処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
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



/*
 * 目的（DrawCircle1のUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void DrawCircle1::Update()
{
    
}
//`

/*
 * 目的（DrawCircle1のDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void DrawCircle1::Draw()
{
    

}