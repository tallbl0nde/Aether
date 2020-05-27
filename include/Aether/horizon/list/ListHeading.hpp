#ifndef AETHER_LISTHEADING_HPP
#define AETHER_LISTHEADING_HPP

#include "Aether/base/Element.hpp"
#include "Aether/primary/Rectangle.hpp"
#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief A ListHeading is a small rectangle with text alongside, usually used in a list.
     */
    class ListHeading : public Element {
        private:
            /** @brief Rectangle for ListHeading */
            Rectangle * rect;
            /** @brief Text for ListHeading */
            Text * text;

        public:
            /**
             * @brief Construct a new List Heading object.
             * Creates rectangle and text associated when list heading is instantiated.
             * 
             * @param s initial text string
             */
            ListHeading(std::string s);

            /**
             * @brief Get the rectangle colour
             * 
             * @return rectangle colour
             */
            Colour getRectColour();

            /**
             * @brief Set the rectangle colour
             * 
             * @param c new rectangle colour
             */
            void setRectColour(Colour c);

            /**
             * @brief Get the text colour
             * 
             * @return text colour
             */
            Colour getTextColour();

            /**
             * @brief Set the text colour
             * 
             * @param c new text colour
             */
            void setTextColour(Colour c);
    };
};

#endif