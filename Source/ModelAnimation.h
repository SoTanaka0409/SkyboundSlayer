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
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    ModelAnimation(int modelHandle);   // コンストラクタ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    ~ModelAnimation();  // デストラクタ

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void Update();  // 更新

    // note: mixamo でダウンロードすると、
    // 0番目のアニメーションデータはダミーが入っているので、
    // デフォルトで適用するインデックスを1にしておく
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void ChangeAnimation(AnimationState state, int index = 1); // アニメーション切り替え処理

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetLoop(bool isLoop) { loop_ = isLoop; }  // ループ設定
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetLoopFinishState(AnimationState state) { loop_finish_state_ = state; }    // ループ終了時に再生するアニメーション
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetAnimationBlend(bool isBlend);   // アニメーションのブレンド設定

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    AnimationState GetNowState() { return state_; }    // 現在再生されているアニメーションの取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    bool IsLoopFinish() { return loop_finish_; }        // アニメーションのループが終了しているかどうか

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetAnimationCount(float count) { animation_count_ = count; }

private:
    int model_handle_;  // モデルのハンドル

    float animation_time_;  // 再生しているアニメーションの現在の再生時間
    float animation_count_;//再生時間を決める
    int animation_index_;   // 再生しているアニメーションのインデックス

    float old_animation_time_;   // 1つ前のアニメーション再生時間
    int old_animation_index_;    // １つ前のアニメーションのインデックス

    float anim_blend_rate_;      // モーションの切り替わり度合

    AnimationState state_; // 現在再生しているアニメーションの番号

    bool loop_;            // モーションをループさせるかどうか
    AnimationState loop_finish_state_;   // ループが終わった時に再生したいアニメーション番号
    bool loop_finish_;      // モーションループが終わったかどうか
};