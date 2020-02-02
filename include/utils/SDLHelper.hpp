#ifndef SDLHELPER_HPP
#define SDLHELPER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <switch.h>

// These are a bunch of functions to turn long repetitive actions in SDL
// into one-liners.
namespace SDLHelper {
    // Initialize all required/used parts of SDL
    // Returns true if all succeeded, false otherwise
    bool initSDL();
    // Clean up all initialized parts of SDL
    void exitSDL();

    // Clears the screen (renderer) with the specified colour
    void clearScreen(SDL_Color);

    // Create texture with given dimensions
    SDL_Texture * createTexture(int, int);
    // Wrapper for DestroyTexture
    void destroyTexture(SDL_Texture *);
    // Wrapper for QueryTexture
    void getDimensions(SDL_Texture *, int *, int *);

    // Return the render offset in the given pointers (x, y)
    void getOffset(int *, int *);
    // Set the offset of drawing operations
    void setOffset(int, int);

    // Reset renderer to screen
    void renderToScreen();
    // Set renderer to given texture
    void renderToTexture(SDL_Texture *);

    // Return the current texture blend mode
    SDL_BlendMode getBlendMode();
    // Set the blend mode of drawn textures
    void setBlendMode(SDL_BlendMode);
    // Set the blend mode of draw operations (i.e. render to screen ops)
    void setRendererBlendMode(SDL_BlendMode);

    // === DRAWING FUNCTIONS ===
    // -> Draw directly to the screen/renderer

    // Update screen
    void draw();

    // Draw an ellipse with given diameters
    // Colour, center x, center y, x diameter, y diameter
    void drawEllipse(SDL_Color, int, int, unsigned int, unsigned int);

    // Draw a filled rectangle with the given dimensions
    // Colour, x, y, w, h
    void drawFilledRect(SDL_Color, int, int, int, int);

    // Draw a filled rounded rectangle with the given values
    // Colour, x, y, w, h, corner radius
    void drawFilledRoundRect(SDL_Color, int, int, int, int, unsigned int);

    // Draw a rounded rectangle (outline) with given border size
    // Colour, x, y, w, h, corner radius, border size
    void drawRoundRect(SDL_Color, int, int, int, int, unsigned int, unsigned int);

    // Draw a rectangle (outline) with given border size
    // Colour, x, y, w, h, border thickness (in px)
    void drawRect(SDL_Color, int, int, int, int, unsigned int);

    // Draw provided texture at specified coordinates tinted with given colour
    // Texture, Colour, x, y, w (op), h (op), maskX (op), maskY (op), maskW (op), maskH (op)
    void drawTexture(SDL_Texture *, SDL_Color, int, int, int = -1, int = -1, int = -1, int = -1, int = -1, int = -1);

    // === RENDERING FUNCTIONS ===
    // -> Draw to a texture and return it
    // -> The caller must destroy the texture

    // Simply changes target to texture and then calls draw...() with white colour

    // x diameter, y diameter
    SDL_Texture * renderEllipse(unsigned int, unsigned int);
    // width, height
    SDL_Texture * renderFilledRect(int, int);
    // width, height, corner radius
    SDL_Texture * renderFilledRoundRect(int, int, unsigned int);
    // width, height, corner radius, border thickness
    SDL_Texture * renderRoundRect(int, int, unsigned int, unsigned int);
    // width, height, border thickness
    SDL_Texture * renderRect(int, int, unsigned int);

    // Reads an image from a pointer to it and returns a texture containing it
    SDL_Texture * renderImage(u8 *, size_t);
    // Returns a texture with the specified text drawn at the specified font size
    // Always drawn in white!
    SDL_Texture * renderText(const char *, int, bool = false);
    // Same as renderText but wraps text at given width
    SDL_Texture * renderTextWrapped(const char *, int, uint32_t, bool = false);
};

#endif