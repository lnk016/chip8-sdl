#include "chip8.h"
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

uint8_t Font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
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

void InitChip8(struct Chip8* chip){
    memset(chip, 0, sizeof(struct Chip8));

    chip->ProgramCounter = 0x200;

    for (int i = 0; i < 80; i ++){
        chip->Memory[i] = Font[i];
    }

    srand(time(NULL));
}

bool LoadRom(struct Chip8* chip, const char* filename){
    FILE* file = fopen(filename, "rb");
    if (file == NULL){
        printf("Failed to open ROM\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size > 4096 - 0x200){
        printf("ROM too large");
        fclose(file);
        return 1;
    }

    fread(&chip->Memory[0x200], 1, size, file);
    fclose(file);

    return 0;
}

void Chip8Cycle(struct Chip8* chip){
    uint16_t opcode = (chip->Memory[chip->ProgramCounter] << 8) | chip->Memory[chip->ProgramCounter + 1];
    chip->ProgramCounter += 2;

    ExecOpcode(chip, opcode);

    return;
}

void ExecOpcode(struct Chip8* chip, uint16_t code){
    uint16_t nnn = (code & 0x0FFF);
    uint8_t n = (code & 0x000F);
    uint8_t x = (code & 0x0F00) >> 8;
    uint8_t y = (code & 0x00F0) >> 4;
    uint16_t kk = (code & 0x00FF);
    uint8_t head = (code & 0xF000) >> 12;

    switch (head){
        case 0x0:
            switch (code){
                case 0x00E0:
                    memset(chip->Display, 0, sizeof(chip->Display));
                    break;
                case 0x00EE:
                    if (chip->StackPointer > 0){
                        chip->StackPointer--;
                        chip->ProgramCounter = chip->Stack[chip->StackPointer];
                    }
                    break;

                default:
                    printf("Unknown opcode: %04X\n", code);
                    break;

            }
            break;

        case 0x1:
            chip->ProgramCounter = nnn;
            break;

        case 0x2:
            if (chip->StackPointer < 16){
                chip->Stack[chip->StackPointer] = chip->ProgramCounter;
                chip->StackPointer++;
                chip->ProgramCounter = nnn;
            }
            break;

        case 0x3:
            if (chip->RegistersVx[x] == kk){
                chip->ProgramCounter += 2;
            }
            break;

        case 0x4:
            if (chip->RegistersVx[x] != kk){
                chip->ProgramCounter += 2;
            }
            break;

        case 0x5:
            if (n == 0 && (chip->RegistersVx[x] == chip->RegistersVx[y])){
                chip->ProgramCounter += 2;
            }
            break;

        case 0x6:
            chip->RegistersVx[x] = kk;
            break;

        case 0x7:
            chip->RegistersVx[x] += kk;
            break;

        case 0x8:
            switch (n){
                case 0x0:
                    chip->RegistersVx[x] = chip->RegistersVx[y];
                    break;

                case 0x1:
                    chip->RegistersVx[x] |= chip->RegistersVx[y];
                    break;

                case 0x2:
                    chip->RegistersVx[x] &= chip->RegistersVx[y];
                    break;

                case 0x3:
                    chip->RegistersVx[x] ^= chip->RegistersVx[y];
                    break;

                case 0x4:
                    int sum = chip->RegistersVx[x] + chip->RegistersVx[y];
                    
                    chip->RegistersVx[0xF] = (sum > 255) ? 1 : 0;

                    chip->RegistersVx[x] = sum & 0xFF;
                    break;

                case 0x5:
                    chip->RegistersVx[0xF] = (chip->RegistersVx[x] >= chip->RegistersVx[y]) ? 1 : 0;

                    chip->RegistersVx[x] -= chip->RegistersVx[y];
                    break;

                case 0x6:
                    chip->RegistersVx[0xF] = chip->RegistersVx[x] & 0x1;

                    chip->RegistersVx[x] >>= 1;
                    break;

                case 0x7:
                    chip->RegistersVx[0xF] = (chip->RegistersVx[y] >= chip->RegistersVx[x]) ? 1 : 0;

                    chip->RegistersVx[x] = chip->RegistersVx[y] - chip->RegistersVx[x];
                    break;

                case 0xE:
                    chip->RegistersVx[0xF] = (chip->RegistersVx[x] & 0x80) >> 7;
                    
                    chip->RegistersVx[x] <<= 1;
                    break;

                default:
                    printf("Unknown opcode: %04X\n", code);
                    break;

            }
            break;

        case 0x9:
            if ( n == 0 && (chip->RegistersVx[x] != chip->RegistersVx[y])){
                chip->ProgramCounter += 2;
            }
            break;

        case 0xA:
            chip->RegisterI = nnn;
            break;

        case 0xB:
            chip->ProgramCounter = chip->RegistersVx[0] + nnn;
            break;

        case 0xC:
           chip->RegistersVx[x] = (rand() % 256) & kk;
           break;

        case 0xD:
            uint8_t posX = chip->RegistersVx[x] & 63;
            uint8_t posY = chip->RegistersVx[y] & 31;
            chip->RegistersVx[0xF] = 0;
            for (int row = 0; row < n; row++){
                int t1 = posY + row;
                if (t1 >= 32){
                    break;
                }

                uint8_t sprite = chip->Memory[chip->RegisterI + row];

                for (int bit = 0; bit < 8; bit++){
                    int t2 = posX + bit;
                    if (t2 >= 64){
                        break;
                    }

                    if (sprite & (0x80 >> bit)){
                        if (chip->Display[t2][t1]){
                            chip->RegistersVx[0xF] = 1;
                        }
                        
                        chip->Display[t2][t1] ^= 1;
                    }
                }
            }

            chip->DrawFlag = true;
            break;
        
        case 0xE:
            switch(kk){
                case 0x9E:
                    if (chip->Keypad[chip->RegistersVx[x]] == 1){
                        chip->ProgramCounter += 2;
                    }
                    break;

                case 0xA1:
                    if (chip->Keypad[chip->RegistersVx[x]] != 1){
                        chip->ProgramCounter += 2;
                    }
                    break;

                default:
                    printf("Unknown opcode: %04X\n", code);
                    break;

            }
            break;

        case 0xF:
            switch(kk){
                case 0x07:
                    chip->RegistersVx[x] = chip->RegisterDelay;
                    break;

                case 0x0A:
                    bool KeyPressed = false;
                    for (int i = 0; i < 16; i++){
                        if (chip->Keypad[i]){
                            chip->RegistersVx[x] = i;
                            KeyPressed = true;
                            break;
                        }
                    }
                    if (KeyPressed == false){
                        chip->ProgramCounter -= 2;
                    }
                    break;

                case 0x15:
                    chip->RegisterDelay = chip->RegistersVx[x];
                    break;

                case 0x18:
                    chip->RegisterSound = chip->RegistersVx[x];
                    break;

                case 0x1E:
                    chip->RegisterI += chip->RegistersVx[x];
                    break;

                case 0x29:
                    chip->RegisterI = chip->RegistersVx[x] * 5;
                    break;

                case 0x33:
                    chip->Memory[chip->RegisterI] = chip->RegistersVx[x] / 100;
                    chip->Memory[chip->RegisterI + 1] = (chip->RegistersVx[x] / 10) % 10;
                    chip->Memory[chip->RegisterI + 2] = chip->RegistersVx[x] % 10;
                    break;

                case 0x55:
                    for (int i = 0; i <= x; i++){
                        chip->Memory[chip->RegisterI + i] = chip->RegistersVx[i];
                    }
                    break;

                case 0x65:
                    for (int i = 0; i <= x; i++){
                        chip->RegistersVx[i] = chip->Memory[chip->RegisterI + i];
                    }
                    break;

                default:
                    printf("Unknown opcode: %04X\n", code);
                    break;
            }
            break;

        default:
            printf("Unknown opcode: %04X\n", code);
            break;

    }

}

