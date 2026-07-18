#pragma once
#include"Dxlib.h"
#include"Object3D.h"

#include"DrawHp.h"
#include"Debug.h"
#include"EnemyManager.h"
#include"Enemy.h"
#include"Collision.h"

class Model;

// 汎用的なEnemyクラスを継承し、重力や高低差など3D空間特有の挙動を実装するための派生クラス
class Enemy3D : public Enemy
{
public:
    // 入力: filename(モデルパス), 初期座標, ステータス群(HP, 速度等), 当たり判定サイズ群, 所持金, アニメ分離フラグ
    // 出力: なし / 副作用: 基底クラスの初期化に加え、3D環境に依存する専用リソースや状態をセットアップする
    Enemy3D(std::string filename, VECTOR initPos, float hp, float speed, float HitSize, float Serch1, float Serch2, float Serch3, int money, bool is_separate_anim_);

    // 入力: なし / 出力: なし
    // 副作用: Enemy3D側で独自に確保したリソースがあれば破棄し、シーン遷移時のメモリリークを防ぐ
    ~Enemy3D();

    // 入力: なし / 出力: なし
    // 副作用: 3Dモデルの描画に加え、ワールド座標に追従する空間UI（HPバーなど）の描画命令をバッファに登録する
    void Draw() override;

    // 入力: なし / 出力: なし
    // 副作用: 3D地形との接地判定（レイキャスト）や重力落下を加味して、親クラスのAIステートと座標を更新する
    void Update() override;

    // 入力: なし / 出力: なし
    // 副作用: 3D専用の攻撃モーションを再生し、Z軸を含んだ立体的な攻撃判定（Sphere等）を空間に生成する
    void Attack() override;
};