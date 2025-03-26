#pragma once

#include <SDL3/SDL_render.h>
#include "scene.h"

/**
 * Scene title data
 */
typedef struct SceneTitleData {
    int dummy;
} SceneTitleData;

/**
 * Create a title scene
 *
 * @return Scene The title scene
 */
Scene scene_title_create();

/**
 * Initialize the title scene
 *
 * @param self The title scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 */
void scene_title_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

/**
 * Update the title scene
 *
 * @param self The title scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 * @return SceneUpdateResult The scene update result
 */
SceneUpdateResult scene_title_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

/**
 * Draw the title scene
 *
 * @param self The title scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 */
void scene_title_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

/**
 * Destroy the title scene
 *
 * @param self The title scene
 * @param globalGameState The global game state
 * @param renderer The SDL renderer
 */
void scene_title_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
