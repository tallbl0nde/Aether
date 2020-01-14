#ifndef AETHER_MENUSEPERATOR_HPP
#define AETHER_MENUSEPARATOR_HPP

#include "primary/Rectangle.hpp"
#include "MenuItem.hpp"

namespace Aether {
    // A MenuSeparator is a MenuItem that contains a rectangle with some padding
    // above/below. It is not selectable.
    class MenuSeparator : public MenuItem {
        private:
            // Rectangle object to draw
            Rectangle * rect;

        public:
            // Constructor creates rectangle with width 100px
            MenuSeparator(Colour = {255, 255, 255, 255});

            // Adjusting width needs to rescale rectangle
            void setW(int);
    };
};

#endif