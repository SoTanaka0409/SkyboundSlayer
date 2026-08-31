#include "Master.h"
#include "DxLib.h"
#include "SeparateModelAnimation.h"

/// @param modelHandle (対象の3Dモデルハンドル)
/// @details アニメーション管理用変数の初期化、および不正アクセスを防ぐための無効値(-1)セット
SeparateModelAnimation::SeparateModelAnimation(int modelHandle)
    : model_handle_(modelHandle)
    , animation_time_(0.0f)
    , animation_index_(-1)
    , old_animation_time_(0.0f)
    , old_animation_index_(-1)
    , anim_blend_rate_(1.0f)
    , state_(AnimationState::ANIMATION_MAX)
    , loop_(true)
    , loop_finish_state_(AnimationState::ANIMATION_MAX)
    , loop_finish_(false)
    , animation_info_list_()
    , animation_count_(0.5f)
{
}

/// @details 動的確保したアニメーション情報およびDxLibの追加モデルハンドルの完全破棄（メモリリーク回避）
SeparateModelAnimation::~SeparateModelAnimation()
{
    if (!animation_info_list_.empty())
    {
        for (auto itr = animation_info_list_.begin(); itr != animation_info_list_.end(); )
        {
            auto temp = *itr;
            itr = animation_info_list_.erase(itr);
            if (temp->animation_handle_ != -1) {
                MV1DeleteModel(temp->animation_handle_);
                temp->animation_handle_ = -1;
            }

            delete temp;
            temp = nullptr;
        }
    }
}

/// @details 再生時間の進行とループ制御、および旧モーションからの滑らかな遷移（ブレンド）計算の適用
void SeparateModelAnimation::Update()
{
    // モーション切り替え時の不自然なカクつきを防ぐため、徐々にブレンド率を上げる
    if (anim_blend_rate_ < 1.0f)
    {
        anim_blend_rate_ += 0.1f;
        if (anim_blend_rate_ > 1.0f)
        {
            anim_blend_rate_ = 1.0f;
        }
    }

    float fAnimTotalTime = 0.0f;
    if (animation_index_ != -1)
    {
        fAnimTotalTime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);
        animation_time_ += animation_count_;

        // アニメーションが終端に達した場合のループ処理、または指定された次状態への自動遷移
        if (animation_time_ > fAnimTotalTime)
        {
            if (!loop_)
            {
                // 次の遷移先が未指定の場合は現在位置でアニメーションを停止させる
                if (loop_finish_state_ == ANIMATION_MAX)
                {
                    loop_finish_ = true;
                    return;
                }

                // 待機モーション等へ自動遷移させるため、即時切り替え（ブレンド無効）で適用する
                ChangeAnimation(loop_finish_state_);
                SetAnimationBlend(false);
                fAnimTotalTime = MV1GetAttachAnimTotalTime(model_handle_, animation_index_);
            }

            animation_time_ = 0.0f;
        }

        MV1SetAttachAnimTime(model_handle_, animation_index_, animation_time_);
        MV1SetAttachAnimBlendRate(model_handle_, animation_index_, anim_blend_rate_);
    }

    // ブレンド中の破綻を防ぐため、フェードアウトしていく旧モーション側も並行して時間を進める
    if (old_animation_index_ != -1)
    {
        fAnimTotalTime = MV1GetAttachAnimTotalTime(model_handle_, old_animation_index_);
        old_animation_time_ += animation_count_;

        if (old_animation_time_ > fAnimTotalTime)
        {
            old_animation_time_ = 0.0f;
        }

        MV1SetAttachAnimTime(model_handle_, old_animation_index_, old_animation_time_);
        MV1SetAttachAnimBlendRate(model_handle_, old_animation_index_, 1.0f - anim_blend_rate_);
    }
}

/// @param state(遷移先状態), index(アタッチするアニメーション番号)
/// @details 旧モーション状態を退避しつつ新モーションをアタッチし、次フレームからのブレンド遷移を準備する
void SeparateModelAnimation::ChangeAnimation(AnimationState state, int index)
{
    // 重複切り替えによるモーションの初期化（巻き戻り）を防ぐための早期リターン
    if (state_ == state)
    {
        return;
    }

    state_ = state;
    loop_ = true;
    loop_finish_state_ = AnimationState::ANIMATION_MAX;
    loop_finish_ = false;

    // DxLibのアタッチ上限超過を防ぐため、既に用済みの「1つ前の旧モーション」は確実にデタッチする
    if (old_animation_index_ != -1)
    {
        MV1DetachAnim(model_handle_, old_animation_index_);
        old_animation_index_ = -1;
    }

    // ブレンド用に現在のモーションを「旧モーション」として退避させる
    old_animation_index_ = animation_index_;
    old_animation_time_ = animation_time_;

    animation_index_ = MV1AttachAnim(model_handle_, index, GetAnimationHandle(state), TRUE);
    animation_time_ = 0.0f;

    // 初回設定時など旧モーションが存在しない場合は、ブレンド不要のため即時1.0fをセットする
    anim_blend_rate_ = (old_animation_index_ == -1 ? 1.0f : 0.0f);
}

/// @param isBlend (ブレンド有効化フラグ)
/// @details false時は旧モーションをデタッチして破棄し、ブレンドなしの即時切り替え状態を強制する
void SeparateModelAnimation::SetAnimationBlend(bool isBlend)
{
    if (isBlend)
    {
        anim_blend_rate_ = (old_animation_index_ == -1 ? 1.0f : 0.0f);
    }
    else
    {
        anim_blend_rate_ = 1.0f;

        // ブレンドを行わないため、不要になった旧モーションは即座にメモリから切り離す
        if (old_animation_index_ != -1)
        {
            MV1DetachAnim(model_handle_, old_animation_index_);
            old_animation_index_ = -1;
        }
    }
}

/// @param state(紐づける状態), filename(ファイルパス)
/// @details 外部ファイルからモーションをロードし、NEUTRAL指定時は初期モーションとして自動適用する
void SeparateModelAnimation::AddAnimation(AnimationState state, std::string filename)
{
    int handle = Master::resource_manager_->LoadModel(filename.c_str());

    if (handle == -1)
    {
        return;
    }

    AnimationInfo* pInfo = new AnimationInfo();
    pInfo->state_ = state;
    pInfo->animation_handle_ = handle;
    animation_info_list_.push_back(pInfo);

    // キャラクター生成直後にTポーズ等の無効状態が描画されるのを防ぐため、待機状態をデフォルト設定する
    if (state == AnimationState::ANIMATION_NEUTRAL)
    {
        ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
    }
}

/// @param state (検索する状態)
/// @return 対応するモデルハンドル(-1で未登録)
/// @details なし（状態とハンドルの紐付けリストからの単なる検索処理）
int SeparateModelAnimation::GetAnimationHandle(AnimationState state)
{
    if (animation_info_list_.empty())
    {
        return -1;
    }

    for (auto itr = animation_info_list_.begin(); itr != animation_info_list_.end(); itr++)
    {
        auto temp = *itr;

        if (temp->state_ == state)
        {
            return temp->animation_handle_;
        }
    }

    return -1;
}