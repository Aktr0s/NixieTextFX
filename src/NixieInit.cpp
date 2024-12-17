#include <iostream>
#include "NixieInitGlobals.hpp"
// Define the global variables
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;

int NixieInicial(Sint16 width = 600, Sint16 height = 145) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError();
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError();
        SDL_Quit();
        return 1;
    }

    g_window = SDL_CreateWindow(
        "NixieTube Clock",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!g_window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(g_window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    std::cout << "Nixie Initialization Successful!" << std::endl;
    return 0; // Success
}

void NixieCleanup() {
    if (g_renderer) SDL_DestroyRenderer(g_renderer);
    if (g_window) SDL_DestroyWindow(g_window);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Nixie Cleanup Done.." << std::endl;
}
