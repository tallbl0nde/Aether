#ifndef AETHER_CONTROLITEM_HPP
#define AETHER_CONTROLITEM_HPP

#include "primary/Text.hpp"

namespace Aether {
    // A ControlItem is stored and handled by the "Controls" element.
    // It contains the two textures for a button icon and hint.
    class ControlItem : public Element {
        private:
            // Textures for button + hint
            Text * icon;
            Text * hint;

            // Colour for text
            Colour colour;

            // These functions can't be called
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;
            using Element::setSelectable;

        public:
            // Constructor accepts key/button and text hint
            ControlItem(Key, std::string);

            // Adjusting XY has unique behaviour (reposition all elements)
            void setX(int);
            void setY(int);
            void setXY(int, int);

            // Return the set colour
            Colour getColour();
            Color getColor();
            // Set the colour
            void setColour(Colour);
            void setColour(uint8_t, uint8_t, uint8_t, uint8_t);
            void setColor(Color);
            void setColor(uint8_t, uint8_t, uint8_t, uint8_t);
    };
};

#endif