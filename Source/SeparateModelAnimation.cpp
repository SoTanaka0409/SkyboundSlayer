#include "Master.h"
#include "DxLib.h"
#include "SeparateModelAnimation.h"

/// @param modelHandle (対象の3Dモデルハンドル)
/// @details アニメーション管理用変数の初期化、および不正アクセスを防ぐための無効値(-1)セット
SeparateModelAnimation::SeparateModelAnimation(int modelHandle)
    : m_ModelHandle(modelHandle)
    , m_AnimationTime(0.0f)
    , m_AnimationIndex(-1)
    , m_OldAnimationTime(0.0f)
    , m_OldAnimationIndex(-1)
    , m_AnimBlendRate(1.0f)
    , m_State(AnimationState::ANIMATION_MAX)
    , m_Loop(true)
    , m_LoopFinishState(AnimationState::ANIMATION_MAX)
    , m_LoopFinish(false)
    , m_AnimationInfoList()
    , m_AnimationCount(0.5f)
{
}

/// @details 動的確保したアニメーション情報およびDxLibの追加モデルハンドルの完全破棄（メモリリーク回避）
SeparateModelAnimation::~SeparateModelAnimation()
{
    if (!m_AnimationInfoList.empty())
    {
        for (auto itr = m_AnimationInfoList.begin(); itr != m_AnimationInfoList.end(); )
        {
            auto temp = *itr;
            itr = m_AnimationInfoList.erase(itr);
            if (temp->m_AnimationHandle != -1) {
                MV1DeleteModel(temp->m_AnimationHandle);
                temp->m_AnimationHandle = -1;
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
    if (m_AnimBlendRate < 1.0f)
    {
        m_AnimBlendRate += 0.1f;
        if (m_AnimBlendRate > 1.0f)
        {
            m_AnimBlendRate = 1.0f;
        }
    }

    float fAnimTotalTime = 0.0f;
    if (m_AnimationIndex != -1)
    {
        fAnimTotalTime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_AnimationIndex);
        m_AnimationTime += m_AnimationCount;

        // アニメーションが終端に達した場合のループ処理、または指定された次状態への自動遷移
        if (m_AnimationTime > fAnimTotalTime)
        {
            if (!m_Loop)
            {
                // 次の遷移先が未指定の場合は現在位置でアニメーションを停止させる
                if (m_LoopFinishState == ANIMATION_MAX)
                {
                    m_LoopFinish = true;
                    return;
                }

                // 待機モーション等へ自動遷移させるため、即時切り替え（ブレンド無効）で適用する
                ChangeAnimation(m_LoopFinishState);
                SetAnimationBlend(false);
                fAnimTotalTime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_AnimationIndex);
            }

            m_AnimationTime = 0.0f;
        }

        MV1SetAttachAnimTime(m_ModelHandle, m_AnimationIndex, m_AnimationTime);
        MV1SetAttachAnimBlendRate(m_ModelHandle, m_AnimationIndex, m_AnimBlendRate);
    }

    // ブレンド中の破綻を防ぐため、フェードアウトしていく旧モーション側も並行して時間を進める
    if (m_OldAnimationIndex != -1)
    {
        fAnimTotalTime = MV1GetAttachAnimTotalTime(m_ModelHandle, m_OldAnimationIndex);
        m_OldAnimationTime += m_AnimationCount;

        if (m_OldAnimationTime > fAnimTotalTime)
        {
            m_OldAnimationTime = 0.0f;
        }

        MV1SetAttachAnimTime(m_ModelHandle, m_OldAnimationIndex, m_OldAnimationTime);
        MV1SetAttachAnimBlendRate(m_ModelHandle, m_OldAnimationIndex, 1.0f - m_AnimBlendRate);
    }
}

/// @param state(遷移先状態), index(アタッチするアニメーション番号)
/// @details 旧モーション状態を退避しつつ新モーションをアタッチし、次フレームからのブレンド遷移を準備する
void SeparateModelAnimation::ChangeAnimation(AnimationState state, int index)
{
    // 重複切り替えによるモーションの初期化（巻き戻り）を防ぐための早期リターン
    if (m_State == state)
    {
        return;
    }

    m_State = state;
    m_Loop = true;
    m_LoopFinishState = AnimationState::ANIMATION_MAX;
    m_LoopFinish = false;

    // DxLibのアタッチ上限超過を防ぐため、既に用済みの「1つ前の旧モーション」は確実にデタッチする
    if (m_OldAnimationIndex != -1)
    {
        MV1DetachAnim(m_ModelHandle, m_OldAnimationIndex);
        m_OldAnimationIndex = -1;
    }

    // ブレンド用に現在のモーションを「旧モーション」として退避させる
    m_OldAnimationIndex = m_AnimationIndex;
    m_OldAnimationTime = m_AnimationTime;

    m_AnimationIndex = MV1AttachAnim(m_ModelHandle, index, GetAnimationHandle(state), TRUE);
    m_AnimationTime = 0.0f;

    // 初回設定時など旧モーションが存在しない場合は、ブレンド不要のため即時1.0fをセットする
    m_AnimBlendRate = (m_OldAnimationIndex == -1 ? 1.0f : 0.0f);
}

/// @param isBlend (ブレンド有効化フラグ)
/// @details false時は旧モーションをデタッチして破棄し、ブレンドなしの即時切り替え状態を強制する
void SeparateModelAnimation::SetAnimationBlend(bool isBlend)
{
    if (isBlend)
    {
        m_AnimBlendRate = (m_OldAnimationIndex == -1 ? 1.0f : 0.0f);
    }
    else
    {
        m_AnimBlendRate = 1.0f;

        // ブレンドを行わないため、不要になった旧モーションは即座にメモリから切り離す
        if (m_OldAnimationIndex != -1)
        {
            MV1DetachAnim(m_ModelHandle, m_OldAnimationIndex);
            m_OldAnimationIndex = -1;
        }
    }
}

/// @param state(紐づける状態), filename(ファイルパス)
/// @details 外部ファイルからモーションをロードし、NEUTRAL指定時は初期モーションとして自動適用する
void SeparateModelAnimation::AddAnimation(AnimationState state, std::string filename)
{
    int handle = Master::m_ResourceManager->LoadModel(filename.c_str());

    if (handle == -1)
    {
        return;
    }

    AnimationInfo* pInfo = new AnimationInfo();
    pInfo->m_State = state;
    pInfo->m_AnimationHandle = handle;
    m_AnimationInfoList.push_back(pInfo);

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
    if (m_AnimationInfoList.empty())
    {
        return -1;
    }

    for (auto itr = m_AnimationInfoList.begin(); itr != m_AnimationInfoList.end(); itr++)
    {
        auto temp = *itr;

        if (temp->m_State == state)
        {
            return temp->m_AnimationHandle;
        }
    }

    return -1;
}