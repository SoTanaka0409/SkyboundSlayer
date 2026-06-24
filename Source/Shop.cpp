#include"Shop.h"
#include"InputManager.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"InfClass.h"
#include"Chat.h"
#include"SceneGame.h"
#include"GameManager.h"

Shop::Shop(std::string filename, VECTOR vec)
	:Object3D(VGet(vec.x, vec.y, vec.z + 4000.0f))
	,r(200.0f)
	,mnShopListCount(0)
	,oldPosition(vec)
	,mTargetPosition(vec)
	,mShopState(ShopState::WAIT_PHASE)
{
	SetTag(Tag3D_Shop);
	mpModel = new Model(filename, mvPosition, false);
	
	mpWeaponManager = new WeaponManager();
	mpItemManager = new ItemManager();
	mpEquipmentManager = new EquipmentManager();
	mpShopIn = new SphereCollider(this, vec, r);
	mpSafeZoon = new SphereCollider(this, vec, 1000);//驍ｵ・ｺ髦ｮ蜷ｶ繝ｻ驛｢・ｧ繝ｻ・ｾ驛｢譎｢・ｽ・ｼ驛｢譎｢・ｽ・ｳ驍ｵ・ｺ繝ｻ・ｯ鬮ｫ蛛・ｽｽ・ｲ驛｢・ｧ闕ｳ螂・ｽｽ讙趣ｽｸ・ｺ繝ｻ・ｪ驍ｵ・ｺ郢晢ｽｻ
	
	Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count =99;
	itemInfo->ID = Item::ItemID::HEAL;
	itemInfo->Name = "鬯ｯ・ｮ陋滂ｽ｡繝ｻ・ｴ陞｢・ｼ陞ｻ讌｢?蛹・ｽｽ・ｩ鬮ｦ・ｮ繝ｻ・ｬ";
	itemInfo->Name = "陜玲ｧｫ・ｾ・ｩ髦ｮ・ｬ";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	
	// --------------------------------------- //

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->Name = "鬯ｮ蛟｡・ｴ螢ｼ螻楢�包ｽｩ髦ｮ・ｬ";
	itemInfo->Name = "鬯ｯ・ｮ陋滂ｽ｡繝ｻ・ｴ陞｢・ｼ陞ｻ讌｢?蛹・ｽｽ・ｩ鬮ｦ・ｮ繝ｻ・ｬ";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);


	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->Name = "隰ｾ・ｻ隰ｦ繝ｻ魘ｨUP";
	itemInfo->Name = "髫ｰ・ｾ繝ｻ・ｻ髫ｰ・ｦ郢晢ｽｻ鬲假ｽｨUP";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 6;
	itemInfo->ID = Item::ItemID::SPEED;
	itemInfo->Name = "郢ｧ・ｹ郢晄鱒繝ｻ郢晁爆P";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	// 鬨ｾ繝ｻ繝ｻ・趣ｽｦ
	// 髫ｲ・｡繝ｻ・ｾ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ雋・･・樣Δ・ｧ繝ｻ・､驛｢譏ｴ繝ｻ・主�､・ｸ・ｺ闕ｵ譎｢・ｽ霑ＵemInformation驛｢・ｧ髮区ｧｫ蠕宣辧蜍溷ｹｲ隨倥・・ｹ・ｧ郢晢ｽｻ
	// 驍ｵ・ｺ隴擾ｽｴ繝ｻ讙趣ｽｹ・ｧ隴ｽ蝸ｾemManager驍ｵ・ｺ繝ｻ・ｫ髮九ｑ・ｽ・｡驍ｵ・ｺ陷会ｽｱ遯ｶ・ｻ驍ｵ・ｺ郢ｧ繝ｻ・ｿ・｡驛｢・ｧ郢晢ｽｻ

	Weapon::WeaponDate* weaponInf;
	weaponInf = new Weapon::WeaponDate;
	weaponInf->damage = 1;
	weaponInf->filename = "Resource/3D/Sabel.mv1";
	weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon1;
	weaponInf->name = "weapon3";
	weaponInf->isLog = true;
	mpWeaponManager->AddWeapon(weaponInf);


	weaponInf = new Weapon::WeaponDate;
	weaponInf->damage = 5;
	weaponInf->filename = "Resource/3D/Hero.mv1";
	weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon2;
	weaponInf->name = "weapon2";
	weaponInf->isLog = true;
	mpWeaponManager->AddWeapon(weaponInf);

	weaponInf = new Weapon::WeaponDate;
	weaponInf->damage = 7;
	weaponInf->filename = "Resource/3D/Sabel.mv1";
	weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon3;
	weaponInf->name = "weapon3";
	weaponInf->isLog = true;
	mpWeaponManager->AddWeapon(weaponInf);


	weaponInf = new Weapon::WeaponDate;
	weaponInf->damage =9;
	weaponInf->filename = "Resource/3D/Hero.mv1";
	weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon4;
	weaponInf->name = "weapon2";
	weaponInf->isLog = true;
	mpWeaponManager->AddWeapon(weaponInf);

	weaponInf = new Weapon::WeaponDate;
	weaponInf->damage = 10;
	weaponInf->filename = "Resource/3D/Hero.mv1";
	weaponInf->id = Weapon::Tag_Weapon::Tag_Weapon5;
	weaponInf->name = "weapon2";
	weaponInf->isLog = true;
	mpWeaponManager->AddWeapon(weaponInf);

	Equipment::EquipmentDate* equipmentInf;
	equipmentInf = new Equipment::EquipmentDate;
	equipmentInf->damage = 1;
	equipmentInf->id = Equipment::Tag_Equipment1;
	equipmentInf->isLog = true;
	mpEquipmentManager->AddEquipment(equipmentInf);

	equipmentInf = new Equipment::EquipmentDate;
	equipmentInf->damage = 2;
	equipmentInf->id = Equipment::Tag_Equipment2;
	equipmentInf->isLog = true;
	mpEquipmentManager->AddEquipment(equipmentInf);

	equipmentInf = new Equipment::EquipmentDate;
	equipmentInf->damage = 3;
	equipmentInf->id = Equipment::Tag_Equipment3;
	equipmentInf->isLog = true;
	mpEquipmentManager->AddEquipment(equipmentInf);

	equipmentInf = new Equipment::EquipmentDate;
	equipmentInf->damage = 4;
	equipmentInf->id = Equipment::Tag_Equipment4;
	equipmentInf->isLog = true;
	mpEquipmentManager->AddEquipment(equipmentInf);

	equipmentInf = new Equipment::EquipmentDate;
	equipmentInf->damage = 5;
	equipmentInf->id = Equipment::Tag_Equipment5;
	equipmentInf->isLog =true;
	mpEquipmentManager->AddEquipment(equipmentInf);
	

	//// 髣厄ｽｴ髴郁ｲｻ・ｽ螳夲ｽｫ・｡繝ｻ・ｾ驍ｵ・ｺ繝ｻ・｣驍ｵ・ｺ繝ｻ・ｦ驛｢・ｧ郢ｧ繝ｻ・ｽ・ｿ郢晢ｽｻ隨倥・鞫手ｮ抵ｽｫ繝ｻ・ｾ繝ｻ・ｩ鬮ｦ・ｮ繝ｻ・ｬ ---------------- //

	mpTexture = new Texture("Resource/2D/Shop.png", VGet(500, 500, 0), true);
	mpTexture2 = new Texture("Resource/2D/Chat.png", VGet(500, 700, 0), true);

	Y = 220;
	X = 300;


}

Shop::~Shop()
{
	if (mpShopIn != nullptr)
	{
		mpShopIn->SetDeleteFlag(true);
	}
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
	
}

void Shop::Draw()
{
	if (mShopState == ShopState::WAIT_PHASE) return;

	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

	auto currentScene = Master::mpSceneManager->GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(currentScene);
	if (sceneGame && sceneGame->mpGameManager) {
		if (sceneGame->mpGameManager->GetShopTimer() <= 60 && sceneGame->mpGameManager->GetCurrentPhase() != GameManager::Phase::SHOP_3) {
			if (Master::ShopClassOn) {
				Master::ShopClassOn = false;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
			}
		}
	}

	if (Master::ShopClassOn)
	{
		//鬮ｮ莨夲ｽｽ・ｼ髯ｷ闌ｨ・ｽ・･驍ｵ・ｺ陷ｷ・ｶ繝ｻ迢暦ｽｸ・ｺ繝ｻ・ｨ驍ｵ・ｺ鬮ｦ・ｪ郢晢ｽｻ鬨ｾ蛹・ｽｽ・ｻ髯ｷ蜑・ｽｸ蟯ｩ繝ｻ髮手ｶ｣・ｽ・ｺ髯橸ｽｳ陞溘ｇ諢幃ｬｮ・ｱ繝ｻ・｢驍ｵ・ｺ繝ｻ・ｮ鬮ｯ・ｦ繝ｻ・ｨ鬩穂ｼ夲ｽｽ・ｺ
		
		player->mpHaveMoney->Draw();
	}
	else
	{
		VECTOR DrawName3D = VAdd(mvPosition, VGet(0.0f, 250.0f, 0.0f));
		VECTOR DrawNameWorld = ConvWorldPosToScreenPos(DrawName3D);

		
		DrawFormatString(DrawNameWorld.x, DrawNameWorld.y, GetColor(255, 0, 0), "Shop::Enter驛｢・ｧ陷ｻ蝓滂ｽｬ・ｾ驍ｵ・ｺ陷会ｽｱ遯ｶ・ｻ驍ｵ・ｺ繝ｻ・ｭ");

		mpModel->Draw();
	}
	
}

void Shop::Update()
{
	if (mShopState == ShopState::WAIT_PHASE) return;
	
	auto currentScene = Master::mpSceneManager->GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(currentScene);
	if (sceneGame && sceneGame->mpGameManager) {
		if (sceneGame->mpGameManager->GetShopTimer() <= 60 && sceneGame->mpGameManager->GetCurrentPhase() != GameManager::Phase::SHOP_3) {
			if (Master::ShopClassOn) {
				Master::ShopClassOn = false;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);
			}
		}
	}

	if (Master::ShopClassOn)
	{
		mpTexture->Draw();
		Selectclass();
		Buyclass();//髫ｴ蟠｢ﾂ髯具ｽｻ隴擾ｽｴ郢晢ｽｻbuy驛｢・ｧ繝ｻ・ｯ驛｢譎｢・ｽ・ｩ驛｢・ｧ繝ｻ・ｹ驍ｵ・ｺ繝ｻ・ｮ驍ｵ・ｺ繝ｻ・ｿ
		
	}
	movePosition();
}
void Shop::movePosition()
{
	if (mShopState == ShopState::WALKING_IN)
	{
		VECTOR dir = VSub(mTargetPosition, mvPosition);
		dir.y = 0.0f;
		float dist = VSize(dir);
		if (dist < 10.0f)
		{
			mvPosition.x = mTargetPosition.x;
			mvPosition.z = mTargetPosition.z;
			mShopState = ShopState::ARRIVED;
			mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
		}
		else
		{
			VECTOR nDir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(nDir, 4.0f));
			mpModel->ChangeAnimation(ANIMATION_WALKING);
		}
	}
	else if (mShopState == ShopState::WALKING_OUT)
	{
		VECTOR startPos = VGet(mTargetPosition.x, mTargetPosition.y, mTargetPosition.z + 4000.0f);
		VECTOR dir = VSub(startPos, mvPosition);
		dir.y = 0.0f;
		float dist = VSize(dir);
		if (dist < 10.0f)
		{
			mShopState = ShopState::WAIT_PHASE;
			mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
		}
		else
		{
			VECTOR nDir = VNorm(dir);
			mvPosition = VAdd(mvPosition, VScale(nDir, 4.0f));
			mpModel->ChangeAnimation(ANIMATION_WALKING);
		}
	}
	else if (mShopState == ShopState::ARRIVED)
	{
		if (mvPosition.y > mTargetPosition.y) mvPosition.y -= 2.0f;
		mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
	}

	mpShopIn->mvPosition = mvPosition;
	mpModel->SetPosition(mvPosition);
	mpModel->Update();
}

void Shop::StartWalkingIn()
{
	if (mShopState == ShopState::WAIT_PHASE || mShopState == ShopState::WALKING_OUT)
	{
		mShopState = ShopState::WALKING_IN;
		mvPosition = VGet(mTargetPosition.x, mTargetPosition.y, mTargetPosition.z + 4000.0f);
	}
}

void Shop::StartWalkingOut()
{
	if (mShopState == ShopState::ARRIVED || mShopState == ShopState::WALKING_IN)
	{
		mShopState = ShopState::WALKING_OUT;
		Master::ShopClassOn = false;
	}
}

void Shop::Firstclass()
{


}

void Shop::Selectclass()
{
	if (InputManager::CheckDownKey(KEY_INPUT_UP))
	{
		Select--;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//髯ｷ莨夲ｽｽ・ｹ髫ｴ・ｫ隲橸ｽｺ雎ｬ・ｹ
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		Select++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//髯ｷ莨夲ｽｽ・ｹ髫ｴ・ｫ隲橸ｽｺ雎ｬ・ｹ
	}
	if (Select > SelectMax)Select = SelectMax;
	if (Select < SelectMin) Select = SelectMin;
}
void Shop::Sellclass()
{
	


}

void Shop::Buyclass()
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	SelectMax = mpWeaponManager->mDateList.size() + mpItemManager->mItemList.size()+mpEquipmentManager->mDateList.size()-1;
	for (auto list = mpItemManager->mItemList.begin(); list != mpItemManager->mItemList.end(); list++)
	{
		if (mnShopListCount == Select)
		{
			DrawBox(X, Y + 40 * mnShopListCount, X+300,Y+30 + 40 * mnShopListCount, GetColor(0,0,0), true);
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			{
				if (!TryPurchase(player, (*list)->price)) return;
				(*list)->Count = 1;
				Master::mpItemManager->AddItem((*list));
			}

		}
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d蜀・", (*list)->Name.c_str(), (*list)->price);
		mnShopListCount++;
	}
	for (auto list = mpWeaponManager->mDateList.begin(); list != mpWeaponManager->mDateList.end(); list++)
	{
		if (mnShopListCount == Select)
		{
			DrawBox(X, Y + 40 * mnShopListCount, X + 300, Y + 30 + 40 * mnShopListCount, GetColor(0, 0, 0), true);
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			{
				if (!TryPurchase(player, (*list)->price)) return;
				player->mpWeaponManager->AddWeapon((*list));
			}
		}
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d蜀・", (*list)->name.c_str(), (*list)->price);
		mnShopListCount++;
	}
	for (auto list = mpEquipmentManager->mDateList.begin(); list != mpEquipmentManager->mDateList.end(); list++)
	{
		if (mnShopListCount == Select)
		{
			DrawBox(X, Y + 40 * mnShopListCount, X + 300, Y + 30 + 40 * mnShopListCount, GetColor(0, 0, 0), true);
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			{
				if (!TryPurchase(player, (*list)->price)) return;
				player->mpEquipmentManager->AddEquipment((*list));
			}
		}
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d蜀・", (*list)->name.c_str(), (*list)->price);
		mnShopListCount++;
	}
	mnShopListCount = 0;
	if (InputManager::CheckDownKey(KEY_INPUT_BACK)) 
	{
		Master::ShopClassOn = false;
		Master::TutorialCount++;
	}

}

void Shop::OnEnter(Collider* collider, Collider* check)
{
	if (IsDeleteFlag())return;
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	
	if (check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = dynamic_cast<Player3D*>(check->mpParentObject);
		if (collider == mpShopIn && pPlayer->GetCollisionCollider() == check)
		{
			Master::NearShopOn = true;
			Master::mpChat->Draw("遏｢蜊ｰ繧ｭ繝ｼ縺ｧ驕ｸ謚槭；縺ｧ豎ｺ螳・");
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN)&&!Master::ShopClassOn)
			{
				Master::ShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//蜉ｹ譫憺浹
			}
		}

	}
	
	if (collider == mpSafeZoon&& check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
		if (player == nullptr)return;
		if (check == player->GetCollisionCollider())
		{
			Master::SafePointOn = true;
		}

	}
	if (mvPosition.y < 0)mvPosition = oldPosition;


}

void Shop::OnTrigger(Collider* collider, Collider* check)
{

}

void Shop::OnExit(Collider* collider, Collider* check)
{
	if (check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = dynamic_cast<Player3D*>(check->mpParentObject);
		if (collider == mpShopIn && pPlayer->GetCollisionCollider() == check)
		{
			Master::NearShopOn = false;
		}
	}
	
	if (collider == mpSafeZoon&& check->mpParentObject->GetTag() == Tag3D_Player3D)
	{

		Player3D* player = dynamic_cast<Player3D*>(check->mpParentObject);
		if (check == player->GetCollisionCollider())
		{
			Master::SafePointOn = false;

		}

	}
	Master::mpChat->Reset();
}

bool Shop::TryPurchase(Player3D* player, int price)
{
	if (player->mpHaveMoney->HaveMoney() < price)
	{
		Master::mpInfClassManager->LogList.push_back(new InfClass(400, "マネーが足りません", 5));
		return false;
	}
	player->mpHaveMoney->PullMoney(price);
	Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);
	return true;
}
