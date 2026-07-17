#pragma once
#pragma once
#include "DxLib.h"
#include "Texture.h"
#include"Object3D.h"
#include"Model.h"
#include"Debug.h"


#include"Item.h"
#include"ColliderManager.h"
#include"EquipmentManager.h"

class SphereCollider;
class CapsuleCollider;


class Enemy :public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetHitSizeFlag(bool flag) { is_hit_size_flag_ = flag; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsHitSizeFlag() { return is_hit_size_flag_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetNewEnemyFlag(bool flag) { is_new_enemy_flag_ = flag; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsNewEnemyFlag() { return is_new_enemy_flag_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual CapsuleCollider* GetEnemyCollider() { return capsule_collider_; }//邵ｺ阮呻ｽ檎ｸｺ謔滓ｬ｡陜暦ｿｽ繝ｻ貅倥″郢晏干縺晉ｹ晢ｽｫ郢ｧ・ｳ郢晢ｽｩ郢ｧ・､郢敖郢晢ｽｼ邵ｺ・ｽ邵ｺ隨ｬ・ｶ蛹ｻ竏ｴ邵ｺ・ｪ邵ｺ繝ｻ

private:
	bool is_hit_size_flag_;//陟冶侭笳・ｹｧ髮∵・陞ｳ螟ら舞邵ｺ・ｮ郢ｧ・ｵ郢ｧ・､郢ｧ・ｺ郢ｧ蜻域亜邵ｺ・｣邵ｺ・ｦ邵ｺ荳奇ｽ狗ｸｺ荵昶・邵ｺ繝ｻﾂｰ邵ｺ・ｮflag?
	bool is_new_enemy_flag_;//隰ｨ・ｵ郢ｧ蝨ｰew邵ｺ蜷ｶ・狗ｸｺ貅假ｽ∫ｸｺ・ｮ郢晁ｼ釆帷ｹｧ・ｰ繝ｻ貊会ｽｽ・ｿ邵ｺ繝ｻ蟀ｿ邵ｺ・ｯ郢ｧ蛹ｻ・･郢ｧ荳環ｰ郢ｧ蟲ｨ竊醍ｸｺ繝ｻ

public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Enemy(std::string filename, VECTOR initPos, float hp, float speed, float attack, float HitSize, float Serch1, float Serch2,float Serch3,int money,bool is_separate_anim_);

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Enemy();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw()override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update()override;

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Move();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    virtual void RotationByMove();//驕假ｽｻ陷崎ｼ披・郢ｧ蛹ｻ・玖摎讚・ｽｻ・｢陷・ｽｦ騾・・
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Damage(float damage);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void AttackList();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Attack();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void Delete();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void DeathColliderPosition();


    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void DeathEnemy();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void GiveRewards();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void UpdateColliderPosition();


    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetHp() { return hp_; };
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetHp(float hp) { hp_ = hp; };

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetMaxHp() { return max_hp_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetMaxHp(float mhp) { max_hp_ = mhp; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool GetInvisible() { return is_invisible_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetInvisible(int Inv) { is_invisible_ = Inv; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VECTOR GetGoPosition() { return go_position_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetGoPosition(VECTOR goplayer) { go_position_ = goplayer; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetInitPosition(VECTOR pos) { init_position_ = pos; }//陋ｻ譎・ｄ郢晢ｽｪ郢ｧ・ｹ郢晄亢繝ｻ郢晢ｽｳ陜ｨ・ｰ霓､・ｹ邵ｺ・ｮ陟趣ｽｧ隶灘生・定愾謔ｶ笆ｲ邵ｺ・ｦ邵ｺ鄙ｫ・･
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VECTOR GetInitPosition() { return init_position_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	float GetSize() { return size_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetHitJudgmentFlagPlayer(bool flag) { is_hit_judgment_flag_player_=flag; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsHitJudgmentFlagPlayer() { return is_hit_judgment_flag_player_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsDead() { return is_dead_; }
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetIsDead(bool dead) { is_dead_ = dead; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsHitSearchFlag() const { return is_hit_search_flag_; }


    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnEnter(Collider* collider, Collider* check) ;
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnTrigger(Collider* collider, Collider* check);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	virtual void OnExit(Collider* collider, Collider* check) ;





private:
	

protected:
	
	SphereCollider* attach_collider_;
	CapsuleCollider* capsule_collider_;
	SphereCollider* serch_collider_;
	SphereCollider* attack_collider_;//隰ｾ・ｻ隰ｦ繝ｻ・陝倶ｹ晢ｽ∫ｹｧ蜿･繝ｻ
	SphereCollider* stop_collider_;
	Model* model_;
	Debug* debug_;
protected:
	float hp_;
	float max_hp_;
	float speed_;
	float normal_speed_;
	float max_speed_;
	float attack_;

	int walk_timer_;//陷ｷ蠕個ｧ陞ｳ譎擾ｽｺ・ｫ邵ｺ・ｫ雎・ｽｩ邵ｺ蜥ｲ・ｶ螢ｹ・郢ｧ蛹ｺ蜃ｾ鬮｢繝ｻ
	int walk_count_;//

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	VECTOR hit_pos_ = VGet(0.0f, 0.0f, 0.0f);
	VECTOR go_position_;//陷ｷ莉｣ﾂｰ邵ｺ繝ｻ繝ｻ郢ｧ・ｯ郢晏現ﾎ昴・蛹ｻ繝ｻ郢晢ｽｬ郢ｧ・､郢晢ｽ､郢晢ｽｼ郢ｧ蜻遺楳驕擾ｽ･邵ｺ蜉ｱ笳・ｸｺ・ｨ邵ｺ繝ｻ
	VECTOR old_position_;
	VECTOR init_position_;//spown陜ｨ・ｰ霓､・ｹ
	VECTOR no_position_;
	VECTOR move_vec_;
	VECTOR up_move_vector_;
	VECTOR left_move_vector_;
	bool is_invisible_;//霎滂ｽ｡隰ｨ・ｵ邵ｺ荵昶・邵ｺ繝ｻﾂｰ
	int algorithm_id_;//郢晢ｽｩ郢晢ｽｳ郢敖郢晢ｿｽ驕假ｽｻ陷阪・
	int alg_hit_;

	int attack_interval_;
	int attack_count_;

	bool is_animation_;
	float target_angle_;//騾ｶ・ｮ隶灘生繝ｻ陜玲ｫ・ｽｻ・｢陜ｨ・ｰ
	float angle_;//霑ｴ・ｾ陜ｨ・ｨ邵ｺ・ｮ陜玲ｫ・ｽｻ・｢陜ｨ・ｰ
	const float ROTATE_SPEED = 0.1f;//陜玲ｫ・ｽｻ・｢鬨ｾ貅ｷ・ｺ・ｦ

	float size_;//隰ｨ・ｵ邵ｺ・ｮ郢ｧ・ｵ郢ｧ・､郢ｧ・ｺ
	float hit_search_;//髴台ｻ｣・･邵ｺ・ｫ郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ蠕鯉ｼ樒ｹｧ荵敖ｰ
	float hit_attack_search_;//髴台ｻ｣・･邵ｺ・ｫ郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ蠕鯉ｼ樒ｹｧ荵敖ｰ&隰ｾ・ｻ隰ｦ繝ｻ縲堤ｸｺ髦ｪ・矩恪譎槫ｱｬ邵ｺ繝ｻ
	float hit_stop_search_;//髴台ｻ｣・･邵ｺ・ｫ郢晏干ﾎ樒ｹｧ・､郢晢ｽ､郢晢ｽｼ邵ｺ蠕鯉ｼ樒ｹｧ繝ｻ鬩包ｽｩ陋ｻ繝ｻ竊鷹恪譎槫ｱｬ邵ｺ繝ｻ
	bool is_dead_;

	bool is_hit_search_flag_;
	bool is_hit_attack_search_flag_;
	bool is_hit_search_stop_flag_;
	bool is_hit_attack_flag_;//隰ｾ・ｻ隰ｦ繝ｻ窶ｲ陟冶侭笳・ｸｺ・｣邵ｺ貅伉ｰ

	int chance_;//inventory郢ｧ螳夊ｪ邵ｺ・ｨ邵ｺ蜥擾ｽ｢・ｺ驍・・
	
	bool has_item_=true;//item郢ｧ螳夊ｪ邵ｺ・ｨ邵ｺ蜷ｶﾂｰ陷ｷ・ｦ邵ｺ繝ｻ

	bool is_attack_hit_judgment_flag_;//隰ｾ・ｻ隰ｦ繝ｻ窶ｲ闕ｳﾂ陜玲ｧｫ・ｽ阮吮螺邵ｺ・｣邵ｺ貅假ｽ臥ｸｺ譏ｴ繝ｻ隰ｨ・ｵ邵ｺ・ｫ陝・ｽｾ邵ｺ蜉ｱ窶ｻ隰ｾ・ｻ隰ｦ繝ｻ窶ｲ鬩･蟠趣ｽ､繝ｻ・邵ｺ・ｪ邵ｺ繝ｻ・育ｸｺ繝ｻ竊鍋ｸｺ蜷ｶ・・

	bool is_hit_judgment_flag_player_;
	int have_money_;
	/// <summary>
	/// ///////////郢昶・ﾎ礼ｹ晢ｽｼ郢晏現ﾎ懃ｹｧ・｢郢晢ｽｫ
	/// </summary>

	

};

