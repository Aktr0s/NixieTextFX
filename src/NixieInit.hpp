#ifndef NIXIE_INIT_HPP
#define NIXIE_INIT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Global SDL_Window pointer for the Nixie display.
 * 
 * This global variable holds the main SDL window created during initialization.
 * It is shared across the application for rendering purposes.
 */
extern SDL_Window* g_window;

/**
 * @brief Global SDL_Renderer pointer for the Nixie display.
 * 
 * This global variable holds the main SDL renderer associated with the window.
 * It is used to draw elements onto the screen.
 */
extern SDL_Renderer* g_renderer;

/**
 * @brief Initializes SDL, creates the main window and renderer.
 * 
 * This function initializes the SDL video subsystem and SDL_ttf library, 
 * and sets up the global window and renderer for the Nixie display. If any 
 * of the initialization steps fail, resources are properly cleaned up.
 * 
 * @param width Width of the SDL window. Default is 600.
 * @param height Height of the SDL window. Default is 145.
 * @return int Returns 0 on successful initialization, 1 on failure.
 * 
 * Example usage:
 * @code
 * if (NixieInicial() != 0) {
 *     std::cerr << "Failed to initialize Nixie display!" << std::endl;
 *     return -1;
 * }
 * @endcode
 */
int NixieInicial(Sint16 width = 600, Sint16 height = 145);

/**
 * @brief Cleans up SDL resources.
 * 
 * This function destroys the global window and renderer created during 
 * initialization, and quits the SDL video subsystem and SDL_ttf library.
 * 
 * Example usage:
 * @code
 * NixieCleanup();
 * @endcode
 */
void NixieCleanup();

#endif // NIXIE_INIT_HPP
