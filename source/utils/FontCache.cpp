#include "Aether/utils/FontCache.hpp"
#include "Aether/types/LRUCache.hpp"
#include "Aether/Renderer.hpp"
#include "Aether/utils/Utils.hpp"
#include <SDL2/SDL_ttf.h>
#ifdef __SWITCH__
#include <switch.h>
#endif

// Number of font objects kept in cache for each font
// (Each font size requires a separate object)
static constexpr unsigned int maxFontObjects = 30;

// Number of individual surfaces kept in cache, regardless
// of the surface's font size/font
// Although font size influences memory used, this shouldn't
// use more than a few MB ever
static constexpr unsigned int maxSurfaces = 200;

#ifdef __SWITCH__
// Inherit proper struct
struct PlFontData_ : public PlFontData{};
#endif

namespace Aether {
    FontCache::FontCache(Renderer * renderer) {
        this->customFontPath = "";
        this->renderer = renderer;
        TTF_Init();

        // Get metadata about built-in fonts
        #ifdef __SWITCH__
            this->ninFontData = static_cast<PlFontData_ *>(new PlFontData[PlSharedFontType_Total]);
            for (int i = 0; i < PlSharedFontType_Total; i++) {
                plGetSharedFontByType(this->ninFontData+i, static_cast<PlSharedFontType>(i));
            }
        #endif

        // Create initial structures
        #ifdef __SWITCH__
            this->fontCache.resize(PlSharedFontType_Total + 1, nullptr);
        #else
            this->fontCache.resize(1, nullptr);
        #endif
        this->surfaceCache = nullptr;
        this->empty();
    }

    void FontCache::empty() {
        // Replacing both LRUCache's will cause them to remove each object,
        // and call the specified callbacks to clean them up.
        for (size_t i = 0; i < this->fontCache.size(); i++) {
            delete this->fontCache[i];
            this->fontCache[i] = new LRUCache<unsigned int, TTF_Font *>(maxFontObjects, [this](const unsigned int size, const TTF_Font * font) {
                TTF_CloseFont(const_cast<TTF_Font *>(font));
            });
        }

        delete this->surfaceCache;
        this->surfaceCache = new LRUCache<SurfaceKey, SDL_Surface *>(maxSurfaces, [this](const SurfaceKey key, const SDL_Surface * surf) {
            this->renderer->destroySurface(const_cast<SDL_Surface *>(surf), false);
        });
    }

    void FontCache::setCustomFont(const std::string & path) {
        // Ensure file exists
        if (!path.empty() && !Utils::fileExists(path)) {
            return;
        }

        // Update path and empty current caches
        this->customFontPath = path;
        this->empty();
    }

    SDL_Surface * FontCache::getGlyph(const uint16_t ch, const unsigned int fontSize) {
        // Surface to return
        SDL_Surface * surf = nullptr;

        // Check custom font first
        if (!this->customFontPath.empty()) {
            // Create TTF_Font * if not in cache
            size_t customFontIdx = this->fontCache.size()-1;
            if (!this->fontCache[customFontIdx]->hasKey(fontSize)) {
                this->fontCache[customFontIdx]->addData(fontSize, TTF_OpenFont(this->customFontPath.c_str(), fontSize));
            }

            // Ensure the glyph is present before proceeding
            TTF_Font * font = this->fontCache[customFontIdx]->getData(fontSize);
            if (TTF_GlyphIsProvided(font, ch)) {
                // Check if we have a cached surface, creating one if we need to
                SurfaceKey key = SurfaceKey(customFontIdx, fontSize, ch);
                if (this->surfaceCache->hasKey(key)) {
                    surf = this->surfaceCache->getData(key);

                } else {
                    surf = TTF_RenderGlyph_Blended(font, ch, {255, 255, 255, 255});
                    this->surfaceCache->addData(key, surf);
                }
            }
        }

        #ifdef __SWITCH__
            // Don't check internal fonts if we have a glyph already
            if (surf != nullptr) {
                return surf;
            }

            // Otherwise if no custom font or glyph not in font, iterate over Nintendo's fonts
            for (int i = 0; i < PlSharedFontType_Total; i++) {
                // Create TTF_Font * if not in cache
                if (!this->fontCache[i]->hasKey(fontSize)) {
                    this->fontCache[i]->addData(fontSize, TTF_OpenFontRW(SDL_RWFromMem(this->ninFontData[i].address, this->ninFontData[i].size), 1, fontSize));
                }

                // Ensure the glyph is present before proceeding
                TTF_Font * font = this->fontCache[i]->getData(fontSize);
                if (TTF_GlyphIsProvided(font, ch)) {
                    // Check if we have a cached surface, creating one if we need to
                    SurfaceKey key = SurfaceKey(i, fontSize, ch);
                    if (this->surfaceCache->hasKey(key)) {
                        surf = this->surfaceCache->getData(key);

                    } else {
                        surf = TTF_RenderGlyph_Blended(font, ch, {255, 255, 255, 255});
                        this->surfaceCache->addData(key, surf);
                    }

                    break;
                }
            }
        #endif

        // Otherwise return a default index in order to draw box
        // If there is no font created, this will have no effect and thus a nullptr is returned
        if (surf == nullptr && this->fontCache[this->fontCache.size() - 1]->hasKey(fontSize)) {
            SurfaceKey key = SurfaceKey(this->fontCache.size() - 1, fontSize, ch);
            if (this->surfaceCache->hasKey(key)) {
                surf = this->surfaceCache->getData(key);

            } else {
                surf = TTF_RenderGlyph_Blended(this->fontCache[this->fontCache.size() - 1]->getData(fontSize), ch, {255, 255, 255, 255});
                this->surfaceCache->addData(key, surf);
            }
        }

        return surf;
    }

    GlyphMetrics FontCache::getMetrics(const uint16_t ch, const unsigned int fontSize) {
        // Find a font containing the character at the specified size,
        // creating a new object if needed
        if (!this->customFontPath.empty()) {
            // Create TTF_Font * if not in cache
            size_t customFontIdx = this->fontCache.size()-1;
            if (!this->fontCache[customFontIdx]->hasKey(fontSize)) {
                this->fontCache[customFontIdx]->addData(fontSize, TTF_OpenFont(this->customFontPath.c_str(), fontSize));
            }

            // Ensure the glyph is present
            TTF_Font * font = this->fontCache[customFontIdx]->getData(fontSize);
            if (TTF_GlyphIsProvided(font, ch)) {
                int width, height;
                uint16_t str[2] = {ch, '\0'};
                TTF_SizeUNICODE(font, str, &width, &height);
                return GlyphMetrics(ch, width, height, TTF_FontLineSkip(font));
            }
        }

        #ifdef __SWITCH__
            // Otherwise if no custom font or glyph not in font, iterate over Nintendo's fonts
            for (int i = 0; i < PlSharedFontType_Total; i++) {
                // Create TTF_Font * if not in cache
                if (!this->fontCache[i]->hasKey(fontSize)) {
                    this->fontCache[i]->addData(fontSize, TTF_OpenFontRW(SDL_RWFromMem(this->ninFontData[i].address, this->ninFontData[i].size), 1, fontSize));
                }

                // Ensure the glyph is present before proceeding
                TTF_Font * font = this->fontCache[i]->getData(fontSize);
                if (TTF_GlyphIsProvided(font, ch)) {
                    int width, height;
                    uint16_t str[2] = {ch, '\0'};
                    TTF_SizeUNICODE(font, str, &width, &height);
                    return GlyphMetrics(ch, width, height, TTF_FontLineSkip(font));
                }
            }
        #endif

        // Return default metrics object if not found
        return GlyphMetrics();
    }

    FontCache::~FontCache() {
        // Empty caches
        for (size_t i = 0; i < this->fontCache.size(); i++) {
            delete this->fontCache[i];
        }
        delete this->surfaceCache;

        #ifdef __SWITCH__
            delete[] this->ninFontData;
        #endif

        TTF_Quit();
    }
};