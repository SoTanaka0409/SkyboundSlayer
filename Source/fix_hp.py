import codecs

with codecs.open('Source/DrawHp.cpp', 'r', 'cp932', errors='ignore') as f:
    cpp = f.read()

boss_bg_old = '''		// wi
		drawSlanted(barX - 4, barY - 4, barW + 8, barH + 8, skew, GetColor(0, 0, 0), TRUE);
		drawSlanted(barX, barY, barW, barH, skew, GetColor(50, 0, 0), TRUE);'''

boss_bg_new = '''		// 豪華なボス用フレーム枠（黒・金・黒の多重枠）
		drawSlanted(barX - 8, barY - 8, barW + 16, barH + 16, skew + 2, GetColor(20, 20, 20), TRUE); // 外縁の黒
		drawSlanted(barX - 6, barY - 6, barW + 12, barH + 12, skew + 1, GetColor(212, 175, 55), TRUE); // メインの金枠
		drawSlanted(barX - 2, barY - 2, barW + 4, barH + 4, skew, GetColor(0, 0, 0), TRUE); // 内側の黒枠
		drawSlanted(barX, barY, barW, barH, skew, GetColor(50, 0, 0), TRUE);'''

boss_fg_old = '''		// Ogidgō؂Ɂj
		drawSlanted(barX - 4, barY - 4, barW + 8, barH + 8, skew, GetColor(200, 200, 200), FALSE);
		drawSlanted(barX - 2, barY - 2, barW + 4, barH + 4, skew, GetColor(100, 100, 100), FALSE);
		
		// {Xip̑j
		DrawFormatString(barX + skew, barY - 35, GetColor(255, 100, 100), "BOSS");'''

boss_fg_new = '''		// ゴールド枠のハイライト（枠をさらに立体的に見せる）
		drawSlanted(barX - 6, barY - 6, barW + 12, barH + 12, skew + 1, GetColor(255, 235, 120), FALSE); 
		drawSlanted(barX - 2, barY - 2, barW + 4, barH + 4, skew, GetColor(150, 110, 20), FALSE);
		
		// BOSSテキスト（影をつけて見やすく）
		DrawFormatString(barX + skew + 2, barY - 33, GetColor(0, 0, 0), "BOSS");
		DrawFormatString(barX + skew, barY - 35, GetColor(255, 100, 100), "BOSS");'''

ene_bg_old = '''			// wi
			drawSlanted(HpBarX - 1, HpBarY - 1, kHpBarWidth + 2, kHpBarHeight + 2, skew, GetColor(20, 20, 20), TRUE);
			drawSlanted(HpBarX, HpBarY, kHpBarWidth, kHpBarHeight, skew, GetColor(60, 60, 60), TRUE);'''

ene_bg_new = '''			// 通常の敵用シルバーフレーム
			drawSlanted(HpBarX - 4, HpBarY - 4, kHpBarWidth + 8, kHpBarHeight + 8, skew + 1, GetColor(10, 10, 10), TRUE);
			drawSlanted(HpBarX - 3, HpBarY - 3, kHpBarWidth + 6, kHpBarHeight + 6, skew, GetColor(160, 160, 160), TRUE);
			drawSlanted(HpBarX - 1, HpBarY - 1, kHpBarWidth + 2, kHpBarHeight + 2, skew, GetColor(30, 30, 30), TRUE);
			drawSlanted(HpBarX, HpBarY, kHpBarWidth, kHpBarHeight, skew, GetColor(60, 60, 60), TRUE);'''

ene_fg_old = '''			// Og
			drawSlanted(HpBarX - 1, HpBarY - 1, kHpBarWidth + 2, kHpBarHeight + 2, skew, GetColor(100, 100, 100), FALSE);'''

ene_fg_new = '''			// シルバー枠のハイライト
			drawSlanted(HpBarX - 3, HpBarY - 3, kHpBarWidth + 6, kHpBarHeight + 6, skew, GetColor(220, 220, 220), FALSE);'''


cpp = cpp.replace(boss_bg_old, boss_bg_new)
cpp = cpp.replace(boss_fg_old, boss_fg_new)
cpp = cpp.replace(ene_bg_old, ene_bg_new)
cpp = cpp.replace(ene_fg_old, ene_fg_new)

with codecs.open('Source/DrawHp.cpp', 'w', 'cp932') as f:
    f.write(cpp)
