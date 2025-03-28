#pragma once

#include <SDL3/SDL_render.h>
#include <lua.h>
#include "scene.h"

constexpr int TILE_SIZE = 50;

enum PendingMovementState {
    PENDING_MOVEMENT_NONE,
    PENDING_MOVEMENT_UP,
    PENDING_MOVEMENT_DOWN,
    PENDING_MOVEMENT_LEFT,
    PENDING_MOVEMENT_RIGHT,
};

typedef struct SceneMapData {
    lua_State *L;
    int **map;
    int mapWidth;
    int mapHeight;
    int playerX;
    int playerY;
    enum PendingMovementState pendingMovement;
    char pendingMovementTicks;
    SDL_Texture *textureMap;
} SceneMapData;

Scene scene_map_create();
void scene_map_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
SceneUpdateResult scene_map_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_map_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
void scene_map_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer);
