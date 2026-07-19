#include"Save.h"
#include"Master.h"
#include"InputManager.h"
#include"Texture.h"
#include"ObjectManager.h"
#include"SceneManager.h"

// 入力：なし
// 出力：なし
// 副作用：選択カーソルの初期化、およびセーブ画面用UIテクスチャの動的生成
Save::Save()
	:Select(0)
	, SelectMax(0)
	, SelectMin(0)
{
	texture_ = new Texture("Resource/画像/UI/02_セーブ画面画像.png", VGet(500, 500, 0), true);
}

// 入力：なし
// 出力：なし
// 副作用：なし
Save::~Save()
{
}

// 入力：なし
// 出力：なし
// 副作用：セーブメニュー展開時（is_save_ == true）の専用テクスチャレイヤー描画
void Save::Draw()
{
	// UI仕様：ゲームの進行状況（本編の描画）を背景に残したまま、画面最前面にセーブUIをオーバーレイ表示させるためのステート監視
	if (Master::is_save_)
	{
		texture_->Draw();
	}
}

// 入力：なし
// 出力：なし
// 副作用：Lキーでのセーブ画面呼び出し、およびBACKキーでの画面閉じる（キャンセル）処理のトグル制御
void Save::Update()
{
	// UX仕様：プレイヤーがゲームプレイ中にいつでもシームレスに進行状況を記録できるよう、Lキーによる即時展開とBACKキーによる直感的なキャンセル（離脱）のトグルフローを提供する
	if (InputManager::CheckDownKey(KEY_INPUT_L))
	{
		Master::is_save_ = true;
	}

	if (Master::is_save_)
	{
		if (InputManager::CheckDownKey(KEY_INPUT_BACK))
		{
			Master::is_save_ = false;
		}
	}
}

// 入力：date = 読み込むセーブデータのポインタ
// 出力：なし
// 副作用：指定されたスロットのデータをゲーム内ステータスへ展開
void Save::GameLoad(SaveDate* date)
{
}

// 入力：date = 保存するセーブデータのポインタ
// 出力：なし
// 副作用：セーブデータリストへの新規追加、または既存スロット番号との合致時はデータの上書き保存処理
void Save::GameSave(SaveDate* date)
{
	// アーキテクチャ設計：同一スロットへのセーブ時にデータが重複してリストが肥大化・破損するのを防ぐため、保存前に既存のデータ番号（num）と照合し安全に上書きするための検索レイヤー
	for (auto itr = AllSaveDate.begin(); itr != AllSaveDate.end(); itr++)
	{
		if ((*itr)->num == date->num)
		{

		}
	}
}

// 入力：なし
// 出力：なし
// 副作用：上下キー入力に応じた、メニュー内の選択カーソル（Select）のインクリメント/デクリメント
void Save::SelectClass()
{
	// UX仕様：マウス操作に依存せず、コントローラーやキーボード（方向キー）のみでも快適にセーブスロットをブラウジング・選択できる直感的なUIナビゲーションを担保する
	if (InputManager::CheckDownKey(KEY_INPUT_UP))
	{
		Select--;
	}
	if (InputManager::CheckDownKey(KEY_INPUT_DOWN))
	{
		Select++;
	}
}