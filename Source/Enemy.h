#pragma once
#pragma once
#include "DxLib.h"
#include "Texture.h"
#include"Object3D.h"
#include"Model.h"
#include"Debug.h"


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

	virtual CapsuleCollider* GetEnemycoll() { return mpCapsuleCollider; }//邵ｺ阮呻ｽ檎ｸｺ謔滓ｬ｡陜暦ｿｽ繝ｻ貅倥″郢晏干縺晉ｹ晢ｽｫ郢ｧ・ｳ郢晢ｽｩ郢ｧ・､郢敖郢晢ｽｼ邵ｺ・ｽ邵ｺ隨ｬ・ｶ蛹ｻ竏ｴ邵ｺ・ｪ邵ｺ繝ｻ

private:
	bool mbHitSizeflag;//陟冶侭笳・ｹｧ髮∵・陞ｳ螟ら舞邵ｺ・ｮ郢ｧ・ｵ郢ｧ・､郢ｧ・ｺ郢ｧ蜻域亜邵ｺ・｣邵ｺ・ｦ邵ｺ荳奇ｽ狗ｸｺ荵昶・邵ｺ繝ｻﾂｰ邵ｺ・ｮflag?
	bool mbNewEnemyflag;//隰ｨ・ｵ郢ｧ蝨ｰew邵ｺ蜷ｶ・狗ｸｺ貅假ｽ∫ｸｺ・ｮ郢晁ｼ釆帷ｹｧ・ｰ繝ｻ貊会ｽｽ・ｿ邵ｺ繝ｻ蟀ｿ邵ｺ・ｯ郢ｧ蛹ｻ・･郢ｧ荳環ｰ郢ｧ蟲ｨ竊醍ｸｺ繝ｻ

public:
	Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2,float Serch3,int money,bool isSeparateAnim);

	~Enemy();

	void Draw()override;

	void Update()override;

	virtual void Move();
    virtual void RotationByMove();//驕假ｽｻ陷崎ｼ披・郢ｧ蛹ｻ・玖摎讚・ｽｻ・｢陷・ｽｦ騾・・
	virtual void Damage(float damage);
	virtual void AttackList();
	virtual void Attack();

	virtual void Delete();
	virtual void DeathColliderPosition();


	virtual void DeathEnemy();
	virtual void GiveRewards();
	virtual void CollPositionUpdate();


	float GetHp() { return hp_; };
	void SetHp(float hp) { hp_ = hp; };

	float GetMaxHp() { return max_hp_; }
	void SetMaxHp(float mhp) { max_hp_ = mhp; }

	bool GetInvisible() { return mbInvisible; }
	void SetInvisible(int Inv) { mbInvisible = Inv; }

	VECTOR GetGoPlayer() { return GoPosition; }
	void SetGoPlayer(VECTOR goplayer) { GoPosition = goplayer; }

	void SetVPosition(VECTOR pos) { VinitPos = pos; }//陋ｻ譎・ｄ郢晢ｽｪ郢ｧ・ｹ郢晄亢繝ｻ郢晢ｽｳ陜ｨ・ｰ霓､・ｹ邵ｺ・ｮ陟趣ｽｧ隶灘生・定愾謔ｶ笆ｲ邵ｺ・ｦ邵ｺ鄙ｫ・･
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
	SphereCollider* mpAttackCollider;//隰ｾ・ｻ隰ｦ繝ｻ・陝倶ｹ晢ｽ∫ｹｧ蜿･繝ｻ
	SphereCollider* mpStopCollider;
	Model* model_;
	Debug* mpDebug;
protected:
	float hp_;
	float max_hp_;
	float speed_;
	float mfNormalSpeed;
	float mfMaxSpeed;
	float attack_;

	int WalkTimer;//陷ｷ蠕個ｧ陞ｳ譎擾ｽｺ・ｫ邵ｺ・ｫ雎・ｽｩ邵ｺ蜥ｲ・ｶ螢ｹ・郢ｧ蛹ｺ蜃ｾ鬮｢繝ｻ
	int WalkCount;//

	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR GoPosition;//陷ｷ莉｣ﾂｰ邵ｺ繝ｻ繝ｻ郢ｧ・ｯ郢晏現ﾎ昴・蛹ｻ繝ｻ郢晢ｽｬ郢ｧ・､郢晢ｽ､郢晢ｽｼ郢ｧ蜻遺楳驕擾ｽ･邵ｺ蜉ｱ笳・ｸｺ・ｨ邵ｺ繝ｻ
	VECTOR oldPosition;
	VECTOR VinitPos;//spown陜ｨ・ｰ霓､・ｹ
	VECTOR NoPosition;
	VECTOR moveVec;
	VECTOR UpMoveVector;
	VECTOR leftMoveVector;
	bool mbInvisible;//霎滂ｽ｡隰ｨ・ｵ邵ｺ荵昶・邵ｺ繝ｻﾂｰ
	int mnAlgorithm;//郢晢ｽｩ郢晢ｽｳ郢敖郢晢ｿｽ驕假ｽｻ陷阪・
	int AlgHit;

	int AttackInterval;
	int AttackCount;

	bool Animation;
	float mfTargetAngle;//騾ｶ・ｮ隶灘生繝ｻ陜玲ｫ・ｽｻ・｢陜ｨ・ｰ
	float mfAngle;//霑ｴ・ｾ陜ｨ・ｨ邵ｺ・ｮ陜玲ｫ・ｽｻ・｢陜ｨ・ｰ
	const float ROTATE_SPEED = 0.1f;//陜玲ｫ・ｽｻ・｢鬨ｾ貅ｷ・ｺ・ｦ

	float mfSize;//隰ｨ・ｵ邵ｺ・ｮ郢ｧ・ｵ郢ｧ・､郢ｧ・ｺ
	float HitSerch;//髴台ｻ｣・･邵ｺ・ｫ郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ蠕鯉ｼ樒ｹｧ荵敖ｰ
	float HitAttackSerch;//髴台ｻ｣・･邵ｺ・ｫ郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ蠕鯉ｼ樒ｹｧ荵敖ｰ&隰ｾ・ｻ隰ｦ繝ｻ縲堤ｸｺ髦ｪ・矩恪譎槫ｱｬ邵ｺ繝ｻ
	float HitStopSerch;//髴台ｻ｣・･邵ｺ・ｫ郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ蠕鯉ｼ樒ｹｧ繝ｻ鬩包ｽｩ陋ｻ繝ｻ竊鷹恪譎槫ｱｬ邵ｺ繝ｻ
	bool isDead;

	bool isHitSearch;
	bool isHitAttackSearch;
	bool isHitSerchStop;
	bool isHitAttack;//隰ｾ・ｻ隰ｦ繝ｻ窶ｲ陟冶侭笳・ｸｺ・｣邵ｺ貅伉ｰ

	int mnChance;//inventory郢ｧ螳夊ｪ邵ｺ・ｨ邵ｺ蜥擾ｽ｢・ｺ驍・・
	
	bool mbItem=true;//item郢ｧ螳夊ｪ邵ｺ・ｨ邵ｺ蜷ｶﾂｰ陷ｷ・ｦ邵ｺ繝ｻ

	bool AttackHitJudgmentflag;//隰ｾ・ｻ隰ｦ繝ｻ窶ｲ闕ｳﾂ陜玲ｧｫ・ｽ阮吮螺邵ｺ・｣邵ｺ貅假ｽ臥ｸｺ譏ｴ繝ｻ隰ｨ・ｵ邵ｺ・ｫ陝・ｽｾ邵ｺ蜉ｱ窶ｻ隰ｾ・ｻ隰ｦ繝ｻ窶ｲ鬩･蟠趣ｽ､繝ｻ・邵ｺ・ｪ邵ｺ繝ｻ・育ｸｺ繝ｻ竊鍋ｸｺ蜷ｶ・・

	bool HitJudgmentflag_Player;
	int mfHaveMoney;
	/// <summary>
	/// ///////////郢昶・ﾎ礼ｹ晢ｽｼ郢晏現ﾎ懃ｹｧ・｢郢晢ｽｫ
	/// </summary>

	

};

