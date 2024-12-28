#include "NixieDisplay.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

NixieDisplay::NixieDisplay(SDL_Renderer* renderer, const std::string& fontPath, 
                           SDL_Color textColor, SDL_Color glowColor, int textSize)
    : renderer(renderer), font(nullptr), textColor(textColor), glowColor(glowColor), 
      textSize(textSize), externalFontPath(fontPath), embeddedFontData(nullptr), embeddedFontSize(0) {
    if (fontPath.empty()) {
        throw std::invalid_argument("Font path cannot be empty for external font.");
    }
    loadFontExternal(); // Load the external font
    destRect = {0, 0, 0, 0}; // Initialize position
    glowRadius = 3; // Default glow radius
    flickeringEnabled = false; // Disable flickering by default
    textSpacing = 0; // Default spacing
    clockModeEnabled = false; // On default clockMode is dissabled
    clockSecondsEnabled = WITHOUT_SECONDS; // And on default it does not show seconds
    dotVisible = true;
    std::srand(std::time(nullptr));
}

NixieDisplay::NixieDisplay(SDL_Renderer* renderer, const unsigned char* fontData, size_t fontDataSize, 
                           SDL_Color textColor, SDL_Color glowColor, int textSize)
    : renderer(renderer), font(nullptr), textColor(textColor), glowColor(glowColor), 
      textSize(textSize), externalFontPath(""), embeddedFontData(fontData), embeddedFontSize(fontDataSize) {
    if (!fontData || fontDataSize == 0) {
        throw std::invalid_argument("Embedded font data cannot be null or empty.");
    }
    loadFontEmbedded(); // Load the embedded font
    destRect = {0, 0, 0, 0}; // Initialize position
    glowRadius = 3; // Default glow radius
    flickeringEnabled = false; // Disable flickering by default
    textSpacing = 0; // Default spacing
    clockModeEnabled = false; // On default clockMode is dissabled
    clockSecondsEnabled = WITHOUT_SECONDS; // And on default it does not show seconds
    dotVisible = true;
    std::srand(std::time(nullptr));
}

//Destructor
NixieDisplay::~NixieDisplay() {
    cleanupResources(); // Free resources
    //TTF_CloseFont(font); //Causes EXC_BAD_ACCESS
}

void NixieDisplay::loadFontExternal() {
    font = TTF_OpenFont(externalFontPath.c_str(), textSize);
    if (!font) {
        throw std::runtime_error("Failed to load external font: " + externalFontPath);
    }
}

void NixieDisplay::loadFontEmbedded() {
    SDL_RWops* rw = SDL_RWFromConstMem(embeddedFontData, embeddedFontSize);
    if (!rw) {
        throw std::runtime_error("Failed to create SDL_RWops for embedded font.");
    }
    font = TTF_OpenFontRW(rw, 1, textSize); // `1` means SDL will close the RWops
    if (!font) {
        throw std::runtime_error("Failed to load embedded font.");
    }
}

void NixieDisplay::updateFlicker() {
    if (!flickeringEnabled) return;

    for (float& intensity : flickerIntensities) {
        intensity = 0.2f + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / 1.0f));
    }
}

// Free all textures and surfaces
void NixieDisplay::cleanupResources() {
    for (SDL_Texture* texture : textures) {
        SDL_DestroyTexture(texture);
    }
    for (SDL_Surface* surface : surfaces) {
        SDL_FreeSurface(surface);
    }
    textures.clear();
    surfaces.clear();
    flickerIntensities.clear();
}

// Recalculate the font size
void NixieDisplay::updateTextSize() {
    TTF_SetFontSize(font,textSize);
}

// Update the text
void NixieDisplay::setText(const std::string& newText) {
    cleanupResources(); // Clear existing resources

    for (char c : newText) {
        // Render character to a surface
        std::string charStr(1, c);
        SDL_Surface* surface = TTF_RenderText_Blended(font, charStr.c_str(), textColor);
        if (!surface) {
            std::cerr << "Error creating surface: " << SDL_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        textures.push_back(texture);
        surfaces.push_back(surface);
        flickerIntensities.push_back(1.0f); // Default flicker intensity
    }
}

// Render the nixie display
void NixieDisplay::render(renderType type) {
    updateClock();
    updateFlicker(); // Update flicker intensities
    SDL_SetRenderDrawColor(renderer, 21, 23, 24, 255); // Black background
    SDL_RenderClear(renderer);

    int xOffset = destRect.x; // Start rendering from initial position
    for (size_t i = 0; i < textures.size(); ++i) {
        if (!textures[i]) continue;

        Uint8 glowAlpha = static_cast<Uint8>(128 * flickerIntensities[i]); // Adjust glow alpha


        SDL_Rect charRect = {xOffset, destRect.y, surfaces[i]->w, surfaces[i]->h};

        // Render glow
        SDL_SetTextureColorMod(textures[i], glowColor.r, glowColor.g, glowColor.b);
        SDL_SetTextureAlphaMod(textures[i], glowAlpha);
        for (int dx = -glowRadius; dx <= glowRadius; ++dx) {
            for (int dy = -glowRadius; dy <= glowRadius; ++dy) {
                if (dx != 0 || dy != 0) {
                    SDL_Rect glowRect = charRect;
                    glowRect.x += dx;
                    glowRect.y += dy;
                    SDL_RenderCopy(renderer, textures[i], nullptr, &glowRect);
                }
            }
        }

        // Render the main character
        SDL_SetTextureColorMod(textures[i], textColor.r, textColor.g, textColor.b);
        SDL_SetTextureAlphaMod(textures[i], 255);
        SDL_RenderCopy(renderer, textures[i], nullptr, &charRect);

        xOffset += charRect.w + textSpacing; // Space between characters
    }
    if (type) {
        SDL_RenderPresent(renderer);
    }
}

void NixieDisplay::updateClock(){
    if (clockModeEnabled){
        std::string currentTime = getLocalTime(C_STYLE, dotVisible, clockSecondsEnabled);
        static std::string previousTime;
        if (currentTime != previousTime) {
            previousTime = currentTime;
            if (dotBlinkEnabled) {
                dotVisible = !dotVisible;
            }
            setText(currentTime);
        }
    }
}

void NixieDisplay::clockMode(clockEnable enable, withSeconds secondsEnabled, blinkingDots blinkDotEnabled){
    clockModeEnabled = enable;
    clockSecondsEnabled = secondsEnabled;
    dotBlinkEnabled = blinkDotEnabled;
}


// Toggle flickering
void NixieDisplay::toggleFlickering(bool enable) {
    flickeringEnabled = enable;
}

// Set glow radius
void NixieDisplay::setGlowRadius(int radius) {
    glowRadius = radius;
}

// Change text color
void NixieDisplay::setTextColor(SDL_Color newColor) {
    textColor = newColor;
}

// Change glow color
void NixieDisplay::setGlowColor(SDL_Color newColor) {
    glowColor = newColor;
}

// Change text size
void NixieDisplay::setTextSize(int newSize) {
    textSize = newSize;
    updateTextSize();
}

// Set position
void NixieDisplay::setPosition(int x, int y) {
    destRect.x = x;
    destRect.y = y;
}

void NixieDisplay::setSpacing(int newSpacing) {
    textSpacing = newSpacing;
}
