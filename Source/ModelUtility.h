#pragma once
#include"InputManager.h"
// アニメーションの切り替え番号
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
	// note: ↑ここから上には追加しない。
	//       Hero.mv1 がこの順番じゃないと機能しないので維持しておく。
	//       ただし、SeparateModelAnimation の方で使ってはいけない訳ではないので注意。

	// 上にある定義以外で使いたいモーションがあれば、このように下に追加していけばOK。
	ANIMATION_WALKING,      // 歩き
	ANIMATION_DYING,        // 死亡時
	ANIMATION_DROP_KICK,    // ドロップキック

	ANIMATION_MAX
};





// 分割されているアニメーションのデータ
struct AnimationInfo
{
	AnimationState mState;
	int mnAnimationHandle;
};
