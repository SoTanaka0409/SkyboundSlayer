#include"DrawCircle1.h"
#include"Master.h"
#include"Dxlib.h"
#include"SceneManager.h"
#include"ObjectManager.h"


DrawCircle1::DrawCircle1(std::string filename, VECTOR centerPos)
	:Object3D(centerPos)
    ,radius(600)
    ,Maxradius(400)
    ,mnCenter(centerPos)
{
	//^O
	SetTag(Object3D::Tag3D_Object);

    
	//
	mnGraphHandle = LoadGraph(filename.c_str());

    auto mpPlayer = Master::mpPlayer;
    auto pPlayer = Master::mpPlayer;
   //ANVN 
    OldPosition = pPlayer->GetPosition();

    
    vtx.reserve(div * 3);
}



DrawCircle1::~DrawCircle1()
{

	//j
	DeleteGraph(mnGraphHandle);
}


//XV
void DrawCircle1::Update()
{
    
}
//`
void DrawCircle1::Draw()
{
    




    //    // S_
    //    v0.dif = GetColorU8(255, 0, 0, 100); // 

    //    // O@
    //    v1.pos = VGet(
    //        center.x + sinf(a1) * radius,
    //        center.y + 0.5f,
    //        center.z + cosf(a1) * radius

    //    // OA
    //    v2.pos = VGet(
    //        center.x + sinf(a2) * radius,
    //        center.y + 0.5f,
    //        center.z + cosf(a2) * radius

    //// ON


}