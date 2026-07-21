#include"Object2D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Scene.h"




/// @brief Object2Dの初期化（コンストラクタ）
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

/// @brief Object2Dの描画処理
void Object2D::Draw()
{

}

//更新

/// @brief Object2Dの状態更新処理
void Object2D::Update()
{

}
