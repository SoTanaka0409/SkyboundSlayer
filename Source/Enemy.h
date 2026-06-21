#pragma once
#pragma once
#include "DxLib.h"
#include "Texture.h"
#include"Object3D.h"
#include"Model.h"
#include"Debugh.h"

#include"WeaponManager.h"

#include"Item.h"
#include"ColliderManager.h"
#include"EquipmentManager.h"

class SphereCollider;
class CapsuleCollider;


class Enemy :public Object3D
{
public:
	void SetHitSizeFlag(bool flag) { mbHitSizeflag = flag; }
	bool GetHitSizeFlag() { return mbHitSizeflag; }

	void SetNewEnemyFlag(bool flag) { mbNewEnemyflag = flag; }
	bool GetNewEnemyFlag() { return mbNewEnemyflag; }

	virtual CapsuleCollider* GetEnemycoll() { return mpCapsuleCollider; }//これが原因？カプセルコライダーだけ消えない

private:
	bool mbHitSizeflag;//当たり判定用のサイズを持ってくるかどうかのflag?
	bool mbNewEnemyflag;//敵をnewするためのフラグ？使い方はよくわからない

public:
	Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2,float Serch3, float xp,int money,bool isSeparateAnim);

	~Enemy();

	void Draw()override;

	void Update()override;

	virtual void Move();
    virtual void RotationByMove();//移動による回転処理
	virtual void Damage(float damage);
	virtual void AttackList();
	virtual void Attack();

	virtual void Delete();
	virtual void DeathColliderPosition();


	virtual void DeathEnemy();
	virtual void CollPositionUpdate();


	float GetHp() { return mfHp; };
	void SetHp(float hp) { mfHp = hp; };

	float GetMaxHp() { return mfMaxHp; }
	void SetMaxHp(float mhp) { mfMaxHp = mhp; }

	bool GetInvisible() { return mbInvisible; }
	void SetInvisible(int Inv) { mbInvisible = Inv; }

	VECTOR GetGoPlayer() { return GoPosition; }
	void SetGoPlayer(VECTOR goplayer) { GoPosition = goplayer; }

	void SetVPosition(VECTOR pos) { VinitPos = pos; }//初期リスポーン地点の座標を取っておく
	VECTOR GetVPosition() { return VinitPos; }

	float GetSize() { return mfSize; }

	void SetHitjudgment(bool flag) { HitJudgmentflag_Player=flag; }
	bool GetHitjudgment() { return HitJudgmentflag_Player; }

	bool GetisDead() { return isDead; }
	void SetIsDead(bool dead) { isDead = dead; }

	virtual void OnEnter(Collider* collider, Collider* check) ;
	virtual void OnTrigger(Collider* collider, Collider* check);
	virtual void OnExit(Collider* collider, Collider* check) ;





private:
	

protected:
	
	SphereCollider* mpAttachCollider;
	CapsuleCollider* mpCapsuleCollider;
	SphereCollider* mpSerchCollider;
	SphereCollider* mpAttackCollider;//攻撃し始める円
	SphereCollider* mpStopCollider;
	Model* mpModel;
	Debug* mpDebug;
protected:
	float mfHp;
	float mfMaxHp;
	float mfSpeed;
	float mfNormalSpeed;
	float mfMaxSpeed;
	float mfAttack;

	int WalkTimer;//同じ宝庫に歩き続ける時間
	int WalkCount;//

	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR GoPosition;//向かうベクトル（プレイヤーを感知したとき)
	VECTOR oldPosition;
	VECTOR VinitPos;//spown地点
	VECTOR NoPosition;
	VECTOR moveVec;
	VECTOR UpMoveVector;
	VECTOR leftMoveVector;
	bool mbInvisible;//無敵かどうか
	int mnAlgorithm;//ランダム移動
	int AlgHit;

	int AttackInterval;
	int AttackCount;

	bool Animation;
	float mfTargetAngle;//目標の回転地
	float mfAngle;//現在の回転地
	const float ROTATE_SPEED = 0.1f;//回転速度

	float mfSize;//敵のサイズ
	float HitSerch;//近くにプレイヤーがいるか
	float HitAttackSerch;//近くにプレイヤーがいるか&攻撃できる距離か
	float HitStopSerch;//近くにプレイヤーがいる&適切な距離か
	bool isDead;

	bool isHitSearch;
	bool isHitAttackSearch;
	bool isHitSerchStop;
	bool isHitAttack;//攻撃が当たったか

	int mnChance;//inventoryを落とす確率
	bool mbWeapon =false;//weaponを落とすか否か
	bool mbItem=true;//itemを落とすか否か

	bool AttackHitJudgmentflag;//攻撃が一回当たったらその敵に対して攻撃が重複しないようにする

	bool HitJudgmentflag_Player;
	float mfHaveXp;
	int mfHaveMoney;
	/// <summary>
	/// ///////////チュートリアル
	/// </summary>

	

};
