#include"DrawHp.h"
#include"Enemy3D.h"
#include"EnemyBoss_1.h"
#include"Enemy.h"
#include"Master.h"
#include"Object3D.h"
#include"ObjectManager.h"
#include"Model.h"
#include"GameScene.h"
#include"SceneManager.h"
#include"Scene.h"
#include"Player3D.h"
#include <algorithm>

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
	
	std::vector<Enemy*> active_enemies;
	for (int i = 0; i < pObjList.size(); i++)
	{
		auto pObj = pObjList[i];
		Enemy* pEne = pObj->CastTo<Enemy>();
		if (pEne != nullptr)
		{
			active_enemies.push_back(pEne);
			DrawSingleHpBar(pEne);
		}
	}
	
	// 存在しなくなった敵の遅延HPデータをクリーンアップ
	for (auto it = delayed_hp_map_.begin(); it != delayed_hp_map_.end(); ) {
		if (std::find(active_enemies.begin(), active_enemies.end(), it->first) == active_enemies.end()) {
			it = delayed_hp_map_.erase(it);
		} else {
			++it;
		}
	}
}

/// @brief DrawHpのDrawSingleHpBar処理
void DrawHp::DrawSingleHpBar(Enemy* pEne)
{
	if (pEne->GetHp() <= 0) return;

	float currentHp = pEne->GetHp();
	float maxHp = pEne->GetMaxHp();

	// 遅延HPの初期化・更新処理
	if (delayed_hp_map_.find(pEne) == delayed_hp_map_.end()) {
		delayed_hp_map_[pEne] = currentHp;
	}
	float& delayedHp = delayed_hp_map_[pEne];
	if (delayedHp > currentHp) {
		delayedHp -= maxHp * 0.005f; // 最大HPの0.5%ずつ毎フレーム減少させる (遅延ダメージ表現)
		if (delayedHp < currentHp) delayedHp = currentHp;
	} else if (delayedHp < currentHp) {
		delayedHp = currentHp; // 回復時は即座に追いつく
	}

	auto drawSlanted = [](int x, int y, int w, int h, int skew, int color, int fill) {
		DrawQuadrangle(x + skew, y, x + w + skew, y, x + w - skew, y + h, x - skew, y + h, color, fill);
	};

	bool is_boss = (pEne->CastTo<EnemyBoss_1>() != nullptr);

	if (is_boss)
	{
		// 画面上部に巨大なボス専用HPゲージを描画
		int screenW = 1920; 
		int barW = 800;
		int barH = 24;
		int barX = (screenW - barW) / 2;
		int barY = 60;
		int skew = 20; // 斜めの角度
		
		float HpRatio = currentHp / maxHp;
		float DelayedRatio = delayedHp / maxHp;

		// 背景
		// ボス用特殊フレーム（豪華な金枠など）
		drawSlanted(barX - 8, barY - 8, barW + 16, barH + 16, skew + 2, GetColor(20, 20, 20), TRUE); // 外枠の黒
		drawSlanted(barX - 6, barY - 6, barW + 12, barH + 12, skew + 1, GetColor(212, 175, 55), TRUE); // 金色の枠
		drawSlanted(barX - 2, barY - 2, barW + 4, barH + 4, skew, GetColor(0, 0, 0), TRUE); // 内枠の黒
		drawSlanted(barX, barY, barW, barH, skew, GetColor(50, 0, 0), TRUE);
		
		// 遅延ゲージ (黄色) - ダメージを受けた際の削り演出
		drawSlanted(barX, barY, (int)(barW * DelayedRatio), barH, skew, GetColor(255, 200, 50), TRUE);
		
		// 現在のゲージ (赤)
		drawSlanted(barX, barY, (int)(barW * HpRatio), barH, skew, GetColor(220, 20, 20), TRUE);
		
		// HP目盛り (飾りとして10分割のセグメントを描画)
		for (int i = 1; i < 10; ++i) {
			int tickX = barX + (barW * i / 10);
			DrawLine(tickX + skew, barY, tickX - skew, barY + barH, GetColor(20, 0, 0), 2);
		}
		
		// 上部ハイライトで立体感を出す
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
		drawSlanted(barX, barY, (int)(barW * DelayedRatio), barH / 2, skew, GetColor(255, 255, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 外枠を二重線で豪華に見せる
		drawSlanted(barX - 6, barY - 6, barW + 12, barH + 12, skew + 1, GetColor(255, 235, 120), FALSE); 
		drawSlanted(barX - 2, barY - 2, barW + 4, barH + 4, skew, GetColor(150, 110, 20), FALSE);
		
		// BOSSテキスト（影をつけて見やすくする）
		DrawFormatString(barX + skew + 2, barY - 33, GetColor(0, 0, 0), "BOSS");
		DrawFormatString(barX + skew, barY - 35, GetColor(255, 100, 100), "BOSS");
	}
	else
	{
		VECTOR DrawHpBar3D = VAdd(pEne->GetPosition(), VGet(0.0f, kHpBarOffsetY, 0.0f));
		VECTOR DrawHpBarWorld = ConvWorldPosToScreenPos(DrawHpBar3D);

		// カメラの後ろにいる場合は描画しない
		if (DrawHpBarWorld.z < 0.0f || DrawHpBarWorld.z > 1.0f) return;

		int HpBarX = static_cast<int>(DrawHpBarWorld.x) - kHpBarWidth / 2;
		int HpBarY = static_cast<int>(DrawHpBarWorld.y) - kHpBarHeight / 2;
		int skew = 6; // 通常敵も少し斜めにしてスタイリッシュに

		bool is_hit_search_flag_ = pEne->IsHitSearchFlag();
		if (is_hit_search_flag_)
		{
			float HpRatio = currentHp / maxHp;
			float DelayedRatio = delayedHp / maxHp;

			// 背景
			// 通常敵用シルバーフレーム
			drawSlanted(HpBarX - 4, HpBarY - 4, kHpBarWidth + 8, kHpBarHeight + 8, skew + 1, GetColor(10, 10, 10), TRUE);
			drawSlanted(HpBarX - 3, HpBarY - 3, kHpBarWidth + 6, kHpBarHeight + 6, skew, GetColor(160, 160, 160), TRUE);
			drawSlanted(HpBarX - 1, HpBarY - 1, kHpBarWidth + 2, kHpBarHeight + 2, skew, GetColor(30, 30, 30), TRUE);
			drawSlanted(HpBarX, HpBarY, kHpBarWidth, kHpBarHeight, skew, GetColor(60, 60, 60), TRUE);
			
			// 遅延ゲージ (白)
			drawSlanted(HpBarX, HpBarY, (int)(kHpBarWidth * DelayedRatio), kHpBarHeight, skew, GetColor(255, 255, 255), TRUE);

			// ゲージの色(HP残量で変化)
			int hpColor = GetColor(50, 220, 50);
			if (HpRatio <= 0.2f) hpColor = GetColor(220, 50, 50);
			else if (HpRatio <= 0.5f) hpColor = GetColor(220, 220, 50);

			drawSlanted(HpBarX, HpBarY, (int)(kHpBarWidth * HpRatio), kHpBarHeight, skew, hpColor, TRUE);
			
			// 上部ハイライト
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
			drawSlanted(HpBarX, HpBarY, (int)(kHpBarWidth * DelayedRatio), kHpBarHeight / 2, skew, GetColor(255, 255, 255), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// シルバー枠のハイライト（明るくして立体感を出す）
			drawSlanted(HpBarX - 3, HpBarY - 3, kHpBarWidth + 6, kHpBarHeight + 6, skew, GetColor(220, 220, 220), FALSE);
		}
	}
}

/// @brief DrawHpの描画処理
void DrawHp::Draw()
{
}
