#ifndef AETHER_MENUSEPERATOR_HPP
#define AETHER_MENUSEPARATOR_HPP

#include "Aether/base/Element.hpp"
#include "Aether/primary/Rectangle.hpp"

namespace Aether {
    /**
     * @brief A MenuSeparator contains a rectangle with some padding
     * above/below. It is not selectable.
     */
    class MenuSeparator : public Element {
        private:
            /** @brief Rectangle object to draw */
            Rectangle * rect;

        public:
            /**
             * @brief Construct a new Menu Separator object.
             * Creates a rectangle with width 100px.
             * 
             * @param c rectangle colour
             */
            MenuSeparator(Colour c = {255, 255, 255, 255});

            /**
             * @brief Adjusts menu separator width.
             * Adjusting width also rescales the rectangle.
             * 
             * @param w new width
             */
            void setW(int w);
    };
};

#endif