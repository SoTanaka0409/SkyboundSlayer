#include"Object2D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Scene.h"




/*
 * 目的（Object2DのObject2D処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Object2D::Object2D(VECTOR initPos)
	:position_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag2D::None2D)
	, draw_flag_(true)
{
	//現在のシーンのobjectManagerに自信（this)を追加する
	Master::scene_manager_->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

//デストラクタ
Object2D::~Object2D()
{

}
//描画

/*
 * 目的（Object2DのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Object2D::Draw()
{

}

//更新

/*
 * 目的（Object2DのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Object2D::Update()
{

}
