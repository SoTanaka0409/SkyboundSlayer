#include "EffekseerManager.h"

// 副作用：エフェクトマネージャーの初期生成
EffekseerManager::EffekseerManager()
{
}

EffekseerManager::~EffekseerManager()
{
	// メモリ管理：解放時に呼び出されることで、VRAM上のEffekseerコンテキスト全体のクリーンアップを保証する
	End();
}

// 入力：なし
// 出力：なし
// 副作用：Effekseerシステムの起動、およびDxLibグラフィックデバイスロスト時の復帰フック関数の登録
void EffekseerManager::Init()
{
	// パフォーマンス理由：雷雨や爆発が重なるゲーム後半（クライシスモード）でも、描画個数上限によってエフェクトが途中で途切れないよう最大描画数を十分に確保
	Effekseer_Init(8000);

	// バグ回避：フルスクリーン切り替え時やウィンドウ最小化等によってDirect3Dデバイスがロストした際、ロード済みパーティクルのVRAM復旧を自動で行わせる
	SetUseASyncLoadFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

// 入力：なし
// 出力：なし
// 副作用：再生中エフェクトの時間経過、アニメーションフレーム、および物理挙動の進行更新
void EffekseerManager::Update()
{
	// 外部仕様依存：DxLib標準の3D描画システムとEffekseerの頂点更新スレッドを同期させるため、毎フレーム描画直前に必ず呼び出す
	UpdateEffekseer3D();
}

// 入力：なし
// 出力：なし
// 副作用：DxLibの現在の3Dカメラマトリクス（Cameraオブジェクト）に基づいた、各エフェクトポリゴンのバックバッファへの描画
void EffekseerManager::Draw()
{
	// 描画順：3Dモデルのレンダリングがすべて完了した後に、深度バッファを保護しつつ半透明ブレンドでエフェクトを重ねる
	DrawEffekseer3D();
}

// 入力：なし
// 出力：なし
// 副作用：再生中の個別インスタンスの一括削除、登録エフェクトリソースのクリア、およびライブラリ終了処理
void EffekseerManager::End()
{
	// バグ回避：Finalizeの二重呼び出しや、デストラクタ経由での多重解放によるDxLib内部コンテキスト破綻（クラッシュバグ）を完全に防ぐセーフティ
	static bool isEnded = false;
	if (isEnded) return;
	isEnded = true;

	for (auto& effect : effects_)
	{
		DeleteEffekseerEffect(effect.second);
	}
	effects_.clear();

	Effkseer_End();
}

// 入力：name = 管理用識別キー, filepath = アセットパス, magnification = 初期表示スケーリング倍率
// 出力：ロードされたエフェクトハンドル（失敗時は -1）
// 副作用：一時的な非同期ロード解除、およびロード済み配列への登録
int EffekseerManager::LoadEffect(const std::string& name, const char* filepath, float magnification)
{
	if (effects_.find(name) != effects_.end())
	{
	return_handle:
		return effects_[name];
	}

	// 技術スタック制約：EffekseerのリソースロードAPIはDxLib側の非同期スレッド（ASync）に対応していないため、読み込み完了までメインスレッドを同期ブロックする
	int oldFlag = GetUseASyncLoadFlag();
	SetUseASyncLoadFlag(FALSE);
	int handle = LoadEffekseerEffect(filepath, magnification);
	SetUseASyncLoadFlag(oldFlag);
	if (handle != -1)
	{
		effects_[name] = handle;
	}
	return handle;
}

// 入力：name = 再生するアセット識別キー, pos = 発生させる3D空間座標（VECTOR）
// 出力：再生中のエフェクトを一意に識別する管理用インスタンスハンドル（失敗時は -1）
// 副作用：3D空間上へのエフェクトインスタンスの発行、および初期位置のバインド
int EffekseerManager::PlayEffect(const std::string& name, VECTOR pos)
{
	if (effects_.find(name) == effects_.end())
	{
		return -1;
	}

	int playingHandle = PlayEffekseer3DEffect(effects_[name]);
	SetPosPlayingEffekseer3DEffect(playingHandle, pos.x, pos.y, pos.z);

	return playingHandle;
}

// 入力：playingHandle = 停止させる再生中インスタンスハンドル
// 出力：なし
// 副作用：対象エフェクトの再生停止（寿命の強制終了処理）
void EffekseerManager::StopEffect(int playingHandle)
{
	if (playingHandle != -1) {
		StopEffekseer3DEffect(playingHandle);
	}
}

// 入力：playingHandle = 対象インスタンスハンドル, pos = 移動先の新3D空間座標
// 出力：なし
// 副作用：再生中のエフェクト位置の更新（追従アクターの移動に同期させる）
void EffekseerManager::SetEffectPosition(int playingHandle, VECTOR pos)
{
	if (playingHandle != -1) {
		SetPosPlayingEffekseer3DEffect(playingHandle, pos.x, pos.y, pos.z);
	}
}

// 入力：playingHandle = 対象インスタンスハンドル, x, y, z = 各軸の回転角（ラジアン）
// 出力：なし
// 副作用：再生中のエフェクトの回転行列の更新（アブダクションビームの照射角度調整等に使用）
void EffekseerManager::SetEffectRotation(int playingHandle, float x, float y, float z)
{
	if (playingHandle != -1) {
		SetRotationPlayingEffekseer3DEffect(playingHandle, x, y, z);
	}
}

// 入力：playingHandle = 対象インスタンスハンドル, x, y, z = 各軸の拡大縮小率
// 出力：なし
// 副作用：再生中エフェクトのスケール変更（クライシスモード移行時の竜巻エフェクトの巨大化等に使用）
void EffekseerManager::SetEffectScale(int playingHandle, float x, float y, float z)
{
	if (playingHandle != -1) {
		SetScalePlayingEffekseer3DEffect(playingHandle, x, y, z);
	}
}

// 入力：playingHandle = 判定対象の再生中インスタンスハンドル
// 出力：再生中であれば true、すでに寿命消滅または停止していれば false
bool EffekseerManager::IsPlaying(int playingHandle)
{
	if (playingHandle == -1) return false;
	return IsEffekseer3DEffectPlaying(playingHandle) != 0;
}

// 入力：playingHandle = 対象インスタンスハンドル, speed = 再生速度スケール（1.0fが等速）
// 出力：なし
// 副作用：対象エフェクトのアニメーション更新速度の変更（ヒットストップによる演出のスローモーション表現等に使用）
void EffekseerManager::SetEffectSpeed(int playingHandle, float speed)
{
	if (playingHandle != -1) {
		SetSpeedPlayingEffekseer3DEffect(playingHandle, speed);
	}
}