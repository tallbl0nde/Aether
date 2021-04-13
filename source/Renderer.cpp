#include "Aether/Renderer.hpp"
#include "Aether/types/Drawable.hpp"
#include "Aether/types/ImageData.hpp"
#include "Aether/utils/FontCache.hpp"
#include "Aether/utils/Image.hpp"
#include "Aether/utils/SDL2_gfx_ext.hpp"
#include "Aether/utils/Utils.hpp"
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_image.h>
#ifdef __SWITCH__
#include <switch.h>
#endif

namespace Aether {
    Renderer::Renderer() {
        this->renderer = nullptr;
        this->window = nullptr;
        this->windowWidth_ = 0;
        this->windowHeight_ = 0;

        this->logHandler = nullptr;
        this->memoryUsage_ = 0;
        this->surfaceCount_ = 0;
        this->textureCount_ = 0;

        this->fontCache = nullptr;
        this->fontSpacing = 1.1;
    }

    void Renderer::logMessage(const std::string & msg, const bool imp) {
        // Do nothing if no handler set
        if (this->logHandler == nullptr) {
            return;
        }

        this->logHandler(msg, imp);
    }

    SDL_Texture * Renderer::convertSurfaceToTexture(SDL_Surface * surf) {
        // Sanity checks
        if (this->renderer == nullptr || surf == nullptr) {
            this->logMessage(std::string("Couldn't convert surface to texture: ") + std::string(surf == nullptr ? "Null surface passed" : "Renderer isn't initialized"), true);
            return nullptr;
        }

        // Actually convert
        SDL_Texture * tex = SDL_CreateTextureFromSurface(this->renderer, surf);
        if (tex == nullptr) {
            this->logMessage(std::string("Couldn't convert surface to texture: ") + std::string(SDL_GetError()), true);
        }

        // Update monitoring variables
        if (tex != nullptr) {
            this->textureCount_++;
        } else {
            this->memoryUsage_ -= (surf->pitch * surf->h);
        }
        this->surfaceCount_--;

        // Free passed surface regardless of outcome
        this->destroySurface(surf, false);
        return tex;
    }

    SDL_Texture * Renderer::createTexture(const unsigned int width, const unsigned int height) {
        // Sanity checks
        if (this->renderer == nullptr || width == 0 || height == 0) {
            this->logMessage(std::string("Couldn't create texture: ") + std::string(this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid dimensions requested"), true);
            return nullptr;
        }

        // Create the texture
        SDL_Texture * tex = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
        if (tex == nullptr) {
            this->logMessage(std::string("Couldn't create texture: ") + std::string(SDL_GetError()), true);
        }

        // Increment monitoring variables
        if (tex != nullptr) {
            this->textureCount_++;
            this->memoryUsage_ += (width * height * 4);     // 4 bytes per pixel
        }

        return tex;
    }

    void Renderer::destroyTexture(SDL_Texture * tex, const bool stats) {
        // Sanity check
        if (tex == nullptr) {
            this->logMessage("Couldn't destroy texture: Null texture passed", false);
            return;
        }

        // Destroy the texture, getting it's info first
        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        SDL_DestroyTexture(tex);

        // Decrease monitoring variables
        if (stats) {
            this->textureCount_--;
            this->memoryUsage_ -= (w * h * 4);      // 4 bytes per pixel
        }
    }

    void Renderer::destroySurface(SDL_Surface * surf, const bool stats) {
        // Sanity check
        if (surf == nullptr) {
            this->logMessage("Couldn't destroy surface: Null surface passed", false);
            return;
        }

        // Destroy the surface
        int mem = surf->pitch * surf->h;
        SDL_FreeSurface(surf);

        // Update monitoring variables
        if (stats) {
            this->surfaceCount_--;
            this->memoryUsage_ -= mem;
        }
    }

    void Renderer::drawFilledRect(const Colour & col, const int x, const int y, const int width, const int height) {
        // Sanity check
        if (this->renderer == nullptr) {
            this->logMessage("Couldn't draw filled rectangle: Renderer isn't initialized", true);
            return;
        }

        // Form SDL_Rect and draw rectangle
        SDL_Rect rect = {x, y, width, height};
        SDL_SetRenderDrawColor(this->renderer, col.r(), col.g(), col.b(), col.a());
        SDL_RenderFillRect(this->renderer, &rect);
    }

    void Renderer::drawTexture(SDL_Texture * tex, const Colour & col, const int x, const int y, const unsigned int width, const unsigned int height, const int maskX, const int maskY, const unsigned int maskW, const unsigned int maskH) {
        // Sanity check (no logging as this will be called often)
        if (this->renderer == nullptr || tex == nullptr) {
            return;
        }

        // Set texture's colour
        SDL_SetTextureColorMod(tex, col.r(), col.g(), col.b());
        SDL_SetTextureAlphaMod(tex, col.a());
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

        // Copy to framebuffer
        SDL_Rect src = {maskX, maskY, maskW, maskH};
        SDL_Rect dest = {x, y, width, height};
        SDL_RenderCopy(this->renderer, tex, &src, &dest);
    }

    void Renderer::renderOnTexture(SDL_Texture * tex, const std::function<void(SDL_Renderer *)> & func) {
        // Sanity checks
        std::string msg = "";
        if (this->renderer == nullptr) {
            msg = "Renderer isn't initialized";
        } else if (tex == nullptr) {
            msg = "Null texture passed";
        } else if (func == nullptr) {
            msg = "Null function passed";
        }

        if (!msg.empty()) {
            this->logMessage(std::string("Couldn't render on texture: ") + msg, true);
            return;
        }

        // Change render target to texture and invoke function
        SDL_SetRenderTarget(this->renderer, tex);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 0);
        SDL_RenderClear(this->renderer);
        func(this->renderer);

        // Reset back to window
        SDL_SetRenderTarget(this->renderer, nullptr);
    }

    SDL_Surface * Renderer::scaleSurface(SDL_Surface * surface, const size_t width, const size_t height) {
        // Convert surface to 'ImageData'
        uint8_t * pixelPtr = static_cast<uint8_t *>(surface->pixels);
        std::vector<uint8_t> data;
        data.assign(pixelPtr, pixelPtr + (surface->pitch * surface->h));
        ImageData * image = new ImageData(data, surface->w, surface->h, surface->pitch/surface->w);
        data.clear();

        // Resize and replace surface
        ImageData * scaled = Utils::Image::scaleOptimal(image, width, height);
        if (scaled == nullptr) {
            this->logMessage(std::string("Couldn't scale image to: ") + std::to_string(width) + std::string("x") + std::to_string(height), true);

        } else {
            SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, scaled->width(), scaled->height(), 32, SDL_PIXELFORMAT_RGBA32);
            if (surf == nullptr) {
                this->logMessage(std::string("Couldn't create scaled surface: ") + std::string(SDL_GetError()), true);

            } else {
                this->destroySurface(surface, false);
                surface = surf;
                data = scaled->toByteVector();
                std::memcpy(surface->pixels, &data[0], data.size());
            }
        }

        delete image;
        delete scaled;

        return surface;
    }

    void Renderer::setLogHandler(const LogHandler & func) {
        this->logHandler = func;
    }

    bool Renderer::initialize(const std::string & name, const unsigned int windowW, const unsigned int windowH) {
        // Sanity check
        if (this->window != nullptr || this->renderer != nullptr || this->fontCache != nullptr) {
            this->logMessage("Already initialized!", true);
            return true;
        }

        // Initialize libraries
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
            this->logMessage(std::string("Couldn't init SDL: ") + std::string(SDL_GetError()), true);
            return false;
        }

        int flags = IMG_INIT_JPG | IMG_INIT_PNG;
        if ((IMG_Init(flags) & flags) != flags) {
            this->logMessage("Couldn't initialize SDL_image", true);
            return false;
        }

        // Create the window
        #ifdef __SWITCH__
        this->window = SDL_CreateWindow(name.empty() ? "Aether" : name.c_str(), 0, 0, 1280, 720, 0);
        this->windowWidth_ = 1280;
        this->windowHeight_ = 720;
        #else
        this->window = SDL_CreateWindow(name.empty() ? "Aether" : name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, 0);
        this->windowWidth_ = windowW;
        this->windowHeight_ = windowH;
        #endif

        if (this->window == nullptr) {
            this->logMessage(std::string("Couldn't create window: ") + std::string(SDL_GetError()), true);
            return false;
        }

        // Create the renderer
        this->renderer = SDL_CreateRenderer(this->window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (this->renderer == nullptr) {
            this->logMessage(std::string("Couldn't create renderer: ") + std::string(SDL_GetError()), true);
            return false;
        }

        // Prepare controller support
        #ifdef __SWITCH__
        if (SDL_JoystickOpen(0) == nullptr) {
            this->logMessage(std::string("Couldn't open joystick device: ") + std::string(SDL_GetError()), true);
            return false;
        }
        #endif

        // Set up blending
        SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

        // Finally set up platform specific things
        #ifdef __SWITCH__
        Result rc = plInitialize(PlServiceType_User);
        if (!R_SUCCEEDED(rc)) {
            this->logMessage(std::string("Couldn't initialize pl: ") + std::to_string(rc), true);
            return false;
        }
        #endif

        this->fontCache = new FontCache(this);
        this->logMessage("Initialized successfully!", false);
        return true;
    }

    void Renderer::cleanup() {
        delete this->fontCache;
        this->fontCache = nullptr;
        #ifdef __SWITCH__
        plExit();
        #endif

        while (!this->clipStack.empty()) {
            delete this->clipStack.top();
            this->clipStack.pop();
        }

        if (this->renderer != nullptr) {
            SDL_DestroyRenderer(this->renderer);
            this->renderer = nullptr;
        }

        if (this->window != nullptr) {
            SDL_DestroyWindow(this->window);
            this->window = nullptr;
        }

        IMG_Quit();
        SDL_Quit();
        this->logMessage("Cleaned up", false);
    }

    unsigned int Renderer::memoryUsage() {
        return this->memoryUsage_;
    }

    unsigned int Renderer::surfaceCount() {
        return this->surfaceCount_;
    }

    unsigned int Renderer::textureCount() {
        return this->textureCount_;
    }

    unsigned int Renderer::windowWidth() {
        return this->windowWidth_;
    }

    unsigned int Renderer::windowHeight() {
        return this->windowHeight_;
    }

    void Renderer::fillWindow(const Colour & col) {
        // Sanity check (not logged as this will be called often)
        if (this->renderer == nullptr) {
            return;
        }

        if (col.a() == 255) {
            SDL_SetRenderDrawColor(this->renderer, col.r(), col.g(), col.b(), col.a());
            SDL_RenderClear(this->renderer);
        } else {
            this->drawFilledRect(col, 0, 0, this->windowWidth_, this->windowHeight_);
        }
    }

    void Renderer::present() {
        // Sleep if not initialized to prevent spinlock
        if (this->renderer == nullptr) {
            this->logMessage("Fatal error: SDL not initialized", true);
            SDL_Delay(500);
        }

        SDL_RenderPresent(this->renderer);
    }

    void Renderer::resetClipArea() {
        // If we've only got one clip rectangle reset clip
        if (this->clipStack.size() <= 1) {
            SDL_RenderSetClipRect(this->renderer, nullptr);
            if (!this->clipStack.empty()) {
                delete this->clipStack.top();
                this->clipStack.pop();
            }

        // Otherwise set to last rectangle
        } else {
            delete this->clipStack.top();
            this->clipStack.pop();
            SDL_Rect * r = this->clipStack.top();
            SDL_RenderSetClipRect(this->renderer, r);
            delete r;
        }
    }

    void Renderer::setClipArea(const int x1, const int y1, const int x2, const int y2) {
        // Do nothing if the points are not in the correct order
        if (x1 == x2 || y1 == y2 || x2 < x1 || y2 < y1) {
            SDL_Rect * r = new SDL_Rect{0, 0, this->windowWidth_, this->windowHeight_};
            this->clipStack.push(r);
            return;
        }

        // Limit to within current clipping rectangle if we already have one active
        SDL_Rect r;
        if (!this->clipStack.empty()) {
            SDL_RenderGetClipRect(this->renderer, &r);
            int rx2 = r.x + r.w;
            int ry2 = r.y + r.h;
            r.x = (x1 < r.x ? r.x : x1);
            r.y = (y1 < r.y ? r.y : y1);
            rx2 = (x2 > rx2 ? rx2 : x2);
            ry2 = (y2 > ry2 ? ry2 : y2);
            r.w = rx2-r.x;
            r.h = ry2-r.y;

        // Otherwise use raw coordinates
        } else {
            r = SDL_Rect{x1, y1, x2-x1, y2-y1};
        }

        // Make a copy on the heap
        SDL_Rect * rr = new SDL_Rect{r.x, r.y, r.w, r.h};

        // Set clip
        int rc = SDL_RenderSetClipRect(this->renderer, rr);
        if (rc == 0) {
            this->clipStack.push(rr);
        }
    }

    void Renderer::setFont(const std::string & path) {
        if (this->fontCache == nullptr) {
            this->logMessage("Can't set font as renderer isn't initialized", true);
            return;
        }

        this->fontCache->setCustomFont(path);
    }

    void Renderer::setFontSpacing(const double amt) {
        this->fontSpacing = amt;
    }

    std::pair<int, int> Renderer::calculateTextDimensions(const std::string & str, const unsigned int size) {
        int width = 0;
        int height = 0;

        unsigned int pos = 0;
        while (pos < str.length()) {
            unsigned int posCopy = pos;
            uint16_t ch = Utils::getUTF8Char(str, pos);

            // Break if pos isn't changed (meaning no character could be extracted)
            if (pos == posCopy) {
                break;
            }

            // Get the metrics for the required character
            GlyphMetrics metrics;
            {
                std::scoped_lock<std::mutex> mtx(this->ttfMtx);
                metrics = this->fontCache->getMetrics(ch, size);
                if (metrics.character() == 0) {
                    this->logMessage("Couldn't get metrics for glyph, is a font set?", true);
                    return std::pair<int, int>(0, 0);
                }
            }

            // Update current measurements
            if (metrics.height() > height) {
                height = metrics.height();
            }
            width += metrics.width();
        }

        return std::pair<int, int>(width, height);
    }

    std::tuple<std::vector<std::string>, int, int> Renderer::calculateWrappedTextDimensions(const std::string & str, const unsigned int size, const unsigned int width) {
        std::vector<std::string> lines;         // Vector of lines, where each line fits within the width
        unsigned int maxLineWidth = 0;          // Maximum width of one line
        unsigned int maxLineHeight = 0;         // Maximum height of one line (all lines will use this height)
        std::string word;                       // Characters forming current word
        unsigned int wordWidth = 0;             // Width of current word

        // Iterate over all characters in string
        unsigned int pos = 0;
        while (pos < str.length()) {
            int charSize = 0;                   // Number of chars forming unicode character
            unsigned int lastCharWidth = 0;     // Width of last character
            std::string line;                   // Characters forming a line
            unsigned int lineHeight = 0;        // Height of current line
            unsigned int lineWidth = 0;         // Width of line so far

            // Keep appending characters (and words) to line until we hit the edge
            while (lineWidth + wordWidth < width) {
                // Stop if at the end
                if (pos >= str.length()) {
                    line += word;
                    lineWidth += wordWidth;
                    word = "";
                    wordWidth = 0;
                    break;
                }

                // Break if end of line
                bool b = false;
                if (str[pos] == '\r') {
                    b = true;
                    pos++;
                }
                if (str[pos] == '\n') {
                    b = true;
                    pos++;
                }
                if (b) {
                    line += word;
                    lineWidth += wordWidth;
                    word = "";
                    wordWidth = 0;
                    break;
                }

                // Get next char
                unsigned int oldPos = pos;
                uint16_t ch = Utils::getUTF8Char(str, pos);
                charSize = pos - oldPos;
                if (charSize == 0) {
                    // Character couldn't be found
                    break;
                }

                // Get the metrics for the required character
                GlyphMetrics metrics;
                {
                    std::scoped_lock<std::mutex> mtx(this->ttfMtx);
                    metrics = this->fontCache->getMetrics(ch, size);
                }

                // Hard abort if we couldn't get the metrics
                if (metrics.character() == 0) {
                    this->logMessage("Couldn't get metrics for character", true);
                    return std::make_tuple(std::vector<std::string>(), 0, 0);
                }

                // Update variables regarding positioning
                lastCharWidth = metrics.width();
                wordWidth += metrics.width();
                lineHeight = (metrics.height() > lineHeight ? metrics.height() : lineHeight);
                word += str.substr(oldPos, charSize);

                // Add word to current line if space
                if (static_cast<char>(ch) == ' ') {
                    line += word;
                    lineWidth += wordWidth;
                    word = "";
                    wordWidth = 0;
                }
            }

            // Add word so far if line is empty
            if (line.length() == 0 && word.length() > 0) {
                line += word.substr(0, word.length() - charSize);
                lineWidth += wordWidth;
                lineWidth -= lastCharWidth;
                word = word.substr(word.length() - charSize, charSize);
                wordWidth = lastCharWidth;
            }

            // Update height and add line
            maxLineHeight = (lineHeight > maxLineHeight ? lineHeight : maxLineHeight);
            maxLineWidth = (lineWidth > maxLineWidth ? lineWidth: maxLineWidth);
            lines.push_back(line);
            line = "";
        }

        // Form tuple and return
        return std::make_tuple(lines, maxLineWidth, lines.size() * (maxLineHeight * this->fontSpacing));
    }

    Drawable * Renderer::renderImageSurface(const std::string & path, const size_t scaleWidth, const size_t scaleHeight) {
        SDL_Surface * surf = nullptr;
        {
            // Sanity check
            std::scoped_lock<std::mutex> mtx(this->imgMtx);
            if (this->renderer == nullptr || !Utils::fileExists(path)) {
                this->logMessage(std::string("Couldn't render image to surface from file: ") + std::string(this->renderer == nullptr ? "Renderer isn't initialized" : "File doesn't exist"), true);
                return new Drawable();
            }

            // Read in image
            surf = IMG_Load(path.c_str());
            if (surf == nullptr) {
                this->logMessage(std::string("Couldn't render image to surface: ") + std::string(IMG_GetError()), true);
                return new Drawable();
            }
        }

        // Convert pixel format
        SDL_Surface * newSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
        this->destroySurface(surf, false);
        if (newSurf == nullptr) {
            this->logMessage(std::string("Couldn't convert image surface: ") + std::string(SDL_GetError()), true);
            return new Drawable();
        }

        // Scale if we need
        if (scaleWidth != 0 || scaleHeight != 0) {
            newSurf = this->scaleSurface(newSurf, scaleWidth, scaleHeight);
        }

        // Increment monitoring variables
        this->surfaceCount_++;
        this->memoryUsage_ += (newSurf->pitch * newSurf->h);

        return new Drawable(this, newSurf, newSurf->w, newSurf->h);
    }

    Drawable * Renderer::renderImageSurface(const std::vector<unsigned char> & data, const size_t scaleWidth, const size_t scaleHeight) {
        // Sanity check
        if (this->renderer == nullptr) {
            this->logMessage("Couldn't render image to surface from memory: Renderer isn't initialized", true);
            return new Drawable();
        }

        // Read in image
        SDL_Surface * surf = nullptr;
        {
            std::scoped_lock<std::mutex> mtx(this->imgMtx);
            surf = IMG_Load_RW(SDL_RWFromMem(const_cast<unsigned char *>(&data[0]), data.size()), 1);
            if (surf == nullptr) {
                this->logMessage(std::string("Couldn't render image to surface: ") + std::string(IMG_GetError()), true);
                return new Drawable();
            }
        }

        // Convert pixel format
        SDL_Surface * newSurf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
        this->destroySurface(surf, false);
        if (newSurf == nullptr) {
            this->logMessage(std::string("Couldn't convert image surface: ") + std::string(SDL_GetError()), true);
            return new Drawable();
        }

        // Scale if we need
        if (scaleWidth != 0 || scaleHeight != 0) {
            newSurf = this->scaleSurface(newSurf, scaleWidth, scaleHeight);
        }

        // Increment monitoring variables
        this->surfaceCount_++;
        this->memoryUsage_ += (newSurf->pitch * newSurf->h);

        return new Drawable(this, newSurf, newSurf->w, newSurf->h);
    }

    Drawable * Renderer::renderTextSurface(const std::string str, const unsigned int size) {
        // Sanity check
        if (this->renderer == nullptr || size == 0) {
            this->logMessage(std::string("Couldn't render text to surface: ") + std::string(size == 0 ? "Invalid size" : "Renderer isn't initialized"), true);
            return new Drawable();
        }

        // Get the overall required surface size and create
        std::pair<int, int> dim = this->calculateTextDimensions(str, size);
        if (dim.first == 0 || dim.second == 0) {
            this->logMessage("Couldn't render text to surface: Invalid metrics returned", true);
            return new Drawable();
        }

        // Iterate over each character in string and blit glyph
        SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, dim.first, dim.second, 32, SDL_PIXELFORMAT_RGBA32);
        SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 255, 255, 255, 0));
        unsigned int pos = 0;
        unsigned int x = 0;

        while (pos < str.length()) {
            unsigned int posCopy = pos;
            uint16_t ch = Utils::getUTF8Char(str, pos);

            // Break if pos isn't changed (meaning no character could be extracted)
            if (pos == posCopy) {
                break;
            }

            // Get a surface for the glyph and blit
            {
                std::scoped_lock<std::mutex> mtx(this->ttfMtx);
                SDL_Surface * tmp = this->fontCache->getGlyph(ch, size);
                if (tmp == nullptr) {
                    // Hard abort if no character returned
                    this->logMessage("Couldn't get surface for glyph, is a font set?", true);
                    this->destroySurface(surf, false);
                    return new Drawable();
                }

                SDL_Rect r = SDL_Rect{x, 0, tmp->w, tmp->h};
                SDL_BlitSurface(tmp, NULL, surf, &r);
                x += tmp->w;
            }
        }

        // Increment monitoring variables
        this->surfaceCount_++;
        this->memoryUsage_ += (surf->pitch * surf->h);

        return new Drawable(this, surf, surf->w, surf->h);
    }

    Drawable * Renderer::renderWrappedTextSurface(const std::string str, const unsigned int size, const unsigned int width) {
        // Sanity check
        if (this->renderer == nullptr || size == 0 || width == 0) {
            this->logMessage(std::string("Couldn't render wrapped text: ") + std::string(this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid values"), true);
            return new Drawable();
        }

        // Calculate required lines + dimensions
        std::tuple<std::vector<std::string>, int, int> dims = this->calculateWrappedTextDimensions(str, size, width);
        if (std::get<0>(dims).empty()) {
            this->logMessage("Couldn't render wrapped text: invalid metrics", true);
            return new Drawable();
        }

        // Create the surface
        SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, std::get<1>(dims), std::get<2>(dims), 32, SDL_PIXELFORMAT_RGBA32);
        if (surf == nullptr) {
            this->logMessage(std::string("Couldn't create surface for entire wrapped string: ") + std::string(SDL_GetError()), true);
            return new Drawable();
        }

        // Render each glyph onto the surface
        std::vector<std::string> & lines = std::get<0>(dims);
        SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 255, 255, 255, 0));
        for (size_t i = 0; i < lines.size(); i++) {
            int x = 0;
            unsigned int pos = 0;
            while (pos < lines[i].length()) {
                unsigned int oldPos = pos;
                uint16_t ch = Utils::getUTF8Char(lines[i], pos);
                if (pos == oldPos) {
                    // Character couldn't be found
                    break;
                }

                // Get glyph for character and blit onto surface
                std::scoped_lock<std::mutex> mtx(this->ttfMtx);
                SDL_Surface * glyph = this->fontCache->getGlyph(ch, size);
                if (glyph == nullptr) {
                    this->logMessage("Couldn't get surface for glyph, is a font set?", true);
                    this->destroySurface(surf, false);
                    return new Drawable();
                }

                SDL_Rect r = SDL_Rect{x, i * (std::get<2>(dims)/lines.size()), glyph->w, glyph->h};
                x += glyph->w;
                SDL_BlitSurface(glyph, NULL, surf, &r);
            }
        }

        // Increment monitoring variables
        this->surfaceCount_++;
        this->memoryUsage_ += (surf->pitch * surf->h);

        return new Drawable(this, surf, surf->w, surf->h);
    }

    Drawable * Renderer::renderEllipseTexture(const unsigned int rx, const unsigned int ry, const unsigned int thick) {
        // Sanity check
        if (this->renderer == nullptr || rx == 0 || ry == 0 || thick == 0) {
            this->logMessage(std::string("Couldn't render ellipse texture: ") + (this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid values"), true);
            return new Drawable();
        }

        // Create texture
        unsigned int width = 2 * (rx + thick);
        unsigned int height = 2 * (ry + thick);
        SDL_Texture * tex = createTexture(width, height);
        if (tex == nullptr) {
            this->logMessage("Couldn't render ellipse texture: Null texture returned", true);
            return new Drawable();
        }

        // Render ellipse
        this->renderOnTexture(tex, [rx, ry, thick](SDL_Renderer * renderer) {
            // Drawing in quadrants is faster than the full 360 degrees
            for (unsigned short i = 0; i < 4; i++) {
                aaArcRGBA(renderer, rx + thick, ry + thick, rx + thick/2, ry + thick/2, i*90, (i+1)*90 + 1, thick, 255, 255, 255, 255);
            }
        });

        return new Drawable(this, tex, width, height);
    }

    Drawable * Renderer::renderFilledEllipseTexture(const unsigned int rx, const unsigned int ry) {
        // Sanity check
        if (this->renderer == nullptr || rx == 0 || ry == 0) {
            this->logMessage(std::string("Couldn't render filled ellipse texture: ") + (this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid values"), true);
            return new Drawable();
        }

        // Create texture
        unsigned int width = 2 * rx;
        unsigned int height = 2 * ry;
        SDL_Texture * tex = createTexture(width, height);
        if (tex == nullptr) {
            this->logMessage("Couldn't render filled ellipse texture: Null texture returned", true);
            return new Drawable();
        }

        // Render filled ellipse
        this->renderOnTexture(tex, [rx, ry](SDL_Renderer * renderer) {
            aaFilledEllipseRGBA(renderer, rx, ry, rx, ry, 255, 255, 255, 255);
        });

        return new Drawable(this, tex, width, height);
    }

    Drawable * Renderer::renderRectTexture(const int width, const int height, const unsigned int thick) {
        // Sanity check
        if (this->renderer == nullptr || width == 0 || height == 0 || thick == 0) {
            this->logMessage(std::string("Couldn't render rectangle texture: ") + (this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid values"), true);
            return new Drawable();
        }

        // Create texture
        SDL_Texture * tex = createTexture(width, height);
        if (tex == nullptr) {
            this->logMessage("Couldn't render rectangle texture: Null texture returned", true);
            return new Drawable();
        }

        // Render rectangle
        this->renderOnTexture(tex, [width, height, thick](SDL_Renderer * renderer) {
            for (unsigned int i = 0; i < thick; i++) {
                rectangleRGBA(renderer, width - i, i, i, height - i, 255, 255, 255, 255);
            }
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        });

        return new Drawable(this, tex, width, height);
    }

    Drawable * Renderer::renderFilledRectTexture(const int width, const int height) {
        // Sanity check
        if (this->renderer == nullptr || width == 0 || height == 0) {
            this->logMessage(std::string("Couldn't render filled rectangle texture: ") + (this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid values"), true);
            return new Drawable();
        }

        // Create texture
        SDL_Texture * tex = createTexture(width, height);
        if (tex == nullptr) {
            this->logMessage("Couldn't render filled rectangle texture: Null texture returned", true);
            return new Drawable();
        }

        // Render rectangle
        this->renderOnTexture(tex, [width, height](SDL_Renderer * renderer) {
            SDL_Rect r = {0, 0, width, height};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &r);
        });

        return new Drawable(this, tex, width, height);
    }

    Drawable * Renderer::renderRoundRectTexture(const int width, const int height, const unsigned int radius, const unsigned int thick) {
        // Sanity check
        if (this->renderer == nullptr || width == 0 || height == 0 || radius == 0 || thick == 0) {
            this->logMessage(std::string("Couldn't render round rectangle texture: ") + (this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid values"), true);
            return new Drawable();
        }

        // Create texture
        SDL_Texture * tex = createTexture(width, height);
        if (tex == nullptr) {
            this->logMessage("Couldn't render round rectangle texture: Null texture returned", true);
            return new Drawable();
        }

        // Render round rectangle
        this->renderOnTexture(tex, [width, height, radius, thick](SDL_Renderer * renderer) {
            // Starting with top left corner, moving clockwise
            aaArcRGBA(renderer, radius        , radius         , radius - thick/2.0, radius - thick/2.0, 180, 270, thick, 255, 255, 255, 255);
            aaArcRGBA(renderer, width - radius, radius         , radius - thick/2.0, radius - thick/2.0, 270, 360, thick, 255, 255, 255, 255);
            aaArcRGBA(renderer, width - radius, height - radius, radius - thick/2.0, radius - thick/2.0,   0,  90, thick, 255, 255, 255, 255);
            aaArcRGBA(renderer, radius        , height - radius, radius - thick/2.0, radius - thick/2.0,  90, 180, thick, 255, 255, 255, 255);

            // Join with lines (starting at top, clockwise)
            thickLineRGBA(renderer, radius           , thick/2.0         , width - radius   , thick/2.0         , thick, 255, 255, 255, 255);
            thickLineRGBA(renderer, width - thick/2.0, radius            , width - thick/2.0, height - radius   , thick, 255, 255, 255, 255);
            thickLineRGBA(renderer, radius           , height - thick/2.0, width - radius   , height - thick/2.0, thick, 255, 255, 255, 255);
            thickLineRGBA(renderer, thick/2.0        , radius            , thick/2.0        , height - radius   , thick, 255, 255, 255, 255);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        });

        return new Drawable(this, tex, width, height);
    }

    Drawable * Renderer::renderFilledRoundRectTexture(const int width, const int height, const unsigned int radius) {
        // Sanity check
        if (this->renderer == nullptr || width == 0 || height == 0 || radius == 0) {
            this->logMessage(std::string("Couldn't render filled round rectangle texture: ") + (this->renderer == nullptr ? "Renderer isn't initialized" : "Invalid values"), true);
            return new Drawable();
        }

        // Create texture
        SDL_Texture * tex = createTexture(width, height);
        if (tex == nullptr) {
            this->logMessage("Couldn't render filled round rectangle texture: Null texture returned", true);
            return new Drawable();
        }

        // Render round rectangle
        this->renderOnTexture(tex, [width, height, radius](SDL_Renderer * renderer) {
            // Starting with top left corner, moving clockwise
            aaFilledPieRGBA(renderer, radius        , radius         , radius, radius, 180, 270, 0, 255, 255, 255, 255);
            aaFilledPieRGBA(renderer, width - radius, radius         , radius, radius, 270, 360, 0, 255, 255, 255, 255);
            aaFilledPieRGBA(renderer, width - radius, height - radius, radius, radius,   0,  90, 0, 255, 255, 255, 255);
            aaFilledPieRGBA(renderer, radius        , height - radius, radius, radius,  90, 180, 0, 255, 255, 255, 255);

            // Fill in with rectangles (left to right)
            SDL_Rect r;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            r = {0, radius, radius, height - 2*radius};
            SDL_RenderFillRect(renderer, &r);

            r = {radius, 0, width - 2*radius, height};
            SDL_RenderFillRect(renderer, &r);

            r = {width - radius, radius, radius, height - 2*radius};
            SDL_RenderFillRect(renderer, &r);
        });

        return new Drawable(this, tex, width, height);
    }

    Renderer::~Renderer() {
        // Check the renderer was cleaned up
        if (this->renderer != nullptr) {
            this->logMessage("Aether wasn't correctly cleaned up", true);
        }

        // Check for memory leaks
        if (this->memoryUsage_ != 0) {
            this->logMessage(std::string("Aether seems to have leaked approximately ") + std::to_string(this->memoryUsage_/1024) + std::string(" KB of memory"), true);
            this->logMessage(std::string("Leaked surfaces: ") + std::to_string(this->surfaceCount_), true);
            this->logMessage(std::string("Leaked textures: ") + std::to_string(this->textureCount_), true);
        }
    }
};