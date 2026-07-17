#include"haikei.h"
#include"Object2D.h"
#include"ObjectManager.h"
#include"Master.h"
#include"Scene.h"
#include"InputManager.h"


/*
 * 目的（haikeiのhaikei処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
haikei::haikei(VECTOR initPos, std::string filename)
	:Object2D(initPos)

{
	texture_ = new Texture("", initPos, true);
}
haikei::~haikei()
{
}


/*
 * 目的（haikeiのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void haikei::Update()
{
	Object2D::Update();
	texture_->Update();
}


/*
 * 目的（haikeiのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void haikei::Draw()
{
	texture_->Draw();
	Object2D::Draw();
}

