#include"TutorialScene.h"
#include"ObjectManager.h"
#include"Player3D.h"
#include"Wall.h"
#include"Camera.h"
#include"Enemy3D.h"
//#include"slope.h"
#include"Stage.h"
#include"Master.h"
#include"Weapon.h"
#include"Bush.h"
#include"EnemyManager.h"
#include"WeaponManager.h"
#include"InputManager.h"
#include"Scene.h"
#include"StageBox.h"
#include"Tree.h"
#include"SkyBox.h"
#include"Object3D.h"
#include"SceneManager.h"
#include"DrawHp.h"
#include"Shop.h"
#include"StageCollider.h"





TutorialScene::TutorialScene()
	:mnTutorialCount(0)
{
	mpTexture=new Texture("Resource/juu.jpg", VGet(500, 800, 0), true);
	//mpEnemy3D = new Enemy3D("", VGe;t(-1000, 5000, 5000), 18, 5.0f, 3000, 2000);
	mpEnemyManager = new EnemyManager();
	
}


TutorialScene::~TutorialScene()
{
	

}

void TutorialScene::Initialize()
{
	Master::TutorialFlag = true;
	Master::TutorialClearCount = 5;
	Master::TutorialCount = 0;
	

	new Player3D("Resource/Model/T.mv1", VGet(0.0f, 0.0f, 0.0f), 30.0f, 12.0f, 600.0f, true);//ジャンプ、アタック、スピード、ｈｐ
	
	//new Dino2("Resource/3D/spino.mv1", VGet(2400.0f, 300.0f, 1200.0f), 10, 6.0f, 3.0f, 300.0f, 1.0f);//スピの hp,speed,attack,Hitsize,size
	//new DinoTori("Resource/3D/tori/uploads_files_4895089_Sauros.mv1", VGet(2400.0f, 300.0f, -2400.0f), 10, 5.0f, 400.0f, 1.0f);//トリケラトプス,hp,speed,Hitsize,size
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-6000.0f, 300.0f, -4400.0f), 11, 5.0f, 3000, 2000);//hp.speedサーチ距離1,2
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-1000.0f, 300.0f, -6000), 18, 5.0f, 3000, 2000);//hp.speed,サーチ距離1,2
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-3000.0f, 300.0f, -4000.0f), 22, 5.0f, 3000, 2000);//hp.speed,サーチ距離1,2
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-2000.0f, 300.0f, -3400.0f), 19, 5.0f, 3000, 2000);//hp.speed,サーチ距離1,2

	new Stage(VGet(0.0f, 0.0f, 0.0f), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1");
	new Tree("Resource/3D/Tree.mv1", VGet(500, 0, -4600), 400.0, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(1000, 0, -1200), 400.0f, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(-400, 0, -3400), 400.0f, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(1500, 0, -5600), 400.0, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(1000, 0, -3200), 400.0f, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(-400, 0, -3400), 400.0f, 150.0f, true);

	//new Enemy3D("Resource/3D/Hero.mv1", VGet(0.0f, 100.0f, -12000.0f), 2442, 5.0f, 0, 0);//hp.speed,サーチ距離1,2

	const float wallWidth = 8000.0f;
	const float wallDistance = 2000.0f;
	new Wall(
		"Resource/Kuro.png",
		VGet(wallDistance/2 , 0.0f, -wallWidth),
		VGet(wallDistance /2, 3000.0f, wallWidth*2),
		VGet(wallDistance /2, 0.0f, -wallWidth *3));
	new Wall(
		"Resource/Kuro.png",
		VGet(-wallDistance/2 , 0.0f, -wallWidth ),
		VGet(-wallDistance /2, 3000.0f, wallWidth*2),
		VGet(-wallDistance /2, 0.0f, -wallWidth * 3));
	new Wall(
		"Resource/Kuro.png",
		VGet(0.0f , 0.0f, 300.0f),
		VGet(wallWidth, 3000.0f, 300.0f),
		VGet(-wallWidth / 2, 0.0f, 300.0f));
	new Wall(
		"Resource/Kuro.png",
		VGet(0.0f, 0.0f, -6500),
		VGet(wallWidth, 3000.0f, -6500),
		VGet(-wallWidth / 2, 0.0f, -6500));
	///天井
	//new Wall(
	//	"Resource/Kuro.png",
	//	VGet(0.0f, 3000.0f, -wallWidth),
	//	VGet(wallWidth, 3000.0f,1000),
	//	VGet(-wallWidth / 2, 3000.0f, -wallWidth * 2));
	VECTOR pos = VGet(10000, 0, 10000);

	const float wallWidth_boss = 2500.0f;//stageのサイズ
	const float wallDistance_boss = 5000.0f;//stageのサイズ main

	new Wall("Resource/2D/mori.png",
		VAdd(VGet(-wallWidth_boss, 0, 0), pos),//左
		VAdd(VGet(-wallWidth_boss, 1500, -wallDistance_boss), pos),
		VAdd(VGet(-wallWidth_boss, 0, wallDistance_boss), pos));
	new Wall("Resource/2D/mori.png",//右
		VAdd(VGet(wallWidth_boss, 0, 0), pos),
		VAdd(VGet(wallWidth_boss, 1500, -wallDistance_boss), pos),
		VAdd(VGet(wallWidth_boss, 0, wallDistance_boss), pos));
	new Wall("Resource/2D/mori.png",//上
		VAdd(VGet(0, 0, wallWidth_boss), pos),
		VAdd(VGet(-wallDistance_boss, 1500, wallWidth_boss), pos),
		VAdd(VGet(wallDistance_boss, 0, wallWidth_boss), pos));
	new Wall("Resource/2D/mori.png",//下
		VAdd(VGet(0, 0, -wallWidth_boss), pos),
		VAdd(VGet(-wallDistance_boss, 1500, -wallWidth_boss), pos),
		VAdd(VGet(wallDistance_boss, 0, -wallWidth_boss), pos));
	
}


void TutorialScene::Update()
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);

	Scene::Update();
	if (InputManager::CheckDownKey(KEY_INPUT_P))
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);///タイトル
	}
	if (Master::TutorialCount == 0)
	{
		auto e = new EnemyManager::enemydate;
		e->initPos = VGet(1000, 100, -4000);
		e->tag = EnemyManager::night_Defo;
		e->Count = 1;
		e->money = 2000;
		mpEnemyManager->NewEnemyList((*e));
		e = new EnemyManager::enemydate;
		e->initPos = VGet(-1000, 100, -4000);
		e->money = 2000;
		e->tag = EnemyManager::night_Defo;
		e->Count = 1;
		mpEnemyManager->NewEnemyList((*e));
		Master::TutorialCount++;
	}
	if (Master::TutorialCount == 3)//敵を倒したら
	{
		new Shop("Resource/3D/Hero.mv1", VGet(18000,100,18000));
		new StageCollider();
		Master::TutorialCount++;
	}

}

void TutorialScene::Draw()
{
	if (Master::TutorialCount == 1)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		DrawFormatString(300, 20, GetColor(255, 0, 0), "左クリックでこうげきして敵を倒そう");
		DrawFormatString(300, 20, GetColor(255, 0, 0), "\nEで攻撃方法を変えられるよ");
	}
	if (Master::TutorialCount == 4)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		DrawFormatString(300, 20, GetColor(255, 0, 0), "目の前のobject入ろう");
		DrawFormatString(300, 20, GetColor(255, 0, 0),"\nチュートリアルではその先にショップがあるよ");

	}
	if (Master::ShopClassOn&&Master::TutorialCount==4)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		DrawFormatString(300, 20, GetColor(255, 0, 0), "shopでアイテムを買って、ショップを閉じたらoを押してみよう");
		
	}
	
	if (Master::TutorialCount == 5&&!Master::ShopClassOn)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		DrawFormatString(300, 20, GetColor(255, 0, 0), "最後に装備の変更を行ってみよう.oを押してね");
		DrawFormatString(300, 20, GetColor(255, 0, 0), "\n矢印キーとenterキーを使うよ");
		DrawFormatString(300, 20, GetColor(255, 0, 0), "\n\nこれでチュートリアルを終了します。Pを押してね");

	}
	
	Scene::Draw();
	int Color = GetColor(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(650, 650, 1000, 1000, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatString(650, 650, GetColor(0, 0, 255), "/////移動キー/////");
	DrawFormatString(650, 690, Color, "WASD:移動キー");
	DrawFormatString(650, 690, GetColor(255, 0, 0), "\n/////アタックキー////");
	DrawFormatString(650, 690, Color, "\n\nMouseLeft:アタック");
	DrawFormatString(650, 690, Color, "\n\n\nSHIFT:回避");
	DrawFormatString(650, 690, Color, "\n\n\n\nE:攻撃方法チェンジ");
	DrawFormatString(650, 690, GetColor(0, 255, 0), "\n\n\n\n\n/////そのほか/////");
	DrawFormatString(650, 690, Color, "\n\n\n\n\n\nO:インベントリ");
	DrawFormatString(650, 690, Color, "\n\n\n\n\n\n\nQ:視点変更");
	DrawFormatString(650, 690, GetColor(255, 0, 0), "\n\n\n\n\n\n\n\n死んだらお金を少しなくす");
}

void TutorialScene::Finalize()
{

}
