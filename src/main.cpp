#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "NixieDisplay.cpp"
#include "NixieInitGlobals.h"
#include "NixieInit.cpp"
#include "LocalClock.cpp"
#include "NixieOne_Mono_ttf.hpp"


int main(int argc, char* argv[]) {

    if (NixieInicial() != 0) {
        return 1; // Initialization failed
    }

    bool running = true;
    SDL_Event event;
    NixieDisplay nixie(g_renderer,NixieOne_Mono_ttf,NixieOne_Mono_ttf_len);
    nixie.setPosition(13,0);
    nixie.clockMode(CLOCK_ENABLE,WITH_SECONDS,BLINKING_DOTS_OFF);
    nixie.toggleFlickering(true);
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        nixie.render(RENDER_PRESENT);
        // Cap frame rate at ~60 FPS
        SDL_Delay(30);
    }

    // Cleanup resources
    NixieCleanup();

    return 0;
}
