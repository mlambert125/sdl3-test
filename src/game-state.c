#include <SDL3_ttf/SDL_ttf.h>
#include "../include/game-state.h"

GlobalGameState globalGameState_create() {
    GlobalGameState globalGameState;

    globalGameState.fontDejaVuSans = TTF_OpenFont("resources/DejaVuSans.ttf", 24);
    return globalGameState;
}

void globalGameState_free(GlobalGameState globalGameState) {
    TTF_CloseFont(globalGameState.fontDejaVuSans);
}
