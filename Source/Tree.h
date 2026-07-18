#pragma once
#include"Dxlib.h"
#include "string"
#include"Object3D.h"
#include"Model.h"
class SphereCollider;
class CapsuleCollider;

class Tree :public Object3D
{
public:
	Tree(std::string filename,VECTOR initPos,float Size,float getSize,bool HitFlag);

	~Tree();

// [入力]  [出力]  [副作用] 
	void Draw();

// [入力]  [出力]  [副作用] 
	void Update();

	float GetSize() { return size_; }

// [入力]  [出力]  [副作用] 
	virtual void OnEnter(Collider* collider, Collider* check)override;
// [入力]  [出力]  [副作用] 
	virtual void OnTrigger(Collider* collider, Collider* check)override;
// [入力]  [出力]  [副作用] 
	virtual void OnExit(Collider* collider, Collider* check)override;


	bool GetHitFlag() { return is_hit_flag_; }
private:
	
	CapsuleCollider* capsule_collider_;
	
	Model* model_;
	float size_;
	int model_handle_;
	bool is_hit_flag_;
};