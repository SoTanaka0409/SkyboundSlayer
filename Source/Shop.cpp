#include"Shop.h"
#include"InputManager.h"
#include"Master.h"
#include"SceneManager.h"
#include"ObjectManager.h"
#include"SphereCollider.h"
#include"CapsuleCollider.h"
#include"InfClass.h"
#include"Chat.h"

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
	mpSafeZoon = new SphereCollider(this, vec, 1000);//このゾーンは襲われない
	
	Item::ItemInformation* itemInfo;
	itemInfo = new Item::ItemInformation();
	itemInfo->Count =99;
	itemInfo->ID = Item::ItemID::HEAL;
	itemInfo->Name = "回復薬";
	itemInfo->price = 100;
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	
	// --------------------------------------- //

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->ID = Item::ItemID::HIGHHEAL;
	itemInfo->Name = "highheal";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);


	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 99;
	itemInfo->ID = Item::ItemID::POWER;
	itemInfo->Name = "Power";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	itemInfo = new Item::ItemInformation();
	itemInfo->Count = 6;
	itemInfo->ID = Item::ItemID::SPEED;
	itemInfo->Name = "speed";
	itemInfo->isLog = true;
	mpItemManager->AddItem(itemInfo);

	// 理想
	// 拾ったアイテムからItemInformationを取得する
	// それをItemManagerに渡してあげる

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
	

	//// 何を拾っても必ず回復薬 ---------------- //
	//Item::ItemInformation* itemInfo;
	//itemInfo = new Item::ItemInformation();
	//itemInfo->Count = 6;
	//itemInfo->ID = Item::ItemID::HEAL;
	//itemInfo->Name = "回復薬";
	//mpItemManager->AddItem(itemInfo);

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

	if (Master::ShopClassOn)
	{
		//購入するときの画像と決定画面の表示
		
		player->mpHaveMoney->Draw();
	}
	else
	{
		VECTOR DrawName3D = VAdd(mvPosition, VGet(0.0f, 250.0f, 0.0f));
		VECTOR DrawNameWorld = ConvWorldPosToScreenPos(DrawName3D);

		
		//(40);
		DrawFormatString(DrawNameWorld.x, DrawNameWorld.y, GetColor(255, 0, 0), "Shop::Enterを押してね");

		mpModel->Draw();
	}
	
}

void Shop::Update()
{
	
	if (Master::ShopClassOn)
	{
		mpTexture->Draw();
		Selectclass();
		Buyclass();//最初はbuyクラスのみ
		
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
	//SelectMax = 1;


}

void Shop::Selectclass()
{
	if (InputManager::CheckDownKey(KEY_INPUT_UP))
	{
		Select--;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//効果音
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		Select++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//効果音
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
			{
				
				if (player->mpHaveMoney->HaveMoney() < (*list)->price) { Master::mpInfClassManager->LogList.push_back(new InfClass(400,"マネーが足りません",5)); return; }
				(*list)->Count = 1;
				player->mpItemManager->AddItem((*list));
				player->mpHaveMoney->PullMoney((*list)->price);
				Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);//効果音
				
			}

		}
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d円", (*list)->Name.c_str(), (*list)->price);
		mnShopListCount++;
	}
	for (auto list = mpWeaponManager->mDateList.begin(); list != mpWeaponManager->mDateList.end(); list++)
	{
		if (mnShopListCount == Select)
		{
			DrawBox(X, Y + 40 * mnShopListCount, X + 300, Y + 30 + 40 * mnShopListCount, GetColor(0, 0, 0), true);
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			{
				if (player->mpHaveMoney->HaveMoney() < (*list)->price) { Master::mpInfClassManager->LogList.push_back(new InfClass(400, "マネーが足りません", 5)); return; }
				Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);//効果音
				player->mpWeaponManager->AddWeapon((*list));
				player->mpHaveMoney->PullMoney((*list)->price);
			}

		}
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d円", (*list)->name.c_str(), (*list)->price);
		mnShopListCount++;
	}
	for (auto list = mpEquipmentManager->mDateList.begin(); list != mpEquipmentManager->mDateList.end(); list++)
	{
		if (mnShopListCount == Select)
		{
			DrawBox(X, Y + 40 * mnShopListCount, X + 300, Y + 30 + 40 * mnShopListCount, GetColor(0, 0, 0), true);
			if (InputManager::CheckDownKey(KEY_INPUT_G))
			{
				
				if (player->mpHaveMoney->HaveMoney() < (*list)->price) { Master::mpInfClassManager->LogList.push_back(new InfClass(400, "マネーが足りません", 5)); return; }
				player->mpEquipmentManager->AddEquipment((*list));
				player->mpHaveMoney->PullMoney((*list)->price);
				Master::mpSoundManager->PlaySE(SoundManager::SE_SHOP);//効果音
			}

		}
		DrawFormatString(X, Y + 40 * mnShopListCount, GetColor(255, 255, 255), "%s:%d円", (*list)->name.c_str(), (*list)->price);
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
			Master::mpChat->Draw("矢印キーで選択、Gで決定");
			if (InputManager::CheckDownKey(KEY_INPUT_RETURN)&&!Master::InventoryClasOn&&!Master::ShopClassOn)
			{
				Master::ShopClassOn = true;
				Master::mpSoundManager->PlaySE(SoundManager::SE_WINDOW);//効果音
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