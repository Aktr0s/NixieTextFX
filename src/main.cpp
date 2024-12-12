#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Function to calculate flicker intensity (same as your previous function, but now returning float)
float getFlickerIntensity() {
    return 0.2f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.0f));
}

void renderWithGlow(SDL_Renderer* renderer, SDL_Texture* textTexture, SDL_Rect destRect, SDL_Color glowColor, bool flickering) {
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
    for (int i = -6; i <=6; ++i) {
        for (int j = -6; j <= 6; ++j) {
            if (i != 0 || j != 0) { // Avoid overwriting the original position
                SDL_Rect glowRect = destRect;
                glowRect.x += i;
                glowRect.y += j;
                SDL_RenderCopy(renderer, textTexture, NULL, &glowRect);
            }
        }
    }

    // Render the main digit (no flicker, always fully opaque)
    SDL_SetTextureColorMod(textTexture, 255, 165, 0); // Set the main color (orange)
    SDL_SetTextureAlphaMod(textTexture, 255); // Full opacity for the main text
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);
}


int main(int argc, char* argv[]) {
    std::srand(std::time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init Error: " << SDL_GetError();
        return 1;
    }

    if (TTF_Init() != 0){
        std::cerr << "TTF_Init Error: " << TTF_GetError();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("NixieTube Test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600, SDL_WINDOW_SHOWN);
    if (!window){
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if (!renderer){
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("../assets/NixieOne-Regular.ttf",120);
    if (!font){
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color color = {255, 225, 134, 255}; //Orange
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, "0123456789", color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer,textSurface);

    SDL_Rect destRect = {100, 50, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);

    bool running = true;
    SDL_Event event;

    while (running) {
    // Handle events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Random flickering intensity
    float flickerIntensity = getFlickerIntensity();

    // Glow color with flickering
    SDL_Color glowColor = {219, 48, 32, 128};
    renderWithGlow(renderer, textTexture, destRect, glowColor, true);

    // Present frame
    SDL_RenderPresent(renderer);

    // Cap frame rate
    SDL_Delay(30); // Roughly 60 FPS
}


    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
