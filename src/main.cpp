#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "globals.h"
#include "init.cpp"
#include "localTime.cpp"
SDL_Color color = {255, 225, 225, 255};
SDL_Rect destRect = {15, 0, 0, 0};
// Function to calculate flicker intensity (same as your previous function, but now returning float)

float getFlickerIntensity() {
    return 0.2f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.0f));
}

void renderWithGlow(SDL_Renderer* renderer, SDL_Texture* textTexture, SDL_Rect destRect, SDL_Color glowColor, bool flickering, Sint8 glowIntensity) {
    // Get the flicker intensity value, which will control the opacity of the glow
    Uint8 glowAlpha;
    if (flickering){
        float flickerIntensity = getFlickerIntensity();
        glowAlpha = static_cast<Uint8>(128 * flickerIntensity);  // Scale the glow alpha based on flicker intensity
    } else {
        glowAlpha = static_cast<Uint8>(128);
    }
    // Render the glow with adjusted opacity
    SDL_SetTextureColorMod(textTexture, glowColor.r, glowColor.g, glowColor.b); // Set glow color
    SDL_SetTextureAlphaMod(textTexture, glowAlpha); // Adjust glow opacity based on flicker intensity
    for (Sint8 i = -glowIntensity; i <=glowIntensity; ++i) {
        for (Sint8 j = -glowIntensity; j <= glowIntensity; ++j) {
            if (i != 0 || j != 0) { // Avoid overwriting the original position
                SDL_Rect glowRect = destRect;
                glowRect.x += i;
                glowRect.y += j;
                SDL_RenderCopy(renderer, textTexture, NULL, &glowRect);
            }
        }
    }

    // Render the main digit (no flicker, always fully opaque)
    SDL_SetTextureColorMod(textTexture, 255, 200, 0); // Set the main color (orange)
    SDL_SetTextureAlphaMod(textTexture, 255); // Full opacity for the main text
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
}

void changeNixieText(const std::string& newText, SDL_Texture*& customTexture, SDL_Surface*& customSurface, SDL_Renderer* renderer, TTF_Font* font) {
    // Check if the new text is valid
    std::string textToDisplay = newText.empty() ? "0123456789" : newText;

    // Free the old texture and surface if they exist
    if (customTexture) {
        SDL_DestroyTexture(customTexture);
    }
    if (customSurface) {
        SDL_FreeSurface(customSurface);
    }

    // Render the new text surface
    customSurface = TTF_RenderText_Blended(font, textToDisplay.c_str(), color);

    // Create a new texture from the surface
    customTexture = SDL_CreateTextureFromSurface(renderer, customSurface);

    // Update the destination rectangle with the new text size
    destRect.w = customSurface->w;
    destRect.h = customSurface->h;
}


int main(int argc, char* argv[]) {
    std::srand(std::time(NULL));

    if (SDLInicial() != 0) {
        return 1; // Initialization failed
    }

    //Orange
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *textTexture = nullptr;

    
    SDL_FreeSurface(textSurface);

    bool running = true;
    SDL_Event event;

    bool dotBlink = false; // State for colon blinking
    Uint32 lastBlinkTime = SDL_GetTicks(); // Track the last blink time
    Uint32 blinkInterval = 500; // Blink interval in milliseconds

while (running) {
    // Handle events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }

    // Get the current time in milliseconds
    Uint32 currentTimeMs = SDL_GetTicks();

    // Update `dotBlink` based on the blink interval
    if (currentTimeMs - lastBlinkTime >= blinkInterval) {
        dotBlink = !dotBlink; // Toggle the colon state
        lastBlinkTime = currentTimeMs; // Reset blink timer
    }

    // Get the current time string with blinking colon
    std::string currentTime = getLocalTime(C_STYLE, dotBlink);

    // Update the text texture only if the time string has changed
    static std::string previousTime;
    if (currentTime != previousTime) {
        changeNixieText(currentTime, textTexture, textSurface, g_renderer, g_font);
        previousTime = currentTime;
    }

    // Clear screen
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    // Glow color with flickering
    SDL_Color glowColor = {200, 10, 0, 128};
    renderWithGlow(g_renderer, textTexture, destRect, glowColor, true, 3);

    // Present frame
    SDL_RenderPresent(g_renderer);

    // Cap frame rate
    SDL_Delay(30); // Roughly 60 FPS
}



    SDL_DestroyTexture(textTexture);
    Cleanup();

    return 0;
}
