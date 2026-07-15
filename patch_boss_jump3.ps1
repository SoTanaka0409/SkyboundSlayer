$content = Get-Content -Encoding Default -Path Source\EnemyBoss_1.cpp -Raw

$search1 = "if \(mJumpSpeed < 10\.0f\) mJumpSpeed = 10\.0f;"
$replace1 = "// if (mJumpSpeed < 10.0f) mJumpSpeed = 10.0f; // removed to prevent overshooting"

$content = $content -replace $search1, $replace1

Set-Content -Encoding Default -Path Source\EnemyBoss_1.cpp -Value $content
