#ifndef SDLHELPER_HPP
#define SDLHELPER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>
#include <unordered_map>

// These are a bunch of functions to turn long repetitive actions in SDL
// into one-liners.
namespace SDLHelper {
    // Initialize all required/used parts of SDL
    // Returns true if all succeeded, false otherwise
    bool initSDL();

    // Clean up all initialized parts of SDL
    void exitSDL();

    // Clears the screen (renderer) with the specified colour
    void clearScreen();

    // Wrapper for DestroyTexture
    void destroyTexture(SDL_Texture *);

    // Wrapper for QueryTexture
    void getDimensions(SDL_Texture *, int *, int *);

    // Set clear screen colour
    void setColour(SDL_Color);

    // === DRAWING FUNCTIONS ===
    // -> Draw directly to the screen/renderer

    // Update screen
    void draw();

    // Draw a rectangle with given colour, position + size
    void drawRect(SDL_Color, int, int, int, int);

    // Draw provided texture at specified coordinates tinted with given colour
    // width and height are optional
    void drawTexture(SDL_Texture *, SDL_Color, int, int, int = -1, int = -1, int = -1, int = -1, int = -1, int = -1);

    // === RENDERING FUNCTIONS ===
    // -> Draw to a texture and return it
    // -> The caller must destroy the texture

    // Draw an ellipse with given diameters
    SDL_Texture * renderEllipse(unsigned int, unsigned int);

    // Reads an image from a pointer to it and returns a texture containing it
    SDL_Texture * renderImage(u8 *, size_t);

    // Draw a whtie rounded box with the given values
    SDL_Texture * renderRoundedBox(int, int, unsigned int, unsigned int);

    // Draw a white rounded rectangle with the given values
    SDL_Texture * renderRoundedRect(int, int, unsigned int);

    // Draw a white rectangle with the given dimensions
    SDL_Texture * renderRect(int, int);

    // Returns a texture with the specified text drawn at the specified font size
    // Always drawn in white!
    SDL_Texture * renderText(const char *, int, bool = false);

    // Same as renderText but wraps text at given width
    SDL_Texture * renderTextWrapped(const char *, int, uint32_t, bool = false);

    // === MISCELLANEOUS ===
    // Set an offset for all drawing
    void setOffset(int, int);
    // Reset offset to zero
    void resetOffset();

    // Create texture with given dimensions
    SDL_Texture * createTexture(int, int);

    // Reset renderer to screen
    void renderToScreen();
    // Set renderer to given texture
    void renderToTexture(SDL_Texture *);
};

#endif