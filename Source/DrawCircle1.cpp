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
	//タグ設定
	SetTag(Object3D::Tag3D_Object);

    
	//画像の読み込み
	mnGraphHandle = LoadGraph(filename.c_str());

    auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
    auto pPlayer = dynamic_cast<Player3D*>(mpPlayer);
   //最初にアクションを起こした場所を保存 
    OldPosition = pPlayer->GetPosition();

    
    vtx.reserve(div * 3);
}



DrawCircle1::~DrawCircle1()
{

	//画像の破棄
	DeleteGraph(mnGraphHandle);
}


//更新
void DrawCircle1::Update()
{
    
}
//描画
void DrawCircle1::Draw()
{
    
    //auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
    //auto pPlayer = dynamic_cast<Player3D*>(mpPlayer);

    //VECTOR center = pPlayer->GetPosition();
    //center.y = OldPosition.y-250.0f;

    //std::vector<VERTEX3D> vtx;
    //for (int i = 0; i < div; i++)
    //{
    //    float a1 = DX_PI_F * 2.0f * i / div;
    //    float a2 = DX_PI_F * 2.0f * (i + 1) / div;

    //    VERTEX3D v0, v1, v2;

    //    // 中心点
    //    v0.pos = VGet(center.x, center.y + 0.5f, center.z);
    //    v0.dif = GetColorU8(255, 0, 0, 100); // 半透明
    //    v0.spc = GetColorU8(0, 0, 0, 0);
    //    v0.u = 0.5f;
    //    v0.v = 0.5f;

    //    // 外周①
    //    v1.pos = VGet(
    //        center.x + sinf(a1) * radius,
    //        center.y + 0.5f,
    //        center.z + cosf(a1) * radius
    //    );
    //    v1.dif = GetColorU8(255, 0, 0, 100);
    //    v1.spc = GetColorU8(0, 0, 0, 0);
    //    v1.u = cosf(a1) * 0.5f + 0.5f;
    //    v1.v = sinf(a1) * 0.5f + 0.5f;

    //    // 外周②
    //    v2.pos = VGet(
    //        center.x + sinf(a2) * radius,
    //        center.y + 0.5f,
    //        center.z + cosf(a2) * radius
    //    );
    //    v2.dif = GetColorU8(255, 0, 0, 100);
    //    v2.spc = GetColorU8(0, 0, 0, 0);
    //    v2.u = cosf(a2) * 0.5f + 0.5f;
    //    v2.v = sinf(a2) * 0.5f + 0.5f;

    //    vtx.push_back(v0);
    //    vtx.push_back(v1);
    //    vtx.push_back(v2);
    //}
    //// 半透明ON
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

    //DrawPolygon3D(vtx.data(), div * 3, DX_NONE_GRAPH, TRUE);

    //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}