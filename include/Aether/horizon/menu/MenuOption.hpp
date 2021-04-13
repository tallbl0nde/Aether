#ifndef AETHER_MENUOPTION_HPP
#define AETHER_MENUOPTION_HPP

#include "Aether/base/Element.hpp"
#include "Aether/primary/Rectangle.hpp"
#include "Aether/primary/Text.hpp"

namespace Aether {
    /**
     * @brief A MenuOption is a combination of elements which looks like and functions
     * similar to Horizon's. All children are handled internally.
     */
    class MenuOption : public Element {
        private:
            /** @brief Indicator on whether to draw active style */
            bool active;
            /** @brief Colour when active */
            Colour activeColour;
            /** @brief Colour when inactive */
            Colour inactiveColour;
            /** @brief Pointer to menu rectangle */
            Rectangle * rect;
            /** @brief Pointer to menu text */
            Text * text;

        public:
            /**
             * @brief Construct a new Menu Option object
             *
             * @param s menu text
             * @param a active colour
             * @param ia inactive Colour
             * @param f function to invoke when button pressed
             */
            MenuOption(std::string s, Colour a, Colour ia, std::function<void()> f);

            /**
             * @brief Updates width for menu option
             * Setting new width also adjusts the width of text texture if necessary.
             * @param w new width
             */
            void setW(int w);

            /**
             * @brief Set the active status of option
             *
             * @param b true if option is selected, false otherwise
             */
            void setActive(bool b);

            /**
             * @brief Set the active colour for option
             *
             * @param c new active colour
             */
            void setActiveColour(Colour c);

            /**
             * @brief Set the inactive colour for option
             *
             * @param c new inactive colour
             */
            void setInactiveColour(Colour c);
    };
};

#endif