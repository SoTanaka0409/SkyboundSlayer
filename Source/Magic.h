#pragma once
#include"dxlib.h"
#include"Collider.h"
#include"ColliderManager.h"
#include"Object3D.h"
#include"Model.h"

class SphereCollider;
class CapsuleCollider;


class Magic:public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Magic(std::string filename,VECTOR initPos,float r,float damage,float speed,VECTOR movevec,int count,int time);//file,initpos,蜊雁ｾ・謾ｻ謦・鴨,speed,蜷代°縺・婿蜷・count,豸医∴繧区凾髢・
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Magic();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Move();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Death();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnEnter(Collider* collider, Collider* check) override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnTrigger(Collider* collider, Collider* check) override;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnExit(Collider* collider, Collider* check) override;
protected:
	SphereCollider* hit_collider_;
	int graph_handle_;
	std::string Filename;
	float speed_;
	float mfAttack_chara;
	float attack_;
	float magic_size_;
	VECTOR move_vec_;
	int DeleteCount;
	int DeleteTime;
	bool delete_;
	
};