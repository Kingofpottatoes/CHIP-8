#include "chip8.h"
#include "platform.h"

int main(){
    Chip8 chip8;
    chip8_init(&chip8);
    initWindow();
    chip8_load_rom(&chip8, "/home/king/Code/Projects/CHIP-8/roms/7-beep.ch8");
    int i;
    while (1){
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


    return 0;
}