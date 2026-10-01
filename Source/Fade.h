#pragma once

/// @brief シーン遷移時の暗転・明転エフェクトを管理するシングルトン（シーン破棄を跨いで描画を維持するため）
class Fade
{
public:
    enum class State
    {
        None,
        FadeIn,
        FadeOut
    };

    static Fade* GetInstance()
    {
        static Fade instance;
        return &instance;
    }

/// @details アルファ値とステートを初期値にリセットし、前回のフェード状態が残るバグ（画面が暗いまま等）を防ぐ
    void Initialize();

/// @details 現在のステート（In
    void Update();

/// @details 画面最前面に対して現在のアルファ値で黒矩形を描画し、視覚的なトランジションを適用する
    void Draw();

/// @details 新しいシーンのロード直後に呼ばれ、暗転状態から徐々に画面を可視化していく処理を開始する
    void StartFadeIn();

/// @details シーン離脱時に呼ばれ、画面を徐々に暗転させていく処理を開始する
    void StartFadeOut();

/// @return 現在のステート
    State GetState() const { return m_State; }

/// @return 明転演出が完了したか(bool)
    /// @brief フェードイン完了を検知し、プレイヤーの操作受付やゲームタイマーのカウントダウンを開始するために用いる
    bool IsFadeInFinished() const { return m_State == State::FadeIn && m_Alpha <= 0; }

/// @return 暗転演出が完了したか(bool)
    /// @brief シーンマネージャーが、安全に現在シーンの破棄と次シーンの初期化を行うための同期トリガーとして使用する
    bool IsFadeOutFinished() const { return m_State == State::FadeOut && m_Alpha >= 255; }

/// @return フェード実行中か(bool)
    /// @brief 演出進行中にプレイヤーがUIを操作したり、ポーズメニューを開いたりするのをブロック（入力無視）するために使用する
    bool IsFading() const { return m_State != State::None; }

private:
    /// @brief シングルトン設計の制約として、外部からの不用意なインスタンス生成や破棄をコンパイルレベルで禁止する
    Fade();
    ~Fade();

    State m_State;    // フェードの多重呼び出しなどによる進行状態の矛盾を防ぐための内部ステート
    int m_Alpha;      // 描画時の不透明度（0:完全透過 - 255:完全暗転。オーバーフロー防止のためint型で保持）
    int m_FadeSpeed; // 1フレームあたりのアルファ変動量（環境ごとのFPS依存を防ぐ場合は時間依存への改修が必要）
};
