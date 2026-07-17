#include "Collision.h"
#include"Master.h"
#include"SceneManager.h"




/*
 * 目的（CollisionのCollision処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
Collision::Collision()
{
	//Master::scene_manager_->GetCurrentScene()->GetCollisionManager()
}
Collision::~Collision()
{

}


/*
 * 目的（CollisionのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collision::Update()
{
	for (auto list = size_list_.begin(); list != size_list_.end(); list++)
	{

	}

}


/*
 * 目的（CollisionのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void Collision::Draw()
{

}

