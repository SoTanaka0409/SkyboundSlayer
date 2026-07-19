$gmContent = Get-Content -Encoding Default Source\GameManager.cpp
$newGmContent = @()
foreach ($line in $gmContent) {
    if ($line -match "new StageObject\(mBossPortalPos, `"Resource/3D/ì]ëóêw/source/portal\.mv1`", VGet\(portalSize, portalSize, portalSize\), `"`", 0\.0f, -520\.0f\);") {
        $newGmContent += "    new Stage(VAdd(mBossPortalPos, VGet(0.0f, -520.0f, 0.0f)), `"Resource/3D/ì]ëóêw/source/portal.mv1`", `"Resource/3D/ì]ëóêw/source/portal.mv1`", VGet(portalSize, portalSize, portalSize));"
    } else {
        $newGmContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newGmContent
