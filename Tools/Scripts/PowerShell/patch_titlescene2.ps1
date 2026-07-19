$content = Get-Content -Encoding Default -Path Source\TitleScene.cpp -Raw

# 1. 必要なインクルードを追加
$search1 = "#include <math.h>"
$replace1 = "#include <math.h>`r`n#include <fstream>`r`n#include <sstream>`r`n#include `"Stage.h`"`r`n#include `"StageObject.h`"`r`n#include `"SkyBox.h`"`r`n#include `"Config.h`""
$content = $content -replace [regex]::Escape($search1), $replace1

# 2. コンストラクタから不要なハンドル初期化を削除
$search2 = ", mnSkyBoxHandle(-1), mnStageHandle(-1), mnCastleHandle(-1), mnGroundHandle(-1), mnTreeHandle(-1), mnPortalHandle(-1)"
$replace2 = ""
$content = $content -replace [regex]::Escape($search2), $replace2

# 3. デストラクタの削除処理を空に
$search3 = "TitleScene::~TitleScene()`r`n{`r`n`tMV1DeleteModel(mnSkyBoxHandle);`r`n`tMV1DeleteModel(mnStageHandle);`r`n`tMV1DeleteModel(mnCastleHandle);`r`n`tMV1DeleteModel(mnGroundHandle);`r`n`tMV1DeleteModel(mnTreeHandle);`r`n`tMV1DeleteModel(mnPortalHandle);`r`n}"
$replace3 = "TitleScene::~TitleScene()`r`n{`r`n}"
$content = $content -replace [regex]::Escape($search3), $replace3

# 4. Initialize() の中身をScene3D互換に書き換え
$search4 = "`t// 3DモチEのローチE`r`n`tmnSkyBoxHandle = MV1LoadModel(`"Resource/3D/SkyBox/SkyBox.x`");`r`n`tmnStageHandle = MV1LoadModel(`"Resource/3D/stage_sky/source/Flooting_Stage.mv1`");`r`n`tmnCastleHandle = MV1LoadModel(`"Resource/3D/Stage_casule/source/Parede castelo.mv1`");`r`n`tmnGroundHandle = MV1LoadModel(`"Resource/3D/Stage/Stage00.mv1`");`r`n`tmnTreeHandle = MV1LoadModel(`"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`");`r`n`tmnPortalHandle = MV1LoadModel(`"Resource/3D/portal/source/portal.mv1`");`r`n`t`r`n`t// スケールと位置の設宁E`r`n`tMV1SetScale(mnSkyBoxHandle, VGet(13.0f, 13.0f, 13.0f));`r`n`tMV1SetPosition(mnSkyBoxHandle, VGet(0, 0, -5000));`r`n`t`r`n`tMV1SetScale(mnStageHandle, VGet(200.0f, 100.0f, 200.0f));`r`n`tMV1SetPosition(mnStageHandle, VGet(0.0f, 5000.0f, -20000.0f));`r`n`t`r`n`tMV1SetScale(mnCastleHandle, VGet(10.0f, 10.0f, 10.0f));`r`n`tMV1SetPosition(mnCastleHandle, VGet(0.0f, 0.0f, 0.0f));`r`n`t`r`n`tMV1SetScale(mnGroundHandle, VGet(3.0f, 0.3f, 3.0f));`r`n`tMV1SetPosition(mnGroundHandle, VGet(0.0f, 0.0f, 0.0f));`r`n`t`r`n`tMV1SetScale(mnTreeHandle, VGet(3.0f, 3.0f, 3.0f));`r`n`t`r`n`tMV1SetScale(mnPortalHandle, VGet(3.0f, 3.0f, 3.0f));`r`n`tMV1SetPosition(mnPortalHandle, VGet(0.0f, 0.0f, 500.0f));"
$replace4 = "`t// 本編と同じステージとオブジェクトを読み込む`r`n`tnew Stage(VGet(0.0f, 5000.0f, -20000.0f), `"Resource/3D/stage_sky/source/Flooting_Stage.mv1`", `"Resource/3D/stage_sky/source/Flooting_Stage.mv1`", VGet(200.0f, 100.0f, 200.0f));`r`n`tnew Stage(Config::GetStageCenter(), `"Resource/3D/Stage/Stage00.mv1`", `"Resource/3D/Stage/Stage00_c.mv1`",VGet(3.0f,0.3f,3.0f));`r`n`t`r`n`tstd::ifstream file(`"Resource/CSV/stage_objects.csv`");`r`n`tif (file.is_open())`r`n`t{`r`n`t`tstd::string line;`r`n`t`tstd::getline(file, line);`r`n`t`twhile (std::getline(file, line))`r`n`t`t{`r`n`t`t`tif (line.empty()) continue;`r`n`t`t`tstd::stringstream ss(line);`r`n`t`t`tstd::string type, model, xStr, yStr, zStr, sxStr, syStr, szStr, texture, colSizeStr, isRelativeStr;`r`n`t`t`tstd::getline(ss, type, ',');`r`n`t`t`tstd::getline(ss, model, ',');`r`n`t`t`tstd::getline(ss, xStr, ',');`r`n`t`t`tstd::getline(ss, yStr, ',');`r`n`t`t`tstd::getline(ss, zStr, ',');`r`n`t`t`tstd::getline(ss, sxStr, ',');`r`n`t`t`tstd::getline(ss, syStr, ',');`r`n`t`t`tstd::getline(ss, szStr, ',');`r`n`t`t`tstd::getline(ss, texture, ',');`r`n`t`t`tstd::getline(ss, colSizeStr, ',');`r`n`t`t`tstd::getline(ss, isRelativeStr, ',');`r`n`t`t`tfloat x = std::stof(xStr);`r`n`t`t`tfloat y = std::stof(yStr);`r`n`t`t`tfloat z = std::stof(zStr);`r`n`t`t`tfloat sx = std::stof(sxStr);`r`n`t`t`tfloat sy = std::stof(syStr);`r`n`t`t`tfloat sz = std::stof(szStr);`r`n`t`t`tint isRelative = 0;`r`n`t`t`tif (!isRelativeStr.empty()) isRelative = std::stoi(isRelativeStr);`r`n`t`t`tVECTOR pos = VGet(x, y, z);`r`n`t`t`tif (isRelative == 1) { pos = VAdd(Config::GetStageCenter(), pos); }`r`n`t`t`tVECTOR scale = VGet(sx, sy, sz);`r`n`t`t`tif (type == `"StageObject`") {`r`n`t`t`t`tfloat colSize = 0.0f;`r`n`t`t`t`tif (!colSizeStr.empty()) colSize = std::stof(colSizeStr);`r`n`t`t`t`tnew StageObject(pos, model, scale, `"`", colSize);`r`n`t`t`t} else if (type == `"Stage`") {`r`n`t`t`t`tnew Stage(pos, model, model, scale, texture);`r`n`t`t`t}`r`n`t`t}`r`n`t}`r`n`t`r`n`tSkyBox* pSkyBox = new SkyBox(`"Resource/3D/SkyBox/SkyBox.x`",VGet(0,0,-5000));`r`n`tfloat scale = 13.0f;`r`n`tpSkyBox->SetScale(VGet(scale, scale, scale));`r`n`tpSkyBox->SetModelTexture(`"Resource/3D/SkyBox/sky001.jpg`");`r`n`t`r`n`t// タイトル用にテレポーターも一つ置いておく`r`n`tnew StageObject(VGet(0.0f, 0.0f, 500.0f), `"Resource/3D/portal/source/portal.mv1`", VGet(3.0f, 3.0f, 3.0f));"
$content = $content -replace $search4, $replace4

# 5. Draw() から個別の描画処理を削除
$search5 = "`t// 3D背景の描画`r`n`tMV1DrawModel(mnSkyBoxHandle);`r`n`tMV1DrawModel(mnStageHandle);`r`n`tMV1DrawModel(mnCastleHandle);`r`n`tMV1DrawModel(mnGroundHandle);`r`n`t`r`n`tMV1SetPosition(mnTreeHandle, VGet(1000.0f, 0.0f, 800.0f));`r`n`tMV1DrawModel(mnTreeHandle);`r`n`tMV1SetPosition(mnTreeHandle, VGet(-1200.0f, 0.0f, 600.0f));`r`n`tMV1DrawModel(mnTreeHandle);`r`n`tMV1SetPosition(mnTreeHandle, VGet(800.0f, 0.0f, -1500.0f));`r`n`tMV1DrawModel(mnTreeHandle);`r`n`tMV1SetPosition(mnTreeHandle, VGet(-800.0f, 0.0f, -800.0f));`r`n`tMV1DrawModel(mnTreeHandle);`r`n`t`r`n`t// テレポーターの描画`r`n`tMV1DrawModel(mnPortalHandle);"
$replace5 = "`t// 3D背景は Scene::Draw() によってObjectManager経由で自動描画される"
$content = $content -replace $search5, $replace5

Set-Content -Encoding Default -Path Source\TitleScene.cpp -Value $content
