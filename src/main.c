#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "../include/scene.h"
#include "../include/scene-title.h"
#include "../include/game-state.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

int main() {
    window = nullptr;
    renderer = nullptr;
    lua_State *L = luaL_newstate();

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("SDL3 Window", 640, 480, 0);
    renderer = SDL_CreateRenderer(window, nullptr);

    GlobalGameState gameState = globalGameState_create(); 

    Scene currentScene = scene_title_create();
    currentScene.init(&currentScene, &gameState, renderer);

    while (1) {
        currentScene.draw(&currentScene, &gameState, renderer);
        SDL_RenderPresent(renderer);
        SceneUpdateResult result = currentScene.update(&currentScene, &gameState, renderer);

        if (result.shouldQuit) {
            break;
        } else if (result.nextScene != nullptr) {
            currentScene.destroy(&currentScene, &gameState, renderer);
            currentScene = result.nextScene();
            currentScene.init(&currentScene, &gameState, renderer);
        }
    }

    currentScene.destroy(&currentScene, &gameState, renderer);
    globalGameState_free(gameState);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    lua_close(L);

    return 0;
}
