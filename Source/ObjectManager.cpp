#include"ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

//更新
void ObjectManager::Update()
{
	//2Dオブジェクトの更新
	//mObject2DList.begin()...リスト戦闘の要素へのイテレーターを取得する
	//mObject2DList.end()...リストの末尾の要素へのイテレーターを取得する
	//イテレーターとは？。。。std::Listの要素のことをさす.
	//リストの利点...要素と要素の間に新しい要素を差し込むことが容易にできる
	//std::List...双方系連結リスト(要素の前後の要素にアクセスすることが簡単)

	for (std::list < Object3D*>::iterator itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		(*itr)->Update();
	}

	for (std::list < Object2D*>::iterator itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Update();
	}
	// 3Dの更新が終わった時点でカメラとの距離を計算する
	for (std::list<Object3D*>::iterator itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		VECTOR cameraPos = Master::mpCamera->GetPosition();
		VECTOR objPos = (*itr)->GetPosition();
		(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	// 当たり判定オブジェクトの更新
	ColliderManager::GetInstance()->Update();

}

//描画
void ObjectManager::Draw()
{
	//auto...型推論。＝より右側の型を推測してくれる便利な奴
	//atd::List<Object2D*>::iterator==auto
	
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();

		}
	}
	
}
void ObjectManager::AddObject(Object3D* object3D)
{
	mObject3DList.push_back(object3D);


}

//3Dオブジェクトの全削除
void ObjectManager::DeleteAll3D()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();/*ここは空っななおで注意*/)
	{
		

		//リストから削除
		itr = mObject3DList.erase(itr);

		itr++;
		DeleteAll3DIfNeeded();
	}


}

Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		mObject3DList.begin(),
		mObject3DList.end(),
		[&](Object3D* obj) {return obj->GetTag() == tag; } //ラムダ式
		//[&]...今回の場合、mObject3DListの要素を[参照]するという意味合い
		//(Object3D *obj)...参照したオブジェクトの型と引数名
		//{...}...処理内容（今回は条件)
		//[](){...}この形がラムダ式の基本
	);
	if (itr != mObject3DList.end())
	{
		return (*itr);//オブジェクトが見つかった
	}
	return nullptr;  //オブジェクトが見つからなかった
}

//指定したたぐの3Dオブジェクトのリストを取得
std::vector<Object3D*>ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	std::vector<Object3D*>ret;

	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		//tagとおなじタグを持っているオブジェクトがあればvectorに入れる
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}

	return ret;
}

void ObjectManager::DeleteAll3DIfNeeded()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object3D* temp = *itr;

			//リストから削除
			itr = mObject3DList.erase(itr);

			//オブジェクトそのものを削除
			delete temp;
			temp = nullptr;
		}
		else
		{
			//つぎのitrに進める
			itr++;
		}
	}

}

//２Dオブジェクトの追加
void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
}

//2Dオブジェクトの全削除
void ObjectManager::DeleteAll2D()
{
	Master::mpInfClassManager->LogList.clear();
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();/*ここは空っななおで注意*/)
	{
		Object2D* temp = *itr;

		//リストから削除
		itr = mObject2DList.erase(itr);

		//オブジェクトそのものを削除
		delete temp;
		temp = nullptr;
	}
}

//削除する必要のあるオブジェクトがあれば削除する
void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();/*ここは空っななおで注意*/)
	{
		if ((*itr)->IsDeleteFlag() == true)//省略すると(*itr)->IsDeleteFlag()
		{
			Object2D* temp = *itr;

			//リストから削除
			itr = mObject2DList.erase(itr);

			//オブジェクトそのものを削除
			delete temp;
			temp = nullptr;
		}
		else
		{
			//次の要素へ進める
			itr++;
		}

	}
}

//指定したタグの２Dオブジェクトを取得
Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	//std::findを利用して対象のオブジェクトを探す
	auto itr = std::find_if(
		mObject2DList.begin(),
		mObject2DList.end(),
		[&](Object2D* obj) {return obj->GetTag() == tag; } //ラムダ式
		//[&]...今回の場合、mObject2DListの要素を[参照]するという意味合い
		//(Object2D *obj)...参照したオブジェクトの型と引数名
		//{...}...処理内容（今回は条件)
		//[](){...}この形がラムダ式の基本
	);

	//見つかったかどうかを判定
	if (itr != mObject2DList.end())
	{
		return (*itr);//オブジェクトが見つかった
	}
	return nullptr;  //オブジェクトが見つからなかった
}

//指定したたぐの2Dオブジェクトのリストを取得
std::vector<Object2D*>ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	std::vector<Object2D*>ret;

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		//tagとおなじタグを持っているオブジェクトがあればvectorに入れる
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}

	return ret;
}