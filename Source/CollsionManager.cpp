#include"CollsionManager.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Stage.h"

CollsionManager::CollsionManager()
{

}

CollsionManager::~CollsionManager()
{

}

void CollsionManager::Update()//ここに当たり判定を書いていく
{
	
	//VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	////ステージとの当たり判定をする
	//hitPos = VGet(0.0f, 0.0f, 0.0f);
	//bool isHit = false;

	//auto StageColl = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Stage);//ステージの情報を持ってくる
	//auto CharactorColl = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_charactor);//指定したキャラクターのクラスを持ってくる
	//
	//if (StageColl != nullptr)
	//{
	//	Stage* pStage = dynamic_cast<Stage*>(StageColl);
	//	if (pStage != nullptr)
	//	{
	//		for (int i = 0; i < CharactorColl.size(); i++)//指定したキャラ全部」
	//		{
	//			auto pChara = dynamic_cast<Object3D*>(CharactorColl.at(i));
	//			//ステージとプレイヤーのカプセルが当たっている場合
	//			if (pStage->CheckHit_Capsule(pChara->GetPosition(), VAdd(pChara->GetPosition(), VGet(0.0f, 150.0f, 0.0f)), 40.0f))
	//			{
	//				//当たっているであろうポリゴンとの接触点を求める
	//				hitPos = pStage->CheckHit_Line(
	//					VAdd(pChara->GetPosition(), VGet(0.0f, 150.0f, 0.0f)),//enemyの膝当たり(多分)と
	//					VAdd(pChara->GetPosition(), VGet(0.0f, -40.0f, 0.0f))//enemyの少し下あたりを線分として指定
	//				);

	//				//当たった判定を取っておく
	//				isHit = true;
	//			}
	//			if (isHit)
	//			{
	//				//地面に沿って歩いている状態として、Y座標をステージに合わせる
	//				pChara->SetPosition(VAdd(pChara->GetPosition(),VGet(0,hitPos.y,0)));
	//			}
	//			if (isHit == false)
	//			{
	//				//要改善
	//				pChara->SetPosition(VAdd(pChara->GetPosition(),VGet(0,-4.0f,0)));//落下する速度
	//				if (pChara->GetPosition().y <= 0.0f)
	//				{
	//					pChara->SetPosition(pChara->GetOldPosition());
	//				}
	//			}
	//		}
	//	}
	//}

	


}
void CollsionManager::Draw()
{

}

void CollsionManager::CollSphere(VECTOR center1,float centerR,VECTOR pos1, VECTOR pos2, float R)
{
	Hit=HitCheck_Sphere_Capsule(
		center1,
		centerR,
		pos1,
		pos2,
		R
	);
	if (Hit)
	{
		//ここに書くべきかわからない.
	}

	

}

void CollsionManager::AddCollObject(Collision* coll)
{
	mColl.push_back(coll);
}

//std::vector<Collision*>CollsionManager::GetObject3DCollListByTag(Collision::Tag_Coll tag)
//{
//	std::vector<Collision*>ret;
//
//	for (auto itr = mColl.begin(); itr != mColl.end(); itr++)
//	{
//		//tagとおなじタグを持っているオブジェクトがあればvectorに入れる
//		if ((*itr)->GetTag() == tag)
//		{
//			ret.push_back(*itr);
//		}
//	}
//
//	return ret;
//}