$content = Get-Content -Encoding Default -Path Source\TitleScene.cpp -Raw

# 1. Constructor Initialization
$search1 = "mnSkyBoxHandle(-1), mnStageHandle(-1), mnCastleHandle(-1)"
$replace1 = "mnSkyBoxHandle(-1), mnStageHandle(-1), mnCastleHandle(-1), mnGroundHandle(-1), mnTreeHandle(-1)"
$content = $content -replace [regex]::Escape($search1), $replace1

# 2. Load Models in Initialize
$search2 = "mnCastleHandle = MV1LoadModel(`"Resource/3D/Stage_casule/source/Parede castelo.mv1`");"
$replace2 = "mnCastleHandle = MV1LoadModel(`"Resource/3D/Stage_casule/source/Parede castelo.mv1`");`r`n`tmnGroundHandle = MV1LoadModel(`"Resource/3D/Stage/Stage00.mv1`");`r`n`tmnTreeHandle = MV1LoadModel(`"Resource/3D/Tree_central/tree_central.mv1`");"
$content = $content -replace [regex]::Escape($search2), $replace2

# 3. Setup Scale & Pos in Initialize
$search3 = "MV1SetPosition\(mnCastleHandle, VGet\(0\.0f, 0\.0f, 0\.0f\)\);"
$replace3 = "MV1SetPosition(mnCastleHandle, VGet(0.0f, 0.0f, 0.0f));`r`n`t`r`n`tMV1SetScale(mnGroundHandle, VGet(3.0f, 0.3f, 3.0f));`r`n`tMV1SetPosition(mnGroundHandle, VGet(0.0f, 0.0f, 0.0f));`r`n`t`r`n`tMV1SetScale(mnTreeHandle, VGet(10.0f, 10.0f, 10.0f));"
$content = $content -replace $search3, $replace3

# 4. Draw Models
$search4 = "MV1DrawModel\(mnCastleHandle\);"
$replace4 = "MV1DrawModel(mnCastleHandle);`r`n`tMV1DrawModel(mnGroundHandle);`r`n`t`r`n`tMV1SetPosition(mnTreeHandle, VGet(1000.0f, 0.0f, 800.0f));`r`n`tMV1DrawModel(mnTreeHandle);`r`n`tMV1SetPosition(mnTreeHandle, VGet(-1200.0f, 0.0f, 600.0f));`r`n`tMV1DrawModel(mnTreeHandle);`r`n`tMV1SetPosition(mnTreeHandle, VGet(800.0f, 0.0f, -1500.0f));`r`n`tMV1DrawModel(mnTreeHandle);`r`n`tMV1SetPosition(mnTreeHandle, VGet(-800.0f, 0.0f, -800.0f));`r`n`tMV1DrawModel(mnTreeHandle);"
$content = $content -replace $search4, $replace4

# 5. Finalize
$search5 = "TitleScene::~TitleScene\(\)`r`n{`r`n}"
$replace5 = "TitleScene::~TitleScene()`r`n{`r`n`tMV1DeleteModel(mnSkyBoxHandle);`r`n`tMV1DeleteModel(mnStageHandle);`r`n`tMV1DeleteModel(mnCastleHandle);`r`n`tMV1DeleteModel(mnGroundHandle);`r`n`tMV1DeleteModel(mnTreeHandle);`r`n}"
$content = $content -replace $search5, $replace5

Set-Content -Encoding Default -Path Source\TitleScene.cpp -Value $content
