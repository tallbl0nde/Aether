#ifndef AETHER_BORDERBUTTON_HPP
#define AETHER_BORDERBUTTON_HPP

#include "Aether/base/Element.hpp"
#include "Aether/primary/Box.hpp"
#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief A BorderButton is exactly that. The text/box are
     * resized/repositioned to be centered on any resizes.
     */
    class BorderButton : public Element {
        private:
            /** @brief The box associated with the button */
            Box * box;
            /** @brief The text in the button */
            Text * text;

        public:
            /**
             * @brief Construct a new Border Button object
             *
             * @param x x-coordinate of the start position offset
             * @param y y-coordinate of the start position offset
             * @param w width of the button
             * @param h height of the buttpn
             * @param b border size
             * @param t text in button
             * @param s button size
             * @param f callback on button press
             */
            BorderButton(int x, int y, int w, int h, unsigned int b, std::string t, unsigned int s, std::function<void()> f);

            /**
             * @brief Get the colour of the border
             *
             * @return border colour
             */
            Colour getBorderColour();

            /**
             * @brief Set the colour of the border
             *
             * @param c new border colour
             */
            void setBorderColour(Colour c);

            /**
             * @brief Get the colour of the text
             *
             * @return text colour
             */
            Colour getTextColour();

            /**
             * @brief Set the colour of the text
             *
             * @param c new text colour
             */
            void setTextColour(Colour c);

            /**
             * @brief Get the text for the button
             *
             * @return button text
             */
            std::string getString();

            /**
             * @brief Set the text for the button
             *
             * @param s new button text
             */
            void setString(std::string s);

            /**
             * @brief Set the width for the button
             * @note Changing dimension requires text position to be adjusted
             * @note Box to be rendered again
             * @param w new button width
             */
            void setW(int w);

            /**
             * @brief Set the height for the button
             * @note Changing dimension requires text position to be adjusted
             * @note Box to be rendered again
             * @param h new button height
             */
            void setH(int h);

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