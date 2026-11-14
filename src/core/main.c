#include "chip8.h"
#include "platform.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static Chip8 chip8;
static bool rom_loaded=false;


void RunFrame(){
    int i;
    if (chip8.delay_timer>0){chip8.delay_timer-=1;}
    if (chip8.sound_timer>0){chip8.sound_timer-=1;}
    platform_beep(chip8.sound_timer>0);
    #ifndef __EMSCRIPTEN__
    chip8.keyboard=UpdateKeyboard();
    #endif
    for (i=0;i<11;i++){
        bool DXYN=chip8_cycle(&chip8);
        if (DXYN){
            break;
        }
    }
    DrawOneFrame(chip8.display);
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
void web_load_rom(){
    chip8_init(&chip8);
    chip8_load_rom(&chip8,"/rom.ch8");
    rom_loaded=true;
}
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
void web_set_keyboard(int state){
    chip8.keyboard=state;
}
#endif


int main(){
    chip8_init(&chip8);
    chip8_load_rom(&chip8, "roms/6-keypad.ch8");
    initWindow();
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(RunFrame, 60, 1);
    
    #else
    while (1){
        RunFrame();
    }



    #endif
    return 0;
}