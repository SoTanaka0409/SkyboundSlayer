$files = Get-ChildItem -Path "Source" -Include *.cpp,*.h -Recurse

$utf8Bom = New-Object System.Text.UTF8Encoding($true)

foreach ($f in $files) {
    try {
        # Read the file as UTF-8
        $lines = [System.IO.File]::ReadAllLines($f.FullName, [System.Text.Encoding]::UTF8)
        
        $newLines = @()
        $consecutiveBlanks = 0
        
        foreach ($line in $lines) {
            if ($line.Trim() -eq "") {
                $consecutiveBlanks++
                # Keep up to 2 consecutive blank lines
                if ($consecutiveBlanks -le 2) {
                    $newLines += $line
                }
            } else {
                $consecutiveBlanks = 0
                $newLines += $line
            }
        }
        
        # Write back to file with UTF-8 BOM
        [System.IO.File]::WriteAllLines($f.FullName, $newLines, $utf8Bom)
        
    } catch {
        Write-Host "Error processing $($f.Name): $_"
    }
}

Write-Host "Cleanup and UTF-8 BOM conversion completed."
