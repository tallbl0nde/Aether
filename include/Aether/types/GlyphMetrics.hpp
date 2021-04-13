#ifndef AETHER_FONTMETRICS_HPP
#define AETHER_FONTMETRICS_HPP

#include <cstdint>

namespace Aether {
    /**
     * @brief Class storing metrics for a font's character. Designed to be read-only
     * as it is only able to return values passed at construction.
     */
    class GlyphMetrics {
        private:
            uint16_t ch_;           /** @brief Character code */
            int width_;             /** @brief Width of glyph */
            int height_;            /** @brief Height of glyph (ignoring 'descent') */
            int lineHeight_;        /** @brief Recommended height of a line containing the glyph */

        public:
            /**
             * @brief Default constructor to initialize with blank values
             */
            GlyphMetrics();

            /**
             * @brief Create a new GlyphMetrics object
             *
             * @param ch Character metrics are for
             * @param width Width of glyph
             * @param height Height of glyph
             * @param lineHeight Height of line containing glyph
             */
            GlyphMetrics(const uint16_t ch, const int width, const int height, const int lineHeight);

            /**
             * @brief Returns character referenced by metrics
             *
             * @return UTF-8 character code
             */
            uint16_t character();

            /**
             * @brief Return the width of the glyph
             *
             * @return Width of glyph in pixels
             */
            int width();

            /**
             * @brief Return the height of the glyph
             *
             * @return Height of glyph in pixels
             */
            int height();

            /**
             * @brief Return the recommend line height for the glyph
             *
             * @return Recommended height of a line containing this glyph
             */
            int lineHeight();
    };
};

#endif