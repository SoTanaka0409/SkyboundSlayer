#pragma once
#pragma once
#include "DxLib.h"
#include "Texture.h"
#include"Object3D.h"
#include"Model.h"
#include"Debugh.h"


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

	virtual CapsuleCollider* GetEnemycoll() { return mpCapsuleCollider; }//縺薙ｌ縺悟次蝗�・溘き繝励そ繝ｫ繧ｳ繝ｩ繧､繝繝ｼ縺�縺第ｶ医∴縺ｪ縺・

private:
	bool mbHitSizeflag;//蠖薙◆繧雁愛螳夂畑縺ｮ繧ｵ繧､繧ｺ繧呈戟縺｣縺ｦ縺上ｋ縺九←縺・°縺ｮflag?
	bool mbNewEnemyflag;//謨ｵ繧地ew縺吶ｋ縺溘ａ縺ｮ繝輔Λ繧ｰ・滉ｽｿ縺・婿縺ｯ繧医￥繧上°繧峨↑縺・

public:
	Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2,float Serch3, float xp,int money,bool isSeparateAnim);

	~Enemy();

	void Draw()override;

	void Update()override;

	virtual void Move();
    virtual void RotationByMove();//遘ｻ蜍輔↓繧医ｋ蝗櫁ｻ｢蜃ｦ逅・
	virtual void Damage(float damage);
	virtual void AttackList();
	virtual void Attack();

	virtual void Delete();
	virtual void DeathColliderPosition();


	virtual void DeathEnemy();
	virtual void GiveRewards();
	virtual void CollPositionUpdate();


	float GetHp() { return mfHp; };
	void SetHp(float hp) { mfHp = hp; };

	float GetMaxHp() { return mfMaxHp; }
	void SetMaxHp(float mhp) { mfMaxHp = mhp; }

	bool GetInvisible() { return mbInvisible; }
	void SetInvisible(int Inv) { mbInvisible = Inv; }

	VECTOR GetGoPlayer() { return GoPosition; }
	void SetGoPlayer(VECTOR goplayer) { GoPosition = goplayer; }

	void SetVPosition(VECTOR pos) { VinitPos = pos; }//蛻晄悄繝ｪ繧ｹ繝昴・繝ｳ蝨ｰ轤ｹ縺ｮ蠎ｧ讓吶ｒ蜿悶▲縺ｦ縺翫￥
	VECTOR GetVPosition() { return VinitPos; }

	float GetSize() { return mfSize; }

	void SetHitjudgment(bool flag) { HitJudgmentflag_Player=flag; }
	bool GetHitjudgment() { return HitJudgmentflag_Player; }

	bool GetisDead() { return isDead; }
	void SetIsDead(bool dead) { isDead = dead; }

	bool GetIsHitSearch() const { return isHitSearch; }


	virtual void OnEnter(Collider* collider, Collider* check) ;
	virtual void OnTrigger(Collider* collider, Collider* check);
	virtual void OnExit(Collider* collider, Collider* check) ;





private:
	

protected:
	
	SphereCollider* mpAttachCollider;
	CapsuleCollider* mpCapsuleCollider;
	SphereCollider* mpSerchCollider;
	SphereCollider* mpAttackCollider;//謾ｻ謦・＠蟋九ａ繧句・
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

	int WalkTimer;//蜷後§螳晏ｺｫ縺ｫ豁ｩ縺咲ｶ壹￠繧区凾髢・
	int WalkCount;//

	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR GoPosition;//蜷代°縺・・繧ｯ繝医Ν・医・繝ｬ繧､繝､繝ｼ繧呈─遏･縺励◆縺ｨ縺・
	VECTOR oldPosition;
	VECTOR VinitPos;//spown蝨ｰ轤ｹ
	VECTOR NoPosition;
	VECTOR moveVec;
	VECTOR UpMoveVector;
	VECTOR leftMoveVector;
	bool mbInvisible;//辟｡謨ｵ縺九←縺・°
	int mnAlgorithm;//繝ｩ繝ｳ繝繝�遘ｻ蜍・
	int AlgHit;

	int AttackInterval;
	int AttackCount;

	bool Animation;
	float mfTargetAngle;//逶ｮ讓吶・蝗櫁ｻ｢蝨ｰ
	float mfAngle;//迴ｾ蝨ｨ縺ｮ蝗櫁ｻ｢蝨ｰ
	const float ROTATE_SPEED = 0.1f;//蝗櫁ｻ｢騾溷ｺｦ

	float mfSize;//謨ｵ縺ｮ繧ｵ繧､繧ｺ
	float HitSerch;//霑代￥縺ｫ繝励Ξ繧､繝､繝ｼ縺後＞繧九°
	float HitAttackSerch;//霑代￥縺ｫ繝励Ξ繧､繝､繝ｼ縺後＞繧九°&謾ｻ謦・〒縺阪ｋ霍晞屬縺・
	float HitStopSerch;//霑代￥縺ｫ繝励Ξ繧､繝､繝ｼ縺後＞繧・驕ｩ蛻・↑霍晞屬縺・
	bool isDead;

	bool isHitSearch;
	bool isHitAttackSearch;
	bool isHitSerchStop;
	bool isHitAttack;//謾ｻ謦・′蠖薙◆縺｣縺溘°

	int mnChance;//inventory繧定誠縺ｨ縺咏｢ｺ邇・
	
	bool mbItem=true;//item繧定誠縺ｨ縺吶°蜷ｦ縺・

	bool AttackHitJudgmentflag;//謾ｻ謦・′荳蝗槫ｽ薙◆縺｣縺溘ｉ縺昴・謨ｵ縺ｫ蟇ｾ縺励※謾ｻ謦・′驥崎､・＠縺ｪ縺・ｈ縺・↓縺吶ｋ

	bool HitJudgmentflag_Player;
	float mfHaveXp;
	int mfHaveMoney;
	/// <summary>
	/// ///////////繝√Η繝ｼ繝医Μ繧｢繝ｫ
	/// </summary>

	

};

