#include"Bush.h"
#include"Master.h"
#include"ObjectManager.h"
#include"Stage.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Scene.h"

Bush::Bush(std::string stageModelName, VECTOR initPos,int size) 
	:Object3D(initPos)//座標は原点としておく
   ,mnSize(size)
{
	//タグ設定
	SetTag(Object3D::Tag3D_Stage);

	//ステージモデルの読み込み
	mnModelHandle = MV1LoadModel(stageModelName.c_str());
	//コリジョンモデル(当たり判定用のモデル)の読み込み
	mpModel = new Model(stageModelName, initPos, false);
	mpModel->SetScale(VGet(mnSize, mnSize, mnSize));
	//当たり判定情報の作成　子リジョンなので作ってくれている
	//自動的にデータが作成される便利な関数
	MV1SetPosition(mnModelHandle,initPos);
	mvPosition = initPos;
}

Bush::~Bush()
{
	//読み込んだモデルの破棄
	MV1DeleteModel(mnModelHandle);
	
}

void Bush::Update()
{
	//ステージとの当たり判定をする
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;
	auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::Tag3D_Stage);
	if (obj != nullptr)
	{
		Stage* pStage = dynamic_cast<Stage*>(obj);
		if (pStage != nullptr)
		{
			//ステージとプレイヤーのカプセルが当たっている場合
			if (pStage->CheckHit_Capsule(VAdd(mvPosition, VGet(0.0f, -150.0f, 0.0f)), VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f))
			{
				//当たっているであろうポリゴンとの接触点を求める
				hitPos = pStage->CheckHit_Line(
					VAdd(mvPosition, VGet(0.0f, 1000.0f, 0.0f)),//プレイヤーの膝当たり(多分)と
					VAdd(mvPosition, VGet(0.0f, -1000.0f, 0.0f))//プレイヤーの少し下あたりを線分として指定
				);

				//当たった判定を取っておく
				isHit = true;
			}
		}
	}

	if (isHit)
	{
		//地面に沿って歩いている状態として、Y座標をステージに合わせる
		mvPosition.y = hitPos.y;
	}
	if (isHit == false)
	{
		//要改善
		mvPosition.y += -8.0f;//落下する速度
		if (mvPosition.y <= 0.0f || mvPosition.y <= hitPos.y)
		{
			mvPosition.y = hitPos.y;
		}
	}

}
void Bush::Draw()
{
	//ステージモデルの描画
	mpModel->Draw();

	//コリジョンモデルの描画(ワイヤーフレームみたいな感じで描画)
	// ///当たり判定用のモデルとして作られている
	//読み込むモデル、色、
}

bool Bush::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	//生成していた当たり判定を基にカプセルとの当たり判定を行う
	//コリジョン結果代入用ポリゴン配列
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mnCollisionHandle, -1, pos1, pos2, r);

	//ポリゴンに一つ以上当たっている場合
	if (result.HitNum > 1)
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
	//当たり判定情報の後片づけ
	MV1CollResultPolyDimTerminate(result);


	return(result.HitNum > 1);
}

VECTOR Bush::CheckHit_Line(VECTOR pos1, VECTOR pos2)
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

VECTOR Bush::CheckHit_LineDebug(VECTOR pos1, VECTOR pos2)
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
		DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit: x:%f, y:%f. z:%f", ret.x, ret.y, ret.z);
	}
	else
	{
		DrawFormatString(200, 0, GetColor(255, 0, 0), "Hit None");
	}

	return ret;
}
