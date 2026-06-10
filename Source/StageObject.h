#pragma once
#include"Model.h"
#include"Object3D.h"
class StageObject:public Object3D
{
public:
	StageObject(VECTOR initPos, std::string filename,VECTOR scale);

	~StageObject();

	void Update();
	void Draw();

private:
	Model* mpModel;
	float mfScale;
};