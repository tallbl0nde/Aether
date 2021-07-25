#ifndef AETHER_RENDERER_HPP
#define AETHER_RENDERER_HPP

#include "Aether/types/Colour.hpp"
#include <atomic>
#include <functional>
#include <mutex>
#include <stack>
#include <string>
#include <vector>

// Forward declare member types to reduce compilation time
namespace Aether {
    class FontCache;
    class Drawable;
};
struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Window;

namespace Aether {
    /**
     * @brief Aether's main renderer instance. It provides all methods relating
     * to drawing directly to the screen, rendering graphics to memory as well as
     * measuring resource usage (with respect to textures).
     * @note Presenting individual textures are done with the \ref Drawable class.
     */
    class Renderer {
        // Allow access to private members
        friend Drawable;
        friend FontCache;

        public:
            /** @brief Function to handle log messages */
            typedef std::function<void(const std::string, const bool)> LogHandler;

        private:
            SDL_Renderer * renderer;                         /** @brief SDL renderer */
            SDL_Window * window;                             /** @brief SDL window object */
            unsigned int windowWidth_;                       /** @brief Width of window */
            unsigned int windowHeight_;                      /** @brief Height of window */

            LogHandler logHandler;                           /** @brief Handler for log messages */
            std::atomic<unsigned long> memoryUsage_;         /** @brief Number of bytes used by surfaces/textures */
            std::atomic<unsigned int> surfaceCount_;         /** @brief Number of created surfaces */
            std::atomic<unsigned int> textureCount_;         /** @brief Number of created textures */

            std::stack<SDL_Rect *> clipStack;                /** @brief Stack of clipping rectangles */

            FontCache * fontCache;                           /** @brief Object that cache's font data */
            double fontSpacing;                              /** @brief Height of one line of wrapped text (multiple of line height) */

            std::mutex imgMtx;                               /** @brief Mutex protecting access to SDL_image */
            std::mutex ttfMtx;                               /** @brief Mutex protecting access to SDL_ttf */

            /**
             * @brief Small helper to invoke log handler only if one is set.
             *
             * @param msg Log message
             * @param imp Set true to mark message as 'important'
             */
            void logMessage(const std::string & msg, const bool imp);

            /**
             * @brief Convert the given surface to a texture, destroying the surface.
             * The passed surface is always destroyed, even on an error.
             * @note Must be called in the same thread that initialized the renderer.
             *
             * @param surf The surface to convert
             * @return The created texture, or nullptr if an error occurred.
             */
            SDL_Texture * convertSurfaceToTexture(SDL_Surface * surf);

            /**
             * @brief Create a blank, transparent texture with the given dimensions.
             *
             * @param width Required width
             * @param height Required height
             * @return The created texture, or nullptr if an error occurred.
             */
            SDL_Texture * createTexture(const unsigned int width, const unsigned int height);

            /**
             * @brief Destroy the given texture. Does nothing if passed nullptr.
             *
             * @param tex Texture to destroy
             * @param stats Whether to decrement usage stats
             */
            void destroyTexture(SDL_Texture * tex, const bool stats);

            /**
             * @brief Destroy the given surface. Does nothing if passed nullptr.
             *
             * @param surf Surface to destroy
             * @param stats Whether to decrement usage stats
             */
            void destroySurface(SDL_Surface * surf, const bool stats);

            /**
             * @brief Draw a filled rectangle to the framebuffer with the given position + dimensions.
             *
             * @param col Colour to fill rectangle with
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param width Width of rectangle
             * @param height Height of rectangle
             */
            void drawFilledRect(const Colour & col, const int x, const int y, const int width, const int height);

            /**
             * @brief Draw the given texture to the framebuffer.
             *
             * @param tex The texture to render
             * @param col Colour to tint texture with
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param width Width of area to stretch texture to if needed
             * @param height Height of area to stretch texture to if needed
             * @param maskX Top-left x mask coordinate
             * @param maskY Top-left y mask coordinate
             * @param maskW Mask width
             * @param maskH Mask height
             */
            void drawTexture(SDL_Texture * tex, const Colour & col, const int x, const int y, const unsigned int width, const unsigned int height, const int maskX, const int maskY, const unsigned int maskW, const unsigned int maskH);

            /**
             * @brief Perform all actions in the passed function on the given texture.
             * Also clears existing content on texture.
             *
             * @param tex The texture to render to
             * @param func Function containing actions to perform on the texture
             */
            void renderOnTexture(SDL_Texture * tex, const std::function<void(SDL_Renderer *)> & func);

            /**
             * @brief Scales the provided surface to the requested dimensions.
             * This method will always return a valid surface, even if scaling fails.
             *
             * @param surface The surface to scale
             * @param width The width to scale the surface to (in pixels)
             * @param height The height to scale the surface to (in pixels)
             */
            SDL_Surface * scaleSurface(SDL_Surface * surface, const size_t width, const size_t height);

        public:
            /**
             * @brief Initializes the renderer object, but does not initialize the actual
             * rendering backend (SDL). See \ref initialize().
             */
            Renderer();

            /**
             * @brief Sets the handler to be used for logging. Expects a function
             * that is passed the log message as a string, and a boolean specifying
             * if the message is 'important'. Note that this may be called extremely
             * quickly depending on the circumstances.
             *
             * @param func The function to handle the provided log message, or nullptr
             * to disable logging
             */
            void setLogHandler(const LogHandler & func);

            /**
             * @brief Initialize the rendering backend.
             * @note Window name + size cannot be changed on some platforms.
             *
             * @param name Name of window
             * @param windowW Width of window to create
             * @param windowH Height of window to create
             *
             * @return true on success, false otherwise
             */
            bool initialize(const std::string & name, const unsigned int windowW, const unsigned int windowH);

            /**
             * @brief Clean up the rendering backend. This should always be called, even if
             * initialization failed.
             */
            void cleanup();

            /**
             * @brief Returns an approximated value for memory used by all rendered textures.
             * @note This isn't very accurate, it is useful for estimation purposes only.
             *
             * @return Number of *bytes* allocated to all current textures/surfaces.
             */
            unsigned int memoryUsage();

            /**
             * @brief Returns the number of allocated 'surfaces' that haven't been destroyed.
             *
             * @return Number of surfaces created by the renderer that haven't been freed.
             */
            unsigned int surfaceCount();

            /**
             * @brief Returns the number of allocated 'textures' that haven't been destroyed.
             *
             * @return Number of textures created by the renderer that haven't been freed.
             */
            unsigned int textureCount();

            /**
             * @brief Returns the width of the window.
             *
             * @return Width of rendering window.
             */
            unsigned int windowWidth();

            /**
             * @brief Returns the height of the window.
             *
             * @return Height of rendering window.
             */
            unsigned int windowHeight();

            /**
             * @brief Fill the window with the given colour
             *
             * @param col Colour to fill the window with (can be transparent)
             */
            void fillWindow(const Colour & col);

            /**
             * @brief Present all rendered graphics
             */
            void present();

            /**
             * @brief Reset the clipping area for rendered objects
             */
            void resetClipArea();

            /**
             * @brief Set the clipping area for rendered objects
             *
             * @param x1 x value of top left corner
             * @param y1 y value of top left corner
             * @param x2 x value of bottom right corner
             * @param y2 y value of bottom right corner
             */
            void setClipArea(const int x1, const int y1, const int x2, const int y2);

            /**
             * @brief Set the font used for text rendering from a file. Pass an empty string
             * to revert to the default font.
             * @note Supports TTF.
             *
             * @param path Path to font file
             */
            void setFont(const std::string & path);

            /**
             * @brief Set the font spacing for wrapped text. Expects a multiplier for the line
             * height. Defaults to 1.15.
             *
             * @param amt Amount of spacing for each line of wrapped text
             */
            void setFontSpacing(const double amt);

            /**
             * @brief Return the dimensions of the given string if rendered using
             * the current font and given font size.
             *
             * @param str String to measure
             * @param size Font size to measure with
             * @return Pair of dimensions, with the first value being
             * the width and second being the height.
             */
            std::pair<int, int> calculateTextDimensions(const std::string & str, const unsigned int size);

            /**
             * @brief Return the dimensions of the given string if rendered using
             * the current font and given font size, as a text block.
             *
             * @param str String to measure
             * @param size Font size to measure with
             * @param width Maximum width of one line
             * @return Tuple of lines of characters, and dimensions, with the first dimension being
             * the width and second being the height.
             */
            std::tuple<std::vector<std::string>, int, int> calculateWrappedTextDimensions(const std::string & str, const unsigned int size, const unsigned int width);

            /**
             * @brief Returns the RGBA pixel data stored in the passed surface.
             * @param surface Surface to read
             * @return Pixels stored in surface.
             */
            std::vector<Colour> readSurfacePixels(SDL_Surface * surface);

            /**
             * @brief Returns the RGBA pixel data stored in the passed surface.
             * @param texture Texture to read
             * @return Pixels stored in surface.
             */
            std::vector<Colour> readTexturePixels(SDL_Texture * texture);

            /**
             * @brief Render the image at the specified path as a surface
             * @note Supports PNG, JPEG and BMP.
             *
             * @param path Path to image file
             * @param scaleWidth Width (in pixels) to scale image to (0 to disable scaling)
             * @param scaleHeight Height (in pixels) to scale image to (0 to disable scaling)
             * @return Drawable containing either the rendered image as a surface or nothing on an error (Type set as None).
             */
            Drawable * renderImageSurface(const std::string & path, const size_t scaleWidth, const size_t scaleHeight);

            /**
             * @brief Render the image passed in the vector as a surface
             * @note Supports PNG, JPEG and BMP.
             *
             * @param data Vector containing image data
             * @param scaleWidth Width (in pixels) to scale image to (0 to disable scaling)
             * @param scaleHeight Height (in pixels) to scale image to (0 to disable scaling)
             * @return Drawable containing either the rendered image as a surface or nothing on an error (Type set as None).
             */
            Drawable * renderImageSurface(const std::vector<unsigned char> & data, const size_t scaleWidth, const size_t scaleHeight);

            /**
             * @brief Render a UTF8 string as a surface
             *
             * @param str String to render
             * @param size Font size to render text at
             * @return Drawable containing either the rendered text as a surface or nothing on an error (Type set as None).
             */
            Drawable * renderTextSurface(const std::string str, const unsigned int size);

            /**
             * @brief Render a UTF8 string with automatic text wrapping as a surface
             *
             * @param str String to render
             * @param size Font size to render text at
             * @param width Maximum width of one line
             * @return Drawable containing either the rendered text as a surface or nothing on an error (Type set as None).
             */
            Drawable * renderWrappedTextSurface(const std::string str, const unsigned int size, const unsigned int width);

            /**
             * @brief Render an ellipse as a texture
             *
             * @param rx Radius along x dimension
             * @param ry Radius along y dimension
             * @param thick Thickness of outline
             * @return Drawable containing either the rendered ellipse or nothing on an error (Type set as None).
             */
            Drawable * renderEllipseTexture(const unsigned int rx, const unsigned int ry, const unsigned int thick);

            /**
             * @brief Render a filled ellipse as a texture
             *
             * @param rx Radius along x dimension
             * @param ry Radius along y dimension
             * @return Drawable containing either the rendered ellipse or nothing on an error (Type set as None).
             */
            Drawable * renderFilledEllipseTexture(const unsigned int rx, const unsigned int ry);

            /**
             * @brief Render a rectangle (outline) as a texture
             *
             * @param width Width of rectangle
             * @param height Height of rectangle
             * @param thick Thickness of outline
             * @return Drawable containing either the rendered rectangle or nothing on an error (Type set as None).
             */
            Drawable * renderRectTexture(const int width, const int height, const unsigned int thick);

            /**
             * @brief Render a filled rectangle as a texture
             *
             * @param width Width of rectangle
             * @param height Height of rectangle
             * @return Drawable containing either the rendered rectangle or nothing on an error (Type set as None).
             */
            Drawable * renderFilledRectTexture(const int width, const int height);

            /**
             * @brief Render an rounded rectangle (outline) as a texture
             *
             * @param width Width of rectangle
             * @param height Height of rectangle
             * @param radius Corner radius
             * @param thick Thickness of outline
             * @return Drawable containing either the rendered ellipse or nothing on an error (Type set as None).
             */
            Drawable * renderRoundRectTexture(const int width, const int height, const unsigned int radius, const unsigned int thick);

            /**
             * @brief Render a filled rounded rectangle as a texture
             *
             * @param width Width of rectangle
             * @param height Height of rectangle
             * @param radius Corner radius
             * @return Drawable containing either the rendered ellipse or nothing on an error (Type set as None).
             */
            Drawable * renderFilledRoundRectTexture(const int width, const int height, const unsigned int radius);

            /**
             * @brief Deletes the renderer object, but does not clean up the backend (SDL).
             * See \ref cleanup().
             */
            ~Renderer();
    };
};

#endif
