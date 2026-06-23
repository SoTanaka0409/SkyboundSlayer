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
	:Object3D(vec)
	,r(200.0f)
	,mnShopListCount(0)
	,oldPosition(vec)
{
	mpModel = new Model(filename, vec, false);
	
	mpWeaponManager = new WeaponManager();
	mpItemManager = new ItemManager();
	mpEquipmentManager = new EquipmentManager();
	mpShopIn = new SphereCollider(this, vec, r);
	mpSafeZoon = new SphereCollider(this, vec, 1000);//é‚µEºé˜®å¶ãƒ»éƒ¢E§ãƒ»E¾éƒ¢æ™¢E½E¼éƒ¢æ™¢E½E³é‚µEºãƒ»E¯é««åE½½E²éƒ¢E§è³å¥E½½æªŽï½¸Eºãƒ»Eªé‚µEºç¹ï½»
	
	Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count =99;
	itemInfo->ID = Item::ItemID::HEAL;
	itemInfo->Name = "é¬¯E®è›Ÿï½¡ãƒ»E´èž¢E¼èž»æ¥¢?åŒE½½E©é«¦E®ãƒ»E¬";
	itemInfo->Name = "è—æ§«E¾E©é˜®E¬";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	
	// --------------------------------------- //

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->Name = "é¬®å€¡E´å£¼å±“è •ï½©é˜®E¬";
	itemInfo->Name = "é¬¯E®è›Ÿï½¡ãƒ»E´èž¢E¼èž»æ¥¢?åŒE½½E©é«¦E®ãƒ»E¬";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);


	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->Name = "è¬¾E»è¬¦ãƒ»é´¨UP";
	itemInfo->Name = "éš°E¾ãƒ»E»éš°E¦ç¹ï½»é­˜ï½¨UP";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 6;
	itemInfo->ID = Item::ItemID::SPEED;
	itemInfo->Name = "ç¹§E¹ç¹æ–ãƒ»ç¹è”šP";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	// é¨¾ãƒ»ãƒ»EŽï½¦
	// éš²E¡ãƒ»E¾é‚µEºãƒ»E£é‚µEºè²E€¥Ežéƒ¢E§ãƒ»E¤éƒ¢æ˜´ãƒ»EŽå ¤E¸Eºèµæ™¢E½è¿‚temInformationéƒ¢E§é›‹æ§«å¾é™Ÿå‹Ÿå¹²ç¬˜ãEE¹E§ç¹ï½»
	// é‚µEºè­ï½´ãƒ»æªŽï½¹E§è­½å—¾emManageré‚µEºãƒ»E«é›‹ã‚‘E½E¡é‚µEºèœ‰ï½±çª¶E»é‚µEºç¹§ãƒ»E¿E¡éƒ¢E§ç¹ï½»

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
	

	//// é—–ï½´éœˆè²»E½å®šï½«E¡ãƒ»E¾é‚µEºãƒ»E£é‚µEºãƒ»E¦éƒ¢E§ç¹§ãƒ»E½E¿ç¹ï½»ç¬˜ãEæ‘Žè®’ï½«ãƒ»E¾ãƒ»E©é«¦E®ãƒ»E¬ ---------------- //

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
		//é«®ä¼šï½½E¼é™·èŒ¨E½E¥é‚µEºèœ·E¶ãƒ»ç‹—ï½¸Eºãƒ»E¨é‚µEºé«¦Eªç¹ï½»é¨¾åŒE½½E»é™·å‰E½¸å²©ãƒ»é›Žè¶£E½Eºé™žï½³èžŸã‚‡æ„›é¬®E±ãƒ»E¢é‚µEºãƒ»E®é«¯E¦ãƒ»E¨é©•ä¼šï½½Eº
		
		player->mpHaveMoney->Draw();
	}
	else
	{
		VECTOR DrawName3D = VAdd(mvPosition, VGet(0.0f, 250.0f, 0.0f));
		VECTOR DrawNameWorld = ConvWorldPosToScreenPos(DrawName3D);

		
		DrawFormatString(DrawNameWorld.x, DrawNameWorld.y, GetColor(255, 0, 0), "Shop::Enteréƒ¢E§èœ»åŸŸï½¬E¾é‚µEºèœ‰ï½±çª¶E»é‚µEºãƒ»E­");

		mpModel->Draw();
	}
	
}

void Shop::Update()
{
	
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
		Buyclass();//éš´å´¢Â€é™‹ï½»è­ï½´ç¹ï½»buyéƒ¢E§ãƒ»E¯éƒ¢æ™¢E½E©éƒ¢E§ãƒ»E¹é‚µEºãƒ»E®é‚µEºãƒ»E¿
		
	}
	movePosition();
}
void Shop::movePosition()
{
	if (mvPosition.y > 0)mvPosition.y -= 2.0f;
	mpShopIn->mvPosition = mvPosition;
	mpModel->Update();
	mpModel->SetPosition(mvPosition);
	mpModel->ChangeAnimation(ANIMATION_NEUTRAL);
}

void Shop::Firstclass()
{


}

void Shop::Selectclass()
{
	if (InputManager::CheckDownKey(KEY_INPUT_UP))
	{
		Select--;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//é™·ä¼šï½½E¹éš´E«è«žï½ºè±¬E¹
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		Select++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//é™·ä¼šï½½E¹éš´E«è«žï½ºè±¬E¹
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
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%då†E", (*list)->Name.c_str(), (*list)->price);
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
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%då†E", (*list)->name.c_str(), (*list)->price);
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
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%då†E", (*list)->name.c_str(), (*list)->price);
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
			Master::mpChat->Draw("çŸ¢å°ã‚­ãƒ¼ã§é¸æŠžã€Gã§æ±ºå®E");
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN)&&!Master::ShopClassOn)
			{
				Master::ShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//åŠ¹æžœéŸ³
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
		Master::mpInfClassManager->LogList.push_back(new InfClass(400, "ƒ}ƒl[‚ª‘«‚è‚Ü‚¹‚ñ", 5));
		return false;
	}
	player->mpHaveMoney->PullMoney(price);
	Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);
	return true;
}
