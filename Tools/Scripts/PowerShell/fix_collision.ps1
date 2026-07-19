$gmContent = Get-Content -Encoding Default Source\GameManager.cpp
$newGmContent = @()
foreach ($line in $gmContent) {
    if ($line -match "new StageObject\(VAdd\(mBossPortalPos, VGet\(-550\.0f, 0\.0f, -200\.0f\)\), `"Resource/3D/Tree_central/tree-gn/source/TreeGen\.mv1`", VGet\(1\.8f, 1\.8f, 1\.8f\), `"`", 150\.0f\);") {
        $newGmContent += "    new Stage(VAdd(mBossPortalPos, VGet(-550.0f, 0.0f, -200.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.8f, 1.8f, 1.8f));"
    } elseif ($line -match "new StageObject\(VAdd\(mBossPortalPos, VGet\(550\.0f, 0\.0f, -200\.0f\)\), `"Resource/3D/Tree_central/tree-gn/source/TreeGen\.mv1`", VGet\(1\.6f, 1\.6f, 1\.6f\), `"`", 150\.0f\);") {
        $newGmContent += "    new Stage(VAdd(mBossPortalPos, VGet(550.0f, 0.0f, -200.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.6f, 1.6f, 1.6f));"
    } elseif ($line -match "new StageObject\(mBossPortalPos, `"Resource/3D/ì]ëóêw/source/portal\.mv1`", VGet\(80\.0f, 80\.0f, 80\.0f\), `"`", 0\.0f, -20\.0f\);") {
        $newGmContent += "    new Stage(VAdd(mBossPortalPos, VGet(0.0f, -20.0f, 0.0f)), `"Resource/3D/ì]ëóêw/source/portal.mv1`", `"Resource/3D/ì]ëóêw/source/portal.mv1`", VGet(80.0f, 80.0f, 80.0f));"
    } else {
        $newGmContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newGmContent
