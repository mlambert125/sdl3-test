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

    if (x < 0 || x >= 500 || y < 0 || y >= 500) {
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

    data->playerX = x;
    data->playerY = y;

    return 0;
}

Scene scene_map_create() {
    printf("scene_map_create\n");
    void *data = malloc(sizeof(SceneMapData));

    return (Scene){.init = scene_map_init,
                   .update = scene_map_update,
                   .draw = scene_map_draw,
                   .destroy = scene_map_destroy,
                   .data = data};
}

void scene_map_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    printf("scene_map_init\n");

    SceneMapData *data = (SceneMapData *)self->data;

    data->L = luaL_newstate();
    luaL_openlibs(data->L);

    lua_register(data->L, "setTile", setTile);
    lua_register(data->L, "setPlayerPosition", setPlayerPosition);
    lua_register(data->L, "printLuaTable", printLuaTable);

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
}

SceneUpdateResult scene_map_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_EVENT_QUIT) {
        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = true};
    } else if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_ESCAPE:
        case SDLK_Q:
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = true};
        case SDLK_SPACE:
            return (SceneUpdateResult){.nextScene = scene_title_create, .shouldQuit = false};
        case SDLK_A:
        case SDLK_LEFT:
            if (data->playerX > 12) data->playerX--;
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
        case SDLK_D:
        case SDLK_RIGHT:
            if (data->playerX < 487) data->playerX++;
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
        case SDLK_W:
        case SDLK_UP:
            if (data->playerY > 9) data->playerY--;
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
        case SDLK_S:
        case SDLK_DOWN:
            if (data->playerY < 490) data->playerY++;
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
        default:
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
        }
    } else {
        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
    }
}

void scene_map_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < 19; y++) {
        for (int x = 0; x < 25; x++) {

            int mapX = x + data->playerX - 12;
            int mapY = y + data->playerY - 9;

            if (x < 0 || x >= 475 || y < 0 || y >= 482) {
                continue;
            }

            int tileCode = data->map[mapY][mapX];

            int screenX = x * 50;
            int screenY = y * 50;

            SDL_RenderTexture(renderer, globalGameState->textureTiles[tileCode], nullptr,
                              &(SDL_FRect){screenX, screenY, 50, 50});
        }
    }
}

void scene_map_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    printf("scene_map_destroy\n");

    SceneMapData *data = (SceneMapData *)self->data;
    lua_close(data->L);

    if (self->data != nullptr) free(self->data);
    self->data = nullptr;
}
