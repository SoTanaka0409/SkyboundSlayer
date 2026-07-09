#include"Rule.h"
#include"Texture.h"
#include"Master.h"
#include"SceneManager.h"
#include"InputManager.h"


Rule::Rule()
	:mnPause(0)
	, Color1(1)
	, Colorflag(false)
{
	mnPause = 1;
	mpTexture = new Texture("Resource/Rure.png", VGet(500, 500, 0), true);
	mpTexture2 = new Texture("Resource/Set.png", VGet(540, 460, 0), true);
	mpTexture3 = new Texture("",VGet(350,300,0),true);
}

Rule::~Rule()
{


}
void Rule::Initialize()
{
	filename1 = "Resource/ruleB1.jpg";
	mnHandle1 = LoadGraph(filename1.c_str());
	new Texture("Resource/ruleA.jpg", VGet(500, 900, 0), true);
	new Texture("Resource/ruleB.jpg", VGet(300, 300, 0), true);


}

void Rule::Update()
{
	Scene::Update();



}

void Rule::Draw()
{
	int size = GetFontSize();
	Scene::Draw();
	
	mpTexture->Draw();
	


	//// 半透明の黒い矩形を描画

	//// ブレンドモードを元に戻す（重要）

	if (Colorflag == true)
	{
		Color1 -= 4;
		if (Color1 <= 0)
		{
			Color1 = 0;
			Colorflag = false;
		}
	}
	if (Color1 >= 0 && Colorflag == false)
	{
		Color1 += 4;
		if (Color1 >= 255)
		{
			Color1 = 255;
			Colorflag = true;
		}
	}

	(25);
	mpTexture3->Draw();
	DrawBox(0, 700, 1000, 1000, GetColor(0, 0, 0), true);
	DrawFormatString(10, 710, GetColor(255, 255, 255), "クリア条件：敵をすべて倒す\n(クリア時間が短いほどスコアが上昇する):敵の見た目は恐竜以外");
	DrawFormatString(10, 780, GetColor(255,0, 0), "...敵の数（０になったらゲームクリア！）");
	DrawFormatString(10, 780, GetColor(185, 0, 255), "\n弾の種類は弱・中・強の3種類があり、上部にそれぞれの効果が表示される\n例：一定確率で敵を即死させる など");
	DrawFormatString(10, 780, GetColor(205, 255, 0), "\n\n\nOkが出てきたらできる。");
	DrawFormatString(10, 780, GetColor(0, 255, 0), "\n\n\n\n武器の種類（恐竜を倒したら切り替わる)1...トリケラ  2...スピノ");
	DrawFormatString(10, 780, GetColor(0, 255, 0), "\n\n\n\n\n1...平均的に火力が弱く、自動回復するモード\n2...火力が高くスコアが稼ぎ易く、hpが減り続けるモード  ");
	DrawFormatString(10, 780, GetColor(255, 0, 255), "\n\n\n\n\n\n\n残弾数（上から弱、中、強）");    

	mpTexture2->Draw();
	/*DrawBox(700, 850, 850, 900, GetColor(0, 255, 0), true);
	DrawFormatString(700, 850, GetColor(0, 0, 0), "トリケラ");
	DrawFormatString(940, 860, GetColor(0, 0, 0), "中");
	DrawFormatString(700, 960, GetColor(0, 0, 0), "リロードok");
	DrawFormatString(50, 750, GetColor(Color1, Color1, Color1), "BackSpaceで戻る");*/

	if (InputManager::CheckDownKey(KEY_INPUT_BACK))
	{
		(size);
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
	}

}

void Rule::Finalize()
{

}