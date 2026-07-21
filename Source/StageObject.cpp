#include"StageObject.h"

/// @param 初期座標, モデルパス, スケール, テクスチャパス, 当たり判定半径, 設置時のY軸オフセット
/// @details 描画用Modelインスタンスの動的生成と、必要に応じた専用テクスチャの上書き適用
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

/// @details 動的確保したModelインスタンスを確実に破棄し、オブジェクト消滅時のメモリリークを防ぐ
StageObject::~StageObject()
{
	if (model_ != nullptr)
	{
		delete model_;
	}
}

/// @details オブジェクト生成直後の1フレーム目のみ地形の高さに合わせて接地座標を計算し、以降はその座標でモデルを更新する
void StageObject::Update()
{
	// 毎フレームのレイキャスト負荷を避けるため、地形への追従計算は初回のみ実行して座標を固定する
	if (!has_followed_terrain_)
	{
		TerrainFollow();
		position_.y += y_offset_;
		has_followed_terrain_ = true;
	}

	model_->SetPosition(position_);
	model_->Update();
}

/// @details 内部で保持しているModelインスタンスへ描画処理を委譲する（描画バッファへの登録）
void StageObject::Draw()
{
	model_->Draw();
}