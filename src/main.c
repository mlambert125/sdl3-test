#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "../include/scene.h"
#include "../include/scene-title.h"
#include "../include/game-state.h"

int main() {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!TTF_Init()) {
        printf("TTF_Init failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("SDL3 Window", 1250, 950, 0);

    if (window == nullptr) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer == nullptr) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    GlobalGameState gameState = globalGameState_create(renderer);

    Scene currentScene = scene_title_create();
    currentScene.init(&currentScene, &gameState, renderer);

    SceneUpdateResult result = {.nextScene = nullptr, .shouldQuit = false};

    while (!result.shouldQuit) {
        currentScene.draw(&currentScene, &gameState, renderer);
        SDL_RenderPresent(renderer);
        result = currentScene.update(&currentScene, &gameState, renderer);

        if (!result.shouldQuit && result.nextScene != nullptr) {
            currentScene.destroy(&currentScene, &gameState, renderer);
            currentScene = result.nextScene();
            currentScene.init(&currentScene, &gameState, renderer);
        }
    }

    currentScene.destroy(&currentScene, &gameState, renderer);
    globalGameState_free(gameState);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
