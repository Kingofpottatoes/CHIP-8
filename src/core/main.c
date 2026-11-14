#include "chip8.h"
#include "platform.h"

int main(){
    Chip8 chip8;
    chip8_init(&chip8);
    initWindow();
    chip8_load_rom(&chip8, "/home/king/Code/Projects/CHIP-8/roms/IBM.ch8");
    while (1){
        chip8_cycle(&chip8);
        DrawOneFrame(chip8.display);
        
    }


    return 0;
}