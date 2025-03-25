#pragma once

#include <SDL3/SDL_render.h>
#include "game-state.h"

/**
 * Scene definition
 */
typedef struct Scene Scene;

/**
 * Scene constructor function pointer
 */
typedef Scene (*scene_constructor)();

/**
 * Scene update result
 */
typedef struct SceneUpdateResult {
    /**
     * The next scene constructor
     */
    scene_constructor nextScene;

    /**
     * Whether the game should quit
     */
    bool shouldQuit;
} SceneUpdateResult;

/**
 * Scene definition
 */
typedef struct Scene {
    /**
     * Initialize the scene
     *
     * @param self The scene
     * @param globalGameState The global game state
     * @param renderer The SDL renderer
     */
    void (*init)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

    /**
     * Update the scene
     *
     * @param self The scene
     * @param globalGameState The global game state
     * @param renderer The SDL renderer
     * @return SceneUpdateResult The scene update result
     */
    SceneUpdateResult (*update)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

    /**
     * Draw the scene
     *
     * @param self The scene
     * @param globalGameState The global game state
     * @param renderer The SDL renderer
     */
    void (*draw)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

    /**
     * Destroy the scene
     *
     * @param self The scene
     * @param globalGameState The global game state
     * @param renderer The SDL renderer
     */
    void (*destroy)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);

    /**
     * The scene data
     */
    void *data;
} Scene;
