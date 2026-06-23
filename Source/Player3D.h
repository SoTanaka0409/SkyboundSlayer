#pragma once
#include"DxLib.h"
#include<string>
#include"Object3D.h"
#include"Model.h"

#include"Weapon.h"
#include"camera.h"
#include"WeaponManager.h"
#include"Shield.h"
#include"Camera1.h"
#include"Texture.h"
#include"Debugh.h"
#include"EffectU.h"
#include"EnemyManager.h"
#include"LevelUp.h"
#include"Collision.h"
#include"ItemManager.h"
#include"ShortInventory.h"
#include"InfClassManager.h"
#include"BuffManager.h"
#include"HaveMoneyClass.h"
#include"EquipmentManager.h"

class SphereCollider;
class CapsuleCollider;

class Player3D :public Object3D
{
public:
	enum AttackStateInfo {
		Attack_Normal,
		Attack_Jump,
		Attack_Slide,
	}AttackState;
	
public:
	// 分割アニメーションを使用するかの設定を追加（デフォルトはfalse（使用しない））
	Player3D(std::string filename, VECTOR initPos, float junppower, float speed, float hp, bool isSeparateAnim = false);

	~Player3D();

	void Draw()override;

	void Update()override;
	void ManagerUpdate();

	void if_StageOut();
	void MoveEx();

	void SelectAttack();
	void Attack();
	void AttackSlide();
	void AttackJump();
	void Damage(float damage);
	void ResetNUETRAL();

	void SetInivisible(bool flag) { mbInvisible = flag; }
	bool GetInvisible() { return mbInvisible; }

	VECTOR movevec_Player() { return moveVec; }

	void Shot();
	void Jump();
	void Result();
	void Test();
	void ChangeView();
	void EnemySerch();
	void CollPositionUpdate();
	void AttackCountClass();
	void Evasion();
	void CollDelete();
	void mpTargetOn();//targetがいる状態か.これがないとtargetがいなくなった瞬間にnullになる


	float GetAngle() { return mfAngle; }

	void RotationByMove();//移動による回転処理

	float GetHp() { return mfHp; }
	void SetHp(float hp) { mfHp = hp; }
	float GetAttack() { return mfAttack; }
	void SetAttack(float attack) { mfAttack = attack; }
	float GetAllStatusState(Object3D::StatusState state);
	float GetSpeed() { return mfSpeed; }
	void SetSpeed(float speed) { mfSpeed = mfSpeed + speed; }
	float GetSize() { return mfSize; }
	
	// --- Upgrade Setters ---
	void AddUpgradeMaxHp(float add) { mfUpgradeMaxHp += add; }
	void AddUpgradeAttack(float add) { mfUpgradeAttack += add; }
	void AddUpgradeSpeed(float add) { mfUpgradeSpeed += add; }
	void AddUpgradeEvasionSpeed(float add) { mfUpgradeEvasionSpeed += add; }
	void AddUpgradeEvasionInvincibility(int add) { mnUpgradeEvasionInvincibility += add; }
	// -----------------------

	bool GetStageOutFlag() { return mbStageOutFlag; }

	void bar();


	// アニメーション追加
   // Modelクラスへの橋渡し関数
	void AddAnimation(AnimationState state, std::string filename);


	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

	CapsuleCollider* GetCollisionCollider() { return mpCapsuleCollider; }
	SphereCollider* Get500Collider() { return mpSerchEnemyCollider; }

	Model* mpModel;//モデルクラスのポインタ
	ShortInventory* mpShortInventory;
	BuffManager* mpBuffManager;
	ItemManager* mpItemManager;
	WeaponManager* mpWeaponManager;
	HaveMoneyClass* mpHaveMoney;
	EquipmentManager* mpEquipmentManager;
	LevelUp* mpLevelUp;
private:

	Weapon* mpWeapon;

	Shield* mpShield;
	
	Texture* mpTexture;
	Texture* mpTexture2;
	Debug* mpDebug;
	EffectU* mpEffectU;
	Object3D* mpTarget;
	




	bool Animation;
	int NewShieldCount;
	bool NewShieldFast;
	float mfTargetAngle;//目標の回転地
	float mfAngle;//現在の回転地

	const float ROTATE_SPEED = 0.2f;//回転速度
	const float JUMP_POWER = 30.0f;//ジャンプ力

	bool mbjump;//ジャンプモーションをするか否か
	bool mbjumpDown;//落下する座標にいったら下降をするフラグ
	bool HighPositionFlag;//目標座標に行ったときにtrue
	bool OnJumpCollider;
	bool mbAttackSlideSerch;

	float mfjumpPower;//ジャンプ力
	float mfSpeed;
	float mfEvasionSpeed;
	float mfDashSpeed;
	float mfNormalSpeed;
	float mfAttackSlideSpeed;
	float mfHp;
	float mfMaxHp;
	float mfRideOldHp;
	float mfSize;
	
	// --- Upgrade Stats from StatShop ---
	float mfUpgradeMaxHp = 0.0f;
	float mfUpgradeAttack = 0.0f;
	float mfUpgradeSpeed = 0.0f;
	float mfUpgradeEvasionSpeed = 0.0f;
	int mnUpgradeEvasionInvincibility = 0; // 追加無敵フレーム
	int mnInvincibleTimer = 0; // 現在の無敵タイマー
	// -----------------------------------

	int AttackSlideCount;//攻撃間隔をカウントする
	int AttackJumpCount;
	int AttackCount;

	int AttackSlideTime = 200;//攻撃可能時のクールタイム
	int AttackTime = 30;
	int AttackJumpTime=200;
	////////////////	攻撃力	

	float mfAttack;//ダメージを計算するときに使われる基本的な値
	float mfNormalAttack;//何らかの要因でmfAttackの値を変えた時に戻すための値
	float mfAttackjump;
	float mfAttackSlide;
	float mfAttackBullet;


	
	VECTOR slide = VGet(0.0f, 0.0f, 0.0f);//壁沿いベクトル
	VECTOR  UpMoveVector;
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//移動方向
	VECTOR oldmoveVec;
	VECTOR GoPosition;//移動方向
	VECTOR TargetPosition;//目標とする座標

	//ステージとの当たり判定をする
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR hitPos2 = VGet(0.0f, 0.0f, 0.0f);
	VECTOR WeaponHitPos;
	bool OkAttackCount;//攻撃が連続で当たらないようにする


	bool Pause;
	bool EffectUflag;//エフェクトUが実行されたとき
	int EffectUCount;
	

	bool Gpush;
	int GpushCount;

	int mnPause;


	float DashGuage;
	float MaxDashGauge;


	bool mbInvisible;
	int BulletCount;//玉の打つ感覚
	int Interval;
	float mfHorizontalAngle;  //水平方向アングル
	float mfVerticalAngle;   // 垂直方向アングル

	int FirstNearVec;//一番近い敵
	float VECSizeFirst;
	float  VECSize;;

	VECTOR pos = { 30,880 };
	VECTOR size = { 300,40 };

	VECTOR pos2 = { 30,940 };
	VECTOR size2 = { 300,40 };

	VECTOR pos3 = { 30,820 };
	VECTOR size3 = { 300,40 };

	int redAlpha = 0;

	int ChangeCamera;


	int EneSerchCount;

	const int SIZE_RAND_MAX = 800;//サイズの最大乱数地
	const int SIZE_RAND_MIN = 400;//サイズの最小乱数地


	const int VISIBLE_TIME_RAND_MAX = 30;//表示時間の最大乱数地
	const int VISIBLE_TIME_RAND_MIN = 5;//表示時間の最小乱数地

	SphereCollider* mpAttachCollider;
	SphereCollider* mpSerchEnemyCollider;
	SphereCollider* mpAttackSlideCollider;
	SphereCollider* mpAttackJumpCollider;
	CapsuleCollider* mpCapsuleCollider;


	int ChangeStateCount;

	int mouseInput = GetMouseInput(); // マウスの状態を取得

	bool AttackHitJudgmentflag;

	bool mbStageOutFlag;//trueだったらoldPositionに戻るようにする
};
