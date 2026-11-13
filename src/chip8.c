#include "chip8.h"
#include <stdio.h>

void chip8_init(Chip8 *p){
    *p = (Chip8){
        .pc=0x200,//rom start at this adress
    };
}

bool chip8_load_rom(Chip8 *p,const char * rom_path){
    FILE * file=fopen(rom_path,"rb");
    if (file==NULL){
        return false;
    }
    size_t result=fread(p->memory+0x200,1,4096-0x200,file);
    int bit=fgetc(file);
    if (!(bit==feof(file))){
        return false;
    }
    if (result==0){
        //file empty
        return false;
    }
    fclose(file);
    
    return true;
}