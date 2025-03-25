#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

constexpr int MAX_TEXTURES = 100;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *textures[MAX_TEXTURES];
int textureCount = 0;

int drawString(lua_State *L) {
    TTF_Font *font = nullptr;
    font = TTF_OpenFont("resources/DejaVuSans.ttf", 36);

    if (textureCount >= MAX_TEXTURES) {
        fprintf(stderr, "Texture limit reached\n");
        return 0;
    }
    const char *text = luaL_checkstring(L, 1);
    const int x = luaL_checkinteger(L, 2);
    const int y = luaL_checkinteger(L, 3);

    printf("Drawing text: %s at (%d, %d)\n", text, x, y);

    SDL_Surface *surfaceText = TTF_RenderText_Blended(font, text, strlen(text), (SDL_Color){255, 255, 255, 255});
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    textures[textureCount++] = textureText;
    SDL_DestroySurface(surfaceText);

    int textWidth, textHeight;

    TTF_GetStringSize(font, text, strlen(text), &textWidth, &textHeight);
    SDL_RenderTexture(renderer, textureText, nullptr, &(SDL_FRect){x, y, textWidth, textHeight});

    TTF_CloseFont(font);
    return 0;
}

int main() {
    window = nullptr;
    renderer = nullptr;
    lua_State *L = luaL_newstate();

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("SDL3 Window", 640, 480, 0);
    renderer = SDL_CreateRenderer(window, nullptr);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    luaL_openlibs(L);
    lua_pushcfunction(L, drawString);
    lua_setglobal(L, "drawString");

    if (luaL_dostring(L, "for i=0,8 do drawString(tostring(i) .. ' ^ 2 =' .. tostring(i * i), 10, i*50) end") != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

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
    for (int i = 0; i < textureCount; i++) {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    lua_close(L);

    return 0;
}
