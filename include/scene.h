#pragma once

#include <SDL3/SDL_render.h>
#include "game-state.h"

typedef struct Scene Scene;
typedef Scene (*scene_constructor)();

typedef struct SceneUpdateResult {
    scene_constructor nextScene;
    bool shouldQuit;
} SceneUpdateResult;

typedef struct Scene {
    void (*init)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
    SceneUpdateResult (*update)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
    void (*draw)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
    void (*destroy)(struct Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
    void *data;
} Scene;
