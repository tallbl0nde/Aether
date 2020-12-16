#ifndef AETHER_ELLIPSE_HPP
#define AETHER_ELLIPSE_HPP

#include "Aether/base/Texture.hpp"

namespace Aether {
    /**
     * @brief An ellipse is a texture containing an ellipse.
     * It can be used as both a circle and ellipse/oval.
     * @note This element can not have the texture generation deferred yet!
     */
    class Ellipse : public Texture {
        private:
            /** @brief Horizontal diameter */
            unsigned int xDiameter_;
            /** @brief Vertical diameter */
            unsigned int yDiameter_;

            /** @brief Generate an ellipse surface */
            void generateSurface();

        public:
            /**
             * @brief Construct a new Ellipse object.
             * Shape will be circle if y-diameter is not specified
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param xd x-diameter
             * @param yd y-diameter
             */
            Ellipse(int x, int y, unsigned int xd, unsigned int yd = 0);

            /**
             * @brief Get ellipse's x-diameter
             *
             * @return ellipse's x-diameter
             */
            unsigned int xDiameter();

            /**
             * @brief Set ellipse's x-diameter
             *
             * @param xd ellipse's new x-diameter
             */
            void setXDiameter(unsigned int xd);

            /**
             * @brief Get ellipse's y-diameter
             *
             * @return ellipse's y-diameter
             */
            unsigned int yDiameter();

            /**
             * @brief Set ellipse's y-diameter
             *
             * @param yd ellipse's y-diameter
             */
            void setYDiameter(unsigned int yd);

            /**
             * @brief Render highlight background (elliptical)
             */
            Drawable * renderHighlightBG();

            /**
             * @brief Render highlight border (elliptical)
             */
            Drawable * renderHighlight();

            /**
             * @brief Render selection layer (elliptical)
             */
            Drawable * renderSelection();
    };
};

#endif