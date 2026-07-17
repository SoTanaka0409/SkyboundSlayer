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




/*
 * 目的（Scene3DのScene3D処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Scene3D::Scene3D()
    :load_timer_(500)
    ,load_count_(0)
    ,is_load_flag_(true)
{

}

Scene3D::~Scene3D()
{

}



/*
 * 目的（Scene3DのInitialize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Scene3D::Initialize()
{
    SceneGame::Initialize();
    SetupEnvironment();
    CreateInitialActors();
    CreateStage();
    LoadStageObjectsFromCsv();
    CreateSkyBox();
}

void Scene3D::SetupEnvironment()
{
    SetFogEnable(TRUE);
    SetFogColor(20, 30, 50);
    SetFogStartEnd(500.0f, 10000.0f);

    SetGlobalAmbientLight(GetColorF(0.15f, 0.15f, 0.2f, 1.0f));

    SetLightDifColor(GetColorF(0.6f, 0.6f, 0.8f, 1.0f));
    SetLightSpcColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));
    SetLightDirection(VNorm(VGet(-0.5f, -0.8f, 0.3f)));
}

void Scene3D::CreateInitialActors()
{
    new Player3D("Resource/Model/T.mv1", VGet(-1200, 20.0f, -1000), 30.0f, 12.0f, 150.0f, true);
    new StatShop("Resource/Model/shop.mv1", VGet(-1500, 100, 1500));
    new StageObject(VGet(-1250, 20.0f, -1050), "Resource/3D/low-poly-campfire/source/campfire.mv1", VGet(10.0f, 10.0f, 10.0f));
}

void Scene3D::CreateStage()
{
    new Stage(VGet(0.0f, 5000.0f, -20000.0f), "Resource/3D/stage_sky/source/Flooting_Stage.mv1", "Resource/3D/stage_sky/source/Flooting_Stage.mv1",
        VGet(200.0f, 100.0f, 200.0f));
    new Stage(Config::GetStageCenter(), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1", VGet(3.0f, 0.3f, 3.0f));
}

void Scene3D::LoadStageObjectsFromCsv()
{
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
            if (isRelative == 1)
            {
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
}

void Scene3D::CreateSkyBox()
{
    SkyBox* pSkyBox = new SkyBox("Resource/3D/SkyBox/SkyBox.x", VGet(0, 0, -5000));
    float scale = 13.0f;
    pSkyBox->SetScale(VGet(scale, scale, scale));
    pSkyBox->SetModelTexture("Resource/3D/SkyBox/sky001.jpg");
}

/*
 * 目的（Scene3DのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Scene3D::Update()
{
    SceneGame::Update();
    Master::save_->Update();
    auto player_ = Master::player_;
    Player3D* player = Master::player_;

    if (game_manager_->GetCurrentPhase() == GameManager::Phase::kClear) 
    {
        Master::game_clear_count_ = 2;
    }

    if (Master::game_clear_count_ == 2)
    {
        if (player != nullptr && Master::score_manager_ != nullptr) {
            Master::score_manager_->SetFinalStats(player->GetAllStatusState(Object3D::Status_Hp), player->GetAllStatusState(Object3D::Status_Attack), player->GetAllStatusState(Object3D::Status_Speed));
        }
        Master::scene_manager_->SetNextScene(SceneManager::kSceneResultWin);
    }
   

    if (player->GetStageOutFlag() == false)player->GetPosition() = player->GetOldPosition();

   
}



/*
 * 目的（Scene3DのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Scene3D::Draw()
{
    SceneGame::Draw();
    Master::save_->Draw();
    DrawDebugGrid();
   /* if (is_load_flag_)
    {
        Master::is_pause_on_ = true;
        load_count_++;

        unsigned int Color;
        unsigned int Color2;
        Color = GetColor(255, 255, 255);
        Color2 = GetColor(0, 0, 0);
        DrawBox(0, 0, 1000, 1000, GetColor(0, 0, 0), true);
        DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, Color2, true);
        DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, GetColor(255, 255, 255), true);
        DrawBox(pos_.x + 1, pos_.y + 1,
        pos_.x + size_.x * (load_count_ / load_timer_) - 1,
        pos_.y + size_.y - 1,
        GetColor(0, 255, 0), true);
        texture_->Draw();
        if (load_count_ > load_timer_)
        {
            is_load_flag_ = false;
            Master::is_pause_on_ = false;
        }
    }*/
}

void Scene3D::DrawDebugGrid()
{
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
}

/*
 * 目的（Scene3DのFinalize処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Scene3D::Finalize()
{
    Master::sound_manager_->StopBGM();
    SceneGame::Finalize();
    SetFogEnable(FALSE);
}


