#ifndef CHIP8_H
#define CHIP8_H
#include <stdint.h>
#include <stdbool.h>

//thats our chip :)
typedef struct {
    uint8_t memory[4096];//memory
    uint8_t V[16];//cpu vars
    uint16_t I;//memory adress used by instruction
    uint16_t pc; //current instruction id
    uint16_t stack[16]; //the stack
    uint8_t sp;//stack top
    uint8_t delay_timer,sound_timer; //timers decremented at 60Hz
    bool display[32][64]; //the screen
    uint16_t keyboard; //the keyboard 
    //and thats all :)
}Chip8;

void chip8_init(Chip8 *p);
bool chip8_load_rom(Chip8 *p,const char *);
void chip8_cycle(Chip8 *p);
#endif