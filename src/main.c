#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

#include "chip8.h"
#include "beep.h"
#include "button.h"
#include "app.h"

#define WINDOW_W 1280
#define WINDOW_H 640

int InitSound(AppState* state){
    SDL_IOStream* io = SDL_IOFromConstMem(beep_wav, beep_wav_len);
    SDL_AudioSpec spec;

    if (!SDL_LoadWAV_IO(io, true, &spec, &state->WavBuffer, &state->WavLength)){
        SDL_Log("Couldn't load audio: %s", SDL_GetError());
        return 1;
    }

    state->beep = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK , &spec, NULL, NULL);
    if (!state->beep){
        SDL_Log("Couldn't load audio: %s", SDL_GetError());
        return 1;
    }

    SDL_ResumeAudioStreamDevice(state->beep);

    return 0;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv){
    (void) argc;

    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        SDL_Log("Could not initialize program: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }    

    AppState* state = SDL_calloc(1, sizeof(AppState));
    if (!state){
        SDL_Log("Could not initialize program: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    *appstate = state;

    state->window = SDL_CreateWindow("Window", WINDOW_W, WINDOW_H, 0);
    if (!state->window){
        SDL_Log("Could not initialize program: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->renderer = SDL_CreateRenderer(state->window, "opengl");
    if (!state->renderer){
        SDL_Log("Could not initialize program: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    state->chip8 = SDL_calloc(1, sizeof(struct Chip8));
    if (!state->chip8){
        SDL_Log("Could not initialize program: %s", SDL_GetError());
    }

    InitChip8(state->chip8);
    
    if (LoadRom(state->chip8, argv[1])){
        return SDL_APP_FAILURE;
    }

    if (InitSound(state) == 1){
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event){
    AppState* state = (AppState*) appstate;

    switch (event->type){
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN:
            HandleKeyPress(state, event);
            break;

        case SDL_EVENT_KEY_UP:
            HandleKeyRelease(state, event);
            break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate){
    AppState* state = (AppState*) appstate;

    for (int i = 0; i < 10; i++){
        Chip8Cycle(state->chip8);
    }

    if (state->chip8->RegisterDelay > 0){
        state->chip8->RegisterDelay--;
    }

    if (state->chip8->RegisterSound > 0){
        if (SDL_GetAudioStreamQueued(state->beep) == 0){
            SDL_PutAudioStreamData(state->beep, state->WavBuffer, state->WavLength);
        }

        state->chip8->RegisterSound--;
    }
    else {
        SDL_ClearAudioStream(state->beep);
    }

    if (state->chip8->DrawFlag){
        state->chip8->DrawFlag = false;
    }

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state->renderer);

    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (int y = 0; y < 32; y++){
        for (int x = 0; x < 64; x++){
            if (state->chip8->Display[x][y]){
                SDL_FRect rect = {x * 20, y * 20, 20, 20};
                SDL_RenderFillRect(state->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result){
    (void) result;

    AppState* state = (AppState*) appstate;

    if (state){
        if (state->renderer){
            SDL_DestroyRenderer(state->renderer);
        }

        if (state->window){
            SDL_DestroyWindow(state->window);
        }

        if (state->chip8){
            SDL_free(state->chip8);
        }

        if (state->beep){
            SDL_DestroyAudioStream(state->beep);
            SDL_free(state->WavBuffer);
        }


        SDL_free(state);
    }

    SDL_Quit();
}
