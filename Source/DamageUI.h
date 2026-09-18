#pragma once
#include <vector>
#include "DxLib.h"

// 単一のダメージポップアップを表す構造体
struct DamagePopUp {
    int value;           // ダメージ量
    VECTOR pos_3d;       // 3D空間上の発生位置
    int life_timer;      // 発生からの経過フレーム
    int max_life;        // 表示する最大フレーム数
    bool is_critical;    // クリティカルかどうか（色や大きさに影響）
    float random_x_dir;  // X方向への飛び散り具合
    float random_z_dir;  // Z方向への飛び散り具合
};

// ダメージUIを管理するクラス（シングルトン推奨）
class DamageUIManager {
private:
    std::vector<DamagePopUp> popups_;
    int font_graph_handles_[10]; // 0~9の数字画像ハンドル
    bool is_image_loaded_;

    DamageUIManager();
    ~DamageUIManager();

public:
    static DamageUIManager* GetInstance() {
        static DamageUIManager instance;
        return &instance;
    }

    // 数字画像の読み込み（AI生成画像を切り抜いたものを想定）
    void Load();

    // 画像の解放
    void Unload();

    // ダメージUIの追加
    void AddDamage(int value, VECTOR pos, bool is_critical);

    // 全ダメージUIの更新
    void Update();

    // 全ダメージUIの描画
    void Draw();
};
