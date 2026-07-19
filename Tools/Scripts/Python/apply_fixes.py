import os

def read_file(path):
    with open(path, 'r', encoding='cp932') as f:
        return f.read()

def write_file(path, content):
    with open(path, 'w', encoding='cp932') as f:
        f.write(content)

# 1. Player3D.h
p3d_h = read_file('Source/Player3D.h')
p3d_h = p3d_h.replace('#include"EffectU.h"\n', '')
p3d_h = p3d_h.replace('\tEffectU* mpEffectU;\n', '')
p3d_h = p3d_h.replace('\tbool EffectUflag;//エフェクトUが実行されたとき\n\tint EffectUCount;\n', '')
write_file('Source/Player3D.h', p3d_h)

# 2. Player3D.cpp
p3d_c = read_file('Source/Player3D.cpp')
p3d_c = p3d_c.replace('\t, EffectUflag(false)\n\t, EffectUCount(0)\n', '')
p3d_c = p3d_c.replace('\tmpEffectU = new EffectU();\n\tmpEffectU->Load();\n', '')
write_file('Source/Player3D.cpp', p3d_c)

# 3. GameManager.h
gm_h = read_file('Source/GameManager.h')
gm_h = gm_h.replace('    void Draw();\n', '    void Draw();\n    void DrawMinimap();\n')
write_file('Source/GameManager.h', gm_h)

# 4. GameManager.cpp
gm_c = read_file('Source/GameManager.cpp')
gm_c = gm_c.replace('#include "Item.h"', '#include "Item.h"\n#include "EffekseerObject.h"')

portal_str = '''    // Portal Base
    float portalSize = 100.0f;
    new Stage(VAdd(mBossPortalPos, VGet(0.0f, -570.0f, 0.0f)), "Resource/3D/転送陣/source/portal.mv1", "Resource/3D/転送陣/source/portal.mv1", VGet(portalSize, portalSize, portalSize));'''
portal_new_str = portal_str + '''

    // Magic Circle Effect
    new EffekseerObject("Mahoujin", "Resource/effect/MAGICAL/魔法陣.efkproj", VAdd(mBossPortalPos, VGet(0.0f, 10.0f, 0.0f)), nullptr, false);'''
gm_c = gm_c.replace(portal_str, portal_new_str)

draw_end_str = '''        DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}'''

minimap_str = '''        DrawBox(0, 0, Config::ScreenWidth, Config::ScreenHeight, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    
    // Draw Minimap
    DrawMinimap();
}

void GameManager::DrawMinimap()
{
    // Minimap Background
    int mapRadius = 130;
    int mapCenterX = Config::ScreenWidth - mapRadius - 20;
    int mapCenterY = mapRadius + 20;
    
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawCircle(mapCenterX, mapCenterY, mapRadius, GetColor(0, 0, 0), TRUE);
    DrawCircle(mapCenterX, mapCenterY, mapRadius, GetColor(255, 255, 255), FALSE); // Border
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    
    auto p = Master::mpPlayer;
    if (!p) return;
    Player3D* player = p->CastTo<Player3D>();
    if (!player) return;
    
    VECTOR pPos = player->GetPosition();
    float mapScale = 120.0f / 8000.0f; // Max range 8000 units maps to 120 pixels
    
    auto WorldToMap = [&](VECTOR wPos, int& outX, int& outY) {
        float dx = wPos.x - pPos.x;
        float dz = wPos.z - pPos.z;
        
        float mapDx = dx * mapScale;
        float mapDy = -dz * mapScale;
        
        float dist = (float)sqrt(mapDx*mapDx + mapDy*mapDy);
        if (dist > mapRadius) {
            mapDx = (mapDx / dist) * mapRadius;
            mapDy = (mapDy / dist) * mapRadius;
        }
        
        outX = mapCenterX + int(mapDx);
        outY = mapCenterY + int(mapDy);
    };
    
    // Boss Portal
    int px, py;
    WorldToMap(mBossPortalPos, px, py);
    DrawCircle(px, py, 6, GetColor(255, 255, 0), TRUE);
    DrawString(px + 8, py - 8, "PORTAL", GetColor(255, 255, 0));
    
    // Enemies
    const auto& enemies = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::Tag3D_Enemy3D);
    for (auto e : enemies) {
        int ex, ey;
        WorldToMap(e->GetPosition(), ex, ey);
        DrawCircle(ex, ey, 4, GetColor(255, 0, 0), TRUE);
    }
    
    // Player
    VECTOR pRot = player->GETRotation();
    DrawCircle(mapCenterX, mapCenterY, 5, GetColor(0, 255, 0), TRUE);
    
    // Player facing direction line
    float yaw = pRot.y - DX_PI_F; // Remove the 180 degree offset added in Player3D
    int lineX = mapCenterX + int(sin(yaw) * 15.0f);
    int lineY = mapCenterY - int(cos(yaw) * 15.0f);
    DrawLine(mapCenterX, mapCenterY, lineX, lineY, GetColor(0, 255, 0));
}'''
gm_c = gm_c.replace(draw_end_str, minimap_str)

write_file('Source/GameManager.cpp', gm_c)
print('Done!')