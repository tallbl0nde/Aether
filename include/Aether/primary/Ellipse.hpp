#ifndef AETHER_ELLIPSE_HPP
#define AETHER_ELLIPSE_HPP

#include "Aether/base/Texture.hpp"

namespace Aether {
    /**
     * @brief Element for rendering an ellipse.
     * @note This element can not be drawn asynchronously!
     */
    class Ellipse : public Texture {
        private:
            unsigned int xDiameter_;        /** @brief Horizontal diameter */
            unsigned int yDiameter_;        /** @brief Vertical diameter */

            /**
             * @brief Overrides Texture's method to render an ellipse.
             */
            Drawable * renderDrawable();

        public:
            /**
             * @brief Constructs a new Ellipse element. Creates a circle if
             * the vertical diameter is not specified.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param xd Horizontal diameter in pixels
             * @param yd Vertical diameter in pixels (optional)
             */
            Ellipse(const int x, const int y, const unsigned int xd, const unsigned int yd = 0);

            /**
             * @brief Returns the ellipse's horizontal diameter
             *
             * @return Horizontal diameter in pixels
             */
            unsigned int xDiameter();

            /**
             * @brief Set a new horizontal diameter
             *
             * @param xd New diameter in pixels
             */
            void setXDiameter(const unsigned int xd);

            /**
             * @brief Returns the ellipse's vertical diameter
             *
             * @return Vertical diameter in pixels
             */
            unsigned int yDiameter();

            /**
             * @brief Set a new vertical diameter
             *
             * @param yd New diameter in pixels
             */
            void setYDiameter(const unsigned int yd);

            /**
             * @brief Render matching elliptical highlight background
             */
            Drawable * renderHighlightBG();

            /**
             * @brief Render matching elliptical highlight border
             */
            Drawable * renderHighlight();

            /**
             * @brief Render matching elliptical selection layer
             */
            Drawable * renderSelection();
    };
};

#endif