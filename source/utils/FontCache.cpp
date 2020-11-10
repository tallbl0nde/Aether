#include "Aether/utils/FontCache.hpp"

namespace Aether {
    FontCache::FontCache() {
        this->customFontPath = "";
        this->emptyCache();

        // Get metadata about built-in fonts
        this->ninFontData = new PlFontData[PlSharedFontType_Total];
        for (int i = 0; i < PlSharedFontType_Total; i++) {
            plGetSharedFontByType(this->ninFontData+i, static_cast<PlSharedFontType>(i));
        }

        // Create initial maps in vector
        this->cache.resize(PlSharedFontType_Total + 1);
    }

    void FontCache::emptyCache() {
        // Free + empty all structures
        std::scoped_lock<std::mutex> mtx(this->mutex);
        for (size_t i = 0; i < this->cache.size(); i++) {
            for (auto it = this->cache[i].begin(); it != this->cache[i].end(); it++) {
                TTF_CloseFont(it->second);
            }
            this->cache[i].clear();
        }
    }

    void FontCache::setCustomFont(const std::string & path) {
        this->customFontPath = path;
        this->emptyCache();
    }

    TTF_Font * FontCache::getFontWithGlyph(const uint16_t ch, const int fontSize) {
        std::scoped_lock<std::mutex> mtx(this->mutex);

        // Check custom font first if one is set
        if (!this->customFontPath.empty()) {
            // Create TTF_Font * if it doesn't exist yet
            size_t customFontIdx = this->cache.size()-1;
            if (this->cache[customFontIdx].find(fontSize) == this->cache[customFontIdx].end()) {
                this->cache[customFontIdx][fontSize] = TTF_OpenFont(this->customFontPath.c_str(), fontSize);
            }

            // Check if glyph is in font
            if (TTF_GlyphIsProvided(this->cache[customFontIdx][fontSize], ch)) {
                return this->cache[customFontIdx][fontSize];
            }
        }

        // Otherwise if no custom font or glyph not in font, iterate over Nintendo's fonts
        for (int i = 0; i < PlSharedFontType_Total; i++) {
            // Create TTF_Font * if it doesn't exist yet
            if (this->cache[i].find(fontSize) == this->cache[i].end()) {
                this->cache[i][fontSize] = TTF_OpenFontRW(SDL_RWFromMem(this->ninFontData[i].address, this->ninFontData[i].size), 1, fontSize);
            }

            // Check if glyph is in font
            if (TTF_GlyphIsProvided(this->cache[i][fontSize], ch)) {
                return this->cache[i][fontSize];
            }
        }

        // Otherwise return a default index in order to draw box
        return (!this->customFontPath.empty() ? this->cache[this->cache.size()-1][fontSize] : this->cache[0][fontSize]);
    }

    FontCache::~FontCache() {
        this->emptyCache();
        delete this->ninFontData;
    }
};