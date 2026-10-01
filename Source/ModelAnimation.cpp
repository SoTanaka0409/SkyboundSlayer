#include"DxLib.h"
#include"ModelAnimation.h"


/// @brief ModelAnimationの初期化（コンストラクタ）
ModelAnimation::ModelAnimation(int ModelHandle)
	:m_ModelHandle(ModelHandle)
	, m_AnimationTime(0.0f)
	, m_AnimationIndex(-1)
	, m_State(AnimationState::ANIMATION_MAX)//最初は最大値
	, m_OldAnimationTime(0.0f)
	, m_OldAnimationIndex(-1)
	, m_AnimBlendRate(1.0f)
	,m_AnimationCount(0.5f)
	/*, Do(false)*/
	, m_Loop(true)
	, m_LoopFinishState(AnimationState::ANIMATION_MAX)
	, m_LoopFinish(false)
{
	// アニメモーションで移動しているフレーム番号を検索する
	int moveAnimFrameIndex = MV1SearchFrame(m_ModelHandle, "root");
	// 移動を無効にする
	MV1SetFrameUserLocalMatrix
	(m_ModelHandle
		, moveAnimFrameIndex
		, MV1GetFrameLocalMatrix(m_ModelHandle, moveAnimFrameIndex)
	);


	ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);




}


ModelAnimation::~ModelAnimation()
{

}



/// @brief ModelAnimationの状態更新処理
void ModelAnimation::Update()
{


	// アニメーションのブレンド率を高める
	if (m_AnimBlendRate < 1.0f)
	{
		m_AnimBlendRate += 0.1f;   //＋＝0.1fはブレンド速度。自由に変えてもOK
		if (m_AnimBlendRate > 1.0f)
		{
			m_AnimBlendRate = 1.0f;
		}
	}



	// アニメーションの更新
	float fAnimTotaltime = 0.0f;



	if (m_AnimationIndex != -1)
	{//そう再生時間の取得
		fAnimTotaltime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_AnimationIndex);


		//アニメーションを進める
		m_AnimationTime += m_AnimationCount;

		//ループさせる
		if (m_AnimationTime > fAnimTotaltime)
		{
			//ループしない設定であれば
			if (!m_Loop)
			{
				//次のアニメションが設定されていないっ場合	
				if (m_LoopFinishState == ANIMATION_MAX)
				{
					//アニメーションはこれ以上進めず。処理を中断させる
					m_LoopFinish = true;
					return;
				}
				ChangeAnimation(m_LoopFinishState);

				//ブレンドはしない
				SetAnimationBlend(false);
				//変更されたので改めて取っておく
				fAnimTotaltime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_AnimationIndex);
			}
			m_AnimationTime = 0.0f;
		}

		//アニメーションを反映
		MV1SetAttachAnimTime(m_ModelHandle, m_AnimationIndex, m_AnimationTime);

		//ブレンド率を設定
		MV1SetAttachAnimBlendRate(m_ModelHandle, m_AnimationIndex, m_AnimBlendRate);
	}

	// 一つ目のアニメーションを更新
	if (m_OldAnimationIndex != -1)
	{//そう再生時間の取得
		fAnimTotaltime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_OldAnimationIndex);

		////アニメーションを進める
		m_OldAnimationTime += m_AnimationCount;

		//ループさせる
		if (m_OldAnimationTime > fAnimTotaltime)
		{
			m_OldAnimationTime = 0.0f;

		}

		//アニメーションを反映
		MV1SetAttachAnimTime(m_ModelHandle, m_OldAnimationIndex, m_OldAnimationTime);

		//ブレンド率を設定
		MV1SetAttachAnimBlendRate(m_ModelHandle, m_OldAnimationIndex, 1.0f - m_AnimBlendRate);
	}

}

/// @brief アニメーションの切り替え

/// @brief ModelAnimationのChangeAnimation処理
void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	/*if (AnimationStart())
	{
		return;
	}*/

	// 切り替えようとしているアニメーションが既に設定されている場合
	if (m_State == state)
	{
		return; //何もしない
	}



	// 切り替え先の番号を保持
	m_State = state;

	// ループの初期化
	m_Loop = true;//設定が特にない場合はループ
	m_LoopFinishState = AnimationState::ANIMATION_MAX;//ループ終了時にアニメーションは特になし
	m_LoopFinish = false;

	if (m_OldAnimationIndex != -1)
	{
		//アニメーションのデタッチ（取り外す)
		MV1DetachAnim(m_ModelHandle, m_OldAnimationIndex);
		m_OldAnimationIndex = -1;
	}

	// 現在のアニメーション状態を保持する
	m_OldAnimationIndex = m_AnimationIndex;
	m_OldAnimationTime = m_AnimationTime;



	// アニメーションのアタッチ
	m_AnimationIndex = MV1AttachAnim(m_ModelHandle, (int)state);

	// 再生時間の初期化
	m_AnimationTime = 0.0f;

	// ブレンド状態を初期化
	m_AnimBlendRate = (m_OldAnimationIndex == -1 ? 1.0f : 0.0f);

}

/// @brief bool ModelAnimation::m_IsAnimation()
/// @brief {//そう再生時間の取得
// //ループさせる
// アニメションのブレンド設定

// ModelAnimationのSetAnimationBlend処理
void ModelAnimation::SetAnimationBlend(bool isblend)
{
	if (isblend)
	{
		m_AnimBlendRate = (m_OldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else//ブレンドしない
	{
		//ブレンドしない状態にする
		m_AnimBlendRate = 1.0f;

		//ブレン祖する必要がないので古いアニメーションはデタッチしておく
		if (m_OldAnimationIndex != -1)
		{
			MV1DetachAnim(m_ModelHandle, m_OldAnimationIndex);
			m_OldAnimationIndex = -1;
		}
	}

}
