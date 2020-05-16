#ifndef AETHER_BOX_HPP
#define AETHER_BOX_HPP

#include "base/Texture.hpp"

namespace Aether {
    /**
     * @brief A Box is a rectangle outline with no fill.
     * @note This element can not have the texture generation deferred yet!
     */
    class Box : public Texture {
        private:
            /** @brief Size of border */
            unsigned int border_;

            /** @brief Radius of each corner (draws rounded rectangle when > 0) */
            unsigned int cornerRadius_;

            /** @brief Generate a box surface */
            void generateSurface();

        public:
            /**
             * @brief Construct a new Box object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of box
             * @param h height of box
             * @param b border thickness
             * @param r corner radius
             */
            Box(int x, int y, int w, int h, unsigned int b = 1, unsigned int r = 0);

            /**
             * @brief Get the border thickness of box
             *
             * @return border thickness
             */
            unsigned int border();
            /**
             * @brief Set the border thickness of box
             *
             * @param b new border thickness
             */
            void setBorder(unsigned int b);

            /**
             * @brief Get the current corner radius for box
             *
             * @return corner radius
             */
            unsigned int cornerRadius();

            /**
             * @brief Set the corner radius for box
             *
             * @param r new corner radius
             */
            void setCornerRadius(unsigned int r);

            /**
             * @brief Adjust box size
             *
             * @param w new width of box
             * @param h new height of box
             */
            void setBoxSize(int w, int h);
    };
};

#endif