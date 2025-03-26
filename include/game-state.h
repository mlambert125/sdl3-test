#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

/**
 * Global game state
 */
typedef struct GlobalGameState {
    /**
     * The global SDL renderer.
     */
    TTF_Font *fontDejaVuSans;
} GlobalGameState;

/**
 * Create a global game state
 *
 * @return GlobalGameState The global game state
 */
GlobalGameState globalGameState_create();

/**
 * Free a global game state
 *
 * @param globalGameState The global game state
 */
void globalGameState_free(GlobalGameState globalGameState);
