$content = Get-Content -Encoding Default Source\Scene3D.cpp
$newContent = @()
foreach ($line in $content) {
    if ($line -match "SetGlobalAmbientLight\(GetColorF\(0\.15f, 0\.15f, 0\.2f\)\);") {
        $newContent += "    SetGlobalAmbientLight(GetColorF(0.15f, 0.15f, 0.2f, 1.0f));"
    } elseif ($line -match "SetLightDifColor\(GetColorF\(0\.6f, 0\.6f, 0\.8f\)\);") {
        $newContent += "    SetLightDifColor(GetColorF(0.6f, 0.6f, 0.8f, 1.0f));"
    } elseif ($line -match "SetLightSpcColor\(GetColorF\(0\.4f, 0\.4f, 0\.5f\)\);") {
        $newContent += "    SetLightSpcColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));"
    } elseif ($line -match "SetLightDirection\(VGet\(-0\.5f, -0\.8f, 0\.3f\)\);") {
        $newContent += "    SetLightDirection(VNorm(VGet(-0.5f, -0.8f, 0.3f)));"
    } else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\Scene3D.cpp $newContent
