#pragma once

#include <SDL3/SDL_render.h>
#include <lua.h>
#include "scene.h"

typedef struct SceneMapData {
    lua_State *L;
    int map[500][500];
    int playerX;
    int playerY;
} SceneMapData;

Scene scene_map_create();
void scene_map_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
SceneUpdateResult scene_map_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_map_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_map_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
