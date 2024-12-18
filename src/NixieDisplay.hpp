#ifndef NIXIE_DISPLAY_HPP
#define NIXIE_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LocalClock.hpp"
#include <string>
#include <vector>

enum renderType {
    RENDER_PRESENT = true,
    RENDER_PREPARE = false
};

enum blinkingDots {
    BLINKING_DOTS_ON = true,
        BLINKING_DOTS_OFF = false
};

enum clockEnable {
    CLOCK_ENABLE = true,
    CLOCK_DISABLE = false
};

class NixieDisplay {
private:
    // Variables
    std::vector<SDL_Texture*> textures;
    std::vector<SDL_Surface*> surfaces;
    std::vector<float> flickerIntensities;
    SDL_Rect destRect;
    SDL_Color textColor;
    SDL_Color glowColor;
    SDL_Renderer* renderer;
    TTF_Font* font;
    int textSpacing;
    int textSize;
    std::string externalFontPath;
    const unsigned char* embeddedFontData;
    size_t embeddedFontSize;
    bool flickeringEnabled;
    int glowRadius;
    bool dotVisible; // Used dynamically
    bool clockModeEnabled;
    withSeconds clockSecondsEnabled;
    bool dotBlinkEnabled;

    // Helping functions
    
    void cleanupResources();
    void updateTextSize();

    // Font loading functions
    void loadFontExternal();
    void loadFontEmbedded();
    void updateFlicker();
    void updateClock();
public:
    // Constructor and Destructor
    NixieDisplay(SDL_Renderer* renderer, const std::string& fontPath, 
                SDL_Color textColor = {225, 200, 0, 225}, SDL_Color glowColor = {200,10,0,128}, int textSize = 120);

    // Constructor for embedded fonts
    NixieDisplay(SDL_Renderer* renderer, const unsigned char* fontData, size_t fontDataSize,
                SDL_Color textColor = {225, 200, 0, 255}, SDL_Color glowColor = {200,10,0,128}, int textSize = 120);


    ~NixieDisplay();

    // Methods with extended functionality
    void setText(const std::string& newText);
    void render(renderType type);
    void toggleFlickering(bool enable);
    void setGlowRadius(int radius);
    void setTextColor(SDL_Color newColor);
    void setGlowColor(SDL_Color newColor);
    void setTextSize(int newSize);
    void setPosition(int x, int y);
    void setSpacing(int spacing);
    void clockMode(clockEnable enable, withSeconds secondsEnabled, blinkingDots blinkDotEnabled);
};

#endif