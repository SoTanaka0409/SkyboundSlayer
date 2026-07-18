#pragma once

// シーン遷移時の暗転・明転エフェクトを管理するシングルトン（シーン破棄を跨いで描画を維持するため）
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

    // 入力: なし / 出力: なし
    // 副作用: アルファ値とステートを初期値にリセットし、前回のフェード状態が残るバグ（画面が暗いまま等）を防ぐ
    void Initialize();

    // 入力: なし / 出力: なし
    // 副作用: 現在のステート（In/Out）に応じてアルファ値を増減させ、演出アニメーションを進行させる
    void Update();

    // 入力: なし / 出力: なし
    // 副作用: 画面最前面に対して現在のアルファ値で黒矩形を描画し、視覚的なトランジションを適用する
    void Draw();

    // 入力: なし / 出力: なし
    // 副作用: 新しいシーンのロード直後に呼ばれ、暗転状態から徐々に画面を可視化していく処理を開始する
    void StartFadeIn();

    // 入力: なし / 出力: なし
    // 副作用: シーン離脱時に呼ばれ、画面を徐々に暗転させていく処理を開始する
    void StartFadeOut();

    // 入力: なし / 出力: 現在のステート / 副作用: なし
    State GetState() const { return state_; }

    // 入力: なし / 出力: 明転演出が完了したか(bool) / 副作用: なし
    // フェードイン完了を検知し、プレイヤーの操作受付やゲームタイマーのカウントダウンを開始するために用いる
    bool IsFadeInFinished() const { return state_ == State::FadeIn && alpha_ <= 0; }

    // 入力: なし / 出力: 暗転演出が完了したか(bool) / 副作用: なし
    // シーンマネージャーが、安全に現在シーンの破棄と次シーンの初期化を行うための同期トリガーとして使用する
    bool IsFadeOutFinished() const { return state_ == State::FadeOut && alpha_ >= 255; }

    // 入力: なし / 出力: フェード実行中か(bool) / 副作用: なし
    // 演出進行中にプレイヤーがUIを操作したり、ポーズメニューを開いたりするのをブロック（入力無視）するために使用する
    bool IsFading() const { return state_ != State::None; }

private:
    // 入力: なし / 出力: なし / 副作用: なし
    // シングルトン設計の制約として、外部からの不用意なインスタンス生成や破棄をコンパイルレベルで禁止する
    Fade();
    ~Fade();

    State state_;    // フェードの多重呼び出しなどによる進行状態の矛盾を防ぐための内部ステート
    int alpha_;      // 描画時の不透明度（0:完全透過 〜 255:完全暗転。オーバーフロー防止のためint型で保持）
    int fade_speed_; // 1フレームあたりのアルファ変動量（環境ごとのFPS依存を防ぐ場合は時間依存への改修が必要）
};