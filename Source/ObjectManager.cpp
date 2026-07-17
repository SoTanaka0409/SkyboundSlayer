#include"ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"


/*
 * 目的（ObjectManagerのコンストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] 各種変数の初期化
 */
ObjectManager::ObjectManager()
	: cache_dirty_(true)
{
}


/*
 * 目的（ObjectManagerのデストラクタ）
 * [入力] なし
 * [出力] なし
 * [副作用] オブジェクトの破棄
 */
ObjectManager::~ObjectManager()
{
	DeleteAll3D();
	DeleteAll2D();
}


/*
 * 目的（全オブジェクトの状態を更新するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 各オブジェクトのUpdate呼び出し
 */
void ObjectManager::Update()
{

	for (std::list < Object3D*>::iterator itr = object_3d_list_.begin(); itr != object_3d_list_.end(); itr++)
	{
		(*itr)->Update();
	}

	for (std::list < Object2D*>::iterator itr = object_2d_list_.begin(); itr != object_2d_list_.end(); itr++)
	{
		(*itr)->Update();
	}
	for (std::list<Object3D*>::iterator itr = object_3d_list_.begin(); itr != object_3d_list_.end(); itr++)
	{
		VECTOR cameraPos = Master::camera_->GetPosition();
		VECTOR objPos = (*itr)->GetPosition();
		(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	ColliderManager::GetInstance()->Update();

}


/*
 * 目的（全オブジェクトを描画するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 各オブジェクトのDraw呼び出し
 */
void ObjectManager::Draw()
{
	
	for (auto itr = object_3d_list_.begin(); itr != object_3d_list_.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();
		}
	}
	ColliderManager::GetInstance()->Draw();

	for (auto itr = object_2d_list_.begin(); itr != object_2d_list_.end(); itr++)
	{

		if ((*itr)->IsDrawFlag() == true)
		{
			(*itr)->Draw();

		}
	}
	
}

/*
 * 目的（オブジェクトをリストに追加するため）
 * [入力] オブジェクトポインタ
 * [出力] なし
 * [副作用] リストへの要素追加
 */
void ObjectManager::AddObject(Object3D* object3D)
{
	object_3d_list_.push_back(object3D);
	cache_dirty_ = true;


}


/*
 * 目的（全ての3Dオブジェクトを削除するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 3Dオブジェクトリストのクリア
 */
void ObjectManager::DeleteAll3D()
{
	for (auto itr = object_3d_list_.begin(); itr != object_3d_list_.end(); itr++)
	{
		delete *itr;
	}
	object_3d_list_.clear();
	cache_dirty_ = true;
}

Object3D* 
/*
 * 目的（特定のタグを持つ3Dオブジェクトを取得するため）
 * [入力] Object3D::Tag3D tag
 * [出力] Object3D*
 * [副作用] なし
 */
ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		object_3d_list_.begin(),
		object_3d_list_.end(),
		[&](Object3D* obj) {return obj->GetTag() == tag; }
	);
	if (itr != object_3d_list_.end())
	{
		return (*itr);
	}
	return nullptr;
}

const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	if (cache_dirty_)
	{
		cached_3d_lists_.clear();
		for (auto itr = object_3d_list_.begin(); itr != object_3d_list_.end(); itr++)
		{
			cached_3d_lists_[(*itr)->GetTag()].push_back(*itr);
		}
		cache_dirty_ = false;
	}
	return cached_3d_lists_[tag];
}


/*
 * 目的（削除フラグが立っている3Dオブジェクトを削除するため）
 * [入力] なし
 * [出力] なし
 * [副作用] リストからの要素削除
 */
void ObjectManager::DeleteAll3DIfNeeded()
{
	for (auto itr = object_3d_list_.begin(); itr != object_3d_list_.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object3D* temp = *itr;

			itr = object_3d_list_.erase(itr);
			cache_dirty_ = true;

			delete temp;
			temp = nullptr;
		}
		else
		{
			itr++;
		}
	}

}


/*
 * 目的（オブジェクトをリストに追加するため）
 * [入力] オブジェクトポインタ
 * [出力] なし
 * [副作用] リストへの要素追加
 */
void ObjectManager::AddObject(Object2D* object2D)
{
	object_2d_list_.push_back(object2D);
}


/*
 * 目的（全ての2Dオブジェクトを削除するため）
 * [入力] なし
 * [出力] なし
 * [副作用] 2Dオブジェクトリストのクリア
 */
void ObjectManager::DeleteAll2D()
{
	Master::inf_class_manager_->LogList.clear();
	for (auto itr = object_2d_list_.begin(); itr != object_2d_list_.end();)
	{
		Object2D* temp = *itr;

		itr = object_2d_list_.erase(itr);

		delete temp;
		temp = nullptr;
	}
}


/*
 * 目的（削除フラグが立っている2Dオブジェクトを削除するため）
 * [入力] なし
 * [出力] なし
 * [副作用] リストからの要素削除
 */
void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = object_2d_list_.begin(); itr != object_2d_list_.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object2D* temp = *itr;

			itr = object_2d_list_.erase(itr);

			delete temp;
			temp = nullptr;
		}
		else
		{
			itr++;
		}

	}
}

Object2D* 
/*
 * 目的（特定のタグを持つ2Dオブジェクトを取得するため）
 * [入力] Object2D::Tag2D tag
 * [出力] Object2D*
 * [副作用] なし
 */
ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	auto itr = std::find_if(
		object_2d_list_.begin(),
		object_2d_list_.end(),
		[&](Object2D* obj) {return obj->GetTag() == tag; }
	);

	if (itr != object_2d_list_.end())
	{
		return (*itr);
	}
	return nullptr;
}

std::vector<Object2D*>ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	std::vector<Object2D*>ret;

	for (auto itr = object_2d_list_.begin(); itr != object_2d_list_.end(); itr++)
	{
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}

	return ret;
}
