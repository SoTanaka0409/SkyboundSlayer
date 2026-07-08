$content = Get-Content -Encoding Default Source\Scene3D.cpp
$newContent = @()
$inFogBlock = $false

foreach ($line in $content) {
    if ($line -match "SetFogEnable\(TRUE\);") {
        $inFogBlock = $true
        $newContent += "    // 霧の設定（深く、暗く）"
        $newContent += "    SetFogEnable(TRUE);"
        $newContent += "    SetFogColor(20, 30, 50); // 暗いネイビーブルー"
        $newContent += "    SetFogStartEnd(500.0f, 10000.0f); // 霧が手前から濃くかかるように"
        $newContent += ""
        $newContent += "    // 環境光（影の明るさ）の設定"
        $newContent += "    SetGlobalAmbientLight(GetColorF(0.15f, 0.15f, 0.2f));"
        $newContent += ""
        $newContent += "    // メインの平行光源（月光のような冷たく弱い光）"
        $newContent += "    SetLightDifColor(GetColorF(0.6f, 0.6f, 0.8f));"
        $newContent += "    SetLightSpcColor(GetColorF(0.4f, 0.4f, 0.5f));"
        $newContent += "    SetLightDirection(VGet(-0.5f, -0.8f, 0.3f));"
    } elseif ($inFogBlock) {
        if ($line -match "SetFogStartEnd") {
            $inFogBlock = $false
        }
    } else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\Scene3D.cpp $newContent
