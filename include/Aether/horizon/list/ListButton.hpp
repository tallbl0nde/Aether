#ifndef AETHER_LISTBUTTON_HPP
#define AETHER_LISTBUTTON_HPP

#include "Aether/primary/Rectangle.hpp"
#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief A ListButton is similar to what appears in Horizon's
     * lists. It has a string and should be used to open something else.
     */
    class ListButton : public Element {
        private:
            /** @brief Pointer to top rectangle */
            Rectangle * topR;
            /** @brief Pointer to bottom rectangle */
            Rectangle * bottomR;
            /** @brief Pointer to text */
            Text * text_;

        public:
            /**
             * @brief Construct a new List Button object
             *
             * @param s list button text
             * @param f function to invoke on button press
             */
            ListButton(std::string s, std::function<void()> f);

            /**
             * @brief Get the line colour
             *
             * @return line colour
             */
            Colour getLineColour();

            /**
             * @brief Set the line colour
             *
             * @param c new line colour
             */
            void setLineColour(Colour c);

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

            /**
             * @brief Set all colours at once: line, text
             *
             * @param l new line colour
             * @param t new text colour
             */
            void setColours(Colour l, Colour t);

            /**
             * @brief Get the list text
             *
             * @return list text
             */
            std::string text();

            /**
             * @brief Set the list text
             *
             * @param s new list text
             */
            void setText(std::string s);

            /**
             * @brief Get the font size
             *
             * @return font size
             */
            unsigned int fontSize();

            /**
             * @brief Set the font size
             *
             * @param f new font size
             */
            void setFontSize(unsigned int f);

            /**
             * @brief Set the width of the list button
             * Adjusting width also changes the width of rectangles.
             *
             * @param w new width
             */
            void setW(int w);

            /**
             * @brief Set the height of the list button
             * Adjusting height also moves same elements.
             *
             * @param h new height
             */
            void setH(int h);
    };
};

#endif