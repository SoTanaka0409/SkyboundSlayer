$content = Get-Content -Encoding Default -Path Source\EnemyBoss_1.cpp -Raw

$search1 = "OnJumpCollider = false;`r`n`t`t}"
$replace1 = "OnJumpCollider = false;`r`n`t`t`t`r`n`t`t`tif (Master::mpPlayer) {`r`n`t`t`t`tVECTOR toPlayer = VSub(Master::mpPlayer->GetPosition(), position_);`r`n`t`t`t`ttoPlayer.y = 0.0f;`r`n`t`t`t`tif (VSquareSize(toPlayer) > 0.0f) {`r`n`t`t`t`t`tmJumpTargetDir = VNorm(toPlayer);`r`n`t`t`t`t} else {`r`n`t`t`t`t`tmJumpTargetDir = VGet(0.0f, 0.0f, 1.0f);`r`n`t`t`t`t}`r`n`t`t`t`tmJumpSpeed = 30.0f;`r`n`t`t`t} else {`r`n`t`t`t`tmJumpTargetDir = VGet(0.0f, 0.0f, 0.0f);`r`n`t`t`t`tmJumpSpeed = 0.0f;`r`n`t`t`t}`r`n`t`t}"

$content = $content -replace [regex]::Escape($search1), $replace1

$search2 = "if \(!HighPositionFlag\)`r`n`t`t`t`t{`r`n`t`t`t`t`tposition_ = VAdd\(position_, VGet\(0\.0f, 15\.0f, 0\.0f\)\);"
$replace2 = "position_ = VAdd(position_, VScale(mJumpTargetDir, mJumpSpeed));`r`n`t`t`t`t`r`n`t`t`t`tif (!HighPositionFlag)`r`n`t`t`t`t{`r`n`t`t`t`t`tposition_ = VAdd(position_, VGet(0.0f, 15.0f, 0.0f));"

$content = $content -replace $search2, $replace2

Set-Content -Encoding Default -Path Source\EnemyBoss_1.cpp -Value $content
