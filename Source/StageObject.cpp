#include"StageObject.h"

StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius)
	:Object3D(initPos), mfHitRadius(hitRadius)
{
	model_ = new Model(filename, initPos, false);
	model_->SetScale(scale);
	SetTag(Object3D::Tag3D_Object);
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
	TerrainFollow(); // 地形（ステージ）の高さに沿うように自身のposition_を更新
	model_->SetPosition(position_); // 更新された座標をモデルに反映
	model_->Update();
}

void StageObject::Draw()
{
	model_->Draw();

}
