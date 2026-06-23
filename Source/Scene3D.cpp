#include"Scene3D.h"
#include"Config.h"
#include"Enemy3D_AT.h"
#include"Player3D.h"
#include"InputManager.h"
#include"Enemy3D.h"
#include"ObjectManager.h"
#include"Stage.h"
#include"Tree.h"
#include"Shop.h"
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

    //const float wallWidth = Config::StageWallWidth;//stageのサイズ
    //const float wallDistance = Config::StageWallDistance;//stageのサイズ main
    const float Spawnpos = 12000.0f;
    VECTOR initPOS = VGet(Spawnpos, 100, Spawnpos);

    new Player3D("Resource/Model/T.mv1", VGet(-1200, 2000.0f, -1000), 30.0f, 12.0f, 150.0f, true);//ジャンプ、アタック、スピード、ｈｐ
    new StatShop("Resource/3D/Hero.mv1", Config::GetStageCenter()); // ステージの真ん中に配置
    //  new DinoTori("Resource/3D/tori/uploads_files_4895089_Sauros.mv1", VGet(-1800.0f, 800.0f, -240.0f), 15, 0.0f, 400.0f, 1.2f);//トリケラトプス,hp,speed,Hitsize,size
       //new Dino2("Resource/3D/T_REX.mv1", VGet(400.0f, 300.0f, 800.0f), 10, 0.0f, 0.0f, 400.0f, 1.0f);//スピの hp,speed,attack,Hitsize,size
    new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", 
        VGet(200.0f, 100.0f, 200.0f));
    new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1");
   
    float ObjectSize=10.0f;
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
   


    const float wallWidth_boss = Config::StageBossWallWidth;//stageのサイズ
    const float wallDistance_boss = Config::StageBossWallDistance;//stageのサイズ main
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
    auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
    Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

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
}


