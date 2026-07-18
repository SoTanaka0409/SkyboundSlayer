#pragma once
#include <string>
#include <vector>
#include "ModelUtility.h"

class SeparateModelAnimation
{
public:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    SeparateModelAnimation(int modelHandle);   // コンストラクタ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    ~SeparateModelAnimation();  // デストラクタ

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void Update();  // 更新

   
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void ChangeAnimation(AnimationState state, int index = 0); // モーション切り替え処理

   
    // モーションデータの追加
    // note: 分割されているモーションをデータとして登録する。
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void AddAnimation(AnimationState state, std::string filename);

   
    // 対応したモーションハンドルの取得
    // note: vector配列にデータが格納されているため、
    //       毎回ハンドルを検索する手間が出てきてしまうので、
    //       それを行うための関数。
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    int GetAnimationHandle(AnimationState state);


    // ループ設定
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetLoop(bool isLoop) { loop_ = isLoop; }
    // ループ終了時に再生するモーション
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetLoopFinishState(AnimationState state) { loop_finish_state_ = state; }
    // モーションのブレンド設定
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetAnimationBlend(bool isBlend);

    // 現在再生されているモーションの取得
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    AnimationState GetNowState() { return state_; }
    // モーションのループが終了しているかどうか
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    bool IsLoopFinish() { return loop_finish_; }

    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
    void SetAnimationCount(float count) { animation_count_ = count; }

private:
    int model_handle_;  // モデルのハンドル

    float animation_time_;  // 再生しているモーションの現在の再生時間
    float animation_count_;
    int animation_index_;   // 再生しているモーションのインデックス

    float old_animation_time_;   // 1つ前のモーション再生時間
    int old_animation_index_;    // １つ前のモーションのインデックス

    float anim_blend_rate_;      // モーションの切り替わり度合

    AnimationState state_; // 現在再生しているモーションの番号

    bool loop_;            // モーションをループさせるかどうか
    AnimationState loop_finish_state_;   // ループが終わった時に再生したいモーション番号
    bool loop_finish_;      // モーションループが終わったかどうか

    // ★New★
    // モーションデータのリスト
    std::vector<AnimationInfo*> animation_info_list_;
};