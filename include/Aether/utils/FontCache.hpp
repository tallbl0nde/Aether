#ifndef AETHER_FONT_HPP
#define AETHER_FONT_HPP

#include <mutex>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <switch.h>
#include <unordered_map>
#include <vector>

namespace Aether {
    /**
     * @brief Caches SDL_ttf font objects and allows for easy searching of glyphs.
     */
    class FontCache {
        private:
            // Typedef cache structure
            typedef std::vector< std::unordered_map<int, TTF_Font *> > Cache;

            std::string customFontPath;     /** @brief Path to custom font file */
            PlFontData * ninFontData;       /** @brief Metadata about built-in fonts */

            Cache cache;                    /** @brief Cache of TTF_Font * */
            std::mutex mutex;               /** @brief Mutex protecting TTF_Font cache */

        public:
            /**
             * @brief Initialize the font cache object
             *
             * @note Must be constructed after initializing Switch services.
             */
            FontCache();

            /**
             * @brief Remove all cached data.
             */
            void emptyCache();

            /**
             * @brief Set a custom font to use before checking built-in fonts.
             * Pass an empty string to remove.
             */
            void setCustomFont(const std::string &);

            /**
             * @brief Get a font containing the provided character.
             * Searches a custom font first before in-built fonts.
             *
             * @param ch UTF-8 character code
             * @param fontSize Font size to render character with
             *
             * @return TTF_Font * containing the given character to render.
             */
            TTF_Font * getFontWithGlyph(const uint16_t ch, const int fontSize);

            /**
             * @brief Cleans up all allocated resources
             */
            ~FontCache();
    };
};

#endif