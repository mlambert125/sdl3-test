#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

int main() {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!TTF_Init()) {
        fprintf(stderr, "TTF_Init Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("resources/DejaVuSans.ttf", 24);
    if (!font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("SDL3 Window", 640, 480, 0);
    if (window == nullptr) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (renderer == nullptr) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *surface = TTF_RenderText_Blended(font, "Hello, SDL3!", 12, (SDL_Color){255, 255, 255, 255});
    if (!surface) {
        SDL_Log("TTF_RenderText_Solid Error: %s\n", SDL_GetError());
        goto cleanup;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_DestroySurface(surface);
        goto cleanup;
    }

    SDL_DestroySurface(surface);
    SDL_RenderTexture(renderer, texture, nullptr, &(SDL_FRect){ 100, 100, 200, 50 });

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

    return 0;
}
