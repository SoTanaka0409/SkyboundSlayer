#include"Scene3D.h"
#include"Enemy3D_AT.h"
#include"Player3D.h"
#include"InputManager.h"
#include"Enemy3D.h"
#include"ObjectManager.h"
#include"Stage.h"
#include"Tree.h"
#include"Shop.h"
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
    const float wallWidth = 4000.0f;//stageのサイズ
    const float wallDistance = 10000.0f;//stageのサイズ main
    const float Spawnpos = 12000.0f;
    VECTOR initPOS = VGet(Spawnpos, 100, Spawnpos);

    mpEnemyManager = new EnemyManager();
    mCurrentPhase = Phase::PHASE_1;
    SpawnPhaseEnemies();
    new Player3D("Resource/Model/T.mv1", VGet(-1200, 2000.0f, 0), 30.0f, 12.0f, 150.0f, true);//ジャンプ、アタック、スピード、ｈｐ
    new Shop("Resource/3D/Hero.mv1", VGet(7500, 200, 7000));
    //  new DinoTori("Resource/3D/tori/uploads_files_4895089_Sauros.mv1", VGet(-1800.0f, 800.0f, -240.0f), 15, 0.0f, 400.0f, 1.2f);//トリケラトプス,hp,speed,Hitsize,size
       //new Dino2("Resource/3D/T_REX.mv1", VGet(400.0f, 300.0f, 800.0f), 10, 0.0f, 0.0f, 400.0f, 1.0f);//スピの hp,speed,attack,Hitsize,size
    new Stage(VGet(-5000.0f, 3000.0f, -5000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1", 
        VGet(200.0f, 100.0f, 200.0f));
    new Stage(VGet(15000.0f, 3000.0f, 15000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1",
        VGet(200.0f, 100.0f, 200.0f));
    new Stage(VGet(0,0,0),"Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1",
        VGet(1.0f,0.0f,1.0f));
    float ObjectSize=10.0f;
    new StageObject(VGet(0, 0, 0), "Resource/3D/Stage_casule/source/Parede castelo.mv1",VGet(ObjectSize, ObjectSize, ObjectSize));
    //  //new Tree("Resource/3D/Tree.mv1", VGet(500, 0, 600), 400.0, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(2000, 0, 1200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(2900, 0, -400), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(6000, 0, 1200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(7900, 0, -4000), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(2000, 0, -2200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(2900, 0, -4000), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(6000, 0, 3200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(-3900, 0, -000), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(-2000, 0, 1200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(-2900, 0, -500), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(-6000, 0, 1200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(-3900, 0, -4000), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(2000, 0, 7200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(3900, 0, 6400), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(5000, 0, 5200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(7900, 0, 6000), 400.0f, 80.0f, true);
  // 
  //  new Tree("Resource/3D/Tree.mv1", VGet(2000, 0, 1200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(-2900, 0, 4400), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(6000, 0, -3200), 400.0f, 80.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(5900, 0, -4000), 400.0f, 80.0f, true);
  // /* new Rock("Resource/3D/rock.mv1", VGet(0, 0, 1000), 0, 3000,400);
  //  new Rock("Resource/3D/rock.mv1", VGet(-5000, 0, -5000), 0, 3000, 400);
  //  new Rock("Resource/3D/rock.mv1", VGet(5000, 0, 3000), 0, 3000, 400);
  //  new Rock("Resource/3D/rock.mv1", VGet(6000, 0, -300), 0, 3000, 400);*/

  // 
  ////  new Tree("Resource/3D/Bush.mv1", VGet(6000, 0, -6000), 400.0f, 80.0f, true);

  //  new Wall("",
  //      VGet(-wallWidth, 0, 0),//左
  //      VGet(-wallWidth, 3000, -wallDistance),
  //      VGet(-wallWidth, 0, wallDistance));
  //  new Wall("",//右
  //      VGet(wallWidth, 0, 0),
  //      VGet(wallWidth, 3000, -wallDistance),
  //      VGet(wallWidth, 0, wallDistance));
  //  new Wall("",//上
  //      VGet(0, 0, wallWidth),
  //      VGet(-wallDistance, 3000, wallWidth),
  //      VGet(wallDistance, 0, wallWidth));
  //  new Wall("",//下
  //      VGet(0, 0, -wallWidth),
  //      VGet(-wallDistance, 3000, -wallWidth),
  //      VGet(wallDistance, 0, -wallWidth));
    dd;
    SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x",VGet(10000,0,0));
    float scale = 5.0f;
   pSkyBox->SetScale(VGet(scale, scale, scale));
   pSkyBox->SetScale(VGet(scale, scale, scale));
   pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky000.jpg");
   
  //  SkyBox* pSkyBox2 = new SkyBox("Resource/3D/SkyBox/SkyBox.x",VGet(20000,0,20000));
  //  pSkyBox2->SetScale(10.0f);
  //  pSkyBox2->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");


  //  const float wallWidth_boss = 2000.0f;//stageのサイズ
  //  const float wallDistance_boss = 4000.0f;//stageのサイズ main
  // 
  //  VECTOR pos = VGet(10000, 0, 10000);
  //  
  //  new Wall("Resource/2D/mori.png",
  //     VAdd(VGet(-wallWidth_boss, 0, 0),pos),//左
  //     VAdd( VGet(-wallWidth_boss,3000, -wallDistance_boss),pos),
  //     VAdd( VGet(-wallWidth_boss, 0, wallDistance_boss),pos));
  //  new Wall("Resource/2D/mori.png",//右
  //     VAdd( VGet(wallWidth_boss, 0, 0),pos),
  //     VAdd( VGet(wallWidth_boss, 3000, -wallDistance_boss),pos),
  //     VAdd( VGet(wallWidth_boss, 0, wallDistance_boss),pos));
  //  new Wall("Resource/2D/mori.png",//上
  //     VAdd( VGet(0, 0, wallWidth_boss),pos),
  //     VAdd( VGet(-wallDistance_boss, 3000, wallWidth_boss),pos),
  //     VAdd( VGet(wallDistance_boss, 0, wallWidth_boss),pos));
  //  new Wall("Resource/2D/mori.png",//下
  //     VAdd( VGet(0, 0, -wallWidth_boss),pos),
  //     VAdd( VGet(-wallDistance_boss, 3000, -wallWidth_boss),pos),
  //     VAdd( VGet(wallDistance_boss, 0, -wallWidth_boss),pos));
  // /* SkyBox* pSkyBox2 = new SkyBox("Resource/3D/SkyBox/SkyBox.x");
  //  pSkyBox2->SetScale(3.0f);
  //  pSkyBox2->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");*/
  // 
  //  new Tree("Resource/3D/Tree.mv1", VGet(18000, 0, 18000), 4000.0f, 500.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(21000, 0, 18000), 1000.0f, 300.0f, true);
  //  new Tree("Resource/3D/Tree.mv1", VGet(18000, 0, 21000), 1000.0f,300.0f, true);
  // 
  // 
  //  Master::mpSoundManager->PlayBGM(SoundManager::BGM_GAME);
  //  new StageCollider();
  //  mpTexture = new Texture("Resource/2D/Load.png", VGet(500, 300, 0), true);
}

void Scene3D::Update()
{
    Scene::Update();
    Master::mpSave->Update();
    auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
    Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
    const int count = 51;
    const float distance = 500.0f;
    /*for (int i = 0; i < count; i++)
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
    }*/
    auto enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    if (enemies.empty() && !Loadflag)
    {
        if (mCurrentPhase == Phase::PHASE_1) {
            mCurrentPhase = Phase::PHASE_2;
            SpawnPhaseEnemies();
        } else if (mCurrentPhase == Phase::PHASE_2) {
            mCurrentPhase = Phase::BOSS;
            SpawnPhaseEnemies();
        } else if (mCurrentPhase == Phase::BOSS) {
            mCurrentPhase = Phase::CLEAR;
            Master::GameClearCount = 2;
        }
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
    Scene::Draw();
    Master::mpSave->Draw();
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


}

void Scene3D::SpawnPhaseEnemies()
{
    const float Spawnpos = 12000.0f;
    VECTOR initPOS = VGet(Spawnpos, 100, Spawnpos);

   /* if (mCurrentPhase == Phase::PHASE_1) {
        auto e = new EnemyManager::enemydate;
        e->filename = "Resource/Model/T.mv1";
        e->initPos = initPOS;
        e->hp = 20;
        e->speed = 3;
        e->attack = 2;
        e->HitSize = 60.0f;
        e->Serch1 = 1000;
        e->Serch2 = 100;
        e->Serch3 = 100;
        e->isSeparateAnim = true;
        e->xp = 30;
        e->money = 200;
        e->tag = EnemyManager::night_stagg1;
        e->Count = 10;
        mpEnemyManager->NewEnemyList(*e);
    }
    else if (mCurrentPhase == Phase::PHASE_2) {
        auto e1 = new EnemyManager::enemydate;
        e1->filename = "Resource/Model/T.mv1";
        e1->initPos = initPOS;
        e1->hp = 20;
        e1->speed = 3;
        e1->attack = 2;
        e1->HitSize = 60.0f;
        e1->money = 200;
        e1->Serch1 = 2000;
        e1->Serch2 = 1000;
        e1->Serch3 = 1000;
        e1->isSeparateAnim = true;
        e1->xp = 30;
        e1->tag = EnemyManager::archerl_stage1;
        e1->Count = 10;
        mpEnemyManager->NewEnemyList(*e1);
    }
    else if (mCurrentPhase == Phase::BOSS) {
        VECTOR initPOS2 = VGet(22000, 100, 22000);
        auto e2 = new EnemyManager::enemydate;
        e2->filename = "Resource/3D/Boss1.mv1";
        e2->initPos = initPOS2;
        e2->hp = 300;
        e2->speed = 10;
        e2->attack = 20;
        e2->HitSize = 300.0f;
        e2->Serch1 = 2000;
        e2->Serch2 = 1000;
        e2->Serch3 = 1000;
        e2->money = 3000;
        e2->isSeparateAnim = true;
        e2->xp = 300;
        e2->tag = EnemyManager::boss_stage1;
        e2->Count = 1;
        mpEnemyManager->NewEnemyList(*e2);
    }*/
}
