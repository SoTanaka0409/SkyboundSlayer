#pragma once
#include <DxLib.h>
#include <vector>

//====================================================
// EffectU クラス
// Uキー用：魔法陣演出
//
// ・押している間（Holding）
//   - 魔法陣を表示してゆっくり回転（Y軸）
// ・離した瞬間（Release & Shatter）
//   - 原型を少し残す（PreShatter）
//   - 砕けて破片ごとに上昇しながらフェードアウト（Shatter）
//====================================================
class EffectU
{
public:
    EffectU() = default;
    ~EffectU() = default;

    //====================================================
    // テクスチャや初期データの読み込み
    //====================================================
    bool Load();

    //====================================================
    // Uキー押した瞬間：魔法陣を出す
    // Holding状態に遷移
    //====================================================
    void StartHold(const VECTOR& playerPos);

    //====================================================
    // Uキー離した瞬間
    // - PreShatter状態に移行
    // - 少し残してからShatter状態に移行
    //====================================================
    void ReleaseAndShatter();

    //====================================================
    // 毎フレーム呼ぶ
    // - 中心位置追従
    // - 回転や破片の更新
    //====================================================
    void UpdateFollow(const VECTOR& playerPos);

    //====================================================
    // 描画
    // - Holding: 魔法陣全体
    // - Shatter: 破片ごとに描画
    //====================================================
    void Draw() const;

private:
    //====================================================
    // 内部状態
    //====================================================
    enum class State
    {
        Idle,        // 何もしていない
        Holding,     // 魔法陣表示中
        PreShatter,  // 原型で少し残す
        Shatter      // 破片に分かれてフェード＆上昇
    };

    //====================================================
    // 破片情報
    //====================================================
    struct Piece
    {
        bool   alive = false;   // 生存中かどうか

        VECTOR pos = VGet(0, 0, 0); // 現在位置
        VECTOR vel = VGet(0, 0, 0); // 速度

        float  alpha = 0.0f;    // 透明度（255→0）
        float  rotY = 0.0f;     // 回転角度（Y軸）
        float  rotSpd = 0.0f;   // 回転速度

        float  halfSize = 1.0f; // サイズ半分（スケーリング用）

        // UV（テクスチャ座標 0..1）
        float u0 = 0.0f, v0 = 0.0f;
        float u1 = 0.0f, v1 = 0.0f;

        // Shatter用
        int   startDelay = 0;   // 上昇開始までの遅延（フレーム）
        int   age = 0;          // 生存フレーム
        float upAccel = 0.0f;   // 上昇加速
    };


    //====================================================
    // 内部処理
    //====================================================
    void BuildPieces(const VECTOR& center, float baseRotY); // 破片生成
    void DrawHoldingBoard() const;  // Holding状態描画
    void DrawPiece(const Piece& p) const; // 破片描画


    int   mTex = -1;       // 魔法陣テクスチャ
    State mState = State::Idle;

    VECTOR mCenter = VGet(0, 0, 0); // 魔法陣中心

    // Holding中の回転
    float mHoldRotY = 0.0f;       // 現在角度
    float mHoldRotSpd = 0.02f;    // 回転速度（0.01～0.05が目安）

    // PreShatter：原型で残す時間
    int   mPreShatterTimer = 0;           // 経過フレーム
    int   mPreShatterFrames = 10;         // 残すフレーム数
    bool  mPreShatterKeepRotating = false; // 残っている間も回転するか

    std::vector<Piece> mPieces;           // 破片配列

    //====================================================
    // 見た目調整パラメータ
    //====================================================
    int   mDiv = 12;              // 魔法陣分割数
    float mBoardSize = 220.0f;    // 魔法陣サイズ
    float mYOffset = 2.0f;        // 地面から少し浮かせる

    // Shatter：フェード
    float mFadeSpd = 3.2f;

    // Shatter：上昇
    float mUpAccelBase = 0.0040f;
    float mUpStartMin = 0.06f;    // 遅延最小
    float mUpStartMax = 0.13f;    // 遅延最大

    // 減衰
    float mDampXZ = 0.985f;
    float mDampY = 0.999f;

    // 横方向の散り
    float mOutPower = 0.018f;
    float mRandJitter = 0.006f;

    // バラバラ開始遅延（最大80Fで運用）
    int   mStartDelayMin = 0;
    int   mStartDelayMax = 80;

    // PreShatter中の微振動
    float mIdleJitter = 0.25f;

    // 灰っぽさ
    int mBright = 200;
};
