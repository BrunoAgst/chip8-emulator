//
// Created by Bruno Augusto on 05/09/24.
//

#include "display.h"

int display::sdlInit() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){
        return 1;
    } else {
        return 0;
    }
}

SDL_Window* display::createWindow(int width, int height) {
    return SDL_CreateWindow("CHIP-8 Emulator",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            width, height, 0);
}

SDL_Renderer* display::createRender(SDL_Window *window) {
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void display::renderScreen(SDL_Renderer *renderer, unsigned char (*array)[32]){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int y = 0; y < 32; ++y){
        for(int x = 0; x < 64; ++x){
            if(array[x][y]){
                SDL_Rect pixel = { x * 10, y * 10, 8,  10};
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
}


