#pragma once
#include <string>
#include <vector>
#include "ModelUtility.h"

class ModelAnimation
{
public:
    // ★New★
    // ここにあった定義が ModelUtilityへ移動されている]

public:
    ModelAnimation(int modelHandle);   // コンストラクタ
    ~ModelAnimation();  // デストラクタ

    void Update();  // 更新

    // note: mixamo でダウンロードすると、
    // 0番目のアニメーションデータはダミーが入っているので、
    // デフォルトで適用するインデックスを1にしておく
    void ChangeAnimation(AnimationState state, int index = 1); // アニメーション切り替え処理

    void SetLoop(bool isLoop) { mbLoop = isLoop; }  // ループ設定
    void SetLoopFinishState(AnimationState state) { mnLoopFinishState = state; }    // ループ終了時に再生するアニメーション
    void SetAnimationBlend(bool isBlend);   // アニメーションのブレンド設定

    AnimationState GetNowState() { return mnState; }    // 現在再生されているアニメーションの取得
    bool IsLoopFinish() { return mbLoopFinish; }        // アニメーションのループが終了しているかどうか

    void SetAnimationCount(float count) { mfAnimationCount = count; }

private:
    int mnModelHandle;  // モデルのハンドル

    float mfAnimationTime;  // 再生しているアニメーションの現在の再生時間
    float mfAnimationCount;//再生時間を決める
    int mnAnimationIndex;   // 再生しているアニメーションのインデックス

    float mfOldAnimationTime;   // 1つ前のアニメーション再生時間
    int mnOldAnimationIndex;    // １つ前のアニメーションのインデックス

    float mfAnimBlendRate;      // モーションの切り替わり度合

    AnimationState mnState; // 現在再生しているアニメーションの番号

    bool mbLoop;            // モーションをループさせるかどうか
    AnimationState mnLoopFinishState;   // ループが終わった時に再生したいアニメーション番号
    bool mbLoopFinish;      // モーションループが終わったかどうか
};