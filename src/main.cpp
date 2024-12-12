#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "globals.h"
#include "init.cpp"
SDL_Color color = {255, 225, 225, 255};
SDL_Rect destRect = {10, 0, 0, 0};
// Function to calculate flicker intensity (same as your previous function, but now returning float)

std::string getLocalTime() {
    // Get current time
    std::time_t now = std::time(0);
    
    // Convert to local time
    struct tm* localTime = std::localtime(&now);
    
    // Use stringstream to format time
    std::stringstream timeStream;
    timeStream << std::setfill('0') 
               << std::setw(2) << localTime->tm_hour << ":"
               << std::setw(2) << localTime->tm_min << ":"
               << std::setw(2) << localTime->tm_sec;

    // Return the formatted time string
    return timeStream.str();
}

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
    std::cout << static_cast<int>(glowAlpha) << std::endl;
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

    while (running) {
    // Handle events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }
    std::string localTimeStr = getLocalTime();
    const char* localTimeCStr = localTimeStr.c_str();

    //Time update
    changeNixieText(localTimeCStr, textTexture, textSurface, g_renderer, g_font);

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
