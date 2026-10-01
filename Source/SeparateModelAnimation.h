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
    void SetLoop(bool isLoop) { m_Loop = isLoop; }
    /// @brief ループ終了時に再生するモーション
    void SetLoopFinishState(AnimationState state) { m_LoopFinishState = state; }
    /// @brief モーションのブレンド設定
    void SetAnimationBlend(bool isBlend);

    /// @brief 現在再生されているモーションの取得
    AnimationState GetNowState() { return m_State; }
    /// @brief モーションのループが終了しているかどうか
    bool IsLoopFinish() { return m_LoopFinish; }
    void SetAnimationCount(float count) { m_AnimationCount = count; }

private:
    int m_ModelHandle;  // モデルのハンドル

    float m_AnimationTime;  // 再生しているモーションの現在の再生時間
    float m_AnimationCount;
    int m_AnimationIndex;   // 再生しているモーションのインデックス

    float m_OldAnimationTime;   // 1つ前のモーション再生時間
    int m_OldAnimationIndex;    // １つ前のモーションのインデックス

    float m_AnimBlendRate;      // モーションの切り替わり度合

    AnimationState m_State; // 現在再生しているモーションの番号

    bool m_Loop;            // モーションをループさせるかどうか
    AnimationState m_LoopFinishState;   // ループが終わった時に再生したいモーション番号
    bool m_LoopFinish;      // モーションループが終わったかどうか

  
    /// @brief モーションデータのリスト
    std::vector<AnimationInfo*> m_AnimationInfoList;
};