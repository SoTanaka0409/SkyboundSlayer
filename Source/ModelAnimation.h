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
	void SetLoop(bool isLoop) { loop_ = isLoop; }

	/// @brief 非ループアニメーションの再生終了時に自動移行するアニメーションステートを設定する
	/// @param state 移行先のアニメーションステート
	void SetLoopFinishState(AnimationState state) { loop_finish_state_ = state; }

	/// @brief モーション切り替え時のブレンド（補間）処理の有効/無効を設定する
	/// @param isBlend ブレンドを行う場合はtrue
	void SetAnimationBlend(bool isBlend);

	/// @brief 現在再生中のアニメーションステートを取得する
	/// @return AnimationState 現在のアニメーションステート
	AnimationState GetNowState() { return state_; }

	/// @brief アニメーションの再生（または1ループ）が終了したか判定する
	/// @return bool 再生完了した場合はtrue
	bool IsLoopFinish() { return loop_finish_; }

	/// @brief アニメーションの進行速度（コマ送り・カウントの加算量）を設定する
	/// @param count 再生時間の進行倍率・速度
	void SetAnimationCount(float count) { animation_count_ = count; }

private:
	int model_handle_;                   ///< 制御対象となる3Dモデルのグラフィックハンドル

	float animation_time_;               ///< 現在再生中のアニメーションの経過時間
	float animation_count_;              ///< アニメーション再生速度の進行カウント係数
	int animation_index_;                ///< 現在再生中のアニメーションインデックス番号

	float old_animation_time_;           ///< 切り替え前のアニメーション経過時間（ブレンド用）
	int old_animation_index_;            ///< 切り替え前のアニメーションインデックス番号（ブレンド用）

	float anim_blend_rate_;              ///< 前のモーションから現在のモーションへの補間ブレンド率（0.0f〜1.0f）

	AnimationState state_;               ///< 現在再生されているアニメーションステート識別番号

	bool loop_;                          ///< アニメーションをループ再生させるかどうかの判定フラグ
	AnimationState loop_finish_state_;   ///< 非ループアニメーション終了時に移行するアニメーションステート
	bool loop_finish_;                   ///< モーションの再生が最終フレームまで到達・終了したかを示すフラグ
};