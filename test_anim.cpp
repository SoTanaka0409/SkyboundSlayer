#include "DxLib.h"
#include <stdio.h>
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
    ChangeWindowMode(TRUE);
    if(DxLib_Init() == -1) return -1;
    int model = MV1LoadModel("Resource/Model/monster.mv1");
    int animNum = MV1GetAnimNum(model);
    FILE* fp = fopen("anim_dump.txt", "w");
    fprintf(fp, "AnimNum: %d\n", animNum);
    for(int i=0; i<animNum; i++){
        fprintf(fp, "Anim %d: %s\n", i, MV1GetAnimName(model, i));
    }
    fclose(fp);
    DxLib_End();
    return 0;
}
