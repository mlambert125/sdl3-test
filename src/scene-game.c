#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "../include/scene-game.h"
#include "../include/scene-title.h"

char *testScript = "drawString('Lua Calculating Squares:')\n"
                   "for i = 1, 10 do\n"
                   "    drawString(i .. ' ^ 2 = ' .. (i * i))\n"
                   "end\n";

int drawString(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);

    lua_getglobal(L, "renderer");
    SDL_Renderer *renderer = lua_touserdata(L, -1);

    lua_getglobal(L, "globalGameState");
    GlobalGameState *globalGameState = lua_touserdata(L, -1);

    lua_getglobal(L, "sceneState");
    SceneGameData *data = lua_touserdata(L, -1);

    SDL_Surface *surface =
        TTF_RenderText_Blended(globalGameState->fontDejaVuSans, text, strlen(text), (SDL_Color){255, 255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    data->textTextures = realloc(data->textTextures, sizeof(SDL_Texture *) * (data->textTextureCount + 1));
    data->textTextures[data->textTextureCount++] = texture;

    return 0;
}

Scene scene_game_create() {
    return (Scene){.init = scene_game_init,
                   .update = scene_game_update,
                   .draw = scene_game_draw,
                   .destroy = scene_game_destroy,
                   .data = nullptr};
}

void scene_game_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneGameData *data = (SceneGameData *)malloc(sizeof(SceneGameData));
    data->L = luaL_newstate();
    luaL_openlibs(data->L);
    self->data = data;
    data->textTextureCount = 0;
    data->textTextures = nullptr;

    lua_register(data->L, "drawString", drawString);

    lua_pushlightuserdata(data->L, renderer);
    lua_setglobal(data->L, "renderer");

    lua_pushlightuserdata(data->L, globalGameState);
    lua_setglobal(data->L, "globalGameState");

    lua_pushlightuserdata(data->L, self->data);
    lua_setglobal(data->L, "sceneState");

    luaL_dostring(data->L, testScript);
}

SceneUpdateResult scene_game_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
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
        default:
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
        }
    } else {
        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
    }
}

void scene_game_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    SceneGameData *data = (SceneGameData *)self->data;

    for (int i = 0; i < data->textTextureCount; i++) {
        SDL_Texture *texture = data->textTextures[i];

        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        SDL_RenderTexture(renderer, texture, nullptr, &(SDL_FRect){10.0f, 10.0f + (float)i * 30.0f, w, h});
    }
}

void scene_game_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneGameData *data = (SceneGameData *)self->data;
    lua_close(data->L);
    for (int i = 0; i < data->textTextureCount; i++) {
        SDL_DestroyTexture(data->textTextures[i]);
    }
    free(data->textTextures);
    if (self->data != nullptr) free(self->data);
}
