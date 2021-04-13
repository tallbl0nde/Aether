#ifndef AETHER_BOX_HPP
#define AETHER_BOX_HPP

#include "Aether/base/Texture.hpp"

namespace Aether {
    /**
     * @brief Element for rendering a 'box' (rectangle with no fill).
     * @note This element can not be drawn asynchronously!
     */
    class Box : public Texture {
        private:
            unsigned int border_;               /** @brief Size of border */
            unsigned int cornerRadius_;         /** @brief Radius of each corner (draws rounded rectangle when > 0) */

            /**
             * @brief Override in order to render a box.
             */
            Drawable * renderDrawable();

        public:
            /**
             * @brief Constructs a new Box element.
             *
             * @param x Top-left x coordinate
             * @param y Top-left y coordinate
             * @param w Width of box
             * @param h Height of box
             * @param thick Border thickness (optional)
             * @param radius Border corner radius (optional)
             */
            Box(const int x, const int y, const int w, const int h, const unsigned int thick = 1, const unsigned int radius = 0);

            /**
             * @brief Returns the thickness of the border
             *
             * @return Border thickness in pixels
             */
            unsigned int thickness();

            /**
             * @brief Set the border thickness of the box
             *
             * @param thick New border thickness
             */
            void setThickness(const unsigned int thick);

            /**
             * @brief Returns the corner radius of the border
             *
             * @return Corner radius in pixels
             */
            unsigned int cornerRadius();

            /**
             * @brief Set the corner radius of the border
             *
             * @param radius New corner radius
             */
            void setCornerRadius(const unsigned int radius);

            /**
             * @brief Adjusts the dimensions of the box
             *
             * @param w New width of box
             * @param h New height of box
             */
            void setBoxSize(const int w, const int h);
    };
};

#endif