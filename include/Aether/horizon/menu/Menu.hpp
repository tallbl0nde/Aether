#ifndef AETHER_MENU_HPP
#define AETHER_MENU_HPP

#include "Aether/base/Scrollable.hpp"
#include "Aether/horizon/menu/MenuOption.hpp"

namespace Aether {
    /**
     * @brief A menu is simply a scrollable object that sets some values
     * on creation in order to appear like Horizon's menu.
     */
    class Menu : public Scrollable {
        private:
            /** @brief Pointer to current active menu option */
            MenuOption * active;

        public:
            /**
             * @brief Construct a new Menu object.
             * Hides scrollbar + adjusts scroll 'catchup'
             *
             * @param x x-coordinate of the start position offset
             * @param y y-coordinate of the start position offset
             * @param w width of menu
             * @param h height of menu
             */
            Menu(int x, int y, int w, int h);

            /**
             * @brief Updates menu.
             * Keeps the selected item centred.
             *
             * @param dt change in time
             */
            void update(unsigned int dt);

            /**
             * @brief Set the given option as the highlighted option.
             * Unsets previously highlighted
             *
             * @param o option to set as active
             */
            void setActiveOption(MenuOption * o);
    };
};

#endif