$gmContent = Get-Content -Encoding Default Source\GameManager.cpp
$newGmContent = @()
foreach ($line in $gmContent) {
    # Skip rocks and trees around boss portal (keep only the portal itself)
    if ($line -match "Rock Low Poly\.mv1" -and $line -match "mBossPortalPos") {
        # skip
    } elseif ($line -match "TreeGen\.mv1" -and $line -match "mBossPortalPos") {
        # skip
    } elseif ($line -match "// ---- Add Portal Decorations ----") {
        # skip comment too
    } else {
        $newGmContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newGmContent
