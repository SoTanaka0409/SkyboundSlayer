#include "effect_r.h"
#include <cmath>


EffectR::EffectR(float X, float Y, float Z)
    :mvPosition(VGet(X, Y, Z))
{

}
/*====================================================
  -1.0～+1.0 の乱数
====================================================*/
static float RandSigned()
{
    return (float)(GetRand(20000) / 10000.0 - 1.0);
}

/*====================================================
  テクスチャ不要なので読み込みは空
====================================================*/
bool EffectR::Load()
{
    return true;
}

/*====================================================
  雷撃ボルト生成
====================================================*/
void EffectR::Trigger(const VECTOR& pos)
{
    Bolt b{};

    // 雷の起点（プレイヤー頭上などにオフセット）
    b.base = mvPosition;//VGet(rand() % 10000 - 5000, 3000.0f, rand() % 10000 - 5000);

    // ライフタイム設定
    b.maxLife = mDefaultLife;
    b.life = mDefaultLife;

    // ボルト形状
    b.height = mHeight;
    b.segs = mSegments;

    // 形状揺らぎ
    b.jitter = mJitter;
    b.jitterHz = mJitterHz;

    b.timeAcc = 0.0f;   // 時間計測用
    b.flash = 1.0f;     // 光の強さ初期値

    // セグメント数+1 の頂点配列を確保
    b.points.resize(b.segs + 1);

    // ボルトの頂点位置を初期生成
    RebuildBoltPoints(b);

    // リストに追加
    mBolts.push_back(b);
}

/*====================================================
  ボルト更新（位置揺れ + ライフ減少）
====================================================*/
void EffectR::UpdateFollow(const VECTOR&)
{
    const float dt = 1.0f / 60.0f;

    for (auto& b : mBolts)
    {
        b.life -= dt;
        if (b.life <= 0.0f) continue;

        // 光の減衰
        b.flash -= dt * 6.0f;
        if (b.flash < 0.0f) b.flash = 0.0f;

        // jitterHzに応じて頂点を更新
        b.timeAcc += dt;
        const float interval = 1.0f / b.jitterHz;

        while (b.timeAcc >= interval)
        {
            b.timeAcc -= interval;
            RebuildBoltPoints(b);
        }
    }

    // ライフ切れボルトを削除
    for (int i = (int)mBolts.size() - 1; i >= 0; --i)
    {
        if (mBolts[i].life <= 0.0f)
        {
            mBolts.erase(mBolts.begin() + i);
        }
    }
}

/*====================================================
  ボルト頂点位置を再計算
====================================================*/
void EffectR::RebuildBoltPoints(Bolt& b)
{
    for (int i = 0; i <= b.segs; ++i)
    {
        float t = (float)i / (float)b.segs;
        float y = b.base.y + t * b.height;

        // 上に行くほど揺れを強く
        float strength = b.jitter * (0.2f + 0.8f * t);

        float ox = RandSigned() * strength;
        float oz = RandSigned() * strength;

        // 始点・終点は少し揺れを抑える
        if (i == 0) { ox *= 0.08f; oz *= 0.08f; }
        if (i == b.segs) { ox *= 0.12f; oz *= 0.12f; }

        b.points[i] = VGet(b.base.x + ox, y, b.base.z + oz);
    }
}

/*====================================================
  描画
====================================================*/
void EffectR::Draw() const
{
    if (mBolts.empty()) return;

    // ===== 画面フラッシュ =====
    if (mScreenFlashStrength > 0.0f)
    {
        float flashMax = 0.0f;
        for (const auto& b : mBolts)
        {
            if (b.flash > flashMax) flashMax = b.flash;
        }

        if (flashMax > 0.0f)
        {
            int screenW = 0, screenH = 0;
            GetDrawScreenSize(&screenW, &screenH);

            int a = (int)(255.0f * flashMax * mScreenFlashStrength);
            if (a > 0)
            {
                SetDrawBlendMode(DX_BLENDMODE_ADD, a);
                DrawBox(0, 0, screenW, screenH, GetColor(255, 255, 255), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
        }
    }

    // カラー設定
    const int colGlow = GetColor(mColorR, mColorG, mColorB);
    const int colCore = GetColor(255, 255, 255);

    // 各ボルトを描画
    for (const auto& b : mBolts)
    {
        if ((int)b.points.size() < 2) continue;

        // ライフに応じてアルファを変化
        float life01 = Clamp01(b.life / b.maxLife);
        int glowAlpha = (int)(mGlowAdd * (0.65f + 0.35f * life01));
        int coreAlpha = (int)(mCoreAdd * (0.60f + 0.40f * life01));

        const int n = (int)b.points.size();
        for (int i = 1; i < n; ++i)
        {
            VECTOR p0 = ConvWorldPosToScreenPos(b.points[i - 1]);
            VECTOR p1 = ConvWorldPosToScreenPos(b.points[i]);

            // ===== 外側の光（グロー） =====
            SetDrawBlendMode(DX_BLENDMODE_ADD, glowAlpha);
            for (int k = -mGlowThickness; k <= mGlowThickness; ++k)
            {
                DrawLine((int)p0.x + k, (int)p0.y, (int)p1.x + k, (int)p1.y, colGlow);
                DrawLine((int)p0.x, (int)p0.y + k, (int)p1.x, (int)p1.y + k, colGlow);
            }

            // ===== 中心線（コア） =====
            SetDrawBlendMode(DX_BLENDMODE_ADD, coreAlpha);
            for (int k = -mCoreThickness; k <= mCoreThickness; ++k)
            {
                DrawLine((int)p0.x + k, (int)p0.y, (int)p1.x + k, (int)p1.y, colCore);
                DrawLine((int)p0.x, (int)p0.y + k, (int)p1.x, (int)p1.y + k, colCore);
            }
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
