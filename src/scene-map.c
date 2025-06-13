#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "../include/scene-map.h"
#include "../include/scene-title.h"

Scene scene_map_create() {
    SceneMapData *data = malloc(sizeof(SceneMapData));
    return (Scene){.init = scene_map_init,
                   .update = scene_map_update,
                   .draw = scene_map_draw,
                   .destroy = scene_map_destroy,
                   .data = data};
}

void scene_map_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;
    data->map = nullptr;
    data->playerX = 12;
    data->playerY = 9;
    data->movementStartTime = SDL_GetTicks();
    data->pendingMovement = PENDING_MOVEMENT_NONE;

    char mapScriptPath[512];
    sprintf(mapScriptPath, "resources/scripts/%s", globalGameState->mapScript);

    data->L = luaL_newstate();
    luaL_openlibs(data->L);

    if (luaL_dofile(data->L, mapScriptPath) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(data->L, -1));
        lua_pop(data->L, 1);
    }

    if (!lua_istable(data->L, -1)) {
        fprintf(stderr, "Error Processing Script: Expected table\n");
        goto done_processing_script;
    } else {
        lua_pushnil(data->L);
        while (lua_next(data->L, -2) != 0) {
            char *key = (char *)lua_tostring(data->L, -2);

            if (strcmp(key, "map") == 0) {
                if (!lua_istable(data->L, -1)) {
                    fprintf(stderr, "Error Processing Script: Expected table\n");
                    goto done_processing_script;
                }
                lua_len(data->L, -1);
                int row_count = (int)lua_tointeger(data->L, -1);
                lua_pop(data->L, 1);

                data->map = (int **)realloc(data->map, sizeof(int *) * row_count);
                data->mapHeight = row_count;
                data->mapWidth = 0;

                for (int i = 0; i < row_count; ++i) {
                    lua_rawgeti(data->L, -1, i + 1);
                    if (!lua_istable(data->L, -1)) {
                        fprintf(stderr, "Error Processing Script: Expected table for row %d\n", i);
                        goto done_processing_script;
                    }

                    lua_len(data->L, -1);
                    int col_count = (int)lua_tointeger(data->L, -1);
                    lua_pop(data->L, 1);

                    if (data->mapWidth == 0) {
                        data->mapWidth = col_count;
                    } else if (col_count != data->mapWidth) {
                        fprintf(stderr, "Error: Inconsistent column count at row %d\n", i);
                        goto done_processing_script;
                    }

                    data->map[i] = malloc(sizeof(int) * col_count);

                    for (int j = 0; j < col_count; ++j) {
                        lua_rawgeti(data->L, -1, j + 1);
                        data->map[i][j] = (int)lua_tointeger(data->L, -1);
                        lua_pop(data->L, 1);
                    }
                    lua_pop(data->L, 1);
                }
                lua_pop(data->L, 1);
            } else if (strcmp(key, "playerPosition") == 0) {
                if (!lua_istable(data->L, -1)) {
                    fprintf(stderr, "Error Processing Script: Expected table\n");
                    goto done_processing_script;
                }

                lua_len(data->L, -1);
                int table_size = (int)lua_tointeger(data->L, -1);
                lua_pop(data->L, 1);

                if (table_size != 2) {
                    fprintf(stderr, "Error: playerPosition table should have exactly 2 elements\n");
                    goto done_processing_script;
                }

                lua_rawgeti(data->L, -1, 1);
                data->playerX = (int)luaL_checkinteger(data->L, -1);
                lua_pop(data->L, 1);

                lua_rawgeti(data->L, -1, 2);
                data->playerY = (int)luaL_checkinteger(data->L, -1);
                lua_pop(data->L, 1);

                lua_pop(data->L, 1);
            } else {
                fprintf(stderr, "Warning: Unhandled key '%s' in script\n", key);
            }
        }
    }

done_processing_script:
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

uint64_t ticksElapsedSince(uint64_t lastTime) {
    uint64_t currentTime = SDL_GetTicks();
    if (currentTime < lastTime) {
        return (UINT64_MAX - lastTime) + currentTime;
    }
    return currentTime - lastTime;
}

SceneUpdateResult scene_map_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;
    SDL_Event event;
    uint64_t timeSinceLastMove = ticksElapsedSince(data->movementStartTime);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = true};
        }
    }
    const bool *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_ESCAPE] || keys[SDL_SCANCODE_Q]) {
        return (SceneUpdateResult){.nextScene = scene_title_create, .shouldQuit = false};
    }

    if (data->pendingMovement != PENDING_MOVEMENT_NONE && timeSinceLastMove < moveDelay) {
        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
    } else {
        if (data->pendingMovement == PENDING_MOVEMENT_LEFT)
            data->playerX -= 1;
        else if (data->pendingMovement == PENDING_MOVEMENT_RIGHT)
            data->playerX += 1;
        else if (data->pendingMovement == PENDING_MOVEMENT_UP)
            data->playerY -= 1;
        else if (data->pendingMovement == PENDING_MOVEMENT_DOWN)
            data->playerY += 1;

        data->pendingMovement = PENDING_MOVEMENT_NONE;

        if ((keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) && data->playerX > 12 && timeSinceLastMove >= moveDelay) {
            data->pendingMovement = PENDING_MOVEMENT_LEFT;
            data->movementStartTime = SDL_GetTicks();
        } else if ((keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) && data->playerX < data->mapWidth - 13 &&
                   timeSinceLastMove >= moveDelay) {
            data->pendingMovement = PENDING_MOVEMENT_RIGHT;
            data->movementStartTime = SDL_GetTicks();
        } else if ((keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) && data->playerY > 9 &&
                   timeSinceLastMove >= moveDelay) {
            data->pendingMovement = PENDING_MOVEMENT_UP;
            data->movementStartTime = SDL_GetTicks();
        } else if ((keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) && data->playerY < data->mapHeight - 10 &&
                   timeSinceLastMove >= moveDelay) {
            data->pendingMovement = PENDING_MOVEMENT_DOWN;
            data->movementStartTime = SDL_GetTicks();
        }

        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
    }
}

void scene_map_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int xOffset = (data->playerX - 12) * TILE_SIZE;
    int yOffset = (data->playerY - 9) * TILE_SIZE;

    if (data->pendingMovement != PENDING_MOVEMENT_NONE) {
        uint64_t timeSinceLastMove = ticksElapsedSince(data->movementStartTime);
        double pixelOffset = ((double)timeSinceLastMove / moveDelay) * TILE_SIZE;

        if (data->pendingMovement == PENDING_MOVEMENT_LEFT) {
            xOffset -= pixelOffset;
        } else if (data->pendingMovement == PENDING_MOVEMENT_RIGHT) {
            xOffset += pixelOffset;
        } else if (data->pendingMovement == PENDING_MOVEMENT_UP) {
            yOffset -= pixelOffset;
        } else if (data->pendingMovement == PENDING_MOVEMENT_DOWN) {
            yOffset += pixelOffset;
        }
    }

    SDL_FRect viewport = {xOffset, yOffset, 1250, 950};
    SDL_RenderTexture(renderer, data->textureMap, &viewport, &(SDL_FRect){0, 0, 1250, 950});
}

void scene_map_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneMapData *data = (SceneMapData *)self->data;
    lua_close(data->L);

    for (int i = 0; i < data->mapHeight; i++) {
        free(data->map[i]);
    }
    free(data->map);

    if (self->data != nullptr) free(self->data);
    self->data = nullptr;
}
