#include"Dino.h"
#include"Model.h"
#include"Master.h"
#include"Player3D.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"BulletTarget.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Stage.h"
#include"Wall.h"
#include"Scene.h"

Dino::Dino(std::string filename, VECTOR initPos, float hp,float speed)
	:Object3D(initPos)
	,mnTagD(TagDINO_NONE)
	, mnHp(hp)
	, mnInvisible(false)
	, mnSpeed(speed)
	,DinoSize(300.0f)
	,mnMaxHp(hp)
	
{
	
	//モデルの生成
	mpModel = new Model(filename, initPos);

}

Dino::~Dino()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
}

void Dino::Update()
{

	if (mpModel != nullptr)
	{
		RotationByMove();
		Move();
		mpModel->Update();
	}
}

void Dino::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}

	DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
		DinoSize,
		8,
		GetColor(255, 255, 255),
		GetColor(255, 255, 255),
		false
	);

}

void Dino::Dead()
{
}

void Dino::Move()
{
	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);
	VECTOR UpMoveVector = VGet(0.0f, 0.0f, 0.0f);
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);
	{
		////上方向への移動ベクトルは、カメラ視点方向からy成分を抜いたものとする

		////左方向への移動ベクトルが、上方向の移動ベクトルと、Y軸のプラス方向へのベクトルに垂直な方向（外積）


		////移動ベクトルは移動量を加味しないので、正規化しておく(ベクトルの長さを１にすること)
	}

	{
		





		/*auto mpPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Player3D::Tag3D_Player3D);
		Player3D* pPlayer = dynamic_cast<Player3D*>(mpPlayer);

		VECTOR GoPosition = VSub(pPlayer->GetPosition(), mvPosition);
		GoPosition = VNorm(GoPosition);*///プレイヤーへ向かう処理

		float GetmvPositionY = mvPosition.y;
		if (isWalk)
		{
			float  GetmvPositionX = mvPosition.x + rand() % -500 + rand()%500;
			float  GetmvPositionZ = mvPosition.z + rand() % -500 + rand()%500;
			
			isWalk = false;
		}
		PurposePosition = VGet(GetmvPositionX, GetmvPositionY, GetmvPositionZ);
		PurposePosition =VNorm(PurposePosition);
		moveVec = VAdd(moveVec, PurposePosition);

		bool isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
		if (isMove)
		{
			mpModel->ChangeAnimation(ANIMATION_RUN);

			//移動方向を正規化しておく
			mfTargetAngle = atan2f(moveVec.x, moveVec.z);

		}
		
		VECTOR oldPosition = mvPosition;//前回の座標を一旦保持
		mvPosition = VAdd(mvPosition, VScale(moveVec, mnSpeed));
		VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
		//ステージとの当たり判定をする
		// 地形に沿う処理
		TerrainFollow(0.0f, 150.0f, 40.0f, 150.0f, -40.0f, 4.0f);
		bool hitwall = false;
		bool hitwalls = false;
		auto walls = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Wall3D);
		if (!walls.empty())
		{
			// 当たっている壁を調べる
			// hint: 現状の処理では1枚の壁しか最終的に判定されないので、2枚（以上）に当たっていた場合の処理を考える
			for (int i = 0; i < walls.size(); i++)
			{
				Wall* wall = dynamic_cast<Wall*>(walls.at(i));
				if (wall != nullptr)
				{
					std::vector<VERTEX3D> vertex = wall->GetVertex();

					// プレイヤーを包むようなカプセル型の判定と、壁の三角形ポリゴンとの当たり判定を行う
					if (HitCheck_Capsule_Triangle(
						mvPosition,
						VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
						80.0f,
						vertex.at(0).pos, vertex.at(1).pos, vertex.at(2).pos) ||
						HitCheck_Capsule_Triangle(
							mvPosition,
							VAdd(mvPosition, VGet(0.0f, 200.0f, 0.0f)),
							80.0f,
							vertex.at(3).pos, vertex.at(1).pos, vertex.at(2).pos)
						)
					{
						hitwall = true;
						// 壁に沿って行くようなベクトルを取得
						VECTOR slide = VGet(0.0f, 0.0f, 0.0f);  // 壁沿いベクトル
						float a = VDot(VScale(moveVec, -1.0f), vertex.at(0).norm);  // 移動方向ベクトルの反対ベクトルと、壁の法線との内積を求める
						slide = VAdd(moveVec, VScale(vertex.at(0).norm, a));    // 壁沿いベクトルを計算

						if (hitwall == true && hitwalls == false)
						{
							mvPosition = oldPosition;
							mvPosition = VAdd(mvPosition, VScale(slide, mnSpeed));
							hitwalls = true;
						}
						else if (hitwalls == true)
						{
							mvPosition = oldPosition;
						}
					}
				}
			}

		}
		mpModel->SetPosition(mvPosition);

	}
}
void Dino::RotationByMove()
{
	//現在の回転値から目標の回転値の差分を求める
	float subAngle = mfTargetAngle - mfAngle;

	//ある方向からある方向の差が１８０度以上（以下）になることがないはずなので
	//差の値が１８０以上（以下）になっていたら矯正する
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	//角度の差分を徐々に０に近づける
	if (subAngle > 0.0f)
	{
		subAngle -= ROTATE_SPEED;
		if (subAngle < 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	else if (subAngle < 0.0f)
	{
		subAngle += ROTATE_SPEED;
		if (subAngle > 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	//今向いてほしい角度を産出
	mfAngle = mfTargetAngle - subAngle;

	//回転値を設定
	mvRotation.y = mfAngle + DX_PI_F;
	//モデルに伝える
	mpModel->SetRotation(mvRotation);


}

void Dino::Invisible()
{
	mnInvisible = true;


}

void Dino::InvisibleNo()
{
	mnInvisible = false;
}
