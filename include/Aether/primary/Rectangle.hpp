#ifndef AETHER_RECTANGLE_HPP
#define AETHER_RECTANGLE_HPP

#include "Aether/base/Texture.hpp"

namespace Aether {
    /**
     * @brief A rectangle is a texture containing either a normal or rounded rectangle.
     * @note This element can not be drawn asynchronously!
     */
    class Rectangle : public Texture {
        private:
            /** @brief Radius of each corner (draws rounded rectangle when > 0) */
            unsigned int cornerRadius_;

            /**
             * @brief Overrides Texture's method to render a rectangle.
             */
            Drawable * renderDrawable();

        public:
            /**
             * @brief Constructs a new Rectangle element.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param w Width of rectangle
             * @param h Height of rectangle
             * @param radius Corner radius (optional)
             */
            Rectangle(const int x, const int y, const int w, const int h, const unsigned int radius = 0);

            /**
             * @brief Returns the corner radius of the rectangle
             *
             * @return Corner radius in pixels
             */
            unsigned int cornerRadius();

            /**
             * @brief Set the new corner radius for the rectangle
             *
             * @param radius New corner radius in pixels
             */
            void setCornerRadius(const unsigned int r);

            /**
             * @brief Set new dimensions for the rectangle
             *
             * @param w New width of rectangle
             * @param h New height of rectangle
             */
            void setRectSize(const int w, const int h);
    };
};

#endif