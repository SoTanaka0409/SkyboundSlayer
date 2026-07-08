#include"StageObject.h"

StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius, float yOffset)
	:Object3D(initPos), mfHitRadius(hitRadius), mbHasFollowedTerrain(false), mYOffset(yOffset)
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
	if (!mbHasFollowedTerrain)
	{
		TerrainFollow();
		position_.y += mYOffset;
		mbHasFollowedTerrain = true;
	} // 地形��ス�ージ���高さに沿�ように自身のposition_を更新
	model_->SetPosition(position_); // 更新された座標をモ�ルに反映
	model_->Update();
}

void StageObject::Draw()
{
	model_->Draw();

}
