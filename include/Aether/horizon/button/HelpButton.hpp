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
             * @param f function to invoke on button press
             */
            HelpButton(int x, int y, int d, std::function<void()> f);

            /**
             * @brief Get the colour of the question mark
             *
             * @return colour
             */
            Colour colour();

            /**
             * @brief Set the colour of the question mark
             *
             * @param c new colour
             */
            void setColour(Colour c);

            /**
             * @brief Render highlight background
             */
            Drawable * renderHighlightBG();

            /**
             * @brief Render highlight border
             */
            Drawable * renderHighlight();

            /**
             * @brief Render selection layer
             */
            Drawable * renderSelection();
    };
};

#endif