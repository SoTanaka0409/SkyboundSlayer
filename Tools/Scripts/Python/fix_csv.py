import math
import random

lines = []
lines.append("Type,Model,X,Y,Z,ScaleX,ScaleY,ScaleZ,Texture,ColSize,IsRelative")

# Center Tree
lines.append("StageObject,Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1,0.0,0.0,0.0,5.0,10.0,5.0,,750.0,1")

# 15 Trees (Fibonacci)
numTrees = 15
maxRadius = 5000.0
goldenAngle = 137.507764 * (math.pi / 180.0)

for i in range(numTrees):
    r = maxRadius * math.sqrt((i + 0.5) / numTrees)
    angle = i * goldenAngle
    x = math.cos(angle) * r
    z = math.sin(angle) * r
    lines.append(f"StageObject,Resource/3D/Tree_central/tree-gn/source/TreeGen.mv1,{x:.1f},0.0,{z:.1f},1.0,1.0,1.0,,150.0,1")

# 20 Rocks
for i in range(20):
    angle = random.randint(0, 359) * (math.pi / 180.0)
    r = -2000.0 + random.randint(0, 4000)
    x = math.cos(angle) * r
    z = math.sin(angle) * r
    scale = 30.0 + random.randint(0, 40)
    lines.append(f"Stage,Resource/3D/Rock/mossy-rock/source/Rock Low Poly.mv1,{x:.1f},0.0,{z:.1f},{scale:.1f},{scale:.1f},{scale:.1f},Resource/3D/Rock/mossy-rock/source/rock1_color.jpg,,1")

# Castle Wall
lines.append("StageObject,Resource/3D/Stage_casule/source/Parede castelo.mv1,0.0,0.0,0.0,10.0,10.0,10.0,,,0")

import os
with open("Resource/CSV/stage_objects.csv", "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
print("CSV fixed.")
