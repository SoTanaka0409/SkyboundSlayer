#pragma once
#include "ModelUtility.h"
#include <string>
#include <vector>

/// @brief 3Dモデル（DxLibモデルハンドル）の単一・ブレンドアニメーションの再生・遷移制御を行うクラス
class ModelAnimation
{
public:
	/// @brief ModelAnimationのコンストラクタ
	/// @param modelHandle 制御対象となるDxLibの3Dモデルハンドル
	ModelAnimation(int modelHandle);

	/// @brief ModelAnimationのデストラクタ
	~ModelAnimation();

	/// @brief アニメーション時間の進行、ブレンド率計算、ループ終端判定などの毎フレーム更新を行う
	void Update();

	/// @brief 再生するアニメーションを切り替える
	/// @param state 切り替え先のアニメーションステート
	/// @param index モデルデータ内のアニメーションインデックス（Mixamo等のダミー回避のため既定値1）
	void ChangeAnimation(AnimationState state, int index = 1);

	/// @brief モーションのループ再生の有効/無効を設定する
	/// @param isLoop ループ再生を行う場合はtrue
	void SetLoop(bool isLoop) { m_Loop = isLoop; }

	/// @brief 非ループアニメーションの再生終了時に自動移行するアニメーションステートを設定する
	/// @param state 移行先のアニメーションステート
	void SetLoopFinishState(AnimationState state) { m_LoopFinishState = state; }

	/// @brief モーション切り替え時のブレンド（補間）処理の有効/無効を設定する
	/// @param isBlend ブレンドを行う場合はtrue
	void SetAnimationBlend(bool isBlend);

	/// @brief 現在再生中のアニメーションステートを取得する
	/// @return AnimationState 現在のアニメーションステート
	AnimationState GetNowState() { return m_State; }

	/// @brief アニメーションの再生（または1ループ）が終了したか判定する
	/// @return bool 再生完了した場合はtrue
	bool IsLoopFinish() { return m_LoopFinish; }

	/// @brief アニメーションの進行速度（コマ送り・カウントの加算量）を設定する
	/// @param count 再生時間の進行倍率・速度
	void SetAnimationCount(float count) { m_AnimationCount = count; }

private:
	int m_ModelHandle;                   ///< 制御対象となる3Dモデルのグラフィックハンドル

	float m_AnimationTime;               ///< 現在再生中のアニメーションの経過時間
	float m_AnimationCount;              ///< アニメーション再生速度の進行カウント係数
	int m_AnimationIndex;                ///< 現在再生中のアニメーションインデックス番号

	float m_OldAnimationTime;           ///< 切り替え前のアニメーション経過時間（ブレンド用）
	int m_OldAnimationIndex;            ///< 切り替え前のアニメーションインデックス番号（ブレンド用）

	float m_AnimBlendRate;              ///< 前のモーションから現在のモーションへの補間ブレンド率（0.0f〜1.0f）

	AnimationState m_State;               ///< 現在再生されているアニメーションステート識別番号

	bool m_Loop;                          ///< アニメーションをループ再生させるかどうかの判定フラグ
	AnimationState m_LoopFinishState;   ///< 非ループアニメーション終了時に移行するアニメーションステート
	bool m_LoopFinish;                   ///< モーションの再生が最終フレームまで到達・終了したかを示すフラグ
};