$source_dir = 'c:\Users\Admin\Desktop\progtram\LevelMonster\Source'
$files = Get-ChildItem -Path $source_dir -Filter "*.cpp"
$sjis = [System.Text.Encoding]::GetEncoding("shift_jis")

foreach ($file in $files) {
    $content = [System.IO.File]::ReadAllLines($file.FullName, $sjis)
    $newLines = @()
    $modified = $false
    
    $braceLevel = 0
    $loopLevels = @()
    
    for ($i = 0; $i -lt $content.Count; $i++) {
        $line = $content[$i]
        
        # mpTarget fix for Player3D.cpp
        if ($file.Name -eq "Player3D.cpp" -and $line -match "^\s*Test\(\);\s*$") {
            $newLines += "		if (mpTarget != nullptr)"
            $newLines += "		{"
            $newLines += "			bool isTargetValid = false;"
            $newLines += "			auto mpEneList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);"
            $newLines += "			for (int i = 0; i < mpEneList.size(); i++)"
            $newLines += "			{"
            $newLines += "				if (mpTarget == mpEneList.at(i) && !mpTarget->IsDeleteFlag())"
            $newLines += "				{"
            $newLines += "					isTargetValid = true;"
            $newLines += "					break;"
            $newLines += "				}"
            $newLines += "			}"
            $newLines += "			if (!isTargetValid)"
            $newLines += "			{"
            $newLines += "				mpTarget = nullptr;"
            $newLines += "			}"
            $newLines += "		}"
            $modified = $true
        }
        
        if ($line -match "\{") {
            $braceLevel += ($line.ToCharArray() | Where-Object { $_ -eq '{' }).Count
        }
        if ($line -match "\}") {
            $braceLevel -= ($line.ToCharArray() | Where-Object { $_ -eq '}' }).Count
            $loopLevels = $loopLevels | Where-Object { $_ -le $braceLevel }
        }
        
        if ($line -match "for\s*\(" -or $line -match "while\s*\(") {
            if ($line -match "\{") {
                $loopLevels += $braceLevel
            } else {
                $loopLevels += ($braceLevel + 1)
            }
        }
        
        $newLines += $line
        
        if ($line -notmatch "^\s*//") {
            $match = [regex]::Match($line, "(?:auto|[\w:]+\s*\*)\s+(\w+)\s*=\s*dynamic_cast<")
            if ($match.Success) {
                $varName = $match.Groups[1].Value
                
                $hasCheck = $false
                for ($j = 1; $j -le 3; $j++) {
                    if (($i + $j) -lt $content.Count) {
                        $nextLine = $content[$i + $j]
                        if ($nextLine -match "nullptr" -or $nextLine -match "NULL" -or $nextLine -match "!\s*$varName" -or $nextLine -match "if\s*\(\s*$varName\s*\)") {
                            $hasCheck = $true
                            break
                        }
                    }
                }
                
                if (-not $hasCheck) {
                    $indent = $line.Substring(0, $line.Length - $line.TrimStart().Length)
                    $action = if ($loopLevels.Count -gt 0) { "continue;" } else { "return;" }
                    $checkLine = $indent + "if (" + $varName + " == nullptr) { " + $action + " }"
                    $newLines += $checkLine
                    $modified = $true
                    Write-Host "File: $($file.Name) | Line: $($i+1)"
                    Write-Host "  $($line.Trim())"
                    Write-Host "  + $($checkLine.Trim())"
                }
            }
        }
    }
    
    if ($modified) {
        [System.IO.File]::WriteAllLines($file.FullName, $newLines, $sjis)
    }
}