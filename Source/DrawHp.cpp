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

/// @brief DrawHp縺ｮ蛻晄悄蛹厄ｼ医さ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿｼ
DrawHp::DrawHp()
{
}

DrawHp::~DrawHp()
{
}

/// @brief DrawHp縺ｮ迥ｶ諷区峩譁ｰ蜃ｦ逅
void DrawHp::Update()
{
	UpdateHpBars();
}

/// @brief DrawHp縺ｮUpdateHpBars蜃ｦ逅
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
	
	// 蟄伜惠縺励↑縺上↑縺｣縺滓雰縺ｮ驕蟒ｶHP繝繝ｼ繧ｿ繧偵け繝ｪ繝ｼ繝ｳ繧｢繝繝
	for (auto it = delayed_hp_map_.begin(); it != delayed_hp_map_.end(); ) {
		if (std::find(active_enemies.begin(), active_enemies.end(), it->first) == active_enemies.end()) {
			it = delayed_hp_map_.erase(it);
		} else {
			++it;
		}
	}
}

/// @brief DrawHp縺ｮDrawSingleHpBar蜃ｦ逅
void DrawHp::DrawSingleHpBar(Enemy* pEne)
{
	if (pEne->GetHp() <= 0) return;

	float currentHp = pEne->GetHp();
	float maxHp = pEne->GetMaxHp();

	// 驕蟒ｶHP縺ｮ蛻晄悄蛹悶ｻ譖ｴ譁ｰ蜃ｦ逅
	if (delayed_hp_map_.find(pEne) == delayed_hp_map_.end()) {
		delayed_hp_map_[pEne] = currentHp;
	}
	float& delayedHp = delayed_hp_map_[pEne];
	if (delayedHp > currentHp) {
		delayedHp -= maxHp * 0.005f; // 譛螟ｧHP縺ｮ0.5%縺壹▽豈弱ヵ繝ｬ繝ｼ繝�貂帛ｰ代＆縺帙ｋ
		if (delayedHp < currentHp) delayedHp = currentHp;
	} else if (delayedHp < currentHp) {
		delayedHp = currentHp; // 蝗槫ｾｩ譎ゅｯ蜊ｳ蠎ｧ縺ｫ霑ｽ縺縺､縺
	}

	auto drawSlanted = [](int x, int y, int w, int h, int skew, int color, int fill) {
		DrawQuadrangle(x + skew, y, x + w + skew, y, x + w - skew, y + h, x - skew, y + h, color, fill);
	};

	bool is_boss = (pEne->CastTo<EnemyBoss_1>() != nullptr);

	if (is_boss)
	{
		// 逕ｻ髱｢荳企Κ縺ｫ蟾ｨ螟ｧ縺ｪ繝懊せ蟆ら畑HP繧ｲ繝ｼ繧ｸ繧呈緒逕ｻ
		int screenW = 1920; 
		int barW = 800;
		int barH = 24;
		int barX = (screenW - barW) / 2;
		int barY = 60;
		int skew = 20; // 譁懊ａ縺ｮ隗貞ｺｦ
		
		float HpRatio = currentHp / maxHp;
		float DelayedRatio = delayedHp / maxHp;

		// 閭梧勹
		// 豪華なボス用フレーム枠（黒・金・黒の多重枠）
		drawSlanted(barX - 8, barY - 8, barW + 16, barH + 16, skew + 2, GetColor(20, 20, 20), TRUE); // 外縁の黒
		drawSlanted(barX - 6, barY - 6, barW + 12, barH + 12, skew + 1, GetColor(212, 175, 55), TRUE); // メインの金枠
		drawSlanted(barX - 2, barY - 2, barW + 4, barH + 4, skew, GetColor(0, 0, 0), TRUE); // 内側の黒枠
		drawSlanted(barX, barY, barW, barH, skew, GetColor(50, 0, 0), TRUE);
		
		// 驕蟒ｶ繧ｲ繝ｼ繧ｸ (鮟濶ｲ) - 繝繝｡繝ｼ繧ｸ繧貞女縺代◆髫帙ｮ蜑翫ｊ貍泌ｺ
		drawSlanted(barX, barY, (int)(barW * DelayedRatio), barH, skew, GetColor(255, 200, 50), TRUE);
		
		// 迴ｾ蝨ｨ縺ｮ繧ｲ繝ｼ繧ｸ (襍､)
		drawSlanted(barX, barY, (int)(barW * HpRatio), barH, skew, GetColor(220, 20, 20), TRUE);
		
		// HP逶ｮ逶帙ｊ (鬟ｾ繧翫→縺励※10蛻蜑ｲ縺ｮ繧ｻ繧ｰ繝｡繝ｳ繝医ｒ謠冗判)
		for (int i = 1; i < 10; ++i) {
			int tickX = barX + (barW * i / 10);
			DrawLine(tickX + skew, barY, tickX - skew, barY + barH, GetColor(20, 0, 0), 2);
		}
		
		// 荳企Κ繝上う繝ｩ繧､繝医〒遶倶ｽ捺─繧貞ｺ縺
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
		drawSlanted(barX, barY, (int)(barW * DelayedRatio), barH / 2, skew, GetColor(255, 255, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 螟匁棧ｼ井ｺ碁\璇縺ｧ雎ｪ闖ｯ縺ｫｼ
		drawSlanted(barX - 6, barY - 6, barW + 12, barH + 12, skew + 1, GetColor(255, 235, 120), FALSE); 
		drawSlanted(barX - 2, barY - 2, barW + 4, barH + 4, skew, GetColor(150, 110, 20), FALSE);
		
		// BOSSテキスト（影をつけて見やすく）
		DrawFormatString(barX + skew + 2, barY - 33, GetColor(0, 0, 0), "BOSS");
		DrawFormatString(barX + skew, barY - 35, GetColor(255, 100, 100), "BOSS");
	}
	else
	{
		VECTOR DrawHpBar3D = VAdd(pEne->GetPosition(), VGet(0.0f, kHpBarOffsetY, 0.0f));
		VECTOR DrawHpBarWorld = ConvWorldPosToScreenPos(DrawHpBar3D);

		// 繧ｫ繝｡繝ｩ縺ｮ蠕後ｍ縺ｫ縺繧句�ｴ蜷医ｯ謠冗判縺励↑縺
		if (DrawHpBarWorld.z < 0.0f || DrawHpBarWorld.z > 1.0f) return;

		int HpBarX = static_cast<int>(DrawHpBarWorld.x) - kHpBarWidth / 2;
		int HpBarY = static_cast<int>(DrawHpBarWorld.y) - kHpBarHeight / 2;
		int skew = 6; // 騾壼ｸｸ謨ｵ繧ょｰ代＠譁懊ａ縺ｫ縺励※繧ｹ繧ｿ繧､繝ｪ繝繧ｷ繝･縺ｫ

		bool is_hit_search_flag_ = pEne->IsHitSearchFlag();
		if (is_hit_search_flag_)
		{
			float HpRatio = currentHp / maxHp;
			float DelayedRatio = delayedHp / maxHp;

			// 閭梧勹
			// 通常の敵用シルバーフレーム枠
			drawSlanted(HpBarX - 4, HpBarY - 4, kHpBarWidth + 8, kHpBarHeight + 8, skew + 1, GetColor(10, 10, 10), TRUE);
			drawSlanted(HpBarX - 3, HpBarY - 3, kHpBarWidth + 6, kHpBarHeight + 6, skew, GetColor(160, 160, 160), TRUE);
			drawSlanted(HpBarX - 1, HpBarY - 1, kHpBarWidth + 2, kHpBarHeight + 2, skew, GetColor(30, 30, 30), TRUE);
			drawSlanted(HpBarX, HpBarY, kHpBarWidth, kHpBarHeight, skew, GetColor(60, 60, 60), TRUE);
			
			// 驕蟒ｶ繧ｲ繝ｼ繧ｸ (逋ｽ)
			drawSlanted(HpBarX, HpBarY, (int)(kHpBarWidth * DelayedRatio), kHpBarHeight, skew, GetColor(255, 255, 255), TRUE);

			// 繧ｲ繝ｼ繧ｸ縺ｮ濶ｲ(HP谿矩㍼縺ｧ螟牙喧)
			int hpColor = GetColor(50, 220, 50);
			if (HpRatio <= 0.2f) hpColor = GetColor(220, 50, 50);
			else if (HpRatio <= 0.5f) hpColor = GetColor(220, 220, 50);

			drawSlanted(HpBarX, HpBarY, (int)(kHpBarWidth * HpRatio), kHpBarHeight, skew, hpColor, TRUE);
			
			// 荳企Κ繝上う繝ｩ繧､繝
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
			drawSlanted(HpBarX, HpBarY, (int)(kHpBarWidth * DelayedRatio), kHpBarHeight / 2, skew, GetColor(255, 255, 255), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// シルバー枠のハイライト（少し明るくして立体感を出す）
			drawSlanted(HpBarX - 3, HpBarY - 3, kHpBarWidth + 6, kHpBarHeight + 6, skew, GetColor(220, 220, 220), FALSE);
		}
	}
}

/// @brief DrawHp縺ｮ謠冗判蜃ｦ逅
void DrawHp::Draw()
{
}
