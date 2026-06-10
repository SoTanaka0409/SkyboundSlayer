#include"DxLib.h"
#include"ModelAnimation.h"

ModelAnimation::ModelAnimation(int ModelHandle)
	:mnModelHandle(ModelHandle)
	, mfAnimationTime(0.0f)
	, mnAnimationIndex(-1)
	, mnState(AnimationState::ANIMATION_MAX)//最初は最大値
	, mfOldAnimationTime(0.0f)
	, mnOldAnimationIndex(-1)
	, mfAnimBlendRate(1.0f)
	,mfAnimationCount(0.5f)
	/*, Do(false)*/
	, mbLoop(true)
	, mnLoopFinishState(AnimationState::ANIMATION_MAX)
	, mbLoopFinish(false)
{
	//アニメモーションで移動しているフレーム番号を検索する
	int moveAnimFrameIndex = MV1SearchFrame(mnModelHandle, "root");
	//移動を無効にする
	MV1SetFrameUserLocalMatrix
	(mnModelHandle
		, moveAnimFrameIndex
		, MV1GetFrameLocalMatrix(mnModelHandle, moveAnimFrameIndex)
	);


	ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);




}


ModelAnimation::~ModelAnimation()
{

}


void ModelAnimation::Update()
{


	//アニメーションのブレンド率を高める
	if (mfAnimBlendRate < 1.0f)
	{
		mfAnimBlendRate += 0.1f;   //＋＝0.1fはブレンド速度。自由に変えてもOK
		if (mfAnimBlendRate > 1.0f)
		{
			mfAnimBlendRate = 1.0f;
		}
	}



	//アニメーションの更新
	float fAnimTotaltime = 0.0f;



	if (mnAnimationIndex != -1)
	{//そう再生時間の取得
		fAnimTotaltime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);


		//アニメーションを進める
		mfAnimationTime += mfAnimationCount;

		//ループさせる
		if (mfAnimationTime > fAnimTotaltime)
		{
			//ループしない設定であれば
			if (!mbLoop)
			{
				//次のアニメションが設定されていないっ場合	
				if (mnLoopFinishState == ANIMATION_MAX)
				{
					//アニメーションはこれ以上進めず。処理を中断させる
					mbLoopFinish = true;
					return;
				}
				ChangeAnimation(mnLoopFinishState);

				//ブレンドはしない
				SetAnimationBlend(false);
				//変更されたので改めて取っておく
				fAnimTotaltime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
			}
			mfAnimationTime = 0.0f;
			//ChangeAnimation(ANIMATION_NEUTRAL);
			//Do = false;
		}

		//アニメーションを反映
		MV1SetAttachAnimTime(mnModelHandle, mnAnimationIndex, mfAnimationTime);

		//ブレンド率を設定
		MV1SetAttachAnimBlendRate(mnModelHandle, mnAnimationIndex, mfAnimBlendRate);
	}

	//一つ目のアニメーションを更新
	if (mnOldAnimationIndex != -1)
	{//そう再生時間の取得
		fAnimTotaltime = MV1GetAttachAnimTotalTime(mnModelHandle, mnOldAnimationIndex);

		////アニメーションを進める
		//if (mnState != ANIMATION_JUMP_IN)
		//{
		//	mfOldAnimationTime += 0.5f;
		//}
		//else
		//{
		//	mfOldAnimationTime += 0.25f;
		//}

		//ループさせる
		if (mfOldAnimationTime > fAnimTotaltime)
		{
			mfOldAnimationTime = 0.0f;

		}

		//アニメーションを反映
		//MV1SetAttachAnimTime(mnModelHandle, mnOldAnimationIndex, mfOldAnimationTime);

		//ブレンド率を設定
		MV1SetAttachAnimBlendRate(mnModelHandle, mnOldAnimationIndex, 1.0f - mfAnimBlendRate);
	}

}

//アニメーションの切り替え
void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	/*if (AnimationStart())
	{
		return;
	}*/

	//切り替えようとしているアニメーションが既に設定されている場合
	if (mnState == state)
	{
		return; //何もしない
	}



	//切り替え先の番号を保持
	mnState = state;

	//ループの初期化
	mbLoop = true;//設定が特にない場合はループ
	mnLoopFinishState = AnimationState::ANIMATION_MAX;//ループ終了時にアニメーションは特になし
	mbLoopFinish = false;

	if (mnOldAnimationIndex != -1)
	{
		//アニメーションのデタッチ（取り外す)
		MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
		mnOldAnimationIndex = -1;
	}

	//現在のアニメーション状態を保持する
	mnOldAnimationIndex = mnAnimationIndex;
	mfOldAnimationTime = mfAnimationTime;



	//アニメーションのアタッチ
	mnAnimationIndex = MV1AttachAnim(mnModelHandle, (int)state);

	//再生時間の初期化
	mfAnimationTime = 0.0f;

	//ブレンド状態を初期化
	//ブレンド率は、古いモーションが有効でない場合は1.1f（ブレンドしない状態）にしておく

	/*if (mnOldAnimationIndex == -1)
	{
		mfAnimBlendRate = 1.0f;
	}
	else
	{
		mfAnimBlendRate = 0.0f;
	}*/

}

//bool ModelAnimation::Animation()
//{
//	float fAnimTotaltime = 0.0f;
//
//
//
//	if (mnAnimationIndex != -1)
//	{//そう再生時間の取得
//		fAnimTotaltime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
//
//		//ループさせる
//		if (mfAnimationTime > fAnimTotaltime-5)
//		{
//			return true;
//		}
//	}
//
//	return false;
//
//}
//アニメションのブレンド設定
void ModelAnimation::SetAnimationBlend(bool isblend)
{
	if (isblend)
	{
		mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else//ブレンドしない
	{
		//ブレンドしない状態にする
		mfAnimBlendRate = 1.0f;

		//ブレン祖する必要がないので古いアニメーションはデタッチしておく
		if (mnOldAnimationIndex != -1)
		{
			MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
			mnOldAnimationIndex = -1;
		}
	}

}