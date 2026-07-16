#include <fstream>
#include <sstream>
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

    SetFogEnable(TRUE);
    SetFogColor(20, 30, 50);
    SetFogStartEnd(500.0f, 10000.0f);

    SetGlobalAmbientLight(GetColorF(0.15f, 0.15f, 0.2f, 1.0f));

    SetLightDifColor(GetColorF(0.6f, 0.6f, 0.8f, 1.0f));
    SetLightSpcColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));
    SetLightDirection(VNorm(VGet(-0.5f, -0.8f, 0.3f)));

    const float Spawnpos = 12000.0f;
    VECTOR initPOS = VGet(Spawnpos, 100, Spawnpos);

    new Player3D("Resource/Model/T.mv1", VGet(-1200, 20.0f, -1000), 30.0f, 12.0f, 150.0f, true);
    new StatShop("Resource/Model/shop.mv1", VGet(-1500,100,1500));
    new StageObject(VGet(-1250, 20.0f, -1050), "Resource/3D/low-poly-campfire/source/campfire.mv1", VGet(10.0f, 10.0f, 10.0f));
    
    new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1",
        VGet(200.0f, 100.0f, 200.0f));
    new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1",VGet(3.0f,0.3f,3.0f));
   

    
    std::ifstream file("Resource/CSV/stage_objects.csv");
    if (file.is_open())
    {
        std::string line;
        std::getline(file, line);
        while (std::getline(file, line))
        {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr, isRelativeStr;
            
            std::getline(ss, type, ',');
            std::getline(ss, model, ',');
            std::getline(ss, xStr, ',');
            std::getline(ss, yStr, ',');
            std::getline(ss, zStr, ',');
            std::getline(ss, sxStr, ',');
            std::getline(ss, syStr, ',');
            std::getline(ss, szStr, ',');
            std::getline(ss, texture, ',');
            std::getline(ss, colSizeStr, ',');
            std::getline(ss, isRelativeStr, ',');
            
            float x = std::stof(xStr);
            float y = std::stof(yStr);
            float z = std::stof(zStr);
            float sx = std::stof(sxStr);
            float sy = std::stof(syStr);
            float sz = std::stof(szStr);
            int isRelative = 0;
            if (!isRelativeStr.empty()) isRelative = std::stoi(isRelativeStr);
            
            VECTOR pos = VGet(x, y, z);
            if (isRelative == 1) {
                pos = VAdd(Config::GetStageCenter(), pos);
            }
            VECTOR scale = VGet(sx, sy, sz);
            
            if (type == "StageObject")
            {
                float colSize = 0.0f;
                if (!colSizeStr.empty()) colSize = std::stof(colSizeStr);
                new StageObject(pos, model, scale, "", colSize);
            }
            else if (type == "Stage")
            {
                new Stage(pos, model, model, scale, texture);
            }
        }
    }
    
    SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x",VGet(0,0,-5000));
    float scale = 13.0f;
   pSkyBox->SetScale(VGet(scale, scale, scale));
   pSkyBox->SetScale(VGet(scale, scale, scale));
   pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");
   


   
}

void Scene3D::Update()
{
    SceneGame::Update();
    Master::mpSave->Update();
    auto mpPlayer = Master::mpPlayer;
    Player3D* player = Master::mpPlayer;

    if (game_manager_->GetCurrentPhase() == GameManager::Phase::kClear) 
    {
        Master::GameClearCount = 2;
    }

    if (Master::GameClearCount == 2)
    {
        if (player != nullptr && Master::mpScoreManager != nullptr) {
            Master::mpScoreManager->SetFinalStats(player->GetHp(), player->GetAttack(), player->GetSpeed());
        }
        Master::mpSceneManager->SetNextScene(SceneManager::kSceneResultWin);
    }
   

    if (player->GetStageOutFlag() == false)player->GetPosition() = player->GetOldPosition();

   
}

void Scene3D::Draw()
{
    float Load = LoadCount / LoadTimer;
    SceneGame::Draw();
    Master::mpSave->Draw();

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


