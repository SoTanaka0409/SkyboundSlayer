#include "Collision.h"
#include"Master.h"
#include"SceneManager.h"




/// @brief Collisionの初期化（コンストラクタ）
Collision::Collision()
{
	//Master::m_SceneManager->GetCurrentScene()->GetCollisionManager()
}
Collision::~Collision()
{

}


/// @brief Collisionの状態更新処理
void Collision::Update()
{
	for (auto list = m_SizeList.begin(); list != m_SizeList.end(); list++)
	{

	}

}


/// @brief Collisionの描画処理
void Collision::Draw()
{

}

