#include <SDL3/SDL.h>

#include "app.h"

void HandleKeyPress(AppState* state, SDL_Event* event){
    switch (event->key.scancode){
        case SDL_SCANCODE_1:
            state->chip8->Keypad[0x1] = true;
            break;

        case SDL_SCANCODE_2:
            state->chip8->Keypad[0x2] = true;
            break;

        case SDL_SCANCODE_3:
            state->chip8->Keypad[0x3] = true;
            break;

        case SDL_SCANCODE_4:
            state->chip8->Keypad[0xC] = true;
            break;

        case SDL_SCANCODE_Q:
            state->chip8->Keypad[0x4] = true;
            break;

        case SDL_SCANCODE_W:
            state->chip8->Keypad[0x5] = true;
            break;

        case SDL_SCANCODE_E:
            state->chip8->Keypad[0x6] = true;
            break;

        case SDL_SCANCODE_R:
            state->chip8->Keypad[0xD] = true;
            break;

        case SDL_SCANCODE_A:
            state->chip8->Keypad[0x7] = true;
            break;

        case SDL_SCANCODE_S:
            state->chip8->Keypad[0x8] = true;
            break;

        case SDL_SCANCODE_D:
            state->chip8->Keypad[0x9] = true;
            break;

        case SDL_SCANCODE_F:
            state->chip8->Keypad[0xE] = true;
            break;

        case SDL_SCANCODE_Z:
            state->chip8->Keypad[0xA] = true;
            break;

        case SDL_SCANCODE_X:
            state->chip8->Keypad[0x0] = true;
            break;

        case SDL_SCANCODE_C:
            state->chip8->Keypad[0xB] = true;
            break;

        case SDL_SCANCODE_V:
            state->chip8->Keypad[0xF] = true;
            break;

        default:
            break;
    }
    return;
}

void HandleKeyRelease(AppState* state, SDL_Event* event){
    switch (event->key.scancode){
        case SDL_SCANCODE_1:
            state->chip8->Keypad[0x1] = false;
            break;

        case SDL_SCANCODE_2:
            state->chip8->Keypad[0x2] = false;
            break;

        case SDL_SCANCODE_3:
            state->chip8->Keypad[0x3] = false;
            break;

        case SDL_SCANCODE_4:
            state->chip8->Keypad[0xC] = false;
            break;

        case SDL_SCANCODE_Q:
            state->chip8->Keypad[0x4] = false;
            break;

        case SDL_SCANCODE_W:
            state->chip8->Keypad[0x5] = false;
            break;

        case SDL_SCANCODE_E:
            state->chip8->Keypad[0x6] = false;
            break;

        case SDL_SCANCODE_R:
            state->chip8->Keypad[0xD] = false;
            break;

        case SDL_SCANCODE_A:
            state->chip8->Keypad[0x7] = false;
            break;

        case SDL_SCANCODE_S:
            state->chip8->Keypad[0x8] = false;
            break;

        case SDL_SCANCODE_D:
            state->chip8->Keypad[0x9] = false;
            break;

        case SDL_SCANCODE_F:
            state->chip8->Keypad[0xE] = false;
            break;

        case SDL_SCANCODE_Z:
            state->chip8->Keypad[0xA] = false;
            break;

        case SDL_SCANCODE_X:
            state->chip8->Keypad[0x0] = false;
            break;

        case SDL_SCANCODE_C:
            state->chip8->Keypad[0xB] = false;
            break;

        case SDL_SCANCODE_V:
            state->chip8->Keypad[0xF] = false;
            break;

        default:
            break;
    }
    return;
}
