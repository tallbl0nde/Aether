#ifndef AETHER_HELPBUTTON_HPP
#define AETHER_HELPBUTTON_HPP

#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief HelpButton is a round button displaying a question
     * mark as seen in Horizon.
     */
    class HelpButton : public Element {
        private:
            /** @brief Button Text */
            Text * text;

        public:
            /**
             * @brief Creates the texture
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param d diamater of round button
             * @param f callback function when button is pressed
             */
            HelpButton(int x, int y, int d, std::function<void()> f);

            /**
             * @brief Get the colour of the question mark
             *
             * @return colour
             */
            Colour getColour();

            /**
             * @brief Set the colour of the question mark
             *
             * @param c new colour
             */
            void setColour(Colour c);

            /**
             * @brief Render highlight background
             */
            void renderHighlightBG();

            /**
             * @brief Render highlight border
             */
            void renderHighlight();

            /**
             * @brief Render selection layer
             */
            void renderSelection();
    };
};

#endif