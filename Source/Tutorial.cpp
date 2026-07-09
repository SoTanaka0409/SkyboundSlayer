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
#include"EnemyManager.h"
#include"InputManager.h"
#include"Scene.h"
#include"StageBox.h"
#include"Tree.h"
#include"SkyBox.h"
#include"Object3D.h"
#include"SceneManager.h"
#include"DrawHp.h"
#include"StageCollider.h"





TutorialScene::TutorialScene()
	:mnTutorialCount(0)
{
	mpTexture=new Texture("Resource/juu.jpg", VGet(500, 800, 0), true);
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
	

	new Player3D("Resource/Model/T.mv1", VGet(0.0f, 0.0f, 0.0f), 30.0f, 12.0f, 600.0f, true);//繧ｸ繝｣繝ｳ繝励√い繧ｿ繝繧ｯ縲√せ繝斐ｼ繝峨ｽ茨ｽ
	
	//new Dino2("Resource/3D/spino.mv1", VGet(2400.0f, 300.0f, 1200.0f), 10, 6.0f, 3.0f, 300.0f, 1.0f);//繧ｹ繝斐ｮ hp,speed,attack,Hitsize,size
	//new DinoTori("Resource/3D/tori/uploads_files_4895089_Sauros.mv1", VGet(2400.0f, 300.0f, -2400.0f), 10, 5.0f, 400.0f, 1.0f);//繝医Μ繧ｱ繝ｩ繝医励せ,hp,speed,Hitsize,size
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-6000.0f, 300.0f, -4400.0f), 11, 5.0f, 3000, 2000);//hp.speed繧ｵ繝ｼ繝∬ｷ晞屬1,2
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-1000.0f, 300.0f, -6000), 18, 5.0f, 3000, 2000);//hp.speed,繧ｵ繝ｼ繝∬ｷ晞屬1,2
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-3000.0f, 300.0f, -4000.0f), 22, 5.0f, 3000, 2000);//hp.speed,繧ｵ繝ｼ繝∬ｷ晞屬1,2
	//new Enemy3D("Resource/3D/Hero.mv1", VGet(-2000.0f, 300.0f, -3400.0f), 19, 5.0f, 3000, 2000);//hp.speed,繧ｵ繝ｼ繝∬ｷ晞屬1,2

	new Stage(VGet(0.0f, 0.0f, 0.0f), "Resource/3D/Stage/Stage00.mv1", "Resource/3D/Stage/Stage00_c.mv1");
	new Tree("Resource/3D/Tree.mv1", VGet(500, 0, -4600), 400.0, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(1000, 0, -1200), 400.0f, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(-400, 0, -3400), 400.0f, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(1500, 0, -5600), 400.0, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(1000, 0, -3200), 400.0f, 150.0f, true);
	new Tree("Resource/3D/Tree.mv1", VGet(-400, 0, -3400), 400.0f, 150.0f, true);

	//new Enemy3D("Resource/3D/Hero.mv1", VGet(0.0f, 100.0f, -12000.0f), 2442, 5.0f, 0, 0);//hp.speed,繧ｵ繝ｼ繝∬ｷ晞屬1,2

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
	///螟ｩ莠
	//new Wall(
	//	"Resource/Kuro.png",
	VECTOR pos = VGet(10000, 0, 10000);

	const float wallWidth_boss = Config::TutorialBossWallWidth;//stage縺ｮ繧ｵ繧､繧ｺ
	const float wallDistance_boss = Config::TutorialBossWallDistance;//stage縺ｮ繧ｵ繧､繧ｺ main

	new Wall("Resource/2D/mori.png",
		VAdd(VGet(-wallWidth_boss, 0, 0), pos),//蟾ｦ
		VAdd(VGet(-wallWidth_boss, 1500, -wallDistance_boss), pos),
		VAdd(VGet(-wallWidth_boss, 0, wallDistance_boss), pos));
	new Wall("Resource/2D/mori.png",//蜿ｳ
		VAdd(VGet(wallWidth_boss, 0, 0), pos),
		VAdd(VGet(wallWidth_boss, 1500, -wallDistance_boss), pos),
		VAdd(VGet(wallWidth_boss, 0, wallDistance_boss), pos));
	new Wall("Resource/2D/mori.png",//荳
		VAdd(VGet(0, 0, wallWidth_boss), pos),
		VAdd(VGet(-wallDistance_boss, 1500, wallWidth_boss), pos),
		VAdd(VGet(wallDistance_boss, 0, wallWidth_boss), pos));
	new Wall("Resource/2D/mori.png",//荳
		VAdd(VGet(0, 0, -wallWidth_boss), pos),
		VAdd(VGet(-wallDistance_boss, 1500, -wallWidth_boss), pos),
		VAdd(VGet(wallDistance_boss, 0, -wallWidth_boss), pos));
	
}


void TutorialScene::Update()
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* pPlayer = Master::mpPlayer;

	Scene::Update();
	if (InputManager::CheckDownKey(KEY_INPUT_P))
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);///繧ｿ繧､繝医Ν
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
	if (Master::TutorialCount == 3)//謨ｵ繧貞偵＠縺溘ｉ
	{
		new StageCollider();
		Master::TutorialCount++;
	}

}

void TutorialScene::Draw()
{
	if (Master::TutorialCount == 1)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		
	/*	DrawFormatString(300, 20, GetColor(255, 0, 0), "蟾ｦ繧ｯ繝ｪ繝繧ｯ縺ｧ縺薙≧縺偵″縺励※謨ｵ繧貞偵◎縺");

		DrawFormatString(300, 20, GetColor(255, 0, 0), "\nE縺ｧ謾ｻ謦譁ｹ豕輔ｒ螟峨∴繧峨ｌ繧九ｈ");*/
	}
	if (Master::TutorialCount == 4)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		DrawFormatString(300, 20, GetColor(255, 0, 0),"逶ｮ縺ｮ蜑阪ｮobject蜈･繧阪≧");
		DrawFormatString(300, 20, GetColor(255, 0, 0),"\n繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｧ縺ｯ縺昴ｮ蜈医↓繧ｷ繝ｧ繝繝励′縺ゅｋ繧");

	}
	if (Master::StatShopClassOn&&Master::TutorialCount==4)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		DrawFormatString(300, 20, GetColor(255, 0, 0), "shop縺ｧ繧｢繧､繝繝繧定ｲｷ縺｣縺ｦ縲√す繝ｧ繝繝励ｒ髢峨§縺溘ｉo繧呈款縺励※縺ｿ繧医≧");
		
	}
	
	if (Master::TutorialCount == 5&&!Master::StatShopClassOn)
	{
		DrawBox(0, 0, 1000, 100, GetColor(0, 0, 0), true);
		DrawFormatString(300, 20, GetColor(255, 0, 0), "譛蠕後↓陬蛯吶ｮ螟画峩繧定｡後▲縺ｦ縺ｿ繧医≧.o繧呈款縺励※縺ｭ");
		DrawFormatString(300, 20, GetColor(255, 0, 0), "\n遏｢蜊ｰ繧ｭ繝ｼ縺ｨenter繧ｭ繝ｼ繧剃ｽｿ縺繧");
		DrawFormatString(300, 20, GetColor(255, 0, 0), "\n\n縺薙ｌ縺ｧ繝√Η繝ｼ繝医Μ繧｢繝ｫ繧堤ｵゆｺ縺励∪縺吶１繧呈款縺励※縺ｭ");

	}
	
	Scene::Draw();
	int Color = GetColor(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(650, 650, 1000, 1000, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatString(650, 650, GetColor(0, 0, 255), "/////遘ｻ蜍輔く繝ｼ/////");
	DrawFormatString(650, 690, Color, "WASD:遘ｻ蜍輔く繝ｼ");
	DrawFormatString(650, 690, GetColor(255, 0, 0), "\n/////繧｢繧ｿ繝繧ｯ繧ｭ繝ｼ////");
	DrawFormatString(650, 690, Color, "\n\nMouseLeft:繧｢繧ｿ繝繧ｯ");
	DrawFormatString(650, 690, Color, "\n\n\nSHIFT:蝗樣∩");
	DrawFormatString(650, 690, Color, "\n\n\n\nE:謾ｻ謦譁ｹ豕輔メ繧ｧ繝ｳ繧ｸ");
	DrawFormatString(650, 690, GetColor(0, 255, 0), "\n\n\n\n\n/////縺昴ｮ縺ｻ縺/////");
	DrawFormatString(650, 690, Color, "\n\n\n\n\n\nO:繧､繝ｳ繝吶Φ繝医Μ");
	DrawFormatString(650, 690, Color, "\n\n\n\n\n\n\nQ:隕也せ螟画峩");
	DrawFormatString(650, 690, GetColor(255, 0, 0), "\n\n\n\n\n\n\n\n豁ｻ繧薙□繧峨♀驥代ｒ蟆代＠縺ｪ縺上☆");
}

void TutorialScene::Finalize()
{

}
