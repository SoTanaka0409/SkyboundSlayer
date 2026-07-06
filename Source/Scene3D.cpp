#include"Scene3D.h"
#include"Config.h"
#include"Enemy3D_AT.h"
#include"Player3D.h"
#include"InputManager.h"
#include"Enemy3D.h"
#include"ObjectManager.h"
#include"Stage.h"
#include"Tree.h"
#include"StatShop.h"
#include"Wall.h"
#include"EnemyManager.h"
#include"SkyBox.h"
#include"StageCollider.h"
#include"Rock.h"
#include"Bush.h"
#include"StageObject.h"


Scene3D::Scene3D()
    :LoadTimer(500)
    ,LoadCount(0)
    ,Loadflag(true)
{

}

Scene3D::~Scene3D()
{

}

void Scene3D::Initialize()
{
    SceneGame::Initialize();

    // 霧の設定
    SetFogEnable(TRUE);
    SetFogColor(200, 220, 255);
    SetFogStartEnd(3000.0f, 30000.0f);

    //const float wallWidth = Config::StageWallWidth;//stageのサイズ
    //const float wallDistance = Config::StageWallDistance;//stageのサイズ main
    const float Spawnpos = 12000.0f;
    VECTOR initPOS = VGet(Spawnpos, 100, Spawnpos);

    new Player3D("Resource/Model/T.mv1", VGet(-1200, 20.0f, -1000), 30.0f, 12.0f, 150.0f, true);//ジャンプ、アタック、スピード、ｈｐ
    new StatShop("Resource/Model/shop.mv1", VGet(-1500,100,1500)); // ステージの真ん中に配置
    new StageObject(VGet(-1250, 20.0f, -1050), "Resource/3D/low-poly-campfire/source/campfire.mv1", VGet(10.0f, 10.0f, 10.0f)); // キャンプファイアをショップが開く位置の近くに配置
    //  new DinoTori("Resource/3D/tori/uploads_files_4895089_Sauros.mv1", VGet(-1800.0f, 800.0f, -240.0f), 15, 0.0f, 400.0f, 1.2f);//トリケラトプス,hp,speed,Hitsize,size
       //new Dino2("Resource/3D/T_REX.mv1", VGet(400.0f, 300.0f, 800.0f), 10, 0.0f, 0.0f, 400.0f, 1.0f);//スピの hp,speed,attack,Hitsize,size
    new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1",
        VGet(200.0f, 100.0f, 200.0f));
    new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1",VGet(3.0f,0.3f,3.0f));
   

    float ObjectSize=10.0f;
    new StageObject(Config::GetStageCenter(), "Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1", VGet(5.0f, 10.0f, 5.0f), "", 5.0f * 150.0f);//centertree
    
    int randomScale = 1 + rand() % 2; // 1または2のランダムなスケール
    float treeScale = 1.0f;
    // ランダムに木と岩を配置
    // 木を中央から半径5000メートル以内に均等に配置（フィボナッチ螺旋）
      int numTrees = 15; // 木の本数（必要に応じて増やせます）
      float maxRadius = 5000.0f;
      float goldenAngle = 137.507764f * (DX_PI_F / 180.0f); // 黄金角
      for (int i = 0; i < numTrees; i++)
      {
          float r = maxRadius * sqrtf((float)(i + 0.5f) / numTrees);
          float angle = i * goldenAngle;
          
          VECTOR offset = VGet(cosf(angle) * r, 0.0f, sinf(angle) * r);
          VECTOR pos = VAdd(Config::GetStageCenter(), offset);
  		
          new StageObject(pos, "Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1", VGet(treeScale, treeScale, treeScale), "", treeScale * 150.0f);
      }
    for (int i = 0; i < 20; i++)
    {
        float angle = GetRand(359) * (DX_PI_F / 180.0f);
        float r = -2000 + GetRand(4000);
        VECTOR offset = VGet(cosf(angle) * r, 0.0f, sinf(angle) * r);
        VECTOR pos = VAdd(Config::GetStageCenter(), offset);
        float scale = 30.0f + GetRand(40); // 30~70のランダムスケール
        new Stage(pos, "Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1", "Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1", VGet(scale, scale, scale), "Resource/3D/Rock/mossy-rock/source/rock1_color.jpg");
    }
    
    new StageObject(VGet(0, 0, 0), "Resource/3D/Stage_casule/source/Parede castelo.mv1",VGet(ObjectSize, ObjectSize, ObjectSize));
  // 
  //  new Rock("Resource/3D/rock.mv1", VGet(6000, 0, -300), 0, 3000, 400);*/

  // 

  //  new Wall("",
  //  new Wall("",//右
  //  new Wall("",//上
  //  new Wall("",//下
   
    SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x",VGet(0,0,-5000));
    float scale = 13.0f;
   pSkyBox->SetScale(VGet(scale, scale, scale));
   pSkyBox->SetScale(VGet(scale, scale, scale));
   pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky000.jpg");
   


   
  // 
  //  
  //  new Wall("Resource/2D/mori.png",
  //  new Wall("Resource/2D/mori.png",//右
  //  new Wall("Resource/2D/mori.png",//上
  //  new Wall("Resource/2D/mori.png",//下
  //  pSkyBox2->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");*/
  // 
  // 
  // 
}

void Scene3D::Update()
{
    SceneGame::Update();
    Master::mpSave->Update();
    auto mpPlayer = Master::mpPlayer;
    Player3D* player = Master::mpPlayer;

    if (mpGameManager->GetCurrentPhase() == GameManager::Phase::CLEAR) 
    {
        Master::GameClearCount = 2;
    }

    if (Master::GameClearCount == 2)
    {
        Master::mpSceneManager->SetNextScene(SceneManager::SCENE_RESULTWIN);
    }
   

    if (player->GetStageOutFlag() == false)player->GetPosition() = player->GetOldPosition();//ひとつ前の場所に戻る

   
}

void Scene3D::Draw()
{
    float Load = LoadCount / LoadTimer;
    SceneGame::Draw();
    Master::mpSave->Draw();

    // 地面のグリッド（ステージ）を描画
    const int count = 51;
    const float distance = 500.0f;
    for (int i = 0; i < count; i++)
    {
        float base = (count / 2 - i) * -distance;

        DrawLine3D(
            VGet(-distance * (count / 2), 0.0f, base),
            VGet(distance * (count / 2), 0.0f, base),
            GetColor(255, 255, 255)
        );

        DrawLine3D(
            VGet(base, 0.0f, -distance * (count / 2)),
            VGet(base, 0.0f, distance * (count / 2)),
            GetColor(255, 255, 255)
        );
    }
   /* if (Loadflag)
    {
        Master::PauseOn = true;
        LoadCount++;
       
        unsigned int Color;
        unsigned int Color2;
        Color = GetColor(255, 255, 255);
        Color2 = GetColor(0, 0, 0);
        DrawBox(0, 0, 1000, 1000, GetColor(0, 0, 0), true);
        DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, Color2, true);
        DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, GetColor(255, 255, 255), true);
        DrawBox(pos.x + 1, pos.y + 1,
        pos.x + size.x * Load - 1,
        pos.y + size.y - 1,
        GetColor(0, 255, 0), true);
        mpTexture->Draw();
        if (LoadCount > LoadTimer)
        {
            Loadflag = false;
            Master::PauseOn = false;
        }
    }*/
}

void Scene3D::Finalize()
{
    Master::mpSoundManager->StopBGM();
    SceneGame::Finalize();
    SetFogEnable(FALSE);
}


