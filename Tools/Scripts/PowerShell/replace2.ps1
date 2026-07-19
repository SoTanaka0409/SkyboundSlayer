$content = Get-Content -Encoding Default Source\GameManager.cpp
$newContent = @()
$includesAdded = $false

foreach ($line in $content) {
    if (!$includesAdded -and $line -match "#include <DxLib.h>") {
        $newContent += $line
        $newContent += "#include `"Stage.h`""
        $newContent += "#include `"StageObject.h`""
        $newContent += "#include `"Tree.h`""
        $includesAdded = $true
    }
    elseif ($line -match "mBossPortalPos = VAdd\(center, VScale\(dir, -2000\.0f\)\);") {
        $newContent += $line
        $newContent += "                // ---- Add Portal Decorations ----"
        $newContent += "                new Stage(VAdd(mBossPortalPos, VGet(-250.0f, 0.0f, -250.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(40.0f, 40.0f, 40.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent += "                new Stage(VAdd(mBossPortalPos, VGet(250.0f, 0.0f, -250.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(50.0f, 50.0f, 50.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent += "                new Stage(VAdd(mBossPortalPos, VGet(-300.0f, 0.0f, 200.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(45.0f, 45.0f, 45.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent += "                new Stage(VAdd(mBossPortalPos, VGet(300.0f, 0.0f, 200.0f)), `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", `"Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1`", VGet(35.0f, 35.0f, 35.0f), `"Resource/3D/Rock/mossy-rock/source/rock1_color.jpg`");"
        $newContent += "                new StageObject(VAdd(mBossPortalPos, VGet(-450.0f, 0.0f, -100.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.5f, 1.5f, 1.5f), `"`", 150.0f);"
        $newContent += "                new StageObject(VAdd(mBossPortalPos, VGet(450.0f, 0.0f, -100.0f)), `"Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1`", VGet(1.2f, 1.2f, 1.2f), `"`", 150.0f);"
    }
    else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newContent
