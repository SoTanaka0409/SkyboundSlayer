#include "Model.h"
#include "AttachmentModel.h"
#include "Master.h"

/// @brief Modelクラスのコンストラクタ
/// @param filename 3Dモデルのファイルパス
/// @param initPos 初期配置座標
/// @param isSeparateAnimation アニメーションを分離して読み込むかどうかのフラグ
/// @details 3Dモデルリソースのロードおよびアニメーション制御クラス（ModelAnimationまたはSeparateModelAnimation）の初期化を行う
Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
	: m_Position(initPos)
	, m_Attachment(nullptr)
	, mvScale(VGet(1.0f, 1.0f, 1.0f))
	, m_ChangeTextureHandle(-1)
	, m_IsSeparate(isSeparateAnimation)
{
	m_Handle = Master::m_ResourceManager->LoadModel(filename.c_str());

	if (isSeparateAnimation)
	{
		m_SeparateAnimation = new SeparateModelAnimation(m_Handle);
		m_Animation = nullptr;
	}
	else
	{
		m_Animation = new ModelAnimation(m_Handle);
		m_SeparateAnimation = nullptr;
	}
}

/// @brief Modelクラスのデストラクタ
/// @details 動的確保したアニメーションオブジェクト、アタッチメントモデル、差分テクスチャ、3Dモデル本体の解放処理を行う
Model::~Model()
{
	if (m_SeparateAnimation != nullptr)
	{
		delete m_SeparateAnimation;
		m_SeparateAnimation = nullptr;
	}
	if (m_Animation != nullptr)
	{
		delete m_Animation;
		m_Animation = nullptr;
	}

	if (m_Attachment != nullptr)
	{
		m_Attachment->SetDeleteFlag(true);
	}

	if (m_ChangeTextureHandle != -1)
	{
		DeleteGraph(m_ChangeTextureHandle);
		m_ChangeTextureHandle = -1;
	}

	if (m_Handle != -1)
	{
		MV1DeleteModel(m_Handle);
		m_Handle = -1;
	}
}

/// @brief アニメーションの追加登録を行う（分離アニメーション時のみ有効）
/// @param state アニメーション識別用ステート
/// @param filename アニメーションファイルパス
void Model::AddAnimation(AnimationState state, std::string filename)
{
	if (m_SeparateAnimation != nullptr)
	{
		m_SeparateAnimation->AddAnimation(state, filename);
	}
}

/// @brief モデルの状態更新処理を行う
/// @details アニメーションの進行更新およびDxLibのモデル位置・回転パラメータの再設定を行う
void Model::Update()
{
	if (m_Animation != nullptr)
	{
		m_Animation->Update();
	}

	if (m_SeparateAnimation != nullptr)
	{
		m_SeparateAnimation->Update();
	}

	MV1SetPosition(m_Handle, m_Position);
	MV1SetRotationXYZ(m_Handle, m_Rotation);
}

/// @brief 3Dモデルの描画処理を行う
void Model::Draw()
{
	MV1DrawModel(m_Handle);
}

/// @brief 再生するアニメーションを変更する
/// @param state 移行先のアニメーションステート
void Model::ChangeAnimation(AnimationState state)
{
	if (m_Animation != nullptr)
	{
		m_Animation->ChangeAnimation(state);
	}
	if (m_SeparateAnimation != nullptr)
	{
		m_SeparateAnimation->ChangeAnimation(state);
	}
}

/// @brief アニメーションのループ再生フラグを設定する
/// @param loop ループ再生を行う場合はtrue
void Model::SetLoop(bool loop)
{
	if (m_Animation != nullptr)
	{
		m_Animation->SetLoop(loop);
	}
	if (m_SeparateAnimation != nullptr)
	{
		m_SeparateAnimation->SetLoop(loop);
	}
}

/// @brief 非ループアニメーション終了後の状態切り替えを設定する
/// @param state アニメーション再生終了時に移行するステート
void Model::SetLoopFinishState(AnimationState state)
{
	if (m_Animation != nullptr)
	{
		m_Animation->SetLoopFinishState(state);
	}
	if (m_SeparateAnimation != nullptr)
	{
		m_SeparateAnimation->SetLoopFinishState(state);
	}
}

/// @brief アニメーションブレンド（滑らかな切り替え）の有効/無効を設定する
/// @param isBlend ブレンドを行う場合はtrue
void Model::SetAnimationBlend(bool isBlend)
{
	if (m_Animation != nullptr)
	{
		m_Animation->SetAnimationBlend(isBlend);
	}
	if (m_SeparateAnimation != nullptr)
	{
		m_SeparateAnimation->SetAnimationBlend(isBlend);
	}
}

/// @brief 現在再生されているアニメーションのステートを取得する
/// @return AnimationState 現在のアニメーションステート
AnimationState Model::GetNowState()
{
	AnimationState ret = AnimationState::ANIMATION_MAX;

	if (m_Animation != nullptr)
	{
		ret = m_Animation->GetNowState();
	}
	if (m_SeparateAnimation != nullptr)
	{
		ret = m_SeparateAnimation->GetNowState();
	}

	return ret;
}

/// @brief アニメーションが最後まで再生完了したか判別する
/// @return bool 再生完了した場合はtrue
bool Model::IsAnimationLoopFinish()
{
	bool ret = false;

	if (m_Animation != nullptr)
	{
		ret = m_Animation->IsLoopFinish();
	}
	if (m_SeparateAnimation != nullptr)
	{
		ret = m_SeparateAnimation->IsLoopFinish();
	}

	return ret;
}

/// @brief 武器などのアタッチメントモデルをボーン（フレーム）に結合・登録する
/// @param filename アタッチメントの3Dモデルパス
/// @param attachFrameName 結合対象のボーン（フレーム）名
/// @param offsetPos ボーンからのオフセット位置
/// @param offsetRot ボーンからのオフセット回転角
void Model::AddAttachment(std::string filename, std::string attachFrameName, VECTOR offsetPos, VECTOR offsetRot)
{
	if (m_Attachment != nullptr)
	{
		m_Attachment->SetDeleteFlag(true);
		m_Attachment = nullptr;
	}
	int frameIndex = MV1SearchFrame(m_Handle, attachFrameName.c_str());
	if (frameIndex != -1)
	{
		m_Attachment = new AttachmentModel(filename, m_Handle, frameIndex, offsetPos, offsetRot);
	}
}

/// @brief アタッチメント非依存で、指定ボーン（フレーム）のワールド座標を取得する
/// @param attachFrameName 取得したいボーン（フレーム）名
/// @return VECTOR ボーンのワールド座標（見つからない場合は初期高さ座標）
VECTOR Model::GetAttachmentPosition_None(std::string attachFrameName)
{
	int frameIndex = MV1SearchFrame(m_Handle, attachFrameName.c_str());
	if (frameIndex != -1)
	{
		return MV1GetFramePosition(m_Handle, frameIndex);
	}
	return VGet(0.0f, 30.0f, 0.0f);
}

/// @brief 登録済みアタッチメントの現在の先端・オフセット適用後のワールド座標を取得する
/// @return VECTOR 計算されたアタッチメントのワールド座標
VECTOR Model::GetAttachmentPosition()
{
	if (m_Attachment != nullptr)
	{
		VECTOR vec = VGet(0.0f, -50.0f, 0.0f);

		MATRIX matrix = MV1GetFrameLocalWorldMatrix(m_Attachment->GetHandle(), 0);

		vec = VTransform(vec, matrix);

		return vec;
	}

	return VGet(0.0f, 0.0f, 0.0f);
}

/// @brief モデルの拡大スケールを設定する
/// @param scale 各軸の倍率ベクトル
void Model::SetScale(VECTOR scale)
{
	MV1SetScale(m_Handle, scale);
}

/// @brief モデルのテクスチャを動的に差し替える
/// @param filename 差し替え用テクスチャの画像パス
/// @param index 差し替えるテクスチャのインデックス番号
void Model::SetTexture(std::string filename, int index)
{
	if (m_ChangeTextureHandle != -1)
	{
		DeleteGraph(m_ChangeTextureHandle);
	}

	m_ChangeTextureHandle = Master::m_ResourceManager->LoadGraphics(filename);

	MV1SetTextureGraphHandle(m_Handle, index, m_ChangeTextureHandle, FALSE);
}