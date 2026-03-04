#ifndef BUTTON_H
#define BUTTON_H

#include <SDL3/SDL.h>

#include "app.h"

void HandleKeyPress(AppState* state, SDL_Event* event);
void HandleKeyRelease(AppState* state, SDL_Event* event);

#endif
