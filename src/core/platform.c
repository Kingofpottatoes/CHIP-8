#include <raylib.h>
#include <stdint.h>

static RenderTexture2D Screen;
static Color black={0,0,0,255};
static Color white={255,255,255,255};



void DrawScreen(RenderTexture2D Screen,bool display[32][64]){
    int i,j;
    Color color;
    BeginTextureMode(Screen);
    for (j=0;j<32;j++){
        for (i=0;i<64;i++){
            if (display[j][i]){
                color=white;
            }
            else{
                color=black;
            }
            DrawPixel(i,j, color);
        }
    }
    EndTextureMode();
    DrawTexturePro(Screen.texture,
                (Rectangle){0,0,64,-32},
                (Rectangle){0,0,640,320},
                (Vector2){0,0},
                0,
                (Color){255,255,255,255}
            );

}

void initWindow(){
    InitWindow(640,320,"CHIP-8");
    SetTargetFPS(60);
    Screen=LoadRenderTexture(64, 32);
}

void DrawOneFrame(bool display[32][64]){
    BeginDrawing();
    DrawScreen(Screen,display);
    EndDrawing();
}
