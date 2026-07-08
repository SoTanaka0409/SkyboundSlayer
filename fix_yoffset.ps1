# 1. Update StageObject.h
$hContent = Get-Content -Encoding Default Source\StageObject.h
$newHContent = @()
foreach ($line in $hContent) {
    if ($line -match "StageObject\(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename = `"`", float hitRadius = 0\.0f\);") {
        $newHContent += "	StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename = `"`", float hitRadius = 0.0f, float yOffset = 0.0f);"
    } elseif ($line -match "bool mbHasFollowedTerrain;") {
        $newHContent += $line
        $newHContent += "	float mYOffset;"
    } else {
        $newHContent += $line
    }
}
Set-Content -Encoding Default Source\StageObject.h $newHContent

# 2. Update StageObject.cpp
$cppContent = Get-Content -Encoding Default Source\StageObject.cpp
$newCppContent = @()
foreach ($line in $cppContent) {
    if ($line -match "StageObject::StageObject\(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius\)") {
        $newCppContent += "StageObject::StageObject(VECTOR initPos, std::string filename, VECTOR scale, std::string textureFilename, float hitRadius, float yOffset)"
    } elseif ($line -match ":Object3D\(initPos\), mfHitRadius\(hitRadius\), mbHasFollowedTerrain\(false\)") {
        $newCppContent += "	:Object3D(initPos), mfHitRadius(hitRadius), mbHasFollowedTerrain(false), mYOffset(yOffset)"
    } elseif ($line -match "TerrainFollow\(\);") {
        $newCppContent += $line
        $newCppContent += "		position_.y += mYOffset;"
    } else {
        $newCppContent += $line
    }
}
Set-Content -Encoding Default Source\StageObject.cpp $newCppContent

# 3. Update GameManager.cpp to use yOffset
$gmContent = Get-Content -Encoding Default Source\GameManager.cpp
$newGmContent = @()
foreach ($line in $gmContent) {
    if ($line -match "`"Resource/3D/ì]ëóêw/source/portal\.mv1`"") {
        $newGmContent += $line -replace "VAdd\(mBossPortalPos, VGet\(0\.0f, -5\.0f, 0\.0f\)\)", "mBossPortalPos" -replace "0\.0f\);", "0.0f, -20.0f);"
    } else {
        $newGmContent += $line
    }
}
Set-Content -Encoding Default Source\GameManager.cpp $newGmContent
