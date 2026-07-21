#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"

class Player3D;
class SphereCollider;

// ステータス強化を行うショップNPCの制御クラス。徒歩での入場・退場演出と、購入メニューのUIを管理する
class StatShop : public Object3D
{
public:
    enum class ShopState
    {
        WAIT_PHASE,
        WALKING_IN,
        ARRIVED,
        WALKING_OUT
    };

/// @param filename, vec(出現座標)
/// @details NPCモデルのロードと、プレイヤー進入検知用のコライダー(shop_in_)を初期化する
    StatShop(std::string filename, VECTOR vec);

/// @details 動的確保したモデルやコライダーを破棄し、メモリリークを防ぐ
    ~StatShop();

/// @details 到着後であればショップメニューを描画し、NPCモデルとUIをバッファへ登録する
    void Draw() override;

/// @details 徒歩移動演出の進行や、プレイヤーとの距離によるメニューの開閉（Close
    void Update() override;

/// @details 待機状態から店舗前への移動ステートを開始する
    void StartWalkingIn();
/// @details 店舗外への移動ステートを開始し、終了後にインスタンスを破棄予約する
    void StartWalkingOut();
/// @details 目標座標へ向けてNPCモデルの座標を線形補間（Lerp）する
    void movePosition();
/// @return 到着完了か(bool)
    bool IsArrived() const { return shop_state_ == ShopState::ARRIVED; }

    virtual void OnEnter(Collider* collider, Collider* check) override;
    virtual void OnTrigger(Collider* collider, Collider* check) override;
    virtual void OnExit(Collider* collider, Collider* check) override;

private:
/// @return 更新可能か(bool)
    bool CanUpdateShop() const;
/// @details ゲーム終了やフェーズ遷移時に、強制的にショップメニューを閉じる
    void CloseShopIfPhaseEnding();
/// @details プレイヤーのカーソル移動や決定入力を監視し、購入処理を呼び出す
    void UpdateShopMenu();
/// @details キャンセルボタン検知時に購入画面を閉じ、ゲームプレイへ復帰する
    void HandleShopCloseInput();

    void DrawShopMenu(Player3D* player);
    void DrawShopHeader(Player3D* player);
    void DrawShopOptions();
    void DrawShopFooter();
    void DrawShopNpc(Player3D* player);

/// @details 選択中の強化項目(select_)に対し、所持金とコストを照合して購入処理を実行する
    void SelectClass();
    void BuyClass();
/// @param upgradeCount(強化回数)
/// @return 必要金額(int)
    int GetCost(int upgradeCount);

    void UpdateWalkIn();
    void UpdateWalkOut();
    void UpdateShopColliderVisibility();
    void SyncModelTransform();

    Model* model_;                      // ショップNPCの3Dモデル
    SphereCollider* shop_in_;           // プレイヤーが店内に侵入したことを検知するトリガー
    SphereCollider* safe_zoon_;         // 店内での安全領域（敵の攻撃が届かない範囲）

    VECTOR target_position_;            // 徒歩移動演出における目標の店舗前座標
    VECTOR mvStartPosition;             // 移動演出の開始地点（Lerp計算用）
    ShopState shop_state_;              // 移動演出やメニュー展開を制御する内部ステート

    int select_;                        // 現在選択中の強化項目インデックス
    int select_max_;                    // 項目数上限（カーソルのループ制限用）
    int select_min_;                    // 項目数下限
    bool old_mouse_down_;               // 入力の多重検知を防ぐための前フレームのマウス状態

    int upgrade_max_hp_count_;          // 各項目の強化回数（コスト算出用）
    int upgrade_attack_count_;
    int upgrade_speed_count_;
    int upgrade_evasion_speed_count_;

    int icon_max_hp_handle_;            // UI表示用の各スキルアイコン画像ハンドル
    int icon_attack_handle_;
    int icon_speed_handle_;
    int icon_evasion_dist_handle_;
    int icon_evasion_inv_handle_;
};