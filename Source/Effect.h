#pragma once
#include "DxLib.h"
#include "Object3D.h"
#include <string>

/// @brief パーティクルベースの簡易エフェクト生成および描画管理を行うクラス
class Effect
{
public:
	static const int PARTICLE_NUM = 256; ///< パーティクルの最大数

	/// @brief 粒ひとつひとつの個別データ構造体
	struct ParticleInfo
	{
		VECTOR pos;         ///< パーティクルのワールド座標
		VECTOR dir;         ///< 進行方向ベクトル
		float speed;        ///< 移動速度
		float size;         ///< 描画サイズ（半径・スケール）
		float alpha;        ///< 不透明度（アルファ値）
		float visibleTime;  ///< 生存時間・表示制限時間
	};

	/// @brief 複数のパーティクルをまとめたひとつのエフェクトとしての全体データ構造体
	struct EffectInfo
	{
		COLOR_U8 color;                       ///< エフェクト全体のカラー（RGB・アルファ）
		ParticleInfo particle[PARTICLE_NUM];  ///< パーティクルデータの配列
	};

public:
	Effect();
	~Effect();

	/// @brief エフェクトの生成および再生を開始する
	/// @param initPos 発生の起点となるワールド座標
	/// @param filename 使用する画像テクスチャのファイルパス
	/// @param Changecolor エフェクトの指定カラー
	/// @param Size パーティクルの基本サイズスケール
	/// @param VisibleTime 表示の継続時間係数
	void Play(VECTOR initPos, std::string filename, COLOR_U8 Changecolor, float Size, float VisibleTime);

	/// @brief エフェクトが現在アクティブ（再生中）かどうかを取得する
	/// @return bool 再生中ならtrue
	bool IsActive() const { return active_; }

	/// @brief パーティクルの移動・減衰・タイマー等の更新処理を行う
	void Update();

	/// @brief アクティブなパーティクル群の3D描画処理を行う
	void Draw();

private:
	bool active_;             ///< エフェクトが有効・再生中かどうかを示すフラグ
	int graph_handle_;        ///< パーティクル描画用画像グラフィックハンドル
	EffectInfo* effect_;      ///< 動的確保されたエフェクトデータ構造体へのポインタ

	const int SPEED_RAND_MAX = 550;        ///< 移動速度計算用の最大乱数値
	const int SPEED_RAND_MIN = 200;        ///< 移動速度計算用の最小乱数値

	const int SIZE_RAND_MAX = 1600;        ///< パーティクルサイズ計算用の最大乱数値
	const int SIZE_RAND_MIN = 800;         ///< パーティクルサイズ計算用の最小乱数値

	const int VISIBLE_TIME_RAND_MAX = 30;  ///< 表示持続時間計算用の最大乱数値
	const int VISIBLE_TIME_RAND_MIN = 5;   ///< 表示持続時間計算用の最小乱数値
};