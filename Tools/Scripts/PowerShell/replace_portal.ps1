$content = Get-Content -Encoding Default Source\GameManager.cpp
$newContent = @()
$inDraw = $false

foreach ($line in $content) {
    if ($line -match "new StageObject\(VAdd\(mBossPortalPos, VGet\(450\.0f, 0\.0f, -100\.0f\)\)") {
        $newContent += $line
        $newContent += "                // Portal Base"
        $newContent += "                new StageObject(VAdd(mBossPortalPos, VGet(0.0f, -5.0f, 0.0f)), `"Resource/3D/ì]ëóêw/source/portal.mv1`", VGet(5.0f, 5.0f, 5.0f), `"`", 0.0f);"
    }
    elseif ($line -match "// Draw placeholder teleporter") {
        $inDraw = $true
        $newContent += $line
    }
    elseif ($inDraw -and $line -match "DrawCapsule3D") {
        $newContent += "            // " + $line.TrimStart()
    }
    elseif ($inDraw -and $line -match "DrawSphere3D") {
        $newContent += "            // " + $line.TrimStart()
        $inDraw = $false
    }
    else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newContent
