$content = Get-Content -Encoding Default Source\Scene3D.cpp
$newContent = @()
foreach ($line in $content) {
    if ($line -match "SetGlobalAmbientLight" -or $line -match "SetLightDifColor" -or $line -match "SetLightSpcColor" -or $line -match "SetLightDirection") {
        $newContent += "// " + $line
    } else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\Scene3D.cpp $newContent
