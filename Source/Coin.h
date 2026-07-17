#pragma once
#include "Object3D.h"
#include "Model.h"
#include <string>

class Coin : public Object3D
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	Coin(std::string filename, VECTOR pos, int value);
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~Coin();

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

private:
	// 出現直後の上に跳ねる処理
	void UpdatePopPhysics();
	// プレイヤーとの距離を計算し、吸い寄せと回収判定を行う処理
	void UpdateSuckToPlayer();

	// === 定数 ===
	static constexpr float kSpawnOffsetY = 30.0f;
	static constexpr float kScale = 150.0f;
	static constexpr int kPopDuration = 20;
	static constexpr float kPopSpeedY = 2.0f;
	static constexpr float kSuckRadius = 600.0f;
	static constexpr float kSuckSpeed = 30.0f;
	static constexpr float kCollectRadius = 80.0f;

	Model* model_;
	int value_;
	bool is_sucking_;
	bool collected_;
	int age_;
	float speed_;
};
