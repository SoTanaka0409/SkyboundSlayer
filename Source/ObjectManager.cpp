#include"ObjectManager.h"
#include "Master.h"
#include "ColliderManager.h"

ObjectManager::ObjectManager()
	: mCacheDirty(true)
{
}

ObjectManager::~ObjectManager()
{
	DeleteAll3D();
	DeleteAll2D();
}

void ObjectManager::Update()
{

	for (std::list < Object3D*>::iterator itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		(*itr)->Update();
	}

	for (std::list < Object2D*>::iterator itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		(*itr)->Update();
	}
	for (std::list<Object3D*>::iterator itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		VECTOR cameraPos = Master::mpCamera->GetPosition();
		VECTOR objPos = (*itr)->GetPosition();
		(*itr)->SetCameraDistance(VSize(VSub(objPos, cameraPos)));
	}

	ColliderManager::GetInstance()->Update();

}

void ObjectManager::Draw()
{
	
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
	mCacheDirty = true;


}

void ObjectManager::DeleteAll3D()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
	{
		delete *itr;
	}
	mObject3DList.clear();
	mCacheDirty = true;
}

Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		mObject3DList.begin(),
		mObject3DList.end(),
		[&](Object3D* obj) {return obj->GetTag() == tag; }
	);
	if (itr != mObject3DList.end())
	{
		return (*itr);
	}
	return nullptr;
}

const std::vector<Object3D*>& ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	if (mCacheDirty)
	{
		mCached3DLists.clear();
		for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); itr++)
		{
			mCached3DLists[(*itr)->GetTag()].push_back(*itr);
		}
		mCacheDirty = false;
	}
	return mCached3DLists[tag];
}

void ObjectManager::DeleteAll3DIfNeeded()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object3D* temp = *itr;

			itr = mObject3DList.erase(itr);
			mCacheDirty = true;

			delete temp;
			temp = nullptr;
		}
		else
		{
			itr++;
		}
	}

}

void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
}

void ObjectManager::DeleteAll2D()
{
	Master::mpInfClassManager->LogList.clear();
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		Object2D* temp = *itr;

		itr = mObject2DList.erase(itr);

		delete temp;
		temp = nullptr;
	}
}

void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		if ((*itr)->IsDeleteFlag() == true)
		{
			Object2D* temp = *itr;

			itr = mObject2DList.erase(itr);

			delete temp;
			temp = nullptr;
		}
		else
		{
			itr++;
		}

	}
}

Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag2D tag)
{
	auto itr = std::find_if(
		mObject2DList.begin(),
		mObject2DList.end(),
		[&](Object2D* obj) {return obj->GetTag() == tag; }
	);

	if (itr != mObject2DList.end())
	{
		return (*itr);
	}
	return nullptr;
}

std::vector<Object2D*>ObjectManager::GetObject2DListByTag(Object2D::Tag2D tag)
{
	std::vector<Object2D*>ret;

	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); itr++)
	{
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}

	return ret;
}
