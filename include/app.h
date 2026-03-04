#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>

#include "chip8.h"

typedef struct AppState{
    SDL_Window* window;
    SDL_Renderer* renderer;

    struct Chip8* chip8;

    SDL_AudioStream* beep;
    Uint8* WavBuffer;
    Uint32 WavLength;
} AppState;

#endif
