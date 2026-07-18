#include"Stage.h"
#include"Master.h"

// 入力: 初期座標, 表示モデル名, 当たり判定モデル名, スケール, テクスチャ名 / 出力: なし
// 副作用: 描画用と判定用（不可視）のモデルを分離してロードし、無効値(-1)時はデフォルトスケールを適用する
Stage::Stage(VECTOR initPos, std::string stageModelName, std::string stageCollisionModelName, VECTOR scale, std::string textureFilename)
	:Object3D(initPos)
{
	SetTag(Object3D::Tag3D_Stage);

	model_handle_ = MV1LoadModel(stageModelName.c_str());
	collision_handle_ = MV1LoadModel(stageCollisionModelName.c_str());

	// 呼び出し元からスケール指定が省略された場合は、従来の固定ステージサイズ(300.0f)でフォールバックする
	if (scale.x == -1.0f && scale.y == -1.0f && scale.z == -1.0f) {
		float StageSize = 300.0f;
		MV1SetScale(model_handle_, VGet(StageSize, 50.0f, StageSize));
		MV1SetScale(collision_handle_, VGet(StageSize, StageSize, StageSize));
	}
	else {
		MV1SetScale(model_handle_, scale);
		MV1SetScale(collision_handle_, scale);
	}

	MV1SetPosition(collision_handle_, initPos);
	MV1SetPosition(model_handle_, initPos);
	MV1SetupCollInfo(collision_handle_);

	if (!textureFilename.empty()) {
		int texHandle = Master::resource_manager_->LoadGraphics(textureFilename);
		if (texHandle != -1) {
			MV1SetTextureGraphHandle(model_handle_, 0, texHandle, FALSE);
		}
	}
}

// 入力: なし / 出力: なし
// 副作用: VRAM上のモデルリソース（描画用・判定用）を破棄し、シーン切り替え時のメモリリークを防ぐ
Stage::~Stage()
{
	MV1DeleteModel(model_handle_);
	MV1DeleteModel(collision_handle_);
}

// 入力: なし / 出力: なし
// 副作用: なし（背景ステージ等の静的な環境オブジェクトを想定しているため、動的な状態更新は行わない）
void Stage::Update()
{
}

// 入力: なし / 出力: なし
// 副作用: プレイヤーに視認させるための描画用モデルのみを描画バッファへ登録する
void Stage::Draw()
{
	MV1DrawModel(model_handle_);
}

// 入力: pos1, pos2 (カプセルの始点・終点), r (半径) / 出力: 衝突の有無(bool)
// 副作用: デバッグ有効時は衝突ポリゴンを可視化描画し、判定後はDxLib側のメモリ(result)を確実に解放する
bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(collision_handle_, -1, pos1, pos2, r);

	if (Master::debug_->Getdebug() == true)
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

// 入力: pos1, pos2 (レイの始点・終点) / 出力: 衝突した空間座標（未ヒット時はゼロベクトル）
// 副作用: なし（レイキャストによる着弾点の計算や、射線が通っているかの判定などに使用する）
VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
	auto result = MV1CollCheck_Line(collision_handle_, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
	}

	return ret;
}

// 入力: pos1, pos2 (レイの始点・終点) / 出力: 衝突した空間座標（未ヒット時はゼロベクトル）
// 副作用: 画面上にヒットした座標テキスト、または未ヒットの警告UIを描画する（デバッグ用途）
VECTOR Stage::CheckHit_LineDebug(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
	auto result = MV1CollCheck_Line(collision_handle_, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
		if (Master::debug_->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit: x:%f, y:%f. z:%f", ret.x, ret.y, ret.z);
		}
	}
	else
	{
		if (Master::debug_->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit None");
		}
	}

	return ret;
}