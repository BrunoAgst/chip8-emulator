#include <iostream>
#include "chip8.h"
#include "display.h"
#include "file.h"
#include <SDL.h>
#include <SDL_timer.h>
#include <vector>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 320;

int main(int argc, char* argv[]) {

    std::string filePath = argv[1];

    if(filePath.empty()){
        std::cerr << "ROM path not found" << std::endl;
        return 1;
    }


    std::vector<unsigned char> rom;
    int fOpen = file::getFileBytes(rom, filePath);

    if(fOpen == 1){
        std::cerr << "Error get file bytes" << std::endl;
        return 1;
    }

    int sdlInit = display::sdlInit();

    if(sdlInit == 1){
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = display::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

    if(!window){
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = display::createRender(window);
    if(!renderer){
        std::cerr << "Error creating render: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    chip8 c = {};
    c.initMemory(rom);

    bool running = true;
    SDL_Event e;
    while(running){
        while (SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                running = false;
            }
        }

        c.execute();

        if(c.drawFlag){
            display::renderScreen(renderer, c.chip8ScreenBuffer);
            c.drawFlag = false;
        }

        SDL_Delay(1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    c.printRegister();

    return 0;
}
