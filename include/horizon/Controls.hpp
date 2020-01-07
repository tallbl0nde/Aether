#ifndef AETHER_CONTROLS_HPP
#define AETHER_CONTROLS_HPP

#include "base/Element.hpp"
#include "ControlItem.hpp"

namespace Aether {
    // "Controls" contains the control scheme found at the bottom of a screen.
    // It can be positioned anywhere though.
    // Note that x/y should be the top left-most point
    // and the w/h should be set to the desired size.
    // Items are automatically right aligned.
    // First = right-most; Last = left-most
    class Controls : public Element {
        private:
            // Colour to draw controls in
            Colour colour;

            // Store pointer to elements in order to
            // access class-specific methods
            std::vector<ControlItem *> items;

            // Prevent interfering with elements directly
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;

            // Called to position ControlItems
            void repositionElements();

        public:
            // Wrappers for add/remove elements
            void addItem(ControlItem *);
            bool removeItem(ControlItem *);
            void removeAllItems();

            // Adjusting XY has unique behaviour (reposition all elements)
            void setX(int);
            void setY(int);
            void setW(int);
            void setH(int);
            void setXY(int, int);
            void setWH(int, int);
            void setXYWH(int, int, int, int);

            // Return set colour
            Colour getColour();
            Color getColor();
            // Set colour of controls
            void setColour(Colour);
            void setColor(Color);
            void setColour(uint8_t, uint8_t, uint8_t, uint8_t);
            void setColor(uint8_t, uint8_t, uint8_t, uint8_t);
    };
};


#endif