#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include "Model.h"

class Player3D;
class SphereCollider;

/// @brief ステータス強化を行うショップNPCの制御クラス。徒歩での入場・退場演出と、購入メニューのUIを管理する
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
/// @details NPCモデルのロードと、プレイヤー進入検知用のコライダー(m_ShopIn)を初期化する
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
    bool IsArrived() const { return m_ShopState == ShopState::ARRIVED; }

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

/// @details 選択中の強化項目(m_Select)に対し、所持金とコストを照合して購入処理を実行する
    void SelectClass();
    void BuyClass();
/// @param upgradeCount(強化回数)
/// @return 必要金額(int)
    int GetCost(int upgradeCount);

    void UpdateWalkIn();
    void UpdateWalkOut();
    void UpdateShopColliderVisibility();
    void SyncModelTransform();

    Model* m_Model;                      // ショップNPCの3Dモデル
    SphereCollider* m_ShopIn;           // プレイヤーが店内に侵入したことを検知するトリガー
    SphereCollider* m_SafeZoon;         // 店内での安全領域（敵の攻撃が届かない範囲）

    VECTOR m_TargetPosition;            // 徒歩移動演出における目標の店舗前座標
    VECTOR mvStartPosition;             // 移動演出の開始地点（Lerp計算用）
    ShopState m_ShopState;              // 移動演出やメニュー展開を制御する内部ステート

    int m_Select;                        // 現在選択中の強化項目インデックス
    int m_SelectMax;                    // 項目数上限（カーソルのループ制限用）
    int m_SelectMin;                    // 項目数下限
    bool m_OldMouseDown;               // 入力の多重検知を防ぐための前フレームのマウス状態

    int m_UpgradeMaxHpCount;          // 各項目の強化回数（コスト算出用）
    int m_UpgradeAttackCount;
    int m_UpgradeSpeedCount;
    int m_UpgradeEvasionSpeedCount;

    int m_IconMaxHpHandle;            // UI表示用の各スキルアイコン画像ハンドル
    int m_IconAttackHandle;
    int m_IconSpeedHandle;
    int m_IconEvasionDistHandle;
    int m_IconEvasionInvHandle;
};