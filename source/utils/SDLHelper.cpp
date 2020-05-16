#include <atomic>
#include <mutex>
#include "SDLHelper.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <vector>

// === SDL RENDERING ===
// SDL Renderer instance
static SDL_Renderer * renderer;
// SDL Window instance
static SDL_Window * window;

// === FONT RENDERING ===
// Height of a line for wrapped text (multiple of line height)
#define FONT_SPACING 1.15
// Stores data about Nintendo fonts
static PlFontData fontData[PlSharedFontType_Total];
// Caches font sizes (note index 0 is used when a custom font is set)
static std::unordered_map<int, TTF_Font *> fontCache[PlSharedFontType_Total];
// Set true if a custom font is used
static bool customFont;
static std::string customFontPath;

// === MISCELLANEOUS ===
// Offset position
static int offsetX;
static int offsetY;
// Set to current blend mode
static SDL_BlendMode tex_blend_mode;
// Mutex for concurrent access to fontCache
static std::mutex fontCacheMutex;

// === STATUS ===
// Counters
static std::atomic<int> memUsage;
static std::atomic<int> surfNum;
static std::atomic<int> texNum;

// Helper function which returns one UTF8 character given a string (from U+0000 to U+FFFF)
// The second argument takes the character/byte to start at and is updated to the next character/byte to look at
static uint16_t getUTF8Char(std::string str, unsigned int & pos) {
    uint16_t c = 0;

    // One byte
    if ((str.length() - pos) >= 1) {
        if ((str[0 + pos] & 0b10000000) == 0) {
            c = str[0 + pos];
            pos += 1;
        }
    }

    // Two bytes
    if (c == 0 && (str.length() - pos) >= 2) {
        if ((str[0 + pos] & 0b11100000) == 0b11000000) {
            c = (str[0 + pos] & 0b00011111) << 6;
            c = (c | (str[1 + pos] & 0b00111111));
            pos += 2;
        }
    }

    // Three bytes
    if (c == 0 & (str.length() - pos) >= 3) {
        if ((str[0 + pos] & 0b11110000) == 0b11100000) {
            c = (str[0 + pos] & 0b00001111) << 12;
            c = (c | ((str[1 + pos] & 0b00111111) << 6));
            c = (c | (str[2 + pos] & 0b00111111));
            pos += 3;
        }
    }

    return c;
}

namespace SDLHelper {
    bool initSDL() {
        // Init main SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
            return false;
        }

        // Initialize SDL_ttf
        if (TTF_Init() == -1) {
            return false;
        }

        // Create SDL Window
        window = SDL_CreateWindow("window", 0, 0, 1280, 720, 0);
        if (!window) {
            return false;
        }

        // Create SDL Renderer
        renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            return false;
        }

        // Prepare controller (only railed for now)
        if (SDL_JoystickOpen(0) == NULL) {
            return false;
        }

        // Set up blending
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        tex_blend_mode = SDL_BLENDMODE_BLEND;

        // Ensure offset starts at (0, 0)
        offsetX = 0;
        offsetY = 0;

        // Load fonts
        customFont = false;
        Result rc = plInitialize(PlServiceType_User);
        if (!R_SUCCEEDED(rc)) {
            return false;
        }
        for (int i = 0; i < PlSharedFontType_Total; i++) {
            plGetSharedFontByType(&fontData[i], (PlSharedFontType)i);
        }
        return true;
    }

    void exitSDL() {
        // Delete created fonts
        emptyFontCache();
        plExit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    int memoryUsage() {
        return memUsage;
    }

    int numSurfaces() {
        return surfNum;
    }

    int numTextures() {
        return texNum;
    }

    void clearScreen(SDL_Color c) {
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderClear(renderer);
    }

    SDL_Texture * convertSurfaceToTexture(SDL_Surface * s) {
        SDL_Texture * tex = SDL_CreateTextureFromSurface(renderer, s);

        // Change counters
        if (s != NULL) {
            surfNum--;
        }
        if (tex != NULL) {
            texNum++;
        }

        SDL_FreeSurface(s);
        return tex;
    }

    SDL_Texture * createTexture(int w, int h) {
        SDL_Texture * t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);

        // Increment counters
        if (t != NULL) {
            texNum++;
            memUsage += (w * h * 4);    // 4 bytes per pixel
        }

        return t;
    }

    void destroyTexture(SDL_Texture * t) {
        // Decrease counters
        if (t != NULL) {
            int w, h;
            SDL_QueryTexture(t, nullptr, nullptr, &w, &h);
            texNum--;
            memUsage -= (w * h * 4);    // 4 bytes per pixel
        }

        SDL_DestroyTexture(t);
    }

    void freeSurface(SDL_Surface * s) {
        // Decrease counters
        if (s != NULL) {
            surfNum--;
            memUsage -= (s->pitch * s->h);
        }

        SDL_FreeSurface(s);
    }

    void getDimensions(SDL_Texture * t, int * w, int * h) {
        SDL_QueryTexture(t, nullptr, nullptr, w, h);
    }

    void getOffset(int * x, int * y) {
        *x = offsetX;
        *y = offsetY;
    }

    void setOffset(int x, int y) {
        offsetX = x;
        offsetY = y;
    }

    void emptyFontCache() {
        std::lock_guard<std::mutex> mtx(fontCacheMutex);
        for (int i = 0; i < PlSharedFontType_Total; i++) {
            for (auto it = fontCache[i].begin(); it != fontCache[i].end(); it++) {
                TTF_CloseFont(it->second);
            }
            fontCache[i].clear();
        }
    }

    void setFont(std::string p) {
        if (p == "") {
            customFont = false;
        } else {
            customFontPath = p;
            customFont = true;
        }

        emptyFontCache();
    }

    void renderToScreen() {
        SDL_SetRenderTarget(renderer, nullptr);
    }

    void renderToTexture(SDL_Texture * t) {
        SDL_SetRenderTarget(renderer, t);
        SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);
    }

    SDL_BlendMode getBlendMode() {
        return tex_blend_mode;
    }

    void setBlendMode(SDL_BlendMode b) {
        tex_blend_mode = b;
    }

    void setRendererBlendMode(SDL_BlendMode b) {
        SDL_SetRenderDrawBlendMode(renderer, b);
    }

    // === DRAWING FUNCTIONS ===

    void draw() {
        SDL_RenderPresent(renderer);
    }

    void drawEllipse(SDL_Color c, int x, int y, unsigned int w, unsigned int h) {
        filledEllipseRGBA(renderer, x + offsetX, y + offsetY, w/2, h/2, c.r, c.g, c.b, c.a);
        aaellipseRGBA(renderer, x + offsetX, y + offsetY, w/2, h/2, c.r, c.g, c.b, c.a);
    }

    void drawFilledRect(SDL_Color c, int x, int y, int w, int h) {
        SDL_Rect rr = {x + offsetX, y + offsetY, w, h};
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(renderer, &rr);
    }

    void drawFilledRoundRect(SDL_Color c, int x, int y, int w, int h, unsigned int r) {
        roundedBoxRGBA(renderer, offsetX + (x + w - 2), offsetY + y, offsetX + x, offsetY + (y + h - 2), r, c.r, c.g, c.b, c.a);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    void drawRoundRect(SDL_Color c, int x, int y, int w, int h, unsigned int r, unsigned int b) {
        for (unsigned int i = 0; i < b; i++) {
            roundedRectangleRGBA(renderer, (x + offsetX + w) - i, y + offsetY + i, offsetX + x + i, (offsetY + y + h) - i, r, c.r, c.g, c.b, c.a);
        }
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    void drawRect(SDL_Color c, int x, int y, int w, int h, unsigned int b) {
        for (unsigned int i = 0; i < b; i++) {
            rectangleRGBA(renderer, (x + offsetX + w) - i, y + offsetY + i, x + offsetX + i, (y + offsetY + h) - i, c.r, c.g, c.b, c.a);
        }
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    }

    void drawTexture(SDL_Texture * tex, SDL_Color c, int x, int y, int w, int h, int tx, int ty, int tw, int th) {
        if (tex == nullptr) {
            return;
        }

        // Set color
        SDL_SetTextureColorMod(tex, c.r, c.g, c.b);
        SDL_SetTextureAlphaMod(tex, c.a);
        SDL_SetTextureBlendMode(tex, tex_blend_mode);

        // Get dimensions
        int width, height;
        SDL_QueryTexture(tex, nullptr, nullptr, &width, &height);
        if (w == -1) {
            w = width;
        }
        if (h == -1) {
            h = height;
        }

        // Scale if necessary + render
        SDL_Rect r = {x + offsetX, y + offsetY, w, h};
        if (tx != -1 && tw != -1 && ty != -1 && th != -1) {
            SDL_Rect s = {tx, ty, tw, th};
            SDL_RenderCopy(renderer, tex, &s, &r);
        } else {
            SDL_RenderCopy(renderer, tex, nullptr, &r);
        }

        // Reset color
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
    }

    // === RENDERING FUNCTIONS ===

    // -= SURFACES =-
    SDL_Surface * renderFilledRectS(int w, int h) {
        SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
        SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 255, 255, 255, 255));

        // Increment counters
        if (surf != NULL) {
            surfNum++;
            memUsage += (surf->pitch * surf->h);
        }

        return surf;
    }

    SDL_Surface * renderRectS(int w, int h, unsigned int b) {
        SDL_Surface * surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
        SDL_Rect rects[4];
        rects[0] = SDL_Rect{0, 0, w, b};
        rects[1] = SDL_Rect{0, b, b, h - (b*2)};
        rects[2] = SDL_Rect{w - b, b, b, h - (b*2)};
        rects[3] = SDL_Rect{0, h - b, w, b};
        SDL_FillRects(surf, rects, 4, SDL_MapRGBA(surf->format, 255, 255, 255, 255));

        // Increment counters
        if (surf != NULL) {
            surfNum++;
            memUsage += (surf->pitch * surf->h);
        }

        return surf;
    }

    SDL_Surface * renderImageS(std::string path, int xF, int yF) {
        SDL_Surface * tmp = IMG_Load(path.c_str());
        SDL_Surface * tmp2 = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(tmp);
        if (xF != 1 || yF != 1) {
            SDL_Surface * tmp = shrinkSurface(tmp2, xF, yF);
            SDL_FreeSurface(tmp2);
            tmp2 = tmp;
        }

        // Increment counters
        if (tmp2 != NULL) {
            surfNum++;
            memUsage += (tmp2->pitch * tmp2->h);
        }

        return tmp2;
    }

    SDL_Surface * renderImageS(u8 * ptr, size_t size, int xF, int yF) {
        SDL_Surface * tmp = IMG_Load_RW(SDL_RWFromMem(ptr, size), 1);
        SDL_Surface * tmp2 = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(tmp);
        if (xF != 1 || yF != 1) {
            SDL_Surface * tmp = shrinkSurface(tmp2, xF, yF);
            SDL_FreeSurface(tmp2);
            tmp2 = tmp;
        }

        // Increment counters
        if (tmp2 != NULL) {
            surfNum++;
            memUsage += (tmp2->pitch * tmp2->h);
        }

        return tmp2;
    }

    SDL_Surface * renderTextS(std::string str, int font_size, int style) {
        // Lock cache mutex
        std::unique_lock<std::mutex> mtx(fontCacheMutex);

        // Create font for given size if not already cached
        if (customFont) {
            if (fontCache[0].find(font_size) == fontCache[0].end()) {
                fontCache[0][font_size] = TTF_OpenFont(customFontPath.c_str(), font_size);
            }
        } else {
            for (int i = 0; i < PlSharedFontType_Total; i++) {
                if (fontCache[i].find(font_size) == fontCache[i].end()) {
                    fontCache[i][font_size] = TTF_OpenFontRW(SDL_RWFromMem(fontData[i].address, fontData[i].size), 1, font_size);
                }
            }
        }

        // Simply render and convert if custom font
        SDL_Surface * surf;
        if (customFont) {
            if (TTF_GetFontStyle(fontCache[0][font_size]) != style) {
                TTF_SetFontStyle(fontCache[0][font_size], style);
            }
            surf = TTF_RenderUTF8_Blended(fontCache[0][font_size], str.c_str(), SDL_Color{255, 255, 255, 255});

        // Need to examine multiple fonts when using Nintendo's
        } else {
            // Have a vector of surfaces for each character
            std::vector<SDL_Surface *> surfs;

            // Iterate over each character in string
            unsigned int width = 0;
            unsigned int height = 0;
            unsigned int pos = 0;
            while (pos < str.length()) {
                unsigned int posCopy = pos;
                uint16_t ch = getUTF8Char(str, pos);
                // Break if pos isn't changed (meaning no character could be extracted)
                if (pos == posCopy) {
                    break;
                }

                // Find which font contains current glyph
                int fontIndex = 0;
                for (int i = 0; i < PlSharedFontType_Total; i++) {
                    if (TTF_GlyphIsProvided(fontCache[i][font_size], ch)) {
                        fontIndex = i;
                        break;
                    }
                }

                // Draw character and insert surface into array
                if (TTF_GetFontStyle(fontCache[fontIndex][font_size]) != style) {
                    TTF_SetFontStyle(fontCache[fontIndex][font_size], style);
                }
                SDL_Surface * tmp = TTF_RenderGlyph_Blended(fontCache[fontIndex][font_size], ch, SDL_Color{255, 255, 255, 255});
                width += tmp->w;
                height = (tmp->h > height ? tmp->h : height);
                surfs.push_back(tmp);
            }

            // Unlock mutex now that access to the cache is no longer needed
            mtx.unlock();

            // Render characters to larger surface
            unsigned int x = 0;
            surf = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
            SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 255, 255, 255, 0));
            for (size_t j = 0; j < surfs.size(); j++) {
                SDL_Rect r = SDL_Rect{x, 0, surfs[j]->w, surfs[j]->h};
                SDL_BlitSurface(surfs[j], NULL, surf, &r);
                x += surfs[j]->w;
                SDL_FreeSurface(surfs[j]);
            }
        }

        // Increment counters
        if (surf != NULL) {
            surfNum++;
            memUsage += (surf->pitch * surf->h);
        }

        return surf;
    }

    SDL_Surface * renderTextWrappedS(std::string str, int font_size, uint32_t max_w, int style) {
        // Lock cache mutex
        std::lock_guard<std::mutex> mtx(fontCacheMutex);

        // Create font for given size if not already cached
        if (customFont) {
            if (fontCache[0].find(font_size) == fontCache[0].end()) {
                fontCache[0][font_size] = TTF_OpenFont(customFontPath.c_str(), font_size);
            }
        } else {
            for (int i = 0; i < PlSharedFontType_Total; i++) {
                if (fontCache[i].find(font_size) == fontCache[i].end()) {
                    fontCache[i][font_size] = TTF_OpenFontRW(SDL_RWFromMem(fontData[i].address, fontData[i].size), 1, font_size);
                }
            }
        }

        // Simply render and convert if custom font
        SDL_Surface * surf;
        if (customFont) {
            if (TTF_GetFontStyle(fontCache[0][font_size]) != style) {
                TTF_SetFontStyle(fontCache[0][font_size], style);
            }
            surf = TTF_RenderUTF8_Blended_Wrapped(fontCache[0][font_size], str.c_str(), SDL_Color{255, 255, 255, 255}, max_w);

        // Need to examine multiple fonts when using Nintendo's
        } else {
            std::vector<std::string> lines;         // Vector of characters forming line and max height of chars
            unsigned int maxLineH = 0;              // Maximum height of one line (all lines will use this height)
            unsigned int wordWidth = 0;             // Width of current word
            std::string word;                       // Characters forming word

            // Iterate over all characters in string
            unsigned int pos = 0;
            while (pos < str.length()) {
                int charSize = 0;                   // Number of chars forming unicode character
                unsigned int lastCharW = 0;         // Width of last character
                std::string line;                   // Characters forming a line
                unsigned int lineHeight = 0;        // Height of current line
                unsigned int lineWidth = 0;         // Width of line so far

                // Keep appending characters (and words) to line until we hit the edge
                while (lineWidth + wordWidth < max_w) {
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
                    uint16_t ch = getUTF8Char(str, pos);
                    charSize = pos - oldPos;
                    if (charSize == 0) {
                        // Character couldn't be found
                        break;
                    }

                    // Find which font contains current glyph
                    int fontIndex = 0;
                    for (int i = 0; i < PlSharedFontType_Total; i++) {
                        if (TTF_GlyphIsProvided(fontCache[i][font_size], ch)) {
                            fontIndex = i;
                            break;
                        }
                    }

                    // Get dimensions of glyph and update variables
                    int adv;
                    TTF_GlyphMetrics(fontCache[fontIndex][font_size], ch, NULL, NULL, NULL, NULL, &adv);
                    lastCharW = adv;
                    wordWidth += adv;
                    int h = TTF_FontLineSkip(fontCache[fontIndex][font_size]) - TTF_FontDescent(fontCache[fontIndex][font_size]);
                    lineHeight = (h > lineHeight ? h : lineHeight);
                    word += str.substr(oldPos, charSize);

                    // Add word to line if space
                    if ((char)ch == ' ') {
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
                    lineWidth -= lastCharW;
                    word = word.substr(word.length() - charSize, charSize);
                    wordWidth = lastCharW;
                }

                // Update height and add line
                maxLineH = (lineHeight > maxLineH ? lineHeight : maxLineH);
                lines.push_back(line);
                line = "";
            }

            // Now that we have dimensions, etc... render onto a surface
            surf = SDL_CreateRGBSurfaceWithFormat(0, max_w, lines.size() * (maxLineH * FONT_SPACING), 32, SDL_PIXELFORMAT_RGBA32);
            SDL_FillRect(surf, NULL, SDL_MapRGBA(surf->format, 255, 255, 255, 0));
            for (size_t i = 0; i < lines.size(); i++) {
                int x = 0;
                unsigned int pos = 0;
                while (pos < lines[i].length()) {
                    unsigned int oldPos = pos;
                    uint16_t ch = getUTF8Char(lines[i], pos);
                    if (pos == oldPos) {
                        // Character couldn't be found
                        break;
                    }

                    // Find which font contains current glyph
                    int fontIndex = 0;
                    for (int i = 0; i < PlSharedFontType_Total; i++) {
                        if (TTF_GlyphIsProvided(fontCache[i][font_size], ch)) {
                            fontIndex = i;
                            break;
                        }
                    }

                    // Draw character and blit onto surface
                    if (TTF_GetFontStyle(fontCache[fontIndex][font_size]) != style) {
                        TTF_SetFontStyle(fontCache[fontIndex][font_size], style);
                    }
                    SDL_Surface * tmp = TTF_RenderGlyph_Blended(fontCache[fontIndex][font_size], ch, SDL_Color{255, 255, 255, 255});
                    SDL_Rect r = SDL_Rect{x, i * (maxLineH * FONT_SPACING), tmp->w, tmp->h};
                    x += tmp->w;
                    SDL_BlitSurface(tmp, NULL, surf, &r);
                    SDL_FreeSurface(tmp);
                }
            }
        }

        // Increment counters
        if (surf != NULL) {
            surfNum++;
            memUsage += (surf->pitch * surf->h);
        }

        return surf;
    }

    // -= TEXTURES =-
    SDL_Texture * renderEllipse(unsigned int xd, unsigned int yd) {
        SDL_Texture * tex = createTexture(xd + 4, yd + 4);
        renderToTexture(tex);
        drawEllipse(SDL_Color{255, 255, 255, 255}, xd/2 + 2, yd/2 + 2, xd, yd);
        renderToScreen();
        return tex;
    }

    SDL_Texture * renderFilledRect(int w, int h) {
        SDL_Texture * tex = createTexture(w, h);
        renderToTexture(tex);
        drawFilledRect(SDL_Color{255, 255, 255, 255}, 0, 0, w, h);
        renderToScreen();
        return tex;
    }

    SDL_Texture * renderFilledRoundRect(int w, int h, unsigned int c) {
        SDL_Texture * tex = createTexture(w, h);
        renderToTexture(tex);
        drawFilledRoundRect(SDL_Color{255, 255, 255, 255}, 0, 0, w, h, c);
        renderToScreen();
        return tex;
    }

    SDL_Texture * renderRoundRect(int w, int h, unsigned int r, unsigned int b) {
        SDL_Texture * tex = createTexture(w, h);
        renderToTexture(tex);
        drawRoundRect(SDL_Color{255, 255, 255, 255}, 0, 0, w, h, r, b);
        renderToScreen();
        return tex;
    }

    SDL_Texture * renderRect(int w, int h, unsigned int b) {
        SDL_Texture * tex = createTexture(w, h);
        renderToTexture(tex);
        drawRect(SDL_Color{255, 255, 255, 255}, 0, 0, w, h, b);
        renderToScreen();
        return tex;
    }

    SDL_Texture * renderImage(std::string path, int xF, int yF) {
        return convertSurfaceToTexture(renderImageS(path, xF, yF));
    }

    SDL_Texture * renderImage(u8 * ptr, size_t size, int xF, int yF) {
        return convertSurfaceToTexture(renderImageS(ptr, size, xF, yF));
    }

    SDL_Texture * renderText(std::string str, int font_size, int style) {
        return convertSurfaceToTexture(renderTextS(str, font_size, style));
    }

    SDL_Texture * renderTextWrapped(std::string str, int font_size, uint32_t max_w, int style) {
        return convertSurfaceToTexture(renderTextWrappedS(str, font_size, max_w, style));
    }
};