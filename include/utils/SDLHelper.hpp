#ifndef SDLHELPER_HPP
#define SDLHELPER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <switch.h>

// These are a bunch of functions to turn long repetitive actions in SDL
// into one-liners.
namespace SDLHelper {
    // Initialize all required/used parts of SDL
    // Returns true if all succeeded, false otherwise
    bool initSDL();
    // Clean up all initialized parts of SDL
    void exitSDL();

    // === STATUS ===
    // Note: These aren't exact - they just increase/decrease a variable when a texture/surface is created/freed
    // But can be used to check for memory leaks - textures/surfaces/memory should be zero if all are destroyed!
    // Returns approximate memory usage by textures and surfaces (in bytes)
    // Only returns (w * h * (bytes per pixel)), so not accurate!
    int memoryUsage();
    // Returns number of surface (allocations - frees)
    int numSurfaces();
    // Returns number of texture (allocations - frees)
    int numTextures();

    // Clears the screen (renderer) with the specified colour
    void clearScreen(SDL_Color);

    // Convert surface to texture (must be called in main thread)
    // Automatically frees surface
    SDL_Texture * convertSurfaceToTexture(SDL_Surface *);
    // Create texture with given dimensions
    SDL_Texture * createTexture(int, int);
    // Wrapper for DestroyTexture
    void destroyTexture(SDL_Texture *);
    // Wrapper for FreeSurface
    void freeSurface(SDL_Surface *);
    // Wrapper for QueryTexture
    void getDimensions(SDL_Texture *, int *, int *);

    // Return the render offset in the given pointers (x, y)
    void getOffset(int *, int *);
    // Set the offset of drawing operations
    void setOffset(int, int);

    // Self-explanatory
    void emptyFontCache();
    // Set the font for future text rendering
    void setFont(std::string);

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
    // -> Draw to a surface/texture and return it
    // -> All rendering done in white
    // -> The caller must destroy the surface/texture

    // -= SURFACES =-
    // width, height
    SDL_Surface * renderFilledRectS(int, int);
    // width, height, border thickness
    SDL_Surface * renderRectS(int, int, unsigned int);

    // path to image, x scale factor, y scale factor (only shrinks!)
    SDL_Surface * renderImageS(std::string, int = 1, int = 1);
    // same as above but reads from pointer to memory
    SDL_Surface * renderImageS(u8 *, size_t, int = 1, int = 1);

    // string to render, font size, font style
    SDL_Surface * renderTextS(std::string, int, int = TTF_STYLE_NORMAL);
    // same as above but wraps at given width (uint32_t arg)
    SDL_Surface * renderTextWrappedS(std::string, int, uint32_t, int = TTF_STYLE_NORMAL);

    // -= TEXTURES =-
    // x diameter, y diameter
    SDL_Texture * renderEllipse(unsigned int, unsigned int);
    // width, height, corner radius
    SDL_Texture * renderFilledRoundRect(int, int, unsigned int);
    // width, height, corner radius, border thickness
    SDL_Texture * renderRoundRect(int, int, unsigned int, unsigned int);

    // These call the surface functions above and then convert to a texture (or call renderer functions if available)
    SDL_Texture * renderFilledRect(int, int);
    SDL_Texture * renderRect(int, int, unsigned int);
    SDL_Texture * renderImage(std::string, int = 1, int = 1);
    SDL_Texture * renderImage(u8 *, size_t, int = 1, int = 1);
    SDL_Texture * renderText(std::string, int, int = TTF_STYLE_NORMAL);
    SDL_Texture * renderTextWrapped(std::string, int, uint32_t, int = TTF_STYLE_NORMAL);
};

#endif