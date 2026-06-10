#include"Inventory.h"
#include"InputManager.h"
#include"Master.h"
#include"ObjectManager.h"

Inventory::Inventory(Object3D::Tag3D tag)
	:Select(0)
	,SelectMax(3)
	,SelectMin(0)
	,InventoryOn(false)
	,mbPlayer(false)
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	
	if (tag==Object3D::Tag3D_Enemy3D)
	{
		mbPlayer = false;
		mpItemManager = new ItemManager();
		mpWeaponManager = new WeaponManager();
		mpEquipmentManager = new EquipmentManager();
	}
	if (tag == Object3D::Tag3D_Player3D)
	{
		mbPlayer = true;
		
	}
	mpTexture = new Texture("Resource/2D/Inventory.png", VGet(500, 500, 0), true);
	
}

Inventory::~Inventory()
{

}

void Inventory::Draw()
{
	//addが呼び出された瞬間表示する  

}

void Inventory::Update()
{
	if (mbPlayer)
	{
		if (Master::InventoryClasOn)
		{
			mpTexture->Draw();
			SelectClass();//Wsで各コマンドを選択する為
			if (InventoryOn)//各インベントリに触れられるようにするフラグ、falseだとselect1の選択画面になる
			{
				SelectWeapon();
				SelectItem();
				SelectEquipment();
				
			}
			else
			{
				Select1();
			}
		}
	}
	else
	{

	}
}

void Inventory::Select1()
{
	SelectMax = 3;
	
	
		switch (Select)
		{
		case 0:
			list = In_Item;
			DrawBox(400, 400,600,420, GetColor(255, 255, 255), true);
			break;
		case 1:
			list = In_Weapon;	
			DrawBox(400, 450, 600, 470,GetColor(255, 255, 255), true);
			break;
		case 2:
			list = In_Equipment;	
			DrawBox(400, 500, 600, 520, GetColor(255, 255, 255), true);
			break;
		default:
			break;
		}
		DrawFormatString(400, 400, FontColor, "item");
		DrawFormatString(400, 450, FontColor, "Weapon");
		DrawFormatString(400, 500, FontColor, "Equ");
	
		if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
		{
			InventoryOn = true;
			Select = 0;
		}
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))Master::InventoryClasOn = false;
		
	
}

void Inventory::SelectWeapon()
{
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	
	if (list == In_Weapon)
	{
		
		SelectMax = player->mpWeaponManager->mDateList.size();
		
			for (auto itr = player->mpWeaponManager->mDateList.begin(); itr != player->mpWeaponManager->mDateList.end(); itr++)
			{
				if (Select == count)
				{
					DrawBox(mnFirst_X, mnFirst_Y + 50 * count, 1000, mnFirst_Y + 50 + 50 * count, GetColor(0, 0, 0), true);
					if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
					{
						player->mpWeaponManager->ChangeWeapon((*itr));
					

					}
				}
				DrawFormatString(mnFirst_X, mnFirst_Y + 50 * count, GetColor(255, 5, 255), "%s", (*itr)->name.c_str());
				if ((*itr)->mbGet)DrawFormatString(500, 400 + 50 * count, GetColor(255, 0, 0), "装備中");
				count++;
				
				
				
			}
			count = 0;
		
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			InventoryOn = false;
			
		}

	}
	
}

void Inventory::SelectClass()
{
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		Select++;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//効果音
	}
	if (InputManager::CheckDownKey(KEY_INPUT_UP))
	{
		Select--;
		Master::mpSoundManager->PlaySE(SoundManager::SE_SELECT);//効果音

	}
	if (Select >= SelectMax)
	{
		Select = SelectMax-1;
	}
	if (Select <= SelectMin)
	{
		Select = SelectMin;
	}
	
}

void Inventory::SelectEquipment()
{
	if(list== In_Equipment)
	{
		auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

		SelectMax = player->mpEquipmentManager->mDateList.size();

		for (auto itr = player->mpEquipmentManager->mDateList.begin(); itr != player->mpEquipmentManager->mDateList.end(); itr++)
		{
			//if ((*itr)->mbGet)DrawFormatString(500, 200 + 50 * count, GetColor(255, 255, 255), "装備中");
			if (Select == count)
			{
				DrawBox(mnFirst_X, mnFirst_Y + 50 * count, 1000, mnFirst_Y + 50 + 50 * count, GetColor(0, 0, 0), true);
				
				if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
				{
					player->mpEquipmentManager->WearEquipment((*itr));


				}
			}
			DrawFormatString(mnFirst_X, mnFirst_Y + 50 * count, GetColor(255, 255, 255), "%s:", (*itr)->name.c_str());
			if ((*itr)->mbGet)DrawFormatString(500, 200 + 50 * count, GetColor(255,0,0), "装備中");
			count++;


		}
		count = 0;
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			InventoryOn = false;
		}
	}
	
}

void Inventory::SelectItem()
{
	if (list == In_Item)
	{
		auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
		Player3D* player = dynamic_cast<Player3D*>(mpPlayer);

		SelectMax = player->mpItemManager->mItemList.size();
		
			for (auto itr = player->mpItemManager->mItemList.begin(); itr != player->mpItemManager->mItemList.end(); itr++)
			{
				if (Select == count)
				{
					DrawBox(mnFirst_X, mnFirst_Y + 50 * count, 1000, mnFirst_Y+50 + 50 * count, GetColor(0, 0, 0), true);
					if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
					{
						player->mpItemManager->UseItem((*itr)->ID);
					
						
					}
				}
				DrawFormatString(mnFirst_X, mnFirst_Y + 50 * count, GetColor(255, 255, 255), "%s:%d個所持", (*itr)->Name.c_str(), (*itr)->Count);
				count++;
				
				
			}
			count = 0;

		
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			InventoryOn = false;
		}

	}

}
//ショップの受け渡し処理も載せていく
void Inventory::DropInventory(int chance, bool onWeapon, bool onItem)//プレイヤー以外に渡す方法が知りたい
{
	int C = rand()%100;
	if (C > chance)return;//dropする確率
	auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
	Player3D* player = dynamic_cast<Player3D*>(mpPlayer);
	for (auto e = mpWeaponManager->mDateList.begin(); e != mpWeaponManager->mDateList.end(); e++)
	{
		if (!onWeapon)break;
		player->mpWeaponManager->AddWeapon((*e));
		
	}
	for (auto e = mpItemManager->mItemList.begin(); e != mpItemManager->mItemList.end(); e++)
	{
		if (!onItem)break;
		player->mpItemManager->AddItem((*e));
	}
	
	

}