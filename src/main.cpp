#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "NixieDisplay.cpp"
#include "Globals.hpp"
#include "init.cpp"
#include "LocalTime.cpp"

// Define text color and initial destination rectangle
SDL_Color color = {255, 225, 225, 255};
SDL_Rect destRect = {15, 0, 0, 0};

// Function to calculate flicker intensity
float getFlickerIntensity() {
    return 0.2f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.0f));
}

// Render text with a glowing effect for individual characters
void renderWithGlow(SDL_Renderer* renderer, SDL_Texture* textTexture, SDL_Rect destRect, SDL_Color glowColor, Uint8 glowAlpha, Sint8 glowIntensity) {
    // Set glow color and opacity
    SDL_SetTextureColorMod(textTexture, glowColor.r, glowColor.g, glowColor.b);
    SDL_SetTextureAlphaMod(textTexture, glowAlpha);

    // Render glow effect around the text
    for (Sint8 i = -glowIntensity; i <= glowIntensity; ++i) {
        for (Sint8 j = -glowIntensity; j <= glowIntensity; ++j) {
            if (i != 0 || j != 0) { // Skip the original position
                SDL_Rect glowRect = destRect;
                glowRect.x += i;
                glowRect.y += j;
                SDL_RenderCopy(renderer, textTexture, NULL, &glowRect);
            }
        }
    }

    // Render the main text without flicker (fully opaque and orange)
    SDL_SetTextureColorMod(textTexture, 255, 200, 0); // Orange color
    SDL_SetTextureAlphaMod(textTexture, 255);         // Full opacity
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
}

// Update the textures and surfaces for individual characters
void changeNixieText(const std::string& newText, std::vector<SDL_Texture*>& textures, std::vector<SDL_Surface*>& surfaces, SDL_Renderer* renderer, TTF_Font* font) {
    // Clear existing textures and surfaces
    for (SDL_Texture* texture : textures) {
        SDL_DestroyTexture(texture);
    }
    for (SDL_Surface* surface : surfaces) {
        SDL_FreeSurface(surface);
    }
    textures.clear();
    surfaces.clear();

    // Generate a texture and surface for each character
    for (char c : newText) {
        std::string charStr(1, c);
        SDL_Surface* surface = TTF_RenderText_Blended(font, charStr.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        surfaces.push_back(surface);
        textures.push_back(texture);
    }

    // Adjust the destination rectangle size for individual characters
    if (!surfaces.empty()) {
        destRect.w = surfaces[0]->w;
        destRect.h = surfaces[0]->h;
    }
}

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr)); // Seed for random flicker

    if (NixieInicial() != 0) {
        return 1; // Initialization failed
    }

    // Initialize variables for text rendering
    std::vector<SDL_Texture*> characterTextures;
    std::vector<SDL_Surface*> characterSurfaces;
    std::vector<float> flickerIntensities;

    bool running = true;
    SDL_Event event;

    // State for blinking colon
    bool dotBlink = true;
    Uint32 lastBlinkTime = SDL_GetTicks();
    Uint32 blinkInterval = 500; // 500ms interval for blinking

    // Main application loop
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Handle blinking state
        Uint32 currentTimeMs = SDL_GetTicks();
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

        // Present the frame
        SDL_RenderPresent(g_renderer);

        // Cap frame rate at ~60 FPS
        SDL_Delay(30);
    }

    // Cleanup resources
    for (SDL_Texture* texture : characterTextures) {
        SDL_DestroyTexture(texture);
    }
    for (SDL_Surface* surface : characterSurfaces) {
        SDL_FreeSurface(surface);
    }
    Cleanup();

    return 0;
}
