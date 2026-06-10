#pragma once
#include "Object3D.h"
#include "Model.h"
#include <string>

class Coin : public Object3D
{
public:
	Coin(std::string filename, VECTOR pos, int value);
	~Coin();

	void Draw();
	void Update();

private:
	Model* mpModel;
	int mValue;
	bool mIsSucking;
	bool mCollected;
	int mAge;
	float mfSpeed;
};
