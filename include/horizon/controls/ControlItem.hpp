#ifndef AETHER_CONTROLITEM_HPP
#define AETHER_CONTROLITEM_HPP

#include "primary/Text.hpp"

namespace Aether {
    /**
     * @brief A ControlItem is stored and handled by the "Controls" element.
     * It contains the two textures for a button icon and hint.
     */
    class ControlItem : public Element {
        private:
            /** @brief Texture for icon */
            Text * icon;
            /** @brief Texture for hint */
            Text * hint;
            /** @brief Colour for text */
            Colour colour;

            // These functions can't be called
            using Element::addElement;
            using Element::removeElement;
            using Element::removeAllElements;
            using Element::setSelectable;

        public:
            /**
             * @brief Construct a new Control Item object
             * 
             * @param k button associated with Control Item
             * @param s text hint
             */
            ControlItem(Button k, std::string s);

            /**
             * @brief Get the text colour
             * 
             * @return text colour
             */
            Colour getColour();

            /**
             * @brief Set the text colour
             * 
             * @param c new text colour
             */
            void setColour(Colour c);

            /**
             * @brief Set the text colour
             * 
             * @param r red value of text colour
             * @param g green value of text colour
             * @param b blue value of text colour
             * @param a alpha value of text colour
             */
            void setColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    };
};

#endif