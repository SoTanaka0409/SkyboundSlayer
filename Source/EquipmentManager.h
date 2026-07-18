#pragma once
#include"Equipment.h"
#include"list"

// プレイヤーが所持・装着している武器や防具のデータを一元管理し、ステータスへの反映を統括するマネージャー
class EquipmentManager
{
public:
    // 入力: なし / 出力: なし
    // 副作用: 装備データ管理用の内部リストや、ステータス計算用変数の初期化を行う
    EquipmentManager();

    // 入力: なし / 出力: なし
    // 副作用: リスト内に動的確保された全装備データ(EquipmentDate)を明示的に破棄し、メモリリークを防ぐ
    ~EquipmentManager();

    // 入力: なし / 出力: なし
    // 副作用: プレイヤーの手に追従する武器の3Dモデルや、インベントリUIなどの描画命令をバッファへ登録する
    void Draw();

    // 入力: なし / 出力: なし
    // 副作用: 装備品の耐久値の減少や、自動回復（リジェネ）など毎フレーム発生する装備の特殊効果を進行させる
    void Update();

    // 入力: date (新規取得した装備データ) / 出力: なし
    // 副作用: 宝箱やショップから入手した装備を所持リスト(date_list_)へ追加し、インベントリを更新する
    void AddEquipment(Equipment::EquipmentDate* date);

    // 入力: date (装着対象の装備データ) / 出力: なし
    // 副作用: 指定された装備をアクティブ状態に切り替え、プレイヤーのステータス（攻撃力等）に補正を適用する
    void WearEquipment(Equipment::EquipmentDate* date);

    // 入力: なし / 出力: 装備による加算ダメージ量(float) / 副作用: なし
    // 攻撃ヒット時に呼び出され、現在アクティブな全装備の攻撃力ボーナスを合算してダメージ計算に提供する
    float GetDamage();

    std::list<Equipment::EquipmentDate*> date_list_; // 所持品の売却やドロップなど、頻繁な要素の追加・削除によるメモリ再確保の負荷を抑えるためのリスト構造
private:

};