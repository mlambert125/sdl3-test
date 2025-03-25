#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct GlobalGameState {
    TTF_Font *fontDejaVuSans;
} GlobalGameState;

GlobalGameState globalGameState_create();
void globalGameState_free(GlobalGameState globalGameState);
