$content = Get-Content -Encoding Default Source\GameManager.cpp
$newContent = @()

foreach ($line in $content) {
    if ($line -match "mBossPortalPos = VAdd\(center, VScale\(dir, -2000\.0f\)\);") {
        $newContent += "                // TEST: Spawn near player instead of opposite side"
        $newContent += "                mBossPortalPos = VAdd(playerPos, VGet(0.0f, 0.0f, 500.0f));"
    }
    elseif ($line -match "`"Resource/3D/ì]ëóêw/source/portal\.mv1`", VGet\(5\.0f, 5\.0f, 5\.0f\)") {
        $newContent += $line -replace "VGet\(5\.0f, 5\.0f, 5\.0f\)", "VGet(50.0f, 50.0f, 50.0f)"
    }
    else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newContent
