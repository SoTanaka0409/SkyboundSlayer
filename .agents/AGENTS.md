# LevelMonster Project Rules

## 1. コメント規則
*   **.cpp 関数定義前 (/* */ ブロック)**: 目的（〜するため）・[入力]・[出力]・[副作用]
*   **.h 関数宣言前 (// 1〜2行)**: [入力] [出力] [副作用] を1行で記載。
*   **処理の途中 (// 1行)**: 「なぜ」そうしているか。変数に関しては、シンプルに使用用途などを書く。

## 2. 命名規則
*   **クラス名**: PascalCase (例: `CowManager`, `StageLoader`)
*   **関数名**: PascalCase (例: `Initialize()`, `SpawnCow()`)
*   **メンバー変数**: snake_case + 末尾 `_` (例: `mass_spawn_timer_`, `model_`)
*   **ローカル変数・引数**: snake_case (例: `rock_scale`, `csv_path`)
*   **定数・enum 値**: `k` + PascalCase (例: `kNormal`, `kCowDefault`)
*   **構造体メンバー**: snake_case (例: `model_path`, `speed`)
