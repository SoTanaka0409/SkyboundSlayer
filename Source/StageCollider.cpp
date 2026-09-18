#include"StageCollider.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"Master.h"
#include"ObjectManager.h"
#include"SceneManager.h"
#include"Effect.h"
#include"Config.h"
#include"Player3D.h"


/// @brief StageColliderの初期化（コンストラクタ）
StageCollider::StageCollider()
	:Object3D(VGet(0,1000,0))
	,Time(50)
	,Count(0)
{
	
	go_boss_collider_ = new SphereCollider(this,VAdd( Config::GetStageCenter(),VGet(0,0,-2000)), 100);
	
	

	texture_ = new Texture("Resource/image/area/01_boss_area.png",VGet(400,400,0),true);
	texture2_ = new Texture("Resource/image/area/02_normal_area.png", VGet(400, 400, 0), true);

}

StageCollider::~StageCollider()
{

	if (go_boss_collider_ != nullptr)
	{
		go_boss_collider_->SetDeleteFlag(true);
	}
	

	SetDeleteFlag(true);
}


/// @brief StageColliderの描画処理
void StageCollider::Draw()
{
	
	
}


/// @brief StageColliderの状態更新処理
void StageCollider::Update()
{

}


/// @brief StageColliderのOnEnter処理
void StageCollider::OnEnter(Collider* collider, Collider* check)
{
	


}


/// @brief StageColliderのOnTrigger処理
void StageCollider::OnTrigger(Collider* collider, Collider* check)
{
	
	

		if (collider == go_boss_collider_ && check->parent_object_->GetTag() == Tag3D_Player3D)//normalstageに行く
		{
			
			Player3D* player = Master::player_;
			if (player == nullptr) return;
			texture2_->Draw();
			if (check == player->GetCollisionCollider())
			{
				player->SetPosition(VAdd(Config::GetStageBossCenter(), VGet(500.0f, 0,-2000)));//場所を移勁
				Master::sound_manager_->PlaySE(SoundManager::SE_WARP);//warp音を鳴らす
				/*new Effect(VGet(17000, 0, 16000), "Resource/image/battle/01_damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);
				new Effect(VGet(16000, 0, 17000), "Resource/image/battle/01_damage.png", GetColorU8(0, 255, 30, 0), 500.0f, 2.5f);*/
			}
		}
	
	
	

}


/// @brief StageColliderのOnExit処理
void StageCollider::OnExit(Collider* collider, Collider* check)
{
	
}


/// @brief StageColliderのColliderUpdate処理
void StageCollider::ColliderUpdate()
{
	
}
