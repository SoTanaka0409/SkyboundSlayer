$content = Get-Content -Encoding Default Source\GameManager.cpp
$newContent = @()
foreach ($line in $content) {
    if ($line -match "mCurrentPhase = Phase::SHOP_3;") {
        $newContent += $line
        $newContent += "                VECTOR playerPos = Config::GetStageCenter();"
        $newContent += "                if (Master::mpPlayer) {"
        $newContent += "                    playerPos = Master::mpPlayer->CastTo<Player3D>()->GetPosition();"
        $newContent += "                }"
        $newContent += "                VECTOR center = Config::GetStageCenter();"
        $newContent += "                VECTOR dir = VSub(playerPos, center);"
        $newContent += "                dir.y = 0.0f;"
        $newContent += "                if (VSize(dir) < 1.0f) dir = VGet(0.0f, 0.0f, 1.0f);"
        $newContent += "                else dir = VNorm(dir);"
        $newContent += "                mBossPortalPos = VAdd(center, VScale(dir, -2000.0f));"
    }
    elseif ($line -match "VECTOR teleporterPos = VAdd\(Config::GetStageCenter\(\), VGet\(0\.0f, 0\.0f, 800\.0f\)\);") {
        $newContent += $line -replace "VAdd\(Config::GetStageCenter\(\), VGet\(0\.0f, 0\.0f, 800\.0f\)\)", "mBossPortalPos"
    }
    elseif ($line -match "mCurrentPhase\(Phase::PHASE_1\), mShopTimer\(0\), mFadeAlpha\(0\)") {
        $newContent += $line -replace "mFadeAlpha\(0\)", "mFadeAlpha(0), mBossPortalPos(VGet(0,0,0))"
    }
    else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newContent
