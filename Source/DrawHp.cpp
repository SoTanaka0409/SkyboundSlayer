
#include"DrawHp.h"
#include"Enemy3D.h"
#include"Master.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"Model.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Scene.h"
#include"Player3D.h"



DrawHp::DrawHp()
{
	
}



DrawHp::~DrawHp()
{

}


//更新
void DrawHp::Update()
{
	//明日はバーを作って右下と右上を削るようにプログラムを作る
	//敵の見ている方角に合わせ、ｚ座標もｘ座標も合わせる
	//mpmodel->update()みたいにＤｒａｗＨＰもエネミーのアップデート処理に入れる
	const auto& pObjList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	for (int i = 0; i < pObjList.size(); i++)
	{
		auto pObj = pObjList[i];
		const auto& pEnemyList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
		auto pEnemy = pEnemyList[i];

		Enemy* pEne = pEnemy->CastTo<Enemy>();

		if (pObj != nullptr)
		{
			VECTOR DrawHpBar3D = VAdd(pEne->GetPosition(), VGet(0.0f, 250.0f, 0.0f));
			VECTOR DrawHpBarWorld = ConvWorldPosToScreenPos(DrawHpBar3D);

			int BarWidth = 60;
			int BarHeight = 10;

			float HpBar = pEne->GetHp() / pEne->GetMaxHp();

			int HpBarX = DrawHpBarWorld.x - BarWidth / 2;
			int HpBarY = DrawHpBarWorld.y - BarHeight / 2;

			bool isHitSearch = pEne->GetIsHitSearch();
			if (isHitSearch&&pEne->GetHp()>0)
			{
				DrawBox(HpBarX, HpBarY, HpBarX + BarWidth, HpBarY + BarHeight, GetColor(255, 255, 255), FALSE);
				DrawBox(HpBarX, HpBarY, HpBarX + (int)(BarWidth * HpBar), HpBarY + BarHeight, GetColor(0, 255, 0), TRUE);
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	


}
//描画
void DrawHp::Draw()
{
	
}