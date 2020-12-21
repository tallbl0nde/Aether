#ifndef AETHER_CONTROLITEM_HPP
#define AETHER_CONTROLITEM_HPP

#include "Aether/primary/Text.hpp"

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
            Colour colour_;

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
            Colour colour();

            /**
             * @brief Set the text colour
             *
             * @param c new text colour
             */
            void setColour(Colour c);
    };
};

#endif