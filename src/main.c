#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

char *message = "Hello, SDL3!";

int setMessage(lua_State *L) {
    const char *newMessage = luaL_checkstring(L, 1);
    message = (char *)newMessage;
    return 0;
}

int main() {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;
    lua_State *L = luaL_newstate();

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    font = TTF_OpenFont("resources/DejaVuSans.ttf", 36);
    window = SDL_CreateWindow("SDL3 Window", 640, 480, 0);
    renderer = SDL_CreateRenderer(window, nullptr);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    luaL_openlibs(L);
    lua_pushcfunction(L, setMessage);
    lua_setglobal(L, "setMessage");

    if (luaL_dostring(L, "setMessage('Hello from a lua script!')") != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    SDL_Surface *surfaceText = TTF_RenderText_Blended(font, message, strlen(message), (SDL_Color){255, 255, 255, 255});
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_DestroySurface(surfaceText);

    int w, h;
    TTF_GetStringSize(font, message, strlen(message), &w, &h);

    printf("Text width: %d, height: %d\n", w, h);

    SDL_RenderTexture(renderer, textureText, nullptr, &(SDL_FRect){10, 100, w, h});
    SDL_RenderPresent(renderer);

    while (1) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                goto cleanup;
            }
        }
    }

cleanup:
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    lua_close(L);

    return 0;
}
