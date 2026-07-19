$files = Get-ChildItem -Path "Source" -Include *.cpp,*.h -Recurse

$totalBlankLines = 0
$totalCommentedCode = 0
$filesWithExcessiveBlanks = @()
$filesWithCommentedCode = @()

foreach ($f in $files) {
    try {
        $lines = [System.IO.File]::ReadAllLines($f.FullName, [System.Text.Encoding]::Default)
        
        $consecutiveBlanks = 0
        $fileBlanks = 0
        $fileCommentedCode = 0

        foreach ($line in $lines) {
            $trimmed = $line.Trim()
            
            if ($trimmed -eq "") {
                $consecutiveBlanks++
                if ($consecutiveBlanks -ge 3) {
                    $fileBlanks++
                    $totalBlankLines++
                }
            } else {
                $consecutiveBlanks = 0
                
                if ($trimmed.StartsWith("//")) {
                    # Check if it looks like code
                    if ($trimmed -match ";$|{$|^}$| = |if \(|for \(|while \(") {
                        # Exclude Japanese characters (very basic check)
                        if (-not ($trimmed -match "[^\x00-\x7F]")) {
                            $fileCommentedCode++
                            $totalCommentedCode++
                        }
                    }
                }
            }
        }

        if ($fileBlanks -gt 5) {
            $filesWithExcessiveBlanks += "$($f.Name) ($fileBlanks excessive blank lines)"
        }
        if ($fileCommentedCode -gt 5) {
            $filesWithCommentedCode += "$($f.Name) ($fileCommentedCode lines of commented code)"
        }
    } catch {
        Write-Host "Error reading $($f.Name)"
    }
}

Write-Host "--- Analysis Results ---"
Write-Host "Total excessive blank lines (>2 consecutive): $totalBlankLines"
Write-Host "Total suspected commented-out code lines: $totalCommentedCode"

Write-Host "`nFiles with most excessive blank lines:"
$filesWithExcessiveBlanks | Select-Object -First 10 | ForEach-Object { Write-Host $_ }

Write-Host "`nFiles with most commented-out code:"
$filesWithCommentedCode | Select-Object -First 10 | ForEach-Object { Write-Host $_ }
