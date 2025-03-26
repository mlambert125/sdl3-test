#include <SDL3_ttf/SDL_ttf.h>
#include "../include/game-state.h"

GlobalGameState globalGameState_create(SDL_Renderer *renderer) {
    GlobalGameState globalGameState;

    globalGameState.fontDejaVuSans = TTF_OpenFont("resources/DejaVuSans.ttf", 24);

    SDL_Surface *surfaceGrass = SDL_LoadBMP("resources/tile-grass.bmp");
    globalGameState.textureTiles[TILE_GRASS] = SDL_CreateTextureFromSurface(renderer, surfaceGrass);
    SDL_DestroySurface(surfaceGrass);

    SDL_Surface *surfaceTree = SDL_LoadBMP("resources/tile-trees.bmp");
    globalGameState.textureTiles[TILE_TREE] = SDL_CreateTextureFromSurface(renderer, surfaceTree);
    SDL_DestroySurface(surfaceTree);

    SDL_Surface *surfaceHills = SDL_LoadBMP("resources/tile-hills.bmp");
    globalGameState.textureTiles[TILE_HILLS] = SDL_CreateTextureFromSurface(renderer, surfaceHills);
    SDL_DestroySurface(surfaceHills);

    SDL_Surface *surfaceMountains = SDL_LoadBMP("resources/tile-mountains.bmp");
    globalGameState.textureTiles[TILE_MOUNTAINS] = SDL_CreateTextureFromSurface(renderer, surfaceMountains);
    SDL_DestroySurface(surfaceMountains);

    SDL_Surface *surfaceWater = SDL_LoadBMP("resources/tile-water.bmp");
    globalGameState.textureTiles[TILE_WATER] = SDL_CreateTextureFromSurface(renderer, surfaceWater);
    SDL_DestroySurface(surfaceWater);

    strcpy(globalGameState.mapScript, "overworld.lua");

    return globalGameState;
}

void globalGameState_free(GlobalGameState globalGameState) {
    TTF_CloseFont(globalGameState.fontDejaVuSans);
    SDL_DestroyTexture(globalGameState.textureTiles[TILE_GRASS]);
    SDL_DestroyTexture(globalGameState.textureTiles[TILE_TREE]);
    SDL_DestroyTexture(globalGameState.textureTiles[TILE_HILLS]);
    SDL_DestroyTexture(globalGameState.textureTiles[TILE_MOUNTAINS]);
    SDL_DestroyTexture(globalGameState.textureTiles[TILE_WATER]);
}
