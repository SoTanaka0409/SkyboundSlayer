#include"ResetScene.h"
#include"Enemy3D_AT.h"
#include"Player3D.h"
#include"InputManager.h"
#include"Enemy3D.h"

#include"ObjectManager.h"

#include"Stage.h"
#include"Tree.h"
#include"Wall.h"
#include"EnemyManager.h"
#include"SkyBox.h"

void ResetScene::Initialize()
{
    const float wallWidth = 4000.0f;//stageのサイズ
    const float wallDistance = 10000.0f;//stageのサイズ main
    const float Spawnpos = 8000.0f;
    VECTOR initPOS = VGet(Spawnpos, 100, Spawnpos);
   
    mpEnemyManager = new EnemyManager();
    auto e = new EnemyManager::enemydate;
    e->filename = "Resource/Model/T.mv1";
    e->initPos = initPOS;
    e->hp = 10;
    e->speed = 3;
    e->attack = 2;
    e->HitSize = 60.0f;
    e->Serch1 = 1000;
    e->Serch2 = 100;
    e->Serch3 = 100;
    e->isSeparateAnim = true;
    e->xp = 1;
    e->tag = EnemyManager::night_stage1;
    e->Count = 10;
    auto e1 = new EnemyManager::enemydate;
    e1->filename = "Resource/Model/T.mv1";
    e1->initPos = initPOS;
    e1->hp = 10;
    e1->speed = 3;
    e1->attack = 2;
    e1->HitSize = 60.0f;
    e1->Serch1 = 2000;
    e1->Serch2 = 1000;
    e1->Serch3 = 1000;
    e1->isSeparateAnim = true;
    e1->xp = 1;
    e1->tag = EnemyManager::archerl_stage1;
    e1->Count = 10;


    mpEnemyManager->NewEnemyList(*e);
    mpEnemyManager->NewEnemyList(*e1);
    new Player3D("Resource/Model/T.mv1", VGet(0.0f, 0.0f, 0.0f), 30.0f, 12.0f, 600.0f,true);//ジャンプ、アタック、スピード、ｈｐ
   // new DinoTori("Resource/3D/tori/uploads_files_4895089_Sauros.mv1", VGet(-1800.0f, 800.0f, -240.0f), 15, 0.0f, 400.0f, 1.2f);//トリケラトプス,hp,speed,Hitsize,size
      //new Dino2("Resource/3D/T_REX.mv1", VGet(400.0f, 300.0f, 800.0f), 10, 0.0f, 0.0f, 400.0f, 1.0f);//スピの hp,speed,attack,Hitsize,size
     new Stage(VGet(0.0f, 0.0f, 0.0f), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1");
      new Tree("Resource/3D/Tree.mv1", VGet(500, 0, 600), 400.0, 80.0f, true);
      new Tree("Resource/3D/Tree.mv1", VGet(2000, 0, 1200), 400.0f, 80.0f, true);
      new Tree("Resource/3D/Tree.mv1", VGet(2900, 0, -400), 400.0f, 80.0f, true);
    
     
      new Wall("Resource/2D/mori.png",
          VGet(-wallWidth, 0, 0),//左
          VGet(-wallWidth, 1500, -wallDistance),
          VGet(-wallWidth, 0, wallDistance));
      new Wall("Resource/2D/mori.png",//右
          VGet(wallWidth, 0, 0),
          VGet(wallWidth, 1500, -wallDistance),
          VGet(wallWidth, 0, wallDistance));
      new Wall("Resource/2D/mori.png",//上
          VGet(0, 0, wallWidth),
          VGet(-wallDistance, 1500, wallWidth),
          VGet(wallDistance, 0, wallWidth));
      new Wall("Resource/2D/mori.png",//下
          VGet(0, 0, -wallWidth),
          VGet(-wallDistance, 1500, -wallWidth),
          VGet(wallDistance, 0, -wallWidth));
   
     /* SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x");
      pSkyBox->SetScale(VGet(10.0f, 10.0f, 10.0f));
      pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");*/

     
}

void ResetScene::Update()
{
    auto mpPlayer = Master::mpPlayer;
    Player3D* player = Master::mpPlayer;
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
	if (InputManager::CheckDownKey(KEY_INPUT_1))
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_NAME);
	}
   
    if (player->GetStageOutFlag() == false)player->GetPosition() = player->GetOldPosition();//ひとつ前の場所に戻る
   
    Scene::Update();
}
void ResetScene::SetResetScene()
{
   
}
void ResetScene::Draw()
{
   
    Scene::Draw();
   

}

