#ifndef SDLHELPER_HPP
#define SDLHELPER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <switch.h>

/** @brief SDL helper functions to turn long repetitive actions in SDL into one-liners. */
namespace SDLHelper {
    // === STATUS ===
    // Note: These aren't exact - they just increase/decrease a variable when a texture/surface is created/freed
    // Thus not using the wrappers will cause incorrect readings!
    // But can be used to check for memory leaks - textures/surfaces/memory should be zero if all are destroyed!

    /**
     * @brief Returns approximate memory usage due to textures and surfaces in use
     * @note This is in no way accurate, use it as an estimation!
     *
     * @return int number of bytes occupied by SDL textures/surfaces
     */
    int memoryUsage();

    /**
     * @brief Returns the number of allocated surfaces that haven't been destroyed yet
     *
     * @return int indicating number of surfaces
     */
    int numSurfaces();

    /**
     * @brief Returns the number of allocated textures that haven't been destroyed yet
     *
     * @return int indicating number of textures
     */
    int numTextures();

    /**
     * @brief Initialize all required/used parts of SDL
     *
     * @return true if initialzation succeeded
     * @return false otherwise
     */
    bool initSDL();

    /**
     * @brief Clean up all initialized parts of SDL
     */
    void exitSDL();

    /**
     * @brief Clears the screen (renderer) with the specified colour
     *
     * @param c color to clear with
     */
    void clearScreen(SDL_Color c);

    /**
     * @brief Convert provided surface to a texture and frees the surface
     * @note Must be called in main thread!
     *
     * @param s surface to convert to texture
     * @return pointer to texture
     */
    SDL_Texture * convertSurfaceToTexture(SDL_Surface * s);

    /**
     * @brief Create texture with given dimensions
     *
     * @param w width of texture
     * @param h height of texture
     * @return created texture
     */
    SDL_Texture * createTexture(int w, int h);

    /**
     * @brief DestroyTexture wrapper
     *
     * @param t texture to destroy
     */
    void destroyTexture(SDL_Texture * t);

    /**
     * @brief FreeSurface wrapper
     *
     * @param s surface to destroy
     */
    void freeSurface(SDL_Surface * s);

    /**
     * @brief QueryTexture wrapper
     *
     * @param t texture to query
     * @param w pointer to write width to
     * @param h pointer to write height to
     */
    void getDimensions(SDL_Texture * t, int * w, int * h);

    /**
     * @brief Get render offset
     *
     * @param x pointer to write x-coordinate to
     * @param y pointer to write y-coordinate to
     */
    void getOffset(int * x, int * y);

    /**
     * @brief Set the offset of drawing operations
     *
     * @param x x-coordinate of offset
     * @param y y-coordinate of offset
     */
    void setOffset(int x, int y);

    /**
     * @brief Get the clip rectangle
     *
     * @param x1 pointer to write x value of top left corner to
     * @param y1 pointer to write y value of top left corner to
     * @param x2 pointer to write x value of bottom right corner to
     * @param y2 pointer to write y value of bottom right corner to
     */
    void getClip(int * x1, int * y1, int * x2, int * y2);

    /**
     * @brief Reset the clip area
     */
    void resetClip();

    /**
     * @brief Set clip area
     *
     * @param x1 x value of top left corner
     * @param y1 y value of top left corner
     * @param x2 x value of bottom right corner
     * @param y2 y value of bottom right corner
     *
     * @return true on success, false otherwise
     */
    bool setClip(int x1, int y1, int x2, int y2);

    /**
     * @brief Emptys the font cache
     */
    void emptyFontCache();

    /**
     * @brief Set the font for future text rendering
     *
     * @param p file path to font file
     */
    void setFont(std::string p);

    /**
     * @brief Set height of a text line in terms of font size (normal values are 1 - 1.5+), defaults to 1.15
     *
     * @param h height of one line of text
     */
    void setFontSpacing(double h);

    /**
     * @brief Reset renderer to screen
     */
    void renderToScreen();

    /**
     * @brief Set renderer to given texture
     *
     * @param t texture to render to
     */
    void renderToTexture(SDL_Texture * t);

    /**
     * @brief Get the current texture blend mode
     *
     * @return SDL_BlendMode
     */
    SDL_BlendMode getBlendMode();

    /**
     * @brief Set the blend mode of drawn textures
     *
     * @param b blend mode to set to
     */
    void setBlendMode(SDL_BlendMode b);

    // === DRAWING FUNCTIONS ===
    // -> Draw directly to the screen/renderer

    /**
     * @brief Update screen
     */
    void draw();

    /**
     * @brief Draw an ellipse outline with given parameters
     *
     * @param c color
     * @param x center x-coordinate
     * @param y center y-coordinate
     * @param rx x radius
     * @param ry y radius
     & @param t outline thickness
     */
    void drawEllipse(SDL_Color c, int x, int y, unsigned int rx, unsigned int ry, unsigned int t);

    /**
     * @brief Draw a filled ellipse with given parameters
     *
     * @param c color
     * @param x center x-coordinate
     * @param y center y-coordinate
     * @param rx x radius
     * @param ry y radius
     */
    void drawFilledEllipse(SDL_Color c, int x, int y, unsigned int rx, unsigned int ry);

    /**
     * @brief Draw a filled rectangle with the given dimensions
     *
     * @param c color to draw with
     * @param x x-coordinate of drawing start position
     * @param y y-coordinate of drawing start position
     * @param w width of rectangle
     * @param h height of rectangle
     */
    void drawFilledRect(SDL_Color c, int x, int y, int w, int h);

    /**
     * @brief Draw a filled rounded rectangle with the given values
     *
     * @param c color to draw with
     * @param x x-coordinate of drawing start position
     * @param y y-coordinate of drawing start position
     * @param w width of rectangle
     * @param h height of rectangle
     * @param r corner radius
     */
    void drawFilledRoundRect(SDL_Color c, int x, int y, int w, int h, unsigned int r);

    /**
     * @brief Draw a rounded rectangle (outline) with given border size
     *
     * @param c color to draw with
     * @param x x-coordinate of drawing start position
     * @param y y-coordinate of drawing start position
     * @param w width of rectangle
     * @param h height of rectangle
     * @param r corner radius
     * @param b border thickness (in pixels)
     */
    void drawRoundRect(SDL_Color c, int x, int y, int w, int h, unsigned int r, unsigned int b);

    /**
     * @brief Draw a rectangle (outline) with given border size
     *
     * @param c color to render with
     * @param x x-coordinate of drawing start position
     * @param y y-coordinate of drawing start position
     * @param w width of rectangle
     * @param h height of rectangle
     * @param b border thickness (in pixels)
     */
    void drawRect(SDL_Color c, int x, int y, int w, int h, unsigned int b);

    /**
     * @brief Draw provided texture at specified coordinates tinted with given colour
     *
     * @param tex pointer of rendered texture to draw
     * @param c colour to tint with
     * @param x texture offset x-coordinate
     * @param y texture offset y-coordinate
     * @param w texture width
     * @param h texture height
     * @param tx mask offset x-coordinate
     * @param ty mask offset y-coordinate
     * @param tw mask width
     * @param th mask height
     */
    void drawTexture(SDL_Texture * tex, SDL_Color c, int x, int y, int w = -1, int h = -1, int tx = -1, int ty = -1, int tw = -1, int th = -1);

    // === RENDERING FUNCTIONS ===
    // -> Draw to a surface/texture and return it
    // -> All rendering done in white
    // -> The caller must destroy the surface/texture

    // -= SURFACES =-
    /**
     * @brief Renders a filled corner rectangle
     *
     * @param w width of rectangle
     * @param h height of rectangle
     * @return pointer to rendered surface
     */
    SDL_Surface * renderFilledRectS(int w, int h);

    /**
     * @brief Renders a basic rectangle
     *
     * @param w width of rectangle
     * @param h height of rectangle
     * @param b border thickness
     * @return pointer to rendered surface
     */
    SDL_Surface * renderRectS(int w, int h, unsigned int b);

    /**
     * @brief Renders image from image path
     * @note The scaling factors only shrink for the time being!
     *
     * @param path file path to image
     * @param xF x scaling factor
     * @param yF y scaling factor
     * @return pointer to rendered surface
     */
    SDL_Surface * renderImageS(std::string path, int xF = 1, int yF = 1);

    /**
     * @brief Renders image from image pointer and image size
     * @note The scaling factors only shrink for the time being!
     *
     * @param ptr pointer to image
     * @param size image size
     * @param xF x scaling factor
     * @param yF y scaling factor
     * @return pointer to rendered surface
     */
    SDL_Surface * renderImageS(u8 * ptr, size_t size, int xF = 1, int yF = 1);

    /**
     * @brief Renders text
     * @note Always drawn in white!
     *
     * @param str text to render
     * @param font_size font size to render width in
     * @param style font style to render with
     * @return pointer to rendered surface
     */
    SDL_Surface * renderTextS(std::string str, int font_size, int style = TTF_STYLE_NORMAL);

    /**
     * @brief Renders text with specified width
     *
     * @param str text to render
     * @param font_size font size to render width in
     * @param max_w max width to render in
     * @param style font style to render with
     * @return pointer to rendered surface
     */
    SDL_Surface * renderTextWrappedS(std::string str, int font_size, uint32_t max_w, int style = TTF_STYLE_NORMAL);

    /**
     * @brief Render an ellipse outline with given parameters
     *
     * @param rx x radius
     * @param ry y radius
     * @param t outline thickness
     */
    SDL_Texture * renderEllipse(unsigned int rx, unsigned int ry, unsigned int t);

    /**
     * @brief Render a filled ellipse
     *
     * @param rx x radius
     * @param ry y radius
     * @return pointer to rendered texture
     */
    SDL_Texture * renderFilledEllipse(unsigned int rx, unsigned int ry);

    /**
     * @brief Renders a filled corner rectangle
     *
     * @param w width of rectangle
     * @param h height of rectangle
     * @return pointer to rendered texture
     */
    SDL_Texture * renderFilledRect(int w, int h);

    /**
     * @brief Renders a filled rounded corner rectangle
     *
     * @param w width of rectangle
     * @param h height of rectangle
     * @param c corner radius
     * @return pointer to rendered texture
     */
    SDL_Texture * renderFilledRoundRect(int w, int h, unsigned int c);

    /**
     * @brief Renders a rounded corner rectangle
     *
     * @param w width of rectangle
     * @param h height of rectangle
     * @param r corner radius
     * @param b border thickness
     * @return pointer to rendered texture
     */
    SDL_Texture * renderRoundRect(int w, int h, unsigned int r, unsigned int b);

    /**
     * @brief Renders a basic rectangle
     *
     * @param w width of rectangle
     * @param h height of rectangle
     * @param b border thickness
     * @return pointer to rendered texture
     */
    SDL_Texture * renderRect(int w, int h, unsigned int b);

    /**
     * @brief Renders image from image path
     *
     * @param path file path to image
     * @param xF x scaling factor
     * @param yF y scaling factor
     * @return pointer to rendered texture
     */
    SDL_Texture * renderImage(std::string path, int xF = 1, int yF = 1);

    /**
     * @brief Renders image from image pointer and image size
     *
     * @param ptr pointer to image
     * @param size image size
     * @param xF x scaling factor
     * @param yF y scaling factor
     * @return pointer to rendered texture
     */
    SDL_Texture * renderImage(u8 * ptr, size_t size, int xF = 1, int yF = 1);

    /**
     * @brief Renders text
     * @note Always drawn in white!
     *
     * @param str text to render
     * @param font_size font size to render width in
     * @param style font style to render with
     * @return pointer to rendered texture
     */
    SDL_Texture * renderText(std::string str, int font_size, int style = TTF_STYLE_NORMAL);

    /**
     * @brief Renders text with specified width
     *
     * @param str text to render
     * @param font_size font size to render width in
     * @param max_w max width to render in
     * @param style font style to render with
     * @return pointer to rendered texture
     */
    SDL_Texture * renderTextWrapped(std::string str, int font_size, uint32_t max_w, int style = TTF_STYLE_NORMAL);
};

#endif