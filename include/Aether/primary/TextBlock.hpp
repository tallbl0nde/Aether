#ifndef AETHER_TEXTBLOCK_HPP
#define AETHER_TEXTBLOCK_HPP

#include "Aether/base/BaseText.hpp"

namespace Aether {
    /**
     * @brief Element for rendering a multi-line block of text.
     */
    class TextBlock : public BaseText {
        private:
            /** @brief Width in pixels to wrap at */
            std::atomic<unsigned int> wrapWidth_;

            /**
             * @brief Overrides Texture's method to render a block of text.
             */
            Drawable * renderDrawable();

        public:
            /**
             * @brief Constructs a new TextBlock element.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param str Text to render
             * @param size Font size to render in
             * @param wrap Line wrap width (in pixels)
             * @param type \ref Render type of rendering to perform
             */
            TextBlock(const int x, const int y, const std::string & str, const unsigned int size, const unsigned int wrap, const Render type = Render::Sync);

            /**
             * @brief Returns the dimensions of the given string if it were to be rendered, without
             * performing any slow rendering.
             *
             * @param str String to measure
             * @param size Font size to measure text with
             * @param width Maximum width of one line
             * @return Pair of dimensions, with the first value being
             * the width and second being the height.
             */
            static std::pair<int, int> getDimensions(const std::string & str, const unsigned int size, const unsigned int width);

            /**
             * @brief Returns the maximum width allowed for a line
             *
             * @return Max line width in pixels
             */
            unsigned int wrapWidth();

            /**
             * @brief Set the new max line width
             *
             * @param wrap New line width in pixels
             */
            void setWrapWidth(const unsigned int wrap);
    };
};

#endif