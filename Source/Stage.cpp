#include"Stage.h"
#include"Master.h"

/// @param 初期座標, 表示モデル名, 当たり判定モデル名, スケール, テクスチャ名
/// @details 描画用と判定用（不可視）のモデルを分離してロードし、無効値(-1)時はデフォルトスケールを適用する
Stage::Stage(VECTOR initPos, std::string stageModelName, std::string stageCollisionModelName, VECTOR scale, std::string textureFilename)
	:Object3D(initPos)
{
	SetTag(Object3D::Tag3D_Stage);

	m_ModelHandle = MV1LoadModel(stageModelName.c_str());
	m_CollisionHandle = MV1LoadModel(stageCollisionModelName.c_str());

	// 呼び出し元からスケール指定が省略された場合は、従来の固定ステージサイズ(300.0f)でフォールバックする
	if (scale.x == -1.0f && scale.y == -1.0f && scale.z == -1.0f) {
		float StageSize = 300.0f;
		MV1SetScale(m_ModelHandle, VGet(StageSize, 50.0f, StageSize));
		MV1SetScale(m_CollisionHandle, VGet(StageSize, StageSize, StageSize));
	}
	else {
		MV1SetScale(m_ModelHandle, scale);
		MV1SetScale(m_CollisionHandle, scale);
	}

	MV1SetPosition(m_CollisionHandle, initPos);
	MV1SetPosition(m_ModelHandle, initPos);
	MV1SetupCollInfo(m_CollisionHandle);

	if (!textureFilename.empty()) {
		int texHandle = Master::m_ResourceManager->LoadGraphics(textureFilename);
		if (texHandle != -1) {
			MV1SetTextureGraphHandle(m_ModelHandle, 0, texHandle, FALSE);
		}
	}
}

/// @details VRAM上のモデルリソース（描画用・判定用）を破棄し、シーン切り替え時のメモリリークを防ぐ
Stage::~Stage()
{
	MV1DeleteModel(m_ModelHandle);
	MV1DeleteModel(m_CollisionHandle);
}

/// @details なし（背景ステージ等の静的な環境オブジェクトを想定しているため、動的な状態更新は行わない）
void Stage::Update()
{
}

/// @details プレイヤーに視認させるための描画用モデルのみを描画バッファへ登録する
void Stage::Draw()
{
	MV1DrawModel(m_ModelHandle);
}

/// @param pos1, pos2 (カプセルの始点・終点), r (半径)
/// @return 衝突の有無(bool)
/// @details デバッグ有効時は衝突ポリゴンを可視化描画し、判定後はDxLib側のメモリ(result)を確実に解放する
bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(m_CollisionHandle, -1, pos1, pos2, r);

	if (Master::m_Debug->Getdebug() == true)
	{
		if (result.HitNum >= 1)
		{
			for (int i = 0; i < result.HitNum; i++)
			{
				DrawTriangle3D(
					result.Dim[i].Position[0],
					result.Dim[i].Position[1],
					result.Dim[i].Position[2],
					GetColor(255, 0, 0),
					0
				);
			}
		}
	}

	// DxLibの仕様上、取得したポリゴン情報は明示的に破棄しないとメモリリークを起こすため必須
	MV1CollResultPolyDimTerminate(result);

	return(result.HitNum >= 1);
}

/// @param pos1, pos2 (レイの始点・終点)
/// @return 衝突した空間座標（未ヒット時はゼロベクトル）
/// @details なし（レイキャストによる着弾点の計算や、射線が通っているかの判定などに使用する）
VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
	auto result = MV1CollCheck_Line(m_CollisionHandle, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
	}

	return ret;
}

/// @param pos1, pos2 (レイの始点・終点)
/// @return 衝突した空間座標（未ヒット時はゼロベクトル）
/// @details 画面上にヒットした座標テキスト、または未ヒットの警告UIを描画する（デバッグ用途）
VECTOR Stage::CheckHit_LineDebug(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
	auto result = MV1CollCheck_Line(m_CollisionHandle, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
		if (Master::m_Debug->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit: x:%f, y:%f. z:%f", ret.x, ret.y, ret.z);
		}
	}
	else
	{
		if (Master::m_Debug->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit None");
		}
	}

	return ret;
}