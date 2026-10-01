#pragma once
#include"InputManager.h"
/// @brief アニメーションの切り替え番号
enum AnimationState
{
	ANIMATION_NEUTRAL = 0,  // 待機
	ANIMATION_RUN,          // 走り
	ANIMATION_ATTACKJUMP,
	ANIMATION_JUMP_IN,      // ジャンプ開始
	ANIMATION_JUMP_LOOP,    // ジャンプ中
	ANIMATION_JUMP_OUT,     // 着地時
	ANIMATION_ATTACK,       // 攻撃
	ANIMATION_ATTACKSLIDE,   //攻撃
	ANIMATION_SLIDE,        //回避   
	ANIMATION_ATTACKMAGIC,
	/// @brief note: ↑ここから上には追加しない。
	/// @brief Hero.mv1 がこの順番じゃないと機能しないので維持しておく。
	/// @brief ただし、SeparateModelAnimation の方で使ってはいけない訳ではないので注意。

	/// @brief 上にある定義以外で使いたいモーションがあれば、このように下に追加していけばOK。
	ANIMATION_WALKING,      // 歩き
	ANIMATION_DYING,        // 死亡時
	ANIMATION_DROP_KICK,    // ドロップキック

	ANIMATION_MAX
};





/// @brief 分割されているアニメーションのデータ
struct AnimationInfo
{
	AnimationState m_State;
	int m_AnimationHandle;
};
