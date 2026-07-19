$gmContent = Get-Content -Encoding Default Source\GameManager.cpp
$newGmContent = @()
foreach ($line in $gmContent) {
    if ($line -match "`"Resource/3D/ì]ëóêw/source/portal\.mv1`"") {
        $newGmContent += $line -replace "-520\.0f,", "-570.0f,"
    } else {
        $newGmContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newGmContent
