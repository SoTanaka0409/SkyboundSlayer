#include"Object3D.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Scene.h"
#include"Tree.h"
#include"Stage.h"
#include"wall.h"



Object3D::Object3D(VECTOR initPos)
	:mvPosition(initPos)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, mbDeleteFlag(false)
	, mnTag(Tag3D::None3D)
	,mbDrawFlag(true)
{
	//現在のシーンのobjectManagerに自信（this)を追加する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

//デストラクタ
Object3D::~Object3D()
{

}
//描画
void Object3D::Draw()
{

}

//更新
void Object3D::Update()
{

}

void Object3D::OnEnter(Collider* collider, Collider* check)
{

}

void Object3D::OnTrigger(Collider* collider, Collider* check)
{

}

void Object3D::OnExit(Collider* collider, Collider* check)
{

}

