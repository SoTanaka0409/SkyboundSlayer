#include"ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"


/// @brief ObjectManagerのコンストラクタ
/// @details 各種変数の初期化
ObjectManager::ObjectManager()
	: cache_dirty_(true)
{
}


/// @brief ObjectManagerのデストラクタ
/// @details オブジェクトの破棄
ObjectManager::~ObjectManager()
{
	DeleteAll3D();
	DeleteAll2D();
}


/// @brief 全オブジェクトの状態を更新するため
/// @details 各オブジェクトのUpdate呼び出し
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


/// @brief 全オブジェクトを描画するため
/// @details 各オブジェクトのDraw呼び出し
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

/// @brief オブジェクトをリストに追加するため
/// @param オブジェクトポインタ
/// @details リストへの要素追加
void ObjectManager::AddObject(Object3D* object3D)
{
	object_3d_list_.push_back(object3D);
	cache_dirty_ = true;


}


/// @brief 全ての3Dオブジェクトを削除するため
/// @details 3Dオブジェクトリストのクリア
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
/// @brief 特定のタグを持つ3Dオブジェクトを取得するため
/// @param Object3D::Tag3D tag
/// @return Object3D*
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


/// @brief 削除フラグが立っている3Dオブジェクトを削除するため
/// @details リストからの要素削除
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


/// @brief オブジェクトをリストに追加するため
/// @param オブジェクトポインタ
/// @details リストへの要素追加
void ObjectManager::AddObject(Object2D* object2D)
{
	object_2d_list_.push_back(object2D);
}


/// @brief 全ての2Dオブジェクトを削除するため
/// @details 2Dオブジェクトリストのクリア
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


/// @brief 削除フラグが立っている2Dオブジェクトを削除するため
/// @details リストからの要素削除
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
/// @brief 特定のタグを持つ2Dオブジェクトを取得するため
/// @param Object2D::Tag2D tag
/// @return Object2D*
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
