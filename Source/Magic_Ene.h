#pragma once
#include"Magic.h"

class Magic_Ene:public Magic
{
public:
	Magic_Ene(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time);

	~Magic_Ene();

	void Update();

	void Draw();

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;
};