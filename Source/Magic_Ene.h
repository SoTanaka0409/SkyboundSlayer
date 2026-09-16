#pragma once
#include"Magic.h"

class Magic_Ene:public Magic
{
public:
	Magic_Ene(std::string filename, VECTOR initPos, float r, float damage, float speed, VECTOR movevec, int count, int time);

	~Magic_Ene();

	void Update();
	virtual void Draw() override;

	virtual void OnEnter(Collider* collider, Collider* check) override;
	virtual void OnTrigger(Collider* collider, Collider* check) override;
	virtual void OnExit(Collider* collider, Collider* check) override;

private:
	// === ƒpƒ‰ƒ[ƒ^’è” ===
	static constexpr float kMagicScaleMax = 1.8f;
	static constexpr float kMagicScaleMid = 1.3f;
	static constexpr float kMagicScaleMin = 0.7f;
};