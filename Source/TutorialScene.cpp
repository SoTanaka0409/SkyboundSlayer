#include"TutorialScene.h"
#include"Config.h"
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
#include"Dino2.h"
#include"DinoTori.h"
#include"WeaponManager.h"
#include"InputManager.h"
#include"Scene.h"
#include"StageBox.h"
#include"Tree.h"
#include"SkyBox.h"
#include"DinoFriend.h"
#include"Object3D.h"
#include"SceneManager.h"
#include"DrawHp.h"
#include"RideDino.h"






TutorialScene::TutorialScene()
	:Clear1(false)
	,Clear2(false)
	,Clear3(false)
	,AllTimer(0)
	, ClearCount(5)
	,Drawflag1(false)
	,Drawflag2(false)
	,Drawflag3(false)
	,Count(0)
	, Color1(1)
	, Colorflag(false)
	
	
{
	mpTexture=new Texture("Resource/juu.jpg", VGet(500, 800, 0), true);
	//mpEnemy3D = new Enemy3D("", VGet(-1000, 5000, 5000), 18, 5.0f, 3000, 2000);
}


TutorialScene::~TutorialScene()
{

}

void TutorialScene::Initialize()
{
	Master::TutorialFlag = true;
	Master::TutorialClearCount = 5;
	Master::TutorialCount = 0;
	new Player3D("Resource/3D/Hero.mv1", VGet(0,0,0), 30.0f, 12.0f, 100000.0f);//ジャンプ、アタック、スピード、ｈｐ
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

	const float wallWidth = Config::TutorialWallWidth;
	const float wallDistance = Config::TutorialWallDistance;
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
	
}


void TutorialScene::Update()
{
	
}

void TutorialScene::Draw()
{

}
void TutorialScene::Finalize()
{

}
