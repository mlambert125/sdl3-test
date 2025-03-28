#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "../include/scene-map.h"
#include "../include/scene-title.h"

int printLuaTable(lua_State *L) {

    lua_getfield(L, -1, "x");
    int x = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "y");
    int y = lua_tointeger(L, -1);
    lua_pop(L, 1);

    if (!lua_getfield(L, -1, "nothing")) {
        printf("Error: 'nothing' not found\n");
        lua_pop(L, 1);
    }

    lua_getfield(L, -1, "z");
    lua_pushnil(L);

    while (lua_next(L, -2) != 0) {
        lua_tointeger(L, -2);

        lua_pushnil(L);

        while (lua_next(L, -2) != 0) {
            int key = lua_tointeger(L, -2);
            int value = lua_tointeger(L, -1);

            printf("(%d: %d)", key, value);
            lua_pop(L, 1);
        }
        printf("\n");

        lua_pop(L, 1);
    }

    printf("x: %d, y: %d\n", x, y);

    return 0;
}

int setTile(lua_State *L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int tileCode = luaL_checkinteger(L, 3);

    lua_getglobal(L, "sceneState");
    SceneMapData *data = lua_touserdata(L, -1);

    if (x < 0 || x >= data->mapWidth || y < 0 || y >= data->mapHeight) {
        printf("Error: setTile out of bounds\n");
        return 1;
    }
    data->map[y][x] = tileCode;

    return 0;
}

int setPlayerPosition(lua_State *L) {
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);

    lua_getglobal(L, "sceneState");
    SceneMapData *data = lua_touserdata(L, -1);

    if (x < 12 || y < 9 || x >= data->mapWidth - 12 || y >= data->mapHeight - 9) {
        printf("Error: setPlayerPosition out of bounds\n");
        return 1;
    }

    data->playerX = x;
    data->playerY = y;

    return 0;
}

int initMap(lua_State *L) {
    lua_getglobal(L, "sceneState");
    SceneMapData *data = lua_touserdata(L, -1);

    for (int i = 0; i < data->mapHeight; i++) {
        free(data->map[i]);
    }
    free(data->map);

    int width = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);

    if (width < 25 || height < 19) {
        printf("Error: initMap dimensions are too small\n");
        return 1;
    }

    data->map = (int **)malloc(height * sizeof(int *));

    for (int i = 0; i < height; i++) {
        data->map[i] = (int *)malloc(width * sizeof(int));
    }

    data->mapWidth = width;
    data->mapHeight = height;

    return 0;
}

Scene scene_map_create() {
    printf("scene_map_create\n");
    SceneMapData *data = malloc(sizeof(SceneMapData));

    return (Scene){.init = scene_map_init,
                   .update = scene_map_update,
                   .draw = scene_map_draw,
                   .destroy = scene_map_destroy,
                   .data = data};
}

void scene_map_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    printf("scene_map_init\n");

    SceneMapData *data = (SceneMapData *)self->data;

    int width = 25;
    int height = 19;

    data->map = (int **)malloc(height * sizeof(int *));

    for (int i = 0; i < height; i++) {
        data->map[i] = (int *)calloc(width, sizeof(int));
    }

    data->mapWidth = width;
    data->mapHeight = height;
    data->playerX = 12;
    data->playerY = 9;

    data->pendingMovement = PENDING_MOVEMENT_NONE;
    data->pendingMovementTicks = 0;

    data->L = luaL_newstate();
    luaL_openlibs(data->L);

    lua_register(data->L, "setTile", setTile);
    lua_register(data->L, "setPlayerPosition", setPlayerPosition);
    lua_register(data->L, "printLuaTable", printLuaTable);
    lua_register(data->L, "initMap", initMap);

    lua_pushlightuserdata(data->L, renderer);
    lua_setglobal(data->L, "renderer");

    lua_pushlightuserdata(data->L, globalGameState);
    lua_setglobal(data->L, "globalGameState");

    lua_pushlightuserdata(data->L, self->data);
    lua_setglobal(data->L, "sceneState");

    char mapScriptPath[512];

    sprintf(mapScriptPath, "resources/scripts/%s", globalGameState->mapScript);

    if (luaL_dofile(data->L, mapScriptPath) != LUA_OK) {
        printf("Error: %s\n", lua_tostring(data->L, -1));
        lua_pop(data->L, 1);
    }

    data->textureMap = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                         data->mapWidth * TILE_SIZE, data->mapHeight * TILE_SIZE);

    for (int y = 0; y < data->mapHeight; y++) {
        for (int x = 0; x < data->mapWidth; x++) {
            SDL_SetRenderTarget(renderer, data->textureMap);
            SDL_RenderTexture(renderer, globalGameState->textureTiles[data->map[y][x]], nullptr,
                              &(SDL_FRect){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE});
        }
    }
}

SceneUpdateResult scene_map_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;
    SDL_Event event;
    int moveDelay = 60;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = true};
        }
    }

    if (data->pendingMovement != PENDING_MOVEMENT_NONE) {

        if (data->pendingMovementTicks == 2) {
            if (data->pendingMovement == PENDING_MOVEMENT_LEFT && data->playerX > 12) {
                data->playerX--;
            } else if (data->pendingMovement == PENDING_MOVEMENT_RIGHT && data->playerX < data->mapWidth - 13) {
                data->playerX++;
            } else if (data->pendingMovement == PENDING_MOVEMENT_UP && data->playerY > 9) {
                data->playerY--;
            } else if (data->pendingMovement == PENDING_MOVEMENT_DOWN && data->playerY < data->mapHeight - 10) {
                data->playerY++;
            }
        }

        SDL_Delay(moveDelay);
        if (++data->pendingMovementTicks >= 3) { 
            data->pendingMovementTicks = 0;
            data->pendingMovement = PENDING_MOVEMENT_NONE;
        }
        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
    }

    const bool *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        if (data->playerX > 12) {
            data->pendingMovement = PENDING_MOVEMENT_LEFT;
        }
    } else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        if (data->playerX < data->mapWidth - 13) {
            data->pendingMovement = PENDING_MOVEMENT_RIGHT;
        }
    } else if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
        if (data->playerY > 9) {
            data->pendingMovement = PENDING_MOVEMENT_UP;
        }
    } else if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
        if (data->playerY < data->mapHeight - 10) {
            data->pendingMovement = PENDING_MOVEMENT_DOWN;
        }
    } else if (keys[SDL_SCANCODE_ESCAPE] || keys[SDL_SCANCODE_Q]) {
        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = true};
    } else if (keys[SDL_SCANCODE_SPACE]) {
        //return (SceneUpdateResult){.nextScene = scene_title_create, .shouldQuit = false};
    }


    return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
}

void scene_map_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int xOffset = (data->playerX - 12) * TILE_SIZE;
    int yOffset = (data->playerY - 9) * TILE_SIZE;

    if (data->pendingMovement == PENDING_MOVEMENT_LEFT) {
        xOffset -= (TILE_SIZE / 4 * (data->pendingMovementTicks + 1)); // Move left
    } else if (data->pendingMovement == PENDING_MOVEMENT_RIGHT) {
        xOffset += (TILE_SIZE / 4 * (data->pendingMovementTicks + 1)); // Move left;
    } else if (data->pendingMovement == PENDING_MOVEMENT_UP) {
        yOffset -= (TILE_SIZE / 4 * (data->pendingMovementTicks + 1)); // Move up
    } else if (data->pendingMovement == PENDING_MOVEMENT_DOWN) {
        yOffset += (TILE_SIZE / 4 * (data->pendingMovementTicks + 1)); // Move down
    }

    SDL_FRect viewport = {xOffset, yOffset, 1250, 950};
    SDL_RenderTexture(renderer, data->textureMap, &viewport, &(SDL_FRect){0, 0, 1250, 950});
}

void scene_map_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    printf("scene_map_destroy\n");

    SceneMapData *data = (SceneMapData *)self->data;
    lua_close(data->L);

    for (int i = 0; i < data->mapHeight; i++) {
        free(data->map[i]);
    }
    free(data->map);

    if (self->data != nullptr) free(self->data);
    self->data = nullptr;
}
