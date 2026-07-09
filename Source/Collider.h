#pragma once
#pragma once
#include "DxLib.h"
#include <vector>

class Object3D;

class Collider
{
public:
	Collider(Object3D* parent);
	virtual ~Collider();

	virtual void Update(Collider* check);
	virtual void Draw();

	virtual void OnEnter();
	virtual void OnTrigger();
	virtual void OnExit();

	void HitCheck(Collider* check, bool isHit);

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }

public:
	Object3D* parent_object_;

	VECTOR position_;
	VECTOR position2_;
	float radius_;

	bool delete_flag_;

protected:
	std::vector<Collider*> collision_list_;	// 衝突しているColliderのリスト
	
};