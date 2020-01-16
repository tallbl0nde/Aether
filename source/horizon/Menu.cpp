#include "Menu.hpp"

namespace Aether {
    Menu::Menu() : Scrollable() {
        this->setShowScrollBar(false);
        this->setCatchup(15);
    }
};