#include "Model.h"
#include "AttachmentModel.h"
#include "Master.h"

/// @brief Modelクラスのコンストラクタ
/// @param filename 3Dモデルのファイルパス
/// @param initPos 初期配置座標
/// @param isSeparateAnimation アニメーションを分離して読み込むかどうかのフラグ
/// @details 3Dモデルリソースのロードおよびアニメーション制御クラス（ModelAnimationまたはSeparateModelAnimation）の初期化を行う
Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
	: position_(initPos)
	, attachment_(nullptr)
	, mvScale(VGet(1.0f, 1.0f, 1.0f))
	, change_texture_handle_(-1)
	, is_separate_(isSeparateAnimation)
{
	handle_ = Master::resource_manager_->LoadModel(filename.c_str());

	if (isSeparateAnimation)
	{
		separate_animation_ = new SeparateModelAnimation(handle_);
		animation_ = nullptr;
	}
	else
	{
		animation_ = new ModelAnimation(handle_);
		separate_animation_ = nullptr;
	}
}

/// @brief Modelクラスのデストラクタ
/// @details 動的確保したアニメーションオブジェクト、アタッチメントモデル、差分テクスチャ、3Dモデル本体の解放処理を行う
Model::~Model()
{
	if (separate_animation_ != nullptr)
	{
		delete separate_animation_;
		separate_animation_ = nullptr;
	}
	if (animation_ != nullptr)
	{
		delete animation_;
		animation_ = nullptr;
	}

	if (attachment_ != nullptr)
	{
		attachment_->SetDeleteFlag(true);
	}

	if (change_texture_handle_ != -1)
	{
		DeleteGraph(change_texture_handle_);
		change_texture_handle_ = -1;
	}

	if (handle_ != -1)
	{
		MV1DeleteModel(handle_);
		handle_ = -1;
	}
}

/// @brief アニメーションの追加登録を行う（分離アニメーション時のみ有効）
/// @param state アニメーション識別用ステート
/// @param filename アニメーションファイルパス
void Model::AddAnimation(AnimationState state, std::string filename)
{
	if (separate_animation_ != nullptr)
	{
		separate_animation_->AddAnimation(state, filename);
	}
}

/// @brief モデルの状態更新処理を行う
/// @details アニメーションの進行更新およびDxLibのモデル位置・回転パラメータの再設定を行う
void Model::Update()
{
	if (animation_ != nullptr)
	{
		animation_->Update();
	}

	if (separate_animation_ != nullptr)
	{
		separate_animation_->Update();
	}

	MV1SetPosition(handle_, position_);
	MV1SetRotationXYZ(handle_, rotation_);
}

/// @brief 3Dモデルの描画処理を行う
void Model::Draw()
{
	MV1DrawModel(handle_);
}

/// @brief 再生するアニメーションを変更する
/// @param state 移行先のアニメーションステート
void Model::ChangeAnimation(AnimationState state)
{
	if (animation_ != nullptr)
	{
		animation_->ChangeAnimation(state);
	}
	if (separate_animation_ != nullptr)
	{
		separate_animation_->ChangeAnimation(state);
	}
}

/// @brief アニメーションのループ再生フラグを設定する
/// @param loop ループ再生を行う場合はtrue
void Model::SetLoop(bool loop)
{
	if (animation_ != nullptr)
	{
		animation_->SetLoop(loop);
	}
	if (separate_animation_ != nullptr)
	{
		separate_animation_->SetLoop(loop);
	}
}

/// @brief 非ループアニメーション終了後の状態切り替えを設定する
/// @param state アニメーション再生終了時に移行するステート
void Model::SetLoopFinishState(AnimationState state)
{
	if (animation_ != nullptr)
	{
		animation_->SetLoopFinishState(state);
	}
	if (separate_animation_ != nullptr)
	{
		separate_animation_->SetLoopFinishState(state);
	}
}

/// @brief アニメーションブレンド（滑らかな切り替え）の有効/無効を設定する
/// @param isBlend ブレンドを行う場合はtrue
void Model::SetAnimationBlend(bool isBlend)
{
	if (animation_ != nullptr)
	{
		animation_->SetAnimationBlend(isBlend);
	}
	if (separate_animation_ != nullptr)
	{
		separate_animation_->SetAnimationBlend(isBlend);
	}
}

/// @brief 現在再生されているアニメーションのステートを取得する
/// @return AnimationState 現在のアニメーションステート
AnimationState Model::GetNowState()
{
	AnimationState ret = AnimationState::ANIMATION_MAX;

	if (animation_ != nullptr)
	{
		ret = animation_->GetNowState();
	}
	if (separate_animation_ != nullptr)
	{
		ret = separate_animation_->GetNowState();
	}

	return ret;
}

/// @brief アニメーションが最後まで再生完了したか判別する
/// @return bool 再生完了した場合はtrue
bool Model::IsAnimationLoopFinish()
{
	bool ret = false;

	if (animation_ != nullptr)
	{
		ret = animation_->IsLoopFinish();
	}
	if (separate_animation_ != nullptr)
	{
		ret = separate_animation_->IsLoopFinish();
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
	if (attachment_ != nullptr)
	{
		attachment_->SetDeleteFlag(true);
		attachment_ = nullptr;
	}
	int frameIndex = MV1SearchFrame(handle_, attachFrameName.c_str());
	if (frameIndex != -1)
	{
		attachment_ = new AttachmentModel(filename, handle_, frameIndex, offsetPos, offsetRot);
	}
}

/// @brief アタッチメント非依存で、指定ボーン（フレーム）のワールド座標を取得する
/// @param attachFrameName 取得したいボーン（フレーム）名
/// @return VECTOR ボーンのワールド座標（見つからない場合は初期高さ座標）
VECTOR Model::GetAttachmentPosition_None(std::string attachFrameName)
{
	int frameIndex = MV1SearchFrame(handle_, attachFrameName.c_str());
	if (frameIndex != -1)
	{
		return MV1GetFramePosition(handle_, frameIndex);
	}
	return VGet(0.0f, 30.0f, 0.0f);
}

/// @brief 登録済みアタッチメントの現在の先端・オフセット適用後のワールド座標を取得する
/// @return VECTOR 計算されたアタッチメントのワールド座標
VECTOR Model::GetAttachmentPosition()
{
	if (attachment_ != nullptr)
	{
		VECTOR vec = VGet(0.0f, -50.0f, 0.0f);

		MATRIX matrix = MV1GetFrameLocalWorldMatrix(attachment_->GetHandle(), 0);

		vec = VTransform(vec, matrix);

		return vec;
	}

	return VGet(0.0f, 0.0f, 0.0f);
}

/// @brief モデルの拡大スケールを設定する
/// @param scale 各軸の倍率ベクトル
void Model::SetScale(VECTOR scale)
{
	MV1SetScale(handle_, scale);
}

/// @brief モデルのテクスチャを動的に差し替える
/// @param filename 差し替え用テクスチャの画像パス
/// @param index 差し替えるテクスチャのインデックス番号
void Model::SetTexture(std::string filename, int index)
{
	if (change_texture_handle_ != -1)
	{
		DeleteGraph(change_texture_handle_);
	}

	change_texture_handle_ = Master::resource_manager_->LoadGraphics(filename);

	MV1SetTextureGraphHandle(handle_, index, change_texture_handle_, FALSE);
}