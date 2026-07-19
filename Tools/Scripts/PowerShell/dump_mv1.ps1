param([string]$FilePath)
$bytes = [System.IO.File]::ReadAllBytes($FilePath)
$text = [System.Text.Encoding]::ASCII.GetString($bytes)
$text = $text -replace '[^\x20-\x7E]', ' '

$matches = [regex]::Matches($text, '([a-zA-Z0-9_\-\.\/\\:]+\.(?:jpg|png|tga|bmp))')
$results = @()
foreach ($m in $matches) {
    $results += $m.Value
}
$results | Select-Object -Unique
