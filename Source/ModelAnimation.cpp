#include"DxLib.h"
#include"ModelAnimation.h"


/// @brief ModelAnimationの初期化（コンストラクタ）
ModelAnimation::ModelAnimation(int ModelHandle)
	:model_handle_(ModelHandle)
	, animation_time_(0.0f)
	, animation_index_(-1)
	, state_(AnimationState::ANIMATION_MAX)//最初は最大値
	, old_animation_time_(0.0f)
	, old_animation_index_(-1)
	, anim_blend_rate_(1.0f)
	,animation_count_(0.5f)
	/*, Do(false)*/
	, loop_(true)
	, loop_finish_state_(AnimationState::ANIMATION_MAX)
	, loop_finish_(false)
{
	//アニメモーションで移動しているフレーム番号を検索する
	int moveAnimFrameIndex = MV1SearchFrame(model_handle_, "root");
	//移動を無効にする
	MV1SetFrameUserLocalMatrix
	(model_handle_
		, moveAnimFrameIndex
		, MV1GetFrameLocalMatrix(model_handle_, moveAnimFrameIndex)
	);


	ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);




}


ModelAnimation::~ModelAnimation()
{

}



/// @brief ModelAnimationの状態更新処理
void ModelAnimation::Update()
{


	//アニメーションのブレンド率を高める
	if (anim_blend_rate_ < 1.0f)
	{
		anim_blend_rate_ += 0.1f;   //＋＝0.1fはブレンド速度。自由に変えてもOK
		if (anim_blend_rate_ > 1.0f)
		{
			anim_blend_rate_ = 1.0f;
		}
	}



	//アニメーションの更新
	float fAnimTotaltime = 0.0f;



	if (animation_index_ != -1)
	{//そう再生時間の取得
		fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);


		//アニメーションを進める
		animation_time_ += animation_count_;

		//ループさせる
		if (animation_time_ > fAnimTotaltime)
		{
			//ループしない設定であれば
			if (!loop_)
			{
				//次のアニメションが設定されていないっ場合	
				if (loop_finish_state_ == ANIMATION_MAX)
				{
					//アニメーションはこれ以上進めず。処理を中断させる
					loop_finish_ = true;
					return;
				}
				ChangeAnimation(loop_finish_state_);

				//ブレンドはしない
				SetAnimationBlend(false);
				//変更されたので改めて取っておく
				fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);
			}
			animation_time_ = 0.0f;
		}

		//アニメーションを反映
		MV1SetAttachAnimTime(model_handle_, animation_index_, animation_time_);

		//ブレンド率を設定
		MV1SetAttachAnimBlendRate(model_handle_, animation_index_, anim_blend_rate_);
	}

	//一つ目のアニメーションを更新
	if (old_animation_index_ != -1)
	{//そう再生時間の取得
		fAnimTotaltime = MV1GetAttachAnimTotalTime(model_handle_, old_animation_index_);

		////アニメーションを進める
		old_animation_time_ += animation_count_;

		//ループさせる
		if (old_animation_time_ > fAnimTotaltime)
		{
			old_animation_time_ = 0.0f;

		}

		//アニメーションを反映
		MV1SetAttachAnimTime(model_handle_, old_animation_index_, old_animation_time_);

		//ブレンド率を設定
		MV1SetAttachAnimBlendRate(model_handle_, old_animation_index_, 1.0f - anim_blend_rate_);
	}

}

//アニメーションの切り替え

/// @brief ModelAnimationのChangeAnimation処理
void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	/*if (AnimationStart())
	{
		return;
	}*/

	//切り替えようとしているアニメーションが既に設定されている場合
	if (state_ == state)
	{
		return; //何もしない
	}



	//切り替え先の番号を保持
	state_ = state;

	//ループの初期化
	loop_ = true;//設定が特にない場合はループ
	loop_finish_state_ = AnimationState::ANIMATION_MAX;//ループ終了時にアニメーションは特になし
	loop_finish_ = false;

	if (old_animation_index_ != -1)
	{
		//アニメーションのデタッチ（取り外す)
		MV1DetachAnim(model_handle_, old_animation_index_);
		old_animation_index_ = -1;
	}

	//現在のアニメーション状態を保持する
	old_animation_index_ = animation_index_;
	old_animation_time_ = animation_time_;



	//アニメーションのアタッチ
	animation_index_ = MV1AttachAnim(model_handle_, (int)state);

	//再生時間の初期化
	animation_time_ = 0.0f;

	//ブレンド状態を初期化
	anim_blend_rate_ = (old_animation_index_ == -1 ? 1.0f : 0.0f);

}

//bool ModelAnimation::is_animation_()
//	{//そう再生時間の取得
//		//ループさせる
//アニメションのブレンド設定

/// @brief ModelAnimationのSetAnimationBlend処理
void ModelAnimation::SetAnimationBlend(bool isblend)
{
	if (isblend)
	{
		anim_blend_rate_ = (old_animation_index_ == -1 ? 1.0f : 0.0f);
	}
	else//ブレンドしない
	{
		//ブレンドしない状態にする
		anim_blend_rate_ = 1.0f;

		//ブレン祖する必要がないので古いアニメーションはデタッチしておく
		if (old_animation_index_ != -1)
		{
			MV1DetachAnim(model_handle_, old_animation_index_);
			old_animation_index_ = -1;
		}
	}

}
