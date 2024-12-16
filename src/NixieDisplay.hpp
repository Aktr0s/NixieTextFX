#ifndef NIXIE_DISPLAY_HPP
#define NIXIE_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

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
    SDL_RWops* fontRW;
    TTF_Font* font;
    int textSpacing;
    int textSize;
    std::string externalFontPath;
    const unsigned char* embeddedFontData;
    size_t embeddedFontSize;
    bool flickeringEnabled;
    int glowRadius;
    bool clockMode;
    bool clockSeconds;

    // Helping functions
    void updateFlicker();
    void cleanupResources();
    void updateTextSize();

    // Font loading functions
    void loadFontExternal();
    void loadFontEmbedded();
    void updateClock();

public:
    // Constructor and Destructor
    NixieDisplay(SDL_Renderer* renderer, const std::string& fontPath, 
                 SDL_Color textColor, SDL_Color glowColor, int textSize = 50);

    // Constructor for embedded fonts
    NixieDisplay(SDL_Renderer* renderer, const unsigned char* fontData, size_t fontDataSize, 
                 SDL_Color textColor, SDL_Color glowColor, int textSize = 50);


    ~NixieDisplay();

    // Methods with extended functionality
    void setText(const std::string& newText);
    void render();
    void toggleFlickering(bool enable);
    void setGlowRadius(int radius);
    void setTextColor(SDL_Color newColor);
    void setGlowColor(SDL_Color newColor);
    void setTextSize(int newSize);
    void setPosition(int x, int y);
    void setSpacing(int spacing);
};

#endif