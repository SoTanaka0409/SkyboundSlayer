#pragma once
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include <string>
#include <unordered_map>

// 外部ライブラリ(Effekseer)のコンテキストやリソースをアプリケーション全体で一元管理するシングルトン
class EffekseerManager
{
public:
    static EffekseerManager* GetInstance()
    {
        static EffekseerManager instance;
        return &instance;
    }

    // 入力: なし / 出力: なし
    // 副作用: Effekseerのコンテキストを構築する（DxLib_Init後に呼ばないと内部クラッシュするため順序厳守）
    void Init();

    // 入力: なし / 出力: なし
    // 副作用: 全てのアクティブなエフェクトのパーティクル演算（寿命・座標・状態更新）を1フレーム進める
    void Update();

    // 入力: なし / 出力: なし
    // 副作用: 半透明エフェクトのZテスト破綻を防ぐため、必ず3Dモデル描画後の最終パスとしてバッファに書き込む
    void Draw();

    // 入力: なし / 出力: なし
    // 副作用: DxLib_End前にライブラリ管理下の全リソースを明示的に解放し、不正なメモリアクセスを防ぐ
    void End();

    // 入力: name(登録名), filepath(パス), magnification(ベース倍率) / 出力: ロード済みエフェクトハンドル
    // 副作用: 毎フレームのディスクI/O負荷を避けるため、efkファイルをメモリへロードしハッシュマップにキャッシュする
    int LoadEffect(const std::string& name, const char* filepath, float magnification = 1.0f);

    // 入力: name(キャッシュ名), pos(初期座標) / 出力: 再生中エフェクトの固有ハンドル
    // 副作用: キャッシュからエフェクトのインスタンスを生成・配置し、再生キューへ登録する
    int PlayEffect(const std::string& name, VECTOR pos);

    // 入力: playingHandle / 出力: なし
    // 副作用: 指定されたエフェクトの再生を強制終了し、リソース（パーティクル群）を即座に回収する
    void StopEffect(int playingHandle);

    // 入力: playingHandle / 出力: 再生中か否か(bool) / 副作用: なし
    // 攻撃判定の持続時間や、次の演出への遷移タイミングをエフェクトの生存状態と同期させるために使用する
    bool IsPlaying(int playingHandle);

    // 入力: playingHandle, speed(倍率) / 出力: なし
    // 副作用: ヒットストップやスローモーション等の演出時、特定エフェクトのみ進行速度を動的に書き換える
    void SetEffectSpeed(int playingHandle, float speed);

    // 入力: playingHandle, pos / 出力: なし
    // 副作用: 移動するキャラクターや弾にエフェクト（オーラなど）を追従させるため、毎フレーム座標を上書きする
    void SetEffectPosition(int playingHandle, VECTOR pos);

    // 入力: playingHandle, x,y,z(ラジアン) / 出力: なし
    // 副作用: 武器の振り下ろし角度や進行方向の変化に合わせて、エフェクトの射出方向や姿勢を動的に同期する
    void SetEffectRotation(int playingHandle, float x, float y, float z);

    // 入力: playingHandle, x,y,z(スケール) / 出力: なし
    // 副作用: 爆発の規模やチャージサイズなど、実行時のパラメータ変動に応じてエフェクトの描画サイズを拡縮する
    void SetEffectScale(int playingHandle, float x, float y, float z);

private:
    // 入力: なし / 出力: なし / 副作用: なし
    // シングルトンパターンの制約として、外部からの不用意なインスタンス生成や破棄を禁止する
    EffekseerManager();
    ~EffekseerManager();

    std::unordered_map<std::string, int> effects_; // 文字列比較による検索負荷をハッシュ計算で軽減するエフェクトキャッシュ
};