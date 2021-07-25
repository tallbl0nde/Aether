#ifndef AETHER_FILLEDBUTTON_HPP
#define AETHER_FILLEDBUTTON_HPP

#include "Aether/base/Element.hpp"
#include "Aether/primary/Rectangle.hpp"
#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief A FilledButton is exactly that. The
     * text/rectangle are resized/repositioned to
     * be centered on any resizes.
     */
    class FilledButton : public Element {
        private:
            /** @brief Pointer to rectangle associated with the button */
            Rectangle * rect;
            /** @brief Pointer to text associated with the button */
            Text * text;

        public:
            /**
             * @brief Construct a new Filled Button object
             *
             * @param x x-coordinate of start position offset
             * @param y y-coordinate of start position offset
             * @param w width of button
             * @param h height of button
             * @param t button text
             * @param s font size (will be smaller if text can't fit)
             * @param f function to invoke on button press
             */
            FilledButton(int x, int y, int w, int h, std::string t, unsigned int s, std::function<void()> f);

            /**
             * @brief Get the colour to fill rectangle with
             *
             * @return rectangle colour
             */
            Colour getFillColour();

            /**
             * @brief Set the colour to fill rectangle with
             *
             * @param c new rectangle colour
             */
            void setFillColour(Colour c);

            /**
             * @brief Get the text's colour
             *
             * @return text colour
             */
            Colour getTextColour();

            /**
             * @brief Set the text's colour
             *
             * @param c new colour to set
             */
            void setTextColour(Colour c);

            /**
             * @brief Get current button text
             *
             * @return button text
             */
            std::string getString();

            /**
             * @brief Set new button text
             *
             * @param s new button text
             */
            void setString(std::string s);

            /**
             * @brief Set new width for the button
             * @note Adjusting dimensions causes text position to be adjusted
             * @note Rectangle needs to be rendered again
             * @param w new width
             */
            void setW(int w);

            /**
             * @brief Set new width for the button
             * @note Adjusting dimensions causes text position to be adjusted
             * @note Rectangle needs to be rendered again
             * @param h new height
             */
            void setH(int h);

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
