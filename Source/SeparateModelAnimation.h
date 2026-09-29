#pragma once
#include <string>
#include <vector>
#include "ModelUtility.h"

class SeparateModelAnimation
{
public:
    SeparateModelAnimation(int modelHandle);   // コンストラクタ
    ~SeparateModelAnimation();  // デストラクタ
    void Update();  // 更新
    void ChangeAnimation(AnimationState state, int index = 0); // モーション切り替え処理

   
    /// @brief モーションデータの追加
    /// @brief note: 分割されているモーションをデータとして登録する。
    void AddAnimation(AnimationState state, std::string filename);

   
    /// @brief 対応したモーションハンドルの取得
    /// @brief note: vector配列にデータが格納されているため、
    /// @brief 毎回ハンドルを検索する手間が出てきてしまうので、
    /// @brief それを行うための関数。
    int GetAnimationHandle(AnimationState state);


    /// @brief ループ設定
    void SetLoop(bool isLoop) { loop_ = isLoop; }
    /// @brief ループ終了時に再生するモーション
    void SetLoopFinishState(AnimationState state) { loop_finish_state_ = state; }
    /// @brief モーションのブレンド設定
    void SetAnimationBlend(bool isBlend);

    /// @brief 現在再生されているモーションの取得
    AnimationState GetNowState() { return state_; }
    /// @brief モーションのループが終了しているかどうか
    bool IsLoopFinish() { return loop_finish_; }
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

  
    /// @brief モーションデータのリスト
    std::vector<AnimationInfo*> animation_info_list_;
};