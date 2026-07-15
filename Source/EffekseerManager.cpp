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
	// 隴崢陞滂ｽｧ郢昜ｻ｣繝ｻ郢昴・縺・ｹｧ・ｯ郢晢ｽｫ隰蜀怜愛隰ｨ・ｰ郢ｧ蜻域ｬ陞ｳ螢ｹ・邵ｺ・ｦ陋ｻ譎・ｄ陋ｹ繝ｻ	Effekseer_Init(8000);

	// 郢昴・繝ｰ郢ｧ・､郢ｧ・ｹ郢晢ｽｭ郢ｧ・ｹ郢晏現窶ｲ騾具ｽｺ騾墓ｺ假ｼ邵ｺ貊灘・邵ｺ・ｫ陋ｯ蜷ｶ竏ｴ邵ｺ・ｦ郢ｧ・ｳ郢晢ｽｼ郢晢ｽｫ郢晁・繝｣郢ｧ・ｯ郢ｧ蝣､蛹ｳ鬪ｭ・ｲ
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}

void EffekseerManager::Update()
{
	// 雎亥ｼｱ繝ｵ郢晢ｽｬ郢晢ｽｼ郢晢｣ｰ邵ｺ・ｮ郢ｧ・ｨ郢晁ｼ斐♂郢ｧ・ｯ郢晏沺蟲ｩ隴・ｽｰ陷・ｽｦ騾・・	UpdateEffekseer3D();
}

void EffekseerManager::Draw()
{
	// 雎亥ｼｱ繝ｵ郢晢ｽｬ郢晢ｽｼ郢晢｣ｰ邵ｺ・ｮ郢ｧ・ｨ郢晁ｼ斐♂郢ｧ・ｯ郢晏沺邱帝包ｽｻ陷・ｽｦ騾・・	DrawEffekseer3D();
}

void EffekseerManager::End()
{
	static bool isEnded = false;
	if (isEnded) return;
	isEnded = true;

	// 再生中のエフェクトを全て削除
	for (auto& effect : mEffects)
	{
		DeleteEffekseerEffect(effect.second);
	}
	mEffects.clear();

	// Effekseerの終了処理
	Effkseer_End();
}

int EffekseerManager::LoadEffect(const std::string& name, const char* filepath, float magnification)
{
	if (mEffects.find(name) != mEffects.end())
	{
		return mEffects[name];
	}

	// Effekseer effects do not support DxLib's asynchronous loading, so we must load them synchronously
	int oldFlag = GetUseASyncLoadFlag();
	SetUseASyncLoadFlag(FALSE);
	int handle = LoadEffekseerEffect(filepath, magnification);
	SetUseASyncLoadFlag(oldFlag);
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
	if (playingHandle != -1) {
		StopEffekseer3DEffect(playingHandle);
	}
}

void EffekseerManager::SetEffectPosition(int playingHandle, VECTOR pos)
{
	if (playingHandle != -1) {
		SetPosPlayingEffekseer3DEffect(playingHandle, pos.x, pos.y, pos.z);
	}
}

void EffekseerManager::SetEffectRotation(int playingHandle, float x, float y, float z)
{
	if (playingHandle != -1) {
		SetRotationPlayingEffekseer3DEffect(playingHandle, x, y, z);
	}
}

void EffekseerManager::SetEffectScale(int playingHandle, float x, float y, float z)
{
	if (playingHandle != -1) {
		SetScalePlayingEffekseer3DEffect(playingHandle, x, y, z);
	}
}

bool EffekseerManager::IsPlaying(int playingHandle)
{
	if (playingHandle == -1) return false;
	return IsEffekseer3DEffectPlaying(playingHandle) != 0;
}

void EffekseerManager::SetEffectSpeed(int playingHandle, float speed)
{
	if (playingHandle != -1) {
		SetSpeedPlayingEffekseer3DEffect(playingHandle, speed);
	}
}

