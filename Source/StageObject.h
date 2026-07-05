#pragma once
#include"Model.h"
#include"Object3D.h"
class StageObject:public Object3D
{
public:
	StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename = "");

	~StageObject();

	void Update();
	void Draw();

private:
	Model* model_;
	float mfScale;
};