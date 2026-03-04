#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

struct Chip8;
void InitChip8(struct Chip8* chip);
bool LoadRom(struct Chip8* chip, const char* filename); // returns 1/true on failure
void ExecOpcode(struct Chip8* chip, uint16_t code);
void Chip8Cycle(struct Chip8* chip);

struct Chip8 {
    uint16_t RegisterI;
    uint16_t Stack[16];
    uint16_t ProgramCounter;

    uint8_t Memory[4096];

    uint8_t RegistersVx[16];
    uint8_t RegisterDelay;
    uint8_t RegisterSound;
    
    uint8_t StackPointer;

    bool Display[64][32];
    bool Keypad[16];
    bool DrawFlag;
};

#endif
