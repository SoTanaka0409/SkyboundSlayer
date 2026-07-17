#pragma once
#include"Dxlib.h"
#include"Object3D.h"
#include"list"
#include"Enemy.h"

class EnemyManager
{
public:
	enum enemy_tag
	{
		night_stage1,
		archerl_stage1,
		animal_stage1,
		boss_stage1,
		boss_stage2,
		boss_stage3,
		night_Defo,
		Archer_Defo,
		monster_stage1,
	};
	struct enemydate
	{
		enemy_tag tag;
		std::string filename;
		VECTOR spawnCenter;
		VECTOR initPos;
		float hp;//hp
		float speed;//speed
		float attack;//攻撃力
		float HitSize;//あたり判定のサイズ
		float Serch1;//敵をサーチする大きさ
		float Serch2;//敵に攻撃し始めるサイズ
		float Serch3;//敵に近ずきすぎたときに立ち止まる
		bool is_separate_anim_;//セパレートアニメーションをするかどうか
		int MaxEnemyCount;//stageの最大沸き数
		int money;
		int Count;//敵の数
	};

public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	EnemyManager();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~EnemyManager();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void NewEnemyList(enemydate date);
	
	

	std::list<Enemy*>ene_list_;
private:
	enemydate date_;
	enemy_tag tag_;
	
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	/*std::vector<Enemy*> GetEnemyVec() { return enemyVec; }
	std::vector<Enemy*> enemyVec;*/
	
};