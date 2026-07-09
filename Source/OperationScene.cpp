#include"OperationScene.h"
#include"Texture.h"
#include"Master.h"
#include"SceneManager.h"
#include"InputManager.h"


OperationScene::OperationScene()
	:mnPause(0)
	, Color1(1)
	, Colorflag(false)
	,FirstPage(true)
	,NextPage(false)
{
	mnPause = 1;
	mpTexture = new Texture("Resource/wallJ.png", VGet(450, 500, 0), true);
	mpTexture2 = new Texture("Resource/Ope.png", VGet(300, 250, 0), true);
	mpTexture3= new Texture("", VGet(300, 250, 0), true);
}

OperationScene::~OperationScene()
{


}
void OperationScene::Initialize()
{
	/*filename1 = "Resource/ruleB1.jpg";
	mnHandle1 = LoadGraph(filename1.c_str());*/
	/*new Texture("Resource/ruleA.jpg", VGet(500, 900, 0), true);
	new Texture("Resource/ruleB.jpg", VGet(300, 300, 0), true);*/


}

void OperationScene::Update()
{
	if (InputManager::CheckDownKey(KEY_INPUT_BACK))
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
	}


}

void OperationScene::Draw()
{
	
	
	
	mpTexture->Draw();
	int Color = GetColor(255, 255, 255);
	SetFontSize(40);
	DrawFormatString(150, 150, GetColor(0, 0, 255), "/////移動キー/////");
	DrawFormatString(150, 190, Color, "WASD:移動キー");
	DrawFormatString(150, 190, GetColor(255, 0, 0), "\n/////アタックキー////");
	DrawFormatString(150, 190, Color, "\n\nMouseLeft:アタック");
	DrawFormatString(150, 190, Color, "\n\n\nSHIFT:回避");
	DrawFormatString(150, 190, Color, "\n\n\n\nE:攻撃方法チェンジ");
	DrawFormatString(150, 190, GetColor(0, 255, 0), "\n\n\n\n\n/////そのほか/////");
	DrawFormatString(150, 190, Color, "\n\n\n\n\n\nO:インベントリ");
	DrawFormatString(150, 190, Color, "\n\n\n\n\n\n\nQ:視点変更");
	DrawFormatString(150, 190, GetColor(255, 0, 0), "\n\n\n\n\n\n\n\n死んだらお金を少しなくす");
	DrawFormatString(150, 190, GetColor(255,255, 0), "\n\n\n\n\n\n\n\n\n///クリア条件///");
	DrawFormatString(150, 190, GetColor(255, 255, 0), "\n\n\n\n\n\n\n\n\n\nbossを二回倒す");
	DrawFormatString(700, 900, GetColor(255,255,255), "backspaceで戻る");
	SetFontSize(size);
}

void OperationScene::Finalize()
{

}