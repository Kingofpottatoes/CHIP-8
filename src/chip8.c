#include "chip8.h"
#include <stdio.h>
#include <string.h>

void chip8_init(Chip8 *p){
    static const uint8_t fontset[80]={0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                     0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
                        };
    *p = (Chip8){
        .pc=0x200,//roms start here
    };
    memcpy(p->memory+0x050,fontset,sizeof(fontset));//load font in memory
}

bool chip8_load_rom(Chip8 *p,const char * rom_path){
    FILE * file=fopen(rom_path,"rb");
    if (file==NULL){
        return false;
    }
    size_t result=fread(p->memory+0x200,1,4096-0x200,file);
    int bit=fgetc(file);
    fclose(file);
    if (!(bit==EOF)){
        return false;
    }
    if (result==0){
        //file empty
        return false;
    }
    
    return true;
}
void chip8_cycle(Chip8 *p){
    //FETCH
    uint8_t instruction_one=p->memory[p->pc];
    uint8_t instruction_two=p->memory[p->pc+1];
    uint16_t instruction= (instruction_one<<8)|| instruction_two;
    p->pc+=2;
    //DECODE AND EXECUTE
    uint8_t first_quartet=instruction & 0b1111000000000000>>12;//mask to only keep the first octet, and move it to the left for more lisibility
    uint8_t second_quartet=instruction & 0b0000111100000000>>8;
    uint8_t third_quartet=instruction & 0b0000000011110000>>4;
    uint8_t last_quartet=instruction & 0b0000000000001111;
    uint8_t two_last_quartet=instruction & 0b0000000011111111;
    uint8_t three_last_quartet=instruction & 0b0000111111111111;
    switch ((first_quartet)){
        case 0x0:{
            int i,j;
            for (i=0;i<32;i++){
                for (j=0;j<64;j++){
                    p->display[i][j]=false;
                }
            }
            break;
        }
        case 0x1:{
            break;
        }
        case 0x2:{
            break;
        }
        case 0x3:{
            break;
        }
        case 0x4:{
            break;
        }
        case 0x5:{
            break;
        }
        case 0x6:{
            break;
        }
        case 0x7:{
            break;
        }
        case 0x8:{
            break;
        }
        case 0x9:{
            break;
        }
        case 0xA:{
            break;
        }
        case 0xB:{
            break;
        }
        case 0xC:{
            break;
        }
        case 0xD:{
            break;
        }
        case 0xE:{
            break;
        }
        case 0xF:{
            break;
        }
    }
}