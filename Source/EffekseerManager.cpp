#include "EffekseerManager.h"

EffekseerManager::EffekseerManager()
{
}

EffekseerManager::~EffekseerManager()
{
	End();
}

void EffekseerManager::Init()
{
	// 譛螟ｧ繝代・繝・ぅ繧ｯ繝ｫ謠冗判謨ｰ繧呈欠螳壹＠縺ｦ蛻晄悄蛹・	Effekseer_Init(8000);

	// 繝・ヰ繧､繧ｹ繝ｭ繧ｹ繝医′逋ｺ逕溘＠縺滓凾縺ｫ蛯吶∴縺ｦ繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ繧堤匳骭ｲ
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}

void EffekseerManager::Update()
{
	// 豈弱ヵ繝ｬ繝ｼ繝縺ｮ繧ｨ繝輔ぉ繧ｯ繝域峩譁ｰ蜃ｦ逅・	UpdateEffekseer3D();
}

void EffekseerManager::Draw()
{
	// 豈弱ヵ繝ｬ繝ｼ繝縺ｮ繧ｨ繝輔ぉ繧ｯ繝域緒逕ｻ蜃ｦ逅・	DrawEffekseer3D();
}

void EffekseerManager::End()
{
	// 繝ｭ繝ｼ繝峨＠縺溘お繝輔ぉ繧ｯ繝医ｒ隗｣謾ｾ
	for (auto& effect : mEffects)
	{
		DeleteEffekseerEffect(effect.second);
	}
	mEffects.clear();

	// Effekseer縺ｮ邨ゆｺ・・逅・	Effekseer_End();
}

int EffekseerManager::LoadEffect(const std::string& name, const char* filepath, float magnification)
{
	if (mEffects.find(name) != mEffects.end())
	{
		return mEffects[name];
	}

	int handle = LoadEffekseerEffect(filepath, magnification);
	if (handle != -1)
	{
		mEffects[name] = handle;
	}
	return handle;
}

int EffekseerManager::PlayEffect(const std::string& name, VECTOR pos)
{
	if (mEffects.find(name) == mEffects.end())
	{
		return -1;
	}

	int playingHandle = PlayEffekseer3DEffect(mEffects[name]);
	SetPosPlayingEffekseer3DEffect(playingHandle, pos.x, pos.y, pos.z);
	
	return playingHandle;
}

void EffekseerManager::StopEffect(int playingHandle)
{
	StopEffekseer3DEffect(playingHandle);
}

void EffekseerManager::SetEffectPosition(int playingHandle, VECTOR pos)
{
	SetPosPlayingEffekseer3DEffect(playingHandle, pos.x, pos.y, pos.z);
}

void EffekseerManager::SetEffectRotation(int playingHandle, float x, float y, float z)
{
	SetRotationPlayingEffekseer3DEffect(playingHandle, x, y, z);
}

void EffekseerManager::SetEffectScale(int playingHandle, float x, float y, float z)
{
	SetScalePlayingEffekseer3DEffect(playingHandle, x, y, z);
}

