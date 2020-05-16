#ifndef AETHER_TEXTBLOCK_HPP
#define AETHER_TEXTBLOCK_HPP

#include "base/BaseText.hpp"

namespace Aether {
    /**
     * @brief TextBlock extends BaseText by wrapping text with the given width
     *
     * It's literally a block of text
     */
    class TextBlock : public BaseText {
        private:
            /** @brief Width in pixels to wrap at */
            std::atomic<unsigned int> wrapWidth_;

            /** @brief Generate a text block surface */
            void generateSurface();

        public:
            /**
             * @brief Construct a new Text Block object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param s block string
             * @param f font size
             * @param w wrap width (in pixels)
             * @param l font style to use for block
             * @param t \ref ::RenderType to use for texture generation
             */
            TextBlock(int x, int y, std::string s, unsigned int f, unsigned int w, FontStyle l = FontStyle::Regular, RenderType t = RenderType::OnCreate);

            /**
             * @brief Get block wrap width
             *
             * @return wrap width
             */

            unsigned int wrapWidth();
            /**
             * @brief Set new wrap width
             * @note Altering requires re-render of text
             *
             * @param w new wrap width
             */
            void setWrapWidth(unsigned int w);
    };
};

#endif