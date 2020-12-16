#ifndef AETHER_FONTCACHE_HPP
#define AETHER_FONTCACHE_HPP

#include "Aether/types/GlyphMetrics.hpp"
#include "Aether/types/LRUCache.hpp"
#include <cstddef>
#include <mutex>
#include <string>
#include <vector>

// Forward declare all pointers used
namespace Aether {
    class Renderer;
}
#ifdef __SWITCH__
struct PlFontData_;
#endif
struct SDL_Surface;
struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

namespace {
    // Typedef cause this is really long
    typedef std::tuple<size_t, unsigned int, uint16_t> SurfaceKey;
};

// Custom hash function for above tuple, required for LRUCache
namespace std {
    template <>
    struct hash<SurfaceKey> {
        size_t operator() (const SurfaceKey & k) const {
            // Format: ....0000XXXXXXXXYYYYYYYYZZZZZZZZZZZZZZZZ
            // X: index
            // Y: font size
            // Z: UTF8 code
            size_t hash = 0;
            hash |= std::get<0>(k);
            hash <<= 8;
            hash |= std::get<1>(k);
            hash <<= 16;
            hash |= std::get<2>(k);
            return hash;
        }
    };
};

namespace Aether {
    /**
     * @brief Caches SDL_ttf font objects + surfaces to reduce rendering.
     * This class is not thread-safe, all operations should be protected
     * from the outside by a mutex.
     * @note Only one of these should be instantiated at one time.
     */

    class FontCache {
        private:
            std::string customFontPath;                                         /** @brief Path to custom font file */
            #ifdef __SWITCH__
            PlFontData_ * ninFontData;                                           /** @brief Metadata about built-in fonts */
            #endif

            std::vector< LRUCache<unsigned int, TTF_Font *> * > fontCache;      /** @brief Cache of TTF_Font objects */
            LRUCache<SurfaceKey, SDL_Surface *> * surfaceCache;                 /** @brief Cache of rendered surfaces */

            Renderer * renderer;                                                /** @brief Pointer to main renderer in order to manipulate surfaces */

        public:
            /**
             * @brief Initialize the font cache object + rendering backend.
             *
             * @param renderer Pointer to main renderer
             */
            FontCache(Renderer * renderer);

            /**
             * @brief Remove all cached data.
             */
            void empty();

            /**
             * @brief Set a custom font to use before checking built-in fonts.
             * Pass an empty string to remove.
             * @note An invalid path will not make any changes.
             *
             * @param path Path to TTF file.
             */
            void setCustomFont(const std::string & path);

            /**
             * @brief Render the requested character.
             * Searches a custom font first before in-built fonts.
             * @note The returned surface could be removed at anytime
             * subsequent calls, so a copy should be made unless
             * it's immediately used.
             *
             * @param ch UTF-8 character code
             * @param fontSize Font size to render character with
             *
             * @return Surface containing the rendered character.
             */
            SDL_Surface * getGlyph(const uint16_t ch, const unsigned int fontSize);

            /**
             * @brief Get the \ref GlyphMetrics for the character at the passed font size
             *
             * @param ch UTf-8 character code
             * @param fontSize Font size to get metrics for
             * @return \ref GlyphMetrics containing metrics of glyph
             */
            GlyphMetrics getMetrics(const uint16_t ch, const unsigned int fontSize);

            /**
             * @brief Cleans up all allocated resources
             */
            ~FontCache();
    };
};

#endif