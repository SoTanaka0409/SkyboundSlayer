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

	// 初期化 (DxLib_Initの後に呼ぶ)
	void Init();

	// 更新 (毎フレーム呼ぶ)
	void Update();

	// 描画 (3D描画の後に呼ぶ)
	void Draw();

	// 終了処理 (DxLib_Endの前に呼ぶ)
	void End();

	// エフェクトの読み込み
	// name: プログラム内で使う登録名
	// filepath: efkファイルのパス
	// magnification: 拡大率 (デフォルト1.0f)
	int LoadEffect(const std::string& name, const char* filepath, float magnification = 1.0f);

	// エフェクトの再生
	// name: 読み込んだ時の登録名
	// pos: 再生する3D座標
	// 戻り値: 再生中のエフェクトハンドル (停止時などに使用)
	int PlayEffect(const std::string& name, VECTOR pos);

	// 再生中のエフェクトを停止
	void StopEffect(int playingHandle);

	// 特定のエフェクトハンドルの位置を更新する
	void SetEffectPosition(int playingHandle, VECTOR pos);

	// 特定のエフェクトハンドルの回転を更新する (ラジアン)
	void SetEffectRotation(int playingHandle, float x, float y, float z);

	// 特定のエフェクトハンドルのスケールを更新する
	void SetEffectScale(int playingHandle, float x, float y, float z);

private:
	EffekseerManager();
	~EffekseerManager();

	std::unordered_map<std::string, int> mEffects;
};
