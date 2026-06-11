
#include"Master.h"
#include"ObjectManager.h"
#include"Buff.h"
#include"BuffManager.h"

ItemManager::ItemManager()

{

	/*auto Itemlistinformation = new Item::ItemInformation();
	Itemlistinformation->Count = 3;
	Itemlistinformation->Name = "回復薬";
	Itemlistinformation->ID = Item::HEAL;
	mItemList.push_back(Itemlistinformation);
	auto Itemlistinformation2 = new Item::ItemInformation();
	Itemlistinformation2->Count = 3;
	Itemlistinformation2->Name = "回復薬";
	Itemlistinformation2->ID = Item::HEAL;
	mItemList.push_back(Itemlistinformation2);*/


}
ItemManager::~ItemManager()
{

}

void ItemManager::Update()
{

}



void ItemManager::AddItem(Item::ItemInformation* mItem)
{
    if (mItem == nullptr) return;
    if (Master::mpInfClassManager == nullptr) return;
	mbGetItemflag = true;
	for (auto itr = mItemList.begin(); itr != mItemList.end(); itr++)
	{
		switch (mItem->ID)
		{
		case Item::ItemID::NONE:
			break;
		case Item::ItemID::HEAL:
			mItem->Name = "回復薬";
			mItem->price = 100;
			break;
		case Item::ItemID::POWER:
			mItem->Name = "強化薬";
			mItem->price = 100;
			break;
		case Item::ItemID::HIGHHEAL:
			mItem->Name = "ハイヒ�Eルポ�Eション";
			mItem->price = 300;
			break;
		case Item::ItemID::SPEED:
			mItem->Name = "スピ�Eド�Eーション";
			mItem->price = 50;
		default:
			break;
		}
		if ((*itr)->ID == mItem->ID)//GetItemを回す方法がわからなぁE
		{
			(*itr)->Count += mItem->Count;
			
			if((*mItem).isLog)Master::mpInfClassManager->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));//もしログしてもいぁE��態なら、獲得したとぁE
			return;
		}




	}
	if ((*mItem).isLog)Master::mpInfClassManager->LogList.push_back(new InfClass(400, mItem->Name.c_str(), 1));
	mItemList.push_back(mItem);

}

void ItemManager::UseItem(Item::ItemID id)
{
	auto mpplayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
    Player3D* player = dynamic_cast<Player3D*>(mpplayer);
    if (player == nullptr) return;
	for (auto itr = mItemList.begin(); itr != mItemList.end(); itr++)
	{
		if ((*itr)->ID == id)
		{
			//残りの数が０又は既に使用されてぁE��とぁE
			if ((*itr)->Count <= 0) { Master::mpInfClassManager->LogList.push_back(new InfClass(400, (*itr)->Name.c_str(), 4)); return; }

			if (id == Item::HIGHHEAL || id == Item::HEAL)Master::mpSoundManager->PlaySE(SoundManager::SE_HEAL);//heal音を鳴らす
			if (id == Item::POWER || id == Item::SPEED)Master::mpSoundManager->PlaySE(SoundManager::SE_POWER);//statusUpの音
			(*itr)->Count -= 1;
			(*itr)->mbUse = true;


			Effect(id);
			Master::mpInfClassManager->LogList.push_back(new InfClass(400, (*itr)->Name.c_str(), 2));
			return;
		}

	}

}

void ItemManager::Effect(Item::ItemID id)
{
	auto mpplayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Player3D);
    Player3D* player = dynamic_cast<Player3D*>(mpplayer);
    if (player == nullptr) return;
	if (id == Item::HEAL)
	{
		player->Damage(-20);
	}
	if (id == Item::HIGHHEAL)
	{
		player->Damage(-60);
	}
	if (id == Item::POWER)
	{
		player->mpBuffManager->AddBuff(new Buff(400, 10.0f, Object3D::Status_Attack));
		+player->GetAllStatusState(Object3D::Status_Attack);
	}
	if (id == Item::SPEED)
	{
		player->mpBuffManager->AddBuff(new Buff(400, 10.0f, Object3D::Status_Speed));
		player->GetAllStatusState(Object3D::Status_Attack);
	}


}

//void ItemManager::SetEraseItem(int id)
//{
//	for (auto itr = mItemList.begin(); itr != mItemList.end(); itr++)
//	{
//		if (itr->GetItem() == id)//GetItemを回す方法がわからなぁE
//		{
//			if (itr->GetItemCount() <= 0)
//			{
//				itr = mItemList.erase(itr);
//			}
//		}
//	}
//}
