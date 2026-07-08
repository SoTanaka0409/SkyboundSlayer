$gmContent = Get-Content -Encoding Default Source\GameManager.cpp
$newGmContent = @()
foreach ($line in $gmContent) {
    if ($line -match "`"Resource/3D/ì]ëóêw/source/portal\.mv1`"") {
        $newGmContent += $line -replace "VAdd\(mBossPortalPos, VGet\(0\.0f, -2500\.0f, 0\.0f\)\)", "mBossPortalPos"
    } else {
        $newGmContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newGmContent
