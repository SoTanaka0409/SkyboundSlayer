$content = Get-Content -Encoding Default Source\GameManager.cpp
$newContent = @()
$inDuplicate = $false
$count = 0

foreach ($line in $content) {
    if ($line -match "// Initialize Boss Portal at the start of the game") {
        $count++
        if ($count -gt 1) {
            $inDuplicate = $true
        } else {
            $newContent += $line
        }
    }
    elseif ($inDuplicate) {
        if ($line -match "`"Resource/3D/ì]ëóêw/source/portal\.mv1`"") {
            $inDuplicate = $false # end of duplicate block
        }
    }
    else {
        $newContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newContent
