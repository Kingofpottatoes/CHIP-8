#include "chip8.h"
#include "platform.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static Chip8 chip8;


void RunFrame(){
    int i;
    if (chip8.delay_timer>0){chip8.delay_timer-=1;}
    if (chip8.sound_timer>0){chip8.sound_timer-=1;}
    platform_beep(chip8.sound_timer>0);
    chip8.keyboard=UpdateKeyboard();
    for (i=0;i<11;i++){
        bool DXYN=chip8_cycle(&chip8);
        if (DXYN){
            break;
        }
    }
    DrawOneFrame(chip8.display);
}




int main(){
    chip8_init(&chip8);
    chip8_load_rom(&chip8, "roms/IBM.ch8");
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