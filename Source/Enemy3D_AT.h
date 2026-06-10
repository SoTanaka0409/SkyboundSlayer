#pragma once
#include"Dxlib.h"
#include"Object3D.h"

#include"DrawHp.h"
#include"Debugh.h"
#include"EnemyManager.h"
#include"Enemy.h"
#include"Collision.h"

class Model;

class Enemy3D_AT :public Enemy
{
public:
	Enemy3D_AT(std::string filename, VECTOR initPos, float hp, float speed,float HitSize, float Serch1, float Serch2,float Serch3,float Attack,float xp,int money,bool isSeparateAnim);

	~Enemy3D_AT();

	void Draw()override;

	void Update()override;

	

	//void RotationByMove();
	//void Delete()override;

	//void DeathEnemy()override;
	

	//void Delete();


	//void AddAnimation(AnimationState state, std::string filename);

	
private:
	Debug* mpDebug;

	//SphereCollider* mpAttachCollider;//UŒ‚‚Ì“–‚½‚è”»’è‚Ì‰~
	//CapsuleCollider* mpCapsuleCollider;//©g‚Ì“–‚½‚è”»’è‚Ì‰~
	//SphereCollider* mpSerchCollider;//“G‚ğŒ©‚Â‚¯‚é‚½‚ß‚Ì‰~
	//SphereCollider* mpAttackCollider;//UŒ‚‚µn‚ß‚é‰~
	//SphereCollider* mpStopCollider;//‚ ‚é’ö“x‚Ì‹——£‚Å—§‚¿~‚Ü‚é‰~
	
	//DrawHp* mpDH;

	
	
	//float mnHp;

	/// <summary>
	/// ///////////ƒ`ƒ…[ƒgƒŠƒAƒ‹
	/// </summary>




};