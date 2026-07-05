#include"Stage.h"
#include"Master.h"

Stage::Stage(VECTOR initPos,std::string stageModelName, std::string stageCollisionModelName, VECTOR scale, std::string textureFilename)//座標は原点としておく
	:Object3D(initPos)
{
	//タグ設定
	SetTag(Object3D::Tag3D_Stage);
	//ステージモデルの読み込み
	mnModelHandle = MV1LoadModel(stageModelName.c_str());
	
	//コリジョンモデル(当たり判定用のモデル)の読み込み
	mnCollisionHandle = MV1LoadModel(stageCollisionModelName.c_str());

	//デフォルトの引数が渡された場合は従来のスケールを使用
	if (scale.x == -1.0f && scale.y == -1.0f && scale.z == -1.0f) {
		float StageSize = 300.0f;
		MV1SetScale(mnModelHandle, VGet(StageSize, 50.0f, StageSize));
		MV1SetScale(mnCollisionHandle, VGet(StageSize, StageSize, StageSize));
	} else {
		MV1SetScale(mnModelHandle, scale);
		MV1SetScale(mnCollisionHandle, scale);
	}
	MV1SetPosition(mnCollisionHandle, initPos);
	MV1SetPosition(mnModelHandle, initPos);
	MV1SetupCollInfo(mnCollisionHandle);

	if (!textureFilename.empty()) {
		int texHandle = Master::mpResourceManager->LoadGraphics(textureFilename);
		if (texHandle != -1) {
			MV1SetTextureGraphHandle(mnModelHandle, 0, texHandle, FALSE);
		}
	}
	
}

Stage::~Stage()
{
	//読み込んだモデルの破棄
	MV1DeleteModel(mnModelHandle);
	MV1DeleteModel(mnCollisionHandle);
}

void Stage::Update()
{

}
void Stage::Draw()
{
	//ステージモデルの描画
	MV1DrawModel(mnModelHandle);

	//コリジョンモデルの描画(ワイヤーフレームみたいな感じで描画)
	// ///当たり判定用のモデルとして作られている
	//読み込むモデル、色、
}

bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	//生成していた当たり判定を基にカプセルとの当たり判定を行う
	//コリジョン結果代入用ポリゴン配列
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mnCollisionHandle, -1, pos1, pos2, r);

	if (Master::mpDebug->Getdebug() == true)
	{
		//ポリゴンに一つ以上当たっている場合
		if (result.HitNum >= 1)
		{
			//回数を当たった回数を回す
			for (int i = 0; i < result.HitNum; i++)
			{
				//3Dの三角形を描画する
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
	//当たり判定情報の後片づけ
	MV1CollResultPolyDimTerminate(result);


	return(result.HitNum >= 1);
}

VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

	//当たり判定情報と線分との当たり判定を行う
	auto result = MV1CollCheck_Line(mnCollisionHandle, -1, pos1, pos2);

	//当たっていた場合
	if (result.HitFlag)//result.HitNum >= 1
	{
		//当たった個数のポジションをreturnするように取得する
		//壁の時みたいにHitPositionをとる必要ない
		ret = result.HitPosition;
	}

	return ret;
}

VECTOR Stage::CheckHit_LineDebug(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

	//当たり判定情報と線分との当たり判定を行う
	auto result = MV1CollCheck_Line(mnCollisionHandle, -1, pos1, pos2);

	//当たっていた場合
	if (result.HitFlag)//result.HitNum >= 1
	{
		//当たった個数のポジションをreturnするように取得する
		//壁の時みたいにHitPositionをとる必要ない
		ret = result.HitPosition;
		if (Master::mpDebug->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit: x:%f, y:%f. z:%f", ret.x, ret.y, ret.z);
		}
	}
	else
	{
		if (Master::mpDebug->Getdebug() == true)
		{
			DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit None");

		}
	}

	return ret;
}