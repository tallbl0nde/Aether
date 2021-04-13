#ifndef AETHER_LISTOPTION_HPP
#define AETHER_LISTOPTION_HPP

#include "Aether/primary/Rectangle.hpp"
#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief A ListOption is similar to what appears in Horizon's
     * lists. It has a string and value, as well as a function
     * which is called when selected.
     */
    class ListOption : public Element {
        private:
            /** @brief Pointer to top rectangle */
            Rectangle * topR;
            /** @brief Pointer to bottom rectangle */
            Rectangle * bottomR;
            /** @brief Pointer to hint text */
            Text * hint_;
            /** @brief Pointer to value text */
            Text * value_;

            /**
             * @brief Repositions all elements.
             */
            void positionElements();

        public:
            /**
             * @brief Construct a new List Option object
             *
             * @param t hint string
             * @param v value string
             * @param f function to invoke when option is selected
             */
            ListOption(std::string t, std::string v, std::function<void()> f);

            /**
             * @brief Get the line colour (top rectangle colour)
             *
             * @return line colour
             */
            Colour getLineColour();

            /**
             * @brief Set the line colour (top rectangle colour)
             *
             * @param c new line colour
             */
            void setLineColour(Colour c);

            /**
             * @brief Get the hint colour
             *
             * @return hint colour
             */
            Colour getHintColour();

            /**
             * @brief Set the hint colour
             *
             * @param c new hint colour
             */
            void setHintColour(Colour c);

            /**
             * @brief Get the value colour
             *
             * @return value colour
             */
            Colour getValueColour();

            /**
             * @brief Set the value colour
             *
             * @param c new value colour
             */
            void setValueColour(Colour c);

            /**
             * @brief Set all colours at once: line, hint, value
             *
             * @param l new line colour
             * @param h new help colour
             * @param v new value colour
             */
            void setColours(Colour l, Colour h, Colour v);

            /**
             * @brief Get the hint string
             *
             * @return hint string
             */
            std::string hint();

            /**
             * @brief Set the hint string
             *
             * @param s new hint string
             */
            void setHint(std::string s);

            /**
             * @brief Get the value string
             *
             * @return value string
             */
            std::string value();

            /**
             * @brief Set the value string
             *
             * @param s new value string
             */
            void setValue(std::string s);

            /**
             * @brief Set the font size.
             * Also repositions centers.

             * @param s new font size
             */
            void setFontSize(unsigned int s);

            /**
             * @brief Get the font size
             *
             * @return font size
             */
            unsigned int fontSize();

            /**
             * @brief Set the width of the list option
             * All elements are repositioned when this is called.
             *
             * @param w new width
             */
            void setW(int w);

            /**
             * @brief Set the height of the list option
             * All elements are repositioned when this is called.
             *
             * @param h new height
             */
            void setH(int h);
    };
};

#endif