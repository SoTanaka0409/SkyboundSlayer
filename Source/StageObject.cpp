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
	} // 蝨ｰ蠖｢ｼ医せ繝繝ｼ繧ｸｼ峨ｮ鬮倥＆縺ｫ豐ｿ縺繧医≧縺ｫ閾ｪ霄ｫ縺ｮposition_繧呈峩譁ｰ
	model_->SetPosition(position_); // 譖ｴ譁ｰ縺輔ｌ縺溷ｺｧ讓吶ｒ繝｢繝繝ｫ縺ｫ蜿肴丐
	model_->Update();
}

void StageObject::Draw()
{
	model_->Draw();

}
