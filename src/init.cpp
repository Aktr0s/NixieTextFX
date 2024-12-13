#include <iostream>
#include "globals.hpp"
#include "NixieOne_Mono_ttf.hpp"
// Define the global variables
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
TTF_Font* g_font = nullptr;

int NixieInicial() {
    SDL_RWops *fontRW = SDL_RWFromMem(NixieOne_Mono_ttf, NixieOne_Mono_ttf_len);
    if (!fontRW) {
        std::cerr << "Failed to create RWops from memory: " << SDL_GetError() << std::endl;
        return -1;
    }

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
        600,
        145,
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

    //g_font = TTF_OpenFont("../assets/NixieOne-Mono.ttf", 120);
    g_font = TTF_OpenFontRW(fontRW, 1, 120);
    if (!g_font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    return 0; // Success
}

void Cleanup() {
    if (g_font) TTF_CloseFont(g_font);
    if (g_renderer) SDL_DestroyRenderer(g_renderer);
    if (g_window) SDL_DestroyWindow(g_window);
    TTF_Quit();
    SDL_Quit();
}
