$content = Get-Content -Encoding Default Source\Scene3D.cpp
$newContent = @()

foreach ($line in $content) {
    $newContent += $line
    if ($line -match "new Player3D\(`"Resource/Model/T\.mv1`", VGet\(-1200, 20\.0f, -1000\)") {
        $newContent += "    // --- TEST PORTAL PLACEMENT ---"
        $newContent += "    new StageObject(VGet(-1200, 20.0f, -1200), `"Resource/3D/ì]ëóêw/source/portal.mv1`", VGet(50.0f, 50.0f, 50.0f), `"`", 0.0f);"
    }
}
Set-Content -Encoding Default Source\Scene3D.cpp $newContent
