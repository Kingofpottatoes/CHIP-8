#include "chip8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
void chip8_display(Chip8 *p){
    char display[32*65+1];
    int x,y;
    for (y=0;y<32;y++){
        for(x=0;x<64;x++){
            if (p->display[y][x]){
                display[y*65+x]='#';
            }
            else{display[y*65 +x]=' ';}
        }
        display[y*65+x]='\n';
    }
    display[32*65]='\0';
    printf("%s",display);
}
bool chip8_cycle(Chip8 *p){
    //FETCH
    uint8_t instruction_one=p->memory[p->pc];
    uint8_t instruction_two=p->memory[p->pc+1];
    uint16_t instruction= (instruction_one<<8) | instruction_two;
    p->pc+=2;
    //DECODE AND EXECUTE
    uint8_t first_quartet=(instruction & 0b1111000000000000)>>12;//mask to only keep the first octet, and move it to the left for more lisibility
    uint8_t second_quartet=(instruction & 0b0000111100000000)>>8;
    uint8_t third_quartet=(instruction & 0b0000000011110000)>>4;
    uint8_t last_quartet=instruction & 0b0000000000001111;
    uint8_t two_last_quartet=instruction & 0b0000000011111111;
    uint16_t three_last_quartet=instruction & 0b0000111111111111;
    switch ((first_quartet)){
        case 0x0:{
            if (instruction==0x00E0){
                int i,j;
                for (i=0;i<32;i++){
                    for (j=0;j<64;j++){
                        p->display[i][j]=false;
                    }
                }
            }
            else if (instruction==0x00EE){
                if (p->sp==0){printf("%03X underflow \n",p->pc);break;}
                p->sp-=1;
                p->pc=p->stack[p->sp];
            }
            else{
                printf("Unknow opcode %04X",instruction);
            }
            break;
        }
        case 0x1:{
            p->pc=three_last_quartet;
            break;
        }
        case 0x2:{
            if (p->sp<16){
                p->stack[p->sp]=p->pc;
                p->sp+=1;
                p->pc=three_last_quartet;
            }else{printf("%03X overflow \n",p->pc);break;}
            break;
        }
        case 0x3:{
            if (p->V[second_quartet]==two_last_quartet){
                p->pc+=2;
            }
            break;
        }
        case 0x4:{
            if (p->V[second_quartet]!=two_last_quartet){
                p->pc+=2;
            }
            break;
        }
        case 0x5:{
            if (p->V[second_quartet]==p->V[third_quartet]){
                p->pc+=2;
            }
            break;
        }
        case 0x6:{
            p->V[second_quartet]=two_last_quartet;
            break;
        }
        case 0x7:{
            p->V[second_quartet]+=two_last_quartet;
            break;
        }
        case 0x8:{
            switch (last_quartet) {
                case 0:{
                    p->V[second_quartet]=p->V[third_quartet];
                    break;
                }
                case 1:{
                    p->V[second_quartet]=p->V[second_quartet] | p->V[third_quartet];
                    p->V[0xF]=0;
                    break;
                }
                case 2:{
                    p->V[second_quartet]=p->V[second_quartet] & p->V[third_quartet];
                    p->V[0xF]=0;
                    break;
                }
                case 3:{
                    p->V[second_quartet]=p->V[second_quartet] ^ p->V[third_quartet];
                    p->V[0xF]=0;
                    break;
                }
                case 4:{
                    uint8_t r;
                    uint8_t VX=p->V[second_quartet];
                    p->V[second_quartet]=p->V[second_quartet] + p->V[third_quartet];
                    if (p->V[second_quartet]<VX){r=1;}else{r=0;}
                    p->V[0xF]=r;
                    break;
                }
                case 5:{
                    uint8_t r;
                    if (p->V[second_quartet] >= p->V[third_quartet]){r=1;}else{r=0;}
                    p->V[second_quartet]=p->V[second_quartet] - p->V[third_quartet];
                    p->V[0xF]=r;
                    break;
                }
                case 6:{
                    uint8_t tmp=p->V[third_quartet] & 0b0001;
                    p->V[second_quartet]=p->V[third_quartet]>>1;
                    p->V[0xF]=tmp;
                    break;
                }
                case 7:{
                    uint8_t r;
                    if (p->V[third_quartet] >= p->V[second_quartet]){r=1;}else{r=0;}
                    p->V[second_quartet]=p->V[third_quartet] - p->V[second_quartet];
                    p->V[0xF]=r;
                    break;
                }
                case 14:{//CASE E
                    uint8_t tmp=(p->V[third_quartet]>>7);
                    p->V[second_quartet]=p->V[third_quartet]<<1;
                    p->V[0xF]=tmp;
                    break;
                }
                default:{
                    printf("Unknow opcode %04X",instruction);
                }
            }
            break;
        }
        case 0x9:{
            if(p->V[second_quartet]!=p->V[third_quartet]){
                p->pc+=2;
            }
            break;
        }
        case 0xA:{
            p->I=three_last_quartet;
            break;
        }
        case 0xB:{
            p->pc=three_last_quartet+p->V[0];
            break;
        }
        case 0xC:{
            uint8_t rdm=rand()%256;
            p->V[second_quartet]=rdm & two_last_quartet;
            break;
        }
        case 0xD:{
            int i,j;
            uint16_t X=p->V[second_quartet]%64;
            uint16_t Y=p->V[third_quartet]%32;
            p->V[0xF]=0;
            for (i=0;i<last_quartet;i++){
                if(Y+i>=32){break;}
                uint8_t sprite=p->memory[p->I+i];
                for (j=0;j<8;j++){
                    if(X+j>=64){break;}
                    if (((sprite >> (7-j)) & 1)&&(p->display[Y+i][X+j])){
                        p->V[0xF]=1;
                    }
                    if(((sprite >> (7-j)) & 1)){
                        p->display[Y+i][X+j]=!p->display[Y+i][X+j];
                    }
                }
            }
            return true;
            break;
        }
        case 0xE:{
            switch (last_quartet){
                case 14:{
                    if((p->keyboard >> p->V[second_quartet]) & 1){
                        p->pc+=2;
                    }
                    break;
                }
                case 1:{
                    if(!((p->keyboard >> p->V[second_quartet]) & 1)){
                        p->pc+=2;
                    }
                    break;
                }
                default:{
                    printf("Unknow opcode %04X",instruction);
                }
            }
            break;
        }
        case 0xF:{
            switch (two_last_quartet) {
                case 7 :{
                    p->V[second_quartet]=p->delay_timer;
                    break;
                }
                case 0x0A:{
                    if (!(p->keyboard==0)){
                        int i;
                        for (i=0;i<16;i++){
                            if (p->keyboard>>i & 1){//is the ith key pressed ?
                                p->V[second_quartet]=i;
                            }
                        }
                    }
                    else {p->pc-=2;}
                    break;
                }
                case 0x15:{
                    p->delay_timer=p->V[second_quartet];
                    break;
                }
                case 0x18:{
                    p->sound_timer=p->V[second_quartet];
                    break;
                }
                case 0x1E:{
                    p->I+=p->V[second_quartet];
                    break;
                }
                case 0x29:{
                    p->I=0x050+p->V[second_quartet]*5;//adresse of font VX
                    break;
                }
                case 0x33:{
                    int cents=p->V[second_quartet]/100;
                    int dec=(p->V[second_quartet]%100)/10;
                    int units=p->V[second_quartet]%10;
                    p->memory[p->I]=cents;
                    p->memory[p->I+1]=dec;
                    p->memory[p->I+2]=units;
                    break;
                }
                case 0x55:{
                    int i;
                    for (i=0;i<=second_quartet;i++){
                        p->memory[p->I+i]=p->V[i];
                    }
                    p->I+=second_quartet+1;
                    break;
                }
                case 0x65:{
                    int i;
                    for (i=0;i<=second_quartet;i++){
                        p->V[i]=p->memory[p->I+i];
                    }
                    p->I+=second_quartet+1;
                    break;
                }
                default:{
                    printf("Unknow opcode %04X",instruction);
                }
        
            
            }
            break;
        }
        default:{
            //if this ever happen im loosing my mind
            //probably a cosmic flip
            printf("Unknow first quartet, HOW DID YOU DO THAT :%04X \n",instruction);
        }
    }
    return false;
}