#pragma once
#include"Model.h"
#include"Object3D.h"
class StageObject:public Object3D
{
public:
	StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename = "", float hitRadius = 0.0f, float yOffset = 0.0f);

	float GetHitRadius() const { return mfHitRadius; }
	bool IsHitEnabled() const { return mfHitRadius > 0.0f; }

	~StageObject();

	void Update();
	void Draw();

private:
	Model* model_;
	float mfScale;
	float mfHitRadius;
	bool mbHasFollowedTerrain;
	float mYOffset;
};
