#include"Object2D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Scene.h"



Object2D::Object2D(VECTOR initPos)
	:position_(initPos)
	, rotation_(VGet(0.0f, 0.0f, 0.0f))
	, delete_flag_(false)
	, tag_(Tag2D::None2D)
	, draw_flag_(true)
{
	//現在のシーンのobjectManagerに自信（this)を追加する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

//デストラクタ
Object2D::~Object2D()
{

}
//描画
void Object2D::Draw()
{

}

//更新
void Object2D::Update()
{

}
