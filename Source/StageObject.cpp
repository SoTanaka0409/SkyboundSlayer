#include"StageObject.h"

StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename)
	:Object3D(initPos)
{
	model_ = new Model(filename, initPos, false);
	model_->SetScale(scale);
	if (!textureFilename.empty())
	{
		model_->SetTexture(textureFilename);
	}
}
StageObject::~StageObject()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
	
}

void StageObject::Update()
{
	model_->Update();
}

void StageObject::Draw()
{
	model_->Draw();

}
