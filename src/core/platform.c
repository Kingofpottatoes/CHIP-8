#include <raylib.h>
#include <stdint.h>

static RenderTexture2D Screen;
static Color black={0,0,0,255};
static Color white={255,255,255,255};
static const int keymap[0xF+1]={
    [0x1]=KEY_ONE,
    [0x2]=KEY_TWO,
    [0x3]=KEY_THREE,
    [0xC]=KEY_FOUR,
    [0x4]=KEY_Q,
    [0x5]=KEY_W,
    [0x6]=KEY_E,
    [0xD]=KEY_R,
    [0x7]=KEY_A,
    [0x8]=KEY_S,
    [0x9]=KEY_D,
    [0xE]=KEY_F,
    [0xA]=KEY_Z,
    [0x0]=KEY_X,
    [0xB]=KEY_C,
    [0xF]=KEY_V

};


void DrawScreen(bool display[32][64]){
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

uint16_t UpdateKeyboard(){
    int key;
    uint16_t keyboard=0;
    for (key=0x0;key<16;key++){
        if (IsKeyDown(keymap[key])){
            keyboard=keyboard | (1<<key);
        }
    }
    return keyboard;
}

void initWindow(){
    InitWindow(640,320,"CHIP-8");
    SetTargetFPS(60);
    Screen=LoadRenderTexture(64, 32);
}

void DrawOneFrame(bool display[32][64]){
    BeginDrawing();
    DrawScreen(display);
    EndDrawing();
}
void closeWindow(){
    UnloadRenderTexture(Screen);
    CloseWindow();
}