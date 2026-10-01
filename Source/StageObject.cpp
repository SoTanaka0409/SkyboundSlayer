#include"StageObject.h"

/// @param 初期座標, モデルパス, スケール, テクスチャパス, 当たり判定半径, 設置時のY軸オフセット
/// @details 描画用Modelインスタンスの動的生成と、必要に応じた専用テクスチャの上書き適用
StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius, float yOffset)
	:Object3D(initPos), m_HitRadius(hitRadius), m_HasFollowedTerrain(false), m_YOffset(yOffset)
{
	m_Model = new Model(filename, initPos, false);
	m_Model->SetScale(scale);
	SetTag(Object3D::Tag3D_Object);

	if (!textureFilename.empty())
	{
		m_Model->SetTexture(textureFilename);
	}
}

/// @details 動的確保したModelインスタンスを確実に破棄し、オブジェクト消滅時のメモリリークを防ぐ
StageObject::~StageObject()
{
	if (m_Model != nullptr)
	{
		delete m_Model;
	}
}

/// @details オブジェクト生成直後の1フレーム目のみ地形の高さに合わせて接地座標を計算し、以降はその座標でモデルを更新する
void StageObject::Update()
{
	// 毎フレームのレイキャスト負荷を避けるため、地形への追従計算は初回のみ実行して座標を固定する
	if (!m_HasFollowedTerrain)
	{
		TerrainFollow();
		m_Position.y += m_YOffset;
		m_HasFollowedTerrain = true;
	}

	m_Model->SetPosition(m_Position);
	m_Model->Update();
}

/// @details 内部で保持しているModelインスタンスへ描画処理を委譲する（描画バッファへの登録）
void StageObject::Draw()
{
	m_Model->Draw();
}