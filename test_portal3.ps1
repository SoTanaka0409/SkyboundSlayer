$content = Get-Content -Encoding Default Source\Scene3D.cpp
$newContent = @()

foreach ($line in $content) {
    if ($line -match "// --- TEST PORTAL PLACEMENT ---" -or $line -match "`"Resource/3D/ì]ëóêw/source/portal\.mv1`"") {
        # skip
    } else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\Scene3D.cpp $newContent

$content2 = Get-Content -Encoding Default Source\GameManager.cpp
$newContent2 = @()
$inSpawnPhase = $false

foreach ($line in $content2) {
    if ($line -match "SpawnPhaseEnemies\(\);") {
        $newContent2 += $line
        $newContent2 += ""
        $newContent2 += "    // Initialize Boss Portal at the start of the game, opposite to player spawn"
        $newContent2 += "    VECTOR playerStartPos = VGet(-1200.0f, 20.0f, -1000.0f);"
        $newContent2 += "    VECTOR center = Config::GetStageCenter();"
        $newContent2 += "    VECTOR dir = VSub(playerStartPos, center);"
        $newContent2 += "    dir.y = 0.0f;"
        $newContent2 += "    if (VSize(dir) < 1.0f) dir = VGet(0.0f, 0.0f, 1.0f);"
        $newContent2 += "    else dir = VNorm(dir);"
        $newContent2 += "    mBossPortalPos = VAdd(center, VScale(dir, -2000.0f));"
        $newContent2 += "    "
        $newContent2 += "    // ---- Add Portal Decorations ----"
        $newContent2 += "    new Stage(VAdd(mBossPortalPos, VGet(-350.0f, 0.0f, -350.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(60.0f, 60.0f, 60.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent2 += "    new Stage(VAdd(mBossPortalPos, VGet(350.0f, 0.0f, -350.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(70.0f, 70.0f, 70.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent2 += "    new Stage(VAdd(mBossPortalPos, VGet(-400.0f, 0.0f, 300.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(55.0f, 55.0f, 55.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent2 += "    new Stage(VAdd(mBossPortalPos, VGet(400.0f, 0.0f, 300.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(65.0f, 65.0f, 65.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent2 += "    new StageObject(VAdd(mBossPortalPos, VGet(-550.0f, 0.0f, -200.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.8f, 1.8f, 1.8f), `"`", 150.0f);"
        $newContent2 += "    new StageObject(VAdd(mBossPortalPos, VGet(550.0f, 0.0f, -200.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.6f, 1.6f, 1.6f), `"`", 150.0f);"
        $newContent2 += "    "
        $newContent2 += "    // Portal Base"
        $newContent2 += "    new StageObject(VAdd(mBossPortalPos, VGet(0.0f, -5.0f, 0.0f)), `"Resource/3D/ì]ëóêw/source/portal.mv1`", VGet(80.0f, 80.0f, 80.0f), `"`", 0.0f);"
    }
    elseif ($line -match "mCurrentPhase = Phase::SHOP_3;") {
        $inSpawnPhase = $true
        $newContent2 += $line
    }
    elseif ($inSpawnPhase -and $line -match "// Portal Base") {
        $inSpawnPhase = $false
        # skip this line
    }
    elseif ($inSpawnPhase) {
        # skip the lines that were generating the portal and decorations dynamically
    }
    elseif (!$inSpawnPhase -and $line -match "`"Resource/3D/ì]ëóêw/source/portal\.mv1`"") {
        # skip any remaining portal gen line if it got left behind
    }
    else {
        $newContent2 += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newContent2
