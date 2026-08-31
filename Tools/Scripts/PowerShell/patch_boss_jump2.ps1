$content = Get-Content -Encoding Default -Path Source\EnemyBoss_1.cpp -Raw

# 1. Attack() のジャンプ速度・向き計算の修正
$search1 = "if \(Master::mpPlayer\) {`r`n`t`t`t`tVECTOR toPlayer = VSub\(Master::mpPlayer->GetPosition\(\), position_\);`r`n`t`t`t`ttoPlayer.y = 0\.0f;`r`n`t`t`t`tif \(VSquareSize\(toPlayer\) > 0\.0f\) {`r`n`t`t`t`t`tmJumpTargetDir = VNorm\(toPlayer\);`r`n`t`t`t`t} else {`r`n`t`t`t`t`tmJumpTargetDir = VGet\(0\.0f, 0\.0f, 1\.0f\);`r`n`t`t`t`t}`r`n`t`t`t`tmJumpSpeed = 30\.0f;`r`n`t`t`t} else {`r`n`t`t`t`tmJumpTargetDir = VGet\(0\.0f, 0\.0f, 0\.0f\);`r`n`t`t`t`tmJumpSpeed = 0\.0f;`r`n`t`t`t}"

$replace1 = "if (Master::mpPlayer) {`r`n`t`t`t`tVECTOR toPlayer = VSub(Master::mpPlayer->GetPosition(), position_);`r`n`t`t`t`ttoPlayer.y = 0.0f;`r`n`t`t`t`tfloat dist = sqrtf(VSquareSize(toPlayer));`r`n`t`t`t`tif (dist > 0.0001f) {`r`n`t`t`t`t`tmJumpTargetDir = VNorm(toPlayer);`r`n`t`t`t`t} else {`r`n`t`t`t`t`tmJumpTargetDir = VGet(0.0f, 0.0f, 1.0f);`r`n`t`t`t`t}`r`n`t`t`t`tmJumpSpeed = dist / 42.0f;`r`n`t`t`t`tif (mJumpSpeed > 50.0f) mJumpSpeed = 50.0f;`r`n`t`t`t`tif (mJumpSpeed < 10.0f) mJumpSpeed = 10.0f;`r`n`t`t`t`tmfTargetAngle = atan2f(mJumpTargetDir.x, mJumpTargetDir.z);`r`n`t`t`t} else {`r`n`t`t`t`tmJumpTargetDir = VGet(0.0f, 0.0f, 0.0f);`r`n`t`t`t`tmJumpSpeed = 0.0f;`r`n`t`t`t}"

$content = $content -replace $search1, $replace1

# 2. Update() のジャンプ中の回転処理追加
$search2 = "if \(model_->GetNowState\(\) == ANIMATION_ATTACK && mAttackType == 2\)`r`n`t`t`t{`r`n`t`t`t`tposition_ = VAdd\(position_, VScale\(mJumpTargetDir, mJumpSpeed\)\);"

$replace2 = "if (model_->GetNowState() == ANIMATION_ATTACK && mAttackType == 2)`r`n`t`t`t{`r`n`t`t`t`tRotationByMove();`r`n`t`t`t`tposition_ = VAdd(position_, VScale(mJumpTargetDir, mJumpSpeed));"

$content = $content -replace $search2, $replace2

Set-Content -Encoding Default -Path Source\EnemyBoss_1.cpp -Value $content
