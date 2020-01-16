#ifndef AETHER_MENU_HPP
#define AETHER_MENU_HPP

#include "base/Scrollable.hpp"

namespace Aether {
    // A menu is simply a scrollable object that sets some values
    // on creation in order to appear like Horizon's menu.
    class Menu : public Scrollable {
        public:
            // Hides scrollbar + adjusts scroll 'catchup'
            Menu();
    };
};

#endif