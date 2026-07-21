#pragma once
#include "Effect.h"
#include <vector>

// エフェクトの動的生成・破棄によるメモリ確保負荷（断片化・カクつき）を回避するため、固定数インスタンスを再利用するシングルトン
class EffectPool {
private:
    static const int POOL_SIZE = 50; // 同時表示可能なエフェクト上限数（これを超える演出は古いものから上書き、または無視する）
    Effect* pool_[POOL_SIZE];        // 事前に確保したエフェクトインスタンスのメモリ領域

    static EffectPool* sInstance;

public:
/// @return シングルトンインスタンス
    static EffectPool* GetInstance();

/// @details POOL_SIZE分のエフェクトインスタンスを事前に動的確保し、待機状態にする
    EffectPool();

/// @details 確保した全エフェクトインスタンスを破棄し、メモリリークを防ぐ
    ~EffectPool();

/// @details 使用中の全エフェクトの生存時間をカウントし、寿命が尽きたものを非アクティブ（待機状態）へ戻す
    void Update();

/// @details アクティブな全てのエフェクトインスタンスを描画バッファへ順次登録する
    void Draw();

/// @param initPos, filename, Changecolor, Size, VisibleTime
/// @details プール内の未使用エフェクトを検索し、指定パラメータで初期化してアクティブ状態へ切り替える
    void Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime);
};