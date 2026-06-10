#include"StageObject.h"

StageObject::StageObject(VECTOR initPos,std::string filename,VECTOR scale)
	:Object3D(initPos)
{
	mpModel = new Model(filename, initPos, false);
	mpModel->SetScale(scale);
}
StageObject::~StageObject()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
	
}

void StageObject::Update()
{
	mpModel->Update();
}

void StageObject::Draw()
{
	mpModel->Draw();

}
