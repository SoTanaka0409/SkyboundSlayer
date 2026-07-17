#include"StageObject.h"


/*
 * 目的（StageObjectのStageObject処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius, float yOffset)
	:Object3D(initPos), hit_radius_(hitRadius), has_followed_terrain_(false), y_offset_(yOffset)
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


/*
 * 目的（StageObjectのUpdate処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void StageObject::Update()
{
	if (!has_followed_terrain_)
	{
		TerrainFollow();
		position_.y += y_offset_;
		has_followed_terrain_ = true;
	} // 蝨ｰ蠖｢ｼ医せ繝繝ｼ繧ｸｼ峨ｮ鬮倥＆縺ｫ豐ｿ縺繧医≧縺ｫ閾ｪ霄ｫ縺ｮposition_繧呈峩譁ｰ
	model_->SetPosition(position_); // 譖ｴ譁ｰ縺輔ｌ縺溷ｺｧ讓吶ｒ繝｢繝繝ｫ縺ｫ蜿肴丐
	model_->Update();
}


/*
 * 目的（StageObjectのDraw処理を行うため）
 * [入力] 引数参照
 * [出力] 戻り値参照
 * [副作用] クラス内部状態の変更など
 */
void StageObject::Draw()
{
	model_->Draw();

}
