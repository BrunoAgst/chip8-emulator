//
// Created by Bruno Augusto on 05/09/24.
//

#include <SDL.h>

#ifndef CHIP_8_DISPLAY_H
#define CHIP_8_DISPLAY_H


class display {
public:
    static int sdlInit();
    static SDL_Renderer* createRender(SDL_Window* window);
    static void renderScreen(SDL_Renderer* renderer, unsigned char (*array)[32]);
    static SDL_Window* createWindow(int width, int height);

};

#endif //CHIP_8_DISPLAY_H
