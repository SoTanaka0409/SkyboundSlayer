#pragma once
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include <string>
#include <unordered_map>

class EffekseerManager
{
public:
	static EffekseerManager* GetInstance()
	{
		static EffekseerManager instance;
		return &instance;
	}

	// 蛻晄悄蛹・(DxLib_Init縺ｮ蠕後↓蜻ｼ縺ｶ)
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Init();

	// 譖ｴ譁ｰ (豈弱ヵ繝ｬ繝ｼ繝蜻ｼ縺ｶ)
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Update();

	// 謠冗判 (3D謠冗判縺ｮ蠕後↓蜻ｼ縺ｶ)
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void Draw();

	// 邨ゆｺ・・逅・(DxLib_End縺ｮ蜑阪↓蜻ｼ縺ｶ)
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void End();

	// 繧ｨ繝輔ぉ繧ｯ繝医・隱ｭ縺ｿ霎ｼ縺ｿ
	// name: 繝励Ο繧ｰ繝ｩ繝蜀・〒菴ｿ縺・匳骭ｲ蜷・
	// filepath: efk繝輔ぃ繧､繝ｫ縺ｮ繝代せ
	// magnification: 諡｡螟ｧ邇・(繝・ヵ繧ｩ繝ｫ繝・.0f)
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int LoadEffect(const std::string& name, const char* filepath, float magnification = 1.0f);

	// 繧ｨ繝輔ぉ繧ｯ繝医・蜀咲函
	// name: 隱ｭ縺ｿ霎ｼ繧薙□譎ゅ・逋ｻ骭ｲ蜷・
	// pos: 蜀咲函縺吶ｋ3D蠎ｧ讓・
	// 謌ｻ繧雁､: 蜀咲函荳ｭ縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν (蛛懈ｭ｢譎ゅ↑縺ｩ縺ｫ菴ｿ逕ｨ)
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	int PlayEffect(const std::string& name, VECTOR pos);

	// 蜀咲函荳ｭ縺ｮ繧ｨ繝輔ぉ繧ｯ繝医ｒ蛛懈ｭ｢
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void StopEffect(int playingHandle);

	// 再生中か確認
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	bool IsPlaying(int playingHandle);

	// 再生速度を設定
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetEffectSpeed(int playingHandle, float speed);

	// 迚ｹ螳壹・繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν縺ｮ菴咲ｽｮ繧呈峩譁ｰ縺吶ｋ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetEffectPosition(int playingHandle, VECTOR pos);

	// 迚ｹ螳壹・繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν縺ｮ蝗櫁ｻ｢繧呈峩譁ｰ縺吶ｋ (繝ｩ繧ｸ繧｢繝ｳ)
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetEffectRotation(int playingHandle, float x, float y, float z);

	// 迚ｹ螳壹・繧ｨ繝輔ぉ繧ｯ繝医ワ繝ｳ繝峨Ν縺ｮ繧ｹ繧ｱ繝ｼ繝ｫ繧呈峩譁ｰ縺吶ｋ
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	void SetEffectScale(int playingHandle, float x, float y, float z);

private:
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	EffekseerManager();
    // [入力] 引数参照 [出力] 戻り値参照 [副作用] 状態変更
	~EffekseerManager();

	std::unordered_map<std::string, int> effects_;
};
