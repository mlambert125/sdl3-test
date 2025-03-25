#pragma once

#include <SDL3/SDL_render.h>
#include <lua.h>
#include "scene.h"

/**
 * Map scene data
 */
typedef struct SceneMapData {
    /**
     * The Lua state
     */
    lua_State *L;

    /**
     * The text textures
     */
    int textTextureCount;

    /**
     * The text textures
     */
    SDL_Texture **textTextures;
} SceneMapData;

/**
 * Create a map scene
 *
 * @return Scene The map scene
 */
Scene scene_map_create();

/**
 * Initialize the map scene
 *
 * @param self The map scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 */
void scene_map_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

/**
 * Update the map scene
 *
 * @param self The map scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 * @return SceneUpdateResult The scene update result
 */
SceneUpdateResult scene_map_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

/**
 * Draw the map scene
 *
 * @param self The map scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 */
void scene_map_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

/**
 * Destroy the map scene
 *
 * @param self The map scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 */
void scene_map_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
