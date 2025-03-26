#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

constexpr int TILE_GRASS = 0;
constexpr int TILE_TREE = 1;
constexpr int TILE_HILLS = 2;
constexpr int TILE_MOUNTAINS = 3;
constexpr int TILE_WATER = 4;

typedef struct GlobalGameState {
    TTF_Font *fontDejaVuSans;
    SDL_Texture* textureTiles[5];
    char mapScript[255];
} GlobalGameState;

GlobalGameState globalGameState_create(SDL_Renderer *renderer);
void globalGameState_free(GlobalGameState globalGameState);
