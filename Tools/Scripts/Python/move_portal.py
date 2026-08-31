import codecs

with codecs.open('Source/GameManager.cpp', 'r', 'shift_jis') as f:
    lines = f.readlines()

new_lines = []
for i, line in enumerate(lines):
    if 'mCurrentPhase = Phase::SHOP_3;' in line:
        new_lines.append(line)
        new_lines.append('\t\t\t\tVECTOR playerPos = Config::GetStageCenter();\n')
        new_lines.append('\t\t\t\tif (Master::mpPlayer) {\n')
        new_lines.append('\t\t\t\t\tplayerPos = Master::mpPlayer->CastTo<Player3D>()->GetPosition();\n')
        new_lines.append('\t\t\t\t}\n')
        new_lines.append('\t\t\t\tVECTOR center = Config::GetStageCenter();\n')
        new_lines.append('\t\t\t\tVECTOR dir = VSub(playerPos, center);\n')
        new_lines.append('\t\t\t\tdir.y = 0.0f;\n')
        new_lines.append('\t\t\t\tif (VSize(dir) < 1.0f) dir = VGet(0.0f, 0.0f, 1.0f);\n')
        new_lines.append('\t\t\t\telse dir = VNorm(dir);\n')
        new_lines.append('\t\t\t\tmBossPortalPos = VAdd(center, VScale(dir, -2000.0f));\n')
    elif 'VECTOR teleporterPos = VAdd(Config::GetStageCenter(), VGet(0.0f, 0.0f, 800.0f));' in line:
        new_lines.append(line.replace('VECTOR teleporterPos = VAdd(Config::GetStageCenter(), VGet(0.0f, 0.0f, 800.0f));', 'VECTOR teleporterPos = mBossPortalPos;'))
    else:
        new_lines.append(line)

with codecs.open('Source/GameManager.cpp', 'w', 'shift_jis') as f:
    f.writelines(new_lines)
