#include"Tree.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Master.h"
#include"Stage.h"
#include"Scene.h"
#include"CapsuleCollider.h"

Tree::Tree(std::string filename, VECTOR initPos,int Size,float getSize,bool HitFlag)
	:Object3D(initPos)
	,mfSize(getSize)//あたり判定のっサイズ
{
	mpModel = new Model(filename, initPos);
	mpModel->SetScale(VGet(Size, Size, Size));
	SetTag(Object3D::Tag3D_Obj);

	mvPosition = initPos;
	mnHitFlag = HitFlag;
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
}
Tree::~Tree()
{
	delete mpModel;
}

void Tree::Update()
{
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;
	auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Stage);
	if (obj != nullptr)
	{
		Stage* pStage = dynamic_cast<Stage*>(obj);
		if (pStage != nullptr)
		{
			//ステージとプレイヤーのカプセルが当たっている場合
			if (pStage->CheckHit_Capsule(VAdd(mvPosition, VGet(0.0f, -150.0f, 0.0f)), VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
			{
				//当たっているであろうポリゴンとの接触点を求める
				hitPos = pStage->CheckHit_Line(
					VAdd(mvPosition, VGet(0.0f, 1000.0f, 0.0f)),//プレイヤーの膝当たり(多分)と
					VAdd(mvPosition, VGet(0.0f, -1000.0f, 0.0f))//プレイヤーの少し下あたりを線分として指定
				);

				//当たった判定を取っておく
				isHit = true;
			}
		}
	}

	if (isHit)
	{
		//地面に沿って歩いている状態として、Y座標をステージに合わせる
		mvPosition.y = hitPos.y;
	}
	if (isHit == false)
	{
		//要改善
		mvPosition.y += -8.0f;//落下する速度
		if (mvPosition.y <= 0.0f || mvPosition.y <= hitPos.y)
		{
			mvPosition.y = hitPos.y;
		}
	}
	mpModel->Update();

}

void Tree::Draw()
{
	
	/*DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, mnSize, 0.0f)),
		mnSize,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);*/
	mpModel->Draw();
}

void Tree::OnEnter(Collider* collider, Collider* check)
{
	
}
void Tree::OnTrigger(Collider* collider, Collider* check)
{

}
void Tree::OnExit(Collider* collider, Collider* check)
{

}
