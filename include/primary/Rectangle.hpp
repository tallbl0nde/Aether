#ifndef AETHER_RECTANGLE_HPP
#define AETHER_RECTANGLE_HPP

#include "base/Texture.hpp"

namespace Aether {
    /**
     * @brief A rectangle is a texture containing either a normal or
     * rounded rectangle. The object stores all relevant information
     * so that it can be regenerated whenever the dimensions/radius
     * changes.
     * @note This element can not have the texture generation deferred yet!
     */
    class Rectangle : public Texture {
        private:
            /** @brief Radius of each corner (draws rounded rectangle when > 0) */
            unsigned int cornerRadius_;

            /** @brief Generate a rectangle surface */
            void generateSurface();

        public:
            /**
             * @brief Construct a new Rectangle object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of rectangle
             * @param h height of rectangle
             * @param r corner radius (optional)
             */
            Rectangle(int x, int y, int w, int h, unsigned int r = 0);

            /**
             * @brief Get the corner radius for rectangle
             *
             * @return corner radius
             */
            unsigned int cornerRadius();

            /**
             * @brief Set new corner radius for rectangle
             *
             * @param r new corner radius
             */
            void setCornerRadius(unsigned int r);

            /**
             * @brief Set new size for rectangle
             *
             * @param w new width of rectangle
             * @param h new height of rectangle
             */
            void setRectSize(int w, int h);
    };
};

#endif