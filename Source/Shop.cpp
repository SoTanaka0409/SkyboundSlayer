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
	mpSafeZoon = new SphereCollider(this, vec, 1000);//鬩搾ｽｵ繝ｻ・ｺ鬮ｦ・ｮ陷ｷ・ｶ郢晢ｽｻ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｾ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｼ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｳ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｯ鬯ｮ・ｫ陋帙・・ｽ・ｽ繝ｻ・ｲ鬩幢ｽ｢繝ｻ・ｧ髣包ｽｳ陞ゅ・・ｽ・ｽ隶呵ｶ｣・ｽ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｪ鬩搾ｽｵ繝ｻ・ｺ驛｢譎｢・ｽ・ｻ
	
	Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count =99;
	itemInfo->ID = Item::ItemID::HEAL;
	itemInfo->Name = "鬯ｯ・ｯ繝ｻ・ｮ髯区ｻゑｽｽ・｡郢晢ｽｻ繝ｻ・ｴ髯橸ｽ｢繝ｻ・ｼ髯橸ｽｻ隶鯉ｽ｢?陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｩ鬯ｮ・ｦ繝ｻ・ｮ郢晢ｽｻ繝ｻ・ｬ";
	itemInfo->Name = "髯懃軸・ｧ・ｫ繝ｻ・ｾ繝ｻ・ｩ鬮ｦ・ｮ繝ｻ・ｬ";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	
	// --------------------------------------- //

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->Name = "鬯ｯ・ｮ陋滂ｽ｡繝ｻ・ｴ陞｢・ｼ陞ｻ讌｢・ｽ蛹・ｽｽ・ｩ鬮ｦ・ｮ繝ｻ・ｬ";
	itemInfo->Name = "鬯ｯ・ｯ繝ｻ・ｮ髯区ｻゑｽｽ・｡郢晢ｽｻ繝ｻ・ｴ髯橸ｽ｢繝ｻ・ｼ髯橸ｽｻ隶鯉ｽ｢?陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｩ鬯ｮ・ｦ繝ｻ・ｮ郢晢ｽｻ繝ｻ・ｬ";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);


	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->Name = "髫ｰ・ｾ繝ｻ・ｻ髫ｰ・ｦ郢晢ｽｻ鬲假ｽｨUP";
	itemInfo->Name = "鬮ｫ・ｰ繝ｻ・ｾ郢晢ｽｻ繝ｻ・ｻ鬮ｫ・ｰ繝ｻ・ｦ驛｢譎｢・ｽ・ｻ鬯ｲ蛛・ｽｽ・ｨUP";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 6;
	itemInfo->ID = Item::ItemID::SPEED;
	itemInfo->Name = "驛｢・ｧ繝ｻ・ｹ驛｢譎・ｱ堤ｹ晢ｽｻ驛｢譎∫・P";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	// 鬯ｨ・ｾ郢晢ｽｻ郢晢ｽｻ繝ｻ雜｣・ｽ・ｦ
	// 鬮ｫ・ｲ繝ｻ・｡郢晢ｽｻ繝ｻ・ｾ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ髮九・ﾂ・･繝ｻ讓｣ﾎ斐・・ｧ郢晢ｽｻ繝ｻ・､鬩幢ｽ｢隴擾ｽｴ郢晢ｽｻ繝ｻ荳ｻ・ｽ・､繝ｻ・ｸ繝ｻ・ｺ髣包ｽｵ隴趣ｽ｢繝ｻ・ｽ髴托ｼｵemInformation鬩幢ｽ｢繝ｻ・ｧ鬮ｮ蛹ｺ・ｧ・ｫ陟募ｮ｣霎ｧ陷肴ｺｷ・ｹ・ｲ髫ｨ蛟･繝ｻ繝ｻ・ｹ繝ｻ・ｧ驛｢譎｢・ｽ・ｻ
	// 鬩搾ｽｵ繝ｻ・ｺ髫ｴ謫ｾ・ｽ・ｴ郢晢ｽｻ隶呵ｶ｣・ｽ・ｹ繝ｻ・ｧ髫ｴ・ｽ陜ｸ・ｾemManager鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｫ鬮ｮ荵晢ｽ代・・ｽ繝ｻ・｡鬩搾ｽｵ繝ｻ・ｺ髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ驛｢・ｧ郢晢ｽｻ繝ｻ・ｿ繝ｻ・｡鬩幢ｽ｢繝ｻ・ｧ驛｢譎｢・ｽ・ｻ

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
	

	//// 鬮｣蜴・ｽｽ・ｴ鬮ｴ驛・ｽｲ・ｻ繝ｻ・ｽ陞ｳ螟ｲ・ｽ・ｫ繝ｻ・｡郢晢ｽｻ繝ｻ・ｾ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・｣鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｦ鬩幢ｽ｢繝ｻ・ｧ驛｢・ｧ郢晢ｽｻ繝ｻ・ｽ繝ｻ・ｿ驛｢譎｢・ｽ・ｻ髫ｨ蛟･繝ｻ髷ｫ謇具ｽｮ謚ｵ・ｽ・ｫ郢晢ｽｻ繝ｻ・ｾ郢晢ｽｻ繝ｻ・ｩ鬯ｮ・ｦ繝ｻ・ｮ郢晢ｽｻ繝ｻ・ｬ ---------------- //

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

	auto mpPlayer = Master::mpPlayer;
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

	if (!player) return;

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
		//鬯ｮ・ｮ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｼ鬮ｯ・ｷ髣鯉ｽｨ繝ｻ・ｽ繝ｻ・･鬩搾ｽｵ繝ｻ・ｺ髯ｷ・ｷ繝ｻ・ｶ郢晢ｽｻ霑｢證ｦ・ｽ・ｸ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｨ鬩搾ｽｵ繝ｻ・ｺ鬯ｮ・ｦ繝ｻ・ｪ驛｢譎｢・ｽ・ｻ鬯ｨ・ｾ陋ｹ繝ｻ・ｽ・ｽ繝ｻ・ｻ鬮ｯ・ｷ陷代・・ｽ・ｸ陝ｯ・ｩ郢晢ｽｻ鬮ｮ謇具ｽｶ・｣繝ｻ・ｽ繝ｻ・ｺ鬮ｯ讖ｸ・ｽ・ｳ髯樊ｺ假ｽ・ｫ｢蟷・ｽｬ・ｮ繝ｻ・ｱ郢晢ｽｻ繝ｻ・｢鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬯ｮ・ｯ繝ｻ・ｦ郢晢ｽｻ繝ｻ・ｨ鬯ｩ遨ゑｽｼ螟ｲ・ｽ・ｽ繝ｻ・ｺ
		
		player->mpHaveMoney->Draw();
	}
	else
	{
		VECTOR DrawName3D = VAdd(mvPosition, VGet(0.0f, 250.0f, 0.0f));
		VECTOR DrawNameWorld = ConvWorldPosToScreenPos(DrawName3D);

		
		DrawFormatString(DrawNameWorld.x, DrawNameWorld.y, GetColor(255, 0, 0), "Shop::Enter鬩幢ｽ｢繝ｻ・ｧ髯ｷ・ｻ陜捺ｻゑｽｽ・ｬ繝ｻ・ｾ鬩搾ｽｵ繝ｻ・ｺ髯ｷ莨夲ｽｽ・ｱ驕ｯ・ｶ繝ｻ・ｻ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｭ");

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
		Buyclass();//鬮ｫ・ｴ陝�・｢・つ鬮ｯ蜈ｷ・ｽ・ｻ髫ｴ謫ｾ・ｽ・ｴ驛｢譎｢・ｽ・ｻbuy鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｯ鬩幢ｽ｢隴趣ｽ｢繝ｻ・ｽ繝ｻ・ｩ鬩幢ｽ｢繝ｻ・ｧ郢晢ｽｻ繝ｻ・ｹ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｮ鬩搾ｽｵ繝ｻ・ｺ郢晢ｽｻ繝ｻ・ｿ
		
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
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｹ鬮ｫ・ｴ繝ｻ・ｫ髫ｲ讖ｸ・ｽ・ｺ髮趣ｽｬ繝ｻ・ｹ
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		Select++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//鬮ｯ・ｷ闔ｨ螟ｲ・ｽ・ｽ繝ｻ・ｹ鬮ｫ・ｴ繝ｻ・ｫ髫ｲ讖ｸ・ｽ・ｺ髮趣ｽｬ繝ｻ・ｹ
	}
	if (Select > SelectMax)Select = SelectMax;
	if (Select < SelectMin) Select = SelectMin;
}
void Shop::Sellclass()
{
	


}

void Shop::Buyclass()
{
	auto mpPlayer = Master::mpPlayer;
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

	if (!player) return;

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
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d陷繝ｻ", (*list)->Name.c_str(), (*list)->price);
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
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d陷繝ｻ", (*list)->name.c_str(), (*list)->price);
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
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d陷繝ｻ", (*list)->name.c_str(), (*list)->price);
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
	auto mpPlayer = Master::mpPlayer;
	
	if (check->mpParentObject && check->mpParentObject->GetTag() == Tag3D_Player3D)
	{
		Player3D* pPlayer = dynamic_cast<Player3D*>(check->mpParentObject);
		if (collider == mpShopIn && pPlayer->GetCollisionCollider() == check)
		{
			Master::NearShopOn = true;
			Master::mpChat->Draw("驕擾ｽ｢陷奇ｽｰ郢ｧ・ｭ郢晢ｽｼ邵ｺ・ｧ鬩包ｽｸ隰壽ｧｭﾂ・帷ｸｺ・ｧ雎趣ｽｺ陞ｳ繝ｻ");
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN)&&!Master::ShopClassOn)
			{
				Master::ShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//陷会ｽｹ隴ｫ諞ｺ豬ｹ
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
