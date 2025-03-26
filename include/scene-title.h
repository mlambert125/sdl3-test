#pragma once

#include <SDL3/SDL_render.h>
#include "scene.h"

typedef struct SceneTitleData {
    int dummy;
} SceneTitleData;

Scene scene_title_create();
void scene_title_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
SceneUpdateResult scene_title_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_title_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_title_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
