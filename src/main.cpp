#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "NixieDisplay.cpp"
#include "NixieInitGlobals.hpp"
#include "NixieInit.cpp"
//#include "LocalTime.cpp"
#include "NixieOne_Mono_ttf.hpp"

// Define text color and initial destination rectangle
//SDL_Color glowcolor = {255, 225, 225, 255};
//SDL_Color textcolor = {255, 200, 0};
//SDL_Rect destRect = {15, 0, 0, 0};

// Function to calculate flicker intensity

int main(int argc, char* argv[]) {

    if (NixieInicial() != 0) {
        return 1; // Initialization failed
    }

    bool running = true;
    SDL_Event event;

    // State for blinking colon
    bool dotBlink = true;
    Uint32 lastBlinkTime = SDL_GetTicks();
    Uint32 blinkInterval = 500; // 500ms interval for blinking
    NixieDisplay nixie(g_renderer,NixieOne_Mono_ttf,NixieOne_Mono_ttf_len);
    // Main application loop
    nixie.setText("12:34:56");
    nixie.toggleFlickering(true);
    //nixie.toggleFlickering(true);
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        nixie.updateFlicker();
        nixie.render();
        // Handle blinking state
        /*Uint32 currentTimeMs = SDL_GetTicks();
        if (currentTimeMs - lastBlinkTime >= blinkInterval) {
            dotBlink = !dotBlink;
            lastBlinkTime = currentTimeMs;
        }
        dotBlink = true;

        // Get current time with optional blinking colon
        std::string currentTime = getLocalTime(C_STYLE, dotBlink, WITH_SECONDS);

        // Update textures only if the time string has changed
        static std::string previousTime;
        if (currentTime != previousTime) {
            changeNixieText(currentTime, characterTextures, characterSurfaces, g_renderer, g_font);
            previousTime = currentTime;

            // Resize the flicker intensity vector to match the current text
            flickerIntensities.resize(currentTime.size(), 1.0f);
        }

        // Update individual flicker intensities for each character
        for (float& intensity : flickerIntensities) {
            intensity = getFlickerIntensity();
        }
        
        // Clear the screen
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
        SDL_RenderClear(g_renderer);

        // Render each character with its own flicker
        SDL_Color glowColor = {200, 10, 0, 128}; // Red glow
        int xOffset = destRect.x;
        for (size_t i = 0; i < characterTextures.size(); ++i) {
            if (!characterTextures[i]) continue;

            // Calculate glow alpha based on flicker intensity
            Uint8 glowAlpha = static_cast<Uint8>(128 * flickerIntensities[i]);

            // Set the destination rectangle for the current character
            SDL_Rect charRect = {xOffset, destRect.y, characterSurfaces[i]->w, characterSurfaces[i]->h};

            // Render the character with its glow effect
            renderWithGlow(g_renderer, characterTextures[i], charRect, glowColor, glowAlpha, 3);

            // Update the x offset for the next character
            xOffset += charRect.w; // Add spacing between characters
        }
    */
        // Present the frame
        SDL_RenderPresent(g_renderer);

        // Cap frame rate at ~60 FPS
        SDL_Delay(30);
    }

    // Cleanup resources
    NixieCleanup();

    return 0;
}
