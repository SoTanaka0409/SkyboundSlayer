#include"Player3D.h"
#include"Model.h"
#include"ModelAnimation.h"
#include"Master.h"
#include"InputManager.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Scene3D.h"
#include"Wall.h"
#include<string>
#include <iostream>
#include <cstring> // strcmp, strncmp
//#include"Effect.h"
#include"Enemy3D.h"
#include"stage.h"


#include"Camera.h"
#include"Camera1.h"
#include"Bush.h"
#include"WeaponManager.h"
#include"Weapon.h"

#include"Shield.h"

#include"Tree.h"
#include"Scene.h"
#include"Texture.h"
#include"Debugh.h"

#include"DrawHp.h"
#include"Effect.h"
#include"DrawCircle1.h"

#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include"InfClass.h"
#include"HaveMoneyClass.h"

// 分割アニメーションを使用するかの設定を追加（デフォルトはfalse（使用しない））
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool isSeparateAnim)
	:Object3D(initPos)
	, mfAttack(0)//ダメージ
	, mfAttackjump(5)
	, mfAttackSlide(7)

	,OnJumpCollider(false)//この瞬間に当たり判定をつける
	, mbInvisible(false)//攻撃が重複しないように
	, mbjump(false)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(speed)//speed
	, JUMP_POWER(jumppower)
	, mfHp(hp)//hp
	, mfMaxHp(hp)//maxhp
	, mfDashSpeed(speed * 2)
	, NewShieldFast(false)//シールドを生成するかどうか
	, mfSize(60.0f)//あたり判定
	, Pause(false)//ポーズ画面
	, ChangeCamera(0)//視点変更
	, DashGuage(100.0f)//
	, EneSerchCount(0)
	, Gpush(false)//必殺技をしていいか
	, GpushCount(2)//必殺技のカウント
	, EffectUflag(false)
	, EffectUCount(0)
	, mfRideOldHp(0)
	, mfjumpPower(150.0f)
	, mfAttackSlideSpeed(20.0f)

	, AttackSlideCount(0)
	, AttackJumpCount(0)
	,AttackCount(0)

	, ChangeStateCount(0)
	, mbjumpDown(false)
	, HighPositionFlag(false)
	, AttackHitJudgmentflag(false)
	, mbAttackSlideSerch(false)
	, mfEvasionSpeed(20.0f)
	,mbStageOutFlag(true)
	//, mnLose(false)
	//, mnWin(false)
{
	mpEffectU = new EffectU();
	mpEffectU->Load();


	SetTag(Object3D::Tag3D_Player3D);
	mpBuffManager = new BuffManager();
	mpWeaponManager = new WeaponManager();
	mpEquipmentManager = new EquipmentManager();
	mpInventory = new Inventory(Object3D::Tag3D_Player3D);
	mpLevelUp = new LevelUp();
	mpModel = new Model(filename, initPos, isSeparateAnim);
	mpHaveMoney = new HaveMoneyClass(30000);
	mpModel->AddAttachment("Resource/3D/Sabel.mv1", "wp");
	mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Slash.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKSLIDE, "Resource/Model/Attack.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/Model/Jump Attack.mv1");
	//mpModel->AddAnimation(ANIMATION_SLIDE, "Resource/Model/Slide.mv1");
	mpModel->AddAnimation(ANIMATION_SLIDE, "Resource/Model/kaihi.mv1");

	Master::mpCamera->Initialize();
	//mpCamerafa = Master::mpCamerafa;

	float HpRatio = (float)mfHp / mfMaxHp;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;
	{//初期装備
		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 1000;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon1;
		weaponInf->name = "weapon1";
		mpWeaponManager->AddWeapon(weaponInf);
		mpWeaponManager->ChangeWeapon(weaponInf);

		mfNormalAttack = weaponInf->damage;//最初に初期装備の攻撃力の初期化処理を行う
		mfAttack = weaponInf->damage;//最初に初期装備の攻撃力の初期化処理を行う
	}




	MaxDashGauge = DashGuage;

	//mpTarget = new Enemy3D("", VGet(20000, 0, 0), 0, 0, 0, 0, 0);
	
	//mpCamera->ResetCameraPlayer(true);
	 // コライダー生成
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 60.0f);
	mpAttackSlideCollider = new SphereCollider(this, mvPosition, 200.0f);
	mpSerchEnemyCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 500.0f);
	mpAttackJumpCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 300.0f);
	
	
	//WeaponHitPos = VAdd(mpModel->GetAttachmentPosition(), VGet(0.0f, 50.0f, 0.0f));
	
	AttackState = Attack_Normal;
	mpItemManager = Master::mpItemManager;
}

Player3D::~Player3D()
{
	delete mpModel;
	CollDelete();
}

void Player3D::Update()
{
	mpInventory->Update();//inventoryだけはここに
	if (Master::ShopClassOn || Master::InventoryClasOn)return;
	///////////////////////////////////////////////
	

	if (InputManager::CheckDownKey(KEY_INPUT_O)&&!Master::NearShopOn)
	{
		Master::InventoryClasOn = true;
		Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//効果音
	}
		
	
	if (Master::PauseOn == false)//ポーズ中は動かないようにする
	{
		Test();
		ManagerUpdate();
		if (InputManager::CheckDownKey(KEY_INPUT_Q))
		{
			ChangeView();
		}
		////////////////////////行動/////////////////
		SelectAttack();
		Result();
		ResetNUETRAL();
		//ジャンプ処理
		CollPositionUpdate();
		Evasion();
		//攻撃処理
		SelectAttack();
		//移動処理
		MoveEx();
		//回転処理
		RotationByMove();
		EnemySerch();

		//Interval++;
		//if (Interval >= 10)//銃を撃つインターバル
		//{
		//	Shot();
		//	Interval = 0;
		//}
		if (mpLevelUp->GetLevelUp())
		{ 
			Master::mpSoundManager->PlaySE(SoundManager::SE_LEVELUP);
			mfHp = GetAllStatusState(Object3D::Status_Hp);//hpを全回させる
		}
		mpModel->Update();
	}

}

void Player3D::ResetNUETRAL()
{
	AnimationState now = mpModel->GetNowState();
	if (now == ANIMATION_NEUTRAL)
	{
		if (mpModel->GetIsSeparate() == true)
		{
			mpModel->mpSeparateAnimation->SetAnimationCount(0.5f);
		}
		else
		{
			mpModel->mpAnimation->SetAnimationCount(0.5f);
		}


		FirstNearVec = 0;//攻撃してないときに敵との距離を測るカウントをリセット
		mpTarget = nullptr;//目標のターゲットをリセット

	}
}

void Player3D::ManagerUpdate()
{
	mpBuffManager->DeleteList();
	mpBuffManager->Update();
	mpLevelUp->Update();
	
}

void Player3D::Draw()
{

	if (mfHp <= 0)return;
	if (Master::ShopClassOn || Master::InventoryClasOn)return;
	if (Master::PauseOn == false)///ポーズ画面ではないとき
	{
		
		bar();
		
	}
	////////////////////////三人称視点の時のみプレイヤーが見えるようにする///////////////
	if (Master::mpCamera->GetCamera3() == true)
	{
		//モデルの描画
		mpModel->Draw();
	}

	if (Master::mpDebug->Getdebug() == true)
	{
		//でバック表示方法
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		DrawFormatString(100, 300, GetColor(255, 255, 255), "攻撃力:%f", GetAllStatusState(Object3D::Status_Attack));
		DrawFormatString(100, 400, GetColor(255, 255, 255), "防御力:%f", mpEquipmentManager->GetDamage());
		DrawFormatString(100, 500, GetColor(255, 255, 255), "sスピード:%f", GetAllStatusState(Object3D::Status_Speed));
		DrawFormatString(100, 350, GetColor(255, 255, 255), "経験値:%d", mpLevelUp->GetXp());
		DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f       Z:%f", mvPosition.x, mvPosition.y, mvPosition.z);
	}
	//武器を包むような球体
	DrawSphere3D(
		mpModel->GetAttachmentPosition(),
		30.0f,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

//移動処理ステージとの当たり判定
void Player3D::MoveEx()
{
	AnimationState state = mpModel->GetNowState();
	if (state==ANIMATION_ATTACKJUMP||state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE||state==ANIMATION_ATTACKSLIDE||Master::InventoryClasOn||Master::ShopClassOn)
	{
		return;//特定のモーション中であれば何もしない
	}

	moveVec = VGet(0.0f, 0.0f, 0.0f);//移動方向
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);//カメラの上方向ベクトル
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);//カメラの左方向ベクトル
	//VECTOR DownMoveVector = VGet(0.0f, 0.0f, 0.0f);//カメラの下方向ベクトル
	//VECTOR RightMoveVector = VGet(0.0f, 0.0f, 0.0f);//カメラの右方向ベクトル


	//カメラの向きから移動ベクトルを求める
	{
		//上方向への移動ベクトルは、カメラ視点方向からy成分を抜いたものとする
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());
		//UpMoveVector.y = 0.0f;

		//左方向への移動ベクトルが、上方向の移動ベクトルと、Y軸のプラス方向へのベクトルに垂直な方向（外積）
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));
		//leftMoveVector.y = 0.0f;


		//移動ベクトルは移動量を加味しないので、正規化しておく(ベクトルの長さを１にすること)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	//VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//移動方向

	if (CheckHitKey(KEY_INPUT_A))//左方向への移送
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))//右方向への移送
	{
		//veVec.x = 1.0f;
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));
		//moveVec = VSub(moveVec, leftMoveVector);

	}
	if (CheckHitKey(KEY_INPUT_W))//奥方向への移送
	{
		//veVec.z = 1.0;
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))//手前方向への移送
	{
		//moveVec.z = -1.0;
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));
		//moveVec = VSub(moveVec, UpMoveVector);

	}
	//移動している状態であれば
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{
		//mpModel->ChangeAnimation(ModelAnimation::ANIMATION_RUN);

		//移動方向を正規化しておく
		moveVec = VNorm(moveVec);

		//新しい回転をセット
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;//ここで今キャラクタ―が向いている方向を出す。保存
	}
	//ジャンプ開始のアニメじゃないのであれば
	if (state != ANIMATION_JUMP_IN || state != ANIMATION_JUMP_LOOP)
	{
		if (isMove)
		{
			mpModel->ChangeAnimation(ANIMATION_RUN);
		}
		else
		{
			mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
		}
	}



	//移動させる
	mvOldPosition = mvPosition;//前回の座標を一旦保持


	mvPosition = VAdd(mvPosition, VScale(moveVec, GetAllStatusState(Object3D::Status_Speed)));
	////////////////////////////////////////////////////////



		/////ステージとの当たり判定をする////////////
	hitPos = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;
	auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Stage);
	for (int i = 0; i < obj.size(); i++)
	{
		Stage* pStage = dynamic_cast<Stage*>(obj.at(i));
		if (pStage != nullptr)
		{
			
			if (pStage != nullptr)
			{
				//ステージとプレイヤーのカプセルが当たっている場合
				if (pStage->CheckHit_Capsule(VAdd(mvPosition, VGet(0.0f, -150.0f, 0.0f)), VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
				{
					//当たっているであろうポリゴンとの接触点を求める
					hitPos = pStage->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, 1000.0f, 0.0f)),//プレイヤーの膝当たり(多分)と
						VAdd(mvPosition, VGet(0.0f, -1000.0f, 0.0f))//プレイヤーの少し下あたりを線分として指定
					);

					//当たった判定を取っておく
					isHit = true;
				}
			}
		}
	}

	if (isHit)
	{
		//地面に沿って歩いている状態として、Y座標をステージに合わせる
		mvPosition.y = hitPos.y;
	}
	if (isHit == false)
	{
		//要改善
		mvPosition.y += -8.0f;//落下する速度
		if (mvPosition.y <= 0.0f || mvPosition.y <= hitPos.y)
		{
			mvPosition.y = hitPos.y;
		}
	}

	/////////////////////////壁の当たり判定////////////////////////////
	bool hitwall = false;
	bool hitwalls = false;
	auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
	if (!walls.empty())
	{
		// 当たっている壁を調べる
		// hint: 現状の処理では1枚の壁しか最終的に判定されないので、2枚（以上）に当たっていた場合の処理を考える
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// プレイヤーを包むようなカプセル型の判定と、壁の三角形ポリゴンとの当たり判定を行う
				if (HitCheck_Capsule_Triangle(
					mvPosition,
					VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
					80.0f,
					vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
					HitCheck_Capsule_Triangle(
						mvPosition,
						VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
					)
				{
					int WallFontSize = GetFontSize();
					(40);
					DrawFormatString(20, 820, GetColor(255, 0, 0), "エリアの端っこです！！");
					(WallFontSize);
					hitwall = true;
					// 壁に沿って行くようなベクトルを取得
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 壁沿いベクトル
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 移動方向ベクトルの反対ベクトルと、壁の法線との内積を求める
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 壁沿いベクトルを計算

					if (hitwall == true && hitwalls == false)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
						//hitwall = false;
					}
					else if (hitwalls == true)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}

	}



	mpModel->SetPosition(mvPosition);
	mpModel->SetRotation(mvRotation);

}

void Player3D::Damage(float damage)
{
	AnimationState now = mpModel->GetNowState();
	if (damage - mpEquipmentManager->GetDamage() <= 0) { mfHp -= 1; return; }//最低でも1減らす
	if (now == ANIMATION_SLIDE)return;
	mfHp -= (damage-mpEquipmentManager->GetDamage());//装備しているダメージ分ダメージを減らす

}

void Player3D::Evasion()
{
	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		mpModel->ChangeAnimation(ANIMATION_SLIDE);
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
	}
	if (mpModel->GetNowState() == ANIMATION_SLIDE)
	{
		mvPosition = VAdd(mvPosition, VScale(oldmoveVec, mfEvasionSpeed));
		mpModel->SetPosition(mvPosition);
	}






}


void Player3D::RotationByMove()
{

	//////今向いてほしい角度を産出
	//Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag_3D_Camera)
	//現在の回転値から目標の回転値の差分を求める
	float subAngle = mfTargetAngle - mfAngle;



	//ある方向からある方向の差が１８０度以上（以下）になることがないはずなので
	//差の値が１８０以上（以下）になっていたら矯正する
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//角度の差分を徐々に０に近づける
	if (subAngle > 0.0f)
	{
		subAngle -= ROTATE_SPEED;
		if (subAngle < 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	else if (subAngle < 0.0f)
	{
		subAngle += ROTATE_SPEED;
		if (subAngle > 0.0f)
		{
			subAngle = 0.0f;
		}
	}

	//(400, 560, GetColor(0, 0, 0), "subAngle::%f",subAngle);

	//今向いてほしい角度を産出
	mfAngle = mfTargetAngle - subAngle;

	//DrawFormatString(400, 600, GetColor(0, 0, 0), "mfAngle::%f", subAngle);

	//回転値を設定
	mvRotation.y = mfAngle + DX_PI_F;
	//モデルに伝える
	mpModel->SetRotation(mvRotation);
	//mfAngle = mfTargetAngle - Master::mpCamera->GetLookCamera();

	////回転値を設定
	//mvRotation.y = mpCamera->GetlookAtPosition().y; //mfAngle + DX_PI_F;
	////モデルに伝える
	//mpModel->SetRotation(mvRotation);


}




void Player3D::Jump()
{

	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		mvPosition.y += 300.0f;
		mbjump = true;
		mfjumpPower = JUMP_POWER;

		//ジャンプ開始アニメーションにする
		
	}

	

}

void Player3D::Result()
{


}

void Player3D::Attack()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // マウスの状態を取得
	if (mouseInput & MOUSE_INPUT_LEFT && AttackCount >= AttackTime&&now!=ANIMATION_ATTACK)
	{
		AttackCount = 0;
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACK);
		//攻撃モーションに変更
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//ループはさせない
		mpModel->SetLoop(false);
		//モーション後は待機モーションに戻す
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		if (mpModel->GetIsSeparate() == true)
		{
			mpModel->mpSeparateAnimation->SetAnimationCount(1.3f);
		}
		else
		{
			mpModel->mpAnimation->SetAnimationCount(0.5f);
		}
	}


	auto pObjList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	if (now == ANIMATION_ATTACK)
	{
	
		AttackState = Attack_Normal;
		

	}



}

void Player3D::AttackJump()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // マウスの状態を取得
	if (mouseInput & MOUSE_INPUT_LEFT && AttackJumpCount >= AttackJumpTime&&!mbjump)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_JUMP);
		mbjump = true;
		AttackJumpCount = 0;
		mfjumpPower = JUMP_POWER;
		//攻撃モーションに変更
		mpModel->ChangeAnimation(ANIMATION_ATTACKJUMP);
		//ループはさせない
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//モーション後は待機モーションに戻す
		
		if (mpModel->GetIsSeparate() == true)
		{
			mpModel->mpSeparateAnimation->SetAnimationCount(1.0f);
		}
		else
		{
			mpModel->mpAnimation->SetAnimationCount(1.0f);
		}
	}


	auto pObjList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	if (now == ANIMATION_ATTACKJUMP)
	{
		AttackState = Attack_Jump;
		if (mfjumpPower >= mvPosition.y && !HighPositionFlag)
		{
			mvPosition = VAdd(mvPosition, VGet(0.0f, 5.0f, 0.0f));
		}
		if(mfjumpPower <= mvPosition.y)
		{
			HighPositionFlag = true;
			
			mbjumpDown = true;

		}
		if (HighPositionFlag)
		{
			mvPosition = VAdd(mvPosition, VGet(0.0f, mfjumpPower, 0.0f));
			mfjumpPower -= 1.0f;
		}
		


		if (mvPosition.y <= hitPos.y)
		{
			OnJumpCollider = true;
			mvPosition.y = hitPos.y;//Y座標を0にしておく
			
		}
		

	}
	else
	{
		OnJumpCollider = false;
		HighPositionFlag = false;
		mbjump = false;
		mbjumpDown = false;
	}
}

void Player3D::AttackSlide()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // マウスの状態を取得
	if (mouseInput & MOUSE_INPUT_LEFT && AttackSlideCount >= AttackSlideTime)
	{
		//mvOldPosition = mvPosition;
		if (mpTarget == nullptr)return;//敵を感知できなったらretrurn
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		AttackSlideCount = 0;
		GoPosition = (VSub(mpTarget->GetPosition(), mvPosition));
		TargetPosition = VScale(GoPosition, 0.5f);

		//攻撃モーションに変更
		mpModel->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		//ループはさせない
		mpModel->SetLoop(false);
		//モーション後は待機モーションに戻す
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);

	}
	

	
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		new Effect(VAdd(mvPosition, VGet(0.0f, 0.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 255, 30, 20), 50.0f, 0.03f);
		GoPosition = VNorm(GoPosition);
		mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);


		////////////////スライド攻撃の座標移動////////////////
		if (AttackSlideCount < 5)
		{
			mvPosition = VAdd(mvPosition, TargetPosition);
		}
		mpModel->SetPosition(mvPosition);
	}

	



}




void Player3D::Shot()
{


}
void Player3D::bar()
{
	float HpRatio = mfHp / GetAllStatusState(Object3D::Status_Hp);


	mfHp = mfHp < 0 ? 0 : mfHp; //HPが0未満になったら0を入れる
	mfHp = mfHp > GetAllStatusState(Object3D::Status_Hp) ? GetAllStatusState(Object3D::Status_Hp) : mfHp; //HPが最大値を超えていたら、最大値を入れる
	unsigned int Color;
	unsigned int Color2;
	Color = GetColor(255, 255, 255);
	Color2 = GetColor(0, 0, 0);
	DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, Color2, true);
	DrawBox(pos.x, pos.y, pos.x + size.x, pos.y + size.y, GetColor(255, 255, 255), true);
	DrawBox(pos.x + 1, pos.y + 1,
		pos.x + size.x * HpRatio - 1,
		pos.y + size.y - 1,
		GetColor(0, 255, 0), true);

	float DashRatio = DashGuage / MaxDashGauge;

	DashGuage = DashGuage < 0 ? 0 : DashGuage; //HPが0未満になったら0を入れる
	DashGuage = DashGuage > MaxDashGauge ? MaxDashGauge : DashGuage; //HPが最大値を超えていたら、最大値を入れる
	unsigned int Color3;
	unsigned int Color4;
	Color3 = GetColor(255, 255, 255);
	Color4 = GetColor(0, 0, 0);
	DrawBox(pos2.x, pos2.y, pos2.x + size2.x, pos2.y + size2.y, Color3, true);
	//DrawBox(pos2.x, pos2.y, pos2.x + size2.x, pos2.y + size2.y, GetColor(255, 255, 255), true);
	DrawBox(pos2.x + 1, pos2.y + 1,
		pos2.x + size2.x * DashRatio - 1,
		pos2.y + size2.y - 1,
		GetColor(255, 255, 0), true);

	//DrawFormatString(pos.x, pos.y, GetColor(255, 255, 255), "hp:%f", mnHp);



	mpLevelUp->Draw();
}

void Player3D::ChangeView()
{

	ChangeCamera++;
	if (ChangeCamera == 0)
	{
		Master::mpCamera->SetCamera1(true);
		Master::mpCamera->SetCamera3(false);
	}
	if (ChangeCamera == 1)
	{
		Master::mpCamera->SetCamera1(false);
		Master::mpCamera->SetCamera3(true);
		ChangeCamera = -1;
	}

}



void Player3D::EnemySerch()
{

}

void Player3D::Test()
{
	//Jump();
	if (InputManager::CheckDownKey(KEY_INPUT_2))
	{
		// 何を拾っても必ず回復薬 ---------------- //
		Item::ItemInformation* itemInfo;
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::HEAL;
		itemInfo->Name = "回復薬";
		mpItemManager->AddItem(itemInfo);

		//Master::mpInfClassManager->LogList.push_back(new InfClass(400, itemInfo->Name.c_str(), 1));
		// --------------------------------------- //

		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 1;
		itemInfo->ID = Item::ItemID::HIGHHEAL;
		itemInfo->Name = "highheal";
		mpItemManager->AddItem(itemInfo);


		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::POWER;
		itemInfo->Name = "Power";
		mpItemManager->AddItem(itemInfo);

		// 理想
		// 拾ったアイテムからItemInformationを取得する
		// それをItemManagerに渡してあげる

		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 5;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon3;
		weaponInf->name = "weapon3";
		mpWeaponManager->AddWeapon(weaponInf);


		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 7;
		weaponInf->filename = "Resource/3D/Hero.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon2;
		weaponInf->name = "weapon2";
		mpWeaponManager->AddWeapon(weaponInf);

	}
	if (InputManager::CheckDownKey(KEY_INPUT_3))
	{
		// 何を拾っても必ず回復薬 ---------------- //
		Item::ItemInformation* itemInfo;
		
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 6;
		itemInfo->ID = Item::ItemID::SPEED;
		itemInfo->Name = "speed";
		mpItemManager->AddItem(itemInfo);
		mpHaveMoney->AddMoney(1000);
		mpLevelUp->AddXp(20);

	}
	if (InputManager::CheckDownKey(KEY_INPUT_4))
	{
		mfHp -= 30;
	}


}
void Player3D::OnEnter(Collider* collider, Collider* check)
{
	

	if (collider == mpSerchEnemyCollider && check->mpParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
	{
		

		auto pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		VECTOR mnEneDistance = VSub(pEne->GetPosition(), mvPosition);
		if (check == pEne->GetEnemycoll())
		{
			mbAttackSlideSerch = true;
			float VECSize = VSize(mnEneDistance);
			//float VECSizeSecound = VSize(SecoundEneDistance);
			FirstNearVec++;
			if (FirstNearVec == 1)
			{
				VECSizeFirst = VECSize;
			}

			if (VECSizeFirst <= VECSize)
			{
				VECSizeFirst = VECSize;
				mpTarget = pEne;

			}
		}
	}
	//if (collider == 
	// 
	// 
	// 
	// 
	// && check->mpParentObject->GetTag() == Object3D::Tag3D_Player3D)
	//{
	//	mbStageOutFlag = false;
	//	if (check == this->mpCapsuleCollider)
	//	{
	//		mbStageOutFlag = true;//stage内にいなかったら場所移動する前の所に戻る
	//	}

	//}

	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Obj)
	{
		
		//mvPosition = VAdd(mvOldPosition, VScale(moveVec, -mfSpeed*3));
	}

}

void Player3D::OnTrigger(Collider* collider, Collider* check)
{

	AnimationState now = mpModel->GetNowState();

	if (now == ANIMATION_ATTACK)
	{
		if (collider == mpAttachCollider && check->mpParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
		{
			Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
			if (check == pEne->GetEnemycoll())
			{
				
				// 攻撃モーション&&Attack1
				if (now == ANIMATION_ATTACK && AttackState == Attack_Normal && !mbjump && !pEne->GetHitjudgment())
				{
					pEne->SetHitjudgment(true);
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));
					AttackHitJudgmentflag = true;
					// 画面揺れさせる
					Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
					new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 0, 30, 0), 30.0f, 0.1f);
				}


			}

		}
	}
	if (collider == mpAttackJumpCollider && check->mpParentObject->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		if (check == pEne->GetEnemycoll())
		{
			if (now == ANIMATION_ATTACKJUMP && AttackState == Attack_Jump && !pEne->GetHitjudgment())
			{
				pEne->SetHitjudgment(true);
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + mfAttackjump);

				// 画面揺れさせる
				Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
				new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(0, 255, 30, 0), 30.0f, 0.1f);
			}
		}
	}

	if (collider == mpAttackSlideCollider && check->mpParentObject->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		if (check == pEne->GetEnemycoll())
		{
			if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide && !pEne->GetHitjudgment())
			{
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + mfAttackSlide);
				pEne->SetHitjudgment(true);
				// 画面揺れさせる
				Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
				new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(35, 0, 255, 0), 60.0f, 1.0f);
			}
		}
	}
	


}

void Player3D::OnExit(Collider* collider, Collider* check)
{
	/*if (collider==mpAttachCollider&&check->mpParentObject->GetTag() == Object3D::Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);

	}*/
}

void Player3D::SelectAttack()
{
	AttackCountClass();
	AnimationState now = mpModel->GetNowState();
	if (InputManager::CheckDownKey(KEY_INPUT_E) && now != ANIMATION_ATTACK)
	{
		ChangeStateCount++;
		if (ChangeStateCount > 2)
		{
			ChangeStateCount = 0;
		}

	}

	switch (ChangeStateCount)
	{
	case 0:

		AttackState = Attack_Normal;
		Attack();
		break;
	case 1:

		//if (!mbAttackSlideSerch)break;
		AttackState = Attack_Slide;
		AttackSlide();
		break;
	case 2:
		AttackState = Attack_Jump;
		AttackJump();
	default:
		break;
	}
	if (Master::mpDebug)
	{
		DrawFormatString(300, 300, GetColor(255, 255, 255), "%d", ChangeStateCount);
	}
	if (!(now==ANIMATION_ATTACK)&& !(now == ANIMATION_ATTACKJUMP)&& !(now == ANIMATION_ATTACKSLIDE))//当たり判定の復活
	{
		auto mpEne = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < mpEne.size(); i++)
		{
			Enemy* pEne = dynamic_cast<Enemy*>(mpEne.at(i));
			pEne->SetHitjudgment(false);
		}
	}

}

void Player3D::AttackCountClass()
{


	AttackCount++;
	AttackSlideCount++;
	AttackJumpCount++;


}
void Player3D::CollPositionUpdate()
{
	AnimationState now = mpModel->GetNowState();
	// 当たり判定更新
	mpCapsuleCollider->mvPosition = mvPosition;
	mpCapsuleCollider->mvPosition2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));

	mpAttachCollider->mvPosition = VGet(1000, 10000, 1000);
	mpSerchEnemyCollider->mvPosition = mvPosition;
	mpAttackSlideCollider->mvPosition = VGet(1000, 10000, 1000);
	mpAttackJumpCollider->mvPosition = VGet(1000, 10000, 1000);
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		mpAttackSlideCollider->mvPosition = mvPosition;
	}
	else if (now == ANIMATION_ATTACKJUMP && AttackState == Attack_Jump&& OnJumpCollider)
	{
		mpAttackJumpCollider->mvPosition = mvPosition;
	}
	else if (now == ANIMATION_ATTACK)
	{
		mpAttachCollider->mvPosition = mpModel->GetAttachmentPosition();
	}

}

float Player3D::GetAllStatusState(Object3D::StatusState state)
{
	if (mpLevelUp == nullptr || mpBuffManager == nullptr || mpWeaponManager == nullptr)return 0;

	if (state == Status_Attack)
	{
		return mpWeaponManager->GetDamage() + mpBuffManager->GetBuff(state)+mpLevelUp->GetAttack();
	}
	if (state == Status_Speed)
	{
		return mfSpeed + mpBuffManager->GetBuff(state)+mpLevelUp->GetSpeed();
	}
	if (state == Status_Hp)
	{
		return mfMaxHp + mpLevelUp->GetHp();
	}


}

void Player3D::CollDelete()
{
	if (mpAttachCollider != nullptr)
	{
		mpAttachCollider->SetDeleteFlag(true);
	}
	if (mpAttackJumpCollider != nullptr)
	{
		mpAttackJumpCollider->SetDeleteFlag(true);
	}
	if (mpAttackSlideCollider != nullptr)
	{
		mpAttackSlideCollider->SetDeleteFlag(true);
	}
	if ( mpSerchEnemyCollider!= nullptr)
	{
		mpSerchEnemyCollider->SetDeleteFlag(true);
	}
}
