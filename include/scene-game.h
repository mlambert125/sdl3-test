#pragma once

#include <SDL3/SDL_render.h>
#include <lua.h>
#include "scene.h"

typedef struct SceneGameData {
    lua_State *L;
    int textTextureCount;
    SDL_Texture **textTextures;
} SceneGameData;

Scene scene_game_create();
void scene_game_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
SceneUpdateResult scene_game_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_game_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_game_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
