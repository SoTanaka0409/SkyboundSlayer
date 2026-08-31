$gmContent = Get-Content -Encoding Default Source\GameManager.cpp
$newGmContent = @()
foreach ($line in $gmContent) {
    if ($line -match "new Stage\(VAdd\(mBossPortalPos, VGet\(-550\.0f, 0\.0f, -200\.0f\)\), `"Resource/3D/Tree_central/tree-gn/source/TreeGen\.mv1`"") {
        $newGmContent += "    new StageObject(VAdd(mBossPortalPos, VGet(-550.0f, 0.0f, -200.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.8f, 1.8f, 1.8f), `"`", 0.0f);"
    } elseif ($line -match "new Stage\(VAdd\(mBossPortalPos, VGet\(550\.0f, 0\.0f, -200\.0f\)\), `"Resource/3D/Tree_central/tree-gn/source/TreeGen\.mv1`"") {
        $newGmContent += "    new StageObject(VAdd(mBossPortalPos, VGet(550.0f, 0.0f, -200.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.6f, 1.6f, 1.6f), `"`", 0.0f);"
    } else {
        $newGmContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newGmContent
