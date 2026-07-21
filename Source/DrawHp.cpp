#include"DrawHp.h"
#include"Enemy3D.h"
#include"Enemy.h"
#include"Master.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"Model.h"
#include"Scene3D.h"
#include"SceneManager.h"
#include"Scene.h"
#include"Player3D.h"

/// @brief DrawHpの初期化（コンストラクタ）
DrawHp::DrawHp()
{
}

DrawHp::~DrawHp()
{
}

/// @brief DrawHpの状態更新処理
void DrawHp::Update()
{
	UpdateHpBars();
}

/// @brief DrawHpのUpdateHpBars処理
void DrawHp::UpdateHpBars()
{
	const auto& pObjList = Master::scene_manager_->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
	for (int i = 0; i < pObjList.size(); i++)
	{
		auto pObj = pObjList[i];
		Enemy* pEne = pObj->CastTo<Enemy>();
		if (pEne != nullptr)
		{
			DrawSingleHpBar(pEne);
		}
	}
}

/// @brief DrawHpのDrawSingleHpBar処理
void DrawHp::DrawSingleHpBar(Enemy* pEne)
{
	VECTOR DrawHpBar3D = VAdd(pEne->GetPosition(), VGet(0.0f, kHpBarOffsetY, 0.0f));
	VECTOR DrawHpBarWorld = ConvWorldPosToScreenPos(DrawHpBar3D);

	float HpBar = pEne->GetHp() / pEne->GetMaxHp();

	int HpBarX = static_cast<int>(DrawHpBarWorld.x) - kHpBarWidth / 2;
	int HpBarY = static_cast<int>(DrawHpBarWorld.y) - kHpBarHeight / 2;

	bool is_hit_search_flag_ = pEne->IsHitSearchFlag();
	if (is_hit_search_flag_ && pEne->GetHp() > 0)
	{
		DrawBox(HpBarX, HpBarY, HpBarX + kHpBarWidth, HpBarY + kHpBarHeight, GetColor(255, 255, 255), FALSE);
		DrawBox(HpBarX, HpBarY, HpBarX + (int)(kHpBarWidth * HpBar), HpBarY + kHpBarHeight, GetColor(0, 255, 0), TRUE);
	}
}

/// @brief DrawHpの描画処理
void DrawHp::Draw()
{
}
