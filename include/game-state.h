#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

constexpr int TILE_GRASS = 0;
constexpr int TILE_TREE = 1;
constexpr int TILE_HILLS = 2;
constexpr int TILE_MOUNTAINS = 3;
constexpr int TILE_WATER = 4;

/**
 * Global game state
 */
typedef struct GlobalGameState {
    /**
     * The global SDL renderer.
     */
    TTF_Font *fontDejaVuSans;

    /**
     * Tile textures
     */
    SDL_Texture* textureTiles[5];

    /**
     * The map script
     */
    char mapScript[255];
} GlobalGameState;

/**
 * Create a global game state
 *
 * @return GlobalGameState The global game state
 */
GlobalGameState globalGameState_create(SDL_Renderer *renderer);

/**
 * Free a global game state
 *
 * @param globalGameState The global game state
 */
void globalGameState_free(GlobalGameState globalGameState);
