#ifndef NIXIE_DISPLAY_HPP
#define NIXIE_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LocalClock.hpp"
#include <string>
#include <vector>

/**
 * @enum renderType
 * @brief Specifies the render type options.
 * 
 * This enumeration defines whether to present or prepare rendering.
 */
enum renderType {
    RENDER_PRESENT = true,  /**< Present the rendered content. */
    RENDER_PREPARE = false  /**< Prepare rendering without presenting. */
};

/**
 * @enum clockEnable
 * @brief Controls the clock mode state.
 * 
 * This enumeration defines the options to enable or disable clock mode.
 */
enum clockEnable {
    CLOCK_ENABLE = true,    /**< Enables clock mode. */
    CLOCK_DISABLE = false   /**< Disables clock mode. */
};

/**
 * @enum blinkingDots
 * @brief Toggles the blinking dots state.
 * 
 * This enumeration defines whether blinking dots are enabled or disabled.
 */
enum blinkingDots {
    BLINKING_DOTS_ON = true,  /**< Enables blinking dots. */
    BLINKING_DOTS_OFF = false /**< Disables blinking dots. */
};

/**
 * @mainpage
 * # NixieTextFX Documentation
 * @warning This code requires SDL2 to be installed on your system.<br>
 * Please ensure that SDL2 is properly set up before attempting to compile and run this code.<br>
 * It directly interacts with SDL2.<br>
 * You will also need a basic understanding of SDL2 to effectively use the functions in this file.
 * @note
 * Additional setup instructions can be found at the official SDL2 website:
 * https://wiki.libsdl.org/Installation
 */




/**
 * @class NixieDisplay
 * @brief A class to manage and render Nixie tube displays with various font sources.
 * 
 * This class allows you to initialize and render a Nixie display either using
 * an external font file (.ttf) or embedded font data.
 * 
 * @author Aktr0s (Acer0sik)
 * @version 1.0
*/

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
    
    /**
     * @brief 
     * ## Constructor to initialize the Nixie display with an external .ttf font file.
     * 
     * This constructor initializes the Nixie display by loading a TrueType font (.ttf) from
     * the specified file path. <br>It also accepts color and size options for the text and glow effect.
     * 
     * @param renderer A pointer to the SDL_Renderer used to render the Nixie display.
     * @param fontPath The file path to the external .ttf font to be used.
     * @param textColor The color of the text to be displayed. Default is a yellowish color (225, 200, 0, 225).
     * @param glowColor The color of the glow effect around the text. Default is a red color (200, 10, 0, 128).
     * @param textSize The size of the text. Default is 120.
     */
    NixieDisplay(SDL_Renderer* renderer, const std::string& fontPath, 
                SDL_Color textColor = {225, 200, 0, 225}, SDL_Color glowColor = {200,10,0,128}, int textSize = 120); ///< Constructor to inicialize the nixie display with external .ttf file

    /**
    * @brief 
    * ## Constructor to initialize the Nixie display using embedded raw byte font data.
    * 
    * This constructor initializes the Nixie display using raw byte data for the font, allowing the use
    * of embedded fonts without requiring an external .ttf file. <br>It also accepts color and size options.
    * 
    * @param renderer A pointer to the SDL_Renderer used to render the Nixie display.
    * @param fontData A pointer to the embedded font data (raw bytes).
    * @param fontDataSize The size of the embedded font data in bytes.
    * @param textColor The color of the text to be displayed. Default is a yellowish color (225, 200, 0, 255).
    * @param glowColor The color of the glow effect around the text. Default is a red color (200, 10, 0, 128).
    * @param textSize The size of the text. Default is 120.
    */
    NixieDisplay(SDL_Renderer* renderer, const unsigned char* fontData, size_t fontDataSize,
                SDL_Color textColor = {225, 200, 0, 255}, SDL_Color glowColor = {200,10,0,128}, int textSize = 120); ///< Constructor to inicialize the nixie display using embedded raw byte font

    /**
    * @brief 
    * ## Destructor to deinitialize the NixieDisplay class.
    * 
    * This destructor cleans up any resources used by the NixieDisplay instance, such as freeing font data and SDL resources.<br>
    * It is called automatically when an instance of NixieDisplay is destroyed.
    * @warning It does not quit SDL and/or TTF using SDL_Quit(); or TTF_Quit; Respectively
    */
    ~NixieDisplay();


    /**
    * @brief 
    * ### Sets the text of the display.
    * @param newText std::string
    * ## Example usage:
    * @code
    * nixie.setText("Some String");
    * @endcode
    * @warning This method assumes the passed string is valid and non-empty.
    * @note This method does not return any value.
    */
    void setText(const std::string& newText);

    /**
    * @brief 
    * ### Renders the display based on the specified rendering type.
    * This function renders content based on the mode specified by the `type` parameter.<br>
    * The rendering behavior changes depending on whether the `type` is `RENDER_PREPARE`, `RENDER_PRESENTS`.
    * Designed to run in the loop just like SDL2
    * @param type The rendering mode to use. It can be one of the following:
    * - `RENDER_PREPARE`: The ready render is copied into the renderer but not automatically presented. Needs manual SDL2 presentation.
    * - `RENDER_PRESENT`: Renders the user interface elements.
    * ## Example usage:
    * @code
    * nixie.render(RENDER_PREPARE);
    * @endcode
    * @note This method does not return any value
    */
    void render(renderType type);

    /**
    * @brief 
    * ### Toggles the flickering effect on the display
    * @param bool enable
    * ## Example usage:
    * @code
    * nixie.toggleFlickering(true);
    * @endcode
    * @note When creating a class this defaults to false.<br>This method does not return any value.
    */
    void toggleFlickering(bool enable);

    /**
    * @brief 
    * ### Sets a radius for glow effect
    * @param int radius
    * ## Example usage:
    * @code
    * nixie.setGlowRadius(5);
    * @endcode
    * @warning It expects valid and non-negative integer.<br> Ignoring this may lead to unexpected behaviour.
    * @note When creating a class this defaults to 3.<br>This method does not return any value.
    */
    void setGlowRadius(int radius);

    /**
    * @brief 
    * ### Sets a color of the text
    * It requires SDL_Color object.
    * @param SDL_Color newColor
    * ## Example usage:
    * @code
    * SDL_Color betterColor = {255, 0, 0, 255};
    * nixie.setTextColor(betterColor) // Usage with variable
    * 
    * nixie.setTextColor({255, 0, 0, 255}); // Usage without variable
    * @endcode
    * @warning It expects valid SDL_Color object
    * @note When creating a class this defaults to {225, 200, 0, 225}.<br>This method does not return any value.<br>
    */
    void setTextColor(SDL_Color newColor);

    /**
    * @brief 
    * ### Sets a color of the glow
    * It requires SDL_Color object.
    * @param SDL_Color newColor
    * ## Example usage:
    * @code
    * SDL_Color betterColor = {90, 128, 0, 255};
    * nixie.setGlowColor(betterColor) // Usage with variable
    * 
    * nixie.setGlowColor({90, 128, 0, 255}); // Usage without variable
    * @endcode
    * @warning It expects valid SDL_Color object
    * @note When creating a class this defaults to {200,10,0,128}.<br>This method does not return any value.<br>
    */
    void setGlowColor(SDL_Color newColor);

    /**
    * @brief 
    * ### Sets the size of a displayed text
    * @param int newSize
    * ## Example usage:
    * @code
    * nixie.setTextSize(5);
    * @endcode
    * @warning It expects valid and non-negative integer.<br> Ignoring this may lead to unexpected behaviour.
    * @note When creating a class this defaults to 120.<br>This method does not return any value.
    */
    void setTextSize(int newSize);

    /**
    * @brief 
    * ### Sets the position of a displayed text
    * @param int x
    * @param int y
    * ## Example usage:
    * @code
    * nixie.setPosition(15,20);
    * @endcode
    * @warning It expects valid and non-negative integer.<br> Ignoring this may lead to unexpected behaviour.
    * @note When creating a class this defaults to x = 0 and y = 0.<br>This method does not return any value.
    */
    void setPosition(int x, int y);

    /**
    * @brief 
    * ### Sets the spacing between characters
    * @param int spacing
    * ## Example usage:
    * @code
    * nixie.setSpacing(4);
    * @endcode
    * @warning It expects valid and non-negative integer.<br> Ignoring this may lead to unexpected behaviour.
    * @note When creating a class this defaults to 0.<br>This method does not return any value.
    */
    void setSpacing(int spacing);

    /**
    * @brief 
    * ## Configures the clock display mode.
    * 
    * This function allows enabling or disabling the clock, enabling or disabling the display of seconds,
    * and toggling the blinking of the separator dots on the clock.
    *
    * @param enable Specifies whether the clock is enabled or disabled. 
    *               Use `CLOCK_ENABLE` to enable or `CLOCK_DISABLE` to disable the clock.
    * @param secondsEnabled Specifies whether seconds are displayed.
    *                       Use `WITH_SECONDS` to show seconds, or `WITHOUT_SECONDS` to hide them.
    * @param blinkDotEnabled Specifies whether the blinking dots (separator) are enabled.
    *                         Use `BLINKING_DOTS_ON` to enable blinking dots or `BLINKING_DOTS_OFF` to disable them.
    * ## Example usage:
    * @code
    * nixie.clockMode(CLOCK_ENABLE,WITHOUT_SECONDS,BLINKING_DOTS_ON);
    * // It turn on clock mode that will not display seconds and enabled separator dots
    * @endcode
    * @warning When clock mode is enabled, .setText() method will not work and might cause issues.
    * @note It does not return any value.
    */
    void clockMode(clockEnable enable, withSeconds secondsEnabled, blinkingDots blinkDotEnabled);
};

#endif