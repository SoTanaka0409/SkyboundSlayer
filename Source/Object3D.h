#pragma once
#include"DxLib.h"
#include<string>

class Collider;

class Object3D
{
public:
	enum Tag3D
	{
		None3D = 0,//險ｭ螳壹↑縺・
		Tag3D_Player3D = 2000,
		Tag3D_Wall3D = 2100,
		Tag3D_Enemy3D = 2200,
		Tag3D_Stage = 2300,
		Tag3D_Bullet = 2400,
		Tag3D_Weapon = 2500,
		Tag3D_Dino = 2600,
		Tag_3D_Camera=2700,
		Tag_3D_Shield=2800,
		Tag3D_Obj=2900,
		Tag3D_DinoFriend=3000,
		Tag3D_Object = 3050,
		Tag3D_RideDino=3500,
		Tag3D_Shop=4000,
	};
	enum StatusState
	{
		None_Status=0,
		Status_Attack,
		Status_Speed,
		Status_Hp,
		
	}state;
	// 笘・ew!! Z繧ｽ繝ｼ繝育畑笘・
public: // 繧ｽ繝ｼ繝磯未謨ｰ螳夂ｾｩ

	// Z繧ｽ繝ｼ繝育畑縺ｮ險ｭ螳・
	void SetCameraDistance(float distance) { current_camera_distance_ = distance; }

	// 繧ｫ繝｡繝ｩ霍晞屬繧貞渕貅悶↓繧ｽ繝ｼ繝医☆繧九◆繧√・繧ｪ繝壹Ξ繝ｼ繧ｿ繝ｼ
	// note: 繧ｫ繝｡繝ｩ縺ｨ縺ｮ霍晞屬縺碁屬繧後ｋ縺ｻ縺ｩ繝ｪ繧ｹ繝医・謇句燕縺ｫ譚･繧九ｈ縺・↓縺吶ｋ・亥・縺ｫ謠冗判縺励◆縺・ｼ・
	struct CompareZOrder {
		bool operator()(Object3D* a, Object3D* b) const {
			return a->current_camera_distance_ > b->current_camera_distance_;
		}
	};



public:
	Object3D(VECTOR initPos);

	template <typename T>
	T* CastTo() {
		return dynamic_cast<T*>(this);
	}


	virtual ~Object3D();

	virtual void Update();

	virtual void Draw();

	virtual void OnEnter(Collider* collider, Collider* check);
	virtual void OnTrigger(Collider* collider, Collider* check);
	virtual void OnExit(Collider* collider, Collider* check);

	// 蝨ｰ蠖｢・・tage・峨↓鬮倥＆繧貞粋繧上○繧句・逅・
	void TerrainFollow(float capsuleBottomY = -150.0f, float capsuleTopY = 150.0f, float capsuleRadius = 40.0f, float lineTopY = 1000.0f, float lineBottomY = -1000.0f, float gravity = 8.0f);

public://繧ｲ繝・ち繝ｼ縲√そ繝・ち繝ｼw
	void SetPosition(VECTOR pos) { position_ = pos; };
	VECTOR GetPosition() { return position_; }

	void SetOldPosition(VECTOR pos) { old_position_ = pos; }
	VECTOR GetOldPosition() { return old_position_; }

	void SetRotation(VECTOR rot) { rotation_ = rot; }
	VECTOR GETRotation() { return rotation_; }

	void SetDeleteFlag(bool flag) { delete_flag_ = flag; }
	bool IsDeleteFlag() { return delete_flag_; }

	void SetDrawFlag(bool flag) { draw_flag_ = flag; } //謨ｵ縺ｮ蜑企勁繝輔Λ繧ｰ險ｭ螳壹
	bool IsDrawFlag() { return draw_flag_; }          //謨ｵ縺ｮ蜑企勁繝輔Λ繧ｰ縺ｮ菴懈・

	//繧ｿ繧ｰ
	void SetTag(Tag3D tag) { tag_ = tag; }
	Tag3D GetTag() { return tag_; }

protected:
	VECTOR position_;  //蠎ｧ讓・
	VECTOR rotation_;   //蝗櫁ｻ｢
	VECTOR old_position_;
private:
	bool delete_flag_;  //蜑企勁繝輔Λ繧ｰ
	Tag3D tag_;   //繧ｿ繧ｰ
	bool draw_flag_;//謠冗判繝輔Λ繧ｰ
	float current_camera_distance_;     // 迴ｾ蝨ｨ縺ｮ繧ｫ繝｡繝ｩ縺ｨ縺ｮ霍晞屬
	
};