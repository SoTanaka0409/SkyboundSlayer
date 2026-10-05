#include "DamageUI.h"
#include <string>
#include <cmath>

DamageUIManager::DamageUIManager() : is_image_loaded_(false) {
    for (int i = 0; i < 10; ++i) {
        font_graph_handles_[i] = -1;
    }
}

DamageUIManager::~DamageUIManager() {
    Unload();
}

void DamageUIManager::Load() {
    if (is_image_loaded_) return;
    
    bool success = true;
    for (int i = 0; i < 10; ++i) {
        std::string path = "Resource/image/UI/damage_numbers/damage_numbers_" + std::to_string(i) + ".png";
        font_graph_handles_[i] = LoadGraph(path.c_str());
        if (font_graph_handles_[i] == -1) {
            success = false;
        }
    }
    if (success) {
        is_image_loaded_ = true;
    }
}

void DamageUIManager::Unload() {
    for (int i = 0; i < 10; ++i) {
        if (font_graph_handles_[i] != -1) {
            DeleteGraph(font_graph_handles_[i]);
            font_graph_handles_[i] = -1;
        }
    }
    is_image_loaded_ = false;
}

void DamageUIManager::AddDamage(int value, VECTOR pos, bool is_critical) {
    DamagePopUp popup;
    popup.value = value;
    // 発生位置を敵の中心から少し散らす
    popup.pos_3d = pos;
    popup.pos_3d.y += 10.0f; 
    popup.life_timer = 0;
    popup.max_life = 60; // 1秒間表示
    popup.is_critical = is_critical;
    
    // ランダムな散らばり方向
    popup.random_x_dir = (GetRand(100) - 50) * 0.02f;
    popup.random_z_dir = (GetRand(100) - 50) * 0.02f;

    popups_.push_back(popup);
}

void DamageUIManager::Update() {
    for (auto it = popups_.begin(); it != popups_.end(); ) {
        it->life_timer++;
        
        // 上にフワッと上がる処理
        it->pos_3d.y += 0.3f;
        it->pos_3d.x += it->random_x_dir;
        it->pos_3d.z += it->random_z_dir;

        if (it->life_timer >= it->max_life) {
            it = popups_.erase(it);
        } else {
            ++it;
        }
    }
}

void DamageUIManager::Draw() {
    for (const auto& popup : popups_) {
        // 3D座標を2Dスクリーン座標に変換
        VECTOR screen_pos = ConvWorldPosToScreenPos(popup.pos_3d);
        
        // カメラの後ろにある場合は描画しない
        if (screen_pos.z < 0.0f || screen_pos.z > 1.0f) continue;

        // フェードアウトの計算
        int alpha = 255;
        if (popup.life_timer > popup.max_life - 20) {
            alpha = 255 * (popup.max_life - popup.life_timer) / 20;
        }

        // スケールの計算（出た瞬間に大きく、その後標準サイズになる）
        float scale = 1.0f;
        if (popup.life_timer < 5) {
            scale = 1.0f + (5 - popup.life_timer) * 0.2f;
        }
        if (popup.is_critical) {
            scale *= 1.5f; // クリティカルは1.5倍大きい
        }

        std::string val_str = std::to_string(popup.value);
        int total_width = 0;
        int digit_w = 60; // 画像1文字あたりの表示幅（大きく調整）
        int digit_h = 75;

        total_width = (int)(val_str.length() * digit_w * scale);
        int draw_x = (int)screen_pos.x - total_width / 2;
        int draw_y = (int)screen_pos.y;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        if (is_image_loaded_) {
            // AI生成画像の数字を描画
            for (char c : val_str) {
                int num = c - '0';
                if (num >= 0 && num <= 9) {
                    DrawExtendGraph(draw_x, draw_y, draw_x + (int)(digit_w * scale), draw_y + (int)(digit_h * scale), font_graph_handles_[num], TRUE);
                }
                draw_x += (int)(digit_w * scale);
            }
        } else {
            // 画像がない場合のフォールバック（標準フォントで少し豪華に縁取り描画）
            int color = popup.is_critical ? GetColor(255, 50, 50) : GetColor(255, 255, 255);
            int edge_color = GetColor(0, 0, 0);
            
            // 少し大きめのフォントサイズを擬似的に表現
            DrawFormatString(draw_x - 2, draw_y - 2, edge_color, "%d", popup.value);
            DrawFormatString(draw_x + 2, draw_y - 2, edge_color, "%d", popup.value);
            DrawFormatString(draw_x - 2, draw_y + 2, edge_color, "%d", popup.value);
            DrawFormatString(draw_x + 2, draw_y + 2, edge_color, "%d", popup.value);
            DrawFormatString(draw_x, draw_y, color, "%d", popup.value);
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}
