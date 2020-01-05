#ifndef AETHER_MENUITEM_HPP
#define AETHER_MENUITEM_HPP

#include "base/Element.hpp"
#include "primary/Rectangle.hpp"
#include "primary/Text.hpp"

namespace Aether {
    // A MenuItem is a combination of elements which looks like and functions
    // similar to Horizon's. All children are handled internally
    class MenuItem : public Element {
        private:
            // Prevent interfering with elements
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;
            using Element::setSelectable;

            // Child element pointers (required to update pointers)
            Rectangle * rect;
            Text * text;

            // Is this item the active one?
            bool active;

            // Colours for sub elements
            Colour activeColour;
            Colour inactiveColour;

        public:
            // Constructor takes parent element, string and callback function
            MenuItem(Element *, std::string, std::function<void()>);

            // Adjusting XY has unique behaviour (reposition all elements)
            void setX(int);
            void setY(int);
            void setW(int);
            void setXY(int, int);
            void setXYWH(int, int, int, int);

            // Setter for isActive
            void setActive(bool);

            // Setter for colours
            void setActiveColour(Colour);
            void setActiveColor(Color);
            void setInactiveColour(Colour);
            void setInactiveColor(Color);

            // Destructor deletes all children
            ~MenuItem();
    };
};

#endif