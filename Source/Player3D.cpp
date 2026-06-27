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

// 鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｴ螟ｧ・､・ｲ繝ｻ・ｽ繝ｻ・｡鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荵励・繝ｻ・ｽ繝ｻ・ｹ髫ｴ竏ｵ・ｻ繝ｻ・ｱ・ｪE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E鬮ｯ・ｷ髦ｮ・ｦEE驛｢譎｢・ｽ・ｻE鬯ｯ・ｨ繝ｻ・ｾ髯具ｽｹ郢晢ｽｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ髴托ｽ｢隴会ｽｦ繝ｻ・ｽ繝ｻ・ｸE鬮｣蛹・ｽｽ・ｵ髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬯ｯ・ｮ繝ｻ・ｫE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E驛｢譎｢・ｽ・ｻ髯晢ｽｶ隴弱・魘ｬ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷ髣費ｽｨ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｿE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｯ蜈ｷ・ｽ・ｹE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｧ鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻ髫ｴ繝ｻ謳ｨ繝ｻ・ｰ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｻ・碑ｭ趣ｽ｢繝ｻ・ｽ繝ｻ・ｻfalse鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮｣雋ｻ・ｿ・ｽ髯具ｽｹ郢晢ｽｻE驛｢譎｢・ｽ・ｻE鬯ｯ・ｨ繝ｻ・ｾ髯具ｽｹ郢晢ｽｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ鬩包ｽｶ騾｡遯ｪE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE鬮｣雋ｻ・ｽ・ｨ髯樊ｻゑｽｽ・ｲEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
Player3D::Player3D(std::string filename, VECTOR initPos, float jumppower, float speed, float hp, bool isSeparateAnim)
	:Object3D(initPos)
	, mfAttack(0)//鬯ｩ蟷｢・ｽ・｢髫ｰ・ｨ陞滓・遒托ｽｭ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE
	, mfAttackjump(5)
	, mfAttackSlide(7)

	,OnJumpCollider(false)//鬯ｩ謳ｾ・ｽ・ｵE鬯ｮ・ｦ繝ｻ・ｮ髯ｷ・ｷ繝ｻ・ｶ驛｢譎｢・ｽ・ｻ鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・ｸ驛｢譎｢・ｽ・ｻE鬯ｯ・ｯ繝ｻ・ｮE鬯ｮ・ｦ繝ｻ・ｮ髯ｷ・ｷ繝ｻ・ｮ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶呻ｽｸ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｮ繝ｻ・ｮ驕ｶ謫ｾ・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E驛｢譎｢・ｽ・ｻ髯懶ｽ｣繝ｻ・､EE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
	, mbInvisible(false)//鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｯ繝ｻ・ｶE鬯ｯ・ｯ繝ｻ・ｩE鬮ｯ譎｢・ｿ・ｽ鬮ｮ諛ｶ・ｽ・｣EE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ鬮｢・ｧ繝ｻ・ｲEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ騾｡繝ｻ
	, mbjump(false)
	, mfVerticalAngle(0.0f)
	, mfHorizontalAngle(0.0f)
	, mfSpeed(speed)//speed
	, JUMP_POWER(jumppower)
	, mfHp(hp)//hp
	, mfMaxHp(hp)//maxhp
	, mfDashSpeed(speed * 2)
	, NewShieldFast(false)//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ蜿門ｾ励・・ｽ繝ｻ・ｳE驛｢譎｢・ｽ・ｻ髯晢ｽｶ隴取得・ｽ・｢鬮ｮ・｣繝ｻ・ｽ繝ｻ・ｻ鬮ｦ・ｮ陷呈ｨ｣・ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ髴托ｽ｢隴会ｽｦ繝ｻ・ｽ繝ｻ・ｸE鬮｣蛹・ｽｽ・ｵ髫ｴ謫ｾ・ｽ・ｶ驛｢譎｢・ｽ・ｻ鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻE郢晢ｽｻ繝ｻ・ｰ
	, mfSize(60.0f)//鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ鬮ｯ讖ｸ・ｽ・ｺ鬯ｩ蟷｢・ｽ・｢E鬯ｯ・ｮ繝ｻ・ｮ驕ｶ謫ｾ・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	, Pause(false)//鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｯ・ｨ繝ｻ・ｾ髯具ｽｹ郢晢ｽｻE鬯ｯ・ｯ繝ｻ・ｮE驛｢譎｢・ｽ・ｻE
	, ChangeCamera(0)//鬯ｯ・ｮ繝ｻ・ｫ鬩包ｽｨ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｹ髮九・ﾂ・ｪ鬮ｮ・ｷ鬯ｮ・ｯ隶灘･・ｽｽ・ｺ繝ｻ・ｽ髯具ｽｻ繝ｻ・､鬮ｯ譎｢・ｽ・ｲE
	, DashGuage(100.0f)//
	, EneSerchCount(0)
	, Gpush(false)//鬯ｮ・ｯ雋翫ｑ・ｽ・ｽ繝ｻ・｢鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰ髯懶ｽ｣驕擾ｽｩ・主ｿ埼ｫｯ蜿･・ｹ・｢繝ｻ・ｽ繝ｻ・ｵ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ髫ｶ謐ｺ諷｣繝ｻ・ｽ繝ｻ・ｸE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	, GpushCount(2)//鬯ｮ・ｯ雋翫ｑ・ｽ・ｽ繝ｻ・｢鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰ髯懶ｽ｣驕擾ｽｩ邵ｺ蜍・Δ譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ
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
	Master::mpPlayer = this;
	
	mpEffectU = new EffectU();
	mpEffectU->Load();


	SetTag(Object3D::Tag3D_Player3D);
	mpBuffManager = new BuffManager();
	mpWeaponManager = new WeaponManager();
	mpEquipmentManager = new EquipmentManager();
	mpShortInventory = new ShortInventory();
	mpLevelUp = new LevelUp();
	mpModel = new Model(filename, initPos, isSeparateAnim);
	mpHaveMoney = new HaveMoneyClass(0);
	mpModel->AddAttachment("Resource/3D/Sabel.mv1", "mixamorig:RightHand");
	mpModel->AddAnimation(ANIMATION_NEUTRAL, "Resource/Model/Idle.mv1");
	mpModel->AddAnimation(ANIMATION_RUN, "Resource/Model/Run.mv1");
	mpModel->AddAnimation(ANIMATION_DYING, "Resource/Model/Dying.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACK, "Resource/Model/Attack_3.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKSLIDE, "Resource/Model/Attack.mv1");
	mpModel->AddAnimation(ANIMATION_ATTACKJUMP, "Resource/Model/Jump Attack.mv1");
	mpModel->AddAnimation(ANIMATION_SLIDE, "Resource/Model/kaihi.mv1");

	Master::mpCamera->Initialize();

	float HpRatio = (float)mfHp / mfMaxHp;
	mfMaxHp = mfHp;
	mfNormalSpeed = mfSpeed;
	{//鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｴ隹ｿ・ｺEEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
		Weapon::WeaponDate* weaponInf;
		weaponInf = new Weapon::WeaponDate;
		weaponInf->damage = 10;
		weaponInf->filename = "Resource/3D/Sabel.mv1";
		weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon1;
		weaponInf->name = "weapon1";
		mpWeaponManager->AddWeapon(weaponInf);
		mpWeaponManager->ChangeWeapon(weaponInf);

		mfNormalAttack = weaponInf->damage;//鬯ｮ・ｫ繝ｻ・ｴ髯晢ｿｽ繝ｻ・｢鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｴ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬩包ｽｶ鬯・汚・ｽ・･繝ｻ・｢髯晢ｿｽ隰悟･・ｽｽ・ｭ隹ｿ・ｺEEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ髯ｷ・･隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｸE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｲ陋帙・・ｽ・ｽ繝ｻ・ｨ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｴ隹ｿ・ｺEEE鬯ｯ・ｮ繝ｻ・｢鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ髯晢ｽｶ隴主臆・ｷ譎櫁ｾｧ陷肴ｻゑｽｽ・ｨ繝ｻ・ｯ鬯ｲ蛛・ｽｽ・ｬ
		mfAttack = weaponInf->damage;//鬯ｮ・ｫ繝ｻ・ｴ髯晢ｿｽ繝ｻ・｢鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｴ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬩包ｽｶ鬯・汚・ｽ・･繝ｻ・｢髯晢ｿｽ隰悟･・ｽｽ・ｭ隹ｿ・ｺEEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ髯ｷ・･隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｸE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｲ陋帙・・ｽ・ｽ繝ｻ・ｨ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｴ隹ｿ・ｺEEE鬯ｯ・ｮ繝ｻ・｢鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ髯晢ｽｶ隴主臆・ｷ譎櫁ｾｧ陷肴ｻゑｽｽ・ｨ繝ｻ・ｯ鬯ｲ蛛・ｽｽ・ｬ
	}




	MaxDashGauge = DashGuage;

	
	 // 鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｰ・ｨ陞滓・遒托ｽｭ雜｣・ｽ・｢EE鬯ｯ・ｨ繝ｻ・ｾ髯溘・螻ｮ繝ｻ・ｽ繝ｻ・ｻ鬮ｦ・ｮ陷偵・
	mpCapsuleCollider = new CapsuleCollider(this, mvPosition, VAdd(mvPosition, VGet(0.0f, mfSize, 0.0f)), mfSize);
	mpAttachCollider = new SphereCollider(this, mpModel->GetAttachmentPosition(), 60.0f);
	mpAttackSlideCollider = new SphereCollider(this, mvPosition, 200.0f);
	mpSerchEnemyCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 500.0f);
	mpAttackJumpCollider = new SphereCollider(this, VAdd(mvPosition, VGet(0.0f, 120.0f, 0.0f)), 300.0f);
	
	mvFirstPosition = initPos;
	
	AttackState = Attack_Normal;
	mpItemManager = Master::mpItemManager;
}

Player3D::~Player3D()
{
	if (Master::mpPlayer == this) Master::mpPlayer = nullptr;
	delete mpModel;
	delete mpShortInventory;
	CollDelete();
}

void Player3D::Update()
{
	if (mnInvincibleTimer > 0) {
		mnInvincibleTimer--;
	}

	mpShortInventory->Update();
	if (Master::ShopClassOn || Master::StatShopClassOn)return;
	///////////////////////////////////////////////
	

	
	
	if (Master::PauseOn == false)//鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ髫ｴ螟ｲ・ｽ・ｿE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ鬮｢・ｧ繝ｻ・ｲEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ闔ｨ竏ｬ・ｱ・ｪEE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
	{
		if (mpModel != nullptr)
		{
			mpTargetOn();
			Test();
			ManagerUpdate();
			if (InputManager::CheckDownKey(KEY_INPUT_Q))
			{
				ChangeView();
			}
			////////////////////////鬯ｯ・ｮ繝ｻ・ｯE鬮ｫ・ｰ騾搾ｽｲ繝ｻ・ｺ繝ｻ・ｯE////////////////

			Result();
			ResetNUETRAL();
			//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻ髫ｲ蟶ｷ髦憺ｬｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
			
			Evasion();
			//鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
			SelectAttack();
			//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髫ｶ謇具ｽｴ蜈ｷ・ｽ・ｾ陝ｶ・ｷ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
			MoveEx();
			CollPositionUpdate();
			//鬯ｮ・ｯ隲帙・・ｻ・ｸE驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
			RotationByMove();
			EnemySerch();

			if (mpLevelUp->GetLevelUp())
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_LEVELUP);
				mfHp = GetAllStatusState(Object3D::Status_Hp);//hp鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｭ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隲帙・・ｻ・ｸEE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻEE鬮ｯ譎｢・ｽ・ｶ髯ｷ・ｻ繝ｻ・ｻE驛｢譎｢・ｽ・ｻ
			}
			mpModel->Update();
		}
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


		FirstNearVec = 0;//鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ鬮ｮ繝ｻ・ｿ・ｽ繝ｻ・､EE鬯ｯ・ｮ繝ｻ・ｦE鬩包ｽｶ鬯・汚・ｽ・･繝ｻ・｢EE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｯ・ｮ繝ｻ・ｴ鬮｢・ｧ繝ｻ・ｴ鬮ｴ雜｣・ｽ・ｨ鬮ｯ讖ｸ・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ・ｷ繝ｻ・ｻ鬮｣魃会ｽｽ・ｨEE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮｣蛹・ｽｽ・ｵ髫ｴ謫ｾ・ｽ・ｴ驕ｯ・ｶ繝ｻ・ｳ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｺｽ・ｹ譎｢・ｽ・ｻ髯懶ｽ｣繝ｻ・､E髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｨ
		mpTarget = nullptr;//鬯ｯ・ｨ繝ｻ・ｾE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｶ髴難ｽ｣陋滂ｽ｡陷・ｽｽ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｨ鬯ｩ蟷｢・ｽ・｢E鬮ｯ蜿･・ｹ・｢繝ｻ・ｽ繝ｻ・ｵE髫ｲ蟶吶∴E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｨ

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
	if (Master::ShopClassOn || Master::StatShopClassOn)return;
	if (Master::PauseOn == false)///鬩幢ｽ｢隴弱・・ｺ・｢驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧE鬯ｨ・ｾ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｻ鬯ｯ・ｮ繝ｻ・ｱE鬩搾ｽｵ繝ｻ・ｺE鬩搾ｽｵ繝ｻ・ｺE鬩搾ｽｵ繝ｻ・ｺE鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ驕ｶ莨√・繝ｻ・ｹ繝ｻ・ｧ鬮｢・ｾ繝ｻ・･E髫ｴ蠑ｱ繝ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｯ・ｨ繝ｻ・ｾ髯具ｽｹ郢晢ｽｻE鬯ｯ・ｯ繝ｻ・ｮE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ鬮ｮ繝ｻ・ｿ・ｽ繝ｻ・､EE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	{
		
		bar();
		
	}
	////////////////////////鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ鬮ｫ・ｰ隴ｴ・ｧ雎主祷E驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｯ・ｮ繝ｻ・ｫ鬩包ｽｨ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｹ髮九・ﾂ・ｪ鬮ｮ・ｷ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ髯滓汚・ｽ・ｱEE郢晢ｽｻ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲE髫ｶ謐ｺ諷｣繝ｻ・ｽ繝ｻ・ｹE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE鬮ｴ謇假ｽｽ・ｹ髯樊ｻゑｽｽ・ｲEE鬮｣蛹・ｽｽ・ｵ髫ｴ謫ｾ・ｽ・ｶ髫ｴ繝ｻ・ｽ・｡鬯ｩ蟷｢・ｽ・｢E鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E鬮｢・ｧ繝ｻ・ｲEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ闔ｨ竏ｬ・ｱ・ｪEE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ//////////////
	if (Master::mpCamera->GetCamera3() == true)
	{
		//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻE髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬮ｯ・ｷ・つ髫ｲ・､陝抵ｽ｢
		mpModel->Draw();
	}

	if (Master::mpDebug->Getdebug() == true)
	{
		//鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｣鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｯ・ｮ繝ｻ・ｯE驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ驕ｨ繧托ｽｽ・ｼ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｽE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｮ隰・・・ｽ・ｼ陟｢繝ｻ
		DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)),
			mfSize,
			8,
			GetColor(255, 255, 255),
			GetColor(255, 255, 255),
			false
		);
		DrawFormatString(100, 300, GetColor(255, 255, 255), "鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｲ陋帙・・ｽ・ｽ繝ｻ・ｨ:%f", GetAllStatusState(Object3D::Status_Attack));
		DrawFormatString(100, 400, GetColor(255, 255, 255), "鬯ｯ・ｯ繝ｻ・ｮE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ雋・ｽｷ隴ｯ繝ｻE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｽｲ繝ｻ・ｨ驛｢譎｢・ｽ・ｻ%f", mpEquipmentManager->GetDamage());
		DrawFormatString(100, 500, GetColor(255, 255, 255), "s鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ%f", GetAllStatusState(Object3D::Status_Speed));
		DrawFormatString(100, 350, GetColor(255, 255, 255), "鬯ｯ・ｩ隰ｳ・ｾ繝ｻ・ｽ繝ｻ・ｨ鬯ｩ謌岩筏EE鬮ｴ髮｣・ｽ・｣髯区ｻゑｽｽ・ｪ驍ｵ・ｺ隹ｿ・ｺE:%d", mpLevelUp->GetXp());
		DrawFormatString(100, 450, GetColor(255, 255, 255), "X:%f        Y:%f       Z:%f", mvPosition.x, mvPosition.y, mvPosition.z);
	}
	//鬯ｮ・ｮ隹ｿ・ｺEE鬯ｮ・ｯ隲帙・・ｽ・ｶ繝ｻ・｣EE鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｨ陞ゅ・・ｽ・ｽ繝ｻ・｡驕ｶ謫ｾ・ｽ・ｫEEE鬯ｩ蟷｢・ｽ・｢E鬮ｯ蜈ｷ・ｽ・ｹE鬩包ｽｶ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｧ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	DrawSphere3D(
		mpModel->GetAttachmentPosition(),
		30.0f,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);
}

//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髫ｶ謇具ｽｴ蜈ｷ・ｽ・ｾ陝ｶ・ｷ繝ｻ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｺ髯晢ｽｶ繝ｻ・ｷE髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶呻ｽｸ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｮ繝ｻ・ｮ驕ｶ謫ｾ・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
void Player3D::MoveEx()
{
	AnimationState state = mpModel->GetNowState();
	if (state==ANIMATION_ATTACKJUMP||state == ANIMATION_ATTACK || state == ANIMATION_JUMP_OUT || state == ANIMATION_SLIDE||state==ANIMATION_ATTACKSLIDE||Master::ShopClassOn||Master::StatShopClassOn)
	{
		return;
	}//鬯ｮ・ｴ陞滂ｽｧ繝ｻ・､繝ｻ・ｲEE鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻE髫ｶ蜻ｵ・ｶ・｣繝ｻ・ｽ繝ｻ・ｸE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陷ｴ繝ｻ・ｽ・ｽ繝ｻ・ｴ鬯ｮ・ｴ鬩帙・・ｽ・ｲ繝ｻ・ｻE驛｢・ｧ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｸE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ鬩包ｽｶ騾｡遯ｪE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ

	moveVec = VGet(0.0f, 0.0f, 0.0f);//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髫ｶ荳ｻ・･繝ｻ・ｽ・ｽ繝ｻ・｢髴難ｽ｣陋帙・・ｽ・ｽ繝ｻ・ｩE鬯ｮ・ｯ繝ｻ・ｷE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳEEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸE
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｾ驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸE
	//VECTOR DownMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ鬮ｯ蜈ｷ・ｽ・ｹE鬮ｯ謨鳴E鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸE
	//VECTOR RightMoveVector = VGet(0.0f, 0.0f, 0.0f);//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸE


	//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｯ繝ｻ・ｶE鬯ｩ謳ｾ・ｽ・ｵE鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E鬮ｯ蜈ｷ・ｽ・ｾ繝ｻ繧托ｽｽ・ｧ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ髫ｴ竏ｬ繝ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE鬮ｯ・ｷ繝ｻ・ｻ鬮｣魃会ｽｽ・ｨEE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻE驕ｶ謫ｾ・ｽ・ｫEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	{
		//鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳEEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ陷托ｽｰ邵ｺ蠢孔驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ髫ｴ竏ｬ繝ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩包ｽｶ闕ｳ讖ｸ・ｽ・｣繝ｻ・ｹ驕ｯ・ｶ繝ｻ・ｳ鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｯ・ｮ繝ｻ・ｫ鬩包ｽｨ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｹ髮九・ﾂ・ｪ鬮ｮ・ｷ鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣E郢晢ｽｻ繝ｻ・ｰ鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｫ繝ｻ・ｰ驕ｶ謫ｾ・ｽ・ｬE髯溷桁・ｽ・｡E髯ｷ・ｷ繝ｻ・ｶ驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E鬮ｯ・ｷ繝ｻ・ｻ髯懈瑳・ｪ・ｸ鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
		UpMoveVector = VSub(Master::mpCamera->GetlookAtPosition(), Master::mpCamera->GetPosition());

		//鬯ｮ・ｯ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｾ驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ陷托ｽｰ邵ｺ蠢孔驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ髫ｴ竏ｬ繝ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE鬮ｯ貅ｷ萓帙・・ｨ繝ｻ・ｯ驍ｵ・ｺ陞ｳ闌ｨ・ｽ・ｫ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｽ驛｢譎｢・ｽ・ｻEEEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ髫ｴ竏ｬ繝ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩搾ｽｵ繝ｻ・ｲ驛｢譎｢・ｽ・ｻ髫ｲ・､鬯ｮ・ｭE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲE髯晢ｽｶ繝ｻ・ｷEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ陷托ｽｰ邵ｺ蠢孔驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ陷・ｽｽ鬩搾ｽｵ繝ｻ・ｺ鬯ｩ・｢隰ｳ・ｾ繝ｻ・ｽ繝ｻ・ｹ髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隲幢ｽｷ騾墓ｪｸ鬮ｯ譎｢・ｽ・ｲE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮ｫ・ｰ郢晢ｽｻEE鬮｣雋ｻ・ｿ・ｽE驛｢譎｢・ｽ・ｻE鬮｣蛹・ｽｽ・ｵ髮狗ｿｫ・代・・ｽ繝ｻ・ｽE鬮ｫ・ｰ繝ｻ・ｳE驛｢譎｢・ｽ・ｻE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		leftMoveVector = VCross(UpMoveVector, VGet(0.0f, 1.0f, 0.0f));


		//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ髫ｴ竏ｬ繝ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ鬮｢・ｧ繝ｻ・ｴ髯懶ｽｮ繝ｻ・ｩE鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｫ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ繝ｻ・ｷE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ鬩包ｽｶ騾｡遯ｪE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｵ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｯ・ｮ繝ｻ・ｫ鬩包ｽｨ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｸ髫ｶ謚ｵ・ｽ・ｫEE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ鬩包ｽｯ繝ｻ・ｶE鬯ｩ謳ｾ・ｽ・ｵE鬯ｩ諤憺●繝ｻ・ｽ繝ｻ・ｫ驛｢譎｢・ｽ・ｻE(鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ陷・ｽｽ鬩搾ｽｵ繝ｻ・ｺ鬯ｩ・｢隰ｳ・ｾ繝ｻ・ｽ繝ｻ・ｹ髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｯ・ｯ繝ｻ・ｮ髮狗ｿｫ・代・・ｽ繝ｻ・ｽE鬯ｩ謳ｾ・ｽ・ｵE鬯ｮ・ｴ鬩帙・・ｽ・ｲ繝ｻ・ｻE髯句ｹ｢・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ闔ｨ竏ｬ・ｱ・ｪEE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ髴托ｽ｢隴会ｽｦ繝ｻ・ｽ繝ｻ・ｸE鬯ｮ・ｦ繝ｻ・ｮ髯ｷ・ｷ繝ｻ・ｮ驛｢譎｢・ｽ・ｻ)
		UpMoveVector = VNorm(UpMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	//VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髫ｶ荳ｻ・･繝ｻ・ｽ・ｽ繝ｻ・｢髴難ｽ｣陋帙・・ｽ・ｽ繝ｻ・ｩE鬯ｮ・ｯ繝ｻ・ｷE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ

	if (CheckHitKey(KEY_INPUT_A))//鬯ｮ・ｯ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｾ驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ陷托ｽｰ邵ｺ蠢孔驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｯ・ｯ繝ｻ・ｨE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_D))//鬯ｮ・ｯ繝ｻ・ｷE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ陷托ｽｰ邵ｺ蠢孔驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｯ・ｯ繝ｻ・ｨE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	{
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f));

	}
	if (CheckHitKey(KEY_INPUT_W))//鬯ｮ・ｯ隶抵ｽｭ繝ｻ阮ｦE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ陷托ｽｰ邵ｺ蠢孔驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｯ・ｯ繝ｻ・ｨE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	{
		moveVec = VAdd(moveVec, UpMoveVector);
	}
	if (CheckHitKey(KEY_INPUT_S))//鬯ｮ・ｫ繝ｻ・ｰ驛｢譎｢・ｽ・ｻ髯ｷ螟ｲ・ｽ・ｱ鬮ｴ雜｣・ｽ・ｯ髫ｰ謔滂ｽ･繝ｻ・ｽ・ｽ繝ｻ・ｭ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩包ｽｶ陷托ｽｰ邵ｺ蠢孔驛｢譎｢・ｽ・ｻE鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｯ・ｯ繝ｻ・ｨE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	{
		moveVec = VAdd(moveVec, VScale(UpMoveVector, -1.0f));

	}
	//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ鬮ｮ蜈ｷ・ｽ・ｻEE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ鬩阪・遘√・・ｽ繝ｻ・ｿE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｲE鬮｣蛹・ｽｽ・ｵ髫ｴ謫ｾ・ｽ・ｴ髯懶ｽ｣繝ｻ・､EE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻE髫ｶ蜻ｵ・ｶ・｣繝ｻ・ｽ繝ｻ・ｸE驛｢譎｢・ｽ・ｻE
	bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{

		//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髫ｶ荳ｻ・･繝ｻ・ｽ・ｽ繝ｻ・｢髴難ｽ｣陋帙・・ｽ・ｽ繝ｻ・ｩE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣驛｢譎｢・ｽ・ｻ髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｱ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｮ繝ｻ・ｫ鬩包ｽｨ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｸ髫ｶ謚ｵ・ｽ・ｫEE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ鬩包ｽｯ繝ｻ・ｶE鬯ｩ謳ｾ・ｽ・ｵE鬯ｩ諤憺●繝ｻ・ｽ繝ｻ・ｫ驛｢譎｢・ｽ・ｻE
		moveVec = VNorm(moveVec);

		//鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ驛｢譎｢・ｽ・ｻ髫ｶ螢ｺ謇ｱ驍ｯ諞ｺ蝙ｳ陷ｩ讚摘驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ蜿･・ｹ・｢繝ｻ・ｽ繝ｻ・ｵ鬩搾ｽｵ繝ｻ・ｺ髫ｴ諠ｹ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｹ髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｨ
		mfTargetAngle = atan2f(moveVec.x, moveVec.z);
		oldmoveVec = moveVec;//鬯ｩ謳ｾ・ｽ・ｵE鬯ｮ・ｦ繝ｻ・ｮ髯ｷ・ｻ繝ｻ・ｻE驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陟｢蟄忍鬩搾ｽｵ繝ｻ・ｺ髯ｷﾂ隴会ｽｦ繝ｻ・ｽ繝ｻ・ｹ髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蛹・ｽｽ・ｯE鬯ｮ・ｴ陜捺ｺｷ郢ｭE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣驛｢譎｢・ｽ・ｻ髫ｶ謐ｺ諷｣繝ｻ・ｽ繝ｻ・ｸE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ鬩阪・遘√・・ｽ繝ｻ・ｭ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣驛｢譎｢・ｽ・ｻ髯橸ｽｳ陞｢・ｽ・つ隹ｿ・ｺE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ繝ｻ・ｷEE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻEE鬮ｫ・ｴ陷ｿ髢・ｾ蜍⑥鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	}
	//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ蜿厄ｽｨ髮∬｢夜ｫｯ譎｢・ｽ・ｷ髫ｰ遒・ｿｽ謇假ｽｽ・ｲ郢ｧ髮・ｽｵ・ｯ髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荵励・繝ｻ・ｽ繝ｻ・ｹ髫ｴ竏ｵ・ｻ繝ｻ・ｱ・ｪEE鬮ｯ蛹ｺ・ｻ繧托ｽｽ・ｽ繝ｻ・･驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻE髫ｶ蜻ｵ・ｶ・｣繝ｻ・ｽ繝ｻ・ｸE驛｢譎｢・ｽ・ｻE
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



	//鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髯晢ｿｽ髮懶ｽ｣繝ｻ・ｽ繝ｻ・ｼ鬮ｮ蜈ｷ・ｽ・ｻE驛｢譎｢・ｽ・ｻEE鬮ｯ譎｢・ｽ・ｶ髯ｷ・ｻ繝ｻ・ｻE驛｢譎｢・ｽ・ｻ
	mvOldPosition = mvPosition;//鬯ｮ・ｯ繝ｻ・ｷ鬮ｫ・ｨ繝ｻ・ｬE鬯ｮ・ｮ郢晢ｽｻ鬯ｪ・ｰ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｸE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ雋・ｽｯ繝ｻ・ｶ繝ｻ・｣EE鬯ｮ・ｫ繝ｻ・ｶ髴難ｽ｣陋滂ｽ｡陷・ｽｽ驛｢譎｢・ｽ・ｻ髯橸ｽｳ陞｢・ｼ陋ｻ謌ｲ鬯ｮ・ｫ繝ｻ・ｴ鬯ｲ繝ｻ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｽE鬯ｮ・｣陷ｴ繝ｻ・ｽ・ｽ繝ｻ・ｫ鬮ｫ・ｴ隹ｿ・ｺE


	mvPosition = VAdd(mvPosition, VScale(moveVec, GetAllStatusState(Object3D::Status_Speed)));
	////////////////////////////////////////////////////////



	// 地形（ステージ）に沿う処理
	TerrainFollow();

	/////////////////////////鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・｢鬩包ｽｶ闕ｳ讖ｸ・ｽ・｣繝ｻ・ｹ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶呻ｽｸ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｮ繝ｻ・ｮ驕ｶ謫ｾ・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ///////////////////////////
	bool hitwall = false;
	bool hitwalls = false;
	auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
	if (!walls.empty())
	{
		// 鬯ｮ・ｯ隶呻ｽｸ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ鬩阪・謌溘・・､繝ｻ・ｶ鬩包ｽｶ闕ｳ讖ｸ・ｽ・､繝ｻ・ｲE髯晢ｽｶ隴惹ｹ怜･鈴Δ譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		// hint: 鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・ｴ驛｢譎｢・ｽ・ｻE鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・･驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬩搾ｽｵ繝ｻ・ｲ髯懶ｽ｣繝ｻ・､EE驛｢譎｢・ｽ・ｻE1鬯ｮ・ｫ繝ｻ・ｴE鬮ｯ讖ｸ・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・｢鬩包ｽｶ闕ｳ讖ｸ・ｽ・､繝ｻ・ｲEE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ蜈ｷ・ｽ・ｹE鬮ｫ・ｲ繝ｻ・､髯ｷ・･陟托ｽｱ隨卍鬩幢ｽ｢繝ｻ・ｧ髴大｣ｼ陌ｫ繝ｻ・｣繝ｻ・ｭ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻEE鬮ｯ貅ｷ萓帙・・ｨ繝ｻ・ｯ驛｢譎｢・ｽ・ｻ鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｫ繝ｻ・ｴE鬮ｯ讓奇ｽｻ繧托ｽｽ・ｽ繝ｻ・ｲ驛｢譎｢・ｽ・ｻE鬮｣雋ｻ・ｿ・ｽ髯具ｽｹ郢晢ｽｻE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ鬮ｯ讒ｭ繝ｻEE鬮ｯ譎｢・ｽ・ｲE鬩包ｽｶ鬯・汚・ｽ・･繝ｻ・｢E髯ｷﾂ繝ｻ・ｶ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻ髫ｰ・ｦ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷE鬮ｯ蜈ｷ・ｽ・ｹE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｯ繝ｻ・ｷ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ髯晢ｽｶ隴寂悪・ｮ遘佩碑ｭ趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬯ｩ蟷｢・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		for (int i = 0; i < walls.size(); i++)
		{
			Wall* wall = dynamic_cast<Wall*>(walls.at(i));
			if (wall != nullptr)
			{
				std::vector<VERTEX3D> vertex = wall->GetVertex();

				// 鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲE髫ｶ謐ｺ諷｣繝ｻ・ｽ繝ｻ・ｹE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｨ陞ゅ・・ｽ・ｽ繝ｻ・｡驕ｶ謫ｾ・ｽ・ｫEEE鬯ｩ蟷｢・ｽ・｢E鬮ｯ蜈ｷ・ｽ・ｹE鬩包ｽｶ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｧ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲ鬩搾ｽｵ繝ｻ・ｺ髫ｴ諠ｹ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｹ髫ｴ雜｣・ｽ・｢EE鬯ｮ・ｯ隲幢ｽｷ陝区遭髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E鬩包ｽｶ鬮ｮ繝ｻ・ｿ・ｽ繝ｻ・､EE鬩包ｽｶ闕ｵ諤懈ｬｾEE鬩包ｽｶ闕ｳ讖ｸ・ｽ・｣繝ｻ・ｹ驛｢譎｢・ｽ・ｻ鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳE驛｢譎｢・ｽ・ｻE鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｩ繝ｻ・ｸEE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻE髫ｲ蟶吶∴E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶呻ｽｸ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｮ繝ｻ・ｮ驕ｶ謫ｾ・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E驛｢譎｢・ｽ・ｻ髯晢ｽｶ隴主臆・ｷ譎櫁ｾｧ陷肴ｻゑｽｽ・ｨ繝ｻ・ｯ鬯ｲ蛛・ｽｽ・ｬ
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
					

					// 鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・｢鬩包ｽｶ闕ｳ讖ｸ・ｽ・｣繝ｻ・ｺ驛｢譎｢・ｽ・ｻ鬯ｮ・ｮ陷ｿ蜀ｶ繝ｻEE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｯ・ｮ繝ｻ・ｯE鬮ｯ貊灘擠繝ｻ・ｯ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｿE鬯ｩ蟷｢・ｽ・｢E鬮ｯ蜈ｷ・ｽ・ｹE鬩包ｽｶ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｧ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ陷・ｽｽ鬩搾ｽｵ繝ｻ・ｺ鬯ｩ・｢隰ｳ・ｾ繝ｻ・ｽ繝ｻ・ｹ髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｫ髯溷供・ｮ・｣髴趣ｽｧ鬮ｴ莠･・ｿ繝ｻ
					VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・｢鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｵ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸE
					float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 鬯ｯ・ｩ陷肴ｺｪ繝ｻEE鬯ｮ・ｯ繝ｻ・ｷ髫ｶ荳ｻ・･繝ｻ・ｽ・ｽ繝ｻ・｢髴難ｽ｣陋帙・・ｽ・ｽ繝ｻ・ｩE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷE鬮ｫ・ｶ隹ｿ・ｺE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ陷・ｽｽ鬩搾ｽｵ繝ｻ・ｺ鬯ｩ・｢隰ｳ・ｾ繝ｻ・ｽ繝ｻ・ｹ髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩包ｽｶ闕ｵ諤懈ｬｾEE鬩包ｽｶ闕ｳ讖ｸ・ｽ・｣繝ｻ・ｹ驛｢譎｢・ｽ・ｻ鬯ｮ・ｮ闕ｳ・ｻ繝ｻ・｢髷ｫ蛟｡・ｹ譎｢・ｽ・ｻE鬮ｯ讖ｸ・ｽ・｢E鬩包ｽｶ鬮ｮ繝ｻ・ｿ・ｽ繝ｻ・､EE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻE鬯ｯ・ｮ繝ｻ・ｦE驛｢譎｢・ｽ・ｻ髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｱ髯滓汚・ｽ・ｱEEE驕ｶ謫ｾ・ｽ・ｫEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
					slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・｢鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｵ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ荳ｻ繝ｻ隶捺ｪｸ髫ｴ荳ｻ謇ｿE鬮ｯ讖ｸ・ｽ・ｳ髯樊ｻゑｽｽ・ｲEE鬯ｮ・｢繝ｻ・ｧE驛｢譎｢・ｽ・ｻE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ

					if (hitwall == true && hitwalls == false)
					{
						mvPosition = mvOldPosition;
						mvPosition = VAdd(mvPosition, VScale(slide, mfSpeed));
						hitwalls = true;
					}
					else if (hitwalls == true)
					{
						mvPosition = mvOldPosition;
					}
				}
			}
		}

	}

	//if_StageOut();

	mpModel->SetPosition(mvPosition);
	mpModel->SetRotation(mvRotation);

}

void Player3D::Damage(float damage)
{
	AnimationState now = mpModel->GetNowState();
	if (mnInvincibleTimer > 0) return; // 鬮ｴ蜿厄ｽｻ繧托ｽｽ・ｽ繝ｻ・｡鬮ｫ・ｰ繝ｻ・ｨE鬮ｫ・ｴ陟托ｽｱ繝ｻ邇門ｰ・・・｣鬮｣蛹・ｽｽ・ｳE鬩搾ｽｵ繝ｻ・ｺE鬩幢ｽ｢隴惹ｼ夲ｽｽ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・｡鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧE鬮ｴ蜿厄ｽｻ繧托ｽｽ・ｽ繝ｻ・｡鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｹ
	//if (damage - mpEquipmentManager->GetDamage() <= 0) { mfHp -= 1; return; }//鬮ｫ・ｴ陝ｷ繝ｻ蜑ｲ髯滓汚・ｽ・ｱ驍ｵ・ｲ陜｣・､繝ｻ・ｹ繝ｻ・ｧ驛｢・ｧ髣鯉ｽｨ繝ｻ・ｽ繝ｻ・ｸ髯晢ｽｶ陷ｻ・ｻ繝ｻ・ｽ髢ｾ・･繝ｻ・ｸ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
	if (now == ANIMATION_SLIDE || now == ANIMATION_ATTACKSLIDE)return;
	mfHp -= (damage-mpEquipmentManager->GetDamage());//鬯ｮ・ｯ繝ｻ・ｬ驛｢譎｢・ｽ・ｻE鬩搾ｽｵ繝ｻ・ｺ髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻE鬩幢ｽ｢隴惹ｼ夲ｽｽ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・｡鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧE鬮ｯ蜈ｷ・ｽ・ｻ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｿ・ｽ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・｡鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢繝ｻ・ｧE鬩幢ｽ｢繝ｻ・ｧ髯ｷ・ｻ髣鯉ｽｨ繝ｻ・ｽ繝ｻ・ｸ髯晢ｽｶ陷ｻ・ｻ繝ｻ・ｽ髢ｾ・･繝ｻ・ｸ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ

}

void Player3D::if_StageOut()
{
	float radiusX = Config::StageVector_x; // X譁ｹ蜷托ｼ域ｨｪ・峨・髯千阜縺ｮ蠎・＆
	float radiusZ = Config::StageVector_z; // Z譁ｹ蜷托ｼ亥･･・峨・髯千阜縺ｮ蠎・＆
	auto scene = Master::mpSceneManager->GetCurrentScene();
	SceneGame* game = dynamic_cast<SceneGame*>(scene);
	VECTOR centerPos;//stageの真ん中
	if (game->mpGameManager->GetCurrentPhase() == GameManager::Phase::BOSS)
	{
		centerPos = VGet(0, 5000, -20000);//空中島の真ん中の地点
		radiusX = 0;
	}
	else
	{
		centerPos = VGet(Config::GetStageCenter().x, 0, Config::GetStageCenter().z);//通常のステージ中央
	}
	float dx = mvPosition.x - centerPos.x;
	float dz = mvPosition.z - centerPos.z;
	//縺昴ｌ縺槭ｌ縺ｮ蜊雁ｾ・〒蜑ｲ縺｣縺ｦ縲∽ｸ譎ら噪縺ｫ縲悟濠蠕・縺ｮ逵溘ｓ荳ｸ縺ｪ蜀・阪・荳也阜縺ｫ螟画鋤縺吶ｋ
	float normX = dx / radiusX;
	float normZ = dz / radiusZ;
	//縺昴・縲悟濠蠕・縺ｮ蜀・阪・荳也阜縺ｧ縺ｮ縲∽ｸｭ蠢・°繧峨・霍晞屬繧定ｨ育ｮ励☆繧・
	float distance = sqrtf(normX * normX + normZ * normZ);
	//霍晞屬縺・1.0 繧定ｶ・∴縺ｦ縺・◆繧会ｼ域･募・縺ｮ螟悶↓縺ｯ縺ｿ蜃ｺ縺励※縺・◆繧会ｼ・
	if (distance > 1.0f)
	{
		mvPosition = mvOldPosition;//map外に出たら位置座標を戻す
	}
	

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
		
		// 鬩幢ｽ｢繝ｻ・ｧE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｩ鬩幢ｽ｢繝ｻ・ｧE鬩幢ｽ｢隰鯉ｽｲ驍ｵ・ｺ郢晢ｽｻ郢晢ｽｻ繝ｻ・ｳ鬩幢ｽ｢繝ｻ・ｧE鬯ｯ・ｮ繝ｻ・｢髯ｷ・ｿ繝ｻ・･E髯具ｽｹ繝ｻ・ｺ髯ｷ繝ｻ・ｽ・ｾ鬩搾ｽｵ繝ｻ・ｺE鬮ｴ蜿厄ｽｻ繧托ｽｽ・ｽ繝ｻ・｡鬮ｫ・ｰ繝ｻ・ｨE鬮ｫ・ｴ陟托ｽｱ繝ｻ邇門ｰ・・・｣鬩幢ｽ｢繝ｻ・ｧ髯句ｹ｢・ｽ・ｵ驍ｵ・ｺ隴惹ｼ夲ｽｽ・ｹ隰鯉ｽｲ驛｢譎｢・ｽ・ｨ (鬮ｯ諞ｺ螻ｮ繝ｻ・ｽ繝ｻ・ｺ鬮ｫ・ｴ陝ｷ・｢繝ｻ・ｽ繝ｻ・ｬ30鬩幢ｽ｢隴弱・・ｽ・ｼ鬩･繝ｻ・ｨ謚ｵ・ｽ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢郢晢ｽｻ+ 鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・ｽ鬮ｯ・ｷ髣・ｽｽ繝ｻ・ｾ雋翫ｑ・ｽ・ｽ繝ｻ・｡鬮ｫ・ｰ繝ｻ・ｨE鬩幢ｽ｢隴弱・・ｽ・ｼ鬩･繝ｻ・ｨ謚ｵ・ｽ・ｹ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢郢晢ｽｻ
		mnInvincibleTimer = 30 + mnUpgradeEvasionInvincibility;
	}
	if (mpModel->GetNowState() == ANIMATION_SLIDE)
	{
		// 鬮ｯ諛・ｻｸ繝ｻ・ｨ繝ｻ・｣驕ｶ謫ｾ・ｽ・ｩ鬯ｯ・ｨ繝ｻ・ｾ髮九・・ｽ・ｷEE驛｢譎｢・ｽ・ｻ鬯ｩ蟶吶・髫ｴ蜿厄ｽｧ・ｫ繝ｻ・ｱ繝ｻ・ｬ驛｢譎｢・ｽ・ｻ髯晢ｽｲ繝ｻ・ｨ驕ｶ莨∬ｱｪ繝ｻ・ｹ繝ｻ・ｧE鬩幢ｽ｢隰鯉ｽｲ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｬ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢隴手・讚ｨ驛｢譎｢・ｽ・ｻ鬩幢ｽ｢繝ｻ・ｧ髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｶE鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
		mvPosition = VAdd(mvPosition, VScale(oldmoveVec, mfEvasionSpeed + mfUpgradeEvasionSpeed));
		mpModel->SetPosition(mvPosition);
		//if_StageOut();
	}






}


void Player3D::RotationByMove()
{

	//////鬯ｮ・｣驕貞､ｧ・ｴ貅ｯ讌懊・・ｦ鬯ｯ・ｯ繝ｻ・ｮ驛｢譎｢・ｽ・ｻEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｯ繝ｻ・ｶE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ驛｢譎｢・ｽ・ｻ髫ｶ髮｣・ｽ・｣髯憺斡・ｦ・ｴ陝・ｫ・ｽｮ蜴・ｽｽ・ｸEE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ諛ｶ・ｽ・｣E鬯ｮ・｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ髫ｲ・､隹ｿ・ｺE
	//Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag_3D_Camera)
	//鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・ｴ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隲幢ｽｶ繝ｻ・ｽ繝ｻ・ｨ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隲帙・・ｻ・ｸE驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ謳ｾ・ｽ・ｵE鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E鬮ｯ蜈ｷ・ｽ・ｾE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｶ髴難ｽ｣陋滂ｽ｡陷・ｽｽ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｯ隲帙・・ｻ・ｸE驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｾ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｱ髯滓汚・ｽ・ｱEEE驕ｶ謫ｾ・ｽ・ｫEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	float subAngle = mfTargetAngle - mfAngle;



	//鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻE鬩阪・遘√・・ｽ繝ｻ・ｭ驛｢譎｢・ｽ・ｻEE鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣E郢晢ｽｻ繝ｻ・ｰ鬯ｩ蟷｢・ｽ・｢E鬮ｯ譎｢・ｽ・ｲE鬩包ｽｶ隴会ｽｮ・主ｿ埼ｫｯ蜈ｷ・ｽ・ｹE鬮ｯ謨鳴E鬯ｮ・ｯ繝ｻ・ｷE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｯ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｾ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬯ｯ・ｲ郢晢ｽｻ郢晢ｽｻ繝ｻ・ｽE鬮ｫ・ｰ郢晢ｽｻEE鬮ｯ蜿･・ｦ・｣EE鬮｣逧ｮ逕･鬩募ｻ｢E驛｢譎｢・ｽ・ｻE鬯ｮ・｣雎郁ｲｻ・ｽ・ｼ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｽE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ鬮ｯ讒ｭ繝ｻEE鬮｣雋ｻ・ｿ・ｽ髯具ｽｹ郢晢ｽｻE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ鬮ｯ・ｷ髣鯉ｽｨ繝ｻ・ｽ繝ｻ・ｷ驛｢譎｢・ｽ・ｻE鬮ｯ譎｢・ｽ・ｲE鬩包ｽｶ闔ｨ竏ｬ・ｱ・ｪEE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ貅ｷ萓帙・・ｨ繝ｻ・ｯ驛｢譎｢・ｽ・ｻ鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ讖ｸ・ｽ・｢E鬩包ｽｶ騾｡遯ｪE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE
	//鬯ｮ・ｯ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｾ驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ謳ｾ・ｽ・ｵE鬯ｯ・ｲ郢晢ｽｻ郢晢ｽｻ繝ｻ・ｽE鬮ｫ・ｰ郢晢ｽｻEE鬮ｯ蜿･・ｦ・｣EE鬯ｮ・ｯ隲帑ｼ夲ｽｽ・ｼ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｽE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ鬮ｯ讒ｭ繝ｻEE鬮｣雋ｻ・ｿ・ｽ髯具ｽｹ郢晢ｽｻE驛｢譎｢・ｽ・ｻE鬯ｮ・｣陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｳ鬮ｯ・ｷ髣鯉ｽｨ繝ｻ・ｽ繝ｻ・ｷ驛｢譎｢・ｽ・ｻE鬮ｯ譎｢・ｽ・ｲE鬩包ｽｶ闔ｨ竏ｬ・ｱ・ｪEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬯ｮ・｢繝ｻ・ｾE鬮ｮ雜｣・ｽ・ｬE鬯ｮ・ｮ隹ｿ・ｺEE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//鬯ｯ・ｮ繝ｻ・ｫ鬮｣蝓ｼ螻ｮ繝ｻ・ｽ繝ｻ・ｲ髫ｶ蜴・ｽｽ・ｸEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｾ驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陷茨ｽｷ繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ驛｢譎｢・ｽ・ｻ髯橸ｽｳ陞｢・ｽ郢晢ｽｻEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ鬯伜・ｽｾ・ｭ驛｢譎｢・ｽ・ｻ鬯ｮ・｣鬮ｮ繝ｻ・ｽ・ｨ繝ｻ・ｯ驛｢譎｢・ｽ・ｻ鬯ｯ・ｮ繝ｻ・ｴ髯ｷ・ｿ繝ｻ・ｰEEE鬯ｩ謳ｾ・ｽ・ｵE鬮｣豈費ｽｼ螟ｲ・ｽ・ｽ繝ｻ・｣驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
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


	//鬯ｮ・｣驕貞､ｧ・ｴ貅ｯ讌懊・・ｦ鬯ｯ・ｯ繝ｻ・ｮ驛｢譎｢・ｽ・ｻEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｯ繝ｻ・ｶE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ驛｢譎｢・ｽ・ｻ髫ｶ髮｣・ｽ・｣髯憺斡・ｦ・ｴ陝・ｫ・ｽｮ蜴・ｽｽ・ｸEE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ諛ｶ・ｽ・｣E鬯ｮ・｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ髫ｲ・､隹ｿ・ｺE
	mfAngle = mfTargetAngle - subAngle;


	//鬯ｮ・ｯ隲帙・・ｻ・ｸE驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ蟷｢・ｽ・｢E鬮ｯ讖ｸ・ｽ・ｳ髯樊ｻゑｽｽ・ｲEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	mvRotation.y = mfAngle + DX_PI_F;
	//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻE髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｮ・｣髮具ｽｻ繝ｻ・ｽ繝ｻ・ｨ鬮ｫ・ｴ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬯ｩ蟷｢・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	mpModel->SetRotation(mvRotation);

	////鬯ｮ・ｯ隲帙・・ｻ・ｸE驛｢譎｢・ｽ・ｻEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ蟷｢・ｽ・｢E鬮ｯ讖ｸ・ｽ・ｳ髯樊ｻゑｽｽ・ｲEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	////鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻE髫ｴ荳ｻ謇ｿE驛｢譎｢・ｽ・ｻE鬯ｮ・｣髮具ｽｻ繝ｻ・ｽ繝ｻ・ｨ鬮ｫ・ｴ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬯ｩ蟷｢・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ


}




void Player3D::Jump()
{

	if (InputManager::CheckDownKey(KEY_INPUT_SPACE))
	{
		mvPosition.y += 300.0f;
		mbjump = true;
		mfjumpPower = JUMP_POWER;

		//鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ蜿厄ｽｨ髮∬｢夜ｫｯ譎｢・ｽ・ｷ髫ｰ遒・ｿｽ謇假ｽｽ・ｲ郢ｧ髮・ｽｵ・ｯ髫ｴ謫ｾ・ｽ・ｴE鬯ｩ蟷｢・ｽ・｢髫ｴ荵励・繝ｻ・ｽ繝ｻ・ｹ髫ｴ竏ｵ・ｻ繝ｻ・ｱ・ｪE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ繝ｻ・ｷE驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ
		
	}

	

}

void Player3D::Result()
{


}

void Player3D::Attack()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻE鬩搾ｽｵ繝ｻ・ｺ鬮｢・ｧ繝ｻ・ｲEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・･驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｲE鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E髯橸ｽｳ陞｢・ｽ隨翫ｋ・ｬ・ｮ繝ｻ・｢驛｢譎｢・ｽ・ｻE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackCount >= AttackTime&&now!=ANIMATION_ATTACK)
	{
		AttackCount = 0;
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACK);
		//鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻE鬨ｾ・ｧ繝ｻ・ｮE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶灘･・ｽｽ・ｺ繝ｻ・ｽ髯具ｽｻ繝ｻ・､鬮ｯ譎｢・ｽ・ｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACK);
		//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE鬯ｮ・ｴ陜捺ｺｷ郢ｭ鬯ｮ・ｮ繝ｻ・ｷ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		mpModel->SetLoop(false);
		//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｮ・ｯ雋・ｽｷ關灘ｦ｣髯滓・・ｪ・ｪ鬯ｮ・ｯ雋・ｽｯ繝ｻ・ｼ陟｢蜥ｲ・ｹ譎｢・ｽ・ｻEE鬨ｾ・ｧ繝ｻ・ｮE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰ鬯ｲ繝ｻ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｽE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
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
		
		if (AttackCount % 5 == 0)
		{
			for (int i = 0; i < pObjList.size(); i++)
			{
				Enemy* pEne = dynamic_cast<Enemy*>(pObjList.at(i));
				if (pEne == nullptr) continue;
				pEne->SetHitjudgment(false);
			}
		}
	}



}

void Player3D::AttackJump()
{
	AnimationState now = mpModel->GetNowState();
	int mouseInput = GetMouseInput(); // 鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻE鬩搾ｽｵ繝ｻ・ｺ鬮｢・ｧ繝ｻ・ｲEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・･驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｲE鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E髯橸ｽｳ陞｢・ｽ隨翫ｋ・ｬ・ｮ繝ｻ・｢驛｢譎｢・ｽ・ｻE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackJumpCount >= AttackJumpTime&&!mbjump)
	{
		Master::mpSoundManager->PlaySE(SoundManager::SE_JUMP);
		mbjump = true;
		AttackJumpCount = 0;
		mfjumpPower = JUMP_POWER;
		//鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻE鬨ｾ・ｧ繝ｻ・ｮE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶灘･・ｽｽ・ｺ繝ｻ・ｽ髯具ｽｻ繝ｻ・､鬮ｯ譎｢・ｽ・ｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACKJUMP);
		//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE鬯ｮ・ｴ陜捺ｺｷ郢ｭ鬯ｮ・ｮ繝ｻ・ｷ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		mpModel->SetLoop(false);
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);
		//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｮ・ｯ雋・ｽｷ關灘ｦ｣髯滓・・ｪ・ｪ鬯ｮ・ｯ雋・ｽｯ繝ｻ・ｼ陟｢蜥ｲ・ｹ譎｢・ｽ・ｻEE鬨ｾ・ｧ繝ｻ・ｮE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰ鬯ｲ繝ｻ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｽE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		
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
			mvPosition.y = hitPos.y;//Y鬯ｮ・ｯ雋・ｽｯ繝ｻ・ｶ繝ｻ・｣EE鬯ｮ・ｫ繝ｻ・ｶ髴難ｽ｣陋滂ｽ｡陷・ｽｽ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ鬩包ｽｯ繝ｻ・ｶE鬯ｩ謳ｾ・ｽ・ｵE鬯ｩ諤憺●繝ｻ・ｽ繝ｻ・ｫ驛｢譎｢・ｽ・ｻE
			
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
	int mouseInput = GetMouseInput(); // 鬯ｩ蟷｢・ｽ・｢髫ｴ蠑ｱ繝ｻE鬩搾ｽｵ繝ｻ・ｺ鬮｢・ｧ繝ｻ・ｲEE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｴ隰・∞・ｽ・ｽ繝ｻ・･驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｲE鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E髯橸ｽｳ陞｢・ｽ隨翫ｋ・ｬ・ｮ繝ｻ・｢驛｢譎｢・ｽ・ｻE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
	if (mouseInput & MOUSE_INPUT_LEFT && AttackSlideCount >= AttackSlideTime)
	{
		if (mpTarget == nullptr)return;//鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ・ｷ繝ｻ・ｻ鬯ｩ蛹・ｽｽ・ｺ髫ｶ魃会ｽｽ・ｳ鬯ｯ・ｩ陟・§・ｾ蜍⑥鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬯ｯ・ｮ繝ｻ・ｦE鬩包ｽｶ騾｡遯ｪE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵEEE鬮ｫ・ｧ繝ｻ・ｽetrurn
		Master::mpSoundManager->PlaySE(SoundManager::SE_ATTACKSLIDE);
		if (mpModel->GetIsSeparate())mpModel->mpSeparateAnimation->SetAnimationCount(1.2f);
		else mpModel->mpAnimation->SetAnimationCount(1.2f);
		AttackSlideCount = 0;
		GoPosition = (VSub(mpTarget->GetPosition(), mvPosition));
		TargetPosition = VScale(GoPosition, 2.5f / 30.0f);

		//鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻE鬨ｾ・ｧ繝ｻ・ｮE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ隶灘･・ｽｽ・ｺ繝ｻ・ｽ髯具ｽｻ繝ｻ・､鬮ｯ譎｢・ｽ・ｲE
		mpModel->ChangeAnimation(ANIMATION_ATTACKSLIDE);
		
		//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ諠ｹ・ｸ讖ｸ・ｽ・ｹ繝ｻ・ｲ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｩ謳ｾ・ｽ・ｵE鬯ｮ・ｴ陜捺ｺｷ郢ｭ鬯ｮ・ｮ繝ｻ・ｷ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		mpModel->SetLoop(false);
		//鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｮ・ｯ雋・ｽｷ關灘ｦ｣髯滓・・ｪ・ｪ鬯ｮ・ｯ雋・ｽｯ繝ｻ・ｼ陟｢蜥ｲ・ｹ譎｢・ｽ・ｻEE鬨ｾ・ｧ繝ｻ・ｮE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰ鬯ｲ繝ｻ・ｼ螟ｲ・ｽ・ｽ繝ｻ・ｽE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		mpModel->SetLoopFinishState(ANIMATION_NEUTRAL);

	}
	

	
	if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide)
	{
		new Effect(VAdd(mvPosition, VGet(0.0f, 0.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 255, 30, 20), 50.0f, 0.03f);
		GoPosition = VNorm(GoPosition);
		mfTargetAngle = atan2f(GoPosition.x, GoPosition.z);


		////////////////鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ螳育櫨隲｢蟷・飴繝ｻ・ｻE鬯ｮ・ｫ繝ｻ・ｰE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｯ雋・ｽｯ繝ｻ・ｶ繝ｻ・｣EE鬯ｮ・ｫ繝ｻ・ｶ髴難ｽ｣陋滂ｽｩ繝ｻ・｡繝ｻ・ｷ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｮ・ｯ繝ｻ・ｷ鬯ｮ・ｦ繝ｻ・ｪ驛｢譎｢・ｽ・ｻ///////////////
		if (AttackSlideCount < 30)
		{
			mvPosition = VAdd(mvPosition, TargetPosition);
			Master::mpCamera->AddHorizontalAngle(90.0f / 30.0f);
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


	mfHp = mfHp < 0 ? 0 : mfHp; //HP鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｫ繝ｻ・ｴ髯晢ｽｷ繝ｻ・｢EE鬯ｮ・ｮ騾｡・ｿ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵEEE驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｭ驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E鬮ｯ貊灘擠繝ｻ・ｯ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｽ驛｢譎｢・ｽ・ｻ
	mfHp = mfHp > GetAllStatusState(Object3D::Status_Hp) ? GetAllStatusState(Object3D::Status_Hp) : mfHp; //HP鬯ｩ謳ｾ・ｽ・ｵE鬮ｫ・ｴ繝ｻ・ｴE鬮ｫ・ｲ繝ｻ・､髯ｷ・ｻ繝ｻ・ｵ髫ｴ・ｽ陟｢蟄秘ｬｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ蟷｢・ｽ・｢E鬮ｯ讖ｸ・ｽ・ｳ髯樊ｻゑｽｽ・ｲEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬮ｯ譎｢・ｽ・ｲEE鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｵ鬮ｫ・ｲ繝ｻ・､髯ｷ・ｻ繝ｻ・ｵ髫ｴ・ｽ陟｢蟄秘ｬｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｭ驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E鬮ｯ貊灘擠繝ｻ・ｯ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｽ驛｢譎｢・ｽ・ｻ
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

	DashGuage = DashGuage < 0 ? 0 : DashGuage; //HP鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｫ繝ｻ・ｴ髯晢ｽｷ繝ｻ・｢EE鬯ｮ・ｮ騾｡・ｿ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵEEE驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｭ驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E鬮ｯ貊灘擠繝ｻ・ｯ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｽ驛｢譎｢・ｽ・ｻ
	DashGuage = DashGuage > MaxDashGauge ? MaxDashGauge : DashGuage; //HP鬯ｩ謳ｾ・ｽ・ｵE鬮ｫ・ｴ繝ｻ・ｴE鬮ｫ・ｲ繝ｻ・､髯ｷ・ｻ繝ｻ・ｵ髫ｴ・ｽ陟｢蟄秘ｬｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ蟷｢・ｽ・｢E鬮ｯ讖ｸ・ｽ・ｳ髯樊ｻゑｽｽ・ｲEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬮ｯ譎｢・ｽ・ｲEE鬩包ｽｶ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｵ鬮ｫ・ｲ繝ｻ・､髯ｷ・ｻ繝ｻ・ｵ髫ｴ・ｽ陟｢蟄秘ｬｮ・ｯ陋ｹ・ｺ繝ｻ・ｻ郢ｧ謇假ｽｽ・ｽ繝ｻ・ｽE鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｭ驛｢譎｢・ｽ・ｻ鬯ｩ蟷｢・ｽ・｢E鬮ｯ貊灘擠繝ｻ・ｯ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｽ驛｢譎｢・ｽ・ｻ
	unsigned int Color3;
	unsigned int Color4;
	Color3 = GetColor(255, 255, 255);
	Color4 = GetColor(0, 0, 0);
	DrawBox(pos2.x, pos2.y, pos2.x + size2.x, pos2.y + size2.y, Color3, true);
	DrawBox(pos2.x + 1, pos2.y + 1,
		pos2.x + size2.x * DashRatio - 1,
		pos2.y + size2.y - 1,
		GetColor(255, 255, 0), true);

	// Slide Attack Cooldown UI
	float SlideRatio = (float)AttackSlideCount / AttackSlideTime;
	SlideRatio = SlideRatio > 1.0f ? 1.0f : SlideRatio;
	VECTOR uiSlidePos = { 30, 820 };
	VECTOR uiSlideSize = { 140, 20 };
	DrawBox(uiSlidePos.x, uiSlidePos.y, uiSlidePos.x + uiSlideSize.x, uiSlidePos.y + uiSlideSize.y, GetColor(0, 0, 0), true);
	DrawBox(uiSlidePos.x + 1, uiSlidePos.y + 1, uiSlidePos.x + uiSlideSize.x * SlideRatio - 1, uiSlidePos.y + uiSlideSize.y - 1, GetColor(0, 255, 255), true);
	DrawFormatString(uiSlidePos.x + 5, uiSlidePos.y + 2, GetColor(255, 255, 255), "SLIDE");

	// Jump Attack Cooldown UI
	float JumpRatio = (float)AttackJumpCount / AttackJumpTime;
	JumpRatio = JumpRatio > 1.0f ? 1.0f : JumpRatio;
	VECTOR uiJumpPos = { 180, 820 };
	VECTOR uiJumpSize = { 150, 20 };
	DrawBox(uiJumpPos.x, uiJumpPos.y, uiJumpPos.x + uiJumpSize.x, uiJumpPos.y + uiJumpSize.y, GetColor(0, 0, 0), true);
	DrawBox(uiJumpPos.x + 1, uiJumpPos.y + 1, uiJumpPos.x + uiJumpSize.x * JumpRatio - 1, uiJumpPos.y + uiJumpSize.y - 1, GetColor(255, 0, 255), true);
	DrawFormatString(uiJumpPos.x + 5, uiJumpPos.y + 2, GetColor(255, 255, 255), "JUMP");

	mpLevelUp->Draw();
	mpShortInventory->Draw();
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
	if (InputManager::CheckDownKey(KEY_INPUT_2))
	{
		// 鬯ｮ・｣陷ｴ繝ｻ・ｽ・ｽ繝ｻ・ｴ鬯ｮ・ｴ鬩帙・・ｽ・ｲ繝ｻ・ｻE髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｫE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｨ隶夐ｦｴ讀ｨ隰・・・ｽ・ｮ隰夲ｽｵ繝ｻ・ｽ繝ｻ・ｫ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｯ・ｮ繝ｻ・ｦE驛｢譎｢・ｽ・ｻE ---------------- //
		Item::ItemInformation* itemInfo;
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::HEAL;
		itemInfo->Name = "陜玲ｧｫ・ｾ・ｩ髦ｮ・ｬ";
		Master::mpItemManager->AddItem(itemInfo);

		// --------------------------------------- //

		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 1;
		itemInfo->ID = Item::ItemID::HIGHHEAL;
		itemInfo->Name = "highheal";
		Master::mpItemManager->AddItem(itemInfo);


		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 2;
		itemInfo->ID = Item::ItemID::POWER;
		itemInfo->Name = "Power";
		Master::mpItemManager->AddItem(itemInfo);

		// 鬯ｯ・ｨ繝ｻ・ｾ驛｢譎｢・ｽ・ｻ驛｢譎｢・ｽ・ｻE郢晢ｽｻ繝ｻ・ｦ
		// 鬯ｮ・ｫ繝ｻ・ｲE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵEEE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻE髯懶ｽ｣繝ｻ・､EE鬮｣蛹・ｽｽ・ｵ髫ｴ雜｣・ｽ・｢E鬮ｴ謇假ｽｼ・ｵemInformation鬯ｩ蟷｢・ｽ・｢E鬯ｮ・ｮ陋ｹ・ｺ繝ｻ・ｧ繝ｻ・ｫ髯溷供・ｮ・｣髴趣ｽｧ髯ｷ閧ｴ・ｺ・ｷ繝ｻ・ｹ繝ｻ・ｲ鬮ｫ・ｨ隶壽遭E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ
		// 鬯ｩ謳ｾ・ｽ・ｵE鬮ｫ・ｴ隰ｫ・ｾ繝ｻ・ｽ繝ｻ・ｴ驛｢譎｢・ｽ・ｻ髫ｶ蜻ｵ・ｶ・｣繝ｻ・ｽ繝ｻ・ｹE鬮ｫ・ｴ繝ｻ・ｽ髯懶ｽｸ繝ｻ・ｾemManager鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｮ・ｮ闕ｵ譎｢・ｽ阮ｦE鬯ｩ謳ｾ・ｽ・ｵE鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｱ鬩包ｽｯ繝ｻ・ｶE鬯ｩ謳ｾ・ｽ・ｵE鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻEE鬯ｩ蟷｢・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ

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
		// 鬯ｮ・｣陷ｴ繝ｻ・ｽ・ｽ繝ｻ・ｴ鬯ｮ・ｴ鬩帙・・ｽ・ｲ繝ｻ・ｻE髯橸ｽｳ陞滂ｽｲ繝ｻ・ｽ繝ｻ・ｫE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ謳ｾ・ｽ・ｵE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻEE鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬮ｫ・ｨ隶夐ｦｴ讀ｨ隰・・・ｽ・ｮ隰夲ｽｵ繝ｻ・ｽ繝ｻ・ｫ驛｢譎｢・ｽ・ｻE驛｢譎｢・ｽ・ｻE鬯ｯ・ｮ繝ｻ・ｦE驛｢譎｢・ｽ・ｻE ---------------- //
		Item::ItemInformation* itemInfo;
		
		itemInfo = new Item::ItemInformation();
		itemInfo->Count = 6;
		itemInfo->ID = Item::ItemID::SPEED;
		itemInfo->Name = "speed";
		Master::mpItemManager->AddItem(itemInfo);
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
		if (pEne == nullptr) return;
		VECTOR mnEneDistance = VSub(pEne->GetPosition(), mvPosition);
		if (check == pEne->GetEnemycoll())
		{
			mbAttackSlideSerch = true;
			float VECSize = VSize(mnEneDistance);
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
	// 
	// 
	// 
	// 
	// && check->mpParentObject->GetTag() == Object3D::Tag3D_Player3D)


	if (collider == mpCapsuleCollider && check->mpParentObject->GetTag() == Tag3D_Obj)
	{
		
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
			if (pEne == nullptr) return;
			if (check == pEne->GetEnemycoll())
			{
				
				// 鬯ｮ・ｫEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫEE鬩幢ｽ｢隴趣ｽ｢EEE鬨ｾ・ｧEE髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢E驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE鬯ｩ蟷｢・ｽ・｢髫ｴ雜｣・ｽ・｢EE&&Attack1
				if (now == ANIMATION_ATTACK && AttackState == Attack_Normal && !mbjump && !pEne->GetHitjudgment())
				{
					
					pEne->Damage(GetAllStatusState(Object3D::Status_Attack));
					// 鬯ｯ・ｨE髯具ｽｹ郢晢ｽｻE鬯ｯ・ｯEE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫEE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ貊灘擠E闔ｨ螟ｲ・ｽ・ｽE驛｢譎｢・ｽ・ｻEE鬮ｯ譎｢・ｽ・ｶ髯ｷ・ｻEE驛｢譎｢・ｽ・ｻ
					Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
					new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(255, 0, 30, 0), 30.0f, 0.1f);
				}


			}

		}
	}
	if (collider == mpAttackJumpCollider && check->mpParentObject->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		if (pEne == nullptr) return;
		if (check == pEne->GetEnemycoll())
		{
			if (now == ANIMATION_ATTACKJUMP && AttackState == Attack_Jump && !pEne->GetHitjudgment())
			{
				pEne->SetHitjudgment(true);
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + mfAttackjump);

				// 鬯ｯ・ｨ繝ｻ・ｾ髯具ｽｹ郢晢ｽｻE鬯ｯ・ｯ繝ｻ・ｮE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ貊灘擠繝ｻ・ｯ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｼ驛｢譎｢・ｽ・ｻEE鬮ｯ譎｢・ｽ・ｶ髯ｷ・ｻ繝ｻ・ｻE驛｢譎｢・ｽ・ｻ
				Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
				new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(0, 255, 30, 0), 30.0f, 0.1f);
			}
		}
	}

	if (collider == mpAttackSlideCollider && check->mpParentObject->GetTag() == Tag3D_Enemy3D)
	{
		Enemy* pEne = dynamic_cast<Enemy*>(check->mpParentObject);
		if (pEne == nullptr) return;
		if (check == pEne->GetEnemycoll())
		{
			if (now == ANIMATION_ATTACKSLIDE && AttackState == Attack_Slide && !pEne->GetHitjudgment())
			{
				pEne->Damage(GetAllStatusState(Object3D::Status_Attack) + mfAttackSlide);
				pEne->SetHitjudgment(true);
				// 鬯ｯ・ｨ繝ｻ・ｾ髯具ｽｹ郢晢ｽｻE鬯ｯ・ｯ繝ｻ・ｮE驛｢譎｢・ｽ・ｻE鬯ｮ・ｫ繝ｻ・ｰE驛｢譎｢・ｽ・ｻE鬯ｩ蟷｢・ｽ・｢E鬮ｯ貊灘擠繝ｻ・ｯ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｼ驛｢譎｢・ｽ・ｻEE鬮ｯ譎｢・ｽ・ｶ髯ｷ・ｻ繝ｻ・ｻE驛｢譎｢・ｽ・ｻ
				Master::mpCamera->SetupShake(5.0f, 10.0f, 5.0f);
				new Effect(VAdd(pEne->GetPosition(), VGet(0.0f, 60.0f, 0.0f)), "Resource/Damage.png", GetColorU8(35, 0, 255, 0), 60.0f, 1.0f);
			}
		}
	}
	


}
void Player3D::mpTargetOn()
{
	if (mpTarget != nullptr)
	{
		bool isTargetValid = false;
		auto mpEneList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < mpEneList.size(); i++)
		{
			if (mpTarget == mpEneList.at(i) && !mpTarget->IsDeleteFlag())
			{
				isTargetValid = true;
				break;
			}
		}
		if (!isTargetValid)
		{
			mpTarget = nullptr;
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
	if (!(now==ANIMATION_ATTACK)&& !(now == ANIMATION_ATTACKJUMP)&& !(now == ANIMATION_ATTACKSLIDE))//鬯ｮ・ｯ隶呻ｽｸ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｮ繝ｻ・ｮ驕ｶ謫ｾ・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｻ鬯ｮ・ｯ雋・ｽｷ隴ｯ繝ｻE鬯ｮ・ｮ髮懶ｽ｣繝ｻ・ｽ繝ｻ・｢驛｢譎｢・ｽ・ｻE
	{
		auto mpEne = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		for (int i = 0; i < mpEne.size(); i++)
		{
			Enemy* pEne = dynamic_cast<Enemy*>(mpEne.at(i));
			if (pEne == nullptr) continue;
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
	// 鬯ｮ・ｯ隶呻ｽｸ髣憺屮・ｽ・ｭ鬮ｫ・ｨ繝ｻ・ｳ驛｢譎｢・ｽ・ｻEE鬯ｯ・ｮ繝ｻ・ｮ驕ｶ謫ｾ・ｽ・ｵ驛｢譎｢・ｽ・ｻ鬯ｮ・ｯ隶厄ｽｸ繝ｻ・ｽ繝ｻ・ｳ鬮ｯ讖ｸ・ｽ・｢E鬮ｯ譎｢・ｽ・ｲE鬯ｮ・ｫ繝ｻ・ｴ驛｢譎｢・ｽ・ｻEE
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
		return mpWeaponManager->GetDamage() + mpBuffManager->GetBuff(state)+mpLevelUp->GetAttack() + mfUpgradeAttack;
	}
	if (state == Status_Speed)
	{
		return mfSpeed + mpBuffManager->GetBuff(state)+mpLevelUp->GetSpeed() + mfUpgradeSpeed;
	}
	if (state == Status_Hp)
	{
		return mfMaxHp + mpLevelUp->GetHp() + mfUpgradeMaxHp;
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


