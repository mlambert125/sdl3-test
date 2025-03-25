#include <stdlib.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../include/scene-title.h"
#include "../include/scene-game.h"

Scene scene_title_create() {
    return (Scene){.init = scene_title_init,
                   .update = scene_title_update,
                   .draw = scene_title_draw,
                   .destroy = scene_title_destroy,
                   .data = nullptr};
}

void scene_title_init(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SceneTitleData *data = (SceneTitleData *)malloc(sizeof(SceneTitleData));
    data->counter = 0;
    self->data = data;
}

SceneUpdateResult scene_title_update(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
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
            return (SceneUpdateResult){.nextScene = scene_game_create, .shouldQuit = false};
        default:
            return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
        }
    } else {
        return (SceneUpdateResult){.nextScene = nullptr, .shouldQuit = false};
    }
}

void scene_title_draw(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    char *text = "Hello, world!";
    int x = 100;
    int y = 100;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Surface *surfaceText =
        TTF_RenderText_Blended(globalGameState->fontDejaVuSans, text, strlen(text), (SDL_Color){255, 255, 255, 255});
    SDL_Texture *textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_DestroySurface(surfaceText);

    int textWidth, textHeight;
    TTF_GetStringSize(globalGameState->fontDejaVuSans, text, strlen(text), &textWidth, &textHeight);
    SDL_RenderTexture(renderer, textureText, nullptr, &(SDL_FRect){x, y, textWidth, textHeight});
    SDL_DestroyTexture(textureText);
}

void scene_title_destroy(Scene *self, GlobalGameState *globalGameState, SDL_Renderer *renderer) {
    if (self->data != nullptr) free(self->data);
}
