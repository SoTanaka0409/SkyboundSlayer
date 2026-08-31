#include "Collision.h"
#include"Master.h"
#include"SceneManager.h"




/// @brief Collisionの初期化（コンストラクタ）
Collision::Collision()
{
	//Master::scene_manager_->GetCurrentScene()->GetCollisionManager()
}
Collision::~Collision()
{

}


/// @brief Collisionの状態更新処理
void Collision::Update()
{
	for (auto list = size_list_.begin(); list != size_list_.end(); list++)
	{

	}

}


/// @brief Collisionの描画処理
void Collision::Draw()
{

}

